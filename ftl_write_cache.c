#include "ftl_write_cache.h"

#include "tcm.h"
#include "list.h"
#include "fw_cfg.h"

ftl_write_cache_t g_ftl_write_cache;

swlist_t g_ftl_write_cache_flush_list;

void ftl_write_cache_init(void)
{
    g_ftl_write_cache.hash_tbl = tcm_get_ptr(SYS_MEM_TYPE_B1TCM, WC_HASH_TBL_IDX);
    g_ftl_write_cache.push = ftl_write_cache_push;
    g_ftl_write_cache.del = ftl_write_cache_del;
    g_ftl_write_cache.del_list = ftl_write_cache_del_list;
    g_ftl_write_cache.hit = ftl_write_cache_hit;
}

void ftl_write_cache_push(fw_desc_t *desc)
{
    return;
}

void ftl_write_cache_del(fw_desc_t *desc)
{
    return;
}

void ftl_write_cache_del_list(u16 desc_id)
{
    u16 id;
    fw_desc_t *desc;

    // for eache desc
    g_ftl_write_cache.del(desc);
    return;
}

fw_desc_t *ftl_write_cache_hit(u32 laa)
{
    return NULL;
}

bool write_cache_flush_condition(void)
{
    return g_ftl_write_cache_flush_list.cnt >= WC_AU_MIN_NUM;
}

bool write_cache_host_write_condition(void)
{
    return g_ftl_write_cache_flush_list.cnt < WC_AU_MAX_NUM;
}