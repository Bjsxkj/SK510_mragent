#ifndef _IMI_API_IMU_SENSOR_H_
#define _IMI_API_IMU_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>
#include <ImiImuDefines.h>

IMINECT_EXP int32_t imiCreateImuSensor(ImiDeviceHandle device, ImiSensorHandle* pImuSensor);
IMINECT_EXP int32_t imiDeleteImuSensor(ImiSensorHandle imuSensor);
IMINECT_EXP int32_t imiGetSupportImuMode(ImiSensorHandle imuSensor, const uint32_t** pMode, uint32_t* pNumber);
IMINECT_EXP const uint32_t* imiGetCurrentImuMode(ImiSensorHandle positionTrackSensor);
IMINECT_EXP int32_t imiSetImuMode(ImiSensorHandle imuSensor, ImiImuMode* pMode);
IMINECT_EXP int32_t imiStartImuSensor(ImiSensorHandle imuSensor, ImiNewImuFrameCallback callback);
IMINECT_EXP int32_t imiStopImuSensor(ImiSensorHandle imuSensor);
IMINECT_EXP int32_t imiReadImuFrame(ImiSensorHandle imuSensor, ImiImuFrameHandle* ppFrame, int32_t timeout);
IMINECT_EXP int32_t imiReleaseImuFrame(ImiImuFrameHandle* pFrame);
IMINECT_EXP int32_t imiImuFrameAddRef(ImiImuFrameHandle* pFrame);
IMINECT_EXP int32_t imiSetImuProperty(ImiSensorHandle imuSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);
IMINECT_EXP int32_t imiGetImuProperty(ImiSensorHandle imuSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

#endif // _IMI_API_POSITION_TRACK_SENSOR_H_
