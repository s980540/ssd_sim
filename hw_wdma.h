#ifndef HW_WDMA_H
#define HW_WDMA_H

#include "global.h"

#include "reg_hw_wdma_ctrl_cclk.h"

int hw_xdma_desc_init(void);
int hw_xdma_desc_deinit(void);
int hw_xdma_desc_lock(void);
int hw_xdma_desc_unlock(void);
bool hw_xdma_desc_full(void);
bool hw_xdma_desc_empty(void);
u16 hw_xdma_desc_alloc(void);
int hw_xdma_desc_free(u16 desc_id);
int hw_xdma_desc_check(void);

int hw_wdma_init(void);
int hw_wdma_deinit(void);
int hw_wdma_ctrl_cclk_lock(void);
int hw_wdma_ctrl_cclk_unlock(void);
bool hw_wdma_desc_rdy(void);
void hw_wdma_desc_set(void);
void hw_wdma_desc_clr(void);
u32 hw_wdma_desc_ctrl0(void);

#if (OPT_SW_SIM)
extern reg_hw_wdma_ctrl_cclk_t *reg_hw_wdma_ctrl_cclk;
#endif

#endif // ~ HW_WDMA_H