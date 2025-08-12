#include "ImiMrDebugFile.h"

// #define USE_LOG_FILE


ImiMrDebugFile::ImiMrDebugFile(const std::string& filename) {

#ifdef USE_LOG_FILE

#ifdef ANDROID
    debugFile.open("/data/local/tmp/" + filename, std::ios::out | std::ios::app);
#else
    debugFile.open(filename, std::ios::out | std::ios::app);
#endif
    
    if (!debugFile.is_open()) {
        throw std::runtime_error("Unable to open debug file");
    }
#else

#endif
}

ImiMrDebugFile::~ImiMrDebugFile() {

#ifdef USE_LOG_FILE
    if (debugFile.is_open()) {
        debugFile.close();
    }
#endif

}

#ifdef ANDROID
#include <android/log.h>
#endif

void ImiMrDebugFile::log(const char* tag, int frameNumber, int dataType, uint64_t timeStamp) {
#ifdef USE_LOG_FILE
    if (debugFile.is_open()) {
        debugFile << frameNumber << "," << dataType << "," << timeStamp << std::endl;
        debugFile.flush();
    }
#else

#ifdef ANDROID
    __android_log_print(ANDROID_LOG_INFO, "ImiLog", "[%s] %d,%d,%ld", tag, frameNumber , dataType , timeStamp);
#else
    printf("[MrAgentDebug] %d,%d,%ldn", tag, frameNumber , dataType , timeStamp);
#endif

#endif
}