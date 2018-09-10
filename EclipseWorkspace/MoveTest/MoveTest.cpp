/*
 *  MoveTest.cpp
 *
 *  Created on: 2017-04-16
 *      Author: Igor Bender
 *
 */
#include <iostream>

using namespace std;

// For test purposes consider TestClass has expensive constructor and some attribute hard to copy.
class TestClass
{
public:
    TestClass() {}
    ~TestClass() {}

    void setInt(uint32_t Int)
    {
        m_Int = Int;
    }
    uint32_t getInt()
    {
        return m_Int;
    }

protected:
    uint32_t m_Int = 0;
};

class MoveTestClass
{
public:
    // Default constructor.
    MoveTestClass() { m_pTestClass = new TestClass; }
    // Copy-constructor.
    MoveTestClass(const MoveTestClass& other) {
        m_pTestClass = new TestClass; // Create new intwrnal resource.
        *m_pTestClass = *(other.m_pTestClass); // Copy internal resource.
    }
    // Move-constructor.
    MoveTestClass(MoveTestClass&& other) {
        m_pTestClass = other.m_pTestClass; // Copy internal resource pointer.
        other.m_pTestClass = nullptr; // Zero internal resource pointer of other.
    }
    // Destructor.
    ~MoveTestClass() {
        delete m_pTestClass;
        m_pTestClass = nullptr;
    }
    MoveTestClass& operator=(MoveTestClass&& other) {
        delete m_pTestClass; // Free internal resource.
        m_pTestClass = other.m_pTestClass; // Copy internal resource pointer.
        other.m_pTestClass = nullptr; // Zero internal resource pointer of other.
        return *this;
    }

    void setInt(uint32_t Int)
    {
        if(m_pTestClass)
            m_pTestClass->setInt(Int);
    }
    uint32_t getInt()
    {
        if(m_pTestClass)
            return m_pTestClass->getInt();
        return 0;
    }

protected:
    TestClass* m_pTestClass = nullptr;
};

int main(int argc, char *argv[])
{
    MoveTestClass t1;
    t1.setInt(3);
    MoveTestClass t2(t1); // t2 is copy of t1.
    MoveTestClass t3(move(t1)); // t3 contains resource of t1. t1 don't contain resource.
    MoveTestClass t4;
    t4 = move(t2); // t4 free its internal resource and gets t2 internal resource, t2 don't contain resource.
    return 0;
}
