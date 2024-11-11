#include "afpack.h"

void
handle_sigint(int signum __attribute__((unused)))
{
    printf("\rSIGINT recieved. Afpack is closing...\n");
    close(afpack_socket);
    exit(EXIT_SUCCESS);
}

void
listen_signals(void)
{
    signal(SIGINT, handle_sigint);
}
