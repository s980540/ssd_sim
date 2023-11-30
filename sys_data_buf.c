#include "sys_data_buf.h"

sys_data_buf_mgr_t g_sys_data_buf_mgr;

bool sys_data_buf_free(u16 buf_ptr, bm_buf_id_e buf_id)
{
    return 0;
}

void sys_data_buf_mgr_init(void)
{
    g_sys_data_buf_mgr.free = sys_data_buf_free;
    return;
}