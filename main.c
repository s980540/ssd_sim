#include "main.h"
#include "thread.h"
#include "fifo.h"
#include "sram.h"
#include "tcm.h"
#include "list.h"

foo_ts foo = {.mutex = (PTHREAD_MUTEX_INITIALIZER), .val = 0};

void main_test(void)
{
    #include "fw_desc.h"
    #include "bdm_desc.h"
    printf("sizeof(com_cmd_sq_entry_t): %d\n", sizeof(com_cmd_sq_entry_t));
    printf("sizeof(com_cmd_sq_format_add_t): %d\n", sizeof(com_cmd_sq_format_add_t));
    printf("sizeof(swlist_t): %d\n", sizeof(swlist_t));
    printf("%x %x\n",
        tcm_get_ptr(SYS_MEM_TYPE_B1TCM, SHARED_VAR_IDX),
        &r_pause_write);

    printf("sizeof(fw_desc_t): %d\n", sizeof(fw_desc_t));
    printf("sizeof(hw_desc_t): %d\n", sizeof(hw_desc_t));
    printf("sizeof(hw_dma_desc_t): %d\n", sizeof(hw_dma_desc_t));

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

    main_test();

    thread_init();
    // fifo_unit_test();
    // tcm_release();
    // thread_delete();
    // fifo_delete();

    return 0;
}