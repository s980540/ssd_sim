#include "hw_wdma.h"
#include "mutex.h"
#include "tcm.h"

#include "desc.h"
#include "fw_desc.h"
#include "bdm_desc.h"
#include "nvme_hw_dma.h"

typedef struct _hw_xdma_desc_manager_t
{
    u16 head_id;
    u16 tail_id;
    u16 desc_cnt;
    pthread_mutex_t mutex;
} __attribute__((packed)) hw_xdma_desc_manager_t;

static hw_xdma_desc_manager_t m_xdma_desc_manager;

int hw_xdma_desc_init(void)
{
    // TODO: FW_CMD_DESC_NUM, FCL_DESC_ID_2_PTR
    if (mutex_init(&m_xdma_desc_manager.mutex))
        return -1;

    m_xdma_desc_manager.head_id = 0;
    m_xdma_desc_manager.tail_id = FW_CMD_DESC_NUM - 1;
    m_xdma_desc_manager.desc_cnt = FW_CMD_DESC_NUM;

    for (u16 desc_id = 0; desc_id < FW_CMD_DESC_NUM; desc_id++) {
        hw_wdma_desc_t *desc = (hw_wdma_desc_t *)FCL_DESC_ID_2_PTR(desc_id);
        memset(desc, 0, sizeof(hw_wdma_desc_t));
        if (desc_id < (FW_CMD_DESC_NUM - 1))
            desc->dw0.next_id = desc_id + 1;
        else
            desc->dw0.next_id = DESC_ID_NULL;
    }

    return 0;
}

int hw_xdma_desc_deinit(void)
{
    m_xdma_desc_manager.head_id = DESC_ID_NULL;
    m_xdma_desc_manager.tail_id = DESC_ID_NULL;
    m_xdma_desc_manager.desc_cnt = 0;
}

int hw_xdma_desc_lock(void)
{
    if (mutex_lock(&m_xdma_desc_manager.mutex))
        return -1;

    return 0;
}

int hw_xdma_desc_unlock(void)
{
    if (mutex_unlock(&m_xdma_desc_manager.mutex))
        return -1;

    return 0;
}

bool hw_xdma_desc_full(void)
{
    return (FW_CMD_DESC_NUM == m_xdma_desc_manager.desc_cnt);
}

bool hw_xdma_desc_empty(void)
{
    return (0 == m_xdma_desc_manager.desc_cnt);
}

u16 hw_xdma_desc_alloc(void)
{
    u16 desc_id;

    if (DESC_ID_NULL == m_xdma_desc_manager.head_id) {
        if (m_xdma_desc_manager.desc_cnt != 0)
            printf("xdma desc err: %x\n", m_xdma_desc_manager.desc_cnt);
        return DESC_ID_NULL;
    }

    desc_id = m_xdma_desc_manager.head_id;
    m_xdma_desc_manager.head_id = FCL_DESC_ID_2_PTR(desc_id)->next_id;
    FCL_DESC_ID_2_PTR(desc_id)->next_id = DESC_ID_NULL;
    m_xdma_desc_manager.desc_cnt--;

    if (DESC_ID_NULL == m_xdma_desc_manager.head_id) {
        if (m_xdma_desc_manager.desc_cnt != 0) {
            printf("xdma desc err: %x\n", m_xdma_desc_manager.desc_cnt);
            return DESC_ID_NULL;
        }
        m_xdma_desc_manager.tail_id = DESC_ID_NULL;
    }

    if ((0 == m_xdma_desc_manager.desc_cnt)
        && ((DESC_ID_NULL != m_xdma_desc_manager.head_id)
            || (DESC_ID_NULL != m_xdma_desc_manager.tail_id))) {
        printf("xdma desc err: %x,%x\n",
            m_xdma_desc_manager.head_id, m_xdma_desc_manager.tail_id);
        return DESC_ID_NULL;
    }

    return desc_id;
}

int hw_xdma_desc_free(u16 desc_id)
{
    if (DESC_ID_NULL == desc_id) {
        printf("DESC_ID_NULL == desc_id\n");
        return -1;
    }

    if (m_xdma_desc_manager.desc_cnt >= FW_CMD_DESC_NUM) {
        printf("m_xdma_desc_manager.desc_cnt >= FW_CMD_DESC_NUM: %x,%x\n",
            m_xdma_desc_manager.desc_cnt, desc_id);
        return -1;
    }

    if ((DESC_ID_NULL == m_xdma_desc_manager.head_id)
        || (DESC_ID_NULL == m_xdma_desc_manager.tail_id)) {

        if (m_xdma_desc_manager.head_id != m_xdma_desc_manager.tail_id) {
            printf("m_xdma_desc_manager.head_id != m_xdma_desc_manager.tail_id: %x,%x\n",
                m_xdma_desc_manager.head_id, m_xdma_desc_manager.tail_id);
            return -1;
        }

        if (0 != m_xdma_desc_manager.desc_cnt) {
            printf("0 != m_xdma_desc_manager.desc_cnt: %x\n",
                m_xdma_desc_manager.desc_cnt);
            return -1;
        }

        if (DESC_ID_NULL != FCL_DESC_ID_2_PTR(desc_id)->next_id) {
            printf("DESC_ID_NULL != (desc_id)->next_id: %x\n",
                FCL_DESC_ID_2_PTR(desc_id)->next_id);
            return -1;
        }

        m_xdma_desc_manager.head_id = desc_id;
        m_xdma_desc_manager.tail_id = desc_id;
        m_xdma_desc_manager.desc_cnt = 1;
    } else {
        if (DESC_ID_NULL != FCL_DESC_ID_2_PTR(m_xdma_desc_manager.tail_id)->next_id) {
            printf("DESC_ID_NULL != (m_xdma_desc_manager.tail_id)->next_id: %x\n",
                FCL_DESC_ID_2_PTR(m_xdma_desc_manager.tail_id)->next_id);
            return -1;
        }

        if (DESC_ID_NULL != FCL_DESC_ID_2_PTR(desc_id)->next_id) {
            printf("DESC_ID_NULL != (desc_id)->next_id: %x\n",
                FCL_DESC_ID_2_PTR(desc_id)->next_id);
            return -1;
        }

        FCL_DESC_ID_2_PTR(m_xdma_desc_manager.tail_id)->next_id = desc_id;
        m_xdma_desc_manager.desc_cnt++;
    }

    return 0;
}

int hw_xdma_desc_check(void)
{
    u16 desc_id, next_id, cnt, cnt2;

    if (DESC_ID_NULL == m_xdma_desc_manager.head_id) {
        if (DESC_ID_NULL != m_xdma_desc_manager.tail_id) {
            printf("DESC_ID_NULL != m_xdma_desc_manager.tail_id: %x\n",
                m_xdma_desc_manager.tail_id);
            return -1;
        }
        return 0;
    }

    desc_id = m_xdma_desc_manager.head_id;
    cnt = 1;
    while (1) {
        if (desc_id == DESC_ID_NULL) {
            printf("DESC_ID_NULL == desc_id\n");
            return -1;
        }

        if (desc_id >= FW_CMD_DESC_NUM) {
            printf("desc_id >= FW_CMD_DESC_NUM: %x\n", desc_id);
            return -1;
        }

        next_id = FCL_DESC_ID_2_PTR(desc_id)->next_id;
        cnt2 = 1;
        while (1) {
            if (next_id != DESC_ID_NULL) {
                if (next_id == desc_id) {
                    printf("next_id == desc_id: %x\n", desc_id);
                    return -1;
                }

                if (next_id >= FW_CMD_DESC_NUM) {
                    printf("next_id >= FW_CMD_DESC_NUM: %x\n", next_id);
                    return -1;
                }
            } else
                break;

            next_id = FCL_DESC_ID_2_PTR(next_id)->next_id;
            cnt2++;
            if (cnt2 > FW_CMD_DESC_NUM) {
                printf("cnt2 > FW_CMD_DESC_NUM\n");
                return -1;
            }
        }

        if (desc_id == m_xdma_desc_manager.tail_id) {
            if (DESC_ID_NULL != FCL_DESC_ID_2_PTR(desc_id)->next_id) {
                printf("DESC_ID_NULL != FCL_DESC_ID_2_PTR(desc_id)->next_id: %x\n",
                    FCL_DESC_ID_2_PTR(desc_id)->next_id);
                return -1;
            }
            break;
        }

        desc_id = FCL_DESC_ID_2_PTR(desc_id)->next_id;
        cnt++;

        if (cnt > FW_CMD_DESC_NUM) {
            printf("cnt > FW_CMD_DESC_NUM\n");
            return -1;
        }
    }

    printf("cnt != m_xdma_desc_manager.desc_cnt: %x,%x\n",
        cnt, m_xdma_desc_manager.desc_cnt);
    if (cnt != m_xdma_desc_manager.desc_cnt) {
        return -1;
    }

    return 0;
}

#if (OPT_SW_SIM)
reg_hw_wdma_ctrl_cclk_t *reg_hw_wdma_ctrl_cclk = NULL;
#endif

int hw_wdma_init(void)
{
#if (OPT_SW_SIM)
    if (mutex_init(&reg_hw_wdma_ctrl_cclk->mutex))
        return -1;

    reg_hw_wdma_ctrl_cclk = malloc(sizeof(reg_hw_wdma_ctrl_cclk_t));
    if (NULL == reg_hw_wdma_ctrl_cclk)
        return -1;

    memset(reg_hw_wdma_ctrl_cclk, 0, sizeof(reg_hw_wdma_ctrl_cclk_t));
#endif

    return 0;
}

int hw_wdma_deinit(void)
{
#if (OPT_SW_SIM)
    if (NULL != reg_hw_wdma_ctrl_cclk)
        free(reg_hw_wdma_ctrl_cclk);
#endif

    return 0;
}

int hw_wdma_ctrl_cclk_lock(void)
{
    if (mutex_lock(&reg_hw_wdma_ctrl_cclk->mutex))
        return -1;

    return 0;
}

int hw_wdma_ctrl_cclk_unlock(void)
{
    if (mutex_unlock(&reg_hw_wdma_ctrl_cclk->mutex))
        return -1;

    return 0;
}

bool hw_wdma_desc_rdy(void)
{
    return reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld;
}

void hw_wdma_desc_set(void)
{
    reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld = 1;
}

void hw_wdma_desc_clr(void)
{
    reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.bits.vld = 0;
}

u32 hw_wdma_desc_ctrl0(void)
{
    return reg_hw_wdma_ctrl_cclk->hw_wdma_desc_ctrl0.config;
}