#ifndef HOST_WRITE_H
#define HOST_WRITE_H

#include "global.h"
#include "fw_desc.h"
#include "fw_cfg.h"

typedef struct _host_write_list_t
{
    u64 lba;
    fw_desc_t *tail_desc;

    u16 head_desc_id;
    u16 total_desc_cnt;
    u16 total_lba_cnt;

    u16 tail_desc_id;
    u16 tmp_desc_num;
    u16 tmp_head_lba_cnt;

    bool head_4k_aligned;
    bool tail_4k_aligned;
    bool sequential;

    bool fua;
    bool cmp;   // compare

    u8 fua_entry_id;
    bool first_4k;

} __attribute__((packed)) host_write_list_t;

#define HW_DESC_CNT_MAX (FRAG_QNTY)

void host_write_top_exec(void);

#endif // HOST_WRITE_H