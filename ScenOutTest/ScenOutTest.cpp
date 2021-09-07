/**********************************************************************
 * File : ScenOutTest.cpp
 * Created by Igor Bender 2019-04-12
 * ********************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

const uint8_t CHANNEL_1_STATE = 0x01;
const uint8_t CHANNEL_2_STATE = CHANNEL_1_STATE << 1;
const uint8_t CHANNEL_3_STATE = CHANNEL_1_STATE << 2;
const uint8_t CHANNEL_4_STATE = CHANNEL_1_STATE << 3;
const uint8_t POLAR_1_STATE = CHANNEL_1_STATE << 4;
const uint8_t POLAR_2_STATE = CHANNEL_1_STATE << 5;
const uint8_t POLAR_3_STATE = CHANNEL_1_STATE << 6;
const uint8_t POLAR_4_STATE = CHANNEL_1_STATE << 7;

struct BinaryData
{
    uint8_t ActivityBits;
    int16_t Target1Azimuth;
    int16_t Target1Elevation;
    uint16_t Target1Polarization;
    int16_t Target2Azimuth;
    int16_t Target2Elevation;
    uint16_t Target2Polarization;
    int16_t Target3Azimuth;
    int16_t Target3Elevation;
    uint16_t Target3Polarization;
    int16_t Target4Azimuth;
    int16_t Target4Elevation;
    uint16_t Target4Polarization;
} __attribute__ ((packed));


int main(int argc, char* argv[])
{
    if(2 != argc)
    {
        cout << "Usage : " << argv[0] << " scenario_binary_file" << endl;
        return 1;
    }

    ifstream In(argv[1], ios::binary);
    if(!In)
    {
        cout << "Wrong input file" << endl;
        In.close();
        return 1;
    }

    size_t FileSize = fs::file_size(argv[1]);
    if(FileSize % sizeof(BinaryData))
    {
        cout << argv[1] << " is corrupted." << endl;
        In.close();
        return 1;
    }

    ofstream Targ1("target1.txt");
    ofstream Targ2("target2.txt");
    ofstream Targ3("target3.txt");
    ofstream Targ4("target4.txt");
    uint32_t Frame = 0;
    for(size_t i = 0; i < FileSize / sizeof(BinaryData); ++i)
    {
        BinaryData Data;
        In.read(reinterpret_cast<char*>(&Data), sizeof(BinaryData));
        Targ1 << setw(8) << Frame << " ";
        Targ2 << setw(8) << Frame << " ";
        Targ3 << setw(8) << Frame << " ";
        Targ4 << setw(8) << Frame++ << " ";
        if(CHANNEL_1_STATE & Data.ActivityBits)
        {
            Targ1 << setw(8) << 1 << " ";
            if(POLAR_1_STATE & Data.ActivityBits)
            {
                Targ1 << setw(8) << 1 << " ";
            }
            else
            {
                Targ1 << setw(8) << 0 << " ";
            }
            Targ1 << fixed << setw(5) << setprecision(1) << Data.Target1Azimuth / 10.0 << " ";
            Targ1 << fixed << setw(5) << setprecision(1) << Data.Target1Elevation / 10.0 << " ";
            Targ1 << fixed << setw(5) << setprecision(1) << Data.Target1Polarization;
        }
        else
        {
            Targ1 << setw(8) << 0 << " ";
            Targ1 << setw(8) << 0 << " ";
            Targ1 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ1 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ1 << fixed << setw(5) << setprecision(1) << 0.0;
        }
        if(CHANNEL_2_STATE & Data.ActivityBits)
        {
            Targ2 << setw(8) << 1 << " ";
            if(POLAR_2_STATE & Data.ActivityBits)
            {
                Targ2 << setw(8) << 1 << " ";
            }
            else
            {
                Targ2 << setw(8) << 0 << " ";
            }
            Targ2 << fixed << setw(5) << setprecision(1) << Data.Target2Azimuth / 10.0 << " ";
            Targ2 << fixed << setw(5) << setprecision(1) << Data.Target2Elevation / 10.0 << " ";
            Targ2 << fixed << setw(5) << setprecision(1) << Data.Target2Polarization;
        }
        else
        {
            Targ2 << setw(8) << 0 << " ";
            Targ2 << setw(8) << 0 << " ";
            Targ2 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ2 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ2 << fixed << setw(5) << setprecision(1) << 0.0;
        }
        if(CHANNEL_3_STATE & Data.ActivityBits)
        {
            Targ3 << setw(8) << 1 << " ";
            if(POLAR_3_STATE & Data.ActivityBits)
            {
                Targ3 << setw(8) << 1 << " ";
            }
            else
            {
                Targ3 << setw(8) << 0 << " ";
            }
            Targ3 << fixed << setw(5) << setprecision(1) << Data.Target3Azimuth / 10.0 << " ";
            Targ3 << fixed << setw(5) << setprecision(1) << Data.Target3Azimuth / 10.0 << " ";
            Targ3 << fixed << setw(5) << setprecision(1) << Data.Target3Elevation / 10.0 << " ";
            Targ3 << fixed << setw(5) << setprecision(1) << Data.Target3Polarization;
        }
        else
        {
            Targ3 << setw(8) << 0 << " ";
            Targ3 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ3 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ3 << fixed << setw(5) << setprecision(1) << 0.0;
        }
        if(CHANNEL_4_STATE & Data.ActivityBits)
        {
            Targ4 << setw(8) << 1 << " ";
            if(POLAR_4_STATE & Data.ActivityBits)
            {
                Targ4 << setw(8) << 1 << " ";
            }
            else
            {
                Targ4 << setw(8) << 0 << " ";
            }
            Targ4 << fixed << setw(5) << setprecision(1) << Data.Target4Azimuth / 10.0 << " ";
            Targ4 << fixed << setw(5) << setprecision(1) << Data.Target4Elevation / 10.0 << " ";
            Targ4 << fixed << setw(5) << setprecision(1) << Data.Target4Polarization;
        }
        else
        {
            Targ4 << setw(8) << 0 << " ";
            Targ4 << setw(8) << 0 << " ";
            Targ4 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ4 << fixed << setw(5) << setprecision(1) << 0.0 << " ";
            Targ4 << fixed << setw(5) << setprecision(1) << 0.0;
        }
        Targ1 << endl;
        Targ2 << endl;
        Targ3 << endl;
        Targ4 << endl;
    }
    In.close();
    Targ1.close();
    Targ2.close();
    Targ3.close();
    Targ4.close();
    return 0;
}
