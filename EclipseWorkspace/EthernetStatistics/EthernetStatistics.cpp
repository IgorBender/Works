/*
 *  EthernetStatistics.cpp
 *
 *  Created on: 2017-04-28
 *  Author: Igor Bender
 *
 */
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/ethernet.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/if_link.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>

using namespace std;

int main(int argc, char *argv[])
{
    struct InterfaceInfo
    {
        bool Loppback = false;
        bool IPv4 = false;
        bool IPv6 = false;
        bool Statistics = false;
        list<in_addr> Addresses;
        list<in6_addr> AddressesV6;
        uint32_t TxErrors = 0;
        uint32_t TxBytes = 0;
        uint32_t RxErrors = 0;
        uint32_t RxBytes = 0;
        uint8_t PhysAddress[ETHER_ADDR_LEN];
        InterfaceInfo()
        {
            bzero(PhysAddress, sizeof(PhysAddress));
        }
    };

    map<string, InterfaceInfo> IfDb;

    struct ifaddrs *ifaddr, *ifa;
    int n;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    // Walk through link list and find packet interface with same name
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        switch(ifa->ifa_addr->sa_family)
        {
        case AF_INET:
        {
            IfDb[ifa->ifa_name].IPv4 = true;
            IfDb[ifa->ifa_name].Loppback = (ifa->ifa_flags & IFF_LOOPBACK) ? true : false;
            sockaddr_in* pSockAddr = reinterpret_cast<sockaddr_in*>(ifa->ifa_addr);
            IfDb[ifa->ifa_name].Addresses.push_back(pSockAddr->sin_addr);
        }
            break;

        case AF_INET6:
        {
            IfDb[ifa->ifa_name].IPv6 = true;
            IfDb[ifa->ifa_name].Loppback = (ifa->ifa_flags & IFF_LOOPBACK) ? true : false;
            sockaddr_in6* pSockAddr = reinterpret_cast<sockaddr_in6*>(ifa->ifa_addr);
            IfDb[ifa->ifa_name].AddressesV6.push_back(pSockAddr->sin6_addr);
        }
            break;

        case AF_PACKET:
            IfDb[ifa->ifa_name].Statistics = true;
            IfDb[ifa->ifa_name].TxErrors = reinterpret_cast<rtnl_link_stats *>(ifa->ifa_data)->tx_errors;
            IfDb[ifa->ifa_name].TxBytes = reinterpret_cast<rtnl_link_stats *>(ifa->ifa_data)->tx_bytes;
            IfDb[ifa->ifa_name].RxErrors = reinterpret_cast<rtnl_link_stats *>(ifa->ifa_data)->rx_errors;
            IfDb[ifa->ifa_name].RxBytes = reinterpret_cast<rtnl_link_stats *>(ifa->ifa_data)->rx_bytes;
            break;

        default:
            ;
        }
    }
    freeifaddrs(ifaddr);

    // Walk through interfaces database and obtain hardware addresses of appropriate interfaces.
    int Socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(0 > Socket)
    {
        return 1;
    }
    for(auto Element : IfDb)
    {
        ifreq Request;
        strcpy(Request.ifr_name, Element.first.c_str());
        if(ioctl(Socket, SIOCGIFHWADDR, &Request) < 0)
        {
            continue;
        }
        memcpy(IfDb[Element.first].PhysAddress, Request.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
    }
    close(Socket);

    char AddressString[INET6_ADDRSTRLEN];
    bzero(AddressString, INET6_ADDRSTRLEN);
    for(auto Element : IfDb)
    {
        cout << Element.first << endl;
        if(Element.second.IPv4)
        {
            for(auto Address : Element.second.Addresses)
            {
                inet_ntop(AF_INET, &Address, AddressString, INET6_ADDRSTRLEN);
                cout << "\t" << "IPv4 address : " << AddressString << endl;
            }
        }
        if(Element.second.IPv6)
        {
            for(auto AddressV6 : Element.second.AddressesV6)
            {
                inet_ntop(AF_INET6, &AddressV6, AddressString, INET6_ADDRSTRLEN);
                cout << "\t" << "IPv6 address : " << AddressString << endl;
            }
        }
        if(!Element.second.Loppback)
        {
            cout << "\tPysical address : ";
            uint32_t Counter = 0;
            for(auto c : Element.second.PhysAddress)
            {
                cout << hex << setw(2) << setfill('0') <<  uint16_t(c);
                if(++Counter < ETHER_ADDR_LEN)
                    cout << ":";
            }
            cout << dec << endl;
        }
        if(Element.second.Statistics)
        {
            cout << "\tTx -- Errors: " << Element.second.TxErrors << " Bytes: " << Element.second.TxBytes << endl;
            cout << "\tRx -- Errors: " << Element.second.RxErrors << " Bytes: " << Element.second.RxBytes << endl;
        }
    }
    return 0;
}
