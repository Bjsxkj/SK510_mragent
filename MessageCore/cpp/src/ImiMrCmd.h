enum cmd_type
{
    CMD_GET_VERSION = 0,            ///< 无命令
    CMD_SET_PROPERTY = 1,           ///< 启动命令
    CMD_GET_PROPERTY = 2,           ///< 停止命令
    CMD_SET_SENSOR = 3,             ///< 暂停命令
    CMD_GET_SENSOR = 4,             ///< 恢复命令
    CMD_heartbeat = 5,              ///< 心跳命令
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

#pragma pack(push, 1)
struct cmd_data_native
{
        cmd_type cmdtype; // 4 字节
        int propId;       // 业务ID
        int cmdDataSize;  // 4 字节 (强制固定宽度)
        char cmdData[64]; // 64 字节
};
#pragma pack(pop)

// 编译时验证
static_assert(sizeof(cmd_data_native) == 76, "结构体大小必须严格为72字节");