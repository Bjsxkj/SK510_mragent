/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiColorSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : Color Sensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_COLOR_SENSOR_H_
#define _IMI_API_COLOR_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateColorSensor(ImiDeviceHandle device, ImiSensorHandle* pColorSensor);

IMINECT_EXP int32_t imiDeleteColorSensor(ImiSensorHandle colorSensor);

IMINECT_EXP int32_t imiGetSupportColorFrameMode(ImiSensorHandle colorSensor, const ImiFrameMode** pMode, uint32_t* pNumber);

IMINECT_EXP const ImiFrameMode* imiGetCurrentColorFrameMode(ImiSensorHandle colorSensor);

IMINECT_EXP int32_t imiSetColorFrameMode(ImiSensorHandle colorSensor, ImiFrameMode* pMode);

IMINECT_EXP int32_t imiStartColorSensor(ImiSensorHandle colorSensor, ImiNewFrameCallback callback, void* pUserData = 0);

IMINECT_EXP int32_t imiStopColorSensor(ImiSensorHandle colorSensor);

IMINECT_EXP int32_t imiReadNextColorFrame(ImiSensorHandle colorSensor, ImiImageFrame** ppFrame, int32_t timeout);

IMINECT_EXP int32_t imiGetOtherResolutionData(ImiImageFrame** ppFrame,ImiConverResolution dstResolution,void* dstData,int32_t* dstDataLen);

IMINECT_EXP int32_t imiReleaseColorFrame(ImiImageFrame** pFrame);

IMINECT_EXP int32_t imiColorFrameAddRef(ImiImageFrame** pFrame);

IMINECT_EXP int32_t imiSetColorProperty(ImiSensorHandle colorSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetColorProperty(ImiSensorHandle colorSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

// ????,?????????

/*! @} */
#endif // _IMI_API_COLOR_SENSOR_H_
