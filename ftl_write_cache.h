#ifndef FTL_WRITE_CACHE_H
#define FTL_WRITE_CACHE_H

#include "global.h"

#include "fw_desc.h"

typedef struct _ftl_write_cache_t
{
    u16 *hash_tbl;
    u16 hash_cnt;

    void (*push)(fw_desc_t *desc);
    void (*del)(fw_desc_t *desc);
    void (*del_list)(u16 desc_id);
    fw_desc_t *(*hit)(u32 laa);
} ftl_write_cache_t;

#define WC_HOST_WR_COND (write_cache_host_write_condition())
#define WC_FLUSH_COND   (write_cache_flush_condition())

void ftl_write_cache_init(void);
void ftl_write_cache_push(fw_desc_t *desc);
void ftl_write_cache_del(fw_desc_t *desc);
void ftl_write_cache_del_list(u16 desc_id);
fw_desc_t *ftl_write_cache_hit(u32 laa);

extern bool write_cache_flush_condition(void);
extern bool write_cache_host_write_condition(void);

#endif // FTL_WRITE_CACHE_H