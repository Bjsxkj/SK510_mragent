// 测试获取骨架

// #include <gtest.h>

#include "ImiMrAgent.h"

#include "log_util.h"

#define LOG_D(fmt, ...) printf(fmt "\n", ## __VA_ARGS__);

using namespace imimr;



int main(int argc, char** argv) {

    // LOG_D("version:%s", IMI_MR_API_VERSION)


    ImiContext* ctx = ImiContext::create(nullptr);

    ctx->initialize("./mragent.toml");


    if(1){

        auto dev =   ctx->createDevice();
        if(dev){
            cout << "dev=" << dev << endl;
        dev->open((int)imimr::ImiSensorType::USERTRACK);
        auto sensor = dev->getSensor(imimr::ImiSensorType::USERTRACK);

        if(sensor){
            cout << sensor << endl;
            sensor->start();
            for(int i = 0;i < 500; i++){
                const int MAX_DT = 120;
                static int64_t last = 0;
                int64_t now;
                {
                    //LOG_TS(ts1,"readFrame time")
                    auto fr = sensor->readFrame();
                    if(fr){
                        now = getTimeInMsForNow();
                        int64_t ts =  fr->getTimeStamp();
                        auto dt = now - ts;

                        printf("fr: num=%d, delay=%ldms, ts=%lld\n", fr->getFrameNum(), dt, ts);
                        // cout << "fr:" << fr->getFrameNum() << endl;
                    }else{
                        now = getTimeInMsForNow();
                    }
                }

                if((now - last)<MAX_DT){ //
                    int wait = MAX_DT-(now - last);
                    printf("wait %dms\n");
                    this_thread::sleep_for(chrono::milliseconds(wait));
                }
                last = now;
                //this_thread::sleep_for(chrono::milliseconds(80));
            }

            sensor->stop();

        }


        }else{
            cout << "dev = null";
        }

    }

    // ImiContext::destroy();
}