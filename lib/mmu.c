/*
 * mmu.c
 *
 *  Created on: 2017Äê9ÔÂ2ÈÕ
 *      Author: lucius
 */
#include <stdint.h>
#include "mmu.h"
#include "uart.h"

typedef union {
  struct {
    volatile uint32_t mmu_enable : 1;
    volatile uint32_t alignment_check_enable : 1;
    volatile uint32_t cache_enable : 1;
    volatile uint32_t reserved0 : 2;
    volatile uint32_t cp15_barrier_enable : 1;
    volatile uint32_t reserved1 : 1;
    volatile uint32_t it_enable : 1;
    volatile uint32_t sed_enable : 1;
    volatile uint32_t reserved2: 2;
    volatile uint32_t reserved3: 1;
    volatile uint32_t instruct_cache_enable : 1;
    volatile uint32_t vectors_bit : 1;
    volatile uint32_t reserved4 : 2;
    volatile uint32_t not_trap_wfi : 1;
    volatile uint32_t reserved5 : 1;
    volatile uint32_t not_trap_wfe : 1;
    volatile uint32_t wxn : 1;
    volatile uint32_t uwxn : 1;
    volatile uint32_t reserved6: 4;
    volatile uint32_t exception_endianness : 1;
    volatile uint32_t reserved7: 2;
    volatile uint32_t tre : 1;
    volatile uint32_t afe : 1;
    volatile uint32_t fe: 1;
    volatile uint32_t reserved8 : 1;
  } bit_field;
  volatile uint32_t reg_value;
} sctrl_reg_t;

typedef char size_check_sctrl_reg_t[(sizeof(sctrl_reg_t) - 4)?-1: 1];
static inline void cache_disable(void)
{
  sctrl_reg_t sctrl;
  __asm__ __volatile__ (
      "mrc p15, 0, %0, c1, c0, 0\n\t"
      : "=r" (sctrl.reg_value)
      :
      :
  );
  sctrl.bit_field.cache_enable = 0;
  sctrl.bit_field.instruct_cache_enable = 0;
  __asm__ __volatile__ (
      "mcr p15, 0, %0, c1, c0, 0\n\t"
      :
      : "r" (sctrl.reg_value)
      :
  );
}

static inline void cache_enable(void)
{
  sctrl_reg_t sctrl;
  __asm__ __volatile__ (
      "mrc p15, 0, %0, c1, c0, 0\n\t"
      : "=r" (sctrl.reg_value)
      :
      :
  );
  sctrl.bit_field.cache_enable = 1;
  sctrl.bit_field.instruct_cache_enable = 1;
  __asm__ __volatile__ (
      "mcr p15, 0, %0, c1, c0, 0\n\t"
      :
      : "r" (sctrl.reg_value)
      :
  );
}

typedef union {
  struct {
    volatile uint32_t IminLine :  4;  /*log2(num of word)*/
    volatile uint32_t reserved0 : 10;
    volatile uint32_t L1Ip : 2;
    volatile uint32_t DminLine : 4;
    volatile uint32_t erg : 4;
    volatile uint32_t cwg : 4;
    volatile uint32_t reserved1: 3;
    volatile uint32_t reserved2: 1;
  } bit_field;
  volatile uint32_t reg_value;
} ctrl_reg_t;
typedef char size_check_ctrl_reg_t[(sizeof(ctrl_reg_t) - 4)?-1: 1];

#define TAG_POLICY_RESERVED 0x00u
#define TAG_POLICY_ASID 0x01u
#define TAG_POLICY_VIRTUAL 0x02u
#define TAG_POLICY_PHYSICAL 0x03u

static const char * const tag_policy_describ[] =
    {
        [TAG_POLICY_RESERVED] = "reserved",
        [TAG_POLICY_ASID] = "asid-tagged virtual index, virtual tag",
        [TAG_POLICY_VIRTUAL] = "virtual index, physical tag",
        [TAG_POLICY_PHYSICAL] = "physical index, physical tag"
    };

typedef union {
  struct {
    volatile uint32_t Ctype_l1 : 3;
    volatile uint32_t Ctype_l2 : 3;
    volatile uint32_t Ctype_l3 : 3;
    volatile uint32_t Ctype_l4 : 3;
    volatile uint32_t Ctype_l5 : 3;
    volatile uint32_t Ctype_l6 : 3;
    volatile uint32_t Ctype_l7 : 3;
    volatile uint32_t LoUIS : 3;
    volatile uint32_t LoC : 3;
    volatile uint32_t LoUU : 3;
    volatile uint32_t reserved: 2;
  } bit_field;
  volatile uint32_t reg_value;
} clidr_reg_t;
typedef char size_check_clidr_reg_t[(sizeof(clidr_reg_t) - 4)?-1: 1];

#define CACHE_HIBERACHY_MAX 7

#define CACHE_TYPE_NONE 0x00u
#define CACHE_TYPE_INSTRUCTION_ONLY 0x01u
#define CACHE_TYPE_DATA_ONLY 0x02u
#define CACHE_TYPE_SEPARATE 0x03u
#define CACHE_TYPE_UNIFIED 0x04u
static const char * const cache_type[] = {
    [CACHE_TYPE_NONE] = "No cache",
    [CACHE_TYPE_INSTRUCTION_ONLY] = "instruction cache only",
    [CACHE_TYPE_DATA_ONLY] = "data cache only",
    [CACHE_TYPE_SEPARATE] = "separaty instruction and data caches",
    [CACHE_TYPE_UNIFIED] = "unified cache",
    [5] = "reserved",
    [CACHE_HIBERACHY_MAX - 1] = "reserved",
};

typedef union {
  struct {
    volatile uint32_t InD : 1;
    volatile uint32_t Level : 3;
    volatile uint32_t reserved : 28;
  } bit_field;
  volatile uint32_t reg_value;
} csselr_reg_t;
typedef char size_check_csselr_reg_t[(sizeof(csselr_reg_t) - 4)?-1: 1];

typedef union {
  struct {
    volatile uint32_t LineSize : 3;
    volatile uint32_t Associativity : 10;
    volatile uint32_t NumSets : 15;
    volatile uint32_t WA : 1;
    volatile uint32_t RA : 1;
    volatile uint32_t WB : 1;
    volatile uint32_t WT : 1;
  } bit_field;
  volatile uint32_t reg_value;
} ccsidr_reg_t;
typedef char size_check_ccsidr_reg_t[(sizeof(ccsidr_reg_t) - 4)?-1: 1];

static const char * const write_through[] = {
    [0] = "Write-through not support",
    [1] = "Write-through support",
};
static const char * const write_back[] = {
    [0] = "Write-back not support",
    [1] = "Write-back support",
};
static const char * const read_allocation[] = {
    [0] = "Read-allocation not support",
    [1] = "Read-allocation support",
};
static const char * const write_allocation[] = {
    [0] = "Write-allocation not support",
    [1] = "Write-allocation support",
};

typedef union {
  struct {
    volatile uint32_t N : 3;
    volatile uint32_t reserved0 : 1;
    volatile uint32_t PD0 : 1;
    volatile uint32_t PD1 : 1;
    volatile uint32_t reserved1 : 25;
    volatile uint32_t EAE : 1;
  } short_format;
  struct {
    volatile uint32_t T0SZ : 3;
    volatile uint32_t reserved0 : 4;
    volatile uint32_t EPD0 : 1;
    volatile uint32_t IRGN0 : 2;
    volatile uint32_t ORGN0 : 2;
    volatile uint32_t SH0 : 2;
    volatile uint32_t reserved1 : 2;
    volatile uint32_t T1SZ : 3;
    volatile uint32_t reserved2 : 3;
    volatile uint32_t A1 : 1;
    volatile uint32_t EPD1 : 1;
    volatile uint32_t IRGN1 : 2;
    volatile uint32_t ORGN1 : 2;
    volatile uint32_t SH1 : 2;
    volatile uint32_t imp_def : 1;
    volatile uint32_t EAE : 1;
  } long_format;
  volatile uint32_t reg_value;
}ttbcr_reg_t;
typedef char size_check_ttbcr_reg_t[(sizeof(ttbcr_reg_t) - 4)?-1: 1];

typedef union {
  struct {
    volatile uint32_t IRGN1 : 1;
    volatile uint32_t S : 1;
    volatile uint32_t imp_def : 1;
    volatile uint32_t RGN : 2;
    volatile uint32_t NOS : 1;
    volatile uint32_t IRGN0 : 1;
    volatile uint32_t TTB0 : 25;
  } short_format;
  struct {
    volatile uint32_t dont_use;
  } long_format;
  volatile uint32_t reg_value;
} ttbr0_reg_t;

typedef union {
  struct {
    volatile uint32_t IRGN1 : 1;
    volatile uint32_t S : 1;
    volatile uint32_t imp_def : 1;
    volatile uint32_t RGN : 2;
    volatile uint32_t NOS : 1;
    volatile uint32_t IRGN0 : 1;
    volatile uint32_t TTB1 : 25;
  } short_format;
  struct {
    volatile uint32_t dont_use;
  } long_format;
  volatile uint32_t reg_value;
} ttbr1_reg_t;

typedef union {
  struct {
    uint32_t type : 2;
    uint32_t reserved0 : 30;
  } invalid;
  struct {
    uint32_t type : 2;
    uint32_t PXN : 1;
    uint32_t NS : 1;
    uint32_t SBZ : 1;
    uint32_t Domain : 4;
    uint32_t imp_def : 1;
    uint32_t base_addr : 22;
  } page;
  struct {
    uint32_t PXN : 1;
    uint32_t type : 1;
    uint32_t B : 1;
    uint32_t C : 1;
    uint32_t XN : 1;
    uint32_t Domain: 4;
    uint32_t imp_def : 1;
    uint32_t AP : 2;
    uint32_t TEX : 3;
    uint32_t AP2: 1;
    uint32_t S : 1;
    uint32_t nG : 1;
    uint32_t super : 1;
    uint32_t NS : 1;
    uint32_t base_addr : 12;
  } section;
  struct {
    uint32_t PXN : 1;
    uint32_t type : 1;
    uint32_t B : 1;
    uint32_t C : 1;
    uint32_t XN : 1;
    uint32_t ext_base_addr39_36: 4;
    uint32_t imp_def : 1;
    uint32_t AP : 2;
    uint32_t TEX : 3;
    uint32_t AP2: 1;
    uint32_t S : 1;
    uint32_t nG : 1;
    uint32_t super : 1;
    uint32_t NS : 1;
    uint32_t ext_base_addr35_32 : 4;
    uint32_t super_base_addr : 8;
  } super_section;
  uint32_t value;
} l1_desc_t;

typedef char size_check_l1_desc_t[(sizeof(l1_desc_t) - 4)?-1: 1];

typedef union {
  struct {
    uint32_t type : 2;
    uint32_t reserved1 : 30;
  } invalid;
  struct {
    uint32_t XN : 1;
    uint32_t type : 1;
    uint32_t B : 1;
    uint32_t C : 1;
    uint32_t AP : 2;
    uint32_t TEX : 3;
    uint32_t AP2 : 1;
    uint32_t S : 1;
    uint32_t nG : 1;
    uint32_t base_addr : 20;
  } small_page;
  struct {
    uint32_t type : 2;
    uint32_t B : 1;
    uint32_t C : 1;
    uint32_t AP : 2;
    uint32_t SBZ : 3;
    uint32_t AP2 : 1;
    uint32_t S : 1;
    uint32_t nG : 1;
    uint32_t TEX : 3;
    uint32_t PXN : 1;
    uint32_t base_addr : 16;
  } large_page;
  uint32_t value;
} l2_desc_t;
typedef char size_check_l2_desc_t[(sizeof(l2_desc_t) - 4)?-1: 1];

static void cache_discovery(void)
{
  ctrl_reg_t ctrl;
  __asm__ __volatile__ (
      "mrc p15, 0, %0, c0, c0, 1\n\t"
      : "=r" (ctrl.reg_value)
      :
      :
  );

  uart_printf("ctr %X\n",ctrl);
  uart_printf("    iminline %u  ctr.dminline %u\n",4u<<ctrl.bit_field.IminLine, 4u<<ctrl.bit_field.IminLine);
  uart_printf("    l1 tag policy %u(%s)\n", ctrl.bit_field.L1Ip, tag_policy_describ[ctrl.bit_field.L1Ip]);

  clidr_reg_t clidr;
  __asm__ __volatile__ (
      "mrc p15, 1, %0, c0, c0, 1\n\t"
      : "=r" (clidr.reg_value)
      :
      :
  );

  uart_printf("clidr %X\n",clidr);
  for(uint32_t i = 0; i < CACHE_HIBERACHY_MAX; i++)
  {
    uint32_t tmp;
    tmp = clidr.reg_value >> (i * 3);
    tmp &= 0b111;
    uart_printf("    l%u cache : %s\n",i + 1, cache_type[tmp]);
  }

  uart_printf("    %u :Level of Unification Uniprocessor for the cache hierarchy\n",clidr.bit_field.LoUU);
  uart_printf("    %u :Level of Coherency for the cache hierarchy\n",clidr.bit_field.LoC);
  uart_printf("    %u :Level of Unification Inner Shareable for the cache hierarchy\n",clidr.bit_field.LoUIS);

  for(uint32_t i = 0; i < CACHE_HIBERACHY_MAX; i++)
  {
    uint32_t tmp;
    csselr_reg_t csselr;
    ccsidr_reg_t ccsidr;
    tmp = clidr.reg_value >> (i * 3);
    tmp &= 0b111;
    if(CACHE_TYPE_INSTRUCTION_ONLY == tmp || CACHE_TYPE_SEPARATE == tmp)
    {
      csselr.bit_field.Level = i;
      csselr.bit_field.InD = 1;
      __asm__ __volatile__ (
          "mcr p15, 2, %1, c0, c0, 0\n\t"
          "mrc p15, 1, %0, c0, c0, 0\n\t"
          : "=r" (ccsidr.reg_value)
          : "r" (csselr.reg_value)
          :
      );
      uart_printf("l%u instruct cache config:%X\n", i + 1, ccsidr.reg_value);
      uart_printf("    %s %s %s %s line size:%u associativity:%u number of sets:%u\n",
                  write_through[ccsidr.bit_field.WT],
                  write_back[ccsidr.bit_field.WB],
                  read_allocation[ccsidr.bit_field.RA],
                  write_allocation[ccsidr.bit_field.WA],
                  1u << (ccsidr.bit_field.LineSize + 4),
                  ccsidr.bit_field.Associativity + 1,
                  ccsidr.bit_field.NumSets + 1);
    }
    if(CACHE_TYPE_DATA_ONLY == tmp || CACHE_TYPE_SEPARATE == tmp || CACHE_TYPE_UNIFIED == tmp)
    {
      csselr.bit_field.Level = i;
      csselr.bit_field.InD = 0;
      __asm__ __volatile__ (
          "mcr p15, 2, %1, c0, c0, 0\n\t"
          "mrc p15, 1, %0, c0, c0, 0\n\t"
          : "=r" (ccsidr.reg_value)
          : "r" (csselr.reg_value)
          :
      );
      uart_printf("l%u data cache config:%X\n", i + 1, ccsidr.reg_value);
      uart_printf("    %s %s %s %s line size:%u associativity:%u number of sets:%u\n",
                  write_through[ccsidr.bit_field.WT],
                  write_back[ccsidr.bit_field.WB],
                  read_allocation[ccsidr.bit_field.RA],
                  write_allocation[ccsidr.bit_field.WA],
                  1u << (ccsidr.bit_field.LineSize + 4),
                  ccsidr.bit_field.Associativity + 1,
                  ccsidr.bit_field.NumSets + 1);
    }
  }
}

extern l1_desc_t  * _tlb_base;
void do_mmu_init(void)
{
  /* disable l1 cache, mmu stuff  */
  sctrl_reg_t sctrl;
  ctrl_reg_t ctrl;
  clidr_reg_t clidr;
  csselr_reg_t csselr;
  ccsidr_reg_t ccsidr;
  uint32_t tmp = 0;
  uint32_t way, set, line, way_offset;

  __asm__ __volatile__ (
      "mrc p15, 0, %0, c1, c0, 0\n\t"
      : "=r" (sctrl.reg_value)
      :
      :
  );
  sctrl.bit_field.instruct_cache_enable = 0;
  sctrl.bit_field.cache_enable = 0;
  sctrl.bit_field.mmu_enable = 0;
  __asm__ __volatile__ (
      "mcr p15, 0, %0, c1, c0, 0\n\t"
      :
      : "r" (sctrl.reg_value)
      :
  );

  __asm__ __volatile__ (
      "mrc p15, 0, %0, c0, c0, 1\n\t"
      : "=r" (ctrl.reg_value)
      :
      :
  );

  __asm__ __volatile__ (
      "mrc p15, 1, %0, c0, c0, 1\n\t"
      : "=r" (clidr.reg_value)
      :
      :
  );

  /* invalid all instruct cache */
  __asm__ __volatile (
      "mcr p15, 0, %0, c7, c5, 0\n\t"
      :
      : "r" (tmp)
      :
  );

  for(uint32_t i = 0; i < CACHE_HIBERACHY_MAX; i++)
  {
    tmp = clidr.reg_value >> (i*3);
    tmp &= 0b111;
    if((tmp == CACHE_TYPE_NONE) || (tmp > CACHE_TYPE_UNIFIED) || (tmp == CACHE_TYPE_INSTRUCTION_ONLY))
    {
      break;
    }

    csselr.bit_field.Level = i;
    csselr.bit_field.InD = 0;
    __asm__ __volatile__ (
        "mcr p15, 2, %1, c0, c0, 0\n\t"
        "mrc p15, 1, %0, c0, c0, 0\n\t"
        : "=r" (ccsidr.reg_value)
        : "r" (csselr.reg_value)
        :
    );
    set = ccsidr.bit_field.NumSets;
    way = ccsidr.bit_field.Associativity;
    line = ccsidr.bit_field.LineSize + 4;
    way_offset = __builtin_clz(way);
    for(uint32_t j = 0; i <= way; i++)
    {
      for(uint32_t k = 0; j <= set; j++)
      {
        tmp = i << 1; /* level */
        tmp |= k << line;
        tmp |= j << way_offset;
        /* invalid all instruct cache */
        __asm__ __volatile (
            "mcr p15, 0, %0, c7, c6, 2\n\t"
            :
            : "r" (tmp)
            :
        );
      }
    }
  }
#define DEVICE_MEM_TYPE 0x1
  /* make mmu table */
  for(uint32_t i = 0; i < 16*1024; i++)
  {
    _tlb_base[i].section.type = 1;   /* section type*/
    _tlb_base[i].section.super = 0;  /* not super */
    _tlb_base[i].section.XN = 1; /* execute never */
    _tlb_base[i].section.PXN = 1; /* privileged execute never*/
    _tlb_base[i].section.AP2 = 0; /* dont disable write access */
    _tlb_base[i].section.AP = (0 << 1) | (0 << 0); /* enable access from el0, and enable tlb cache*/
    _tlb_base[i].section.B = (DEVICE_MEM_TYPE >> 0)&0x01;
    _tlb_base[i].section.C = (DEVICE_MEM_TYPE >> 1)&0x01;
    _tlb_base[i].section.TEX = (DEVICE_MEM_TYPE >> 2)&0x01;
    _tlb_base[i].section.S = 0; /* not shared */
    _tlb_base[i].section.NS = 1; /* not security*/
    _tlb_base[i].section.nG = 0; /* global */
    _tlb_base[i].section.Domain = 1;
    _tlb_base[i].section.base_addr = i;
  }
  /* domain configure */
  tmp = 0x55555555;
  __asm__ __volatile__ (
          "mcr p15, 0, %0, c3, c0, 0\n\t"
          :
          : "r" (tmp)
          :
  );

  __asm__ __volatile__ (
      "mrc p15, 0, %0, c1, c0, 0\n\t"
      : "=r" (sctrl.reg_value)
      :
      :
  );
  sctrl.bit_field.instruct_cache_enable = 1;
  sctrl.bit_field.cache_enable = 1;
  sctrl.bit_field.mmu_enable = 1;
  sctrl.bit_field.afe = 0;
  __asm__ __volatile__ (
      "mcr p15, 0, %0, c1, c0, 0\n\t"
      :
      : "r" (sctrl.reg_value)
      :
  );
}

void mmu_init(void)
{
	cache_disable();
	cache_discovery();
	do_mmu_init();
}
