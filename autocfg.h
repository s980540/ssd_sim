#ifndef AUTO_CFG_H
#define AUTO_CFG_H

#define FALCON_LITE
#define TLC_NAND
#define TSB3D_T
#define SD_BICS5_128GB
#define ONE_PASS_MODE

#define VDIE_SHIFT      (5)
#define DIE_SHIFT       (5)

#define CE_SHIFT        (3)
#define LUN_SHIFT       (1)
#define CH_SHIFT        (2)

#define CE_QNTY         (8)
#define LUN_QNTY        (2)
#define BANK_QNTY       (4)
#define CH_QNTY         (4)

#define ONE_STAGE_DIE_CNT       (32)
#define MAX_SUPER_PAGES_FOR_ONE_RAID_STRIPE         (1)
#define MAX_SLC_SUPER_PAGES_FOR_ONE_RAID_STRIPE     (1)

#endif // AUTO_CFG_H