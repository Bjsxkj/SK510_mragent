
#include "MessageCoreApi.h"

#include <iostream>

using namespace imimr;
using namespace mrcore;
using namespace std;

class MyCore : public MessageCore {

public:
    virtual void onError(int errCode, const char *err) override {
        printf("[E:%d] %s\n", errCode, err);
    }

    virtual void onLog(int level, const char *msg) override{
        printf("%s\n", msg);
    }
    
    virtual std::string onCommandHandler(cmd_data* cmd){
       // char a[64];
        //memcpy(a, cmd->cmdData.c_str(), cmd->cmdDataSize);
        printf("onCommandHandler cmd=%d, propid=%d, cmddata=%s\n", cmd->cmd,cmd->propId,cmd->cmdData.c_str ());
        return "ok";
    }

};

int main(int argc, char** argv){
    printf("1\n");
    MyCore core;
    printf("before initialize] \n");
    core.initialize("./config.ini");
    printf("[I] initialize\n");
    // core.prepare(nullptr, nullptr);  
    core.start(1);

    printf("[I] start\n");
#if 0
    //初始化SDK时调用
   // ImiContext& ctx = ImiContext.getContext();
    printf("2\n");
   // std::vector<ImiDeviceAttribute> dev_list;
    auto dev_list = ctx.getDeviceList();
     
    if (!dev_list.empty()) {      
        // 创建设备
        printf("have device found\n");
    } else {
        printf("No device found\n");
        return -1;
    }
    /*
    auto dev = ctx->createDevice(dev_list[0]);
   
    int type = ImiSensorType.COLOR | ImiSensorType.USERTRACK;
    dev->open(type);
    auto color = dev->getSensor(ImiSensorType.COLOR);
    auto usertrack = dev->getSensor(ImiSensorType.USERTRACK);

    int rc =color->start();
    rc = usertrack->start();
    */
  #endif      
    string l;
    while(!(l=="q" || l=="Q")){
        cin >> l;
        cout << ":" << l << ">" << endl;
        if(l.at(0)=='q' || l=="q"||l=="Q"){
            break;
        }
    }

    cout << "[I] stop0\n" << endl;

    //color->stop();
    //usertrack->stop();

    core.stop();

    cout << "[I] stop\n";


    core.join();

    cout << "[I] stopdddddddddddddddddddddddd\n";
    core.release();

    
    cout << "[I] stopddddddddddddfasdfadsfasdf ddddddddddddd\n";
    return 0;
}