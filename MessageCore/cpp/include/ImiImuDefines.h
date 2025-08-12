#ifndef _IMI_IMU_DEFINES_H_
#define _IMI_IMU_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>


#ifdef __cplusplus
extern "C" {
#endif

struct ImiImuFrame;
typedef ImiImuFrame* ImiImuFrameHandle;

#if defined(_WIN32)

typedef void (_stdcall* ImiNewImuFrameCallback)(ImiSensorHandle imuSensor, void* pUserData);

#else

typedef void (* ImiNewImuFrameCallback)(ImiSensorHandle imuSensor, void* pUserData);

#endif


#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_POSITION_TRACK_DEFINES_H_
