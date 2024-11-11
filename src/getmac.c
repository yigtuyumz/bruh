#include "afpack.h"

int
get_mac(const char *interface)
{
    // MAC_INTERFACE is set to broadcast by default.
    if (strlen(interface) == 2 && strncmp(interface, "lo", 2) == 0) {
        printf("searching lo\n");
        if (strncmp((const char *) MAC_INTERFACE, "\0\0\0\0\0\0", 6) == 0) {
            printf("buffer already broadcast.\n");
            return (0);
        }
    }

    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);        // UDP socket
    
    if (sockfd < 0) {
        fprintf(stderr, "get_mac > socket creation failed.\n");
        return (1);
    }

    strncpy(ifr.ifr_ifrn.ifrn_name, interface, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        fprintf(stderr, "get_mac > ioctl failed.\n");
        return (1);
    }

    memcpy(MAC_INTERFACE, ifr.ifr_ifru.ifru_hwaddr.sa_data, 6);

    close(sockfd);

    return (0);
}

void
print_mac(void)
{
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
        (unsigned char) MAC_INTERFACE[0],
        (unsigned char) MAC_INTERFACE[1],
        (unsigned char) MAC_INTERFACE[2],
        (unsigned char) MAC_INTERFACE[3],
        (unsigned char) MAC_INTERFACE[4],
        (unsigned char) MAC_INTERFACE[5]
    );
}