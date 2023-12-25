#ifndef MBEDTLS_THREADING_ALT_H
#define MBEDTLS_THREADING_ALT_H

#include "tal_mutex.h"

typedef struct mbedtls_threading_mutex_t {
    MUTEX_HANDLE mutex;
    char is_valid;
} mbedtls_threading_mutex_t;

#endif /* threading_alt.h */
