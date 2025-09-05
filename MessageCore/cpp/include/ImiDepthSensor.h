/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiDepthSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : Depth Sensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_DEPTH_SENSOR_H_
#define _IMI_API_DEPTH_SENSOR_H_

#include <ImiPlatform.h>
#include <ImiDefines.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateDepthSensor(ImiDeviceHandle device, ImiSensorHandle* pDepthSensor);

IMINECT_EXP int32_t imiDeleteDepthSensor(ImiSensorHandle depthSensor);

IMINECT_EXP int32_t imiGetSupportDepthFrameMode(ImiSensorHandle depthSensor, const ImiFrameMode** pMode, uint32_t* pNumber);

IMINECT_EXP const ImiFrameMode* imiGetCurrentDepthFrameMode(ImiSensorHandle depthSensor);

IMINECT_EXP int32_t imiSetDepthFrameMode(ImiSensorHandle depthSensor, ImiFrameMode* pMode);

IMINECT_EXP int32_t imiStartDepthSensor(ImiSensorHandle depthSensor, ImiNewFrameCallback callback, void* pUserData = 0);

IMINECT_EXP int32_t imiStopDepthSensor(ImiSensorHandle depthSensor);

IMINECT_EXP int32_t imiReadNextDepthFrame(ImiSensorHandle depthSensor, ImiImageFrame** ppFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleaseDepthFrame(ImiImageFrame** pFrame);

IMINECT_EXP int32_t imiDepthFrameAddRef(ImiImageFrame** pFrame);

IMINECT_EXP int32_t imiSetDepthProperty(ImiSensorHandle depthSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetDepthProperty(ImiSensorHandle depthSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

// 其他设置，比如彩色曝光等设置



/*! @} */
#endif // _IMI_API_DEPTH_SENSOR_H_
