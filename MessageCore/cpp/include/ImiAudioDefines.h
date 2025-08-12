/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiPositionTrackDefines.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : PositionTrack Defines
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_AUDIO_DEFINES_H_
#define _IMI_AUDIO_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>


#ifdef __cplusplus
extern "C" {
#endif

//struct ImiAudioMode
//{
//	uint32_t nChannel;//Í¨µÀÊý
//	uint32_t nSamplingRate;//²ÉÑùÂÊ
//	uint32_t nSamplingAccuracy;//²ÉÑù¾«¶È
//};

struct ImiAudioFrame;
typedef ImiAudioFrame* ImiAudioFrameHandle;

#if defined(_WIN32)

typedef void (_stdcall* ImiNewAudioFrameCallback)(ImiSensorHandle audioSensor, void* pUserData);

#else

typedef void (* ImiNewAudioFrameCallback)(ImiSensorHandle audioSensor, void* pUserData);

#endif


#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_POSITION_TRACK_DEFINES_H_
