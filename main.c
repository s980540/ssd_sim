#include "main.h"
#include "thread.h"
#include "fifo.h"
#include "sram.h"
#include "tcm.h"
#include "list.h"

#include "hw_wdma.h"
#include "host_sim.h"

foo_ts foo = {.mutex = (PTHREAD_MUTEX_INITIALIZER), .val = 0};

void main_test(void)
{
    #include "fw_desc.h"
    #include "bdm_desc.h"
    #include "nvme_hw_dma.h"

    printf("sizeof(com_cmd_sq_entry_t): %d\n", sizeof(com_cmd_sq_entry_t));
    printf("sizeof(com_cmd_sq_format_add_t): %d\n", sizeof(com_cmd_sq_format_add_t));
    printf("sizeof(swlist_t): %d\n", sizeof(swlist_t));
    printf("%x %x\n",
        tcm_get_ptr(SYS_MEM_TYPE_B1TCM, SHARED_VAR_IDX),
        &r_pause_write);

    if (sizeof(fw_desc_t) != 48) {
        printf("sizeof(fw_desc_t): %d\n", sizeof(fw_desc_t));
        while (1);
    }

    if (sizeof(hw_desc_t) != 48) {
        printf("sizeof(hw_desc_t): %d\n", sizeof(hw_desc_t));
        while (1);
    }

    if (sizeof(hw_wdma_desc_t) != 48) {
        printf("sizeof(hw_wdma_desc_t): %d\n", sizeof(hw_wdma_desc_t));
        while (1);
    }

    if (sizeof(hw_desc_config0_t) != 4) {
        printf("sizeof(hw_desc_config0_t): %d\n", sizeof(hw_desc_config0_t));
        while (1);
    }

    if (sizeof(hw_desc_config1_t) != 4) {
        printf("sizeof(hw_desc_config1_t): %d\n", sizeof(hw_desc_config1_t));
        while (1);
    }

    if (sizeof(hw_desc_config_t) != 8) {
        printf("sizeof(hw_desc_config_t): %d\n", sizeof(hw_desc_config_t));
        while (1);
    }

    if (sizeof(hw_desc_status_t) != 4) {
        printf("sizeof(hw_desc_sts_t): %d\n", sizeof(hw_desc_status_t));
        while (1);
    }

    if (sizeof(fw_desc_status_t) != 4) {
        printf("sizeof(hw_desc_sts_t): %d\n", sizeof(fw_desc_status_t));
        while (1);
    }

    if (sizeof(fw_desc_unc_laa_t) != 4) {
        printf("sizeof(fw_desc_unc_laa_t): %d\n", sizeof(fw_desc_unc_laa_t));
        while (1);
    }

    system("pause");
}

int main(int argc, char *argv[])
{
    printf("ssd_sim\n");
    srand(time(NULL));
    print_sram_info();
    print_tcm_info();
    tcm_init();
    fifo_init();
    if (hw_xdma_desc_init())
        printf("hw_xdma_desc_init fail\n");
    if (hw_xdma_desc_check())
        printf("hw_xdma_desc_check fail\n");
    else
        printf("hw_xdma_desc_check pass\n");
    if (hw_wdma_init())
        printf("hw_wdma_init fail\n");
    system("pause");

    main_test();

    thread_init();

    // fifo_unit_test();

    thread_delete();
    hw_wdma_deinit();
    hw_xdma_desc_deinit();
    // fifo_deinit();
    tcm_release();


    return 0;
}