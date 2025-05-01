// UdpServV6Tst.cpp : Defines the entry point for the console application.
//


#include <cstdlib>
#include <DatagramBoundV6.h>
#include <iostream>
#include <iomanip>
using namespace std;


#ifdef _MSC_VER
#include <Ws2tcpip.h>
#include <iphlpapi.h>
#else
#include <strings.h>
#include <net/if.h>
#ifndef __linux__
#include <sys/sockio.h>
#else
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <ifaddrs.h> // getifaddrs()
#endif
#include <fcntl.h>
#endif

#define forever for(;;)

const short SERV_PORT = 15000;
const int MAX_BUF_SIZE = 4096;

#ifndef _NO_IPV6
bool getAddresses(in6_addr& InterfaceAddr)
{
#ifdef __linux__
	ifaddrs* pAddrs;
	int Result = getifaddrs(&pAddrs);
	if(0 > Result)
	{
		return false;
	}
	ifaddrs* pTempAddr;
	pTempAddr = pAddrs;
	while(NULL != pTempAddr)
	{
		if(AF_INET6 == pTempAddr->ifa_addr->sa_family)
		{
			sockaddr_in6* pIpV6Addr = reinterpret_cast<sockaddr_in6*>(pTempAddr->ifa_addr);
			in6_addr TmpAddr;
			inet_pton(AF_INET6, "::1", &TmpAddr);
			if(!memcmp(TmpAddr.s6_addr, pIpV6Addr->sin6_addr.s6_addr, sizeof(TmpAddr)))
			{
				pTempAddr = pTempAddr->ifa_next;
				continue;
			}
			if(0xfe == pIpV6Addr->sin6_addr.s6_addr[0] && 0x80 == pIpV6Addr->sin6_addr.s6_addr[1])
			{
				pTempAddr = pTempAddr->ifa_next;
				continue;
			}
			memcpy(&InterfaceAddr, &pIpV6Addr->sin6_addr, sizeof(InterfaceAddr));
			freeifaddrs(pAddrs);
			return true;
		}
		pTempAddr = pTempAddr->ifa_next;
	}
	freeifaddrs(pAddrs);
	return false;
#elif _MSC_VER
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	unsigned long OutBufLen = 16 * 1024;
	const int MAX_TRIES = 3;
	int Iterations = 0;
	unsigned int RetVal;
	do {
		pAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new char[OutBufLen]);
		if (pAddresses == NULL)
		{
			return false;
		}
		RetVal = GetAdaptersAddresses(AF_INET6,
			GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME, NULL,
			pAddresses, &OutBufLen);

		if (RetVal == ERROR_BUFFER_OVERFLOW)
		{
			delete[] pAddresses;
			pAddresses = NULL;
		}
		else
		{
			break;
		}

		Iterations++;
	} while ((RetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	PIP_ADAPTER_UNICAST_ADDRESS_LH pUnicast = NULL;
	sockaddr_in6* pTmp = NULL;
	if (RetVal == NO_ERROR)
	{
		IP_ADAPTER_ADDRESSES* pCurrAddresses = pAddresses;
		while (pCurrAddresses)
		{
			if (TUNNEL_TYPE_NONE != pCurrAddresses->TunnelType)
			{
				pCurrAddresses = pCurrAddresses->Next;
				continue;
			}

			pUnicast = pCurrAddresses->FirstUnicastAddress;
			while (pUnicast != NULL)
			{
				in6_addr TmpAddr;
				inet_pton(AF_INET6, "::1", &TmpAddr);
				pTmp = reinterpret_cast<sockaddr_in6*>(pUnicast->Address.lpSockaddr);
				if (!memcmp(TmpAddr.s6_addr, pTmp->sin6_addr.s6_addr, sizeof(TmpAddr)))
				{
					pUnicast = pUnicast->Next;
					continue;
				}
				if (unsigned char('\xfe') == pTmp->sin6_addr.s6_addr[0] && unsigned char('\x80') == pTmp->sin6_addr.s6_addr[1])
				{
					pUnicast = pUnicast->Next;
					continue;
				}
				break;
			}
			if (NULL == pUnicast)
			{
				pCurrAddresses = pCurrAddresses->Next;
				continue;
			}
			else
			{
				break;
			}
			pCurrAddresses = pCurrAddresses->Next;
		}
		if (NULL == pCurrAddresses)
		{
			cout << "Error resolving address." << endl;
			return false;
		}
		memcpy(&InterfaceAddr, pTmp->sin6_addr.s6_addr, sizeof(InterfaceAddr));
		return true;
	}
	else
	{
		cout << "Error resolving address." << endl;
		return false;
	}

#endif
}
#endif

int main(int argc, char* argv[])
{
    if(1 < argc)
    {
        cout << argv[0] << " has no command line arguments." << endl;
    }
#ifdef _NO_IPV6
	cout << "No IPv6 support configured in sockclasslib." << endl;
	return 1;
#else
    in6_addr InterfAddr;
#ifdef _MSC_VER

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

#endif

    if(!getAddresses(InterfAddr))
        InterfAddr = in6addr_any;
    char Tmp[64];
    cout << "Interface address : " << inet_ntop(AF_INET6, &InterfAddr, Tmp, 64) << endl;

    SOCK_TRY
    {
        DatagramBoundV6 Serv(htons(SERV_PORT));
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
            SOCKET SockNum = Serv.getSock() + 1;
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
                cout << "Send to " << inet_ntop(AF_INET6, &Serv.getSource().sin6_addr, Tmp, 64) << endl;
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

        }
    }
    SOCK_EXCEPT_CATCH_BEGIN(cout)
    SOCK_EXCEPT_CATCH_ALL(cout)
    SOCK_EXCEPT_CATCH_END

#ifdef _MSC_VER
    WSACleanup();
#endif

    return 0;
#endif
}

