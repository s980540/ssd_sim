#ifndef FTL_FUA_H
#define FTL_FUA_H

#include "global.h"

#define FUA_ENTRY_NUM       (U32_SHIFT)
#define FUA_ENTRY_BITMAP    (U32_MASK)

typedef struct _ftl_fua_entry_t
{

} ftl_fua_entry_t;

typedef struct _ftl_fua_t
{
    ftl_fua_entry_t entry[FUA_ENTRY_NUM];
    u32 free_entry_bitmap;
    u32 done_entry_bitmap;
    u8 task;
    u8 (*add)(u8 func_id, u16 cmd_tag, u32 hc_uid, u64 start_lba, u32 nlb);
} ftl_fua_t;

extern ftl_fua_t g_ftl_fua;

#define FTL_FUA_TODO            (g_ftl_fua.task)
#define FTL_FUA_ENTRY_IS_FULL   (0 == g_ftl_fua.free_entry_bitmap)
#define FTL_FUA_DONE            (g_ftl_fua.done_entry_bitmap)

extern void ftl_fua_init(void);
extern void ftl_fua_cmpl_exec(void);


#endif // ~ FTL_FUA_H