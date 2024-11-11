#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Get the IPv4 address
    char *ip_address = argv[1];

    // check the format of given IPv4 address
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0) {
        fprintf(stderr, "Invalid IP address: %s\n", ip_address);
        exit(EXIT_FAILURE);
    }

    // get hostname from IPv$ address
    struct hostent *he;
    he = gethostbyaddr(&(sa.sin_addr), sizeof(sa.sin_addr), AF_INET);

    if (he == NULL) {
        perror("gethostbyaddr");
        exit(EXIT_FAILURE);
    }

    printf("Hostname: %s\n", he->h_name);

    return 0;
}
