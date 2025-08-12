/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiPositionTrackDefines.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : PositionTrack Defines
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_POSITION_TRACK_DEFINES_H_
#define _IMI_POSITION_TRACK_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>


#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup iminect
* @{
*/
//System APIs

enum ImiPositionTrackProperties
{
    IMI_PROPERTY_SLAM_CONTROL = 0x41
};


enum SlamPoseTrackingState
{
    CAMERA_POSE		= 0,
    RESET_POSE		= 1,
    INVALID_POSE	= 2
};

#pragma pack (push, 1)

struct ImiMat4f
{
    float data[16];
};

struct ImiSlamPoseFrame
{
    uint64_t timeStamp;
    uint32_t frameNum;
    SlamPoseTrackingState trackState;
    ImiMat4f cameraPose;
};

struct ImiAccData
{
    ImiVector4 acc;
};

struct ImiPositionTrackMode
{
    //ImiPositionTrackAlgorithmMode algorithmMode;

};

#pragma pack(pop)


struct ImiPositionTrackFrame;
typedef ImiPositionTrackFrame* ImiPositionTrackFrameHandle;


#if defined(_WIN32)

typedef void (_stdcall* ImiNewPositionTrackFrameCallback)(ImiSensorHandle positionTrackSensor, void* pUserData);

#else

typedef void (* ImiNewPositionTrackFrameCallback)(ImiSensorHandle positionTrackSensor, void* pUserData);

#endif


#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_POSITION_TRACK_DEFINES_H_
