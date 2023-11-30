#include "shared_mem.h"
#include "tcm.h"
#include "ftl.h"

void shared_mem_init(void)
{
    r_pause_write = 0;
    r_ftl_cmp_cmd->cmp = 0;
    r_write_protect = 0;
}
