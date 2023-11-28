#include "sram.h"
#include "print.h"

typedef mem_info_t sram_info_t;

#define SRAM_INFO(base, end) \
    {#base, (base), (end), (end) - (base), \
    (((((base) >= SRAM_BASE) && ((end) <= SRAM_END)) && ((base) <= (end))) \
        ? SYS_MEM_TYPE_SRAM : SYS_MEM_TYPE_UNKNOWN)}

static const char *m_sys_mem_type_name[SYS_MEM_TYPE_MAX_NUM] = {
    "B0TCM_CPU0",
    "B0TCM_CPU1",
    "B0TCM_CPU2",
    "B0TCM_CPU3",
    "B1TCM",
    "SRAM",
    "UNKNOWN"
};

static const sram_info_t m_sram_info[] = {
    SRAM_INFO(FC_FWSQ0_BASE, FC_FW_SUBMISSON_Q_END),
    #if (EN_FWSQ1)
    SRAM_INFO(FC_FWSQ1_BASE, FC_FW_SUBMISSON_Q1_END),
    #endif
    SRAM_INFO(FC_HPSQ_BASE, FC_FW_HP_SUBMISSON_Q_END),
    SRAM_INFO(FC_FWSS_BASE, FC_FW_SINGLE_SUBMISSON_Q_END),
    SRAM_INFO(FC_FWCQ0_BASE, FC_FW_CMPL_Q0_END),
    #if (EN_FWSQ1)
    SRAM_INFO(FC_FWCQ1_BASE, FC_FW_CMPL_Q1_END),
    #endif
    SRAM_INFO(FC_HPCQ_BASE, FC_HP_CMPL_Q_END),
    //
    SRAM_INFO(DATA_BUF_SRAM_LL_BASE, DATA_BUF_SRAM_LL_END),
    #if (!EN_PROJECT_DRAM_LESS)
    SRAM_INFO(DATA_BUF_DRAM_LL_BASE, DATA_BUF_DRAM_LL_END),
    #endif
    SRAM_INFO(DATA_BUF_SRAM_BASE, DATA_BUF_SRAM_END),
    //
    SRAM_INFO(SWAP_CMD_BUF_BASE, SWAP_CMD_BUF_END),
    //
    SRAM_INFO(UCODE_BUF_BASE_ADDR, UCODE_BUF_END),
    SRAM_INFO(LDR_MEMORY_BASE_ADDR, LDR_MEMORY_END),
    //
    #if (EN_UEFI_FEATURE)
    SRAM_INFO(UEFI_CODE_BASE, UEFI_CODE_END),
    #endif
    //
    #if (EN_FCL_UNIT_TEST_30)
    SRAM_INFO(UT_CMD_SRAM_BASE, UT_CMD_SRAM_END),
    SRAM_INFO(UT_CASE_SRAM_BASE, UT_CASE_SRAM_END),
    SRAM_INFO(UT_BLOCK_SRAM_BASE, UT_BLOCK_SRAM_END),
    SRAM_INFO(UT_INFO_SRAM_BASE, UT_INFO_SRAM_END),
    #endif
    //
    #if (OPT_TRIM_FULL_DISK_CHK)
    SRAM_INFO(FTL_BLK_VC_BASE, FTL_BLK_VC_END),
    #endif
    //
    #if (EN_TBL_31)
    SRAM_INFO(MERGE_USED_BUF_BASE, MERGC_USED_BUF_END),
    #endif
    SRAM_INFO(DATA_CRC_BUF_BASE, DATA_CRC_BUF_END),
    SRAM_INFO(UNC_DEL_BUF_BASE, UNC_DEL_BUF_END),
    //
    SRAM_INFO(RCT_MEM_BASE0, RCT_MEM_END0),
    SRAM_INFO(RCT_MEM_BASE1, RCT_MEM_END1),
    SRAM_INFO(RCT_MEM_BASE, RCT_MEM_END),
    // SRAM_INFO(RCT_TBL_DUMMY_BASE, RCT_TBL_DUMMY_END),
    // SRAM_INFO(GC_RCT_MEM_BASE, GC_RCT_MEM_END),
    //
    #if (OPT_HMB_RCT)
    SRAM_INFO(RCT_HMB_BUFF_BASE, RCT_HMB_BUFF_END),
    #endif
    //
    SRAM_INFO(PBT_VC_BASE, PBT_VC_END),
    //
    SRAM_INFO(PBT_CHK_VC_BASE, PBT_CHK_VC_END),
    //
    #if (OPT_CMT_VFC_USE_HWBV)
    SRAM_INFO(PBT_CMT_BUILD_VC_BASE, PBT_CMT_BUILD_VC_END),
    #endif
    //
    #if (OPT_HMB_RCT) && (OPT_HMB_AES)
    SRAM_INFO(HMB_AES_RD_BASE, HMB_AES_RD_END),
    #endif
    //
    #if (EN_WAI_TEST)
    SRAM_INFO(WAI_TEST_MEM_BASE, WAI_TEST_MEM_SIZE),
    #endif
    //
    SRAM_INFO(BAD_MAP_BASE, BAD_MAP_END),

    SRAM_INFO(SN_BASE, SN_END),
    //
    SRAM_INFO(TRIM_TBL_BASE, TRIM_TBL_END),
    //
    SRAM_INFO(FCL_FIFO_ADMIN_SQ_MEM_BASE, FCL_FIFO_ADMIN_SQ_MEM_END),
    //
    SRAM_INFO(FCL_FIFO_ADMIN_CQ_MEM_BASE, FCL_FIFO_ADMIN_CQ_MEM_END),
    //
    SRAM_INFO(FCL_FIFO_IO_SQ_MEM_BASE, FCL_FIFO_IO_SQ_MEM_END),
    //
    SRAM_INFO(FCL_FIFO_IO_CQ_MEM_BASE, FCL_FIFO_IO_CQ_MEM_END),
    //
    #if (LUN_SHIFT != 0)
    SRAM_INFO(MT_TBL_BID_BASE, MT_TBL_BID_END),
    #endif

    //
    #if (LUN_SHIFT != 0)
    SRAM_INFO(FW_BUF_SRAM_BASE, FW_BUF_SRAM_END),
    #endif

    //
    #if (OPT_SEPERATE_VFC)
    SRAM_INFO(VFC_BASE, VFC_END),
    #endif
    //
    #if (OPT_USE_UART_INT)
    SRAM_INFO(UART_BUF_START, UART_BUF_END),
    #endif
    //
    SRAM_INFO(TBMT_MEM_BASE, TBMT_MEM_END),
    //
    SRAM_INFO(BLK_UNIT_BASE, BLK_UNIT_END),
    //
    #if (EN_MT_2CPU)
    SRAM_INFO(CHT_MEM_BASE, CMT_BITMAP_END),
    #endif
    //
    SRAM_INFO(SRAM_USED_CACHE_BASE, BLK_UNIT_END),
    //
    #if (OPT_TBMT_DRAM)
    SRAM_INFO(DIRTY_DBMT_VCNT_MEM_BASE, DIRTY_DBMT_VCNT_MEM_END),
    SRAM_INFO(TBMT_DIRTY_BITMAP_MEM_BASE, TBMT_DIRTY_BITMAP_MEM_END),
    SRAM_INFO(TBMT_RDY_BITMAP_MEM_BASE, TBMT_RDY_BITMAP_MEM_END),
    SRAM_INFO(TBMT_SUBMIT_BITMAP_MEM_BASE, TBMT_SUBMIT_BITMAP_MEM_END),
    #endif
    // //
    // SRAM_INFO(DATA_BUF_DRAM_BASE, DATA_BUF_DRAM_END),
    //
    #if (OPT_DRAM_L2P_CHK)
    SRAM_INFO(FTL_DATA_L2P_BASE2, FTL_DATA_L2P_END2),
    #endif
    // //
    // SRAM_INFO(PBT_TMP_BASE, PBT_TMP_END),
    //
    #if (EN_FTL_UNIT_TEST)
    SRAM_INFO(FAKE_HW_NCQ_REG_BASE, FAKE_HW_NCQ_REG_END),
    #endif

    {NULL}
};

void print_sram_info(void)
{
    const sram_info_t *_sram_info = NULL;
    int total_size = 0, addr_end = SRAM_BASE;
    printf("\n");
    print_str("Name",   4, 0, 19);
    print_str("Type",   4, 0, 8);
    print_str("Base",   4, 0, 9);
    print_str("End",    3, 0, 9);
    print_str("Size",   5, 0, 17);
    print_str("Status", 7, 0, 6);
    printf("\n");
    print_ch('-',       95);
    printf("\n");

    for (_sram_info = m_sram_info; _sram_info->name != NULL; _sram_info++) {
        int overlap = false;
        for (int i = 0; m_sram_info[i].name != _sram_info->name; i++) {
            if ((_sram_info->type == m_sram_info[i].type)
             && ((_sram_info->base < m_sram_info[i].end)
                || (_sram_info->end < m_sram_info[i].end))) {
                overlap = true;
                break;
            }
        }
        print_str(_sram_info->name, 20, 0, 0);
        print_str(":", 1, 0, 2);  // 23

        print_str((_sram_info->type != SYS_MEM_TYPE_UNKNOWN ? m_sys_mem_type_name[_sram_info->type] : "UNKNOWN"),
            10, 0, 2);  // 12

        printf(" %8XH", _sram_info->base);
        print_str("-", 1, 1, 1);  // 13

        printf(" %8XH", _sram_info->end);
        print_str(":", 1, 0, 1);  // 12

        if (_sram_info->type != SYS_MEM_TYPE_UNKNOWN)
            printf("  %8X (%4d %2s)  ", _sram_info->size,
                (_sram_info->size >= KB ? _sram_info->size / KB : _sram_info->size),
                (_sram_info->size >= KB ? "KB" : "B"));    // 22
        else {
            print_ch('*', 20);
            print_ch(' ', 2);
        }

        if (overlap)
            printf("**overlap**  "); // 13

        printf("\n");
        if (!overlap) {
            total_size += _sram_info->size;
            addr_end = _sram_info->end;
        }
    }
    print_ch('-', 95);
    printf("\n");

    // printf("[%-10s] Totol Size: %8x (%4d KB / %4d KB / %4d KB / %4d KB)\n",
    //     "SRAM",
    //     total_size,
    //     total_size / KB,
    //     (addr_end > SRAM_BASE ? addr_end - SRAM_BASE : 0) / KB,
    //     (SRAM_BASE + SRAM_SIZE - addr_end) / KB,
    //     SRAM_SIZE / KB);

    printf("[%-10s] Totol Size: %8x (",
        "SRAM", total_size);

    printf("%4d %2s / ",
        (total_size >= KB ? total_size / KB : total_size),
        (total_size >= KB ? "KB" : "B"));

    if (addr_end >= SRAM_BASE) {
        printf("%4d %2s / ",
            (((addr_end - SRAM_BASE) >= KB) ? ((addr_end - SRAM_BASE) / KB) : (addr_end - SRAM_BASE)),
            (((addr_end - SRAM_BASE) >= KB) ? "KB" : "B"));

        printf("%4d %2s / ",
            (((SRAM_BASE + SRAM_SIZE - addr_end) >= KB) ? ((SRAM_BASE + SRAM_SIZE - addr_end) / KB) : (SRAM_BASE + SRAM_SIZE - addr_end)),
            (((SRAM_BASE + SRAM_SIZE - addr_end) >= KB) ? "KB" : "B"));
    } else {
        printf("%4d  B / %4d  B / ", 0, 0);
    }
    printf("%4d KB)\n", SRAM_SIZE / KB);
}