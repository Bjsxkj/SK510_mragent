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
#ifndef _IMI_API_AUDIO_SENSOR_H_
#define _IMI_API_AUDIO_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>
#include <ImiAudioDefines.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateAudioSensor(ImiDeviceHandle device, ImiSensorHandle* pAudioSensor);

IMINECT_EXP int32_t imiDeleteAudioSensor(ImiSensorHandle audioSensor);

IMINECT_EXP int32_t imiGetSupportAudioMode(ImiSensorHandle audioSensor, const uint32_t** pMode, uint32_t* pNumber);

IMINECT_EXP const uint32_t* imiGetCurrentAudioMode(ImiSensorHandle audioSensor);

IMINECT_EXP int32_t imiSetAudioMode(ImiSensorHandle audioSensor, ImiAudioMode* pMode);

IMINECT_EXP int32_t imiStartAudioSensor(ImiSensorHandle audioSensor, ImiNewAudioFrameCallback callback);

IMINECT_EXP int32_t imiStopAudioSensor(ImiSensorHandle audioSensor);

IMINECT_EXP int32_t imiReadAudioFrame(ImiSensorHandle audioSensor, ImiAudioFrameHandle* ppFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleaseAudioFrame(ImiAudioFrameHandle* pFrame);

IMINECT_EXP int32_t imiAudioFrameAddRef(ImiAudioFrameHandle* pFrame);

IMINECT_EXP int32_t imiSetAudioProperty(ImiSensorHandle audioSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetAudioProperty(ImiSensorHandle audioSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);


/*! @} */
#endif // _IMI_API_POSITION_TRACK_SENSOR_H_
