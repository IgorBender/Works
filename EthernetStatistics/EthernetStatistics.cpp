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
#include <iomanip>
#include <set>
using namespace std;

int main(int argc, char *argv[])
{
    struct ifaddrs *ifaddr, *ifa;
    int family, n;

    if(getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    set<string> Names;
    string IfName;

    /* Walk through linked list, maintaining head pointer so we
       can free list later.
       Find interface names and print.
       */
    for(ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;
        if(AF_INET == family)
            Names.insert(string(ifa->ifa_name));
    }

    for(auto& name : Names)
        cout << "\t" << name;
    cout << endl;

    /* Walk through link list second time and find packet interface which is not loop-back */
    for(ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;
        if(family == AF_PACKET && ifa->ifa_data != NULL && "lo" != string(ifa->ifa_name))
        {
            cout << endl << "\t" << string(ifa->ifa_name) << endl;
        	struct rtnl_link_stats *stats = reinterpret_cast<rtnl_link_stats *>(ifa->ifa_data);
            cout << "\ttx_errors = " << setw(10) << stats->tx_errors << "; rx_errors = " << setw(10) << stats->rx_errors << endl
                 << "\ttx_bytes  = " << setw(10) << stats->tx_bytes << "; rx_bytes  = " << setw(10) << stats->rx_bytes << endl << endl;
		}
    }
    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}

