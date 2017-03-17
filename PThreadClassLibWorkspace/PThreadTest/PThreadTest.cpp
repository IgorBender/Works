#ifdef __linux__
#include <inttypes.h>
#include <cstdlib>
#endif
#include <iostream>
#ifdef WIN32
#include <stdafx.h>
#endif
#include <PThreadExtended.h>
#include <PRWLockClass.h>
#include <PBarrierClass.h>
using namespace std;



void* helloRoutine(void* pContext);
void* parseMessage(void* pContext);
void* receiveMessage(void* pContext);
void cleanCount(void* pContext);
void* oneSecond(void* pContext);
void stopNotif(void* p);
void* endless(void* p);

// Globals
PThreadClass* pReceiver;
PThreadClass* pParser;
PBarrierClass Barrier(2);
int Count = 0;
double d;

int main(int args, char** argv)
{
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
        Thread.start(); // Start the thread, see "Hello World" of first cycle
#ifdef WIN32
        Sleep(3100);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
        timespec Delay;
        Delay.tv_sec = 3;
        Delay.tv_nsec = 40 * 1000 * 1000;
        nanosleep(&Delay, NULL);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
        Thread.stop();  // Stop the thread, the thread is turned to be one shot
        cout << "---------------------------------------" << endl << flush;
        Thread.start(); // Print "Hello World" more one time (5 times)
#ifdef WIN32
        Sleep(10);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
        Delay.tv_sec = 0;
        Delay.tv_nsec = 10 * 1000 * 1000;
        nanosleep(&Delay, NULL);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
        cout << "---------------------------------------" << endl << flush;
        Thread.setCyclic(true);
        Thread.start();
#ifdef WIN32
        Sleep(3100);    // Four time more of "Hello World" (9 times)
#else
        Delay.tv_sec = 3;
        Delay.tv_nsec = 20 * 1000 * 1000;
        nanosleep(&Delay, NULL);     // Four time more of "Hello World" (9 times)
#endif
        Thread.stop();  // Stop the thread
        cout << "---------------------------------------" << endl << flush;
        Thread.exit();
        Thread.join();

        cout << "Reuse thread" << endl;
        Thread.run();
        Thread.setCyclic(true);
        Thread.start();
#ifdef WIN32
        Sleep(3100);    // Four time more of "Hello World" (9 times)
#else
        Delay.tv_sec = 3;
        Delay.tv_nsec = 20 * 1000 * 1000;
        nanosleep(&Delay, NULL);     // Four time more of "Hello World" (9 times)
#endif
        Thread.stop();  // Stop the thread
        cout << "---------------------------------------" << endl << flush;
        Thread.exit();
        Thread.join();
        cout << "End of \"Hello World\" test" << endl << flush;

        cout << "---------------------------------------" << endl << flush;
        cout << "Thread cancellation test" << endl;
        Runnable CancelRun(PThreadRoutineType(endless), PThreadCleanUpRoutineType(stopNotif), NULL, NULL, true);
        PThreadClass CancelTest(CancelRun, PTHREAD_INFINITE, true);
        CancelTest.run();
        CancelTest.start();
#ifdef WIN32
        Sleep(3100);
#else
        Delay.tv_sec = 3;
        Delay.tv_nsec = 20 * 1000 * 1000;
        nanosleep(&Delay, NULL);
#endif
//        pthread_cancel(CancelTest.getThreadId());
        CancelTest.cancel();
        CancelTest.join();
        cout << "End of thread cancellation test" << endl;
        cout << "---------------------------------------" << endl << flush;

        Runnable MessagesRun(reinterpret_cast < PThreadRoutineType > (parseMessage));
        Runnable ReceiverRun(reinterpret_cast < PThreadRoutineType > (receiveMessage),
            reinterpret_cast < PThreadCleanUpRoutineType > (cleanCount));

        // Count = 0;
        pReceiver = NULL;
        pParser = NULL;
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
        pReceiver = NULL;
        delete pParser;
        pParser = NULL;
        cout << "End of prog" << endl; 

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
    Thread.start(); // Start the thread, see "Hello World" of first cycle
#ifdef WIN32
    Sleep(3000);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
    timespec Delay;
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, NULL);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
    Thread.stop();  // Stop the thread, the thread is turned to be one shot
    cout << "---------------------------------------" << endl << flush;
    Thread.start(); // Print "Hello World" more one time (5 times)
#ifdef WIN32
    Sleep(10);    // Wait for 3 seconds and see "Hello World" three times more (4 times)
#else
    Delay.tv_sec = 0;
    Delay.tv_nsec = 10 * 1000 * 1000;
    nanosleep(&Delay, NULL);    // Wait for 3 seconds + 20 mili and see "Hello World" three times more (4 times)
#endif
    cout << "---------------------------------------" << endl << flush;
    Thread.setCyclic(true);
    Thread.start();
#ifdef WIN32
    Sleep(3000);    // Four time more of "Hello World" (9 times)
#else
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, NULL);     // Four time more of "Hello World" (9 times)
#endif
    Thread.stop();  // Stop the thread
    cout << "---------------------------------------" << endl << flush;
    
    Thread.exit();
    Thread.join();
    cout << "Reuse thread" << endl;
    Thread.run();
    Thread.setCyclic(true);
    Thread.start();
#ifdef WIN32
    Sleep(3100);    // Four time more of "Hello World" (9 times)
#else
    Delay.tv_sec = 3;
    Delay.tv_nsec = 20 * 1000 * 1000;
    nanosleep(&Delay, NULL);     // Four time more of "Hello World" (9 times)
#endif
    Thread.stop();  // Stop the thread
    cout << "---------------------------------------" << endl << flush;
    Thread.exit();
    Thread.join();
    cout << "End of \"Hello World\" test" << endl << flush;

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
    pReceiver = NULL;
    delete pParser;
    pParser = NULL;

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
#endif // _WITHOUT_THREAD_EXCEPTIONS
    cout << "End of test programm" << endl << flush;
    return 0;
}


void* helloRoutine(void* pContext)
{
    cout << "Hello World" << endl << flush;
    return NULL;
}

void* parseMessage(void* pContext)
{
	// Assume that pContext points to message buffer
	// Parse message here
    cout << "Message is parsed successfully" << endl << flush;
    Barrier.wait();
    return NULL;
}

void* receiveMessage(void* pContext)
{
	// Receve messages and signal parser
    if(Count > 10)
    {
        pReceiver->exit();
        return NULL;
    }
    pParser->start();
    Barrier.wait();
    ++Count;
    return NULL;
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
    return NULL;
}

void stopNotif(void* p)
{
    cout << "The thread is stopped with code " << reinterpret_cast <long> (p) <<  endl;
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
    return NULL;
}
