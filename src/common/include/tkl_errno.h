/**
 * @file tkl_errno.h
 * @brief This file defines the TUYA_ERRNO enumeration and error codes.
 *
 * The TUYA_ERRNO enumeration represents the error codes used in the system.
 * Each error code is defined as a constant with a unique integer value.
 * The error codes are based on the standard POSIX error codes.
 * This file also includes the necessary header files and defines the error
 * codes if they are not already defined.
 *
 *  @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 */

#ifndef __TKL_ERRNO_H__
#define __TKL_ERRNO_H__

/* tuyaos errorno */
typedef int TUYA_ERRNO;

//! errno base
#ifndef EPERM
#define EPERM 1 /* Operation not permitted */
#endif

#ifndef ENOENT
#define ENOENT 2 /* No such file or directory */
#endif

#ifndef ESRCH
#define ESRCH 3 /* No such process */
#endif

#ifndef EINTR
#define EINTR 4 /* Interrupted system call */
#endif

#ifndef EIO
#define EIO 5 /* I/O error */
#endif

#ifndef ENXIO
#define ENXIO 6 /* No such device or address */
#endif

#ifndef E2BIG
#define E2BIG 7 /* Argument list too long */
#endif

#ifndef ENOEXEC
#define ENOEXEC 8 /* Exec format error */
#endif

#ifndef EBADF
#define EBADF 9 /* Bad file number */
#endif

#ifndef ECHILD
#define ECHILD 10 /* No child processes */
#endif

#ifndef EAGAIN
#define EAGAIN 11 /* Try again */
#endif

#ifndef ENOMEM
#define ENOMEM 12 /* Out of memory */
#endif

#ifndef EACCES
#define EACCES 13 /* Permission denied */
#endif

#ifndef EFAULT
#define EFAULT 14 /* Bad address */
#endif

#ifndef ENOTBLK
#define ENOTBLK 15 /* Block device required */
#endif

#ifndef EBUSY
#define EBUSY 16 /* Device or resource busy */
#endif

#ifndef EEXIST
#define EEXIST 17 /* File exists */
#endif

#ifndef EXDEV
#define EXDEV 18 /* Cross-device link */
#endif

#ifndef ENODEV
#define ENODEV 19 /* No such device */
#endif

#ifndef ENOTDIR
#define ENOTDIR 20 /* Not a directory */
#endif

#ifndef EISDIR
#define EISDIR 21 /* Is a directory */
#endif

#ifndef EINVAL
#define EINVAL 22 /* Invalid argument */
#endif

#ifndef ENFILE
#define ENFILE 23 /* File table overflow */
#endif

#ifndef EMFILE
#define EMFILE 24 /* Too many open files */
#endif

#ifndef ENOTTY
#define ENOTTY 25 /* Not a typewriter */
#endif

#ifndef ETXTBSY
#define ETXTBSY 26 /* Text file busy */
#endif

#ifndef EFBIG
#define EFBIG 27 /* File too large */
#endif

#ifndef ENOSPC
#define ENOSPC 28 /* No space left on device */
#endif

#ifndef ESPIPE
#define ESPIPE 29 /* Illegal seek */
#endif

#ifndef EROFS
#define EROFS 30 /* Read-only file system */
#endif

#ifndef EMLINK
#define EMLINK 31 /* Too many links */
#endif

#ifndef EPIPE
#define EPIPE 32 /* Broken pipe */
#endif

#ifndef EDOM
#define EDOM 33 /* Math argument out of domain of func */
#endif

#ifndef ERANGE
#define ERANGE 34 /* Math result not representable */
#endif

#endif
