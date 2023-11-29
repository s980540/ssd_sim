#ifndef DESC_H
#define DESC_H

#include "sys_mem.h"
#include "tcm.h"

#define FTL_DESC_ID_NUM     (0x800)
#define FTL_DESC_ID_MASK    (FTL_DESC_ID_NUM - 1)
#define DESC_ID_NULL        (0xFFFF)

#define FTL_DESC_NEXT(id, member) \
    (((fw_desc_t *)tcm_get_ptr(SYS_MEM_TYPE_B1TCM, FW_CMD_DESC0_IDX) + (id & FTL_DESC_ID_MASK))->member)

#define FTL_DESC_FOR_EACH(id, head_id, member) \
    for (id = head_id; id != DESC_ID_NULL; id = FTL_DESC_NEXT(id, member))

#endif // ~ DESC_H
