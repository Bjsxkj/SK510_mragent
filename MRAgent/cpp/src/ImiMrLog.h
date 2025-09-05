#pragma once

#ifdef NO_LOG
#    define LOGD(...)
#    define LOGE(...)
#    define LOGI(...)
#else // NO_LOG

// 定义日志等级
#    define LOG_LEVEL_DEBUG 0
#    define LOG_LEVEL_INFO 1
#    define LOG_LEVEL_ERROR 2
#    define LOG_LEVEL_NONE 3

// 默认日志等级为DEBUG
#    ifndef LOG_LEVEL
#        define LOG_LEVEL LOG_LEVEL_INFO
#    endif

#    ifdef ANDROID
#        include <android/log.h>
#        define LOG_TAG "MrAgent"
#        define LOGFMT(L, ...) __android_log_print(L, LOG_TAG, ##__VA_ARGS__);

// Android环境下的日志宏定义
#        if LOG_LEVEL <= LOG_LEVEL_DEBUG
#            define LOGD(...) LOGFMT(ANDROID_LOG_DEBUG, ##__VA_ARGS__)
#        else
#            define LOGD(...)
#        endif

#        if LOG_LEVEL <= LOG_LEVEL_INFO
#            define LOGI(...) LOGFMT(ANDROID_LOG_INFO, ##__VA_ARGS__)
#        else
#            define LOGI(...)
#        endif

#        if LOG_LEVEL <= LOG_LEVEL_ERROR
#            define LOGE(...) LOGFMT(ANDROID_LOG_ERROR, ##__VA_ARGS__)
#        else
#            define LOGE(...)
#        endif

#        define LOGDS(...)

#    else // ANDROID

// 非Android环境下的日志宏定义
#        if LOG_LEVEL <= LOG_LEVEL_DEBUG
#            define LOGD(fmt, ...)                                                                                     \
                printf("\033[0m\033[1;43m [D:%s, %d] " fmt " \033[0m \n", __FILE__, __LINE__, ##__VA_ARGS__)
#        else
#            define LOGD(...)
#        endif

#        if LOG_LEVEL <= LOG_LEVEL_INFO
#            define LOGI(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#        else
#            define LOGI(...)
#        endif

#        if LOG_LEVEL <= LOG_LEVEL_ERROR
#            define LOGE(fmt, ...) printf("\033[0m\033[1;31m" fmt " \033[0m \n", ##__VA_ARGS__)
#        else
#            define LOGE(...)
#        endif

// 流式日志仅在DEBUG模式生效
#        if LOG_LEVEL <= LOG_LEVEL_DEBUG
#            define LOGDS(fmt, ...)                                                                                    \
                {                                                                                                      \
                    std::cout << fmt << ##__VA_ARGS__;                                                                 \
                    std::cout << std::endl;                                                                            \
                }
#        else
#            define LOGDS(...)
#        endif

#    endif // ANDROID

// 兼容旧宏定义
#    define LOGED(fmt, ...) LOGD(fmt, ##__VA_ARGS__)

#endif // NO_LOG