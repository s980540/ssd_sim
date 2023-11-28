#include "host_sim.h"

#include "fw_cfg.h"
#include "fw_desc.h"
#include "hw_wdma.h"
#include "nvme_hw_dma.h"

typedef struct _host_sim_manager_t
{
    u64 last_lba;
} host_sim_manager_t;

host_sim_manager_t host_sim_manager;

void host_sim_seq_write_init(void)
{
    // hw_wdma_init();
    host_sim_manager.last_lba = 0;
}

u64 host_sim_seq_write_exec(void)
{
    u64 lba;

    if (hw_wdma_ctrl_cclk_lock()) {
        printf("hw_wdma_ctrl_cclk_lock fail\n");
        goto exit;
    }

    if (0 == reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld) {
        if (hw_xdma_desc_lock()) {
            printf("hw_xdma_desc_lock fail\n");
            goto unlock_reg;
        }

        if (hw_xdma_desc_empty()) {
            printf("hw_xdma_desc_empty\n");
            goto unlock_desc;
        }

        u16 desc_id = hw_xdma_desc_alloc();
        hw_wdma_desc_t *desc = (hw_wdma_desc_t *)FCL_DESC_ID_2_PTR(desc_id);
        desc->glb.current_nlb = SECTOR_NUM;
        desc->nlb.remain_nlb = 0;
        // desc->lba_max.first_4k = 1;
        desc->lba_low.lba_31_0 = host_sim_manager.last_lba & U32_MASK;
        desc->glb.lba_37_32 = host_sim_manager.last_lba >> U32_SHIFT;
        host_sim_manager.last_lba += SECTOR_NUM;

        desc->nlb.fua_cmd = 0;
        desc->nlb.cmp_cmd = 0;

        reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.desc_id = desc_id;
        reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld = 1;
    }

unlock_desc:
    if (hw_xdma_desc_unlock())
        printf("hw_xdma_desc_unlock fail\n");

unlock_reg:
    if (hw_wdma_ctrl_cclk_unlock())
        printf("hw_wdma_ctrl_cclk_unlock fail\n");

exit:
    return host_sim_manager.last_lba;
}