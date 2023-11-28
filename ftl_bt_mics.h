#ifndef FTL_BT_MICS_H
#define FTL_BT_MICS_H

#include "global.h"

#include "nv_cfg.h"
#include "fw_cfg.h"
#include "ftl_geo.h"
#include "fw_exp.h"

#define FTL_BT_MICS_TRACE   (1)

typedef struct _mp_die_2_cech
{
    u8 die[GDP_MAX_CH_NUM];
    u8 die_num;
} MP_DIE_2_CECH;

extern void ftl_bt_build_die_map(void);

#endif  // FTL_BT_MICS_H