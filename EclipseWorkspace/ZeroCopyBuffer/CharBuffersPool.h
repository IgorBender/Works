/*
 * CharBuffersPool.h
 *
 *  Created on: Mar 25, 2017
 *      Author: Igor Bender
 */

#ifndef CHARBUFFERSPOOL_H_
#define CHARBUFFERSPOOL_H_

#include "ResorcesPool.h"
#include <queue>

/// @typedef CharBuffersPoolType - ResourcesPool instantiated on buffers of chars.
typedef ResoucesPool<BufferWrapper<char>> CharBuffersPoolType;
/// @typedef CharBufferElement - wrapper element instantiated on buffer of chars.
typedef Element<char> CharBufferElement;
/// @typedef CharBuffersQueueType - queue of Elements instantiated on buffer of chars.
typedef std::queue<CharBufferElement> CharBuffersQueueType;

/// @class BuffersPool - pool of memory allocated buffers.
class BuffersPool : public CharBuffersPoolType
{
public:
    /// Constructor.
    /// @param Size - size of buffers to be allocated.
    /// @param Number - number of buffers to be allocated.
    BuffersPool(size_t Size, uint32_t Number)
    {
        for(uint32_t i = 0; i < Number; ++i)
        {
            insertResource(new BufferWrapper<char>(Size));
//            std::cout << "Allocate resource" << std::endl;
        }
    }
};

#endif /* CHARBUFFERSPOOL_H_ */
