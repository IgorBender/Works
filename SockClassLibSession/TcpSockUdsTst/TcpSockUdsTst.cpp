#ifndef _MSC_VER
#include <strings.h>
#endif
#include <cstdlib>
#include <memory>
#include <iomanip>
#include <ClientSimpleUds.h>
#include <signal.h>

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
const size_type MAX_BUF_SIZE = 4096;

int main(int argc, char* argv[])
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
    SOCK_TRY
    {
        shared_ptr < ClientSimpleUds > Sock(new ClientSimpleUds);
        forever
		{
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            SOCK_TRY
            {
                Sock->connect(ServerPath);
            }
            SOCK_EXCEPT_CATCH_BEGIN(cout)
            //SOCK_EXCEPT_CATCH_BEGIN_NOREP
#if !(_MSC_VER || __linux__)
                Sock.reset();
#endif

                shared_ptr < ClientSimpleUds > SockTmp(new ClientSimpleUds);
                Sock = SockTmp;
                //SockTmp.release();
#ifndef _MSC_VER

                sleep(1);
#else

                Sleep(1000);
#endif

                continue;
             SOCK_EXCEPT_CATCH_END;
#else

            SockException::setErrNum();
            int ConnRes = Sock->connect(ServerPath);
            if(SOCKET_ERROR == ConnRes)
			{
			    char MsgStr[256];
                if(SockException::formatMessage(&MsgStr))
                    cout << "Error : " << SockException::getErrNum() << " - " << MsgStr << endl;
                else
                    cout << "Connection refused" << endl;
#ifndef _MSC_VER

                Sock.reset();
#endif

                shared_ptr < ClientSimpleUds > SockTmp(new ClientSimpleUds);
                Sock = SockTmp;
//                SockTmp.release();
#ifndef _MSC_VER

                sleep(1);
#else

                Sleep(1000);
#endif

                continue;
            }
#endif
            break;
        }
//        sockaddr_un Name;
        sockaddr_un Peer;
//#ifndef _WITHOUT_SOCK_EXCEPTIONS
//        Name = Sock->getSockName();
//#else
//        Sock->getSockName(&Name);
//#endif

        Peer = Sock->getDestination();
        cout << "Socket " << " connected to " << std::string(Peer.sun_path) << endl;
        forever
        {
            char StrBuf[MAX_BUF_SIZE];
#ifndef _MSC_VER
            bzero(StrBuf, MAX_BUF_SIZE);
#else
            memset(StrBuf, 0, MAX_BUF_SIZE);
#endif
            //string Str(StrBuf, MAX_BUF_SIZE);
            cout << "Enter string or END for end -> " << flush;
            cin.getline(StrBuf, MAX_BUF_SIZE);
            if(!strlen(StrBuf))
            continue;
            char Buf[MAX_BUF_SIZE];
            //Sock.send(const_cast <char*> (Str.c_str()), Str.length());
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            Sock->send(StrBuf, strlen(StrBuf));
#else
            int SendRes = Sock->send(StrBuf, strlen(StrBuf));
            if(SendRes == SOCKET_ERROR)
            {
                char MsgBuf[256];
                    SockException::formatMessage(&MsgBuf);
                    cout << "Error : " << SockException::getErrNum() << " - " << MsgBuf << endl;
                    cout << "Disconnected..." << endl;
                    exit(1);
	        }
#endif
            if(strcmp(StrBuf, "END"))
            {
#ifndef _MSC_VER
                bzero(Buf, MAX_BUF_SIZE);
#else
                memset(Buf, 0, MAX_BUF_SIZE);
#endif
                Sock->receive(Buf, MAX_BUF_SIZE);
                cout << "Echo is -> " << Buf << endl;

#ifndef _WITHOUT_SOCK_EXCEPTIONS
//                Name = Sock->getSockName();
                Peer = Sock->getPeerName();
#else
//                Sock->getSockName(&Name);
                Sock->getPeerName(&Peer);
#endif
                cout << "Socket " << " connected to " << std::string(Peer.sun_path) << endl;
            }
            else
            {
                Sock->disconnect(5000);
                break;
            }
        }
    }
    DISCONN_EXCEPT_CATCH_BEGIN_NOREP
    {
        cout << "Disconnected..." << endl;
    }
    DISCONN_EXCEPT_CATCH_END
    SOCK_EXCEPT_CATCH_BEGIN(cout)
//    SOCK_EXCEPT_CATCH_BEGIN_NOREP;
    SOCK_EXCEPT_CATCH_END
#ifdef _MSC_VER
    WSACleanup();
#endif

    return 0;
}

