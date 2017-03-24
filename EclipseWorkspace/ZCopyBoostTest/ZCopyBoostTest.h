/*
 * ZCopyBoostTest.h
 *
 *  Created on: Mar 24, 2017
 *      Author: Igor Bender
 */

#ifndef ZCOPYBOOSTTEST_H_
#define ZCOPYBOOSTTEST_H_

#include "CharBuffersPool.h"
#include <boost/lockfree/spsc_queue.hpp>
#include <mutex>
#include <condition_variable>

/// @struct Synch - notification synchronization primitives.
struct Synch
{
    std::mutex m_Mutex; ///< Mutual exclusive lock.
    std::condition_variable m_Event; ///< Condition variable.
};


/// @typedef - BOOST lock free queue with single provider and single consumer.
typedef boost::lockfree::spsc_queue<Element, boost::lockfree::capacity<1024> > CharBuffersQueueType;

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

    void notify(const Element& e);
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

#endif /* ZCOPYBOOSTTEST_H_ */
