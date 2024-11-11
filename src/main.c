#include "afpack.h"


int
main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    print_mac();
    get_mac("wlp3s0");
    print_mac();
    get_mac("enp2s20");
    print_mac();
    ipv4_header_t h;


    h.version_and_length_union.version_and_length.version = 0b1100;
    h.version_and_length_union.version_and_length.ihl = 0b0011;
    printf("%d\n", h.version_and_length_union.version_and_length_combined);



    // ipheader.ip_version_union.ip_version_and_ihl.ihl = 0b1111;
    // ipheader.ip_version_union.ip_version_and_ihl.version = 0b1111;
    // ipheader.ip_version_union.version_and_ihl = ipheader.ip_version_union.ip_version_and_ihl.ihl << 4 | ipheader.ip_version_union.ip_version_and_ihl.version;
    // printf("%d\n", ipheader.ip_version_union.version_and_ihl);
    printf("sizeof(ipv4_t): %lu\n", sizeof(ipv4_header_t));
    return (0);
}