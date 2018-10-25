
#include <vector>

#include <PThreadsPool.h>


PThreadsPool::PThreadsPool(unsigned int NumberOfThreadsInPool, int BaseThreadPriority) : m_NumberOfThreadsInPool(NumberOfThreadsInPool),
    m_BaseThreadPriority(BaseThreadPriority), m_pFreeThreadsMutex(NULL), m_pRunParams(NULL)
{
    m_pRunParams = new JobParameters[m_NumberOfThreadsInPool];
    m_pFreeThreadsMutex = new PMutexClass;
    for(unsigned int i = 0; i < NumberOfThreadsInPool; ++i)
    {
        m_Threads.insert(ThreadSetElementType(i, new PoolThread(i)));
        m_FreeThreads.push(m_Threads[i]);
        m_Threads[i]->setPolicy(SCHED_FIFO);
        m_Threads[i]->run();
        m_Threads[i]->setCancelType(PTHREAD_CANCEL_ASYNCHRONOUS);
        m_Threads[i]->setPriority(m_BaseThreadPriority);
    }
    sem_init(&m_ThreadsSemaphor, 0, NumberOfThreadsInPool);
}

PThreadsPool::~PThreadsPool()
{
    for(ThreadSetIteratortype i = m_Threads.begin(); i != m_Threads.end(); ++i)
    {
        i->second->exit();
        while(i->second->isRunning())
        {
#ifndef WIN32
			timespec Delay;
            Delay.tv_sec = 0;
            Delay.tv_nsec = 100;
            nanosleep(&Delay, NULL);
#else
			Sleep(1);
#endif
        }
        delete i->second;
    }
    m_Threads.clear();
    sem_destroy(&m_ThreadsSemaphor);
    delete m_pFreeThreadsMutex;
    m_pFreeThreadsMutex = NULL;
    delete [] m_pRunParams;
    m_pRunParams = NULL;
}

PoolThread* PThreadsPool::getFreeThreadPtr()
{
    sem_wait(&m_ThreadsSemaphor);
    m_pFreeThreadsMutex->lockMutex();
    PoolThread* pThread = m_FreeThreads.front();
    m_FreeThreads.pop();
    m_pFreeThreadsMutex->unlockMutex();
    return pThread;
}

bool PThreadsPool::doJob(PThreadRoutineType pJobRoutine, void* pJobArgument, PThreadCleanUpRoutineType pCleanUp, void* pCleanUpArg)
{
    PoolThread* pThread = getFreeThreadPtr();
    pThread->setPriority(m_BaseThreadPriority);
    m_pRunParams[pThread->getPoolThreadId()].m_pPool = this;
    m_pRunParams[pThread->getPoolThreadId()].m_pThread = pThread;
    m_pRunParams[pThread->getPoolThreadId()].m_pJobRoutine = pJobRoutine;
    m_pRunParams[pThread->getPoolThreadId()].m_pJobArgument = pJobArgument;
    m_pRunParams[pThread->getPoolThreadId()].m_pCleanUpRoutine = pCleanUp;
    m_pRunParams[pThread->getPoolThreadId()].m_pCleanUpArgument = pCleanUpArg;
    pThread->registerRoutine(PThreadRoutineType(PThreadsPool::threadControlRoutine));
    pThread->registerRutinesAgrument(&m_pRunParams[pThread->getPoolThreadId()]);
    pThread->registerCleanUp(PThreadCleanUpRoutineType(PThreadsPool::threadCleanUpRoutine));
    pThread->registerCleanUpAgrument(&m_pRunParams[pThread->getPoolThreadId()]);
    pThread->start();
    return true;
}

void* PThreadsPool::threadControlRoutine(void* pContext)
{
    JobParameters* pParams = reinterpret_cast<JobParameters*>(pContext);
    void* pReturn = pParams->m_pJobRoutine(pParams->m_pJobArgument);
    pParams->m_pPool->m_pFreeThreadsMutex->lockMutex();
    pParams->m_pPool->m_FreeThreads.push(pParams->m_pThread);
    pParams->m_pPool->m_pFreeThreadsMutex->unlockMutex();
    sem_post(&(pParams->m_pPool->m_ThreadsSemaphor));
    return pReturn;
}

void PThreadsPool::threadCleanUpRoutine(void* pContext)
{
    JobParameters* pParams = reinterpret_cast<JobParameters*>(pContext);
    if(pParams->m_pCleanUpRoutine)
    {
        pParams->m_pCleanUpRoutine(pParams->m_pCleanUpArgument);
    }
//    pParams->m_pThread->run();
//    pParams->m_pPool->m_pFreeThreadsMutex->lockMutex();
//    pParams->m_pPool->m_FreeThreads.push(pParams->m_pThread);
//    pParams->m_pPool->m_pFreeThreadsMutex->unlockMutex();
//    sem_post(&(pParams->m_pPool->m_ThreadsSemaphor));
//    cout << "---" << endl;
}
