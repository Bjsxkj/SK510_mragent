/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiUserTrackDefines.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : UserTrack Defines
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_PAY_DEFINES_H_
#define _IMI_PAY_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>
#include <ImiFaceDetectDefines.h>


#define MAX_FACE_INFO 6
#if defined(_WIN32)

typedef void (_stdcall* ImiNewPayFrameCallback)(ImiSensorHandle paySensor, void* pUserData);

#else

typedef void (* ImiNewPayFrameCallback)(ImiSensorHandle paySensor, void* pUserData);

#endif
#ifdef __cplusplus
extern "C" {
#endif

/*! \add to group iminect
* @{
*/
//System APIs

//typedef uint32_t ImiFaceAlgorithmMode;
//enum
//{
//	IMI_FACE_DETECT_MODE                         = 0x00,
//	IMI_FACE_IDENTIFIER_MODE                     = 0x01,
//	IMI_FACE_ANTI_SPOOFING_MODE                  = 0x02,
//	IMI_FACE_IDENTIFIER_AND_ANTI_SPOOFING_MODE   = 0x03,
//	IMI_FACE_ECHO_FRAME_MODE                     = 0x04,
//};
//
//typedef uint32_t ImiFaceSelectMode;
//enum
//{
//	IMI_FACE_SELECT_ALL             = 0x00,
//	IMI_FACE_SELECT_LARGEST         = 0x01,
//	IMI_FACE_SELECT_NEAREST         = 0x02,
//};

#define		FACE_TOO_SMALL 				-1		//face too small		
#define		FACE_TOO_BIG 				-2		//face too big		
#define		LIVE_SPLIT_SCREEN_ERROR		-3		//分屏异常		
#define		LIVE_IMAGE_ERROR  			-4	//图像错误		
#define		LIVE_SCORE_TOO_LOW  		-5	//活体检测质量分过低		
#define		LIVE_FACE_DIS_ERROR   		-102	//人脸与摄像头距离异常		
#define		LIVE_CLOSE_TO_IMAGE_EDGE 	-103	//CLOSE_TO_IMAGE_EDGE		
#define		LIVE_FACE_AREA_ERROR   		-104	//人脸大小异常		
#define		LIVE_FACE_IS_SPOOF 			-105	//check whether live or spoof //mask		
#define		VPU_PROC_TIMEOUT 			-200	//VPU_PROCESS_TIMEOUT		
#define		MH1902_PROC_ERROR			-201	//加密芯片处理异常

enum QU_ERROR
{
	QU_SUCCESS = 0,					//无异常返回		   
	QU_FACEDETECT_FAILED,          	//人脸检测失败-无人脸		
	QU_ERROR_MODE,                 	//模式选择错误，目前仅使用模式2（有3种模式，0：银联过检图片版，1：银联过检视频版，2：商用版）		
	QU_INIT_FAILED,                	//初始化失败		
	QU_FACE_OVERSTEP_BOUNDARY,     	//图像靠近边缘		
	QU_IMAGE_ERROR,                	//图像错误		
	//新增				
	QU_ANGLE_ABNORMAL,				//姿态角度不符合要求		
	QU_EXPRE_EYE_CLOSE,     		//表情不满足：闭眼		
	QU_EXPRE_MOUTH_OPEN,       		//表情不满足：张嘴		
	QU_EYEDIST_ABNORMAL,			//眼间距异常		
	QU_BLURRED,						//模糊		
	QU_IS_COVERED,					//有遮挡		
	QU_LIGHT_ABNORMAL,				//光照异常		
	QU_RETURN_ERROR,				//质量评估返回异常		


};



#pragma pack (push, 1)

//struct ImiFaceDetectMode
//{
//	ImiFaceAlgorithmMode faceAlgorithmMode;
//	ImiFaceSelectMode selectMode;
//};

struct ImiPayLisenceKey 
{
	uint8_t key[1024]; //todo
};

struct ImiPayPublicKey
{
	uint8_t key[226];
};

struct ImiPayUmsKey
{
	uint8_t key[64];
};

struct ImiPayFaceKey
{
	uint8_t key[180];
};

struct ImiKeepLiveKey
{
	uint8_t key[1024];
};

struct ImiDeviceIdKey
{
	uint8_t key[1024];
};

struct ImiSNKey
{
	uint8_t key[1024];
};


struct ImiPayFrame
{
	uint32_t			headLen;
	uint32_t			encryptLen;
	uint32_t			thumbnailLen;
	uint32_t			thumbnailWidth;
	uint32_t			thumbnailHeight;
	ImiLivenessInfo		livenessInfo;   
	int32_t				isLive;
	int32_t				livenessScore;	
	bool				isEyeClosed;
	uint32_t			bioRslt;
	uint8_t				open_mouth_flag;
	int32_t				error_code;
	uint64_t			rgbTimeStamp;
	float				rgbBrightness;
	float				rgbDefinition;
    void*				pData;
	void*				pThumbnailData;
};

struct DepthResult
{
	void*			depthData;
	uint32_t		depthDataSize;
	uint64_t		depthTimeStamp;
	float			voidage;
	float			depthEyeDistance;
	ImiRect			depthRect;
};

struct IrResult
{
	void*		irData;
	uint32_t	irDataSize;
	uint64_t	irTimeStamp;
	float		irBrightness;
	float		irDefinition;
	ImiRect		irRect;

};
struct ImiFacePayFrame
{
	bool		isLive;
	float		livenessScore;
	void *		pColorData;
	uint32_t	colorFrameSize;

	void*		faceThumbnail;
	uint32_t    faceThumbnailSize;
	uint32_t    faceThumbnailHight;
	uint32_t	faceThumbnailWidth;

	uint64_t	rgbTimeStamp;
	float		rgbBrightness;
	float		rgbDefinition;
	DepthResult depthResult;
	IrResult	irResult;
	bool        isEyeClosed;
	uint32_t    bioRslt;
	uint8_t		open_mouth_flag;
	int			error;
};
struct CameraParams {
	float Fx;
	float Fy;
	float Cx;
	float Cy;
};
struct RotationMatrix {
	float R00;
	float R01;
	float R02;
	float R10;
	float R11;
	float R12;
	float R20;
	float R21;
	float R22;
};
struct TranslationalParams {
	float T0;
	float T1;
	float T2;
};
struct imiYouTuKey
{
    uint8_t key[1024];
};
struct fw_ver {
	unsigned char major;
	unsigned char minor;
	unsigned short ver;
};
struct hw_ver {
	unsigned char hw;
	unsigned char chip;
	unsigned char ap_hw;
};
struct protocol_ver {
	struct fw_ver fw;
	struct hw_ver hw;
};
struct sensor_info {
	unsigned char ep_num;
	unsigned char sensor_type;
};
struct device_param {
	unsigned char serial[32];   //设备SN号
	unsigned char usb_speed;
	struct protocol_ver versions;
	struct sensor_info sensor_list[16];
	unsigned char dev_fw_version[32];
	unsigned char sec_key_version[32];  //密钥版本号
	unsigned char dev_version[32]; //模组版本名称
	unsigned char dev_type[32]; //模组型号名称
	unsigned short pid; // pid 预留用途
	unsigned short vid;  // vid 预留用途
	unsigned short orientation; //相机旋转角度
	unsigned char facing; //相机朝向
	unsigned char mirror; //是否镜像
	unsigned int default_rgb_width; // 默认彩图宽度
	unsigned int default_rgb_height; // 默认彩图高度
	unsigned int default_depth_width; // 默认深度图宽度
	unsigned int default_depth_height; // 默认深度图高度
	unsigned unused[32];
};

struct ImiModuleInfo {
	unsigned char moduleVendorId[6];
	ImiSNKey tuSn;
	unsigned char cameraOpenType;//0 :uvc 1:原生api
	unsigned char serial[32];   //设备SN号
	//unsigned char usb_speed;
	//struct protocol_ver versions;
	//struct sensor_info sensor_list[16];
	unsigned char dev_fw_version[32];
	unsigned char sec_key_version[32];  //密钥版本号
	unsigned char dev_version[32]; //模组版本名称
	unsigned char dev_type[32]; //模组型号名称
	unsigned short pid; // pid 预留用途
	unsigned short vid;  // vid 预留用途
	unsigned short orientation; //相机旋转角度
	unsigned char facing; //相机朝向
	unsigned char mirror; //是否镜像
	unsigned int default_rgb_width; // 默认彩图宽度
	unsigned int default_rgb_height; // 默认彩图高度
	unsigned int default_depth_width; // 默认深度图宽度
	unsigned int default_depth_height; // 默认深度图高度

	float	m_ir_param[9]; //红外
	float   m_depth_param[9];	//深度
	float	m_rgb_param[9];//彩色
	float	m_rot_mat[9];//旋转
	float	m_trans[3];;//平移
};

struct imi_run_statistics_info {
	unsigned int runtime; //second
	unsigned int open_payment_frame_times;
	unsigned int get_payment_encry_frame_times;
	unsigned int get_one_payment_frame_time; //usecond;
};
struct LivenessDetectOption
{
	float	mThreshold;
	float	mValidPitch;        //上下阈值过滤超过该角度的人脸
	float	mValidRoll;    //旋转阈值过滤超过该角度的人脸
	float	mValidYaw;     //左右阈值过滤超过该角度的人脸
	float	mValidEyeDistance; //眼间距阈值，过滤超过该值的眼间距人脸
	float	mValidQualityScore; //过滤低于该质量分的人脸
	int		mValidMinFaceSize; //过滤小于该阈值的人脸
	int		mValidMaxFaceSize; //过滤大于该阈值的人脸
	int		mMaxFaceNumSupport; //支持检测的最多人脸
	float	mValidFaceSizeRate; //活检主体无法确定推荐阈值0.6，1为不过滤
	bool	isRejectEyeClose; //是否拒绝闭眼
	bool	isRejectMouthMask; //是否拒绝口罩
	bool	IsRejectSunGlasses; //是否拒绝墨镜
	float	scaleW; //人脸缩略图的宽度扩大倍数
	float	scaleH; //人脸缩略图高度扩大倍数
	int		compressRate;                 //活检成功时彩色图的压缩比
	int		compressFormat; //活检成功时彩色图格式：0jpeg1png2webp

	float	mVoidage;  //深度图空洞率阈值，过滤超过该空洞 率的⼈脸 取值[0,1]
	float	mDepthEyeDistance;  //深度图⼈脸眼间距阈值，过滤超过该 眼间距的⼈脸。 取值[0,1]

	float	mIrDefinition;  //非必须参数。红外图清晰度阈值，过 滤超过该清晰度的⼈脸。 取值[0,1]
	float	mIrBrightness;  //非必须参数。红外图亮度阈值，过滤 超过该亮度的⼈脸。 取值[0,1]
	float	mIrEyeDistance; //非必须参数。红外图人眼间距阈值， 过滤超过该间距的⼈脸。 取值[0,1]

	unsigned short mIrLightAdjustMinThreshold;	//红外图亮度下限值，低于此值需要进行调亮
	unsigned short mIrLightAdjustMaxThreshold;	//红外图亮度上限值，高于此值需要进行调暗

};
struct Random
{
	uint8_t random[4096];
};

//add by zhangzf for new protocal
typedef struct
{
	uint32_t infocont_offset;			//info内容相对于当前faceInfo_head帧头的偏移
	uint32_t len;						//单个FaceInfo_cont[i]元素的长度, 即sizeof(FaceInfo_cont[i])；FaceInfo总长度 = faceCount * len；
	uint8_t encry_flag;				//加密标志, 0-不加密，1-加密；
	uint16_t faceCount;				//检测到的人脸个数，0~6.超过6个只截取前6个结果给SDK。
	uint16_t selectfaceidx;			//优选人脸的下标。0~5,其他值非法，0xffff表示该值无效。
	int error_code;				//返回错误码,默认0-无已知错误
	uint8_t temp[111];					//备用字节，默认值0
}FaceInfo_head;	//共预留 128 B 空间;

typedef struct 
{
	//人脸检测
	ImiRect bbox;					// 人脸框信息
	ImiPoint2D landmark[5];		// 每一个是一组(x,y)坐标，表示5个特征点，分别是左眼右眼鼻尖和两个嘴角

	//活体
	float depth;      			// 表示距离摄像头的距离
	float deplivscr;  			// 深度活体得分，计算值value<=1.0
	float irlivscr;   			// 红外活体得分，计算值value<=1.0
	float livscr;     			// 活体检测分值，计算值value<=1.0

	//质量评估
	uint8_t blur_flag;			//1：图像清晰，0：图像模糊
	uint8_t light_flag;			//1：光照正常，0：光照不满足
	uint8_t cover_flag;			//1：无遮挡，0：有遮挡
	uint8_t angle_flag;			//1：姿态正常，0：姿态角度过大
	uint8_t eye_distance_flag;	//1：眼间距正常，0：眼间距不满足预定值
	uint8_t expression;         //1：表情正常，0：表情不满足
	uint8_t open_mouth_flag;		//1：闭嘴，0：张嘴
	uint8_t close_eye_flag;		//1：睁眼,0：闭眼 
	uint8_t multi_face_flag;		//1：1个人脸，0：多人脸
	uint8_t ok_flag;				//1：不正常图像，0：正常图像，默认为1，以上任一条件为0时都不满足图像合格的条件

	float   eye_distance;			//眼间距信息
	float   roll;					//roll角信息
	float   pitch;          		//pitch角信息
	float   yaw;           		//yaw角信息
	float	quality_score;   		//质量得分，只针对blur、light//无用
} FaceInfo_cont;

typedef struct
{
	FaceInfo_head head; 						//共预留 128 B 空间;
	FaceInfo_cont faces[MAX_FACE_INFO];	//人脸算法结果，最多6组。
}FaceInfoResult;

typedef struct
{
	uint32_t data_offset;			//数据内容相对于当前head帧头的偏移
	uint32_t len;					//数据内容长度
	uint8_t encry_flag;			//加密标志, 0-不加密，1-加密；
	uint8_t batch;					//数据块数量, 本程序目前默认值1
	uint16_t height;				//图像高度
	uint16_t width;					//图像宽度
	uint8_t channel;				//通道数
	uint8_t type;					//图像数据类型, 0-raw;1-jpeg;
	uint8_t bit_depth;			//位深度, 0-8UC3; 1-16UC1; 3-8UC1;
	uint8_t temp[111];				//备用字节，默认值0
}ImageInfo_head; //共预留 128 B 空间;

typedef struct
{
	ImageInfo_head head; 	//共预留 128 B 空间;
	uint8_t data[640*480*3];			 		//数据内容
}ImageInfo;

typedef struct
{
	uint32_t	head_flag;				//固定值0x5A5B
	uint32_t	head_len;				//帧头总长度
	uint32_t	FaceInfo_offset;		//FaceInfo struct的实际偏移值--相对于frame_head帧头
	uint32_t	FaceInfo_len;			//FaceInfo长度
	uint32_t	color_offset;			//彩图偏移值--相对于frame_head帧头
	uint32_t	color_len;				//彩图长度
	uint32_t	depth_offset;			//深度图偏移值--相对于frame_head帧头
	uint32_t	depth_len;				//深度图长度
	uint32_t	infrar_offset;		//红外图偏移值--相对于frame_head帧头
	uint32_t	infrar_len;			//红外图长度
	uint32_t	color_thumb_offset;	//彩图缩略图偏移值--相对于frame_head帧头
	uint32_t	color_thumb_len;		//彩图缩略图长度
	uint8_t		pay_frame_flag;			//标识
	uint64_t	test_detec_num;
	uint8_t	temp[71]; 				//备用字节，默认值0
}ImiFaceInfoFrame_head; //共预留 128 B 空间;

// 人脸数据帧（下位机需要打包上传给上位机的数据）
typedef struct tagImiFaceInfoFrame
{
	ImiFaceInfoFrame_head head;		//共预留 128 B 空间;
	FaceInfoResult detect_res;				//存放算法结果信息
	ImageInfo color;					//存放彩图
	ImageInfo depth;					//存放深度图
	ImageInfo infrar;					//存放红外图
	ImageInfo color_thumb;			//存放彩图缩略图

}ImiFaceInfoFrame;




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_PAY_DEFINES_H_
