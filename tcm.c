#include "tcm.h"
// #include "hw_cfg.h"
#include "print.h"

typedef mem_info_t tcm_info_t;

typedef struct _sys_tcm_info_t
{
    int init;
    u32 base;
    u32 end;
    u32 size;
    u32 align;
    void *ptr;
    // void *tcm_ptr;
    const tcm_info_t *tcm_info;
} sys_tcm_info_t;

#define B0TCM_INFO(base, end, type) \
    {#base, (base), (end), (end) - (base), \
    (((((base) >= B0TCM_BASE) && ((end) <= B0TCM_END)) && ((base) <= (end))) \
        ? type : SYS_MEM_TYPE_UNKNOWN)}

#define B1TCM_INFO(NAME) \
    {#NAME, (NAME##_BASE), (NAME##_END), (NAME##_END) - (NAME##_BASE), \
    ((((NAME##_BASE) >= B1TCM_BASE) && ((NAME##_END) <= B1TCM_END)) && ((NAME##_BASE) <= (NAME##_END))) \
        ? SYS_MEM_TYPE_B1TCM : SYS_MEM_TYPE_UNKNOWN, NAME##_IDX}

#define SYS_TCM_INFO(base, end, align, ptr, tcm_info) \
    {false, (base), (end), (end) - (base), (align), (ptr), (tcm_info)}

static const char *m_sys_mem_type_name[SYS_MEM_TYPE_MAX_NUM] = {
    "B0TCM_CPU0",
    "B0TCM_CPU1",
    "B0TCM_CPU2",
    "B0TCM_CPU3",
    "B1TCM",
    "SRAM",
    "UNKNOWN"
};

static const tcm_info_t m_b1tcm_info[] =
{
    // B0TCM_INFO(CMT_BITMAP_BASE, CMT_BITMAP_END, SYS_MEM_TYPE_B0TCM_CPU2),
    // B0TCM_INFO(CMT_BITMAP_BASE, CMT_BITMAP_END, SYS_MEM_TYPE_B0TCM_CPU3),
    // B1TCM_INFO(LFF_WR_BASE, LFF_WR_END),
    // B1TCM_INFO(LFF_WR_ERR_BASE, LFF_WR_ERR_END),
    // B1TCM_INFO(LFF_RD_BASE, LFF_RD_END),
    // #if (EN_PRED_RD_31)
    // B1TCM_INFO(LFF_PR_DESC_BASE, LFF_PR_DESC_END),
    // #if (EN_PRED_RD_31_4CPU)
    // B1TCM_INFO(LFF_PR_CMD_BASE, LFF_PR_CMD_END),
    // B1TCM_INFO(LFF_PR_CB_BASE, LFF_PR_CB_END),
    // #if (OPT_FCL_FW_RCQ)
    // B1TCM_INFO(LFF_PR_CB1_BASE, LFF_PR_CB1_END),
    // #endif
    // #endif
    // #endif
    // B1TCM_INFO(LFF_CB_BASE, LFF_CB_END),
    // B1TCM_INFO(LFF_UNC_BASE, LFF_UNC_END),
    // B1TCM_INFO(LFF_CMD_BASE, LFF_CMD_END),
    // B1TCM_INFO(LFF_FRONT_BUF_ADDR, LFF_FRONT_BUF_END),
    // B1TCM_INFO(LFF_FTL_CMD_BASE, LFF_FTL_CMD_END),
    // B1TCM_INFO(LFF_PER_CLOCK_BASE, LFF_PER_CLOCK_END),
    // B1TCM_INFO(LFF_SYS_VLD_BIT_MAP_BASE, LFF_SYS_VLD_BIT_MAP_END),
    // #if (EN_FE_HOST_RW_FTL_IF)
    // B1TCM_INFO(LFF_FTL_FE_RESULT, LFF_FTL_FE_RESULT_END),
    // #endif
    // B1TCM_INFO(LFF_FRONT_RESULT_BASE, LFF_FRONT_RESULT_END),
    // #if (EN_DEVICE_SELF_TEST)
    // B1TCM_INFO(LFF_DST_RESULT_BASE, LFF_DST_RESULT_END),
    // #endif
    // B1TCM_INFO(LFF_BE_MEM_BASE, LFF_BE_MEM_END),
    // B1TCM_INFO(LFF_BE_FLUSH_BASE, LFF_BE_FLUSH_END),
    // B1TCM_INFO(LFF_SYS_BLK_FULL_BASE, LFF_SYS_BLK_FULL_END),
    // B1TCM_INFO(LFF_UEFI_BOOT_DONE_BASE, LFF_UEFI_BOOT_DONE_END),
    B1TCM_INFO(CMP_CMD),
    // B1TCM_INFO(LFF_GC_FLAG_BASE, LFF_GC_FLAG_END),
    // #if (EN_SAVE_LOG)
    // B1TCM_INFO(LFF_SYS_TIMER_BASE, LFF_SYS_TIMER_END),
    // #endif
    // B1TCM_INFO(LFF_FORCE_FLUSH_TBL_BASE, LFF_FORCE_FLUSH_TBL_END),
    // B1TCM_INFO(LFF_DL_RESET_BASE, LFF_DL_RESET_END),
    // B1TCM_INFO(LFF_DL_RESULT_BASE, LFF_DL_RESULT_END),
    // B1TCM_INFO(LFF_DL_CHECK_SUM_BASE, LFF_DL_CHECK_SUM_END),

    // B1TCM_INFO(LFF_ERR_ENTRY_IDX_BASE, LFF_ERR_ENTRY_IDX_END),
    // B1TCM_INFO(LFF_ERR_BUF_IDX_BASE, LFF_ERR_BUF_IDX_END),

    // B1TCM_INFO(LFF_SYS_WRITE_STS_BASE, LFF_SYS_WRITE_STS_END),
    // B1TCM_INFO(LFF_PAUSE_MERGE_BASE, LFF_PAUSE_MERGE_END),
    // B1TCM_INFO(LFF_WRITE_PROTECT_BASE, LFF_WRITE_PROTECT_END),
    // B1TCM_INFO(LFF_IDLE_REQUEST_BASE, LFF_IDLE_REQUEST_END),

    // B1TCM_INFO(LFF_IDLE_TASK_BASE, LFF_IDLE_TASK_END),

    // B1TCM_INFO(LFF_AES_BASE, LFF_AES_END),
    // B1TCM_INFO(LFF_PS_BASE, LFF_PS_END),

    // B1TCM_INFO(LFF_NEED_IDLE_PRO_CHECK_BASE, LFF_NEED_IDLE_PRO_CHECK_END),
    // B1TCM_INFO(LFF_INVALID_QBT_BASE, LFF_INVALID_QBT_END),
    // B1TCM_INFO(LFF_TLC_START_BASE, LFF_TLC_START_END),
    // B1TCM_INFO(LFF_THERMAL_RANGE_BASE, LFF_THERMAL_RANGE_END),
    // B1TCM_INFO(LFF_PS_DIE_REQ_BASE, LFF_PS_DIE_REQ_END),
    // B1TCM_INFO(LFF_CPU1_ASSERT_FLAG_BASE, LFF_CPU1_ASSERT_FLAG_END),
    // B1TCM_INFO(LFF_FLUSH_QBT_FLAG_BASE, LFF_FLUSH_QBT_FLAG_END),
    // B1TCM_INFO(LFF_FORMAT_SIZE_BASE, LFF_FORMAT_SIZE_END),
    // B1TCM_INFO(LFF_ASSERT_FLAG_BASE, LFF_ASSERT_FLAG_END),
    // #if (EN_SHUTTLE_CHIP) && (OPT_L1_WORKAROUND)
    // B1TCM_INFO(LFF_EXIT_L1_FLAG_BASE, LFF_EXIT_L1_FLAG_END),
    // #endif

    #if (EN_TBL_31)
    // B1TCM_INFO(MT_ACQ_PQ_BASE, MT_ACQ_PQ_END),
    // B1TCM_INFO(LFF_HR_SQ0_FIFO_BASE, LFF_HR_SQ0_FIFO_END),
    // B1TCM_INFO(LFF_HR_CQ0_FIFO_BASE, LFF_HR_CQ0_FIFO_END),
    B1TCM_INFO(COM_CMD_CQ0_FIFO),
    #if (EN_MT_HMB)
    // B1TCM_INFO(LFF_HMB_SQ0_FIFO_BASE, LFF_HMB_SQ0_FIFO_END),
    // B1TCM_INFO(LFF_HMB_CQ0_FIFO_BASE, LFF_HMB_CQ0_FIFO_END),
    #endif
    #if (EN_MT_4CPU)
    // B1TCM_INFO(LFF_HR_SQ1_FIFO_BASE, LFF_HR_SQ1_FIFO_END),
    // B1TCM_INFO(LFF_HR_CQ1_FIFO_BASE, LFF_HR_CQ1_FIFO_END),
    B1TCM_INFO(COM_CMD_SQ0_FIFO),
    B1TCM_INFO(COM_CMD_SQ1_FIFO),
    B1TCM_INFO(COM_CMD_CQ1_FIFO),
    #if (EN_MT_HMB)
    // B1TCM_INFO(LFF_HMB_SQ1_FIFO_BASE, LFF_HMB_SQ1_FIFO_END),
    // B1TCM_INFO(LFF_HMB_CQ1_FIFO_BASE, LFF_HMB_CQ1_FIFO_END),
    #endif
    // B1TCM_INFO(T2F_FIFO_SQ0_MEM_BASE, T2F_FIFO_SQ0_MEM_END),
    // B1TCM_INFO(T2F_FIFO_SQ1_MEM_BASE, T2F_FIFO_SQ1_MEM_END),
    // B1TCM_INFO(T2F_FIFO_CQ0_MEM_BASE, T2F_FIFO_CQ0_MEM_END),
    // B1TCM_INFO(T2F_FIFO_CQ1_MEM_BASE, T2F_FIFO_CQ1_MEM_END),
    #endif
    #endif // EN_TBL_31

    // #if (EN_AES)
    // B1TCM_INFO(LFF_AES_KEY_FLAG_BASE, LFF_AES_KEY_FLAG_END),
    // #endif
    // #if (EN_FCL_30)
    // #if (OPT_FCL_FW_RCQ)
    // B1TCM_INFO(FCL_FIFO_FW_RCQ_MEM_BASE, FCL_FIFO_FW_RCQ_MEM_END),
    // #endif
    // #endif
    // #if (LUN_SHIFT == 0)
    // B1TCM_INFO(MT_TBL_BID_BASE, MT_TBL_BID_END),
    // #endif
    // #if (OPT_CMT_BYPASS)
    // B1TCM_INFO(MT_BYPASS_BASE, MT_BYPASS_END),
    // #else
    // B1TCM_INFO(MT_SERVICE_SN_1_BASE, MT_SERVICE_SN_1_END),
    // #endif
    // #if (EN_MT_4CPU)
    // B1TCM_INFO(PROG_V2P_ARRAY_BASE, PROG_V2P_ARRAY_END),
    // B1TCM_INFO(PROG_P2V_ARRAY_BASE, PROG_P2V_ARRAY_END),
    // B1TCM_INFO(SHARE_CMT_BASE, SHARE_CMT_END),
    // B1TCM_INFO(SHARE_CMUT_BASE, SHARE_CMUT_END),
    // B1TCM_INFO(GLOBAL_TF_BASE, GLOBAL_TF_END),
    // B1TCM_INFO(SYNC_CMD_SQ_BASE, SYNC_CMD_SQ_END),
    // B1TCM_INFO(SYNC_CMD_CQ_BASE, SYNC_CMD_CQ_END),
    // B1TCM_INFO(DEBUG_CMD_SQ_BASE, DEBUG_CMD_SQ_END),
    // B1TCM_INFO(DEBUG_CMD_CQ_BASE, DEBUG_CMD_CQ_END),
    // B1TCM_INFO(DEBUG_MODE_FLAG, DEBUG_MODE_FLAG_END),
    // #endif
    #if (OPT_TCM_DESC)
    B1TCM_INFO(FW_CMD_DESC0),
    #endif
    // #if (OPT_BUF_USE_DBG2) || (OPT_DESC_USE_DBG2)
    // B1TCM_INFO(DRAM_BUF_HW_BITMAP_BASE, DRAM_BUF_HW_BITMAP_END),
    // B1TCM_INFO(DRAM_BUF_FW_BITMAP_BASE, DRAM_BUF_FW_BITMAP_END),
    // B1TCM_INFO(SRAM_BUF_HW_BITMAP_BASE, SRAM_BUF_HW_BITMAP_END),
    // B1TCM_INFO(SRAM_BUF_FW_BITMAP_BASE, SRAM_BUF_FW_BITMAP_END),
    // B1TCM_INFO(DESC_HW_BITMAP_BASE, DESC_HW_BITMAP_END),
    // B1TCM_INFO(DESC_FW_BITMAP_BASE, DESC_FW_BITMAP_END),
    // B1TCM_INFO(DESC1_HW_BITMAP_BASE, DESC1_HW_BITMAP_END),
    // B1TCM_INFO(DESC1_FW_BITMAP_BASE, DESC1_FW_BITMAP_END),
    // #endif

    // B1TCM_INFO(HW_TRIM_VC_BASE2, HW_TRIM_VC_END2),
    // B1TCM_INFO(WC_HASH_TBL_BASE, WC_HASH_TBL_END),
    // B1TCM_INFO(CMT_MEM_BASE, CMT_MEM_END),
    // B1TCM_INFO(CMT_HASH_MEM_BASE, CMT_HASH_MEM_END),
    // B1TCM_INFO(CMUT_MEM_BASE, CMUT_MEM_END),
    // #if (EN_AES)
    // B1TCM_INFO(AES_KEY_BASE, AES_KEY_END),
    // #endif
    // B1TCM_INFO(FTL_FLUSH_LAA_TEMP_BASE, FTL_FLUSH_LAA_TEMP_END),
    // B1TCM_INFO(FTL_MERGE_LAA_TEMP_BASE, FTL_MERGE_LAA_TEMP_END),

    B1TCM_INFO(SHARED_VAR),
    // #if (EN_NVME_HMB)
    // B1TCM_INFO(HMB_ACC_CNT_MEM_BASE, HMB_ACC_CNT_MEM_END),
    // B1TCM_INFO(HMB_DESC_BADD_MEM_BASE, HMB_DESC_BADD_MEM_END),
    // B1TCM_INFO(HMB_DESC_SIZE_MEM_BASE, HMB_DESC_SIZE_MEM_END),
    // #endif
    // #if (EN_SAVE_LOG)
    // B1TCM_INFO(LOG_BASE, LOG_END),
    // #endif
    // #if (EN_TCG_OPAL)
    // B1TCM_INFO(R_OPAL_VAL_ONE_BASE, EN_TCG_OPAL_MEM_END),
    // #elif (EN_AES)
    // B1TCM_INFO(R_OPAL_CRYPT_TWO_BASE, EN_TCG_OPAL_MEM_END),
    // #endif
    // B1TCM_INFO(SMART_ATTRI_APACER_MEM_BASE, SMART_ATTRI_APACER_MEM_END),
    // B1TCM_INFO(APACER_ATTRI_MEM_BASE, APACER_ATTRI_MEM_END),
    // #if (EN_NAND_REINIT_FEATURE)
    // B1TCM_INFO(NAND_REINIT_FLAG_MEM_BASE, NAND_REINIT_FLAG_MEM_END),
    // #endif
    // #if (OPT_CALCULATE_ROSC_FREQ)
    // B1TCM_INFO(CALCULATE_ROSC_FREQ_MEM_BASE, B1TCM_USED_END),
    // #endif

    // B1TCM_INFO(B1TCM_BASE, B1TCM_END),
    {NULL}
};

sys_tcm_info_t m_sys_tcm_info[SYS_MEM_TYPE_MAX_NUM - 2] =
{
    SYS_TCM_INFO(B0TCM_BASE, B0TCM_END, B0TCM_SIZE, NULL, NULL),
    SYS_TCM_INFO(B0TCM_BASE, B0TCM_END, B0TCM_SIZE, NULL, NULL),
    SYS_TCM_INFO(B0TCM_BASE, B0TCM_END, B0TCM_SIZE, NULL, NULL),
    SYS_TCM_INFO(B0TCM_BASE, B0TCM_END, B0TCM_SIZE, NULL, NULL),
    SYS_TCM_INFO(B1TCM_BASE, B1TCM_DPD_BASE, B1TCM_SIZE, NULL, m_b1tcm_info)
};

void print_tcm_info(void)
{
    const tcm_info_t *_tcm_info = NULL;
    int total_size[SYS_MEM_TYPE_MAX_NUM - 2], addr_end[SYS_MEM_TYPE_MAX_NUM - 2];

    memset(total_size, 0, sizeof(total_size));
    for (int i = 0; i < SYS_MEM_TYPE_SRAM; i++)
        addr_end[i] = m_sys_tcm_info[i].base;

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

    for (_tcm_info = m_b1tcm_info; _tcm_info->name != NULL; _tcm_info++) {
        int overlap = false;
        for (int i = 0; m_b1tcm_info[i].name != _tcm_info->name; i++) {
            if ((_tcm_info->type == m_b1tcm_info[i].type)
             && ((_tcm_info->base < m_b1tcm_info[i].end)
                || (_tcm_info->end < m_b1tcm_info[i].end))) {
                overlap = true;
                break;
            }
        }
        print_str(_tcm_info->name, 20, 0, 0);
        print_str(":", 1, 0, 2);  // 23

        print_str((_tcm_info->type != SYS_MEM_TYPE_UNKNOWN ? m_sys_mem_type_name[_tcm_info->type] : "UNKNOWN"),
            10, 0, 2);  // 12

        printf(" %8XH", _tcm_info->base);
        print_str("-", 1, 1, 1);  // 13

        printf(" %8XH", _tcm_info->end);
        print_str(":", 1, 0, 1);  // 12

        if (_tcm_info->type != SYS_MEM_TYPE_UNKNOWN)
            printf("  %8X (%4d %2s)  ", _tcm_info->size,
                (_tcm_info->size >= KB ? _tcm_info->size / KB : _tcm_info->size),
                (_tcm_info->size >= KB ? "KB" : "B"));    // 22
        else {
            print_ch('*', 20);
            print_ch(' ', 2);
        }

        if (overlap)
            printf("**overlap**  "); // 13

        printf("\n");
        if (!overlap) {
            total_size[_tcm_info->type] += _tcm_info->size;
            addr_end[_tcm_info->type] = _tcm_info->end;
        }
    }
    print_ch('-', 95);
    printf("\n");

    for (int i = 0; i < SYS_MEM_TYPE_SRAM; i++) {
        u32 base = m_sys_tcm_info[i].base;
        u32 size = m_sys_tcm_info[i].size;

        printf("[%-10s] Totol Size: %8x (",
            m_sys_mem_type_name[i], total_size[i]);

        printf("%4d %2s / ",
            (total_size[i] >= KB ? total_size[i] / KB : total_size[i]),
            (total_size[i] >= KB ? "KB" : "B"));

        if (addr_end[i] >= base) {
            printf("%4d %2s / ",
                (((addr_end[i] - base) >= KB) ? ((addr_end[i] - base) / KB) : (addr_end[i] - base)),
                (((addr_end[i] - base) >= KB) ? "KB" : "B"));

            printf("%4d %2s / ",
                (((base + size - addr_end[i]) >= KB) ? ((base + size - addr_end[i]) / KB) : (base + size - addr_end[i])),
                (((base + size - addr_end[i]) >= KB) ? "KB" : "B"));
        } else {
            printf("%4d  B / %4d  B / ", 0, 0);
        }
        printf("%4d KB)\n", size / KB);
    }
}

int tcm_init(void)
{
    const tcm_info_t *_tcm_info = NULL;
    u32 addr_end[SYS_MEM_TYPE_MAX_NUM - 2];
    bool overlap;

    for (int i = 0; i < SYS_MEM_TYPE_SRAM; i++)
        addr_end[i] = m_sys_tcm_info[i].base;

    for (_tcm_info = m_b1tcm_info; _tcm_info->name != NULL; _tcm_info++) {
        overlap = false;
        for (int i = 0; m_b1tcm_info[i].name != _tcm_info->name; i++) {
            if ((_tcm_info->type == m_b1tcm_info[i].type)
            && ((_tcm_info->base < m_b1tcm_info[i].end)
             || (_tcm_info->end  < m_b1tcm_info[i].end))) {
                overlap = true;
            }
        }

        if (!overlap)
            addr_end[_tcm_info->type] = _tcm_info->end;
    }

    for (int i = 0; i < SYS_MEM_TYPE_SRAM; i++) {
        sys_tcm_info_t *_sys_tcm_info = &m_sys_tcm_info[i];
        // _sys_tcm_info->base = _sys_tcm_info->base;
        // _sys_tcm_info->end = addr_end[i];
        _sys_tcm_info->size = _sys_tcm_info->size -
            ((_sys_tcm_info->end > addr_end[i]) ? _sys_tcm_info->end - addr_end[i] : 0);
        // printf("%d %x %x %x\n", i, _sys_tcm_info->size, _sys_tcm_info->end, addr_end[i]);

        void * aligned_ptr = aligned_alloc(_sys_tcm_info->size, _sys_tcm_info->align);
        if (aligned_ptr == NULL) {
            tcm_release();
            return -1;
        }
        _sys_tcm_info->ptr = aligned_ptr;
        _sys_tcm_info->init = true;
    }

    // debug
    for (int id = 0; id < B1TCM_ID_MAX_NUM; id++) {
        const tcm_info_t *tcm_info = &m_sys_tcm_info[SYS_MEM_TYPE_B1TCM].tcm_info[id];
        printf("id: %d, name: %s, base: %8x, size: %4x, tcm: %8x\n",
            tcm_info->id, tcm_info->name, tcm_info->base, tcm_info->size,
            tcm_get_addr(SYS_MEM_TYPE_B1TCM, id));
    }

    return 0;
}

int tcm_release(void)
{
    for (int i = 0; i < SYS_MEM_TYPE_SRAM; i++) {
        sys_tcm_info_t *_sys_tcm_info = &m_sys_tcm_info[i];
        if (_sys_tcm_info->ptr != NULL) {
            void *aligned_ptr = _sys_tcm_info->ptr;
            free((void *)(*((size_t *)aligned_ptr - 1)));
            _sys_tcm_info->ptr = NULL;
            _sys_tcm_info->init = false;
        }
    }

    return 0;
}

u32 tcm_get_addr(sys_mem_type_e tcm_type, b1tcm_id_e tcm_id)
{
    if (tcm_type < SYS_MEM_TYPE_B0TCM_CPU0 || tcm_type > SYS_MEM_TYPE_B1TCM)
        return U32_MASK;

    sys_tcm_info_t *_sys_tcm_info = &m_sys_tcm_info[tcm_type];
    return _sys_tcm_info->tcm_info[tcm_id].base - _sys_tcm_info->base + (u32)_sys_tcm_info->ptr;
}

void *tcm_get_ptr(sys_mem_type_e tcm_type, b1tcm_id_e tcm_id)
{
    if (tcm_type < SYS_MEM_TYPE_B0TCM_CPU0 || tcm_type > SYS_MEM_TYPE_B1TCM)
        return NULL;

    sys_tcm_info_t *_sys_tcm_info = &m_sys_tcm_info[tcm_type];
    return (void *)(_sys_tcm_info->tcm_info[tcm_id].base - _sys_tcm_info->base + (u32)_sys_tcm_info->ptr);
}