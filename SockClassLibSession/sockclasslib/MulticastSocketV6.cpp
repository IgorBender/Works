/* MulticastSocketV6.cpp
 *
 *
 * Original code by Igor Bender
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 * not claim that you wrote the original software. If you use this
 * software in a product, an acknowledgment in the product documentation
 * would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 */

#ifndef _NO_IPV6

#include "MulticastSocketV6.h"

#ifdef _WIN32
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#if (__linux__)
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>
#include <sys/ioctl.h>
#endif
#endif

MulticastSocketV6::~MulticastSocketV6()
{}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocketV6::setTTL(unsigned char ttl)
{
	unsigned int Hops = static_cast<unsigned int>(ttl);
    setIpLevelOpt(IPV6_MULTICAST_HOPS,
#ifndef __VXWORKS__
                  reinterpret_cast < const char* > (&Hops),
#else
                  reinterpret_cast < char* > (&Hops),
#endif
                  sizeof(Hops));
}
#else
int MulticastSocketV6::setTTL(unsigned char ttl)
{
    int Result = setIpLevelOpt(IPV6_MULTICAST_HOPS,
#ifndef __VXWORKS__
                            reinterpret_cast < const char* > (&ttl),
#else
                            reinterpret_cast < char* > (&ttl),
#endif
                            sizeof(ttl));
    return Result;
}
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocketV6::setDefaultIf(in6_addr Address)
#else
int MulticastSocketV6::setDefaultIf(in6_addr Address)
#endif
{
    // Resolve index by address
#if (__linux__)
    ifreq Request;
    memset(&Request, 0, sizeof(Request));

    ifaddrs* pAddrs;
    int Result = getifaddrs(&pAddrs);
    if(0 > Result)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Error in getting addresses.");
#else
        return SOCKET_ERROR;
#endif
    }
    ifaddrs* pTempAddr;
    pTempAddr = pAddrs;
    while(nullptr != pTempAddr)
    {
    	if(AF_INET6 == pTempAddr->ifa_addr->sa_family)
    	{
    		sockaddr_in6* pIpV6Addr = reinterpret_cast<sockaddr_in6*>(pTempAddr->ifa_addr);
    		in6_addr TmpAddr;
    		if(memcmp(&Address, pIpV6Addr->sin6_addr.s6_addr, sizeof(TmpAddr)))
    		{
    	    	pTempAddr = pTempAddr->ifa_next;
    			continue;
    		}
    		strncpy(Request.ifr_name, pTempAddr->ifa_name, sizeof(Request.ifr_name));
    		Request.ifr_name[sizeof(Request.ifr_name)-1] = '\0';
    	    freeifaddrs(pAddrs);
    	    break;
    	}
    	pTempAddr = pTempAddr->ifa_next;
    }
    if(nullptr == pTempAddr)
    {
    	freeifaddrs(pAddrs);
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    	SOCK_EXCEPT_THROW("Wrong address.");
#else
        return SOCKET_ERROR;
#endif
    }
    Result = ioctl(m_Sock, SIOCGIFINDEX, &Request);
    if(0 > Result)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    	SOCK_EXCEPT_THROW("ioctl error.");
#else
        return SOCKET_ERROR;
#endif
    }
    m_Index = static_cast<uint32_t>(Request.ifr_ifindex);
#endif
#ifdef _WIN32
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	unsigned long OutBufLen = 16 * 1024;
	const int MAX_TRIES = 3;
	int Iterations = 0;
	unsigned int RetVal;
	do {

		pAddresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new char[OutBufLen]);
		if (pAddresses == NULL) {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
			SOCK_EXCEPT_THROW("Memory allocation failed for IP_ADAPTER_ADDRESSES struct.");
#else
			return SOCKET_ERROR;
#endif
		}
		RetVal = GetAdaptersAddresses(AF_INET6,
			GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_FRIENDLY_NAME, NULL,
			pAddresses, &OutBufLen);

		if (RetVal == ERROR_BUFFER_OVERFLOW) {
			delete [] pAddresses;
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;
	} while ((RetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (RetVal == NO_ERROR)
	{
		IP_ADAPTER_ADDRESSES* pCurrAddresses = pAddresses;
		while(pCurrAddresses)
		{
			PIP_ADAPTER_UNICAST_ADDRESS_LH pUnicast = pCurrAddresses->FirstUnicastAddress;
			while(pUnicast != NULL)
			{
				sockaddr_in6* pTmp = reinterpret_cast<sockaddr_in6*>(pUnicast->Address.lpSockaddr);
				if (memcmp(&Address.s6_addr, pTmp->sin6_addr.s6_addr, sizeof(Address)))
				{
					pUnicast = pUnicast->Next;
					continue;
				}
				else
				{
					break;
				}
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
#ifndef _WITHOUT_SOCK_EXCEPTIONS
			SOCK_EXCEPT_THROW("Wrong address.");
#else
			return SOCKET_ERROR;
#endif
		}
		m_Index = pCurrAddresses->Ipv6IfIndex;
	}
	else
	{
#ifndef _WITHOUT_SOCK_EXCEPTIONS
		SOCK_EXCEPT_THROW("Wrong address.");
#else
		return SOCKET_ERROR;
#endif
	}
#endif

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    setDefaultIf(m_Index);
#else
    return setDefaultIf(m_Index);
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void  MulticastSocketV6::setDefaultIf(const char* Address)
#else
int  MulticastSocketV6::setDefaultIf(const char* Address)
#endif
{
#ifndef _NO_IPV6
	in6_addr AddressStruct;
	int ConvertResult = inet_pton(INET6_DOMAIN, Address, &AddressStruct);
    if(0 >= ConvertResult)
    {
#ifndef _WITHOUT_SOCK_EXCEPTIONS
        SOCK_EXCEPT_THROW("Wrong address");
#else
        return SOCKET_ERROR;
#endif
    }

#ifndef _WITHOUT_SOCK_EXCEPTIONS
    setDefaultIf(static_cast<uint32_t>(ConvertResult));
#else
    return setDefaultIf(static_cast<uint32_t>(ConvertResult));
#endif
#else
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    SOCK_EXCEPT_THROW("No IPv6 support defined");
#else
    return SOCKET_ERROR;
#endif
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocketV6::setDefaultIf(unsigned int Index)
#else
int MulticastSocketV6::setDefaultIf(unsigned int Index)
#endif
{
    m_Index = Index;
#ifndef _WITHOUT_SOCK_EXCEPTIONS
    setIpLevelOpt(IPV6_MULTICAST_IF,
#else
    int SetResult =  setIpLevelOpt(IPV6_MULTICAST_IF,
#endif
#ifndef __VXWORKS__
                  reinterpret_cast < const char* > (&m_Index),
#else
                  reinterpret_cast < char* > (&m_Index),
#endif
                  sizeof m_Index);

#ifdef _WITHOUT_SOCK_EXCEPTIONS
    return SetResult;
#endif
}

#ifndef _WITHOUT_SOCK_EXCEPTIONS
void MulticastSocketV6::setLoopBack(bool On)
{
    unsigned int Loop = On ? 1 : 0;
    setIpLevelOpt(IPV6_MULTICAST_LOOP,
#ifndef __VXWORKS__
                  reinterpret_cast < const char* > (&Loop),
#else
                  reinterpret_cast < char* > (&Loop),
#endif
                  sizeof Loop);
}
#else
int MulticastSocketV6::setLoopBack(bool On)
{
    unsigned int Loop = On ? 1 : 0;
	int iResult = setIpLevelOpt(IPV6_MULTICAST_LOOP,
#ifndef __VXWORKS__
		reinterpret_cast <const char*> (&Loop),
#else
		reinterpret_cast < char* > (&Loop),
#endif
		sizeof Loop);
    return iResult;
}
#endif

#endif /* _NO_IPV6 */
