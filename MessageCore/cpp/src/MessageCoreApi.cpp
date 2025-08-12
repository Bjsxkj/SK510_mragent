#include "MessageCoreApi.h"

#include "ImiMrLog.h"
#include "ImiUserTrackSensor.h"

#include "DoubleBuffer.h"
#include "MessageHeader.h"
#include "utils.h"

#include "ImiVersion.h"
#include "cpptoml.h"
#include "zmq.hpp"
#include <atomic>
#include <fstream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

struct UserTrackFrame
{
        uint64_t timeStamp;
        uint32_t frameNum;
};

#include <chrono>
void LOGD_TS(const char *tag, int frameNumber, int dataType)
{
#ifdef ANDROID
    auto timeStamp =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    __android_log_print(ANDROID_LOG_INFO, "ImiLog", "[%s] %d,%d,%ld", tag, frameNumber, dataType, timeStamp);
#endif
}

using namespace imimr;
#define DEBUG_TS

static const char *LOCAL_IP = "127.0.0.1";
#define MEG_HEADER_SIZE sizeof(MessageHeader)

namespace mrcore
{

    std::shared_ptr<cpptoml::table> gMrCoreConfig;

    std::string getVersion()
    {
        return std::to_string(IMI_MESSAGECORE_MAJOR_VERSION) + "." + std::to_string(IMI_MESSAGECORE_MINOR_VERSION) +
               "." + std::to_string(IMI_MESSAGECORE_REVISION_VERSION);
    }

    MessageCore::MessageCore() {}

    MessageCore::~MessageCore() {}

    bool MessageCore::initialize(const char *configFilePath)
    {
        LOGD("MessageCore initialized started");
        if (configFilePath == nullptr)
        {
            onError(ErrorCode::ERR, "configuration file is required");
            return false;
        }

        std::ifstream file(configFilePath);
        if (!file.good())
        {
            onError(ErrorCode::ERR, "configuration file does not exist");
            return false;
        }

        auto cfg = cpptoml::parse_file(configFilePath);
        if (cfg == nullptr)
        {
            onError(ErrorCode::ERR, "parse configuration file failed");
            return false;
        }

        gMrCoreConfig = cfg;

        LOGD("MessageCore initialized");

        bindSocket();
        if (!setup_shared_memory())
            return false;

        colorDataBuffer_size = MEG_HEADER_SIZE + 1280 * 720 * 3; // 彩色数据缓冲区大小
        if (colorDataBuffer == nullptr)
        {
            colorDataBuffer = new uint8_t[colorDataBuffer_size];
            LOGI("MessageCore initialized, colorDataBuffer_size=%d", colorDataBuffer_size);
        }

        return true;
    }

    void MessageCore::bindSocket()
    {
        LOGD("bindSocket");
        auto config = gMrCoreConfig;
        auto dataPortsCfg = config->get_table("server");
        // 创建共享的 context

        context = zmq::context_t(1); // 单例 context
        int port = *dataPortsCfg->get_as<int>("usertrack_data_port");
        usertrack_socket = new zmq::socket_t(context, zmq::socket_type::pub);
        usertrack_socket->setsockopt(ZMQ_LINGER, 0);
        try
        {
            // 使用 stringstream 生成地址
            std::stringstream ss;
            ss << "tcp://*:" << port;
            std::string usertrack_address = ss.str();

            LOGI("Binding usertrack to: %s", usertrack_address.c_str());
            usertrack_socket->bind(usertrack_address);
            LOGI("usertrack bind success");
        }
        catch (const zmq::error_t &ex)
        {
            LOGE("Failed to bind usertrack: %s", ex.what());
        }
        // 创建第二个 socket (color)
        port = *dataPortsCfg->get_as<int>("color_data_port");
        color_socket = new zmq::socket_t(context, zmq::socket_type::pub);

        try
        {
            // 重新初始化 stringstream
            std::stringstream ss;
            ss << "tcp://*:" << port;
            std::string color_address = ss.str();

            LOGI("Binding color to: %s", color_address.c_str());
            color_socket->bind(color_address);
            LOGI("color bind success");
        }
        catch (const zmq::error_t &ex)
        {
            LOGE("Failed to bind color: %s", ex.what());
        }

        bindCmdServerSocket();
    }

    void MessageCore::bindCmdServerSocket()
    {
        cmd_server_socket = new zmq::socket_t(context, zmq::socket_type::router);
        // 设置路由ID
        cmd_server_socket->setsockopt(ZMQ_IDENTITY, "Router", 6);
        cmd_server_socket->setsockopt(ZMQ_LINGER, 0);
        // 设置接收超时，避免阻塞
        int timeout = 1000;
        cmd_server_socket->setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
        cmd_server_socket->bind("tcp://*:5600");
        LOGI("cmd_server_socket bind success");
    }

    void MessageCore::heartRecv()
    {
        LOGI("isCoreStarted = %d", isCoreStarted);
        int index = 0;
        while (isCoreStarted)
        {
            if (!cmd_server_socket)
            {
                LOGE("cmd_server_socket is null");
                break;
            }

            // 接收客户端标识
            zmq::message_t identity;
            if (!cmd_server_socket->recv(identity, zmq::recv_flags::dontwait))
            {
                if (true)
                {
                    // 超时，没有消息
                    index++;

                    if (index == 60)
                    {
                        LOGI("60s 未收到 ack , client color sensor 关闭");
                        onColorSensorClose();
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }

            // 接收消息内容
            zmq::message_t request;
            cmd_server_socket->recv(request, zmq::recv_flags::none);

            if (request.size() == 0)
            {
                LOGI("收到空内容消息");
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            }

            index = 0;

            parseCmd(identity, request);

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    void MessageCore::parseCmd(zmq::message_t &identity, zmq::message_t &request)
    {
        std::string client_id(static_cast<char *>(identity.data()), identity.size());
        // 直接转换（需确保消息大小与结构体大小一致）
        LOGI("request size = %d", request.size());
        if (request.size() == sizeof(cmd_data_native))
        {
            const cmd_data_native *cmd = static_cast<const cmd_data_native *>(request.data());
            auto cmdType = cmd->cmdtype;
            LOGI("cmdtype , %d ", cmdType);
            if (cmdType == CMD_DEVICE_CLOSE)
            {
                bool isAdded = isClientAdded(client_id);
                if (isAdded)
                {
                    LOGI("remove client , %s", client_id.c_str());
                    clientList.remove(client_id);
                }
            }
            else if (cmdType == CMD_DEVICE_OPEN)
            {
                bool isAdded = isClientAdded(client_id);
                if (!isAdded)
                {
                    LOGI("add client , %s", client_id.c_str());
                    clientList.push_back(client_id);
                }
            }
            else if (cmdType == CMD_COLOR_ACK)
            { // color ack
                LOGI("color ack");
            }
            else
            {
                // 通过回调处理命令
                cmd_data cmd_str;
                memcpy(&cmd_str, cmd, sizeof(cmd_data_native) - sizeof(cmd->cmdData));
                cmd_str._data.len = 0;
                cmd_str.cmdData.assign(cmd->cmdData);
                if (cmd->cmdDataSize == 1) // 配准 或者 mouse gesture smooth enable
                {
                    if (cmd->cmdData[0] == 0x01)
                    {
                        cmd_str.cmdData = "open";
                    }
                    else if (cmd->cmdData[0] == 0x00)
                    {
                        cmd_str.cmdData = "close";
                    }
                    else
                    {
                        cmd_str.cmdData = "error";
                    }
                }
                bool ret = onCommandHandler(&cmd_str);

                LOGI("extraInfo , %s ", cmd_str.extraInfo.c_str());
                if (cmd_str.extraInfo != "")
                {
                    sendHeartbeatMsg(client_id, cmd_str.extraInfo);
                }
                else if (cmd_str._data.len <= 0)
                {
                }
                else
                {
                    const char *response = (const char *)cmd_str._data.data;
                    sendHeartbeatMsg(client_id, response);
                }
            }
        }
    }

    bool MessageCore::isClientAdded(std::string &client_id)
    {
        auto it = std::find(clientList.begin(), clientList.end(), client_id);
        return it != clientList.end();
    }

    void MessageCore::sendHeartbeatMsg(std::string client_id, std::string message)
    {
        LOGI("client_id = %s , message = %s", client_id.c_str(), message.c_str());
        zmq::message_t identity(client_id.size());
        memcpy(identity.data(), client_id.data(), client_id.size());

        // 创建并发送消息
        zmq::message_t zmq_msg(message.size());
        memcpy(zmq_msg.data(), message.data(), message.size());

        // 发送消息（使用 ZMQ_NOBLOCK 可以非阻塞发送）
        try
        {
            zmq::message_t empty;

            // 发送响应（格式：客户端ID + 空帧 + 响应内容）
            cmd_server_socket->send(identity, zmq::send_flags::sndmore);
            cmd_server_socket->send(empty, zmq::send_flags::sndmore);
            cmd_server_socket->send(zmq_msg, zmq::send_flags::none);
            std::cout << "已发布: " << message << std::endl;
            LOGI("已发布消息: %s", message.c_str()); // 记录成功信息
        }
        catch (const zmq::error_t &e)
        {
            //            std::cerr << "发送消息时出错: " << e.what() << std::endl;
            LOGE("发送消息失败: %s (错误码: %d)", e.what(), e.num());
        }
    }

    void MessageCore::release()
    {
        if (usertrack_socket)
        {
            usertrack_socket->close();
            delete usertrack_socket;
            usertrack_socket = nullptr;
        }
        if (color_socket)
        {
            color_socket->close();
            delete color_socket;
            color_socket = nullptr;
        }

        if (colorDataBuffer)
        {
            delete[] colorDataBuffer;
            colorDataBuffer = nullptr;
            colorDataBuffer_size = 0;
        }

        if (cmd_server_socket)
        {
            // 设置LINGER为0，强制丢弃未发送的消息
            int linger = 0;
            cmd_server_socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));

            cmd_server_socket->close();
            delete cmd_server_socket;
            cmd_server_socket = nullptr;
        }

        if (context)
        {
            context.close();
        }

        ColorPool.release();     // 只需这一行
        SkeletonPool.release();  // 只需这一行
        ForegroudPool.release(); // 只需这一行

        LOGD("MessageCore released");
    }

    bool MessageCore::isStarted()
    {
        return isCoreStarted;
    }

    bool MessageCore::start(int startFlags)
    {
        printf("MessageCore::start() startFlags=%d\n", startFlags);
        LOGI("MessageCore::start begein");
        isCoreStarted = true;
        if (startFlags)
        {
        }
        th_heartbeat = new std::thread(&MessageCore::heartRecv, this);
        LOGI("MessageCore::start()");

        return true;
    }

    void MessageCore::join()
    {
        for (auto it = _allThreads.begin(); it != _allThreads.end(); it++)
        {
            auto p = *it;
            p->join();
            delete p;
        }
        _allThreads.clear();
    }

    void MessageCore::stop()
    {
        isCoreStarted = false;
        LOGD("MessageCore  stop()");

        LOGI("client size = %d", clientList.size());
        for (const auto &client_id : clientList)
        {
            LOGI("send heartbeat exit");
            sendHeartbeatMsg(client_id, "heartbeat exit");
        }

        if (th_heartbeat)
        {
            if (th_heartbeat->joinable())
            {
                th_heartbeat->join();
            }
            delete th_heartbeat;
            th_heartbeat = nullptr;
        }

        join();
        LOGI("MessageCore::stop() 999");

        release();
        LOGI("MessageCore::stop()    1010\n");
    }

    void MessageCore::pause() {}

    void MessageCore::resume() {}

    void MessageCore::onError(int errCode, const char *err)
    {
    }

    void MessageCore::onLog(int level, const char *msg)
    {
        static const char *L[] = {"D", "I", "E", "X"};
        printf("[%s] %s\n", L[level & 3], msg);
    }

    void MessageCore::sendMessage(void *socket, const char *msg, int len, int frameNum)
    {
        auto p = (MessageHeader *)msg;
        p->majorVersion = 1;
        p->minorVersion = 3;
        p->frameNum = frameNum;
        p->timeStamp = getCurrentTimeInMs();

        ((zmq::socket_t *)socket)->send(zmq::message_t(msg, len), zmq::send_flags::none);
    }

    int MessageCore::syncColorData(unsigned char *array_ref, int array_len)
    {

        return 0;
    }

    int MessageCore::syncSkeletonData(unsigned char *array_ref, int array_len)
    {

        return 0;
    }

    int MessageCore::syncForegroundData(unsigned char *array_ref, int array_len)
    {

        return 0;
    }

    int MessageCore::syncActionsData(std::string data)
    {

        return 0;
    }

    struct UserTrackFrame
    {
            uint64_t timeStamp;
            uint32_t frameNum;
    };

    int MessageCore::publishColorFrame(unsigned char *array_ref, int array_len, int w, int h, int frameNum)
    {
        if (!isCoreStarted || array_ref == nullptr)
        {
            LOGE("isCoreStarted is false or array_ref is null");
            return -1;
        }

        if (frameNum % 100 == 1)
        {
            auto now = getCurrentTimeInMs();
            LOGD("Color_rate#L2 frameNum =%d,  sdk_timeStamp=%lld", frameNum, now);
        }

        if (address.find(LOCAL_IP) != std::string::npos)
        {
            static int count = 0;
            static uint64_t recored_time = 0;
            if (count == 0)
            {
                recored_time = getCurrentTimeInMs();
            }
            MessageHeader header{0};
            header.frameNum = frameNum;
            ColorPool.writeImg(&header, MEG_HEADER_SIZE, (uint8_t *)(array_ref), array_len, w, h);
            if (++count >= 500)
            {
                float diff = getCurrentTimeInMs() - recored_time;
                float fps = 500.0 * 1000.0 / diff;
                count = 0;
                LOGI("MessageCore::publishColorFrame size=%d, frameNum=%d, fps=%f w=%d h=%d",
                     MEG_HEADER_SIZE + array_len, header.frameNum, fps, w, h);
            }
        }
        else
        {
            static int count = 0;
            static int preFrameNum = 0;
            uint64_t static recored_time = 0;
            if (count == 0)
            {
                recored_time = getCurrentTimeInMs();
            }
            std::vector<uint8_t> colorDataBuffer;
            colorDataBuffer.resize(MEG_HEADER_SIZE + array_len);
            memset(&colorDataBuffer[0], 0, MEG_HEADER_SIZE);
            memcpy(&colorDataBuffer[0] + MEG_HEADER_SIZE, array_ref, array_len);
            sendMessage((void *)color_socket, (const char *)&colorDataBuffer[0], MEG_HEADER_SIZE + array_len, frameNum);
            LOGI("MessageCore::publishColorFrame size=%d, frameNum=%d, fps=%f w=%d h=%d", MEG_HEADER_SIZE + array_len,
                 frameNum, -1, w, h);
            if (count++ >= 499)
            {
                float diff = getCurrentTimeInMs() - recored_time;
                float fps;
                fps = 500.0 * 1000.0 / diff;
                count = 0;
                MessageHeader *pHeader = (MessageHeader *)&colorDataBuffer[0];
                LOGI("MessageCore::publishColorFrame bytesPublish=%d, num=%d, fps=%f w=%d h=%d",
                     MEG_HEADER_SIZE + array_len, pHeader->frameNum, fps, w, h);
            }
        }

        if (frameNum % 100 == 1)
        {
            auto now = getCurrentTimeInMs();
            LOGD("Color_rate#L3 frameNum =%d,  sdk_timeStamp=%lld", frameNum, now);
        }
        return 0;
    }

    int MessageCore::publishForegroundData(unsigned char *array_ref, int array_len, int w, int h, int frameNum)
    {
        if (!isCoreStarted || array_ref == 0)
        {
            return -1;
        }

        if (address.find(LOCAL_IP) != std::string::npos)
        {
            MessageHeader header{0};
            header.frameNum = frameNum;

            ForegroudPool.writeImg(&header, sizeof(header), array_ref, w * h * 2, w, h);
        }

        else // zmq
        {
            int len = w * h * 2;

            uint8_t foregroundDataBuffer[MEG_HEADER_SIZE + 640 * 480 * 2] = {0};

            memcpy((uint8_t *)((uint8_t *)&foregroundDataBuffer) + MEG_HEADER_SIZE, array_ref, len);

            //  topic
            std::string topic = "foreground_data";
            zmq::message_t topic_msg(topic.c_str(), topic.size());
            usertrack_socket->send(topic_msg, zmq::send_flags::sndmore);
            sendMessage((void *)usertrack_socket, (const char *)&foregroundDataBuffer, MEG_HEADER_SIZE + w * h * 2,
                        frameNum);
        }
        static int count = 0;
        count++;
        if (count == 2000)
        {
            LOGI("publish foreground data %d times, frameNum=%d", count, frameNum);
            count = 0;
        }
        return 0;
    }

    int MessageCore::publishBodySkeletonData(const imimr::ImiBodySkeleton &skeletons, int frameNum)
    {
        if (!isCoreStarted)
        {
            return -1;
        }
        if (frameNum % 100 == 1)
        {
            auto now = getCurrentTimeInMs();
            LOGD("Statistical_rate#T2 frameNum =%d,  sdk_timeStamp=%lld", frameNum, now);
        }

#ifndef MOCK_TEST

        if (address.find(LOCAL_IP) != std::string::npos)
        {
            MessageHeader header{0};
            header.frameNum = frameNum;

            uint8_t skeleton_msg[sizeof(imimr::ImiBodySkeleton)] = {0};

            imimr::ImiBodySkeleton *p = (imimr::ImiBodySkeleton *)(((uint8_t *)&skeleton_msg));

            p->width = skeletons.width;   // dt.width;
            p->height = skeletons.height; // dt.height;
            memcpy(&p->floorClipPlane, &skeletons.floorClipPlane, sizeof(p->floorClipPlane));
            memcpy(p->skeletonDatas, skeletons.skeletonDatas, sizeof(p->skeletonDatas));

            SkeletonPool.writeImg(&header, sizeof(header), (const char *)p, sizeof(imimr::ImiBodySkeleton), p->width,
                                  p->height);
        }

        else // zmq
        {
            uint8_t skeleton_msg[MEG_HEADER_SIZE + sizeof(imimr::ImiBodySkeleton)] = {0};

            imimr::ImiBodySkeleton *p = (imimr::ImiBodySkeleton *)(((uint8_t *)&skeleton_msg) + MEG_HEADER_SIZE);

            p->width = skeletons.width;   // dt.width;
            p->height = skeletons.height; // dt.height;
            memcpy(&p->floorClipPlane, &skeletons.floorClipPlane, sizeof(p->floorClipPlane));
            memcpy(p->skeletonDatas, skeletons.skeletonDatas, sizeof(p->skeletonDatas));
#else
        uint8_t skeleton_msg[MEG_HEADER_SIZE + sizeof(imimr::ImiBodySkeleton)] = {0};
#endif
            std::string topic = "skeleton_data";
            zmq::message_t topic_msg(topic.c_str(), topic.size());

            stringstream ss;
            ss << skeletons;

            usertrack_socket->send(topic_msg, zmq::send_flags::sndmore);
            sendMessage((void *)usertrack_socket, (const char *)&skeleton_msg,
                        MEG_HEADER_SIZE + sizeof(imimr::ImiBodySkeleton), frameNum);
        }
        static int count = 0;
        count++;
        if (count == 2000)
        {
            LOGI("publish skeleton data %d times, frameNum=%d", count, frameNum);
            count = 0;
        }
        if (frameNum % 100 == 1)
        {
            auto now = getCurrentTimeInMs();
            LOGD("Statistical_rate#T3 frameNum =%d,  sdk_timeStamp=%lld", frameNum, now);
        }
        return 0;
    }

    bool MessageCore::setup_shared_memory()
    {
        LOGI("MessageCore::setup_shared_memory");
        auto dataPortsCfg = gMrCoreConfig->get_table("server");
        address = *dataPortsCfg->get_as<std::string>("address");
        if (address.find(LOCAL_IP) == std::string::npos)
        {
            LOGI("MessageCore::setup_shared_memory skip shared memory setup");
            return true;
        }
        LOGI("MessageCore::setup_shared_memory running on localhost");
        if (!ColorPool.create("COLOR"))
        {
            LOGE("setup_shared_memory COLOR shared memory create failed");
            return false;
        }
        if (!SkeletonPool.create("SKELETON"))
        {
            LOGE("setup_shared_memory SKELETON shared memory create failed");
            return false;
        }
        if (!ForegroudPool.create("FOREGROUND"))
        {
            LOGE("setup_shared_memory FOREGROUND shared memory create failed");
            return false;
        }
        return true;
    }
} // namespace mrcore
