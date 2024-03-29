#ifdef __linux__
#include <inttypes.h>
#include <cstdlib>
#endif
#include <iostream>

#include <ThreadClass.h>
#include <CondVarCountedClass.h>

int Count = 0;
ThreadClassLib::ThreadClass* pReceiver = nullptr;
ThreadClassLib::ThreadClass* pParser = nullptr;
ThreadClassLib::ThreadClass* pProducer;
ThreadClassLib::ThreadClass* pConsumer;
ThreadClassLib::CondVarCountedClass TestCondVar;

int main()
{
#ifdef __linux__
    sched_param Param;
    Param.__sched_priority = 10;
    int SchedRes =  sched_setscheduler(0, SCHED_FIFO, &Param);
    if(SchedRes)
    {
        std::cout << "Wrong sched_setscheduler" << std::endl;
    }
#endif
    THREAD_TRY
    {
        ThreadClassLib::VersionTriple Version;
        ThreadClassLib::getVersion(Version);
        std::cout << "Version " << Version.Major << "." << Version.Minor << "."
                << Version.SubMinor << std::endl;

        struct HelloRoutine : public ThreadClassLib::ThreadClass::ThreadRoutineType
        {
            virtual ~HelloRoutine() {}
            virtual void operator()()
            {
                std::cout << "Hello World." << std::endl;
            }
        } Test1Routine;

        std::cout << "Test basic thread behavior." << std:: endl;
        ThreadClassLib::ThreadClass Test1(&Test1Routine, true, std::chrono::milliseconds(1000));
        Test1.run();
        std::cout << "Cyclic thread ones in second during 3.1 seconds, runs 4 times." << std::endl << std::flush;;
        std::cout << "Expected results - 4 prints." << std::endl;
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3100));
        Test1.stop();
        std::cout << "---------------------------------------" << std::endl
                << std::flush;
        std::cout << "One shot thread." << std::endl << std::flush;
        std::cout << "Expected results - 1 print." << std::endl;
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "---------------------------------------" << std::endl
                << std::flush;
        Test1.setCyclic(true);
        std::cout << "Cyclic thread ones in second during 3.1 seconds, runs 4 times." << std::endl << std::flush;;
        std::cout << "Expected results - 4 prints." << std::endl;
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3100));
        Test1.stop();
        std::cout << "---------------------------------------" << std::endl
                << std::flush;
        Test1.exit();
        Test1.join();

        std::cout << "Reuse thread." << std::endl;
        std::cout << "All is as above." << std::endl;
        std::cout << "Expected results - 4 prints." << std::endl;
        Test1.run();
        Test1.setCyclic(true);
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3100));
        Test1.stop();
        std::cout << "---------------------------------------" << std::endl
                << std::flush;
        std::cout << "Expected results - 1 print." << std::endl;
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "---------------------------------------" << std::endl
                << std::flush;
        std::cout << "Expected results - 4 prints." << std::endl;
        Test1.setCyclic(true);
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3100));
        Test1.stop();
        std::cout << "---------------------------------------" << std::endl
                << std::flush;
        Test1.exit();
        Test1.join();

        std::cout << "End of \"Hello World\" test" << std::endl << std::endl << std::flush;

        struct RecvMsgRoutine : public ThreadClassLib::ThreadClass::ThreadRoutineType
        {
            virtual ~RecvMsgRoutine() {}
            virtual void operator()()
            {
                if (Count >= 10)
                {
                    pReceiver->exit();
                    return;
                }
                pParser->start();
                ++Count;
            }
        } RecvMsgRoutine;
        struct ParseMsgRoutine : public ThreadClassLib::ThreadClass::ThreadRoutineType
        {
            virtual ~ParseMsgRoutine() {}
            virtual void operator()()
            {
                std::cout << "Message is parsed successfully" << std::endl
                        << std::flush;
            }
        } ParseMsgRoutine;

        std::cout << "Test two threads one for periodic send and another reactive." << std::endl
                << "To set priorities the program should has sufficient permissions." << std::endl
                << "Expected results - 10 prints." << std::endl;
        pReceiver = new ThreadClassLib::ThreadClass(&RecvMsgRoutine, true, std::chrono::milliseconds(1000));
        pParser = new ThreadClassLib::ThreadClass(&ParseMsgRoutine, false);
        pParser->run();
        pReceiver->run();
        std::cout << "Parser priority is " << pParser->getPriority() << std::endl;
        pParser->setPriority(20);
        std::cout << "New Parser priority is " << pParser->getPriority() << std::endl;
        pReceiver->start();
        pParser->start();
        pReceiver->join();
        pParser->stop();
        pParser->exit();
        pParser->join();
        std::cout << "End of \"Receiver-Parser\" test" << std::endl << std::flush;
        delete pReceiver;
        pReceiver = nullptr;
        delete pParser;
        pParser = nullptr;

        std::cout << "---------------------------------------" << std::endl << std::flush;
        std::cout << "Test of stop notification" << std::endl;
        struct TestThreadStop : public ThreadClassLib::ThreadClass::ThreadStopNotificatonType
        {
            TestThreadStop(int Count = 0) : Counter(Count) {}
            virtual ~TestThreadStop() {}
            void setCounter(int c)
            {
                Counter = c;
            }
            virtual void operator()(void* p)
            {
                std::cout << "Thread is stopped" << std::endl;
                if(0 < --Counter)
                {
                    (reinterpret_cast<ThreadClassLib::ThreadClass*>(p))->start();
                }
            }
            int Counter;
        };
        TestThreadStop ThreadStopNofifier(5);
        Test1.run();
        std::cout << "---------------------------------------" << std::endl << std::flush;
        std::cout << "One shot thread, thread stop notification re-triggers the thread for 5 times." << std::endl
                << "Expected results - 5 prints" << std::endl;
        Test1.setCyclic(false);
        Test1.nameThread("Test thread");
        Test1.registerStopNotificationFunc(&ThreadStopNofifier, &Test1);
        Test1.start();
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::cout << "---------------------------------------" << std::endl << std::flush;
        std::cout << "Another stop notifier test. 4 + 2 prints expected." << std::endl;
        ThreadStopNofifier.setCounter(2);
        Test1.setCyclic(true);
        Test1.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(3100));
        Test1.stop();
        std::this_thread::sleep_for(std::chrono::milliseconds(1100));
        Test1.stop();
        Test1.exit();
        Test1.join();

        Count = 0;
        std::cout << "Test of counted condition variable." << std::endl;
        std::cout << "10 prints by 2 lines at ones." << std::endl;
        struct ProducerRoutine : public ThreadClassLib::ThreadClass::ThreadRoutineType
        {
            virtual ~ProducerRoutine() {}
            virtual void operator()()
            {
                THREAD_TRY
                {
                    if(Count >= 10)
                    {
                        pProducer->exit();
                        return;
                    }
                    {
                        std::unique_lock<std::mutex> Lock(TestCondVar.getMutex());
                        TestCondVar.signal();
                    }
                    ++Count;
                }
                THREAD_EXCEPT_CATCH_BEGIN(std::cout)
                THREAD_EXCEPT_CATCH_END
            }
        } ProducerRoutine;

        struct  ConsumerRoutine : public ThreadClassLib::ThreadClass::ThreadRoutineType
        {
            virtual ~ConsumerRoutine() {}
            virtual void operator()()
            {
                THREAD_TRY
                {
                    int32_t ConditionVariableCounter;
                    ConditionVariableCounter = TestCondVar.getConter();
                    if(0 != ConditionVariableCounter)
                    {
                        bool WaitResult = TestCondVar.wait(std::chrono::steady_clock::duration(std::chrono::milliseconds{1000}));
                        if(!WaitResult)
                            return;
                        std::cout << "Triggered by producer " << std::endl;
                    }
                    else
                        std::this_thread::sleep_for(std::chrono::seconds{2});
                }
                THREAD_EXCEPT_CATCH_BEGIN(std::cout)
                THREAD_EXCEPT_CATCH_END
            }
        } ConsumerRoutine;


        pProducer = new ThreadClassLib::ThreadClass(&ProducerRoutine, true, std::chrono::milliseconds(1000));
        pConsumer = new ThreadClassLib::ThreadClass(&ConsumerRoutine, true);
        pProducer->run();
        pConsumer->run();
        pConsumer->start();
        pProducer->start();
        pProducer->join();
        pConsumer->stop();
        pConsumer->exit();
        pConsumer->join();
        delete pConsumer;
        delete pProducer;
    }
    THREAD_EXCEPT_CATCH_BEGIN(std::cout)
    THREAD_EXCEPT_CATCH_END
    catch (std::system_error& exc)
    {
        std::cout << exc.what() << std::endl;
    }
    return 0;
} // end of ThreadClassLib namespace
