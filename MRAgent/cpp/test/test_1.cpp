
#include "ImiMrAgent.h"
#include "utils.h"
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <thread>

using namespace std;
using namespace imimr;

int main(int argc, char **argv) {
  printf("1\n");
  ImiContext *ctx = ImiContext::create(nullptr);
  printf("2\n");
  ctx->initialize("./mragent.toml");
  ImiBaseDevice dev = ctx->createDevice();
  dev->open((int)ImiSensorType::COLOR);
  printf("3\n");
  auto color = dev->getSensor(ImiSensorType::COLOR);

  int rc = color->start();
  printf("4\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  // 类型安全转换
  auto now = getCurrentTimeInMs();
  printf("begin color readFrame %lld\n", now);
  int i = 0;
  for (i = 0; i < 1000;) {
    // printf("before color->readFrame()\n");
    auto c_fr = color->readFrame();
    // printf("after color->readFrame()\n");
    IImiColorFrame *cf = dynamic_cast<IImiColorFrame *>(c_fr.get());
    static int prevFrameNum = 0;
    if (cf == nullptr) {
      printf("color frame is null\n");
    } else {
      int num = c_fr->getFrameNum();
      // printf("framenum:%d\n", num);
      if (cf->getDataSize() > 0 && num != prevFrameNum) {
        i++;
        prevFrameNum = num;
        printf("++++++ %d\n", i);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  printf("end color readFrame fps = %f\n",
         i / ((getCurrentTimeInMs() - now) / 1000.0));
  color->stop();
  printf("ptr count %d\n", dev.use_count());
  return 0;
}
