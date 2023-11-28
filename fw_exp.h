// Firmware External Parameter
#ifndef FW_EXP_H
#define FW_EXP_H

#include "global.h"

#include "nv_cfg.h"
#include "fw_cfg.h"

#define DIE_2_CECH_MAX_NUM  (64)
#define CECH_2_DIE_MAX_NUM  (128)

typedef struct _dpd_para_tbl
{
    u8 die_2_cech[DIE_2_CECH_MAX_NUM];
    u8 cech_2_die[CECH_2_DIE_MAX_NUM];
} DPD_PARA_TBL;

extern DPD_PARA_TBL g_dpd_para_tbl;
extern void init_dpd_para_tbl(void);

#endif // FW_EXP_H