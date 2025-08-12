/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiMoveTrackSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : MoveTrack Sensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_MOVE_TRACK_SENSOR_H_
#define _IMI_API_MOVE_TRACK_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>
#include <ImiMoveTrackDefines.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateMoveTrackSensor(ImiDeviceHandle device, ImiSensorHandle* pMoveSensor);

IMINECT_EXP int32_t imiDeleteMoveTrackSensor(ImiSensorHandle moveTrackSensor);

IMINECT_EXP int32_t imiGetSupportMoveTrackMode(ImiSensorHandle moveTrackSensor, const ImiMoveTrackMode** pMode, uint32_t* pNumber);

IMINECT_EXP const ImiMoveTrackMode* imiGetCurrentMoveTrackMode(ImiSensorHandle moveTrackSensor);

IMINECT_EXP int32_t imiSetMoveTrackMode(ImiSensorHandle moveTrackSensor, ImiMoveTrackMode* pMode);

IMINECT_EXP int32_t imiStartMoveTrackSensor(ImiSensorHandle moveTrackSensor, ImiNewMoveTrackFrameCallback callback);

IMINECT_EXP int32_t imiStopMoveTrackSensor(ImiSensorHandle moveTrackSensor);

IMINECT_EXP int32_t imiReadMoveTrackFrame(ImiSensorHandle moveTrackSensor, ImiMoveTrackFrameHandle* ppFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleaseMoveTrackFrame(ImiMoveTrackFrameHandle* pFrame);

IMINECT_EXP int32_t imiMoveTrackFrameAddRef(ImiMoveTrackFrameHandle* pFrame);

IMINECT_EXP int32_t imiGetMoveDataList(ImiMoveTrackFrameHandle frame, ImiMoveTrackDataList* pDataList);

IMINECT_EXP int32_t imiSetMoveTrackProperty(ImiSensorHandle moveTrackSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetMoveTrackProperty(ImiSensorHandle moveTrackSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

/*! @} */
#endif // _IMI_API_POSITION_TRACK_SENSOR_H_
