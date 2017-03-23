/*
 * BoostQueueTest.cpp
 *
 *  Created on: Mar 3, 2017
 *      Author: Igor Bender
 */

#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <set>
#include <cstring>

/// @class ResoucesPool - template of resources pool class.
/// Resources pool is pool of dynamically allocated resources providing automatic
/// return of the acquired resource to the pool by using shared_ptr template with
/// custom deleter.
template <typename T> class ResoucesPool
{
public:
    /// @class BufferPtrWrapper - wrapper on memory allocated pointer.
    class PtrWrapper
    {
    public:
        /// Constructor.
        /// @param pPtr - allocated resource pointer.
        /// @param pPool - pointer to parent resource pool.
        PtrWrapper(T* pPtr, ResoucesPool* pPool) : m_pPtr(pPtr), m_pPool(pPool) {}
        /// Copy constructor.
        /// @param Wrapper - other PtrWrapper instance.
        PtrWrapper(const PtrWrapper& Wrapper) : m_pPtr(Wrapper.m_pPtr), m_pPool(Wrapper.m_pPool) {}
        /// Move constructor.
        /// @param Wrapper - other PtrWrapper instance.
        PtrWrapper(const PtrWrapper&& Wrapper) : m_pPtr(Wrapper.m_pPtr), m_pPool(Wrapper.m_pPool) {}
        /// Destructor.
        virtual ~PtrWrapper()
        {
            if(m_pPtr)
            {
                delete m_pPtr;
                m_pPtr = nullptr;
            }
        }
        /// assign operator
        /// @param Wrapper - other PtrWrapper instance.
        /// @return reference to this PtrWrapper instance.
        PtrWrapper& operator=(const PtrWrapper& Wrapper)
        {
            m_pPtr = Wrapper.m_pPtr;
            m_pPool = Wrapper.m_pPool;
            return *this;
        }

        T* m_pPtr = nullptr; ///< Allocated resource pointer.
        ResoucesPool* m_pPool = nullptr; ///< Parent resource pool pointer.
    };

    /// @lass Deleter - deleter functor to be used by shred_ptr<PtrWrapper> .
    class Deleter
    {
    public:
        /// Function operator
        /// @param p - pointer to PtrWrapper.
        virtual void operator()(PtrWrapper* p)
        {
            if(p && p->m_pPool)
                p->m_pPool->reclaimRecource(p);
        }
    };
    /// @typedef shared_ptr of PtrWrapper.
    typedef std::shared_ptr<PtrWrapper> ResourcePtrType;
    /// Constructor.
    /// @param Number - number of resources to be allocated.
    ResoucesPool() {}
    /// Destructor.
    virtual ~ResoucesPool()
    {
        for(auto i : m_PoolFree)
            delete i;
        m_PoolFree.clear();
    }
    /// Insert resource into the pool.
    /// @param pResource - pointer to allocated resource.
    void insertResource(T* pResource)
    {
        m_PoolFree.insert(new PtrWrapper(pResource, this));
    }
    /// Acquire resource from the pool.
    /// @return - shared_ptr of a resource, containing nullptr if no free resources.
    ResourcePtrType acqireResource()
    {
        std::cout << "Acquire resource" << std::endl;
        PtrWrapper Buff(nullptr, nullptr);
        if(m_PoolFree.empty())
        {
            return ResourcePtrType(&Buff);
        }
        ResoucesPool::Deleter Deleter;
        ResourcePtrType pBuff(*m_PoolFree.begin(), Deleter);
        m_PoolAcquired.insert(*m_PoolFree.begin());
        m_PoolFree.erase(m_PoolFree.begin());
        Buff.m_pPool = this;
        return pBuff;
    }
    /// Return resource to the pool.
    /// @param pPtr - pointer to a resource.
    void reclaimRecource(PtrWrapper* pPtr)
    {
        m_PoolFree.insert(pPtr);
        m_PoolAcquired.erase(pPtr);
        std::cout << "Reclaim resource" << std::endl;
    }
    /// @return - nubber of resources allocated.
    uint32_t capacity()
    {
        return m_PoolFree.size() + m_PoolAcquired.size();
    }
    /// @return - number of available resources.
    uint32_t size()
    {
        return m_PoolFree.size();
    }

protected:
    std::set<PtrWrapper*> m_PoolFree; ///< Available resources storage.
    std::set<PtrWrapper*> m_PoolAcquired; ///< Acquired resources storage.
};

/// @struct BufferWrapper - wrapper on buffer of entities.
template <typename T> struct BufferWrapper
{
    BufferWrapper(int Size)
    {
        m_pBuffer = new T[Size];
    }
    virtual ~BufferWrapper()
    {
        if(nullptr != m_pBuffer)
            delete [] m_pBuffer;
        std::cout << "Free resource" << std::endl;
    }
    operator T*() { return m_pBuffer; }

    T* m_pBuffer = nullptr;
};

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

/// @struct Element - queue element.
struct Element
{
    uint32_t EventType; ///< Type discriminator.
    BuffersPool::ResourcePtrType m_pBuff; ///< shared_ptr to buffer wrapper.
    /// Default constructor. Needs to be defined explicitly for older versions of GCC and BOOST.
    Element()
    {
        EventType = 0xFFFFFFFF;
        BuffersPool::Deleter Deleter;
        BuffersPool::ResourcePtrType pBuff(nullptr, Deleter);
        m_pBuff = pBuff;
    }
    /// Constructor.
    Element(uint32_t Type, BuffersPool::ResourcePtrType pBuff) : EventType(Type), m_pBuff(pBuff) {}
    /// Copy constructor.
    Element(const Element& e) : EventType(e.EventType), m_pBuff(e.m_pBuff) {}
    /// Assign operator.
    Element& operator=(const Element& e)
    {
        EventType = e.EventType;
        m_pBuff = e.m_pBuff;
        return *this;
    }
};

/// @struct Synch - notification synchronization primitives.
struct Synch
{
    std::mutex m_Mutex; ///< Mutual exclusive lock.
    std::condition_variable m_Event; ///< Condition variable.
};

/// @typedef - BOOST lock free queue with single provider and single consumer.
typedef boost::lockfree::spsc_queue<Element, boost::lockfree::capacity<1024> > EventQueueType;


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
    EventQueueType m_Queue;
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

// Test.
int main(int argc, char* argv[])
{
    Consumer Cons;
    Producer Prod(&Cons);

    std::thread ConsumerThread(Cons.functor());
    std::thread ProducerThread(Prod.functor());

    std::this_thread::sleep_for(std::chrono::seconds{10});

    Prod.setExit();
    Cons.setExit();
    ProducerThread.join();
    ConsumerThread.join();

    return 0;
}

void Consumer::ThreadFunctor::operator()()
{
    while(!m_pCons->m_Exit.load())
    {
        {
            // Wait for notification.
            std::unique_lock<std::mutex> Lock(m_pCons->m_Synch.m_Mutex);
            m_pCons->m_Synch.m_Event.wait_for(Lock, std::chrono::milliseconds{1200});
        }
        while(!m_pCons->m_Queue.empty())
        {
            // Process the whole queue.
            {
                Element e(0, BuffersPool::ResourcePtrType(nullptr));
                m_pCons->m_Queue.pop(e);
                std::cout << e.EventType << " " << *(e.m_pBuff->m_pPtr) << std::endl;
            }
        }
    }
}

void Consumer::notify(const Element& e)
{
    m_Queue.push(e);
    std::unique_lock<std::mutex> Lock(m_Synch.m_Mutex);
    m_Synch.m_Event.notify_one();
}


void Producer::ThreadFunctor::operator()()
{
    while(!m_pProd->m_Exit.load())
    {
        static uint32_t Count = 0;
        {
            // Acquire buffer from pool.
            BuffersPool::ResourcePtrType pBuff = m_pProd->m_Pool.acqireResource();
            if(!pBuff.get())
            {
                // If no free buffers wait and continue.
                std::this_thread::sleep_for(std::chrono::milliseconds{200});
                continue;
            }
            // The buffer had successfully acquired.
            Element e(0, pBuff);
            e.EventType = Count++; // Set type.
            strcpy(*(e.m_pBuff->m_pPtr), "Hello"); // Fill buffer.
            m_pProd->m_pCons->notify(e); // Notify the consumer.
            if(5 == Count) { // At count equals 5 double the action
                BuffersPool::ResourcePtrType pBuff = m_pProd->m_Pool.acqireResource();
                if(!pBuff.get())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds{200});
                    continue;
                }
                Element e(0, pBuff);
                e.EventType = Count++;
                strcpy(*(e.m_pBuff->m_pPtr), "Hello2");
                m_pProd->m_pCons->notify(e);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

