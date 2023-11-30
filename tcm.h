#ifndef TCM_H
#define TCM_H

#include "global.h"
#include "types.h"

// #include "hw_cfg.h"
#include "sys_mem.h"
#include "fw_cfg.h"
#include "fcl_def.h"

#if (OPT_SW_SIM)
#define ATCM_BASE                   (ATCM0_BASE)
#define B0TCM_BASE                  (PRIVATE_B0TCM_BASE)
#define B1TCM_BASE                  (PRIVATE_B1TCM_BASE)
#define ICACHE_BASE                 (I_CACHE0_MEM_BASE)
#define DCACHE_BASE                 (D_CACHE0_MEM_BASE)

#define B0TCM_END                   (B0TCM_BASE + B0TCM_SIZE)
#define B1TCM_END                   (B1TCM_BASE + B1TCM_SIZE)

#define B1TCM_DPD_BASE              (B1TCM_END - B1TCM_DPD_SIZE)
#define B1TCM_DPD_SIZE              (0x800)

// b0tcm
// cpu2 & cpu3
#define CMT_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(B0TCM_BASE + 0x8000))
#define CMT_BITMAP_END              (CMT_BITMAP_BASE + FTL_CMT_BITMAP_SIZE)
#define CHT_MEM_BASE                (FW_MEM_ADDR_ALIGNED(CMT_BITMAP_END))
#define CHT_MEM_END                 (CHT_MEM_BASE + FTL_CHT_ENTRY_NUM * 2)

// hmd related
#if (EN_TBL_31)
#if (EN_MT_HMB)
#define HMB_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(CHT_MEM_END))
 // 1K u8
#define HMB_BITMAP_END              (HMB_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_BUSY_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(HMB_BITMAP_END))
// 1K u8
#define HMB_BUSY_BITMAP_END         (HMB_BUSY_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_MEM_BASE                (FW_MEM_ADDR_ALIGNED(HMB_BUSY_BITMAP_END))
// 4 bytes each HMB entry
#define HMB_MEM_END                 (HMB_MEM_BASE + HMB_TBL_SIZE)
#else
#define HMB_MEM_END                 (FW_MEM_ADDR_ALIGNED(CHT_MEM_END))
#endif
#endif

#define B0TCM_USED_END              (HMB_MEM_END)

#if (B0TCM_USED_END >= (B0TCM_END))
#error "B0TCM_USED_END exceeds B0TCM_END."
#endif

// b1tcm
#if (OPT_SD_MISCOMPARE_DBG)
#define SD_MEM_ADDR_ALIGN           (4096)
#define SD_MEM_ADDR_ALIGN_MASK      (SD_MEM_ADDR_ALIGN - 1)
// #define SD_MEM_ADDR_ALIGNED(addr)   (((addr) + SD_MEM_ADDR_ALIGN_MASK) & (~(u32)SD_MEM_ADDR_ALIGN_MASK))
#define SD_MEM_ADDR_ALIGNED(addr)   (((addr) + SD_MEM_ADDR_ALIGN_MASK) & (0xfffff000))
#endif

// fifo
#if (1 < MAX_PAGES_PER_PU)
#if defined(YMTC_X2_9060)|| defined(YMTC_X3_9060)
#define WRCMD_INFO_NUM              (MAX_PAGES_PER_PU * PLANE_QNTY + 1)
#else
#define WRCMD_INFO_NUM              (3 * PLANE_QNTY * 2)
#endif
#else
#define WRCMD_INFO_NUM              (3 * PLANE_QNTY)
#endif

#define WRCMD_INFO_MASK             (WRCMD_INFO_NUM - 1)
#define RDCMD_INFO_NUM              (0x40)
#define RDCMD_INFO_MASK             (RDCMD_INFO_NUM - 1)
#define WR_ERR_INFO_NUM             (0x10)
#define WR_ERR_INFO_MASK            (WR_ERR_INFO_NUM - 1)

#define CB_INFO_NUM                 (0x100)
#define UNC_INFO_NUM                (0x80)
#define PRED_RD_INFO_NUM            (FRAG_QNTY * MAX_CHAN_NUM * 2)
#define PRED_RD_DESC_NUM            (CECH_QNTY * 3)
#define PRED_RD_CMD_NUM             (CECH_QNTY)
#define PRED_RD_CB_NUM              (0x08)

#define SUB_CMD_INFO_NUM            (2)
// ftl_cmd lff depth should be 1, for trim
#if (SUB_CMD_INFO_NUM > 2)
#error "SUB_CMD_INFO_NUM over"
#endif

#define FTL_CMD_INFO_NUM            (2)
#define SYS_USED_PAGE_QNTY          (56)
#define SYS_L2P_PAGE_QNTY           (SYS_USED_PAGE_QNTY)
//
#define LFF_WR_BASE                 (DW_ALIGNED(B1TCM_BASE))
#define LFF_WR_END                  (LFF_WR_BASE + WRCMD_INFO_NUM * 4)
#define rwWrCmdHeadDesc(n)          (*(volatile u16 *)(LFF_WR_BASE + (n << 2)))
#define rwWrCmdTailDesc(n)          (*(volatile u16 *)(LFF_WR_BASE + 2 + (n << 2)))
//
#define LFF_WR_ERR_BASE             (WD_ALIGNED(LFF_WR_END))
#define LFF_WR_ERR_END              (LFF_WR_ERR_BASE + WR_ERR_INFO_NUM * 2)
#define rwWrErrHeadDesc(n)          (*(volatile u16 *)(LFF_WR_ERR_BASE + (n << 1)))
//
#define LFF_RD_BASE                 (WD_ALIGNED(LFF_WR_ERR_END))
#define LFF_RD_END                  (LFF_RD_BASE + RDCMD_INFO_NUM * 2)
#define rwRdCmdDesc(n)              (*(volatile u16 *)(LFF_RD_BASE + (n << 1)))
//
#if (EN_PRED_RD_31)
#define LFF_PR_BASE                 (DW_ALIGNED(LFF_RD_END))
#define LFF_PR_DESC_BASE            (LFF_PR_BASE)
#define LFF_PR_DESC_END             (LFF_PR_DESC_BASE + PRED_RD_DESC_NUM * 8)
#define rwPRDescHead(n)             (*(volatile u16 *)(LFF_PR_DESC_BASE + 0 + (n << 3)))
#define rwPRDescTail(n)             (*(volatile u16 *)(LFF_PR_DESC_BASE + 2 + (n << 3)))
#define rdwPRDescCnt(n)             (*(volatile u32 *)(LFF_PR_DESC_BASE + 4 + (n << 3)))
//
#if (EN_PRED_RD_31_4CPU)
#define LFF_PR_CMD_BASE             (WD_ALIGNED(LFF_PR_DESC_END))
#define LFF_PR_CMD_END              (LFF_PR_CMD_BASE + PRED_RD_CMD_NUM * 4)
#define rwPRCmdHead(n)              (*(volatile u16 *)(LFF_PR_CMD_BASE + 0 + (n << 2)))
#define rwPRCmdCnt(n)               (*(volatile u16 *)(LFF_PR_CMD_BASE + 2 + (n << 2)))
//
#define LFF_PR_CB_BASE              (WD_ALIGNED(LFF_PR_CMD_END))
#define LFF_PR_CB_END               (LFF_PR_CB_BASE + PRED_RD_CB_NUM * 2)
#define rwPRCBHead(n)               (*(volatile u16 *)(LFF_PR_CB_BASE + 0 + (n << 1)))
//
#if (OPT_FCL_FW_RCQ)
#define LFF_PR_CB1_BASE             (WD_ALIGNED(LFF_PR_CB_END))
#define LFF_PR_CB1_END              (LFF_PR_CB1_BASE + PRED_RD_CB_NUM * 2)
#define rwPRCB1Head(n)              (*(volatile u16 *)(LFF_PR_CB1_BASE + 0 + (n << 1)))
#define LFF_PR_END                  (LFF_PR_CB1_END)
#else
#define LFF_PR_END                  (LFF_PR_CB_END)
#endif
#else
#define LFF_PR_END                  (LFF_PR_DESC_END)
#endif
#else
#define LFF_PR_BASE                 (LFF_RD_END)
#define LFF_PR_END                  (LFF_PR_BASE)
#endif

#define LFF_CB_BASE                 (WD_ALIGNED(LFF_PR_END))
#define LFF_CB_END                  (LFF_CB_BASE + CB_INFO_NUM * 2)
#define rwCbDesc(n)                 (*(volatile u16 *)(LFF_CB_BASE + (n << 1)))
//
#define LFF_UNC_BASE                (DW_ALIGNED(LFF_CB_END))
#define LFF_UNC_END                 (LFF_UNC_BASE + UNC_INFO_NUM * 16)
#define rqwUncDesc(m,n)             (*(volatile u64 *)(LFF_UNC_BASE + (m << 4) + (n << 3)))
//
#define LFF_CMD_BASE                (DW_ALIGNED(LFF_UNC_END))
#define LFF_CMD_END                 (LFF_CMD_BASE + SUB_CMD_INFO_NUM * 16) // only submit 1 cmd
#define rdwCmdaddr(m,n)             (volatile u32 *)(LFF_CMD_BASE + (m * 16) + (n << 2))
#define rbCmdaddr(m,n)              (volatile u8 *)(LFF_CMD_BASE + (m * 16) + (n << 2))

// front side alloc buf, and then submit to ftl
#define LFF_FRONT_BUF_ADDR          (LFF_CMD_END)
#if (EN_TCG_OPAL)
#define LFF_FRONT_BUF_END           (LFF_FRONT_BUF_ADDR + (FRAG_QNTY + HALF_OPAL_TABLE_BUF_SIZE + OPAL_DYNAMIC_BUF_ALLOC_MAX_NUM) * 4)
#else
#define LFF_FRONT_BUF_END           (LFF_FRONT_BUF_ADDR + FRAG_QNTY * 4)
#endif
//
#define LFF_FTL_CMD_BASE            (DW_ALIGNED(LFF_FRONT_BUF_END))
#define rdwFTLCmdInfo(m, n)         (*(volatile u32 *)(LFF_FTL_CMD_BASE + (m << 3) + (n << 2)))
#define LFF_FTL_CMD_END             (LFF_FTL_CMD_BASE + FTL_CMD_INFO_NUM * 8)
// #define LFF_VARIABLE_END            (DW_ALIGNED(LFF_FTL_CMD_END))
// 4 u8 for save current P clock
#define LFF_PER_CLOCK_BASE          (DW_ALIGNED(LFF_FTL_CMD_END))
#define LFF_PER_CLOCK               *(volatile u32 *)(LFF_PER_CLOCK_BASE)
#define LFF_PER_CLOCK_END           (LFF_PER_CLOCK_BASE + 4)
// (SYS_L2P_PAGE_QNTY / 8 u8 for sys_gc bitmap)
#define LFF_SYS_VLD_BIT_MAP_BASE    (LFF_PER_CLOCK_END)
#define LFF_SYS_VLD_BIT_MAP         (volatile u8 *)(LFF_SYS_VLD_BIT_MAP_BASE)
#define LFF_SYS_VLD_BIT_MAP_END     (LFF_SYS_VLD_BIT_MAP_BASE + (4 * ((SYS_L2P_PAGE_QNTY + 31) / 32)))
// 1 u8 for ftl handle result call back
#if (EN_FE_HOST_RW_FTL_IF)
#define LFF_FTL_FE_RESULT           (DW_ALIGNED(LFF_SYS_VLD_BIT_MAP_END))
#define r_ftl_fe_rslt               (*(volatile u32 *)(LFF_FTL_FE_RESULT))
#define LFF_FTL_FE_RESULT_END       (LFF_SYS_VLD_BIT_MAP_END + 4)
#else
#define LFF_FTL_FE_RESULT           (LFF_SYS_VLD_BIT_MAP_END)
#define LFF_FTL_FE_RESULT_END       (LFF_FTL_FE_RESULT)
#endif
// occupy 4 bytes for ecc feature use
// 1 u8 for ftl handle result call back
#define LFF_FRONT_RESULT_BASE       (LFF_FTL_FE_RESULT_END)
#define LFF_FRONT_RESULT            (volatile u8 *)(LFF_FRONT_RESULT_BASE)
#define LFF_FRONT_RESULT_END        (LFF_FRONT_RESULT_BASE + 1)

// 1 u8 for ftl DST handle result
#if (EN_DEVICE_SELF_TEST)
#define LFF_DST_RESULT_BASE         (LFF_FRONT_RESULT_END)
#define LFF_DST_RESULT              (volatile u8 *)(LFF_DST_RESULT_BASE)
#define LFF_DST_RESULT_END          (LFF_DST_RESULT_BASE + 1)
#else
#define LFF_DST_RESULT_BASE         (LFF_FRONT_RESULT_END)
#define LFF_DST_RESULT_END          (LFF_DST_RESULT_BASE)
#endif

// 1 u8 for ftl cmd result
#define LFF_BE_MEM_BASE             (LFF_DST_RESULT_END)
#define r_be_trim_rslt              (*(volatile u8 *)(LFF_BE_MEM_BASE))
#define LFF_BE_MEM_END              (LFF_BE_MEM_BASE + 1)

// 1 u8 for ftl cmd result
#define LFF_BE_FLUSH_BASE           (LFF_BE_MEM_END)
#define r_be_flush_rslt             (*(volatile u8 *)(LFF_BE_FLUSH_BASE))
#define LFF_BE_FLUSH_END            (LFF_BE_FLUSH_BASE + 1)

// 1 u8 for sys info gc
#define LFF_SYS_BLK_FULL_BASE       (LFF_BE_FLUSH_END)
#define LFF_SYS_BLK_FULL            (volatile u8 *)(LFF_SYS_BLK_FULL_BASE)
#define LFF_SYS_BLK_FULL_END        (LFF_SYS_BLK_FULL_BASE + 1)

// 2 u8 for UEFI code boot done label
#define LFF_UEFI_BOOT_DONE_BASE     (WD_ALIGNED(LFF_SYS_BLK_FULL_END))
#define LFF_UEFI_BOOT_DONE          *(volatile u16 *)(LFF_UEFI_BOOT_DONE_BASE)
#define LFF_UEFI_BOOT_DONE_END      (LFF_UEFI_BOOT_DONE_BASE + 2)

// 32 u8
#define _LFF_CMP_CMD_BASE           (DW_ALIGNED(LFF_UEFI_BOOT_DONE_END))
#define LFF_CMP_CMD_BASE            (g_b1tcm_base[LFF_CMP_CMD_IDX])
#define r_ftl_cmp_cmd               ((volatile ftl_cmp_cmd_info_t *)(LFF_CMP_CMD_BASE))
#define LFF_CMP_CMD_END             (_LFF_CMP_CMD_BASE + 32)

// for ps3_1 exit time to do idle procedure
#define LFF_GC_FLAG_BASE            (DW_ALIGNED(LFF_CMP_CMD_END))
#define LFF_GC_FLAG                 *(volatile u32 *)(LFF_GC_FLAG_BASE)
#define LFF_GC_FLAG_END             (LFF_GC_FLAG_BASE + 4)

//
#if (EN_SAVE_LOG)
#define LFF_SYS_TIMER_BASE          (QW_ALIGNED(LFF_GC_FLAG_END))
#define LFF_SYS_TIMER               (*(volatile u64 *)(LFF_SYS_TIMER_BASE))
#define LFF_SYS_TIMER_END           (LFF_SYS_TIMER_BASE + 8)
#else
#define LFF_SYS_TIMER_BASE          (QW_ALIGNED(LFF_GC_FLAG_END))
#define LFF_SYS_TIMER_END           (LFF_SYS_TIMER_BASE)
#endif

// 1 u8 for force flush table before flush qbt
#define LFF_FORCE_FLUSH_TBL_BASE    (LFF_SYS_TIMER_END)
#define LFF_FORCE_FLUSH_TBL         *(volatile u8 *)(LFF_FORCE_FLUSH_TBL_BASE)
#define LFF_FORCE_FLUSH_TBL_END     (LFF_FORCE_FLUSH_TBL_BASE + 1)

// 1 u8 for reset download info in ftl
#define LFF_DL_RESET_BASE           (LFF_FORCE_FLUSH_TBL_END)
#define LFF_DL_RESET                *(volatile u8 *)(LFF_DL_RESET_BASE)
#define LFF_DL_RESET_END            (LFF_DL_RESET_BASE + 1)

// 1 u8 for FTL handle download result
#define LFF_DL_RESULT_BASE          (LFF_DL_RESET_END)
#define LFF_DL_RESULT               *(volatile u8 *)(LFF_DL_RESULT_BASE)
#define LFF_DL_RESULT_END           (LFF_DL_RESULT_BASE + 1)

// 1 u8 for download bin file check sum
#define LFF_DL_CHECK_SUM_BASE       (LFF_DL_RESULT_END)
#define LFF_DL_CHECK_SUM            *(volatile u8 *)(LFF_DL_CHECK_SUM_BASE)
#define LFF_DL_CHECK_SUM_END        (LFF_DL_CHECK_SUM_BASE + 1)

//
#define LFF_ERR_ENTRY_IDX_BASE      (DW_ALIGNED(LFF_DL_CHECK_SUM_END))
#define LFF_ERR_ENTRY_IDX           *(volatile u32 *)(LFF_ERR_ENTRY_IDX_BASE)
#define LFF_ERR_ENTRY_IDX_END       (LFF_ERR_ENTRY_IDX_BASE + 4)

//
#define LFF_ERR_BUF_IDX_BASE        (DW_ALIGNED(LFF_ERR_ENTRY_IDX_END))
#define LFF_ERR_BUF_IDX             *(volatile u32 *)(LFF_ERR_BUF_IDX_BASE)
#define LFF_ERR_BUF_IDX_END         (LFF_ERR_BUF_IDX_BASE + 4)

// 1 u8 for sys info write sts, if program fail, set sts = 1
#define LFF_SYS_WRITE_STS_BASE      (LFF_ERR_BUF_IDX_END)
#define LFF_SYS_WRITE_STS           *(volatile u8 *)(LFF_SYS_WRITE_STS_BASE)
#define LFF_SYS_WRITE_STS_END       (LFF_SYS_WRITE_STS_BASE + 1)

//
#define LFF_PAUSE_MERGE_BASE        (LFF_SYS_WRITE_STS_END)
#define LFF_PAUSE_MERGE             *(volatile u8 *)(LFF_PAUSE_MERGE_BASE)
#define LFF_PAUSE_MERGE_END         (LFF_PAUSE_MERGE_BASE + 1)

//
#define _LFF_WRITE_PROTECT_BASE     (LFF_PAUSE_MERGE_END)
#define LFF_WRITE_PROTECT_BASE      (g_b1tcm_base[LFF_WRITE_PROTECT_IDX])
#define r_write_protect             *(volatile u8 *)(LFF_WRITE_PROTECT_BASE)
#define LFF_WRITE_PROTECT_END       (_LFF_WRITE_PROTECT_BASE + 1)

//
#define LFF_IDLE_REQUEST_BASE       (LFF_WRITE_PROTECT_END)
#define LFF_IDLE_REQUEST            *(volatile u8 *)(LFF_IDLE_REQUEST_BASE)
#define LFF_IDLE_REQUEST_END        (LFF_IDLE_REQUEST_BASE + 1)

//
#define LFF_IDLE_TASK_BASE          (DW_ALIGNED(LFF_IDLE_REQUEST_END))
#define LFF_IDLE_TASK               *(volatile u32 *)(LFF_IDLE_TASK_BASE)
#define LFF_IDLE_TASK_END           (LFF_IDLE_TASK_BASE + 4)

// 4 u8
#if (EN_AES)
#define LFF_AES_BASE                (DW_ALIGNED(LFF_IDLE_TASK_END))
#define LFF_AES_KEY0                *(volatile u32 *)(LFF_AES_BASE)
#define LFF_AES_KEY0_END            (LFF_AES_BASE + 4)

// 4 u8
#define LFF_AES_KEY1                *(volatile u32 *)(DW_ALIGNED(LFF_AES_KEY0_END))
#define LFF_AES_KEY1_END            (LFF_AES_KEY0_END + 4)

// 4 u8
#define LFF_AES_KEY2                *(volatile u32 *)(DW_ALIGNED(LFF_AES_KEY1_END))
#define LFF_AES_KEY2_END            (LFF_AES_KEY1_END + 4)

// 4 u8
#define LFF_AES_KEY3                *(volatile u32 *)(DW_ALIGNED(LFF_AES_KEY2_END))
#define LFF_AES_KEY3_END            (LFF_AES_KEY2_END + 4)
#define LFF_AES_END                 (LFF_AES_KEY3_END)
#else
#define LFF_AES_BASE                (LFF_IDLE_TASK_END)
#define LFF_AES_END                 (LFF_AES_BASE)
#endif

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_BASE                 (LFF_AES_END)
#define LFF_PS_CPU0                 *(volatile u8 *)(LFF_PS_CPU0_BASE)
#define LFF_PS_CPU0_END             (LFF_AES_KEY3_END + 1)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_CPU1                 *(volatile u8 *)(LFF_PS_CPU0_END)
#define LFF_PS_CPU1_END             (LFF_PS_CPU0_END + 1)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_CPU2                 *(volatile u8 *)(LFF_PS_CPU1_END)
#define LFF_PS_CPU2_END             (LFF_PS_CPU1_END + 1)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_CPU3                 *(volatile u8 *)(LFF_PS_CPU2_END)
#define LFF_PS_CPU3_END             (LFF_PS_CPU2_END + 1)
#define LFF_PS_END                  (LFF_PS_CPU3_END)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_NEED_IDLE_PRO_CHECK_BASE    (LFF_PS_END)
#define LFF_NEED_IDLE_PRO_CHECK         *(volatile u8 *)(LFF_NEED_IDLE_PRO_CHECK_BASE)
#define LFF_NEED_IDLE_PRO_CHECK_END     (LFF_NEED_IDLE_PRO_CHECK_BASE + 1)

// CPU1 inform CPU2 to invalid QBT in idle flow
#define LFF_INVALID_QBT_BASE        (LFF_NEED_IDLE_PRO_CHECK_END)
#define LFF_INVALID_QBT             *(volatile u8 *)(LFF_INVALID_QBT_BASE)
#define LFF_INVALID_QBT_END         (LFF_INVALID_QBT_BASE + 1)

#define LFF_TLC_START_BASE          (LFF_INVALID_QBT_END)
#define LFF_TLC_START               *(volatile u8 *)(LFF_TLC_START_BASE)
#define LFF_TLC_START_END           (LFF_TLC_START_BASE + 1)

#define LFF_THERMAL_RANGE_BASE      (LFF_TLC_START_END)
#define LFF_THERMAL_RANGE           *(volatile u8 *)(LFF_THERMAL_RANGE_BASE)
#define LFF_THERMAL_RANGE_END       (LFF_THERMAL_RANGE_BASE + 1)

#define LFF_PS_DIE_REQ_BASE         (LFF_THERMAL_RANGE_END)
#define LFF_PS_DIE_REQ              *(volatile u8 *)(LFF_PS_DIE_REQ_BASE)
#define LFF_PS_DIE_REQ_END          (LFF_PS_DIE_REQ_BASE + 1)

#define LFF_CPU1_ASSERT_FLAG_BASE   (LFF_PS_DIE_REQ_END)
#define LFF_CPU1_ASSERT_FLAG        *(volatile u8 *)(LFF_CPU1_ASSERT_FLAG_BASE)
#define LFF_CPU1_ASSERT_FLAG_END    (LFF_CPU1_ASSERT_FLAG_BASE + 1)

// = 1 means in QBT flush process
#define LFF_FLUSH_QBT_FLAG_BASE     (LFF_CPU1_ASSERT_FLAG_END)
#define LFF_FLUSH_QBT_FLAG          *(volatile u8 *)(LFF_FLUSH_QBT_FLAG_BASE)
#define LFF_FLUSH_QBT_FLAG_END      (LFF_FLUSH_QBT_FLAG_BASE + 1)

// format size, PBT will recover it first for quick init NVMe and PCIE
#define LFF_FORMAT_SIZE_BASE        (LFF_FLUSH_QBT_FLAG_END)
#define LFF_FORMAT_SIZE             *(volatile u8 *)(LFF_FORMAT_SIZE_BASE)
#define LFF_FORMAT_SIZE_END         (LFF_FORMAT_SIZE_BASE + 1)

// CPU assert, for debug use
#define LFF_ASSERT_FLAG_BASE        (LFF_FORMAT_SIZE_END)
#define LFF_ASSERT_FLAG             *(volatile u8 *)(LFF_ASSERT_FLAG_BASE)
#define LFF_ASSERT_FLAG_END         (LFF_ASSERT_FLAG_BASE + 1)

// exit L1 flag
#if (EN_SHUTTLE_CHIP) && (OPT_L1_WORKAROUND)
#define LFF_EXIT_L1_FLAG_BASE       (LFF_ASSERT_FLAG_END)
#define LFF_EXIT_L1_FLAG            *(volatile u8 *)(LFF_EXIT_L1_FLAG_BASE)
#define LFF_EXIT_L1_FLAG_END        (LFF_EXIT_L1_FLAG_BASE + 1)
#else
#define LFF_EXIT_L1_FLAG_BASE       (LFF_ASSERT_FLAG_END)
#define LFF_EXIT_L1_FLAG_END        (LFF_EXIT_L1_FLAG_BASE)
#endif

#if (EN_TBL_31)
#define MT_ACQ_PQ_BASE              (DW_ALIGNED(LFF_EXIT_L1_FLAG_END))
#define MT_ACQ_PQ_SIZE              (16) // real size is 6 bytes
#define g_acq_pq                    ((volatile swlist *)MT_ACQ_PQ_BASE)
#define MT_ACQ_PQ_END               (MT_ACQ_PQ_BASE + MT_ACQ_PQ_SIZE)

// HR_SQ
#define LFF_HR_SQ_FIFO_NUM          (256) // for dbg, after stable set as 64
#define MT_HR_SQ_ENTRY_SIZE         (8)
#define LFF_HR_SQ0_FIFO_BASE        (QW_ALIGNED(MT_ACQ_PQ_END))
#define LFF_HR_SQ0_FIFO_END         (LFF_HR_SQ0_FIFO_BASE + LFF_HR_SQ_FIFO_NUM * MT_HR_SQ_ENTRY_SIZE)

// HR_CQ
#define LFF_HR_CQ_FIFO_NUM          (256) // for dbg, after stable set as 64
#define MT_HR_CQ_ENTRY_SIZE         (4)
#define LFF_HR_CQ0_FIFO_BASE        (DW_ALIGNED(LFF_HR_SQ0_FIFO_END))
#define LFF_HR_CQ0_FIFO_END         (LFF_HR_CQ0_FIFO_BASE + LFF_HR_CQ_FIFO_NUM * MT_HR_CQ_ENTRY_SIZE)

// COMMAN_CMD_CQ
#define COM_CMD_CQ_FIFO_SIZE        (4 * FRAG_QNTY) // for dbg, after stable set as 128
#define COM_CMD_CQ_FIFO_ENTRY_SIZE  (4)
#define _COM_CMD_CQ0_FIFO_BASE      (DW_ALIGNED(LFF_HR_CQ0_FIFO_END))
#define COM_CMD_CQ0_FIFO_BASE       (g_b1tcm_base[COM_CMD_CQ0_FIFO_IDX])
#define COM_CMD_CQ0_FIFO_END        (_COM_CMD_CQ0_FIFO_BASE + COM_CMD_CQ_FIFO_SIZE * COM_CMD_CQ_FIFO_ENTRY_SIZE)

#if (EN_MT_HMB)
#if (EN_HMB_31)
// HMB CMD SQ
#define LFF_HMB_SQ_FIFO_NUM         (96)
#define MT_HMB_SQ_ENTRY_SIZE        (16)
// HMB CMD CQ
#define LFF_HMB_CQ_FIFO_NUM         (32)
#define MT_HMB_CQ_ENTRY_SIZE        (12)
#else
// HMB CMD SQ
#define LFF_HMB_SQ_FIFO_NUM         (32)
#define MT_HMB_SQ_ENTRY_SIZE        (8)
// HMB CMD CQ
#define LFF_HMB_CQ_FIFO_NUM         (32)
#define MT_HMB_CQ_ENTRY_SIZE        (16)
#endif
#define LFF_HMB_SQ0_FIFO_BASE       (QW_ALIGNED(COM_CMD_CQ0_FIFO_END))
#define LFF_HMB_SQ0_FIFO_END        (LFF_HMB_SQ0_FIFO_BASE + LFF_HMB_SQ_FIFO_NUM * MT_HMB_SQ_ENTRY_SIZE)
#define LFF_HMB_CQ0_FIFO_BASE       (QW_ALIGNED(LFF_HMB_SQ0_FIFO_END))
#define LFF_HMB_CQ0_FIFO_END        (LFF_HMB_CQ0_FIFO_BASE + LFF_HMB_CQ_FIFO_NUM * MT_HMB_CQ_ENTRY_SIZE)
#else
#define LFF_HMB_CQ0_FIFO_BASE       (COM_CMD_CQ0_FIFO_END)
#define LFF_HMB_CQ0_FIFO_END        (LFF_HMB_CQ0_FIFO_BASE)
#endif

#if (EN_MT_4CPU)
#define LFF_HR_SQ1_FIFO_BASE        (QW_ALIGNED(LFF_HMB_CQ0_FIFO_END))
#define LFF_HR_SQ1_FIFO_END         (LFF_HR_SQ1_FIFO_BASE + LFF_HR_SQ_FIFO_NUM * MT_HR_SQ_ENTRY_SIZE)
#define rstHR_SQ1_FIFO_CMD(m)       ((volatile u32 *)(LFF_HR_SQ1_FIFO_BASE + (m) * MT_HR_SQ_ENTRY_SIZE))

#define LFF_HR_CQ1_FIFO_BASE        (DW_ALIGNED(LFF_HR_SQ1_FIFO_END))
#define LFF_HR_CQ1_FIFO_END         (LFF_HR_CQ1_FIFO_BASE + LFF_HR_CQ_FIFO_NUM * MT_HR_CQ_ENTRY_SIZE)
#define rstHR_CQ1_FIFO_CMD(m)       ((volatile u32 *)(LFF_HR_CQ1_FIFO_BASE + (m) * MT_HR_CQ_ENTRY_SIZE))

#define COM_CMD_SQ_FIFO_SIZE        (128)
#define COM_CMD_SQ_FIFO_ENTRY_SIZE  (16)
#define _COM_CMD_SQ0_FIFO_BASE      (DW_ALIGNED(LFF_HR_CQ1_FIFO_END))
#define COM_CMD_SQ0_FIFO_BASE       (g_b1tcm_base[COM_CMD_SQ0_FIFO_IDX])
#define COM_CMD_SQ0_FIFO_END        (_COM_CMD_SQ0_FIFO_BASE + COM_CMD_SQ_FIFO_SIZE * COM_CMD_SQ_FIFO_ENTRY_SIZE)
#define rstCOM_SQ0_FIFO_CMD(m)      ((volatile u32 *)(COM_CMD_SQ0_FIFO_BASE + (m) * COM_CMD_SQ_FIFO_ENTRY_SIZE))

#define _COM_CMD_SQ1_FIFO_BASE      (DW_ALIGNED(COM_CMD_SQ0_FIFO_END))
#define COM_CMD_SQ1_FIFO_BASE       (g_b1tcm_base[COM_CMD_SQ1_FIFO_IDX])
#define COM_CMD_SQ1_FIFO_END        (_COM_CMD_SQ1_FIFO_BASE + COM_CMD_SQ_FIFO_SIZE * COM_CMD_SQ_FIFO_ENTRY_SIZE)
#define rstCOM_SQ1_FIFO_CMD(m)      ((volatile u32 *)(COM_CMD_SQ1_FIFO_BASE + (m) * COM_CMD_SQ_FIFO_ENTRY_SIZE))

#define _COM_CMD_CQ1_FIFO_BASE      (DW_ALIGNED(COM_CMD_SQ1_FIFO_END))
#define COM_CMD_CQ1_FIFO_BASE       (g_b1tcm_base[COM_CMD_CQ1_FIFO_IDX])
#define COM_CMD_CQ1_FIFO_END        (_COM_CMD_CQ1_FIFO_BASE + COM_CMD_CQ_FIFO_SIZE * COM_CMD_CQ_FIFO_ENTRY_SIZE)
#define rstCOM_CQ1_FIFO_CMD(m)      ((volatile u32 *)(COM_CMD_CQ1_FIFO_BASE + (m) * COM_CMD_CQ_FIFO_ENTRY_SIZE))

#if (EN_MT_HMB)
#define LFF_HMB_SQ1_FIFO_BASE       (QW_ALIGNED(COM_CMD_CQ1_FIFO_END))
#define LFF_HMB_SQ1_FIFO_END        (LFF_HMB_SQ1_FIFO_BASE + LFF_HMB_SQ_FIFO_NUM * MT_HMB_SQ_ENTRY_SIZE)
#define LFF_HMB_CQ1_FIFO_BASE       (QW_ALIGNED(LFF_HMB_SQ1_FIFO_END))
#define LFF_HMB_CQ1_FIFO_END        (LFF_HMB_CQ1_FIFO_BASE + LFF_HMB_CQ_FIFO_NUM * MT_HMB_CQ_ENTRY_SIZE)
#else
#define LFF_HMB_CQ1_FIFO_END        (COM_CMD_CQ1_FIFO_END)
#endif

// means 2 DW
#define T2F_FIFO_ENTRY_SIZE         (2)
#define T2F_FIFO_ENTRY_NUM          (128)

// tbl 2 ftl fifo
#define T2F_FIFO_SIZE_DW_SQ         (T2F_FIFO_ENTRY_NUM * T2F_FIFO_ENTRY_SIZE)
#define T2F_FIFO_SIZE_DW_CQ         (T2F_FIFO_ENTRY_NUM * T2F_FIFO_ENTRY_SIZE)
#define T2F_FIFO_SQ0_MEM_BASE       (FW_MEM_ADDR_ALIGNED(LFF_HMB_CQ1_FIFO_END))
#define T2F_FIFO_SQ0_MEM_END        (T2F_FIFO_SQ0_MEM_BASE + T2F_FIFO_SIZE_DW_SQ * 4)
//
#define T2F_FIFO_SQ1_MEM_BASE       (FW_MEM_ADDR_ALIGNED(T2F_FIFO_SQ0_MEM_END))
#define T2F_FIFO_SQ1_MEM_END        (T2F_FIFO_SQ1_MEM_BASE + T2F_FIFO_SIZE_DW_SQ * 4)

#define T2F_FIFO_CQ0_MEM_BASE       (FW_MEM_ADDR_ALIGNED(T2F_FIFO_SQ1_MEM_END))
#define T2F_FIFO_CQ0_MEM_END        (T2F_FIFO_CQ0_MEM_BASE + T2F_FIFO_SIZE_DW_CQ * 4)
#define T2F_FIFO_CQ1_MEM_BASE       (FW_MEM_ADDR_ALIGNED(T2F_FIFO_CQ0_MEM_END))
#define T2F_FIFO_CQ1_MEM_END        (T2F_FIFO_CQ1_MEM_BASE + T2F_FIFO_SIZE_DW_CQ * 4)
#define LFF_TBL31_FIFO_MEM_END      (T2F_FIFO_CQ1_MEM_END)
#else
#define LFF_TBL31_FIFO_MEM_END      (LFF_HMB_CQ0_FIFO_END)
#endif // EN_MT_4CPU
#else
#define LFF_TBL31_FIFO_MEM_END      (LFF_EXIT_L1_FLAG_END)
#endif // EN_TBL_31

// for idle close TLC head block
#if (EN_AES)
#define LFF_AES_KEY_FLAG_BASE       (LFF_TBL31_FIFO_MEM_END)
#define LFF_AES_KEY_FLAG            *(volatile u8 *)(LFF_AES_KEY_FLAG_BASE)
#define LFF_AES_KEY_FLAG_END        (LFF_AES_KEY_FLAG_BASE + 1)
// #define LFF_VARIABLE_BASE_END       (LFF_AES_KEY_FLAG_BASE)
#else
// #define LFF_VARIABLE_BASE_END       (LFF_AES_KEY_FLAG_BASE)
#define LFF_AES_KEY_FLAG_BASE       (LFF_TBL31_FIFO_MEM_END)
#define LFF_AES_KEY_FLAG_END        (LFF_AES_KEY_FLAG_BASE)
#endif

//
#if (EN_FCL_30)
#if (OPT_FCL_FW_RCQ)
#define FW_RCQ_ENTRY_SIZE           (1)  //one u32
#define FW_RCQ_ENTRY_NUM            (64)
#define FCL_FIFO_SIZE_FW_RCQ        (FW_RCQ_ENTRY_NUM * FW_RCQ_ENTRY_SIZE)
#define FCL_FIFO_FW_RCQ_MEM_BASE    (FW_MEM_ADDR_ALIGNED(LFF_AES_KEY_FLAG_END))
#define FCL_FIFO_FW_RCQ_MEM_END     (FCL_FIFO_FW_RCQ_MEM_BASE + FCL_FIFO_SIZE_FW_RCQ * 4)
#define FCL_FIFO_MEM_END            (FCL_FIFO_FW_RCQ_MEM_END)
#else
#define FCL_FIFO_MEM_END            (LFF_AES_KEY_FLAG_END)
#endif
#endif

//
#if (LUN_SHIFT == 0)
#define MT_TBL_BID_BASE             (FW_MEM_ADDR_ALIGNED(FCL_FIFO_MEM_END))
#define MT_TBL_BID_SIZE             (VTBL_BLK_MAX_NUM * 2 + 4 + VTBL_BLK_MAX_NUM)
#define g_mt_tbl_bid                ((volatile ftl_tbmt_bid *)MT_TBL_BID_BASE)
#define MT_TBL_BID_END              (MT_TBL_BID_BASE + MT_TBL_BID_SIZE)
#endif

//
#if (OPT_CMT_BYPASS)
#if (LUN_SHIFT == 0)
#define MT_BYPASS_BASE              (FW_MEM_ADDR_ALIGNED(MT_TBL_BID_END))
#else
#define MT_BYPASS_BASE              (FW_MEM_ADDR_ALIGNED(FCL_FIFO_MEM_END))
#endif
#define MT_BYPASS_SIZE              (4)
#define g_cmt_bypass                (*(volatile u32 *)MT_BYPASS_BASE)
#define MT_BYPASS_END               (MT_BYPASS_BASE + MT_BYPASS_SIZE)
#define MT_SERVICE_SN_1_BASE        (FW_MEM_ADDR_ALIGNED(MT_BYPASS_END))
#else
#if (LUN_SHIFT == 0)
#define MT_SERVICE_SN_1_BASE        (FW_MEM_ADDR_ALIGNED(MT_TBL_BID_END))
#else
#define MT_SERVICE_SN_1_BASE        (FW_MEM_ADDR_ALIGNED(FCL_FIFO_MEM_END))
#endif
#endif
#define MT_SERVICE_SN_1_SIZE        (2 * 4)
#define MT_SERVICE_SN_1_END         (MT_SERVICE_SN_1_BASE + MT_SERVICE_SN_1_SIZE)

//
#if (EN_MT_4CPU)
// Same as PROG_V2P_SIZE
#define PROG_V2P_ARRAY_SIZE         (MAX_DIE_QNTY * MAX_PAGES_PER_PROG_SEQ * PLANE_QNTY * MAX_SUPER_PAGES_FOR_ONE_RAID_STRIPE)
#define PROG_V2P_ARRAY_BASE         (FW_MEM_ADDR_ALIGNED(MT_SERVICE_SN_1_END))
#define PROG_V2P_ARRAY_END          (PROG_V2P_ARRAY_BASE + PROG_V2P_ARRAY_SIZE * 2)
// Same as PROG_P2V_SIZE
#define PROG_P2V_ARRAY_SIZE         ((MAX_SUPER_PAGES_FOR_ONE_RAID_STRIPE * MAX_PAGES_PER_PROG_SEQ) << (VDIE_SHIFT + PLANE_SHIFT))
#define PROG_P2V_ARRAY_BASE         (FW_MEM_ADDR_ALIGNED(PROG_V2P_ARRAY_END))
#define PROG_P2V_ARRAY_END          (PROG_P2V_ARRAY_BASE + PROG_P2V_ARRAY_SIZE * 2)

#define SHARE_CMT_BASE              (FW_MEM_ADDR_ALIGNED(PROG_P2V_ARRAY_END))
#define SHARE_CMT_SIZE              (15 * 4) // (sizeof(ftl_share_cmt))
#define SHARE_CMT_END               (SHARE_CMT_BASE + SHARE_CMT_SIZE)

#define SHARE_CMUT_BASE             (FW_MEM_ADDR_ALIGNED(SHARE_CMT_END))
#define SHARE_CMUT_SIZE             (4) // (sizeof(ftl_share_cmut))
#define SHARE_CMUT_END              (SHARE_CMUT_BASE + SHARE_CMUT_SIZE)

#define GLOBAL_TF_BASE              (FW_MEM_ADDR_ALIGNED(SHARE_CMUT_END))
#define GLOBAL_TF_SIZE              (20 * 4) // (sizeof(ftl_tf_glb_ctx))
#define g_tf_glb_ctx_ptr            ((volatile ftl_tf_glb_ctx *)GLOBAL_TF_BASE)
#define GLOBAL_TF_END               (GLOBAL_TF_BASE + GLOBAL_TF_SIZE)

#define SYNC_CMD_SQ_BASE            (FW_MEM_ADDR_ALIGNED(GLOBAL_TF_END))
#define SYNC_CMD_SQ_SIZE            (2 * 6 * 4) // sizeof(SYNC_OPS_SQ_FORMAT)
#define SYNC_CMD_SQ_END             (SYNC_CMD_SQ_BASE + SYNC_CMD_SQ_SIZE)

#define SYNC_CMD_CQ_BASE            (FW_MEM_ADDR_ALIGNED(SYNC_CMD_SQ_END))
#define SYNC_CMD_CQ_SIZE            (2 * 2 * 4) // sizeof(SYNC_OPS_CQ_FORMAT)
#define SYNC_CMD_CQ_END             (SYNC_CMD_CQ_BASE + SYNC_CMD_CQ_SIZE)

#define DEBUG_CMD_SQ_BASE           (FW_MEM_ADDR_ALIGNED(SYNC_CMD_CQ_END))
#define DEBUG_CMD_SQ_SIZE           (2 * 6 * 4) // sizeof(SYNC_OPS_SQ_FORMAT)
#define DEBUG_CMD_SQ_END            (DEBUG_CMD_SQ_BASE + DEBUG_CMD_SQ_SIZE)

#define DEBUG_CMD_CQ_BASE           (FW_MEM_ADDR_ALIGNED(DEBUG_CMD_SQ_END))
#define DEBUG_CMD_CQ_SIZE           (2 * 2 * 4) // sizeof(SYNC_OPS_CQ_FORMAT)
#define DEBUG_CMD_CQ_END            (DEBUG_CMD_CQ_BASE + DEBUG_CMD_CQ_SIZE)

#define DEBUG_MODE_FLAG             (FW_MEM_ADDR_ALIGNED(DEBUG_CMD_CQ_END))
#define DEBUG_MODE                  (0x12345678) // 1 u32
#define DEBUG_MODE_FLAG_END         (FW_MEM_ADDR_ALIGNED(DEBUG_CMD_CQ_END) + 4)
#define LFF_END                     (FW_MEM_ADDR_ALIGNED(DEBUG_MODE_FLAG_END))
#else
#define LFF_END                     (MT_SERVICE_SN_1_END)
#endif

// table
#if (OPT_TCM_DESC)
#define _FW_CMD_DESC0_BASE          (FW_MEM_ADDR_ALIGNED(LFF_END))
#define FW_CMD_DESC0_BASE           (g_b1tcm_base[FW_CMD_DESC0_IDX])
#else
#define FW_CMD_DESC0_BASE            (SRAM_MEM_BASE)
#endif

#if (!OPT_FCL_IF_DESC_CONFIG2)
#if (META_DATA_SIZE == META_SIZE_8DW)
#define FW_CMD_DESC_SIZE            (64)
#elif (META_DATA_SIZE == META_SIZE_6DW)
#define FW_CMD_DESC_SIZE            (56)
#elif (META_DATA_SIZE == META_SIZE_4DW)
#define FW_CMD_DESC_SIZE            (48)
#elif (META_DATA_SIZE == META_SIZE_3DW)
#define FW_CMD_DESC_SIZE            (44)
#elif (META_DATA_SIZE == META_SIZE_2DW)
#define FW_CMD_DESC_SIZE            (40)
#else
#error "META_DATA_SIZE NOT SUPPORT.2"
#endif
#else
#if (META_DATA_SIZE == META_SIZE_8DW)
#define FW_CMD_DESC_SIZE            (68)
#elif (META_DATA_SIZE == META_SIZE_6DW)
#define FW_CMD_DESC_SIZE            (60)
#elif (META_DATA_SIZE == META_SIZE_4DW)
#define FW_CMD_DESC_SIZE            (52)
#elif (META_DATA_SIZE == META_SIZE_3DW)
#define FW_CMD_DESC_SIZE            (48)
#elif (META_DATA_SIZE == META_SIZE_2DW)
#define FW_CMD_DESC_SIZE            (44)
#else
#error "META_DATA_SIZE NOT SUPPORT.2"
#endif
#endif

#define FW_CMD_DESC0_END             (FW_CMD_DESC_NUM * FW_CMD_DESC_SIZE + _FW_CMD_DESC0_BASE)

#if (OPT_BUF_USE_DBG2) || (OPT_DESC_USE_DBG2)
#define DRAM_BUF_HW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(FW_CMD_DESC0_END))
#define DRAM_BUF_HW_BITMAP_END      (DRAM_BUF_HW_BITMAP_BASE + (DATA_BUF_DRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define DRAM_BUF_FW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(DRAM_BUF_HW_BITMAP_END))
#define DRAM_BUF_FW_BITMAP_END      (DRAM_BUF_FW_BITMAP_BASE + (DATA_BUF_DRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define SRAM_BUF_HW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(DRAM_BUF_FW_BITMAP_END))
#define SRAM_BUF_HW_BITMAP_END      (SRAM_BUF_HW_BITMAP_BASE + (DATA_BUF_SRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define SRAM_BUF_FW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(SRAM_BUF_HW_BITMAP_END))
#define SRAM_BUF_FW_BITMAP_END      (SRAM_BUF_FW_BITMAP_BASE + (DATA_BUF_SRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC_HW_BITMAP_BASE         (FW_MEM_ADDR_ALIGNED(SRAM_BUF_FW_BITMAP_END))
#define DESC_HW_BITMAP_END          (DESC_HW_BITMAP_BASE + (FW_CMD_DESC_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC_FW_BITMAP_BASE         (FW_MEM_ADDR_ALIGNED(DESC_HW_BITMAP_END))
#define DESC_FW_BITMAP_END          (DESC_FW_BITMAP_BASE + (FW_CMD_DESC_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC1_HW_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(DESC_FW_BITMAP_END))
#define DESC1_HW_BITMAP_END         (DESC1_HW_BITMAP_BASE + (FW_CMD_DESC1_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC1_FW_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(DESC1_HW_BITMAP_END))
#define DESC1_FW_BITMAP_END         (DESC1_FW_BITMAP_BASE + (FW_CMD_DESC1_NUM >> 3) + 1)// 4k buf use 1bit
#define DESC_TEMP_END               (DESC1_FW_BITMAP_END)
#else
#define DESC_TEMP_END               (FW_CMD_DESC0_END)
#endif

// hw trim vd
// this address range is re-used by PBT
#define HW_TRIM_VC_BASE             (FW_MEM_ADDR_ALIGNED(DESC_TEMP_END))
#define HW_TRIM_VC_SIZE             (VBLOCK_NUM * 4)
#define HW_TRIM_VC_END              (HW_TRIM_VC_BASE + HW_TRIM_VC_SIZE)
#if (EN_MT_4CPU)
// this address range is re-used by PBT
#define HW_TRIM_VC_BASE2            (FW_MEM_ADDR_ALIGNED(HW_TRIM_VC_END))
#define HW_TRIM_VC_END2             (HW_TRIM_VC_BASE2 + HW_TRIM_VC_SIZE)
#else
#define HW_TRIM_VC_END2             (HW_TRIM_VC_END)
#endif

// wc hash tbl
#define _WC_HASH_TBL_BASE            (FW_MEM_ADDR_ALIGNED(HW_TRIM_VC_END2))
#define WC_HASH_TBL_BASE            (g_b1tcm_base[WC_HASH_TBL_IDX])
#define WC_HASH_TBL_END             (_WC_HASH_TBL_BASE + WRITE_CAHCE_HASH_TBL_ENTRY_NUM * 2)

// rowc
#if !defined(ONE_PASS_MODE)
#define ROWC_BASE                   (FW_MEM_ADDR_ALIGNED(WC_HASH_TBL_END))
#define ROWC_SIZE                   (PAGE_QNTY + 1)
#define ROWC_END                    (FW_MEM_ADDR_ALIGNED(ROWC_BASE + ROWC_SIZE))
#define g_share_page_loc            ((volatile byte *)ROWC_BASE)
#else
#define ROWC_END                    (WC_HASH_TBL_END)
#endif

// cmt
#define CMT_MEM_BASE                (FW_MEM_ADDR_ALIGNED(ROWC_END))
#define CMT_MEM_END                 (CMT_MEM_BASE + FTL_CMT_ENTRY_NUM * 12) // 12 bytes each CMT entry

// cmt hash
#define CMT_HASH_MEM_BASE           (FW_MEM_ADDR_ALIGNED(CMT_MEM_END))
#define CMT_HASH_MEM_END            (CMT_HASH_MEM_BASE + FTL_CMT_ENTRY_NUM * 2)

#define CMUT_MEM_BASE               (FW_MEM_ADDR_ALIGNED(CMT_HASH_MEM_END))
#define CMUT_MEM_END                (CMUT_MEM_BASE + FTL_CMUT_ENTRY_NUM * 8) // 8 bytes each CMUT entry

// hmb, for 2 cpu
#if (EN_TBL_31)
#if (EN_MT_2CPU)
#if (EN_MT_HMB)
#define HMB_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(CMUT_MEM_END))
#define HMB_BITMAP_END              (HMB_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_BUSY_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(HMB_BITMAP_END))
#define HMB_BUSY_BITMAP_END         (HMB_BUSY_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_MEM_BASE                (FW_MEM_ADDR_ALIGNED(HMB_BUSY_BITMAP_END))
// 4 bytes each HMB entry
#define HMB_MEM_END                 (HMB_MEM_BASE + HMB_LOG_ENTRY_NUM * 4)
#else
#define HMB_MEM_END                 (FW_MEM_ADDR_ALIGNED(CMUT_MEM_END))
#endif
#define B1TCM_TABLE_END0            (FW_MEM_ADDR_ALIGNED(HMB_MEM_END))
#else
#define B1TCM_TABLE_END0            (FW_MEM_ADDR_ALIGNED(CMUT_MEM_END))
#endif
#endif

// aes key
#if (EN_AES)
#define AES_KEY_BASE                (FW_MEM_ADDR_ALIGNED(B1TCM_TABLE_END0)) // CHK PERF_TUNING g_host_wr_4K_time
// 1: aes 128 mode: 16 bytes; 2: aes 256 mode: 32 bytes
#define AES_KEY_END                 (AES_KEY_BASE + 64)
#else
#define AES_KEY_END                 (FW_MEM_ADDR_ALIGNED(B1TCM_TABLE_END0))
#endif

// flush laa
#define FTL_FLUSH_LAA_TEMP_BASE     (FW_MEM_ADDR_ALIGNED(AES_KEY_END)) // CHK PERF_TUNING g_host_wr_4K_time
#define FTL_FLUSH_LAA_TEMP_END      (FTL_FLUSH_LAA_TEMP_BASE + FRAG_QNTY * MAX_PAGES_PER_PROG_SEQ * 4) // 4 byte per frag * one page frag * seq prog num
#define FTL_MERGE_LAA_TEMP_BASE     (FW_MEM_ADDR_ALIGNED(FTL_FLUSH_LAA_TEMP_END))
#define FTL_MERGE_LAA_TEMP_END      (FTL_MERGE_LAA_TEMP_BASE + FRAG_QNTY * MAX_PAGES_PER_PROG_SEQ * 4) // 4 byte per frag * one page frag * seq prog num

#define B1TCM_TABLE_END             (FTL_MERGE_LAA_TEMP_END)

// shared var
#define _SHARED_VAR_BASE            (FW_MEM_ADDR_ALIGNED(B1TCM_TABLE_END))
#define SHARED_VAR_BASE             (g_b1tcm_base[SHARED_VAR_IDX])

#define rbWrCmdInfoWrPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0000))
#define rbWrCmdInfoRdPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0001))
#define rbRdCmdInfoWrPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0002)) // use for pred rd
#define rbRdCmdInfoRdPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0003)) // use for pred rd
#define r_fua_flag                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x0004))
#define r_flush_dummy               (*(volatile u8 *)(SHARED_VAR_BASE + 0x0005))
#define g_dummy_buf_ptr             (*(volatile u16 *)(SHARED_VAR_BASE + 0x0006))
#define r_last_fua_desc             (*(volatile u16 *)(SHARED_VAR_BASE + 0x0008))
#define r_pause_write               (*(volatile u8 *)(SHARED_VAR_BASE + 0x000a))
#define g_ftl_trim_flag             (*(volatile u8 *)(SHARED_VAR_BASE + 0x000b))
#define g_trim_insert_cmt           (*(volatile u16 *)(SHARED_VAR_BASE + 0x000c))

#define r_load_code_id              (*(volatile u8 *)(SHARED_VAR_BASE + 0x000e)) // code bank
#define r_load_code_flag            (*(volatile u8 *)(SHARED_VAR_BASE + 0x000f)) // code bank

#define r_power_down                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0010))
#define r_en_plp_or_pld             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0011)) // ==1, enable PLD; ==2, enable PLP; ==0, disable both;
#define r_bios_end                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x0012))
#define r_reset_assert              (*(volatile u8 *)(SHARED_VAR_BASE + 0x0013))
/*******************************************************************
r_cpu1_ready:
now use 3bit for QBT and PBT to inform CPU1 that CPU2 ready status
QBT: when QBT first step done, it will set r_cpu1_ready bit0 and bit7 to 1;when second step done, it will clear bit7 to 0
PBT:  when PBT recover sys info done,it will set bit1 to 1; when PBT all done, it will set bit 0 to 1
so r_cpu1_ready has below status:
1.r_cpu1_ready = 0x81, QBT first step done;  CPU1 can normal boot and get command from host, no need to wait QBT load tbl done
2.r_cpu1_ready = 0x01, QBT load table done; CPU1 can send flush command in reset flow
3.r_cpu1_ready = 0x02, PBT recover sys info done; CPU1 can init PCIE, NVMe controller, but can not alloc buffer and desc;
so CPU1 will always return in create queue command and alloc buffer until PBT done
4.r_cpu1_ready = 0x03, PBT all done; CPU1 can create queue and receive read/write command
*********************************************************************/
#define r_cpu1_ready                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0014))
#define r_print_select              (*(volatile u8 *)(SHARED_VAR_BASE + 0x0015))
#define rbehm                       (*(volatile u8 *)(SHARED_VAR_BASE + 0x0016)) // hmb enable or not

#define r_trim_num                  (*(volatile u16 *)(SHARED_VAR_BASE + 0x0018))
#define rw_rct_hmb_num              (*(volatile u16 *)(SHARED_VAR_BASE + 0x001a)) // real hmb rct entry num, hmb.free_list_2k.cnt
#define rw_rct_hmb_tbl_num          (*(volatile u16 *)(SHARED_VAR_BASE + 0x001c)) // real hmb rct tbl num, hmb.free_list_4k.cnt

#define g_qbt_valid                 (*(volatile u8 *)(SHARED_VAR_BASE + 0x001e))
#define r_host_aes_flag             (*(volatile u8 *)(SHARED_VAR_BASE + 0x001f))

#define g_ftl_se_cmd_step           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0020)) // byte
#define g_sanitize_action           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0021)) // byte
#define g_cmd_info_status           (*(volatile u16 *)(SHARED_VAR_BASE + 0x0022)) // word

#define r_total_spare_blk           (*(volatile u32 *)(SHARED_VAR_BASE + 0x0024))
#define r_dbg_val                   (*(volatile u32 *)(SHARED_VAR_BASE + 0x0028))
#define rdwHMBEntryCnt              (*(volatile u32 *)(SHARED_VAR_BASE + 0x002c)) // How many HMB desc

#define r_prev_rbehm                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0030)) // for hmb with L1.2 PS4
#define r_req_rbehm                 (*(volatile u8 *)(SHARED_VAR_BASE + 0x0031))
#define r_en_debug_print            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0032))
#define r_share_tmr_isr             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0033))

#define r_all_zero_mode             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0034)) // 1:ALL 0 mode begin, 0: ALL 0 mode close
#define r_push_blk_proc_fail        (*(volatile u8 *)(SHARED_VAR_BASE + 0x0035))

#define r_cpu2_ready                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0036))
#define r_cpu3_ready                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0037))

#define g_page_sn                   (*(volatile u64 *)(SHARED_VAR_BASE + 0x0038))

#define r_front_mutex_req           (*(volatile byte *)(SHARED_VAR_BASE + 0x0040))
#define r_front_mutex_rslt          (*(volatile byte *)(SHARED_VAR_BASE + 0x0041))
#define r_host_w_need_au_num        (*(volatile u16 *)(SHARED_VAR_BASE + 0x0042))
#define r_wc_flush_list_cnt         (*(volatile u16 *)(SHARED_VAR_BASE + 0x0044))
#define r_rd_flying_cnt             (*(volatile u16 *)(SHARED_VAR_BASE + 0x0046))

#define rdwHostHmbBuff              ((volatile u32 *)(SHARED_VAR_BASE + 0x0048)) // rdwHostHmbBuff need 2 dword
#define rdwHostHmbBuff_rsvd         ((volatile u32 *)(SHARED_VAR_BASE + 0x004c))

#define rblk_record                 (*(volatile u16 *)(SHARED_VAR_BASE + 0x0054))
#define r_blk_record_flag           (*(volatile u16 *)(SHARED_VAR_BASE + 0x0056))

#define r_qbt_erase_cnt             (*(volatile u32 *)(SHARED_VAR_BASE + 0x0058))

#define rbHMBsplitDesc              (*(volatile u8 *)(SHARED_VAR_BASE + 0x005c))
#define rbsoftRstFlag               (*(volatile u8 *)(SHARED_VAR_BASE + 0x005d))
#define r_aes_critica_flag          (*(volatile u8 *)(SHARED_VAR_BASE + 0x005e))
#define g_power_loss                (*(volatile u8 *)(SHARED_VAR_BASE + 0x005f))

#define g_flash_rd_seccnt           (*(volatile u64 *)(SHARED_VAR_BASE + 0x0060))
#define g_flash_wr_seccnt           (*(volatile u64 *)(SHARED_VAR_BASE + 0x0068))

#define rdwCRCCnt                   (*(volatile u32 *)(SHARED_VAR_BASE + 0x0070))

#define r_ps3_fc_prepare_done       (*(volatile u8 *)(SHARED_VAR_BASE + 0x0074))
#define r_ps3_ps4_rst_dco           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0075))
#define r_power_on                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x0076))
#define r_rd_sz_less_2MB            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0077))

#define r_hr_rsp_to_pq_vld          (*(volatile u8 *)(SHARED_VAR_BASE + 0x0078))
#define r_force_pop_hc_wr           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0079))
#define r_force_pop_vld             (*(volatile u8 *)(SHARED_VAR_BASE + 0x007a))
#define r_raid_close_super_wl       (*(volatile u8 *)(SHARED_VAR_BASE + 0x007b))
#define r_free_desc_cnt             (*(volatile u32 *)(SHARED_VAR_BASE + 0x007c))

#define r_rd_size                   (*(volatile u32 *)(SHARED_VAR_BASE + 0x0080)) // for pred rd perf tune
#define r_rd_hit_wr_lff             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0084))
#define r_dis_seq_hrsd              (*(volatile u8 *)(SHARED_VAR_BASE + 0x0085))
#define r_pred_rd_cnt_thr           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0086))
#define r_pred_read_step            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0087))
#define r_pred_total_cnt            (*(volatile u16 *)(SHARED_VAR_BASE + 0x0088))
#define r_pred_read_smp_cnt         (*(volatile u32 *)(SHARED_VAR_BASE + 0x008c))
#if (EN_PRED_RD_31)
// 8 dword
#define g_ftl_pred_rd_mgr           ((volatile pred_rd_mgr_type *)(SHARED_VAR_BASE + 0x0090))
#else
// 5 dword
#define g_ftl_pred_rd_mgr           ((volatile ftl_pred_rd_mgr *)(SHARED_VAR_BASE + 0x0090))
#endif

// #define r_nand_temp_value           (*(volatile u16 *)(SHARED_VAR_BASE + 0x00AC))
// #define r_tlc_data_flying_flag      (*(volatile u8 *)(SHARED_VAR_BASE + 0x00AE))

#define r_slot_fr                   ((volatile u64 *)(SHARED_VAR_BASE + 0x00C8)) //32 bytes
#define r_dl_chk_sum_flag           (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM))
#define r_dl_success                (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 1))
#define r_fw_update_flag            (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 2))
#define r_slot_afi                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 3))
#if (EN_MT_4CPU)
#define g_in_pbt_process            (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 4))
#endif

#define r_tlc_data_flying_flag      (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 5))
#define r_nand_temp_value           (*(volatile u16 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 6))
#define r_mismatch_flag             (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 8))

#if (EN_READ_PATROL)
#define r_read_patrol_to_do         (*(volatile u32 *)(DW_ALIGNED(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 12)))
#define SHARED_VAR_END              (SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 16)
#else
#define SHARED_VAR_END              (_SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 12)
#endif



#if (EN_NVME_HMB)
#define HMB_ACC_CNT_MEM_BASE        (DW_ALIGNED(SHARED_VAR_END))

#define rdwHMBAccCnt                ((volatile u32 *)(HMB_ACC_CNT_MEM_BASE)) // How many HMB entry in these HMB desc, including 4K entry & 2K entry
#define HMB_ACC_CNT_MEM_END         (HMB_ACC_CNT_MEM_BASE + (HMB_MAX_DESC_NUM * 4))

#define HMB_DESC_BADD_MEM_BASE      (QW_ALIGNED(HMB_ACC_CNT_MEM_END))
#define rqwHMBDescBAdd              ((volatile u64 *)(HMB_DESC_BADD_MEM_BASE)) // The base addr of this HMB desc
#define HMB_DESC_BADD_MEM_END       (HMB_DESC_BADD_MEM_BASE + (HMB_MAX_DESC_NUM * 8))

#define HMB_DESC_SIZE_MEM_BASE      (DW_ALIGNED(HMB_DESC_BADD_MEM_END))
#define rdwHMBDescSize              ((volatile u32 *)(HMB_DESC_SIZE_MEM_BASE)) // The number of MPS of HMB
#define HMB_DESC_SIZE_MEM_END       (HMB_DESC_SIZE_MEM_BASE + (HMB_MAX_DESC_NUM * 4))
#else
#define HMB_ACC_CNT_MEM_BASE        (DW_ALIGNED(SHARED_VAR_END))
#define HMB_DESC_SIZE_MEM_END       (HMB_ACC_CNT_MEM_BASE)
#endif

// save log
#if (EN_SAVE_LOG)
#define LOG_BASE                    (DW_ALIGNED(HMB_DESC_SIZE_MEM_END))
#define LOG_SIZE                    (16 * 4)//(sizeof(ftl_tf_glb_ctx))
#define g_log_ptr                   ((volatile ftl_save_log_buff *)LOG_BASE)
#define LOG_END                     (LOG_BASE + LOG_SIZE)
#else
#define LOG_END                     (HMB_DESC_SIZE_MEM_END)
#endif

#if 0
#if (EN_WAI_TEST)
#define WAI_TEST_MEM_BASE           (QW_ALIGNED(LOG_END))
#define WAI_TEST_MEM_SIZE           (400)
#define g_ftl_wai_caclu             ((volatile ftl_wai_caclu *)WAI_TEST_MEM_BASE)
#define WAI_TEST_MEM_END            (WAI_TEST_MEM_BASE + WAI_TEST_MEM_SIZE)
#else
#define WAI_TEST_MEM_END            (QW_ALIGNED(LOG_END))
#endif
#endif

#if (EN_TCG_OPAL)
#define R_OPAL_VAL_ONE_BASE         (QW_ALIGNED(LOG_END))
#define g_mbrlba                    (*(volatile u32 *)(R_OPAL_VAL_ONE_BASE + 0))    // 04 byte
#define g_first_launch              (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 4))    // 01 byte
#define g_opal_shadow               (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 5))
#define g_lock_flag                 (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 6))
#define g_genkey_flag               (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 7))    // 01 byte: rsp dummy,for test script
#define g_locking_entry             (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 8))    // 16 byte
#define g_aes_table_entry           (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 24))   // 64 byte add for aes xts mode
#define g_no_crypt_default_pwd      (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 88))   // 32 byte
#define g_for_revert_pwd            (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 120))  // 32 byte
#define g_default_auth_table        (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 152))  // 64 byte include KEK + HASH
#define g_xts_dummy_buf_ptr         (*(volatile word   *)(R_OPAL_VAL_ONE_BASE + 216))  // two byte
#define EN_TCG_OPAL_MEM_END         (R_OPAL_VAL_ONE_BASE + 220)

#elif (EN_AES)
#define R_OPAL_CRYPT_TWO_BASE       (QW_ALIGNED(WAI_TEST_MEM_END))
#define R_OPAL_CRYPT_TWO_END        (R_OPAL_CRYPT_TWO_BASE + 64)
#define g_aes_table_entry           (*(volatile byte   *)(R_OPAL_CRYPT_TWO_BASE))    //64 byte add for aes xts mode

#define R_OPAL_XTS_DUMMY_PTR_BASE   (DW_ALIGNED(R_OPAL_CRYPT_TWO_END))
#define EN_TCG_OPAL_MEM_END         (R_OPAL_XTS_DUMMY_PTR_BASE + 2)
#define g_xts_dummy_buf_ptr         (*(volatile word   *)(R_OPAL_XTS_DUMMY_PTR_BASE))
#else
#define EN_TCG_OPAL_MEM_END         (QW_ALIGNED(WAI_TEST_MEM_END))
#endif

#define SMART_ATTRI_APACER_MEM_BASE (QW_ALIGNED(EN_TCG_OPAL_MEM_END))
#define g_deep_read_retry           (*(volatile u32 *)(SMART_ATTRI_APACER_MEM_BASE))
#define g_read_retry                (*(volatile u32 *)(SMART_ATTRI_APACER_MEM_BASE + 4))
#define SMART_ATTRI_APACER_MEM_END  (SMART_ATTRI_APACER_MEM_BASE + 8)

#define APACER_ATTRI_MEM_BASE       (QW_ALIGNED(SMART_ATTRI_APACER_MEM_END))
#define r_merge_data_cnt            (*(volatile u64 *)(APACER_ATTRI_MEM_BASE))
#if (EN_FEATURE_APACER_CORE_POWER)
#define r_plp_step                  (*(volatile u8 *)(APACER_ATTRI_MEM_BASE + 8))
#define APACER_ATTRI_MEM_END        (APACER_ATTRI_MEM_BASE + 9)
#else
#define APACER_ATTRI_MEM_END        (APACER_ATTRI_MEM_BASE + 8)
#endif

#if (EN_NAND_REINIT_FEATURE)
#define NAND_REINIT_FLAG_MEM_BASE   (QW_ALIGNED(APACER_ATTRI_MEM_END))
#define r_nand_reinit_flag          (*(volatile u8 *)(NAND_REINIT_FLAG_MEM_BASE))
#define NAND_REINIT_FLAG_MEM_END    (NAND_REINIT_FLAG_MEM_BASE+1)
#else
#define NAND_REINIT_FLAG_MEM_END    (QW_ALIGNED(APACER_ATTRI_MEM_END))
#endif

#if (OPT_CALCULATE_ROSC_FREQ)
#define CALCULATE_ROSC_FREQ_MEM_BASE    (QW_ALIGNED(NAND_REINIT_FLAG_MEM_END))
#define r_actual_rosc_freq              (*(volatile u32 *)(CALCULATE_ROSC_FREQ_MEM_BASE))
#define B1TCM_USED_END                  (CALCULATE_ROSC_FREQ_MEM_BASE + 4)
#else
#define B1TCM_USED_END                  (QW_ALIGNED(NAND_REINIT_FLAG_MEM_END))
#endif

#if (B1TCM_USED_END > B1TCM_DPD_BASE)
#error "B1TCM over"
#endif
#else
#define ATCM_BASE                   (ATCM0_BASE)
#define B0TCM_BASE                  (PRIVATE_B0TCM_BASE)
#define B1TCM_BASE                  (PRIVATE_B1TCM_BASE)
#define ICACHE_BASE                 (I_CACHE0_MEM_BASE)
#define DCACHE_BASE                 (D_CACHE0_MEM_BASE)

#define B0TCM_END                   (B0TCM_BASE + B0TCM_SIZE)
#define B1TCM_END                   (B1TCM_BASE + B1TCM_SIZE)

#define B1TCM_DPD_BASE              (B1TCM_END - B1TCM_DPD_SIZE)
#define B1TCM_DPD_SIZE              (0x800)

// b0tcm
// cpu2 & cpu3
#define CMT_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(B0TCM_BASE + 0x8000))
#define CMT_BITMAP_END              (CMT_BITMAP_BASE + FTL_CMT_BITMAP_SIZE)
#define CHT_MEM_BASE                (FW_MEM_ADDR_ALIGNED(CMT_BITMAP_END))
#define CHT_MEM_END                 (CHT_MEM_BASE + FTL_CHT_ENTRY_NUM * 2)

// hmd related
#if (EN_TBL_31)
#if (EN_MT_HMB)
#define HMB_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(CHT_MEM_END))
 // 1K u8
#define HMB_BITMAP_END              (HMB_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_BUSY_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(HMB_BITMAP_END))
// 1K u8
#define HMB_BUSY_BITMAP_END         (HMB_BUSY_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_MEM_BASE                (FW_MEM_ADDR_ALIGNED(HMB_BUSY_BITMAP_END))
// 4 bytes each HMB entry
#define HMB_MEM_END                 (HMB_MEM_BASE + HMB_TBL_SIZE)
#else
#define HMB_MEM_END                 (FW_MEM_ADDR_ALIGNED(CHT_MEM_END))
#endif
#endif

#define B0TCM_USED_END              (HMB_MEM_END)

#if (B0TCM_USED_END >= (B0TCM_END))
#error "B0TCM_USED_END exceeds B0TCM_END."
#endif

// b1tcm
#if (OPT_SD_MISCOMPARE_DBG)
#define SD_MEM_ADDR_ALIGN           (4096)
#define SD_MEM_ADDR_ALIGN_MASK      (SD_MEM_ADDR_ALIGN - 1)
// #define SD_MEM_ADDR_ALIGNED(addr)   (((addr) + SD_MEM_ADDR_ALIGN_MASK) & (~(u32)SD_MEM_ADDR_ALIGN_MASK))
#define SD_MEM_ADDR_ALIGNED(addr)   (((addr) + SD_MEM_ADDR_ALIGN_MASK) & (0xfffff000))
#endif

// fifo
#if (1 < MAX_PAGES_PER_PU)
#if defined(YMTC_X2_9060)|| defined(YMTC_X3_9060)
#define WRCMD_INFO_NUM              (MAX_PAGES_PER_PU * PLANE_QNTY + 1)
#else
#define WRCMD_INFO_NUM              (3 * PLANE_QNTY * 2)
#endif
#else
#define WRCMD_INFO_NUM              (3 * PLANE_QNTY)
#endif

#define WRCMD_INFO_MASK             (WRCMD_INFO_NUM - 1)
#define RDCMD_INFO_NUM              (0x40)
#define RDCMD_INFO_MASK             (RDCMD_INFO_NUM - 1)
#define WR_ERR_INFO_NUM             (0x10)
#define WR_ERR_INFO_MASK            (WR_ERR_INFO_NUM - 1)

#define CB_INFO_NUM                 (0x100)
#define UNC_INFO_NUM                (0x80)
#define PRED_RD_INFO_NUM            (FRAG_QNTY * MAX_CHAN_NUM * 2)
#define PRED_RD_DESC_NUM            (CECH_QNTY * 3)
#define PRED_RD_CMD_NUM             (CECH_QNTY)
#define PRED_RD_CB_NUM              (0x08)

#define SUB_CMD_INFO_NUM            (2)
// ftl_cmd lff depth should be 1, for trim
#if (SUB_CMD_INFO_NUM > 2)
#error "SUB_CMD_INFO_NUM over"
#endif

#define FTL_CMD_INFO_NUM            (2)
#define SYS_USED_PAGE_QNTY          (56)
#define SYS_L2P_PAGE_QNTY           (SYS_USED_PAGE_QNTY)
//
#define LFF_WR_BASE                 (DW_ALIGNED(B1TCM_BASE))
#define LFF_WR_END                  (LFF_WR_BASE + WRCMD_INFO_NUM * 4)
#define rwWrCmdHeadDesc(n)          (*(volatile u16 *)(LFF_WR_BASE + (n << 2)))
#define rwWrCmdTailDesc(n)          (*(volatile u16 *)(LFF_WR_BASE + 2 + (n << 2)))
//
#define LFF_WR_ERR_BASE             (WD_ALIGNED(LFF_WR_END))
#define LFF_WR_ERR_END              (LFF_WR_ERR_BASE + WR_ERR_INFO_NUM * 2)
#define rwWrErrHeadDesc(n)          (*(volatile u16 *)(LFF_WR_ERR_BASE + (n << 1)))
//
#define LFF_RD_BASE                 (WD_ALIGNED(LFF_WR_ERR_END))
#define LFF_RD_END                  (LFF_RD_BASE + RDCMD_INFO_NUM * 2)
#define rwRdCmdDesc(n)              (*(volatile u16 *)(LFF_RD_BASE + (n << 1)))
//
#if (EN_PRED_RD_31)
#define LFF_PR_BASE                 (DW_ALIGNED(LFF_RD_END))
#define LFF_PR_DESC_BASE            (LFF_PR_BASE)
#define LFF_PR_DESC_END             (LFF_PR_DESC_BASE + PRED_RD_DESC_NUM * 8)
#define rwPRDescHead(n)             (*(volatile u16 *)(LFF_PR_DESC_BASE + 0 + (n << 3)))
#define rwPRDescTail(n)             (*(volatile u16 *)(LFF_PR_DESC_BASE + 2 + (n << 3)))
#define rdwPRDescCnt(n)             (*(volatile u32 *)(LFF_PR_DESC_BASE + 4 + (n << 3)))
//
#if (EN_PRED_RD_31_4CPU)
#define LFF_PR_CMD_BASE             (WD_ALIGNED(LFF_PR_DESC_END))
#define LFF_PR_CMD_END              (LFF_PR_CMD_BASE + PRED_RD_CMD_NUM * 4)
#define rwPRCmdHead(n)              (*(volatile u16 *)(LFF_PR_CMD_BASE + 0 + (n << 2)))
#define rwPRCmdCnt(n)               (*(volatile u16 *)(LFF_PR_CMD_BASE + 2 + (n << 2)))
//
#define LFF_PR_CB_BASE              (WD_ALIGNED(LFF_PR_CMD_END))
#define LFF_PR_CB_END               (LFF_PR_CB_BASE + PRED_RD_CB_NUM * 2)
#define rwPRCBHead(n)               (*(volatile u16 *)(LFF_PR_CB_BASE + 0 + (n << 1)))
//
#if (OPT_FCL_FW_RCQ)
#define LFF_PR_CB1_BASE             (WD_ALIGNED(LFF_PR_CB_END))
#define LFF_PR_CB1_END              (LFF_PR_CB1_BASE + PRED_RD_CB_NUM * 2)
#define rwPRCB1Head(n)              (*(volatile u16 *)(LFF_PR_CB1_BASE + 0 + (n << 1)))
#define LFF_PR_END                  (LFF_PR_CB1_END)
#else
#define LFF_PR_END                  (LFF_PR_CB_END)
#endif
#else
#define LFF_PR_END                  (LFF_PR_DESC_END)
#endif
#else
#define LFF_PR_BASE                 (LFF_RD_END)
#define LFF_PR_END                  (LFF_PR_BASE)
#endif

#define LFF_CB_BASE                 (WD_ALIGNED(LFF_PR_END))
#define LFF_CB_END                  (LFF_CB_BASE + CB_INFO_NUM * 2)
#define rwCbDesc(n)                 (*(volatile u16 *)(LFF_CB_BASE + (n << 1)))
//
#define LFF_UNC_BASE                (DW_ALIGNED(LFF_CB_END))
#define LFF_UNC_END                 (LFF_UNC_BASE + UNC_INFO_NUM * 16)
#define rqwUncDesc(m,n)             (*(volatile u64 *)(LFF_UNC_BASE + (m << 4) + (n << 3)))
//
#define LFF_CMD_BASE                (DW_ALIGNED(LFF_UNC_END))
#define LFF_CMD_END                 (LFF_CMD_BASE + SUB_CMD_INFO_NUM * 16) // only submit 1 cmd
#define rdwCmdaddr(m,n)             (volatile u32 *)(LFF_CMD_BASE + (m * 16) + (n << 2))
#define rbCmdaddr(m,n)              (volatile u8 *)(LFF_CMD_BASE + (m * 16) + (n << 2))

// front side alloc buf, and then submit to ftl
#define LFF_FRONT_BUF_ADDR          (LFF_CMD_END)
#if (EN_TCG_OPAL)
#define LFF_FRONT_BUF_END           (LFF_FRONT_BUF_ADDR + (FRAG_QNTY + HALF_OPAL_TABLE_BUF_SIZE + OPAL_DYNAMIC_BUF_ALLOC_MAX_NUM) * 4)
#else
#define LFF_FRONT_BUF_END           (LFF_FRONT_BUF_ADDR + FRAG_QNTY * 4)
#endif
//
#define LFF_FTL_CMD_BASE            (DW_ALIGNED(LFF_FRONT_BUF_END))
#define rdwFTLCmdInfo(m, n)         (*(volatile u32 *)(LFF_FTL_CMD_BASE + (m << 3) + (n << 2)))
#define LFF_FTL_CMD_END             (LFF_FTL_CMD_BASE + FTL_CMD_INFO_NUM * 8)
// #define LFF_VARIABLE_END            (DW_ALIGNED(LFF_FTL_CMD_END))
// 4 u8 for save current P clock
#define LFF_PER_CLOCK_BASE          (DW_ALIGNED(LFF_FTL_CMD_END))
#define LFF_PER_CLOCK               *(volatile u32 *)(LFF_PER_CLOCK_BASE)
#define LFF_PER_CLOCK_END           (LFF_PER_CLOCK_BASE + 4)
// (SYS_L2P_PAGE_QNTY / 8 u8 for sys_gc bitmap)
#define LFF_SYS_VLD_BIT_MAP_BASE    (LFF_PER_CLOCK_END)
#define LFF_SYS_VLD_BIT_MAP         (volatile u8 *)(LFF_SYS_VLD_BIT_MAP_BASE)
#define LFF_SYS_VLD_BIT_MAP_END     (LFF_SYS_VLD_BIT_MAP_BASE + (4 * ((SYS_L2P_PAGE_QNTY + 31) / 32)))
// 1 u8 for ftl handle result call back
#if (EN_FE_HOST_RW_FTL_IF)
#define LFF_FTL_FE_RESULT           (DW_ALIGNED(LFF_SYS_VLD_BIT_MAP_END))
#define r_ftl_fe_rslt               (*(volatile u32 *)(LFF_FTL_FE_RESULT))
#define LFF_FTL_FE_RESULT_END       (LFF_SYS_VLD_BIT_MAP_END + 4)
#else
#define LFF_FTL_FE_RESULT           (LFF_SYS_VLD_BIT_MAP_END)
#define LFF_FTL_FE_RESULT_END       (LFF_FTL_FE_RESULT)
#endif
// occupy 4 bytes for ecc feature use
// 1 u8 for ftl handle result call back
#define LFF_FRONT_RESULT_BASE       (LFF_FTL_FE_RESULT_END)
#define LFF_FRONT_RESULT            (volatile u8 *)(LFF_FRONT_RESULT_BASE)
#define LFF_FRONT_RESULT_END        (LFF_FRONT_RESULT_BASE + 1)

// 1 u8 for ftl DST handle result
#if (EN_DEVICE_SELF_TEST)
#define LFF_DST_RESULT_BASE         (LFF_FRONT_RESULT_END)
#define LFF_DST_RESULT              (volatile u8 *)(LFF_DST_RESULT_BASE)
#define LFF_DST_RESULT_END          (LFF_DST_RESULT_BASE + 1)
#else
#define LFF_DST_RESULT_BASE         (LFF_FRONT_RESULT_END)
#define LFF_DST_RESULT_END          (LFF_DST_RESULT_BASE)
#endif

// 1 u8 for ftl cmd result
#define LFF_BE_MEM_BASE             (LFF_DST_RESULT_END)
#define r_be_trim_rslt              (*(volatile u8 *)(LFF_BE_MEM_BASE))
#define LFF_BE_MEM_END              (LFF_BE_MEM_BASE + 1)

// 1 u8 for ftl cmd result
#define LFF_BE_FLUSH_BASE           (LFF_BE_MEM_END)
#define r_be_flush_rslt             (*(volatile u8 *)(LFF_BE_FLUSH_BASE))
#define LFF_BE_FLUSH_END            (LFF_BE_FLUSH_BASE + 1)

// 1 u8 for sys info gc
#define LFF_SYS_BLK_FULL_BASE       (LFF_BE_FLUSH_END)
#define LFF_SYS_BLK_FULL            (volatile u8 *)(LFF_SYS_BLK_FULL_BASE)
#define LFF_SYS_BLK_FULL_END        (LFF_SYS_BLK_FULL_BASE + 1)

// 2 u8 for UEFI code boot done label
#define LFF_UEFI_BOOT_DONE_BASE     (WD_ALIGNED(LFF_SYS_BLK_FULL_END))
#define LFF_UEFI_BOOT_DONE          *(volatile u16 *)(LFF_UEFI_BOOT_DONE_BASE)
#define LFF_UEFI_BOOT_DONE_END      (LFF_UEFI_BOOT_DONE_BASE + 2)

// 32 u8
#define LFF_CMP_CMD_BASE            (DW_ALIGNED(LFF_UEFI_BOOT_DONE_END))
#define LFF_CMP_CMD                 ((volatile ftl_cmp_cmd_info_t *)(LFF_CMP_CMD_BASE))
#define LFF_CMP_CMD_END             (LFF_CMP_CMD_BASE + 32)

// for ps3_1 exit time to do idle procedure
#define LFF_GC_FLAG_BASE            (DW_ALIGNED(LFF_CMP_CMD_END))
#define LFF_GC_FLAG                 *(volatile u32 *)(LFF_GC_FLAG_BASE)
#define LFF_GC_FLAG_END             (LFF_GC_FLAG_BASE + 4)

//
#if (EN_SAVE_LOG)
#define LFF_SYS_TIMER_BASE          (QW_ALIGNED(LFF_GC_FLAG_END))
#define LFF_SYS_TIMER               (*(volatile u64 *)(LFF_SYS_TIMER_BASE))
#define LFF_SYS_TIMER_END           (LFF_SYS_TIMER_BASE + 8)
#else
#define LFF_SYS_TIMER_BASE          (QW_ALIGNED(LFF_GC_FLAG_END))
#define LFF_SYS_TIMER_END           (LFF_SYS_TIMER_BASE)
#endif

// 1 u8 for force flush table before flush qbt
#define LFF_FORCE_FLUSH_TBL_BASE    (LFF_SYS_TIMER_END)
#define LFF_FORCE_FLUSH_TBL         *(volatile u8 *)(LFF_FORCE_FLUSH_TBL_BASE)
#define LFF_FORCE_FLUSH_TBL_END     (LFF_FORCE_FLUSH_TBL_BASE + 1)

// 1 u8 for reset download info in ftl
#define LFF_DL_RESET_BASE           (LFF_FORCE_FLUSH_TBL_END)
#define LFF_DL_RESET                *(volatile u8 *)(LFF_DL_RESET_BASE)
#define LFF_DL_RESET_END            (LFF_DL_RESET_BASE + 1)

// 1 u8 for FTL handle download result
#define LFF_DL_RESULT_BASE          (LFF_DL_RESET_END)
#define LFF_DL_RESULT               *(volatile u8 *)(LFF_DL_RESULT_BASE)
#define LFF_DL_RESULT_END           (LFF_DL_RESULT_BASE + 1)

// 1 u8 for download bin file check sum
#define LFF_DL_CHECK_SUM_BASE       (LFF_DL_RESULT_END)
#define LFF_DL_CHECK_SUM            *(volatile u8 *)(LFF_DL_CHECK_SUM_BASE)
#define LFF_DL_CHECK_SUM_END        (LFF_DL_CHECK_SUM_BASE + 1)

//
#define LFF_ERR_ENTRY_IDX_BASE      (DW_ALIGNED(LFF_DL_CHECK_SUM_END))
#define LFF_ERR_ENTRY_IDX           *(volatile u32 *)(LFF_ERR_ENTRY_IDX_BASE)
#define LFF_ERR_ENTRY_IDX_END       (LFF_ERR_ENTRY_IDX_BASE + 4)

//
#define LFF_ERR_BUF_IDX_BASE        (DW_ALIGNED(LFF_ERR_ENTRY_IDX_END))
#define LFF_ERR_BUF_IDX             *(volatile u32 *)(LFF_ERR_BUF_IDX_BASE)
#define LFF_ERR_BUF_IDX_END         (LFF_ERR_BUF_IDX_BASE + 4)

// 1 u8 for sys info write sts, if program fail, set sts = 1
#define LFF_SYS_WRITE_STS_BASE      (LFF_ERR_BUF_IDX_END)
#define LFF_SYS_WRITE_STS           *(volatile u8 *)(LFF_SYS_WRITE_STS_BASE)
#define LFF_SYS_WRITE_STS_END       (LFF_SYS_WRITE_STS_BASE + 1)

//
#define LFF_PAUSE_MERGE_BASE        (LFF_SYS_WRITE_STS_END)
#define LFF_PAUSE_MERGE             *(volatile u8 *)(LFF_PAUSE_MERGE_BASE)
#define LFF_PAUSE_MERGE_END         (LFF_PAUSE_MERGE_BASE + 1)

//
#define LFF_WRITE_PROTECT_BASE      (LFF_PAUSE_MERGE_END)
#define LFF_WRITE_PROTECT           *(volatile u8 *)(LFF_WRITE_PROTECT_BASE)
#define LFF_WRITE_PROTECT_END       (LFF_WRITE_PROTECT_BASE + 1)

//
#define LFF_IDLE_REQUEST_BASE       (LFF_WRITE_PROTECT_END)
#define LFF_IDLE_REQUEST            *(volatile u8 *)(LFF_IDLE_REQUEST_BASE)
#define LFF_IDLE_REQUEST_END        (LFF_IDLE_REQUEST_BASE + 1)

//
#define LFF_IDLE_TASK_BASE          (DW_ALIGNED(LFF_IDLE_REQUEST_END))
#define LFF_IDLE_TASK               *(volatile u32 *)(LFF_IDLE_TASK_BASE)
#define LFF_IDLE_TASK_END           (LFF_IDLE_TASK_BASE + 4)

// 4 u8
#if (EN_AES)
#define LFF_AES_BASE                (DW_ALIGNED(LFF_IDLE_TASK_END))
#define LFF_AES_KEY0                *(volatile u32 *)(LFF_AES_BASE)
#define LFF_AES_KEY0_END            (LFF_AES_BASE + 4)

// 4 u8
#define LFF_AES_KEY1                *(volatile u32 *)(DW_ALIGNED(LFF_AES_KEY0_END))
#define LFF_AES_KEY1_END            (LFF_AES_KEY0_END + 4)

// 4 u8
#define LFF_AES_KEY2                *(volatile u32 *)(DW_ALIGNED(LFF_AES_KEY1_END))
#define LFF_AES_KEY2_END            (LFF_AES_KEY1_END + 4)

// 4 u8
#define LFF_AES_KEY3                *(volatile u32 *)(DW_ALIGNED(LFF_AES_KEY2_END))
#define LFF_AES_KEY3_END            (LFF_AES_KEY2_END + 4)
#define LFF_AES_END                 (LFF_AES_KEY3_END)
#else
#define LFF_AES_BASE                (LFF_IDLE_TASK_END)
#define LFF_AES_END                 (LFF_AES_BASE)
#endif

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_BASE                 (LFF_AES_END)
#define LFF_PS_CPU0                 *(volatile u8 *)(LFF_PS_CPU0_BASE)
#define LFF_PS_CPU0_END             (LFF_AES_KEY3_END + 1)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_CPU1                 *(volatile u8 *)(LFF_PS_CPU0_END)
#define LFF_PS_CPU1_END             (LFF_PS_CPU0_END + 1)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_CPU2                 *(volatile u8 *)(LFF_PS_CPU1_END)
#define LFF_PS_CPU2_END             (LFF_PS_CPU1_END + 1)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_PS_CPU3                 *(volatile u8 *)(LFF_PS_CPU2_END)
#define LFF_PS_CPU3_END             (LFF_PS_CPU2_END + 1)
#define LFF_PS_END                  (LFF_PS_CPU3_END)

// cpu1 inform cpu2 to check if need to do idle procedure before enter PS4
#define LFF_NEED_IDLE_PRO_CHECK_BASE    (LFF_PS_END)
#define LFF_NEED_IDLE_PRO_CHECK         *(volatile u8 *)(LFF_NEED_IDLE_PRO_CHECK_BASE)
#define LFF_NEED_IDLE_PRO_CHECK_END     (LFF_NEED_IDLE_PRO_CHECK_BASE + 1)

// CPU1 inform CPU2 to invalid QBT in idle flow
#define LFF_INVALID_QBT_BASE        (LFF_NEED_IDLE_PRO_CHECK_END)
#define LFF_INVALID_QBT             *(volatile u8 *)(LFF_INVALID_QBT_BASE)
#define LFF_INVALID_QBT_END         (LFF_INVALID_QBT_BASE + 1)

#define LFF_TLC_START_BASE          (LFF_INVALID_QBT_END)
#define LFF_TLC_START               *(volatile u8 *)(LFF_TLC_START_BASE)
#define LFF_TLC_START_END           (LFF_TLC_START_BASE + 1)

#define LFF_THERMAL_RANGE_BASE      (LFF_TLC_START_END)
#define LFF_THERMAL_RANGE           *(volatile u8 *)(LFF_THERMAL_RANGE_BASE)
#define LFF_THERMAL_RANGE_END       (LFF_THERMAL_RANGE_BASE + 1)

#define LFF_PS_DIE_REQ_BASE         (LFF_THERMAL_RANGE_END)
#define LFF_PS_DIE_REQ              *(volatile u8 *)(LFF_PS_DIE_REQ_BASE)
#define LFF_PS_DIE_REQ_END          (LFF_PS_DIE_REQ_BASE + 1)

#define LFF_CPU1_ASSERT_FLAG_BASE   (LFF_PS_DIE_REQ_END)
#define LFF_CPU1_ASSERT_FLAG        *(volatile u8 *)(LFF_CPU1_ASSERT_FLAG_BASE)
#define LFF_CPU1_ASSERT_FLAG_END    (LFF_CPU1_ASSERT_FLAG_BASE + 1)

// = 1 means in QBT flush process
#define LFF_FLUSH_QBT_FLAG_BASE     (LFF_CPU1_ASSERT_FLAG_END)
#define LFF_FLUSH_QBT_FLAG          *(volatile u8 *)(LFF_FLUSH_QBT_FLAG_BASE)
#define LFF_FLUSH_QBT_FLAG_END      (LFF_FLUSH_QBT_FLAG_BASE + 1)

// format size, PBT will recover it first for quick init NVMe and PCIE
#define LFF_FORMAT_SIZE_BASE        (LFF_FLUSH_QBT_FLAG_END)
#define LFF_FORMAT_SIZE             *(volatile u8 *)(LFF_FORMAT_SIZE_BASE)
#define LFF_FORMAT_SIZE_END         (LFF_FORMAT_SIZE_BASE + 1)

// CPU assert, for debug use
#define LFF_ASSERT_FLAG_BASE        (LFF_FORMAT_SIZE_END)
#define LFF_ASSERT_FLAG             *(volatile u8 *)(LFF_ASSERT_FLAG_BASE)
#define LFF_ASSERT_FLAG_END         (LFF_ASSERT_FLAG_BASE + 1)

// exit L1 flag
#if (EN_SHUTTLE_CHIP) && (OPT_L1_WORKAROUND)
#define LFF_EXIT_L1_FLAG_BASE       (LFF_ASSERT_FLAG_END)
#define LFF_EXIT_L1_FLAG            *(volatile u8 *)(LFF_EXIT_L1_FLAG_BASE)
#define LFF_EXIT_L1_FLAG_END        (LFF_EXIT_L1_FLAG_BASE + 1)
#else
#define LFF_EXIT_L1_FLAG_BASE       (LFF_ASSERT_FLAG_END)
#define LFF_EXIT_L1_FLAG_END        (LFF_EXIT_L1_FLAG_BASE)
#endif

#if (EN_TBL_31)
#define MT_ACQ_PQ_BASE              (DW_ALIGNED(LFF_EXIT_L1_FLAG_END))
#define MT_ACQ_PQ_SIZE              (16) // real size is 6 bytes
#define g_acq_pq                    ((volatile swlist *)MT_ACQ_PQ_BASE)
#define MT_ACQ_PQ_END               (MT_ACQ_PQ_BASE + MT_ACQ_PQ_SIZE)

// HR_SQ
#define LFF_HR_SQ_FIFO_NUM          (256) // for dbg, after stable set as 64
#define MT_HR_SQ_ENTRY_SIZE         (8)
#define LFF_HR_SQ0_FIFO_BASE        (QW_ALIGNED(MT_ACQ_PQ_END))
#define LFF_HR_SQ0_FIFO_END         (LFF_HR_SQ0_FIFO_BASE + LFF_HR_SQ_FIFO_NUM * MT_HR_SQ_ENTRY_SIZE)

// HR_CQ
#define LFF_HR_CQ_FIFO_NUM          (256) // for dbg, after stable set as 64
#define MT_HR_CQ_ENTRY_SIZE         (4)
#define LFF_HR_CQ0_FIFO_BASE        (DW_ALIGNED(LFF_HR_SQ0_FIFO_END))
#define LFF_HR_CQ0_FIFO_END         (LFF_HR_CQ0_FIFO_BASE + LFF_HR_CQ_FIFO_NUM * MT_HR_CQ_ENTRY_SIZE)

// COMMAN_CMD_CQ
#define COM_CMD_CQ_FIFO_SIZE        (4 * FRAG_QNTY) // for dbg, after stable set as 128
#define COM_CMD_CQ_FIFO_ENTRY_SIZE  (4)
#define COM_CMD_CQ0_FIFO_BASE       (DW_ALIGNED(LFF_HR_CQ0_FIFO_END))
#define COM_CMD_CQ0_FIFO_END        (COM_CMD_CQ0_FIFO_BASE + COM_CMD_CQ_FIFO_SIZE * COM_CMD_CQ_FIFO_ENTRY_SIZE)

#if (EN_MT_HMB)
#if (EN_HMB_31)
// HMB CMD SQ
#define LFF_HMB_SQ_FIFO_NUM         (96)
#define MT_HMB_SQ_ENTRY_SIZE        (16)
// HMB CMD CQ
#define LFF_HMB_CQ_FIFO_NUM         (32)
#define MT_HMB_CQ_ENTRY_SIZE        (12)
#else
// HMB CMD SQ
#define LFF_HMB_SQ_FIFO_NUM         (32)
#define MT_HMB_SQ_ENTRY_SIZE        (8)
// HMB CMD CQ
#define LFF_HMB_CQ_FIFO_NUM         (32)
#define MT_HMB_CQ_ENTRY_SIZE        (16)
#endif
#define LFF_HMB_SQ0_FIFO_BASE       (QW_ALIGNED(COM_CMD_CQ0_FIFO_END))
#define LFF_HMB_SQ0_FIFO_END        (LFF_HMB_SQ0_FIFO_BASE + LFF_HMB_SQ_FIFO_NUM * MT_HMB_SQ_ENTRY_SIZE)
#define LFF_HMB_CQ0_FIFO_BASE       (QW_ALIGNED(LFF_HMB_SQ0_FIFO_END))
#define LFF_HMB_CQ0_FIFO_END        (LFF_HMB_CQ0_FIFO_BASE + LFF_HMB_CQ_FIFO_NUM * MT_HMB_CQ_ENTRY_SIZE)
#else
#define LFF_HMB_CQ0_FIFO_BASE       (COM_CMD_CQ0_FIFO_END)
#define LFF_HMB_CQ0_FIFO_END        (LFF_HMB_CQ0_FIFO_BASE)
#endif

#if (EN_MT_4CPU)
#define LFF_HR_SQ1_FIFO_BASE        (QW_ALIGNED(LFF_HMB_CQ0_FIFO_END))
#define LFF_HR_SQ1_FIFO_END         (LFF_HR_SQ1_FIFO_BASE + LFF_HR_SQ_FIFO_NUM * MT_HR_SQ_ENTRY_SIZE)
#define rstHR_SQ1_FIFO_CMD(m)       ((volatile u32 *)(LFF_HR_SQ1_FIFO_BASE + (m) * MT_HR_SQ_ENTRY_SIZE))

#define LFF_HR_CQ1_FIFO_BASE        (DW_ALIGNED(LFF_HR_SQ1_FIFO_END))
#define LFF_HR_CQ1_FIFO_END         (LFF_HR_CQ1_FIFO_BASE + LFF_HR_CQ_FIFO_NUM * MT_HR_CQ_ENTRY_SIZE)
#define rstHR_CQ1_FIFO_CMD(m)       ((volatile u32 *)(LFF_HR_CQ1_FIFO_BASE + (m) * MT_HR_CQ_ENTRY_SIZE))

#define COM_CMD_SQ_FIFO_SIZE        (128)
#define COM_CMD_SQ_FIFO_ENTRY_SIZE  (16)
#define COM_CMD_SQ0_FIFO_BASE       (DW_ALIGNED(LFF_HR_CQ1_FIFO_END))
#define COM_CMD_SQ0_FIFO_END        (COM_CMD_SQ0_FIFO_BASE + COM_CMD_SQ_FIFO_SIZE * COM_CMD_SQ_FIFO_ENTRY_SIZE)
#define rstCOM_SQ0_FIFO_CMD(m)      ((volatile u32 *)(COM_CMD_SQ0_FIFO_BASE + (m) * COM_CMD_SQ_FIFO_ENTRY_SIZE))

#define COM_CMD_SQ1_FIFO_BASE       (DW_ALIGNED(COM_CMD_SQ0_FIFO_END))
#define COM_CMD_SQ1_FIFO_END        (COM_CMD_SQ1_FIFO_BASE + COM_CMD_SQ_FIFO_SIZE * COM_CMD_SQ_FIFO_ENTRY_SIZE)
#define rstCOM_SQ1_FIFO_CMD(m)      ((volatile u32 *)(COM_CMD_SQ1_FIFO_BASE + (m) * COM_CMD_SQ_FIFO_ENTRY_SIZE))

#define COM_CMD_CQ1_FIFO_BASE       (DW_ALIGNED(COM_CMD_SQ1_FIFO_END))
#define COM_CMD_CQ1_FIFO_END        (COM_CMD_CQ1_FIFO_BASE + COM_CMD_CQ_FIFO_SIZE * COM_CMD_CQ_FIFO_ENTRY_SIZE)
#define rstCOM_CQ1_FIFO_CMD(m)      ((volatile u32 *)(COM_CMD_CQ1_FIFO_BASE + (m) * COM_CMD_CQ_FIFO_ENTRY_SIZE))

#if (EN_MT_HMB)
#define LFF_HMB_SQ1_FIFO_BASE       (QW_ALIGNED(COM_CMD_CQ1_FIFO_END))
#define LFF_HMB_SQ1_FIFO_END        (LFF_HMB_SQ1_FIFO_BASE + LFF_HMB_SQ_FIFO_NUM * MT_HMB_SQ_ENTRY_SIZE)
#define LFF_HMB_CQ1_FIFO_BASE       (QW_ALIGNED(LFF_HMB_SQ1_FIFO_END))
#define LFF_HMB_CQ1_FIFO_END        (LFF_HMB_CQ1_FIFO_BASE + LFF_HMB_CQ_FIFO_NUM * MT_HMB_CQ_ENTRY_SIZE)
#else
#define LFF_HMB_CQ1_FIFO_END        (COM_CMD_CQ1_FIFO_END)
#endif

// means 2 DW
#define T2F_FIFO_ENTRY_SIZE         (2)
#define T2F_FIFO_ENTRY_NUM          (128)

// tbl 2 ftl fifo
#define T2F_FIFO_SIZE_DW_SQ         (T2F_FIFO_ENTRY_NUM * T2F_FIFO_ENTRY_SIZE)
#define T2F_FIFO_SIZE_DW_CQ         (T2F_FIFO_ENTRY_NUM * T2F_FIFO_ENTRY_SIZE)
#define T2F_FIFO_SQ0_MEM_BASE       (FW_MEM_ADDR_ALIGNED(LFF_HMB_CQ1_FIFO_END))
#define T2F_FIFO_SQ0_MEM_END        (T2F_FIFO_SQ0_MEM_BASE + T2F_FIFO_SIZE_DW_SQ * 4)
//
#define T2F_FIFO_SQ1_MEM_BASE       (FW_MEM_ADDR_ALIGNED(T2F_FIFO_SQ0_MEM_END))
#define T2F_FIFO_SQ1_MEM_END        (T2F_FIFO_SQ1_MEM_BASE + T2F_FIFO_SIZE_DW_SQ * 4)

#define T2F_FIFO_CQ0_MEM_BASE       (FW_MEM_ADDR_ALIGNED(T2F_FIFO_SQ1_MEM_END))
#define T2F_FIFO_CQ0_MEM_END        (T2F_FIFO_CQ0_MEM_BASE + T2F_FIFO_SIZE_DW_CQ * 4)
#define T2F_FIFO_CQ1_MEM_BASE       (FW_MEM_ADDR_ALIGNED(T2F_FIFO_CQ0_MEM_END))
#define T2F_FIFO_CQ1_MEM_END        (T2F_FIFO_CQ1_MEM_BASE + T2F_FIFO_SIZE_DW_CQ * 4)
#define LFF_TBL31_FIFO_MEM_END      (T2F_FIFO_CQ1_MEM_END)
#else
#define LFF_TBL31_FIFO_MEM_END      (LFF_HMB_CQ0_FIFO_END)
#endif // EN_MT_4CPU
#else
#define LFF_TBL31_FIFO_MEM_END      (LFF_EXIT_L1_FLAG_END)
#endif // EN_TBL_31

// for idle close TLC head block
#if (EN_AES)
#define LFF_AES_KEY_FLAG_BASE       (LFF_TBL31_FIFO_MEM_END)
#define LFF_AES_KEY_FLAG            *(volatile u8 *)(LFF_AES_KEY_FLAG_BASE)
#define LFF_AES_KEY_FLAG_END        (LFF_AES_KEY_FLAG_BASE + 1)
// #define LFF_VARIABLE_BASE_END       (LFF_AES_KEY_FLAG_BASE)
#else
// #define LFF_VARIABLE_BASE_END       (LFF_AES_KEY_FLAG_BASE)
#define LFF_AES_KEY_FLAG_BASE       (LFF_TBL31_FIFO_MEM_END)
#define LFF_AES_KEY_FLAG_END        (LFF_AES_KEY_FLAG_BASE)
#endif

//
#if (EN_FCL_30)
#if (OPT_FCL_FW_RCQ)
#define FW_RCQ_ENTRY_SIZE           (1)  //one u32
#define FW_RCQ_ENTRY_NUM            (64)
#define FCL_FIFO_SIZE_FW_RCQ        (FW_RCQ_ENTRY_NUM * FW_RCQ_ENTRY_SIZE)
#define FCL_FIFO_FW_RCQ_MEM_BASE    (FW_MEM_ADDR_ALIGNED(LFF_AES_KEY_FLAG_END))
#define FCL_FIFO_FW_RCQ_MEM_END     (FCL_FIFO_FW_RCQ_MEM_BASE + FCL_FIFO_SIZE_FW_RCQ * 4)
#define FCL_FIFO_MEM_END            (FCL_FIFO_FW_RCQ_MEM_END)
#else
#define FCL_FIFO_MEM_END            (LFF_AES_KEY_FLAG_END)
#endif
#endif

//
#if (LUN_SHIFT == 0)
#define MT_TBL_BID_BASE             (FW_MEM_ADDR_ALIGNED(FCL_FIFO_MEM_END))
#define MT_TBL_BID_SIZE             (VTBL_BLK_MAX_NUM * 2 + 4 + VTBL_BLK_MAX_NUM)
#define g_mt_tbl_bid                ((volatile ftl_tbmt_bid *)MT_TBL_BID_BASE)
#define MT_TBL_BID_END              (MT_TBL_BID_BASE + MT_TBL_BID_SIZE)
#endif

//
#if (OPT_CMT_BYPASS)
#if (LUN_SHIFT == 0)
#define MT_BYPASS_BASE              (FW_MEM_ADDR_ALIGNED(MT_TBL_BID_END))
#else
#define MT_BYPASS_BASE              (FW_MEM_ADDR_ALIGNED(FCL_FIFO_MEM_END))
#endif
#define MT_BYPASS_SIZE              (4)
#define g_cmt_bypass                (*(volatile u32 *)MT_BYPASS_BASE)
#define MT_BYPASS_END               (MT_BYPASS_BASE + MT_BYPASS_SIZE)
#define MT_SERVICE_SN_1_BASE        (FW_MEM_ADDR_ALIGNED(MT_BYPASS_END))
#else
#if (LUN_SHIFT == 0)
#define MT_SERVICE_SN_1_BASE        (FW_MEM_ADDR_ALIGNED(MT_TBL_BID_END))
#else
#define MT_SERVICE_SN_1_BASE        (FW_MEM_ADDR_ALIGNED(FCL_FIFO_MEM_END))
#endif
#endif
#define MT_SERVICE_SN_1_SIZE        (2 * 4)
#define MT_SERVICE_SN_1_END         (MT_SERVICE_SN_1_BASE + MT_SERVICE_SN_1_SIZE)

//
#if (EN_MT_4CPU)
// Same as PROG_V2P_SIZE
#define PROG_V2P_ARRAY_SIZE         (MAX_DIE_QNTY * MAX_PAGES_PER_PROG_SEQ * PLANE_QNTY * MAX_SUPER_PAGES_FOR_ONE_RAID_STRIPE)
#define PROG_V2P_ARRAY_BASE         (FW_MEM_ADDR_ALIGNED(MT_SERVICE_SN_1_END))
#define PROG_V2P_ARRAY_END          (PROG_V2P_ARRAY_BASE + PROG_V2P_ARRAY_SIZE * 2)
// Same as PROG_P2V_SIZE
#define PROG_P2V_ARRAY_SIZE         ((MAX_SUPER_PAGES_FOR_ONE_RAID_STRIPE * MAX_PAGES_PER_PROG_SEQ) << (VDIE_SHIFT + PLANE_SHIFT))
#define PROG_P2V_ARRAY_BASE         (FW_MEM_ADDR_ALIGNED(PROG_V2P_ARRAY_END))
#define PROG_P2V_ARRAY_END          (PROG_P2V_ARRAY_BASE + PROG_P2V_ARRAY_SIZE * 2)

#define SHARE_CMT_BASE              (FW_MEM_ADDR_ALIGNED(PROG_P2V_ARRAY_END))
#define SHARE_CMT_SIZE              (15 * 4) // (sizeof(ftl_share_cmt))
#define SHARE_CMT_END               (SHARE_CMT_BASE + SHARE_CMT_SIZE)

#define SHARE_CMUT_BASE             (FW_MEM_ADDR_ALIGNED(SHARE_CMT_END))
#define SHARE_CMUT_SIZE             (4) // (sizeof(ftl_share_cmut))
#define SHARE_CMUT_END              (SHARE_CMUT_BASE + SHARE_CMUT_SIZE)

#define GLOBAL_TF_BASE              (FW_MEM_ADDR_ALIGNED(SHARE_CMUT_END))
#define GLOBAL_TF_SIZE              (20 * 4) // (sizeof(ftl_tf_glb_ctx))
#define g_tf_glb_ctx_ptr            ((volatile ftl_tf_glb_ctx *)GLOBAL_TF_BASE)
#define GLOBAL_TF_END               (GLOBAL_TF_BASE + GLOBAL_TF_SIZE)

#define SYNC_CMD_SQ_BASE            (FW_MEM_ADDR_ALIGNED(GLOBAL_TF_END))
#define SYNC_CMD_SQ_SIZE            (2 * 6 * 4) // sizeof(SYNC_OPS_SQ_FORMAT)
#define SYNC_CMD_SQ_END             (SYNC_CMD_SQ_BASE + SYNC_CMD_SQ_SIZE)

#define SYNC_CMD_CQ_BASE            (FW_MEM_ADDR_ALIGNED(SYNC_CMD_SQ_END))
#define SYNC_CMD_CQ_SIZE            (2 * 2 * 4) // sizeof(SYNC_OPS_CQ_FORMAT)
#define SYNC_CMD_CQ_END             (SYNC_CMD_CQ_BASE + SYNC_CMD_CQ_SIZE)

#define DEBUG_CMD_SQ_BASE           (FW_MEM_ADDR_ALIGNED(SYNC_CMD_CQ_END))
#define DEBUG_CMD_SQ_SIZE           (2 * 6 * 4) // sizeof(SYNC_OPS_SQ_FORMAT)
#define DEBUG_CMD_SQ_END            (DEBUG_CMD_SQ_BASE + DEBUG_CMD_SQ_SIZE)

#define DEBUG_CMD_CQ_BASE           (FW_MEM_ADDR_ALIGNED(DEBUG_CMD_SQ_END))
#define DEBUG_CMD_CQ_SIZE           (2 * 2 * 4) // sizeof(SYNC_OPS_CQ_FORMAT)
#define DEBUG_CMD_CQ_END            (DEBUG_CMD_CQ_BASE + DEBUG_CMD_CQ_SIZE)

#define DEBUG_MODE_FLAG             (FW_MEM_ADDR_ALIGNED(DEBUG_CMD_CQ_END))
#define DEBUG_MODE                  (0x12345678) // 1 u32
#define DEBUG_MODE_FLAG_END         (FW_MEM_ADDR_ALIGNED(DEBUG_CMD_CQ_END) + 4)
#define LFF_END                     (FW_MEM_ADDR_ALIGNED(DEBUG_MODE_FLAG_END))
#else
#define LFF_END                     (MT_SERVICE_SN_1_END)
#endif

// table
#if (OPT_TCM_DESC)
#define FW_CMD_DESC0_BASE            (FW_MEM_ADDR_ALIGNED(LFF_END))
#else
#define FW_CMD_DESC0_BASE            (SRAM_MEM_BASE)
#endif

#if (!OPT_FCL_IF_DESC_CONFIG2)
#if (META_DATA_SIZE == META_SIZE_8DW)
#define FW_CMD_DESC_SIZE            (64)
#elif (META_DATA_SIZE == META_SIZE_6DW)
#define FW_CMD_DESC_SIZE            (56)
#elif (META_DATA_SIZE == META_SIZE_4DW)
#define FW_CMD_DESC_SIZE            (48)
#elif (META_DATA_SIZE == META_SIZE_3DW)
#define FW_CMD_DESC_SIZE            (44)
#elif (META_DATA_SIZE == META_SIZE_2DW)
#define FW_CMD_DESC_SIZE            (40)
#else
#error "META_DATA_SIZE NOT SUPPORT.2"
#endif
#else
#if (META_DATA_SIZE == META_SIZE_8DW)
#define FW_CMD_DESC_SIZE            (68)
#elif (META_DATA_SIZE == META_SIZE_6DW)
#define FW_CMD_DESC_SIZE            (60)
#elif (META_DATA_SIZE == META_SIZE_4DW)
#define FW_CMD_DESC_SIZE            (52)
#elif (META_DATA_SIZE == META_SIZE_3DW)
#define FW_CMD_DESC_SIZE            (48)
#elif (META_DATA_SIZE == META_SIZE_2DW)
#define FW_CMD_DESC_SIZE            (44)
#else
#error "META_DATA_SIZE NOT SUPPORT.2"
#endif
#endif

#define FW_CMD_DESC0_END             (FW_CMD_DESC_NUM * FW_CMD_DESC_SIZE + FW_CMD_DESC0_BASE)

#if (OPT_BUF_USE_DBG2) || (OPT_DESC_USE_DBG2)
#define DRAM_BUF_HW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(FW_CMD_DESC0_END))
#define DRAM_BUF_HW_BITMAP_END      (DRAM_BUF_HW_BITMAP_BASE + (DATA_BUF_DRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define DRAM_BUF_FW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(DRAM_BUF_HW_BITMAP_END))
#define DRAM_BUF_FW_BITMAP_END      (DRAM_BUF_FW_BITMAP_BASE + (DATA_BUF_DRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define SRAM_BUF_HW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(DRAM_BUF_FW_BITMAP_END))
#define SRAM_BUF_HW_BITMAP_END      (SRAM_BUF_HW_BITMAP_BASE + (DATA_BUF_SRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define SRAM_BUF_FW_BITMAP_BASE     (FW_MEM_ADDR_ALIGNED(SRAM_BUF_HW_BITMAP_END))
#define SRAM_BUF_FW_BITMAP_END      (SRAM_BUF_FW_BITMAP_BASE + (DATA_BUF_SRAM_LL_ENTRY_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC_HW_BITMAP_BASE         (FW_MEM_ADDR_ALIGNED(SRAM_BUF_FW_BITMAP_END))
#define DESC_HW_BITMAP_END          (DESC_HW_BITMAP_BASE + (FW_CMD_DESC_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC_FW_BITMAP_BASE         (FW_MEM_ADDR_ALIGNED(DESC_HW_BITMAP_END))
#define DESC_FW_BITMAP_END          (DESC_FW_BITMAP_BASE + (FW_CMD_DESC_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC1_HW_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(DESC_FW_BITMAP_END))
#define DESC1_HW_BITMAP_END         (DESC1_HW_BITMAP_BASE + (FW_CMD_DESC1_NUM >> 3) + 1)// 4k buf use 1bit

#define DESC1_FW_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(DESC1_HW_BITMAP_END))
#define DESC1_FW_BITMAP_END         (DESC1_FW_BITMAP_BASE + (FW_CMD_DESC1_NUM >> 3) + 1)// 4k buf use 1bit
#define DESC_TEMP_END               (DESC1_FW_BITMAP_END)
#else
#define DESC_TEMP_END               (FW_CMD_DESC0_END)
#endif

// hw trim vd
// this address range is re-used by PBT
#define HW_TRIM_VC_BASE             (FW_MEM_ADDR_ALIGNED(DESC_TEMP_END))
#define HW_TRIM_VC_SIZE             (VBLOCK_NUM * 4)
#define HW_TRIM_VC_END              (HW_TRIM_VC_BASE + HW_TRIM_VC_SIZE)
#if (EN_MT_4CPU)
// this address range is re-used by PBT
#define HW_TRIM_VC_BASE2            (FW_MEM_ADDR_ALIGNED(HW_TRIM_VC_END))
#define HW_TRIM_VC_END2             (HW_TRIM_VC_BASE2 + HW_TRIM_VC_SIZE)
#else
#define HW_TRIM_VC_END2             (HW_TRIM_VC_END)
#endif

// wc hash tbl
#define WC_HASH_TBL_BASE            (FW_MEM_ADDR_ALIGNED(HW_TRIM_VC_END2))
#define WC_HASH_TBL_END             (WC_HASH_TBL_BASE + WRITE_CAHCE_HASH_TBL_ENTRY_NUM * 2)

// rowc
#if !defined(ONE_PASS_MODE)
#define ROWC_BASE                   (FW_MEM_ADDR_ALIGNED(WC_HASH_TBL_END))
#define ROWC_SIZE                   (PAGE_QNTY + 1)
#define ROWC_END                    (FW_MEM_ADDR_ALIGNED(ROWC_BASE + ROWC_SIZE))
#define g_share_page_loc            ((volatile byte *)ROWC_BASE)
#else
#define ROWC_END                    (WC_HASH_TBL_END)
#endif

// cmt
#define CMT_MEM_BASE                (FW_MEM_ADDR_ALIGNED(ROWC_END))
#define CMT_MEM_END                 (CMT_MEM_BASE + FTL_CMT_ENTRY_NUM * 12) // 12 bytes each CMT entry

// cmt hash
#define CMT_HASH_MEM_BASE           (FW_MEM_ADDR_ALIGNED(CMT_MEM_END))
#define CMT_HASH_MEM_END            (CMT_HASH_MEM_BASE + FTL_CMT_ENTRY_NUM * 2)

#define CMUT_MEM_BASE               (FW_MEM_ADDR_ALIGNED(CMT_HASH_MEM_END))
#define CMUT_MEM_END                (CMUT_MEM_BASE + FTL_CMUT_ENTRY_NUM * 8) // 8 bytes each CMUT entry

// hmb, for 2 cpu
#if (EN_TBL_31)
#if (EN_MT_2CPU)
#if (EN_MT_HMB)
#define HMB_BITMAP_BASE             (FW_MEM_ADDR_ALIGNED(CMUT_MEM_END))
#define HMB_BITMAP_END              (HMB_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_BUSY_BITMAP_BASE        (FW_MEM_ADDR_ALIGNED(HMB_BITMAP_END))
#define HMB_BUSY_BITMAP_END         (HMB_BUSY_BITMAP_BASE + HMB_BITMAP_SIZE)
#define HMB_MEM_BASE                (FW_MEM_ADDR_ALIGNED(HMB_BUSY_BITMAP_END))
// 4 bytes each HMB entry
#define HMB_MEM_END                 (HMB_MEM_BASE + HMB_LOG_ENTRY_NUM * 4)
#else
#define HMB_MEM_END                 (FW_MEM_ADDR_ALIGNED(CMUT_MEM_END))
#endif
#define B1TCM_TABLE_END0            (FW_MEM_ADDR_ALIGNED(HMB_MEM_END))
#else
#define B1TCM_TABLE_END0            (FW_MEM_ADDR_ALIGNED(CMUT_MEM_END))
#endif
#endif

// aes key
#if (EN_AES)
#define AES_KEY_BASE                (FW_MEM_ADDR_ALIGNED(B1TCM_TABLE_END0)) // CHK PERF_TUNING g_host_wr_4K_time
// 1: aes 128 mode: 16 bytes; 2: aes 256 mode: 32 bytes
#define AES_KEY_END                 (AES_KEY_BASE + 64)
#else
#define AES_KEY_END                 (FW_MEM_ADDR_ALIGNED(B1TCM_TABLE_END0))
#endif

// flush laa
#define FTL_FLUSH_LAA_TEMP_BASE     (FW_MEM_ADDR_ALIGNED(AES_KEY_END)) // CHK PERF_TUNING g_host_wr_4K_time
#define FTL_FLUSH_LAA_TEMP_END      (FTL_FLUSH_LAA_TEMP_BASE + FRAG_QNTY * MAX_PAGES_PER_PROG_SEQ * 4) // 4 byte per frag * one page frag * seq prog num
#define FTL_MERGE_LAA_TEMP_BASE     (FW_MEM_ADDR_ALIGNED(FTL_FLUSH_LAA_TEMP_END))
#define FTL_MERGE_LAA_TEMP_END      (FTL_MERGE_LAA_TEMP_BASE + FRAG_QNTY * MAX_PAGES_PER_PROG_SEQ * 4) // 4 byte per frag * one page frag * seq prog num

#define B1TCM_TABLE_END             (FTL_MERGE_LAA_TEMP_END)

// shared var
#define SHARED_VAR_BASE             (FW_MEM_ADDR_ALIGNED(B1TCM_TABLE_END))

#define rbWrCmdInfoWrPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0000))
#define rbWrCmdInfoRdPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0001))
#define rbRdCmdInfoWrPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0002)) // use for pred rd
#define rbRdCmdInfoRdPtr            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0003)) // use for pred rd
#define r_fua_flag                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x0004))
#define r_flush_dummy               (*(volatile u8 *)(SHARED_VAR_BASE + 0x0005))
#define g_dummy_buf_ptr             (*(volatile u16 *)(SHARED_VAR_BASE + 0x0006))
#define r_last_fua_desc             (*(volatile u16 *)(SHARED_VAR_BASE + 0x0008))
#define r_pause_write               (*(volatile u8 *)(SHARED_VAR_BASE + 0x000a))
#define g_ftl_trim_flag             (*(volatile u8 *)(SHARED_VAR_BASE + 0x000b))
#define g_trim_insert_cmt           (*(volatile u16 *)(SHARED_VAR_BASE + 0x000c))

#define r_load_code_id              (*(volatile u8 *)(SHARED_VAR_BASE + 0x000e)) // code bank
#define r_load_code_flag            (*(volatile u8 *)(SHARED_VAR_BASE + 0x000f)) // code bank

#define r_power_down                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0010))
#define r_en_plp_or_pld             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0011)) // ==1, enable PLD; ==2, enable PLP; ==0, disable both;
#define r_bios_end                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x0012))
#define r_reset_assert              (*(volatile u8 *)(SHARED_VAR_BASE + 0x0013))
/*******************************************************************
r_cpu1_ready:
now use 3bit for QBT and PBT to inform CPU1 that CPU2 ready status
QBT: when QBT first step done, it will set r_cpu1_ready bit0 and bit7 to 1;when second step done, it will clear bit7 to 0
PBT:  when PBT recover sys info done,it will set bit1 to 1; when PBT all done, it will set bit 0 to 1
so r_cpu1_ready has below status:
1.r_cpu1_ready = 0x81, QBT first step done;  CPU1 can normal boot and get command from host, no need to wait QBT load tbl done
2.r_cpu1_ready = 0x01, QBT load table done; CPU1 can send flush command in reset flow
3.r_cpu1_ready = 0x02, PBT recover sys info done; CPU1 can init PCIE, NVMe controller, but can not alloc buffer and desc;
so CPU1 will always return in create queue command and alloc buffer until PBT done
4.r_cpu1_ready = 0x03, PBT all done; CPU1 can create queue and receive read/write command
*********************************************************************/
#define r_cpu1_ready                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0014))
#define r_print_select              (*(volatile u8 *)(SHARED_VAR_BASE + 0x0015))
#define rbehm                       (*(volatile u8 *)(SHARED_VAR_BASE + 0x0016)) // hmb enable or not

#define r_trim_num                  (*(volatile u16 *)(SHARED_VAR_BASE + 0x0018))
#define rw_rct_hmb_num              (*(volatile u16 *)(SHARED_VAR_BASE + 0x001a)) // real hmb rct entry num, hmb.free_list_2k.cnt
#define rw_rct_hmb_tbl_num          (*(volatile u16 *)(SHARED_VAR_BASE + 0x001c)) // real hmb rct tbl num, hmb.free_list_4k.cnt

#define g_qbt_valid                 (*(volatile u8 *)(SHARED_VAR_BASE + 0x001e))
#define r_host_aes_flag             (*(volatile u8 *)(SHARED_VAR_BASE + 0x001f))

#define g_ftl_se_cmd_step           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0020)) // byte
#define g_sanitize_action           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0021)) // byte
#define g_cmd_info_status           (*(volatile u16 *)(SHARED_VAR_BASE + 0x0022)) // word

#define r_total_spare_blk           (*(volatile u32 *)(SHARED_VAR_BASE + 0x0024))
#define r_dbg_val                   (*(volatile u32 *)(SHARED_VAR_BASE + 0x0028))
#define rdwHMBEntryCnt              (*(volatile u32 *)(SHARED_VAR_BASE + 0x002c)) // How many HMB desc

#define r_prev_rbehm                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0030)) // for hmb with L1.2 PS4
#define r_req_rbehm                 (*(volatile u8 *)(SHARED_VAR_BASE + 0x0031))
#define r_en_debug_print            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0032))
#define r_share_tmr_isr             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0033))

#define r_all_zero_mode             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0034)) // 1:ALL 0 mode begin, 0: ALL 0 mode close
#define r_push_blk_proc_fail        (*(volatile u8 *)(SHARED_VAR_BASE + 0x0035))

#define r_cpu2_ready                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0036))
#define r_cpu3_ready                (*(volatile u8 *)(SHARED_VAR_BASE + 0x0037))

#define g_page_sn                   (*(volatile u64 *)(SHARED_VAR_BASE + 0x0038))

#define r_front_mutex_req           (*(volatile byte *)(SHARED_VAR_BASE + 0x0040))
#define r_front_mutex_rslt          (*(volatile byte *)(SHARED_VAR_BASE + 0x0041))
#define r_host_w_need_au_num        (*(volatile u16 *)(SHARED_VAR_BASE + 0x0042))
#define r_wc_flush_list_cnt         (*(volatile u16 *)(SHARED_VAR_BASE + 0x0044))
#define r_rd_flying_cnt             (*(volatile u16 *)(SHARED_VAR_BASE + 0x0046))

#define rdwHostHmbBuff              ((volatile u32 *)(SHARED_VAR_BASE + 0x0048)) // rdwHostHmbBuff need 2 dword
#define rdwHostHmbBuff_rsvd         ((volatile u32 *)(SHARED_VAR_BASE + 0x004c))

#define rblk_record                 (*(volatile u16 *)(SHARED_VAR_BASE + 0x0054))
#define r_blk_record_flag           (*(volatile u16 *)(SHARED_VAR_BASE + 0x0056))

#define r_qbt_erase_cnt             (*(volatile u32 *)(SHARED_VAR_BASE + 0x0058))

#define rbHMBsplitDesc              (*(volatile u8 *)(SHARED_VAR_BASE + 0x005c))
#define rbsoftRstFlag               (*(volatile u8 *)(SHARED_VAR_BASE + 0x005d))
#define r_aes_critica_flag          (*(volatile u8 *)(SHARED_VAR_BASE + 0x005e))
#define g_power_loss                (*(volatile u8 *)(SHARED_VAR_BASE + 0x005f))

#define g_flash_rd_seccnt           (*(volatile u64 *)(SHARED_VAR_BASE + 0x0060))
#define g_flash_wr_seccnt           (*(volatile u64 *)(SHARED_VAR_BASE + 0x0068))

#define rdwCRCCnt                   (*(volatile u32 *)(SHARED_VAR_BASE + 0x0070))

#define r_ps3_fc_prepare_done       (*(volatile u8 *)(SHARED_VAR_BASE + 0x0074))
#define r_ps3_ps4_rst_dco           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0075))
#define r_power_on                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x0076))
#define r_rd_sz_less_2MB            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0077))

#define r_hr_rsp_to_pq_vld          (*(volatile u8 *)(SHARED_VAR_BASE + 0x0078))
#define r_force_pop_hc_wr           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0079))
#define r_force_pop_vld             (*(volatile u8 *)(SHARED_VAR_BASE + 0x007a))
#define r_raid_close_super_wl       (*(volatile u8 *)(SHARED_VAR_BASE + 0x007b))
#define r_free_desc_cnt             (*(volatile u32 *)(SHARED_VAR_BASE + 0x007c))

#define r_rd_size                   (*(volatile u32 *)(SHARED_VAR_BASE + 0x0080)) // for pred rd perf tune
#define r_rd_hit_wr_lff             (*(volatile u8 *)(SHARED_VAR_BASE + 0x0084))
#define r_dis_seq_hrsd              (*(volatile u8 *)(SHARED_VAR_BASE + 0x0085))
#define r_pred_rd_cnt_thr           (*(volatile u8 *)(SHARED_VAR_BASE + 0x0086))
#define r_pred_read_step            (*(volatile u8 *)(SHARED_VAR_BASE + 0x0087))
#define r_pred_total_cnt            (*(volatile u16 *)(SHARED_VAR_BASE + 0x0088))
#define r_pred_read_smp_cnt         (*(volatile u32 *)(SHARED_VAR_BASE + 0x008c))
#if (EN_PRED_RD_31)
// 8 dword
#define g_ftl_pred_rd_mgr           ((volatile pred_rd_mgr_type *)(SHARED_VAR_BASE + 0x0090))
#else
// 5 dword
#define g_ftl_pred_rd_mgr           ((volatile ftl_pred_rd_mgr *)(SHARED_VAR_BASE + 0x0090))
#endif

// #define r_nand_temp_value           (*(volatile u16 *)(SHARED_VAR_BASE + 0x00AC))
// #define r_tlc_data_flying_flag      (*(volatile u8 *)(SHARED_VAR_BASE + 0x00AE))

#define r_slot_fr                   ((volatile u64 *)(SHARED_VAR_BASE + 0x00C8)) //32 bytes
#define r_dl_chk_sum_flag           (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM))
#define r_dl_success                (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 1))
#define r_fw_update_flag            (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 2))
#define r_slot_afi                  (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 3))
#if (EN_MT_4CPU)
#define g_in_pbt_process            (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 4))
#endif

#define r_tlc_data_flying_flag      (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 5))
#define r_nand_temp_value           (*(volatile u16 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 6))
#define r_mismatch_flag             (*(volatile u8 *)(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 8))

#if (EN_READ_PATROL)
#define r_read_patrol_to_do         (*(volatile u32 *)(DW_ALIGNED(SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 12)))
#define SHARED_VAR_END              (SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 16)
#else
#define SHARED_VAR_END              (SHARED_VAR_BASE + 0x00C8 + 8 * MAX_SLOT_NUM + 12)
#endif



#if (EN_NVME_HMB)
#define HMB_ACC_CNT_MEM_BASE        (DW_ALIGNED(SHARED_VAR_END))

#define rdwHMBAccCnt                ((volatile u32 *)(HMB_ACC_CNT_MEM_BASE)) // How many HMB entry in these HMB desc, including 4K entry & 2K entry
#define HMB_ACC_CNT_MEM_END         (HMB_ACC_CNT_MEM_BASE + (HMB_MAX_DESC_NUM * 4))

#define HMB_DESC_BADD_MEM_BASE      (QW_ALIGNED(HMB_ACC_CNT_MEM_END))
#define rqwHMBDescBAdd              ((volatile u64 *)(HMB_DESC_BADD_MEM_BASE)) // The base addr of this HMB desc
#define HMB_DESC_BADD_MEM_END       (HMB_DESC_BADD_MEM_BASE + (HMB_MAX_DESC_NUM * 8))

#define HMB_DESC_SIZE_MEM_BASE      (DW_ALIGNED(HMB_DESC_BADD_MEM_END))
#define rdwHMBDescSize              ((volatile u32 *)(HMB_DESC_SIZE_MEM_BASE)) // The number of MPS of HMB
#define HMB_DESC_SIZE_MEM_END       (HMB_DESC_SIZE_MEM_BASE + (HMB_MAX_DESC_NUM * 4))
#else
#define HMB_ACC_CNT_MEM_BASE        (DW_ALIGNED(SHARED_VAR_END))
#define HMB_DESC_SIZE_MEM_END       (HMB_ACC_CNT_MEM_BASE)
#endif

// save log
#if (EN_SAVE_LOG)
#define LOG_BASE                    (DW_ALIGNED(HMB_DESC_SIZE_MEM_END))
#define LOG_SIZE                    (16 * 4)//(sizeof(ftl_tf_glb_ctx))
#define g_log_ptr                   ((volatile ftl_save_log_buff *)LOG_BASE)
#define LOG_END                     (LOG_BASE + LOG_SIZE)
#else
#define LOG_END                     (HMB_DESC_SIZE_MEM_END)
#endif

#if 0
#if (EN_WAI_TEST)
#define WAI_TEST_MEM_BASE           (QW_ALIGNED(LOG_END))
#define WAI_TEST_MEM_SIZE           (400)
#define g_ftl_wai_caclu             ((volatile ftl_wai_caclu *)WAI_TEST_MEM_BASE)
#define WAI_TEST_MEM_END            (WAI_TEST_MEM_BASE + WAI_TEST_MEM_SIZE)
#else
#define WAI_TEST_MEM_END            (QW_ALIGNED(LOG_END))
#endif
#endif

#if (EN_TCG_OPAL)
#define R_OPAL_VAL_ONE_BASE         (QW_ALIGNED(LOG_END))
#define g_mbrlba                    (*(volatile u32 *)(R_OPAL_VAL_ONE_BASE + 0))    // 04 byte
#define g_first_launch              (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 4))    // 01 byte
#define g_opal_shadow               (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 5))
#define g_lock_flag                 (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 6))
#define g_genkey_flag               (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 7))    // 01 byte: rsp dummy,for test script
#define g_locking_entry             (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 8))    // 16 byte
#define g_aes_table_entry           (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 24))   // 64 byte add for aes xts mode
#define g_no_crypt_default_pwd      (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 88))   // 32 byte
#define g_for_revert_pwd            (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 120))  // 32 byte
#define g_default_auth_table        (*(volatile byte   *)(R_OPAL_VAL_ONE_BASE + 152))  // 64 byte include KEK + HASH
#define g_xts_dummy_buf_ptr         (*(volatile word   *)(R_OPAL_VAL_ONE_BASE + 216))  // two byte
#define EN_TCG_OPAL_MEM_END         (R_OPAL_VAL_ONE_BASE + 220)

#elif (EN_AES)
#define R_OPAL_CRYPT_TWO_BASE       (QW_ALIGNED(WAI_TEST_MEM_END))
#define R_OPAL_CRYPT_TWO_END        (R_OPAL_CRYPT_TWO_BASE + 64)
#define g_aes_table_entry           (*(volatile byte   *)(R_OPAL_CRYPT_TWO_BASE))    //64 byte add for aes xts mode

#define R_OPAL_XTS_DUMMY_PTR_BASE   (DW_ALIGNED(R_OPAL_CRYPT_TWO_END))
#define EN_TCG_OPAL_MEM_END         (R_OPAL_XTS_DUMMY_PTR_BASE + 2)
#define g_xts_dummy_buf_ptr         (*(volatile word   *)(R_OPAL_XTS_DUMMY_PTR_BASE))
#else
#define EN_TCG_OPAL_MEM_END         (QW_ALIGNED(WAI_TEST_MEM_END))
#endif

#define SMART_ATTRI_APACER_MEM_BASE (QW_ALIGNED(EN_TCG_OPAL_MEM_END))
#define g_deep_read_retry           (*(volatile u32 *)(SMART_ATTRI_APACER_MEM_BASE))
#define g_read_retry                (*(volatile u32 *)(SMART_ATTRI_APACER_MEM_BASE + 4))
#define SMART_ATTRI_APACER_MEM_END  (SMART_ATTRI_APACER_MEM_BASE + 8)

#define APACER_ATTRI_MEM_BASE       (QW_ALIGNED(SMART_ATTRI_APACER_MEM_END))
#define r_merge_data_cnt            (*(volatile u64 *)(APACER_ATTRI_MEM_BASE))
#if (EN_FEATURE_APACER_CORE_POWER)
#define r_plp_step                  (*(volatile u8 *)(APACER_ATTRI_MEM_BASE + 8))
#define APACER_ATTRI_MEM_END        (APACER_ATTRI_MEM_BASE + 9)
#else
#define APACER_ATTRI_MEM_END        (APACER_ATTRI_MEM_BASE + 8)
#endif

#if (EN_NAND_REINIT_FEATURE)
#define NAND_REINIT_FLAG_MEM_BASE   (QW_ALIGNED(APACER_ATTRI_MEM_END))
#define r_nand_reinit_flag          (*(volatile u8 *)(NAND_REINIT_FLAG_MEM_BASE))
#define NAND_REINIT_FLAG_MEM_END    (NAND_REINIT_FLAG_MEM_BASE+1)
#else
#define NAND_REINIT_FLAG_MEM_END    (QW_ALIGNED(APACER_ATTRI_MEM_END))
#endif

#if (OPT_CALCULATE_ROSC_FREQ)
#define CALCULATE_ROSC_FREQ_MEM_BASE    (QW_ALIGNED(NAND_REINIT_FLAG_MEM_END))
#define r_actual_rosc_freq              (*(volatile u32 *)(CALCULATE_ROSC_FREQ_MEM_BASE))
#define B1TCM_USED_END                  (CALCULATE_ROSC_FREQ_MEM_BASE + 4)
#else
#define B1TCM_USED_END                  (QW_ALIGNED(NAND_REINIT_FLAG_MEM_END))
#endif

#if (B1TCM_USED_END > B1TCM_DPD_BASE)
#error "B1TCM over"
#endif
#endif

typedef enum _b1tcm_id_e
{
    LFF_CMP_CMD_IDX,
    LFF_WRITE_PROTECT_IDX,
    // MT_ACQ_PQ_IDX,
    // LFF_HR_SQ0_FIFO_IDX,
    // LFF_HR_CQ0_FIFO_IDX,
    COM_CMD_CQ0_FIFO_IDX,
    #if (EN_MT_HMB)
    // LFF_HMB_SQ0_FIFO_IDX,
    // LFF_HMB_CQ0_FIFO_IDX,
    #endif
    #if (EN_MT_4CPU)
    // LFF_HR_SQ1_FIFO_IDX,
    // LFF_HR_CQ1_FIFO_IDX,
    COM_CMD_SQ0_FIFO_IDX,
    COM_CMD_SQ1_FIFO_IDX,
    COM_CMD_CQ1_FIFO_IDX,
    #if (OPT_TCM_DESC)
    FW_CMD_DESC0_IDX,
    #endif
    WC_HASH_TBL_IDX,
    #if (EN_MT_HMB)
    // LFF_HMB_SQ1_FIFO_IDX,
    // LFF_HMB_CQ1_FIFO_IDX,
    #endif
    #endif
    SHARED_VAR_IDX,
    B1TCM_ID_MAX_NUM
} b1tcm_id_e;

extern u32 g_b1tcm_base[];

extern void print_tcm_info(void);
extern int tcm_init(void);
extern int tcm_release(void);
extern u32 tcm_get_addr(sys_mem_type_e tcm_type, b1tcm_id_e tcm_id);
extern void *tcm_get_ptr(sys_mem_type_e tcm_type, b1tcm_id_e tcm_id);
#endif // TCM_H