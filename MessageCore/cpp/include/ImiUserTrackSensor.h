/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiUserTrackSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : UserTrack Sensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_USER_TRACK_SENSOR_H_
#define _IMI_API_USER_TRACK_SENSOR_H_

#include <ImiPlatform.h>
#include <ImiDefines.h>
#include <ImiUserTrackDefines.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateUserTrackSensor(ImiDeviceHandle device, ImiSensorHandle* pUserTrackSensor);

IMINECT_EXP int32_t imiDeleteUserTrackSensor(ImiSensorHandle userTrackSensor);

IMINECT_EXP int32_t imiGetSupportUserTrackMode(ImiSensorHandle userTrackSensor, const ImiUserTrackMode** pMode, uint32_t* pNumber);

IMINECT_EXP int32_t imiSetUserTrackMode(ImiSensorHandle userTrackSensor, ImiUserTrackMode* pMode);

IMINECT_EXP const ImiUserTrackMode* imiGetCurrentUserTrackMode(ImiSensorHandle userTrackSensor);

IMINECT_EXP int32_t imiStartUserTrackSensor(ImiSensorHandle userTrackSensor, ImiNewFrameCallback callback, void* pUserData = 0);

IMINECT_EXP int32_t imiStopUserTrackSensor(ImiSensorHandle userTrackSensor);

IMINECT_EXP int32_t imiReadUserTrackFrame(ImiSensorHandle userTrackSensor, ImiUserTrackFrameHandle* pFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleaseUserTrackFrame(ImiUserTrackFrameHandle* pFrame);

IMINECT_EXP int32_t imiUserTrackFrameAddRef(ImiUserTrackFrameHandle* pFrame);

//IMI_TRACK_MODE_FACE
IMINECT_EXP int32_t imiStartDetectFaceByID(ImiSensorHandle userTrackSensor, int32_t userId);

IMINECT_EXP int32_t imiStopDetectFaceByID(ImiSensorHandle userTrackSensor, int32_t userId);

// get data from frame handle

IMINECT_EXP int32_t imiGetFrameBodySkeleton(ImiUserTrackFrameHandle frameHandle, ImiSkeletonFrame* pFrame);

IMINECT_EXP int32_t imiGetFrameHandSkeleton(ImiUserTrackFrameHandle frameHandle, ImiHandSkeletonFrame* pFrame);

IMINECT_EXP int32_t imiGetFrameFaceFeature(ImiUserTrackFrameHandle frameHandle, ImiFaceFeatureFrame* pFrame);

IMINECT_EXP int32_t imiGetFrameForAngleAudioData(ImiUserTrackFrameHandle frameHandle, ImiAngleAudioData* pData);

IMINECT_EXP int32_t imiGetFrameForImuData(ImiUserTrackFrameHandle frameHandle, ImiAngleAudioData* pData);

IMINECT_EXP int32_t imiGetFrameForegroundData(ImiUserTrackFrameHandle frameHandle, ImiForegroundData* pData);

IMINECT_EXP int32_t imiGetFrame3DMattingData(ImiUserTrackFrameHandle frameHandle, Imi3DMattingData* pData);

IMINECT_EXP int32_t imiGetFrameBlobData(ImiUserTrackFrameHandle frameHandle,ImiBlobFrame* pData);

IMINECT_EXP	int32_t imiGetFrameMaskData(ImiUserTrackFrameHandle frameHandle,ImiMaskFrame* pData);

IMINECT_EXP int32_t imiGetPsCtrlframeData(ImiUserTrackFrameHandle frameHandle, ImiPsCtrlframeData* pData);

/// @brief 获取额外的数据
/// @param frameHandle 
/// @param pData 
/// @return 
IMINECT_EXP int32_t imiGetFrameExData(ImiUserTrackFrameHandle frameHandle, ImiSkeletonExData* pData);

// UserTrack Property

IMINECT_EXP int32_t imiSetUserTrackProperty(ImiSensorHandle userTrackSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetUserTrackProperty(ImiSensorHandle userTrackSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

IMINECT_EXP int32_t imiOpenUserTrackEcho(ImiSensorHandle userTrackSensor);

IMINECT_EXP int32_t imiCloseUserTrackEcho(ImiSensorHandle userTrackSensor);
/*! @} */
#endif // _IMI_API_USER_TRACK_SENSOR_H_
