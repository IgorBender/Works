/*
 * NotificationQueue.h
 *
 *  Created on: Mar 19, 2017
 *      Author: Igor Bender
 */

#ifndef NOTIFICATIONQUEUE_H_
#define NOTIFICATIONQUEUE_H_

#include "ResorcesPool.h"

/// @struct Element - queue element.
template <typename T> struct Element
{
    uint32_t EventType; ///< Type discriminator.
    typename ResoucesPool<BufferWrapper<T> >::ResourcePtrType m_pBuff; ///< shared_ptr to buffer wrapper.
    Element(uint32_t Type, typename ResoucesPool<BufferWrapper<T> >::ResourcePtrType pBuff) : EventType(Type), m_pBuff(pBuff) {}
    Element(const Element& e) : EventType(e.EventType), m_pBuff(e.m_pBuff) {}
    Element& operator=(const Element& e)
    {
        EventType = e.EventType;
        m_pBuff = e.m_pBuff;
        return *this;
    }
};

#endif /* NOTIFICATIONQUEUE_H_ */
