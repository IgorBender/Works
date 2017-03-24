/*
 * CharBuffersPool.h
 *
 *  Created on: Mar 24, 2017
 *      Author: Igor Bender
 */

#ifndef CHARBUFFERSPOOL_H_
#define CHARBUFFERSPOOL_H_

#include "ResorcesPool.h"

/// @class BuffersPool - pool of memory allocated buffers.
class BuffersPool : public ResoucesPool<BufferWrapper<char>>
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
            std::cout << "Allocate resource" << std::endl;
        }
    }
};

typedef ElementTemplate<char> Element;

#endif /* CHARBUFFERSPOOL_H_ */
