#ifndef FW_CFG_H
#define FW_FCG_H

#include "global.h"
#include "nv_cfg.h"
#include "fcl_sd_bic5_cfg.h"

#define MAX_DIE_QNTY                    (1 << (VDIE_SHIFT))

// cech: lun | ce | ch
#define CECH_2_PLUN(cech)   (((cech) >> (GDP_MAX_CH_SHIFT + MAX_BANK_SHIFT)) & MAX_LUN_MASK)
#define CECH_2_PBANK(cech)  (((cech) >> (GDP_MAX_CH_SHIFT)) & MAX_BANK_MASK)
#define CECH_2_PCH(cech)    ((cech) & GDP_MAX_CH_MASK)

#define PCECH_2_CECH(lun, bank, ch) \
    ((((lun) & MAX_LUN_MASK) << (GDP_MAX_CH_SHIFT + MAX_BANK_SHIFT)) \
    | (((bank) & MAX_BANK_MASK) << (GDP_MAX_CH_SHIFT)) \
    | ((ch) & GDP_MAX_CH_MASK))

#define VBLOCK_SHIFT        (BLOCK_SHIFT)
#define VBLOCK_QNTY         (BLOCK_QNTY)
#define VBLOCK_NUM          (1 << VBLOCK_SHIFT)
#define VBLOCK_MASK         (VBLOCK_NUM - 1)

#define FCT_SUPPORT_DIE_STATUS_MAX_NUM  (32)
// fw capacity & settings
#define FLIC_FC_CMD_MAX_NUM             (FCT_SUPPORT_DIE_STATUS_MAX_NUM * 6)
#if (EN_FWSQ1)
#define FLIC_FC_CMD1_MAX_NUM            (FCT_SUPPORT_DIE_STATUS_MAX_NUM)
#define FLIC_FC_CQ1_MAX_NUM             (FCT_SUPPORT_DIE_STATUS_MAX_NUM)
#else
#define FLIC_FC_CMD1_MAX_NUM            (0)
#define FLIC_FC_CQ1_MAX_NUM             (0)
#endif
#define FLIC_FC_CMD_HP_MAX_NUM          (FCT_SUPPORT_DIE_STATUS_MAX_NUM)
#define FLIC_FC_CQ_MAX_NUM              (FCT_SUPPORT_DIE_STATUS_MAX_NUM * 2)

// data buf entry num config
#if (OPT_TRIM_FULL_DISK_CHK)
// BUF for TRIM_FULL_DISK_OPT check, FTL_BLK_VC_BASE
#define BUF_FOR_TFDO_CHK                (3)
#else
#define BUF_FOR_TFDO_CHK                (0)
#endif

#if defined(TSB3D_T) || defined(H3DT)
#define BUF_FOR_BAD_MAP                 (CE_QNTY / 2)
#else
#define BUF_FOR_BAD_MAP                 (0)
#endif

#if (EN_UEFI_FEATURE)
#define UEFI_BUF_CNT                    (16)
#else
#define UEFI_BUF_CNT                    (0)
#endif

#if defined(SD_BICS5_128GB)
#define DATA_BUF_SRAM_LL_ENTRY_NUM \
    (107 - BUF_FOR_TFDO_CHK - BUF_FOR_BAD_MAP + 50 - UEFI_BUF_CNT)
#endif

#define DATA_BUF_DRAM_LL_ENTRY_NUM      (0)

#if (EN_P2L_PAGE)
#define FW_BUF_SRAM_ENTRY_NUM           (3 * FRAG_QNTY)
#else
#define FW_BUF_SRAM_ENTRY_NUM           (1)
#endif

#define DATA_BUF_UNIT_SECTOR_NUM        (SECTOR_NUM)

#if (EN_E2E_DATA_CRC)
// both for dram and sram,
#define DATA_BUF_SECTOR_SIZE            (520)
#else
#define DATA_BUF_SECTOR_SIZE            (512)
#endif
#define DATA_BUF_UNIT_SIZE              (DATA_BUF_UNIT_SECTOR_NUM * DATA_BUF_SECTOR_SIZE)
#define DATA_BUF_UNIT_ALINE_SIZE        (512 * DATA_BUF_UNIT_SECTOR_NUM)

#if (EN_CODE_SWAP) && (OPT_CODE_SWAP_BY_BUF)
// set 128KB code for PBT swap and UEFI
#define SWAP_CMD_BUF_NUM                (32)
#endif

#define SECTOR_SHIFT                    (3)
#define SECTOR_NUM                      (1 << SECTOR_SHIFT)

#define ECCU_SHIFT                      (STPU_SHIFT - ECCU_FACTOR)
#define ECCU_NUM                        (1 << ECCU_SHIFT)

#define CMPU_SHIFT                      (STPU_SHIFT - CMPU_FACTOR)
#define CMPU_NUM                        (1 << CMPU_SHIFT)

#define MAPU_SHIFT                      (SECTOR_SHIFT - CMPU_FACTOR)
#define MAPU_NUM                        (1 << MAPU_SHIFT)

#define FRAG_SHIFT                      (STPU_SHIFT - SECTOR_SHIFT)
#define FRAG_NUM                        (1 << FRAG_SHIFT)

#define PFRAG_SHIFT                (FRAG_SHIFT - PLANE_SHIFT)
#define PFRAG_NUM                  (1 << PFRAG_SHIFT)
#define PFRAG_MASK                 (PFRAG_NUM - 1)

#define STPU_NUM                        (1 << STPU_SHIFT)

#if defined(YMTC_X3_9070) || defined(B58R)
#define FRAG_QNTY                       (PFRAG_NUM * PLANE_QNTY)
#define STPU_QNTY                       (FRAG_QNTY * SECTOR_NUM)
#else
#define FRAG_QNTY                       (FRAG_NUM)
#define STPU_QNTY                       (STPU_NUM)
#endif

#if (OPT_HMB_RCT)
#if (EN_MT_2CPU)
#define RCT_SRAM_ENTRY_NUM              (32)
#elif (EN_MT_4CPU)
#define RCT_SRAM_ENTRY_NUM              (16)
#else
#error "RCT_SRAM_ENTRY_NUM undefined"
#endif
#define FTL_DATA_L2P_ENTRY_SIZE         (4)
#if (OPT_HMB_RCT_BUSY_WAIT)
#define RCT_HMB_BUFF_ENTRY_NUM          (1)
#else
#define RCT_HMB_BUFF_ENTRY_NUM          (8)
#endif
#else // OPT_HMB_RCT
#endif // OPT_HMB_RCT

#define FTL_RCT_UNIT_SIZE_SHIFT         (11) // 2 KB
#define FTL_RCT_UNIT_SIZE               (1 << FTL_RCT_UNIT_SIZE_SHIFT)

#if (defined(SD_BICS5) || defined(SD_BICS5_128GB))
#if (EN_PSLC)
#define TBL_BLK_MAX_QNTY                (32)
#else
#define TBL_BLK_MAX_QNTY                (32)
#endif
#endif

#if (TBL_BLK_MAX_QNTY > 64)
    #define TBL_BLK_MAX_NUM_SHIFT       (7)
#elif (TBL_BLK_MAX_QNTY > 32)
    #define TBL_BLK_MAX_NUM_SHIFT       (6)
#elif (TBL_BLK_MAX_QNTY > 16)
    #define TBL_BLK_MAX_NUM_SHIFT       (5)
#elif (TBL_BLK_MAX_QNTY > 8)
    #define TBL_BLK_MAX_NUM_SHIFT       (4)
#elif (TBL_BLK_MAX_QNTY > 4)
    #define TBL_BLK_MAX_NUM_SHIFT       (3)
#elif (TBL_BLK_MAX_QNTY > 2)
    #define TBL_BLK_MAX_NUM_SHIFT       (2)
#elif (TBL_BLK_MAX_QNTY > 0)
    #define TBL_BLK_MAX_NUM_SHIFT       (1)
#else
    #error "TBL_BLK_MAX_NUM_SHIFT undefined"
#endif

#define TBL_BLK_MAX_NUM                 (1 << TBL_BLK_MAX_NUM_SHIFT)
#define TBL_BLK_MAX_MASK                (TBL_BLK_MAX_NUM - 1)

#if (EN_VTR)
#define VTBL_BLK_MAX_NUM                (1 << (TBL_BLK_MAX_NUM_SHIFT - PARTITION_TBL_SHIFT))
#define VTBL_BLK_MAX_MASK               (VTBL_BLK_MAX_NUM - 1)
#else
#define VTBL_BLK_MAX_NUM                (TBL_BLK_MAX_QNTY)
#define VTBL_BLK_MAX_MASK               (TBL_BLK_MAX_MASK)
#endif

#define LOG_SHIFT \
    (BLOCK_SHIFT + PAGE_SHIFT + STPU_SHIFT - SECTOR_SHIFT)

#if (defined(TLC_NAND) || defined(DIS_EXPONENT))
#if (defined(B47R) || defined(YMTC_X3_9070) || defined(B58R))
#define FTL_L2P_TBL_4K_SHIFT            (LOG_SHIFT + DIE_SHIFT - 10 - 1 - 1)
#else
#define FTL_L2P_TBL_4K_SHIFT            (LOG_SHIFT + DIE_SHIFT - 10 - 1)
#endif

#define FTL_L2P_TBL_4K_QNTY \
    (BLOCK_QNTY * PAGE_QNTY * FRAG_QNTY * DIE_QNTY / 1024)
#if (FTL_L2P_TBL_4K_QNTY < (1 << FTL_L2P_TBL_4K_SHIFT))
#error "FTL_L2P_TBL_4K_SHIFT error"
#endif
#else
// 10: 2^10 entry each 4KB tbl
#define FTL_L2P_TBL_SHIFT               (LOG_SHIFT + DIE_SHIFT)
#define FTL_L2P_TBL_4K_SHIFT            (LOG_SHIFT + DIE_SHIFT - 10)
#endif

#if (OPT_DBMT_TIE_2PAGES_TEST)
#if defined(SD_BICS5_128GB)
#if (DIE_SHIFT == 2)
#define FTL_TBMT_ENTRY_MAX_SHIFT        (13)
#elif (DIE_SHIFT == 4)
#define FTL_TBMT_ENTRY_MAX_SHIFT        (15)
#elif (DIE_SHIFT == 5)
#define FTL_TBMT_ENTRY_MAX_SHIFT        (16)
#endif
#endif
#else
#define FTL_TBMT_ENTRY_MAX_SHIFT        (16)
#endif

#define FTL_TBMT_ENTRY_SHIFT \
    ((FTL_L2P_TBL_4K_SHIFT < FTL_TBMT_ENTRY_MAX_SHIFT) \
    ? FTL_L2P_TBL_4K_SHIFT \
    : FTL_TBMT_ENTRY_MAX_SHIFT)
#define FTL_TBMT_ENTRY_NUM              (1 << FTL_TBMT_ENTRY_SHIFT)

#define FTL_DBMT_4K_SHIFT \
    ((FTL_L2P_TBL_4K_SHIFT > FTL_TBMT_ENTRY_SHIFT) \
    ? (FTL_L2P_TBL_4K_SHIFT - FTL_TBMT_ENTRY_SHIFT) \
    : 0)

#define FTL_DBMT_4K_NUM                 (1 << FTL_DBMT_4K_SHIFT)
#define FTL_DBMT_4K_NUM_MASK            (FTL_DBMT_4K_NUM - 1)
#define FTL_DBMT_4K_NUM_BITMAP          ((1 << FTL_DBMT_4K_NUM) - 1)

#if (FTL_DBMT_4K_SHIFT == 4)
#if (FTL_DBMT_4K_SHIFT > FRAG_SHIFT)
#define EN_CROSS_PAGE_DBMT
#endif
#endif

#define FTL_CMT_BITMAP_SIZE             (FTL_DBMT_4K_NUM * 1024 / 8)

#if (EN_TCG_OPAL)
#define HALF_OPAL_TABLE_BUF_SIZE        (8)
#define OPAL_DYNAMIC_BUF_ALLOC_MAX_NUM  (16)
#endif

#define MAX_SLOT_NUM                    (0x3)
#define HMB_MAX_DESC_NUM                (32)

#define FTL_DBMT_ENTRY_NUM_SHIFT        (FTL_DBMT_4K_SHIFT + 10)
#define FTL_DBMT_ENTRY_NUM              (1 << FTL_DBMT_ENTRY_NUM_SHIFT)
#define FTL_DBMT_ENTRY_MASK             (FTL_DBMT_ENTRY_NUM - 1)

// DBMT size = 64K (capacity = 4TB)
#if (FTL_DBMT_4K_SHIFT == 4)
#define FTL_CMT_ENTRY_SHIFT             (13)
#define FTL_CMT_ENTRY_NUM               (0x1B80)
#define FTL_CMUT_ENTRY_SHIFT            (12)
#define FTL_CMUT_ENTRY_NUM              (0x820)
// DBMT size = 32K (capacity = 2TB)
#elif (FTL_DBMT_4K_SHIFT == 3)
#if defined(SD_BIC5_128GB)
#define FTL_CMT_ENTRY_SHIFT             (13)
#define FTL_CMT_ENTRY_NUM               (0x1B00)
#define FTL_CMUT_ENTRY_SHIFT            (11)
#define FTL_CMUT_ENTRY_NUM              (0x800)
#else
#define FTL_CMT_ENTRY_SHIFT             (13)
#define FTL_CMT_ENTRY_NUM               (0x1A00)
#define FTL_CMUT_ENTRY_SHIFT            (11)
#define FTL_CMUT_ENTRY_NUM              (0x800)
#endif
// DBMT size = 16K (capacity = 1TB)
#elif (FTL_DBMT_4K_SHIFT == 2)
#define FTL_CMT_ENTRY_SHIFT             (13)
// #define FTL_CMT_ENTRY_NUM               (0x1A00)
#define FTL_CMT_ENTRY_NUM               (0x1C00)
#define FTL_CMUT_ENTRY_SHIFT            (11)
// #define FTL_CMUT_ENTRY_NUM              (0x500)
#define FTL_CMUT_ENTRY_NUM              (0x700)
// DBMT size = 8K (capacity = 512G)
#elif (FTL_DBMT_4K_SHIFT == 1)
#define FTL_CMT_ENTRY_SHIFT             (13)
// #define FTL_CMT_ENTRY_NUM               (0x1800)
#define FTL_CMT_ENTRY_NUM               (0x1A00)
#define FTL_CMUT_ENTRY_SHIFT            (11)
// #define FTL_CMUT_ENTRY_NUM              (0x600)
#define FTL_CMUT_ENTRY_NUM              (0x800)
// DBMT size = 4K (capacity <= 256G)
#elif (FTL_DBMT_4K_SHIFT == 0)
#define FTL_CMT_ENTRY_SHIFT             (13)
// #define FTL_CMT_ENTRY_NUM               (0x1600)
#define FTL_CMT_ENTRY_NUM               (0x1800)
#define FTL_CMUT_ENTRY_SHIFT            (11)
// #define FTL_CMUT_ENTRY_NUM              (0x600)
#define FTL_CMUT_ENTRY_NUM              (0x800)
#else
#error "DBMT size error"
#endif

#if defined(EN_MT_HMB)
// Indicates the number of bits occupied by the size of host memory buffer.
#if defined(COUGAR)
#if (0 == FTL_DBMT_4K_SHIFT) // Capacity <= 256 GB
#define HMB_HOST_BUFFER_SHIFT       (23)
#else
#define HMB_HOST_BUFFER_SHIFT       (24)
#endif
#elif defined(FALCON_LITE)
#if (0 == FTL_DBMT_4K_SHIFT) // Capacity <= 256 GB
#define HMB_HOST_BUFFER_SHIFT       (24)
#else
#define HMB_HOST_BUFFER_SHIFT       (25)
#endif
#endif
#define HMB_HOST_BUFFER_SIZE            (1 << HMB_HOST_BUFFER_SHIFT)

// Basic HMB slot is same size as CMU (cached mapping unit), same size as DMBT size.
#define HMB_SLOT_SIZE_SHIFT                 (FTL_DBMT_4K_SHIFT + 12)
#define HMB_SLOT_SIZE                       (1 << HMB_SLOT_SIZE_SHIFT)

// HMB sub-slot is same size as RCT slot.
#define HMB_SUB_SLOT_SIZE_SHIFT             (FTL_RCT_UNIT_SIZE_SHIFT)
#define HMB_SUB_SLOT_SIZE                   (1 << HMB_SUB_SLOT_SIZE_SHIFT)

// The number of hmb entries in host memory buffer
#define HMB_PHY_ENTRY_SHIFT             (HMB_HOST_BUFFER_SHIFT - HMB_SLOT_SIZE_SHIFT)
#define HMB_PHY_ENTRY_NUM               (1 << HMB_PHY_ENTRY_SHIFT)
#define HMB_PHY_ENTRY_MASK              (HMB_PHY_ENTRY_NUM - 1)

// The number of hmb entries in host memory buffer
#define HMB_PHY_SUB_ENTRY_SHIFT         (HMB_HOST_BUFFER_SHIFT - HMB_SUB_SLOT_SIZE_SHIFT)
#define HMB_PHY_SUB_ENTRY_NUM           (1 << HMB_PHY_SUB_ENTRY_SHIFT)
#define HMB_PHY_SUB_ENTRY_MASK          (HMB_PHY_SUB_ENTRY_NUM - 1)

// The number of hmb entries in CPU2/CPU3 hmb table.
#if (EN_MT_2CPU)
#define HMB_LOG_ENTRY_SHIFT         (HMB_PHY_ENTRY_SHIFT)
#define HMB_LOG_ENTRY_NUM           (1 << HMB_LOG_ENTRY_SHIFT)
#elif (EN_MT_4CPU)
#define HMB_LOG_ENTRY_SHIFT         (HMB_PHY_ENTRY_SHIFT - 1)
#define HMB_LOG_ENTRY_NUM           (1 << HMB_LOG_ENTRY_SHIFT)
#endif
#define HMB_LOG_ENTRY_MASK              (HMB_LOG_ENTRY_NUM - 1)

#define HMB_TBL_ENTRY_SIZE              (4)
#define HMB_TBL_SIZE                    (HMB_TBL_ENTRY_SIZE * HMB_LOG_ENTRY_NUM)

// The number of bytes occupied by the HMB bitmap. Address need qword align
#if (EN_MT_2CPU)
#define HMB_BITMAP_SIZE             (((HMB_HOST_BUFFER_SIZE / HMB_SUB_SLOT_SIZE) + 63) / 64 * 8)
#elif (EN_MT_4CPU)
#define HMB_BITMAP_SIZE_ALL         (((HMB_HOST_BUFFER_SIZE / HMB_SUB_SLOT_SIZE) + 63) / 64 * 8)
#define HMB_BITMAP_SIZE             (HMB_BITMAP_SIZE_ALL >> 1)
#endif
#endif

// data buf config
#if (EN_PROJECT_DRAM_LESS)
#ifdef FCL_CMD_SEQ_DEBUG
#define FCL_CMD_SEQ_BUF_NUM                  (4)
#define FCL_CMD_SEQ_DEBUG_BUF_REG_PLC        (BM_REQ_PLC_SRAM_ONLY)
#define FCL_CMD_SEQ_DEBUG_BUF_MIN_NUM        (FCL_CMD_SEQ_BUF_NUM)
#define FCL_CMD_SEQ_DEBUG_BUF_MAX_NUM        (FCL_CMD_SEQ_BUF_NUM)
#endif

// #define DATA_BUF_SRAM_LL_ENTRY_NUM  (112) // (256 + 128 + 64) KB / 4 KB = 112

#define WR_BUF_REG_PLC              (BM_REQ_PLC_SRAM_ONLY)
#define WR_BUF_MAX_NUM              (DATA_BUF_SRAM_LL_ENTRY_NUM)
#if defined(EN_HW_FORCE_POP)
#define WR_BUF_MIN_NUM          (0)
#else
#define WR_BUF_MIN_NUM          (FRAG_QNTY)
#endif

#define RD_BUF_REG_PLC              (BM_REQ_PLC_SRAM_ONLY)
// #define RD_BUF_MAX_NUM              (DATA_BUF_SRAM_LL_ENTRY_NUM)
#define RD_BUF_MIN_NUM              (MAX_CHAN_NUM * PFRAG_NUM)  // each ch keep 3 buf

#define FW_BUF_REG_PLC              (BM_REQ_PLC_SRAM_ONLY)
#define FW_BUF_MAX_NUM              (DATA_BUF_SRAM_LL_ENTRY_NUM)
#if ( FTL_DBMT_4K_NUM > FRAG_NUM )
#define FW_BUF_MIN_NUM              (FTL_DBMT_4K_NUM)
#else
#define FW_BUF_MIN_NUM              (FRAG_QNTY)
#endif

#define PARTIAL_WR_BUF_REG_PLC      (BM_REQ_PLC_SRAM_ONLY)
#if defined(COUGAR)
#define PARTIAL_WR_BUF_MAX_NUM      (1)
#else
#define PARTIAL_WR_BUF_MAX_NUM      (2)
#endif
#define PARTIAL_WR_BUF_MIN_NUM      (1)

#define DUMMY_BUF_REG_PLC           (BM_REQ_PLC_SRAM_ONLY)
#define DUMMY_BUF_MIN_NUM           (1)
#define DUMMY_BUF_MAX_NUM           (1)

#if defined(EN_TCG_OPAL) || defined(AES_ENABLE)
#define OPAL_BUF_REG_PLC        (BM_REQ_PLC_SRAM_ONLY)
#define OPAL_BUF_MIN_NUM        (3)
#define OPAL_BUF_MAX_NUM        (3)
#elif defined(AES_ENABLE)
#define OPAL_BUF_REG_PLC        (BM_REQ_PLC_SRAM_ONLY)
#define OPAL_BUF_MIN_NUM        (1)
#define OPAL_BUF_MAX_NUM        (1)
#endif

#define FROUNT_BUF_REG_PLC          (BM_REQ_PLC_SRAM_ONLY)
#define FROUNT_BUF_MIN_NUM          (1)
#ifdef EN_TCG_OPAL
#define OPAL_DYNAMIC_BUF_MAX_NUM    16
// OPAL + Download micro code + trim
#define FROUNT_BUF_MAX_NUM          (2 * FRAG_QNTY + OPAL_DYNAMIC_BUF_MAX_NUM + PFRAG_NUM + 1)
#else
// OPAL + Download micro code + trim
#define FROUNT_BUF_MAX_NUM          (2 * FRAG_QNTY + PFRAG_NUM + 1)
#endif
#define CODE_BLK_BUF_REG_PLC        (BM_REQ_PLC_SRAM_ONLY)
#define CODE_BLK_BUF_MIN_NUM        (0)
#define CODE_BLK_BUF_MAX_NUM        (DATA_BUF_SRAM_LL_ENTRY_NUM)

#if defined(COUGAR)
#define HC_W_DESC_DEPTH                         (0x1) // desc depth value is 0 base.
#define HC_W_AU_MAX_NUM                         (8 + 2 + (HC_W_DESC_DEPTH + 1)) // (WDP + WDP Cmp FIFO + Desc Depth) note: desc depth is 0 base.
#define WC_AU_MIN_NUM                           (PFRAG_NUM)
#define WC_AU_MAX_NUM                           (12)
#define WR_OVER_APPLICATION_AU_NUM              (0)
#define PARTIAL_TRIM_ALLOC_BUF_MIN_NUM          (1)
#elif defined(FALCON) || defined(FALCON_LITE)
#define HC_W_DESC_DEPTH                         (0x3) // desc depth value is 0 base.
#define HC_W_AU_MAX_NUM                         (8 + (HC_W_DESC_DEPTH + 1)) // (WDP(include Cmpl FIFO) + Desc Depth), desc depth is 0 base.
#define WC_AU_MIN_NUM                           (PFRAG_NUM)
#define WC_AU_MAX_NUM                           (PFRAG_NUM)
#define WR_OVER_APPLICATION_AU_NUM              (FRAG_QNTY) // 16xx need another FRAG_NUM, for better S.W performance
#define PARTIAL_TRIM_ALLOC_BUF_MIN_NUM          (PFRAG_NUM)
#endif

#define DOWNLOAD_BUF_MIN_NUM                    (PFRAG_NUM)
#define SLC_PU_NEED_BUF_MIN_NUM                 (FRAG_QNTY)
#define NOT_SLC_PU_NEED_BUF_MIN_NUM             (MAX_PAGES_PER_PU * FRAG_QNTY)
#if defined(CMT_LOG_EN)
// cmt log id table
#define CMT_LOG_NEED_BUF_MAX_NUM                (0x3)
#else
#define CMT_LOG_NEED_BUF_MAX_NUM                (0)
#endif

#define HOST_W_BUF_MAX_NUM_WHEN_NO_MUTEX        (HC_W_AU_MAX_NUM + WC_AU_MAX_NUM + WR_OVER_APPLICATION_AU_NUM + PARTIAL_TRIM_ALLOC_BUF_MIN_NUM + PFRAG_NUM) // PFRAG_NUM is for trim
#if defined (EN_TCG_OPAL) || defined(AES_ENABLE)
#if ((RD_BUF_MIN_NUM + DUMMY_BUF_MIN_NUM + FROUNT_BUF_MIN_NUM + DOWNLOAD_BUF_MIN_NUM + OPAL_BUF_MIN_NUM + NOT_SLC_PU_NEED_BUF_MIN_NUM + PARTIAL_WR_BUF_MAX_NUM + HOST_W_BUF_MAX_NUM_WHEN_NO_MUTEX + CMT_LOG_NEED_BUF_MAX_NUM) > DATA_BUF_SRAM_LL_ENTRY_NUM)
#error "HOST_R_BUF_MAX_NUM_NORM Error"
#endif
#if ((RD_BUF_MIN_NUM + DUMMY_BUF_MIN_NUM + FROUNT_BUF_MIN_NUM + OPAL_BUF_MIN_NUM + FW_BUF_MIN_NUM + PARTIAL_WR_BUF_MAX_NUM + WC_AU_MIN_NUM + PFRAG_NUM) > DATA_BUF_SRAM_LL_ENTRY_NUM)
#error "HOST_R_BUF_MAX_NUM_PRED Error"
#endif

#define HOST_R_BUF_MAX_NUM_NORM             (DATA_BUF_SRAM_LL_ENTRY_NUM - DUMMY_BUF_MIN_NUM - FROUNT_BUF_MIN_NUM - DOWNLOAD_BUF_MIN_NUM - OPAL_BUF_MIN_NUM - NOT_SLC_PU_NEED_BUF_MIN_NUM - PARTIAL_WR_BUF_MAX_NUM - HOST_W_BUF_MAX_NUM_WHEN_NO_MUTEX - CMT_LOG_NEED_BUF_MAX_NUM)
#define HOST_R_BUF_MAX_NUM_PRED             (DATA_BUF_SRAM_LL_ENTRY_NUM - DUMMY_BUF_MIN_NUM - FROUNT_BUF_MIN_NUM - OPAL_BUF_MIN_NUM - FW_BUF_MIN_NUM - PARTIAL_WR_BUF_MAX_NUM - (WC_AU_MIN_NUM + PFRAG_NUM))
#else
#if ((RD_BUF_MIN_NUM + DUMMY_BUF_MIN_NUM + FROUNT_BUF_MIN_NUM + DOWNLOAD_BUF_MIN_NUM + NOT_SLC_PU_NEED_BUF_MIN_NUM + PARTIAL_WR_BUF_MAX_NUM + HOST_W_BUF_MAX_NUM_WHEN_NO_MUTEX + CMT_LOG_NEED_BUF_MAX_NUM) > DATA_BUF_SRAM_LL_ENTRY_NUM)
#error "HOST_R_BUF_MAX_NUM_NORM Error"
#endif
#if ((RD_BUF_MIN_NUM + DUMMY_BUF_MIN_NUM + FROUNT_BUF_MIN_NUM + FW_BUF_MIN_NUM + PARTIAL_WR_BUF_MAX_NUM + WC_AU_MIN_NUM + PFRAG_NUM) > DATA_BUF_SRAM_LL_ENTRY_NUM)
#error "HOST_R_BUF_MAX_NUM_PRED Error"
#endif

#define HOST_R_BUF_MAX_NUM_NORM             (DATA_BUF_SRAM_LL_ENTRY_NUM - DUMMY_BUF_MIN_NUM - FROUNT_BUF_MIN_NUM - DOWNLOAD_BUF_MIN_NUM - NOT_SLC_PU_NEED_BUF_MIN_NUM - PARTIAL_WR_BUF_MAX_NUM - HOST_W_BUF_MAX_NUM_WHEN_NO_MUTEX - CMT_LOG_NEED_BUF_MAX_NUM)
#define HOST_R_BUF_MAX_NUM_PRED             (DATA_BUF_SRAM_LL_ENTRY_NUM - DUMMY_BUF_MIN_NUM - FROUNT_BUF_MIN_NUM - FW_BUF_MIN_NUM - PARTIAL_WR_BUF_MAX_NUM - (WC_AU_MIN_NUM + PFRAG_NUM))
#endif

#define RD_BUF_MAX_NUM              (HOST_R_BUF_MAX_NUM_NORM)

#ifdef DESC_USE_WAR
#define WR_BUF_REAL_MAX_NUM     (DATA_BUF_SRAM_LL_ENTRY_NUM - RD_BUF_MIN_NUM - FW_BUF_MIN_NUM - PARTIAL_WR_BUF_MIN_NUM - DUMMY_BUF_MIN_NUM - FROUNT_BUF_MIN_NUM)
#endif
#endif

// write cache
#define WRITE_CACHE_DATA_UNIT_MIN_NUM    (MAX_CH_NUM * STPU_NUM / DATA_BUF_UNIT_SECTOR_NUM)
#define WRITE_CACHE_DATA_UNIT_NUM        ((WRITE_CACHE_DATA_UNIT_MIN_NUM < WR_BUF_MAX_NUM) ? WR_BUF_MAX_NUM : WRITE_CACHE_DATA_UNIT_MIN_NUM)
#if (OPT_CO_SIM_NO_NAND)
#define WRITE_CAHCE_HASH_TBL_ENTRY_NUM   MEM_ALIGN((WRITE_CACHE_DATA_UNIT_MIN_NUM * 2), 8)
#else
#if (OPT_HASH_ENGINE_UT)
#define WRITE_CAHCE_HASH_TBL_ENTRY_NUM   (128)
#else
#define WRITE_CAHCE_HASH_TBL_ENTRY_NUM   (WRITE_CACHE_DATA_UNIT_NUM / WRITE_CACHE_DATA_UNIT_MIN_NUM * WRITE_CACHE_DATA_UNIT_MIN_NUM * 2)
#endif
#endif
#define WRITE_CAHCE_HASH_TBL_ENTRY_SHIFT (8)
#define WRITE_CAHCE_HASH_TBL_ENTRY_MASK  (WRITE_CAHCE_HASH_TBL_ENTRY_NUM - 1)

// fw desc
#if 0//(VCECH_NUM >= 64)
#define FW_DESC_FOR_PROG_MAX_NUM  (VCECH_NUM * FRAG_QNTY * 3 / 2) //to avoid DESC tbl size exceed B1TCM size, but affect Prog perf.
#else
#define FW_DESC_FOR_PROG_MAX_NUM  (VDIE_NUM * FRAG_QNTY * 2)
#endif
#define FW_DESC_FOR_WRITE_CACHE_MAX_NUM  (WRITE_CACHE_DATA_UNIT_NUM)
#if defined(COUGAR)
#define FW_DESC_FOR_HOST_RD_MAX_NUM  (384)
#elif defined(FALCON_LITE)
// HC regulations rd max num
#define FW_DESC_FOR_HOST_RD_MAX_NUM  (768)
#else
#error "FW_DESC_FOR_HOST_RD_MAX_NUM error"
#endif

#define FW_CMD_DESC_NUM_CAL     (FW_DESC_FOR_PROG_MAX_NUM + FW_DESC_FOR_HOST_RD_MAX_NUM + FW_DESC_FOR_WRITE_CACHE_MAX_NUM)

#if defined(COUGAR)
#if defined(EN_FCL_30)
#define FW_CMD_DESC_NUM_MAX      (740)
#else
#define FW_CMD_DESC_NUM_MAX      (832)
#endif
#else
// falcon & falcon lite
// 0x700, can set smaller for descriptor request fail test
#define FW_CMD_DESC_NUM_MAX      (1792)
#endif

#define FW_CMD_DESC_NUM (FW_CMD_DESC_NUM_MAX)  // need to fix in 4CH/1CE for QBT write
//#define FW_CMD_DESC_NUM ((FW_CMD_DESC_NUM_CAL > FW_CMD_DESC_NUM_MAX) ? FW_CMD_DESC_NUM_MAX : FW_CMD_DESC_NUM_CAL)
#if (OPT_DESC_USE_WAR)
#define HR_OS_AU_MAX_NUM     (384) //512
#define FW_POP_DESC_THR      (WR_BUF_REAL_MAX_NUM + HR_OS_AU_MAX_NUM)
#endif
#if (EN_CODE_SWAP) && (OPT_CODE_SWAP_BY_BUF)
#if (defined(COARSE_FINE_PROG) && defined(COUGAR))
// set 128KB code for PBT swap and UEFI
#define SWAP_CMD_BUF_NUM      (24)
#else
// set 128KB code for PBT swap and UEFI
#define SWAP_CMD_BUF_NUM      (32)
#endif
#endif

#if (EN_DESC1)
#define FW_CMD_DESC1_NUM_MIN (16)
#define FW_CMD_DESC1_NUM_MAX (20)
#define FW_CMD_DESC1_NUM ((FW_CMD_DESC_NUM_CAL > FW_CMD_DESC_NUM_MAX) ? (((FW_CMD_DESC_NUM_CAL - FW_CMD_DESC_NUM_MAX) > FW_CMD_DESC1_NUM_MAX) ? FW_CMD_DESC1_NUM_MAX : (((FW_CMD_DESC_NUM_CAL - FW_CMD_DESC_NUM_MAX) > FW_CMD_DESC1_NUM_MIN) ? (FW_CMD_DESC_NUM_CAL - FW_CMD_DESC_NUM_MAX) : FW_CMD_DESC1_NUM_MIN)) : FW_CMD_DESC1_NUM_MIN)
#else
#define FW_CMD_DESC1_NUM (0)
#endif

#if (OPT_PCIE_LTSSM_STATE_DEBUG)
#define ERROR_BUF_LEN                   (4096 * 1)
#elif (EN_SAVE_LOG)
#define LOG_FLUSH_THR     (4 * 1024)
#define LOG_BUFF_LEN      (LOG_FLUSH_THR)
#define LOG_CACHE_LEN     (1 * 1024)
#define LOG_TOTAL_LEN     (LOG_BUFF_LEN + LOG_CACHE_LEN)
#endif

#if (!EN_PSLC)
#if defined(QLC_NAND)
#define MAX_PAGES_PER_PU        (4)
#elif defined(TLC_NAND)
#define MAX_PAGES_PER_PU        (3)
#endif
#else
#define MAX_PAGES_PER_PU    (3)
#endif

#endif  // FW_FCG_H