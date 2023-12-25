#include "tuya_tls.h"
#include "stdlib.h"

int uni_random_bytes(unsigned char *output, size_t output_len)
{
    return tuya_tls_random(output, output_len);
}

int uni_random_int(void)
{
    unsigned int output;
    uni_random_bytes((unsigned char *)&output, sizeof(output));
    return output & RAND_MAX;
}

int uni_random_range(unsigned int range)
{
    if (range == 0) {
        range = 0xff;
    }

    return uni_random_int() % range;
}

uint32_t uni_random(void)
{
    uint32_t output;
    uni_random_bytes((unsigned char *)&output, sizeof(output));
    return output;
}

int uni_random_string(char* dst, int size)
{
    int i = 0;
    char const* seed = "0123456789abcdef";

    for (i = 0; i < size; ++i) {
        dst[i] = seed[uni_random_range(16)];
    }

    return 0;
}
