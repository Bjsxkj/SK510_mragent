
#include <iostream>
#include <stdio.h>
#include "ImiMrTypes.h"

using namespace std;
using namespace imimr;

typedef struct 
{
    uint8_t majorVersion;
    uint8_t minorVersion;
    uint16_t frameNum;
    uint64_t timeStamp;
    uint8_t dataType;
    //uint8_t reserved[3];    

} MessageHeader;


// #if sizeof(MessageHeader)!=16
// #error "sizeof(MessageHeader)!=16"
// #endif

struct ImiMat3f1 : public ImiData { 
    float data[9];

};

#pragma pack(push, 1)
struct ImiSkeletonOrientationData1 : public ImiData
    {
        ImiMat3f hierarchicalRotation; ///< 本地旋转矩阵, 相对于父节点.
        ImiMat3f absoluteRotation;     ///< 绝对旋转矩阵, 相对于世界坐标系.
        ImiVector4 eulerAngles;        ///< 欧拉角, 相对于父节点, 旋转顺序暂定为Z-X-Y.
        int a;
    };

    struct ImiCameraIntrinsic1 : public ImiData {
        float fxParam;  ///< 焦距fx
        float fyParam;  ///< 焦距fy
        float cxParam;  ///< 主点x
        float cyParam;  ///< 主点y
        float k1Param;  ///< 切向畸变系数1
        float k2Param;  ///< 切向畸变系数2
        float k3Param;  ///< 切向畸变系数3
        float p1Param;  ///< 径向畸变系数1
        float p2Param;  ///< 径向畸变系数2
    };

    #pragma pack(pop)

int main(int argc, char** argv){

    printf("sizeof(MessageHeader)=%d, %d, %d\n",sizeof(MessageHeader), sizeof(MessageHeader)!=16, sizeof(imimr::ImiBodySkeletonData));

    imimr::ImiBodySkeleton ske;
    
    int DP= (long)&(ske.width)-(long)&ske;
    printf("address1 size %d\n", (char*)&ske.skeletonDatas[1] - (char*)&ske.skeletonDatas[0]);
    printf("address2 size %d\n", (char*)&ske.skeletonDatas[2] - (char*)&ske.skeletonDatas[1]);
    printf("%p, %p, dp=%d\n", &(ske.width), &ske, DP);

    printf("sizeof1 trackingState %d\n",
        sizeof(ske.skeletonDatas[0].trackingState));
    printf("sizeof2 trackingID %d\n",
            sizeof(ske.skeletonDatas[0].trackingID));
    printf("sizeof3 skeletonPositions %d\n",
            sizeof(ske.skeletonDatas[0].skeletonPositions[0])*20);
    printf("sizeof4 skeletonPositionTrackingStates %d\n",
            sizeof(ske.skeletonDatas[0].skeletonPositionTrackingStates[0])*20);
    printf("sizeof5 upSkeletonsOrientation %d\n",
            sizeof(ske.skeletonDatas[0].upSkeletonsOrientation));
    printf("sizeof6 skeletonOrientations %d\n",
            sizeof(ske.skeletonDatas[0].skeletonOrientations[0])*20);
    printf("sizeof7 feature %d\n",
            sizeof(ske.skeletonDatas[0].feature));
    printf("sizeof8 handState %d\n",
            sizeof(ske.skeletonDatas[0].handState));
    printf("sizeof9 handState %d\n",
            sizeof(ske.skeletonDatas[0].handRect));
    printf("sizeof10 ske.skeletonDatas[0].skeletonOrientations.hierarchicalRotation %d\n",
            sizeof(ske.skeletonDatas[0].skeletonOrientations[0].hierarchicalRotation));
    printf("sizeof11 ske.skeletonDatas[0].skeletonOrientations.absoluteRotation %d\n",
            sizeof(ske.skeletonDatas[0].skeletonOrientations[0].absoluteRotation));
    printf("sizeof12 ske.skeletonDatas[0].skeletonOrientations.eulerAngles %d\n",
            sizeof(ske.skeletonDatas[0].skeletonOrientations[0].eulerAngles));


            printf("sizeof13 ImiSkeletonOrientationData %d ImiData %d\n",sizeof(ImiSkeletonOrientationData), sizeof(ImiData));
            printf("sizeof14 ImiCameraIntrinsic %d ImiData %d\n",sizeof(ImiCameraIntrinsic1), sizeof(ImiData));
    

}