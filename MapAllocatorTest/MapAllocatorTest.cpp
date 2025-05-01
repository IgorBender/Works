/*
 *  MapAllocatorTest.cpp
 *
 *  Created on: 2021-11-09
 *  Author: Igor Bender
 *
 *  Explore custom allocator for STL map
 */
#include <map>
#include <set>
#include <list>
#include <iostream>
#include <functional>
#include "Allocator.h"

using namespace std;

int main()
{
    // Test allocator with map
    cout << "Create map with custom allocator for fixed size of 4 map elements." << endl;
    map<const uint32_t, double, std::less<uint32_t>, CustomAllocators::AllocatorFixedSize<pair<const uint32_t, double>, 4>> TestMap;
    try {
        TestMap[3] = 3.3;
        TestMap[2] = 2.2;
        TestMap[1] = 1.1;
        TestMap[8] = 8.8; // Last possible element
        TestMap[5] = 5.5; // Exception will be thrown here, no further lines executed
        TestMap[7] = 7.7;
        TestMap[6] = 6.6;
    }
    catch (std::bad_alloc& bad_alloc)
    {
        cout << "exception - " << bad_alloc.what() << " : allocated memory exhausted..." << endl;
    }
    cout << "Map size is " << TestMap.size() << endl;

    // Test allocator with set
    cout << "Create set with custom allocator for fixed size of 4 elements." << endl;
    set<int32_t, less<int32_t>, CustomAllocators::AllocatorFixedSize<int32_t, 4>> TestSet;
    try {
        TestSet.insert(-3);
        TestSet.insert(0);
        TestSet.insert(-1);
        TestSet.insert(4); // Last possible element
        TestSet.insert(16); // Exception will be thrown here, no further lines executed
        TestSet.insert(2);
    }
    catch (std::bad_alloc& bad_alloc)
    {
        cout << "exception - " << bad_alloc.what() << " : allocated memory exhausted..." << endl;
    }
    cout << "Set size is " << TestSet.size() << endl;
    for(auto& i : TestSet)
    {
        cout << i << " ";
    }
    cout << endl;

    // Test allocator with list
    cout << "Create list with custom allocator for fixed size of 4 list elements." << endl;
    list<int32_t, CustomAllocators::AllocatorFixedSize<_List_node<int32_t>, 4>> TestList;
    try {
        TestList.push_back(-1);
        TestList.push_back(-4);
        TestList.push_back(3);
        TestList.push_back(3); // Last possible element
        TestList.push_back(21); // Exception will be thrown here, no further lines executed
        TestList.push_back(21);
        TestList.push_back(21);
    }
    catch (std::bad_alloc& bad_alloc)
    {
        cout << "exception - " << bad_alloc.what() << " : allocated memory exhausted..." << endl;
    }
    cout << "List size is " << TestList.size() << endl;
    cout << "Sort list in accending order..." << endl;
    TestList.sort();
    for(auto& i : TestList)
    {
        cout << i << " ";
    }
    cout << endl;

    cout << "Create list with custom allocator for growing size of 4 list elements each time." << endl;
    list<int32_t, CustomAllocators::Allocator<_List_node<int32_t>, 4>> TestList2;
    try {
        TestList2.push_back(-1);
        TestList2.push_back(-4);
        TestList2.push_back(3);
        TestList2.push_back(3);
        TestList2.push_back(21);
        TestList2.push_back(21);
        TestList2.push_back(21);
    }
    catch (std::bad_alloc& bad_alloc)
    {
        cout << "exception - " << bad_alloc.what() << " : allocated memory exhausted..." << endl;
    }
    cout << "List size is " << TestList2.size() << endl;
    {
        cout << "Sort list in descending order..." << endl;
        greater<int32_t> g;
        TestList2.sort(g);
    }
    for(auto& i : TestList2)
    {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}
