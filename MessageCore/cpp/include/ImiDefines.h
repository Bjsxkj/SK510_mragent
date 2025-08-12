/********************************************************************************
  Copyright (c) 2015 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiDefines.h
  @Author        : Chen Shangwei
  @Date          : 2015-05-27
  @Description   : Defines used in IMI API
  @Version       : 0.1.0

  @History       :
  1.2015-05-27 Chen Shangwei Created file

********************************************************************************/

#ifndef _IMI_DEFINES_H_
#define _IMI_DEFINES_H_

#include <stdint.h>
#include <ImiProperties.h>


#ifdef __cplusplus
extern "C" {
#endif


//types
struct ImiDevice;
struct ImiSensor;
struct ImiRecord;
struct ImiFrame;

typedef struct ImiDevice* ImiDeviceHandle;
typedef struct ImiSensor* ImiSensorHandle;
typedef struct ImiStream* ImiStreamHandle;
typedef struct ImiRecord* ImiRecordHandle;
typedef struct ImiFrame*  ImiFrameHandle;


//basic enums
#define IMI_DEVICE_CAMERA  (1 << 0)
#define IMI_DEVICE_AUDIO   (1 << 1)

typedef uint32_t ImiBOOL;
#define IMI_TRUE    1
#define IMI_FALSE   0

#define MULTI_CAPTURE_NUM  10

/*
enum ImiDeviceProperties
{
    IMI_PROPERTY_LD_OPERATE             = 0,
    IMI_PROPERTY_LASER_SAFETY_MODE      = 1,
    IMI_PROPERTY_FRAME_SYNC             = 2,
    IMI_PROPERTY_REGISTRATION           = 3,
    IMI_PROPERTY_SERIAL_NUMBER          = 4
};

enum ImiColorProperties
{
    IMI_PROPERTY_COLOR_MIRROR           = 1,
    IMI_PROPERTY_COLOR_INTRINSIC_PARAMS = 10

};

enum ImiDepthProperties
{
    IMI_PROPERTY_DEPTH_HOLE_FILTER      = 1,
    IMI_PROPERTY_DEPTH_MIRROR           = 2,
    IMI_PROPERTY_DEPTH_DENOISE          = 4,
    IMI_PROPERTY_DEPTH_INTRINSIC_PARAMS = 5

};

enum ImiIRProperties
{
    IMI_PROPERTY_IR_MIRROR           = 1,
    IMI_PROPERTY_IR_INTRINSIC_PARAMS = 2

};
*/

enum ImiStreamType
{
    IMI_COLOR_STREAM          = 0x00,
    IMI_DEPTH_STREAM          = 0x01,
    IMI_IR_STREAM             = 0x02,
    IMI_USER_TRACK_STREAM     = 0x03,
    IMI_FACE_DETECT_STREAM    = 0x04,
    IMI_POSITION_TRACK_STREAM = 0x05,
    IMI_IR_FLOOD_STREAM        = 0x09,
    IMI_DEPTH_IR_STREAM        = 0x0A,
	IMI_IMU_STREAM = IMI_DEPTH_IR_STREAM,
	IMI_AUDIO_LEFT_STREAM	=  0xd0,
    IMI_INVALID_STREAM        = 0xFFFF
};

enum ImiSensorType
{
	IMI_COLOR_SENSOR          = 0x00,
	IMI_DEPTH_SENSOR          = 0x01,
	IMI_IR_SENSOR             = 0x02,
	IMI_USER_TRACK_SENSOR     = 0x03,
	IMI_FACE_DETECT_SENSOR    = 0x04,
	IMI_POSITION_TRACK_SENSOR = 0x05,
	IMI_MOVE_TRACK_SENSOR     = 0x06,
	IMI_IMU_SENSOR			  = 0x0a,
	IMI_AUDIO_LEFT_SENSOR	  = 0xd0,
	IMI_INVALID_TYPE          = 0xFFFF
};

enum ImiRecordFormat
{
    REC_SINGLE_FILE_COMPRESS,
    REC_SINGLE_FILE_RAW,
    REC_MULTI_FILES_RAW,
	REC_BMP_FRAME,
	REC_RAW_FRAME
};

enum ImiRecDataFormat
{
    REC_DEPTH_DATA_COMPRESS,
    REC_DEPTH_DATA_RAW,
    REC_IR_DATA_COMPRESS,
    REC_IR_DATA_RAW,
    REC_COLOR_DATA_RAW,
    REC_COLOR_DATA_H264,
    REC_COLOR_DATA_MJPEG
};

enum ImiConverResolution
{
	IMI_1080X1920,
	IMI_960X1280,
	IMI_720X1280,
	IMI_480X640,

};

enum DetctMode
{
	FACE_SWIPING_MODE	= 0,
	SCAN_MODE			= 1,
	FACTORY_MODE		= 2,
};

#pragma pack (push, 1)

struct LogConfig
{
    const char* pOutputDir;
    uint32_t    level; // must between 0 and 3
    bool        bConsole;
    bool        bFile;
};

struct RecordFileHeader
{
    uint16_t streamType;
    uint16_t recFormat;
    uint16_t recDataFormat;
    uint16_t resolutionX;
    uint16_t resolutionY;
    uint16_t pixelFormat;

    uint32_t frameNum;
    uint32_t dataSize;
    uint64_t timestamp;
};

struct PlaybackSensorInfo
{
    ImiStreamType streamType;
    uint16_t      resolutionX;
    uint16_t      resolutionY;
    uint32_t      frameCount;
};

#define SUB_SYSTEM_VERSION_LEN 12
struct imi_facedetect_vendor_device_ver {
	unsigned char isp_version[SUB_SYSTEM_VERSION_LEN];
	unsigned char dsp0_version[SUB_SYSTEM_VERSION_LEN];
	unsigned char dsp1_version[SUB_SYSTEM_VERSION_LEN];
	unsigned char sec_version[SUB_SYSTEM_VERSION_LEN];
};
struct facedetect_dsp_version {
	unsigned char fw_tmp_ver;
	unsigned char fw_revise_ver;
	unsigned char fw_minor_ver;
	unsigned char fw_major_ver;
	unsigned char model_tmp_ver;
	unsigned char model_revise_ver;
	unsigned char model_minor_ver;
	unsigned char model_major_ver;
	unsigned char algorithm_tmp_ver;
	unsigned char algorithm_revise_ver;
	unsigned char algorithm_minor_ver;
	unsigned char algorithm_major_ver;
};

struct AlgInfo
{
	char algStandardInfo[32];       ///< 算法版本--BHJ imiLiveness V1.0.
	char algVendorCheckedId[8];     ///< 算法厂商标识码--暂定BHJ.
	char algAbility[4];             ///< 算法能力--B .
	char livenessDetectType[4];     ///< 活检方式--02.
	float threshold;                ///< 活体通过的阈值--50.
	char algName[16];               ///< 算法名称--imiLiveness.
	char algVersion[8];             ///< 算法版本号--V1.0.
};

#pragma pack(pop)

struct ImiSensorAttribute
{
    ImiSensorType type;
    char          szDesc[256];
};

enum ImiDeviceState
{
    IMI_DEVICE_STATE_CONNECT    = 0,
    IMI_DEVICE_STATE_DISCONNECT = 1
};

typedef uint32_t ImiCoordinateConvertMode;
enum
{
    IMI_COORDINATE_VGATOVGA    = 0,
    IMI_COORDINATE_VGATO720P   = 1,
    IMI_COORDINATE_VGATO1080P  = 2,

    IMI_COORDINATE_QVGATOVGA   = 3,
    IMI_COORDINATE_QVGATO720P  = 4,
    IMI_COORDINATE_QVGATO1080P = 5
};

// Pixel Format
enum ImiPixelFormat
{
    // depth
    IMI_PIXEL_FORMAT_DEPTH_16BIT  = 0,
    IMI_PIXEL_FORMAT_DEPTH_IR_16BIT = 1,

    // ir
    IMI_PIXEL_FORMAT_IR_10BIT = 100,
    IMI_PIXEL_FORMAT_IR_16BIT = 101,
    IMI_PIXEL_FORMAT_IR_FLOOD_16BIT = 102,

    // color
    IMI_PIXEL_FORMAT_MJPEG  = 200,
    IMI_PIXEL_FORMAT_H264 = 201,

    
    
    IMI_PIXEL_FORMAT_RGB888    = 230,
    IMI_PIXEL_FORMAT_RGBA8888 = 231,

    IMI_PIXEL_FORMAT_YUV_YUYV = 260,
    IMI_PIXEL_FORMAT_YUV_NV12 = 261,
    IMI_PIXEL_FORMAT_YUV_I420 = 262,


    IMI_PIXEL_FORMAT_INVALID = 400
};

enum bitCaptureMode{
	// color
	IMI_CAPTURE_COLOR  = 0x0001,

	// depth
	IMI_CAPTURE_DEPTH  = 0x0010,

	// ir
	IMI_CAPTURE_IR = 0x0100
};

//basic struct
struct ImiDeviceAttribute
{
    uint16_t vendorId;
    uint16_t productId;
    uint32_t deviceAddress;
    char     uri[255];
};

struct ImiVector4I
{
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
};

struct ImiVector4
{
    float x;
    float y;
    float z;
    float w;
};
typedef struct tagPoint3D {
	float x;
	float y;
	float z;
}ImiPoint3D;
struct ImiMat3f
{
    float val3f[9];
};
typedef uint32_t ImiFrameType;
enum {
	IMI_DEPTH_FRAME               = 0x00,
	IMI_DEPTH_SKELETON_FRAME      = 0x01,
	IMI_USER_INDEX_SKELETON_FRAME = 0x02,
	IMI_SKELETON_FRAME            = 0x03,
	IMI_COLOR_FRAME               = 0x04,
	IMI_DEPTH_IR_FRAME            = 0x05,
	IMI_IR_FRAME                  = 0x06,
	IMI_IR_FLOOD_FRAME            = 0x09,
    // USBLITE 支持三图同出 20210301.
	IMI_DEPTH_IR_COLOR_FRAME     = 0x0A,  
};
struct IMI_Rect
{
    int x;
    int y;
    int width;
    int height;
};

#pragma pack (push, 1)

struct ImiFrameMode
{
    ImiPixelFormat pixelFormat;
    int16_t        resolutionX;
    int16_t        resolutionY;
    int8_t         bitsPerPixel;
    int8_t         framerate;
};

struct ImiAudioMode
{
	uint32_t nChannel;
	uint32_t nSamplingRate;
	uint32_t nSamplingAccuracy;
};

struct ImiImuMode
{
    uint32_t nData0;
    uint32_t nData1;
    uint32_t nData2;
};

struct ImiImuAdvVersion
{
	uint32_t slaveNum;
	uint8_t  masterVersion[3];
	uint8_t  slaveVersion[4][3];
    uint8_t  align;
};

struct ImiImuSetMode
{
    uint16_t connHandle;
    uint32_t value;
};

struct ImiImuConnect
{
    uint8_t adrType;
    uint8_t mac[6];
};

struct ImiImuDisconnect
{
    uint16_t connHandle;
    uint8_t  reason;
};

#pragma pack(pop)

typedef int16_t ImiDepthPixel;
struct ImiImageFrame
{
    ImiPixelFormat pixelFormat;
    ImiStreamType  type;
    uint32_t       frameNum;
    uint64_t       timeStamp;

    uint32_t       fps;
    int32_t        width;
    int32_t        height;

    void*          pData;//depth color pixel data, if not null
    uint32_t       size;
	
};

struct ImiAudioFrame
{
	uint32_t type;
	uint32_t frameNum;
	uint64_t timeStamp;
	uint32_t size;
	uint32_t nChannels;
	void* pData;
};

struct ImiImuFrame
{
	uint32_t type;
	uint32_t frameNum;
	uint64_t timeStamp;
	uint32_t size;
	void* pData;
};


#if defined(_WIN32)
//帧数据回调
typedef void (_stdcall* ImiNewFrameCallback)(ImiSensorHandle sensorHandle, void* pUserData, void*pFrame, int frameType);

typedef void (_stdcall* ImiNewImageFrameCallback)(ImiSensorHandle sensorHandle, void* pUserData);
typedef void (_stdcall* ImiDeviceStateCallback)(const char* deviceUri, ImiDeviceState deviceState, void* pUserData);
#else

typedef void (* ImiNewFrameCallback)(ImiSensorHandle sensorHandle, void* pUserData, void*pFrame, int frameType);

typedef void (* ImiNewImageFrameCallback)(ImiSensorHandle sensorHandle, void* pUserData);
typedef void (* ImiDeviceStateCallback)(const char* deviceUri, ImiDeviceState deviceState, void* pUserData);
#endif

#if defined(_WIN32)
typedef void (_stdcall* ImiTemperatureExceptionCallback)(ImiDeviceHandle device,float temperature1,float temperature2);
#else
typedef void (* ImiTemperatureExceptionCallback)(ImiDeviceHandle device,float temperature1,float temperature2);
#endif

#if 0

typedef int16_t ImiDepthPixel;
typedef struct tagImiFrame
{
    ImiPixelFormat pixelFormat;
    ImiImageType   type;
    uint32_t       frameNum;
    uint64_t       timeStamp;

    uint32_t       fps;
    int32_t        width;
    int32_t        height;

    void*		   pData;//depth color pixel data, if not null
    uint32_t	   size;
} ImiImageFrame;

typedef struct tagImiFrameMode
{
    ImiPixelFormat pixelFormat;
    int16_t resolutionX;
    int16_t resolutionY;
    int8_t  bitsPerPixel;
    int8_t  framerate;
} ImiFrameMode;


typedef struct tagImiAudioMode
{
    int32_t type;
    int32_t nChannels;
    int32_t nBitsPerSample;
    int32_t nSamplesPerSec;
    int32_t nBlockAlign;
} ImiAudioMode;

typedef struct tagImiAudioFrame
{
    int32_t type;
    int32_t nChannels;
    int32_t nBitsPerSample;
    int32_t nBlockAlign;
    uint64_t timeStamp;
    uint32_t frameNum;
    void* pData;
    uint32_t size;
} ImiAudioFrame;



typedef struct tagImiSafetyChipVersion
{
	uint8_t safetyChipVersion[6];
}ImiSafetyChipVersion;

typedef struct tagImiCaptureRequest
{


} ImiCaptureRequest;

typedef struct tagImiInputTrackState
{

} ImiInputTrackState;

typedef struct tagVector3
{
    float x;
    float y;
    float z;
} ImiVector3;

typedef struct tagImiMoveInfo
{
    uint16_t index;
    const char* serial;
    //struct RGB color;

    uint16_t status;
    uint16_t battery;
    uint16_t temperature;
} imiMoveInfo;

typedef struct tagImiMoveTracker
{
    imiMoveInfo moveinfo;

    float rumble;
    int buttons;
    int pressed;
    int released;
    float trigger;


    ImiVector3 accelerometer;
    ImiVector3 gyroscope;
    ImiVector3 magnetometer;
    ImiVector4 quaternion;
    //tracker
    ImiVector3 postion;

} imiMoveTracker;

typedef struct tagImiMoveController
{
    uint8_t ledColor[3];  // rgb
    uint8_t rumble;      // 0 - 255:
} ImiMoveController;


#if defined(_WIN32)
typedef void (_stdcall* ImiNewImageFrameCallback)(ImiImageStreamHandle stream, void* pUserData);
typedef void (_stdcall* ImiDeviceStateCallback)(const char* deviceUri, ImiDeviceState deviceState, void* pUserData);
typedef void (_stdcall* ImiCaptureCallback)(ImiImageFrame** ppFrame, void* pCookie);
typedef void (_stdcall* ImiNewBodyTrackFrameCallback)(ImiBodyTrackHandle stream, void* pCookie);
typedef void (_stdcall* ImiNewAudioFrameCallback)(ImiAudioStreamHandle stream, void* pUserData);

#else
typedef void (* ImiNewImageFrameCallback)(ImiIamgeStreamHandle stream, void* pUserData);
typedef void (* ImiDeviceStateCallback)(const char* deviceUri, ImiDeviceState deviceState, void* pUserData);
typedef void (* ImiCaptureCallback)(ImiImageFrame** ppFrame, void* pCookie);
typedef void (* ImiNewBodyTrackFrameCallback)(ImiBodyTrackHandle stream, void* pCookie);
typedef void (* ImiNewAudioFrameCallback)(ImiAudioStreamHandle stream, void* pUserData);
#endif


#endif

#ifdef __cplusplus
}
#endif
#endif // _IMI_DEFINES_H_
