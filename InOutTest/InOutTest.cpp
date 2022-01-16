/*
 *  InOutTest.cpp
 *
 *  Created on: 2017-06-17
 *  Author: Igor Bender
 *
 */
#include "StringColorizer.h"
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#ifdef _MSC_VER
#include <Windows.h>
#endif

using namespace std;

#ifdef _MSC_VER
// Some old MinGW/CYGWIN distributions don't define this:
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

static HANDLE stdoutHandle;
static DWORD outModeInit;

void setupConsole(void)
{
    DWORD outMode = 0;
    stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if(stdoutHandle == INVALID_HANDLE_VALUE)
    {
      	exit(GetLastError());
    }
    
    if(!GetConsoleMode(stdoutHandle, &outMode))
    {
        exit(GetLastError());
    }
    
    outModeInit = outMode;
    
    // Enable ANSI escape codes
    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    
    if(!SetConsoleMode(stdoutHandle, outMode))
    {
        exit(GetLastError());
    }
}

void restoreConsole(void)
{
    // Reset console mode
    if(!SetConsoleMode(stdoutHandle, outModeInit))
    {
        exit(GetLastError());
    }
}
#else
void setupConsole(void) {}

void restoreConsole(void)
{
    // Reset colors
    printf("\x1b[0m");
}
#endif

//#define format_hex(num, width) showbase << setw(0 == num ? width - 2 : width) << (0 == num ? "0x" : "") \
//    << setfill('0') << internal

//ostream& operator << (ostream& o, uint32_t Num, uint32_t Width)
//{
//    if(Num)
//        o << hex << "0x" << setw(Width - 2) << setfill('0') << Num;
//    else
//        o << hex << internal << setw(Width) << setfill('0') << showbase << Num;
//    return o;
//}

int main()
{
    setupConsole();
    // Instruct cin deduct input automaticly.
    cin.unsetf(ios::hex);
    cin.unsetf(ios::dec);
    cin.unsetf(ios::oct);
    cout.unsetf(ios::hex);
    cout.unsetf(ios::dec);
    cout.unsetf(ios::oct);

    cout << "Enter number of any base --> ";
    uint32_t InVal = 0;
    cin >> InVal;
    if(!cin)
    {
        cout << "Wrong input number" << endl;
        return 1;
    }
    cout << "The number is"
         << StringColorizer::StartColorize(StringColorizer::Forground::Green,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " dec: " << InVal << ","
         << StringColorizer::StartColorize(StringColorizer::Forground::Blue,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
//         << " hex: " << print_hex(cout, 10, InVal) << ","
         << " hex: " << internal << setw(10) << setfill('0') << showbase << hex << InVal << ","
         << StringColorizer::StartColorize(StringColorizer::Forground::Magenta,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " oct: " << oct << InVal << "," << dec
         << StringColorizer::StartColorize(StringColorizer::Forground::Red,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " bin : " << bitset<32>(InVal) << StringColorizer::EndColorize() << "." << endl;

    // Alternative way to state hex base 0x explicitly and ommit showbase
    cout << "The number is"
         << StringColorizer::StartColorize(StringColorizer::Forground::Green,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " dec: " << InVal << ","
         << StringColorizer::StartColorize(StringColorizer::Forground::Blue,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " hex: " << "0x" << setw(8) << setfill('0') << hex << uppercase << InVal << ","
         << StringColorizer::StartColorize(StringColorizer::Forground::Magenta,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " oct: " << oct << InVal << "," << dec
         << StringColorizer::StartColorize(StringColorizer::Forground::Red,
                                           StringColorizer::Background::None,
                                           StringColorizer::Font::Bold)
         << " bin : " << bitset<32>(InVal) << StringColorizer::EndColorize() << "." << endl;


    string Hello;
    Hello = StringColorizer::StartColorize(StringColorizer::Forground::Yellow,
        StringColorizer::Background::Red, StringColorizer::Font::Bold).colorize();
    Hello += "Hello";
    Hello += StringColorizer::EndColorize().ColorEnd;
    Hello +=
        StringColorizer::StartColorize(StringColorizer::Forground::Yellow,
        StringColorizer::Background::Red, StringColorizer::Font::None).colorize();
    Hello += " World";
    Hello += "!!!";
    Hello += StringColorizer::EndColorize().ColorEnd;

    cout << Hello << endl;

    stringstream Out;
    Out << StringColorizer::StartColorize(StringColorizer::Forground::None,
                                          StringColorizer::Background::None,
                                          StringColorizer::Font::Bold)
        << "***" << StringColorizer::EndColorize() << ends;
    cout << Out.str() << endl;
    restoreConsole();

    return 0;
}
