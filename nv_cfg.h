#ifndef NV_CFG_H
#define NV_CFG_H

#include "autocfg.h"

#define CECH_QNTY           (CE_QNTY * CH_QNTY)
#define BANK_SHIFT          (CE_SHIFT - LUN_SHIFT)

#define LUN_NUM             (1 << LUN_SHIFT)
#define BANK_NUM            (1 << BANK_SHIFT)
#define CE_NUM              (1 << CE_SHIFT)
#define CH_NUM              (1 << CH_SHIFT)

#define LUN_MASK            (LUN_NUM - 1)
#define BANK_MASK           (BANK_NUM - 1)
#define CE_MASK             (CE_NUM - 1)
#define CH_MASK             (CH_NUM - 1)

#define GDP_MAX_CH_SHIFT    (3)
#define GDP_MAX_CH_NUM      (1 << GDP_MAX_CH_SHIFT)
#define GDP_MAX_CH_MASK     (GDP_MAX_CH_NUM - 1)

#define GDP_MAX_BANK_SHIFT  (3)
#define GDP_MAX_BANK_NUM    (1 << GDP_MAX_BANK_SHIFT)
#define GDP_MAX_BANK_MASK   (GDP_MAX_BANK_NUM - 1)

#define MAX_CH_SHIFT        (2)
#define MAX_CH_NUM          (1 << MAX_CH_SHIFT)
#define MAX_CH_MASK         (MAX_CH_NUM - 1)

#define MAX_BANK_SHIFT      (3)
#define MAX_BANK_NUM        (1 << MAX_BANK_SHIFT)
#define MAX_BANK_MASK       (MAX_BANK_NUM - 1)

#define MAX_LUN_SHIFT       (LUN_SHIFT)
#define MAX_LUN_NUM         (1 << MAX_LUN_SHIFT)
#define MAX_LUN_MASK        (MAX_LUN_NUM - 1)

#define MAX_PART_SHIFT      (3)
#define MAX_PART_NUM        (1 << MAX_PART_SHIFT)

#define DIE_NUM             (1 << DIE_SHIFT)

#define VDIE_NUM            (1 << VDIE_SHIFT)

#define DIE_QNTY            (DIE_NUM)
#define VDIE_QNTY           (VDIE_NUM)

#endif
