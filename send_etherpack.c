#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#undef __USE_MISC
#define __USE_MISC
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include <sys/socket.h>

#include <netpacket/packet.h> // sockaddr_ll

#define DEST_MAC0 0x00
#define DEST_MAC1 0x00
#define DEST_MAC2 0x00
#define DEST_MAC3 0x00
#define DEST_MAC4 0x00
#define DEST_MAC5 0x00

#define SRC_MAC0 0x00
#define SRC_MAC1 0x00
#define SRC_MAC2 0x00
#define SRC_MAC3 0x00
#define SRC_MAC4 0x00
#define SRC_MAC5 0x00

int main() {
    int sockfd;
    struct sockaddr_ll sa;
    char packet[ETH_FRAME_LEN];

    // Ethernet başlıklarını hazırlayın
    struct ethhdr *eth = (struct ethhdr *) packet;

    // Hedef MAC adresi
    eth->h_dest[0] = DEST_MAC0;
    eth->h_dest[1] = DEST_MAC1;
    eth->h_dest[2] = DEST_MAC2;
    eth->h_dest[3] = DEST_MAC3;
    eth->h_dest[4] = DEST_MAC4;
    eth->h_dest[5] = DEST_MAC5;

    // Kaynak MAC adresi
    eth->h_source[0] = SRC_MAC0;
    eth->h_source[1] = SRC_MAC1;
    eth->h_source[2] = SRC_MAC2;
    eth->h_source[3] = SRC_MAC3;
    eth->h_source[4] = SRC_MAC4;
    eth->h_source[5] = SRC_MAC5;

    // Ethernet türü (örneğin, IPv4)
    eth->h_proto = htons(ETH_P_IP);

    // Veri (örneğin, "Hello, World!")
    strcpy(packet + ETH_HLEN, "Hello, World!");

    // Socket oluştur
    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("Socket oluşturulamadı");
        return 1;
    }

    // Ağ arayüzünü bul
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "lo");  // Ethernet arayüzünü belirtin (örneğin "enp2s0")
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
        perror("Arayüz bilgisi alınamadı");
        close(sockfd);
        return 1;
    }

    // Raw socket için gerekli ayarları yap
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_protocol = htons(ETH_P_IP);  // Ethernet protokolü
    sa.sll_ifindex = ifr.ifr_ifindex;   // Arayüz indexi

    // Ethernet paketini gönder
    if (sendto(sockfd, packet, ETH_HLEN + strlen("Hello, World!"), 0, (struct sockaddr *)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("Paket gönderilemedi");
        close(sockfd);
        return 1;
    }

    printf("Ethernet paketi başarıyla gönderildi!\n");

    close(sockfd);
    return 0;
}
