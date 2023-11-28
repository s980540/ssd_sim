#ifndef HW_CFG_H
#define HW_CFG_H

// tcm
#define D_CACHE3_MEM_BASE           (0x03E00000)
#define I_CACHE3_MEM_BASE           (0x03C00000)
#define B1TCM3_MEM_BASE             (PRIVATE_B1TCM_BASE)
#define B0TCM3_MEM_BASE             (0x03A00000)
#define ATCM3_MEM_BASE              (0x03800000)

#define D_CACHE2_MEM_BASE           (0x03600000)
#define I_CACHE2_MEM_BASE           (0x03400000)
#define B1TCM2_MEM_BASE             (PRIVATE_B1TCM_BASE)
#define B0TCM2_MEM_BASE             (0x03200000)
#define ATCM2_MEM_BASE              (0x03000000)

#define D_CACHE1_MEM_BASE           (0x02E00000)
#define I_CACHE1_MEM_BASE           (0x02C00000)
#define B1TCM1_MEM_BASE             (PRIVATE_B1TCM_BASE)
#define B0TCM1_MEM_BASE             (0x02A00000)
#define ATCM1_MEM_BASE              (0x02800000)

#define D_CACHE0_MEM_BASE           (0x02600000)
#define I_CACHE0_MEM_BASE           (0x02400000)
#define B1TCM0_MEM_BASE             (PRIVATE_B1TCM_BASE)
#define B0TCM0_MEM_BASE             (0x02200000)
#define ATCM0_BASE                  (0x02000000)

#define PRIVATE_D_CACHE_MEM_BASE    (0x01800000)
#define PRIVATE_I_CACHE_MEM_BASE    (0x01000000)
#define PRIVATE_B1TCM_BASE          (0x00840000)
#define PRIVATE_B0TCM_BASE          (0x00800000)
#define PRIVATE_ATCM_BASE           (0x00000000)

#define ATCM_SIZE                   (0x10000)       // 64 KB
#define B0TCM_SIZE                  (0x10000)       // 64 KB
#define B1TCM_SIZE                  (0x40000)       // 256 KB
#define ICACHE_MEM_SIZE             (0x1000)        // 4 KB
#define DCACHE_MEM_SIZE             (0x2000)        // 8 KB

#define PRAM_MEM_SIZE               (0x40000)       // 256K
#define SRAM_SIZE                   (0x180000)      // 1.5M

#define SPLL_DEFAULT_CLOCK          (525000000)
#define MPLL_DEFAULT_CLOCK          (700000000)
#define CPLL_DEFAULT_CLOCK          (500000000)
#define ROSC_10M_CLOCK              (10000000)
#define ROSC_20M_CLOCK              (20000000)
#define XTAL_DEFAULT_CLOCK          (25000000)

// sram
#define ROMC_MEM_BASE               (0xFFFE0000)
#define FWACC_REG_BASE              (0x06900000)
#define HC_REG_BASE                 (0x06700000)
#define FC_REG_BASE                 (0x06600000)
#define BDM_REG_BASE                (0x06500000)
#define ACC_REG_BASE                (0x06300000)
#define CPUS_DBG_REG_BASE           (0x06200000)
#define LRF_REG_BASE                (0x06100000)
#define APB_REG_BASE                (0x06000000)
#define FC_RAID_MEM_BASE            (0x05300000)
#define SRIC_MEM_BASE               (0x04000000)
#define CPUS_MEM_BASE               (0x00000000)

// dram
#define DRAM_MEM_BASE               (0x40000000)

#define SRAM_BASE                   (SRIC_MEM_BASE)
#define SRAM_END                    (SRAM_BASE + SRAM_SIZE)

#endif // HW_CFG_H