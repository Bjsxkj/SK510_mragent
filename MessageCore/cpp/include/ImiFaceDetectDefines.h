/********************************************************************************
=======
�?********************************************************************************
>>>>>>> cd6280af (Initial commit)
  Copyright (c) 2015-2016 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @File Name     : ImiFaceDetectDefines.h
  @Author        : li wei
  @Date          : 2018-10-27
  @Description   : FaceDetect Defines
  @Version       : 0.1.0

  @History       :
  1.2018-10-27 li wei Created file
********************************************************************************/
#ifndef _IMI_FACE_DETECT_DEFINES_H_
#define _IMI_FACE_DETECT_DEFINES_H_

#include <ImiDefines.h>
#include <ImiPlatform.h>

#define MAX_FACE_NUM 6
#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup iminect
* @{
*/
//System APIs

enum ImiFaceProperties
{

};

typedef uint32_t ImiFaceAlgorithmMode;
enum
{
    IMI_FACE_DETECT_MODE                         = 0x00,
    IMI_FACE_IDENTIFIER_MODE                     = 0x01,
    IMI_FACE_ANTI_SPOOFING_MODE                  = 0x02,
    IMI_FACE_IDENTIFIER_AND_ANTI_SPOOFING_MODE   = 0x03,
	IMI_FACE_ECHO_FRAME_MODE                     = 0x04,

	IMI_FACE_PAYFRAME_NOSAFE_MODE				 =0x07,
	IMI_FACE_PAYFRAME_SAFE_SHANG_MODE			 =0x08,
	IMI_FACE_PAYFRAME_SAFE_LIAN_MODE			 =0x09,
};

typedef uint32_t ImiFaceSelectMode;
enum
{
    IMI_FACE_SELECT_ALL             = 0x00,
    IMI_FACE_SELECT_LARGEST         = 0x01,
    IMI_FACE_SELECT_NEAREST         = 0x02,
};

#pragma pack (push, 1)

struct ImiFaceDetectMode
{
    ImiFaceAlgorithmMode faceAlgorithmMode;
    ImiFaceSelectMode selectMode;
};

struct ImiRect
{
    int x;
    int y;
    int width;
    int height;
};

struct ImiPoint2D
{
    int x;
    int y;
};

#define UINT8_BIT_0		0x01
#define UINT8_BIT_1		0x02
#define UINT8_BIT_2		0x04
#define IMI_FACE_DETECT_LAND_MARK_SIZE 5
struct ImiFaceDetectData
{
	////�������
	//ImiRect bbox;     // ��������Ϣ
	//ImiPoint2D landmark[5]; //ÿһ����һ��(x,y)���꣬��ʾ5�������㣬�ֱ����������۱Ǽ���������
	//int profile;      // 0 frontal; 1 lefthalf; 2 right half   ������
	//int gender;       // added on Mar.1,2018     �Ա���ʱ������
	//int age;          // ���䣨��ʱ������

	////���� 
	//float depth;      // ��ʾ��������ͷ�ľ��루��ʱ������
	//float deplivscr;  // ��Ȼ���÷�
	//float irlivscr;   // �������÷�
	//float livscr;     // �������ֵ

	////��������
	//uint8_t blur;               //1��ͼ��������0��ͼ��ģ��
	//uint8_t light;              //1������������0�����ղ�����
	//uint8_t cover;              //1�����ڵ���0�����ڵ�
	//uint8_t angle;              //1����̬������0����̬�Ƕȹ���
	//uint8_t eye_distance_status;        //1���ۼ��������0���ۼ�಻����Ԥ��ֵ
	//uint8_t expression;          //1:����������0�����鲻����
	//uint8_t more_face;           //1��1��������0��������
	//uint8_t resolution;           //�ֱ��ʣ�����ģʽ�¶�δ�����жϣ���������жϣ�bctc:��ֱ�������640��ˮƽ�������480��ͼ������
	//uint8_t ok;                  //1��������ͼ��0������ͼ��Ĭ��Ϊ1��������һ����Ϊ0ʱ��������ͼ��ϸ������
	//float eye_distance;      //�ۼ����Ϣ
	//float roll;           //roll����Ϣ
	//float pitch;          //pitch����Ϣ
	//float yaw;           //yaw����Ϣ
	//float quality_score;   //�����÷֣�ֻ���blur��light
	//uint8_t close_eye;         //==1Ϊ����,==0 Ϊ���ۣ��� expression������ʱ��Ч

	//uint16_t imiFaceAlgrithmMode; // ģʽУ��, 0-15λ��ÿλֵ��0��1�� 1����ʾ�ǣ�0����ʾ�񣩣�
	//// ��0λ����ʾ�Ƿ�Ϊ�������ģʽ��
	//// ��1λ����ʾ�Ƿ�Ϊ������ģʽ��
	//// ��2λ����ʾ�Ƿ�Ϊ����ʶ��ģʽ
	//uint16_t faceFeatureDetectInUse;   // ��ʾlandmark�е���Ч����
	//float faceFeature_detect[256];     // ���������������ʱ������
	//uint16_t faceFeatureIdentifyInUse; // ��ʾfaceFeature_identifier�е���Ч����
	//float faceFeature_identifier[638]; // ����ʶ������

	//uint8_t open_mouth_flag;			//�Ƿ������� 1���� 0����
	//uint8_t temp[3];					
	//int error;							//���ش�����
	//�������
	ImiRect bbox;					// ��������Ϣ
	ImiPoint2D landmark[5];		// ÿһ����һ��(x,y)���꣬��ʾ5�������㣬�ֱ����������۱Ǽ���������

	//����
	float depth;      			// ��ʾ��������ͷ�ľ���
	float deplivscr;  			// ��Ȼ���÷֣�����ֵvalue<=1.0
	float irlivscr;   			// �������÷֣�����ֵvalue<=1.0
	float livscr;     			// �������ֵ������ֵvalue<=1.0

	//��������
	uint8_t blur_flag;			//1��ͼ��������0��ͼ��ģ��
	uint8_t light_flag;			//1������������0�����ղ�����
	uint8_t cover_flag;			//1�����ڵ���0�����ڵ�
	uint8_t angle_flag;			//1����̬������0����̬�Ƕȹ���
	uint8_t eye_distance_flag;	//1���ۼ��������0���ۼ�಻����Ԥ��ֵ
	uint8_t expression;         //1������������0�����鲻����
	uint8_t open_mouth_flag;		//1�����죬0������
	uint8_t close_eye_flag;		//1������,0������ 
	uint8_t multi_face_flag;		//1��1��������0��������
	uint8_t ok_flag;				//1��������ͼ��0������ͼ��Ĭ��Ϊ1��������һ����Ϊ0ʱ��������ͼ��ϸ������

	float   eye_distance;			//�ۼ����Ϣ
	float   roll;					//roll����Ϣ
	float   pitch;          		//pitch����Ϣ
	float   yaw;           		//yaw����Ϣ
	float	quality_score;   		//�����÷֣�ֻ���blur��light//����
};

struct ImiLivenessInfo
{
	int8_t vendorId[3];
	int8_t detectMode[2];
	int8_t detectAbility;
	int8_t detectAlgorithm[20];
	int8_t cameraVendor[20];
	int8_t detectThreshold[2];
	int8_t detectScore[2];
};
struct ImiFaceDetectFrame
{
   bool		isLive;
   float	livenessScore;
   uint64_t	rgbTimeStamp;
   float	rgbBrightness;
   float	rgbDefinition;
   float    bioRslt;
   ImiLivenessInfo		livenessInfo;   

   uint16_t faceCount;//��⵽������������0~6.����6��ֻ��ȡǰ6�������SDK��
   uint16_t maxfaceidx;//����������±ꡣ0~5,����ֵ�Ƿ���0xffff��ʾ��ֵ��Ч��
   uint16_t colorwidth;//0:�ֱ�����Ч����ʾ������÷ֱ��ʶ�Ӧ��ͼ������ֵ����ͼ�Ŀ�
   uint16_t colorheight;//0:�ֱ�����Ч����ʾ������÷ֱ��ʶ�Ӧ��ͼ������ֵ����ͼ�ĸ�
   uint16_t depthwidth;//0:�ֱ�����Ч����ʾ������÷ֱ��ʶ�Ӧ��ͼ������ֵ�����ͼ�Ŀ�
   uint16_t depthheight;//0:�ֱ�����Ч����ʾ������÷ֱ��ʶ�Ӧ��ͼ������ֵ�����ͼ�ĸ�
   uint16_t infrarwidth;//0:�ֱ�����Ч����ʾ������÷ֱ��ʶ�Ӧ��ͼ������ֵ������ͼ�Ŀ�
   uint16_t infrarheight;//0:�ֱ�����Ч����ʾ������÷ֱ��ʶ�Ӧ��ͼ������ֵ������ͼ�ĸ�
   uint16_t thumbnailheight;//����ͼ�ĸ�
   uint16_t thumbnailwidth;//����ͼ�Ŀ�

   ImiFaceDetectData* pData;
   int		error_code;

   void *pColorData;
   void *pThumbnailData;
   void *pDepthData;
   void *pIrData;

  // uint32_t faceDetectDataSize ;
   uint32_t size;
   uint32_t colorFrameSize;
   uint32_t depthFrameSize;
   uint32_t irFrameSize;
   uint32_t thumbnailSize;
   uint32_t frameNum;
   uint64_t timeStamp;

 
};

//typedef struct tagFaceInfoList
//{
//    uint64_t timeStamp;
//    uint16_t faceCount;
//    FaceInfo faces[];   //faceCount * sizeof(FaceInfo)
//}FaceInfoList;

#pragma pack(pop)


#if defined(_WIN32)

typedef void (_stdcall* ImiNewFaceDetectFrameCallback)(ImiSensorHandle faceDetectSensor, void* pUserData);

#else

typedef void (* ImiNewFaceDetectFrameCallback)(ImiSensorHandle faceDetectSensor, void* pUserData);

#endif

#ifdef __cplusplus
}
#endif

/*! @} */
#endif // _IMI_FACE_DETECT_DEFINES_H_
