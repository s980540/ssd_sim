#ifndef BDM_DESC_H
#define BDM_DESC_H

#include "global.h"
#include "fcl_fc_cfg.h"

typedef struct _hw_desc_t
{
    u16 next;
    u16 fw_use0;
    u16 fw_use1;
    u16 hash;
    hw_desc_config_t config;
    u32 buf_addr;
    u32 faa;
    hw_desc_status_t status;
    u32 rsvd;
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_0DW)
    u32 laa;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_1DW)
    u32 meta1;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_2DW)
    u32 meta2;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_3DW)
    u32 meta3;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_4DW)
    u32 meta4;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_5DW)
    u32 meta5;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_6DW)
    u32 meta6;
    #endif
    #if (FCL_IF_META_DATA_SIZE > META_SIZE_7DW)
    u32 meta7;
    #endif
} __attribute__((packed)) hw_desc_t;

#define FCL_DESC_PTR_2_ID(ptr) \
    (((hw_desc_t *)ptr) - (hw_desc_t *)FCL_IF_DESC0_MEM_BASE)

#define FCL_DESC_ID_2_PTR(id) \
    (((hw_desc_t *)FCL_IF_DESC0_MEM_BASE) + (id))

#endif // ~ BDM_DESC_H