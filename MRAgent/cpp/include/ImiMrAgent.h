#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>


#include "ImiMrCmd.h"
#include "ImiMrTypes.h"
#include "JpegHwDecoder.h"
#include "ShareMemory.h"
#include "zmq.hpp"
#include <condition_variable>


#if IMI_DATA_VERSION != 2

#    error ImiMrTypes.h versio is not right
#endif

struct ImiDevice;
struct ImiSensor;
struct ImiRecord;
struct ImiImageFrame;
struct ImiStream;
struct ImiCamera;

#define ACTIONS_DATA_SIZE (1024 * 1024)
namespace imimr
{

#define REF_POINTER(ref_type)                                                                                          \
    class I##ref_type;                                                                                                 \
    typedef std::shared_ptr<I##ref_type> ref_type;

    REF_POINTER(ImiBaseDevice)
    REF_POINTER(ImiBaseSensor)
    REF_POINTER(ImiBaseFrame)
    REF_POINTER(ImiImageFrame)
    REF_POINTER(DeviceStateListener)
    // REF_POINTER(DeviceStateListener, IDeviceStateListener)
    REF_POINTER(ImiColorSensor)
    REF_POINTER(ImiColorFrame)
    REF_POINTER(ImiDepthSensor)
    REF_POINTER(ImiDepthFrame)
    REF_POINTER(ImiIRSensor)
    REF_POINTER(ImiIRFrame)
    REF_POINTER(ImiFaceDetectSensor)
    REF_POINTER(ImiFaceDetectFrame)
    REF_POINTER(ImiUserTrackSensor)
    REF_POINTER(ImiUserTrackFrame)
    REF_POINTER(ImiMoveTrackSensor)
    REF_POINTER(ImiMoveTrackFrame)
    REF_POINTER(ImiPositionTrackSensor)
    REF_POINTER(ImiPositionTrackFrame)
    REF_POINTER(ImiActionSensor)
    REF_POINTER(ImiActionFrame)
    REF_POINTER(ImiIndicatorSensor)
    REF_POINTER(ImiIndicatorFrame)

    /// @brief 获取MrAgent版本
    std::string getVersion();

    class ImiContext;

    /// @brief 与运行平台相关的上下文
    class AppContext
    {
        public:
            virtual ~AppContext() {}
            AppContext() {}

            friend class ImiContext;
    };

    /// @brief SDK运行环境
    class ImiContext
    {
        private:
            std::string _user_id = "";

            std::thread _event_loop_thread;

            AppContext *appContext;

            ImiBaseDevice _device;

            int _parseConfigFile(const char *configFilePath);

        public:
            ImiContext(AppContext *appContext);

            virtual ~ImiContext();

            enum LogLevel
            {
                LOG_DEBUG = 0,
                LOG_INFO = 1,
                LOG_ERROR = 2,
            };

            /// @brief 创建上下文
            /// @param appContext 平台APP的上下文, PC平台传null即可
            /// @return
            static ImiContext *create(AppContext *appContext);

            /// @brief 获取实例
            /// @return
            static ImiContext *getContext();

            /// @brief 初始化上下文
            /// @param 配置文件路径
            /// @return
            bool initialize(const char *configPath);
            void setRemoteAddress(const std::string address);

            /// @brief 释放上下文资源
            void release();

            /// @brief 创建设备
            /// @param deviceAttribue
            /// @return
            ImiBaseDevice createDevice();

            /// @brief 获取错误码
            /// @return
            int getLastError()
            {
                return 0;
            }
            /// @brief 获取错误码的描述
            /// @param errCode 错误码
            /// @return
            static std::string GetErrorString(int errCode)
            {
                return "";
            }

            /// @brief 设置Log
            /// @param config
            /// @return
            int setLogConfig(LogConfig config)
            {
                return 0;
            }

            friend class IImiBaseDevice;
            friend class IImiColorSensor;
            friend class IImiUserTrackSensor;
            friend class IImiActionSensor;
    };

    /// @brief 设备属性类的基类
    /// @ingroup1 imimrsdk
    class IImiDeviceProperty
    {
        protected:
            virtual const void *cPtr() const = 0;

        public:
            virtual ~IImiDeviceProperty() {}

            /// @brief 属性数据大小
            /// @return
            virtual int dataSize() const = 0;

            friend class IImiBaseDevice;
            friend class IImiBaseSensor;
    };

    enum class ColorResolutionMode
    {
        IMI_COLOR_480,
        IMI_COLOR_1080,
        IMI_COLOR_2K,
    };

    enum class MrManagerStatus
    {
        NORMAL,
        EXIT,
        BLOCK,
        UNKNOWN
    };

    enum class MrAgentException
    {
        IMI_MRMANAGER_EXIT,
        IMI_MRMANAGER_BLOCK
    };

    class MrAgentExceptionHandler
    {

        public:
            MrAgentExceptionHandler() {}
            virtual ~MrAgentExceptionHandler() {}
            virtual void onExceptionHandle(MrAgentException exception) {}
    };

    /// @brief IMI设备类
    class IImiBaseDevice
    {
        private:
            MrManagerStatus mrManagerStatus = MrManagerStatus::UNKNOWN;
            MrAgentExceptionHandler *exceptionHandler = nullptr;
            zmq::context_t cmd_ctx; // 需要保证先声明context
            void initClientCmdSocket ();
            bool sendMsg(cmd_data_native &cmd);
            std::string generateIdentity();
            void recvLoop();
            void getCommunicationWay ();
            bool isCmdClientRunning = true;
            std::thread *th_cmd_client;
            std::mutex send_mutex;

            zmq::socket_t cmd_client_socket;
            std::string client_addr = "tcp://localhost:5600";

            // 正在获取属性
            bool isGettingProperty = false;
            std::string identity;

        protected:
            std::map<ImiSensorType, ImiBaseSensor> allSensors;
            IImiBaseDevice();

        public:
            //        /// @brief 发送命令
            bool sendCommand(cmd_data_native cmd)
            {
                return sendMsg(cmd);
            }

            void releaseHeartBeatTh();

            void setOnExceptionHandler(MrAgentExceptionHandler *exceptionHandler);

            MrManagerStatus getMrManagerStatus();

            virtual ~IImiBaseDevice();

            /// @brief 获取Sensor
            /// @param sensorType   Sensor类型
            /// @return 失败时为空.
            virtual ImiBaseSensor getSensor(ImiSensorType sensorType);

            /// @brief 打开设备
            /// @param openFlag ImiSensorType的组合（用“或”操作组合）
            /// @return
            virtual int open(int openFlag);

            /// @brief 关闭设备
            /// @return
            virtual int close();

            /// @brief 获取SDK和固件版本
            /// @return
            virtual ImiVersions getVersions() const;

            /// @brief 设置Sensor属性
            /// @paramx propId   属性ID
            /// @paramx prop     属性数据
            /// @return 0:成功， 非零：失败.
            virtual int setProperty(int propId, IImiDeviceProperty *prop);

            /// @brief 获取Sensor属性
            /// @paramx propId   属性ID
            /// @paramx prop     属性数据
            /// @return 0:成功， 非零：失败.
            virtual int getProperty(int propId, IImiDeviceProperty *prop);

            virtual std::string getStringProperty(int propId);

            friend class ImiContext;
    };

    /// @brief IMI Sensor的基类
    class IImiBaseSensor
    {
        private:
            IImiBaseDevice *m_pDevice;

        protected:
            bool started = false;

            int width = 0;
            int height = 0;
            int depth = 0;

            ImiSensorType sensorType;

            IImiBaseSensor(ImiSensorType sensorType, IImiBaseDevice *pDevice);

            /// @brief 发送命令
            bool sendCommand(cmd_data_native cmd)
            {
                return m_pDevice ? m_pDevice->sendCommand(cmd) : false;
            }

            void releaseHeartbeatTh()
            {
                if (m_pDevice)
                {
                    m_pDevice->releaseHeartBeatTh();
                }
            }

        public:
            virtual ~IImiBaseSensor();

            /// @brief  Sensor类型
            /// @return
            ImiSensorType getSensorType() const
            {
                return sensorType;
            }

            /// @brief  启动Sensor
            /// @return
            virtual int start(int startFlags = 0);

            /// @brief  停止Sensor
            /// @return
            virtual int stop();

            /// @brief  释放Sensor的资源
            /// @return
            virtual int release();

            /// @brief
            /// @return
            virtual bool isStarted() const;

            /// @brief 读取数据帧.
            /// @param timeout  超时（ms）
            /// @return 失败时为空.
            virtual ImiBaseFrame readFrame(int timeout = 100);

            int getWidth() const
            {
                return width;
            }
            int getHeight() const
            {
                return height;
            }
            int getDepth() const
            {
                return depth;
            }

            void setWidth(int value)
            {
                width = value;
            }
            void setHeight(int value)
            {
                height = value;
            }
            void setDepth(int value)
            {
                depth = value;
            }
    };

    /// @brief Sensor数据帧
    class IImiBaseFrame
    {
        protected:
            int width = 0;
            int height = 0;
            int depth = 0;

            ImiSensorType sensorType;
            uint32_t pFrameNum = 0;
            uint64_t pTimestamp = 0;
            IImiBaseFrame(ImiSensorType sensorType) : sensorType(sensorType) {}

        public:
            virtual ~IImiBaseFrame() {}

            /// @brief Sensor类型
            /// @return
            ImiSensorType getSensorType() const
            {
                return sensorType;
            }

            /// @brief 帧ID
            /// @return
            virtual int getFrameNum() const
            {
                return pFrameNum ? (pFrameNum) : 0;
            }

            /// @brief 时间戳
            /// @return
            virtual long getTimeStamp() const
            {
                return pTimestamp ? (pTimestamp) : 0;
            };

            /// @brief 宽
            /// @return
            virtual int getWidth() const
            {
                return width;
            }

            /// @brief 高
            /// @return
            virtual int getHeight() const
            {
                return height;
            }

            /// @brief 深度
            /// @return
            virtual int getDepth() const
            {
                return depth;
            }
    };

    /// @brief 图像数据帧
    class IImiImageFrame : public IImiBaseFrame
    {
        protected:
            IImiImageFrame(ImiSensorType sensorType) : IImiBaseFrame(sensorType) {}

        public:
            virtual ~IImiImageFrame();

            /// @brief 数据格式
            /// @return
            virtual int getPixelFormat() const;

            /// @brief 帧率
            /// @return
            virtual int getFps() const;

            /// @brief 拷贝帧数据
            /// @param array_ref    目标
            /// @param array_len    数据长度
            virtual void getData(unsigned char *array_ref, int array_len) const {}

            /// @brief 数据大小
            /// @return
            virtual int getDataSize() const
            {
                return 0;
            }

            virtual unsigned long getDataPointer() const
            {
                return 0;
            }

            TO_STRING_OP(std::ostream &os, const IImiImageFrame &a)
            {
                return os << "Frame{size=" << a.getWidth() << "x" << a.getHeight() << ", fmt=" << a.getPixelFormat()
                          << ", fps=" << a.getFps() << "}";
            }
    };

    /// @brief 彩色图像帧
    class IImiColorFrame : public IImiImageFrame
    {
        private:
            unsigned char *_colorData = nullptr;
            int _colorDataSize = 0;
            int extraDataSize = 0;
            uint8_t *extraData = nullptr;

        public:
            IImiColorFrame(int dataSize);
            IImiColorFrame(int extraDataSize, uint8_t *extraData);

            virtual ~IImiColorFrame()
            {
                release();
            }

            /// @brief 类型转换
            /// @param p
            /// @return
            static ImiColorFrame asMe(const ImiBaseFrame &p)
            {
                return p && p->getSensorType() == ImiSensorType::COLOR ? std::dynamic_pointer_cast<IImiColorFrame>(p)
                                                                       : ImiColorFrame();
            }

            virtual void getData(unsigned char *array_ref, int array_len) const override;

            virtual int getDataSize() const override;

            virtual unsigned long getDataPointer() const override;

            void release();

            friend class IImiColorSensor;
    };

    /// @brief 彩色图Sensor
    class IImiColorSensor : public IImiBaseSensor
    {

        private:
            std::thread *_color_th = nullptr;
            std::mutex mutex;
            using SHMType = SharedMemoryDoubleBuffer;
            SHMType color_pool;
            JpegHwDecoder jpegDecoder;
            int mode{0}; // mode: 0 480P, !0 1080P

            void onRunColorData();

            void _runThread();
            void _terminateThread();

        public:
            explicit IImiColorSensor(IImiBaseDevice *pDevice) : IImiBaseSensor(ImiSensorType::COLOR, pDevice) {}
            virtual ~IImiColorSensor() {}

            /// @brief 类型转换
            /// @param p
            /// @return
            static ImiColorSensor asMe(const ImiBaseSensor &p)
            {
                return p && p->getSensorType() == ImiSensorType::COLOR ? std::dynamic_pointer_cast<IImiColorSensor>(p)
                                                                       : ImiColorSensor();
            }

            virtual int start(int mode = 0) override;

            virtual int stop() override;

            virtual ImiBaseFrame readFrame(int timeout = 100) override;

            friend class IImiBaseDevice;
            friend class ImiContext;
    };

    /// @brief 深度Sensor
    class IImiDepthSensor : public IImiBaseSensor
    {

        public:
            explicit IImiDepthSensor(IImiBaseDevice *pDevice) : IImiBaseSensor(ImiSensorType::DEPTH, pDevice) {}
            virtual ~IImiDepthSensor() {}

            /// @brief 类型转换.
            /// @param p
            /// @return
            static ImiDepthSensor asMe(const ImiBaseSensor &p)
            {
                return p && p->getSensorType() == ImiSensorType::DEPTH ? std::dynamic_pointer_cast<IImiDepthSensor>(p)
                                                                       : ImiDepthSensor();
            }

            TO_STRING_OP(std::ostream &os, const IImiDepthSensor &a)
            {
                return os << "ImiDepthSensor{id=" << std::hex << (&a) << "}";
            }

            friend class IImiBaseDevice;
    };

    /// @brief 深度图像帧
    class IImiDepthFrame : public IImiImageFrame
    {

        public:
            IImiDepthFrame() : IImiImageFrame(ImiSensorType::DEPTH) {}
            virtual ~IImiDepthFrame() {}

            static ImiDepthFrame asMe(const ImiBaseFrame &p)
            {
                return p && p->getSensorType() == ImiSensorType::DEPTH ? std::dynamic_pointer_cast<IImiDepthFrame>(p)
                                                                       : ImiDepthFrame();
            }

            void getRgbData(unsigned char *array_ref, int array_len) const;
    };

    /// @brief 红外Sensor
    class IImiIRSensor : public IImiBaseSensor
    {

        public:
            explicit IImiIRSensor(IImiBaseDevice *pDevice) : IImiBaseSensor(ImiSensorType::IR, pDevice) {}
            virtual ~IImiIRSensor() {}

            static ImiIRSensor asMe(const ImiBaseSensor &p)
            {
                return p && p->getSensorType() == ImiSensorType::IR ? std::dynamic_pointer_cast<IImiIRSensor>(p)
                                                                    : ImiIRSensor();
            }

            friend class IImiBaseDevice;
    };

    /// @brief 红外图像帧
    class IImiIRFrame : public IImiImageFrame
    {
            IImiIRFrame() : IImiImageFrame(ImiSensorType::IR) {}

        public:
            virtual ~IImiIRFrame() {}

            static ImiIRFrame asMe(const ImiBaseFrame &p)
            {
                return p && p->getSensorType() == ImiSensorType::IR ? std::dynamic_pointer_cast<IImiIRFrame>(p)
                                                                    : ImiIRFrame();
            }

            void getRgbData(unsigned char *array_ref, int array_len) const;
    };

    /// @brief 用户跟踪Sensor
    class IImiUserTrackSensor : public IImiBaseSensor
    {

        private:
            int reconnectIndex = 0;
            std::thread *_usertrack_th{nullptr};
            void onRunUserTrackData();

            void _runThread();
            void _terminateThread();
            using SHMType = SharedMemoryDoubleBuffer;
            SHMType foreground_pool;
            SHMType skeleton_pool;
            std::mutex mutex;

        public:
            explicit IImiUserTrackSensor(IImiBaseDevice *pDevice) : IImiBaseSensor(ImiSensorType::USERTRACK, pDevice) {}
            /// @brief 用户跟踪Sensor属性
            enum ImiUserTrackProperties
            {
                IMI_PROPERTY_USERTRACK_SKELETON_SMOOTH = 1, ///<  // value type uint8_t, 1:smooth, 0: not smooth
                IMI_PROPERTY_USERTRACK_GROUND_CLEANUP =
                    2, ///<  // value type uint8_t, 1:clear the ground, 0: don't clear the ground, default 0
                IMI_PROPERTY_USERTRACK_OUPUT_FOREGROUND =
                    3, ///<  // value type uint8_t, 1: 输出前景图，  0：不输出前景图
                IMI_PROPERTY_USERTRACK_OUPUT_SEG_MASK =
                    4, ///<  // value type uint8_t, 1: 输出抠图mask，0：不输出抠图mask
                IMI_PROPERTY_USERTRACK_GROUND_EQUATION = 5, ///<
                IMI_PROPERTY_USERTRACK_OUPUT_PSCTRL = 6,    ///<  //1 输出体感枪结果 0 ：不输出体感枪结果
            };

            /// @brief 算法模式。
            enum ImiUserTrackAlgorithmMode
            {
                IMI_TRACK_MODE_BODY = 0, ///< 身体骨架.
                IMI_TRACK_MODE_FACE = 1, ///<
                IMI_TRACK_MODE_HAND = 2, ///<
                IMI_TRACK_MODE_ECHO = 3, ///<
                IMI_TRACK_MODE_SEGM = 4, ///< 抠图.
                IMI_TRACK_MODE_RGBM = 5, ///< 彩色多人.
                IMI_TRACK_MODE_RGBO = 6, ///< 彩色单人.
            };

            virtual ~IImiUserTrackSensor() {}

            static ImiUserTrackSensor asMe(const ImiBaseSensor &p)
            {
                return p && p->getSensorType() == ImiSensorType::USERTRACK
                           ? std::dynamic_pointer_cast<IImiUserTrackSensor>(p)
                           : ImiUserTrackSensor();
            }

            /// @brief
            /// @param flags {@link #ImiUserTrackAlgorithmMode}
            /// @return
            virtual int start(int flags = 0) override; //

            virtual int stop() override;

            virtual ImiBaseFrame readFrame(int timeout = 100) override;

            friend class IImiBaseDevice;
            friend class IImiActionSensor;
            friend class ImiContext;
    };

    /// @brief 用户跟踪数据帧
    class IImiUserTrackFrame : public IImiBaseFrame
    {
        private:
            unsigned char *_foregroundData;
            size_t _foregroundDataSize = 0;

            ImiBodySkeleton _bodySkeleton = {};

        public:
            IImiUserTrackFrame(int fgSize);

            virtual ~IImiUserTrackFrame()
            {
                release();
            }

            void release();

            static ImiUserTrackFrame asMe(const ImiBaseFrame &p)
            {
                return p && p->getSensorType() == ImiSensorType::USERTRACK
                           ? std::dynamic_pointer_cast<IImiUserTrackFrame>(p)
                           : ImiUserTrackFrame();
            }

            /// @brief 获取身体骨架数据
            /// @return
            ImiBodySkeleton getBodySkeleton() const;

            bool getBodySkeleton(ImiBodySkeleton *sk) const;

            /// @brief 获取手骨架数据
            /// @return
            ImiHandSkeleton getHandSkeleton() const;

            /// @brief 获取背景图数据
            /// @return
            ImiForegroundData getForegroundInfo() const;

            /// @brief 获取前景图像数据.
            /// @param array_ref    数据
            /// @param array_len    大小
            virtual int getForegroundData(unsigned char *array_ref, int array_len) const;

            /// @brief 获取抠图数据
            /// @return
            ImiMattingData getMattingInfo() const;

            /// @brief 获取抠图图像数据.
            /// @param array_ref    数据
            /// @param array_len    大小： w*h*1
            virtual void getMattingData(unsigned char *array_ref, int array_len) const;

            /// @brief 获取用户人脸特征数据
            /// @return
            ImiFaceFeatureData getFaceFeatureFrame() const;

            int getForegroundRgbData(unsigned char *array_ref, int array_len, int mode) const;

            int getForegroundArgbData(unsigned char *array_ref, int array_len, int mode) const;

            int getBodySkeletonData(unsigned char *array_ref, int array_len) const;

            friend class IImiUserTrackSensor;
    };

    enum class ImiGestureType
    {
        IMI_GESTURE_NONE,

        IMI_GESTURE_LEFT_HAND_WAVE,  // 1 ���ֻӶ�
        IMI_GESTURE_RIGHT_HAND_WAVE, // 2 ���ֻӶ�

        IMI_GESTURE_LEFT_HAND_SWIPE_LEFT,   // 3 ���������
        IMI_GESTURE_LEFT_HAND_SWIPE_RIGHT,  // 4 �������һ�
        IMI_GESTURE_RIGHT_HAND_SWIPE_LEFT,  // 5 ���������
        IMI_GESTURE_RIGHT_HAND_SWIPE_RIGHT, // 6 �������һ�

        IMI_GESTURE_LEFT_HAND_PUSH,    // 7 ������ǰ��
        IMI_GESTURE_RIGHT_HAND_PUSH,   // 8 ������ǰ��
        IMI_GESTURE_LEFT_HAND_HOOKUP,  // 9 �����Ϲ�
        IMI_GESTURE_RIGHT_HAND_HOOKUP, // 10 �����Ϲ�

        IMI_GESTURE_LEAN_FORWARD,  // 11 ����ǰ��(������ͷ�ڷŽǶ�Ӱ��)
        IMI_GESTURE_LEAN_BACKWARD, // 12 �������(������ͷ�ڷŽǶ�Ӱ��)
        IMI_GESTURE_LEAN_LEFT,     // 13 ��������
        IMI_GESTURE_LEAN_RIGHT,    // 14 ��������

        IMI_GESTURE_JUMP,  // 15 ��Ծ����Ҫ���ټ�⵽ϥ�ǣ�
        IMI_GESTURE_SQUAT, // 16 ����
        IMI_GESTURE_WING,  // 17 ˫��ͬʱ���»Ӷ�
        IMI_GESTURE_RUN,   // 18 ˫��ǰ��ڶ�

        IMI_GESTURE_ZOOM_OUT, // 19 ��С
        IMI_GESTURE_ZOOM_IN,  // 20 �Ŵ�

        IMI_GESTURE_LEFT_HAND_OVER_HEAD,  // 21 ���־ٹ�ͷ�������ַ���
        IMI_GESTURE_RIGHT_HAND_OVER_HEAD, // 22 ���־ٹ�ͷ�������ַ���
        IMI_GESTURE_BOTH_HANDS_OVER_HEAD, // 23 ˫�־ٹ�ͷ��
        IMI_GESTURE_BOTH_HANDS_BACK,      // 24 ˫�ַ��������

        IMI_GESTURE_LEFT_ARM_PAUSE,    // 25 ��������屣��45��
        IMI_GESTURE_RIGHT_ARM_PAUSE,   // 26 �ұ������屣��45��
        IMI_GESTURE_LEFT_ARM_FORWARD,  // 27 ���ǰ��
        IMI_GESTURE_RIGHT_ARM_FORWARD, // 28 �ұ�ǰ��
        IMI_GESTURE_BOTH_ARMS_FORWARD, // 29 ˫��ǰ��

        IMI_GESTURE_LEFT_ARM_LATERAL_RAISE,              // 30 ��۲�ƽ�٣��ұ�����£�
        IMI_GESTURE_RIGHT_ARM_LATERAL_RAISE,             // 31 �ұ۲�ƽ�٣��������£�
        IMI_GESTURE_BOTH_ARMS_LATERAL_RAISE,             // 32 ˫�ֲ�ƽ��
        IMI_GESTURE_RIGHT_ARM_LATERAL_RAISE_LEFT_ARM_UP, // 33 �ұ۲�ƽ�٣�����Ͼ�
        IMI_GESTURE_RIGHT_ARM_UP_LEFT_ARM_LATERAL_RAISE, // 34 ��۲�ƽ�٣��ұ��Ͼ�

        IMI_GESTURE_LEFT_KNEE_KICK,  // 35 ��ϥ��
        IMI_GESTURE_RIGHT_KNEE_KICK, // 36 ��ϥ��
        IMI_GESTURE_LEFT_KNEE_LIFT,  // 37 ��ϥ̧�𣨾�̬��
        IMI_GESTURE_RIGHT_KNEE_LIFT, // 38 ��ϥ̧�𣨾�̬��

        IMI_GESTURE_LEFT_FOOT_KICK,  // 39 ����ߣ���Ҫ��⵽��ţ�
        IMI_GESTURE_RIGHT_FOOT_KICK, // 40 �ҽ��ߣ���Ҫ��⵽�ҽţ�

        IMI_GESTURE_BODY_MOVE_FORWARD,  // 41 ������ǰ��
        IMI_GESTURE_BODY_MOVE_BACKWARD, // 42 ����������
        IMI_GESTURE_BODY_MOVE_LEFT,     // 43 ����������
        IMI_GESTURE_BODY_MOVE_RIGHT,    // 44 ����������

        IMI_GESTURE_TENNIS_SWIPE_LEFT,     // 45 ������Ϸ�����
        IMI_GESTURE_TENNIS_SWIPE_RIGHT,    // 46 ������Ϸ���һ�
        IMI_GESTURE_APPLAUSE,              // 47 ���ƣ�˫�ַֿ����ϲ���
        IMI_GESTURE_BOWLING_THROW,         // 48 ����������
        IMI_GESTURE_DEFENSE,               // 49 ����(˫�ַ�����ǰ)
        IMI_GESTURE_DRIVE,                 // 50 ����
        IMI_GESTURE_RIGHT_HAND_THROW_DART, // 51 �����ӷ���
        IMI_GESTURE_BASEBALL_HITTING,      // 52 �������

        IMI_GESTURE_LEFT_FOOT_STOMP,            // 53 �����̤����Ҫ��⵽��ţ�
        IMI_GESTURE_RIGHT_FOOT_STOMP,           // 54 �ҽ���̤����Ҫ��⵽�ҽţ�
        IMI_GESTURE_LEFT_FOOT_SIDE_KICK,        // 55 ��Ų��ߣ���Ҫ��⵽��ţ�
        IMI_GESTURE_RIGHT_FOOT_SIDE_KICK,       // 56 �ҽŲ��ߣ���Ҫ��⵽�ҽţ�
        IMI_GESTURE_SHAKE_FISTS,                // 57 ������ȭ(�����Ⱦٹ��Ҽ磬�ٴ������)
        IMI_GESTURE_GAME_SQUAT,                 // 58 ��Ϸ������߿ա���������
        IMI_GESTURE_LEFT_ARM_135_DEGREE,        // 59 ���̧��135�� ���������������ƽ�У�
        IMI_GESTURE_RIGHT_ARM_135_DEGREE,       // 60 �ұ�̧��135�� �������ұ�������ƽ�У�
        IMI_GESTURE_LEFT_ARM_135_TO_90_DEGREE,  // 61 ��۴�135�Ȼ���90�ȣ��������������ƽ�У�,��̬����
        IMI_GESTURE_RIGHT_ARM_135_TO_90_DEGREE, // 62 �ұ۴�135�Ȼ���90�ȣ������ұ�������ƽ�У�,��̬����
        IMI_GESTURE_LEFT_ARM_45_TO_90_DEGREE,   //  63 ��۴�45�Ȼ���90�ȣ��������������ƽ�У�,��̬����
        IMI_GESTURE_RIGHT_ARM_45_TO_90_DEGREE,  // 64 �ұ۴�45�Ȼ���90�ȣ������ұ�������ƽ�У�,��̬����
        IMI_GESTURE_COUNT
    };

    struct DataUtil
    {
            static ImiBodySkeleton createImiBodySkeleton()
            {
                return ImiBodySkeleton();
            }
    };

    /// @brief 基本属性类型
    /// @tparam T
    template <typename T> class IBaseProperty : public IImiDeviceProperty
    {
        protected:
            T data;
            virtual const void *cPtr() const override
            {
                return &data;
            }

        public:
            IBaseProperty() {}
            IBaseProperty(T v)
            {
                data = v;
            }
            virtual ~IBaseProperty() {}

            /// @brief 设置值
            /// @param val
            void set(T val)
            {
                data = val;
            }

            /// @brief 获取值
            /// @return
            T get() const
            {
                return data;
            }

            int dataSize() const override
            {
                return sizeof(T);
            }
    };

    typedef IBaseProperty<int> IInt32Property;
    typedef IBaseProperty<long> IInt64Property;
    typedef IBaseProperty<short> IInt16Property;
    typedef IBaseProperty<float> IFloatProperty;
    typedef IBaseProperty<unsigned char> IInt8Property;
    typedef IBaseProperty<imimr::ImiCameraIntrinsic> IImiCameraIntrinsicProperty;

} // namespace imimr