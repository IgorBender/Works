#include <cstdint>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <future>
#include <cstring>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Thread, mutex and conditional variable test
struct SyncElement
{
    SyncElement() : m_Predicate(false)
    {
        cout << "Construct SyncElement." << endl;
    }
    ~SyncElement()
    {
        cout << "Destruct SyncElement." << endl;
    }
    mutex m_Mutex;
    condition_variable m_Event;
    bool m_Predicate;
};

class ThreadTest
{
public:
    ThreadTest() : m_pSync(new SyncElement) {}
    ~ThreadTest() {}

    void operator()()
    {
        {
            unique_lock<mutex> Lock(m_pSync->m_Mutex);
            while(!m_pSync->m_Event.wait_for(Lock,  std::chrono::milliseconds(1000), [this](){return m_pSync->m_Predicate;}));
            m_pSync->m_Predicate = false;
        }
        cout << "===" << endl;
    }
    shared_ptr<SyncElement> m_pSync;
};

/////////////////////////////////////////////////////////////////////////////
// Test classes to explore move semantics (constructor and assign operator)
class TestClass;
class SomeClass
{
private:
    int m;

public:
    friend TestClass;
    SomeClass() : m(0)
    {
        cout << "SomeClass Constructor." << endl;
    }
    ~SomeClass()
    {
        cout << "SomeClass Destructor." << endl;
    }
};

class TestClass
{
private:
    unique_ptr<int> IntPtr;
    unique_ptr<SomeClass> SomePtr;
    int Size;

public:
    friend TestClass operator+(const TestClass& a, const TestClass& b);
    TestClass(int n) : IntPtr(new int[n]), SomePtr(new SomeClass), Size(n)
    {
        cout << "TestClass Constructor." << endl;
    }
    TestClass(const TestClass& t) : IntPtr(new int[t.Size]), SomePtr(new SomeClass), Size(t.Size)
    {
        memcpy(IntPtr.get(), t.IntPtr.get(), Size * sizeof(int));
        SomePtr->m = t.SomePtr->m;
        cout << "TestClass Copy constructor." << endl;
    }
    TestClass(TestClass&& t) : IntPtr(t.IntPtr.get()), SomePtr(t.SomePtr.get()), Size(t.Size)
    {
        t.IntPtr.release();
        t.SomePtr.release();
        t.Size = 0;
        cout << "TestClass Move constructor." << endl;
    }
    TestClass& operator=(TestClass&& t)
    {
        IntPtr.swap(t.IntPtr);
        Size = t.Size;
        t.IntPtr.reset(nullptr);
        t.SomePtr.reset(nullptr);
        cout << "TestClass Move assign operator." << endl;
        return *this;
    }
    ~TestClass()
    {
        cout << "TestClass Destructor." << endl;
    }
};

TestClass operator+(const TestClass& a, const TestClass& b)
{
    TestClass Tmp(a.Size + b.Size);
    memcpy(Tmp.IntPtr.get(), a.IntPtr.get(), a.Size * sizeof(int));
    memcpy(Tmp.IntPtr.get() + a.Size * sizeof(int), b.IntPtr.get(), b.Size * sizeof(int));
    cout << "TestClass + operator." << endl;
    return Tmp;
}

/////////////////////////////////////////////////////////////////////////////
// Variadic template function
// Empty list sentinel function
void my_print() {}

template <typename Start, typename... Tn> void my_print(Start s, Tn... t)
{
    cout << "-- " << s << endl;
    my_print(t...);
}

/////////////////////////////////////////////////////////////////////////////
// Packaged tasks futures and promises
int taskRoutine1(shared_ptr<SyncElement> Sync)
{
    unique_lock<mutex> Lock(Sync->m_Mutex);
    while(!Sync->m_Event.wait_for(Lock,  std::chrono::milliseconds(1000), [Sync](){return Sync->m_Predicate;}));
    Sync->m_Predicate = false;
    return 0;
}

int taskRoutine2(shared_ptr<SyncElement> Sync, unsigned int DelayMs)
{
    this_thread::sleep_for(chrono::milliseconds{DelayMs});
    unique_lock<mutex>(Sync->m_Mutex);
    Sync->m_Predicate = true;
    Sync->m_Event.notify_one();
    return DelayMs;
}

int main(int argc, char** argv)
{
    std::map<std::string, std::vector<int> > map;
    std::vector<int> v = { 1, 2, 3 };
//  v.push_back(1);
//  v.push_back(2);
//  v.push_back(3);
    map["one"] = v;

    // Range-for tests
    for (const auto& kvp : map)
    {
        std::cout << kvp.first << std::endl;

        for (auto v : kvp.second)
        {
            std::cout << v << std::endl;
        }
    }

    int arr[] = {1,2,3,4,5};
    for(int& e : arr)
    {
        e = e * e;
    }
    for(int& e : arr)
    {
        cout << e << " ";
    }
    cout << endl;

    // Index notation
    arr[3] = 4[arr];

    // Thread test
    cout << "Test thread, wait for 3 seconds and signal test thread." << endl;
    ThreadTest Test;
    thread t(move(Test));
    this_thread::sleep_for(chrono::seconds{3});
    unique_lock<mutex>(Test.m_pSync->m_Mutex);
    Test.m_pSync->m_Predicate = true;
    Test.m_pSync->m_Event.notify_one();
    t.join();

    // Move test
    cout << "Test move semantics." << endl;
    TestClass a(3);
    TestClass b(5);
    TestClass c(a + b);
    c = a + b;
    TestClass d(move(c));

    // Variadic test
    cout << "Test variadic function." << endl;
    int m = 0;
    my_print(m, "Hello", "World");

    // Packaged tasks
    cout << "Test packaged tasks and futures." << endl;
    using Task1Type = int(shared_ptr<SyncElement>);
    packaged_task<Task1Type> Task1{taskRoutine1};
    using Task2Type = int(shared_ptr<SyncElement>, unsigned int);
    packaged_task<Task2Type> Task2{taskRoutine2};
    future<int> Task1Res{Task1.get_future()};
    future<int> Task2Res{Task2.get_future()};
    shared_ptr<SyncElement> Sync {new SyncElement};
    thread Thread1{move(Task1), Sync};
    thread Thread2{move(Task2), Sync, 2000};
    int Res1 = Task1Res.get();
    int Res2 = Task2Res.get();
    cout << Res1 + Res2 << endl;
    Thread1.join();
    Thread2.join();
}

