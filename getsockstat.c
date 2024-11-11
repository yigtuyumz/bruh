#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>

int
getsockstat(int sockfd)
{
    int s;
    socklen_t optlen = sizeof(int);
    int sock_type;

    s = getsockopt(sockfd, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);
    if (s < 0) {
        printf("Socket is closed.\n");
        return (0);
    }

    printf("Socket is open.\n");
    return (1);
}


int main() {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    int a = getsockstat(s);

    if (a) {
        close(s);
    }

    a = getsockstat(s);

    return 0;
}
