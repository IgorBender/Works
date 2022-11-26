#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/capability.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#define PACKETSIZE  64
struct packet
{
    struct icmphdr hdr;
    char msg[PACKETSIZE - sizeof(struct icmphdr)];
};

//static int pid = -1;
//static uint16_t cnt = 1;

/*--------------------------------------------------------------------*/
/*--- checksum - standard 1s complement checksum                   ---*/
/*--------------------------------------------------------------------*/
unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*) buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~(sum & 0xFFFF);
    return result;
}

/*--------------------------------------------------------------------*/
/*--- ping - Create message and send it.                           ---*/
/*    return 0 if ping OK, return 1 if ping not OK.                ---*/
/*--------------------------------------------------------------------*/
int ping(char *adress)
{
    const int val = 255;
    int8_t i;
    int sd;
    struct packet pckt;
    struct sockaddr_in r_addr;
    int loop;
    struct sockaddr_in addr_ping;
    int pid = -1;
    uint16_t cnt = 1;

    pid = getpid();
    memset(&addr_ping, 0, sizeof(addr_ping));
    addr_ping.sin_family = AF_INET;
    addr_ping.sin_port = 0;
    addr_ping.sin_addr.s_addr = inet_addr(adress);

    sd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(sd < 0)
    {
        perror("socket");
        return 1;
    }
    if(setsockopt(sd, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
    {
        perror("Set TTL option");
        return 1;
    }
    if(fcntl(sd, F_SETFL, O_NONBLOCK) != 0)
    {
        perror("Request nonblocking I/O");
        return 1;
    }

    for(loop = 0; loop < 10; loop++)
    {
        socklen_t len = sizeof(r_addr);

        if(recvfrom(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*) &r_addr,
                &len) > 0)
        {
            return 0;
        }

        memset(&pckt, 0, sizeof(pckt));
        pckt.hdr.type = ICMP_ECHO;
        pckt.hdr.un.echo.id = (uint16_t)pid;
        for(i = 0; i < (int8_t)sizeof(pckt.msg) - 1; i++)
            pckt.msg[i] = (int8_t)i + '0';
        pckt.msg[i] = 0;
        pckt.hdr.un.echo.sequence = cnt++;
        pckt.hdr.checksum = checksum(&pckt, sizeof(pckt));
        if(sendto(sd, &pckt, sizeof(pckt), 0, (struct sockaddr*) &addr_ping,
                sizeof(addr_ping)) <= 0)
            perror("sendto");

        usleep(300000);
    }

    return 1;
}

int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        if(ping(argv[1]))
            printf("Ping is not OK. \n");
        else
            printf("Ping is OK. \n");
    }
    else
    {
        if(ping("192.168.1.24"))
            printf("Ping is not OK. \n");
        else
            printf("Ping is OK. \n");
    }
    return 0;
}

