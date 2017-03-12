#include <cstdlib>
//#include <BroadcastBoundV6.h>
#include <DatagramSocketV6.h>
#include <iostream>
#include <iomanip>
using namespace std;


#ifdef _WIN32
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
#elif _WIN32
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	unsigned long OutBufLen = 16 * 1024;
	const int MAX_TRIES = 3;
	int Iterations = 0;
	unsigned int RetVal;
	do {
		pAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new char[OutBufLen]);
		if(pAddresses == NULL)
		{
			return false;
		}
		RetVal = GetAdaptersAddresses(AF_INET6,
			GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME, NULL,
			pAddresses, &OutBufLen);

		if(RetVal == ERROR_BUFFER_OVERFLOW)
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
#ifdef _NO_IPV6
	cout << "No IPv6 support configured in sockclasslib." << endl;
	return 1;
#else
    in6_addr InterfAddr;
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
	// Explore interface address
#endif

    if(!getAddresses(InterfAddr))
        InterfAddr = in6addr_any;
    char Tmp[64];
    cout << "Interface address : " << inet_ntop(AF_INET6, &InterfAddr, Tmp, 64) << endl;

    SOCK_TRY
    {
        DatagramSocketV6 Sock;
        char Buf[MAX_BUF_SIZE];

        strcpy(Buf, "Hello");

        // --------------------------------------------------------------
        // Send datagram to server
        cout << "-----------------------------" << endl;
        cout << "Send to..." << endl;
        strcpy(Buf, "Hello");

        Sock.setDestination(InterfAddr, htons(SERV_PORT));
        Sock.sendTo(Buf, strlen("Hello"));

#ifdef _WITHOUT_SOCK_EXCEPTIONS
        int Res = Sock.sendTo(Buf, strlen("Hello"));
        if(Res == SOCKET_ERROR)
		{
			char Msg[256];
			SockException::formatMessage(&Msg);
			cout << Msg << endl;
			exit(1);
		}
#endif
        memset(Buf, 0, MAX_BUF_SIZE);
        int Num = Sock.receiveFrom(Buf, MAX_BUF_SIZE);
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
        // --------------------------------------------------------------
        // Connect UDP socket and send datagrams
        cout << "-----------------------------" << endl;
        cout << "Send" << endl;
        DatagramSocketV6 DatSock;
        DatSock.setDestination(InterfAddr, htons(SERV_PORT));
#ifdef _WITHOUT_SOCK_EXCEPTIONS
        Res = DatSock.connect();
        if(Res == SOCKET_ERROR)
		{
			char Msg[256];
			SockException::formatMessage(&Msg);
			cout << Msg << endl;
			exit(1);
		}
#endif
        DatSock.connect();
        strcpy(Buf, "Hello");

        DatSock.send(Buf, strlen("Hello"));
#ifdef _WITHOUT_SOCK_EXCEPTIONS
        int Res = DatSock.send(Buf, strlen("Hello"));
        if(Res == SOCKET_ERROR)
		{
			char Msg[256];
			SockException::formatMessage(&Msg);
			cout << Msg << endl;
			exit(1);
		}
#endif
        memset(Buf, 0, MAX_BUF_SIZE);
        Num = DatSock.receive(Buf, MAX_BUF_SIZE);
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

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
#endif
}

