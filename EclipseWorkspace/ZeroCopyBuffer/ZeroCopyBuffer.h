/*
 * ZeroCopyBuffer.h
 *
 *  Created on: Mar 19, 2017
 *      Author: Igor Bender
 */

#ifndef ZEROCOPYBUFFER_H_
#define ZEROCOPYBUFFER_H_

#include "CharBuffersPool.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

//typedef BufferWrapper<char> CharBufferType;
/// @struct Synch - notification synchronization primitives.
struct Synch
{
    std::mutex m_Mutex; ///< Mutual exclusive lock.
    std::condition_variable m_Event; ///< Condition variable.
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
    virtual ~Consumer()
    {
        while(!m_Queue.empty())
        {
            m_Queue.pop();
        }
    }

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
