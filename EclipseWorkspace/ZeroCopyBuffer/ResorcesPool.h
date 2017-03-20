/*
 * ResorcesPool.h
 *
 *  Created on: Mar 19, 2017
 *      Author: Igor Bender
 */

#ifndef RESORCESPOOL_H_
#define RESORCESPOOL_H_

#include <memory>
#include <set>
#include <mutex>

#include <iostream>

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
        std::lock_guard<std::mutex> Lock(m_Lock);
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
        std::lock_guard<std::mutex> Lock(m_Lock);
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
    std::mutex m_Lock; ///< Thread safety lock.
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

#endif /* RESORCESPOOL_H_ */
