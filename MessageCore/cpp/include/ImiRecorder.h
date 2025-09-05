/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiRecorder.h
  @Author        : Yan Guoqi
  @Date          : 2019-04-12
  @Description   : ImiRecorder Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 Yan Guoqi Created file
********************************************************************************/
#ifndef _IMI_RECORDER_API_H_
#define _IMI_RECORDER_API_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

IMINECT_EXP int32_t imiCreateRecorder(const char* pPathToSave, ImiRecordFormat recordFormat, ImiRecordHandle* pRecorder);

IMINECT_EXP int32_t imiDestroyRecorder(ImiRecordHandle recorder);

IMINECT_EXP int32_t imiRecorderAttachSensor(ImiRecordHandle recorder, ImiSensorHandle sensorHandle);

IMINECT_EXP int32_t imiRecorderStart(ImiRecordHandle recorder);

IMINECT_EXP int32_t imiRecorderStop(ImiRecordHandle recorder);

IMINECT_EXP int32_t imiRecorderPause(ImiRecordHandle recorder);

IMINECT_EXP int32_t imiRecorderResume(ImiRecordHandle recorder);


#endif