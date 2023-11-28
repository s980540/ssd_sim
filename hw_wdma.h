#ifndef HW_WDMA_H
#define HW_WDMA_H

#include "global.h"

int hw_wdma_init(void);
int hw_wdma_deinit(void);
int hw_xdma_desc_init(void);

int hw_wdma_desc_rdy(void);
int hw_wdma_desc_set(void);
int hw_wdma_desc_clr(void);
int hw_wdma_desc_ctrl0(void);

#endif // ~ HW_WDMA_H