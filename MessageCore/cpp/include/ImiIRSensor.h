/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiIRSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : IR Sensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_IR_SENSOR_H_
#define _IMI_API_IR_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateIRSensor(ImiDeviceHandle device, ImiSensorHandle* pIrSensor);

IMINECT_EXP int32_t imiDeleteIRSensor(ImiSensorHandle irSensor);

IMINECT_EXP int32_t imiGetSupportIRFrameMode(ImiSensorHandle irSensor, const ImiFrameMode** pMode, uint32_t* pNumber);

IMINECT_EXP const ImiFrameMode* imiGetCurrentIRFrameMode(ImiSensorHandle irSensor);

IMINECT_EXP int32_t imiSetIRFrameMode(ImiSensorHandle irSensor, ImiFrameMode* pMode);

IMINECT_EXP int32_t imiStartIRSensor(ImiSensorHandle irSensor, ImiNewFrameCallback callback, void* pUserData = 0);

IMINECT_EXP int32_t imiStopIRSensor(ImiSensorHandle irSensor);

IMINECT_EXP int32_t imiReadNextIRFrame(ImiSensorHandle irSensor, ImiImageFrame** ppFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleaseIRFrame(ImiImageFrame** pFrame);

IMINECT_EXP int32_t imiIRFrameAddRef(ImiImageFrame** pFrame);

IMINECT_EXP int32_t imiSetIRProperty(ImiSensorHandle irSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetIRProperty(ImiSensorHandle irSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

// 其他设置，比如彩色曝光等设置

/*! @} */
#endif // _IMI_API_IR_SENSOR_H_
