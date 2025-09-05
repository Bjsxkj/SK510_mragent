
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <stdint.h>
#include <cstdint>
#include <cassert>
#include <thread>
#include <list>

#include "ImiMrTypes.h"

#include "ImiMrUtil.h"
#include "ImiUpgrade.h"


#define API_VERSION_major 1
#define API_VERSION_minor 4
#define API_VERSION_revision 8
#define _STR(x) #x

#define API_VERSION_CON(x,y,z) _STR(x) "." _STR(y) "." _STR(z)

#define IMI_MR_API_VERSION API_VERSION_CON(API_VERSION_major,API_VERSION_minor,API_VERSION_revision)


struct ImiDevice;
struct ImiSensor;
// struct ImiRecord;
// struct ImiImageFrame;
struct ImiStream;
struct ImiCamera;
struct ImiImageFrame;

typedef ::ImiDevice* ImiDeviceHandle;
typedef ::ImiCamera* ImiCameraHandle;
typedef ::ImiSensor* ImiSensorHandle;
typedef ::ImiStream* ImiStreamHandle;
// typedef ::ImiRecord* ImiRecordHandle;
typedef ::ImiImageFrame*  ImiImageFrameHandle;

#define REF_POINTER(ref_type)  class I ## ref_type; \
typedef std::shared_ptr< I ## ref_type > ref_type;

#define REF_POINTER0(ref_type) typedef std::shared_ptr< I ## ref_type > ref_type;


struct DataBuffer{
    uint8_t* data;
    size_t len;
};

#include <functional>

struct MutableDataBuffer{
    uint8_t* data;
    size_t len;
    function<void ()> apply;

    ~MutableDataBuffer(){
        if(apply){
            apply();
        }
    }
};

namespace imimr {

    // class IImiDevice;
    // class IImiBaseSensor;
    // class IImiFrame;
    // class IDeviceStateListener;

    // class IImiColorSensor;
    // class IImiDepthSensor;
    // class IImiIRSensor;
    // class IImiFaceDetectSensor;

    // template<typename T>
    // class ImiSmartPointer {

    // };



    REF_POINTER(ImiBaseDevice)
    REF_POINTER(ImiBaseSensor)
    REF_POINTER(ImiBaseFrame)
    REF_POINTER(ImiImageFrame)
    REF_POINTER(DeviceStateListener)
    //REF_POINTER(DeviceStateListener, IDeviceStateListener)
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
    REF_POINTER(UpgradeStateListener)
    REF_POINTER(NewFrameListener)

    //
    // REF_POINTER()

    REF_POINTER(ImiDeviceProperty)
    REF_POINTER(StringProperty)

    class Wrapper;

    

    
    class ImiContext;

    /// @brief 与运行平台相关的上下文
    /// @defgroup imimrsdk
    /// @ingroup1 imimrsdk
    /// @see OK
    class AppContext{
        void* handleUsbManager = 0;
        void* runtime = 0;
    public:
        virtual ~AppContext();
        AppContext(){}
#if 1 //def USE_ANDROID_API
       // AppContext(void* handleUsbManager, void* runtime); //: handleUsbManager(handleUsbManager), runtime(runtime) {}
        
        /// @brief 获取USB列表
        /// @param flags 
        /// @return 
        virtual std::vector<ImiDeviceAttribute> getUsbList(int flags) { return std::vector<ImiDeviceAttribute>(); }

        /// @brief 打开USB设备
        /// @param devAttribute 
        /// @return 
        virtual long openUsbHandle(ImiDeviceAttribute devAttribute) { return -1; } 

        /// @brief 关闭USB
        /// @param handle 
        /// @return 
        virtual long closeUsbHandle(long handle) { return 0; } 

        virtual bool addDeviceStateListener(DeviceStateListener listener){ return true;}

        virtual bool removeDeviceStateListener(DeviceStateListener listener){ return true;}

#endif
        virtual unsigned long getUsbManagerHandle() { return 0; }

        friend class ImiContext;
    };

    /// @brief SDK运行环境
    /// @ingroup1 imimrsdk 
    class ImiContext
    {
    private:
        /* data */
        ImiContext(AppContext* appContext);
        AppContext* appContext;

        int _fd = -1;

        DeviceStateListener _deviceStateListener;


        // static void onImiDeviceStateCallback(const char* deviceUri, ::ImiDeviceState deviceState, void* pUserData);

    public:

        virtual ~ImiContext();

        /// @brief 创建全局的上下文
        /// @paramx appContext 平台APP的上下文, PC平台传null即可
        /// @return 
        static ImiContext* create(AppContext* appContext);

        /// @brief 获取实例
        /// @return 
        static ImiContext* getContext();

        /// @brief 销毁上下文
        static void destroy();

        /// @brief 设置USB状态监听，注意全局只能设置一个
        /// @paramx listener 
        /// @return 
        bool addDeviceStateListener(DeviceStateListener listener);

        /// @brief 消除USB状态监听
        /// @paramx listener 
        /// @return 
        bool removeDeviceStateListener(DeviceStateListener listener);

        /// @brief 获取IMI设备列表
        /// @paramx bOpenUVC 
        /// @return 
        virtual std::vector<ImiDeviceAttribute> getDeviceList(bool bOpenUVC = true);

        /// @brief 创建设备
        /// @paramx deviceAttribue 
        /// @return 
        ImiBaseDevice createDevice(ImiDeviceAttribute deviceAttribue);

        /// @brief 
        void destroyDevice();

        /// @brief 获取错误码
        /// @return 
        int getLastError() { return 0;}

        /// @brief 获取错误码的描述
        /// @paramx errCode 错误码
        /// @return 
        static std::string GetErrorString(int errCode){ return ""; }

        /// @brief 设置Log
        /// @paramx config 
        /// @return 
        int setLogConfig(LogConfig config){ return 0;}

    };

    /// @brief USB连接状态监听.
    /// @ingroup1 imimrsdk
    class IDeviceStateListener {
    public:

        /// @brief USB连接状态改变时.
        /// @paramx state        状态
        /// @paramx deviceInfo   设备信息
        virtual void onDeviceStateChanged(ImiDeviceState state, std::string deviceInfo) {};
    };

    // class IOnDeviceStateCallBack {

    // };

    /// @brief 设备属性类的基类
    /// @ingroup1 imimrsdk
    class IImiDeviceProperty {
    protected:
        virtual const void* cPtr() const = 0;
    public:
        virtual ~IImiDeviceProperty(){}
        
        /// @brief 属性数据大小
        /// @return 
        virtual int dataSize() const = 0;
        bool getDataBuffer(MutableDataBuffer outData){
            int len = dataSize();
            if (outData.len < len)
            {
                return false;
            }

            memcpy(outData.data, (char *)cPtr(), len);
            return true;
        }

        friend class IImiBaseDevice;
        friend class IImiBaseSensor;
    };

    /// @brief 基本属性类型
    /// @tparam T 
    template<typename T>
    class IBaseProperty : public IImiDeviceProperty {
    protected:
        T data;
        virtual const void* cPtr() const override { return &data; }
    public:
        IBaseProperty(){}
        IBaseProperty(T v){ data = v; }
        virtual ~IBaseProperty(){}
        
        /// @brief 设置值
        /// @param val 
        void set(T val){ data = val; }

        /// @brief 获取值
        /// @return 
        T get() const { return data; }

        int dataSize() const override { return sizeof(T); }
    };


    DEF_TEMPLATE_CLASS(IInt32Property,IBaseProperty<int>)
    DEF_TEMPLATE_CLASS(IInt64Property,IBaseProperty<long>)
    DEF_TEMPLATE_CLASS(IInt16Property,IBaseProperty<short>)
    DEF_TEMPLATE_CLASS(IInt8Property,IBaseProperty<unsigned char>)
    DEF_TEMPLATE_CLASS(IFloatProperty,IBaseProperty<float>)
    DEF_TEMPLATE_CLASS(IDoubleProperty,IBaseProperty<double>)
    DEF_TEMPLATE_CLASS(IImiCameraIntrinsicProperty,IBaseProperty<imimr::ImiCameraIntrinsic>)
    DEF_TEMPLATE_CLASS(IImiImuDataProperty, IBaseProperty<imimr::ImiSkImuData>)

    // typedef IBaseProperty<int>   IInt32Property;
    // typedef IBaseProperty<long>  IInt64Property;
    // typedef IBaseProperty<short> IInt16Property;
    // typedef IBaseProperty<float> IFloatProperty;
    // typedef IBaseProperty<unsigned char> IInt8Property;
    // typedef IBaseProperty<imimr::ImiCameraIntrinsic> IImiCameraIntrinsicProperty;

    // #define DEF_PROPERTY1(name, T)  class name : public IImiDeviceProperty { \
    // protected: \
    //     T data; \
    //     virtual const void* cPtr() const override { return &data; } \
    // public: \
    //     name() {} \
    //     name(T val){ data = val; } \
    //     void set(T val){ data = val; } \
    //     T get() const { return data; } \
    //     int dataSize() const override { return sizeof(T); } \
    // };

    // DEF_PROPERTY1(IInt32Property,int)
    // DEF_PROPERTY1(IInt64Property,long)
    // DEF_PROPERTY1(IInt16Property,short)
    // DEF_PROPERTY1(IFloatProperty,float)
    // DEF_PROPERTY1(IInt8Property,unsigned char)
    // DEF_PROPERTY(IImiCameraIntrinsicProperty, imimr::ImiCameraIntrinsic)




    /// @brief USB连接状态监听.
    class IUpgradeStateListener {
    private:
        ImiUpgradeCallbacks _cb;

        static void _upgradeCallback(IMI_UPGRADE_PROMPT promptID, float progress, IMI_UPGRADE_ERRCODE errCode, const char* errMsg, void* pUserData);

    public:
        IUpgradeStateListener();

        virtual ~IUpgradeStateListener() {}

        /// @brief device升级进度回调.
        /// @param promptID        
        /// @param percentage   
        virtual void onProgress(IMI_UPGRADE_PROMPT promptID, float percentage, const char* msg) { }

        /// @brief device升级进度回调.
        /// @param promptID        
        /// @param percentage   
        virtual void onError(IMI_UPGRADE_PROMPT promptID, IMI_UPGRADE_ERRCODE errCode, const char* errMsg) { }

        friend class IImiBaseDevice;
    };

    /// @brief IMI设备类
    /// @ingroup1 imimrsdk
    class IImiBaseDevice {
    protected:
        ImiDeviceAttribute devAttr;
        ImiDeviceHandle hDevice;
        int fd = -1;
        //ImiCameraHandle hCamDevice;
        std::map<int, ImiBaseSensor> allSensors;

        int openInternal(int openFlag); 

    public:

        /// @brief 设备属性ID
        /// @ingroup1 imimrsdk
        enum ImiDeviceProperties
        {
            IMI_PROPERTY_LD_OPERATE             = 0,
            IMI_PROPERTY_LASER_SAFETY_MODE      = 1,   
            IMI_PROPERTY_SERIAL_NUMBER          = 2, //value uint8_t serialNo[32]; string类型，32 bytes
            IMI_PROPERTY_RESET_DEVICE           = 3, //value type uint8_t, 1 reset Device
            IMI_PROPERTY_CHIP_REGISTRATION      = 4,
            IMI_PROPERTY_SAFETY_DIST            = 5,
            IMI_PROPERTY_LIGHT_THRESHOLD        = 6,
            IMI_PROPERTY_AMBIENT_LIGHT_MODE     = 7,
            IMI_PROPERTY_REAL_SAFETY_DIST       = 8,
            IMI_PROPERTY_REAL_LIGHT_THRESHOLD   = 9,
            IMI_PROPERTY_DEVICE_SWITCH_POWER    = 10, //valuie type uint8_t, 1 :power is 1.3A   0: power is 0.8A   default: 0
            IMI_PROPERTY_DEVICE_VERSION         = 11, //value uint8_t serialNo[32];
            IMI_PROPERTY_KEY_VERSION            = 12,  //value uint8_t serialNo[32];
            IMI_PROPERTY_SAFETYCHIP_VERSION     = 13,   //value  uint8_t  verison[6]
            IMI_PROPERTY_SET_SERIAL_NUMBER		= 14,
            IMI_PROPERTY_GET_CHIP_VERSION		= 0x96e,
            IMI_PROPERTY_ENCRYPTION_SN			= 0x710,
            IMI_PROPERTY_GET_SN					= 0x200,
            IMI_PROPERTY_GET_STATISTICS			= 0xa572,
            IMI_PROPERTY_GET_FW_INFO			= 0x256,
            IMI_PROPERTY_GET_AP_FW_VERSION      = 0xa570,
            IMI_PROPERTY_GET_AP_VERSION			= IMI_PROPERTY_GET_AP_FW_VERSION,
            IMI_PROPERTY_GET_SAFE_FW_VERSION    = 0x96f,
            IMI_PROPERTY_SET_FOCUE				= 0x263,
            IMI_PROPERTY_ISP_AF_SWITCH			= 0x264,
            IMI_PROPERTY_GET_LICENSE_RESULT		= 0x712,
            IMI_PROPERTY_GET_HARDWARE_VERSION   = 0xa582,
            IMI_PROPERTY_GET_FPGA_VERSION		= 0xa581,
            IMI_PROPERTY_GET_SAFE_DISTANCE		= 0x200,
            IMI_PROPERTY_SAFE_DISTANCE			= 0x201,
            IMI_PROPERTY_SET_AUTOEXP_CFG		= 0x7e4,
            IMI_PROPERTY_NN_MOUDLE				= 0x205,
            IMI_PROPERTY_NN_THRESHOLD			= 0x206,
            IMI_PROPERTY_GET_DEVICE_READY_STATUS= 0xa571, //check device ready
            IMI_PROPERTY_DPU_LIGHT_MODE         = 0xa580, //0: flood, 1$)A#:dot
            
            IMI_PROPERTY_GET_PSENDOR_DATA       = 0xa660, //read data
            IMI_PROPERTY_SET_PSENDOR_THRESHOLD       = 0xa661, //write data as threshold
            IMI_PROPERTY_PSENDOR_SWITCH         = 0xa662, //PSensor switch 0:close 1:open
            IMI_PROPERTY_GET_PSENDOR_THRESHOLD       = 0xa663, //get threshold
            IMI_PROPERTY_GET_PSENDOR_SWITCH       = 0xa664, //get PSensor switch
            IMI_PROPERTY_DIAG_PARAM             = 0xa760,
            IMI_PROPERTY_GET_BULK_LOG           = 0xa761,
            IMI_PROPERTY_GET_CAPABLITIES        = 0xa860,
            IMI_PROPERTY_GET_PRODUCT_INFO       = 0xa861,
            IMI_PROPERTY_SET_DYNAMIC_REGISTRATION_SWITCH = 0xa862,  //dynamic registration switch 0:close 1:open  ????
            IMI_PROPERTY_GET_DYNAMIC_REGISTRATION_SWITCH = 0xa863,  //??????
            IMI_PROPERTY_GET_EVB_ADC            = 0xa960,            //??evb A100m ?????adc????
            IMI_PROPERTY_GET_TMP				= 0xaa60,
            IMI_PROPERTY_GET_TMP_2nd            = 0xaa70,
            IMI_PROPERTY_PARAM_TEMPERATURE		= 0x203,
            IMI_PROPERTY_CALIBRATION_IMU		= 0xa870,			//add imu calibration value r & w
            IMI_PROPERTY_SET_IMU_STUTE			= 0xa871			//add imu state for muyuan 1 bool for stute 3 float for  coordinate
            
        };

        /// @hide
        virtual ~IImiBaseDevice();

        /// @brief 获取Sensor
        /// @paramx sensorType   Sensor类型
        /// @return 失败时为空.
        virtual ImiBaseSensor getSensor(ImiSensorType sensorType);

        /// @brief 打开设备
        /// @paramx openFlag ImiSensorType的组合（用“或”操作组合）
        /// @return 0:成功， 非零：失败.
        virtual int open(int openFlag); 

        /// @brief 关闭设备
        /// @return 0:成功， 非零：失败.
        virtual int close();

        /// @brief 获取SDK和固件版本
        /// @return 版本信息
        virtual ImiVersions getVersions() const;

        /// @brief 获取USB设备属性
        /// @return 
        virtual ImiDeviceAttribute getAttribute() const;

        /// @brief 获取IMI设备属性
        /// @paramx propId   属性ID
        /// @paramx prop     属性数据
        /// @return 0:成功， 非零：失败.
        virtual int setProperty(int propId, IImiDeviceProperty* prop);

        /// @brief 设置IMI设备属性
        /// @paramx propId   属性ID
        /// @paramx prop     属性数据
        /// @return 0:成功， 非零：失败.
        virtual int getProperty(int propId, IImiDeviceProperty* prop);

        //virtual ImiDeviceProperty getProperty(int propId) { return ImiDeviceProperty(); }

        /// @brief 升级IMI设备
        /// @param propId   属性ID
        /// @param prop     属性数据
        /// @return 
        int upgrade(std::string path, std::string version, UpgradeStateListener &listener);

        friend class ImiContext;

    };
    
    /// 帧模式
    struct IImiFrameMode {
        int32_t        format;
        int16_t        resolutionX;
        int16_t        resolutionY;
        int8_t         bitsPerPixel;
        int8_t         framerate;

        TO_STRING_OP(ostream& os, const IImiFrameMode& a){
            return os << "{format="<<a.format<<", resolution:\""<<a.resolutionX <<"x"<<a.resolutionY<<"\", bitsPerPixel:" << (int)a.bitsPerPixel <<",framerate:"<<(int)a.framerate<<"}";
        }
    };

    /// @brief 
    enum class ImiSensorStartFlag {
        START_COLOR_640x480 = 1,    //
        START_COLOR_1280x720 = 2,   //
        START_COLOR_1920x1080 = 3,  //
    };

    /// @brief 监听新数据帧.
    class INewFrameListener {
        //using NewFrameCallback = void(*)(ImiBaseFrame frame);
    public:
        INewFrameListener(){}
        ~INewFrameListener(){}

        /// @brief 新数据帧回调
        /// @param frame 
        virtual void onNewFrame(ImiBaseFrame frame) {  printf("new frame:%p\n", frame.get()); }

    };

    
    /// @brief IMI Sensor的基类
    /// @ingroup1 imimrsdk
    class IImiBaseSensor {
    protected:
        ImiSensorType sensorType;
        IImiBaseSensor(ImiSensorType sensorType); 
        IImiBaseDevice* device;
        ImiSensorHandle hSensor;
        int typeIndex;

        bool bIsStarted = false;

        IImiFrameMode frameMode;
        
        INewFrameListener* _cb = nullptr;

        friend class Wrapper;

        void _onRecvNewFrame(ImiBaseFrame frame);

        ImiBaseFrame _curframeList;
        bool _isUpdated = 0;
        bool _isRunning = 0;
        static void _onGetFrame(IImiBaseSensor* sensor);
        std::thread* _rdThread;

    public:
        virtual ~IImiBaseSensor(){}

        /// @brief 设置数据帧模式，在`start()`/之前设置
        /// @param mode 
        void setFrameMode(IImiFrameMode* mode);

        /// @brief  Sensor类型
        /// @return 
        ImiSensorType getSensorType() const { return sensorType; }

        /// @brief  启动Sensor
        /// @return 0:成功， 非零：失败.
        virtual int start(int startFlags = 0); 

        /// @brief  停止Sensor
        /// @return 0:成功， 非零：失败.
        virtual int stop();

        /// @brief  释放Sensor的资源
        /// @return 0:成功， 非零：失败.
        virtual int release();

        /// @brief  判断Sensor是否已经启动
        /// @return true:已经启动， false:未启动.
        virtual bool isStarted() const;

        // /// @brief 
        // /// @return 
        // uint64_t getSensorHandle() const { return 0; }

        /// @brief 读取数据帧.
        /// @paramx timeout  超时（ms）
        /// @return 失败时为空.
        virtual ImiBaseFrame readFrame(int timeout=100);

        /// @brief 设置Sensor属性
        /// @paramx propId   属性ID
        /// @paramx prop     属性数据
        /// @return 0:成功， 非零：失败.
        virtual int setProperty(int propId, IImiDeviceProperty* prop);

        /// @brief 获取Sensor属性
        /// @paramx propId   属性ID
        /// @paramx prop     属性数据
        /// @return 0:成功， 非零：失败.
        virtual int getProperty(int propId, IImiDeviceProperty* prop) const;

        // // @brief 设置新帧回调
        virtual void setOnNewFrameCallback(INewFrameListener* cb) { _cb = cb; }
        
        //virtual ImiDeviceProperty getProperty(int propId) { return ImiDeviceProperty(); }
        // TO_STRING_OP(std::ostream& os, const IImiBaseSensor& a){
        //     return os<<"ImiBaseSensor{type="<< (int)a.sensorType<<", id="<<std::hex<<(&a)<<"}";
        // }

        friend class IImiBaseDevice;

    };

    /// @brief Sensor数据帧
    /// @ingroup1 imimrsdk
    class IImiBaseFrame {
    protected:
        ImiSensorType sensorType;
        uint32_t* pFrameNum = 0;
        uint64_t* pTimestamp = 0;
        IImiBaseFrame(ImiSensorType sensorType):sensorType(sensorType){}

        virtual void onRelease() {}

    public:
        virtual ~IImiBaseFrame(){ onRelease(); }

        void release() { onRelease(); }

        /// @brief Sensor类型
        /// @return 
        ImiSensorType getSensorType() const;

        /// @brief 帧ID
        /// @return 
        virtual int getFrameNum() const { return pFrameNum?(*pFrameNum):0; }

        /// @brief 时间戳
        /// @return 
        virtual long getTimeStamp() const { return pTimestamp? (*pTimestamp): 0; };


    };

    /// @brief 图像数据帧
    /// @ingroup1 imimrsdk
    class IImiImageFrame :public IImiBaseFrame{
    protected:
        ImiImageFrameHandle hFrame;
        int typeIndex;
        int ownHandle = 0;
        IImiImageFrame(ImiSensorType sensorType, ImiImageFrameHandle hFrame, int ownHandle = 0);

        /// @brief 释放资源
        /// @return 
        virtual void onRelease() override;

        friend class Wrapper;

    public:
        
        virtual ~IImiImageFrame() {
            release();
        }

        

        // /// @brief 
        // /// @return 
        // virtual int getFrameNum() const override;

        // /// @brief 
        // /// @return 
        // virtual long getTimeStamp() const override;

        /// @brief 宽
        /// @return 
        virtual int getWidth() const;

        /// @brief 高
        /// @return 
        virtual int getHeight() const;

        /// @brief 数据大小
        /// @return 
        virtual int getDataSize() const;

        /// @brief 数据格式
        /// @return 
        virtual int getPixelFormat() const;

        /// @brief 帧率
        /// @return 
        virtual int getFps() const;

        /// @brief 拷贝帧数据
        /// @paramx array_ref    目标
        /// @paramx array_len    数据长度
        virtual void getData(unsigned char* array_ref, int array_len) const;

        TO_STRING_OP(std::ostream& os, const IImiImageFrame& a){
            return os<<"Frame{size="<<a.getWidth()<<"x"<<a.getHeight()<<", fmt="<<a.getPixelFormat()<<", fps="<<a.getFps()<<"}";
        }

   
    };




    /// @brief 彩色图Sensor
    /// @ingroup1 imimrsdk
    class IImiColorSensor : public IImiBaseSensor { 
        IImiColorSensor() :  IImiBaseSensor(ImiSensorType::COLOR) { printf("color new\n"); } 
    public:

        /// @brief 
        enum ImiColorProperties
        {
            IMI_PROPERTY_COLOR_MIRROR           = 0,    ///< 
            IMI_PROPERTY_COLOR_INTRINSIC_PARAMS = 1,    ///< 
            IMI_PROPERTY_COLOR_SCAN_MODE       = 11,    ///< 扫描模式 UInt8  //value uint8_t , 0$)A#:K"A3  1#:I(Bk

            IMI_PROPERTY_COLOR_AE_MODE       = 0x260,   ///< 
            IMI_PROPERTY_COLOR_EXPOSURE_UP   = 0x379,   ///< 
            IMI_PROPERTY_COLOR_EXPOSURE_DOWN = 0x37a    ///< 
        };

        /// @brief 
        enum ExposureMode {
            IMI_COLOR_EXPOSURE_INVALID  = -1,     ///< 
            IMI_COLOR_EXPOSURE_AUTO     = 0x00,   ///< 
            IMI_COLOR_EXPOSURE_MANUAL   = 0x01,   ///< 
        };

        /// @brief 
        enum BackLightMode {
            IMI_COLOR_BACK_LIGHT_INVALID    = -1,   ///< 
            IMI_COLOR_BACK_LIGHT_FACE       = 0x01, ///< 
            IMI_COLOR_BACK_LIGHT_SCAN       = 0x02, ///< 
        };

        virtual ~IImiColorSensor(){ printf("color del\n"); }

        /// @brief 类型转换
        /// @paramx p 
        /// @return 
        static ImiColorSensor asMe(const ImiBaseSensor& p){ return p->getSensorType()==ImiSensorType::COLOR? std::dynamic_pointer_cast<IImiColorSensor>(p) : ImiColorSensor(); }

        //virtual int start() override { printf("color start"); return 0; }  

        // ImiCameraIntrinsic getCameraIntrinsicParams() const;

        int setMirror(bool bEnabled){ return 0;}

        /// @brief
        BASE_PROPERTY_GET(uint8_t, ScanMode, 0, IMI_PROPERTY_COLOR_SCAN_MODE)

        /// @brief
        BASE_PROPERTY_SET(uint8_t, ScanMode, -1, IMI_PROPERTY_COLOR_SCAN_MODE)

        /// @brief
        BASE_PROPERTY_GET(int, ExposureMode, -1, IMI_PROPERTY_COLOR_AE_MODE)

        /// @brief
        BASE_PROPERTY_SET(int, ExposureMode, -1, IMI_PROPERTY_COLOR_AE_MODE)

        /// @brief 获取内参.
        BASE_PROPERTY_GET(imimr::ImiCameraIntrinsic, CameraIntrinsicParams, imimr::ImiCameraIntrinsic(), IMI_PROPERTY_COLOR_INTRINSIC_PARAMS)

        

        // TO_STRING_OP(std::ostream& os, const IImiColorSensor& a){
        //     return os << "ImiColorSensor{id="<<std::hex<<(&a)<<", handle="<<a.hSensor<<"}";
        // }

        friend class IImiBaseDevice;
    };

    /// @brief 彩色图像帧
    /// @ingroup1 imimrsdk
    class IImiColorFrame: public IImiImageFrame {
    public:
        IImiColorFrame(ImiImageFrameHandle hFrame): IImiImageFrame(ImiSensorType::COLOR, hFrame) {}
    
        /// @brief 类型转换
        /// @paramx p 
        /// @return 
        static ImiColorFrame asMe(const ImiBaseFrame& p){ return p->getSensorType()==ImiSensorType::COLOR? std::dynamic_pointer_cast<IImiColorFrame>(p) : ImiColorFrame(); }

        virtual ~IImiColorFrame(){}

        long long getFrameHandle() const;

        friend class IImiColorSensor;
        friend class IImiColorSensor;
    };

    /// @brief 深度Sensor
    /// @ingroup1 imimrsdk
    class IImiDepthSensor : public IImiBaseSensor {
        IImiDepthSensor() : IImiBaseSensor(ImiSensorType::DEPTH) { } 
    public:

        /// @brief 深度Sensor属性
        /// @ingroup1 imimrsdk
        enum ImiDepthProperties
        {
            IMI_PROPERTY_DEPTH_HOLE_FILTER           = 0,   ///< 去小块.
            IMI_PROPERTY_DEPTH_MIRROR                = 1,   ///< 镜像.
            IMI_PROPERTY_DEPTH_DENOISE               = 2,   ///< 平滑..
            IMI_PROPERTY_DEPTH_INTRINSIC_PARAMS      = 3,   ///< 
            IMI_PROPERTY_DEPTH_REGISTRATION          = 4,   ///< 3图配准.
            IMI_PROPERTY_DEPTH_SKELETON_REGISTRATION = 5,   ///< 
            IMI_PROPERTY_DEPTH_UVC_SYNC              = 6,   ///< 

            IMI_PROPERTY_DEPTH_EXPOSURE_TIME = 0x414,   ///< 
            IMI_PROPERTY_DEPTH_FIGURE_PLUS   = 0x415,   ///< 
            IMI_PROPERTY_DEPTH_SIMULATE_PLUS = 0x416,   ///< 
            IMI_PROPERTY_DEPTH_LIGHT_CURRENT = 0x417,   ///< 
        };



        virtual ~IImiDepthSensor(){}

        /// @brief 类型转换.
        /// @paramx p 
        /// @return 
        static ImiDepthSensor asMe(const ImiBaseSensor& p){ return p->getSensorType()==ImiSensorType::DEPTH? std::dynamic_pointer_cast<IImiDepthSensor>(p) : ImiDepthSensor(); }

       // virtual int start() override { return 0; }  

    //    ImiCameraIntrinsic getCameraIntrinsicParams() const;

        int setMirror(bool bEnabled){ return 0;}

        /// @brief 去小块.
        BOOL_PROPERTY_GET(uint8_t, HoleFilter, 0, IMI_PROPERTY_DEPTH_HOLE_FILTER)

        /// @brief 去小块.
        BOOL_PROPERTY_SET(uint8_t, HoleFilter, -1, IMI_PROPERTY_DEPTH_HOLE_FILTER)

        /// @brief 平滑.
        BOOL_PROPERTY_GET(uint8_t, Denoise, 0, IMI_PROPERTY_DEPTH_DENOISE)

        /// @brief 平滑.
        BOOL_PROPERTY_SET(uint8_t, Denoise, -1, IMI_PROPERTY_DEPTH_DENOISE)

         /// @brief 获取内参.
        BASE_PROPERTY_GET(imimr::ImiCameraIntrinsic, CameraIntrinsicParams, imimr::ImiCameraIntrinsic(), IMI_PROPERTY_DEPTH_INTRINSIC_PARAMS)


        // int setHoleFilter(bool enabled){  return 0;}

        // bool isHoleFilterEnabled(){ return 0; }

        // int setDenoise(bool bEnabled){ return 0; }

        // bool isDenoiseEnabled(){ return 0; }


        TO_STRING_OP(std::ostream& os, const IImiDepthSensor& a){
            return os << "ImiDepthSensor{id="<<std::hex<<(&a)<<", handle="<<a.hSensor<<"}";
        }


        friend class IImiBaseDevice;
    };

    /// @brief 深度图像帧
    /// @ingroup1 imimrsdk
    class IImiDepthFrame: public IImiImageFrame {
        
    public:
        IImiDepthFrame(ImiImageFrameHandle hFrame): IImiImageFrame(ImiSensorType::DEPTH, hFrame) {}
        virtual ~IImiDepthFrame(){}

        /// @brief 类型转换.
        /// @paramx p 
        /// @return 
        static ImiDepthFrame asMe(const ImiBaseFrame& p){ return p->getSensorType()==ImiSensorType::DEPTH? std::dynamic_pointer_cast<IImiDepthFrame>(p) : ImiDepthFrame(); }

        /// @brief 将深度图转换成RGB图
        /// @return 
        void getRgbData(unsigned char* array_ref, int array_len) const;


        static bool convDataToRgb(::DataBuffer depthData, ::MutableDataBuffer rgbData, int w, int h); //{ rgbData.data[1]=10; return true; }

    };

    /// @brief 红外Sensor
    /// @ingroup1 imimrsdk
    class IImiIRSensor : public IImiBaseSensor {
        IImiIRSensor() : IImiBaseSensor(ImiSensorType::IR) { } 
    public:

        /// @brief 
        enum ImiIRProperties
        {
            IMI_PROPERTY_IR_MIRROR				= 0,    ///< 
            IMI_PROPERTY_IR_INTRINSIC_PARAMS	= 1,    ///< 
            IMI_PROPERTY_IR_DIST				= 2,    ///< 
            IMI_PROPERTY_IR_FLOODLIGHT			= 3,    ///< 
            IMI_PROPERTY_IR_OUTDOOR_MODE		= 241,  ///<  //value uint8_t 0$)A#:JRDZindoor 1#:JRMboutdoor  2#:0kJRMbsemioutdoor

            IMI_PROPERTY_IR_EXPOSURE_TIME = 0x4b2,  ///< 
            IMI_PROPERTY_IR_FIGURE_PLUS   = 0x4b3,  ///< 
            IMI_PROPERTY_IR_SIMULATE_PLUS = 0x4b4,  ///< 
            IMI_PROPERTY_IR_LIGHT_CURRENT = 0x4b5   ///< 
        };

        virtual ~IImiIRSensor(){}

        static ImiIRSensor asMe(const ImiBaseSensor& p){ return p->getSensorType()==ImiSensorType::IR? std::dynamic_pointer_cast<IImiIRSensor>(p) : ImiIRSensor(); }

        //virtual int start() override { return 0; }  

        // ImiCameraIntrinsic getCameraIntrinsicParams() const;

         /// @brief 获取内参.
        BASE_PROPERTY_GET(imimr::ImiCameraIntrinsic, CameraIntrinsicParams, imimr::ImiCameraIntrinsic(), IMI_PROPERTY_IR_INTRINSIC_PARAMS)


        int setMirror(bool bEnabled){ return 0;}

        

        friend class IImiBaseDevice;
    };

    /// @brief 红外图像帧
    /// @ingroup1 imimrsdk
    class IImiIRFrame: public IImiImageFrame {
    public:
        IImiIRFrame(ImiImageFrameHandle hFrame): IImiImageFrame(ImiSensorType::IR, hFrame) {}
        virtual ~IImiIRFrame(){}

        static ImiIRFrame asMe(const ImiBaseFrame& p){ return p->getSensorType()==ImiSensorType::IR? std::dynamic_pointer_cast<IImiIRFrame>(p) : ImiIRFrame(); }

        void getRgbData(unsigned char* array_ref, int array_len) const;

        static bool convDataToRgb(::DataBuffer depthData, ::MutableDataBuffer rgbData, int w, int h);

    };

    
    /// @brief 人脸检测Sensor
    /// @ingroup1 imimrsdk
    class IImiFaceDetectSensor : public IImiBaseSensor {
        IImiFaceDetectSensor() : IImiBaseSensor(ImiSensorType::FACEDETECT) { } 
    public:
        virtual ~IImiFaceDetectSensor(){}

        static ImiFaceDetectSensor asMe(const ImiBaseSensor& p){ return p->getSensorType()==ImiSensorType::FACEDETECT? std::dynamic_pointer_cast<IImiFaceDetectSensor>(p) : ImiFaceDetectSensor(); }

        virtual int start(int flags = 0) override; 

        //int setMirror(bool bEnabled){ return 0; }

        virtual ImiBaseFrame readFrame(int timeout) override;

        /// @brief 人脸相似度比较
        /// @paramx faceFeature1     人脸特征值
        /// @paramx faceFeature2     人脸特征值
        /// @paramx faceFeatureInUse 
        /// @return  相似度[0.0-1.0],<0为异常.
        float GetFaceDetectFaceIdentifyScore(ImiFaceFeatureVec faceFeature1, ImiFaceFeatureVec faceFeature2, int faceFeatureInUse){ return 0; }

        friend class IImiBaseDevice;
    };

    /// @brief 人脸检测数据帧
    /// @ingroup1 imimrsdk
    class IImiFaceDetectFrame: public IImiBaseFrame {
    protected:
        void* handle;
        IImiFaceDetectFrame(void* handle): IImiBaseFrame(ImiSensorType::FACEDETECT), handle(handle) {}
    public:
        virtual ~IImiFaceDetectFrame(){}

        static ImiFaceDetectFrame asMe(const ImiBaseFrame& p){ return p->getSensorType()==ImiSensorType::FACEDETECT? std::dynamic_pointer_cast<IImiFaceDetectFrame>(p) : ImiFaceDetectFrame(); }

        std::vector<ImiFaceDetectData> getFaceData() const;

        friend class IImiFaceDetectSensor;
    };

    /// @brief 用户跟踪Sensor
    /// @ingroup1 imimrsdk
    class IImiUserTrackSensor : public IImiBaseSensor {
        IImiUserTrackSensor() : IImiBaseSensor(ImiSensorType::USERTRACK) { } 
    public:

        /// @brief 用户跟踪Sensor属性
        /// @ingroup1 imimrsdk
        enum ImiUserTrackProperties
        {
            IMI_PROPERTY_USERTRACK_SKELETON_SMOOTH  = 1,    ///<  // value type uint8_t, 1:smooth, 0: not smooth
            IMI_PROPERTY_USERTRACK_GROUND_CLEANUP   = 2,    ///<  // value type uint8_t, 1:clear the ground, 0: don't clear the ground, default 0
            IMI_PROPERTY_USERTRACK_OUPUT_FOREGROUND = 3,    ///<  // value type uint8_t, 1: 输出前景图，  0：不输出前景图
            IMI_PROPERTY_USERTRACK_OUPUT_SEG_MASK   = 4,    ///<  // value type uint8_t, 1: 输出抠图mask，0：不输出抠图mask
            IMI_PROPERTY_USERTRACK_GROUND_EQUATION  = 5,    ///< 
            IMI_PROPERTY_USERTRACK_OUPUT_PSCTRL     = 6,    ///<  //1 输出体感枪结果 0 ：不输出体感枪结果

        };

        /// @brief 算法模式。
        /// @ingroup1 imimrsdk
        enum ImiUserTrackAlgorithmMode
        {
            IMI_TRACK_MODE_BODY = 0,
            IMI_TRACK_MODE_FACE = 1,
            IMI_TRACK_MODE_HAND = 2,
            IMI_TRACK_MODE_ECHO = 3,
            IMI_TRACK_MODE_SEGM = 4,    // 抠图
            IMI_TRACK_MODE_RGBM = 5,    // 彩色多人
            IMI_TRACK_MODE_RGBO = 6,    // 彩色单人
            IMI_TRACK_MODE_AUDIO = 0xD0,// 音频模式
            IMI_TRACK_MODE_IMU   = 0xD1,// IMU算法模式
            IMI_TRACK_MODE_MUYUAN_SEGM = 0xD2, //牧原分割算法模式

            IMI_TRACK_MODE_BODY_WITH_FACE_ATTRIBUTE_MODE = 0xB0, ///<骨架+人脸属性 20241105
            IMI_TRACK_MODE_BODY_WITH_HAND_GRASP_MODE    = 0xB1,  ///<骨架+抓握状态 20241105
            //todo bjliu
            IMI_TRACK_MODE_ECHO_WITH_SEGM = 0xB2 ///< 离线抠图 20241202
            //end by bjliu
        };

        
        
        static ImiUserTrackSensor asMe(const ImiBaseSensor& p){ return p->getSensorType()==ImiSensorType::USERTRACK? std::dynamic_pointer_cast<IImiUserTrackSensor>(p) : ImiUserTrackSensor(); }


        virtual ~IImiUserTrackSensor(){}

        /// @brief 
        /// @paramx flags {@link #ImiUserTrackAlgorithmMode}
        /// @return 
        virtual int start(int flags = 0) override; //

        //int setMirror(bool bEnabled){ return 0; }

        virtual ImiBaseFrame readFrame(int timeout) override;

        friend class IImiBaseDevice;
    };

    /// @brief 用户跟踪数据帧
    /// @ingroup1 imimrsdk
    class IImiUserTrackFrame: public IImiBaseFrame {
    protected:
        IImiUserTrackFrame(void* handle, int ownHandle = 0);
        void* handle;
        int ownHandle = 0;
        ImiPoint2D point2D;

        ImiBodySkeletonData::ImiHandState handState {ImiBodySkeletonData::IMI_HAND_UNSURE};
        ImiBodySkeletonData::IMI_Rect handRect;

        virtual void onRelease() override;

        friend class Wrapper; 

    public:
        virtual ~IImiUserTrackFrame() {
            onRelease();
        }

        static ImiUserTrackFrame asMe(const ImiBaseFrame& p){ return p && p->getSensorType()==ImiSensorType::USERTRACK? std::dynamic_pointer_cast<IImiUserTrackFrame>(p) : ImiUserTrackFrame(); }

        /// @brief 获取身体骨架数据
        /// @return 
        ImiBodySkeleton getBodySkeleton(bool isCal);

        /// @brief 获取手骨架数据
        /// @return 
        ImiHandSkeleton getHandSkeleton() const;

        /// @brief 获取背景图数据
        /// @return 
        ImiForegroundData getForegroundInfo() const;

        /// @brief 获取前景图像数据.
        /// @paramx array_ref    数据
        /// @paramx array_len    大小
        virtual void getForegroundData(unsigned char* array_ref, int array_len) const;

        /// @brief 获取抠图数据
        /// @return 
        ImiMattingData getMattingInfo() const;

        /// @brief 获取抠图图像数据.
        /// @paramx array_ref    数据
        /// @paramx array_len    大小： w*h*1
        virtual void getMattingData(unsigned char* array_ref, int array_len) const;

        /// @brief 获取用户人脸特征数据
        /// @return 
        ImiFaceFeatureData getFaceFeatureFrame() const;

        /// @brief 深度图转彩色图
        /// @return 
        void getForegroundRgbData(unsigned char* array_ref, int array_len, int mode) const;

        /// @brief 深度图转带透明度的彩色图
        /// @return 
        void getForegroundArgbData(unsigned char* array_ref, int array_len, int mode) const;

        /// @brief 转化成可显示的前景图
        /// @param depthData 
        /// @param rgbData 
        /// @param w 
        /// @param h 
        /// @param mode 
        /// @return 
        static bool convForegroundDataToRgb(::DataBuffer depthData, ::MutableDataBuffer rgbData, int w, int h, int mode, ::DataBuffer colorTable);

        static bool convForegroundDataToRgb(::DataBuffer depthData, ::MutableDataBuffer rgbData, int w, int h, int mode){
            return convForegroundDataToRgb(depthData, rgbData, w, h, mode, ::DataBuffer{0,0});
        }

        /// @brief 转化成可显示的前景图
        /// @param depthData 
        /// @param argbData 
        /// @param w 
        /// @param h 
        /// @param mode 
        /// @return 
        static bool convForegroundDataToArgb(::DataBuffer depthData, ::MutableDataBuffer argbData, int w, int h, int mode, ::DataBuffer colorTable);

        static bool convForegroundDataToArgb(::DataBuffer depthData, ::MutableDataBuffer argbData, int w, int h, int mode){
            return convForegroundDataToArgb(depthData, argbData, w, h, mode, ::DataBuffer{0,0});
        }



        /// @brief 获得骨架数据
        /// @return 
        int getBodySkeletonData(unsigned char* array_ref, int array_len) const;

        /// @brief 获得mouse坐标
        /// @return 
        ImiPoint2D getMouseXy() const;

        ImiVector4I getHandRect () const;

        int getHandState () const;

        long long getFrameHandle() const;
        
        friend class IImiUserTrackSensor;
        
    };

    /// @brief 
    class IImiMoveTrackSensor : public IImiBaseSensor {
        IImiMoveTrackSensor() : IImiBaseSensor(ImiSensorType::MOVETRACK) { } 
    public:
        virtual ~IImiMoveTrackSensor(){}

        virtual int start(int flags = 0) override; 

        virtual ImiBaseFrame readFrame(int timeout) override;

        //int setMirror(bool bEnabled){ return 0; }


        friend class IImiBaseDevice;
    };

    /// @brief 
    class IImiMoveTrackFrame: public IImiBaseFrame {
    protected:
        void* handle;
        IImiMoveTrackFrame(void* handle): IImiBaseFrame(ImiSensorType::MOVETRACK), handle(handle) {}
    public:
        virtual ~IImiMoveTrackFrame();

        static ImiMoveTrackFrame asMe(const ImiBaseFrame& p){ return p->getSensorType()==ImiSensorType::MOVETRACK? std::dynamic_pointer_cast<IImiMoveTrackFrame>(p) : ImiMoveTrackFrame(); }

        ImiMoveData getMoveData() const { return ImiMoveData(); }

        friend class IImiMoveTrackSensor;

    };

    /// @brief 
    class IImiPositionTrackSensor : public IImiBaseSensor {
        IImiPositionTrackSensor() : IImiBaseSensor(ImiSensorType::POSITIONTRACK) { } 
    public:

        /// @brief 算法模式.
        enum ImiPositionTrackAlgorithmMode
        {
            IMI_POSITION_TRACK_ENABLE_SLAM				= 0x00, ///< 
            IMI_POSITION_TRACK_ENABLE_IMU				= 0x01, ///< 
            IMI_POSITION_TRACK_ENABLE_SLAM_AND_IMU		= 0x02, ///< 
            IMI_POSITION_TRACK_ENABLE_MOVE              = 0x03, ///< 
            IMI_POSTIION_TRACK_ENABLE_MOVE_AND_IMU      = 0x04, ///< 
        };


        virtual ~IImiPositionTrackSensor(){}

        virtual int start(int flags = 0) override; 

        //int setMirror(bool bEnabled){ return 0; }

        virtual ImiBaseFrame readFrame(int timeout) override;

        ImiPositionTrackData getPositionTrackData() const { return ImiPositionTrackData(); }

        friend class IImiBaseDevice;
    };

    /// @brief 
    class IImiPositionTrackFrame: public IImiBaseFrame {
    protected:
        void* handle;
        IImiPositionTrackFrame(void* handle): IImiBaseFrame(ImiSensorType::FACEDETECT), handle(handle) {}
    public:
        virtual ~IImiPositionTrackFrame();

        static ImiPositionTrackFrame asMe(const ImiBaseFrame& p){ return p->getSensorType()==ImiSensorType::POSITIONTRACK? std::dynamic_pointer_cast<IImiPositionTrackFrame>(p) : ImiPositionTrackFrame(); }

        friend class IImiPositionTrackSensor;
    };


   

    // REF_POINTER(Int32Property);
    // REF_POINTER(Int64Property);
    // REF_POINTER(Int16Property);
    // REF_POINTER(Int8Property);
    // REF_POINTER(FloatProperty);

    /// @brief 值为String的属性
    /// @ingroup1 imimrsdk
    class IStringProperty : public IImiDeviceProperty {
    protected:
        virtual const void* cPtr() const override { return _str;}
        char* _str;
        int _len;
    public:
        IStringProperty(int len){ _str = new char[len]; _str[0] = 0;_len = len;  memset(_str, 0, _len); }

        virtual ~IStringProperty() { delete _str; }

        int dataSize() const override { return _len; }

        std::string toHexStr() const {
            std::stringstream ss;
            ss << std::hex;
            for(int i=0; i < _len; i++){
                ss << _str[i];
            }
            return ss.str();
        }


        TO_STRING_OP(std::ostream& os, const IStringProperty& a){
            return os << a._str;
        }
    };

    /// @brief 上传文件的进度.
    class IOnUploadProgress {
    public:
        /// @brief 进度变化时回调.
        /// @param percent 进度百分比.
        virtual void onProgress(float percent){ }
    };

    /// @brief 用于上传文件.
    class IFileUploadProperty : public IImiDeviceProperty {

#pragma pack(push)
#pragma pack(1)
        struct ImiFlashFileDataEx
        {
            int32_t dataSize;
            void* pData;
            uint32_t fileType;
            void (*callback)(void*, float);
            void* usr_data;
        };
#pragma pack(pop)

        ImiFlashFileDataEx _dataex;

        void* _buff;

        static void on_callbak(void* data, float progress);

        std::string tohex() const;


    protected:
        virtual const void* cPtr() const override;

    public:
        /// @brief 
        /// @param fileTypeId 
        /// @param filePath 
        /// @param onProgress 
        IFileUploadProperty(int fileTypeId, std::string filePath, IOnUploadProgress* onProgress = NULL);

        ~IFileUploadProperty();

        virtual int dataSize() const override;

        TO_STRING_OP(std::ostream& os, const IFileUploadProperty& a){
            return os << a.tohex();
        }
    };


    /// @brief 处理图像的一些工具
    class MrImageUtil{

    };

    /// @brief 一些系统工具
    class MrSystemUtil{
    public:
        static long getCurrentTimeMs();
    };




    // class ImiTypeCast {
    // public:
    //     static ImiColorSensor asColorSensor(const ImiBaseSensor& p){ return p->getSensorType()==ImiSensorType::COLOR? std::dynamic_pointer_cast<IImiColorSensor>(p) : ImiColorSensor(); }
    // };

  

    // struct Test1{
    //     void testCallback1(INewFrameListener* cb){
    //         std::thread th([=](){
    //             if(cb){
    //                 for(int i=0;i<100;i++){
    //                     cb->onNewFrame(0);
    //                     this_thread::sleep_for(chrono::milliseconds(100));
    //                 }
    //             }
    //         });
    //         th.join();
    //     }
    // };


    
}