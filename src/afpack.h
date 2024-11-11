#ifndef AFPACK_H
# define AFPACK_H
# include <stdint.h>        /* for unsigned variable length data types. */
# include <stdio.h>
# include <netinet/in.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <signal.h>
# undef __USE_MISC      // prevent re-definition of __USE_MISC
# define __USE_MISC     // struct ifreq requires __USE_MISC
# include <net/if.h>
# undef __USE_MISC      // prevent other things (libraries etc.) use __USE_MISC too.

/*
//~ test.... silinecek....
    # define ALIGNMENT 32
    # define ALIGN(size) ((( size) + ((ALIGNMENT) - 1)) & (~((ALIGNMENT) - 1)))
*/

typedef enum ARP_OPCODE_E {
    // 0 is RESERVED
    REQUEST = 1,
    REPLY = 2,
    REQUEST_RESERVE = 3,
    REPLY_RESERVE = 4,
    DRARP_REQUEST = 5,
    DRARP_REPLY = 6,
    DRARP_ERROR = 7,
    INARP_REQUEST = 8,
    INARP_REPLY = 9,
    ARP_NAK = 10,
    MARS_REQUEST = 11,
    MARS_MULTI = 12,
    MARS_MSERV = 13,
    MARS_JOIN = 14,
    MARS_LEAVE = 15,
    MARS_NAK = 16,
    MARS_UNSERV = 17,
    MARS_SJOIN = 18,
    MARS_SLEAVE = 19,
    MARS_GROUPLIST_REQUEST = 20,
    MARS_GROUPLIST_REPLY = 21,
    MARS_REDIRECT_MAP = 22

    //~ custom operation codes here...

    // 65535 is RESERVED

} ARP_OPCODE;

/*
sockaddr: (generic)
    Temel yapi; diger sockaddr turleri icin genel bir cati saglar.
sockaddr_ll: (link layer)
    Dusuk seviyeli (Layer 2) ag paketleri icin kullanilir.
    Ethernet gibi veri baglantisi katmanindaki protokollerle iliskilidir.
sockaddr_un: (unix)
    Yerel soketler (Unix domain sockets) icin kullanilir.
    Uygulama katmani arasinda iletisim saglar (Layer 7).
sockaddr_in: (internet v4)
    IPv4 adresleri icin kullanilir. Tasima katmanindaki
    (Transport Layer - Layer 4) TCP ve UDP gibi protokollerle iliskilidir.
sockaddr_in6: (internet v6)
    IPv6 adresleri icin kullanilir. Tasima katmani ve ag katmanindaki (Layer 3)
    protokollerle iliskilidir.
sockaddr_storage: 
    Farkli adres aileleri icin yeterli alan saglayan bir yapidir. Hem IPv4
    hem de IPv6 adreslerini saklamak icin kullanilir.
sockaddr_nl: (netlink)
    Linux netlink soketleri icin kullanilir.
    Uygulama ve ag katmani etkilesimlerini yonetir.
sockaddr_rc: (RF communications)
    Bluetooth RFCOMM (RF Communications) protokolu icin kullanilir.
    Tasima katmaninda islev gorur.
*/




// ILNPv4, "Identifier-Locator Network Protocol version 4" 


// Ethernet II Frame Header, 14 Bytes (ETH_HLEN)
// TODO WHAT THE FUCK IS `LG` and `IG` BIT?!?!?!!!!
typedef struct eth2_s {
    uint8_t dest[6];        /* Target MAC Address 6 Bytes, ETH_ALEN */
    uint8_t source[6];      /* Sender MAC Address 6 Bytes, ETH_ALEN */
    uint16_t type;          /* EtherType */
} __attribute__((packed)) eth2_t;


// Address Resolution Protocol (ARP) Frame Header, 28 Bytes
typedef struct arp_s {
    uint16_t hw_type;       /* Hardware Type */
    uint16_t proto_type;    /* Protocol Type */
    uint8_t hw_size;        /* Hardware Address Length */
    uint8_t proto_size;     /* Protocol Address Length */
    uint16_t opcode;        /* Operation Code */
    uint8_t sender_mac[6];  /* Sender Hardware Address */
    uint32_t sender_ipv4;   /* Sender L32 (same as Sender IPv4 address for ARP) */
    uint8_t target_mac[6];  /* Target Hardware Address */
    uint32_t target_ipv4;   /* Target L32 */
} __attribute__((packed)) arp_t;


typedef struct ipv4_header_s {
    union {
        struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            uint8_t ihl: 4;
            uint8_t version: 4;
#else
            uint8_t version: 4;
            uint8_t ihl: 4;
#endif  /* __BYTE_ORDER__ */
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
// RFC 791 says:
// The internet header padding is used to ensure that the internet
// header ends on a 32 bit boundary. The padding is zero.

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-variable"

static int afpack_socket;
// many network systems recognize `ff:ff:ff:ff:ff:ff` as the broadcast address.
static unsigned char MAC_INTERFACE[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

# pragma GCC diagnostic pop


int afpack_status(void);
void handle_sigint(int signum __attribute__((unused)));
void listen_signals(void);
int get_mac(const char *interface);     // acquires the MAC address of the interface, and stores it in MAC_INTERFACE.
void print_mac(void);

#endif /* AFPACK_H */

