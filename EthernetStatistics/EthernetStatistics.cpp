//#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/if_link.h>

#include <string>
#include <iostream>
#include <set>
using namespace std;

int main(int argc, char *argv[])
{
    struct ifaddrs *ifaddr, *ifa;
    int family, n;
 //   char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    char IpAddress[6];
    IpAddress[0] = 0;
    IpAddress[1] = 0;
    IpAddress[2] = 192;
    IpAddress[3] = 168;
    IpAddress[4] = 1;
    IpAddress[5] = 22;

    set<string> Names;
    string IfName;

    /* Walk through linked list, maintaining head pointer so we
       can free list later.
       Find interface name of an address.
       */
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;
        Names.insert(string(ifa->ifa_name));
        if(AF_INET == family && !memcmp(ifa->ifa_addr->sa_data, IpAddress, 6))
        {
        	IfName = ifa->ifa_name;
        }
    }

    for(auto name : Names)
        cout << "   " << name;
    cout << endl;

    /* Walk through link list second time and find packet interface with same name */
    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;
        if (family == AF_PACKET && ifa->ifa_data != NULL && IfName == string(ifa->ifa_name))
        {
            cout << endl << "\t" << string(ifa->ifa_name) << endl;
        	struct rtnl_link_stats *stats = reinterpret_cast<rtnl_link_stats *>(ifa->ifa_data);
            printf("\ttx_errors = %10u; rx_errors = %10u\n"
                   "\ttx_bytes   = %10u; rx_bytes   = %10u\n",
				   stats->tx_errors, stats->rx_errors,
				   stats->tx_bytes, stats->rx_bytes);
		}
    }
    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}

