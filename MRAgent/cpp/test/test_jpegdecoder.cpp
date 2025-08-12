#include <cstdint>
#include <fstream>
#include <vector>

#include "JpegHwDecoder.h"

int main_file() {
    using namespace imimr;
    JpegHwDecoder decoder;
    uint32_t width = 1920;
    uint32_t height = 1080;

    // 初始化解码器
    if (decoder.Init(1) != 0) {
        printf("Failed to initialize JPEG decoder\n");
        return -1;
    }
    printf("Init completed successfully\n");
    for (int i = 2; i <= 4; i++) {
        std::string jpgFileName = std::to_string(i) + ".jpg";
        std::ifstream jpgFile(jpgFileName, std::ios::binary);
        if (!jpgFile.is_open()) {
            printf("Failed to open JPEG file %s\n", jpgFileName.c_str());
            return -1;
        }
        std::vector<uint8_t> jpgData((std::istreambuf_iterator<char>(jpgFile)), std::istreambuf_iterator<char>());
        jpgFile.close();
        void *data = decoder.GetInputData();
        if (data == nullptr) {
            printf("Failed to get input data buffer\n");
            return -1;
        }
        memcpy(data, jpgData.data(), jpgData.size());
        printf("Read JPEG file successfully\n");
        if (decoder.Decode(1920, 1080) <= 0) {
            printf("Failed to decode JPEG data\n");
        } else {
            printf("Decode completed successfully\n");
            std::ofstream outputFile(std::to_string(i) + ".rgb", std::ios::binary);
            outputFile.write(reinterpret_cast<char *>(decoder.GetOutputData()), decoder.GetOutputDataSize());
            outputFile.close();
            printf("Write RGB file successfully\n");
        }
    }
    decoder.DeInit();
    printf("DeInit completed successfully\n");
    return 0;
}

#include "ImiMrAgent.h"

int main() {
    using namespace imimr;
    ImiContext *ctx = ImiContext::create(nullptr);
    ctx->initialize("./mragent.toml");
    ImiBaseDevice dev = ctx->createDevice();
    dev->open((int)ImiSensorType::COLOR);
    auto color = dev->getSensor(ImiSensorType::COLOR);
    // auto color = dev->getSensor(ImiSensorType::USERTRACK);
    int rc = color->start();
    printf("color start\n");
    int i = 0;
    for (i = 0; i < 1000;) {
        auto c_fr = color->readFrame();
        // printf("after color->readFrame()\n");
        IImiColorFrame *cf = dynamic_cast<IImiColorFrame *>(c_fr.get());
        // IImiUserTrackFrame *cf = dynamic_cast<IImiUserTrackFrame *>(c_fr.get());
        static int prevFrameNum = 0;
        if (cf == nullptr) {
            // printf("color frame is null\n");
        } else {
            int num = c_fr->getFrameNum();
            printf("framenum:%d\n", num);
            if (cf->getDataSize() > 0 && num != prevFrameNum) {
                // i++;
                prevFrameNum = num;
                printf("++++++ %d %d\n", i, cf->getDataSize());
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    color->stop();
    return 0;
}
