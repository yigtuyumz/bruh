#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>

typedef struct ipv4_header_s {
    union {
        struct {
        uint8_t version: 4;
        uint8_t ihl: 4;
        } version_and_length;
        uint8_t version_and_length_combined;
    } version_and_length_union;
    uint8_t type_of_service;
    uint16_t total_length;
    uint16_t identification;
    union {
        struct {
            uint16_t flags: 3;
            uint16_t fragment_offset: 13;
        };
        uint16_t flags_and_fragment_offset_combined;
    };
    uint8_t time_to_live;
    uint8_t protocol;
    uint16_t header_checksum;
    uint32_t source_address;
    uint32_t destination_address;
    const uint8_t *options;
} __attribute__((aligned (4))) ipv4_header_t;

// Checksums (calculate checksum for IPv4 header)
unsigned short checksum(void *b, int len) {
    return (0xffff);
}

int main() {
    int sock;
    struct sockaddr_in dest;
    ipv4_header_t ip_header;

    // Create a raw socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Set destination address (example: sending to 8.8.8.8)
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = 0;
    dest.sin_addr.s_addr = inet_addr("8.8.8.8");

    // Initialize the IPv4 header
    memset(&ip_header, 0, sizeof(ip_header));

    // Set the version and IHL (Header Length)
    ip_header.version_and_length_union.version_and_length_combined = 0x45; // Version: 4, IHL: 5 (5 * 4 = 20 bytes)

    // Type of service (TOS)
    ip_header.type_of_service = 0;

    // Total Length (Header + Data length)
    ip_header.total_length = htons(sizeof(ip_header) + 0);  // No data part for now

    // Identification (Random value)
    ip_header.identification = 0xff;

    // Flags (Don't fragment flag, offset 0)
    ip_header.flags_and_fragment_offset_combined = htons(0x4000);  // Don't fragment

    // Time To Live (TTL)
    ip_header.time_to_live = 70;

    // Protocol (ICMP, TCP, UDP, etc. In this case, ICMP)
    ip_header.protocol = IPPROTO_ICMP;

    // Header Checksum (set to 0 initially for checksum calculation)
    ip_header.header_checksum = 0;

    // Source IP address (example: 192.168.1.1)
    ip_header.source_address = inet_addr("192.168.1.1");

    // Destination IP address (already set above)
    ip_header.destination_address = dest.sin_addr.s_addr;

    // Calculate the checksum for the IP header
    ip_header.header_checksum = checksum(&ip_header, sizeof(ip_header));

    // Send the raw packet
    if (sendto(sock, &ip_header, sizeof(ip_header), 0, (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("Send failed");
        return 1;
    }

    printf("IP Header sent successfully.\n");

    close(sock);
    return 0;
}
