#include <iostream>
#include <memory>
#include <ServerReuseIpDual.h>

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

using namespace std;

#ifndef _NO_IPV6
bool getAddresses(in6_addr& InterfaceAddr)
{
#ifdef __linux__
	ifaddrs* pAddrs;
	int Result = getifaddrs(&pAddrs);
	if (0 > Result)
	{
		return false;
	}
	ifaddrs* pTempAddr;
	pTempAddr = pAddrs;
	while (nullptr != pTempAddr)
	{
		if (AF_INET6 == pTempAddr->ifa_addr->sa_family)
		{
			sockaddr_in6* pIpV6Addr = reinterpret_cast<sockaddr_in6*>(pTempAddr->ifa_addr);
			in6_addr TmpAddr;
			inet_pton(AF_INET6, "::1", &TmpAddr);
			if (!memcmp(TmpAddr.s6_addr, pIpV6Addr->sin6_addr.s6_addr, sizeof(TmpAddr)))
			{
				pTempAddr = pTempAddr->ifa_next;
				continue;
			}
			if (0xfe == pIpV6Addr->sin6_addr.s6_addr[0] && 0x80 == pIpV6Addr->sin6_addr.s6_addr[1])
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

int main(int32_t argc, char* argv[])
{
	if (1 < argc)
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
#endif

	if (!getAddresses(InterfAddr))
		InterfAddr = in6addr_any;
	char Tmp[64];
	cout << "Interface address : " << inet_ntop(AF_INET6, &InterfAddr, Tmp, 64) << endl;


    SOCK_TRY
    {
        ServerReuseIpDual Serv(htons(15001));
        Serv.listen();
		char Buf[MAX_BUF_SIZE];
		bool Exit = false;
		while (!Exit)
		{
			shared_ptr < StreamSocketV6 > Sock(Serv.accept());
			char FromAddrString[INET6_ADDRSTRLEN];
			char ToAddrString[INET6_ADDRSTRLEN];
#ifndef _WITHOUT_SOCK_EXCEPTIONS
            sockaddr_in6 PeerName;
            sockaddr_in6 SockName;
            PeerName = Sock->getPeerName();
			SockName = Sock->getSockName();
			if (IN6_IS_ADDR_V4MAPPED(&PeerName.sin6_addr))
			{
#ifdef _MSC_VER
                inet_ntop(AF_INET, PeerName.sin6_addr.s6_addr + 12, FromAddrString,
					INET6_ADDRSTRLEN);
#else
                inet_ntop(AF_INET, &PeerName.sin6_addr.s6_addr32[3], FromAddrString,
					INET6_ADDRSTRLEN);
#endif
			}
			else
			{
				memset(FromAddrString, 0, INET6_ADDRSTRLEN);
				inet_ntop(AF_INET6, &PeerName.sin6_addr, FromAddrString, INET6_ADDRSTRLEN);
			}
			if (IN6_IS_ADDR_V4MAPPED(&SockName.sin6_addr))
			{
#ifdef _MSC_VER
				inet_ntop(AF_INET, SockName.sin6_addr.s6_addr + 12, ToAddrString,
					INET6_ADDRSTRLEN);
#else
                inet_ntop(AF_INET, &SockName.sin6_addr.s6_addr32[3], ToAddrString,
					INET6_ADDRSTRLEN);
#endif
			}
			else
			{
				memset(ToAddrString, 0, INET6_ADDRSTRLEN);
				inet_ntop(AF_INET6, &SockName.sin6_addr, ToAddrString, INET6_ADDRSTRLEN);
			}

			cout << "Connection accepted : "
				<< ToAddrString << "-" << ntohs(Sock->getSockName().sin6_port)
				<< " <--> "
				<< FromAddrString << "-" << ntohs(Sock->getPeerName().sin6_port)
				<< endl;
#else
			if (Sock.get() == nullptr)
			{
				cout << "Accept error" << endl;
				Sock.reset();
				continue;
			}
			sockaddr_in6 SockName;
			sockaddr_in6 PeerName;
			Sock->getSockName(&SockName);
			Sock->getPeerName(&PeerName);
			if (IN6_IS_ADDR_V4MAPPED(&PeerName.sin6_addr))
			{
#ifdef _MSC_VER
				inet_ntop(AF_INET, PeerName.sin6_addr.s6_addr + 12, FromAddrString,
					INET6_ADDRSTRLEN);
#else
                inet_ntop(AF_INET, &PeerName.sin6_addr.s6_addr32[3], FromAddrString,
					INET6_ADDRSTRLEN);
#endif
			}
			else
			{
				memset(FromAddrString, 0, INET6_ADDRSTRLEN);
				inet_ntop(AF_INET6, &PeerName.sin6_addr, FromAddrString, INET6_ADDRSTRLEN);
			}
			if (IN6_IS_ADDR_V4MAPPED(&SockName.sin6_addr))
			{
#ifdef _MSC_VER
				inet_ntop(AF_INET, SockName.sin6_addr.s6_addr + 12, ToAddrString,
					INET6_ADDRSTRLEN);
#else
                inet_ntop(AF_INET, &SockName.sin6_addr.s6_addr32[3], ToAddrString,
					INET6_ADDRSTRLEN);
#endif
			}
			else
			{
				memset(ToAddrString, 0, INET6_ADDRSTRLEN);
				inet_ntop(AF_INET6, &SockName.sin6_addr, ToAddrString, INET6_ADDRSTRLEN);
			}

			cout << "Connection accepted : "
				<< ToAddrString << "-" << ntohs(SockName.sin6_port)
				<< " <--> "
				<< FromAddrString << "-" << ntohs(PeerName.sin6_port)
				<< endl;
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
						Sock->send(Buf, static_cast<size_t>(Res));
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
#endif
}
