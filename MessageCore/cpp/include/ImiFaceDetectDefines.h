/********************************************************************************
=======
锘?********************************************************************************
>>>>>>> cd6280af (Initial commit)
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiFaceDetectDefines.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : FaceDetect Defines
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_FACE_DETECT_DEFINES_H_
#define _IMI_FACE_DETECT_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

#define MAX_FACE_NUM 6
#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup iminect
* @{
*/
//System APIs

enum ImiFaceProperties
{

};

typedef uint32_t ImiFaceAlgorithmMode;
enum
{
    IMI_FACE_DETECT_MODE                         = 0x00,
    IMI_FACE_IDENTIFIER_MODE                     = 0x01,
    IMI_FACE_ANTI_SPOOFING_MODE                  = 0x02,
    IMI_FACE_IDENTIFIER_AND_ANTI_SPOOFING_MODE   = 0x03,
	IMI_FACE_ECHO_FRAME_MODE                     = 0x04,

	IMI_FACE_PAYFRAME_NOSAFE_MODE				 =0x07,
	IMI_FACE_PAYFRAME_SAFE_SHANG_MODE			 =0x08,
	IMI_FACE_PAYFRAME_SAFE_LIAN_MODE			 =0x09,
};

typedef uint32_t ImiFaceSelectMode;
enum
{
    IMI_FACE_SELECT_ALL             = 0x00,
    IMI_FACE_SELECT_LARGEST         = 0x01,
    IMI_FACE_SELECT_NEAREST         = 0x02,
};

#pragma pack (push, 1)

struct ImiFaceDetectMode
{
    ImiFaceAlgorithmMode faceAlgorithmMode;
    ImiFaceSelectMode selectMode;
};

struct ImiRect
{
    int x;
    int y;
    int width;
    int height;
};

struct ImiPoint2D
{
    int x;
    int y;
};

#define UINT8_BIT_0		0x01
#define UINT8_BIT_1		0x02
#define UINT8_BIT_2		0x04
#define IMI_FACE_DETECT_LAND_MARK_SIZE 5
struct ImiFaceDetectData
{
	////人脸检测
	//ImiRect bbox;     // 人脸框信息
	//ImiPoint2D landmark[5]; //每一个是一组(x,y)坐标，表示5个特征点，分别是左眼右眼鼻尖和两个嘴角
	//int profile;      // 0 frontal; 1 lefthalf; 2 right half   正侧脸
	//int gender;       // added on Mar.1,2018     性别（暂时保留）
	//int age;          // 年龄（暂时保留）

	////活体 
	//float depth;      // 表示距离摄像头的距离（暂时保留）
	//float deplivscr;  // 深度活体得分
	//float irlivscr;   // 红外活体得分
	//float livscr;     // 活体检测分值

	////质量评估
	//uint8_t blur;               //1：图像清晰，0：图像模糊
	//uint8_t light;              //1：光照正常，0：光照不满足
	//uint8_t cover;              //1：无遮挡，0：有遮挡
	//uint8_t angle;              //1：姿态正常，0：姿态角度过大
	//uint8_t eye_distance_status;        //1：眼间距正常，0：眼间距不满足预定值
	//uint8_t expression;          //1:表情正常，0：表情不满足
	//uint8_t more_face;           //1：1个人脸，0：多人脸
	//uint8_t resolution;           //分辨率，三种模式下都未进行判断，需在外端判断，bctc:竖直方向低于640或水平方向低于480的图像不满足
	//uint8_t ok;                  //1：不正常图像，0：正常图像，默认为1，以上任一条件为0时都不满足图像合格的条件
	//float eye_distance;      //眼间距信息
	//float roll;           //roll角信息
	//float pitch;          //pitch角信息
	//float yaw;           //yaw角信息
	//float quality_score;   //质量得分，只针对blur、light
	//uint8_t close_eye;         //==1为闭眼,==0 为睁眼，在 expression不满足时有效

	//uint16_t imiFaceAlgrithmMode; // 模式校验, 0-15位（每位值是0或1， 1：表示是，0：表示否）：
	//// 第0位：表示是否为人脸检测模式，
	//// 第1位：表示是否为活体检测模式，
	//// 第2位：表示是否为人脸识别模式
	//uint16_t faceFeatureDetectInUse;   // 表示landmark中的有效个数
	//float faceFeature_detect[256];     // 人脸检测特征（暂时保留）
	//uint16_t faceFeatureIdentifyInUse; // 表示faceFeature_identifier中的有效个数
	//float faceFeature_identifier[638]; // 人脸识别特征

	//uint8_t open_mouth_flag;			//是否是张嘴 1闭嘴 0张嘴
	//uint8_t temp[3];					
	//int error;							//返回错误码
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
};

struct ImiLivenessInfo
{
	int8_t vendorId[3];
	int8_t detectMode[2];
	int8_t detectAbility;
	int8_t detectAlgorithm[20];
	int8_t cameraVendor[20];
	int8_t detectThreshold[2];
	int8_t detectScore[2];
};
struct ImiFaceDetectFrame
{
   bool		isLive;
   float	livenessScore;
   uint64_t	rgbTimeStamp;
   float	rgbBrightness;
   float	rgbDefinition;
   float    bioRslt;
   ImiLivenessInfo		livenessInfo;   

   uint16_t faceCount;//检测到的人脸个数，0~6.超过6个只截取前6个结果给SDK。
   uint16_t maxfaceidx;//最大人脸的下标。0~5,其他值非法，0xffff表示该值无效。
   uint16_t colorwidth;//0:分辨率无效，表示不输出该分辨率对应的图像，其他值：彩图的宽
   uint16_t colorheight;//0:分辨率无效，表示不输出该分辨率对应的图像，其他值：彩图的高
   uint16_t depthwidth;//0:分辨率无效，表示不输出该分辨率对应的图像，其他值：深度图的宽
   uint16_t depthheight;//0:分辨率无效，表示不输出该分辨率对应的图像，其他值：深度图的高
   uint16_t infrarwidth;//0:分辨率无效，表示不输出该分辨率对应的图像，其他值：红外图的宽
   uint16_t infrarheight;//0:分辨率无效，表示不输出该分辨率对应的图像，其他值：红外图的高
   uint16_t thumbnailheight;//缩略图的高
   uint16_t thumbnailwidth;//缩略图的宽

   ImiFaceDetectData* pData;
   int		error_code;

   void *pColorData;
   void *pThumbnailData;
   void *pDepthData;
   void *pIrData;

  // uint32_t faceDetectDataSize ;
   uint32_t size;
   uint32_t colorFrameSize;
   uint32_t depthFrameSize;
   uint32_t irFrameSize;
   uint32_t thumbnailSize;
   uint32_t frameNum;
   uint64_t timeStamp;

 
};

//typedef struct tagFaceInfoList
//{
//    uint64_t timeStamp;
//    uint16_t faceCount;
//    FaceInfo faces[];   //faceCount * sizeof(FaceInfo)
//}FaceInfoList;

#pragma pack(pop)


#if defined(_WIN32)

typedef void (_stdcall* ImiNewFaceDetectFrameCallback)(ImiSensorHandle faceDetectSensor, void* pUserData);

#else

typedef void (* ImiNewFaceDetectFrameCallback)(ImiSensorHandle faceDetectSensor, void* pUserData);

#endif

#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_FACE_DETECT_DEFINES_H_
