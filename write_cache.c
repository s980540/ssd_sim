#include "write_cache.h"
#include "list.h"
#include "fw_cfg.h"

swlist_t g_write_cache_flush_list;

bool write_cache_flush_condition(void)
{
    return g_write_cache_flush_list.cnt >= WC_AU_MIN_NUM;
}

bool write_cache_host_write_condition(void)
{
    return g_write_cache_flush_list.cnt < WC_AU_MAX_NUM;
}