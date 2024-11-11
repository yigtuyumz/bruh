#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#undef __USE_MISC
#define __USE_MISC
#include <net/if.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>

unsigned char mac_addr[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int
get_mac_address(const char *ip_address, const char *interface_name, unsigned char *mac_addr)
{
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in sa;
    struct arpreq arp_req;

    // Socket oluşturuluyor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);        // UDP
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    // IP adresini struct sockaddr_in'e yerleştir
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip_address);

    // ARP sorgusunun yapılacağı arayüzü ayarla
    memset(&arp_req, 0, sizeof(struct arpreq));
    strncpy(arp_req.arp_dev, interface_name, IFNAMSIZ);

    // ARP tablosundan IP'ye karşılık gelen MAC adresini al
    memcpy(&arp_req.arp_pa, &sa, sizeof(struct sockaddr_in));

    if (ioctl(sockfd, SIOCGARP, &arp_req) == -1) {
        perror("ARP lookup failed");
        close(sockfd);
        return -1;
    }

    // MAC adresini elde et
    memcpy(mac_addr, arp_req.arp_ha.sa_data, 6);

    close(sockfd);
    return 0;
}

int main()
{

    const char *ip_addr = "192.168.1.187";
    if (get_mac_address(ip_addr, "wlp3s0", mac_addr) == 0) {
        printf("MAC Address for IP %s: ", ip_addr);
        for (int i = 0; i < 6; i++) {
            printf("%02x", mac_addr[i]);
            if (i < 5) printf(":");
        }
        printf("\n");
    } else {
        printf("Failed to get MAC address\n");
    }

    return 0;
}
