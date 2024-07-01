/*
 * Copyright (c) 2022, kaans, <https://github.com/kaans>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include <fal.h>

#define FLASH_ERASE_MIN_SIZE (4 * 1024)

#include "tkl_flash.h"

static int init(void)
{

    // the values passed to esp_partition_find_first() must correspond to the
    // values set in partitions.csv for the partition named "flashdb".
    // partition = esp_partition_find_first(0x40, 0x00, "flashdb");

    // assert(partition != NULL);

    TUYA_FLASH_BASE_INFO_T info;

    tkl_flash_get_one_type_info(TUYA_FLASH_TYPE_KV_DATA, &info);

    return 1;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    int ret;

    ret = tkl_flash_read(offset, buf, size);

    return ret;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    int ret;

    ret = tkl_flash_write(offset, buf, size);

    return ret;
}

static int erase(long offset, size_t size)
{
    int ret;
    int32_t erase_size = ((size - 1) / FLASH_ERASE_MIN_SIZE) + 1;

    ret = tkl_flash_erase(offset, erase_size);

    return ret;
}

const struct fal_flash_dev nor_flash0 = {
    .name = NOR_FLASH_DEV_NAME,
    .addr = 0x0,                      // address is relative to beginning of partition; 0x0 is start
                                      // of the partition
    .len = (256 * 1024),              // size of the partition as specified in partitions.csv
    .blk_size = FLASH_ERASE_MIN_SIZE, // must be 4096 bytes
    .ops = {init, read, write, erase},
    .write_gran = 1, // 1 byte write granularity
};
