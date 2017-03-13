/*
 * SharedPtrTest.cpp
 *
 *  Created on: Mar 10, 2017
 *      Author: Igor Bender
 */

#include <memory>
#include <iostream>

class PtrWrapper
{
public:
    PtrWrapper()
    {
        m_pPtr = new char[16];
        std::cout << "+++" << std::endl;
    }
    virtual ~PtrWrapper()
    {
        std::cout << "---" << std::endl;
    }

//private:
    char* m_pPtr = nullptr;
};

class Deleter
{
public:
    void operator()(PtrWrapper* p)
    {
        delete [] p->m_pPtr;
        p->m_pPtr = nullptr;
        std::cout << "///" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    PtrWrapper* pw = new PtrWrapper;
    Deleter Del;
    std::shared_ptr<PtrWrapper> p(pw, Del);
    p.reset();
    delete pw;
    return 0;
}
