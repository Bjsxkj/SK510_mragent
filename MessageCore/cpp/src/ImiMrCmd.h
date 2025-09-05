#pragma once
#include <cstdint>

enum cmd_type
{
    CMD_GET_VERSION = 0,            ///< 无命令
    CMD_SET_PROPERTY = 1,           ///< 启动命令
    CMD_GET_PROPERTY = 2,           ///< 停止命令
    CMD_SET_SENSOR = 3,             ///< 暂停命令
    CMD_GET_SENSOR = 4,             ///< 恢复命令
    CMD_HEARTBEAT = 5,              ///< 心跳命令
    CMD_START_COLOR_SENSOR = 6,     ///< 启动彩色传感器命令
    CMD_START_DEPTH_SENSOR = 7,     ///< 启动深度传感器命令
    CMD_START_IR_SENSOR = 8,        ///< 启动红外传感器命令
    CMD_START_USERTRACK_SENSOR = 9, ///< 启动用户跟踪传感器命令
    CMD_STOP_COLOR_SENSOR = 10,     ///< 停止彩色传感器命令
    CMD_STOP_DEPTH_SENSOR = 11,     ///< 停止深度传感器命令
    CMD_STOP_IR_SENSOR = 12,        ///< 停止红外传感器命令
    CMD_STOP_USERTRACK_SENSOR = 13, ///< 停止用户跟踪传感器命令
    CMD_COLOR_ACK = 14,             ///< color ack
};

enum auth_cmd_type
{
    CMD_DO_AUTH = 0x1001,
    CMD_DO_AUTH_ACK = 0x1002,
};

#pragma pack(push, 1)
struct cmd_data_native
{
        cmd_type type;    // 4 字节
        int propId;       // 业务ID
        int cmdDataSize;  // 4 字节 (强制固定宽度)
        char cmdData[64]; // 64 字节
};
struct cmd_data_auth_req
{
        uint16_t type;              // 2 字节
        char uuid[128];             // 128字节，以0结尾的字符串
        char appKey[128];           // 128字节，以0结尾的字符串
        char algorithmVersion[128]; // 128字节，以0结尾的字符串
        uint8_t license[512];       // 512 字节二进制数据
};
struct cmd_data_auth_resp
{
        uint16_t type;    // 2 字节
        char result[128]; // 128字节，以0结尾的字符串
};
#pragma pack(pop)

// 编译时验证
static_assert(sizeof(cmd_data_native) == 76, "结构体大小必须严格为72字节");