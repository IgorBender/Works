/* CondVarClass.cpp
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
#include "CondVarClass.h"
#include <memory>
#include <iostream>

#include <time.h>

#ifndef _WIN32
#include <sys/time.h>
#else
#include <time.h>
#include <sys/timeb.h>
#endif

namespace ThreadClassLib
{
CondVarClass::CondVarClass(int PredNum) : m_PredNum(PredNum)
{
    if(PredNum > MAX_NUM_OF_PREDS)
    {
        THREAD_EXCEPT_THROW("Too much predicates");
    }
    m_Predicates = 0U;
}

CondVarClass::PredicateIdType CondVarClass::addPredicate(bool PredicateValue)
{
    if(m_PredNum >= MAX_NUM_OF_PREDS)
    {
        THREAD_EXCEPT_THROW("Too much predicates");
    }
    setPredicate(m_PredNum++, PredicateValue);
    return m_PredNum - 1;
}

bool CondVarClass::getPredicate(PredicateIdType PredNum)
{
    if(m_PredNum <= PredNum)
    {
        THREAD_EXCEPT_THROW("Wrong predicate index");
    }
    uint32_t Mask = 1U << PredNum;
    return 0 == (m_Predicates & Mask) ? false : true;
}

void CondVarClass::setPredicate(PredicateIdType PredNum, bool PredicateValue)
{
    if(m_PredNum <= PredNum)
    {
        THREAD_EXCEPT_THROW("Wrong predicate index");
    }
	uint32_t Mask = 1 << PredNum;
    if(PredicateValue)
    {
    	m_Predicates |= Mask;
    }
    else
    {
    	Mask = ~Mask;
    	m_Predicates &= Mask;
    }
}

void CondVarClass::signal()
{
    m_CondVar.notify_one();
}

void CondVarClass::broadcast()
{
    m_CondVar.notify_all();
}

uint32_t CondVarClass::wait(std::chrono::steady_clock::duration Timeout)
{
    std::unique_lock<std::mutex> Lock(m_Mutex);
    if(std::chrono::steady_clock::duration::zero() == Timeout)
    {
    	m_CondVar.wait(Lock, [this](){ return 0U != m_Predicates; });
    	return m_Predicates;
    }
    else
    {
    	bool Result = m_CondVar.wait_for(Lock, Timeout, [this](){ return 0U != m_Predicates; });
		if(Result)
			return m_Predicates;
    }
    return 0;
}

SpecificCondition::SpecificCondition(CondVarClass& Cond, CondVarClass::PredicateIdType Index)
        : m_CondVarCl(Cond), m_Index(Index)
{
    if(Cond.getPredNum() <= Index)
    {
        THREAD_EXCEPT_THROW("Wrong predicate index");
    }
}
}
