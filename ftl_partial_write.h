#ifndef FTL_PARTIAL_WRITE_H
#define FTL_PARTIAL_WRITE_H

#include "global.h"

#include "desc.h"
#include "fw_desc.h"

typedef struct _ftl_partial_write_t
{
    u16 head_id;
    u16 tail_id;
    // 4
    u16 issued_desc_id;
    u16 mark_sn;
    // 8
    u8 cnt;
    u8 cb_id;
    u8 seq;
    u8 rsvd;
    // 12
    u32 tick;
    // 16
    void (*push)(fw_desc_t *desc);
    fw_desc_t *(*hit)(u64 lba, u8 sect_cnt, u8 flag, u8 *rlt);
    void (*del)(fw_desc_t *desc, u8 flag);
} __attribute__((packed)) ftl_partial_write_t;

#define FTL_PARTIAL_WRITE_MAX (1)

extern void ftl_partial_write_init(void);

extern ftl_partial_write_t g_ftl_part_wr;

#endif