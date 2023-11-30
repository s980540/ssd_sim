#ifndef FTL_H
#define FTL_H

#include "global.h"

typedef struct _ftl_cmp_info_t
{
    u8 cmp;
    u8 tag;
    u8 sec_cnt;
    u8 cmp_result;
    u32 other_err;
    u64 lba;
    u32 total_len;
    u32 get_len;
    u32 uid;
    u32 cmp_addr;
} __attribute__((packed)) ftl_cmp_info_t;

// #define reg_hw_wdma_ctrl_cclk (volatile reg_hw_wdma_ctrl_cclk_t *)()

#endif // FTL_H