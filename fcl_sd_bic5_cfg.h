#ifndef FCL_SD_BIC5_CFG_H
#define FCL_SD_BIC5_CFG_H

#define ROW_PAGE_SHIFT                  (9)
#define ROW_PLANE_SHIFT                 (1)
#define ROW_BLOCK_SHIFT                 (12)
#define ROW_LUN_SHIFT                   (1)
#define PAGE_SHIFT                      (11)
#define PLANE_SHIFT                     (1)
#define BLOCK_SHIFT                     (12)
#define STPU_SHIFT                      (6)
#define HS_PAGE_SHIFT                   (9)
#define HS_PAGE_QNTY                    (448)
#define PAGE_QNTY                       (1344)
#define BLOCK_QNTY                      (3222)
#define USED_BLOCK_QNTY                 (3122)
#define PAGE_SIZE                       (18336)

#define PAGE_NUM                        (1 << PAGE_SHIFT)
#define ROW_PAGE_NUM                    (1 << ROW_PAGE_SHIFT)
#define PLANE_NUM                       (1 << PLANE_SHIFT)
#define BLOCK_NUM                       (1 << BLOCK_SHIFT)
#define PLANE_QNTY                      (PLANE_NUM)

#define ROW_ADDR_WIDTH                  (3)
#define ADDR_ACRR_TYPE                  (DISABLE)

#define LDPC_CODE_RATE3                 // par_len = 240, min pagesize = 18432
#define NON_TARGET_TYPE                 (DISABLE)

#if defined(ALL_BLK_SLC)
#define MAX_PAGES_PER_PROG_SEQ          (1)
#define MAX_RAID_BUF_ID                 (3)
#else
#define MAX_PAGES_PER_PROG_SEQ          (3)
#endif

#define META_DATA_SIZE                  (4)
#define CAPACITY_PER_DIE                (128)
/*
* 1 MCRC + 3 META for each CW + 512 sector size + ldpc user size(129X32=4128)
* @ ldpc code rate 1 - par_len = 512, min pagesize = 18496
* @ ldpc code rate 2 - par_len = 480, min pagesize = 18368
* @ ldpc code rate 3 - par_len = 448, min pagesize = 18240
* @ ldpc code rate 4 - par_len = 608, min pagesize = 18880
*/

/************nand cmd cycle type setting**************/
#define UCODE_CYC_TYP00             (0x8079)
#define UCODE_CYC_TYP01             (0x4079)
#define UCODE_CYC_TYP02             (0x79)
#define UCODE_CYC_TYP03             (0xC079)
#define UCODE_CYC_TYP04             (0x3F0)
#define UCODE_CYC_TYP05             (0x21D8)
#define UCODE_CYC_TYP06             (0x1278)
#define UCODE_CYC_TYP07             (0x3018)
#define UCODE_CYC_TYP08             (0x86F8)
#define UCODE_CYC_TYP09             (0xAC38)
#define UCODE_CYC_TYP10             (0x3FC)

#define UCODE_CYC_TYP11             (0x3FA)
#define UCODE_CYC_TYP12             (0x278)
#define UCODE_CYC_TYP13             (0x2018)
#define UCODE_CYC_TYP14             (0x3F8)
#define UCODE_CYC_TYP15             (0x1278)
#define UCODE_CYC_TYP16             (0x27C)
#define UCODE_CYC_TYP17             (0x398)
#define UCODE_CYC_TYP18             (0x3018)
#define UCODE_CYC_TYP19             (0x19)
#define UCODE_CYC_TYP20             (0x3F4)

#define UCODE_CYC_TYP21             (0x3F2)
#define UCODE_CYC_TYP22             (0x96F8)
#define UCODE_CYC_TYP23             (0xBC38)
#define UCODE_CYC_TYP24             (0x218)
#define UCODE_CYC_TYP25             (0x274)
#define UCODE_CYC_TYP26             ((ROW_ADDR_WIDTH == 3)?0x3f8:0x3f2)
#define UCODE_CYC_TYP27             ((ADDR_ACRR_TYPE == 1)?0x7f2:0x3f8)

#define UCODE_CYC_TYP28             (0x92F8)
#define UCODE_CYC_TYP29             (0xB838)
#define UCODE_CYC_TYP30             (0xD2F8)
#define UCODE_CYC_TYP31             (0xF838)

#define UCODE_CYC_TYP_WR_IDLE       (0x1278)
#define UCODE_CYC_TYP_RD_IDLE       (0x3018)
#define UCODE_STAND_BY              (0x8079)

#endif // FCL_SD_BIC5_CFG_H