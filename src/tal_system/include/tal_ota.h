/**
 * @file tal_ota.h
 * @brief Provides OTA (Over-The-Air) update functionalities for Tuya IoT
 * applications.
 *
 * This header file defines the interface for OTA update operations within Tuya
 * IoT applications, including functions for querying OTA capabilities,
 * initiating OTA updates, processing OTA data, and finalizing the OTA process.
 * It supports different types of OTA updates such as full package updates and
 * differential updates, catering to various application and device
 * requirements.
 *
 * The OTA functionalities are essential for enabling remote firmware updates,
 * ensuring that IoT devices can receive the latest features, improvements, and
 * security patches without requiring physical access to the device. This file
 * is part of the Tuya IoT Development Platform and is intended for use in
 * Tuya-based applications.
 *
 * @note This file is subject to the platform's license and copyright terms.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_OTA_H__
#define __TAL_OTA_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/

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
 * @brief This API is used for get chip ota ability diff package upgrade use
 * TUYA_OTA_DIFF
 *
 * @param[in] *image_size: max image size
 * @param[in] *type: full package and compress package upgrade use TUYA_OTA_FULL
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_ota_get_ability(uint32_t *image_size, TUYA_OTA_TYPE_E *type);

/**
 * @brief This API is used for ota start notify
 *
 * @param[in] image_size: image size
 * @param[in] type: ota type
 * @param[in] path: path
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_ota_start_notify(uint32_t image_size, TUYA_OTA_TYPE_E type, TUYA_OTA_PATH_E path);

/**
 * @brief This API is used for ota data process
 *
 * @param[in] *pack: point to ota pack
 * @param[in] remain_len: ota pack remain len
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_ota_data_process(TUYA_OTA_DATA_T *pack, uint32_t *remain_len);

/**
 * @brief This API is used for ota end notify
 *
 * @param[in] reset: ota reset
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_ota_end_notify(BOOL_T reset);

/**
 * @brief This API is used for old firmware info, and only used in resumes
 * transmission at break-points
 *
 * @param[in] **info: **info
 *
 * @return OPRT_OK on success. Others on error, please refer to
 * tuya_error_code.h
 */
OPERATE_RET tal_ota_get_old_firmware_info(TUYA_OTA_FIRMWARE_INFO_T **info);

#ifdef __cplusplus
}
#endif

#endif /* __TAL_OTA_H__ */
