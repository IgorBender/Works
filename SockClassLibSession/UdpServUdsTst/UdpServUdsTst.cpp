// UdpServTst.cpp : Defines the entry point for the console application.
//


#include <cstdlib>
#include <DatagramBoundUds.h>
#include <iostream>
#include <iomanip>
using namespace std;


#ifdef _MSC_VER
#include <Ws2tcpip.h>
#else
#include <strings.h>
#include <net/if.h>
#ifndef __linux__
#include <sys/sockio.h>
#else
#include <sys/ioctl.h>
#include <linux/sockios.h>
#endif
#include <fcntl.h>
#endif

#define forever for(;;)


const std::string ServerPath = "/tmp/uds_udp_server";
const int MAX_BUF_SIZE = 4096;

int main(int argc, char* argv[])
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
    SOCK_TRY
    {
        DatagramBoundUds Serv(ServerPath);
        if(!Serv.isBound())
        {
            char Msg[256];
            SockException::formatMessage(&Msg);
            cout << Msg << endl;
            exit(1);
        }

        char Buf[MAX_BUF_SIZE];
        forever
        {
            fd_set ReadSet;
            FD_ZERO(&ReadSet);
            FD_SET(Serv.getSock(), &ReadSet);
            int SelRes = select(Serv.getSock() + 1, &ReadSet, NULL, NULL, NULL);
            if(SelRes <= 0)
			{
				cout << "Select errror" << endl;
				exit(1);
            }
            if(FD_ISSET(Serv.getSock(), &ReadSet))
			{
				int Len = Serv.receiveFrom(Buf, MAX_BUF_SIZE);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
                if(Len == SOCKET_ERROR)
                {
                    char Msg[256];
                    SockException::formatMessage(&Msg);
                    cout << Msg << endl;
                    exit(1);
                }
#endif
                if(std::string(Serv.getSource().sun_path).length())
                {
                    cout << "Send to " << std::string(Serv.getSource().sun_path) << endl;
                    Len = Serv.sendToLast(Buf, Len);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
                    if(Len == SOCKET_ERROR)
                    {
                        char Msg[256];
                        SockException::formatMessage(&Msg);
                        cout << Msg << endl;
                        exit(1);
                    }
#endif
                }
                else
                {
                    cout << "Recieved from anonymous socket, can't send response." << endl;
                }
            }
        }
    }
    SOCK_EXCEPT_CATCH_BEGIN(cout)
    SOCK_EXCEPT_CATCH_ALL(cout)
    SOCK_EXCEPT_CATCH_END

#ifdef _MSC_VER
    WSACleanup();
#endif

    return 0;
}

