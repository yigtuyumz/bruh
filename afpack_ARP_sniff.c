//! ./afpack needs sudo privileges to run.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> // Ethernet header
#include <net/if.h>
#include <signal.h>

#define ALIGNMENT 8     /* must be power of 2 */
#define ALIGN(size) ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

//~ htonX -> little to big endian (X indicates the unit size [s short 16bits | l long 32bits])
//~ ntohX -> big to little endian (X indicates the unit size [s short 16bits | l long 32bits])

int sockfd;     //~ globally declared, because of signal handler mechanism.

void
handle_sigint(int signum)
{
    printf("\rSIGINT recieved. Socket is closing...\n");
    close(sockfd);
    exit(EXIT_SUCCESS);
}

int
main(int argc, char *argv[])
{
    if (argv[1] == NULL) {
        fprintf(stderr, "argv[1] -> Network interface name\n");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_ll sll;                 //! link layer socket address
    unsigned char buffer[ETH_FRAME_LEN];    //! ETH_FRAME_LEN is the maximum value of ethernet frame can carry on.

    // Catch SIGINT
    signal(SIGINT, handle_sigint);

    // Create Socket
    sockfd = socket(AF_PACKET, SOCK_RAW, 0x0608);     //! htons -> Network systems uses Big Endian.
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set Network Interface
    memset(&sll, 0, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ARP);
    sll.sll_ifindex = if_nametoindex(argv[1]); // Network Interface name goes here..

    if (sll.sll_ifindex == 0) {
        perror("if_nametoindex");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    unsigned int packet_count = 0;

    // Listen Incoming ARP Packets
    while (1) {
        ssize_t num_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (num_bytes < 0) {
            perror("recvfrom");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Process Incoming Packet
        struct ethhdr *eth = (struct ethhdr *)buffer;
        printf("[%05u] Incoming packet <ARP> : %02X:%02X:%02X:%02X:%02X:%02X\n",
                ++packet_count,
                eth->h_source[0], eth->h_source[1], eth->h_source[2],
                eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    }

    //~ The socket can be closed only in a signal handler function due to a running while true loop.

    return 0;
}
