#include "ftl_partial_write.h"

ftl_partial_write_t g_ftl_part_wr;

void ftl_partial_write_insert_to_list(fw_desc_t *desc)
{

}

fw_desc_t *ftl_partial_write_hit(u64 lba, u8 sect_cnt, u8 flag, u8 *rlt)
{
    return 0;
}

void ftl_partial_write_delete_from_list(fw_desc_t *desc, u8 flag)
{

}

bool ftl_partial_write_empty(void)
{
    return (0 == g_ftl_part_wr.cnt);
}

void ftl_partial_write_exec(void)
{

}

void ftl_partial_write_init(void)
{
    g_ftl_part_wr.head_id = DESC_ID_NULL;
    g_ftl_part_wr.tail_id = DESC_ID_NULL;
    g_ftl_part_wr.issued_desc_id = DESC_ID_NULL;

    g_ftl_part_wr.mark_sn = 0;
    g_ftl_part_wr.cnt = 0;
    g_ftl_part_wr.seq = 0;

    g_ftl_part_wr.cb_id = 0;

    g_ftl_part_wr.add = ftl_partial_write_insert_to_list;
    g_ftl_part_wr.hit = ftl_partial_write_hit;
    g_ftl_part_wr.del = ftl_partial_write_delete_from_list;
}