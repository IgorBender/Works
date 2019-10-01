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
    TestClass()
    {
        cout << "TestClass instance is created." << endl;
    }
    ~TestClass()
    {
        cout << "TestClass instance is destroyed." << endl;
    }
    TestClass& operator=(TestClass& other)
    {
        m_Int = other.m_Int;
        return *this;
    }

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

// Wrapper of TestClass providing move semantic.
class MoveTestClass
{
public:
    // Default constructor.
    MoveTestClass() { m_pTestClass = new TestClass; }
    // Copy-constructor.
    MoveTestClass(const MoveTestClass& other) {
        m_pTestClass = new TestClass; // Create new internal resource.
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

int main(/*int argc, char *argv[]*/)
{
    cout << "Create MoveTestClass instance t1, includes TestClass instance." << endl;
    MoveTestClass t1;
    t1.setInt(3);
    cout << "Create MoveTestClass instance t2, includes full copy of t1 internal TestClass instance (via copy-constructor)." << endl;
    MoveTestClass t2(t1); // t2 is copy of t1.
    cout << "Create MoveTestClass instance t3, includes instance of t1 (via move-constructor)." << endl;
    MoveTestClass t3(move(t1)); // t3 contains resource of t1. t1 don't contain resource.
    cout << "Create MoveTestClass instance t4, includes own TestClass instance." << endl;
    MoveTestClass t4;
    cout << "Move TestClass instance of t2 into t4, TestClass instance of t4 will be destroyed." << endl;
    t4 = move(t2); // t4 free its internal resource and gets t2 internal resource, t2 don't contain resource.
    cout << "Destroy t3 and t4 internal instances." << endl;
    return 0;
}
