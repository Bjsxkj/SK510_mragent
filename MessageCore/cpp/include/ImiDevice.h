/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiDevice.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : Device Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_DEVICE_H_
#define _IMI_API_DEVICE_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiOpenDevice(const char* pDeviceUri, ImiDeviceHandle* pDevice, int32_t reserve);

IMINECT_EXP int32_t imiOpenDevice2(uint32_t vid, uint32_t pid, uint32_t fd, const char* pUsbFsPath, ImiDeviceHandle* pDevice, int32_t reserve);

IMINECT_EXP int32_t imiCloseDevice(ImiDeviceHandle device);

IMINECT_EXP int32_t imiSetDeviceProperty(ImiDeviceHandle device, uint32_t propertyId, const void* pData, uint32_t dataSize);

IMINECT_EXP int32_t imiGetDeviceProperty(ImiDeviceHandle device, uint32_t propertyId, void* pData, uint32_t* pDataSize);

IMINECT_EXP int32_t imiSetAdbSwitch(ImiDeviceHandle device, const uint32_t* pData, uint32_t dataSize);

IMINECT_EXP int32_t imiGetVersion(ImiDeviceHandle device, ImiVersions* pImiVersion);

IMINECT_EXP int32_t imiGetVersion2280(ImiDeviceHandle device, ImiVersions2280* pImiVersion);

IMINECT_EXP int32_t imiGetSensorList(ImiDeviceHandle pDevice, ImiSensorAttribute** pSensorList, uint32_t* pSensorCount);

IMINECT_EXP int32_t imiReleaseSensorList(ImiSensorAttribute** pSensorList);

IMINECT_EXP int32_t imiGetSupportCapacity(ImiDeviceHandle device, ImiSupportCapacity* pImiSupportCapacity);

IMINECT_EXP int32_t imiSendBulkData(ImiDeviceHandle device, const char* datatPath);

IMINECT_EXP int32_t imiGetKernelLog(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetBootLog(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetImiappLog(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetIspappLog(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetDsp0Log(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetMpuLog(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetSecLog(ImiDeviceHandle device,uint8_t* log,uint64_t& size);

IMINECT_EXP int32_t imiGetSDKVersion(const char* version,uint32_t* len);

IMINECT_EXP int32_t imiGetDsp0FwVersion(ImiDeviceHandle device,const char* version,uint32_t* len);

IMINECT_EXP int32_t imiSetMode(ImiDeviceHandle device, DetctMode mode);

IMINECT_EXP int32_t imiGetCameraInfo(ImiDeviceHandle device,float* info,uint32_t* nLen);

IMINECT_EXP int32_t imiGetAlgInfo(ImiDeviceHandle device,AlgInfo* info,uint32_t* len);

IMINECT_EXP int32_t imiSendDiagParam(ImiDeviceHandle device, uint32_t* data);

IMINECT_EXP int32_t imiSendBulkLogParam(ImiDeviceHandle device, uint32_t* data);

IMINECT_EXP int32_t imiGetBulkLog(ImiDeviceHandle device, const char* datatPath);

IMINECT_EXP int32_t imiRegisterTemperatureExceptionCallback(ImiDeviceHandle device,ImiTemperatureExceptionCallback callback);

IMINECT_EXP int32_t imiUnRegisterTemperatureExceptionCallback(ImiDeviceHandle device);
/*! @} */
#endif // _IMI_API_DEVICE_H_
