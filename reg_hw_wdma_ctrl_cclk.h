#ifndef REG_HW_WDMA_CTRL_CCLK_H
#define REG_HW_WDMA_CTRL_CCLK_H

#include "global.h"
#include "auto_hw_cfg.h"

// Host Write Interface with Firmware for FTL Descriptor
typedef union _reg_hw_wdma_desc_ctrl0_t
{
    struct _reg_hw_wdma_desc_ctrl0_bits_t
    {
        u32 vld : 1;
        u32 rsvd : 5;
        u32 dtype : 2;
        u32 cnt : 8;
        u32 desc_id : 16;
    } bits;
    u32 config;
} reg_hw_wdma_desc_ctrl0_t;

typedef union _reg_hw_wdma_ftl_ptr_ctrl1_t
{
    u32 config;
} reg_hw_wdma_ftl_ptr_ctrl1_t;

typedef union _reg_hw_wdma_ftl_desc_ctrl_t
{
    u32 config;
} reg_hw_wdma_ftl_desc_ctrl_t;

typedef union _reg_hw_wdma_ftl_cclk_eco0_t
{
    u32 config;
} reg_hw_wdma_ftl_cclk_eco0_t;

typedef struct _reg_hw_wdma_ctrl_cclk_t
{
    u32 u32_gap0[64];
    reg_hw_wdma_desc_ctrl0_t hw_wdma_desc_ctrl0;
    reg_hw_wdma_ftl_ptr_ctrl1_t hw_wdma_ftl_ptr_ctrl1;
    reg_hw_wdma_ftl_desc_ctrl_t hw_wdma_ftl_desc_ctrl;
    reg_hw_wdma_ftl_cclk_eco0_t hw_wdma_cclk_eco0;
#if (OPT_SW_SIM)
    pthread_mutex_t mutex;
#endif
} reg_hw_wdma_ctrl_cclk_t;

#if (!OPT_SW_SIM)
#define reg_hw_wdma_ctrl_cclk \
    ((volatile reg_hw_wdma_ctrl_cclk_t *)(REG_HW_WDMA_CTRL_CCLK_BASE))
#endif

#endif // REG_HW_WDMA_CTRL_CCLK_H