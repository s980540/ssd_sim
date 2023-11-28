#include "hw_wdma.h"
#include "reg_hw_wdma_ctrl_cclk.h"
#include "mutex.h"
#include "tcm.h"

static reg_hw_wdma_ctrl_cclk_t *reg_hw_wdma_ctrl_cclk = NULL;

int hw_wdma_init(void)
{
    int sts;

    reg_hw_wdma_ctrl_cclk = malloc(sizeof(reg_hw_wdma_ctrl_cclk_t));
    if (NULL == reg_hw_wdma_ctrl_cclk)
        return -1;

    memset(reg_hw_wdma_ctrl_cclk, 0, sizeof(reg_hw_wdma_ctrl_cclk_t));

    sts = pthread_mutex_init(&reg_hw_wdma_ctrl_cclk->mutex, NULL);
    if (sts) {
        printf("pthread_mutex_init error: %d\n", sts);
        return sts;
    }

    return 0;
}

int hw_wdma_deinit(void)
{
    if (NULL != reg_hw_wdma_ctrl_cclk)
        free(reg_hw_wdma_ctrl_cclk);

    return 0;
}

typedef struct _hw_xdma_desc_manager_t
{
    u32 head_desc_id;
    u32 tail_desc_id;
    u16 head_desc;
} __attribute__((packed)) hw_xdma_desc_manager_t;

static hw_xdma_desc_manager_t m_xdma_desc_manager;

int hw_xdma_desc_init(void)
{
    m_xdma_desc_manager.head_desc_id = 0;
    m_xdma_desc_manager.tail_desc_id =;
    m_xdma_desc_manager.head_desc =
}

int hw_wdma_desc_rdy(void)
{
    int vld;
    if (mutex_lock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;

    vld = reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld;

    if (mutex_unlock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;

    return vld;
}

int hw_wdma_desc_set(void)
{
    if (mutex_lock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;

    reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld = 1;

    if (mutex_unlock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;
}

int hw_wdma_desc_clr(void)
{
    if (mutex_lock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;

    reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld = 0;

    if (mutex_unlock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;
}

int hw_wdma_desc_ctrl0(void)
{
    u32 config;

    if (mutex_lock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;

    config = reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.config;

    if (mutex_unlock(&reg_hw_wdma_ctrl_cclk->mutex))
        return 0;

    return config;
}