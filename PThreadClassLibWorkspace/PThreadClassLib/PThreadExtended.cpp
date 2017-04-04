/* PThreadExtended.cpp
 *
 * Original code by Igor Bender
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#include <PThreadExtended.h>
#include <signal.h>

void* PThreadExtended::threadLoop()
{
#ifdef __VXWORKS__
    TaskId = taskIdSelf();
#endif
    while(1)
    {
        m_ControlCondVar.lockMutex();
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pExitCondition->getPredicate())
#else
        static bool ExitPred = false;
        m_pExitCondition->getPredicate(ExitPred);
        if(ExitPred)
#endif
        {
            m_pExitCondition->setPredicate(false);
            m_ControlCondVar.unlockMutex();
            m_Running = false;
            return NULL;
        }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pStartCondition->getPredicate())
#else
        static bool StartPred = false;
        m_pStartCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_pStartCondition->setPredicate(false);
            m_ControlCondVar.unlockMutex();
            ftime(&m_StartTime);
            m_ThreadRoutine();
            if(m_Cyclic)
            {
                struct timeb TimeStruct;
                ftime(&TimeStruct);
                if(m_TimeOut && m_TimeOut > static_cast < unsigned > (((TimeStruct.time - m_StartTime.time) * 1000 + (TimeStruct.millitm - m_StartTime.millitm))))
                {
                    m_pWaitCondition->lockMutex();
                    m_pWaitCondition->wait(m_TimeOut - ((TimeStruct.time - m_StartTime.time) * 1000 + (TimeStruct.millitm - m_StartTime.millitm)));
                    m_pWaitCondition->unlockMutex();
                }
                m_pStartCondition->lockMutex();
                m_pStartCondition->setPredicate(true);
                m_pStartCondition->unlockMutex();
                continue;
            }
			else
			{
				if(m_StopNotificationFunc)
				{
					m_StopNotificationFunc(m_StopNotificationFuncArg);
				}
			}
            m_ControlCondVar.lockMutex();
        }

#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pExitCondition->getPredicate())
#else
        m_pExitCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_ControlCondVar.unlockMutex();
            return NULL;
        }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
        if(m_pStartCondition->getPredicate())
#else
        m_pStartCondition->getPredicate(StartPred);
        if(StartPred)
#endif
        {
            m_ControlCondVar.unlockMutex();
            continue;
        }
        int WaitResult = m_ControlCondVar.wait();
        if(WaitResult == 0)
        {
            m_ControlCondVar.unlockMutex();
            continue;
        }
        m_ControlCondVar.unlockMutex();
    }
    m_Running = false;
    return NULL;
}

void PThreadExtended::nameThread(std::string Name)
{
    m_Name = Name;
#if (__linux__) || (__SunOS)
#if (__linux__)
    pthread_t Dummy; // Empty thread
    bzero(reinterpret_cast < char* > (&Dummy), sizeof Dummy);
    if(!pthread_equal(Dummy, m_ThreadId) && 0 == pthread_kill(m_ThreadId, 0))
#else
    if(pthread_kill(m_ThreadId, 0) == 0)
#endif
    {
        pthread_setname_np(m_ThreadId, m_Name.c_str());
    }
#endif
}

#ifndef _WITHOUT_THREAD_EXCEPTIONS
bool PThreadExtended::run() throw (ThreadException&)
#else
int PThreadExtended::run()
#endif
{
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    PThreadClass::run();
    if(!m_Name.empty())
    {
        return 0 == pthread_setname_np(m_ThreadId, m_Name.c_str());
    }
    return true;
#else
    if(0 != PThreadClass::run())
        return -1;
    return pthread_setname_np(m_ThreadId, m_Name.c_str());
#endif
}

