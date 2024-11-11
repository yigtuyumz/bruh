#include <stdint.h>

uint32_t
get_network_addr(uint32_t host_addr, uint32_t netmask)
{
    return (host_addr & netmask);
}
