/********************************************************************************
Copyright (c) 2015-2017 Huajie IMI Technology Co., Ltd.
All rights reserved.

@File Name    : ImiCamera.h
@Author        : Yang Feiyi
@Date           : 2017-03-24
@Description  : ImiCamera Application Develop API
@Version       : 0.1.0

@History       :
1.2017-03-24 Yang Feiyi Created file
********************************************************************************/
#ifndef _IMI_CAMERA_H_
#define _IMI_CAMERA_H_

#include "ImiCameraDefines.h"
#include "ImiDefines.h"

/**
 * @brief get device list.
 *
 * @param[in, out] ImiCameraHandle* pCameraDevice: Camera Device handle
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamGetAttrList(ImiDeviceAttribute** pList, int32_t* nCount);

/**
 * @brief get device list.
 *
 * @param[in, out] ImiCameraHandle* pCameraDevice: Camera Device handle
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamDestroyAttrList(ImiDeviceAttribute** pList);

/**
 * @brief get device list.
 *
 * @param[in, out] ImiCameraHandle* pCameraDevice: Camera Device handle
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamOpen(const char* pURI, ImiCameraHandle* pCameraDevice);

/**
 * @brief Open the specified camera with pid, vid.
 *
 * @param[in] int32_t vid: Camera Device vid
 * @param[in] int32_t pid: Camera Device Pid
 * @param[in] int32_t fd: Camera Device fd
 * @param[in] int32_t busnum: Camera Device busnum
 * @param[in] int32_t devaddr: Camera Device addr
 * @param[in] const char * usbfs: Camera Device usbfs
 * @param[in,out] ImiCameraHandle* pCameraDevice: Camera Device handle
 *
 * return 0: success, < 0 : error
 */
IMICAMERA_EXP int32_t imiCamOpen2(int32_t vid, int32_t pid, int32_t fd, int32_t busnum, int32_t devaddr, const char* usbfs, ImiCameraHandle* pCameraDevice);

/**
 * @brief get device list.
 *
 * @param[in, out] ImiCameraHandle* pCameraDevice: Camera Device handle
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamClose(ImiCameraHandle cameraDevice);

/**
 * @brief Get SupportFrameModes by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 * @param[in, out] const ImiCameraFrameMode** pMode: where frame mode list to assign
 * @param[in, out] uint32_t* pNumber: where the supported frame mode count to assign
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamGetSupportFrameModes(ImiCameraHandle cameraDevice, const ImiFrameMode** pMode, uint32_t* pNumber);

/**
 * @brief Get CurrentFrameMode by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 * @param[in, out] const ImiCameraFrameMode* pMode: where frame mode to assign
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP const ImiFrameMode* imiCamGetCurrentFrameMode(ImiCameraHandle cameraDevice);

/**

/**
 * @brief Start Camera Stream by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 * @param[in] const ImiCameraFrameMode* pMode: the pointer of frame mode
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamStartStream(ImiCameraHandle cameraDevice, ImiStreamType frameType, const ImiFrameMode* pMode);

/**
 * @brief Stop Camera Stream by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamStopStream(ImiCameraHandle cameraDevice, ImiStreamType frameType);

/**
 * @brief Get the latest frame of stream by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 * @param[in, out] ImiCameraFrame** pFrame: Pointer of frame pointer to assign the new frame pointer
 * @param[in] uint32_t timeout: Max time to wait utill get the frame, if -1 default wait 1000 ms
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamReadNextFrame(ImiCameraHandle cameraDevice, ImiImageFrame** pFrame, ImiStreamType frameType, int32_t timeout);

/**
 * @brief Release a frame ref.
 *
 * @param[in] ppFrame : Pointer of frame pointer to release ref
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamReleaseFrame(ImiImageFrame** pFrame);

IMICAMERA_EXP int32_t imiCamFrameAddRef(ImiImageFrame** pFrame);

/**
 * @brief Set frame Mirror by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 * @param[in] ImiCAMBOOL mirror: whether or not do Mirror
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamSetMirror(ImiCameraHandle cameraDevice, ImiCAMBOOL bMirror);

/**
 * @brief Set frame Mirror by Camera Device handle.
 *
 * @param[in] ImiCameraHandle cameraDevice: Camera Device handle
 * @param const char* pBmpImagePath: the path bmp image to save.if pBmpImagePath is NULL, save bmp image to workdirectory and set name like 20161212090807.bmp
 *
 * return 0: success, < 0 : error
 **/
IMICAMERA_EXP int32_t imiCamTakePhoto(ImiCameraHandle cameraDevice, const char* pBmpImagePath);

IMICAMERA_EXP int32_t imiCamSetProperty(ImiCameraHandle cameraDevice, uint32_t propertyId, const void* pData, uint32_t dataSize);

IMICAMERA_EXP int32_t imiCamGetProperty(ImiCameraHandle cameraDevice, uint32_t propertyId, void* pData, uint32_t* pDataSize);

IMICAMERA_EXP int32_t imiCamSetFramesSync(ImiCameraHandle cameraDevice, bool bSync);



IMICAMERA_EXP int32_t imiCamSetExposureArea(ImiCameraHandle cameraDevice, uint32_t startX, uint32_t startY, uint32_t areaWidth, uint32_t areaHeight);

IMICAMERA_EXP int32_t imiCamClearExposureArea(ImiCameraHandle cameraDevice);


//???
IMICAMERA_EXP int32_t imiCamAudioGetAttrList(ImiDeviceAttribute** pList, int32_t* nCount);
IMICAMERA_EXP int32_t imiCamAudioOpen(const char* pURI, ImiCameraAudioHandle* pCameraAudioDevice);
IMICAMERA_EXP int32_t imiCamAudioClose(ImiCameraAudioHandle cameraAudioDevice);
IMICAMERA_EXP int32_t imiCamAudioStartStream(ImiCameraAudioHandle cameraAudioDevice);
IMICAMERA_EXP int32_t imiCamAudioStopStream(ImiCameraAudioHandle cameraAudioDevice);
IMICAMERA_EXP int32_t imiCamAudioReadNextFrame(ImiCameraAudioHandle cameraAudioDevice, ImiAudioFrame** pFrame, int32_t timeout);
IMICAMERA_EXP int32_t imiCamAudioReleaseFrame(ImiAudioFrame** pFrame);
IMICAMERA_EXP int32_t imiCamAudioFrameAddRef(ImiAudioFrame** pFrame);

#endif
