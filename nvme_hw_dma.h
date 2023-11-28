#ifndef NVME_HW_DMA_H
#define NVME_HW_DMA_H

#include "global.h"

typedef struct _hw_wdma_dw0_t
{
    u32 next_id : 16;
    u32 rsvd : 16;
} hw_wdma_dw0_t;

typedef struct _hw_wdma_ctl_t
{
    u32 dspec : 16; // directive specific
    u32 rsvd : 16;
} hw_wdma_ctl_t;

typedef struct _hw_wdma_nlb_t
{
    u32 remain_nlb : 16;    // remaining number of logic block
    u32 desc_vld : 1;       // descriptor valid bit
    u32 rls_buf : 1;        // release buffer (only in reset flow)
    u32 limit_retry : 1;    // limit retry
    u32 func_id : 2;        // function id
    u32 fua_cmd : 1;        // FUA command
    u32 cmp_cmd : 1;        // CMP command
    u32 vec_cmd : 1;        // vector chunk command
    u32 dsm : 8;            // data set management
} hw_wdma_nlb_t;

typedef struct _hw_wdma_lba_max_t
{
    u32 desc_id : 4;        //
    u32 first_4k : 1;       // first 4k
    u32 last_4k : 1;        // last 4k
    u32 wdat_typ : 1;       // write data type
    u32 buf_id : 4;         // buffer id
    u32 dtype : 4;          // directive type
    u32 total_nlb : 16;     // define as total AU logic block number
} hw_wdma_lba_max_t;

typedef struct _hw_wdma_buf_t
{
    u32 buf_ptr : 16;       // buffer pointer
    u32 bitmap : 16;        // Each bit represents for one sector in the AU.
} hw_wdma_buf_t;

typedef struct _hw_wdma_uid_t
{
    u32 cmd_id;             //
    u32 q_id;               //
} hw_wdma_uid_t;

typedef struct _hw_wdma_sts_t
{
    u32 sts : 8;            // FTL desc status summary
    u32 rsvd : 24;          // reserved
} hw_wdma_sts_t;

typedef struct _hw_wdma_glb_t
{
    u32 lba_37_32 : 8;  // LBA bit[37:32] or LAA bit[37:32].
    u32 current_nlb : 8;    // In NVME gen2, it should define ad current AU logic block number.
    u32 tag : 16;           // 16-bits write command tag (id) for fw update FUA/CMP command in TMEM.
} hw_wdma_glb_t;

typedef struct _hw_wdma_lba_low_t
{
    u32 lba_31_0 : 32;  // LBA bit[31:0]
} hw_wdma_lba_low_t;

typedef struct _hw_wdma_nsp_t
{
    u32 namespace_id : 32;  // namespace
} hw_wdma_nsp_t;

typedef struct _hw_wdma_desc_t
{
    hw_wdma_dw0_t dw0;
    hw_wdma_ctl_t ctl;
    hw_wdma_nlb_t nlb;
    hw_wdma_lba_max_t lba_max;

    hw_wdma_buf_t buf;
    hw_wdma_uid_t uid;
    hw_wdma_sts_t sts;
    hw_wdma_glb_t glb;
    hw_wdma_lba_low_t lba_low;
    hw_wdma_nsp_t nsp;
    u32 dw10;
} hw_wdma_desc_t;

#endif // ~ NVME_HW_DMA_H