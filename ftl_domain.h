#ifndef FTL_DOMAIN_H
#define FTL_DOMAIN_H

#include "global.h"
#include "ftl_blk.h"

typedef enum _ftl_arena_type_e
{
    FTL_ARENA_TYPE_USER_HIGH = 0,
    FTL_ARENA_TYPE_USER_NORMAL = 1,
    FTL_ARENA_TYPE_TBL,
    FTL_ARENA_TYPE_SYS,
    #if (EN_BT_30)
    FTL_ARENA_TYPE_CHKPT,
    #endif
    FTL_ARENA_TYPE_MAX_NUM
} ftl_arena_type_e;

typedef struct _ftl_arena_t
{

} ftl_arena_t;

typedef struct _ftl_domain_t
{
    ftl_arena_t *arena[FTL_ARENA_TYPE_MAX_NUM];
    ftl_head_blk_t *head_blk[FTL_HEAD_BLK_TYPE_MAX_NUM];
} ftl_domain_t;

// extern bool ftl_data_domain_write(ftl_domain_t *, u16, ftl_head_blk_type_e, u8, u16);

#endif