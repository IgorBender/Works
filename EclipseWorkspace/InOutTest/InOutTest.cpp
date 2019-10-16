/*
 *  InOutTest.cpp
 *
 *  Created on: 2017-06-17
 *  Author: Igor Bender
 *
 */
#include <iostream>
#include <iomanip>
#include <bitset>
#include <fstream>
#include <sstream>
#include "StringColorizer.h"

using namespace std;

int main()
{
    // Instruct cin deduct input automaticly.
    cin.unsetf(ios::hex);
    cin.unsetf(ios::dec);
    cin.unsetf(ios::oct);

    cout << "Enter number of any base --> ";
    uint32_t InVal = 0;
    cin >> InVal;
    if(!cin)
    {
        cout << "Wrong input number" << endl;
        return 1;
    }
    cout << "The number is" << StringColorizer::StartColorize(StringColorizer::Forground::Green, StringColorizer::Background::None, StringColorizer::Font::Bold)
         << " dec: " << InVal << ","
         << StringColorizer::StartColorize(StringColorizer::Forground::Blue, StringColorizer::Background::None, StringColorizer::Font::Bold)
         << " hex: " << internal << setw(10) << setfill('0') << showbase << hex << InVal << ","
         << StringColorizer::StartColorize(StringColorizer::Forground::Magenta, StringColorizer::Background::None, StringColorizer::Font::Bold)
         << " oct: " << oct << InVal << "," << dec
         << StringColorizer::StartColorize(StringColorizer::Forground::Red, StringColorizer::Background::None, StringColorizer::Font::Bold)
         << " bin : " << bitset<32>(InVal)
         << StringColorizer::EndColorize() << "." << endl;

    string Hello = "Hello";
//    Hello = StringColorizer::colorize(Hello, StringColorizer::Forground::Yellow, StringColorizer::Background::Red, StringColorizer::Font::Bold);
//    Hello += StringColorizer::colorize(" World", StringColorizer::Forground::Yellow, StringColorizer::Background::Red/*, StringColorizer::Font::Bold*/);
    Hello += "!!!";
    cout << Hello << endl;

    stringstream Out;
    Out << StringColorizer::StartColorize(StringColorizer::Forground::None, StringColorizer::Background::None, StringColorizer::Font::Bold)
        << "***" << StringColorizer::EndColorize() << ends;
    cout << Out.str() << endl;

    return 0;
}
