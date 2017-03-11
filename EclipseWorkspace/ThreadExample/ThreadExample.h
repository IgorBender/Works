/*
 * ThreadExample.h
 *
 *  Created on: Feb 10, 2017
 *      Author: igor
 */

#ifndef THREADEXAMPLE_H_
#define THREADEXAMPLE_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>

// Simple thread routine.
void regularThreadRoutine();

// POSIX style thread routine.
void* posixThreadRoutine1(uint32_t* i);

// POSIX style thread routine.
void* posixThreadRoutine2(uint32_t* i, uint32_t n);

// POSIX style thread routine.
void* posixThreadRoutine1Safe(void* p);

// POSIX style thread routine.
void* posixThreadRoutine2Safe(void* p);

// Data structure encapsulating actual data with guardian mutex.
struct Data
{
    std::mutex Mutex;
    uint32_t Param;
};

// Functor as thread routine.
class ThreadFunctor
{
public:
    ThreadFunctor() {}
    ThreadFunctor(std::string Name) : m_Name(Name) {}
    void operator()()
    {
        std::cout << "Run " << m_Name << " thread." << std::endl;
    }

protected:
    std::string m_Name = "No name";
};

// Synchronization element.
// Includes condition variable, guardian mutex and boolean predicate.
struct SyncElement
{
    SyncElement()
    {
        std::cout << "Construct SyncElement." << std::endl;
    }
    ~SyncElement()
    {
        // Print to ensure object destruction.
        std::cout << "Destruct SyncElement." << std::endl;
    }
    std::mutex m_Mutex;
    std::condition_variable m_Event;
    bool m_Predicate = false;
};

// Two functors, must be synchronized by number of repetitions (m_Counter = 10).
// Forward declaration.
class Pong;
// Ping:
//      Wait for notification from Pong.
//      Sleep for 500 milliseconds.
//      Notify Pong.
class Ping
{
public:
    Ping() : m_pSync(new SyncElement) {} // SyncElement will be automatically deleted at destruction.
    ~Ping() {}

    void operator()();

    std::shared_ptr<SyncElement> syncElement()
    {
        return m_pSync;
    }
    // Initial Pong notification.
    void start(Pong* pPong);
    void ping()
    {
        std::unique_lock<std::mutex> Lock(m_pSync->m_Mutex);
        m_pSync->m_Predicate = true;
        m_pSync->m_Event.notify_one();
    }

protected:
    std::shared_ptr<SyncElement> m_pSync; // Will be automatically deleted at destruction.
    Pong* m_pPong = nullptr;
    uint32_t m_Counter = 10;
    std::chrono::steady_clock::time_point m_Start;
    std::chrono::steady_clock::time_point m_End;
};

// Pong:
//      Wait for notification from Ping.
//      Sleep for 500 milliseconds.
//      Notify Ping.
class Pong
{
public:
    Pong(Ping* pPing) : m_pSync(new SyncElement), m_pPing(pPing) {} // SyncElement will be automatically deleted at destruction.
    ~Pong() {}

    void operator()()
    {
        while(m_Counter--)
        {
            m_Start = std::chrono::steady_clock::now();
            {
                std::unique_lock<std::mutex> Lock(m_pSync->m_Mutex);
                // lambda with explicitly stated return value type.
                while(!m_pSync->m_Event.wait_for(Lock, std::chrono::milliseconds(3000), [this]()->bool{return m_pSync->m_Predicate;}));
                m_pSync->m_Predicate = false;
            }
            m_End = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(m_End - m_Start).count() << "\t  Ping -" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            m_pPing->ping();
        }
    }

    std::shared_ptr<SyncElement> syncElement()
    {
        return m_pSync;
    }
    void pong()
    {
        std::unique_lock<std::mutex> Lock(m_pSync->m_Mutex);
        m_pSync->m_Predicate = true;
        m_pSync->m_Event.notify_one();
    }

protected:
    std::shared_ptr<SyncElement> m_pSync; // Will be automatically deleted at destruction.
    Ping* m_pPing;
    uint32_t m_Counter = 10;
    std::chrono::steady_clock::time_point m_Start;
    std::chrono::steady_clock::time_point m_End;
};

inline void Ping::operator()()
{
    while(m_Counter--)
    {
        m_Start = std::chrono::steady_clock::now();
        {
            std::unique_lock<std::mutex> Lock(m_pSync->m_Mutex);
            while(!m_pSync->m_Event.wait_for(Lock, std::chrono::milliseconds(3000), [this](){return m_pSync->m_Predicate;}));
            m_pSync->m_Predicate = false;
        }
        m_End = std::chrono::steady_clock::now();
        std::cout << "- Pong\t    " << std::chrono::duration_cast<std::chrono::milliseconds>(m_End - m_Start).count() <<
                "\t\t" << 10 - m_Counter << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds{500});
        m_pPong->pong();
    }
}

inline void Ping::start(Pong* pPong)
{
    m_pPong = pPong;
    m_pPong->pong();
}

#endif /* THREADEXAMPLE_H_ */
