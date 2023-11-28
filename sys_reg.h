#ifndef SYS_REG_H
#define STS_REG_H

#include "global.h"

typedef struct _sys_reg_info_t
{
    u32 base;
} sys_reg_info_t;

typedef enum _sys_reg_id_e
{
    REG_HW_WDMA_CTRL_CCLK_IDX,
} sys_reg_id_e;

#endif // SYS_REG_H