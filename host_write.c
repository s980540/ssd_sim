#include "host_write.h"

// #include "fifo.h"
#include "tcm.h"

#include "sys_reg.h"
#include "nvme_hw_dma.h"
#include "fw_desc.h"
#include "desc.h"

#include "hw_wdma.h"

#include "ftl.h"
#include "ftl_write_cache.h"
#include "ftl_partial_write.h"
#include "ftl_fua.h"

host_write_list_t g_hw_list;

void host_write_init(void)
{
    g_hw_list.lba = U64_MASK;

    g_hw_list.head_id = DESC_ID_NULL;
    g_hw_list.total_desc_cnt = 0;
    g_hw_list.total_lba_cnt = 0;

    g_hw_list.tail_id = DESC_ID_NULL;
    // g_hw_list.tmp_desc_num = 0;
    // g_hw_list.tmp_head_lba_cnt = 0;

    g_hw_list.head_4k_aligned = FALSE;
    g_hw_list.tail_4k_aligned = FALSE;
    g_hw_list.seq = FALSE;

    g_hw_list.cmp = FALSE;
    g_hw_list.fua = FALSE;

    g_hw_list.first_4k = FALSE;
    g_hw_list.fua_entry_id = U8_MASK;
}

void hwt_insert_desc_to_list(void)
{
    hw_wdma_desc_t *hw_wdma_desc;
    fw_desc_t *fw_desc = NULL;
    u32 wdma_desc_ctrl0;

    u16 desc_id;
    u16 lba_cnt;

    hw_wdma_ctrl_cclk_lock();
    wdma_desc_ctrl0 = hw_wdma_desc_ctrl0();
    hw_wdma_ctrl_cclk_unlock();

    while (wdma_desc_ctrl0 & 0x1) {
        desc_id = (wdma_desc_ctrl0 >> 16);
        fw_desc = FW_DESC_ID_2_PTR(desc_id);
        hw_wdma_desc = (hw_wdma_desc_t *)fw_desc;

        lba_cnt = hw_wdma_desc->glb.current_nlb;

        if (hw_wdma_desc->lba_max.first_4k) {
            // TODO: sysinfo
        }

        if (0 == g_hw_list.total_desc_cnt) {
            g_hw_list.head_id = desc_id;
            g_hw_list.total_desc_cnt = 1;
            g_hw_list.lba
                = hw_wdma_desc->lba_low.lba_31_0
                | ((u64)hw_wdma_desc->glb.lba_37_32 << U32_SHIFT);
            printf("[%s] lba:%llx\n", __FUNCTION__, g_hw_list.lba);

            g_hw_list.tail_desc = fw_desc;
            g_hw_list.tail_id = desc_id;

            g_hw_list.fua = hw_wdma_desc->nlb.fua_cmd;
            g_hw_list.cmp = hw_wdma_desc->nlb.cmp_cmd;

            if (hw_wdma_desc->lba_max.first_4k) {
                if (g_hw_list.fua) {
                    g_hw_list.first_4k = hw_wdma_desc->lba_max.first_4k;
                    g_hw_list.fua_entry_id = U8_MASK;
                }
            }

            hw_wdma_ctrl_cclk_lock();
            hw_wdma_desc_clr();
            hw_wdma_ctrl_cclk_unlock();

            if (lba_cnt != SECTOR_NUM) {
                g_hw_list.lba
                    = hw_wdma_desc->lba_low.lba_31_0
                    | (u64)hw_wdma_desc->glb.lba_37_32 << U32_SHIFT;
                g_hw_list.total_lba_cnt = lba_cnt;
                g_hw_list.head_4k_aligned = FALSE;
                fw_desc->next_id = DESC_ID_NULL;
                return;
            }
            g_hw_list.head_4k_aligned = TRUE;
        } else {
            if (lba_cnt != SECTOR_NUM)
                return;

            if ((hw_wdma_desc->nlb.fua_cmd != g_hw_list.fua)
                || (hw_wdma_desc->nlb.cmp_cmd != g_hw_list.cmp))
                return;

            hw_wdma_ctrl_cclk_lock();
            hw_wdma_desc_clr();
            hw_wdma_ctrl_cclk_unlock();

            g_hw_list.total_desc_cnt++;
            g_hw_list.tail_desc = fw_desc;
            g_hw_list.tail_id = desc_id;
            fw_desc->next_id = DESC_ID_NULL;

            if (g_hw_list.total_desc_cnt >= HW_DESC_CNT_MAX)
                return;
        }

        if (g_hw_list.cmp)
            return;

        if ((hw_wdma_desc->glb.current_nlb == hw_wdma_desc->nlb.remain_nlb)
            && (g_hw_list.fua || g_hw_list.cmp))
            return;

        hw_wdma_ctrl_cclk_lock();
        wdma_desc_ctrl0 = hw_wdma_desc_ctrl0();
        hw_wdma_ctrl_cclk_unlock();

    } // ~ while (wdma_desc_ctrl0 & 0x1)
}

void hwt_cmp_cmd(void)
{

}

void hwt_write_protect(void)
{

}

bool hwt_set_desc_config(void)
{
    fw_desc_t *desc = NULL;
    hw_wdma_desc_t *wdma_desc = NULL;

    u64 lba_str;
    u32 lba_cnt;

    u16 head_id, temp_id, desc_id, prev_id;
    bool fua = g_hw_list.fua;
    bool sts = FALSE;

    head_id = g_hw_list.head_id;
    prev_id = DESC_ID_NULL;
    desc = FW_DESC_ID_2_PTR(head_id);

    FTL_DESC_FOR_EACH_SAFE(desc_id, temp_id, head_id, next_id) {
        desc = FW_DESC_ID_2_PTR(desc_id);
        wdma_desc = (hw_wdma_desc_t *)desc;

        if (wdma_desc->nlb.rls_buf || wdma_desc->sts.sts) {

        }
    }
}

void hwt_push_non_4k_aligned(void)
{

}

void hwt_push_4k_aligned(void)
{

}

bool hwt_flush_cmd(void)
{

}

void hwt_handle_list(void)
{
    hw_wdma_desc_t *hw_wdma_desc = NULL;
    fw_desc_t *part_desc = NULL;

    if (g_hw_list.cmp) {
        hwt_cmp_cmd();
        return;
    }

    if (r_write_protect) {
        hwt_write_protect();
        return;
    }

    if (g_ftl_part_wr.cnt) {
        part_desc = FW_DESC_ID_2_PTR(g_ftl_part_wr.tail_id);
        if ((g_hw_list.head_4k_aligned)
            || (part_desc->laa != (g_hw_list.lba >> SECTOR_SHIFT))
            || (g_ftl_part_wr.cnt > FTL_PARTIAL_WRITE_MAX)
            || (g_hw_list.fua)) {
            g_ftl_part_wr.seq = 0;
        }
    }

    if (g_hw_list.fua) {
        if (g_hw_list.first_4k && (U8_MASK == g_hw_list.fua_entry_id)) {
            if (FTL_FUA_ENTRY_IS_FULL)
                return;
            else {
                hw_wdma_desc = (hw_wdma_desc_t *)FW_DESC_ID_2_PTR(g_hw_list.head_id);
                g_hw_list.fua_entry_id = g_ftl_fua.add(
                                            hw_wdma_desc->nlb.func_id,
                                            hw_wdma_desc->glb.tag,
                                            hw_wdma_desc->uid.cmd_id,
                                            hw_wdma_desc->lba_low.lba_31_0
                                                | ((u64)hw_wdma_desc->glb.lba_37_32 << U32_SHIFT),
                                            hw_wdma_desc->lba_max.total_nlb);

                if (U8_MASK == g_hw_list.fua_entry_id) {
                    printf("fua err\n");
                    while (1);
                }
            }
        }
    }

    if (hwt_set_desc_config())
        return;

    if (0 == g_hw_list.head_4k_aligned)
        hwt_push_non_4k_aligned();
    else
        hwt_push_4k_aligned();
}

void host_write_top_exec(void)
{
    if ((g_hw_list.total_desc_cnt == 0)
    && (ftl_write_cache_host_write_cond())
    && (!r_pause_write)
    && (!r_ftl_cmp_cmd->cmp)) {
        // g_system_last_access = g_system_timer;
        hwt_insert_desc_to_list();
        // if (g_hw_list.total_desc_cnt) {

        // }
    }

    if (g_hw_list.total_desc_cnt)
        hwt_handle_list();

    if (hwt_flush_cmd())
        return;

    return;
}