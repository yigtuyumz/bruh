#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#undef __USE_MISC
#define __USE_MISC
#include <net/if.h>
#undef __USE_MISC
#include <arpa/inet.h>
#include <sys/socket.h>


const char *
get_mac(const char *interface)
{
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        fprintf(stderr, "Socket creation failed.\n");
        return (NULL);
    }

    // IF_NAMESIZE = 16         maximum length of an interface name
    strncpy(ifr.ifr_ifrn.ifrn_name, interface, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        fprintf(stderr, "Error while retrieving the MAC address of interface `%s`\n", interface);
        return (NULL);
    }

    char *ret = (char *) malloc(sizeof(char) * 6);

    memcpy(ret, ifr.ifr_ifru.ifru_hwaddr.sa_data, 6);

    close(sockfd);


    return (ret);
}

void
print_mac(const char *mac)
{
    if (mac != NULL) {
        printf("%s\t%X:%X:%X:%X:%X:%X\n",
        mac,
            (unsigned char) mac[0],
            (unsigned char) mac[1],
            (unsigned char) mac[2],
            (unsigned char) mac[3],
            (unsigned char) mac[4],
            (unsigned char) mac[5]
        );
    } else {
        fprintf(stderr, "Given MAC is NULL\n");
    }
}

int
main(int argc, char *argv[])
{
    if (argv[1] == NULL) {
        fprintf(stderr, "argv[1] -> interface name\n");
        return (1);
    }
    const char *mac = get_mac(argv[1]);

    print_mac(mac);

    free((void *) mac);
    return (0);
}