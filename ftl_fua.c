#include "ftl_fua.h"

ftl_fua_t g_ftl_fua;

u8 ftl_fua_add_entry(u8 func_id, u16 cmp_tag, u32 hc_uid, u64 start_lba, u32 nlb)
{
    return 0;
}

void ftl_fua_cmpl_exec(void)
{

}

void ftl_fua_init(void)
{
    g_ftl_fua.task = 0;
    g_ftl_fua.add = ftl_fua_add_entry;
}