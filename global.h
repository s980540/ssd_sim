#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "autocfg.h"

#define EN_PROJECT_DRAM_LESS    (1)
#define EN_MT_2CPU              (0)
#define EN_MT_4CPU              (1)
#define EN_BT_30                (1)
#define EN_FCL_30               (1)
#define EN_TBL_31               (1)
#define EN_NVME_HMB             (1)
#define EN_UEFI_FEATURE         (0)
#define EN_FCL_UNIT_TEST_30     (0)
#define EN_CODE_SWAP            (1)
#define EN_READ_DISTURB         (1)
#define EN_WAI_TEST             (0)
#define EN_VIRTUAL_DIE          (0)
#define EN_VTR                  (0)
#define EN_PSLC                 (1)
#define EN_TCG_OPAL             (1)
#define EN_RAMDISK              (0)
#define EN_CODE_BANK            (0)
#define EN_CO_SIM               (0)
#define EN_FTL_UNIT_TEST        (0)
#define EN_AES                  (1)
#define EN_P2L_PAGE             (0)
#define EN_MT_HMB               (1)
#define EN_HMB_31               (1)
#define EN_PRED_RD_31           (1)
#define EN_PRED_RD_31_4CPU      (1)
#define EN_FE_HOST_RW_FTL_IF    (0)
#define EN_DEVICE_SELF_TEST     (1)
#define EN_SAVE_LOG             (0)
#define EN_SHUTTLE_CHIP         (0)
#define EN_DESC1                (0)

#define EN_READ_PATROL          (0)

#define EN_FEATURE_APACER_CORE_POWER    (0)
#define EN_NAND_REINIT_FEATURE  (1)

#define OPT_SW_SIM_REG          (1)
#if (OPT_SW_SIM_REG)
#include "pthread.h"
#endif

// set desc in B1TCM
#define OPT_TCM_DESC            (1)
#define OPT_NAND_MEM            (1)
#define OPT_UCODE_IN_FW         (1)

#define OPT_TRIM_FULL_DISK      (1)
#define OPT_TRIM_FULL_DISK_CHK  (0)

#if (EN_CODE_SWAP)
#define OPT_CODE_SWAP_BY_BUF    (1)
#else
#define OPT_CODE_SWAP_BY_BUF    (0)
#endif

#if (EN_NVME_HMB)
#if (EN_AES)
#define OPT_HMB_AES             (0)
#else
#define OPT_HMB_AES             (0)
#endif
#define OPT_HMB_RCT             (1)
// only read dword faa from hmb_rct
#define OPT_HMB_RCT_RD_DWORD    (1)
// busy wait for HMB_RCT_RD_DWORD
#define OPT_HMB_RCT_BUSY_WAIT   (1)
#else
#define OPT_HMB_RCT             (0)
#endif

#define OPT_SEPERATE_VFC        (1)
#define OPT_SEPERATE_VFC_DBG    (1)

#define OPT_NEW_SN_SORT         (1)

#define OPT_CMT_VFC_USE_HWBV    (0)

#if (EN_FCL_30)
#define OPT_FCL_30_ADMIN_USE_FIFO   (0)
#endif

#define OPT_USE_UART_INT        (1)
#define OPT_TBMT_DRAM           (0)

#define OPT_OPT_RAMDISK_IN_DRAM (0)

#define OPT_DRAM_L2P_CHK        (0)

#if (EN_HMB_31)
#define OPT_DIRECT_HMB          (1)
#endif

#define OPT_SD_MISCOMPARE_DBG   (0)

#if (EN_PRED_RD_31_4CPU)
#define OPT_FCL_FW_RCQ          (1)
#else
#define OPT_FCL_FW_RCQ          (0)
#endif

#define OPT_L1_WORKAROUND       (0)
#define OPT_CMT_BYPASS          (0)

#define OPT_PCIE_LTSSM_STATE_DEBUG  (0)
#define OPT_FCL_IF_DESC_CONFIG2 (0)

#define OPT_BUF_USE_DBG2        (0)
#define OPT_DESC_USE_DBG2       (0)

#define OPT_CALCULATE_ROSC_FREQ (1)
#define OPT_CO_SIM_NO_NAND      (0)
#define OPT_HASH_ENGINE_UT      (0)
#define OPT_DESC_USE_WAR        (0)

#endif // GLOBAL_H