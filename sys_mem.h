#ifndef SYS_MEM_H
#define SYS_MEM_H

#include "hw_cfg.h"
#include "reg_hw_wdma_ctrl_cclk.h"

typedef enum _sys_mem_type_e
{
    SYS_MEM_TYPE_B0TCM_CPU0 = 0,
    SYS_MEM_TYPE_B0TCM_CPU1 = 1,
    SYS_MEM_TYPE_B0TCM_CPU2,
    SYS_MEM_TYPE_B0TCM_CPU3,
    SYS_MEM_TYPE_B1TCM,
    SYS_MEM_TYPE_SRAM,
    SYS_MEM_TYPE_UNKNOWN,
    SYS_MEM_TYPE_MAX_NUM
} sys_mem_type_e;

typedef struct _mem_info_t
{
    const char *name;
    u32 base;
    u32 end;
    u32 size;
    int type;
    int id;
    // int align;
} mem_info_t;

void *aligned_alloc(size_t size, u32 align);

#endif // SYS_MEM_H