
#include <CircularBuffer.h>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>


struct FindPred : public std::unary_function<bool, int>
{

    FindPred(int n) : m_Threshold(n)
    {
    }

    bool operator()(int i)
    {
        return m_Threshold < i;
    }

    int m_Threshold;
};

int main(int argc, char** argv)
{
    CircularBuffer<int, 16 > a; // = new CyclicBuffer<int, 16>;

//    a.push_back(100);
//    a.push_back(100);
//    std::cout << "size " << a.size() << std::endl;
//    a.pop_front();

    for (int i = 0; i < 34; ++i)
    {
        a.push_back(i);
    }
//    a.pop_front();
//    a.pop_front();
//    a.pop_front();
    std::cout << "size " << a.size() << std::endl;
#if ((_WIN32 && _MSC_VER > 1700) || !_WIN32) // MSVC 2013
    for (auto n : a)
    {
        std::cout << n << std::endl;
    }
#endif

    std::cout << "---- " << std::endl;
    CircularBuffer<int, 16 > ::circular_iterator iter;
    for (iter = a.begin(); iter != a.end(); ++iter)
    {
        std::cout << *iter << std::endl;
    }
    a.at(3);
//     //    return 0;
//     iter--;
    for (size_t j = 0; j < a.size(); ++j)
    {
        std::cout << "---- " << *(--iter) << std::endl;
    }
    CircularBuffer<int, 16 >::reverse_circular_iterator RevIter;
    for(RevIter = a.rbegin(); RevIter != a.rend(); ++RevIter)
    {
        std::cout << *RevIter << std::endl;
    }
    RevIter--;
    for (size_t j = 0; j < a.size(); ++j)
    {
        std::cout << "---- " << *RevIter-- << std::endl;
    }

    a.clear();
    for (int i = 0; i < 8; ++i)
    {
        a.push_back(i);
    }
    a.pop_front();
    std::cout << "size " << a.size() << std::endl;
    RevIter = a.rend();
    RevIter--;
    for (size_t j = 0; j < a.size(); ++j)
    {
        std::cout << "---- " << *RevIter-- << std::endl;
    }
    std::cout << "-- " << a[3] << std::endl;
    CircularBuffer<int, 16 >::circular_iterator FindIter = std::find_if(a.begin(), a.end(), FindPred(5));
    std::cout << "+++ " << *FindIter << std::endl;

    std::vector<int> v;
    std::copy(FindIter, a.end(), std::back_inserter(v));

    std::cout << "Vector size is " << v.size() << std::endl;
    for (std::vector<int>::iterator n = v.begin(); n != v.end(); ++n)
    {
        std::cout << "/// " << *n << std::endl;
    }
    a.pop_front();
    a.pop_front();
    a.pop_front();
    a.pop_front();
    a.pop_front();
    a.pop_front();
    a.pop_front();
    std::cout << "size " << a.size() << std::endl;

    for (int i = 0; i < 8; ++i)
    {
        a.push_back(i);
    }
    std::cout << "size " << a.size() << std::endl;
#if ((_WIN32 && _MSC_VER > 1700) || !_WIN32) // MSVC 2013
    for (auto n : a)
    {
        std::cout << n << std::endl;
    }
#endif

    return 0;
}

