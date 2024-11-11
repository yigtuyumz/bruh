// NOLINTBEGIN clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling

// ping uses the ICMP protocols mandatory ECHO REQUEST datagram to elicit an
// ICMP ECHO RESPONSE from a host or gateway. ECHO REQUEST datagrams (pings) have
// an IP and ICMP header, followed by a struct timeval and then an arbitrary
// number of pad bytes used to fill out the packet.

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>         // AF_INET, SOCK_RAW
#include <arpa/inet.h>          // IPPROTO_ICMP, inet_ntoa()
#include <string.h>             // memset()
#include <netinet/ip_icmp.h>    // struct icmp
#include <netinet/in.h>         // struct sockaddr_in
#include <netinet/ip.h>         // struct iphdr
#include <unistd.h>             // close()

#undef __USE_XOPEN2K
#define __USE_XOPEN2K
#include <netdb.h>          // struct addrinfo
#undef __USE_XOPEN2K

unsigned short checksum(unsigned short *addr, int len);

int
main(int argc, char *argv[])
{
    // check args
    if (argc < 2) {
        fprintf(stderr, "Usage: pink <ip_address>\n");
        exit(EXIT_FAILURE);
    }

    int sockfd;

    // get socket info
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        fprintf(stderr, "Cannot create raw socket. You must be root.\n");
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME;      /* CNAME */
    hints.ai_socktype = 0;

    int n;

    if ((n = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
        fprintf(stderr, "Error in getaddrinfo()\n");
        exit(EXIT_FAILURE);
    }

    /* fill in ICMP header info */
    unsigned char buffer[1024];
    struct icmphdr *icmp;
    int len;

    icmp = (struct icmphdr *) buffer;
    icmp->type = ICMP_ECHO;
    icmp->code = 0;
    icmp->un.echo.id = 0;
    icmp->un.echo.sequence = 0;
    len = 8;
    icmp->checksum = 0;
    icmp->checksum = checksum((unsigned short *) icmp, len);

    while (1) {
    /* send the packet */
    sendto(sockfd, buffer, len, 0, res->ai_addr, res->ai_addrlen);

    /* wait for reply packet and set timeout */
    struct sockaddr_in raddr;
    int ip_len;

    memset(buffer, 0, 1024);
    n = sizeof(struct sockaddr_in);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *) &raddr, &n);

    /* print results to screen */
    struct iphdr *ip;
    ip = (struct iphdr *) buffer;
    ip_len = ip->ihl << 2;
    icmp = (struct icmphdr *) (ip_len + buffer);
    if (icmp->type == ICMP_ECHOREPLY) {
        printf("Alive. %s\n", argv[1]);
    } else {
        fprintf(stderr, "%s is not responding.\n", argv[1]);
    }

    printf("message recieved from %s\n", inet_ntoa(raddr.sin_addr));
    }

    close(sockfd);

    return (0);
}

// add seqential 16 bit words to the accumulator (sum), at the end, fold back
// all the carry bits from the top 16 bits into the lower 16 bits.
unsigned short
checksum(unsigned short *addr, int len)
{
    int nleft = len;
    int sum = 0;                            // 32bit accumulator
    unsigned short *w = addr;
    unsigned short answer = 0;

    while (nleft > 1) {
        sum += *w++;
        nleft -= 2;                         // 1 word is 2 bytes long.
    }

    // odd byte, if neccessary.
    if (nleft == 1) {
        *((unsigned char *) (&answer)) = *((unsigned char *) w);
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);     // add high 16 to low 16
    sum += (sum >> 16);                     // add carry

    answer = ~(sum);                        // truncate to 16 bits

    return (answer);
}


// NOLINTEND clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling
