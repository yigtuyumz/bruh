#include "afpack.h"

int
afpack_status(void)
{
    int result;

    socklen_t optlen = sizeof(int);
    int sock_type;

    result = getsockopt(afpack_socket, SOL_SOCKET, SO_TYPE, &sock_type, &optlen);


    if (result < 0) {
        printf("Afpack socket is not running.\n");
        return (1);
    }

    printf("Afpack socket is running.\n");
    return (0);
}
