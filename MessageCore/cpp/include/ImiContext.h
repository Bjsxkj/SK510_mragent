/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiContext.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : Context Develop API
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_API_CONTEXT_H_
#define _IMI_API_CONTEXT_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

/*! \addtogroup iminect
* @{
*/

// System APIs

/// @brief 
/// @return 
IMINECT_EXP int32_t imiInitialize();

/// @brief 
/// @return 
IMINECT_EXP int32_t imiDestroy();

/// @brief 
/// @param pDeviceList 
/// @param pDeviceCount 
/// @return 
IMINECT_EXP int32_t imiGetDeviceList(ImiDeviceAttribute** pDeviceList, uint32_t* pDeviceCount);

/// @brief 
/// @param pDeviceList 
/// @return 
IMINECT_EXP int32_t imiReleaseDeviceList(ImiDeviceAttribute** pDeviceList);

/// @brief 
/// @param callback 
/// @param pData 
/// @return 
IMINECT_EXP int32_t imiSetDeviceStateCallback(ImiDeviceStateCallback callback, void* pData);

/// @brief 
/// @return 
IMINECT_EXP int32_t imiGetLastError();

/// @brief 
/// @param nErrorCode 
/// @return 
IMINECT_EXP const char* imiGetErrorString(int32_t nErrorCode);

/// @brief 
/// @param config 
/// @return 
IMINECT_EXP int32_t imiSetLogConfig(const LogConfig* config);

/**
 * @brief  Convert coordinate depth to world

 * @param  ImiVector4* pDst: the world coordinate
 * @param  const ImiVector4I* pSrc��the depth coordinate
 * @param  int32_t height: the height
 * @param  int32_t width: the width
 * @return 0:success, < 0 :error
 **/

/// @brief 
/// @internal
/// @param pDst 
/// @param pSrc 
/// @param height 
/// @param width 
/// @return 
IMINECT_EXP int32_t imiConvertCoordinateDepthToWorld(ImiVector4* pDst, const ImiVector4I* pSrc, int32_t height, int32_t width);

/**
 * @brief  Convert coordinate world to depth

 * @param  ImiVector4* pDst: the depth coordinate
 * @param  const ImiVector4I* pSrc��the world coordinate
 * @param  int32_t height: the height
 * @param  int32_t width: the width
 * @return 0:success, < 0 :error
 **/

/// @brief 
/// @internal
/// @param pDst 
/// @param pSrc 
/// @param height 
/// @param width 
/// @return 
IMINECT_EXP int32_t imiConvertCoordinateWorldToDepth(ImiVector4I* pDst, const ImiVector4* pSrc, int32_t height, int32_t width);


/**
 * @brief  Convert depth to point cloud

 * @param  const ImiImageFrame* pDepth : Pointer of frame
 * @param  float factor : camera factor, if the output point in meters , use 1000.0
 * @param  float fx : camera internal reference, the x-axis focal length expressed in pixel units
 * @param  float fy : camera internal reference, the y-axis focal length expressed in pixel units
 * @param  float cx : camera internal reference, the x-axis coordinate value of principal point that is usually at the image center
 * @param  float cy : camera internal reference, the y-axis coordinate value of principal point that is usually at the image center
 * @param  float* pPointClouds: the pointer of the point cloud buffer, the point size is pDepth->width * pDepth->height
 * @return 0:success, < 0:error
 * @ps: if any one of (factor, fx, fy, cx, cy) <= 0, these use default parameters
 **/

/// @brief 
/// @internal
/// @param pDepth 
/// @param factor 
/// @param fx 
/// @param fy 
/// @param cx 
/// @param cy 
/// @param pPointClouds 
/// @return 
IMINECT_EXP int32_t imiConvertDepthToPointCloud(const ImiImageFrame* pDepth, float factor, float fx, float fy, float cx, float cy, ImiPoint3D* pPointClouds);


/*! @} */
#endif // _IMI_API_CONTEXT_H_
