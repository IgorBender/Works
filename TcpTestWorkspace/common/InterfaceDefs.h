/*
 * InterfaceDefs.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Igor Bender
 */

#ifndef COMMON_INTERFACEDEFS_H_
#define COMMON_INTERFACEDEFS_H_

#include <cstdint>

const uint32_t EXIT_MSG_TYPE = 1;

struct MessageHeader
{
    uint32_t MessageType;
    uint32_t MessageSize;
};


struct MessageType1Body
{
    uint32_t Field1;
    uint32_t Field2;
};

struct MessageType1
{
    MessageHeader Header;
    MessageType1Body Body;
};

struct MessageType2Body
{
    uint32_t Field1[1024];
};

struct MessageType2
{
    MessageHeader Header;
    MessageType2Body Body;
};

typedef MessageHeader ExitMessage;

union AllMessages
{
    MessageType1 Type1;
    MessageType2 Type2;
    ExitMessage Exit;
};

#ifndef _WIN32
constexpr uint32_t MaxMessageSize() { return sizeof(AllMessages); }
#else
#define MaxMessageSize() (sizeof(AllMessages))
#endif

#endif /* COMMON_INTERFACEDEFS_H_ */
