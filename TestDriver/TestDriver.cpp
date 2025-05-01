/*
 *  TestDriver.cpp
 *
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <iostream>
#include <sstream>


using namespace std;

#include "testdrv-ioctl.h"


int main(int32_t argc, char** argv)
{
    int32_t TestFd[4] = {-1, -1, -1, -1};
    char* pMem[4] = {nullptr, nullptr, nullptr, nullptr};
    char ReadBuffer[8192];
    char WriteBuffer[8192];
    bool Exit = false;


    while(!Exit)
    {
        uint32_t Choise = 0;
        cout << "Open device file - 1" << endl
             << "Close device file - 2" << endl
             << "Write pattern to device file - 3" << endl
             << "Read pattern from device file - 4" << endl
             << "Write string to device file - 5" << endl
             << "Read string from device file - 6" << endl
             << "Direct device file operation - 7" << endl
             << "Map device file to memory - 8" << endl
             << "Exit - 99" << endl
             << "  Enter your coise --> ";
        cin >> Choise;
        switch (Choise)
        {
        case 1:
        {
            uint32_t DevNum = 5;
            stringstream N;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            stringstream Num;
            Num << DevNum;
            if(3 >= DevNum && -1 == TestFd[DevNum])
            {
                TestFd[DevNum] = open((string("/dev/testdrv")  + Num.str()).c_str(), O_RDWR);
                if(-1 == TestFd[DevNum])
                    cout << endl << "== Failure ==" << endl << endl;
                else
                    cout << endl << "== Success ==" << endl << endl;
            }
            break;
        }

        case 2:
        {
            uint32_t DevNum = 5;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            if(3 >= DevNum && -1 != TestFd[DevNum])
            {
                if(nullptr != pMem[DevNum])
                    munmap(pMem[DevNum], 4096);
                close(TestFd[DevNum]);
                TestFd[DevNum] = -1;
            }
            break;
        }

        case 3:
        {
            uint32_t DevNum = 5;
            uint32_t Size;
            uint8_t Symbol;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            cout << "\tEnter size for write --> ";
            cin >> Size;
            cout << "\tEnter symbol for write --> ";
            cin >> Symbol;
            if(3 >= DevNum && -1 != TestFd[DevNum])
            {
                memset(WriteBuffer, Symbol, sizeof(WriteBuffer));
                lseek(TestFd[DevNum], 0, SEEK_SET);
                int32_t Written = write(TestFd[DevNum], WriteBuffer, Size);
                cout << endl << " == " << Written << " bytes is written. ==" << endl << endl;
            }

            break;
        }

        case 4:
        {
            uint32_t DevNum = 5;
            uint32_t Size;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            cout << "\tEnter size for read --> ";
            cin >> Size;
            if(3 >= DevNum && -1 != TestFd[DevNum])
            {
                lseek(TestFd[DevNum], 0, SEEK_SET);
                int32_t Read = read(TestFd[DevNum], ReadBuffer, Size);
                cout << endl << " == " << Read << " bytes is read. ==" << endl << endl;
            }

            break;
        }

        case 5:
        {
            uint32_t DevNum = 5;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            cout << "\tEnter string --> " << flush;
            cin.getline(WriteBuffer, 4096);
            cin.getline(WriteBuffer, 4096);
            if(3 >= DevNum && -1 != TestFd[DevNum])
            {
                lseek(TestFd[DevNum], 0, SEEK_SET);
                int32_t Written = write(TestFd[DevNum], WriteBuffer, strlen(WriteBuffer) + 1);
                cout << endl << " == " << Written << " bytes is written. ==" << endl << endl;
            }
            break;
        }

        case 6:
        {
            uint32_t DevNum = 5;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            if(3 >= DevNum && -1 != TestFd[DevNum])
            {
                lseek(TestFd[DevNum], 0, SEEK_SET);
                int32_t Read = read(TestFd[DevNum], ReadBuffer, sizeof(ReadBuffer));
                cout << endl << " == " << Read << " bytes is read. ==" << endl << endl;
                cout << " == " << string(ReadBuffer) << " == " << endl << endl;
            }
            break;
        }

        case 7:
        {
            uint32_t DevNum = 5;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            cout << "\tEnter operation (0 - get version, 1 - zero memory, 2 - set memory --> ";
            uint32_t Operation = 5;
            cin >> Operation;
            if(2 < Operation)
                break;
            if(3 >= DevNum && -1 != TestFd[DevNum])
            {
                if(0 == Operation)
                {
                    Version Ver;
                    int32_t OperationResult = ioctl(TestFd[DevNum], TESTDRV_GET_DRIVER_VERSION, &Ver);
                    if(0 != OperationResult)
                    {
                        break;
                    }
                    cout << endl << "== Driver version : " << Ver.Major << "." << Ver.Minor << " ==" << endl << endl;
                }
                else if(1 == Operation)
                {
                    int32_t OperationResult = ioctl(TestFd[DevNum], TESTDRV_ZERO_MEMORY);
                    if(0 != OperationResult)
                    {
                        break;
                    }
                    cout << endl << "== Success ==" << endl << endl;
                }
                else
                {
                    uint8_t Symbol;
                    cout << "\tEnter symbol --> ";
                    cin >> Symbol;
                    int32_t OperationResult = ioctl(TestFd[DevNum], TESTDRV_FILL_MEMORY, Symbol);
                    if(0 != OperationResult)
                    {
                        break;
                    }
                    cout << endl << "== Success ==" << endl << endl;
                }
            }

            break;
        }

        case 8:
        {
            uint32_t DevNum = 5;
            cout << "\tEnter device number (0 - 3) --> ";
            cin >> DevNum;
            if(3 >= DevNum && -1 != TestFd[DevNum] && !pMem[DevNum])
            {
                pMem[DevNum] = (char*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, TestFd[DevNum], 0);
                if(nullptr == pMem[DevNum])
                    cout << endl << "== Failure ==" << endl << endl;
                else
                    cout << endl << "== Success ==" << endl << endl;
            }
            break;
        }

        case 99:
        {
            for(auto& Fd : TestFd)
            {
                if(-1 != Fd && nullptr != pMem[Fd])
                {
                    munmap(pMem[Fd], 4096);
                    pMem[Fd] = nullptr;
                }
                if(-1 != Fd)
                {
                    close(Fd);
                    Fd = -1;
                }
            }
            Exit = true;
            break;
        }

        default:
            break;
        }
    }
    return 0;
}

