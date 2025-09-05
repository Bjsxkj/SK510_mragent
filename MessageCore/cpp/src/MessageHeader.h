#ifndef MESSAGE_HEADER_H
#define MESSAGE_HEADER_H

#include <stdint.h>

namespace mrcore
{

#pragma pack(push)
#pragma pack(1)
    // 为保证不同平台的一致性，必须为16byte
    typedef struct
    {
            uint8_t majorVersion;
            uint8_t minorVersion;
            uint16_t frameNum;
            uint64_t timeStamp;
            uint8_t dataType;
            uint8_t reserved[3];
            uint8_t backup[8];

    } MessageHeader;
#pragma pack(pop)

} // namespace mrcore

#endif
