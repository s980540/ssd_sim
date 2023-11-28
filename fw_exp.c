#include "fw_exp.h"

DPD_PARA_TBL g_dpd_para_tbl;

void init_dpd_para_tbl(void)
{
    u8 die, lun = 0, bank = 0, ch = 0;

    memset(&g_dpd_para_tbl, 0xFF, sizeof(g_dpd_para_tbl));

    for (die = 0; die < DIE_2_CECH_MAX_NUM; die++) {

        #ifdef FW_EXP_TRACE
            printf("%2x:%2x:%x,%x,%x\n", die,
                PCECH_2_CECH(lun, bank, ch),
                lun, bank, ch);
        #endif

        // cech = plun | pce | pch
        g_dpd_para_tbl.die_2_cech[die] = PCECH_2_CECH(lun, bank, ch);

        ch = (ch + 1) % CH_QNTY;
        if (ch == 0)
            bank = (bank + 1) % BANK_QNTY;

        if (ch == 0 && bank == 0)
            lun = (lun + 1) % LUN_QNTY;

        if (ch == 0 && bank == 0 && lun == 0) {
            break;
        }
    }

    #ifdef FW_EXP_TRACE
        die = 0;
        while (1) {
            printf("%2x ", g_dpd_para_tbl.die_2_cech[die]);

            die++;
            if ((die % 8)== 0)
                printf("\n");

            if (die == DIE_2_CECH_MAX_NUM)
                break;
        }

        die = 0;
        while (1) {
            printf("%2x ", g_dpd_para_tbl.die_2_cech[die]);

            die++;
            printf("%2x:%2x:%x,%x,%x\n", die,
                g_dpd_para_tbl.die_2_cech[die],
                CECH_2_PLUN(g_dpd_para_tbl.die_2_cech[die]),
                CECH_2_PBANK(g_dpd_para_tbl.die_2_cech[die]),
                CECH_2_PCH(g_dpd_para_tbl.die_2_cech[die]));

            if (die == DIE_2_CECH_MAX_NUM)
                break;
        }
    #endif
}