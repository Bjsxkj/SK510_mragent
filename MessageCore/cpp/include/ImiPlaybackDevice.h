/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiPlaybackDevice.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : PlaybackDevice Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_PLAYBACK_DEVICE_H_
#define _IMI_API_PLAYBACK_DEVICE_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiOpenPlaybackDevice(const char* pDeviceUri, ImiDeviceHandle* pDevice, int32_t reserve);

IMINECT_EXP int32_t imiClosePlaybackDevice(ImiDeviceHandle device);

IMINECT_EXP int32_t imiGetPlaybackSensorInfo(ImiDeviceHandle device, PlaybackSensorInfo** pSensorInfo, uint32_t* nCount);

IMINECT_EXP int32_t imiPlaybackSeek(ImiDeviceHandle device, uint32_t nPos);

IMINECT_EXP int32_t imiPlaybackPause(ImiDeviceHandle device);

IMINECT_EXP int32_t imiPlaybackResume(ImiDeviceHandle device);

/*! @} */
#endif // _IMI_API_PLAYBACK_DEVICE_H_
