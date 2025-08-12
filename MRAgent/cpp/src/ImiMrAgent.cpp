#include "ImiMrAgent.h"
#include "ImiMrLog.h"
#include "ImiVersion.h"

#include <chrono>
#include <fstream>
#include <string>

#include "cpptoml.h"

#include "DoubleBuffer.h"
#include "nlohmann/json.hpp"
#include "utils.h"

#include "ImiMrDebugFile.h"
#include <random>

#include <codecvt>
#include <locale>

#define MEM_ALIGN alignas(4)
#define MEG_HEADER_SIZE sizeof(ImiMessageHeader)

using namespace std;
using json = nlohmann::json;

const int g_IntensityShiftByPlayerR[] = {1, 2, 0, 2, 0, 0, 2, 0};
const int g_IntensityShiftByPlayerG[] = {1, 2, 2, 0, 2, 0, 0, 1};
const int g_IntensityShiftByPlayerB[] = {1, 0, 2, 2, 0, 2, 0, 2};

#define ACTIONS_DATA_SIZE (1024 * 1024)
#define COLOR_FRAME_WIDTH_MAX 1920
#define COLOR_FRAME_HIGHT_MAX 1080

#pragma pack(push)
#pragma pack(1)
// 为保证不同平台的一致性，必须为 24 byte
typedef struct
{
        uint8_t majorVersion;
        uint8_t minorVersion;
        uint16_t frameNum;
        uint64_t timeStamp;
        uint8_t dataType;
        uint8_t reserved[3];

        uint8_t a[8];

} ImiMessageHeader;
#pragma pack(pop)

struct ServerConfig
{
        std::string serverAddress;
        int signalingServerPort;

        int colorDataPort;
        int userTrackDataPort;
        int actionDataPort;
        std::string communicationWay;

        ServerConfig()
        {
            serverAddress = "127.0.0.1";
            signalingServerPort = 5556;
            colorDataPort = 5557;
            userTrackDataPort = 5558;
            actionDataPort = 5559;
        }
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

static const char *LOCAL_IP = "127.0.0.1";

namespace imimr
{
    static ImiContext *imiCtx = 0; //(nullptr);

    static DoubleBuffer *gColorDataBuffer = nullptr;
    static DoubleBuffer *gForegroundDataBuffer = nullptr;
    static DoubleBuffer *gBodySkeletonDataBuffer = nullptr;
    static DoubleBuffer *gActionsDataBuffer = nullptr;

    static int frameNum = 0;
    static int lastFrameNum = 0;

    static ServerConfig gServerConfig;

    std::string getVersion()
    {
        return std::to_string(IMI_MRAGENT_MAJOR_VERSION) + "." + std::to_string(IMI_MRAGENT_MINOR_VERSION) + "." +
               std::to_string(IMI_MRAGNET_REVISION_VERSION);
    }

    // --------------------------------------
    // Implementation of class ImiContext
    // --------------------------------------

    ImiContext::ImiContext(AppContext *appContext) : appContext(appContext) {}

    ImiContext::~ImiContext()
    {
        release();
    }

    ImiContext *ImiContext::create(AppContext *appContext)
    {
        if (!imiCtx)
        {
            imiCtx = (new ImiContext(nullptr));
        }
        return imiCtx;
    }

    ImiContext *ImiContext::getContext()
    {
        return imiCtx;
    }

    int ImiContext::_parseConfigFile(const char *configFilePath)
    {
        if (configFilePath == nullptr)
        {
            return -1;
        }

        std::ifstream file(configFilePath);
        if (!file.good())
        {
            LOGE("configuration file %s does not exist", configFilePath);
            return -2;
        }

        auto config = cpptoml::parse_file(configFilePath);
        if (config == nullptr)
        {
            if (configFilePath == nullptr)
            {
                LOGE("parse configuration file failed");
                return -3;
            }
        }
        auto serverCfg = config->get_table("server");

        gServerConfig.serverAddress = *serverCfg->get_as<std::string>("address");
        gServerConfig.signalingServerPort = *serverCfg->get_as<int>("signaling_port");

        auto dataPortsCfg = config->get_table("data_ports");
        gServerConfig.colorDataPort = *dataPortsCfg->get_as<int>("color");
        gServerConfig.userTrackDataPort = *dataPortsCfg->get_as<int>("usertrack");
        gServerConfig.actionDataPort = *dataPortsCfg->get_as<int>("action");

        LOGI("MrAgent config: %s", gServerConfig.serverAddress.c_str());

        return 0;
    }

    bool ImiContext::initialize(const char *configFilePath)
    {

        _parseConfigFile(configFilePath);

        return true;
    }

    void ImiContext::setRemoteAddress(std::string address)
    {
        gServerConfig.serverAddress = address;
    }

    void ImiContext::release() {}

    ImiBaseDevice ImiContext::createDevice()
    {
        _device = ImiBaseDevice(new IImiBaseDevice());
        return _device;
    }

    IImiBaseDevice::IImiBaseDevice()
    {
        initClientCmdSocket();
        getCommunicationWay();
    }

    IImiBaseDevice::~IImiBaseDevice() {}

    void IImiBaseDevice::initClientCmdSocket() {
        std::stringstream ss;
        ss << "tcp://" << gServerConfig.serverAddress;
        LOGI("serverAddress  %s", ss.str().c_str());

        if (cmd_ctx.handle() != nullptr)
        {
            cmd_client_socket = zmq::socket_t(cmd_ctx, ZMQ_DEALER);

            if (identity == "")
            {
                LOGI("generateIdentity");
                identity = generateIdentity();
            }
            cmd_client_socket.setsockopt(ZMQ_LINGER, 0); // 立即释放资源
            cmd_client_socket.setsockopt(ZMQ_IDENTITY, identity.c_str(), identity.size());
            client_addr = ss.str() + ":5600";
            try
            {
                // 设置接收超时，避免阻塞
                int timeout = 1000;
                cmd_client_socket.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
                cmd_client_socket.setsockopt(ZMQ_SNDTIMEO, 1000); // 1秒发送超时
                cmd_client_socket.connect(client_addr);
                LOGI("cmd_client_socket have connected");

                th_cmd_client = new std::thread(&IImiBaseDevice::recvLoop, this);
            }
            catch (const zmq::error_t &e)
            {
                std::cerr << "ZMQ 错误: " << e.what() << std::endl;
                LOGE("heartbeat socket connect error");
            }
        }
        else
        {
            LOGE("cmd client socket 上下文初始化失败");
        }
    }

    std::string IImiBaseDevice::generateIdentity()
    {
        // 可选：设置客户端标识
        // 随机数引擎（使用梅森旋转算法）
        std::random_device rd;  // 用于获取种子
        std::mt19937 gen(rd()); // 32位梅森旋转算法

        // 定义分布范围（例如：[1, 1000]）
        std::uniform_int_distribution<> dis(1, 1000);
        int num = dis(gen);

        std::string identity = "client-" + std::to_string(num);
        return identity;
    }

    void IImiBaseDevice::getCommunicationWay() {
        std::string way = getStringProperty(10);
        LOGI("communication way = %s" , way.c_str());
        gServerConfig.communicationWay = way;
    }


    MrManagerStatus IImiBaseDevice::getMrManagerStatus()
    {
        return mrManagerStatus;
    }

    void IImiBaseDevice::setOnExceptionHandler(MrAgentExceptionHandler *exceptionHandler)
    {
        this->exceptionHandler = exceptionHandler;
    }

    bool IImiBaseDevice::sendMsg(cmd_data_native &cmd)
    {
        std::lock_guard<std::mutex> lock(send_mutex);

        LOGI("sendMsg cmdType = %d", cmd.type);
        // 检查 socket 是否有效
        if (!cmd_client_socket)
        {
            LOGE("cmd_client_socket is invalid");
            return false;
        }

        if (cmd_client_socket.handle() == nullptr)
        {
            LOGE("cmd_client_socket 未初始化成功");
            return false;
        }

        try
        {
            // DEALER 端发送消息
            zmq::message_t request(sizeof(cmd));
            memcpy(request.data(), &cmd, sizeof(cmd_data_native)); // 直接拷贝结构体数据
            LOGI("cmd client socket send before...");
            if (!cmd_client_socket.connected())
            {
                LOGE("cmd_client_socket not connected");
                return false;
            }
            if (!cmd_client_socket.send(request, zmq::send_flags::none))
            {
                LOGE("cmd_client_socket send cmd failed");
                return false;
            }
            LOGI("send cmd success");
            return true;
        }
        catch (const zmq::error_t &e)
        {
            LOGE("msg send failed");
            std::cerr << "发送消息失败: " << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            LOGE("sendMsg error: %s", e.what());
        }
        return false;
    }

    void IImiBaseDevice::recvLoop()
    {
        int index = 1;
        while (isCmdClientRunning)
        {
            while (isGettingProperty && isCmdClientRunning)
            {
                //                LOGI("get property ing ...");
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            // 接收消息（使用 ZMQ_NOBLOCK 可以非阻塞接收）
            try
            {
                // 检查 socket 是否有效
                if (!cmd_client_socket)
                {
                    LOGE("cmd_client_socket is invalid");
                    break;
                }

                // 检查 socket 是否有效
                if (cmd_client_socket.handle() == nullptr)
                {
                    LOGE("cmd_client_socket handle = null");
                    break;
                }

                zmq::message_t message;
                auto result = cmd_client_socket.recv(message, zmq::recv_flags::dontwait);
                if (result)
                {
                    if (message.size() == 0)
                    {
                        cmd_client_socket.recv(message, zmq::recv_flags::dontwait);
                    }
                    std::string msg_str(static_cast<char *>(message.data()), message.size());
                    LOGI("收到消息:  %s ", msg_str.c_str());
                    if (msg_str == "heartbeat exit")
                    {
                        mrManagerStatus = MrManagerStatus::EXIT;
                        if (exceptionHandler)
                        {
                            exceptionHandler->onExceptionHandle(MrAgentException::IMI_MRMANAGER_EXIT);
                        }
                        else
                        {
                            LOGI("exceptionHandler is null");
                        }
                        lastFrameNum = 0;
                        frameNum = 0;
                    }
                }
            }
            catch (const zmq::error_t &e)
            {
                std::cerr << "接收消息时出错: " << e.what() << std::endl;
                LOGE("接收消息时出错 ");
                break;
            }
            catch (const std::exception &e)
            {
                LOGE("get string property null pointer exception");
                break;
            }

            if (frameNum == 0)
            {
                index = 1;
            }
            else if (frameNum == lastFrameNum)
            {
                if (mrManagerStatus != MrManagerStatus::EXIT && mrManagerStatus != MrManagerStatus::BLOCK)
                {
                    if (index % 3 == 0)
                    {
                        LOGI("MrManager 发生了 block 3s 以上");
                        mrManagerStatus = MrManagerStatus::BLOCK;
                        if (exceptionHandler)
                        {
                            exceptionHandler->onExceptionHandle(MrAgentException::IMI_MRMANAGER_BLOCK);
                        }
                    }
                    index++;
                }
            }
            else
            {
                if (mrManagerStatus != MrManagerStatus::NORMAL)
                {
                    LOGI("MrManager status Normal");
                    mrManagerStatus = MrManagerStatus::NORMAL;

                    cmd_data_native cmd;
                    cmd.type = CMD_DEVICE_OPEN;
                    cmd.cmdDataSize = 0;
                    sendMsg(cmd);
                }
                index = 1;
            }
            lastFrameNum = frameNum;

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    int IImiBaseDevice::setProperty(int propId, IImiDeviceProperty *prop)
    {
        cmd_data_native cmd;
        cmd.type = CMD_SET_PROPERTY;
        cmd.propId = propId;
        cmd.cmdDataSize = 0;

        if (prop != NULL)
        {
            const void *dataPtr = prop->cPtr(); // 获取void*类型指针
            cmd.cmdDataSize = prop->dataSize(); // 获取数据大小

            LOGD("cmd data size:%d", cmd.cmdDataSize);
            // 假设已知T为int，直接强制类型转换
            if (cmd.cmdDataSize <= sizeof(cmd.cmdData))
            {
                memcpy(cmd.cmdData, dataPtr, cmd.cmdDataSize);
                LOGI("cmdData2 = %s", cmd.cmdData);
                for (int i = 0; i < cmd.cmdDataSize; i++)
                {
                    LOGI("cmdData[%d]=%02x\n", i, cmd.cmdData[i]);
                    printf("cmdData[%d]=%02x\n", i, cmd.cmdData[i]);
                }
                LOGD("cmdData size:%d, data is %s", cmd.cmdDataSize, cmd.cmdData);
            }
            else
            {
                LOGE("prop size too large is %d >64", cmd.cmdDataSize);
                return -1;
            }
        }

        bool ret = sendMsg(cmd);
        if (!ret)
        {
            LOGI("setProperty send cmd failed\n");
            return -1;
        }
        LOGI("setProperty end");
        return 0;
    }

    int IImiBaseDevice::getProperty(int propId, IImiDeviceProperty *prop)
    {
        if (isGettingProperty)
        {
            return -1;
        }
        isGettingProperty = true;

        cmd_data_native cmd;
        cmd.type = CMD_GET_PROPERTY;
        cmd.propId = propId;
        cmd.cmdDataSize = 0;

        LOGD("getProperty %d", propId);
        // 发送命令以获取属性
        bool ret = sendMsg(cmd);
        if (!ret)
        {
            LOGE("getProperty send cmd failed\n");
            return -1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        try
        {
            int index = 0;
            while (index < 6)
            {
                zmq::message_t reply;
                auto result = cmd_client_socket.recv(reply, zmq::recv_flags::dontwait);
                if (result)
                {
                    // 服务端应返回6字节
                    LOGI("getProperty 222");
                    if (reply.size() > 0)
                    {
                        LOGI("getProperty 333");
                        memcpy((void *)prop->cPtr(), reply.data(), prop->dataSize());
                        isGettingProperty = false;
                        return 0;
                    }
                }
                index++;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }

            isGettingProperty = false;
        }
        catch (const zmq::error_t &e)
        {
            isGettingProperty = false;
            std::cerr << "接收消息时出错: " << e.what() << std::endl;
            LOGE("接收消息时出错 ");
        }
        catch (const std::exception &e)
        {
            isGettingProperty = false;
            LOGE("get string property null pointer exception");
        }
        isGettingProperty = false;
        return 0;
    }

    std::string IImiBaseDevice::getStringProperty(int propId)
    {
        if (isGettingProperty)
        {
            return "";
        }
        isGettingProperty = true;

        cmd_data_native cmd;
        cmd.type = CMD_GET_PROPERTY;
        cmd.propId = propId;
        cmd.cmdDataSize = 0;

        LOGD("getProperty %d", propId);
        // 发送命令以获取属性
        bool ret = sendMsg(cmd);
        if (!ret)
        {
            LOGE("getProperty send cmd failed\n");
            return "";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        try
        {

            int index = 0;
            while (index < 5)
            {
                zmq::message_t reply;
                auto result = cmd_client_socket.recv(reply, zmq::recv_flags::dontwait);
                if (result)
                {
                    // 服务端应返回6字节
                    LOGI("getStringProperty 222");
                    if (reply.size() > 0)
                    {
                        std::string reply_str(static_cast<char *>(reply.data()), reply.size());
                        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                        try
                        {
                            converter.from_bytes(reply_str);
                            LOGI("reply_str , %s ", reply_str.c_str());
                            isGettingProperty = false;
                            return reply_str;
                        }
                        catch (const std::range_error &)
                        {
                            // LOGE("字符串乱码");
                            LOGE("String garbled characters");
                            isGettingProperty = false;
                        }
                    }
                }
                index++;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            isGettingProperty = false;
        }
        catch (const zmq::error_t &e)
        {
            //            LOGI("getStringProperty 555");
            isGettingProperty = false;
            std::cerr << "接收消息时出错: " << e.what() << std::endl;
            LOGE("接收消息时出错 ");
        }
        catch (const std::exception &e)
        {
            isGettingProperty = false;
            LOGE("get string property null pointer exception");
            std::cerr << e.what() << std::endl; // 输出: "Null pointer dereference!"
        }
        //        LOGI("getStringProperty 666");
        isGettingProperty = false;
        return "";
    }

    ImiBaseSensor IImiBaseDevice::getSensor(ImiSensorType sensorType)
    {
        auto it = allSensors.find(sensorType);
        if (it != allSensors.end())
        {
            return it->second;
        }

        ImiBaseSensor newSensor;
        switch (sensorType)
        {
            case ImiSensorType::COLOR:
                newSensor = std::make_shared<IImiColorSensor>(this);
                break;
            case ImiSensorType::USERTRACK:
                newSensor = std::make_shared<IImiUserTrackSensor>(this);
                break;
            default:
                return nullptr;
        }

        allSensors[sensorType] = newSensor;
        return newSensor;
    }

    int IImiBaseDevice::open(int openFlag)
    {
        // 定义传感器类型及其对应的位掩码
        const std::vector<std::pair<ImiSensorType, int>> sensorMasks = {
            {ImiSensorType::COLOR, 0x01},         // 0b00000001
            {ImiSensorType::DEPTH, 0x02},         // 0b00000010
            {ImiSensorType::IR, 0x04},            // 0b00000100
            {ImiSensorType::USERTRACK, 0x08},     // 0b00001000
            {ImiSensorType::FACEDETECT, 0x10},    // 0b00010000
            {ImiSensorType::POSITIONTRACK, 0x20}, // 0b00100000
            {ImiSensorType::MOVETRACK, 0x40},     // 0b01000000
            {ImiSensorType::AUDIO, 0x80},         // 0b10000000
            {ImiSensorType::ACTION, 0x100}        // 0b100000000
        };

        // 遍历所有传感器类型
        for (const auto &sensorEntry : sensorMasks)
        {
            // 分解键值对
            const auto &sensorType = sensorEntry.first;
            const auto &mask = sensorEntry.second;

            // 检查 openFlag 是否包含当前传感器类型的位
            if (openFlag & mask)
            {
                getSensor(sensorType); // 创建或获取传感器
            }
        }

        return 0;
    }

    void IImiBaseDevice::releaseHeartBeatTh()
    {
        isCmdClientRunning = false;
        if (th_cmd_client)
        {
            if (th_cmd_client->joinable())
            {
                th_cmd_client->join();
            }
            delete th_cmd_client;
            th_cmd_client = nullptr;
        }
    }

    int IImiBaseDevice::close()
    {
        frameNum = 0;
        lastFrameNum = 0;

        cmd_data_native cmd;
        cmd.type = CMD_DEVICE_CLOSE;
        cmd.cmdDataSize = 0;
        sendMsg(cmd);

        releaseHeartBeatTh();
        try
        {
            if (cmd_client_socket)
            {
                LOGI("client socket close start client_addr = %s", client_addr.c_str());
                //                cmd_client_socket.disconnect(client_addr);
                cmd_client_socket.close();
                LOGI("cmd_client_socket close end");
            }

            if (cmd_ctx.handle() != nullptr)
            {
                cmd_ctx.close();
            }
        }
        catch (const zmq::error_t &e)
        {
            std::cerr << "ZMQ 错误: " << e.what() << std::endl;
            LOGE("ZMQ 错误: ");
        }
        catch (const std::exception &e)
        {
            LOGE("cmd_client_socket close exception");
        }

        printf("clear sensor \n");
        return 0;
    }

    ImiVersions IImiBaseDevice::getVersions() const
    {
        return ImiVersions();
    }

    // --------------------------------------
    // Implementation of class IImiBaseSensor
    // --------------------------------------

    IImiBaseSensor::IImiBaseSensor(ImiSensorType sensorType, IImiBaseDevice *pDevice)
        : sensorType(sensorType), m_pDevice(pDevice)
    {
    }

    IImiBaseSensor::~IImiBaseSensor()
    {
        release();
    }

    int IImiBaseSensor::start(int startFlags)
    {
        return 0;
    }

    int IImiBaseSensor::stop()
    {
        return 0;
    }

    int IImiBaseSensor::release()
    {
        return 0;
    }
    bool IImiBaseSensor::isStarted() const
    {
        return started;
    }

    ImiBaseFrame IImiBaseSensor::readFrame(int timeout)
    {
        return ImiBaseFrame();
    }

    // --------------------------------------
    // Implementation of class IImiColorSensor
    // --------------------------------------
    int IImiColorSensor::start(int mode)
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        LOGI("IImiColorSensor::start");

        cmd_data_native cmd;
        cmd.type = CMD_STOP_COLOR_SENSOR;
        cmd.cmdDataSize = 0;
        if (!sendCommand(cmd))
        { // 调用基类封装的 sendCommand
            LOGE("Failed to send CMD_STOP_COLOR_SENSOR command");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        cmd.type = CMD_START_COLOR_SENSOR;
        if (mode == 0)
        {
            strcpy(cmd.cmdData, "480");
            setWidth(640);
            setHeight(480);
            setDepth(3);
        }
        else if (mode == 1)
        {
            strcpy(cmd.cmdData, "1080");
            setWidth(1920);
            setHeight(1080);
            setDepth(3);
        }
        else
        {
            strcpy(cmd.cmdData, "2k");
            setWidth(2688);
            setHeight(1520);
            setDepth(3);
        }
        cmd.cmdDataSize = 0;

        if (!sendCommand(cmd))
        { // 调用基类封装的 sendCommand
            LOGE("Failed to send START_COLOR_SENSOR command");
            return -1;
        }
        this->mode = mode;
        if (jpegDecoder.Init(this->mode) != 0)
        {
            LOGE("IImiColorSensor::start jpegDecoder Init failed");
            jpegDecoder.DeInit();
        }
        LOGI("IImiColorSensor start mode = %d", mode);

        if (gServerConfig.communicationWay.find("sharedmemory") != std::string::npos)
        {
            LOGI("IImiColorSensor::start share memory");
            if (color_pool.attach("COLOR"))
            {
                started = true;
                LOGI("IImiColorSensor::start COLOR attach completed successfully");
            }
            else
            {
                LOGE("IImiColorSensor::start COLOR attach failed");
            }
        }
        else
        {
            LOGI("IImiColorSensor::start zmq");
            _runThread();
        }
        LOGD("IImiColorSensor start end");
        return 0;
    }

    void IImiColorSensor::_runThread()
    {
        if (started)
        {
            LOGE("IImiColorSensor already started!");
            return;
        }

        started = true;

        _color_th = new std::thread(&IImiColorSensor::onRunColorData, this);
    }

    int IImiColorSensor::stop()
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        LOGI("IImiColorSensor thread stop start");
        printf("IImiColorSensor thread stop start\n");
        cmd_data_native cmd;
        cmd.type = CMD_STOP_COLOR_SENSOR;
        cmd.cmdDataSize = 0;
        if (!sendCommand(cmd))
        { // 调用基类封装的 sendCommand
            LOGE("Failed to send CMD_STOP_COLOR_SENSOR command");
        }
        color_pool.release();
        jpegDecoder.DeInit();
        _terminateThread();
        LOGI("IImiColorSensor thread stop end");
        printf("IImiColorSensor thread stop end\n");
        return 0;
    }

    void IImiColorSensor::_terminateThread()
    {
        started = false;
        LOGI("IImiColorSensor thread begin terminated");
        printf("IImiColorSensor thread begin terminated\n");

        if (_color_th != nullptr)
        {
            printf("_color_th delete start\n");
            if (_color_th->joinable())
            {
                printf("_color_th join be\n");
                _color_th->join();
                printf("_color_th join end\n");
            }
            delete _color_th;
            _color_th = nullptr;
            printf("_color_th delete end\n");
        }
        LOGI("IImiColorSensor thread terminated###");
        printf("terminated end\n");
    }

    ImiBaseFrame IImiColorSensor::readFrame(int timeout)
    {
        static uint32_t read_Color_frameNum = 0;

        std::lock_guard<std::mutex> lock(this->mutex);
        LOGD("before readFrame IImiColorSensor");
        if (!started)
        {
            LOGD("IImiColorSensor not started!");
            return ImiBaseFrame();
        }
        static bool _r1 = [&] {
            LOGI("IImiColorSensor::readFrame gServerConfig.serverAddress=%s color_pool.attached=%d",
                 gServerConfig.serverAddress.c_str(), color_pool.attached());
            return true;
        }();
        if (gServerConfig.communicationWay.find("sharedmemory") != std::string::npos)
        {
            static int count = 0;
            static int preFrameNum = 0;
            uint64_t static recored_time = 0;
            if (count == 0)
            {
                recored_time = getCurrentTimeInMs();
            }

            ImiMessageHeader header{0};
            auto colorFrame = new IImiColorFrame(0, nullptr);
            uint32_t width = 0;
            uint32_t height = 0;
            int bytesRead = 0;
            if (jpegDecoder.Initialized() && jpegDecoder.GetInputData() != nullptr)
            {
                bytesRead = color_pool.readImg(&header, jpegDecoder.GetInputData(), jpegDecoder.GetMaxInputDataSize(),
                                               width, height);
                jpegDecoder.SetInputDataSize(bytesRead);
            }
            if (bytesRead > 0)
            {
                colorFrame->pFrameNum = header.frameNum;

                if (read_Color_frameNum != colorFrame->pFrameNum && colorFrame->pFrameNum % 100 == 1)
                {
                    auto now = getCurrentTimeInMs();
                    LOGD("Color_rate#L4 frameNum =%d,  sdk_timeStamp=%lld", header.frameNum, now);
                    read_Color_frameNum = header.frameNum;
                }
            }
            else if (bytesRead < 0)
            {
                color_pool.release();
                jpegDecoder.DeInit();
                if (color_pool.attach("COLOR"))
                {
                    EXECUTE_EVERY_N_SECONDS(
                        { LOGI("IImiColorSensor::readFrame COLOR attach completed successfully"); }, 5);
                    if (jpegDecoder.Init(this->mode) != 0)
                    {
                        LOGE("IImiColorSensor::readFrame jpegDecoder Init failed");
                        jpegDecoder.DeInit();
                    }
                }
                else
                {
                    LOGE("IImiColorSensor::readFrame COLOR attach failed");
                }
            }
            if (bytesRead > MEG_HEADER_SIZE)
            {
                bytesRead -= MEG_HEADER_SIZE;
                if (jpegDecoder.Initialized())
                {
                    int64_t decode_res = jpegDecoder.Decode(width, height);
                    if (decode_res > 0)
                    {
                        colorFrame->extraData = (uint8_t *)jpegDecoder.GetOutputData();
                        colorFrame->extraDataSize = jpegDecoder.GetOutputDataSize();
                    }
                }
            }
            if (colorFrame->extraDataSize <= 0)
            {
                colorFrame->release();
                delete colorFrame;
                return ImiBaseFrame();
            }
            if (colorFrame->pFrameNum > 0 && colorFrame->pFrameNum != preFrameNum)
            {
                preFrameNum = colorFrame->pFrameNum;
                count++;
            }
            if (count >= 499)
            {
                float diff = getCurrentTimeInMs() - recored_time;
                float fps;
                fps = 500.0 * 1000.0 / diff;
                count = 0;
                LOGI("IImiColorSensor::readFrame colorFrame->extraDataSize=%d, colorFrame->frameNum=%d, fps=%f",
                     colorFrame->extraDataSize, colorFrame->pFrameNum, fps);
                cmd_data_native cmd;
                cmd.type = CMD_COLOR_ACK;
                cmd.cmdDataSize = 0;
                sendCommand(cmd);
            }
            return ImiBaseFrame(colorFrame);
        }
        else
        {
            if (gColorDataBuffer == nullptr)
            {
                LOGD("gColorDataBuffer is null");
                return ImiBaseFrame();
            }
            int dataSize = COLOR_FRAME_WIDTH_MAX * COLOR_FRAME_HIGHT_MAX * 3;
            auto colorFrame = new IImiColorFrame(0, nullptr);
            if (gColorDataBuffer->getFrontSize() > 0)
            {
                static int count = 0;
                static int preFrameNum = 0;
                uint64_t static recored_time = 0;
                if (count == 0)
                {
                    recored_time = getCurrentTimeInMs();
                }

                int bytesRead = 0;
                if (jpegDecoder.Initialized() && jpegDecoder.GetInputData() != nullptr)
                {
                    bytesRead = gColorDataBuffer->read((uint8_t *)jpegDecoder.GetInputData(), dataSize);
                    jpegDecoder.SetInputDataSize(bytesRead);
                }
                if (bytesRead > 0)
                {
                    colorFrame->pFrameNum = gColorDataBuffer->getFrameNum();
                    int64_t decode_res = jpegDecoder.Decode(width, height);
                    if (decode_res > 0)
                    {
                        colorFrame->extraData = (uint8_t *)jpegDecoder.GetOutputData();
                        colorFrame->extraDataSize = jpegDecoder.GetOutputDataSize();
                    }
                }
                if (colorFrame->extraDataSize <= 0)
                {
                    return ImiBaseFrame();
                }
                if (colorFrame->pFrameNum > 0 && colorFrame->pFrameNum != preFrameNum)
                {
                    preFrameNum = colorFrame->pFrameNum;
                    count++;
                }
                if (count >= 499)
                {
                    float diff = getCurrentTimeInMs() - recored_time;
                    float fps;
                    fps = 500.0 * 1000.0 / diff;
                    count = 0;
                    LOGI("IImiColorSensor::readFrame colorFrame->extraData=%d, colorFrame->frameNum=%d, fps=%f",
                         colorFrame->extraData, colorFrame->pFrameNum, fps);
                }
            }
            else
            {
                return ImiBaseFrame();
            }
            return ImiBaseFrame(colorFrame);
        }
    }

    void IImiColorSensor::onRunColorData()
    {
        zmq::context_t sub_context{1};
        zmq::socket_t *sub_socket = new zmq::socket_t(sub_context, zmq::socket_type::sub);

        // 设置接收超时为500 毫秒
        int timeout_ms = 500;
        sub_socket->setsockopt(ZMQ_RCVTIMEO, timeout_ms);

        std::stringstream ss;
        ss << "tcp://" << gServerConfig.serverAddress << ":" << gServerConfig.colorDataPort;
        LOGI("Connecting to color sensor %s", ss.str().c_str());

        try
        {
            sub_socket->connect(ss.str());
            sub_socket->setsockopt(ZMQ_SUBSCRIBE, "", 0);
        }
        catch (const zmq::error_t &ex)
        {
            LOGE("连接失败: %s", ex.what());
            return;
        }

        int width = this->width;
        int height = this->height;
        int depth = this->depth;
        int dataSize = COLOR_FRAME_WIDTH_MAX * COLOR_FRAME_HIGHT_MAX * 3;
        gColorDataBuffer = new DoubleBuffer(dataSize + 32);
        LOGI("width=%d, height=%d, depth=%d dataSize:%d", width, height, depth, dataSize);

        while (this->started)
        {
            zmq::message_t data_msg;

            try
            {
                zmq::recv_result_t result = sub_socket->recv(data_msg, zmq::recv_flags::none);
                if (!result)
                {
                    continue;
                }
            }
            catch (const zmq::error_t &e)
            {
                if (e.num() == EAGAIN)
                {
                    LOGI("color zmq 接收超时");
                    continue;
                }
                else
                {
                    LOGE("color zmq 接收错误: %s", e.what());
                    break;
                }
            }

            LOGD("received color frame size: %d", data_msg.size());
            if (data_msg.size() <= 0)
            {
                LOGE("Received empty message for color data");
                continue;
            }

            int headerSize = sizeof(ImiMessageHeader);
            auto p = (ImiMessageHeader *)(data_msg.data());
            static int count = 0;
            count++;
            if (count % 500 == 0)
            {
                cmd_data_native cmd;
                cmd.type = CMD_COLOR_ACK;
                cmd.cmdDataSize = 0;
                sendCommand(cmd);
            }
            if (count > 2000)
            {
                LOGI("recv zmq color frameNum=%d, size=%d, timeStamp=%ld", p->frameNum, data_msg.size() - headerSize,
                     p->timeStamp);
                count = 0;
            }
            if (!gColorDataBuffer->write((const uint8_t *)data_msg.data() + headerSize, data_msg.size() - headerSize,
                                         p->frameNum))
            {
                LOGE("gColorDataBuffer write failed, buffer_size=%d, write size=%d", dataSize + 32,
                     data_msg.size() - headerSize);
            }
            else
            {
                gColorDataBuffer->setFrameNum(p->frameNum);
                LOGD("gColorDataBuffer write success, buffer_size=%d, write size=%d", dataSize + 32,
                     data_msg.size() - headerSize);
            }
            LOGD("gColorDataBuffer write %d", data_msg.size() - headerSize);
        }
        LOGD("color thread is terminated\n");
        sub_socket->setsockopt(ZMQ_UNSUBSCRIBE, "", 0);

        sub_socket->close();
        delete sub_socket;

        if (gColorDataBuffer)
        {
            delete gColorDataBuffer;
            gColorDataBuffer = nullptr;
            printf("delete gColorDataBuffer\n");
        }
        printf("color thread is finished\n");
        LOGD("color thread is finished\n");
    }

    // --------------------------------------
    // Implementation of class IImiImageFrame
    // --------------------------------------

    IImiImageFrame::~IImiImageFrame() {}

    int IImiImageFrame::getPixelFormat() const
    {
        return 0;
    }

    int IImiImageFrame::getFps() const
    {
        return 25;
    }

    // --------------------------------------
    // Implementation of class IImiColorFrame
    // --------------------------------------
    IImiColorFrame::IImiColorFrame(int dataSize) : IImiImageFrame(ImiSensorType::COLOR)
    {
        _colorData = new unsigned char[dataSize];
        _colorDataSize = dataSize;
    }

    IImiColorFrame::IImiColorFrame(int extraDataSize, uint8_t *extraData) : IImiImageFrame(ImiSensorType::COLOR)
    {
        _colorData = nullptr;
        _colorDataSize = 0;
        this->extraDataSize = extraDataSize;
        this->extraData = extraData;
    }

    void IImiColorFrame::release()
    {
        if (_colorData != nullptr)
        {
            delete[] _colorData;
            _colorData = nullptr;
            _colorDataSize = 0;
        }
    }

    void IImiColorFrame::getData(unsigned char *array_ref, int array_len) const
    {
        if (_colorDataSize > 0)
        {
            int copySize = std::min(array_len, _colorDataSize);
            memcpy(array_ref, (void *)_colorData, copySize);
        }
        else
        {
            int copySize = std::min(array_len, extraDataSize);
            memcpy(array_ref, (void *)extraData, copySize);
        }
    }

    int IImiColorFrame::getDataSize() const
    {
        if (_colorDataSize > 0)
        {
            return _colorDataSize;
        }
        else
        {
            return extraDataSize;
        }
    }

    unsigned long IImiColorFrame::getDataPointer() const
    {
        if (_colorData != nullptr)
        {
            return (unsigned long)_colorData;
        }
        else
        {
            return (unsigned long)extraData;
        }
    }

    // --------------------------------------
    // Implementation of class IImiDepthFrame
    // --------------------------------------

    void IImiDepthFrame::getRgbData(unsigned char *array_ref, int array_len) const {}

    void IImiIRFrame::getRgbData(unsigned char *array_ref, int array_len) const {}

    // --------------------------------------
    // Implementation of class IImiUserTrackSensor
    // --------------------------------------
    int IImiUserTrackSensor::start(int startFlags)
    {
        setWidth(320);
        setHeight(240);
        setDepth(2);
        LOGI("communicationWay IImiUserTrackSensor = %s",gServerConfig.communicationWay.c_str());
        if (gServerConfig.communicationWay.find("sharedmemory") != std::string::npos)
        {
            LOGI("IImiUserTrackSensor::start share memory");
            if (foreground_pool.attach("FOREGROUND") && skeleton_pool.attach("SKELETON"))
            {
                started = true;
                LOGI("IImiUserTrackSensor::start USERTRACK attach completed successfully");
            }
            else
            {
                LOGE("IImiUserTrackSensor::start USERTRACK attach failed");
            }
        }
        else // zmq
        {
            IImiUserTrackSensor::_runThread();
        }

        return 0;
    }

    void IImiUserTrackSensor::_runThread()
    {
        started = true;

        _usertrack_th = new std::thread(&IImiUserTrackSensor::onRunUserTrackData, this);
    }

    int IImiUserTrackSensor::stop()
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        printf("IImiUserTrackSensor thread stop begin\n");
        _terminateThread();
#if defined(NOTGAME)
        std::unique_lock<std::mutex> lck(mtx);
        ready = true;
        cv.notify_all();
#endif

        foreground_pool.release();
        skeleton_pool.release();

        printf("IImiUserTrackSensor thread stop end\n");
        LOGD("IImiUserTrackSensor thread stop end");
        return 0;
    }

    void IImiUserTrackSensor::_terminateThread()
    {
        if (!started)
        {
            return;
        }
        started = false;

        if (_usertrack_th != nullptr)
        {
            if (_usertrack_th->joinable())
            {
                _usertrack_th->join();
            }
            delete _usertrack_th;
            _usertrack_th = nullptr;
        }
    }

    ImiBaseFrame IImiUserTrackSensor::readFrame(int timeout)
    {
        std::lock_guard<std::mutex> lock(this->mutex);
        if (!started && reconnectIndex % 50 == 0)
        {
            if (foreground_pool.attach("FOREGROUND") && skeleton_pool.attach("SKELETON"))
            {
                LOGI("IImiUserTrackSensor::readFrame reattach completed successfully");
                started = true;
            }
            reconnectIndex++;
            return ImiBaseFrame();
        }
        static int read_frameNum = 0;
        static int read_Foreground_frameNum = 0;
        static int read_Skeleton_frameNum = 0;
        static int64_t recored_time = 0;
        static int cur_frameNum = 0;
        static uint32_t frameNum_Skeleton_cache = 0;

        auto now = getCurrentTimeInMs();
        read_frameNum++;

        if (read_frameNum == 1)
        {
            recored_time = now;
        }
        else if (read_frameNum == 1000)
        {
            auto diff = now - recored_time;
            float fps;
            if (diff > 0)
            {
                fps = read_Foreground_frameNum * 1000.0 / diff;
            }
            else
            {
                fps = 0;
            }

			if (0 != cur_frameNum)
			{
				LOGI("usertrack read %d frames cur_frameNum =%d, read Foreground frames =%d, read Skeleton frames =%d,fps "
                 "=%f",
                 read_frameNum, cur_frameNum, read_Foreground_frameNum, read_Skeleton_frameNum, fps);

            	printf("usertrack read %d frames cur_frameNum =%d, read_Foreground_frameNum =%d, read_Skeleton_frameNum "
                   "=%d,fps =%f\n",
                   read_frameNum, cur_frameNum, read_Foreground_frameNum, read_Skeleton_frameNum, fps);
			}

            read_Foreground_frameNum = 0;
            read_Skeleton_frameNum = 0;
            read_frameNum = 0;
        }

        if (gServerConfig.communicationWay.find("sharedmemory") != std::string::npos)
        {
            ImiMessageHeader header{0};
            uint32_t dataSize = width * height * depth;
            auto usertrackFrame = new IImiUserTrackFrame(dataSize);

 			uint32_t w, h;
            int SkeletonbytesRead = skeleton_pool.readImg(&header, (void *)&usertrackFrame->_bodySkeleton,
                                                          sizeof(usertrackFrame->_bodySkeleton), w, h);
            if (SkeletonbytesRead > 0)
            {
                // 构造 ImiBaseFrame 返回
                usertrackFrame->pFrameNum = header.frameNum;
                cur_frameNum = header.frameNum;
                read_Skeleton_frameNum++;
                if (read_Skeleton_frameNum % 10 == 1)
                {
                    LOGD("IImiUserTrackSensor::readFrame read_Skeleton_frameNum=%d, size=%d", read_Skeleton_frameNum,
                         SkeletonbytesRead);
                }
            }

            int foregroundbytesRead =
                foreground_pool.readImg(&header, (void *)usertrackFrame->_foregroundData, dataSize, w, h);
            if (foregroundbytesRead > 0)
            {
                // 构造 ImiBaseFrame 返回
                usertrackFrame->pFrameNum = header.frameNum;
                frameNum = header.frameNum;
                read_Foreground_frameNum++;
                cur_frameNum = header.frameNum;
                if (read_Foreground_frameNum % 10 == 1)
                {
                    LOGD("IImiUserTrackSensor::readFrame read_Foreground_frameNum=%d, size=%d",
                         read_Foreground_frameNum, foregroundbytesRead);
                }
            }
            if (foregroundbytesRead < 0 || SkeletonbytesRead < 0)
            {
                EXECUTE_EVERY_N_SECONDS({ LOGI("IImiUserTrackSensor::readFrame no foreground or skeleton data"); }, 5);
                foreground_pool.release();
                skeleton_pool.release();
                if (foreground_pool.attach("FOREGROUND") && skeleton_pool.attach("SKELETON"))
                {
                    EXECUTE_EVERY_N_SECONDS(
                        { LOGI("IImiUserTrackSensor::readFrame reattach completed successfully"); }, 5);
                }
                else
                {
                    foreground_pool.release();
                    skeleton_pool.release();
                }
            }
            if (frameNum_Skeleton_cache != usertrackFrame->pFrameNum && usertrackFrame->pFrameNum % 100 == 1)
            {
                frameNum_Skeleton_cache = usertrackFrame->pFrameNum;
                auto now = getCurrentTimeInMs();
                LOGD("Statistical_rate#T4 frameNum =%d,  sdk_timeStamp=%lld", usertrackFrame->pFrameNum, now);
            }
            if (foregroundbytesRead <= 0 || SkeletonbytesRead <= 0)
            {
                usertrackFrame->release();
                delete usertrackFrame;
                return ImiBaseFrame();
            }
            return ImiBaseFrame(usertrackFrame);
        }
        else // zmq
        {
            if (gForegroundDataBuffer == nullptr)
            {
                return ImiBaseFrame();
            }

            if (gBodySkeletonDataBuffer == nullptr)
            {
                return ImiBaseFrame();
            }

            if (gForegroundDataBuffer->getFrontSize() <= 0 && gBodySkeletonDataBuffer->getFrontSize() <= 0)
            {
                return ImiBaseFrame();
            }

            int dataSize = width * height * depth;
            auto usertrackFrame = new IImiUserTrackFrame(dataSize);

            static int pre_frame_num = 0;
            if (gForegroundDataBuffer->getFrontSize() > 0)
            {
                // todo:
                if (gForegroundDataBuffer->getFrameNum() != pre_frame_num)
                {
                    read_Foreground_frameNum++;
                    pre_frame_num = gForegroundDataBuffer->getFrameNum();
                }
                std::size_t bytesRead = gForegroundDataBuffer->read(usertrackFrame->_foregroundData, dataSize);
                usertrackFrame->_foregroundDataSize = bytesRead;
                usertrackFrame->pFrameNum = gForegroundDataBuffer->getFrameNum();
                LOGD("usertrackFrame foreground  pFrameNum=%d, size =%d", usertrackFrame->pFrameNum, bytesRead);
            }

            if (gBodySkeletonDataBuffer->getFrontSize() > 0)
            {
                read_Skeleton_frameNum++;
                std::size_t bytesRead = gBodySkeletonDataBuffer->read((uint8_t *)&usertrackFrame->_bodySkeleton,
                                                                      sizeof(usertrackFrame->_bodySkeleton));
                usertrackFrame->pFrameNum = gBodySkeletonDataBuffer->getFrameNum();
                if (frameNum_Skeleton_cache != usertrackFrame->pFrameNum && usertrackFrame->pFrameNum % 10 == 1)
                {
                    frameNum_Skeleton_cache = usertrackFrame->pFrameNum;
                    auto now = getCurrentTimeInMs();
                    LOGD("Statistical_rate#T5 frameNum =%d,  sdk_timeStamp=%lld", usertrackFrame->pFrameNum, now);
                }
                LOGD("usertrackFrame BodySkeleton  pFrameNum=%d, size =%d", usertrackFrame->pFrameNum, bytesRead);
            }
            cur_frameNum = usertrackFrame->pFrameNum;
            LOGD("usertack read frame frameNum =%d,  sdk_timeStamp=%lld", usertrackFrame->pFrameNum, now);
            return ImiBaseFrame(usertrackFrame);
        }
    }

    void IImiUserTrackSensor::onRunUserTrackData()
    {
        int headerSize = sizeof(ImiMessageHeader);
        LOGD(" onRunUserTrackData thread headerSize %d", headerSize);
        zmq::context_t sub_context{1};
        zmq::socket_t *usertrack_socket = new zmq::socket_t(sub_context, zmq::socket_type::sub);
        bool is_usertrack_connected = false;

        std::stringstream ss;
        ss << "tcp://" << gServerConfig.serverAddress << ":" << gServerConfig.userTrackDataPort;
        std::string usertrack_addr = ss.str();
        LOGI("mragent usertrack sensor  connect %s start", ss.str().c_str());
        try
        {
            usertrack_socket->connect(ss.str());
            is_usertrack_connected = true;
        }
        catch (const zmq::error_t &ex)
        {
            LOGE("连接失败: %s", ex.what());
            is_usertrack_connected = false;
            return;
        }

        // 订阅主题
        std::string topic_fg = "foreground_data";
        std::string topic_skt = "skeleton_data";
        usertrack_socket->setsockopt(ZMQ_SUBSCRIBE, topic_fg.c_str(), topic_fg.size());
        usertrack_socket->setsockopt(ZMQ_SUBSCRIBE, topic_skt.c_str(), topic_skt.size());

        if (usertrack_socket == nullptr)
        {
            std::cerr << "Failed to create sub socket for foreground data" << std::endl;
            return;
        }

        int foregroundDataSize = this->width * this->height * this->depth;
        gForegroundDataBuffer = new DoubleBuffer(foregroundDataSize + 32);

        int bodySkeletonDataSize = sizeof(imimr::ImiBodySkeleton);
        gBodySkeletonDataBuffer = new DoubleBuffer(bodySkeletonDataSize + 32);

        // 新增静态变量用于统计接收次数和帧率
        int recv_Num = 0;
        int recv_Foreground_frameNum = 0;
        int recv_Skeleton_frameNum = 0;
        int64_t recv_recorded_time = 0;
        int cur_recv_frameNum = 0;

        int recv_delay = 0;
        int max_delay_times = 100; // 最大延迟次数

        while (this->started)
        {
            zmq::message_t topic_msg;
            zmq::message_t data_msg;
            try
            {
                // 非阻塞接收主题
                zmq::recv_result_t topic_result = usertrack_socket->recv(topic_msg, zmq::recv_flags::dontwait);
                if (!topic_result && is_usertrack_connected)
                {
                    recv_delay++;
                    if (recv_delay == max_delay_times)
                    {
                        LOGI("usertrack socket重置 2");
                        usertrack_socket->disconnect(usertrack_addr);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));

                        // 建立新连接
                        usertrack_socket->connect(usertrack_addr);
                        LOGI("重连usertrack socket success");
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 降低 CPU 占用
                    continue;
                }

                // 非阻塞接收数据
                zmq::recv_result_t data_result = usertrack_socket->recv(data_msg, zmq::recv_flags::dontwait);
                if (!data_result)
                {
                    std::string topicStr(static_cast<char *>(topic_msg.data()), topic_msg.size());
                    LOGE("收到主题但未收到数据，主题: %s", topicStr.c_str());
                    recv_delay++;
                    if (recv_delay == max_delay_times && is_usertrack_connected)
                    {
                        LOGI("usertrack socket重置 3");
                        usertrack_socket->disconnect(usertrack_addr);
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));

                        // 建立新连接
                        usertrack_socket->connect(usertrack_addr);
                        LOGI("重连usertrack socket");
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 降低 CPU 占用
                    continue;
                }
            }
            catch (const zmq::error_t &e)
            {
                if (e.num() == EAGAIN)
                {
                    // 超时，继续循环
                    continue;
                }
                else
                {
                    LOGE("接收错误: %s", e.what());
                    break;
                }
            }
            recv_Num++;
            recv_delay = 0; // 重置接收延迟计数

            // Convert messages to strings
            std::string received_topic(static_cast<char *>(topic_msg.data()), topic_msg.size());

            if (data_msg.size() <= 0)
            {
                std::cout << "data size <= 0" << std::endl;
                continue;
            }

            if (received_topic == topic_fg)
            {
                recv_Foreground_frameNum++;
                int headerSize = sizeof(ImiMessageHeader);
                auto p = (ImiMessageHeader *)(data_msg.data());

                auto now = getCurrentTimeInMs();
                gForegroundDataBuffer->write((const uint8_t *)data_msg.data() + headerSize,
                                             data_msg.size() - headerSize, p->frameNum);
                gForegroundDataBuffer->setFrameNum(p->frameNum);
                cur_recv_frameNum = p->frameNum;
                frameNum = p->frameNum;
                recv_delay = 0; // 重置接收延迟计数
                {
                    auto now = getCurrentTimeInMs();
                    LOGD("recv fg frameNum =%d,  sdk_timeStamp=%lld", p->frameNum, now);
                }
            }
            else if (received_topic == topic_skt)
            {
                recv_Skeleton_frameNum++;
                int headerSize = sizeof(ImiMessageHeader);
                auto p = (ImiMessageHeader *)(data_msg.data());

                auto now = getCurrentTimeInMs();

                uint8_t *pp = (uint8_t *)data_msg.data() + headerSize;

                if (p->frameNum % 10 == 1)
                {
                    LOGD("Statistical_rate#T4 frameNum =%d,  sdk_timeStamp=%lld", p->frameNum, now);
                }

                gBodySkeletonDataBuffer->write((const uint8_t *)data_msg.data() + headerSize,
                                               data_msg.size() - headerSize, -1);
                gBodySkeletonDataBuffer->setFrameNum(p->frameNum);
                cur_recv_frameNum = p->frameNum;
                LOGD("recv sk frameNum =%d,  sdk_timeStamp=%lld", p->frameNum, now);
            }

            if (recv_Num == 1)
            {
                recv_recorded_time = getCurrentTimeInMs();
            }
            else if (recv_Num == 100)
            {
                auto now = getCurrentTimeInMs();
                auto diff = now - recv_recorded_time;
                float fps = 0.0f;
                if (diff > 0)
                {
                    fps = recv_Foreground_frameNum * 1000.0f / diff; // 计算FPS
                }

                // 重置计数器
                recv_Num = 0;
                recv_Foreground_frameNum = 0;
                recv_Skeleton_frameNum = 0;
                recv_recorded_time = now;
            }
        }

        usertrack_socket->setsockopt(ZMQ_UNSUBSCRIBE, "", 0);

        usertrack_socket->close();
        delete usertrack_socket;
        is_usertrack_connected = false;

        if (gForegroundDataBuffer)
        {
            delete gForegroundDataBuffer;
            gForegroundDataBuffer = nullptr;
        }

        if (gBodySkeletonDataBuffer)
        {
            delete gBodySkeletonDataBuffer;
            gBodySkeletonDataBuffer = nullptr;
        }
        LOGD("usertrack thread is finished\n");
    }

    // --------------------------------------
    // Implementation of class IImiUserTrackFrame
    // --------------------------------------
    IImiUserTrackFrame::IImiUserTrackFrame(int fgSize) : IImiBaseFrame(ImiSensorType::USERTRACK)
    {
        _foregroundData = new unsigned char[fgSize];
        _foregroundDataSize = fgSize;
    }

    void IImiUserTrackFrame::release()
    {
        if (_foregroundData != nullptr)
        {
            delete _foregroundData;
            _foregroundData = nullptr;
            _foregroundDataSize = 0;
        }
    }

    ImiBodySkeleton IImiUserTrackFrame::getBodySkeleton() const
    {
        return (_bodySkeleton);
    }

    bool IImiUserTrackFrame::getBodySkeleton(ImiBodySkeleton *sk) const
    {
        memcpy(sk, &_bodySkeleton, sizeof(ImiBodySkeleton));
        return true;
    }

    ImiHandSkeleton IImiUserTrackFrame::getHandSkeleton() const
    {
        return ImiHandSkeleton();
    }

    /// @brief 获取背景图数据
    /// @return
    ImiForegroundData IImiUserTrackFrame::getForegroundInfo() const
    {
        // todo:
        return ImiForegroundData();
    }

    /// @brief 获取前景图像数据.
    /// @param array_ref    数据
    /// @param array_len    大小
    int IImiUserTrackFrame::getForegroundData(unsigned char *array_ref, int array_len) const
    {
        if (array_len <= 0 || _foregroundDataSize <= 0 || array_len < _foregroundDataSize)
        {
            return -1;
        }

        memcpy(array_ref, _foregroundData, _foregroundDataSize);
        return _foregroundDataSize;
    }

    /// @brief 获取抠图数据
    /// @return
    ImiMattingData IImiUserTrackFrame::getMattingInfo() const
    {
        // todo:
        return ImiMattingData();
    }

    /// @brief 获取抠图图像数据.
    /// @param array_ref    数据
    /// @param array_len    大小： w*h*1
    void IImiUserTrackFrame::getMattingData(unsigned char *array_ref, int array_len) const {}

    /// @brief 获取用户人脸特征数据
    /// @return
    ImiFaceFeatureData IImiUserTrackFrame::getFaceFeatureFrame() const
    {
        return ImiFaceFeatureData();
    }

    int IImiUserTrackFrame::getForegroundRgbData(unsigned char *array_ref, int array_len, int mode) const
    {
        if (array_len <= 0 || _foregroundDataSize <= 0 || _foregroundData == nullptr ||
            array_len < _foregroundDataSize / 2 * 3)
        {
            return -1;
        }

        int len = _foregroundDataSize / 2;
        uint16_t *src = (uint16_t *)_foregroundData;
        uint8_t *dst = array_ref;

        for (int i = 0; i < len; i++, dst += 3, src++)
        {
            uint16_t uid = (*src) & 0x0007;
            uint16_t depth;
            if (mode == 0)
            { // mode == 0 过滤掉背景
                depth = 0;
            }
            else
            { // mode == 1 显示所有数据
                depth = ((*src) >> 3) * 0.056;
                depth = depth & 0xff;
            }

            if (uid == 0)
            {
                dst[0] = depth;
                dst[1] = depth;
                dst[2] = depth;
            }
            else
            {
                uint8_t b = depth >> g_IntensityShiftByPlayerB[uid];
                uint8_t g = depth >> g_IntensityShiftByPlayerG[uid];
                uint8_t r = depth >> g_IntensityShiftByPlayerR[uid];

                dst[0] = b;
                dst[1] = g;
                dst[2] = r;
            }
        }

        return _foregroundDataSize / 2 * 3;
    }

    int IImiUserTrackFrame::getForegroundArgbData(unsigned char *array_ref, int array_len, int mode) const
    {
        if (array_len <= 0 || _foregroundDataSize <= 0 || _foregroundData == nullptr ||
            array_len < _foregroundDataSize / 2 * 4)
        {
            return -1;
        }

        int len = _foregroundDataSize / 2;
        uint16_t *src = (uint16_t *)_foregroundData;
        uint8_t *dst = array_ref;

        for (int i = 0; i < len; i++, dst += 4, src++)
        {
            uint16_t uid = (*src) & 0x0007;
            uint16_t depth;
            if (mode == 0)
            { // mode == 0 过滤掉背景
                depth = 0;
            }
            else
            { // mode == 1 显示所有数据
                depth = ((*src) >> 3) * 0.056;
                depth = depth & 0xff;
            }

            if (uid == 0)
            {
                dst[0] = depth;
                dst[1] = depth;
                dst[2] = depth;
                dst[3] = 255;
            }
            else
            {
                uint8_t b = depth >> g_IntensityShiftByPlayerB[uid];
                uint8_t g = depth >> g_IntensityShiftByPlayerG[uid];
                uint8_t r = depth >> g_IntensityShiftByPlayerR[uid];

                dst[0] = b;
                dst[1] = g;
                dst[2] = r;
                dst[3] = 255;
            }
        }

        return _foregroundDataSize / 2 * 4;
    }

    int IImiUserTrackFrame::getBodySkeletonData(unsigned char *array_ref, int array_len) const
    {
        if (array_len <= 0 || array_len < sizeof(_bodySkeleton))
        {
            return -1;
        }

        memcpy(array_ref, &_bodySkeleton, sizeof(_bodySkeleton));
        return sizeof(_bodySkeleton);
    }
} // namespace imimr
