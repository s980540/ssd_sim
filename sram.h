#ifndef SRAM_H
#define SRAM_H

#include "global.h"
#include "types.h"

// #include "hw_cfg.h"
#include "sys_mem.h"
#include "fw_cfg.h"
#include "fcl_def.h"
#include "uart.h"

#include "tcm.h"

// fc cmd
#if (OPT_TCM_DESC)
#define FC_FW_SUBMISSON_Q_BASE      (SRAM_BASE)
#else
#define FC_FW_SUBMISSON_Q_BASE      (FW_MEM_ADDR_ALIGNED(FW_CMD_DESC_END))
#endif
#define FC_FW_SUBMISSON_Q_END \
    (FC_FW_SUBMISSON_Q_BASE         + FCL_FC_CMD_FMT_SIZE * FLIC_FC_CMD_MAX_NUM)

//
#define FC_FW_SUBMISSON_Q1_BASE     (FW_MEM_ADDR_ALIGNED(FC_FW_SUBMISSON_Q_END))
#define FC_FW_SUBMISSON_Q1_END \
    (FC_FW_SUBMISSON_Q1_BASE        + FCL_FC_CMD_FMT_SIZE * FLIC_FC_CMD1_MAX_NUM)

//
#define FC_FW_HP_SUBMISSON_Q_BASE   (FW_MEM_ADDR_ALIGNED(FC_FW_SUBMISSON_Q1_END))
#define FC_FW_HP_SUBMISSON_Q_END \
    (FC_FW_HP_SUBMISSON_Q_BASE      + FCL_FC_CMD_FMT_SIZE * FLIC_FC_CMD_HP_MAX_NUM)

//
#define FC_FW_SINGLE_SUBMISSON_Q_BASE   (FW_MEM_ADDR_ALIGNED(FC_FW_HP_SUBMISSON_Q_END))
#define FC_FW_SINGLE_SUBMISSON_Q_END \
    (FC_FW_SINGLE_SUBMISSON_Q_BASE  + FCL_FC_CMD_FMT_SIZE + FCL_FC_CMD_STS_FMT_SIZE)

//
#define FC_FW_CMPL_Q0_BASE          (FW_MEM_ADDR_ALIGNED(FC_FW_SINGLE_SUBMISSON_Q_END))
#define FC_FW_CMPL_Q0_END \
    (FC_FW_CMPL_Q0_BASE + FCL_FC_CMD_STS_FMT_SIZE * FLIC_FC_CQ_MAX_NUM)

//
#define FC_FW_CMPL_Q1_BASE          (FW_MEM_ADDR_ALIGNED(FC_FW_CMPL_Q0_END))
#define FC_FW_CMPL_Q1_END \
    (FC_FW_CMPL_Q1_BASE + FCL_FC_CMD_STS_FMT_SIZE * FLIC_FC_CQ1_MAX_NUM)

//
#define FC_HP_CMPL_Q_BASE           (FW_MEM_ADDR_ALIGNED(FC_FW_CMPL_Q1_END))
#define FC_HP_CMPL_Q_END \
    (FC_HP_CMPL_Q_BASE  + FCL_FC_CMD_STS_FMT_SIZE * FLIC_FC_CMD_HP_MAX_NUM)

#define FC_FWSQ0_BASE               (FC_FW_SUBMISSON_Q_BASE)
#define FC_FWSQ1_BASE               (FC_FW_SUBMISSON_Q1_BASE)
#define FC_HPSQ_BASE                (FC_FW_HP_SUBMISSON_Q_BASE)
#define FC_FWSS_BASE                (FC_FW_SINGLE_SUBMISSON_Q_BASE)

#define FC_FWCQ0_BASE               (FC_FW_CMPL_Q0_BASE)
#define FC_FWCQ1_BASE               (FC_FW_CMPL_Q1_BASE)
#define FC_HPCQ_BASE                (FC_HP_CMPL_Q_BASE)

// data buffer sram ll table base
#define DATA_BUF_SRAM_LL_BASE       (FW_MEM_ADDR_ALIGNED(FC_HP_CMPL_Q_END))
#define DATA_BUF_SRAM_LL_END \
    (DATA_BUF_SRAM_LL_BASE + DATA_BUF_SRAM_LL_ENTRY_NUM * 2)

// data buffer dram ll table base
#define DATA_BUF_DRAM_LL_BASE       (FW_MEM_ADDR_ALIGNED(DATA_BUF_SRAM_LL_END))
#define DATA_BUF_DRAM_LL_END \
    (DATA_BUF_DRAM_LL_BASE + DATA_BUF_DRAM_LL_ENTRY_NUM * 2)

// data buffer sram data base
#define DATA_BUF_SRAM_BASE          (FW_MEM_ADDR_ALIGNED(DATA_BUF_DRAM_LL_END))
#define DATA_BUF_SRAM_END \
    (DATA_BUF_SRAM_BASE + DATA_BUF_UNIT_SIZE * DATA_BUF_SRAM_LL_ENTRY_NUM)

#if (OPT_CODE_SWAP_BY_BUF)
#define SWAP_CMD_BUF_BASE \
    (DATA_BUF_SRAM_BASE + (DATA_BUF_SRAM_LL_ENTRY_NUM - SWAP_CMD_BUF_NUM) * DATA_BUF_UNIT_SIZE)
#define SWAP_CMD_BUF_END \
    (SWAP_CMD_BUF_BASE + SWAP_CMD_BUF_NUM * DATA_BUF_UNIT_SIZE)

#if (DATA_BUF_SRAM_LL_ENTRY_NUM < SWAP_CMD_BUF_NUM)
    #error "CMD_BUF sub over"
#endif
#endif

#if (OPT_NAND_MEM)
#if (OPT_UCODE_IN_FW)
#define UCODE_BUF_NUM               (4) // 4 * DATA_BUF_UNIT_SIZE = at least 16k
#define UCODE_BUF_ID \
    (DATA_BUF_SRAM_LL_ENTRY_NUM - SWAP_CMD_BUF_NUM - UCODE_BUF_NUM)
#define UCODE_BUF_BASE_ADDR         (DATA_BUF_SRAM_BASE + DATA_BUF_UNIT_SIZE * UCODE_BUF_ID)
#define UCODE_BUF_END               (UCODE_BUF_BASE_ADDR + UCODE_BUF_NUM * DATA_BUF_UNIT_SIZE)
// After UCODE_BUF_BASE_ADDR, MUST BE SAME AS ADDR IN autobuild
#define LDR_MEMORY_BASE_ADDR        (0x4084000)
// before SWAP_CODE_BASE(&Image$$SWAP_CODE$$Base)
#define LDR_MEMORY_END              (LDR_MEMORY_BASE_ADDR + (16 * 1024))
#if ((LDR_MEMORY_BASE_ADDR < UCODE_BUF_BASE_ADDR))
#error "LDR_MEMORY_BASE_ADDR error"
#endif
#else
#define UCODE_BUF_ID                (1)
#define LDR_MEMORY_BASE_ADDR        (DATA_BUF_SRAM_BASE + DATA_BUF_UNIT_SIZE * UCODE_BUF_ID)  //ldr save nand memory base addr, the first 4K for dummy ptr
#define LDR_MEMORY_END              (LDR_MEMORY_BASE_ADDR + (16 * 1024))
#if ((LDR_MEMORY_BASE_ADDR < DATA_BUF_SRAM_BASE) || (LDR_MEMORY_END > DATA_BUF_SRAM_END))
#error "LDR_MEMORY_BASE_ADDR error"
#endif
#endif
#endif

#if (EN_UEFI_FEATURE)
#define UEFI_CODE_SIZE              (64 * 1024)
#define UEFI_CODE_BASE              (DATA_BUF_SRAM_END)
#define UEFI_CODE_END               (UEFI_CODE_BASE + UEFI_CODE_SIZE)
#else
#define UEFI_CODE_BASE              (DATA_BUF_SRAM_END)
#define UEFI_CODE_END               (DATA_BUF_SRAM_END)
#endif

#if (EN_FCL_UNIT_TEST_30)
#define UT_CMD_SRAM_BASE            (FW_MEM_ADDR_ALIGNED(UEFI_CODE_END))
#define UT_CMD_SRAM_END             (UT_CMD_SRAM_BASE + DATA_BUF_UNIT_SIZE )

#define UT_CASE_SRAM_BASE           (FW_MEM_ADDR_ALIGNED(UT_CMD_SRAM_END))
#define UT_CASE_SRAM_END            (UT_CASE_SRAM_BASE + DATA_BUF_UNIT_SIZE )

#define UT_BLOCK_SRAM_BASE          (FW_MEM_ADDR_ALIGNED(UT_CASE_SRAM_END))
#define UT_BLOCK_SRAM_END           (UT_BLOCK_SRAM_BASE + DATA_BUF_UNIT_SIZE )

#define UT_INFO_SRAM_BASE           (FW_MEM_ADDR_ALIGNED(UT_BLOCK_SRAM_END))
#define UT_INFO_SRAM_END            (UT_INFO_SRAM_BASE + DATA_BUF_UNIT_SIZE )

#define SRAM_TEMP0_END              (UT_INFO_SRAM_END)
#else
#define SRAM_TEMP0_END              (UEFI_CODE_END)
#endif

// blk vc
#if (OPT_TRIM_FULL_DISK_CHK)
#define FTL_BLK_VC_BASE             (FW_MEM_ADDR_ALIGNED(SRAM_TEMP0_END))
#define FTL_BLK_VC_END              (FTL_BLK_VC_BASE + VBLOCK_NUM*4)
#else
#define FTL_BLK_VC_END              (FW_MEM_ADDR_ALIGNED(SRAM_TEMP0_END))
#endif

// buf
#if (EN_TBL_31)
#define MERGE_USED_BUF_BASE         (FW_MEM_ADDR_ALIGNED(FTL_BLK_VC_END))
#define MERGC_USED_BUF_END          (MERGE_USED_BUF_BASE + VDIE_QNTY * FRAG_NUM * 4)
#define DATA_CRC_BUF_BASE           (FW_MEM_ADDR_ALIGNED(MERGC_USED_BUF_END))
#else
// for data nor crc 2 crc
#define DATA_CRC_BUF_BASE           (FW_MEM_ADDR_ALIGNED(FTL_BLK_VC_END))
#endif
#define DATA_CRC_BUF_END            (DATA_CRC_BUF_BASE + DATA_BUF_UNIT_SIZE * 1)

#define UNC_DEL_BUF_BASE            (FW_MEM_ADDR_ALIGNED(DATA_CRC_BUF_END))
#define UNC_DEL_BUF_END             (UNC_DEL_BUF_BASE + 1024)

// rct
#define RCT_MEM_SIZE                (RCT_SRAM_ENTRY_NUM * FTL_RCT_UNIT_SIZE)
#if (EN_MT_2CPU)
#define RCT_MEM_BASE                (FW_MEM_ADDR_ALIGNED(UNC_DEL_BUF_END))
#define RCT_MEM_END                 (RCT_MEM_BASE + RCT_MEM_SIZE)
#elif (EN_MT_4CPU)
// for CPU2
#define RCT_MEM_BASE0               (FW_MEM_ADDR_ALIGNED(UNC_DEL_BUF_END))
#define RCT_MEM_END0                (RCT_MEM_BASE0 + RCT_MEM_SIZE)
// for CPU3
#define RCT_MEM_BASE1               (FW_MEM_ADDR_ALIGNED(RCT_MEM_END0))
#define RCT_MEM_END1                (RCT_MEM_BASE1 + RCT_MEM_SIZE)

#define RCT_MEM_BASE                (RCT_MEM_BASE0)
#define RCT_MEM_END                 (RCT_MEM_END1)
#endif

#define RCT_TBL_DUMMY_BASE          (RCT_MEM_END)
#define RCT_TBL_DUMMY_END           (RCT_TBL_DUMMY_BASE)

#define GC_RCT_MEM_BASE             (RCT_TBL_DUMMY_END)
#define GC_RCT_MEM_END              (GC_RCT_MEM_BASE)

#if (OPT_HMB_RCT)
#define RCT_HMB_BUFF_BASE           QW_ALIGNED(GC_RCT_MEM_END)
#define RCT_HMB_BUFF_END \
    (RCT_HMB_BUFF_BASE + FTL_DATA_L2P_ENTRY_SIZE * RCT_HMB_BUFF_ENTRY_NUM)
#else
#define RCT_HMB_BUFF_END            QW_ALIGNED(GC_RCT_MEM_END)
#endif

// pbt vc
// Refer to Ant, if hmb on, use RCT memory, should ensure PBT_VC_BASE in rct region
#define PBT_VC_BASE                 (FW_MEM_ADDR_ALIGNED(RCT_MEM_BASE))
// #define PBT_VC_BASE                 (FW_MEM_ADDR_ALIGNED(FW_BUF_SRAM_BASE))
#define PBT_VC_END                  (PBT_VC_BASE + VBLOCK_QNTY * 4)
#define PBT_CHK_VC_BASE             (FW_MEM_ADDR_ALIGNED(PBT_VC_END))
#define PBT_CHK_VC_END              (PBT_CHK_VC_BASE + VBLOCK_QNTY * 4)

// multiplex in PBT for temporary use
#if (OPT_CMT_VFC_USE_HWBV)
#define PBT_CMT_BUILD_VC_BASE       (FW_MEM_ADDR_ALIGNED(PBT_CHK_VC_END))
#define PBT_CMT_BUILD_VC_END        (PBT_CMT_BUILD_VC_BASE + FW_MEM_ADDR_ALIGNED(VBLOCK_QNTY*4))
#endif

#if RCT_HMB_BUFF_END <= PBT_CMT_BUILD_VC_END
#error "pbt temp overlap err"
#endif

// -- tcm --
// desc1
#if (EN_DESC1)
#define FW_CMD_DESC1_BASE           (ATA_MEM_ADDR_ALIGNED(RCT_HMB_BUFF_END))  // UART_BUF_END CHT_MEM_END
#if (META_DATA_SIZE == META_SIZE_6DW)
    #define FW_CMD_DESC1_END  (FW_CMD_DESC1_NUM * 48 + FW_CMD_DESC1_BASE)
#elif (META_DATA_SIZE == META_SIZE_4DW)
    #define FW_CMD_DESC1_END  (FW_CMD_DESC1_NUM * 40 + FW_CMD_DESC1_BASE)
#elif (META_DATA_SIZE == META_SIZE_2DW)
    #define FW_CMD_DESC1_END  (FW_CMD_DESC1_NUM * 32 + FW_CMD_DESC1_BASE)
#else
    #error "META_DATA_SIZE NOT SUPPORT2."
#endif
#if FW_CMD_DESC1_BASE <= FW_CMD_DESC_END
    #error "Two desc overlap err"
#endif
#else
// #define FW_CMD_DESC1_BASE           (FW_CMD_DESC0_BASE)
#define FW_CMD_DESC1_END            (RCT_HMB_BUFF_END)
#endif

// hmb aes
#if (OPT_HMB_RCT) && (OPT_HMB_AES)
// ***DPU need QWORD alignment
#define HMB_AES_RD_BASE             (QW_ALIGNED(FW_CMD_DESC1_END))
#define HMB_AES_RD_SIZE             (16)
#define HMB_AES_RD_END              (HMB_AES_RD_BASE + HMB_AES_RD_SIZE)
#else
#define HMB_AES_RD_END              (DW_ALIGNED(FW_CMD_DESC1_END))
#endif

// wai
#if (EN_WAI_TEST)
#define WAI_TEST_MEM_BASE           (QW_ALIGNED(HMB_AES_RD_END))
#define WAI_TEST_MEM_SIZE           (400)
#define g_ftl_wai_caclu             ((volatile ftl_wai_caclu *)WAI_TEST_MEM_BASE)
#define WAI_TEST_MEM_END            (WAI_TEST_MEM_BASE + WAI_TEST_MEM_SIZE)
#else
#define WAI_TEST_MEM_END            (DW_ALIGNED(HMB_AES_RD_END))
#endif

// bad map
#define BAD_MAP_SIZE                (0)
#define BAD_MAP_BASE                (DW_ALIGNED(WAI_TEST_MEM_END))
#define BAD_MAP_END                 (BAD_MAP_BASE + BAD_MAP_SIZE)

// sn
#define SN_BASE                     (FW_MEM_ADDR_ALIGNED(BAD_MAP_END))
#define SN_SIZE                     (FW_GDMA_ALIGNED(VBLOCK_QNTY * 4))
#define SN_END                      (SN_BASE + SN_SIZE)

// trim
#define TRIM_TBL_BASE               (FW_MEM_ADDR_ALIGNED(SN_END))
#define TRIM_TBL_END                (TRIM_TBL_BASE + DATA_BUF_UNIT_ALINE_SIZE) // 4K

//
#define TEMP_TCM_FOR_BT             (TRIM_TBL_END + 8) // use to store phfr_info after chkptr_back + trim_buffer

//
#if (EN_FCL_30)
#define ONE_FIFO_ENTRY_SIZE         (2)
#if defined(OPT_FCL_30_ADMIN_USE_FIFO)
#define FCL_ADMIN_ENTRY_NUM         (8)
#else
#define FCL_ADMIN_ENTRY_NUM         (0)
#endif
#define FCL_IO_ENTRY_NUM            (256)
#define FCL_FIFO_SIZE_ADMIN_SQ      (FCL_ADMIN_ENTRY_NUM * ONE_FIFO_ENTRY_SIZE)
#define FCL_FIFO_SIZE_IO_SQ         (FCL_IO_ENTRY_NUM * ONE_FIFO_ENTRY_SIZE)
#define FCL_FIFO_SIZE_ADMIN_CQ      (FCL_ADMIN_ENTRY_NUM * ONE_FIFO_ENTRY_SIZE)
#define FCL_FIFO_SIZE_IO_CQ         (FCL_IO_ENTRY_NUM * ONE_FIFO_ENTRY_SIZE)

//
#define FCL_FIFO_ADMIN_SQ_MEM_BASE  (FW_MEM_ADDR_ALIGNED(TEMP_TCM_FOR_BT))
#define FCL_FIFO_ADMIN_SQ_MEM_END   (FCL_FIFO_ADMIN_SQ_MEM_BASE + FCL_FIFO_SIZE_ADMIN_SQ * 4)

//
#define FCL_FIFO_ADMIN_CQ_MEM_BASE  (FW_MEM_ADDR_ALIGNED(FCL_FIFO_ADMIN_SQ_MEM_END))
#define FCL_FIFO_ADMIN_CQ_MEM_END   (FCL_FIFO_ADMIN_CQ_MEM_BASE + FCL_FIFO_SIZE_ADMIN_CQ * 4)

//
#define FCL_FIFO_IO_SQ_MEM_BASE     (FW_MEM_ADDR_ALIGNED(FCL_FIFO_ADMIN_CQ_MEM_END))
#define FCL_FIFO_IO_SQ_MEM_END      (FCL_FIFO_IO_SQ_MEM_BASE + FCL_FIFO_SIZE_IO_SQ * 4)

//
#define FCL_FIFO_IO_CQ_MEM_BASE     (FW_MEM_ADDR_ALIGNED(FCL_FIFO_IO_SQ_MEM_END))
#define FCL_FIFO_IO_CQ_MEM_END      (FCL_FIFO_IO_CQ_MEM_BASE + FCL_FIFO_SIZE_IO_CQ * 4)
#else
#define FCL_FIFO_IO_CQ_MEM_END      (FW_MEM_ADDR_ALIGNED(TEMP_TCM_FOR_BT))
#endif

// for 128GB 2LUN
#if (LUN_SHIFT != 0)
#define MT_TBL_BID_BASE             (FW_MEM_ADDR_ALIGNED(FCL_FIFO_IO_CQ_MEM_END))
#define MT_TBL_BID_SIZE             (VTBL_BLK_MAX_NUM * 2 + 4 + VTBL_BLK_MAX_NUM)
#define g_mt_tbl_bid                ((volatile ftl_tbmt_bid *)MT_TBL_BID_BASE)
#define MT_TBL_BID_END              (MT_TBL_BID_BASE + MT_TBL_BID_SIZE)
#endif

// fw cmd desc
#define FW_BUF_LL_SIZE              (4)
#if (LUN_SHIFT != 0)
#define FW_BUF_SRAM_BASE            (FW_MEM_ADDR_ALIGNED(MT_TBL_BID_END))
#else
#define FW_BUF_SRAM_BASE            (FW_MEM_ADDR_ALIGNED(FCL_FIFO_IO_CQ_MEM_END))
#endif
// 4 bytes for link
#define FW_BUF_SRAM_END             (FW_BUF_SRAM_BASE + (DATA_BUF_UNIT_SIZE + FW_BUF_LL_SIZE) * FW_BUF_SRAM_ENTRY_NUM)

// error buf
#if (OPT_PCIE_LTSSM_STATE_DEBUG)
#define ERROR_BUF_START             (DW_ALIGNED(FW_BUF_SRAM_END))
#define ERROR_BUF_END               (ERROR_BUF_START + ERROR_BUF_LEN)
#elif (EN_SAVE_LOG)
#define ERROR_BUF_START             (DW_ALIGNED(FW_BUF_SRAM_END))
#define ERROR_BUF_END               (ERROR_BUF_START + LOG_TOTAL_LEN)
#else
#define ERROR_BUF_END               (DW_ALIGNED(FW_BUF_SRAM_END))
#endif

// vfc
#if (OPT_SEPERATE_VFC)
#define VFC_BASE                    (FW_MEM_ADDR_ALIGNED(ERROR_BUF_END))
#define VFC_SIZE                    (FW_GDMA_ALIGNED(VBLOCK_QNTY * 4))
#define VFC_END                     (VFC_BASE + VFC_SIZE)
#else
#define VFC_END                     (ERROR_BUF_END)
#endif

// uart
#if (OPT_USE_UART_INT)
#define UART_BUF_LEN                (CONFIG_UART_TX_BUF_SIZE)
#define TX_BUF_HEAD                 (*(volatile word *)(ATA_MEM_ADDR_ALIGNED(VFC_END)))  // MAP_ID_2_BLK_END
#define TX_BUF_TAIL                 (*(volatile word *)(ATA_MEM_ADDR_ALIGNED(VFC_END) + 2))
#define UART_BUF_START              (ATA_MEM_ADDR_ALIGNED(VFC_END) + 4)
#define UART_BUF_END                (UART_BUF_START + UART_BUF_LEN)
#else
#define UART_BUF_START              (ATA_MEM_ADDR_ALIGNED(VFC_END))
#define UART_BUF_END                (ATA_MEM_ADDR_ALIGNED(VFC_END))
#endif

#define SRAM_USED_END               (UART_BUF_END)

// CPU cache from 0x4084800 for NOT enough MPU slot

// tbmt tbl
#if defined(FALCON) || defined(FALCON_LITE)
// #define TBMT_MEM_BASE               (0x40bf000)
#define TBMT_MEM_BASE               (UART_BUF_END)
#endif
#define TBMT_MEM_END                (TBMT_MEM_BASE + FTL_TBMT_ENTRY_NUM * 3)

// blk unit
#define BLK_UNIT_BASE               (FW_MEM_ADDR_ALIGNED(TBMT_MEM_END))

#if (OPT_SEPERATE_VFC) && (!OPT_SEPERATE_VFC_DBG)
#define BU_VFC_SIZE                 (0)
#else
#define BU_VFC_SIZE                 (4)
#endif

#if (OPT_NEW_SN_SORT)
#define BU_SN_SIZE                  (0)
#else
#define BU_SN_SIZE                  (4)
#endif

#if (EN_READ_DISTURB)
#define BU_RD_DISTURB_SIZE          (4)
#else
#define BU_RD_DISTURB_SIZE          (0)
#endif

#if (EN_VIRTUAL_DIE) || (EN_VTR)
#define CACL_PART_CNT0              (PARTITION_QNTY)
#define CACL_PART_CNT               (CACL_PART_CNT0 + (CACL_PART_CNT0 & 0x1))
#define BU_VD_PBN_SIZE              (CACL_PART_CNT * 2)
#else
#define BU_VD_PBN_SIZE              (0)
#endif

#define ONE_BLK_UNIT_SIZE \
    (12 + BU_VFC_SIZE + BU_SN_SIZE + BU_RD_DISTURB_SIZE + BU_VD_PBN_SIZE)
#define BLK_UNIT_SIZE               ((ONE_BLK_UNIT_SIZE) * VBLOCK_QNTY)
#define BLK_UNIT_END                (FW_MEM_ADDR_ALIGNED(BLK_UNIT_BASE + BLK_UNIT_SIZE))

// FOR 2CPU CHT + CMT
// 4CPU, CHT is at B0TCM
#if (EN_MT_2CPU)
#define CHT_MEM_BASE                (FW_MEM_ADDR_ALIGNED(BLK_UNIT_END))
// 2 bytes each CHT entry
#define CHT_MEM_END                 (CHT_MEM_BASE + FTL_CHT_ENTRY_NUM * 2)
#define CMT_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(CHT_MEM_END))
#define CMT_BITMAP_END              (CMT_BITMAP_BASE + FTL_CMT_BITMAP_SIZE)
#endif

#define SRAM_USED_CACHE_BASE        (TBMT_MEM_BASE)

#if (!EN_RAMDISK)
// Check the region of overlap between sram mem and cache
#if ((SRAM_USED_END - 0x10000) > SRAM_USED_CACHE_BASE)
#error "SRAM_LAYOUT ERR (0x10000)"
#elif ((SRAM_USED_END - 0x9500) > SRAM_USED_CACHE_BASE)
#error "SRAM_LAYOUT ERR (0x9500)"
#elif ((SRAM_USED_END - 0x5000) > SRAM_USED_CACHE_BASE)
#error "SRAM_LAYOUT ERR (0x5000)"
#elif ((SRAM_USED_END - 0x4000) > SRAM_USED_CACHE_BASE)
#error "SRAM_LAYOUT ERR (0x4000)"
#elif (SRAM_USED_END > SRAM_USED_CACHE_BASE)
#error "SRAM_LAYOUT ERR"
#endif
#endif

#if (EN_MT_2CPU)
#define SRAM_USED_CACHE_END         (CMT_BITMAP_END)
#endif

#if (EN_MT_4CPU)
#define SRAM_USED_CACHE_END         (BLK_UNIT_END)
#endif

#if defined(EN_CPU3)
extern unsigned int Image$$CODE$$Base;
#define SRAM_CHECK_END              (unsigned int)&Image$$CODE$$Base
// #define SRAM_CHECK_END              (SRAM_BASE + 0x100000 - 0x28000 - 0x4000)
#endif

// code bank
#if (EN_CODE_BANK)
#define CODE_BANK_BASE0             (0x40d2000)
#define CODE_BANK_SIZE0             (16 * 1024)
#define CODE_BANK_END0              (0x40d6000)
#define CODE_BANK_BASE1             (0x40dc000)
#define CODE_BANK_SIZE1             (16 * 1024)
#define CODE_BANK_END1              (0x40e0000)
#endif

// dram
#define FTL_DATA_L2P_BASE           (DRAM_MEM_BASE)

// l2p dram data base
#if (EN_CO_SIM)
#define FTL_DATA_L2P_ENTRY_NUM      (256)
#else
#if (EN_TCG_OPAL)
// mbr laa num: 128MB = 8000 * 4k
#define OPAL_MBR_ENTRY_NUM          (0x8000)
// ds laa num : 10MB  = a00 * 4k
#define OPAL_DATASTORE_ENTRY_NUM    (0x0A00)
// laa num(mbr + datastore)
#define OPAL_L2P_ENTRY_NUM          (OPAL_MBR_ENTRY_NUM + OPAL_DATASTORE_ENTRY_NUM)
// mbr lba num : 8000 * 8 * 512 bytes
#define OPAL_MBR_LBA_SIZE           (OPAL_MBR_ENTRY_NUM << SECTOR_SHIFT)
// ds lba num : a00 * 8 * 512 bytes
#define OPAL_DATASTORE_LBA_SIZE     (OPAL_DATASTORE_ENTRY_NUM << SECTOR_SHIFT)
// lba num
#define OPAL_LBA_SIZE               (OPAL_L2P_ENTRY_NUM << SECTOR_SHIFT)
// MBR start laa must RCT_UNIT align
#define FTL_MBR_START_LAA_ALIGN     (FTL_RCT_UNIT_SIZE)
// MAX laa + 1 MBR
#define FTL_DATA_L2P_ENTRY_NUM_1 \
    (((((NATIVE_MAX_LBA + SECTOR_NUM) >> SECTOR_SHIFT) + FTL_MBR_START_LAA_ALIGN) >> FTL_RCT_UNIT_SIZE_SHIFT ) << FTL_RCT_UNIT_SIZE_SHIFT)
// opal start lba
#define OPAL_START_LBA              (FTL_DATA_L2P_ENTRY_NUM_1 << SECTOR_SHIFT)
// max laa +1 + MBR :  DataStore
#define FTL_DATA_L2P_ENTRY_NUM_2    (OPAL_MBR_ENTRY_NUM + FTL_DATA_L2P_ENTRY_NUM_1 )
// opal start lba: DataStore
#define OPAL_DATASTORE_LBA          (FTL_DATA_L2P_ENTRY_NUM_2 << SECTOR_SHIFT)
// total entry
#define FTL_DATA_L2P_ENTRY_NUM      (2 * OPAL_L2P_ENTRY_NUM + FTL_DATA_L2P_ENTRY_NUM_1)
#else
#define FTL_DATA_L2P_ENTRY_NUM      ((NATIVE_MAX_LBA >> SECTOR_SHIFT) + 1)
#endif
#endif

// #define FTL_DATA_L2P_END \
//     (FW_MEM_ADDR_ALIGNED(FTL_DATA_L2P_BASE + FTL_DATA_L2P_ENTRY_NUM * 4))
#if (OPT_TBMT_DRAM)
// if FTL_DBMT_SIZE = 1kb, should 4kb align
#define FTL_DATA_L2P_END \
    MEM_ALIGN((FTL_DATA_L2P_BASE + FTL_DATA_L2P_ENTRY_NUM * 4), 4 * FTL_DBMT_SIZE)
// Consider OP
#define FTL_TBMT_REAL_ENTRY_NUM \
    ((FTL_DATA_L2P_ENTRY_NUM >> FTL_DBMT_ENTRY_NUM_SHIFT) + 1)
#else
#define FTL_DATA_L2P_END \
    MEM_ALIGN((FTL_DATA_L2P_BASE + FTL_DATA_L2P_ENTRY_NUM * 4), 16 * KB)
#endif

#if (OPT_TBMT_DRAM)
#define DIRTY_DBMT_VCNT_MEM_SIZE    (DIRTY_DBMT_VCNT_MEM_NUM << 2)
#define TBMT_BITMAP_MEM_SIZE        (TBMT_BITMAP_MEM_NUM << 2)
#define TBMT_RDY_BITMAP_MEM_SIZE    (TBMT_RDY_BITMAP_MEM_NUM << 2)
#define TBMT_SUBMIT_BITMAP_MEM_SIZE (TBMT_SUBMIT_BITMAP_MEM_NUM << 2)
#else
#define DIRTY_DBMT_VCNT_MEM_SIZE    (0)
#define TBMT_BITMAP_MEM_SIZE        (0)
#define TBMT_RDY_BITMAP_MEM_SIZE    (0)
#define TBMT_SUBMIT_BITMAP_MEM_SIZE (0)
#endif

#define DIRTY_DBMT_VCNT_MEM_BASE    (FW_MEM_ADDR_ALIGNED(TBMT_MEM_END))
#define DIRTY_DBMT_VCNT_MEM_END     (DIRTY_DBMT_VCNT_MEM_BASE + DIRTY_DBMT_VCNT_MEM_SIZE)

#define TBMT_DIRTY_BITMAP_MEM_BASE  (FW_MEM_ADDR_ALIGNED(DIRTY_DBMT_VCNT_MEM_END))
#define TBMT_DIRTY_BITMAP_MEM_END   (TBMT_DIRTY_BITMAP_MEM_BASE + TBMT_BITMAP_MEM_SIZE)

#define TBMT_RDY_BITMAP_MEM_BASE    (FW_MEM_ADDR_ALIGNED(TBMT_DIRTY_BITMAP_MEM_END))
#define TBMT_RDY_BITMAP_MEM_END     (TBMT_RDY_BITMAP_MEM_BASE + TBMT_RDY_BITMAP_MEM_SIZE)

#define TBMT_SUBMIT_BITMAP_MEM_BASE (FW_MEM_ADDR_ALIGNED(TBMT_RDY_BITMAP_MEM_END))
#define TBMT_SUBMIT_BITMAP_MEM_END \
    (TBMT_SUBMIT_BITMAP_MEM_BASE + TBMT_SUBMIT_BITMAP_MEM_SIZE)

// // data buf dram data base
// #define DATA_BUF_DRAM_BASE          (DRAM_MEM_BASE + 0x1FC00000)
// #define DATA_BUF_DRAM_END \
//     (DATA_BUF_DRAM_BASE + DATA_BUF_UNIT_SIZE * DATA_BUF_DRAM_LL_ENTRY_NUM)

// ram disk
#if (EN_RAMDISK) && (OPT_RAMDISK_IN_DRAM)
#define FTL_RAMDISK_BUF_BASE        (DRAM_MEM_BASE)
#define FTL_RAMDISK_BUF_END         (FTL_RAMDISK_BUF_BASE + 256 * 1024 * 1024)
#endif

#if (OPT_DRAM_L2P_CHK)
#define FTL_DATA_L2P_BASE2          (FW_MEM_ADDR_ALIGNED(UEFI_CODE_END))
#ifdef TBMT_DRAM
// if FTL_DBMT_SIZE = 1 KB, should 4 KB align
#define FTL_DATA_L2P_END2 \
    MEM_ALIGN((FTL_DATA_L2P_BASE2 + FTL_DATA_L2P_ENTRY_NUM * 4), 4 * FTL_DBMT_SIZE)
#else
#define FTL_DATA_L2P_END2 \
    MEM_ALIGN((FTL_DATA_L2P_BASE2 + FTL_DATA_L2P_ENTRY_NUM * 4), 16 * KB)
#endif
#else
#define FTL_DATA_L2P_BASE2          (FW_MEM_ADDR_ALIGNED(UEFI_CODE_END))
#define FTL_DATA_L2P_END2           (FTL_DATA_L2P_BASE2)
#endif

#define DRAM_USED_END               (FTL_DATA_L2P_END2)

// // memory for tmp use in pbt procedure
// #define PBT_TMP_BASE                (FW_MEM_ADDR_ALIGNED(FTL_DATA_L2P_END2))
// #define PBT_TMP_END                 (0x11111111)

#if (EN_FTL_UNIT_TEST)
#define FAKE_HW_NCQ_REG_BASE        (0xE00FFE00)
#define FAKE_HW_NCQ_REG_END         (FAKE_HW_NCQ_REG_BASE + 0x200)
#endif

extern void print_sram_info(void);

#endif // SRAM_H