#include "ftl_write_cache.h"

#include "tcm.h"
#include "list.h"
#include "fw_cfg.h"

ftl_write_cache_t g_ftl_write_cache;

swlist_t g_ftl_wc_flush_list;

void ftl_write_cache_init(void)
{
    g_ftl_write_cache.hash_tbl = (u16 *)WC_HASH_TBL_BASE;
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

bool ftl_write_cache_flush_cond(void)
{
    return g_ftl_wc_flush_list.cnt >= WC_AU_MIN_NUM;
}

bool ftl_write_cache_host_write_cond(void)
{
    return g_ftl_wc_flush_list.cnt < WC_AU_MAX_NUM;
}