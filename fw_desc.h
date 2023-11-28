#ifndef FW_DESC_H
#define FW_DESC_H

#include "global.h"
#include "fcl_def.h"
#include "fcl_fc_cfg.h"
#include "bdm_desc.h"

typedef union _fw_desc_config0_t
{
    struct _fw_desc_config0_bits_t
    {
        u32 sr : 1;
        u32 hw_desc_rls : 1;
        u32 rcq_sel : 1;
        u32 opc : 1;
        u32 adr_map_sel : 1;
        u32 op_sel : 6;
        u32 rsvd : 1;
        u32 ex_para : 8;
        u32 byp : 1;
        u32 abort : 1;
        u32 vol : 2;
        u32 ce : 4;
        u32 ch : 4;
    } bits;
    u32 config;
} fw_desc_config0_t;

typedef union _fw_desc_config1_t
{
    struct _fw_desc_config1_bits_t
    {
        u32 desc_id : 4;
        u32 bm : 2;
        u32 decm : 2;
        u32 buf_id : 4;
        u32 page_type : 3;
        u32 buf_sel : 1;
        u32 raid_cmd : 4;
        u32 raid_buf_id : 5;
        u32 cw_fmt_id : 4;
        u32 wl_type : 2;
        u32 rcq0_list : 1;
    } bits;
    u32 config;
} fw_desc_config1_t;

typedef union _fw_desc_config2_t
{
    struct _fw_desc_config2_bits_t
    {
        u32 sec_bmp_secd : 16;
        u32 rsvd : 16;
    } bits;
    u32 config;
} fw_desc_config2_t;

typedef struct _fw_desc_config_t
{
    fw_desc_config0_t config0;
    fw_desc_config1_t config1;
#if (EN_FCL_IF_DESC_CONFIG2_OPT)
    fw_desc_config2_t config2;
#endif
} fw_desc_config_t;

typedef struct _fw_desc_t
{
    u16 next;
    u8 cb_id : 7;
    u8 flag : 1;
    u8 start : 3;
    u8 len : 3;
    u8 read_unc : 1;
    u8 ctu : 1;
    u16 cmd_id;
    u16 hash;
    fw_desc_config_t config;
    u32 buf_addr;
    u32 faa;
    fw_desc_sts_t status;
    fw_desc_unc_laa_t unc_info;
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
} __attribute__((packed)) fw_desc_t;

#define FW_FCL_DESC_ID_2_PTR(id) \
    ((fw_desc_t *)(FCL_DESC_ID_2_PTR(id)))

#endif // FW_DESC_H