/*
 * ZeroCopyBuffer.cpp
 *
 *  Created on: Mar 19, 2017
 *      Author: Igor Bender
 */

#include <iostream>
#include "ZeroCopyBuffer.h"
#include <thread>
#include <cstring>

using namespace std;

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
//                CharBufferElement e(0, BuffersPool::ResourcePtrType(nullptr));
                CharBufferElement e = m_pCons->m_Queue.front();
                m_pCons->m_Queue.pop();
                std::cout << e.EventType << " " << *(e.m_pBuff->m_pPtr) << std::endl;
            }
        }
    }
}

void Consumer::notify(const CharBufferElement& e)
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
            CharBufferElement e(0, pBuff);
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
                CharBufferElement e(0, pBuff);
                e.EventType = Count++;
                strcpy(*(e.m_pBuff->m_pPtr), "Hello2");
                m_pProd->m_pCons->notify(e);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

