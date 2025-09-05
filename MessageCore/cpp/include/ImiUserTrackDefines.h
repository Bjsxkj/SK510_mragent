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
#ifndef _IMI_USER_TRACK_DEFINES_H_
#define _IMI_USER_TRACK_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \add to group iminect
* @{
*/
//System APIs

#define MAX_TRSCK_USER_NUM 6
#define MAX_TRSCK_HAND_NUM 2

enum ImiUserTrackProperties
{
    IMI_PROPERTY_USERTRACK_SKELETON_SMOOTH  = 1, // value type uint8_t, 1:smooth, 0: not smooth
    IMI_PROPERTY_USERTRACK_GROUND_CLEANUP   = 2, // value type uint8_t, 1:clear the ground, 0: don't clear the ground, default 0
    IMI_PROPERTY_USERTRACK_OUPUT_FOREGROUND = 3, // value type uint8_t, 1: 输出前景图，  0：不输出前景图
    IMI_PROPERTY_USERTRACK_OUPUT_SEG_MASK   = 4, // value type uint8_t, 1: 输出抠图mask，0：不输出抠图mask
    IMI_PROPERTY_USERTRACK_GROUND_EQUATION  = 5,
	IMI_PROPERTY_USERTRACK_OUPUT_PSCTRL     = 6, //1 输出体感枪结果 0 ：不输出体感枪结果

};

enum ImiBodySkeletonPositionIndex
{
    IMI_BODY_SKELETON_HIP_CENTER      = 0,
    IMI_BODY_SKELETON_SPINE           = 1,
    IMI_BODY_SKELETON_SHOULDER_CENTER = 2,
    IMI_BODY_SKELETON_HEAD            = 3,
    IMI_BODY_SKELETON_SHOULDER_LEFT   = 4,
    IMI_BODY_SKELETON_ELBOW_LEFT      = 5,
    IMI_BODY_SKELETON_WRIST_LEFT      = 6,
    IMI_BODY_SKELETON_HAND_LEFT       = 7,
    IMI_BODY_SKELETON_SHOULDER_RIGHT  = 8,
    IMI_BODY_SKELETON_ELBOW_RIGHT     = 9,
    IMI_BODY_SKELETON_WRIST_RIGHT     = 10,
    IMI_BODY_SKELETON_HAND_RIGHT      = 11,
    IMI_BODY_SKELETON_HIP_LEFT        = 12,
    IMI_BODY_SKELETON_KNEE_LEFT       = 13,
    IMI_BODY_SKELETON_ANKLE_LEFT      = 14,
    IMI_BODY_SKELETON_FOOT_LEFT       = 15,
    IMI_BODY_SKELETON_HIP_RIGHT       = 16,
    IMI_BODY_SKELETON_KNEE_RIGHT      = 17,
    IMI_BODY_SKELETON_ANKLE_RIGHT     = 18,
    IMI_BODY_SKELETON_FOOT_RIGHT      = 19,
    IMI_BODY_SKELETON_COUNT           = 20
};

enum ImiBodySkeletonPositionTrackState
{
    IMI_BODY_SKELETON_POSITION_NOT_TRACKED = 0,
    IMI_BODY_SKELETON_POSITION_INFERRED    = 1,
    IMI_BODY_SKELETON_POSITION_TRACKED     = 2
};

enum ImiSkeletonTrackState
{
    IMI_SKELETON_NOT_TRACKED   = 0, // The user is not selected and is not available for selection
    IMI_SKELETON_POSITION_ONLY = 1, // The user is not selected and is available for selection,
    IMI_SKELETON_TRACKED       = 2, // The user is being tracked and the skeleton position is valid
    IMI_SKELETON_TRACKING      = 3, // The user have been selected but is not tracked yet
    IMI_SKELETON_FAILED        = 4  // The user is selected but failed to be tracked.
};

enum ImiFaceTrackState
{
    IMI_FACE_NOT_TRACKED   = 0, // The user is not selected and is not available for selection
    IMI_FACE_POSITION_ONLY = 1, // The user is not selected and is available for selection,
    IMI_FACE_TRACKED       = 2, // The user is being tracked and the skeleton position is valid
    IMI_FACE_TRACKING      = 3, // The user have been selected but is not tracked yet
    IMI_FACE_FAILED        = 4  // The user is selected but failed to be tracked.
};

enum ImiPoseState
{
    IMI_POSE_UNSURE = 0,
    IMI_POSE_STAND  = 1,
    IMI_POSE_SEATED = 2
};

enum ImiHandState
{
    IMI_HAND_UNSURE = 0,
    IMI_HAND_GRASP  = 1,
    IMI_HAND_OPEN   = 2
};

enum ImiSkeletonOrientationState
{
    IMI_SKELETON_ORIENTATION_AVAILABLE   = 0,
    IMI_SKELETON_ORIENTATION_UNAVAILABLE = 1,
    IMI_SKELETON_ORIENTATION_UNDEFINED   = 2
};

#pragma pack (push, 1)

struct ImiSkeletonOrientationData
{
    ImiMat3f hierarchicalRotation; // 本地旋转矩阵, 相对于父节点
    ImiMat3f absoluteRotation;     // 绝对旋转矩阵, 相对于世界坐标系
    ImiVector4 eulerAngles;        // 欧拉角, 相对于父节点, 旋转顺序暂定为Z-X-Y
};

struct ImiClothesFeature
{
    float clothesFeatures[128]; // 服装特征向量
};


struct ImiFaceFeature_V0 {
    // 开始版的人脸特征，对应A300，S300设备
    IMI_Rect faceRect;       // 人脸框的位置
	uint32_t faceSize;       // 人脸尺寸
    float faceFeatures[256]; // 人脸特征向量
};


struct ImiFaceFeature_V2
{   
    // 2024-12-12 定义
    IMI_Rect faceRect; 
    uint32_t faceSize;       // 人脸尺寸
    uint32_t faceID;         // 人脸ID
    uint32_t age;            // 年龄
    uint32_t gender;         // 性别
    uint32_t confiendence;   //置信度  // 表情
    float faceFeatures[512]; // 人脸特征向量
};


struct ImiFaceFeatureData
{
    uint32_t trackingID;        // 人脸跟踪ID
    uint8_t flags;              // flags = 1 时 faceFeature 有效
    ImiFaceFeature_V0 faceFeature; // 人脸特征
};

struct ImiFaceFeatureFrame
{
    uint64_t timeStamp; // 时间戳
    uint32_t frameNum;  // 帧索引
    uint32_t width;
    uint32_t height;
    ImiFaceFeatureData faceFeatureDatas[MAX_TRSCK_USER_NUM]; // 人脸特征数据
};



struct ImiFeature_V1
{
    uint8_t				faceFeatureFlags; 
    ImiFaceFeature_V2		faceFeature;
    uint8_t				clothesFeatureFlags; 
    ImiClothesFeature	clothesFeature;
};

struct ImiFeature_V2
{
	uint8_t faceFeatureFlags; // 1：faceFeature有效， 0：faceFeature无效
	uint8_t clothesFeatureFlags; // // 1：clothesFeature有效， 0：clothesFeature无效
	uint8_t rev[2]; //为了保持4字节对齐, add20241212

	ImiFaceFeature_V2 faceFeature;
	ImiClothesFeature clothesFeature;

};

typedef ImiFeature_V1 ImiFeature;
typedef ImiFaceFeature_V0 ImiFaceFeature;

struct ImiBodySkeletonData_V0
{
    ImiSkeletonTrackState trackingState;                                                       // 骨架跟踪状态
    uint32_t trackingID;                                                                       // 骨架跟踪ID
    ImiVector4 centerPosition;                                                                 // 人体中心位置坐标
    ImiVector4 skeletonPositions[IMI_BODY_SKELETON_COUNT];                                     // 骨架关节点坐标
    ImiBodySkeletonPositionTrackState skeletonPositionTrackingStates[IMI_BODY_SKELETON_COUNT]; // 骨架各关节点跟踪状态
    uint32_t qualityFlags;                                                                     // 骨架出界情况, 一共4位, 分别为上、下、左、右
    ImiSkeletonOrientationState upSkeletonsOrientationState;                                   // 骨架上半身相对于世界坐标系的旋转矩阵的可用状态
    ImiMat3f upSkeletonsOrientation;                                                           // 骨架上半身相对于世界坐标系的旋转矩阵
    ImiSkeletonOrientationState downSkeletonsOrientationState;                                 // 骨架下半身相对于世界坐标系的旋转矩阵的可用状态
    ImiMat3f downSkeletonsOrientation;                                                         // 骨架下半身相对于世界坐标系的旋转矩阵
    ImiSkeletonOrientationState skeletonOrientationStates[IMI_BODY_SKELETON_COUNT];            // 骨架关节点方向信息的可用状态
    ImiSkeletonOrientationData skeletonOrientations[IMI_BODY_SKELETON_COUNT];                  // 骨架所有关节点的方向信息, 对于没有此类信息的关节点, 显示说明该索引上的数据不可用
    ImiPoseState poseState;     
};

struct ImiBodySkeletonData_V1
{
    ImiSkeletonTrackState trackingState;                                                       // 骨架跟踪状态
    uint32_t trackingID;                                                                       // 骨架跟踪ID
    ImiVector4 centerPosition;                                                                 // 人体中心位置坐标
    ImiVector4 skeletonPositions[IMI_BODY_SKELETON_COUNT];                                     // 骨架关节点坐标
    ImiBodySkeletonPositionTrackState skeletonPositionTrackingStates[IMI_BODY_SKELETON_COUNT]; // 骨架各关节点跟踪状态
    uint32_t qualityFlags;                                                                     // 骨架出界情况, 一共4位, 分别为上、下、左、右
    ImiSkeletonOrientationState upSkeletonsOrientationState;                                   // 骨架上半身相对于世界坐标系的旋转矩阵的可用状态
    ImiMat3f upSkeletonsOrientation;                                                           // 骨架上半身相对于世界坐标系的旋转矩阵
    ImiSkeletonOrientationState downSkeletonsOrientationState;                                 // 骨架下半身相对于世界坐标系的旋转矩阵的可用状态
    ImiMat3f downSkeletonsOrientation;                                                         // 骨架下半身相对于世界坐标系的旋转矩阵
    ImiSkeletonOrientationState skeletonOrientationStates[IMI_BODY_SKELETON_COUNT];            // 骨架关节点方向信息的可用状态
    ImiSkeletonOrientationData skeletonOrientations[IMI_BODY_SKELETON_COUNT];                  // 骨架所有关节点的方向信息, 对于没有此类信息的关节点, 显示说明该索引上的数据不可用
    ImiPoseState poseState;     
    //todo bjliu 
    ImiFeature_V1 feature;

    ImiHandState handState;
	IMI_Rect  handRect;                                                               // 骨架姿态
    //end bjliu
};

struct ImiBodySkeletonData_V2
{
    ImiSkeletonTrackState trackingState;                                                       // 骨架跟踪状态
    uint32_t trackingID;                                                                       // 骨架跟踪ID
    ImiVector4 centerPosition;                                                                 // 人体中心位置坐标
    ImiVector4 skeletonPositions[IMI_BODY_SKELETON_COUNT];                                     // 骨架关节点坐标
    ImiBodySkeletonPositionTrackState skeletonPositionTrackingStates[IMI_BODY_SKELETON_COUNT]; // 骨架各关节点跟踪状态
    uint32_t qualityFlags;                                                                     // 骨架出界情况, 一共4位, 分别为上、下、左、右
    ImiSkeletonOrientationState upSkeletonsOrientationState;                                   // 骨架上半身相对于世界坐标系的旋转矩阵的可用状态
    ImiMat3f upSkeletonsOrientation;                                                           // 骨架上半身相对于世界坐标系的旋转矩阵
    ImiSkeletonOrientationState downSkeletonsOrientationState;                                 // 骨架下半身相对于世界坐标系的旋转矩阵的可用状态
    ImiMat3f downSkeletonsOrientation;                                                         // 骨架下半身相对于世界坐标系的旋转矩阵
    ImiSkeletonOrientationState skeletonOrientationStates[IMI_BODY_SKELETON_COUNT];            // 骨架关节点方向信息的可用状态
    ImiSkeletonOrientationData skeletonOrientations[IMI_BODY_SKELETON_COUNT];                  // 骨架所有关节点的方向信息, 对于没有此类信息的关节点, 显示说明该索引上的数据不可用
    ImiPoseState poseState;     
    //todo bjliu 
    ImiFeature_V2 feature;

    ImiHandState handState;
	IMI_Rect  handRect;                                                               // 骨架姿态
    //end bjliu
};

typedef ImiBodySkeletonData_V2 ImiBodySkeletonData;

struct ImiAngleAudioData
{
    uint32_t size;  
    uint8_t* data;  // 数据
};

struct ImiSkeletonFrame
{
    uint64_t timeStamp; // 时间戳
    uint32_t frameNum;  // 帧索引
    uint32_t width;
    uint32_t height;
    ImiVector4 floorClipPlane; // 地面方程
    ImiBodySkeletonData skeletonDatas[MAX_TRSCK_USER_NUM]; // 骨架数据
};

struct ImiForegroundData
{
    uint32_t width;
    uint32_t height;
    uint32_t size;
    uint16_t* foregroundData; // 前景图数据
};

struct Imi3DMattingData
{
    uint32_t width;
    uint32_t height;
    uint32_t size;
    uint8_t* mattingData; // 抠图mask数据
};


struct ImiPsCtrlData
{
	short  color;              // 0：蓝色 1：红色 2：黄色 3：绿色 其它：无效
	short  rightorleft;        // 0:左手，1：右手；其它：无效	short  x;//小球图像坐标x   
	short  y;//小球图像坐标y
	short  z;//小球深度坐标z
	short  Rad;//小球半径
	float  yaw ;//偏航角 
	float  pitch;//俯仰角
	float  roll;//横滚角
};

struct ImiPsCtrlframeData
{
	uint32_t width;				//计算光球所对应的彩图分辨率 宽	
	uint32_t height;			//计算光球所对应的彩图分辨率 高
	uint8_t  psctrlNum;          // 光球个数 最大4个，大于4只取前四个。
	ImiPsCtrlData  psCtrlData[4]; // 人脸特征
};
//todo bjliu
struct ImiEchoFileData
{
    const char* depthName;
    const char* colorName;
};
//end bjliu
#pragma pack(pop)

enum ImiHandSkeletonPositionIndex
{
    //手腕
    IMI_HAND_SKELETON_WRIST = 0,

    //手掌五根手指根关节
    IMI_HAND_SKELETON_TMCP = 1, // 拇指
    IMI_HAND_SKELETON_IMCP = 2, // 食指
    IMI_HAND_SKELETON_MMCP = 3, // 中指
    IMI_HAND_SKELETON_RMCP = 4, // 无名指
    IMI_HAND_SKELETON_PMCP = 5, // 小指

    //大拇指指关节
    IMI_HAND_SKELETON_TPIP = 6,
    IMI_HAND_SKELETON_TDIP = 7,
    IMI_HAND_SKELETON_TTIP = 8,

    //食指指关节
    IMI_HAND_SKELETON_IPIP = 9,
    IMI_HAND_SKELETON_IDIP = 10,
    IMI_HAND_SKELETON_ITIP = 11,

    //中指指关节
    IMI_HAND_SKELETON_MPIP = 12,
    IMI_HAND_SKELETON_MDIP = 13,
    IMI_HAND_SKELETON_MTIP = 14,

    //无名指指关节
    IMI_HAND_SKELETON_RPIP = 15,
    IMI_HAND_SKELETON_RDIP = 16,
    IMI_HAND_SKELETON_RTIP = 17,

    //小拇指指关节
    IMI_HAND_SKELETON_PPIP = 18,
    IMI_HAND_SKELETON_PDIP = 19,
    IMI_HAND_SKELETON_PTIP = 20,

    IMI_HAND_SKELETON_COUNT
};

enum ImiHandType
{
    IMI_HAND_LEFT  = 0,
    IMI_HAND_RIGHT = 1
};

#pragma pack (push, 1)

struct ImiHandSkeletonData
{
    ImiSkeletonTrackState trackingState;                                      // 手势骨架跟踪状态
    uint32_t trackingID;                                                      // 手势骨架跟踪ID
    ImiHandType handType;                                                     // 左手或右手
    ImiHandState handState;                                                   // 手的状态
    ImiVector4 wristPosition;                                                 // 手腕坐标位置
    ImiVector4 skeletonPositions[IMI_HAND_SKELETON_COUNT];                    // 手势关节点坐标
    ImiSkeletonOrientationData skeletonOrientations[IMI_HAND_SKELETON_COUNT]; // 关节方向数据
};

struct ImiHandSkeletonFrame
{
    uint64_t timeStamp; //时间戳
    uint32_t frameNum;  //帧索引
    uint32_t width;
    uint32_t height;
    ImiHandSkeletonData handSkeletonDatas[MAX_TRSCK_HAND_NUM]; // 手部骨架数据
};

#pragma pack(pop)



#pragma pack (push, 1)
struct ImiMaskFrame
{
    uint64_t		timeStamp;						//时间戳
    uint32_t		frameNum;						//帧索引
	uint8_t			finshFlag;						//精分割完成标识 0：未完成，1：完成

	uint32_t		depthWidth;						//深度宽
	uint32_t		depthHeight;					//深度高
	uint32_t		depthSize;
	uint8_t*		depthData;						//depth数据

	uint32_t		projectWidth;					//投影数据宽
    uint32_t		projectHeight;					//投影数据高
	uint32_t		projectSize;					//投影数据大小
	uint8_t*		projectData;					//投影数据段

    uint32_t		maskWidth;						//mask数据宽
    uint32_t		maskHeight;						//mask数据高
	uint32_t		maskSize;
    uint8_t*		maskDatas;						// mask数据
};

struct ImiBlobFrame
{
	uint64_t		timeStamp;						//时间戳
    uint32_t		frameNum;						//帧索引
	uint8_t			finshFlag;						//blob分割完成标识 0：未完成，1：完成

	uint32_t		depthWidth;						//深度宽
	uint32_t		depthHeight;					//深度高
	uint32_t		depthSize;						//深度Size
	uint8_t*		depthData;						//depth数据

	uint32_t		blobWidth;
	uint32_t		blobHeight;
	uint32_t		blobSize;
	uint8_t*		blobDatas;
};

struct ImiIMUData
{
	float x;
	float y;
	float z;
};
struct ImuImuState
{
	uint8_t imuState;		//0:正常 1:不正常
	ImiIMUData imuData;		// imu data 
};
#pragma pack(pop)

struct UserTrackFrame;
typedef UserTrackFrame* ImiUserTrackFrameHandle;

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

#pragma pack (push, 1)

struct ImiUserTrackMode
{
    ImiUserTrackAlgorithmMode userTrackType;
    ImiPoseState poseState;
    uint32_t width;
    uint32_t height;
    uint32_t fps;
};

// todo: add at 2024.11.12

/// @brief 
struct ImiFaceAttrData {

    IMI_Rect faceRect;       // 人脸框的位置(相对640x480)

    uint32_t faceSize;       // 人脸尺寸

    int nFaceID;

    int nAge;

    int nGender;

    int nSimilar;

    float faceFeatures[512]; // 人脸特征向量
};

/// @brief 
struct ImiSkeletonExDataItem {
    ImiHandState handState[2];
    int hardId; 
    ImiFaceAttrData faceAttr;
};

/// @brief 
struct ImiSkeletonExData {
    ImiSkeletonExDataItem data[6];
};

#pragma pack(pop)

#if defined(_WIN32)
typedef void (_stdcall* ImiNewUserTrackFrameCallback)(ImiSensorHandle userTrackSensor, void* pUserData);
#else
typedef void (* ImiNewUserTrackFrameCallback)(ImiSensorHandle userTrackSensor, void* pUserData);
#endif

#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_USER_TRACK_DEFINES_H_
