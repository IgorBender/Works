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

/// @struct Element - queue element.
template <typename T> struct Element
{
    uint32_t EventType = 0; ///< Type discriminator.
    typename ResoucesPool<BufferWrapper<T>>::ResourcePtrType m_pBuff; ///< shared_ptr to buffer wrapper.
    Element() {}
    Element(uint32_t Type, typename ResoucesPool<BufferWrapper<T> >::ResourcePtrType pBuff) : EventType(Type), m_pBuff(pBuff) {}
    Element(const Element& e) : EventType(e.EventType), m_pBuff(e.m_pBuff) {}
    Element& operator=(const Element& e)
    {
        EventType = e.EventType;
        m_pBuff = e.m_pBuff;
        return *this;
    }
};

/// @struct CharBufferElement - queue element for char buffer.
struct CharBufferElement : public Element<char>
{
    CharBufferElement() {}
    CharBufferElement(size_t ActualSize, uint32_t Type, typename CharBuffersPoolType::ResourcePtrType pBuff) : Element<char>(Type, pBuff), m_ActualSize(ActualSize) {}
    CharBufferElement(uint32_t Type, typename CharBuffersPoolType::ResourcePtrType pBuff) : Element<char>(Type, pBuff) {}
    CharBufferElement(const CharBufferElement& e) : Element<char>(e.EventType, e.m_pBuff), m_ActualSize(e.m_ActualSize) {}
    CharBufferElement& operator=(const CharBufferElement& e)
    {
        m_ActualSize = e.m_ActualSize;
        EventType = e.EventType;
        m_pBuff = e.m_pBuff;
        return *this;
    }
    size_t m_ActualSize = 0;
};

/// @typedef CharBuffersQueueType - queue of Elements.
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
