#ifndef FTL_GEO_H
#define FTL_GEO_H

#include "fw_cfg.h"
#include "fw_exp.h"

#if defined(VIRTUAL_DIE)
#define VDIE_2_PAR(vdie)            (((vdie) / DIE_QNTY) & PARTITION_MASK)
#define VDIE_2_DIE(vdie)            ((vdie) % DIE_QNTY)
#define DIE_PAR_2_VDIE(die, par)    ((die) + (par) * DIE_QNTY)
#else
#define VDIE_2_PAR(vdie)            (0)
#define VDIE_2_DIE(vdie)            (vdie)
#define DIE_PAR_2_VDIE(die, par)    (die)
#endif

#define DIE_2_CECH(die)             (g_dpd_para_tbl.die_2_cech[die])
// cech: lun | ce | ch
#define CECH_2_DIE(cech)            (g_dpd_para_tbl.cech_2_die[cech])
#define DIE_2_CH(die)               (CECH_2_PCH(g_dpd_para_tbl.die_2_cech[die]))
#define DIE_2_BANK(die)             (CECH_2_PBANK(g_dpd_para_tbl.die_2_cech[die]))
#define DIE_2_LUN(die)              (CECH_2_PLUN(g_dpd_para_tbl.die_2_cech[die]))

#endif // FTL_GEO_H