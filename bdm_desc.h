#ifndef BDM_DESC_H
#define BDM_DESC_H

#include "global.h"

#include "fcl_fc_cfg.h"

typedef union _hw_desc_config0_t
{
    struct _hw_desc_config0_bits_t
    {
        u32 sr : 1;
        u32 hw_desc_rls : 1;
        u32 rcq_sel : 1;
        u32 opc : 1;
        u32 adr_map_sel : 1;
        u32 op_sel : 6;
        u32 rsvd : 1;
        u32 ex_param : 8;
        u32 byp : 1;
        u32 abort : 1;
        u32 vol : 2;
        u32 ce : 4;
        u32 ch : 4;
    } bits;
    u32 config;
} hw_desc_config0_t;

typedef union _hw_desc_config1_t
{
    struct _hw_desc_config1_bits_t
    {
        u32 desc_id : 4;
        u32 bm : 2;
        u32 decm : 2;
        u32 buf_id : 4;
        u32 pg_type : 3;
        u32 buf_sel : 1;
        u32 raid_cmd : 4;
        u32 raid_buf_id : 5;
        u32 cw_fmt_id : 4;
        u32 rsvd : 3;
    } bits;
    u32 config;
} hw_desc_config1_t;

typedef struct _hw_desc_config_t
{
    hw_desc_config0_t config0;
    hw_desc_config1_t config1;
} hw_desc_config_t;

typedef union _hw_desc_status_t
{
    struct _hw_desc_sts_
    {
        u8 logical_sts0;
        u8 logical_sts1;
        u8 error_cnt_over;
        u8 physical_sts;
    } bits;
    u32 config;
} hw_desc_status_t;

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