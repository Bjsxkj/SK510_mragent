/********************************************************************************
  Copyright (c) 2015 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiProperties.h
  @Author        : Chen Shangwei
  @Date          : 2015-05-14
  @Description   : Define ImiNect Propeties
  @Version       : 0.1.0

  @History       :
  1.2015-05-14 Chen Shangwei Created file

********************************************************************************/

#ifndef _IMI_C_PROPERTIES_H_
#define _IMI_C_PROPERTIES_H_

enum ImiDeviceProperties
{
    IMI_PROPERTY_LD_OPERATE             = 0,
    IMI_PROPERTY_LASER_SAFETY_MODE      = 1,   
    IMI_PROPERTY_SERIAL_NUMBER          = 2, //value uint8_t serialNo[32];
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

enum ImiColorProperties
{
    IMI_PROPERTY_COLOR_MIRROR           = 0,
    IMI_PROPERTY_COLOR_INTRINSIC_PARAMS = 1,
	IMI_PROPERTY_COLOR_SCAN_MODE       = 11, //value uint8_t , 0$)A#:K"A3  1#:I(Bk

	IMI_PROPERTY_COLOR_AE_MODE       = 0x260,
	IMI_PROPERTY_COLOR_EXPOSURE_UP   = 0x379,
	IMI_PROPERTY_COLOR_EXPOSURE_DOWN = 0x37a
};

enum ImiDepthProperties
{
    IMI_PROPERTY_DEPTH_HOLE_FILTER           = 0,
    IMI_PROPERTY_DEPTH_MIRROR                = 1,
    IMI_PROPERTY_DEPTH_DENOISE               = 2,
    IMI_PROPERTY_DEPTH_INTRINSIC_PARAMS      = 3,
    IMI_PROPERTY_DEPTH_REGISTRATION          = 4,
    IMI_PROPERTY_DEPTH_SKELETON_REGISTRATION = 5,
    IMI_PROPERTY_DEPTH_UVC_SYNC              = 6,

	IMI_PROPERTY_DEPTH_EXPOSURE_TIME = 0x414,
	IMI_PROPERTY_DEPTH_FIGURE_PLUS   = 0x415,
	IMI_PROPERTY_DEPTH_SIMULATE_PLUS = 0x416,
	IMI_PROPERTY_DEPTH_LIGHT_CURRENT = 0x417
};

enum ImiIRProperties
{
    IMI_PROPERTY_IR_MIRROR				= 0,
    IMI_PROPERTY_IR_INTRINSIC_PARAMS	= 1,
    IMI_PROPERTY_IR_DIST				= 2,
    IMI_PROPERTY_IR_FLOODLIGHT			= 3,
	IMI_PROPERTY_IR_OUTDOOR_MODE		= 241, //value uint8_t 0$)A#:JRDZindoor 1#:JRMboutdoor  2#:0kJRMbsemioutdoor

	IMI_PROPERTY_IR_EXPOSURE_TIME = 0x4b2,
	IMI_PROPERTY_IR_FIGURE_PLUS   = 0x4b3,
	IMI_PROPERTY_IR_SIMULATE_PLUS = 0x4b4,
	IMI_PROPERTY_IR_LIGHT_CURRENT = 0x4b5
};

// Public Properties
enum ImiPropertyEnum
{
    //General,
    //IMI_PROPERTY_GENERAL_VERSION       = 0x00,//ImiVersions Read Only
    //IMI_PROPERTY_GENERAL_SERIAL_NUMBER = 0x01,//String, Read Only
    //IMI_PROPERTY_GENERAL_FRAME_SYNC    = 0x02, //Not Support Yet
    //IMI_PROPERTY_IMAGE_REGISTRATION    = 0x03, //value type ImiBOOL, IMI_TRUE:open registration, IMI_FALSE: close registration


    // Color,
    /*IMI_PROPERTY_COLOR_MIRROR     = 0x13, //value type uint8_t, 1:mirror, 0: not mirror
    IMI_PROPERTY_COLOR_SHARPNESS  = 0x14, //Not Support Yet
    IMI_PROPERTY_COLOR_BRIGHTNESS = 0x15, //Not Support Yet
    IMI_PROPERTY_COLOR_CONTRAST   = 0x16, //Not Support Yet
    IMI_PROPERTY_COLOR_SATURATION = 0x17, //Not Support Yet
    IMI_PROPERTY_COLOR_GAIN       = 0X18, //Not Support Yet
    IMI_PROPERTY_COLOR_AUTO_WHITE_BALANCE_MODE = 0x19, //Not Support Yet
    IMI_PROPERTY_COLOR_AUTO_EXPOSURE_MODE      = 0x1a, //Not Support Yet
    IMI_PROPERTY_COLOR_ANTIFLICKER             = 0x1b, //Not Support Yet

    //Depth,
    IMI_PROPERTY_DEPTH_HOLE_FILTER = 0x33,//Not Support Yet
    IMI_PROPERTY_DEPTH_MIRROR      = 0x34, //value type uint8_t, 1:mirror, 0: not mirror
    IMI_PROPERTY_DEPTH_DECIMATION  = 0x35,//Not Support Yet

    //Skeleton,
    IMI_PROPERTY_SKELETON_SMOOTH    = 0x40,
    IMI_PROPERTY_SKELETON_MIRROR    = 0x41, //value type uint8_t, 1:mirror, 0: not mirror
    IMI_PROPERTY_SKELETON_USER_SELECTOR_MODE     = 0x53,
    IMI_PROPERTY_SKELETON_SELECT_TRACK_USER      = 0x54,
    IMI_PROPERTY_SKELETON_UNSELECT_TRACK_USER    = 0x55,
    IMI_PROPERTY_SKELETON_CALIBRATION            = 0x57,  // value type uint8_t, 1:use calibration, 0: don't use calibration, default 0

    IMI_PROPERTY_GROUND_EQUATION    = 0x70,	 // value type uint8_t, 1:calculate ground equation, 0: don't calculate the ground equation, default 0
    IMI_PROPERTY_GROUND_CLEANUP		= 0x71,	 // value type uint8_t, 1:clear the ground, 0: don't clear the ground, default 0*/

};


// Image mirror's values
enum ImiMirror
{
    PARAM_MIRROR_DISABLE      = 0x00,
    PARAM_MIRROR_ENABLE       = 0x01
};

// Color brightness's values
enum ImiColorBrightness
{
    PARAM_COLOR_BRIGHTNESS_DEFAULT  = 0x00,
    PARAM_COLOR_BRIGHTNESS_ADD_1    = 0x01,
    PARAM_COLOR_BRIGHTNESS_ADD_2    = 0x02,
    PARAM_COLOR_BRIGHTNESS_ADD_3    = 0x03,
    PARAM_COLOR_BRIGHTNESS_ADD_4    = 0x04,
    PARAM_COLOR_BRIGHTNESS_REDUCE_1 = 0x05,
    PARAM_COLOR_BRIGHTNESS_REDUCE_2 = 0x06,
    PARAM_COLOR_BRIGHTNESS_REDUCE_3 = 0x07,
    PARAM_COLOR_BRIGHTNESS_REDUCE_4 = 0x08
};

// Color contrast's values
enum ImiColorContrast
{
    PARAM_COLOR_CONTRAST_DEFAULT    = 0x00,
    PARAM_COLOR_CONTRAST_ADD_1      = 0x01,
    PARAM_COLOR_CONTRAST_ADD_2      = 0x02,
    PARAM_COLOR_CONTRAST_ADD_3      = 0x03,
    PARAM_COLOR_CONTRAST_ADD_4      = 0x04,
    PARAM_COLOR_CONTRAST_REDUCE_1   = 0x05,
    PARAM_COLOR_CONTRAST_REDUCE_2   = 0x06,
    PARAM_COLOR_CONTRAST_REDUCE_3   = 0x07,
    PARAM_COLOR_CONTRAST_REDUCE_4   = 0x08
};

// Color saturation's values
enum ImiColorSaturation
{
    PARAM_COLOR_SATURATION_DEFAULT  = 0x00,
    PARAM_COLOR_SATURATION_ADD_1    = 0x01,
    PARAM_COLOR_SATURATION_ADD_2    = 0x02,
    PARAM_COLOR_SATURATION_ADD_3    = 0x03,
    PARAM_COLOR_SATURATION_ADD_4    = 0x04,
    PARAM_COLOR_SATURATION_REDUCE_1 = 0x05,
    PARAM_COLOR_SATURATION_REDUCE_2 = 0x06,
    PARAM_COLOR_SATURATION_REDUCE_3 = 0x07,
    PARAM_COLOR_SATURATION_REDUCE_4 = 0x08
};

// Color auto white balance's values
enum ImiColorWhiteBalance
{
    PARAM_COLOR_WHITE_BALANCE_AUTO          = 0x00,
    PARAM_COLOR_WHITE_BALANCE_MANUAL_SUNNY  = 0x01,
    PARAM_COLOR_WHITE_BALANCE_MANUAL_CLOUDY = 0x02,
    PARAM_COLOR_WHITE_BALANCE_MANUAL_OFFICE = 0x03,
    PARAM_COLOR_WHITE_BALANCE_MANUAL_HOME   = 0x04,
    PARAM_COLOR_WHITE_BALANCE_MANUAL_NIGHT  = 0x05
};

// Color auto exposure's values
enum ImiColorAutoExposure
{
    PARAM_COLOR_AUTO_EXPOSURE_MODE_DISABLE  = 0x00,
    PARAM_COLOR_AUTO_EXPOSURE_MODE_ENABLE   = 0x01
};

// Light mode
enum ImiLightMode
{
    IMI_LIGHT_MODE_SUNNY  = 0x00,
    IMI_LIGHT_MODE_CLOUDY = 0x01,
    IMI_LIGHT_MODE_OFFICE = 0x02,
    IMI_LIGHT_MODE_HOME   = 0x03,
    IMI_LIGHT_MODE_NIGHT  = 0x04
};

enum ImiDeviceDbtype
{
	IMI_DEVICE_DB_NORMAL = 0x00,
	IMI_DEVICE_DB_AUDIO  = 0x01,
	IMI_DEVICE_DB_BULK   = 0x02
};

enum ImiDeviceProInfo
{
	IMI_BULK_FUNC_TYPE = 0x00,
	IMI_PRODUCT_NAME   = 0x01,
	IMI_PRODUCT_RGB    = 0x02,
	IMI_PRODUCT_TX     = 0x03,
	IMI_PRODUCT_RX     = 0x04,
	IMI_PRODUCT_PCB    = 0x05
};

#pragma pack (push, 1)

typedef struct tag_imi_fov_arear
{
    int topLeftX;
    int topLeftY;
    int width;
    int height;

} imi_fov_arear;

typedef struct tag_imi_support_capacity
{
    imi_fov_arear fov_arear_28cm;
    imi_fov_arear fov_arear_60cm;
    imi_fov_arear fov_arear_100cm;
    uint8_t bSupportColorDepthIr;
    uint8_t bSupportOutdoorMode;
    uint8_t bSupportScanCodeMode;
    uint8_t bSupportReset;
} ImiSupportCapacity;


struct imi_sdk_version
{
    uint8_t major;
    uint8_t minor;
    uint16_t revision;
};

struct imi_fw_version
{
    uint8_t major;
    uint8_t minor;
    uint16_t revision;
};

struct imi_hw_version
{
    uint8_t hardware;
    uint8_t chip;
    uint8_t ap_hardware;
};

struct ImiVersions
{
    imi_sdk_version sdk_version;
    imi_fw_version fw_version;
    imi_hw_version hw_version;
    char uvc_color_version[64];
    char bridge_fw_version[64];
};

struct ver
{
	uint8_t tmp_ver;
	uint8_t revise_ver;
	uint8_t minor_ver;
	uint8_t major_ver;
};

struct dsp_version {
	ver fw;
	ver hand;
	ver depth_skt;
	ver segment;
	ver somatic;
};

#define SUB_SYSTEM_VERSION_LEN 12
#define SUB_SYSTEM_DSP0_VERSION_LEN 20
struct imi_vendor_device_ver {
	uint8_t isp_version[SUB_SYSTEM_VERSION_LEN];
	uint8_t dsp0_version[SUB_SYSTEM_DSP0_VERSION_LEN];
	uint8_t dsp1_version[SUB_SYSTEM_VERSION_LEN];
	//uint8_t sec_version[SUB_SYSTEM_VERSION_LEN];
};


struct imi_chip_version{
	uint8_t sf_major_no;
	uint8_t sf_minor_no;
	uint8_t sf_revision_no;
	uint8_t hw_major_no;
	uint8_t hw_minor_no;
	uint8_t hw_revision_no;
};

struct ImiVersions2280
{
	imi_sdk_version sdk_version;
	uint8_t ap_sorftware_version[32];	//Ä£×é¹Ì¼þ°æ±¾ºÅ
	//imi_chip_version security_chip_version;			//°²È«Ð¾Æ¬°æ±¾ºÅ
	dsp_version dsp_2280_version;		//dsp°æ±¾ºÅ	°üº¬fw,model,algorithm,dsp0,ÆäËûµÄ¿ÉÒÔ²»¿¼ÂÇ
};


struct ImiCameraIntrinsic
{
    float fxParam;  //½¹¾à
    float fyParam;  //½¹¾à
    float cxParam;  //Ö÷¹âÖáµã
    float cyParam;  //Ö÷¹âÖáµã
    float k1Param;  //¾¶Ïò»û±ä
    float k2Param;  //¾¶Ïò»û±ä
    float k3Param;  //¾¶Ïò»û±ä
    float p1Param;  //ÇÐÏò»û±ä
    float p2Param;  //ÇÐÏò»û±ä
};

struct ImiMatlabCameraIntrinsic
{
    double fx_1;
    double fy_1;
    double cx_1;
    double cy_1;
};

struct ImiMatlabCameraData
{
    struct ImiMatlabCameraIntrinsic color;
    struct ImiMatlabCameraIntrinsic ir;
    double unused[6];
};

typedef struct vendor_device_caps
{
	unsigned char psenor;
	unsigned char audio;
	unsigned char bluetooth;
	unsigned char flood;
	unsigned char Registrantion;
	unsigned char rev1;
	unsigned char rev2;
	unsigned char rev3;
} ImiVendorDeviceCaps;

//±ê¶¨IMUÊý¾Ý
typedef struct imu_data{
	float x;
	float y;
	float z;
}ImiImuData;

typedef struct imu_state{
	uint8_t verticalState;	//0£º´¹Ö± 1:·Ç´¹Ö±
	ImiImuData imuData;		//µ±Ç°ImuÎ»ÖÃ
}ImiImuState;

#pragma pack(pop)

#endif // _IMI_C_PROPERTIES_H_
