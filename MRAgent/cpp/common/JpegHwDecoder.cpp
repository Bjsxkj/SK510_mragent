#include "JpegHwDecoder.h"

#include <cstring>
#include <memory>

#ifdef ANDROID
#include <sys/mman.h>
#include <sys/system_properties.h>
#endif
#include <chrono>

extern "C" {
int ion_open();
int ion_alloc_fd(int fd, size_t len, size_t align, unsigned int heap_mask, unsigned int flags, int *handle_fd);
}

#define ION_CMA_HEAP_ID (1 << 1) // CMA 堆[3](@ref)

namespace imimr {
constexpr uint32_t IMG_MAX_WIDTH = 2688;
constexpr uint32_t IMG_MAX_HEIGHT = 1520;
constexpr uint32_t BUFFER_MAX_DATA_SIZE = IMG_MAX_WIDTH * IMG_MAX_HEIGHT * 3;
int64_t JpegHwDecoder::Init(int mode) {
#ifdef ANDROID
    if (m_initialized.load(std::memory_order_acquire)) { DeInit(); }
    m_mode = mode; // 保存模式
    char agent_jpeg_hw_decode_mode[PROP_VALUE_MAX + 1];
    int ret = __system_property_get("agent.jpeg_hw_decode_mode", agent_jpeg_hw_decode_mode);
    if (ret > 0 && atoi(agent_jpeg_hw_decode_mode) == 0) {
        mpp_log("JpegHwDecoder::Init use software decode");
        m_hw_decode = false;
    } else {
        mpp_log("JpegHwDecoder::Init use hardware decode");
        m_hw_decode = true;
    }
    if (m_mode == 0) {
        m_width = 640;
        m_height = 480;
    } else if (m_mode == 1) {
        m_width = 1920;
        m_height = 1080;
    } else {
        m_width = 2688;
        m_height = 1520;
    }
    mpp_log("JpegHwDecoder::Init mode %d, width %d, height %d", m_mode, m_width, m_height);
    if (m_hw_decode) {
        // 准备
        MPP_RET ret = MPP_OK;
        // 初始化MPP解码器
        ret = mpp_create(&m_ctx, &m_mpi);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init mpp_create failed");
            return -1;
        }
        ret = mpp_init(m_ctx, MPP_CTX_DEC, MPP_VIDEO_CodingMJPEG);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init mpp_init failed");
            return -1;
        }
        // 创建ION内存并映射
        RK_U32 hor_stride = MPP_ALIGN(IMG_MAX_WIDTH, 64);
        RK_U32 ver_stride = MPP_ALIGN(IMG_MAX_HEIGHT, 64);
        mpp_log("hor_stride %d, ver_stride %d\n", hor_stride, ver_stride);
        m_frame_size = hor_stride * ver_stride * 3; // RGB888格式需要3倍的空间
        m_ion_fd = ion_open();
        if (m_ion_fd <= 0) {
            mpp_err("JpegHwDecoder::Init ion_open failed");
            return -1;
        }
        ion_alloc_fd(m_ion_fd, m_frame_size, 4096, ION_CMA_HEAP_ID, 0x1, &m_dma_fd);
        if (m_dma_fd <= 0) {
            mpp_err("JpegHwDecoder::Init ion_alloc_fd failed");
            return -1;
        }
        // 映射到用户空间
        m_virt_addr = mmap(nullptr, m_frame_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_dma_fd, 0);
        if (m_virt_addr == nullptr) {
            mpp_err("JpegHwDecoder::Init mmap failed");
            return -1;
        }
        // 配置MPP外部内存组
        mpp_buffer_group_get_external(&m_frm_grp, MPP_BUFFER_TYPE_ION);
        MppBufferInfo commit = {.fd = m_dma_fd, .ptr = m_virt_addr, .size = m_frame_size, .type = MPP_BUFFER_TYPE_ION};
        mpp_buffer_commit(m_frm_grp, &commit); // 提交内存[1,2](@ref)
        m_mpi->control(m_ctx, MPP_DEC_SET_EXT_BUF_GROUP, m_frm_grp); // 绑定解码器
        ret = mpp_frame_init(&m_frame);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init mpp_frame_init failed");
            return -1;
        }
        ret = mpp_buffer_get(m_frm_grp, &m_frm_buf, m_frame_size);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init failed to get buffer for input frame ret %d\n", ret);
            return -1;
        }
        mpp_frame_set_buffer(m_frame, m_frm_buf);
        mpp_dec_cfg_init(&m_cfg);
        if (m_cfg == nullptr) {
            mpp_err("JpegHwDecoder::Init mpp_dec_cfg_init failed");
            return -1;
        }
        ret = m_mpi->control(m_ctx, MPP_DEC_GET_CFG, m_cfg);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init get decoder cfg failed");
            return -1;
        }
        RK_U32 need_split = 1;
        ret = mpp_dec_cfg_set_u32(m_cfg, "base:split_parse", need_split);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init set base:split_parse failed");
            return -1;
        }
        ret = m_mpi->control(m_ctx, MPP_DEC_SET_CFG, m_cfg);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init set decoder cfg failed");
            return -1;
        }
        RK_U32 format = MPP_FMT_RGB888;
        ret = m_mpi->control(m_ctx, MPP_DEC_SET_OUTPUT_FORMAT, &format);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Init set output format failed");
            return -1;
        }
        mpp_buffer_group_get_internal(&m_group, MPP_BUFFER_TYPE_ION);
        if (m_group == nullptr) {
            mpp_err("JpegHwDecoder::Init mpp_buffer_group_get_internal failed");
            return -1;
        }
        mpp_buffer_get(m_group, &m_buf, BUFFER_MAX_DATA_SIZE);
        if (m_buf == nullptr) {
            mpp_err("JpegHwDecoder::Init mpp_buffer_get failed");
            return -1;
        }
        m_input_data = mpp_buffer_get_ptr(m_buf);
        if (m_input_data == nullptr) {
            mpp_err("JpegHwDecoder::Init mpp_buffer_get_ptr failed");
            return -1;
        }
        mpp_log("JpegHwDecoder::Init m_input_data %p size %d", m_input_data, BUFFER_MAX_DATA_SIZE);
    } else {
        tjInstance = tjInitDecompress();
        if (tjInstance == nullptr) {
            mpp_err("JpegHwDecoder::Init tjInitDecompress failed");
            return -1;
        }
        m_input_buffer.resize(BUFFER_MAX_DATA_SIZE);
        m_input_data = &m_input_buffer[0];
        m_output_buffer.resize(BUFFER_MAX_DATA_SIZE);
        m_output_data = &m_output_buffer[0];
    }
    m_initialized.store(true, std::memory_order_release);
    mpp_log("JpegHwDecoder::Init finish");
    return 0;
#else
    return -1;
#endif
}

int64_t JpegHwDecoder::DeInit() {
#ifdef ANDROID
    if (m_initialized.load(std::memory_order_acquire) == false) { return 0; }
    mpp_log("JpegHwDecoder::DeInit start");
    m_input_data = nullptr;
    m_input_data_size = 0;
    m_output_data = nullptr;
    m_output_data_size = 0;
    if (m_hw_decode) {
        if (m_buf != nullptr) {
            mpp_buffer_put(m_buf);
            m_buf = nullptr;
        }
        if (m_frm_buf != nullptr) {
            mpp_buffer_put(m_frm_buf);
            m_frm_buf = nullptr;
        }
        if (m_frame != nullptr) {
            mpp_frame_deinit(&m_frame);
            m_frame = nullptr;
        }
        if (m_frm_grp != nullptr) {
            mpp_buffer_group_put(m_frm_grp);
            m_frm_grp = nullptr;
        }
        if (m_group != nullptr) {
            mpp_buffer_group_put(m_group);
            m_group = nullptr;
        }
        if (m_ctx != nullptr) {
            mpp_destroy(m_ctx);
            m_ctx = nullptr;
        }
        if (m_virt_addr != nullptr) {
            munmap(m_virt_addr, m_frame_size);
            m_virt_addr = nullptr;
        }
        if (m_dma_fd > 0) {
            close(m_dma_fd);
            m_dma_fd = -1;
        }
        if (m_ion_fd > 0) {
            close(m_ion_fd);
            m_ion_fd = -1;
        }
        m_mode = 0; // 重置模式
        m_width = 0;
        m_height = 0;
    } else {
        if (tjInstance != nullptr) { tjDestroy(tjInstance); }
    }
    m_initialized.store(false, std::memory_order_release);
    mpp_log("JpegHwDecoder::DeInit finish");
    return 0;
#else
    return -1;
#endif
}

int64_t JpegHwDecoder::Decode(int width, int height) {
#ifdef ANDROID
    // std::chrono::time_point<std::chrono::system_clock> entry_time = std::chrono::system_clock::now();
    if (m_initialized.load(std::memory_order_acquire) == false) { return -1; }
    if (width <= 0 || width > IMG_MAX_WIDTH || height <= 0 || height > IMG_MAX_HEIGHT) {
        mpp_err("JpegHwDecoder::Decode invalid width or height %dx%d", width, height);
        return -1;
    }
    if (width != m_width || height != m_height) {
        mpp_err("JpegHwDecoder::Decode width or height mismatch, expected %dx%d, got %dx%d", m_width, m_height, width, height);
        return -1;
    }
    if (m_hw_decode) {
        if (m_input_data == nullptr) {
            mpp_err("JpegHwDecoder::Decode input buffer is null");
            return -1;
        }
        // 创建输入packet
        MppPacket packet = nullptr;
        mpp_packet_init_with_buffer(&packet, m_buf);
        if (packet == nullptr) {
            mpp_err("JpegHwDecoder::Decode mpp_packet_init_with_buffer failed");
            return -1;
        }
        // mpp_packet_set_eos(packet);
        MppMeta meta = mpp_packet_get_meta(packet);
        if (meta != nullptr) { mpp_meta_set_frame(meta, KEY_OUTPUT_FRAME, m_frame); }
        // mpp_log("JpegHwDecoder::Decode packet size %d, packet data %p", mpp_packet_get_length(packet), mpp_packet_get_data(packet));
        // 4. 解码
        MPP_RET ret = MPP_OK;
        ret = m_mpi->decode_put_packet(m_ctx, packet);
        if (ret != MPP_OK) {
            mpp_err("JpegHwDecoder::Decode decode_put_packet failed");
            mpp_packet_deinit(&packet);
            return -1;
        }
        MppFrame frame_ret = nullptr;
        ret = m_mpi->decode_get_frame(m_ctx, &frame_ret);
        if (ret != MPP_OK || frame_ret == nullptr) {
            mpp_err("JpegHwDecoder::Decode decode_get_frame failed");
            mpp_packet_deinit(&packet);
            return -1;
        }
        // mpp_log("JpegHwDecoder::Decode decode_get_frame completed successfully, frame_ret %p", frame_ret);
        // 5. 拷贝RGB数据
        MppBuffer buf_ret = mpp_frame_get_buffer(frame_ret);
        if (buf_ret == nullptr) {
            mpp_err("JpegHwDecoder::Decode mpp_frame_get_buffer failed");
            mpp_packet_deinit(&packet);
            return -1;
        }
        m_output_data = mpp_buffer_get_ptr(buf_ret);
        m_output_data_size = (int)mpp_buffer_get_size(buf_ret);
        // mpp_log("JpegHwDecoder::Decode rgb_ptr %p, m_output_data_size %d", rgb_ptr, m_output_data_size);
        if (m_output_data == nullptr) {
            mpp_err("JpegHwDecoder::Decode mpp_buffer_get_ptr failed");
            mpp_packet_deinit(&packet);
            return -1;
        }
        if (m_output_data_size == 0) {
            mpp_err("JpegHwDecoder::Decode mpp_buffer_get_size failed");
            mpp_packet_deinit(&packet);
            return -1;
        }
        m_output_data_size = m_output_data_size > m_width * m_height * 3 ? m_width * m_height * 3 : m_output_data_size;
        ret = m_mpi->reset(m_ctx);
        if (ret != MPP_OK) { mpp_err("JpegHwDecoder::Decode reset decoder failed"); }
        // mpp_log("JpegHwDecoder::Decode decode jpeg completed successfully");
        mpp_packet_deinit(&packet);
        // static int count = 0;
        // static uint64_t diff = 0;
        // diff += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - entry_time).count();
        // if (count++ >= 499) {
        //     mpp_log("JpegHwDecoder::Decode 500 times duration %lums", diff);
        //     count = 0;
        //     diff = 0;
        // }
        return m_output_data_size;
    } else {
        int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
        memcpy((uint8_t *)m_input_data + 597, (uint8_t *)m_input_data + 600, m_input_data_size - 600);
        if (tjDecompressHeader3(tjInstance, (const unsigned char *)m_input_data, m_input_data_size, &width, &height, &jpegSubsamp, &jpegColorspace) < 0) {
            mpp_err("JpegHwDecoder::Decode tjDecompressHeader3 failed: %s", tjGetErrorStr());
            return -1;
        }
        int pitch = tjPixelSize[TJPF_RGB] * width;
        m_output_data_size = height * pitch;
        if (tjDecompress2(tjInstance, (const unsigned char *)m_input_data, m_input_data_size, (unsigned char *)m_output_data, width, pitch, height, TJPF_RGB, 0) < 0) {
            mpp_err("JpegHwDecoder::Decode tjDecompress2 failed: %s", tjGetErrorStr());
            return -1;
        }
        // static int count = 0;
        // static uint64_t diff = 0;
        // diff += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - entry_time).count();
        // if (count++ >= 499) {
        //     mpp_log("JpegHwDecoder::Decode 500 times duration %lums", diff);
        //     count = 0;
        //     diff = 0;
        // }
        return m_output_data_size;
    }
#else
    return -1;
#endif
}
} // namespace imimr
