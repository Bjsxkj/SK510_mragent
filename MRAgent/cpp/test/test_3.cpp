#include <iostream>
#include <stdio.h>
#include "ImiMrAgent.h"

using namespace std;
using namespace imimr;

int main(int argc, char** argv){
    printf("1\n");
    ImiContext *ctx = ImiContext::create(nullptr);
    printf("2\n");
    ctx->initialize("./mragent.toml");
    auto dev = ctx->createDevice();
    
    for(int i=0;i<1;i++){
        dev->open((int)ImiSensorType::USERTRACK|(int)ImiSensorType::COLOR);
        printf("3\n");
        auto color = dev->getSensor(ImiSensorType::COLOR);
        //auto usertrack = dev->getSensor(ImiSensorType::USERTRACK);
       // usertrack->start();
       color->start();
        for(int i=0;i<10000;i++){
            // printf("befor color frame %d\n",i);
            //printf("usertrack frame %d\n",i);
                ImiBaseFrame c_fr = NULL;
                c_fr = color->readFrame();
                if(c_fr != NULL)
                {
                    int num = c_fr->getFrameNum();
                    if(i%100 == 0)
                    {
                        printf("color frame %d, num %d\n",i, num);
                    }
                    //printf("tttttt usertrack frame %d, num %d\n",i, num);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
        }

     /*    for(int i = 0; i< 20; i++){
            printf("start color and usertrack %d\n",i);
            int rc =color->start();
        
       


            for(int i=0;i<20;i++){
            // printf("befor color frame %d\n",i);
                auto c_fr = color->readFrame();
                //printf("color frame %d\n",i);
            
            }
            printf("5\n");
            color->stop();
            //dev->close();
            printf("6\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            printf("end color and usertrack %d\n",i);
        } */
        color->stop();
        printf("color->stop() %d\n",i);
        //usertrack->stop();
        printf("usertrack->stop() %d\n",i);
        dev->close();
        printf("dev->close %d\n",i);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}


