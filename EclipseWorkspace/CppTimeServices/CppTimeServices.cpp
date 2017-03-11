/*
 * CppTimeServices.cpp
 *
 *  Created on: Oct 28, 2016
 *      Author: igor
 */

#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>

#include "TimeMeasurement.h"

using namespace std;

int main(int argc, char* argv[])
{
    // Simple delay
//    cout << "Going sleep for 3 seconds..." << flush;
//    this_thread::sleep_for(chrono::seconds(3));
//    cout << " resuming." << endl;

    // Calculate time.
//    chrono::time_point<chrono::high_resolution_clock> Start, End;
//    Start = chrono::high_resolution_clock::now();
//    this_thread::sleep_for(chrono::milliseconds(322));
    TimeMeasurement Measurement;
    Measurement.startMeasure();
    double m = 0.0;
    for(int32_t i = 0; i < 1000; ++i)
    {
        m *= (m * pow(i, 3));
    }
//    End = chrono::high_resolution_clock::now();
//    chrono::duration<float, micro> Measured = End - Start;
    cout << Measurement.getElapsedTimeUsec() << endl;

    return 0;
}
