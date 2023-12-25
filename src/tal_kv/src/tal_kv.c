#include "tal_kv.h"
#include "lfs_config.h"
#include "lfs.h"
#include "tkl_flash.h"
#include "tal_api.h"
#include "tal_security.h"

// variables used by the filesystem
static lfs_t lfs;
static lfs_size_t lfs_flash_addr;
static tal_kv_cfg_t lfs_kv_cfg;
static MUTEX_HANDLE lfs_mutex;

int user_provided_block_device_read(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size)
{

    OPERATE_RET  ret = tkl_flash_read(lfs_flash_addr + c->block_size * block + off,  buffer, size);
    if (OPRT_OK != ret) {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

int user_provided_block_device_prog(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, const void *buffer, lfs_size_t size)
{

    OPERATE_RET  ret = tkl_flash_write(lfs_flash_addr + c->block_size * block + off,  buffer, size);
    if (OPRT_OK != ret) {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

int user_provided_block_device_erase(const struct lfs_config *c, lfs_block_t block)
{

    OPERATE_RET  ret = tkl_flash_erase(lfs_flash_addr + c->block_size * block,  c->block_size);
    if (OPRT_OK != ret) {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}


int user_provided_block_device_sync(const struct lfs_config *c)
{
    return LFS_ERR_OK;
}


int tal_kv_init(tal_kv_cfg_t *kv_cfg)
{
    uint8_t sha256_ret[32];

    //! init falsh key
    memset(&lfs_kv_cfg, 0, sizeof(lfs_kv_cfg));
    tal_sha256_ret((const UINT8_T *)kv_cfg->seed, TAL_LV_KEY_LEN, sha256_ret, 0);
    memcpy(lfs_kv_cfg.seed, sha256_ret, TAL_LV_KEY_LEN);
    tal_sha256_ret((const UINT8_T *)kv_cfg->key,  TAL_LV_KEY_LEN, sha256_ret, 0);
    memcpy(lfs_kv_cfg.key, sha256_ret, TAL_LV_KEY_LEN);

    tal_mutex_create_init(&lfs_mutex);

    TUYA_FLASH_BASE_INFO_T  info;
    tkl_flash_get_one_type_info(TUYA_FLASH_TYPE_UF, &info);
    lfs_flash_addr = info.partition[0].start_addr;

    static struct lfs_config lfs_cfg = {0};

    memset(&lfs_cfg, 0, sizeof(lfs_cfg));

    lfs_cfg.read           = user_provided_block_device_read;
    lfs_cfg.prog           = user_provided_block_device_prog;
    lfs_cfg.erase          = user_provided_block_device_erase;
    lfs_cfg.sync           = user_provided_block_device_sync;
    lfs_cfg.read_size      = info.partition[0].block_size,
    lfs_cfg.prog_size      = info.partition[0].block_size,
    lfs_cfg.block_size     = info.partition[0].block_size,
    lfs_cfg.block_count    = info.partition[0].size / info.partition[0].block_size,
    lfs_cfg.cache_size     = info.partition[0].block_size,
    lfs_cfg.lookahead_size = lfs_cfg.block_count / 8 + (8 - (lfs_cfg.block_count % 8)),
    lfs_cfg.block_cycles   = 500;

    // mount the filesystem
    int err = lfs_mount(&lfs, &lfs_cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &lfs_cfg);
        err = lfs_mount(&lfs, &lfs_cfg);
    }

    return err;

}

int tal_kv_set(const char* key, const uint8_t *value, size_t length)
{   
    int result;
    lfs_file_t file;

    PR_DEBUG("key:%s, len %d", key, length);

    tal_mutex_lock(lfs_mutex);
    result = lfs_file_open(&lfs, &file, key, LFS_O_RDWR | LFS_O_CREAT | LFS_O_TRUNC);
    if (LFS_ERR_OK != result) {
        tal_mutex_unlock(lfs_mutex);
        PR_ERR("lfs open %s err", key);
        return result;
    }
    uint8_t  *ec_data = NULL; 
    uint32_t  ec_len  = 0;
    uint8_t   iv[16];

    memcpy(iv, lfs_kv_cfg.seed, 16);
    result = tal_aes128_cbc_encode((UINT8_T *)value, length, (UINT8_T *)lfs_kv_cfg.key, iv, &ec_data, (UINT32_T *)&ec_len);
    if (OPRT_OK != result) {
        tal_mutex_unlock(lfs_mutex);
        PR_DEBUG("key %s encrypt failed", key);
        return result;
    }
    lfs_file_rewind(&lfs, &file);
    result = lfs_file_write(&lfs, &file, ec_data, ec_len);
    lfs_file_close(&lfs, &file);
    tal_aes_free_data(ec_data);
    tal_mutex_unlock(lfs_mutex);
    if (result != ec_len) {
        PR_ERR("kv write fail %d", result);
        return OPRT_KVS_WR_FAIL;
    }

    return OPRT_OK;
}

int tal_kv_get(const char* key, uint8_t **value, size_t *length)
{
    int result;
    lfs_file_t file;

    if (NULL == key || NULL == value || NULL == length) {
        return OPRT_INVALID_PARM;
    }

    tal_mutex_lock(lfs_mutex);
    result = lfs_file_open(&lfs, &file, key, LFS_O_RDONLY);
    if (LFS_ERR_OK != result) {
        PR_ERR("lfs open %s err", key, result);
        tal_mutex_unlock(lfs_mutex);
        return result;
    }
    uint8_t   *ec_data = NULL; 
    uint32_t   ec_len  = lfs_file_size(&lfs, &file);

    ec_data = tal_malloc(ec_len + 1);
    if (NULL == ec_data) {
        result = OPRT_MALLOC_FAILED;
        tal_mutex_unlock(lfs_mutex);
        return result;
    }
    PR_DEBUG("key:%s, len:%d", key, ec_len);
    result = lfs_file_read(&lfs, &file, ec_data, ec_len);
    lfs_file_close(&lfs, &file);
    tal_mutex_unlock(lfs_mutex);
    if (result <= 0) {
       *length = 0;
        tal_free(ec_data);
        PR_ERR("kv read error %d", result);
        return OPRT_KVS_RD_FAIL;
    }
    uint8_t  *dec_data = NULL; 
    uint32_t  dec_len  = 0;
    uint8_t   iv[16];

    memcpy(iv, lfs_kv_cfg.seed, 16);
    result  = tal_aes128_cbc_decode(ec_data, ec_len, (UINT8_T *)lfs_kv_cfg.key, iv, &dec_data, (UINT32_T *)&dec_len);
    dec_len = tal_aes_get_actual_length(dec_data, dec_len);
    tal_free(ec_data);
    if (OPRT_OK != result || dec_len > ec_len) {
        PR_ERR("key %s decrypt failed %d, %d-%d", key, result, dec_len, ec_len);
        return OPRT_BUFFER_NOT_ENOUGH;
    }
    *value  = dec_data;
    *length = (size_t)dec_len;
     dec_data[dec_len] = 0;

    return OPRT_OK;
}


int tal_kv_del(const char* key)
{
    PR_DEBUG("key:%s", key);

    tal_mutex_lock(lfs_mutex);
    int result = lfs_remove(&lfs, key);
    tal_mutex_unlock(lfs_mutex);
    if (LFS_ERR_OK == result) {
        PR_DEBUG("Deleted successfully");
        return OPRT_OK;
    }

    PR_DEBUG("Deleted failed %s", result);

    return OPRT_COM_ERROR;
}

int tal_kv_free(uint8_t *value)
{
    if (NULL == value) {
        return OPRT_INVALID_PARM;
    }
    tal_free(value);

    return OPRT_OK;
}

void tal_kv_cmd(int argc, char *argv[])
{
    if (argc < 3) {
        return ;
    }

    if (0 == strcmp("set", argv[1])) {
        tal_kv_set(argv[2], (const uint8_t *)argv[3], strlen(argv[3]));
    } else if (0 == strcmp("get", argv[1])) {
        uint8_t *buffer;
        size_t   length;
        if (OPRT_OK == tal_kv_get(argv[2], &buffer, &length)) {
            PR_DEBUG("buffer %s", buffer);
            tal_kv_free(buffer);
        }
    } else if (0 == strcmp("del", argv[1])) {
        tal_kv_del(argv[2]);
    } else if (0 == strcmp("list", argv[1])) {
        lfs_dir_t dir;
        lfs_dir_open(&lfs, &dir, argv[2]);
        struct lfs_info  info;
        while (lfs_dir_read(&lfs, &dir, &info) > 0) {
            PR_DEBUG_RAW("%s  ", info.name);
        }
        PR_DEBUG_RAW("\r\n", info.name);
        lfs_dir_close(&lfs, &dir);
    }
}

int kv_serialize(IN CONST kv_db_t *db, IN CONST UINT_T dbcnt, OUT CHAR_T **out, OUT UINT_T *out_len);
int kv_deserialize(IN CONST CHAR_T *in, INOUT kv_db_t *db, IN CONST UINT_T dbcnt);



int tal_kv_serialize_set(const char* key, kv_db_t *db, size_t dbcnt)
{
    if (NULL == db || 0 == dbcnt) {
        return OPRT_INVALID_PARM;
    }

    CHAR_T  *buf = NULL;
    UINT_T   len = 0;
    int      ret = OPRT_OK;

    ret = kv_serialize(db, dbcnt, &buf, &len);
    if (OPRT_OK != ret) {
        PR_ERR("kv_serialize  fail. %d", ret);
        return ret;
    }
    PR_TRACE("write buf:%s", buf);
    ret = tal_kv_set(key, (const uint8_t *)buf, len);
    tal_free(buf);
    if (OPRT_OK != ret) {
        PR_ERR("kv_set fails %s %d", key, ret);
    }

    return ret;
}


int tal_kv_serialize_get(const char* key, kv_db_t *db, size_t dbcnt)
{
    if (NULL == db || 0 == dbcnt || NULL == key) {
        return OPRT_INVALID_PARM;
    }

    uint8_t  *buf = NULL;
    size_t    len = 0;
    int       ret = OPRT_OK;

    ret = tal_kv_get(key, &buf, &len);
    if (OPRT_OK != ret) {
        PR_ERR("kv_get fails %s %d", key, ret);
        return ret;
    }
    ret = kv_deserialize((CHAR_T *)buf, db, dbcnt);
    tal_free(buf);
    if (OPRT_OK != ret) {
        PR_ERR("kv_deserialize fail. %d", ret);
    }

    return ret;
}

