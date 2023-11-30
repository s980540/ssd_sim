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

#define WC_HOST_WR_COND (ftl_write_cache_host_write_cond())
#define WC_FLUSH_COND   (ftl_write_cache_flush_cond())

extern void ftl_write_cache_init(void);
extern void ftl_write_cache_push(fw_desc_t *desc);
extern void ftl_write_cache_del(fw_desc_t *desc);
extern void ftl_write_cache_del_list(u16 desc_id);
extern fw_desc_t *ftl_write_cache_hit(u32 laa);

extern bool ftl_write_cache_flush_cond(void);
extern bool ftl_write_cache_host_write_cond(void);

#endif // FTL_WRITE_CACHE_H