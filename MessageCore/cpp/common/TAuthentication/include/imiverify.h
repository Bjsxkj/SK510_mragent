#pragma once

#if !defined(IMI_VERIFY_API_EXPORT)
#if defined(_WIN32)
#define IMI_VERIFY_API_EXPORT __declspec(dllexport)
#else
#define IMI_VERIFY_API_EXPORT __attribute__ ((visibility("default")))
#endif
#endif

#define IMI_DLLEXPORT extern "C" IMI_VERIFY_API_EXPORT
// #else // __cplusplus
// #ifndef  USE_LIB 
// #ifdef __cplusplus
// #	define DLLEXPORT extern "C" IMI_VERIFY_API_EXPORT
// #else // __cplusplus
// #	define DLLEXPORT IMI_VERIFY_API_EXPORT
// #endif  // __cplusplus
// #endif 
#define IMI_MAX_PATH_LEN  256
#include <memory.h>
#include <stdint.h>

struct imi_verify_config {
    char filepath[IMI_MAX_PATH_LEN];    ///< license path in offlinemode or cache directory in online mode
    char appKey[128];				///< Application Key
    char uuid[128];					///< UUID of client
    char algorithmVersion[128];		///< Version of target algorithm
    char dataVersion[64];			///< 
 	void * device_Handle;			///< The handle of Huajie Imi Cammera.
    int authType;		///< 0:离线 1：在线 2 : first activate
	char authURL[128];              ///< authentication server url

    char fwVersion[128]; 		///< fw固件版本号
    char uvcVersion[64]; 		///< uvc版本号
    char bridgeVersion[64]; 	///< bridge版本号
	char hardwareVersion[64]; 	///< Hardware

    char uqeryType[64]; 		///< authorization Type; "validate"时,代表授权验证;当为"query"时,代表查询终端可用授权列表

};


// #ifndef WRAP_API

IMI_DLLEXPORT int imiVerify(imi_verify_config* config, uint8_t* token, const char* platinfo = NULL);

// #endif