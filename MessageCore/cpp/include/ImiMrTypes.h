#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <string>

using namespace std;

#define IMI_DATA_VERSION 2

#define _STR_(x)  #x

#ifdef SWIG

#ifdef SWIGPYTHON
#define TARGET_LANG_TO_STRING_NAME %rename("__str__") toString;
#define TARGET_LANG_TO_STRING_METH
#elif defined(SWIGCSHARP) 
#define TARGET_LANG_TO_STRING_NAME %rename("ToString") toString; 
#define TARGET_LANG_TO_STRING_METH %csmethodmodifiers toString "public override ";
#elif defined(SWIGJAVA) 
#define TARGET_LANG_TO_STRING_NAME 
#define TARGET_LANG_TO_STRING_METH %javamethodmodifiers ANY::toString() "@Overrided\npublic";
#else
#define TARGET_LANG_TO_STRING_NAME
#define TARGET_LANG_TO_STRING_METH
#endif

#else
#define TARGET_LANG_TO_STRING_NAME
#define TARGET_LANG_TO_STRING_METH
#endif


#define TO_STRING_OP(...)  TARGET_LANG_TO_STRING_NAME \
TARGET_LANG_TO_STRING_METH \
string toString() const { stringstream ss; ss << *(this); return ss.str(); } \
\
friend ostream& operator<<( __VA_ARGS__ )

#ifdef SWIG
#define DEF_TEMPLATE_CLASS(target_name, cname)  using target_name = cname; \
    %template(target_name) cname;
#else
#define DEF_TEMPLATE_CLASS(target_name, cname)  using target_name = cname; 
#endif


template<typename T, int N>
struct ArrayPrint{
    T* _data;
    ArrayPrint(T* d):_data(d){}
    
    friend ostream& operator<<(ostream& os, const ArrayPrint<T,N>& a){
        T* p = a._data;
        os << "[" << *(p++);
        for(int i=1; i<N && i< 16; i++, p++){
            os << ", " << *p;
        }
        if(N>16){
            os << ", ...";
        }

        return os << "]";
    }
};

template<typename T, int ROW, int COL>
struct ArrayPrint2{
    T* _data;
    ArrayPrint2(T* d):_data(d){}
    friend ostream& operator<<(ostream& os, const ArrayPrint2<T, ROW, COL>& a){
        T* p = a._data;
        for(int i=0; i<ROW; i++){
            os << "[" << *(p++);
            for(int j=1; j<COL; j++){
                os << ",\t" << *(p++);
            }
            os << "]\n";
        }
        return os;
    }
};

/// @brief 基础数据类.
struct ImiData {

    bool empty() const { return this==0; }

    void* getHandle() const { return (void*)this; }

    // 注意：此类不能有任何虚方法.

};



namespace imimr{ 

#pragma pack(push)
#pragma pack(1)

    /// @brief Sensor类型.
    enum class ImiSensorType {
        COLOR        =(0x01),       ///< 彩色
        DEPTH        =(0x01 << 1),  ///< 深度
        IR           =(0x01 << 2),  ///< 红外
        USERTRACK    =(0x01 << 3),  ///< 用户骨架
        FACEDETECT   =(0x01 << 4),  ///< 人脸
        POSITIONTRACK=(0x01 << 5),  ///< SLAM
        MOVETRACK    =(0x01 << 6),  ///< 手柄运动
        AUDIO        =(0x01 << 7),  ///< 音频

        ACTION       =(0x01 << 8),  ///< 动作
        
    };

    /// @brief USB连接状态
    /// @ingroup1 imimrsdk
    enum class ImiDeviceState {

        IMI_DEVICE_STATE_DISCONNECT =(1),   ///< 断开.
        IMI_DEVICE_STATE_CONNECT    =(0),   ///< 连接.
    };

    /// @brief 图像帧格式
    /// @ingroup1 imimrsdk
    enum class ImiPixelFormat {
        //todo:需要统一
        IMI_PIXEL_FORMAT_IMAGE_DEP_16BIT = 0x00,    ///<    //Depth

        IMI_PIXEL_FORMAT_IMAGE_YUV422 = 0x01,       ///<     //Rgb
        IMI_PIXEL_FORMAT_IMAGE_H264 = 0x02,         ///< //H264 Compressed
        IMI_PIXEL_FORMAT_IMAGE_RGB24 = 0x03,        ///< 
        IMI_PIXEL_FORMAT_IMAGE_YUV420SP = 0x04,     ///<  //NV21
        IMI_PIXEL_FORMAT_IMAGE_MJPEG = 0x05,        ///< 
        IMI_PIXEL_FORMAT_IMAGE_YUYV = 0x06,         ///< 

        IMI_PIXEL_FORMAT_INVALID = 0x10             ///< 

        // IMI_PIXEL_FORMAT_MJPEG  =(200),
        // IMI_PIXEL_FORMAT_H264   =(201),
        // IMI_PIXEL_FORMAT_RGB888 =(230),
        // IMI_PIXEL_FORMAT_RGBA8888   =(231),

        // IMI_PIXEL_FORMAT_YUV_YUYV   =(260),
        // IMI_PIXEL_FORMAT_YUV_NV12   =(261),
        // IMI_PIXEL_FORMAT_YUV_I420   =(262),

        // IMI_PIXEL_FORMAT_INVALID    =(400),
    };



    /// @brief SDK和固件版本
    /// @ingroup1 imimrsdk
    struct ImiVersions : public ImiData
    {
    // #ifndef SWIG
        struct imi_sdk_version
        {
            uint8_t major;
            uint8_t minor;
            uint16_t revision;

            TO_STRING_OP(ostream& os, const imi_sdk_version& a){
                return os<< (int)a.major<<"."<<(int)a.minor<<"."<<(int)a.revision;
            }

        };

        struct imi_fw_version
        {
            uint8_t major;
            uint8_t minor;
            uint16_t revision;

            TO_STRING_OP(ostream& os, const imi_fw_version& a){
                return os<< (int)a.major<<"."<<(int)a.minor<<"."<<(int)a.revision;
            }
        };

        struct imi_hw_version
        {
            uint8_t hardware;
            uint8_t chip;
            uint8_t ap_hardware;
            uint8_t ext;

            TO_STRING_OP(ostream& os, const imi_hw_version& a){
                return os<< (int)a.hardware<<"."<<(int)a.chip<<"."<<(int)a.ap_hardware;
            }


        };

        struct imi_dsp_version
        {
            uint8_t fw_tmp_ver;
            uint8_t fw_revise_ver;
            uint8_t fw_minor_ver;
            uint8_t fw_major_ver;

            uint8_t hand_tmp_ver;
            uint8_t hand_revise_ver;
            uint8_t hand_minor_ver;
            uint8_t hand_major_ver;

            uint8_t depth_skt_tmp_ver;
            uint8_t depth_skt_revise_ver;
            uint8_t depth_skt_minor_ver;
            uint8_t depth_skt_major_ver;

            TO_STRING_OP(ostream& os, const imi_dsp_version& a){
                return os<< (int)a.fw_major_ver<<"."<<(int)a.fw_minor_ver<<"."<<(int)a.fw_revise_ver <<"."<< (int)a.fw_tmp_ver
                <<"."<< (int)a.hand_major_ver<<"."<<(int)a.hand_minor_ver<<"."<<(int)a.hand_revise_ver <<"."<< (int)a.hand_tmp_ver
                <<"."<< (int)a.depth_skt_major_ver<<"."<<(int)a.depth_skt_minor_ver<<"."<<(int)a.depth_skt_revise_ver
                <<"." << (int)a.depth_skt_tmp_ver ;
            }


        };

        imi_sdk_version sdkVersion;
        imi_fw_version fwVersion;
        imi_hw_version hwVersion;
        char uvc_color_version[64];
        char bridge_fw_version[64];
        imi_dsp_version dspVersion;
    // #endif
        /* data */
        //string ver;
        TO_STRING_OP(ostream& os, const ImiVersions& a){
            return os<<"sdk:"<<a.sdkVersion<<", fw:"<<a.fwVersion<<", hw:"<<a.hwVersion <<", dsp:"<<a.dspVersion;
        }
    };





    /// @brief USB设备属性.
    /// @ingroup1 imimrsdk
    struct ImiDeviceAttribute : public ImiData {
        int vendorId;       ///< 厂家ID
        int productId;      ///< 产品ID
        std::string uri;    ///< URI路径.
        bool enableUVC;     ///< 是否支持UVC

        ImiDeviceAttribute(int vendorId=0, int productId=0, std::string uri="", bool enableUVC = 0): vendorId(vendorId), productId(productId), uri(uri), enableUVC(enableUVC) {}

        TO_STRING_OP(std::ostream& os, const ImiDeviceAttribute& a){
            return os<<"{vid=0x"<<std::hex <<a.vendorId<<", pid=0x"<< a.productId<<", uri="<<a.uri<<"}";
        }
    };

    /// @brief 相机内参
    /// @ingroup1 imimrsdk
    struct ImiCameraIntrinsic : public ImiData {
        float fxParam;  ///< 焦距fx
        float fyParam;  ///< 焦距fy
        float cxParam;  ///< 主点x
        float cyParam;  ///< 主点y
        float k1Param;  ///< 切向畸变系数1
        float k2Param;  ///< 切向畸变系数2
        float k3Param;  ///< 切向畸变系数3
        float p1Param;  ///< 径向畸变系数1
        float p2Param;  ///< 径向畸变系数2

        ImiCameraIntrinsic(float fx=0, float fy=0, float cx=0, float cy=0):fxParam(fx), fyParam(fy), cxParam(cx), cyParam(cy) {
            k1Param = 0, k2Param =0, k3Param =0, p1Param =0, p2Param =0;
        }
        
        TO_STRING_OP(ostream& os, const ImiCameraIntrinsic& a){
            os <<  ArrayPrint<float,9>((float*)&(a.fxParam)); //"{fx="<<a.fxParam<<", fy="<<a.fyParam <<", cx="<<a.cxParam<<", cy="<<a.cyParam <<"}";
            return os;
        }
    };

    /// @brief 矩形框
    /// @ingroup1 imimrsdk
    struct ImiRect : public ImiData
    {
        
        int x;      ///< 左上角x.
        int y;      ///< 左上角y.
        int width;  ///< 宽.
        int height; ///< 高.

        ImiRect(int x=0, int y=0, int w=0, int h=0): x(x), y(y), width(w), height(h){}

        TO_STRING_OP(ostream& os, const ImiRect& a){
            return os << ArrayPrint<int,4>((int*)&a);
        }
    };

    /// @brief 图像2D坐标.
    /// @ingroup1 imimrsdk
    struct ImiPoint2D : public ImiData
    {
        int x;  ///< x坐标.
        int y;  ///< y坐标.

        ImiPoint2D(int x=0, int y=0):x(x),y(y){}

        TO_STRING_OP(ostream& os, const ImiPoint2D& a){
            return os << ArrayPrint<int,2>((int*)&a);
        }
    };

    /// @brief 
    struct ImiFaceDetectData : public ImiData {
        ImiFaceDetectData(){} // landmark = {1,2,3,4}; }

        ImiRect rect; 				///< 人脸框信息.
        ImiPoint2D landmark[5];     ///< 5个特征点, 分别是左眼右眼鼻尖和两个嘴角.

        //
        float depth;                ///< 表示距离摄像头的距离（暂时保留）.
        float depthLivenessScore;   ///< 深度活体得分.
        float irLivenessScore;   	///< 		//
        float livenessScore;    	///< 		// 

        //
        uint8_t blur;               ///< 1: 图像清晰, 0: 图像模糊.
        uint8_t light;              ///< 1: 光照正常, 0: 光照不满足.
        uint8_t cover;              ///< 1: 无遮挡, 0: 有遮挡.
        uint8_t angle;              ///< 1: 姿态正常, 0: 姿态角度过大.
        uint8_t eyeDistanceStatus;  ///< 1: 眼间距正常, 0: 眼间距不满足预定值.
        uint8_t expression;         ///< 1:表情正常, 0: 表情不满足.
        uint8_t moreFace;           ///< 1: 1个人脸, 0: 多人脸.
        uint8_t openMouth;          ///< 1: 闭嘴, 0: 张嘴.
        uint8_t closeEye;           ///< ==1为闭眼,==0 为睁眼, 在 expression不满足时有效.
        uint8_t ok;                 ///< 1: 不正常图像, 0: 正常图像, 默认为1, 以上任一条件为0时都不满足图像合格的条件.

        // uint8_t open_mouth_flag;		//1�����죬0������
        // uint8_t close_eye_flag;		//1������,0������ 
        // uint8_t multi_face_flag;		//1��1��������0��������
        // uint8_t ok_flag;				//1��������ͼ��0������ͼ��Ĭ��Ϊ1��������һ����Ϊ0ʱ��������ͼ��ϸ������

        float eyeDistance;      ///< 眼间距信息.
        float roll;             ///< roll角信息.
        float pitch;            ///< pitch角信息.
        float yaw;              ///< yaw角信息.
        float qualityScore;     ///< 质量得分, 只针对blur、light.

        // float   eye_distance;			//�ۼ����Ϣ
        // float   roll;					//roll����Ϣ
        // float   pitch;          		//pitch����Ϣ
        // float   yaw;           		//yaw����Ϣ
        // float	quality_score;   		//�����÷֣�ֻ���blur��light//����

        TO_STRING_OP(ostream& os, const ImiFaceDetectData& a){
            return os << "FaceDetectData{rect="<<a.rect<<", landmark"<<ArrayPrint<int,10>((int*)&a.landmark) << "}";
        }

    };
    
    /// @brief  
    /// @ingroup1 imimrsdk
    struct LogConfig : public ImiData
    {
        string pOutputDir;      ///< 输出路径.
        uint32_t level;         ///< 0到3之间.
        bool bConsole;          ///< 输出到控制台.
        bool bFile;             ///< 输出到文件.
    };

    /// @brief 
    /// @ingroup1 imimrsdk
    enum class ImiSkeletonTrackState
    {
        IMI_SKELETON_NOT_TRACKED    = 0, ///< The user is not selected and is not available for selection
        IMI_SKELETON_POSITION_ONLY  = 1, ///< The user is not selected and is available for selection,
        IMI_SKELETON_TRACKED        = 2, ///< The user is being tracked and the skeleton position is valid
        IMI_SKELETON_TRACKING       = 3, ///< The user have been selected but is not tracked yet
        IMI_SKELETON_FAILED         = 4, ///< The user is selected but failed to be tracked.
    };

    // struct

    /// @brief 向量（整数元素）.
    /// @ingroup1 imimrsdk
    struct ImiVector4I : public ImiData {  
        int32_t x,y,z,w;

		ImiVector4I() {}
        ImiVector4I(int32_t x, int32_t y, int32_t z, int32_t w=1): x(x), y(y), z(z), w(w) {}
        TO_STRING_OP(ostream& os, const ImiVector4I& a){ 
            return os << "["<<a.x<<","<<a.y<<","<<a.z<<","<<a.w<<"]";
        }
    };

    /// @brief 向量.
    /// @ingroup1 imimrsdk
    struct ImiVector4 { 
        float x,y,z,w;
        TO_STRING_OP(ostream& os, const ImiVector4& a){
            return os << "["<<a.x<<","<<a.y<<","<<a.z<<","<<a.w<<"]";
        }
    };

    /// @brief 3x3矩阵.
    /// @ingroup1 imimrsdk
    struct ImiMat3f  { 
        float data[9];
        TO_STRING_OP(ostream& os, const ImiMat3f& a){
            return os << ArrayPrint2<float,3,3>((float*)&a);
        }
    };

    /// @brief 4x4矩阵.
    /// @ingroup1 imimrsdk
    struct ImiMat4f { 
        float data[16];

        TO_STRING_OP(ostream& os, const ImiMat4f& a){
            return os << ArrayPrint2<float,4,4>((float*)&a);
        }
    };

    /// @brief 人脸特征向量.
    struct ImiFaceFeatureVec : public ImiData
    {
        float faceFeatures[256]; ///< 人脸特征向量.

        TO_STRING_OP(ostream& os, const ImiFaceFeatureVec& a){
            return os << ArrayPrint<float,256>((float*)&a);
        }
    };

    /// @brief 骨架朝向数据.
    /// @ingroup1 imimrsdk
    struct ImiSkeletonOrientationData : public ImiData
    {
        ImiMat3f hierarchicalRotation; ///< 本地旋转矩阵, 相对于父节点.
        ImiMat3f absoluteRotation;     ///< 绝对旋转矩阵, 相对于世界坐标系.
        ImiVector4 eulerAngles;        ///< 欧拉角, 相对于父节点, 旋转顺序暂定为Z-X-Y.

       TO_STRING_OP(ostream& os, const ImiSkeletonOrientationData& a){
            return os <<"SkeletonOrientationData{hR="<<ArrayPrint<float,9>((float*)&a.hierarchicalRotation) <<", aR=" << ArrayPrint<float,9>((float*)&a.absoluteRotation)<<"}";
        } 
    };




    /// @brief 单人身体骨架数据.
    /// @ingroup1 imimrsdk
    struct ImiBodySkeletonData : public ImiData {

        /// @brief 人体骨架关节点序号.
        enum ImiBodySkeletonPositionIndex
        {
            IMI_BODY_SKELETON_HIP_CENTER      = 0,  ///< 胯.
            IMI_BODY_SKELETON_SPINE           = 1,  ///< 腰.
            IMI_BODY_SKELETON_SHOULDER_CENTER = 2,  ///< 颈.
            IMI_BODY_SKELETON_HEAD            = 3,  ///< 头.
            IMI_BODY_SKELETON_SHOULDER_LEFT   = 4,  ///< 左肩.
            IMI_BODY_SKELETON_ELBOW_LEFT      = 5,  ///< 左肘.
            IMI_BODY_SKELETON_WRIST_LEFT      = 6,  ///< 左手腕.
            IMI_BODY_SKELETON_HAND_LEFT       = 7,  ///< 左手.
            IMI_BODY_SKELETON_SHOULDER_RIGHT  = 8,  ///< 右肩.
            IMI_BODY_SKELETON_ELBOW_RIGHT     = 9,  ///< 右肘.
            IMI_BODY_SKELETON_WRIST_RIGHT     = 10, ///< 右手腕.
            IMI_BODY_SKELETON_HAND_RIGHT      = 11, ///< 右手.
            IMI_BODY_SKELETON_HIP_LEFT        = 12, ///< 左胯.
            IMI_BODY_SKELETON_KNEE_LEFT       = 13, ///< 左膝.
            IMI_BODY_SKELETON_ANKLE_LEFT      = 14, ///< 左脚踝.
            IMI_BODY_SKELETON_FOOT_LEFT       = 15, ///< 左脚.
            IMI_BODY_SKELETON_HIP_RIGHT       = 16, ///< 右胯.
            IMI_BODY_SKELETON_KNEE_RIGHT      = 17, ///< 右膝.
            IMI_BODY_SKELETON_ANKLE_RIGHT     = 18, ///< 右脚踝.
            IMI_BODY_SKELETON_FOOT_RIGHT      = 19, ///< 右脚.
            IMI_BODY_SKELETON_COUNT           = 20, ///< 关节点总数.
        };

        /// @brief 关节点的位置状态.
        enum class ImiBodySkeletonPositionTrackState
        {
            IMI_BODY_SKELETON_POSITION_NOT_TRACKED = 0, ///< 
            IMI_BODY_SKELETON_POSITION_INFERRED = 1,    ///< 
            IMI_BODY_SKELETON_POSITION_TRACKED = 2,     ///< 
        };

        /// @brief 关节点的朝向状态.
        enum ImiSkeletonOrientationState
        {
            IMI_SKELETON_ORIENTATION_AVAILABLE   = 0,   ///< 
            IMI_SKELETON_ORIENTATION_UNAVAILABLE = 1,   ///< 
            IMI_SKELETON_ORIENTATION_UNDEFINED   = 2    ///< 
        };

        /// @brief 位姿状态.
        enum ImiPoseState
        {
            IMI_POSE_UNSURE = 0,    ///< 
            IMI_POSE_STAND  = 1,    ///<
            IMI_POSE_SEATED = 2,    ///< 
        };

        /// @brief 手的状态.
        enum ImiHandState
        {
            IMI_HAND_UNSURE = 0,    ///< 未知
            IMI_HAND_GRASP  = 1,    ///< 抓握
            IMI_HAND_OPEN   = 2,    ///< 打开
        };

        struct IMI_Rect
        {
            int x;
            int y;
            int width;
            int height;
        };

        struct ImiFaceFeature
        {   
            // 2024-12-12 定义
            IMI_Rect faceRect; 
            uint32_t faceSize;       // 人脸尺寸
            uint32_t faceID;         // 人脸ID
            uint32_t age;            // 年龄
            uint32_t gender;         // 性别
            uint32_t confiendence;   //置信度  // 表情
            float faceFeatures[512]; // 人脸特征向量
        };

        struct ImiClothesFeature
        {
            float clothesFeatures[128]; // 服装特征向量
        };

        struct ImiFeature
        {
            uint8_t faceFeatureFlags; // 1：faceFeature有效， 0：faceFeature无效
            uint8_t clothesFeatureFlags; // // 1：clothesFeature有效， 0：clothesFeature无效
            uint8_t rev[2]; //为了保持4字节对齐, add20241212

            ImiFaceFeature faceFeature;
            ImiClothesFeature clothesFeature;

        };

        ImiSkeletonTrackState trackingState;       //4                             ///< 骨架跟踪状态.
        uint32_t trackingID;                       //                             ///< 骨架跟踪ID.
        ImiVector4 centerPosition;                                              ///< 人体中心位置坐标.
        ImiVector4 skeletonPositions[20];                                       ///< 骨架关节点坐标.
        ImiBodySkeletonPositionTrackState skeletonPositionTrackingStates[20];   ///< 骨架各关节点跟踪状态.
        uint32_t qualityFlags;                                                  ///< 骨架出界情况, 一共4位, 分别为上、下、左、右.
        ImiSkeletonOrientationState upSkeletonsOrientationState;                ///< 骨架上半身相对于世界坐标系的旋转矩阵的可用状态.
        ImiMat3f upSkeletonsOrientation;                                        ///< 骨架上半身相对于世界坐标系的旋转矩阵.
        ImiSkeletonOrientationState downSkeletonsOrientationState;              ///< 骨架下半身相对于世界坐标系的旋转矩阵的可用状态.
        ImiMat3f downSkeletonsOrientation;                                      ///< 骨架下半身相对于世界坐标系的旋转矩阵.
        ImiSkeletonOrientationState skeletonOrientationStates[20];              ///< 骨架关节点方向信息的可用状态.
        ImiSkeletonOrientationData skeletonOrientations[20];                    ///< 骨架所有关节点的方向信息, 对于没有此类信息的关节点, 显示说明该索引上的数据不可用.
        ImiPoseState poseState;                                                 ///< 骨架姿态.

        // Extra V2 feature
        ImiFeature feature;

        ImiHandState handState;
        IMI_Rect  handRect;                                                     //< 骨架姿态

        /// @brief 获取骨架顶点
        /// @param array_ref 
        /// @param array_len 
        void getVertexes(float* array_ref, int array_len){
            int len = array_len / 3;
            int n = len<20?len:20;
            ImiVector4* a = skeletonPositions;
            for(int i=0; i<n; i++){
                *array_ref = a->x;
                array_ref++;
                *array_ref = a->y;
                array_ref++;
                *array_ref = a->z;
                array_ref++;
                a++;
            }
        }

        /// @brief 获取骨架连接索引
        /// @return 
        static std::vector<int> getSkeletonLinkIndexes(){
            return std::vector<int>{0,1, 1,2, 2,3,  2,4, 4,5, 4,6, 6,7, 2,8, 8,9, 9,10, 10,11,
                     0,12, 12,13, 13,14, 14,15, 0,16, 16,17, 17,18, 18,19};
        }

        TO_STRING_OP(ostream& os, const ImiBodySkeletonData& a){
            return os << "{id:" << a.trackingID <<", state:" << (int)a.trackingState << ", center_pos:" << a.centerPosition <<
            ", quality:" << a.qualityFlags << ", up_stat:"<< (int)a.upSkeletonsOrientationState <<
            ", down_stat:" << (int)a.downSkeletonsOrientationState << ", pose_stat:" << (int)a.poseState <<
            ", up_ori:" << a.upSkeletonsOrientation << ", down_ori:"<< a.downSkeletonsOrientation <<
            ", skel_stat:"<< ArrayPrint<int,20>((int*)&a.skeletonPositionTrackingStates) << 
            ", skel_pos:" << ArrayPrint<ImiVector4,20>((ImiVector4*)&a.skeletonPositions) <<
            ", ori_stat:" << ArrayPrint<int,20>((int*)&a.skeletonOrientationStates) <<
            ", ori_data:" << ArrayPrint<ImiSkeletonOrientationData,20>((ImiSkeletonOrientationData*)&a.skeletonOrientations) <<
            "}";
        }

    };


    /// @brief 人体骨架帧数据.
    /// @ingroup1 imimrsdk
    struct ImiBodySkeleton : public ImiData {  
    
        int width;      ///< 
        int height;     ///< 
        ImiVector4 floorClipPlane;	                ///< 地面方程.
        ImiBodySkeletonData skeletonDatas[6];       ///< 骨架数据.

        TO_STRING_OP(ostream& os, const ImiBodySkeleton& a){
            return os << "{name=\"BodySkeleton\",w:" << a.width<<", h:"<<a.height<<", plane:"<<a.floorClipPlane<<",data=["<<a.skeletonDatas[0]<<","<<a.skeletonDatas[1]<<",...]}";
        }

        bool empty() const { return width==0 && height==0; }

        ImiBodySkeleton(){}

    protected:
        //virtual bool is_empty() const { return width==0 && height==0; }
    };

    // hand skeleton

    /// @brief 手骨架数据.
    /// @ingroup1 imimrsdk
    struct ImiHandSkeletonData : public ImiData
    {
        /// @brief 手的类型.
        enum ImiHandType
        {
            IMI_HAND_LEFT  = 0, ///< 左手.
            IMI_HAND_RIGHT = 1, ///< 右手.
        };

        /// @brief 手的状态.
        enum ImiHandState
        {
            IMI_HAND_UNSURE = 0,    ///< 未知
            IMI_HAND_GRASP  = 1,    ///< 抓握
            IMI_HAND_OPEN   = 2,    ///< 打开
        };

        /// @brief 手骨架关节点序号
        enum ImiHandSkeletonPositionIndex
        {
            //手腕
            IMI_HAND_SKELETON_WRIST = 0,

            //手掌五根手指根关节
            IMI_HAND_SKELETON_TMCP = 1, ///<  拇指.
            IMI_HAND_SKELETON_IMCP = 2, ///<  食指.
            IMI_HAND_SKELETON_MMCP = 3, ///<  中指.
            IMI_HAND_SKELETON_RMCP = 4, ///<  无名指.
            IMI_HAND_SKELETON_PMCP = 5, ///<  小指.

            //大拇指指关节
            IMI_HAND_SKELETON_TPIP = 6, ///< 拇指1.
            IMI_HAND_SKELETON_TDIP = 7, ///< 拇指2.
            IMI_HAND_SKELETON_TTIP = 8, ///< 拇指3.

            //食指指关节
            IMI_HAND_SKELETON_IPIP = 9,     ///< 食指1.
            IMI_HAND_SKELETON_IDIP = 10,    ///< 食指2.
            IMI_HAND_SKELETON_ITIP = 11,    ///< 食指3.

            //中指指关节
            IMI_HAND_SKELETON_MPIP = 12,    ///< 中指1.
            IMI_HAND_SKELETON_MDIP = 13,    ///< 中指2.
            IMI_HAND_SKELETON_MTIP = 14,    ///< 中指3.

            //无名指指关节
            IMI_HAND_SKELETON_RPIP = 15,    ///< 无名指1.
            IMI_HAND_SKELETON_RDIP = 16,    ///< 无名指2.
            IMI_HAND_SKELETON_RTIP = 17,    ///< 无名指3.

            //小拇指指关节
            IMI_HAND_SKELETON_PPIP = 18,    ///< 小拇指1.
            IMI_HAND_SKELETON_PDIP = 19,    ///< 小拇指2.
            IMI_HAND_SKELETON_PTIP = 20,    ///< 小拇指3.

            IMI_HAND_SKELETON_COUNT = 21,   ///< 关节点总数.
        };

        
        ImiSkeletonTrackState trackingState;                ///< 手势骨架跟踪状态.
        uint32_t trackingID;                                ///< 手势骨架跟踪ID.
        ImiHandType handType;                               ///< 左手或右手.
        ImiHandState handState;                             ///< 手的状态.
        ImiVector4 wristPosition;                           ///< 手腕坐标位置.
        ImiVector4 skeletonPositions[21];                    ///< 手势关节点坐标.
        ImiSkeletonOrientationData skeletonOrientations[21]; ///< 关节方向数据.


        /// @brief 获取骨架顶点
        /// @param array_ref 
        /// @param array_len 
        void getVertexes(float* array_ref, int array_len){
            int len = array_len / 3;
            int n = len<21?len:21;
            ImiVector4* a = skeletonPositions;
            for(int i=0; i<n; i++){
                *array_ref = a->x;
                array_ref++;
                *array_ref = a->y;
                array_ref++;
                *array_ref = a->z;
                array_ref++;
                a++;
            }
        }

        /// @brief 获取骨架连接索引
        /// @return 
        static std::vector<int> getSkeletonLinkIndexes(){
            return std::vector<int>{0,1, 0,2, 0,3, 0,4, 0,5,  1,6, 6,7, 7,8, 2,9, 9,10, 10,11, 3,12, 12,13, 13,14, 
                    4,15, 15,16, 16,17, 5,18, 18,19, 19,20};
        }

        TO_STRING_OP(ostream& os, const ImiHandSkeletonData& a){
            return os << "{name:\"ImiHandSkeletonData\", id:" << a.trackingID << ", stat:"<< (int)a.trackingState << ", type:" << (int)a.handType <<
            ", pos:" << ArrayPrint<ImiVector4,21>((ImiVector4*)&a.skeletonPositions) <<
            "}";
        }

        
    };

    /// @brief 手骨架帧数据
    struct ImiHandSkeleton : public ImiData  {
        int width;  ///< 
        int height; ///< 
        ImiHandSkeletonData handSkeletonDatas[2];   ///< 手骨架数据.

        TO_STRING_OP(ostream& os, const ImiHandSkeleton& a){
            return os << "{name:\"ImiHandSkeleton\", w:"<< a.width<<", h:"<<a.height<<", data:["<<a.handSkeletonDatas[0]<<","<<a.handSkeletonDatas[1]<<"]}";
        }

        bool empty() const { return width==0 && height==0; }
    };


    /// @brief 前景图数据.
    /// @ingroup1 imimrsdk
    struct ImiForegroundData : public ImiData {
        int width;      ///< 宽.
        int height;     ///< 高.
        uint32_t size;  ///< 数据大小.
        uint16_t* foregroundData; ///< 前景图数据.

        int getSize() const { return size; }

        TO_STRING_OP(ostream& os, const ImiForegroundData& a){
            return os << "{name=\"ForegroundData\", addr:[" << (long)&a <<"]{, size:\""<<a.width<<"x"<<a.height<<"\", len:"<<a.size<<"}";
        }

        bool empty() const { return width==0 && height==0; }

    };

    /// @brief 抠图数据.
    /// @ingroup1 imimrsdk
    struct ImiMattingData : public ImiData {
        int width;      ///< 
        int height;     ///< 
        uint32_t size;  ///< 
        // uint8_t* mattingData; ///<  抠图mask数据.
        //int getSize() const  { return size; }

        
        TO_STRING_OP(ostream& os, const ImiMattingData& a){
            return os<<"MattingData{size="<<a.width<<"x"<<a.height<<", len="<<a.size<<"}";
        }

        bool empty() const { return width==0 && height==0; }

    };

    
    /// @brief 人脸特征数据.
    struct ImiFaceFeatureData : public ImiData
    {
        uint32_t trackingID;                ///< 人脸跟踪ID.
        uint8_t bFeatureValid;              ///< flags = 1 时 faceFeature 有效.
        ImiRect faceRect;                   ///< 人脸框的位置.
        uint32_t faceSize;                  ///< 人脸尺寸.
        ImiFaceFeatureVec faceFeatures;     ///< 人脸特征向量.

        TO_STRING_OP(ostream& os, const ImiFaceFeatureData& a){
            return os << "ImiFaceFeatureData";
        }
    };

    
    /// @brief 
    struct ImiMoveTrackData : public ImiData {

        /// @brief 
        struct BallPosition
        {
            float fRadius;
            float x;
            float y;
            float z;
        };

        /// @brief 
        struct ImiBallRGBA
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;

            TO_STRING_OP(ostream& os, const ImiBallRGBA& a){
                return os << "";
            }
        };

        /// @brief 
        struct BallInfo
        {
            ImiBallRGBA rgb;
            BallPosition pos;

            TO_STRING_OP(ostream& os, const BallInfo& a){
                return os << "BallInfo";
            }
        };

        /// @brief 
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

        /// @brief     
        enum ButtonSate
        {
            BUTTOON_PRESS           = 0,
            BUTTOON_LONG_PRESS      = 1,
            BUTTOON_INVALID         = 2
        };  

        // 为了对齐, 与原版不同

        uint8_t nValid;
        uint8_t nSeq;
        uint8_t  rev1;
        uint8_t  rev2;
        float fADvoltage;
        ImiVector4 imuData;
        BallInfo ballInfo;
        ButtonSate btnState[8];

        TO_STRING_OP(ostream& os, const ImiMoveTrackData& a){
            return os << "ImiMoveTrackData";
        }

    };


    /// @brief
    struct ImiMoveData : public ImiData
    {
        ImiMoveTrackData moveTrackData[4];  ///< 

        TO_STRING_OP(ostream& os, const ImiMoveData& a){
            return os << "ImiMoveData";
        }
    };


    /// @brief 
    struct ImiSlamPoseData : public ImiData
    {
        enum SlamPoseTrackingState
        {
            CAMERA_POSE		= 0,
            RESET_POSE		= 1,
            INVALID_POSE	= 2,
        };

        SlamPoseTrackingState trackState;
        ImiMat4f cameraPose;

        TO_STRING_OP(ostream& os, const ImiSlamPoseData& a){
            return os << "ImiSlamPoseData";
        }
    };

    /// @brief 
    struct ImiPositionTrackData : public ImiData
    {
        //uint8_t             flag;
        ImiSlamPoseData     slamPose;
        ImiMoveTrackData    moveTrackData[4];
        ImiVector4          accData;

        TO_STRING_OP(ostream& os, const ImiPositionTrackData& a){
            return os << "ImiPositionTrackData";
        }
    };

#pragma pack(pop)

}