#include "ftl_bt_mics.h"

MP_DIE_2_CECH m_mp_die_2_cech[MAX_LUN_NUM * MAX_BANK_NUM * MAX_PART_NUM];
u8 m_die_2_lce[MAX_DIE_QNTY];

void ftl_bt_build_die_map(void)
{
    u8 change_lce = false;
    u32 die, die_temp;
    u32 lce_idx;
    u32 get_max_ch_num;
    u32 ch_idx;
    u32 lcech_num[GDP_MAX_CH_NUM];

    for (lce_idx = 0; lce_idx < MAX_LUN_NUM * MAX_BANK_NUM * MAX_PART_NUM; lce_idx++) {
        for (ch_idx = 0; ch_idx < GDP_MAX_CH_NUM; ch_idx++) {
            m_mp_die_2_cech[lce_idx].die[ch_idx] = U8_MASK;
            m_mp_die_2_cech[lce_idx].die_num = 0;
        }
    }

    memset(lcech_num, 0, sizeof(lcech_num));

    lce_idx = 0;
    get_max_ch_num = 1;
    die = 0;
    while (die < VDIE_QNTY) {
        die_temp = die;
        die_temp = VDIE_2_DIE(die_temp);
        lcech_num[DIE_2_CH(die_temp)]++;

        // judge if change lce_idx
        for (ch_idx = 0; ch_idx < GDP_MAX_CH_NUM; ch_idx++) {
            if (lcech_num[ch_idx] > get_max_ch_num) {
                get_max_ch_num++;
                change_lce = true;
                break;
            }
        }

        if (true == change_lce) {
            lce_idx++;
            change_lce = false;
        }

        #if (FTL_BT_MICS_TRACE)
            printf("m_mp_die_2_cech[%x].die[%x] = %2x\n",
                lce_idx, m_mp_die_2_cech[lce_idx].die_num, die);
        #endif
        // add die info
        m_mp_die_2_cech[lce_idx].die[m_mp_die_2_cech[lce_idx].die_num] = die;
        m_mp_die_2_cech[lce_idx].die_num++;
        m_die_2_lce[die] = lce_idx;
        die++;
    }

    #if (FTL_BT_MICS_TRACE)
        printf("\nm_mp_die_2_cech:\n");
        for (lce_idx = 0; lce_idx < MAX_LUN_NUM * MAX_BANK_NUM * MAX_PART_NUM; lce_idx++) {
            for (ch_idx = 0; ch_idx < GDP_MAX_CH_NUM; ch_idx++) {
                if (U8_MASK != m_mp_die_2_cech[lce_idx].die[ch_idx])
                    printf("lce:%x ch:%x die:%2x die_cnt:%x\n",
                        lce_idx, ch_idx,
                        m_mp_die_2_cech[lce_idx].die[ch_idx],
                        m_mp_die_2_cech[lce_idx].die_num);
            }
        }

        printf("\nm_die_2_lce:\n");
        for (die = 0; die < VDIE_QNTY; die++)
            printf("die:%2x lce:%x\n", die, m_die_2_lce[die]);
    #endif
}