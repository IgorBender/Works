#ifndef _WIN32
#include <strings.h>
#endif
#include <cstdlib>
#include <memory>
#include <iomanip>
#include <ClientSimple.h>
#include <signal.h>

#ifdef _WIN32
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

const unsigned short SERV_PORT = 15000;
const size_type MAX_BUF_SIZE = 4096;

#ifndef _WIN32
bool getAddresses(in_addr_t& InterfaceAddr, in_addr_t& BroadAddr, in_addr_t& NetMask)
{
    int Sock = socket(AF_INET, SOCK_DGRAM, 0);
    uint32_t NumOfInterfaces;
    int IORes;
#ifndef __linux__
    IORes = ioctl(Sock, SIOCGIFNUM, &NumOfInterfaces);
    if(IORes < 0)
    {
        return false;
    }
#endif
    ifconf IfConfStruct;
#ifdef __sun__
    IfConfStruct.ifc_len = NumOfInterfaces * sizeof(struct lifreq);
    lifreq *pReq = new lifreq[NumOfInterfaces];
#else
    IfConfStruct.ifc_len = 32000;
    IfConfStruct.ifc_buf = nullptr;
    IORes = ioctl(Sock, SIOCGIFCONF, &IfConfStruct);
    NumOfInterfaces = static_cast<uint32_t>(static_cast<uint32_t>(IfConfStruct.ifc_len) / sizeof(struct ifreq));
    IfConfStruct.ifc_len = static_cast<int32_t>(NumOfInterfaces * sizeof(struct ifreq));
    ifreq *pReq = new ifreq[NumOfInterfaces];
#endif
    IfConfStruct.ifc_buf = reinterpret_cast<caddr_t>(pReq);
    IORes = ioctl(Sock, SIOCGIFCONF, &IfConfStruct);
    if(IORes < 0)
    {
        delete [] pReq;
        return false;
    }
    ifreq *IfReqStrPointer;
    uint32_t i;
    for(i = 0, IfReqStrPointer = reinterpret_cast<ifreq*>(IfConfStruct.ifc_buf);
            i < NumOfInterfaces;  i++, IfReqStrPointer++)
    {
        InterfaceAddr = BroadAddr = NetMask = 0;
        sockaddr_in IntfAddr;
        memcpy(&IntfAddr, &(IfReqStrPointer->ifr_addr), sizeof IntfAddr);
        InterfaceAddr = IntfAddr.sin_addr.s_addr;
        bzero(&IntfAddr, sizeof IntfAddr);
        IORes = ioctl(Sock, SIOCGIFBRDADDR, IfReqStrPointer);
        if(IORes >= 0)
        {
            memcpy(&IntfAddr, &(IfReqStrPointer->ifr_broadaddr), sizeof IntfAddr);
            BroadAddr = IntfAddr.sin_addr.s_addr;
        }
        IORes = ioctl(Sock, SIOCGIFNETMASK, IfReqStrPointer);
        if(IORes >= 0)
        {
            memcpy(&IntfAddr, &(IfReqStrPointer->ifr_addr), sizeof IntfAddr);
            NetMask = IntfAddr.sin_addr.s_addr;
        }
        if(InterfaceAddr && BroadAddr && NetMask)
        {
            in_addr_t Addr = inet_addr("127.0.0.1");
            if(InterfaceAddr == Addr)
                continue;
            delete [] pReq;
            closesocket(Sock);
            return true;
        }
    }
    closesocket(Sock);
    delete [] pReq;
    return false;
}
#endif

int main(int argc, char* argv[])
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
    in_addr_t InterfAddr = 0;
    in_addr_t InterfMask = 0;
    in_addr_t InterfBroad = 0;
#ifdef _WIN32

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        return 1;
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        WSACleanup();
        return 1;
    }
    // Explore interface address and mask
    SOCKET Sock = socket(AF_INET, SOCK_DGRAM, 0);
    INTERFACE_INFO AddrBuf[16];
    DWORD Size = 0;
    int CommRes = WSAIoctl(Sock, SIO_GET_INTERFACE_LIST, NULL, 0, AddrBuf,
                           sizeof(INTERFACE_INFO) * 16, &Size, NULL, NULL);
    if (CommRes == SOCKET_ERROR)
    {
        WSACleanup();
        exit(1);
    }

    int i;
    for (i = 0;  static_cast < unsigned long > (i) < Size / sizeof(INTERFACE_INFO);  i++)
    {
        if (strcmp("127.0.0.1", inet_ntoa(((sockaddr_in*)(& (AddrBuf[i].iiAddress)))->sin_addr)))
            break;
    }
    InterfAddr = ((sockaddr_in*)(& (AddrBuf[i].iiAddress)))->sin_addr.s_addr;
    InterfMask = ((sockaddr_in*)(& (AddrBuf[i].iiNetmask)))->sin_addr.s_addr;
    InterfBroad = ((sockaddr_in*)(& (AddrBuf[i].iiBroadcastAddress)))->sin_addr.s_addr;
#else
    if(!getAddresses(InterfAddr, InterfBroad, InterfMask))
        InterfAddr = InterfBroad = InterfMask = INADDR_ANY;
#ifdef __sun
    sigignore(SIGPIPE);
#endif
#endif

    cout << "Interface address : 0x" << hex << setw(8) << ntohl(InterfAddr) << endl
    << "Broadcast address : 0x" << hex << setw(8) << ntohl(InterfBroad) << endl
    << "Network mask      : 0x" << hex << setw(8) << setfill('0') << ntohl(InterfMask) << endl;
    cout << dec;

    SOCK_TRY
    {
        shared_ptr < ClientSimple > Sock(new ClientSimple);
        forever
		{
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            SOCK_TRY
            {
	            Sock->connect(InterfAddr, htons(SERV_PORT));
            }
            SOCK_EXCEPT_CATCH_BEGIN(cout)
            //SOCK_EXCEPT_CATCH_BEGIN_NOREP
#if !(_WIN32 || __linux__)
                Sock.reset();
#endif

                shared_ptr < ClientSimple > SockTmp(new ClientSimple);
                Sock = SockTmp;
                //SockTmp.release();
#ifndef _WIN32

                sleep(1);
#else

                Sleep(1000);
#endif

                continue;
             SOCK_EXCEPT_CATCH_END;
#else

            SockException::setErrNum();
            int ConnRes = Sock->connect(InterfAddr, htons(SERV_PORT));
            if(SOCKET_ERROR == ConnRes)
			{
			    char MsgStr[256];
                if(SockException::formatMessage(&MsgStr))
                    cout << "Error : " << SockException::getErrNum() << " - " << MsgStr << endl;
                else
                    cout << "Connection refused" << endl;
#ifndef _WIN32

                Sock.reset();
#endif

                shared_ptr < ClientSimple > SockTmp(new ClientSimple);
                Sock = SockTmp;
//                SockTmp.release();
#ifndef _WIN32

                sleep(1);
#else

                Sleep(1000);
#endif

                continue;
            }
#endif
            break;
        }
        sockaddr_in Name;
        sockaddr_in Peer;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        Name = Sock->getSockName();
#else
        Sock->getSockName(&Name);
#endif

        Peer = Sock->getDestination();
        cout << "Socket " << inet_ntoa(Name.sin_addr) << ":"
        << ntohs(Name.sin_port) << " connected to "
        << inet_ntoa(Peer.sin_addr) << ":" << ntohs(Peer.sin_port)
        << endl;
        forever
        {
            char StrBuf[MAX_BUF_SIZE];
#ifndef _WIN32
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
#ifndef _WIN32
                bzero(Buf, MAX_BUF_SIZE);
#else
                memset(Buf, 0, MAX_BUF_SIZE);
#endif
                Sock->receive(Buf, MAX_BUF_SIZE);
                cout << "Echo is -> " << Buf << endl;

#ifndef _WITHOUT_SOCK_EXCEPTIONS
                Name = Sock->getSockName();
                Peer = Sock->getPeerName();
#else
                Sock->getSockName(&Name);
                Sock->getPeerName(&Peer);
#endif
                cout << "Socket " << inet_ntoa(Name.sin_addr) << ":"
                     << ntohs(Name.sin_port) << " connected to "
                     << inet_ntoa(Peer.sin_addr) << ":" << ntohs(Peer.sin_port)
                     << endl;
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
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}

