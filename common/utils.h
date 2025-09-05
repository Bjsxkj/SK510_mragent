#ifdef MESSAGECORE
#pragma once

#include <string>

std::string generate_unique_name();

std::string generate_filename(const std::string& baseName, const std::string& extension);

void write_ppm(const char *filename, int width, int height, unsigned char *data);

uint64_t getCurrentTimeInMs();
#endif

#ifdef MRAGENT
#pragma once

#include "ImiMrTypes.h"
#ifdef ANDROID
#include <jni.h>
#endif

uint64_t getCurrentTimeInMs();
#define EXECUTE_EVERY_N_SECONDS(stmt, duration)                             \
    do {                                                                    \
        static uint64_t _last_exec_time = 0;                                \
        uint64_t _now = getCurrentTimeInMs();                               \
        if (_last_exec_time == 0) {                                         \
            stmt;                                                           \
            _last_exec_time = _now;                                         \
        } else if (_now - _last_exec_time >= (uint64_t)(duration) * 1000) { \
            stmt;                                                           \
            _last_exec_time = _now;                                         \
        }                                                                   \
    } while (0)

#ifdef ANDROID
extern "C" JNIEXPORT jobject JNICALL Java_com_sximi_mragent_IImiColorFrame_toByteBuffer(JNIEnv *env, jobject obj, jlong address, jint length);
#endif
#endif
