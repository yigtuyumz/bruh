#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>         /* memset() */
#include <stdlib.h>         /* exit() */
#include <stdio.h>          /* fprintf() */

#undef __USE_XOPEN2K
#define __USE_XOPEN2K
#include <netdb.h>
#undef __USE_XOPEN2K


int
main(int argc, char *argv[])
{
    if (argv[1] == NULL) {
        fprintf(stderr, "argv[1] -> IPv4 or IPv6 address\n");
        exit(EXIT_FAILURE);
    }

    if (argv[2] == NULL) {
        fprintf(stderr, "argv[2] -> service port number\n");
        exit(EXIT_FAILURE);
    }

    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        // all (ipv4, ipv6 ...)
    hints.ai_socktype = SOCK_STREAM;    // TCP socket
    // the IP address portion of the socket address structure will be set to
    // INADDR_ANY for an IPv4 address or IN6ADDR_ANY_INIT for an IPv6 address.
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = PF_UNSPEC;

    char *s;
    if (strcmp(argv[1], "NULL") == 0) {
        s = NULL;
    } else {
        s = argv[1];
    }

    // if node == NULL then getaddrinfo only tries to resolve port.
    // in /etc/services every port number is associated with a service name.
    if ((status = getaddrinfo(s, argv[2], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // servinfo now points to a linked list of 1 or more struct addrinfo's

    // we can easily traverse through the `servinfo` to get desired data.
    struct addrinfo *iter = servinfo;
    uint8_t iter_len = 0;
    while (iter->ai_next) {

        iter = iter->ai_next;
        iter_len++;
    }

    printf("got %d address info field(s).\n", iter_len);

    freeaddrinfo(servinfo);     // free the linked-list

    return (iter_len);          // for ./runner script
}
