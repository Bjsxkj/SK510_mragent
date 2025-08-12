/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiPositionTrackSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : Position TrackSensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_POSITION_TRACK_SENSOR_H_
#define _IMI_API_POSITION_TRACK_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>
#include <ImiPositionTrackDefines.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreatePositionTrackSensor(ImiDeviceHandle device, ImiSensorHandle* pPositionTrackSensor);

IMINECT_EXP int32_t imiDeletePositionTrackSensor(ImiSensorHandle positionTrackSensor);

IMINECT_EXP int32_t imiGetSupportPositionTrackMode(ImiSensorHandle positionTrackSensor, const ImiPositionTrackMode** pMode, uint32_t* pNumber);

IMINECT_EXP const ImiPositionTrackMode* imiGetCurrentPositionTrackMode(ImiSensorHandle positionTrackSensor);

IMINECT_EXP int32_t imiSetPositionTrackMode(ImiSensorHandle positionTrackSensor, ImiPositionTrackMode* pMode);

IMINECT_EXP int32_t imiStartPositionTrackSensor(ImiSensorHandle positionTrackSensor, ImiNewPositionTrackFrameCallback callback);

IMINECT_EXP int32_t imiStopPositionTrackSensor(ImiSensorHandle positionTrackSensor);

IMINECT_EXP int32_t imiReadPositionTrackFrame(ImiSensorHandle positionTrackSensor, ImiPositionTrackFrameHandle* ppFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleasePositionTrackFrame(ImiPositionTrackFrameHandle* pFrame);

IMINECT_EXP int32_t imiPositionTrackFrameAddRef(ImiPositionTrackFrameHandle* pFrame);

IMINECT_EXP int32_t imiGetSlamPoseFrame(ImiPositionTrackFrameHandle frame, ImiSlamPoseFrame* pFrame);

IMINECT_EXP int32_t imiGetImuData(ImiPositionTrackFrameHandle frame, ImiAccData* pData);

//IMINECT_EXP int32_t imiGetMoveDataList(ImiPositionTrackFrameHandle frame, ImiMoveTrackDataList* pDataList);

IMINECT_EXP int32_t imiSetPositionTrackProperty(ImiSensorHandle positionTrackSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetPositionTrackProperty(ImiSensorHandle positionTrackSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);


/*! @} */
#endif // _IMI_API_POSITION_TRACK_SENSOR_H_
