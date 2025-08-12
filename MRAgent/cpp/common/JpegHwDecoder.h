#ifndef JPEG_HW_DECODER_H
#define JPEG_HW_DECODER_H

#include <cstdint>
#include <atomic>
#include <vector>

#ifdef ANDROID
extern "C" {
#include "rk_mpi.h"
#include "mpp_mem.h"
#include "mpp_env.h"
#include "mpp_time.h"
#include "mpp_common.h"
#include "mpi_dec_utils.h"
}
#include <turbojpeg.h>
#endif

namespace imimr {

class JpegHwDecoder {
private:
    std::atomic_bool m_initialized{false};
    int m_mode{0}; // mode: 0 480P, 1 1080P, 2 2k
    uint32_t m_width{0};
    uint32_t m_height{0};
    void *m_input_data{nullptr};
    int m_input_data_size{0};
    void *m_output_data{nullptr};
    int m_output_data_size{0};
    std::vector<uint8_t> m_input_buffer;
    std::vector<uint8_t> m_output_buffer;

#ifdef ANDROID
    size_t m_frame_size{0};
    int m_ion_fd{-1};
    int m_dma_fd{-1};
    void *m_virt_addr{nullptr};
    MppCtx m_ctx{nullptr};
    MppApi *m_mpi{nullptr};
    MppBufferGroup m_frm_grp{nullptr};
    MppFrame m_frame{nullptr};
    MppBuffer m_frm_buf{nullptr};
    MppDecCfg m_cfg{nullptr};
    MppBufferGroup m_group{nullptr};
    MppBuffer m_buf{nullptr};

    tjhandle tjInstance{nullptr};

    bool m_hw_decode{true};
#endif
public:
    virtual ~JpegHwDecoder() { DeInit(); }
    
    int64_t Init(int mode); // mode: 0 480P, 1 1080P, 2 2k
    int64_t DeInit();
    int64_t Decode(int width, int height);
    void *GetInputData() const { return m_input_data; }
    int GetMaxInputDataSize() const { return m_width * m_height * 3; }
    void SetInputDataSize(int size) { m_input_data_size = size; }
    void *GetOutputData() const { return m_output_data; }
    int GetOutputDataSize() const { return m_output_data_size; }
    int GetMaxOutputDataSize() const { return m_width * m_height * 3; }
    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }
    bool Initialized() const { return m_initialized; }
    
};

} // namespace imimr

#endif // JPEG_HW_DECODER_H
