/**
 * @file tal_memory.h
 * @brief This is tal_memory file
 *
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_MEMORY_H__
#define __TAL_MEMORY_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/
#define Malloc(req_size) tal_malloc(req_size)

#define Calloc(req_count, req_size) tal_calloc(req_count, req_size)

#define Free(ptr) tal_free(ptr)

/***********************************************************************
 ********************* struct ******************************************
 **********************************************************************/


/***********************************************************************
 ********************* variable ****************************************
 **********************************************************************/


/***********************************************************************
 ********************* function ****************************************
 **********************************************************************/

/**
 * @brief This API is used to alloc memory of system.
 *
 * @param[in] size: memory size
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T *tal_malloc(SIZE_T size);

/**
 * @brief This API is used to free memory of system.
 *
 * @param[in] ptr: memory point
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tal_free(VOID_T* ptr);

/**
 * @brief Allocate and clear the memory
 *
 * @param[in]       nitems      the numbers of memory block
 * @param[in]       size        the size of the memory block
 *
 * @return the memory address calloced
 */
VOID_T *tal_calloc(SIZE_T nitems, SIZE_T size);

/**
 * @brief Re-allocate the memory
 *
 * @param[in]       nitems      source memory address
 * @param[in]       size        the size after re-allocate
 *
 * @return VOID_T
 */
VOID_T *tal_realloc(VOID_T* ptr, SIZE_T size);

/**
 * @brief Get system free heap size
 *
 * @param[in] param: none
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT32_T tal_system_get_free_heap_size(VOID_T);


#ifdef __cplusplus
}
#endif

#endif /* __TAL_MEMORY_H__ */

