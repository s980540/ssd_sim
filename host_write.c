#include "host_write.h"
#include "write_cache.h"
// #include "fifo.h"
#include "tcm.h"
#include "ftl.h"
#include "sys_reg.h"

#include "nvme_hw_dma.h"
#include "fw_desc.h"
#include "desc.h"

host_write_list_t g_hw_list;

// reg_hw_wdma_ctrl_cclk_t m_reg_hw_wdma_ctrl_cclk;
// volatile reg_hw_wdma_ctrl_cclk_t *reg_hw_wdma_ctrl_cclk;

void host_write_init(void)
{
    // reg_hw_wdma_ctrl_cclk = &m_reg_hw_wdma_ctrl_cclk;
}

void host_write_insert_desc_to_list(void)
{
    hw_dma_desc_t *hw_dma_desc;
    fw_desc_t *fw_desc = NULL;
    u32 hw_dma_desc_ctrl0;

    u16 desc_id;
    u16 lba_cnt;

    hw_dma_desc_ctrl0 = reg_hw_wdma_ctrl_cclk->hw_dma_desc_ctrl0.config;
    while (hw_dma_desc_ctrl0 & 0x1) {
        desc_id = (hw_dma_desc_ctrl0 >> 16);
        fw_desc = FW_FCL_DESC_ID_2_PTR(desc_id);
        hw_dma_desc = (hw_dma_desc_t *)fw_desc;

        lba_cnt = hw_dma_desc->glb.current_nlb;

        if (hw_dma_desc->lba_max.first_4k) {
            // TODO: sysinfo
        }

        if (0 == g_hw_list.total_desc_cnt) {
            g_hw_list.head_desc_id = desc_id;
            g_hw_list.total_desc_cnt = 1;
            g_hw_list.lba
                = hw_dma_desc->lba_low.lba_laa_31_0
                | ((u64)hw_dma_desc->glb.lba_37_32 << U32_SHIFT);

            g_hw_list.tail_desc = fw_desc;
            g_hw_list.tail_id = desc_id;

            g_hw_list.fua = hw_dma_desc->nlb.fua_cmd;
            g_hw_list.cmp = hw_dma_desc->nlb.cmp_cmd;

            if (hw_dma_desc->lba_max.first_4k) {
                if (g_hw_list.fua) {
                    g_hw_list.first_4k = hw_dma_desc->lba_max.first_4k;
                    g_hw_list.fua_entry_id = U8_MASK;
                }
            }

            reg_hw_wdma_ctrl_cclk->hw_dma_desc_ctrl0.bits.vld = 1;

            if (lba_cnt != SECTOR_NUM) {
                g_hw_list.lba
                    = hw_dma_desc->lba_low.lba_31_0
                    | (u64)hw_dma_desc->glb.lba_37_32 << U32_SHIFT;
                g_hw_list.total_lba_cnt = lba_cnt;
                g_hw_list.head_4k_aligned = FALSE;
                fw_desc->next = DESC_ID_NULL;
                return;
            }
            g_hw_list.head_4k_aligned = TRUE;
        } else {
            if (lba_cnt != SECTOR_NUM)
                return;

            if ((hw_dma_desc->nlb.fua_cmd != g_hw_list.fua)
                || (hw_dma_desc->nlb.cmp_cmd != g_hw_list.cmp))
                return;

            reg_hw_wdma_ctrl_cclk->hw_dma_desc_ctrl0.bits.vld = 1;

            g_hw_list.total_desc_cnt++;
            g_hw_list.tail_desc = fw_desc;
            g_hw_list.tail_desc_id = desc_id;
            fw_desc->next = DESC_ID_NULL;

            if (g_hw_list.total_desc_cnt >= HW_DESC_CNT_MAX)
                return;
        }

        if (g_hw_list.cmp)
            return;

        if ((hw_dma_desc->glb.current_nlb == hw_dma_desc->nlb.remain_nlb)
            && (g_hw_list.fua || g_hw_list.cmp))
            return;

        hw_dma_desc_ctrl0 = reg_hw_wdma_ctrl_cclk->hw_dma_desc_ctrl0.config;
    } // ~ while (hw_dma_desc_ctrl0 & 0x1)
}

void host_write_top_exec(void)
{
    if ((g_hw_list.total_desc_cnt == 0)
    && (WC_HOST_WR_COND)
    && (!r_pause_write)
    && (!CMP_CMD->cmp_flag)) {
        // g_system_last_access = g_system_timer;
        host_write_insert_desc_to_list();
        if (g_hw_list.total_desc_cnt) {

        }
    }
}