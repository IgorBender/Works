/*
 * ThreadExample.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: igor
 */

#include <future>
#include "ThreadExample.h"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "This machine has " << thread::hardware_concurrency() << " processors." << endl;

    ////////////////////////////////////////////////////////////////////////////////
    /// Explore promises and futures.
    ////////////////////////////////////////////////////////////////////////////////
    promise<int> Promise1;
    future<int> Future1 = Promise1.get_future();
    // Run created on-the-fly thread with lambda function as thread routine.
    thread([&Promise1]() // Intercept Promise1 variable.
            { this_thread::sleep_for(chrono::milliseconds{100}); // Sleep for 100 milliseconds.
            Promise1.set_value(11); }  // Set promise value and exit.
          ).detach(); // Run detached.
    // Wait for result.
    chrono::steady_clock::time_point Before = chrono::steady_clock::now(); // Start time measurement
    Future1.wait(); // Wait for thread to set promise.
    chrono::steady_clock::time_point After = chrono::steady_clock::now(); // Get end time point.
    cout << "Future1 = " << Future1.get() << " after "
            << chrono::duration_cast<chrono::milliseconds>(After - Before).count() // Compute duration.
            << " milliseconds." << endl;

    Promise1 = promise<int>(); // Reset promise.
    Future1 = Promise1.get_future();
    // Another form of running detached thread.
    thread([&Promise1](){this_thread::sleep_for(chrono::milliseconds{30}); Promise1.set_value(12);}).detach();
    Before = chrono::steady_clock::now();
    Future1.wait();
    After = chrono::steady_clock::now();
    cout << "Future1 = " << Future1.get() << " after " << chrono::duration_cast<chrono::milliseconds>(After - Before).count() << " milliseconds." << endl;

    // Detached thread as asynchronous job.
    future<int>Future2 = async(launch::async, [](){ return 8; });
    cout << "Future2 = " << Future2.get() << endl;

    packaged_task<int(int, int)> PackagedTask([](int left, int right)->int { return left + right;});
    auto ResultFuture = PackagedTask.get_future();
    // Invoke in automatically created detached thread.
    PackagedTask(3, 5);
    cout << "Result of packaged task is " << ResultFuture.get() << endl;
    PackagedTask.reset();
    ResultFuture = PackagedTask.get_future();
    // Invoke in particular attached thread.
    thread PackThread(move(PackagedTask), 3, 7);
    cout << "Result of packaged task is " << ResultFuture.get() << endl;
    PackThread.join();


    ////////////////////////////////////////////////////////////////////////////////
    /// Explore more traditional approach.
    ////////////////////////////////////////////////////////////////////////////////

    // Test thread with simple thread routine.
    thread Simple(regularThreadRoutine);
    Simple.join();

    // Threads with POSIX style thread routines. What is wrong? Not thread-safe code.
    uint32_t Param = 1;
    thread Simple1(posixThreadRoutine1, &Param);
    thread Simple2(posixThreadRoutine2, &Param, 5);
    Simple1.join();
    Simple2.join();

    // Same with thread-safe code.
    Data d;
    d.Param = 3;
    thread Simple1Safe(posixThreadRoutine1Safe, &d);
    thread Simple2Safe(posixThreadRoutine2Safe, &d);
    Simple1Safe.join();
    Simple2Safe.join();

    // Thread with functor as thread routine.
    thread FunctorThread(ThreadFunctor("Functor test"));
    FunctorThread.join();

    // ping - pong test.
    Ping Ping;
    Pong Pong(&Ping);
    cout << endl << "Wait(ms)  Oper.   Oper.\t    Wait(ms)\tCount" << endl << flush;
    Ping.start(&Pong);
    thread PongThread(Pong);
    thread PingThread(Ping);
    PingThread.join();
    PongThread.join();

    cout << "End of test." << endl;
    return 0;
}

// Simple thread routine.
void regularThreadRoutine()
{
    cout << "C++ thread with simple thread routine run." << endl;
}

// POSIX style thread routine.
void* posixThreadRoutine1(uint32_t* i)
{
    this_thread::sleep_for(chrono::milliseconds{1});
    cout << "C++ thread Simple1 with input parameter " << ++(*i) << endl;
//    cout << "C++ thread Simple1 with input parameter " << ++(*(static_cast<uint32_t*>(p))) << endl;
    return nullptr;
}

// POSIX style thread routine.
void* posixThreadRoutine2(uint32_t* i, uint32_t n)
{
//    cout << "C++ thread Simple2 with input parameter " << ++(*(static_cast<uint32_t*>(p))) << endl;
    *i += n;
    cout << "C++ thread Simple2 with input parameter " << ++(*i) << endl;
    return nullptr;
}

// POSIX style thread routine.
void* posixThreadRoutine1Safe(void* p)
{
    this_thread::sleep_for(chrono::milliseconds{1});
    lock_guard<mutex>(reinterpret_cast<Data*>(p)->Mutex);
    cout << "C++ thread Simple1Safe with input parameter " << ++(static_cast<Data*>(p)->Param) << endl;
    return nullptr;
}
// POSIX style thread routine.
void* posixThreadRoutine2Safe(void* p)
{
    lock_guard<mutex>(reinterpret_cast<Data*>(p)->Mutex);
    cout << "C++ thread Simple2Safe with input parameter " << ++(static_cast<Data*>(p)->Param) << endl;
    return nullptr;
}


