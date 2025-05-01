// TcpServTst.cpp : Defines the entry point for the console application.
//

#include <ServerSimpleUds.h>
#include <memory>
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

const std::string ServerPath = "/tmp/uds_tcp_server";
const int MAX_BUF_SIZE = 4096;


int main(int argc, char* argv[])
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
    
    SOCK_TRY
    {
        ServerSimpleUds Serv(ServerPath);
        Serv.listen();
        char Buf[MAX_BUF_SIZE];
        bool Exit = false;
        while (!Exit)
        {
            shared_ptr < StreamSocket > Sock(Serv.accept());
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            cout << "Connection accepted..." << endl;
#else
            if (Sock.get() == NULL)
            {
                cout << "Accept error" << endl;
                Sock.reset();
                continue;
            }
            cout << "Connection accepted..." << endl;
#endif
            SOCK_TRY
            {
                forever
                {
                    int Res;
                    Res = Sock->receive(Buf, MAX_BUF_SIZE);
                    if (Res > 0)
                    {
                        if (!strncmp(Buf, "END", 3))
                        {
                            Exit = true;
                            Sock->disconnect(5000);
                            break;
                        }
                        Sock->send(Buf, Res);
                        cout << "Echo sent..." << endl;
                    }
#ifdef _WITHOUT_SOCK_EXCEPTIONS
                    else if (Res == 0)
                    {
                        cout << "Disconnected..." << endl;
                        break;
                    }
                    else
                    {
                        char MsgBuf[256];
                        SockException::formatMessage(&MsgBuf);
                        cout << "Error : " << SockException::getErrNum() << " - " << MsgBuf;
                        cout << "Disconnected..." << endl;
                        break;
                    }
#endif
                }
            }
            DISCONN_EXCEPT_CATCH_BEGIN(cout);
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            cout << "Disconnected..." << endl;
#endif
            DISCONN_EXCEPT_CATCH_END SOCK_EXCEPT_CATCH_BEGIN(cout);
            SOCK_EXCEPT_CATCH_END;
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

