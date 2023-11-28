#ifndef FTL_BLK_H
#define FTL_BLK_H

#include "global.h"

typedef struct _ftl_blk_list_t
{
    u16 next;
    u16 prev;
} ftl_blk_list_t;

#if (EN_READ_DISTURB)
    #define BLK_READ_CNT_SHIFT      (23)
    #define BLK_READ_CNT_MAX        ((1 << BLK_READ_CNT_SHIFT) - 1)
    #define BLK_HEALTH_STS_SHIFT    (2)
    #define BLK_SCAN_CNT_SHIFT      (4)
    #define BLK_SCAN_CNT_MAX        ((1 << BLK_SCAN_CNT_SHIFT) - 1)
    #define BLK_ECC_BIT_LVL_SHIFT   (2)
    #define BLK_ECC_BIT_LVL_NUM     ((1 << BLK_ECC_BIT_LVL_SHIFT))
#endif

typedef struct _ftl_read_disturb_sts_t
{
    struct _ftl_read_disturb_sts_bit_t
    {
        u32 read_cnt     : BLK_READ_CNT_SHIFT;    // pre-read count
        u32 health_sts   : BLK_HEALTH_STS_SHIFT;  // data scrub sts
        u32 scan_cnt     : BLK_SCAN_CNT_SHIFT;    // read disturb scan cnt
        u32 ecc_bit_lvl  : BLK_ECC_BIT_LVL_SHIFT; // max ecc bit level for read disturb
        u32 reset        : 1;                     // reset==1 means read_cnt is reset by erasing this blk
    } bit;
    u32 config;
} ftl_read_disturb_sts_t;

typedef struct _ftl_blk_config_t
{
    struct _ftl_blk_config_bit_t
    {
        u16 blk_sts     : 2;
        u16 media_id    : 2;
        u16 arena_id    : 3;
        u16 vir_die     : 1;
        u16 bin_type    : 4;
        u16 domain_id   : 2;
        u16 unsafe_pc   : 1;
        u16 rcd_in_sys  : 1;
    } bit;
    u16 config;
} ftl_blk_config_t;

typedef struct _ftl_blk_t
{
    ftl_blk_list_t entry;
    ftl_blk_config_t blk_config;
    u16 blk_id;
    u32 erase_cnt;
#if (!OPT_SEPERATE_VFC) || (OPT_SEPERATE_VFC_DBG)
    u32 vfc;
#endif
#if (!OPT_NEW_SN_SORT)
    u32 sn;
#endif
#if (EN_READ_DISTURB)
    ftl_read_disturb_sts_t rd_sts;
#endif
#if (EN_VIRTUAL_DIE) || (EN_VTR)
    u16 pbn[PARTITION_QNTY];
#endif
} ftl_blk_t;

typedef enum _ftl_head_blk_type_e
{
    FTL_HEAD_BLK_TYPE_FLUSH = 0,
    FTL_HEAD_BLK_TYPE_MERGE = 1,
    FTL_HEAD_BLK_TYPE_WL,
    FTL_HEAD_BLK_TYPE_TBL,
    FTL_HEAD_BLK_TYPE_SYS,
    FTL_HEAD_BLK_TYPE_CHKPTR,
    FTL_HEAD_BLK_TYPE_MAX_NUM
} ftl_head_blk_type_e;

typedef struct _ftl_head_blk_t
{
    ftl_blk_t *blk;
    // fct_media *media;
    u16 blk_id;
    ftl_head_blk_type_e type;
} ftl_head_blk_t;

#define FTL_HEAD_BLK_INVALID(head_blk) \
    (((head_blk) == NULL) || ((head_blk)->blk == NULL))

#endif // FTL_BLK_H