/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiFaceDetectSensor.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : FaceDetect Sensor Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_FACE_DETECT_SENSOR_H_
#define _IMI_API_FACE_DETECT_SENSOR_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>
#include <ImiPayDefines.h>
#include <ImiFaceDetectDefines.h>

/*! \addtogroup iminect
* @{
*/
//System APIs

IMINECT_EXP int32_t imiCreateFaceDetectSensor(ImiDeviceHandle device, ImiSensorHandle* pFaceDetectSensor);

IMINECT_EXP int32_t imiDeleteFaceDetectSensor(ImiSensorHandle faceDetectSensor);

IMINECT_EXP int32_t imiGetSupportFaceDetectMode(ImiSensorHandle faceDetectSensor, const ImiFaceDetectMode** pMode, uint32_t* pNumber);

IMINECT_EXP const ImiFaceDetectMode* imiGetCurrentFaceDetectMode(ImiSensorHandle faceDetectSensor);

IMINECT_EXP int32_t imiSetFaceDetectMode(ImiSensorHandle faceDetectSensor, ImiFaceDetectMode* pMode);

IMINECT_EXP int32_t imiStartFaceDetectSensor(ImiSensorHandle faceDetectSensor, ImiNewFaceDetectFrameCallback callback);

IMINECT_EXP int32_t imiStopFaceDetectSensor(ImiSensorHandle faceDetectSensor);

IMINECT_EXP int32_t imiReadFaceDetectFrame(ImiSensorHandle faceDetectSensor, ImiFaceDetectFrame** ppFaceDetectFrame, int32_t timeout);

IMINECT_EXP int32_t imiReleaseFaceDetectFrame(ImiFaceDetectFrame** ppFaceDetectFrame);

IMINECT_EXP int32_t imiFaceDetectFrameAddRef(ImiFaceDetectFrame** ppFaceDetectFrame);

IMINECT_EXP int32_t imiFaceDetectFaceIdentify(const float* pFaceFeatureIdentify1, const float* pFaceFeatureIdentify2, uint16_t imiFaceFeatureIdentifyInUse, float* pScore);

IMINECT_EXP int32_t imiSetFaceDetectProperty(ImiSensorHandle faceDetectSensor, uint32_t propertyId, void* pValue, uint32_t dataSize);

IMINECT_EXP int32_t imiGetFaceDetectProperty(ImiSensorHandle faceDetectSensor, uint32_t propertyId, void* pValue, uint32_t* dataSize);

IMINECT_EXP int32_t imiOpenEcho(ImiSensorHandle faceDetectSensor);

IMINECT_EXP int32_t imiCloseEcho(ImiSensorHandle faceDetectSensor);

IMINECT_EXP int32_t imiSetPayLisenceKey(ImiSensorHandle faceDetectSensor, const ImiPayLisenceKey* pSKey);

IMINECT_EXP int32_t imiGetPayDeviceIDKey(ImiSensorHandle device, ImiDeviceIdKey* pSKey ,uint32_t *nKeyLen);

IMINECT_EXP int32_t imiSetTUSNKey(ImiSensorHandle faceDetectSensor,const ImiSNKey* pSKey);

IMINECT_EXP int32_t imiGetTUSNKey(ImiSensorHandle faceDetectSensor, ImiSNKey* pSKey , uint32_t * nKenLen);

IMINECT_EXP int32_t imiSetPayFrameFaceKey(ImiSensorHandle faceDetectSensor, const ImiPayFaceKey* pSKey);

IMINECT_EXP int32_t imiSetPayFrameUmsKey(ImiSensorHandle faceDetectSensor, const ImiPayUmsKey* pSKey);

IMINECT_EXP int32_t imiSetLiveKey (ImiSensorHandle faceDetectSensor,const ImiKeepLiveKey* liveKey);

IMINECT_EXP int32_t imiGetPayFramePublicKey(ImiSensorHandle faceDetectSensor, ImiPayPublicKey* pSKey ,uint32_t *nKenLen);

IMINECT_EXP int32_t imiGetDeviceInfo(ImiDeviceHandle device, ImiModuleInfo** pModuleInfo); 

IMINECT_EXP int32_t imiGetStatistics(ImiDeviceHandle device,imi_run_statistics_info * pStatisticInfo ,uint32_t *nKenLen);

IMINECT_EXP int32_t imiGetEncrySNKey(ImiSensorHandle faceDetectSensor, ImiSNKey* pSKey ,uint32_t *nKenLen);

IMINECT_EXP int32_t imiSetYouTuKey(ImiSensorHandle faceDetectSensor,const imiYouTuKey* pSkey);
//
IMINECT_EXP int32_t imiGetYouTuKey(ImiSensorHandle faceDetectSensor,const imiYouTuKey* pSkey , uint32_t * nKenLen);

IMINECT_EXP int32_t imiGetAlgInfo(ImiDeviceHandle device, AlgInfo* pKey ,uint32_t* len);

IMINECT_EXP int32_t imiSetLivenessDetectOption(ImiSensorHandle faceDetectSensor,const LivenessDetectOption* option);

IMINECT_EXP int32_t imiSetRandom(ImiSensorHandle faceDetectSensor ,const Random* random);

/*! @} */
#endif // _IMI_API_FACE_DETECT_SENSOR_H_
