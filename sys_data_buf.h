#ifndef SYS_DATA_BUF_H
#define SYS_DATA_BUF_H

#include "global.h"

typedef enum _bm_buf_id_e
{
    BM_BUF_ID_HOST_WR = 0,
    BM_BUF_ID_HOST_RD = 1,
    BM_BUF_ID_FW,
    BM_BUF_ID_PART_WR,
    BM_BUF_ID_DUMMY,
    BM_BUF_ID_FRONT,
    BM_BUF_ID_OPAL,
    BM_BUF_ID_MAX
} bm_buf_id_e;

typedef struct _sys_data_buf_mgr_t
{
    bool (*free)(u16 buf_ptr, bm_buf_id_e buf_id);

} sys_data_buf_mgr_t;

extern sys_data_buf_mgr_t g_sys_data_buf_mgr;

extern void sys_data_buf_mgr_init(void);

#endif // ~ SYS_DATA_BUF_H