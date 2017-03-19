/*
 * ZeroCopyBuffer.h
 *
 *  Created on: Mar 19, 2017
 *      Author: Igor Bender
 */

#ifndef ZEROCOPYBUFFER_H_
#define ZEROCOPYBUFFER_H_

#include "NotificationQueue.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

typedef BufferWrapper<char> CharBufferType;
typedef ResoucesPool<CharBufferType> CharBuffersPoolTtype;
typedef std::queue<Element<char>> CharBuffersQueueType;
typedef Element<char> CharBufferElement;

/// @struct Synch - notification synchronization primitives.
struct Synch
{
    std::mutex m_Mutex; ///< Mutual exclusive lock.
    std::condition_variable m_Event; ///< Condition variable.
};

/// @class BuffersPool - pool of memory allocated buffers.
class BuffersPool : public CharBuffersPoolTtype
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


////////////////////////////////////////////////////////////////////////////////////////////
/// Test classes.
///

class Consumer
{
public:
    class ThreadFunctor
    {
    public:
        void init(Consumer* pCons)
        {
            m_pCons = pCons;
        }
        void operator()();
    protected:
        Consumer* m_pCons;
    };
    Consumer() : m_Exit(false)
    {
        m_ThreadFunctor.init(this);
    }
    virtual ~Consumer() {}

    void notify(const CharBufferElement& e);
    void setExit()
    {
        m_Exit.store(true);
    }

    ThreadFunctor& functor()
    {
        return m_ThreadFunctor;
    }

protected:
    Synch m_Synch;
    CharBuffersQueueType m_Queue;
    std::atomic<bool> m_Exit;
    ThreadFunctor m_ThreadFunctor;
};

class Producer
{
public:
    class ThreadFunctor
    {
    public:
        void init(Producer* pProd)
        {
            m_pProd = pProd;
        }
        void operator()();
    protected:
        Producer* m_pProd;
    };
    Producer(Consumer* pC) : m_pCons(pC), m_Exit(false), m_Pool(1024, 4) { m_ThreadFunctor.init(this); }
    virtual ~Producer() {}
    void setExit()
    {
        m_Exit.store(true);
    }

    ThreadFunctor& functor()
    {
        return m_ThreadFunctor;
    }

protected:
    Consumer* m_pCons = nullptr;
    std::atomic<bool> m_Exit;
    ThreadFunctor m_ThreadFunctor;
    BuffersPool m_Pool;
};



#endif /* ZEROCOPYBUFFER_H_ */
