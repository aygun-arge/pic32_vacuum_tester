
#include "checksum/checksum.h"

uint8_t checksumParity8(const void * buffer, size_t size) {
    uint8_t             sum;
    size_t              byte;

    sum = 0;

    for (byte = 0u; byte < size; byte++) {
        sum += *(uint8_t *)buffer;
    }

    return (((sum ^ 0xffu) + 1u) & 0xffu);
}
