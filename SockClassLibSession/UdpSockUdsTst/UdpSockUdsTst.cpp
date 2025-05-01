// UdpSockTst.cpp : Defines the entry point for the console application.
//


#include <DatagramBoundUds.h>
#include <iostream>
#include <iomanip>
using namespace std;


#include <cstdlib>
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
const std::string ClientPath = "/tmp/uds_udp_client";
const int MAX_BUF_SIZE = 4096;

int main(int argc, char* argv[])
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
    SOCK_TRY
    {
        DatagramBoundUds Sock(ClientPath);
        char Buf[MAX_BUF_SIZE];

        strcpy(Buf, "Hello");

        // -------------------------------------------------------------
        // Send datagram to server and wait response
        cout << "-----------------------------" << endl;
        cout << "Send to " << ServerPath << endl;
        Sock.setDestination(ServerPath);
        int Res = Sock.sendTo(Buf, strlen("Hello"));
#ifdef _WITHOUT_SOCK_EXCEPTIONS
        if(Res == SOCKET_ERROR)
        {
        	char Msg[256];
            SockException::formatMessage(&Msg);
            cout << Msg << endl;
            exit(1);
        }
#endif
        int32_t Num = Sock.receiveFrom(Buf, MAX_BUF_SIZE);
        Buf[Num] = '\0';
        cout << Buf << endl;


        // Connect UDP socket and send datagrams
        cout << "-----------------------------" << endl;
        cout << "Send" << endl;
#ifdef _WITHOUT_SOCK_EXCEPTIONS
        Res = Sock.connect();
        if(Res == SOCKET_ERROR)
        {
        	char Msg[256];
            SockException::formatMessage(&Msg);
            cout << Msg << endl;
            exit(1);
        }
#else
        Sock.connect();
#endif
        strcpy(Buf, "Hello");

        Res = Sock.send(Buf, strlen("Hello"));
#ifdef _WITHOUT_SOCK_EXCEPTIONS
        if(Res == SOCKET_ERROR)
        {
        	char Msg[256];
            SockException::formatMessage(&Msg);
            cout << Msg << endl;
            exit(1);
        }
#endif
        memset(Buf, 0, MAX_BUF_SIZE);
        Num = Sock.receive(Buf, MAX_BUF_SIZE);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
        if(Num == SOCKET_ERROR)
        {
        	char Msg[256];
            SockException::formatMessage(&Msg);
            cout << Msg << endl;
            exit(1);
        }
#endif
        Buf[Num] = '\0';
        cout << Buf << endl;
    }
    SOCK_EXCEPT_CATCH_BEGIN(cout)
    SOCK_EXCEPT_CATCH_ALL(cout)
    SOCK_EXCEPT_CATCH_END

#ifdef _MSC_VER
    WSACleanup();
#endif

    return 0;
}
