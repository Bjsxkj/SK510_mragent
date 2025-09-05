/********************************************************************************
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiMoveTrackDefines.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : MoveTrack Defines
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_MOVE_TRACK_DEFINES_H_
#define _IMI_MOVE_TRACK_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup iminect
* @{
*/
//System APIs

enum ImiMoveTrackProperties
{
    IMI_PROPERTY_MOVE_TRACK_SHOCK	= 1
};

enum ButtonType
{
    SELECT_BUTTOON     = 0,
    CROSS_BUTTOON      = 1,
    MOVE_BUTTOON       = 2,
    SQUARE_BUTTOON     = 3,
    RESET_BUTTOON      = 4,
    START_BUTTOON      = 5,
    TRIANGLE_BUTTOON   = 6,
    CIRCLE_BUTTOON     = 7
};

enum ButtonSate
{
    BUTTOON_PRESS           = 0,
    BUTTOON_LONG_PRESS      = 1,
    BUTTOON_INVALID         = 2
};

#pragma pack (push, 1)


//光球的颜色
struct ImiBallRGB
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

/*光球位置
(x,y) 光球在彩色图的二维位置
z     光球中心距离摄像头的距离
fRadius 光球的半径
*/
struct BallPosition
{
    float fRadius;
    float x;
    float y;
    float z;
};

struct BallInfo
{
    ImiBallRGB rgb;
    BallPosition pos;
};

//imuData 四元数，向量(x, y, z)+旋转角(w)
struct ImiMoveTrackData
{
    uint8_t nValid;
    uint8_t nSeq;
    float fADvoltage;
    ImiVector4 imuData;
    BallInfo ballInfo;
    ButtonSate btnState[8];
};

struct ImiMoveTrackDataList
{
    uint64_t timeStamp;
    uint32_t frameNum;
    ImiMoveTrackData moveTrack[4];
};

struct ImiMoveTrackMode
{
    //ImiPositionTrackAlgorithmMode algorithmMode;

};

#pragma pack(pop)


struct ImiMoveTrackFrame;
typedef ImiMoveTrackFrame* ImiMoveTrackFrameHandle;


#if defined(_WIN32)

typedef void (_stdcall* ImiNewMoveTrackFrameCallback)(ImiSensorHandle moveTrackSensor, void* pUserData);

#else

typedef void (* ImiNewMoveTrackFrameCallback)(ImiSensorHandle moveTrackSensor, void* pUserData);

#endif


#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_POSITION_TRACK_DEFINES_H_
