#ifdef __linux__
#include <inttypes.h>
#include <cstdlib>
#endif
#include <iostream>
#include <PThreadExtended.h>
#include <PRWLockClass.h>
#include <PBarrierClass.h>
#include <CondVarCountedClass.h>
#include <SemaphoreClass.h>

using namespace std;


void* helloRoutine(void* pContext);
void* parseMessage(void* pContext);
void* receiveMessage(void* pContext);
void cleanCount(void* pContext);
void* oneSecond(void* pContext);
void stopNotif(void* p);
void* endless(void* p);
void* producerRoutine(void*);
void* consumerRoutine(void*);
void* semTestProducerRoutine(void*);
void* semTestConsumerRoutine(void*);

// Globals
PThreadClass* pReceiver;
PThreadClass* pParser;
PBarrierClass Barrier(2);
int Count = 0;
double d;
CondVarCountedClass TestCondVar;
PThreadExtended* pProducer;
PThreadExtended* pConsumer;
SemaphoreClass* pSemaphore;


int main(int argc, char** argv)
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
    PThreadClassLib::VersionTriple Version;
    PThreadClassLib::getVersion(Version);
#ifdef __linux__
    sched_param Param;
    Param.__sched_priority = 10;
    int SchedRes =  sched_setscheduler(0, SCHED_FIFO, &Param);
    if(SchedRes)
    {
        cout << "Wrong sched_setscheduler" << endl;
    }
#endif
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
        Runnable Run(reinterpret_cast < PThreadRoutineType > (helloRoutine));
#ifdef __SunOS
        PThreadClass Thread(Run, 1000, true, true, true); // 1 second delay, cyclic, attached, bound to LWP
#else
        PThreadClass Thread(Run, 1000, true, true); // 1 second delay, cyclic, attached
#endif
//        bool r = Thread.setPolicy(SCHED_FIFO);
//        if(!r)
//        {
//        	cout << "Can't set policy." << endl;
//        }
        Thread.run();   // The thread is created ans waiting for start
//        int p = Thread.getPolicy();
//        if(SCHED_FIFO != p)
//        {
//        	cout << "Wrong policy." << endl;
//        }
        cout << "Print 4 times from cyclic thread" << endl;
        Thread.start(); // Start the thread, see "Hello World" of first cycle
#ifdef WIN32
        Sleep(3100);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
        timespec Delay;
        Delay.tv_sec = 3;
        Delay.tv_nsec = 40 * 1000 * 1000;
        nanosleep(&Delay, nullptr);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
        Thread.stop();  // Stop the thread, the thread is turned to be one shot
        cout << "---------------------------------------" << endl << flush;
        cout << "Print one more time from one-shot thread" << endl;
        Thread.start(); // Print "Hello World" more one time (5 times)
#ifdef WIN32
        Sleep(10);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
        Delay.tv_sec = 0;
        Delay.tv_nsec = 10 * 1000 * 1000;
        nanosleep(&Delay, nullptr);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
        cout << "---------------------------------------" << endl << flush;
        cout << "Print 4 times from cyclic thread" << endl;
        Thread.setCyclic(true);
        Thread.start();
#ifdef WIN32
        Sleep(3100);    // Four time more of "Hello World" (9 times)
#else
        Delay.tv_sec = 3;
        Delay.tv_nsec = 20 * 1000 * 1000;
        nanosleep(&Delay, nullptr);     // Four time more of "Hello World" (9 times)
#endif
        Thread.stop();  // Stop the thread
        cout << "---------------------------------------" << endl << flush;
        Thread.exit();
        Thread.join();

        cout << "Reuse thread" << endl;
        Thread.run();
        Thread.setCyclic(true);
        cout << "Print 4 times from cyclic thread" << endl;
        Thread.start();
#ifdef WIN32
        Sleep(3100);    // Four time more of "Hello World" (9 times)
#else
        Delay.tv_sec = 3;
        Delay.tv_nsec = 20 * 1000 * 1000;
        nanosleep(&Delay, nullptr);     // Four time more of "Hello World" (9 times)
#endif
        Thread.stop();  // Stop the thread
        cout << "---------------------------------------" << endl << flush;
        Thread.exit();
        Thread.join();
        cout << "End of \"Hello World\" test" << endl << flush;

#ifndef _MSC_VER
        cout << "---------------------------------------" << endl << flush;
        cout << "Thread cancellation test" << endl;
        Runnable CancelRun(PThreadRoutineType(endless), PThreadCleanUpRoutineType(stopNotif), nullptr, nullptr, true);
        PThreadClass CancelTest(CancelRun, PTHREAD_INFINITE, true);
        CancelTest.run();
        CancelTest.start();
#ifdef _MSC_VER
        Sleep(3100);
#else
        Delay.tv_sec = 3;
        Delay.tv_nsec = 20 * 1000 * 1000;
        nanosleep(&Delay, nullptr);
#endif
//        pthread_cancel(CancelTest.getThreadId());
        CancelTest.cancel();
        CancelTest.join();
        cout << "End of thread cancellation test" << endl;
#endif
        cout << "---------------------------------------" << endl << flush;

        Runnable MessagesRun(reinterpret_cast < PThreadRoutineType > (parseMessage));
        Runnable ReceiverRun(reinterpret_cast < PThreadRoutineType > (receiveMessage),
            reinterpret_cast < PThreadCleanUpRoutineType > (cleanCount));

        // Count = 0;
        pReceiver = nullptr;
        pParser = nullptr;
        pParser = new PThreadClass(MessagesRun, 0, false, true);
        cout << "Parser thread is running..." << endl;
        pReceiver = new PThreadClass(ReceiverRun, 1000, true, true);
        cout << "Receiver thread is running..." << endl;

        if(!pParser->setPolicy(SCHED_FIFO))
        {
            cout << "Scheduling policy could not be set" << endl << flush;
        }
        if(!pParser->setPriority(30))
        {
            cout << "Scheduling priority could not be set" << endl << flush;
        }
        if(!pReceiver->setPolicy(SCHED_FIFO))
        {
            cout << "Scheduling policy could not be set" << endl << flush;
        }
        if(!pReceiver->setPriority(20))
        {
            cout << "Scheduling priority could not be set" << endl << flush;
        }
        pParser->run();
        pReceiver->run();
        cout << "Priority of receiver thread is " << pReceiver->getPriority() << endl;
        cout << "Priority of parser thread is " << pParser->getPriority() << endl;

        pReceiver->start();

        pReceiver->join();
        pParser->exit();
        pParser->join();
        cout << "End of \"Receiver-Parser\" test" << endl << flush;
        delete pReceiver;
        pReceiver = nullptr;
        delete pParser;
        pParser = nullptr;
        cout << "---------------------------------------" << endl << flush;


        cout << "Test of ExtendedThread" << endl;
        PThreadExtended ExtendefTrhread(Runnable(PThreadRoutineType(oneSecond)), 0, true);
        ExtendefTrhread.registerStopNotificationFunc(PThreadNotificationFuncType(stopNotif), reinterpret_cast<void*>(10));
        ExtendefTrhread.run();
        ExtendefTrhread.nameThread("Ttt");
        ExtendefTrhread.start();
#ifdef WIN32
        Sleep(3000);
#else
        sleep(3);
#endif    
        ExtendefTrhread.stop();
#ifdef WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
		ExtendefTrhread.exit();
		ExtendefTrhread.join();
        cout << "---------------------------------------" << endl << flush;

        Count = 0;
        cout << "Test of counted condition variable." << endl;
        cout << "10 prints by 2 lines at ones." << endl;
        pProducer = new PThreadExtended(Runnable(producerRoutine), 1000, true);
        pConsumer = new PThreadExtended(Runnable(consumerRoutine), PTHREAD_INFINITE, true);
        pProducer->run();
        pConsumer->run();
        pProducer->nameThread("Producer");
        pConsumer->nameThread("Consumer");
        pConsumer->start();
        pProducer->start();
        pProducer->join();
        pConsumer->stop();
        pConsumer->exit();
        pConsumer->join();
        delete pConsumer;
        delete pProducer;

        cout << "---------------------------------------" << endl << flush;
        cout << "Semaphore test" << endl;
        pSemaphore = new SemaphoreClass;
        THREAD_TRY
        {
            pSemaphore->post();
            cout << "Using of un-initialized semaphore." << endl;
            return 1;
        }
        THREAD_EXCEPT_CATCH_BEGIN_NOREP
        THREAD_EXCEPT_CATCH_END;
        pSemaphore->init();
        THREAD_TRY
        {
            pSemaphore->init();
            cout << "Double-init of semaphore." << endl;
            return 1;
        }
        THREAD_EXCEPT_CATCH_BEGIN_NOREP
        THREAD_EXCEPT_CATCH_END;

        Count = 0;
        pProducer = new PThreadExtended(Runnable(semTestProducerRoutine), 1000, true);
        pConsumer = new PThreadExtended(Runnable(semTestConsumerRoutine), PTHREAD_INFINITE, true);
        pProducer->run();
        pConsumer->run();
        pProducer->nameThread("Producer");
        pConsumer->nameThread("Consumer");
        pConsumer->start();
        pProducer->start();
        pProducer->join();
        pConsumer->stop();
        pConsumer->exit();
        pConsumer->join();
        delete pConsumer;
        delete pProducer;
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END;
#else // _WITHOUT_THREAD_EXCEPTIONS
    Runnable Run(reinterpret_cast < PThreadRoutineType > (helloRoutine));
    PThreadClass Thread(Run, 1000, true); // 1 second delay, cyclic, not attached
    if(!Thread.isOk())
    {
        cout << "A thread object couldn't be created" << endl << flush;
        exit(1);
    }
    if(Thread.run() < 0)   // The thread is created ans waiting for start
    {
        cout << "A thread object couldn't be ran" << endl << flush;
        exit(1);
    }
    cout << "Print 4 times from cyclic thread" << endl;
    Thread.start(); // Start the thread, see "Hello World" of first cycle
#ifdef _MSC_VER
    Sleep(3000);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
    timespec Delay;
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, nullptr);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
    Thread.stop();  // Stop the thread, the thread is turned to be one shot
    cout << "---------------------------------------" << endl << flush;
    cout << "Print one more time from one-shot thread" << endl;
    Thread.start(); // Print "Hello World" more one time (5 times)
#ifdef _MSC_VER
    Sleep(10);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
    Delay.tv_sec = 0;
    Delay.tv_nsec = 10 * 1000 * 1000;
    nanosleep(&Delay, nullptr);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
    cout << "---------------------------------------" << endl << flush;
    cout << "Print 4 times from cyclic thread" << endl;
    Thread.setCyclic(true);
    Thread.start();
#ifdef _MSC_VER
    Sleep(3000);    // Four time more of "Hello World" (9 times)
#else
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, nullptr);     // Four time more of "Hello World" (9 times)
#endif
    Thread.stop();  // Stop the thread
    cout << "---------------------------------------" << endl << flush;
    
    Thread.exit();
    Thread.join();
    cout << "Reuse thread" << endl;
    cout << "Print 4 times from cyclic thread" << endl;
    Thread.run();
    Thread.setCyclic(true);
    Thread.start();
#ifdef _MSC_VER
    Sleep(3100);    // Four time more of "Hello World" (9 times)
#else
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, nullptr);     // Four time more of "Hello World" (9 times)
#endif
    Thread.stop();  // Stop the thread
    cout << "---------------------------------------" << endl << flush;
    Thread.exit();
    Thread.join();
    cout << "End of \"Hello World\" test" << endl << flush;

    cout << "---------------------------------------" << endl << flush;
    cout << "Thread cancellation test" << endl;
    Runnable CancelRun(PThreadRoutineType(endless), PThreadCleanUpRoutineType(stopNotif), nullptr, nullptr, true);
    PThreadClass CancelTest(CancelRun, PTHREAD_INFINITE, true);
    CancelTest.run();
    CancelTest.start();
#ifdef _MSC_VER
    Sleep(3100);
#else
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, nullptr);
#endif
//        pthread_cancel(CancelTest.getThreadId());
    CancelTest.cancel();
    CancelTest.join();
    cout << "End of thread cancellation test" << endl;
    cout << "---------------------------------------" << endl << flush;

    Runnable MessagesRun(reinterpret_cast < PThreadRoutineType > (parseMessage));
    Runnable ReceiverRun(reinterpret_cast < PThreadRoutineType > (receiveMessage),
            reinterpret_cast < PThreadCleanUpRoutineType > (cleanCount));

    pParser = new PThreadClass(MessagesRun, 0, false, true);
    if(!pParser->isOk())
    {
        cout << "A thread object couldn't be created" << endl << flush;
        delete pParser;
        exit(1);
    }
    pReceiver = new PThreadClass(ReceiverRun, 1000, true, true);
    if(!pReceiver->isOk())
    {
        cout << "A thread object couldn't be created" << endl << flush;
        delete pParser;
        delete pReceiver;
        exit(1);
    }

    if(!pParser->setPolicy(SCHED_FIFO))
    {
        cout << "Scheduling policy could not be set" << endl << flush;
    }
    if(!pParser->setPriority(30))
    {
        cout << "Scheduling priority could not be set" << endl << flush;
    }
    if(!pReceiver->setPolicy(SCHED_FIFO))
    {
        cout << "Scheduling policy could not be set" << endl << flush;
    }
    if(!pReceiver->setPriority(20))
    {
        cout << "Scheduling priority could not be set" << endl << flush;
    }
    pParser->run();
    pReceiver->run();
    cout << "Priority of receiver thread is " << pReceiver->getPriority() << endl;
    cout << "Priority of parser thread is " << pParser->getPriority() << endl;
    pReceiver->start();

    pReceiver->join();
    pParser->exit();                                       
    pParser->join();
    cout << "End of \"Receiver-Parser\" test" << endl << flush;
    delete pReceiver;
    pReceiver = nullptr;
    delete pParser;
    pParser = nullptr;

	cout << "Test of ExtendedThread" << endl;
	PThreadExtended ExtendefTrhread(Runnable(PThreadRoutineType(oneSecond)), 0, true);
	ExtendefTrhread.registerStopNotificationFunc(PThreadNotificationFuncType(stopNotif), reinterpret_cast<void*>(10));
	ExtendefTrhread.run();
	ExtendefTrhread.nameThread("Ttt");
	ExtendefTrhread.start();
#ifdef _MSC_VER
	Sleep(3000);
#else
	sleep(3);
#endif    
	ExtendefTrhread.stop();
#ifdef _MSC_VER
	Sleep(1000);
#else
	sleep(1);
#endif
	ExtendefTrhread.exit();
	ExtendefTrhread.join();
    cout << "---------------------------------------" << endl << flush;

    Count = 0;
    cout << "Test of counted condition variable." << endl;
    cout << "10 prints by 2 lines at ones." << endl;
    pProducer = new PThreadExtended(Runnable(producerRoutine), 1000, true);
    pConsumer = new PThreadExtended(Runnable(consumerRoutine), PTHREAD_INFINITE, true);
    pProducer->run();
    pConsumer->run();
    pProducer->nameThread("Producer");
    pConsumer->nameThread("Consumer");
    pConsumer->start();
    pProducer->start();
    pProducer->join();
    pConsumer->stop();
    pConsumer->exit();
    pConsumer->join();
    delete pConsumer;
    delete pProducer;

    cout << "---------------------------------------" << endl << flush;
    cout << "Semaphore test" << endl;
    pSemaphore = new SemaphoreClass;
    int32_t SemResult = pSemaphore->post();
    if(0 == SemResult)
    {
        cout << "Using of un-initialized semaphore." << endl;
        return 1;
    }
    pSemaphore->init();
    if(pSemaphore->init())
    {
        cout << "Double-init of semaphore." << endl;
        return 1;
    }

    Count = 0;
    pProducer = new PThreadExtended(Runnable(semTestProducerRoutine), 1000, true);
    pConsumer = new PThreadExtended(Runnable(semTestConsumerRoutine), PTHREAD_INFINITE, true);
    pProducer->run();
    pConsumer->run();
    pProducer->nameThread("Producer");
    pConsumer->nameThread("Consumer");
    pConsumer->start();
    pProducer->start();
    pProducer->join();
    pConsumer->stop();
    pConsumer->exit();
    pConsumer->join();
    delete pConsumer;
    delete pProducer;

#endif // _WITHOUT_THREAD_EXCEPTIONS
    cout << "End of test programm" << endl << flush;
    return 0;
}

#ifdef __clang__
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
void* helloRoutine(void* pContext)
{
    cout << "Hello World" << endl << flush;
    return nullptr;
}

void* parseMessage(void* pContext)
{
	// Assume that pContext points to message buffer
	// Parse message here
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
#endif
    cout << "Message is parsed successfully" << endl << flush;
    Barrier.wait();
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END
#endif
    return nullptr;
}

void* receiveMessage(void* pContext)
{
	// Receve messages and signal parser
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
#endif
    if(Count >= 10)
    {
        pReceiver->exit();
        return nullptr;
    }
    pParser->start();
    Barrier.wait();
    ++Count;
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END
#endif
    return nullptr;
}

void cleanCount(void* pContext)
{
    Count = 0;
    cout << "Count = 0" << endl << flush;
}

void* oneSecond(void* pContext)
{
#ifdef WIN32
    Sleep(1000);
#else
    sleep(1);
#endif
    cout << "----" << endl;
    return nullptr;
}

void stopNotif(void* p)
{
    cout << "The thread is stopped with code " << reinterpret_cast <uint64_t> (p) <<  endl;
}

void* endless(void* p)
{
    int Dummy;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &Dummy);
    while(1)
    {
        double m = 2.57;
        double k = 3.54;
        d = m * k;
    }
    return nullptr;
}

void* producerRoutine(void* p)
{
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
#endif
    if(Count >= 10)
    {
        pProducer->exit();
        return nullptr;
    }
    TestCondVar.lockMutex();
    TestCondVar.incrementCouner();
    TestCondVar.signal();
    TestCondVar.unlockMutex();
    ++Count;
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END
#endif
    return nullptr;
}

void* consumerRoutine(void* p)
{
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
#endif
    int32_t ConditionVariableCounter;
    TestCondVar.lockMutex();
    ConditionVariableCounter = TestCondVar.getConter();
    if(!ConditionVariableCounter)
    {
        int32_t WaitResult = TestCondVar.wait(1000);
        if(ETIMEDOUT == WaitResult)
        {
            TestCondVar.unlockMutex();
            return nullptr;
        }
        if(0 < WaitResult)
        {
            // some error
#ifndef _MSC_VER
            sleep(1);
#else
            Sleep(1000);
#endif
            TestCondVar.unlockMutex();
        }
        TestCondVar.decrementCouner();
        TestCondVar.unlockMutex();
        cout << "Triggered by producer " << endl;
#ifndef _MSC_VER
        sleep(2);
#else
        Sleep(1900);
#endif
    }
    else
    {
        TestCondVar.decrementCouner();
        TestCondVar.unlockMutex();
        cout << "Triggered by producer " << endl;
    }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END
#endif
    return nullptr;
}

void* semTestProducerRoutine(void*)
{
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
#endif
    if(Count >= 10)
    {
        pProducer->exit();
        return nullptr;
    }
    pSemaphore->post();
    ++Count;
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END
#endif
    return nullptr;
}

void* semTestConsumerRoutine(void*)
{
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    THREAD_TRY
    {
#endif
    if(0 < pSemaphore->getvalue())
    {
        pSemaphore->wait();
        cout << "Triggered by producer " << endl;
        return nullptr;
    }
    else
    {
        int32_t WaitResult = pSemaphore->wait(1000);
        {
            if(ETIMEDOUT == WaitResult)
                return nullptr;
        }
        cout << "Triggered by producer " << endl;
#ifndef _MSC_VER
        sleep(2);
#else
        Sleep(1900);
#endif
    }
#ifndef _WITHOUT_THREAD_EXCEPTIONS
    }
    THREAD_EXCEPT_CATCH_BEGIN(cout)
    THREAD_EXCEPT_CATCH_END
#endif
    return nullptr;
}
