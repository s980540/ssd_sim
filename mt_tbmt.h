#ifndef MT_TBMT_H
#define MT_TBMT_H

#include "global.h"
#include "fw_cfg.h"
#include "list.h"

typedef struct _ftl_tblk_id_t
{
    u16 blk_id[VTBL_BLK_MAX_NUM];
    SLIST_DEFINE(VTBL_BLK_MAX_NUM, u8);
} ftl_tblk_id_t;

#endif // MT_TBMT_H