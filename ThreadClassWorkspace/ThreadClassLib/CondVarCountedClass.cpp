/*
 *  CondVarCountedClass.cpp
 *
 *  Created on: 2021-12-03
 *  Author: Igor Bender
 *
 */
#include "CondVarCountedClass.h"
#include <time.h>

#ifndef _MSC_VER
#include <sys/time.h>
#else
#include <time.h>
#include <sys/timeb.h>
#endif

namespace ThreadClassLib
{

void CondVarCountedClass::signal()
{
    ++m_Counter;
    m_CondVar.notify_one();
}

bool CondVarCountedClass::wait(std::chrono::steady_clock::duration Timeout)
{
    std::unique_lock<std::mutex> Lock(m_Mutex);
    if(std::chrono::steady_clock::duration::zero() == Timeout)
    {
        m_CondVar.wait(Lock, [this](){ return true; });
        --m_Counter;
        return true;
    }
    else
    {
        bool Result = m_CondVar.wait_for(Lock, Timeout, [this](){ return true; });
        if(Result)
            --m_Counter;
    }
    return true;
}

} // end of ThreadClassLib namespace
