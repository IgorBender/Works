// UdpServTst.cpp : Defines the entry point for the console application.
//


#include <cstdlib>
#include <BroadcastBound.h>
#include <DatagramBound.h>
#include <iostream>
#include <iomanip>
using namespace std;


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


const short SERV_PORT = 15000;
const int MAX_BUF_SIZE = 4096;

#ifndef _WIN32
bool getAddresses(in_addr_t& InterfaceAddr, in_addr_t& BroadAddr, in_addr_t& NetMask)
{
    SOCKET Sock = socket(AF_INET, SOCK_DGRAM, 0);
    int NumOfInterfaces;
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
    IfConfStruct.ifc_buf = NULL;
    IORes = ioctl(Sock, SIOCGIFCONF, &IfConfStruct);
    NumOfInterfaces = IfConfStruct.ifc_len / sizeof(struct ifreq);
    IfConfStruct.ifc_len = NumOfInterfaces * sizeof(struct ifreq);
    ifreq *pReq = new ifreq[NumOfInterfaces];
#endif
    IfConfStruct.ifc_buf = (caddr_t)pReq;
    IORes = ioctl(Sock, SIOCGIFCONF, &IfConfStruct);
    if(IORes < 0)
    {
        delete [] pReq;
        return false;
    }
    ifreq *IfReqStrPointer;
    int i;
    for(i = 0, IfReqStrPointer = (ifreq *)IfConfStruct.ifc_buf;
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
    closesocket(Sock);
#else
    if(!getAddresses(InterfAddr, InterfBroad, InterfMask))
        InterfAddr = InterfBroad = InterfMask = INADDR_ANY;
#endif

    cout << "Interface address : 0x" << hex << setw(8) << ntohl(InterfAddr) << endl
    << "Broadcast address : 0x" << hex << setw(8) << ntohl(InterfBroad) << endl
    << "Network mask      : 0x" << hex << setw(8) << setfill('0') << ntohl(InterfMask) << endl;

    SOCK_TRY
    {
        DatagramBound Serv(htons(SERV_PORT));
        if(!Serv.isBound())
        {
            char Msg[256];
            SockException::formatMessage(&Msg);
            cout << Msg << endl;
            exit(1);
        }

#ifndef _WIN32
        DatagramBound Serv2(htons(SERV_PORT + 1), InterfBroad);
#else
        DatagramBound Serv2(htons(SERV_PORT + 1), InterfAddr);
#endif

        if(!Serv2.isBound())
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
            FD_SET(Serv2.getSock(), &ReadSet);
            SOCKET SockNum = ((Serv.getSock() > Serv2.getSock()) ? Serv.getSock() : Serv2.getSock()) + 1;
            int SelRes = select(static_cast < int > (SockNum), &ReadSet, NULL, NULL, NULL);
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
				cout << "Send to " << inet_ntoa(Serv.getSource().sin_addr) << endl;
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

            if(FD_ISSET(Serv2.getSock(), &ReadSet))
			{
				int Len = Serv2.receiveFrom(Buf, MAX_BUF_SIZE);
#ifdef _WITHOUT_SOCK_EXCEPTIONS
                if(Len == SOCKET_ERROR)
                {
                    char Msg[256];
                    SockException::formatMessage(&Msg);
                    cout << Msg << endl;
                    exit(1);
                }
#endif
				cout << "Send to " << inet_ntoa(Serv2.getSource().sin_addr) << endl;
                Len = Serv2.sendToLast(Buf, Len);
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
        }
    }
    SOCK_EXCEPT_CATCH_BEGIN(cout)
    SOCK_EXCEPT_CATCH_ALL(cout)
    SOCK_EXCEPT_CATCH_END

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}

