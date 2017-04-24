#ifndef _NDS32_ISR_H
#define _NDS32_ISR_H 
#define NDS32_READY_NESTED 0
#define NDS32_NESTED 1
#define NDS32_NOT_NESTED 2
#define NDS32_CRITICAL 3
#define NDS32_SAVE_CALLER_REGS 0
#define NDS32_SAVE_ALL_REGS 1
typedef struct
{
  int r0;
  int r1;
  int r2;
  int r3;
  int r4;
  int r5;
  int r6;
  int r7;
  int r8;
  int r9;
  int r10;
  int r11;
  int r12;
  int r13;
  int r14;
  int r15;
  int r16;
  int r17;
  int r18;
  int r19;
  int r20;
  int r21;
  int r22;
  int r23;
  int r24;
  int r25;
  int r26;
  int r27;
  int fp;
  int gp;
  int lp;
  int sp;
} NDS32_GPR32;
typedef struct
{
  int r0;
  int r1;
  int r2;
  int r3;
  int r4;
  int r5;
  int r6;
  int r7;
  int r8;
  int r9;
  int r10;
  int r15;
  int fp;
  int gp;
  int lp;
  int sp;
} NDS32_GPR16;
typedef struct
{
  union
    {
      int reg_a[32] ;
      NDS32_GPR32 reg_s ;
    } u ;
} NDS32_REG32_TAB;
typedef struct
{
  union
    {
      int reg_a[16] ;
      NDS32_GPR16 reg_s ;
    } u ;
} NDS32_REG16_TAB;
typedef struct
{
  int d0lo;
  int d0hi;
  int d1lo;
  int d1hi;
} NDS32_DX_TAB;
typedef struct
{
#ifdef __NDS32_EB__
  float fsr0;
  float fsr1;
  float fsr2;
  float fsr3;
  float fsr4;
  float fsr5;
  float fsr6;
  float fsr7;
#else
  float fsr1;
  float fsr0;
  float fsr3;
  float fsr2;
  float fsr5;
  float fsr4;
  float fsr7;
  float fsr6;
#endif
} NDS32_FSR8;
typedef struct
{
  double dsr0;
  double dsr1;
  double dsr2;
  double dsr3;
} NDS32_DSR4;
typedef struct
{
#ifdef __NDS32_EB__
  float fsr0;
  float fsr1;
  float fsr2;
  float fsr3;
  float fsr4;
  float fsr5;
  float fsr6;
  float fsr7;
  float fsr8;
  float fsr9;
  float fsr10;
  float fsr11;
  float fsr12;
  float fsr13;
  float fsr14;
  float fsr15;
#else
  float fsr1;
  float fsr0;
  float fsr3;
  float fsr2;
  float fsr5;
  float fsr4;
  float fsr7;
  float fsr6;
  float fsr9;
  float fsr8;
  float fsr11;
  float fsr10;
  float fsr13;
  float fsr12;
  float fsr15;
  float fsr14;
#endif
} NDS32_FSR16;
typedef struct
{
  double dsr0;
  double dsr1;
  double dsr2;
  double dsr3;
  double dsr4;
  double dsr5;
  double dsr6;
  double dsr7;
} NDS32_DSR8;
typedef struct
{
#ifdef __NDS32_EB__
  float fsr0;
  float fsr1;
  float fsr2;
  float fsr3;
  float fsr4;
  float fsr5;
  float fsr6;
  float fsr7;
  float fsr8;
  float fsr9;
  float fsr10;
  float fsr11;
  float fsr12;
  float fsr13;
  float fsr14;
  float fsr15;
  float fsr16;
  float fsr17;
  float fsr18;
  float fsr19;
  float fsr20;
  float fsr21;
  float fsr22;
  float fsr23;
  float fsr24;
  float fsr25;
  float fsr26;
  float fsr27;
  float fsr28;
  float fsr29;
  float fsr30;
  float fsr31;
#else
  float fsr1;
  float fsr0;
  float fsr3;
  float fsr2;
  float fsr5;
  float fsr4;
  float fsr7;
  float fsr6;
  float fsr9;
  float fsr8;
  float fsr11;
  float fsr10;
  float fsr13;
  float fsr12;
  float fsr15;
  float fsr14;
  float fsr17;
  float fsr16;
  float fsr19;
  float fsr18;
  float fsr21;
  float fsr20;
  float fsr23;
  float fsr22;
  float fsr25;
  float fsr24;
  float fsr27;
  float fsr26;
  float fsr29;
  float fsr28;
  float fsr31;
  float fsr30;
#endif
} NDS32_FSR32;
typedef struct
{
  double dsr0;
  double dsr1;
  double dsr2;
  double dsr3;
  double dsr4;
  double dsr5;
  double dsr6;
  double dsr7;
  double dsr8;
  double dsr9;
  double dsr10;
  double dsr11;
  double dsr12;
  double dsr13;
  double dsr14;
  double dsr15;
} NDS32_DSR16;
typedef struct
{
  double dsr0;
  double dsr1;
  double dsr2;
  double dsr3;
  double dsr4;
  double dsr5;
  double dsr6;
  double dsr7;
  double dsr8;
  double dsr9;
  double dsr10;
  double dsr11;
  double dsr12;
  double dsr13;
  double dsr14;
  double dsr15;
  double dsr16;
  double dsr17;
  double dsr18;
  double dsr19;
  double dsr20;
  double dsr21;
  double dsr22;
  double dsr23;
  double dsr24;
  double dsr25;
  double dsr26;
  double dsr27;
  double dsr28;
  double dsr29;
  double dsr30;
  double dsr31;
} NDS32_DSR32;
typedef struct
{
  union
    {
      NDS32_FSR8 fsr_s ;
      NDS32_DSR4 dsr_s ;
    } u ;
} NDS32_FPU8_TAB;
typedef struct
{
  union
    {
      NDS32_FSR16 fsr_s ;
      NDS32_DSR8 dsr_s ;
    } u ;
} NDS32_FPU16_TAB;
typedef struct
{
  union
    {
      NDS32_FSR32 fsr_s ;
      NDS32_DSR16 dsr_s ;
    } u ;
} NDS32_FPU32_TAB;
typedef struct
{
  union
    {
      NDS32_FSR32 fsr_s ;
      NDS32_DSR32 dsr_s ;
    } u ;
} NDS32_FPU64_TAB;
typedef struct
{
  int ipc;
  int ipsw;
#if defined(NDS32_EXT_FPU_CONFIG_0)
  NDS32_FPU8_TAB fpr;
#elif defined(NDS32_EXT_FPU_CONFIG_1)
  NDS32_FPU16_TAB fpr;
#elif defined(NDS32_EXT_FPU_CONFIG_2)
  NDS32_FPU32_TAB fpr;
#elif defined(NDS32_EXT_FPU_CONFIG_3)
  NDS32_FPU64_TAB fpr;
#endif
#if __NDS32_DX_REGS__
  NDS32_DX_TAB dxr;
#endif
#if __NDS32_EXT_IFC__
  int ifc_lp;
  int filler;
#endif
#if __NDS32_REDUCED_REGS__ || __NDS32_REDUCE_REGS
  NDS32_REG16_TAB gpr;
#else
  NDS32_REG32_TAB gpr;
#endif
} NDS32_CONTEXT;
#define NDS32_VECTOR_RESET 0
#define NDS32_VECTOR_TLB_FILL 1
#define NDS32_VECTOR_PTE_NOT_PRESENT 2
#define NDS32_VECTOR_TLB_MISC 3
#define NDS32_VECTOR_TLB_VLPT_MISS 4
#define NDS32_VECTOR_MACHINE_ERROR 5
#define NDS32_VECTOR_DEBUG_RELATED 6
#define NDS32_VECTOR_GENERAL_EXCEPTION 7
#define NDS32_VECTOR_SYSCALL 8
#define NDS32_VECTOR_INTERRUPT_HW0 9
#define NDS32_VECTOR_INTERRUPT_HW1 10
#define NDS32_VECTOR_INTERRUPT_HW2 11
#define NDS32_VECTOR_INTERRUPT_HW3 12
#define NDS32_VECTOR_INTERRUPT_HW4 13
#define NDS32_VECTOR_INTERRUPT_HW5 14
#define NDS32_VECTOR_INTERRUPT_HW6 15
#define NDS32_VECTOR_SWI 15
#define NDS32_VECTOR_INTERRUPT_HW7 16
#define NDS32_VECTOR_INTERRUPT_HW8 17
#define NDS32_VECTOR_INTERRUPT_HW9 18
#define NDS32_VECTOR_INTERRUPT_HW10 19
#define NDS32_VECTOR_INTERRUPT_HW11 20
#define NDS32_VECTOR_INTERRUPT_HW12 21
#define NDS32_VECTOR_INTERRUPT_HW13 22
#define NDS32_VECTOR_INTERRUPT_HW14 23
#define NDS32_VECTOR_INTERRUPT_HW15 24
#define NDS32_VECTOR_INTERRUPT_HW16 25
#define NDS32_VECTOR_INTERRUPT_HW17 26
#define NDS32_VECTOR_INTERRUPT_HW18 27
#define NDS32_VECTOR_INTERRUPT_HW19 28
#define NDS32_VECTOR_INTERRUPT_HW20 29
#define NDS32_VECTOR_INTERRUPT_HW21 30
#define NDS32_VECTOR_INTERRUPT_HW22 31
#define NDS32_VECTOR_INTERRUPT_HW23 32
#define NDS32_VECTOR_INTERRUPT_HW24 33
#define NDS32_VECTOR_INTERRUPT_HW25 34
#define NDS32_VECTOR_INTERRUPT_HW26 35
#define NDS32_VECTOR_INTERRUPT_HW27 36
#define NDS32_VECTOR_INTERRUPT_HW28 37
#define NDS32_VECTOR_INTERRUPT_HW29 38
#define NDS32_VECTOR_INTERRUPT_HW30 39
#define NDS32_VECTOR_INTERRUPT_HW31 40
#define NDS32_VECTOR_INTERRUPT_HW32 41
#define NDS32_VECTOR_INTERRUPT_HW33 42
#define NDS32_VECTOR_INTERRUPT_HW34 43
#define NDS32_VECTOR_INTERRUPT_HW35 44
#define NDS32_VECTOR_INTERRUPT_HW36 45
#define NDS32_VECTOR_INTERRUPT_HW37 46
#define NDS32_VECTOR_INTERRUPT_HW38 47
#define NDS32_VECTOR_INTERRUPT_HW39 48
#define NDS32_VECTOR_INTERRUPT_HW40 49
#define NDS32_VECTOR_INTERRUPT_HW41 50
#define NDS32_VECTOR_INTERRUPT_HW42 51
#define NDS32_VECTOR_INTERRUPT_HW43 52
#define NDS32_VECTOR_INTERRUPT_HW44 53
#define NDS32_VECTOR_INTERRUPT_HW45 54
#define NDS32_VECTOR_INTERRUPT_HW46 55
#define NDS32_VECTOR_INTERRUPT_HW47 56
#define NDS32_VECTOR_INTERRUPT_HW48 57
#define NDS32_VECTOR_INTERRUPT_HW49 58
#define NDS32_VECTOR_INTERRUPT_HW50 59
#define NDS32_VECTOR_INTERRUPT_HW51 60
#define NDS32_VECTOR_INTERRUPT_HW52 61
#define NDS32_VECTOR_INTERRUPT_HW53 62
#define NDS32_VECTOR_INTERRUPT_HW54 63
#define NDS32_VECTOR_INTERRUPT_HW55 64
#define NDS32_VECTOR_INTERRUPT_HW56 65
#define NDS32_VECTOR_INTERRUPT_HW57 66
#define NDS32_VECTOR_INTERRUPT_HW58 67
#define NDS32_VECTOR_INTERRUPT_HW59 68
#define NDS32_VECTOR_INTERRUPT_HW60 69
#define NDS32_VECTOR_INTERRUPT_HW61 70
#define NDS32_VECTOR_INTERRUPT_HW62 71
#define NDS32_VECTOR_INTERRUPT_HW63 72
#define NDS32ATTR_RESET(option) __attribute__((reset(option)))
#define NDS32ATTR_EXCEPT(type) __attribute__((exception(type)))
#define NDS32ATTR_EXCEPTION(type) __attribute__((exception(type)))
#define NDS32ATTR_INTERRUPT(type) __attribute__((interrupt(type)))
#define NDS32ATTR_ISR(type) __attribute__((interrupt(type)))
#endif
