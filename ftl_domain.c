#include "ftl_domain.h"

bool ftl_data_domain_write(
    ftl_domain_t *domain,
    u16 head_desc_id,
    ftl_head_blk_type_e head_blk_type,
    u8 submit_desc_cnt,
    u16 *left_desc_head_id)
{
    ftl_head_blk_t *head_blk = NULL;

    head_blk = domain->head_blk[head_blk_type];

    if (FTL_HEAD_BLK_INVALID(head_blk)) {

    }
}