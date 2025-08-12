#pragma once

#include <list>
#include <thread>

#include "ImiMrApi.h"
#include "ImiMrCmd.h"
#include "ShareMemory.h"
#include "zmq.hpp"
#include <condition_variable>


#ifndef SWIG
#    if IMI_DATA_VERSION != 2
#        error ImiMrTypes.h versio is not right
#    endif
#endif

namespace mrcore
{
    // @brief 获取MessageCore版本号
    std::string getVersion();

    enum cmd_type
    {
        CMD_GET_VERSION = 0,            ///< 无命令
        CMD_SET_PROPERTY = 1,           ///< 启动命令
        CMD_GET_PROPERTY = 2,           ///< 停止命令
        CMD_SET_SENSOR = 3,             ///< 暂停命令
        CMD_GET_SENSOR = 4,             ///< 恢复命令
        CMD_HEARTBEAT = 5,              ///< 心跳命令
        CMD_START_COLOR_SENSOR = 6,     ///< 启动彩色传感器命令
        CMD_START_DEPTH_SENSOR = 7,     ///< 启动深度传感器命令
        CMD_START_IR_SENSOR = 8,        ///< 启动红外传感器命令
        CMD_START_USERTRACK_SENSOR = 9, ///< 启动用户跟踪传感器命令
        CMD_STOP_COLOR_SENSOR = 10,     ///< 停止彩色传感器命令
        CMD_STOP_DEPTH_SENSOR = 11,     ///< 停止深度传感器命令
        CMD_STOP_IR_SENSOR = 12,        ///< 停止红外传感器命令
        CMD_STOP_USERTRACK_SENSOR = 13, ///< 停止用户跟踪传感器命令
        CMD_COLOR_ACK = 14,             ///< color ack
        CMD_DEVICE_OPEN = 15,           ///< device open
        CMD_DEVICE_CLOSE = 16,          ///< device close
    };

    struct cmd_data
    {
            void bindBuffer(DataBuffer data)
            {
                _data = data;
            }

            cmd_type cmd;        // 4 字节
            int propId;          // 业务ID
            int cmdDataSize;     // 4 字节 (强制固定宽度)
            std::string cmdData; // 64 字节
            std::string extraInfo;
#ifndef SWIG
            DataBuffer _data;
#endif
    };

    enum ErrorCode
    {
        UNKNOWN = 0,
        ERR = 1,
        INVALID_PARAM = 2,
        INTERNAL_ERROR = 3,
    };

    class MessageHandler;

    /// @brief 负责管理消息的订阅/发布,维护用户列表，控制ImiBaseDevice打开必要的Sensor.
    class MessageCore
    {
        public:
            MessageCore();
            virtual ~MessageCore();

            enum LogLevel
            {
                LOG_DEBUG = 0,
                LOG_INFO = 1,
                LOG_ERROR = 2,
            };

            /// @brief 初始化.
            /// @param config 配置文件路径.
            /// @return
            bool initialize(const char *configFilePath);

            /// @brief 准备工作.
            /// @param device       MR设备接口.
            /// @param dataServer   数据服务接口.
            /// @return
            // bool prepare();

            /// @brief 开始工作（非堵塞）.
            /// @return
            bool start(int startFlags);

            /// @brief 停止工作
            void stop();

            /// @brief 释放资源.
            void release();

            /// @brief 停止工作后等待线程结束.
            void join();

            /// @brief 暂停服务。释放MR设备资源
            void pause();

            /// @brief 恢复服务
            void resume();

            // /// @brief MessageCore读取帧数据时的回调，上层APP重载该方法可以拦截数据并渲染做其他操作.
            // /// @param frame 当前读取的frame
            // /// @return
            // virtual void onReadFrame(std::shared_ptr<imimr::IImiBaseFrame> frame)
            // {
            //     // do nothing
            // }

            /// @brief MessageCore发生错误的回调.上层APP重载该方法实现自定义的操作.
            /// @param errCode
            /// @param err
            virtual void onError(int errCode, const char *err);

            /// @brief MessageCore的日志的回调.上层APP重载该方法实现自定义的操作.
            /// @param level 见LogLevel
            /// @param msg
            virtual void onLog(int level, const char *msg);

            int syncColorData(unsigned char *array_ref, int array_len);

            int syncSkeletonData(unsigned char *array_ref, int array_len);

            int syncForegroundData(unsigned char *array_ref, int array_len);

            int syncActionsData(std::string data);

            bool isStarted();

            int publishColorFrame(unsigned char *array_ref, int array_len, int w, int h, int frameNum);

            int publishForegroundData(unsigned char *array_ref, int array_len, int w, int h, int frameNum);

            int publishBodySkeletonData(const imimr::ImiBodySkeleton &skeletons, int frameNum);

            /// @brief callback function for command
            /// @param cmd a
            virtual bool onCommandHandler(cmd_data *cmd)
            {
                return false;
            }

            /// @brief color sensor close
            virtual bool onColorSensorClose()
            {
                return false;
            }

        private:
            zmq::context_t context; // 单例 context
            bool isCoreStarted = false;
            zmq::socket_t *cmd_server_socket = nullptr;
            zmq::socket_t *usertrack_socket = nullptr;
            zmq::socket_t *color_socket = nullptr;

            // @brief BindSocket
            void bindSocket();
            void bindCmdServerSocket();
            void parseCmd(zmq::message_t &identity, zmq::message_t &request);
            void sendHeartbeatMsg(std::string client_id, std::string message);
            void heartRecv();
            int colorDataBuffer_size = 0;
            uint8_t *colorDataBuffer = nullptr;

            std::list<std::thread *> _allThreads;
            std::list<std::string> clientList;

            std::thread *th_heartbeat;

            static void onRunMessageHandler(MessageCore *core);
            static void sendMessage(void *socket, const char *msg, int len, int frameNum);

            bool isClientAdded(std::string &client_id);

            bool setup_shared_memory();

            using SHMType = SharedMemoryDoubleBuffer;
            SHMType ColorPool;     // 只需这一行
            SHMType ForegroudPool; // 只需这一行
            SHMType SkeletonPool;  // 只需这一行
            std::string address;
    };

} // namespace mrcore