#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "storage_interface.h"
#include "tal_log.h"

#define tuya_db_path "./tuyadb"

int posix_storage_set(const char* key, const uint8_t* buffer, size_t length)
{
    if (NULL == key || NULL == buffer) {
        return OPRT_INVALID_PARM;
    }

    FILE* fptr = NULL;

    char name[128];
    sprintf(name, "%s/%s", tuya_db_path, key);

    PR_DEBUG("key:%s", name);

    fptr = fopen(name, "wb+");
    if (NULL == fptr) {
        PR_ERR("open file error");
        return OPRT_COM_ERROR;
    } else {
        PR_DEBUG("open file OK");
    }

    int file_len = fwrite(buffer, 1, length, fptr);
    fclose(fptr);
    if (file_len != length) {
        PR_ERR("uf_kv_write fail %d", file_len);
        return OPRT_COM_ERROR;
    }
    return OPRT_OK;
}

int posix_storage_get(const char* key, uint8_t* buffer, size_t* length)
{
    if (NULL == key || NULL == buffer || NULL == length) {
        return OPRT_INVALID_PARM;
    }

    char name[128];
    sprintf(name, "%s/%s", tuya_db_path, key);

    PR_DEBUG("key:%s, len:%d", name, (int)*length);

    FILE* fptr = fopen(name, "rb");
    if (NULL == fptr) {
        *length = 0;
        PR_WARN("cannot open file");
        return OPRT_COM_ERROR;
    }

    int read_len = *length; // ?
    read_len = fread(buffer, 1, (size_t)read_len, fptr);
    fclose(fptr);
    if (read_len <= 0) {
        *length = 0;
        PR_ERR("read error %d", read_len);
        return OPRT_COM_ERROR;
    }

    *length = read_len;
    return OPRT_OK;
}

int posix_storage_del(const char* key)
{
    char name[128];
    sprintf(name, "%s/%s", tuya_db_path, key);

    PR_DEBUG("key:%s", name);
    if (remove(name) == 0) {
        PR_DEBUG("Deleted successfully");
        return OPRT_OK;
    } else {
        PR_ERR("Unable to delete the file");
        return OPRT_COM_ERROR;
    }
}

#ifdef __cplusplus
}
#endif
