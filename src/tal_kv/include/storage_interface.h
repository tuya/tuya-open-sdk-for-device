#ifndef __STORAGE_INTERFACE_H_
#define __STORAGE_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The platform specific timer header that defines the Timer struct
 */
#include <stdint.h>
#include <stddef.h>

#define local_storage_set   posix_storage_set
#define local_storage_get   posix_storage_get
#define local_storage_del   posix_storage_del

#ifdef __cplusplus
}
#endif

#endif //__TIMER_INTERFACE_H_
