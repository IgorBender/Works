/* 
 * File:   PThreadsPool.h
 * Author: igor
 *
 * Created on November 2, 2009, 8:37 PM
 *
 * Pool of threads. All threads created in advance with defined priority.
 *
 */


#ifndef _PTHREADSPOOL_H
#define	_PTHREADSPOOL_H

#include <map>
#include <queue>
using namespace std;

#include <semaphore.h>
#include <PoolThread.h>
#include <PMutexClass.h>

typedef map <int, PoolThread*> ThreadsSetType;
typedef pair <int, PoolThread*> ThreadSetElementType;
typedef ThreadsSetType::iterator ThreadSetIteratortype;

typedef queue <PoolThread*> FreeThreadsSetType;

class PThreadsPool;

struct JobParameters
{
    PThreadsPool* m_pPool;
    PoolThread* m_pThread;
    PThreadRoutineType m_pJobRoutine;
    void* m_pJobArgument;
    PThreadCleanUpRoutineType m_pCleanUpRoutine;
    void* m_pCleanUpArgument;
    JobParameters() : m_pPool(NULL), m_pThread(NULL), m_pJobRoutine(NULL), m_pJobArgument(NULL), m_pCleanUpRoutine(NULL), m_pCleanUpArgument(NULL)
    {}
};

class PThreadsPool
{
public:
    PThreadsPool(unsigned int NumberOfThreadsInPool, int BaseThreadPriority = 0);
    ~PThreadsPool();

   static void* threadControlRoutine(void* pContext);
   static void threadCleanUpRoutine(void* pContext);

   bool doJob(PThreadRoutineType pJobRoutine, void* pJobArgument, PThreadCleanUpRoutineType pCleanUp = NULL, void* pCleanUpArg = NULL);

   int getNumberOfFreeThreads()
   {
       int Value = -1;
       sem_getvalue(&m_ThreadsSemaphor, &Value);
       return Value;
   }
   
   int getNumberOfThreads()
   {
       return m_NumberOfThreadsInPool;
   }

protected:
    ThreadsSetType m_Threads;
    int m_NumberOfThreadsInPool;
    int m_BaseThreadPriority;
    FreeThreadsSetType m_FreeThreads;
    PMutexClass* m_pFreeThreadsMutex;
    sem_t m_ThreadsSemaphor;
   
private:
    JobParameters* m_pRunParams;

    PoolThread* getFreeThreadPtr();
};

#endif	/* _PTHREADSPOOL_H */

