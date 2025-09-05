#include "ImiMrAgent.h"
#include "ShareMemory.h"
#include <cstdint>
#include <cstdio>
#include <vector>
#include <chrono>
#include <thread>

#pragma pack(push)
#pragma pack(1)
// 为保证不同平台的一致性，必须为 24 byte
typedef struct {
  uint8_t majorVersion;
  uint8_t minorVersion;
  uint16_t frameNum;
  uint64_t timeStamp;
  uint8_t dataType;
  uint8_t reserved[3];
  uint8_t a[8];
} ImiMessageHeader;
#pragma pack(pop)

int main() {
  /*SharedMemoryPool test_poll;
  if (test_poll.attach("COLOR")) {
    printf("attach success\n");
    ImiMessageHeader header;
    std::vector<uint8_t> data;
    data.resize(1920 * 1080 * 3);
    while (true) {
      if (test_poll.read(0, &header, &data[0], data.size()) > 0) {
        // printf("read success\n");
        printf("frameNum: %d, timeStamp: %llu, dataType: %d\n", header.frameNum, header.timeStamp, header.dataType);
      } else {
        // printf("read failed\n");
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  } else {
    printf("attach failed\n");
    return -1;
  }*/
  return 0;
}
