#ifndef _NDS_INTRINSIC_H
#define _NDS_INTRINSIC_H

typedef signed char int8x4_t __attribute ((vector_size(4)));
typedef signed char int8x8_t __attribute ((vector_size(8)));
typedef short int16x2_t __attribute ((vector_size(4)));
typedef short int16x4_t __attribute__((vector_size (8)));
typedef short int16x8_t __attribute__((vector_size (16)));
typedef int int32x2_t __attribute__((vector_size(8)));
typedef int int32x4_t __attribute__((vector_size(16)));
typedef unsigned char uint8x4_t __attribute__ ((vector_size (4)));
typedef unsigned char uint8x8_t __attribute__ ((vector_size (8)));
typedef unsigned short uint16x2_t __attribute__ ((vector_size (4)));
typedef unsigned short uint16x4_t __attribute__((vector_size (8)));
typedef unsigned short uint16x8_t __attribute__((vector_size (16)));
typedef unsigned int uint32x2_t __attribute__((vector_size(8)));
typedef unsigned int uint32x4_t __attribute__((vector_size(16)));

enum riscv_csrs
{
  NDS_FFLAGS = 4096,
  NDS_FRM,
  NDS_FCSR,
  NDS_CYCLE,
  NDS_TIME,
  NDS_INSTRET,
  NDS_HPMCOUNTER3,
  NDS_HPMCOUNTER4,
  NDS_HPMCOUNTER5,
  NDS_HPMCOUNTER6,
  NDS_HPMCOUNTER7,
  NDS_HPMCOUNTER8,
  NDS_HPMCOUNTER9,
  NDS_HPMCOUNTER10,
  NDS_HPMCOUNTER11,
  NDS_HPMCOUNTER12,
  NDS_HPMCOUNTER13,
  NDS_HPMCOUNTER14,
  NDS_HPMCOUNTER15,
  NDS_HPMCOUNTER16,
  NDS_HPMCOUNTER17,
  NDS_HPMCOUNTER18,
  NDS_HPMCOUNTER19,
  NDS_HPMCOUNTER20,
  NDS_HPMCOUNTER21,
  NDS_HPMCOUNTER22,
  NDS_HPMCOUNTER23,
  NDS_HPMCOUNTER24,
  NDS_HPMCOUNTER25,
  NDS_HPMCOUNTER26,
  NDS_HPMCOUNTER27,
  NDS_HPMCOUNTER28,
  NDS_HPMCOUNTER29,
  NDS_HPMCOUNTER30,
  NDS_HPMCOUNTER31,
  NDS_SSTATUS,
  NDS_SIE,
  NDS_STVEC,
  NDS_SSCRATCH,
  NDS_SEPC,
  NDS_SCAUSE,
  NDS_SBADADDR,
  NDS_SIP,
  NDS_SPTBR,
  NDS_MSTATUS,
  NDS_MISA,
  NDS_MEDELEG,
  NDS_MIDELEG,
  NDS_MIE,
  NDS_MTVEC,
  NDS_MSCRATCH,
  NDS_MEPC,
  NDS_MCAUSE,
  NDS_MIP,
  NDS_TSELECT,
  NDS_TDATA1,
  NDS_TDATA2,
  NDS_TDATA3,
  NDS_DCSR,
  NDS_DPC,
  NDS_MCYCLE,
  NDS_MINSTRET,
  NDS_MHPMCOUNTER3,
  NDS_MHPMCOUNTER4,
  NDS_MHPMCOUNTER5,
  NDS_MHPMCOUNTER6,
  NDS_MHPMCOUNTER7,
  NDS_MHPMCOUNTER8,
  NDS_MHPMCOUNTER9,
  NDS_MHPMCOUNTER10,
  NDS_MHPMCOUNTER11,
  NDS_MHPMCOUNTER12,
  NDS_MHPMCOUNTER13,
  NDS_MHPMCOUNTER14,
  NDS_MHPMCOUNTER15,
  NDS_MHPMCOUNTER16,
  NDS_MHPMCOUNTER17,
  NDS_MHPMCOUNTER18,
  NDS_MHPMCOUNTER19,
  NDS_MHPMCOUNTER20,
  NDS_MHPMCOUNTER21,
  NDS_MHPMCOUNTER22,
  NDS_MHPMCOUNTER23,
  NDS_MHPMCOUNTER24,
  NDS_MHPMCOUNTER25,
  NDS_MHPMCOUNTER26,
  NDS_MHPMCOUNTER27,
  NDS_MHPMCOUNTER28,
  NDS_MHPMCOUNTER29,
  NDS_MHPMCOUNTER30,
  NDS_MHPMCOUNTER31,
  NDS_MUCOUNTEREN,
  NDS_MSCOUNTEREN,
  NDS_MHPMEVENT3,
  NDS_MHPMEVENT4,
  NDS_MHPMEVENT5,
  NDS_MHPMEVENT6,
  NDS_MHPMEVENT7,
  NDS_MHPMEVENT8,
  NDS_MHPMEVENT9,
  NDS_MHPMEVENT10,
  NDS_MHPMEVENT11,
  NDS_MHPMEVENT12,
  NDS_MHPMEVENT13,
  NDS_MHPMEVENT14,
  NDS_MHPMEVENT15,
  NDS_MHPMEVENT16,
  NDS_MHPMEVENT17,
  NDS_MHPMEVENT18,
  NDS_MHPMEVENT19,
  NDS_MHPMEVENT20,
  NDS_MHPMEVENT21,
  NDS_MHPMEVENT22,
  NDS_MHPMEVENT23,
  NDS_MHPMEVENT24,
  NDS_MHPMEVENT25,
  NDS_MHPMEVENT26,
  NDS_MHPMEVENT27,
  NDS_MHPMEVENT28,
  NDS_MHPMEVENT29,
  NDS_MHPMEVENT30,
  NDS_MHPMEVENT31,
  NDS_MVENDORID,
  NDS_MARCHID,
  NDS_MIMPID,
  NDS_MHARTID,
  NDS_CYCLEH,
  NDS_TIMEH,
  NDS_INSTRETH,
  NDS_HPMCOUNTER3H,
  NDS_HPMCOUNTER4H,
  NDS_HPMCOUNTER5H,
  NDS_HPMCOUNTER6H,
  NDS_HPMCOUNTER7H,
  NDS_HPMCOUNTER8H,
  NDS_HPMCOUNTER9H,
  NDS_HPMCOUNTER10H,
  NDS_HPMCOUNTER11H,
  NDS_HPMCOUNTER12H,
  NDS_HPMCOUNTER13H,
  NDS_HPMCOUNTER14H,
  NDS_HPMCOUNTER15H,
  NDS_HPMCOUNTER16H,
  NDS_HPMCOUNTER17H,
  NDS_HPMCOUNTER18H,
  NDS_HPMCOUNTER19H,
  NDS_HPMCOUNTER20H,
  NDS_HPMCOUNTER21H,
  NDS_HPMCOUNTER22H,
  NDS_HPMCOUNTER23H,
  NDS_HPMCOUNTER24H,
  NDS_HPMCOUNTER25H,
  NDS_HPMCOUNTER26H,
  NDS_HPMCOUNTER27H,
  NDS_HPMCOUNTER28H,
  NDS_HPMCOUNTER29H,
  NDS_HPMCOUNTER30H,
  NDS_HPMCOUNTER31H,
  NDS_MCYCLEH,
  NDS_MINSTRETH,
  NDS_MHPMCOUNTER3H,
  NDS_MHPMCOUNTER4H,
  NDS_MHPMCOUNTER5H,
  NDS_MHPMCOUNTER6H,
  NDS_MHPMCOUNTER7H,
  NDS_MHPMCOUNTER8H,
  NDS_MHPMCOUNTER9H,
  NDS_MHPMCOUNTER10H,
  NDS_MHPMCOUNTER11H,
  NDS_MHPMCOUNTER12H,
  NDS_MHPMCOUNTER13H,
  NDS_MHPMCOUNTER14H,
  NDS_MHPMCOUNTER15H,
  NDS_MHPMCOUNTER16H,
  NDS_MHPMCOUNTER17H,
  NDS_MHPMCOUNTER18H,
  NDS_MHPMCOUNTER19H,
  NDS_MHPMCOUNTER20H,
  NDS_MHPMCOUNTER21H,
  NDS_MHPMCOUNTER22H,
  NDS_MHPMCOUNTER23H,
  NDS_MHPMCOUNTER24H,
  NDS_MHPMCOUNTER25H,
  NDS_MHPMCOUNTER26H,
  NDS_MHPMCOUNTER27H,
  NDS_MHPMCOUNTER28H,
  NDS_MHPMCOUNTER29H,
  NDS_MHPMCOUNTER30H,
  NDS_MHPMCOUNTER31H,
  NDS_MICM_CFG,
  NDS_MDCM_CFG,
  NDS_MMSC_CFG,
  NDS_MCOUNTEREN,
  NDS_MTVAL,
  NDS_PMPCFG0,
  NDS_PMPCFG1,
  NDS_PMPCFG2,
  NDS_PMPCFG3,
  NDS_PMPADDR0,
  NDS_PMPADDR1,
  NDS_PMPADDR2,
  NDS_PMPADDR3,
  NDS_PMPADDR4,
  NDS_PMPADDR5,
  NDS_PMPADDR6,
  NDS_PMPADDR7,
  NDS_PMPADDR8,
  NDS_PMPADDR9,
  NDS_PMPADDR10,
  NDS_PMPADDR11,
  NDS_PMPADDR12,
  NDS_PMPADDR13,
  NDS_PMPADDR14,
  NDS_PMPADDR15,
  NDS_MCONTROL,
  NDS_MILMB,
  NDS_MDLMB,
  NDS_MECC_CODE,
  NDS_MNVEC,
  NDS_MHSP_CTL,
  NDS_MSP_BOUND,
  NDS_MSP_BASE,
  NDS_MRANDSEQ,
  NDS_MRANDSEQH,
  NDS_MRANDSTATE,
  NDS_MRANDSTATEH,
  NDS_DEXC2DBG,
  NDS_DDCAUSE,
  NDS_USTATUS,
  NDS_UIE,
  NDS_UTVEC,
  NDS_USCRATCH,
  NDS_UEPC,
  NDS_UCAUSE,
  NDS_UBADADDR,
  NDS_UIP,
  NDS_MPFT_CTL,
  NDS_MMISC_CTL,
  NDS_MXSTATUS,
  NDS_UITB,
  NDS_MBASE,
  NDS_MBOUND,
  NDS_MIBASE,
  NDS_MIBOUND,
  NDS_MDBASE,
  NDS_MDBOUND,
  NDS_DSCRATCH0,
  NDS_DSCRATCH1,
  NDS_DCAUSE,
  NDS_MCACHE_CTL,
  NDS_MDCAUSE,
  NDS_MCOUNTERWEN,
  NDS_MCOUNTERINTEN,
  NDS_MCOUNTERMASK_M,
  NDS_MCOUNTERMASK_S,
  NDS_MCOUNTERMASK_U,
  NDS_MCOUNTEROVF,
  NDS_SEDELEG,
  NDS_SIDELEG,
  NDS_SCOUNTEREN,
  NDS_STVAL,
  NDS_SATP,
  NDS_SDCAUSE,
  NDS_SCOUNTERINTEN,
  NDS_SCOUNTERMASK_S,
  NDS_SCOUNTERMASK_U,
  NDS_SCOUNTEROVF,
  NDS_MCCTLBEGINADDR,
  NDS_MCCTLCOMMAND,
  NDS_MCCTLDATA,
  NDS_MSLIDELEG,
  NDS_SLIE,
  NDS_SLIP,
  NDS_SCCTLDATA,
  NDS_SMISC_CTL,
  NDS_UCCTLBEGINADDR,
  NDS_UCCTLCOMMAND,
  NDS_MSAVESTATUS,
  NDS_MSAVEEPC1,
  NDS_MSAVECAUSE1,
  NDS_MSAVEEPC2,
  NDS_MSAVECAUSE2,
  NDS_MCOUNTINHIBIT,
  NDS_SCOUNTINHIBIT,
  NDS_SHPMEVENT3,
  NDS_SHPMEVENT4,
  NDS_SHPMEVENT5,
  NDS_SHPMEVENT6,
  NDS_UTVAL,
  NDS_UDCAUSE,
  NDS_WFE,
  NDS_SLEEPVALUE,
  NDS_TXEVT,
  NDS_MTVT,
  NDS_MNXTI,
  NDS_MINTSTATUS,
  NDS_MSCRATCHCSW,
  NDS_MSCRATCHCSWL,
  NDS_ITRIGGER,
  NDS_ETRIGGER,
  NDS_MSAVEDCAUSE1,
  NDS_MSAVEDCAUSE2,
  NDS_MIRQ_ENTRY,
  NDS_MINTSEL_JAL,
  NDS_PUSHMCAUSE,
  NDS_PUSHMEPC,
  NDS_UCODE,
  NDS_PUSHMXSTATUS,
  NDS_MPPIB,
  NDS_MFIOB,
  NDS_TEXTRA32,
  NDS_TEXTRA64,
  NDS_TINFO,
  NDS_TCONTROL,
  NDS_MCONTEXT,
  NDS_SCONTEXT,
  NDS_SCOUNTERMASK_M,
  NDS_PMACFG0,
  NDS_PMACFG1,
  NDS_PMACFG2,
  NDS_PMACFG3,
  NDS_PMAADDR0,
  NDS_PMAADDR1,
  NDS_PMAADDR2,
  NDS_PMAADDR3,
  NDS_PMAADDR4,
  NDS_PMAADDR5,
  NDS_PMAADDR6,
  NDS_PMAADDR7,
  NDS_PMAADDR8,
  NDS_PMAADDR9,
  NDS_PMAADDR10,
  NDS_PMAADDR11,
  NDS_PMAADDR12,
  NDS_PMAADDR13,
  NDS_PMAADDR14,
  NDS_PMAADDR15,
  NDS_MMSC_CFG2
};

enum riscv_fence
{
  FENCE_W = 1,
  FENCE_R,
  FENCE_RW,
  FENCE_O,
  FENCE_OW,
  FENCE_OR,
  FENCE_ORW,
  FENCE_I,
  FENCE_IW,
  FENCE_IR,
  FENCE_IRW,
  FENCE_IO,
  FENCE_IOW,
  FENCE_IOR,
  FENCE_IORW
};

enum riscv_order
{
  UNORDER,
  RELEASE,
  ACQUIRE,
  SEQUENTIAL
};

#define __nds__msync_all() asm volatile ("fence" : : : "memory")

#define __nds__cctl_l1d_wball_one_lvl()
#define __nds__cctl_l1d_wball_alvl()
#define __nds__cctl_l1d_invalall()

#define __nds__mfsr(srname) \
  (__builtin_riscv_csrr ((srname)))
#define __nds__mtsr(val, srname) \
  (__builtin_riscv_csrw ((val), (srname)))
#define __nds__csrr(srname) \
  (__builtin_riscv_csrr ((srname)))
#define __nds__csrw(val, srname) \
  (__builtin_riscv_csrw ((val), (srname)))
#define __nds__csrs(val, srname) \
  (__builtin_riscv_csrs ((val), (srname)))
#define __nds__csrc(val, srname) \
  (__builtin_riscv_csrc ((val), (srname)))

#define __nds__swap_csr(val, srname) \
  (__builtin_riscv_csrrw ((val), (srname)))
#define __nds__read_and_set_csr(val, srname) \
  (__builtin_riscv_csrrs ((val), (srname)))
#define __nds__read_and_clear_csr(val, srname) \
  (__builtin_riscv_csrrc ((val), (srname)))
#define __nds__read_csr(srname) \
  (__builtin_riscv_csrr ((srname)))
#define __nds__write_csr(val, srname) \
  (__builtin_riscv_csrw ((val), (srname)))
#define __nds__set_csr_bits(val, srname) \
  (__builtin_riscv_csrs ((val), (srname)))
#define __nds__clear_csr_bits(val, srname) \
  (__builtin_riscv_csrc ((val), (srname)))

#define __nds__ecall(sysid) \
  (__builtin_riscv_ecall ((sysid)))
#define __nds__ecall1(sysid, a) \
  (__builtin_riscv_ecall1 ((sysid), (a)))
#define __nds__ecall2(sysid, a, b) \
  (__builtin_riscv_ecall2 ((sysid), (a), (b)))
#define __nds__ecall3(sysid, a, b, c) \
  (__builtin_riscv_ecall3 ((sysid), (a), (b), (c)))
#define __nds__ecall4(sysid, a, b, c, d) \
  (__builtin_riscv_ecall4 ((sysid), (a), (b), (c), (d)))
#define __nds__ecall5(sysid, a, b, c, d, e) \
  (__builtin_riscv_ecall5 ((sysid), (a), (b), (c), (d), (e)))
#define __nds__ecall6(sysid, a, b, c, d, e, f) \
  (__builtin_riscv_ecall6 ((sysid), (a), (b), (c), (d), (e), (f)))
#define __nds__fence(a, b) \
  (__builtin_riscv_fence ((a), (b)))
#define __nds__fencei() \
  (__builtin_riscv_fencei ())
#define __nds__frcsr() \
  (__builtin_riscv_frcsr ())
#define __nds__fscsr(a) \
  (__builtin_riscv_fscsr ((a)))
#define __nds__fwcsr(a) \
  (__builtin_riscv_fwcsr ((a)))
#define __nds__frrm() \
  (__builtin_riscv_frrm ())
#define __nds__fsrm(a) \
  (__builtin_riscv_fsrm ((a)))
#define __nds__fwrm(a) \
  (__builtin_riscv_fwrm ((a)))
#define __nds__frflags() \
  (__builtin_riscv_frflags ())
#define __nds__fsflags(a) \
  (__builtin_riscv_fsflags ((a)))
#define __nds__fwflags(a) \
  (__builtin_riscv_fwflags ((a)))
#define __nds__lrw(a, b) \
  (__builtin_riscv_lrw ((a), (b)))
#define __nds__lrd(a, b) \
  (__builtin_riscv_lrd ((a), (b)))
#define __nds__scw(a, b, c) \
  (__builtin_riscv_scw ((a), (b), (c)))
#define __nds__scd(a, b, c) \
  (__builtin_riscv_scd ((a), (b), (c)))

#define __nds__amoaddw(a, b, c) \
  (__builtin_riscv_amoaddw ((a), (b), (c)))
#define __nds__amominw(a, b, c) \
  (__builtin_riscv_amominw ((a), (b), (c)))
#define __nds__amomaxw(a, b, c) \
  (__builtin_riscv_amomaxw ((a), (b), (c)))
#define __nds__amoswapw(a, b, c) \
  (__builtin_riscv_amoswapw ((a), (b), (c)))
#define __nds__amoxorw(a, b, c) \
  (__builtin_riscv_amoxorw ((a), (b), (c)))
#define __nds__amoandw(a, b, c) \
  (__builtin_riscv_amoandw ((a), (b), (c)))
#define __nds__amoorw(a, b, c) \
  (__builtin_riscv_amoorw ((a), (b), (c)))
#define __nds__amominuw(a, b, c) \
  (__builtin_riscv_amominuw ((a), (b), (c)))
#define __nds__amomaxuw(a, b, c) \
  (__builtin_riscv_amomaxuw ((a), (b), (c)))
#define __nds__amoaddd(a, b, c) \
  (__builtin_riscv_amoaddd ((a), (b), (c)))
#define __nds__amomind(a, b, c) \
  (__builtin_riscv_amomind ((a), (b), (c)))
#define __nds__amomaxd(a, b, c) \
  (__builtin_riscv_amomaxd ((a), (b), (c)))
#define __nds__amoswapd(a, b, c) \
  (__builtin_riscv_amoswapd ((a), (b), (c)))
#define __nds__amoxord(a, b, c) \
  (__builtin_riscv_amoxord ((a), (b), (c)))
#define __nds__amoandd(a, b, c) \
  (__builtin_riscv_amoandd ((a), (b), (c)))
#define __nds__amoord(a, b, c) \
  (__builtin_riscv_amoord ((a), (b), (c)))
#define __nds__amominud(a, b, c) \
  (__builtin_riscv_amominud ((a), (b), (c)))
#define __nds__amomaxud(a, b, c) \
  (__builtin_riscv_amomaxud ((a), (b), (c)))
#define __nds__ebreak(a) \
  (__builtin_riscv_ebreak ((a)))
#define __nds__csrrw(a, b) \
  (__builtin_riscv_csrrw ((a), (b)))
#define __nds__csrrs(a, b) \
  (__builtin_riscv_csrrs ((a), (b)))
#define __nds__csrrc(a, b) \
  (__builtin_riscv_csrrc ((a), (b)))
#define __nds__csrs(a, b) \
  (__builtin_riscv_csrs ((a), (b)))
#define __nds__csrc(a, b) \
  (__builtin_riscv_csrc ((a), (b)))
#define __nds__get_current_sp() \
  (__builtin_riscv_get_current_sp ())
#define __nds__set_current_sp(a) \
  (__builtin_riscv_set_current_sp ((a)))
#define __nds__add16(a, b) \
  (__builtin_riscv_add16 ((a), (b)))
#define __nds__radd16(a, b) \
  (__builtin_riscv_radd16 ((a), (b)))
#define __nds__uradd16(a, b) \
  (__builtin_riscv_uradd16 ((a), (b)))
#define __nds__kadd16(a, b) \
  (__builtin_riscv_kadd16 ((a), (b)))
#define __nds__ukadd16(a, b) \
  (__builtin_riscv_ukadd16 ((a), (b)))
#define __nds__sub16(a, b) \
  (__builtin_riscv_sub16 ((a), (b)))
#define __nds__rsub16(a, b) \
  (__builtin_riscv_rsub16 ((a), (b)))
#define __nds__ursub16(a, b) \
  (__builtin_riscv_ursub16 ((a), (b)))
#define __nds__ksub16(a, b) \
  (__builtin_riscv_ksub16 ((a), (b)))
#define __nds__uksub16(a, b) \
  (__builtin_riscv_uksub16 ((a), (b)))

#define __nds__cras16(a, b) \
  (__builtin_riscv_cras16 ((a), (b)))
#define __nds__rcras16(a, b) \
  (__builtin_riscv_rcras16 ((a), (b)))
#define __nds__urcras16(a, b) \
  (__builtin_riscv_urcras16 ((a), (b)))
#define __nds__kcras16(a, b) \
  (__builtin_riscv_kcras16 ((a), (b)))
#define __nds__ukcras16(a, b) \
  (__builtin_riscv_ukcras16 ((a), (b)))
#define __nds__crsa16(a, b) \
  (__builtin_riscv_crsa16 ((a), (b)))
#define __nds__rcrsa16(a, b) \
  (__builtin_riscv_rcrsa16 ((a), (b)))
#define __nds__urcrsa16(a, b) \
  (__builtin_riscv_urcrsa16 ((a), (b)))
#define __nds__kcrsa16(a, b) \
  (__builtin_riscv_kcrsa16 ((a), (b)))
#define __nds__ukcrsa16(a, b) \
  (__builtin_riscv_ukcrsa16 ((a), (b)))
#define __nds__stas16(a, b) \
  (__builtin_riscv_stas16 ((a), (b)))
#define __nds__rstas16(a, b) \
  (__builtin_riscv_rstas16 ((a), (b)))
#define __nds__urstas16(a, b) \
  (__builtin_riscv_urstas16 ((a), (b)))
#define __nds__kstas16(a, b) \
  (__builtin_riscv_kstas16 ((a), (b)))
#define __nds__ukstas16(a, b) \
  (__builtin_riscv_ukstas16 ((a), (b)))
#define __nds__stsa16(a, b) \
  (__builtin_riscv_stsa16 ((a), (b)))
#define __nds__rstsa16(a, b) \
  (__builtin_riscv_rstsa16 ((a), (b)))
#define __nds__urstsa16(a, b) \
  (__builtin_riscv_urstsa16 ((a), (b)))
#define __nds__kstsa16(a, b) \
  (__builtin_riscv_kstsa16 ((a), (b)))
#define __nds__ukstsa16(a, b) \
  (__builtin_riscv_ukstsa16 ((a), (b)))
#define __nds__add8(a, b) \
  (__builtin_riscv_add8 ((a), (b)))
#define __nds__radd8(a, b) \
  (__builtin_riscv_radd8 ((a), (b)))
#define __nds__uradd8(a, b) \
  (__builtin_riscv_uradd8 ((a), (b)))
#define __nds__kadd8(a, b) \
  (__builtin_riscv_kadd8 ((a), (b)))
#define __nds__ukadd8(a, b) \
  (__builtin_riscv_ukadd8 ((a), (b)))
#define __nds__sub8(a, b) \
  (__builtin_riscv_sub8 ((a), (b)))
#define __nds__rsub8(a, b) \
  (__builtin_riscv_rsub8 ((a), (b)))
#define __nds__ursub8(a, b) \
  (__builtin_riscv_ursub8 ((a), (b)))
#define __nds__ksub8(a, b) \
  (__builtin_riscv_ksub8 ((a), (b)))
#define __nds__uksub8(a, b) \
  (__builtin_riscv_uksub8 ((a), (b)))

#define __nds__sra16(a, b) \
  (__builtin_riscv_sra16 ((a), (b)))
#define __nds__sra16_u(a, b) \
  (__builtin_riscv_sra16_u ((a), (b)))
#define __nds__srl16(a, b) \
  (__builtin_riscv_srl16 ((a), (b)))
#define __nds__srl16_u(a, b) \
  (__builtin_riscv_srl16_u ((a), (b)))
#define __nds__sll16(a, b) \
  (__builtin_riscv_sll16 ((a), (b)))
#define __nds__ksll16(a, b) \
  (__builtin_riscv_ksll16 ((a), (b)))
#define __nds__kslra16(a, b) \
  (__builtin_riscv_kslra16 ((a), (b)))
#define __nds__kslra16_u(a, b) \
  (__builtin_riscv_kslra16_u ((a), (b)))

#define __nds__cmpeq16(a, b) \
  (__builtin_riscv_cmpeq16 ((a), (b)))
#define __nds__scmplt16(a, b) \
  (__builtin_riscv_scmplt16 ((a), (b)))
#define __nds__scmple16(a, b) \
  (__builtin_riscv_scmple16 ((a), (b)))
#define __nds__ucmplt16(a, b) \
  (__builtin_riscv_ucmplt16 ((a), (b)))
#define __nds__ucmple16(a, b) \
  (__builtin_riscv_ucmple16 ((a), (b)))

#define __nds__cmpeq8(a, b) \
  (__builtin_riscv_cmpeq8 ((a), (b)))
#define __nds__scmplt8(a, b) \
  (__builtin_riscv_scmplt8 ((a), (b)))
#define __nds__scmple8(a, b) \
  (__builtin_riscv_scmple8 ((a), (b)))
#define __nds__ucmplt8(a, b) \
  (__builtin_riscv_ucmplt8 ((a), (b)))
#define __nds__ucmple8(a, b) \
  (__builtin_riscv_ucmple8 ((a), (b)))

#define __nds__smin16(a, b) \
  (__builtin_riscv_smin16 ((a), (b)))
#define __nds__umin16(a, b) \
  (__builtin_riscv_umin16 ((a), (b)))
#define __nds__smax16(a, b) \
  (__builtin_riscv_smax16 ((a), (b)))
#define __nds__umax16(a, b) \
  (__builtin_riscv_umax16 ((a), (b)))
#define __nds__sclip16(a, b) \
  (__builtin_riscv_sclip16 ((a), (b)))
#define __nds__uclip16(a, b) \
  (__builtin_riscv_uclip16 ((a), (b)))
#define __nds__khm16(a, b) \
  (__builtin_riscv_khm16 ((a), (b)))
#define __nds__khmx16(a, b) \
  (__builtin_riscv_khmx16 ((a), (b)))
#define __nds__khm8(a, b) \
  (__builtin_riscv_khm8 ((a), (b)))
#define __nds__khmx8(a, b) \
  (__builtin_riscv_khmx8 ((a), (b)))
#define __nds__kabs16(a) \
  (__builtin_riscv_kabs16 ((a)))
#define __nds__smin8(a, b) \
  (__builtin_riscv_smin8 ((a), (b)))
#define __nds__umin8(a, b) \
  (__builtin_riscv_umin8 ((a), (b)))
#define __nds__smax8(a, b) \
  (__builtin_riscv_smax8 ((a), (b)))
#define __nds__umax8(a, b) \
  (__builtin_riscv_umax8 ((a), (b)))
#define __nds__kabs8(a) \
  (__builtin_riscv_kabs8 ((a)))

#define __nds__sunpkd810(a) \
  (__builtin_riscv_sunpkd810 ((a)))
#define __nds__sunpkd820(a) \
  (__builtin_riscv_sunpkd820 ((a)))
#define __nds__sunpkd830(a) \
  (__builtin_riscv_sunpkd830 ((a)))
#define __nds__sunpkd831(a) \
  (__builtin_riscv_sunpkd831 ((a)))
#define __nds__sunpkd832(a) \
  (__builtin_riscv_sunpkd832 ((a)))

#define __nds__zunpkd810(a) \
  (__builtin_riscv_zunpkd810 ((a)))
#define __nds__zunpkd820(a) \
  (__builtin_riscv_zunpkd820 ((a)))
#define __nds__zunpkd830(a) \
  (__builtin_riscv_zunpkd830 ((a)))
#define __nds__zunpkd831(a) \
  (__builtin_riscv_zunpkd831 ((a)))
#define __nds__zunpkd832(a) \
  (__builtin_riscv_zunpkd832 ((a)))

#define __nds__raddw(a, b) \
  (__builtin_riscv_raddw ((a), (b)))
#define __nds__uraddw(a, b) \
  (__builtin_riscv_uraddw ((a), (b)))
#define __nds__rsubw(a, b) \
  (__builtin_riscv_rsubw ((a), (b)))
#define __nds__ursubw(a, b) \
  (__builtin_riscv_ursubw ((a), (b)))

#define __nds__sra_u(a, b) \
  (__builtin_riscv_sra_u ((a), (b)))
#define __nds__ksllw(a, b) \
  (__builtin_riscv_ksllw ((a), (b)))
#define __nds__pkbb16(a, b) \
  (__builtin_riscv_pkbb16 ((a), (b)))
#define __nds__pkbt16(a, b) \
  (__builtin_riscv_pkbt16 ((a), (b)))
#define __nds__pktb16(a, b) \
  (__builtin_riscv_pktb16 ((a), (b)))
#define __nds__pktt16(a, b) \
  (__builtin_riscv_pktt16 ((a), (b)))
#define __nds__pkbb32(a, b) \
  (__builtin_riscv_pkbb32 ((a), (b)))
#define __nds__pkbt32(a, b) \
  (__builtin_riscv_pkbt32 ((a), (b)))
#define __nds__pktb32(a, b) \
  (__builtin_riscv_pktb32 ((a), (b)))
#define __nds__pktt32(a, b) \
  (__builtin_riscv_pktt32 ((a), (b)))

#define __nds__smmul(a, b) \
  (__builtin_riscv_smmul ((a), (b)))
#define __nds__smmul_u(a, b) \
  (__builtin_riscv_smmul_u ((a), (b)))
#define __nds__kmmac(r, a, b) \
  (__builtin_riscv_kmmac ((r), (a), (b)))
#define __nds__kmmac_u(r, a, b) \
  (__builtin_riscv_kmmac_u ((r), (a), (b)))
#define __nds__kmmsb(r, a, b) \
  (__builtin_riscv_kmmsb ((r), (a), (b)))
#define __nds__kmmsb_u(r, a, b) \
  (__builtin_riscv_kmmsb_u ((r), (a), (b)))
#define __nds__kwmmul(a, b) \
  (__builtin_riscv_kwmmul ((a), (b)))
#define __nds__kwmmul_u(a, b) \
  (__builtin_riscv_kwmmul_u ((a), (b)))

#define __nds__smmwb(a, b) \
  (__builtin_riscv_smmwb ((a), (b)))
#define __nds__smmwb_u(a, b) \
  (__builtin_riscv_smmwb_u ((a), (b)))
#define __nds__smmwt(a, b) \
  (__builtin_riscv_smmwt ((a), (b)))
#define __nds__smmwt_u(a, b) \
  (__builtin_riscv_smmwt_u ((a), (b)))
#define __nds__kmmwb2(a, b) \
  (__builtin_riscv_kmmwb2 ((a), (b)))
#define __nds__kmmwb2_u(a, b) \
  (__builtin_riscv_kmmwb2_u ((a), (b)))
#define __nds__kmmwt2(a, b) \
  (__builtin_riscv_kmmwt2 ((a), (b)))
#define __nds__kmmwt2_u(a, b) \
  (__builtin_riscv_kmmwt2_u ((a), (b)))
#define __nds__kmmawb(r, a, b) \
  (__builtin_riscv_kmmawb ((r), (a), (b)))
#define __nds__kmmawb_u(r, a, b) \
  (__builtin_riscv_kmmawb_u ((r), (a), (b)))
#define __nds__kmmawt(r, a, b) \
  (__builtin_riscv_kmmawt ((r), (a), (b)))
#define __nds__kmmawt_u(r, a, b) \
  (__builtin_riscv_kmmawt_u ((r), (a), (b)))
#define __nds__kmmawb2(r, a, b) \
  (__builtin_riscv_kmmawb2 ((r), (a), (b)))
#define __nds__kmmawb2_u(r, a, b) \
  (__builtin_riscv_kmmawb2_u ((r), (a), (b)))
#define __nds__kmmawt2(r, a, b) \
  (__builtin_riscv_kmmawt2 ((r), (a), (b)))
#define __nds__kmmawt2_u(r, a, b) \
  (__builtin_riscv_kmmawt2_u ((r), (a), (b)))
#define __nds__smbb16(a, b) \
  (__builtin_riscv_smbb16 ((a), (b)))
#define __nds__smbt16(a, b) \
  (__builtin_riscv_smbt16 ((a), (b)))
#define __nds__smtt16(a, b) \
  (__builtin_riscv_smtt16 ((a), (b)))
#define __nds__kmda(a, b) \
  (__builtin_riscv_kmda ((a), (b)))
#define __nds__kmxda(a, b) \
  (__builtin_riscv_kmxda ((a), (b)))
#define __nds__smds(a, b) \
  (__builtin_riscv_smds ((a), (b)))
#define __nds__smdrs(a, b) \
  (__builtin_riscv_smdrs ((a), (b)))
#define __nds__smxds(a, b) \
  (__builtin_riscv_smxds ((a), (b)))
#define __nds__kmabb(r, a, b) \
  (__builtin_riscv_kmabb ((r), (a), (b)))
#define __nds__kmabt(r, a, b) \
  (__builtin_riscv_kmabt ((r), (a), (b)))
#define __nds__kmatt(r, a, b) \
  (__builtin_riscv_kmatt ((r), (a), (b)))
#define __nds__kmada(r, a, b) \
  (__builtin_riscv_kmada ((r), (a), (b)))
#define __nds__kmaxda(r, a, b) \
  (__builtin_riscv_kmaxda ((r), (a), (b)))
#define __nds__kmads(r, a, b) \
  (__builtin_riscv_kmads ((r), (a), (b)))
#define __nds__kmadrs(r, a, b) \
  (__builtin_riscv_kmadrs ((r), (a), (b)))
#define __nds__kmaxds(r, a, b) \
  (__builtin_riscv_kmaxds ((r), (a), (b)))
#define __nds__kmsda(r, a, b) \
  (__builtin_riscv_kmsda ((r), (a), (b)))
#define __nds__kmsxda(r, a, b) \
  (__builtin_riscv_kmsxda ((r), (a), (b)))
#define __nds__smal(a, b) \
  (__builtin_riscv_smal ((a), (b)))

#define __nds__bitrev(a, b) \
  (__builtin_riscv_bitrev ((a), (b)))
#define __nds__wext(a, b) \
  (__builtin_riscv_wext ((a), (b)))
#define __nds__bpick(r, a, b) \
  (__builtin_riscv_bpick ((r), (a), (b)))
#define __nds__insb(r, a, b) \
  (__builtin_riscv_insb ((r), (a), (b)))

#define __nds__sadd64(a, b) \
  (__builtin_riscv_sadd64 ((a), (b)))
#define __nds__uadd64(a, b) \
  (__builtin_riscv_uadd64 ((a), (b)))
#define __nds__radd64(a, b) \
  (__builtin_riscv_radd64 ((a), (b)))
#define __nds__uradd64(a, b) \
  (__builtin_riscv_uradd64 ((a), (b)))
#define __nds__kadd64(a, b) \
  (__builtin_riscv_kadd64 ((a), (b)))
#define __nds__ukadd64(a, b) \
  (__builtin_riscv_ukadd64 ((a), (b)))
#define __nds__ssub64(a, b) \
  (__builtin_riscv_ssub64 ((a), (b)))
#define __nds__usub64(a, b) \
  (__builtin_riscv_usub64 ((a), (b)))
#define __nds__rsub64(a, b) \
  (__builtin_riscv_rsub64 ((a), (b)))
#define __nds__ursub64(a, b) \
  (__builtin_riscv_ursub64 ((a), (b)))
#define __nds__ksub64(a, b) \
  (__builtin_riscv_ksub64 ((a), (b)))
#define __nds__uksub64(a, b) \
  (__builtin_riscv_uksub64 ((a), (b)))

#define __nds__smalbb(r, a, b) \
  (__builtin_riscv_smalbb ((r), (a), (b)))
#define __nds__smalbt(r, a, b) \
  (__builtin_riscv_smalbt ((r), (a), (b)))
#define __nds__smaltt(r, a, b) \
  (__builtin_riscv_smaltt ((r), (a), (b)))

#define __nds__smalda(r, a, b) \
  (__builtin_riscv_smalda ((r), (a), (b)))
#define __nds__smalxda(r, a, b) \
  (__builtin_riscv_smalxda ((r), (a), (b)))
#define __nds__smalds(r, a, b) \
  (__builtin_riscv_smalds ((r), (a), (b)))
#define __nds__smaldrs(r, a, b) \
  (__builtin_riscv_smaldrs ((r), (a), (b)))
#define __nds__smalxds(r, a, b) \
  (__builtin_riscv_smalxds ((r), (a), (b)))
#define __nds__smslda(r, a, b) \
  (__builtin_riscv_smslda ((r), (a), (b)))
#define __nds__smslxda(r, a, b) \
  (__builtin_riscv_smslxda ((r), (a), (b)))

#define __nds__uclip32(a, imm) \
  (__builtin_riscv_uclip32 ((a), (imm)))
#define __nds__sclip32(a, imm) \
  (__builtin_riscv_sclip32 ((a), (imm)))
#define __nds__kabsw(a) \
  (__builtin_riscv_kabsw ((a)))

#define __nds__kaddw(a, b) \
  (__builtin_riscv_kaddw ((a), (b)))
#define __nds__kaddh(a, b) \
  (__builtin_riscv_kaddh ((a), (b)))
#define __nds__ksubw(a, b) \
  (__builtin_riscv_ksubw ((a), (b)))
#define __nds__ksubh(a, b) \
  (__builtin_riscv_ksubh ((a), (b)))
#define __nds__ukaddw(a, b) \
  (__builtin_riscv_ukaddw ((a), (b)))
#define __nds__ukaddh(a, b) \
  (__builtin_riscv_ukaddh ((a), (b)))
#define __nds__uksubw(a, b) \
  (__builtin_riscv_uksubw ((a), (b)))
#define __nds__uksubh(a, b) \
  (__builtin_riscv_uksubh ((a), (b)))
#define __nds__kdmbb(a, b) \
  (__builtin_riscv_kdmbb ((a), (b)))
#define __nds__kdmbt(a, b) \
  (__builtin_riscv_kdmbt ((a), (b)))
#define __nds__kdmtt(a, b) \
  (__builtin_riscv_kdmtt ((a), (b)))
#define __nds__khmbb(a, b) \
  (__builtin_riscv_khmbb ((a), (b)))
#define __nds__khmbt(a, b) \
  (__builtin_riscv_khmbt ((a), (b)))
#define __nds__khmtt(a, b) \
  (__builtin_riscv_khmtt ((a), (b)))
#define __nds__kslraw(a, b) \
  (__builtin_riscv_kslraw ((a), (b)))
#define __nds__kslraw_u(a, b) \
  (__builtin_riscv_kslraw_u ((a), (b)))
#define __nds__ave(a, b) \
  (__builtin_riscv_ave ((a), (b)))
#define __nds__maxw(a, b) \
  (__builtin_riscv_maxw ((a), (b)))
#define __nds__minw(a, b) \
  (__builtin_riscv_minw ((a), (b)))

#define __nds__sra8(a, b) \
  (__builtin_riscv_sra8 ((a), (b)))
#define __nds__sra8_u(a, b) \
  (__builtin_riscv_sra8_u ((a), (b)))
#define __nds__srl8(a, b) \
  (__builtin_riscv_srl8 ((a), (b)))
#define __nds__srl8_u(a, b) \
  (__builtin_riscv_srl8_u ((a), (b)))
#define __nds__sll8(a, b) \
  (__builtin_riscv_sll8 ((a), (b)))
#define __nds__ksll8(a, b) \
  (__builtin_riscv_ksll8 ((a), (b)))
#define __nds__kslra8(a, b) \
  (__builtin_riscv_kslra8 ((a), (b)))
#define __nds__kslra8_u(a, b) \
  (__builtin_riscv_kslra8_u ((a), (b)))

#define __nds__pbsad(a, b) \
  (__builtin_riscv_pbsad ((a), (b)))
#define __nds__pbsada(acc, a, b) \
  (__builtin_riscv_pbsada ((acc), (a), (b)))
#define __nds__swap8(a) \
  (__builtin_riscv_swap8 ((a)))
#define __nds__swap16(a) \
  (__builtin_riscv_swap16 ((a)))
#define __nds__sclip8(a, b) \
  (__builtin_riscv_sclip8 ((a), (b)))
#define __nds__uclip8(a, b) \
  (__builtin_riscv_uclip8 ((a), (b)))

#define __nds__ffb(a, b) \
  (__builtin_riscv_ffb ((a), (b)))
#define __nds__ffmism(a, b) \
  (__builtin_riscv_ffmism ((a), (b)))
#define __nds__flmism(a, b) \
  (__builtin_riscv_flmism ((a), (b)))

#define __nds__sraw_u(a, b) \
  (__builtin_riscv_sraw_u ((a), (b)))
#define __nds__add32(a, b) \
  (__builtin_riscv_add32 ((a), (b)))
#define __nds__radd32(a, b) \
  (__builtin_riscv_radd32 ((a), (b)))
#define __nds__uradd32(a, b) \
  (__builtin_riscv_uradd32 ((a), (b)))
#define __nds__kadd32(a, b) \
  (__builtin_riscv_kadd32 ((a), (b)))
#define __nds__ukadd32(a, b) \
  (__builtin_riscv_ukadd32 ((a), (b)))
#define __nds__sub32(a, b) \
  (__builtin_riscv_sub32 ((a), (b)))
#define __nds__rsub32(a, b) \
  (__builtin_riscv_rsub32 ((a), (b)))
#define __nds__ursub32(a, b) \
  (__builtin_riscv_ursub32 ((a), (b)))
#define __nds__ksub32(a, b) \
  (__builtin_riscv_ksub32 ((a), (b)))
#define __nds__uksub32(a, b) \
  (__builtin_riscv_uksub32 ((a), (b)))
#define __nds__cras32(a, b) \
  (__builtin_riscv_cras32 ((a), (b)))
#define __nds__crsa32(a, b) \
  (__builtin_riscv_crsa32 ((a), (b)))
#define __nds__rcras32(a, b) \
  (__builtin_riscv_rcras32 ((a), (b)))
#define __nds__rcrsa32(a, b) \
  (__builtin_riscv_rcrsa32 ((a), (b)))
#define __nds__urcras32(a, b) \
  (__builtin_riscv_urcras32 ((a), (b)))
#define __nds__urcrsa32(a, b) \
  (__builtin_riscv_urcrsa32 ((a), (b)))
#define __nds__kcras32(a, b) \
  (__builtin_riscv_kcras32 ((a), (b)))
#define __nds__kcrsa32(a, b) \
  (__builtin_riscv_kcrsa32 ((a), (b)))
#define __nds__ukcras32(a, b) \
  (__builtin_riscv_ukcras32 ((a), (b)))
#define __nds__ukcrsa32(a, b) \
  (__builtin_riscv_ukcrsa32 ((a), (b)))
#define __nds__stas32(a, b) \
  (__builtin_riscv_stas32 ((a), (b)))
#define __nds__stsa32(a, b) \
  (__builtin_riscv_stsa32 ((a), (b)))
#define __nds__rstas32(a, b) \
  (__builtin_riscv_rstas32 ((a), (b)))
#define __nds__rstsa32(a, b) \
  (__builtin_riscv_rstsa32 ((a), (b)))
#define __nds__urstas32(a, b) \
  (__builtin_riscv_urstas32 ((a), (b)))
#define __nds__urstsa32(a, b) \
  (__builtin_riscv_urstsa32 ((a), (b)))
#define __nds__kstas32(a, b) \
  (__builtin_riscv_kstas32 ((a), (b)))
#define __nds__kstsa32(a, b) \
  (__builtin_riscv_kstsa32 ((a), (b)))
#define __nds__ukstas32(a, b) \
  (__builtin_riscv_ukstas32 ((a), (b)))
#define __nds__ukstsa32(a, b) \
  (__builtin_riscv_ukstsa32 ((a), (b)))
#define __nds__sra32(a, b) \
  (__builtin_riscv_sra32 ((a), (b)))
#define __nds__sra32_u(a, b) \
  (__builtin_riscv_sra32_u ((a), (b)))
#define __nds__srl32(a, b) \
  (__builtin_riscv_srl32 ((a), (b)))
#define __nds__srl32_u(a, b) \
  (__builtin_riscv_srl32_u ((a), (b)))
#define __nds__sll32(a, b) \
  (__builtin_riscv_sll32 ((a), (b)))
#define __nds__ksll32(a, b) \
  (__builtin_riscv_ksll32 ((a), (b)))
#define __nds__kslra32(a, b) \
  (__builtin_riscv_kslra32 ((a), (b)))
#define __nds__kslra32_u(a, b) \
  (__builtin_riscv_kslra32_u ((a), (b)))
#define __nds__smin32(a, b) \
  (__builtin_riscv_smin32 ((a), (b)))
#define __nds__umin32(a, b) \
  (__builtin_riscv_umin32 ((a), (b)))
#define __nds__smax32(a, b) \
  (__builtin_riscv_smax32 ((a), (b)))
#define __nds__umax32(a, b) \
  (__builtin_riscv_umax32 ((a), (b)))
#define __nds__kabs32(a) \
  (__builtin_riscv_kabs32 ((a)))
#define __nds__khmbb16(a, b) \
  (__builtin_riscv_khmbb16 ((a), (b)))
#define __nds__khmbt16(a, b) \
  (__builtin_riscv_khmbt16 ((a), (b)))
#define __nds__khmtt16(a, b) \
  (__builtin_riscv_khmtt16 ((a), (b)))
#define __nds__kdmbb16(a, b) \
  (__builtin_riscv_kdmbb16 ((a), (b)))
#define __nds__kdmbt16(a, b) \
  (__builtin_riscv_kdmbt16 ((a), (b)))
#define __nds__kdmtt16(a, b) \
  (__builtin_riscv_kdmtt16 ((a), (b)))
#define __nds__smbb32(a, b) \
  (__builtin_riscv_smbb32 ((a), (b)))
#define __nds__smbt32(a, b) \
  (__builtin_riscv_smbt32 ((a), (b)))
#define __nds__smtt32(a, b) \
  (__builtin_riscv_smtt32 ((a), (b)))
#define __nds__kmabb32(r, a, b) \
  (__builtin_riscv_kmabb32 ((r), (a), (b)))
#define __nds__kmabt32(r, a, b) \
  (__builtin_riscv_kmabt32 ((r), (a), (b)))
#define __nds__kmatt32(r, a, b) \
  (__builtin_riscv_kmatt32 ((r), (a), (b)))

#define __nds__kmda32(a, b) \
  (__builtin_riscv_kmda32 ((a), (b)))
#define __nds__kmxda32(a, b) \
  (__builtin_riscv_kmxda32 ((a), (b)))
#define __nds__kmada32(r, a, b) \
  (__builtin_riscv_kmada32 ((r), (a), (b)))
#define __nds__kmaxda32(r, a, b) \
  (__builtin_riscv_kmaxda32 ((r), (a), (b)))
#define __nds__kmads32(r, a, b) \
  (__builtin_riscv_kmads32 ((r), (a), (b)))
#define __nds__kmadrs32(r, a, b) \
  (__builtin_riscv_kmadrs32 ((r), (a), (b)))
#define __nds__kmaxds32(r, a, b) \
  (__builtin_riscv_kmaxds32 ((r), (a), (b)))
#define __nds__kmsda32(r, a, b) \
  (__builtin_riscv_kmsda32 ((r), (a), (b)))
#define __nds__kmsxda32(r, a, b) \
  (__builtin_riscv_kmsxda32 ((r), (a), (b)))
#define __nds__smds32(a, b) \
  (__builtin_riscv_smds32 ((a), (b)))
#define __nds__smdrs32(a, b) \
  (__builtin_riscv_smdrs32 ((a), (b)))
#define __nds__smxds32(a, b) \
  (__builtin_riscv_smxds32 ((a), (b)))
#define __nds__rdov() \
  (__builtin_riscv_csrr (0x801))
#define __nds__clrov() \
  (__builtin_riscv_csrc (1, 0x801))

#define __nds__kdmabb16(r, a, b) \
  (__builtin_riscv_kdmabb16 ((r), (a), (b)))
#define __nds__kdmabt16(r, a, b) \
  (__builtin_riscv_kdmabt16 ((r), (a), (b)))
#define __nds__kdmatt16(r, a, b) \
  (__builtin_riscv_kdmatt16 ((r), (a), (b)))
#define __nds__kdmabb(r, a, b) \
  (__builtin_riscv_kdmabb ((r), (a), (b)))
#define __nds__kdmabt(r, a, b) \
  (__builtin_riscv_kdmabt ((r), (a), (b)))
#define __nds__kdmatt(r, a, b) \
  (__builtin_riscv_kdmatt ((r), (a), (b)))

#define __nds__smaqa(r, a, b) \
  (__builtin_riscv_smaqa ((r), (a), (b)))
#define __nds__umaqa(r, a, b) \
  (__builtin_riscv_umaqa ((r), (a), (b)))
#define __nds__smaqa_su(r, a, b) \
  (__builtin_riscv_smaqa_su ((r), (a), (b)))

#define __nds__clrs8(a) \
  (__builtin_riscv_clrs8 ((a)))
#define __nds__clrs16(a) \
  (__builtin_riscv_clrs16 ((a)))
#define __nds__clrs32(a) \
  (__builtin_riscv_clrs32 ((a)))
#define __nds__clo8(a) \
  (__builtin_riscv_clo8 ((a)))
#define __nds__clo16(a) \
  (__builtin_riscv_clo16 ((a)))
#define __nds__clo32(a) \
  (__builtin_riscv_clo32 ((a)))
#define __nds__clz8(a) \
  (__builtin_riscv_clz8 ((a)))
#define __nds__clz16(a) \
  (__builtin_riscv_clz16 ((a)))
#define __nds__clz32(a) \
  (__builtin_riscv_clz32 ((a)))
#define __nds__smul16(a, b) \
  (__builtin_riscv_smul16 ((a), (b)))
#define __nds__umul16(a, b) \
  (__builtin_riscv_umul16 ((a), (b)))
#define __nds__smul8(a, b) \
  (__builtin_riscv_smul8 ((a), (b)))
#define __nds__umul8(a, b) \
  (__builtin_riscv_umul8 ((a), (b)))
#define __nds__smulx16(a, b) \
  (__builtin_riscv_smulx16 ((a), (b)))
#define __nds__smulx8(a, b) \
  (__builtin_riscv_smulx8 ((a), (b)))
#define __nds__umulx16(a, b) \
  (__builtin_riscv_umulx16 ((a), (b)))
#define __nds__umulx8(a, b) \
  (__builtin_riscv_umulx8 ((a), (b)))
#define __nds__smar64(r, a, b) \
  (__builtin_riscv_smar64 ((r), (a), (b)))
#define __nds__smsr64(r, a, b) \
  (__builtin_riscv_smsr64 ((r), (a), (b)))
#define __nds__umar64(r, a, b) \
  (__builtin_riscv_umar64 ((r), (a), (b)))
#define __nds__umsr64(r, a, b) \
  (__builtin_riscv_umsr64 ((r), (a), (b)))
#define __nds__kmar64(r, a, b) \
  (__builtin_riscv_kmar64 ((r), (a), (b)))
#define __nds__kmsr64(r, a, b) \
  (__builtin_riscv_kmsr64 ((r), (a), (b)))
#define __nds__ukmar64(r, a, b) \
  (__builtin_riscv_ukmar64 ((r), (a), (b)))
#define __nds__ukmsr64(r, a, b) \
  (__builtin_riscv_ukmsr64 ((r), (a), (b)))

#if __riscv_xlen == 32
#define __nds__v_uadd16(a, b) \
  (__builtin_riscv_v_uadd16 ((a), (b)))
#define __nds__v_sadd16(a, b) \
  (__builtin_riscv_v_sadd16 ((a), (b)))
#define __nds__v_radd16(a, b) \
  (__builtin_riscv_v_radd16 ((a), (b)))
#define __nds__v_uradd16(a, b) \
  (__builtin_riscv_v_uradd16 ((a), (b)))
#define __nds__v_kadd16(a, b) \
  (__builtin_riscv_v_kadd16 ((a), (b)))
#define __nds__v_ukadd16(a, b) \
  (__builtin_riscv_v_ukadd16 ((a), (b)))
#define __nds__v_usub16(a, b) \
  (__builtin_riscv_v_usub16 ((a), (b)))
#define __nds__v_ssub16(a, b) \
  (__builtin_riscv_v_ssub16 ((a), (b)))
#define __nds__v_rsub16(a, b) \
  (__builtin_riscv_v_rsub16 ((a), (b)))
#define __nds__v_ursub16(a, b) \
  (__builtin_riscv_v_ursub16 ((a), (b)))
#define __nds__v_ksub16(a, b) \
  (__builtin_riscv_v_ksub16 ((a), (b)))
#define __nds__v_uksub16(a, b) \
  (__builtin_riscv_v_uksub16 ((a), (b)))
#define __nds__v_ucras16(a, b) \
  (__builtin_riscv_v_ucras16 ((a), (b)))
#define __nds__v_scras16(a, b) \
  (__builtin_riscv_v_scras16 ((a), (b)))
#define __nds__v_rcras16(a, b) \
  (__builtin_riscv_v_rcras16 ((a), (b)))
#define __nds__v_urcras16(a, b) \
  (__builtin_riscv_v_urcras16 ((a), (b)))
#define __nds__v_kcras16(a, b) \
  (__builtin_riscv_v_kcras16 ((a), (b)))
#define __nds__v_ukcras16(a, b) \
  (__builtin_riscv_v_ukcras16 ((a), (b)))
#define __nds__v_ucrsa16(a, b) \
  (__builtin_riscv_v_ucrsa16 ((a), (b)))
#define __nds__v_scrsa16(a, b) \
  (__builtin_riscv_v_scrsa16 ((a), (b)))
#define __nds__v_rcrsa16(a, b) \
  (__builtin_riscv_v_rcrsa16 ((a), (b)))
#define __nds__v_urcrsa16(a, b) \
  (__builtin_riscv_v_urcrsa16 ((a), (b)))
#define __nds__v_kcrsa16(a, b) \
  (__builtin_riscv_v_kcrsa16 ((a), (b)))
#define __nds__v_ukcrsa16(a, b) \
  (__builtin_riscv_v_ukcrsa16 ((a), (b)))
#define __nds__v_ustas16(a, b) \
  (__builtin_riscv_v_ustas16 ((a), (b)))
#define __nds__v_sstas16(a, b) \
  (__builtin_riscv_v_sstas16 ((a), (b)))
#define __nds__v_rstas16(a, b) \
  (__builtin_riscv_v_rstas16 ((a), (b)))
#define __nds__v_urstas16(a, b) \
  (__builtin_riscv_v_urstas16 ((a), (b)))
#define __nds__v_kstas16(a, b) \
  (__builtin_riscv_v_kstas16 ((a), (b)))
#define __nds__v_ukstas16(a, b) \
  (__builtin_riscv_v_ukstas16 ((a), (b)))
#define __nds__v_ustsa16(a, b) \
  (__builtin_riscv_v_ustsa16 ((a), (b)))
#define __nds__v_sstsa16(a, b) \
  (__builtin_riscv_v_sstsa16 ((a), (b)))
#define __nds__v_rstsa16(a, b) \
  (__builtin_riscv_v_rstsa16 ((a), (b)))
#define __nds__v_urstsa16(a, b) \
  (__builtin_riscv_v_urstsa16 ((a), (b)))
#define __nds__v_kstsa16(a, b) \
  (__builtin_riscv_v_kstsa16 ((a), (b)))
#define __nds__v_ukstsa16(a, b) \
  (__builtin_riscv_v_ukstsa16 ((a), (b)))
#define __nds__v_uadd8(a, b) \
  (__builtin_riscv_v_uadd8 ((a), (b)))
#define __nds__v_sadd8(a, b) \
  (__builtin_riscv_v_sadd8 ((a), (b)))
#define __nds__v_radd8(a, b) \
  (__builtin_riscv_v_radd8 ((a), (b)))
#define __nds__v_uradd8(a, b) \
  (__builtin_riscv_v_uradd8 ((a), (b)))
#define __nds__v_kadd8(a, b) \
  (__builtin_riscv_v_kadd8 ((a), (b)))
#define __nds__v_ukadd8(a, b) \
  (__builtin_riscv_v_ukadd8 ((a), (b)))
#define __nds__v_usub8(a, b) \
  (__builtin_riscv_v_usub8 ((a), (b)))
#define __nds__v_ssub8(a, b) \
  (__builtin_riscv_v_ssub8 ((a), (b)))
#define __nds__v_rsub8(a, b) \
  (__builtin_riscv_v_rsub8 ((a), (b)))
#define __nds__v_ursub8(a, b) \
  (__builtin_riscv_v_ursub8 ((a), (b)))
#define __nds__v_ksub8(a, b) \
  (__builtin_riscv_v_ksub8 ((a), (b)))
#define __nds__v_uksub8(a, b) \
  (__builtin_riscv_v_uksub8 ((a), (b)))
#define __nds__v_sra16(a, b) \
  (__builtin_riscv_v_sra16 ((a), (b)))
#define __nds__v_sra16_u(a, b) \
  (__builtin_riscv_v_sra16_u ((a), (b)))
#define __nds__v_srl16(a, b) \
  (__builtin_riscv_v_srl16 ((a), (b)))
#define __nds__v_srl16_u(a, b) \
  (__builtin_riscv_v_srl16_u ((a), (b)))
#define __nds__v_sll16(a, b) \
  (__builtin_riscv_v_sll16 ((a), (b)))
#define __nds__v_ksll16(a, b) \
  (__builtin_riscv_v_ksll16 ((a), (b)))
#define __nds__v_kslra16(a, b) \
  (__builtin_riscv_v_kslra16 ((a), (b)))
#define __nds__v_kslra16_u(a, b) \
  (__builtin_riscv_v_kslra16_u ((a), (b)))
#define __nds__v_scmpeq16(a, b) \
  (__builtin_riscv_v_scmpeq16 ((a), (b)))
#define __nds__v_ucmpeq16(a, b) \
  (__builtin_riscv_v_ucmpeq16 ((a), (b)))
#define __nds__v_scmplt16(a, b) \
  (__builtin_riscv_v_scmplt16 ((a), (b)))
#define __nds__v_scmple16(a, b) \
  (__builtin_riscv_v_scmple16 ((a), (b)))
#define __nds__v_ucmplt16(a, b) \
  (__builtin_riscv_v_ucmplt16 ((a), (b)))
#define __nds__v_ucmple16(a, b) \
  (__builtin_riscv_v_ucmple16 ((a), (b)))
#define __nds__v_scmpeq8(a, b) \
  (__builtin_riscv_v_scmpeq8 ((a), (b)))
#define __nds__v_ucmpeq8(a, b) \
  (__builtin_riscv_v_ucmpeq8 ((a), (b)))
#define __nds__v_scmplt8(a, b) \
  (__builtin_riscv_v_scmplt8 ((a), (b)))
#define __nds__v_scmple8(a, b) \
  (__builtin_riscv_v_scmple8 ((a), (b)))
#define __nds__v_ucmplt8(a, b) \
  (__builtin_riscv_v_ucmplt8 ((a), (b)))
#define __nds__v_ucmple8(a, b) \
  (__builtin_riscv_v_ucmple8 ((a), (b)))
#define __nds__v_smin16(a, b) \
  (__builtin_riscv_v_smin16 ((a), (b)))
#define __nds__v_umin16(a, b) \
  (__builtin_riscv_v_umin16 ((a), (b)))
#define __nds__v_smax16(a, b) \
  (__builtin_riscv_v_smax16 ((a), (b)))
#define __nds__v_umax16(a, b) \
  (__builtin_riscv_v_umax16 ((a), (b)))
#define __nds__v_sclip16(a, b) \
  (__builtin_riscv_v_sclip16 ((a), (b)))
#define __nds__v_uclip16(a, b) \
  (__builtin_riscv_v_uclip16 ((a), (b)))
#define __nds__v_khm16(a, b) \
  (__builtin_riscv_v_khm16 ((a), (b)))
#define __nds__v_khmx16(a, b) \
  (__builtin_riscv_v_khmx16 ((a), (b)))
#define __nds__v_khm8(a, b) \
  (__builtin_riscv_v_khm8 ((a), (b)))
#define __nds__v_khmx8(a, b) \
  (__builtin_riscv_v_khmx8 ((a), (b)))
#define __nds__v_kabs16(a) \
  (__builtin_riscv_v_kabs16 ((a)))
#define __nds__v_smul16(a, b) \
  (__builtin_riscv_v_smul16 ((a), (b)))
#define __nds__v_smulx16(a, b) \
  (__builtin_riscv_v_smulx16 ((a), (b)))
#define __nds__v_umul16(a, b) \
  (__builtin_riscv_v_umul16 ((a), (b)))
#define __nds__v_umulx16(a, b) \
  (__builtin_riscv_v_umulx16 ((a), (b)))
#define __nds__v_smul8(a, b) \
  (__builtin_riscv_v_smul8 ((a), (b)))
#define __nds__v_smulx8(a, b) \
  (__builtin_riscv_v_smulx8 ((a), (b)))
#define __nds__v_umul8(a, b) \
  (__builtin_riscv_v_umul8 ((a), (b)))
#define __nds__v_umulx8(a, b) \
  (__builtin_riscv_v_umulx8 ((a), (b)))
#define __nds__v_smin8(a, b) \
  (__builtin_riscv_v_smin8 ((a), (b)))
#define __nds__v_umin8(a, b) \
  (__builtin_riscv_v_umin8 ((a), (b)))
#define __nds__v_smax8(a, b) \
  (__builtin_riscv_v_smax8 ((a), (b)))
#define __nds__v_umax8(a, b) \
  (__builtin_riscv_v_umax8 ((a), (b)))
#define __nds__v_kabs8(a) \
  (__builtin_riscv_v_kabs8 ((a)))
#define __nds__v_sunpkd810(a) \
  (__builtin_riscv_v_sunpkd810 ((a)))
#define __nds__v_sunpkd820(a) \
  (__builtin_riscv_v_sunpkd820 ((a)))
#define __nds__v_sunpkd830(a) \
  (__builtin_riscv_v_sunpkd830 ((a)))
#define __nds__v_sunpkd831(a) \
  (__builtin_riscv_v_sunpkd831 ((a)))
#define __nds__v_sunpkd832(a) \
  (__builtin_riscv_v_sunpkd832 ((a)))
#define __nds__v_zunpkd810(a) \
  (__builtin_riscv_v_zunpkd810 ((a)))
#define __nds__v_zunpkd820(a) \
  (__builtin_riscv_v_zunpkd820 ((a)))
#define __nds__v_zunpkd830(a) \
  (__builtin_riscv_v_zunpkd830 ((a)))
#define __nds__v_zunpkd831(a) \
  (__builtin_riscv_v_zunpkd831 ((a)))
#define __nds__v_zunpkd832(a) \
  (__builtin_riscv_v_zunpkd832 ((a)))
#define __nds__v_pkbb16(a, b) \
  (__builtin_riscv_v_pkbb16 ((a), (b)))
#define __nds__v_pkbt16(a, b) \
  (__builtin_riscv_v_pkbt16 ((a), (b)))
#define __nds__v_pktb16(a, b) \
  (__builtin_riscv_v_pktb16 ((a), (b)))
#define __nds__v_pktt16(a, b) \
  (__builtin_riscv_v_pktt16 ((a), (b)))
#define __nds__v_smmwb(a, b) \
  (__builtin_riscv_v_smmwb ((a), (b)))
#define __nds__v_smmwb_u(a, b) \
  (__builtin_riscv_v_smmwb_u ((a), (b)))
#define __nds__v_smmwt(a, b) \
  (__builtin_riscv_v_smmwt ((a), (b)))
#define __nds__v_smmwt_u(a, b) \
  (__builtin_riscv_v_smmwt_u ((a), (b)))
#define __nds__v_kmmwb2(a, b) \
  (__builtin_riscv_v_kmmwb2 ((a), (b)))
#define __nds__v_kmmwb2_u(a, b) \
  (__builtin_riscv_v_kmmwb2_u ((a), (b)))
#define __nds__v_kmmwt2(a, b) \
  (__builtin_riscv_v_kmmwt2 ((a), (b)))
#define __nds__v_kmmwt2_u(a, b) \
  (__builtin_riscv_v_kmmwt2_u ((a), (b)))
#define __nds__v_kmmawb(r, a, b) \
  (__builtin_riscv_v_kmmawb ((r), (a), (b)))
#define __nds__v_kmmawb_u(r, a, b) \
  (__builtin_riscv_v_kmmawb_u ((r), (a), (b)))
#define __nds__v_kmmawt(r, a, b) \
  (__builtin_riscv_v_kmmawt ((r), (a), (b)))
#define __nds__v_kmmawt_u(r, a, b) \
  (__builtin_riscv_v_kmmawt_u ((r), (a), (b)))
#define __nds__v_kmmawb2(r, a, b) \
  (__builtin_riscv_v_kmmawb2 ((r), (a), (b)))
#define __nds__v_kmmawb2_u(r, a, b) \
  (__builtin_riscv_v_kmmawb2_u ((r), (a), (b)))
#define __nds__v_kmmawt2(r, a, b) \
  (__builtin_riscv_v_kmmawt2 ((r), (a), (b)))
#define __nds__v_kmmawt2_u(r, a, b) \
  (__builtin_riscv_v_kmmawt2_u ((r), (a), (b)))
#define __nds__v_smbb16(a, b) \
  (__builtin_riscv_v_smbb16 ((a), (b)))
#define __nds__v_smbt16(a, b) \
  (__builtin_riscv_v_smbt16 ((a), (b)))
#define __nds__v_smtt16(a, b) \
  (__builtin_riscv_v_smtt16 ((a), (b)))
#define __nds__v_kmda(a, b) \
  (__builtin_riscv_v_kmda ((a), (b)))
#define __nds__v_kmxda(a, b) \
  (__builtin_riscv_v_kmxda ((a), (b)))
#define __nds__v_smds(a, b) \
  (__builtin_riscv_v_smds ((a), (b)))
#define __nds__v_smdrs(a, b) \
  (__builtin_riscv_v_smdrs ((a), (b)))
#define __nds__v_smxds(a, b) \
  (__builtin_riscv_v_smxds ((a), (b)))
#define __nds__v_kmabb(r, a, b) \
  (__builtin_riscv_v_kmabb ((r), (a), (b)))
#define __nds__v_kmabt(r, a, b) \
  (__builtin_riscv_v_kmabt ((r), (a), (b)))
#define __nds__v_kmatt(r, a, b) \
  (__builtin_riscv_v_kmatt ((r), (a), (b)))
#define __nds__v_kmada(r, a, b) \
  (__builtin_riscv_v_kmada ((r), (a), (b)))
#define __nds__v_kmaxda(r, a, b) \
  (__builtin_riscv_v_kmaxda ((r), (a), (b)))
#define __nds__v_kmads(r, a, b) \
  (__builtin_riscv_v_kmads ((r), (a), (b)))
#define __nds__v_kmadrs(r, a, b) \
  (__builtin_riscv_v_kmadrs ((r), (a), (b)))
#define __nds__v_kmaxds(r, a, b) \
  (__builtin_riscv_v_kmaxds ((r), (a), (b)))
#define __nds__v_kmsda(r, a, b) \
  (__builtin_riscv_v_kmsda ((r), (a), (b)))
#define __nds__v_kmsxda(r, a, b) \
  (__builtin_riscv_v_kmsxda ((r), (a), (b)))
#define __nds__v_smal(a, b) \
  (__builtin_riscv_v_smal ((a), (b)))
#define __nds__v_smalbb(r, a, b) \
  (__builtin_riscv_v_smalbb ((r), (a), (b)))
#define __nds__v_smalbt(r, a, b) \
  (__builtin_riscv_v_smalbt ((r), (a), (b)))
#define __nds__v_smaltt(r, a, b) \
  (__builtin_riscv_v_smaltt ((r), (a), (b)))
#define __nds__v_smalda(r, a, b) \
  (__builtin_riscv_v_smalda ((r), (a), (b)))
#define __nds__v_smalxda(r, a, b) \
  (__builtin_riscv_v_smalxda ((r), (a), (b)))
#define __nds__v_smalds(r, a, b) \
  (__builtin_riscv_v_smalds ((r), (a), (b)))
#define __nds__v_smaldrs(r, a, b) \
  (__builtin_riscv_v_smaldrs ((r), (a), (b)))
#define __nds__v_smalxds(r, a, b) \
  (__builtin_riscv_v_smalxds ((r), (a), (b)))
#define __nds__v_smslda(r, a, b) \
  (__builtin_riscv_v_smslda ((r), (a), (b)))
#define __nds__v_smslxda(r, a, b) \
  (__builtin_riscv_v_smslxda ((r), (a), (b)))
#define __nds__v_sra8(a, b) \
  (__builtin_riscv_v_sra8 ((a), (b)))
#define __nds__v_sra8_u(a, b) \
  (__builtin_riscv_v_sra8_u ((a), (b)))
#define __nds__v_srl8(a, b) \
  (__builtin_riscv_v_srl8 ((a), (b)))
#define __nds__v_srl8_u(a, b) \
  (__builtin_riscv_v_srl8_u ((a), (b)))
#define __nds__v_sll8(a, b) \
  (__builtin_riscv_v_sll8 ((a), (b)))
#define __nds__v_ksll8(a, b) \
  (__builtin_riscv_v_ksll8 ((a), (b)))
#define __nds__v_kslra8(a, b) \
  (__builtin_riscv_v_kslra8 ((a), (b)))
#define __nds__v_kslra8_u(a, b) \
  (__builtin_riscv_v_kslra8_u ((a), (b)))
#define __nds__v_swap8(a) \
  (__builtin_riscv_v_swap8 ((a)))
#define __nds__v_swap16(a) \
  (__builtin_riscv_v_swap16 ((a)))
#define __nds__v_sclip8(a, b) \
  (__builtin_riscv_v_sclip8 ((a), (b)))
#define __nds__v_uclip8(a, b) \
  (__builtin_riscv_v_uclip8 ((a), (b)))
#define __nds__v_kdmabb(r, a, b) \
  (__builtin_riscv_v_kdmabb ((r), (a), (b)))
#define __nds__v_kdmabt(r, a, b) \
  (__builtin_riscv_v_kdmabt ((r), (a), (b)))
#define __nds__v_kdmatt(r, a, b) \
  (__builtin_riscv_v_kdmatt ((r), (a), (b)))
#define __nds__v_smaqa(r, a, b) \
  (__builtin_riscv_v_smaqa ((r), (a), (b)))
#define __nds__v_umaqa(r, a, b) \
  (__builtin_riscv_v_umaqa ((r), (a), (b)))
#define __nds__v_smaqa_su(r, a, b) \
  (__builtin_riscv_v_smaqa_su ((r), (a), (b)))
#define __nds__v_clrs8(a) \
  (__builtin_riscv_v_clrs8 ((a)))
#define __nds__v_clrs16(a) \
  (__builtin_riscv_v_clrs16 ((a)))
#define __nds__v_clo8(a) \
  (__builtin_riscv_v_clo8 ((a)))
#define __nds__v_clo16(a) \
  (__builtin_riscv_v_clo16 ((a)))
#define __nds__v_clz8(a) \
  (__builtin_riscv_v_clz8 ((a)))
#define __nds__v_clz16(a) \
  (__builtin_riscv_v_clz16 ((a)))
#define __nds__v_kdmbb(a, b) \
  (__builtin_riscv_v_kdmbb ((a), (b)))
#define __nds__v_kdmbt(a, b) \
  (__builtin_riscv_v_kdmbt ((a), (b)))
#define __nds__v_kdmtt(a, b) \
  (__builtin_riscv_v_kdmtt ((a), (b)))
#define __nds__v_khmbb(a, b) \
  (__builtin_riscv_v_khmbb ((a), (b)))
#define __nds__v_khmbt(a, b) \
  (__builtin_riscv_v_khmbt ((a), (b)))
#define __nds__v_khmtt(a, b) \
  (__builtin_riscv_v_khmtt ((a), (b)))
#define __nds__v_pbsad(a, b) \
  (__builtin_riscv_v_pbsad ((a), (b)))
#define __nds__v_pbsada(acc, a, b) \
  (__builtin_riscv_v_pbsada ((acc), (a), (b)))
#else
#define __nds__v_smul16(a, b) \
  (__builtin_riscv_v_smul16 ((a), (b)))
#define __nds__v_umul16(a, b) \
  (__builtin_riscv_v_umul16 ((a), (b)))
#define __nds__v_smul8(a, b) \
  (__builtin_riscv_v_smul8 ((a), (b)))
#define __nds__v_umul8(a, b) \
  (__builtin_riscv_v_umul8 ((a), (b)))
#define __nds__v_smulx16(a, b) \
  (__builtin_riscv_v_smulx16 ((a), (b)))
#define __nds__v_smulx8(a, b) \
  (__builtin_riscv_v_smulx8 ((a), (b)))
#define __nds__v_umulx16(a, b) \
  (__builtin_riscv_v_umulx16 ((a), (b)))
#define __nds__v_umulx8(a, b) \
  (__builtin_riscv_v_umulx8 ((a), (b)))
#define __nds__v_uadd16(a, b) \
  (__builtin_riscv_v64_uadd16 ((a), (b)))
#define __nds__v_sadd16(a, b) \
  (__builtin_riscv_v64_sadd16 ((a), (b)))
#define __nds__v_radd16(a, b) \
  (__builtin_riscv_v64_radd16 ((a), (b)))
#define __nds__v_uradd16(a, b) \
  (__builtin_riscv_v64_uradd16 ((a), (b)))
#define __nds__v_kadd16(a, b) \
  (__builtin_riscv_v64_kadd16 ((a), (b)))
#define __nds__v_ukadd16(a, b) \
  (__builtin_riscv_v64_ukadd16 ((a), (b)))
#define __nds__v_usub16(a, b) \
  (__builtin_riscv_v64_usub16 ((a), (b)))
#define __nds__v_ssub16(a, b) \
  (__builtin_riscv_v64_ssub16 ((a), (b)))
#define __nds__v_rsub16(a, b) \
  (__builtin_riscv_v64_rsub16 ((a), (b)))
#define __nds__v_ursub16(a, b) \
  (__builtin_riscv_v64_ursub16 ((a), (b)))
#define __nds__v_ksub16(a, b) \
  (__builtin_riscv_v64_ksub16 ((a), (b)))
#define __nds__v_uksub16(a, b) \
  (__builtin_riscv_v64_uksub16 ((a), (b)))
#define __nds__v_ucras16(a, b) \
  (__builtin_riscv_v64_ucras16 ((a), (b)))
#define __nds__v_scras16(a, b) \
  (__builtin_riscv_v64_scras16 ((a), (b)))
#define __nds__v_rcras16(a, b) \
  (__builtin_riscv_v64_rcras16 ((a), (b)))
#define __nds__v_urcras16(a, b) \
  (__builtin_riscv_v64_urcras16 ((a), (b)))
#define __nds__v_kcras16(a, b) \
  (__builtin_riscv_v64_kcras16 ((a), (b)))
#define __nds__v_ukcras16(a, b) \
  (__builtin_riscv_v64_ukcras16 ((a), (b)))
#define __nds__v_ucrsa16(a, b) \
  (__builtin_riscv_v64_ucrsa16 ((a), (b)))
#define __nds__v_scrsa16(a, b) \
  (__builtin_riscv_v64_scrsa16 ((a), (b)))
#define __nds__v_rcrsa16(a, b) \
  (__builtin_riscv_v64_rcrsa16 ((a), (b)))
#define __nds__v_urcrsa16(a, b) \
  (__builtin_riscv_v64_urcrsa16 ((a), (b)))
#define __nds__v_kcrsa16(a, b) \
  (__builtin_riscv_v64_kcrsa16 ((a), (b)))
#define __nds__v_ukcrsa16(a, b) \
  (__builtin_riscv_v64_ukcrsa16 ((a), (b)))
#define __nds__v_ustas16(a, b) \
  (__builtin_riscv_v64_ustas16 ((a), (b)))
#define __nds__v_sstas16(a, b) \
  (__builtin_riscv_v64_sstas16 ((a), (b)))
#define __nds__v_rstas16(a, b) \
  (__builtin_riscv_v64_rstas16 ((a), (b)))
#define __nds__v_urstas16(a, b) \
  (__builtin_riscv_v64_urstas16 ((a), (b)))
#define __nds__v_kstas16(a, b) \
  (__builtin_riscv_v64_kstas16 ((a), (b)))
#define __nds__v_ukstas16(a, b) \
  (__builtin_riscv_v64_ukstas16 ((a), (b)))
#define __nds__v_ustsa16(a, b) \
  (__builtin_riscv_v64_ustsa16 ((a), (b)))
#define __nds__v_sstsa16(a, b) \
  (__builtin_riscv_v64_sstsa16 ((a), (b)))
#define __nds__v_rstsa16(a, b) \
  (__builtin_riscv_v64_rstsa16 ((a), (b)))
#define __nds__v_urstsa16(a, b) \
  (__builtin_riscv_v64_urstsa16 ((a), (b)))
#define __nds__v_kstsa16(a, b) \
  (__builtin_riscv_v64_kstsa16 ((a), (b)))
#define __nds__v_ukstsa16(a, b) \
  (__builtin_riscv_v64_ukstsa16 ((a), (b)))
#define __nds__v_uadd8(a, b) \
  (__builtin_riscv_v64_uadd8 ((a), (b)))
#define __nds__v_sadd8(a, b) \
  (__builtin_riscv_v64_sadd8 ((a), (b)))
#define __nds__v_radd8(a, b) \
  (__builtin_riscv_v64_radd8 ((a), (b)))
#define __nds__v_uradd8(a, b) \
  (__builtin_riscv_v64_uradd8 ((a), (b)))
#define __nds__v_kadd8(a, b) \
  (__builtin_riscv_v64_kadd8 ((a), (b)))
#define __nds__v_ukadd8(a, b) \
  (__builtin_riscv_v64_ukadd8 ((a), (b)))
#define __nds__v_usub8(a, b) \
  (__builtin_riscv_v64_usub8 ((a), (b)))
#define __nds__v_ssub8(a, b) \
  (__builtin_riscv_v64_ssub8 ((a), (b)))
#define __nds__v_rsub8(a, b) \
  (__builtin_riscv_v64_rsub8 ((a), (b)))
#define __nds__v_ursub8(a, b) \
  (__builtin_riscv_v64_ursub8 ((a), (b)))
#define __nds__v_ksub8(a, b) \
  (__builtin_riscv_v64_ksub8 ((a), (b)))
#define __nds__v_uksub8(a, b) \
  (__builtin_riscv_v64_uksub8 ((a), (b)))
#define __nds__v_sra16(a, b) \
  (__builtin_riscv_v64_sra16 ((a), (b)))
#define __nds__v_sra16_u(a, b) \
  (__builtin_riscv_v64_sra16_u ((a), (b)))
#define __nds__v_srl16(a, b) \
  (__builtin_riscv_v64_srl16 ((a), (b)))
#define __nds__v_srl16_u(a, b) \
  (__builtin_riscv_v64_srl16_u ((a), (b)))
#define __nds__v_sll16(a, b) \
  (__builtin_riscv_v64_sll16 ((a), (b)))
#define __nds__v_ksll16(a, b) \
  (__builtin_riscv_v64_ksll16 ((a), (b)))
#define __nds__v_kslra16(a, b) \
  (__builtin_riscv_v64_kslra16 ((a), (b)))
#define __nds__v_kslra16_u(a, b) \
  (__builtin_riscv_v64_kslra16_u ((a), (b)))
#define __nds__v_scmpeq16(a, b) \
  (__builtin_riscv_v64_scmpeq16 ((a), (b)))
#define __nds__v_ucmpeq16(a, b) \
  (__builtin_riscv_v64_ucmpeq16 ((a), (b)))
#define __nds__v_scmplt16(a, b) \
  (__builtin_riscv_v64_scmplt16 ((a), (b)))
#define __nds__v_scmple16(a, b) \
  (__builtin_riscv_v64_scmple16 ((a), (b)))
#define __nds__v_ucmplt16(a, b) \
  (__builtin_riscv_v64_ucmplt16 ((a), (b)))
#define __nds__v_ucmple16(a, b) \
  (__builtin_riscv_v64_ucmple16 ((a), (b)))
#define __nds__v_scmpeq8(a, b) \
  (__builtin_riscv_v64_scmpeq8 ((a), (b)))
#define __nds__v_ucmpeq8(a, b) \
  (__builtin_riscv_v64_ucmpeq8 ((a), (b)))
#define __nds__v_scmplt8(a, b) \
  (__builtin_riscv_v64_scmplt8 ((a), (b)))
#define __nds__v_scmple8(a, b) \
  (__builtin_riscv_v64_scmple8 ((a), (b)))
#define __nds__v_ucmplt8(a, b) \
  (__builtin_riscv_v64_ucmplt8 ((a), (b)))
#define __nds__v_ucmple8(a, b) \
  (__builtin_riscv_v64_ucmple8 ((a), (b)))
#define __nds__v_smin16(a, b) \
  (__builtin_riscv_v64_smin16 ((a), (b)))
#define __nds__v_umin16(a, b) \
  (__builtin_riscv_v64_umin16 ((a), (b)))
#define __nds__v_smax16(a, b) \
  (__builtin_riscv_v64_smax16 ((a), (b)))
#define __nds__v_umax16(a, b) \
  (__builtin_riscv_v64_umax16 ((a), (b)))
#define __nds__v_sclip16(a, b) \
  (__builtin_riscv_v64_sclip16 ((a), (b)))
#define __nds__v_uclip16(a, b) \
  (__builtin_riscv_v64_uclip16 ((a), (b)))
#define __nds__v_khm16(a, b) \
  (__builtin_riscv_v64_khm16 ((a), (b)))
#define __nds__v_khmx16(a, b) \
  (__builtin_riscv_v64_khmx16 ((a), (b)))
#define __nds__v_khm8(a, b) \
  (__builtin_riscv_v64_khm8 ((a), (b)))
#define __nds__v_khmx8(a, b) \
  (__builtin_riscv_v64_khmx8 ((a), (b)))
#define __nds__v_kabs16(a) \
  (__builtin_riscv_v64_kabs16 ((a)))
#define __nds__v_smul16(a, b) \
  (__builtin_riscv_v_smul16 ((a), (b)))
#define __nds__v_smulx16(a, b) \
  (__builtin_riscv_v_smulx16 ((a), (b)))
#define __nds__v_umul16(a, b) \
  (__builtin_riscv_v_umul16 ((a), (b)))
#define __nds__v_umulx16(a, b) \
  (__builtin_riscv_v_umulx16 ((a), (b)))
#define __nds__v_smul8(a, b) \
  (__builtin_riscv_v_smul8 ((a), (b)))
#define __nds__v_smulx8(a, b) \
  (__builtin_riscv_v_smulx8 ((a), (b)))
#define __nds__v_umul8(a, b) \
  (__builtin_riscv_v_umul8 ((a), (b)))
#define __nds__v_umulx8(a, b) \
  (__builtin_riscv_v_umulx8 ((a), (b)))
#define __nds__v_smin8(a, b) \
  (__builtin_riscv_v64_smin8 ((a), (b)))
#define __nds__v_umin8(a, b) \
  (__builtin_riscv_v64_umin8 ((a), (b)))
#define __nds__v_smax8(a, b) \
  (__builtin_riscv_v64_smax8 ((a), (b)))
#define __nds__v_umax8(a, b) \
  (__builtin_riscv_v64_umax8 ((a), (b)))
#define __nds__v_kabs8(a) \
  (__builtin_riscv_v64_kabs8 ((a)))
#define __nds__v_sunpkd810(a) \
  (__builtin_riscv_v64_sunpkd810 ((a)))
#define __nds__v_sunpkd820(a) \
  (__builtin_riscv_v64_sunpkd820 ((a)))
#define __nds__v_sunpkd830(a) \
  (__builtin_riscv_v64_sunpkd830 ((a)))
#define __nds__v_sunpkd831(a) \
  (__builtin_riscv_v64_sunpkd831 ((a)))
#define __nds__v_sunpkd832(a) \
  (__builtin_riscv_v64_sunpkd832 ((a)))
#define __nds__v_zunpkd810(a) \
  (__builtin_riscv_v64_zunpkd810 ((a)))
#define __nds__v_zunpkd820(a) \
  (__builtin_riscv_v64_zunpkd820 ((a)))
#define __nds__v_zunpkd830(a) \
  (__builtin_riscv_v64_zunpkd830 ((a)))
#define __nds__v_zunpkd831(a) \
  (__builtin_riscv_v64_zunpkd831 ((a)))
#define __nds__v_zunpkd832(a) \
  (__builtin_riscv_v64_zunpkd832 ((a)))
#define __nds__v_pkbb16(a, b) \
  (__builtin_riscv_v64_pkbb16 ((a), (b)))
#define __nds__v_pkbt16(a, b) \
  (__builtin_riscv_v64_pkbt16 ((a), (b)))
#define __nds__v_pktb16(a, b) \
  (__builtin_riscv_v64_pktb16 ((a), (b)))
#define __nds__v_pktt16(a, b) \
  (__builtin_riscv_v64_pktt16 ((a), (b)))
#define __nds__v_smmwb(a, b) \
  (__builtin_riscv_v64_smmwb ((a), (b)))
#define __nds__v_smmwb_u(a, b) \
  (__builtin_riscv_v64_smmwb_u ((a), (b)))
#define __nds__v_smmwt(a, b) \
  (__builtin_riscv_v64_smmwt ((a), (b)))
#define __nds__v_smmwt_u(a, b) \
  (__builtin_riscv_v64_smmwt_u ((a), (b)))
#define __nds__v_kmmwb2(a, b) \
  (__builtin_riscv_v64_kmmwb2 ((a), (b)))
#define __nds__v_kmmwb2_u(a, b) \
  (__builtin_riscv_v64_kmmwb2_u ((a), (b)))
#define __nds__v_kmmwt2(a, b) \
  (__builtin_riscv_v64_kmmwt2 ((a), (b)))
#define __nds__v_kmmwt2_u(a, b) \
  (__builtin_riscv_v64_kmmwt2_u ((a), (b)))
#define __nds__v_kmmawb(r, a, b) \
  (__builtin_riscv_v64_kmmawb ((r), (a), (b)))
#define __nds__v_kmmawb_u(r, a, b) \
  (__builtin_riscv_v64_kmmawb_u ((r), (a), (b)))
#define __nds__v_kmmawt(r, a, b) \
  (__builtin_riscv_v64_kmmawt ((r), (a), (b)))
#define __nds__v_kmmawt_u(r, a, b) \
  (__builtin_riscv_v64_kmmawt_u ((r), (a), (b)))
#define __nds__v_kmmawb2(r, a, b) \
  (__builtin_riscv_v64_kmmawb2 ((r), (a), (b)))
#define __nds__v_kmmawb2_u(r, a, b) \
  (__builtin_riscv_v64_kmmawb2_u ((r), (a), (b)))
#define __nds__v_kmmawt2(r, a, b) \
  (__builtin_riscv_v64_kmmawt2 ((r), (a), (b)))
#define __nds__v_kmmawt2_u(r, a, b) \
  (__builtin_riscv_v64_kmmawt2_u ((r), (a), (b)))
#define __nds__v_smbb16(a, b) \
  (__builtin_riscv_v64_smbb16 ((a), (b)))
#define __nds__v_smbt16(a, b) \
  (__builtin_riscv_v64_smbt16 ((a), (b)))
#define __nds__v_smtt16(a, b) \
  (__builtin_riscv_v64_smtt16 ((a), (b)))
#define __nds__v_kmda(a, b) \
  (__builtin_riscv_v64_kmda ((a), (b)))
#define __nds__v_kmxda(a, b) \
  (__builtin_riscv_v64_kmxda ((a), (b)))
#define __nds__v_smds(a, b) \
  (__builtin_riscv_v64_smds ((a), (b)))
#define __nds__v_smdrs(a, b) \
  (__builtin_riscv_v64_smdrs ((a), (b)))
#define __nds__v_smxds(a, b) \
  (__builtin_riscv_v64_smxds ((a), (b)))
#define __nds__v_kmabb(r, a, b) \
  (__builtin_riscv_v64_kmabb ((r), (a), (b)))
#define __nds__v_kmabt(r, a, b) \
  (__builtin_riscv_v64_kmabt ((r), (a), (b)))
#define __nds__v_kmatt(r, a, b) \
  (__builtin_riscv_v64_kmatt ((r), (a), (b)))
#define __nds__v_kmada(r, a, b) \
  (__builtin_riscv_v64_kmada ((r), (a), (b)))
#define __nds__v_kmaxda(r, a, b) \
  (__builtin_riscv_v64_kmaxda ((r), (a), (b)))
#define __nds__v_kmads(r, a, b) \
  (__builtin_riscv_v64_kmads ((r), (a), (b)))
#define __nds__v_kmadrs(r, a, b) \
  (__builtin_riscv_v64_kmadrs ((r), (a), (b)))
#define __nds__v_kmaxds(r, a, b) \
  (__builtin_riscv_v64_kmaxds ((r), (a), (b)))
#define __nds__v_kmsda(r, a, b) \
  (__builtin_riscv_v64_kmsda ((r), (a), (b)))
#define __nds__v_kmsxda(r, a, b) \
  (__builtin_riscv_v64_kmsxda ((r), (a), (b)))
#define __nds__v_smal(a, b) \
  (__builtin_riscv_v64_smal ((a), (b)))
#define __nds__v_smalbb(r, a, b) \
  (__builtin_riscv_v64_smalbb ((r), (a), (b)))
#define __nds__v_smalbt(r, a, b) \
  (__builtin_riscv_v64_smalbt ((r), (a), (b)))
#define __nds__v_smaltt(r, a, b) \
  (__builtin_riscv_v64_smaltt ((r), (a), (b)))
#define __nds__v_smalda(r, a, b) \
  (__builtin_riscv_v64_smalda ((r), (a), (b)))
#define __nds__v_smalxda(r, a, b) \
  (__builtin_riscv_v64_smalxda ((r), (a), (b)))
#define __nds__v_smalds(r, a, b) \
  (__builtin_riscv_v64_smalds ((r), (a), (b)))
#define __nds__v_smaldrs(r, a, b) \
  (__builtin_riscv_v64_smaldrs ((r), (a), (b)))
#define __nds__v_smalxds(r, a, b) \
  (__builtin_riscv_v64_smalxds ((r), (a), (b)))
#define __nds__v_smslda(r, a, b) \
  (__builtin_riscv_v64_smslda ((r), (a), (b)))
#define __nds__v_smslxda(r, a, b) \
  (__builtin_riscv_v64_smslxda ((r), (a), (b)))
#define __nds__v_sra8(a, b) \
  (__builtin_riscv_v64_sra8 ((a), (b)))
#define __nds__v_sra8_u(a, b) \
  (__builtin_riscv_v64_sra8_u ((a), (b)))
#define __nds__v_srl8(a, b) \
  (__builtin_riscv_v64_srl8 ((a), (b)))
#define __nds__v_srl8_u(a, b) \
  (__builtin_riscv_v64_srl8_u ((a), (b)))
#define __nds__v_sll8(a, b) \
  (__builtin_riscv_v64_sll8 ((a), (b)))
#define __nds__v_ksll8(a, b) \
  (__builtin_riscv_v64_ksll8 ((a), (b)))
#define __nds__v_kslra8(a, b) \
  (__builtin_riscv_v64_kslra8 ((a), (b)))
#define __nds__v_kslra8_u(a, b) \
  (__builtin_riscv_v64_kslra8_u ((a), (b)))
#define __nds__v_sclip8(a, b) \
  (__builtin_riscv_v64_sclip8 ((a), (b)))
#define __nds__v_uclip8(a, b) \
  (__builtin_riscv_v64_uclip8 ((a), (b)))
#define __nds__v_uadd32(a, b) \
  (__builtin_riscv_v64_uadd32 ((a), (b)))
#define __nds__v_sadd32(a, b) \
  (__builtin_riscv_v64_sadd32 ((a), (b)))
#define __nds__v_radd32(a, b) \
  (__builtin_riscv_v64_radd32 ((a), (b)))
#define __nds__v_uradd32(a, b) \
  (__builtin_riscv_v64_uradd32 ((a), (b)))
#define __nds__v_kadd32(a, b) \
  (__builtin_riscv_v64_kadd32 ((a), (b)))
#define __nds__v_ukadd32(a, b) \
  (__builtin_riscv_v64_ukadd32 ((a), (b)))
#define __nds__v_usub32(a, b) \
  (__builtin_riscv_v64_usub32 ((a), (b)))
#define __nds__v_ssub32(a, b) \
  (__builtin_riscv_v64_ssub32 ((a), (b)))
#define __nds__v_rsub32(a, b) \
  (__builtin_riscv_v64_rsub32 ((a), (b)))
#define __nds__v_ursub32(a, b) \
  (__builtin_riscv_v64_ursub32 ((a), (b)))
#define __nds__v_ksub32(a, b) \
  (__builtin_riscv_v64_ksub32 ((a), (b)))
#define __nds__v_uksub32(a, b) \
  (__builtin_riscv_v64_uksub32 ((a), (b)))
#define __nds__v_ucras32(a, b) \
  (__builtin_riscv_v64_ucras32 ((a), (b)))
#define __nds__v_scras32(a, b) \
  (__builtin_riscv_v64_scras32 ((a), (b)))
#define __nds__v_ucrsa32(a, b) \
  (__builtin_riscv_v64_ucrsa32 ((a), (b)))
#define __nds__v_scrsa32(a, b) \
  (__builtin_riscv_v64_scrsa32 ((a), (b)))
#define __nds__v_rcras32(a, b) \
  (__builtin_riscv_v64_rcras32 ((a), (b)))
#define __nds__v_rcrsa32(a, b) \
  (__builtin_riscv_v64_rcrsa32 ((a), (b)))
#define __nds__v_urcras32(a, b) \
  (__builtin_riscv_v64_urcras32 ((a), (b)))
#define __nds__v_urcrsa32(a, b) \
  (__builtin_riscv_v64_urcrsa32 ((a), (b)))
#define __nds__v_kcras32(a, b) \
  (__builtin_riscv_v64_kcras32 ((a), (b)))
#define __nds__v_kcrsa32(a, b) \
  (__builtin_riscv_v64_kcrsa32 ((a), (b)))
#define __nds__v_ukcras32(a, b) \
  (__builtin_riscv_v64_ukcras32 ((a), (b)))
#define __nds__v_ukcrsa32(a, b) \
  (__builtin_riscv_v64_ukcrsa32 ((a), (b)))
#define __nds__v_ustas32(a, b) \
  (__builtin_riscv_v64_ustas32 ((a), (b)))
#define __nds__v_sstas32(a, b) \
  (__builtin_riscv_v64_sstas32 ((a), (b)))
#define __nds__v_ustsa32(a, b) \
  (__builtin_riscv_v64_ustsa32 ((a), (b)))
#define __nds__v_sstsa32(a, b) \
  (__builtin_riscv_v64_sstsa32 ((a), (b)))
#define __nds__v_rstas32(a, b) \
  (__builtin_riscv_v64_rstas32 ((a), (b)))
#define __nds__v_rstsa32(a, b) \
  (__builtin_riscv_v64_rstsa32 ((a), (b)))
#define __nds__v_urstas32(a, b) \
  (__builtin_riscv_v64_urstas32 ((a), (b)))
#define __nds__v_urstsa32(a, b) \
  (__builtin_riscv_v64_urstsa32 ((a), (b)))
#define __nds__v_kstas32(a, b) \
  (__builtin_riscv_v64_kstas32 ((a), (b)))
#define __nds__v_kstsa32(a, b) \
  (__builtin_riscv_v64_kstsa32 ((a), (b)))
#define __nds__v_ukstas32(a, b) \
  (__builtin_riscv_v64_ukstas32 ((a), (b)))
#define __nds__v_ukstsa32(a, b) \
  (__builtin_riscv_v64_ukstsa32 ((a), (b)))
#define __nds__v_sra32(a, b) \
  (__builtin_riscv_v64_sra32 ((a), (b)))
#define __nds__v_sra32_u(a, b) \
  (__builtin_riscv_v64_sra32_u ((a), (b)))
#define __nds__v_srl32(a, b) \
  (__builtin_riscv_v64_srl32 ((a), (b)))
#define __nds__v_srl32_u(a, b) \
  (__builtin_riscv_v64_srl32_u ((a), (b)))
#define __nds__v_sll32(a, b) \
  (__builtin_riscv_v64_sll32 ((a), (b)))
#define __nds__v_ksll32(a, b) \
  (__builtin_riscv_v64_ksll32 ((a), (b)))
#define __nds__v_kslra32(a, b) \
  (__builtin_riscv_v64_kslra32 ((a), (b)))
#define __nds__v_kslra32_u(a, b) \
  (__builtin_riscv_v64_kslra32_u ((a), (b)))
#define __nds__v_smin32(a, b) \
  (__builtin_riscv_v64_smin32 ((a), (b)))
#define __nds__v_umin32(a, b) \
  (__builtin_riscv_v64_umin32 ((a), (b)))
#define __nds__v_smax32(a, b) \
  (__builtin_riscv_v64_smax32 ((a), (b)))
#define __nds__v_umax32(a, b) \
  (__builtin_riscv_v64_umax32 ((a), (b)))
#define __nds__v_kabs32(a) \
  (__builtin_riscv_v64_kabs32 ((a)))
#define __nds__v_khmbb16(a, b) \
  (__builtin_riscv_v64_khmbb16 ((a), (b)))
#define __nds__v_khmbt16(a, b) \
  (__builtin_riscv_v64_khmbt16 ((a), (b)))
#define __nds__v_khmtt16(a, b) \
  (__builtin_riscv_v64_khmtt16 ((a), (b)))
#define __nds__v_kdmbb16(a, b) \
  (__builtin_riscv_v64_kdmbb16 ((a), (b)))
#define __nds__v_kdmbt16(a, b) \
  (__builtin_riscv_v64_kdmbt16 ((a), (b)))
#define __nds__v_kdmtt16(a, b) \
  (__builtin_riscv_v64_kdmtt16 ((a), (b)))
#define __nds__v_smbb32(a, b) \
  (__builtin_riscv_v64_smbb32 ((a), (b)))
#define __nds__v_smbt32(a, b) \
  (__builtin_riscv_v64_smbt32 ((a), (b)))
#define __nds__v_smtt32(a, b) \
  (__builtin_riscv_v64_smtt32 ((a), (b)))
#define __nds__v_kmabb32(r, a, b) \
  (__builtin_riscv_v64_kmabb32 ((r), (a), (b)))
#define __nds__v_kmabt32(r, a, b) \
  (__builtin_riscv_v64_kmabt32 ((r), (a), (b)))
#define __nds__v_kmatt32(r, a, b) \
  (__builtin_riscv_v64_kmatt32 ((r), (a), (b)))
#define __nds__v_kmda32(a, b) \
  (__builtin_riscv_v64_kmda32 ((a), (b)))
#define __nds__v_kmxda32(a, b) \
  (__builtin_riscv_v64_kmxda32 ((a), (b)))
#define __nds__v_kmada32(r, a, b) \
  (__builtin_riscv_v64_kmada32 ((r), (a), (b)))
#define __nds__v_kmaxda32(r, a, b) \
  (__builtin_riscv_v64_kmaxda32 ((r), (a), (b)))
#define __nds__v_kmads32(r, a, b) \
  (__builtin_riscv_v64_kmads32 ((r), (a), (b)))
#define __nds__v_kmadrs32(r, a, b) \
  (__builtin_riscv_v64_kmadrs32 ((r), (a), (b)))
#define __nds__v_kmaxds32(r, a, b) \
  (__builtin_riscv_v64_kmaxds32 ((r), (a), (b)))
#define __nds__v_kmsda32(r, a, b) \
  (__builtin_riscv_v64_kmsda32 ((r), (a), (b)))
#define __nds__v_kmsxda32(r, a, b) \
  (__builtin_riscv_v64_kmsxda32 ((r), (a), (b)))
#define __nds__v_smds32(a, b) \
  (__builtin_riscv_v64_smds32 ((a), (b)))
#define __nds__v_smdrs32(a, b) \
  (__builtin_riscv_v64_smdrs32 ((a), (b)))
#define __nds__v_smxds32(a, b) \
  (__builtin_riscv_v64_smxds32 ((a), (b)))
#define __nds__v_kdmabb(r, a, b) \
  (__builtin_riscv_v64_kdmabb ((r), (a), (b)))
#define __nds__v_kdmabt(r, a, b) \
  (__builtin_riscv_v64_kdmabt ((r), (a), (b)))
#define __nds__v_kdmatt(r, a, b) \
  (__builtin_riscv_v64_kdmatt ((r), (a), (b)))
#define __nds__v_kdmabb16(r, a, b) \
  (__builtin_riscv_v64_kdmabb16 ((r), (a), (b)))
#define __nds__v_kdmabt16(r, a, b) \
  (__builtin_riscv_v64_kdmabt16 ((r), (a), (b)))
#define __nds__v_kdmatt16(r, a, b) \
  (__builtin_riscv_v64_kdmatt16 ((r), (a), (b)))
#define __nds__v_smaqa(r, a, b) \
  (__builtin_riscv_v64_smaqa ((r), (a), (b)))
#define __nds__v_umaqa(r, a, b) \
  (__builtin_riscv_v64_umaqa ((r), (a), (b)))
#define __nds__v_smaqa_su(r, a, b) \
  (__builtin_riscv_v64_smaqa_su ((r), (a), (b)))
#define __nds__v_clrs8(a) \
  (__builtin_riscv_v64_clrs8 ((a)))
#define __nds__v_clrs16(a) \
  (__builtin_riscv_v64_clrs16 ((a)))
#define __nds__v_clrs32(a) \
  (__builtin_riscv_v64_clrs32 ((a)))
#define __nds__v_clo8(a) \
  (__builtin_riscv_v64_clo8 ((a)))
#define __nds__v_clo16(a) \
  (__builtin_riscv_v64_clo16 ((a)))
#define __nds__v_clo32(a) \
  (__builtin_riscv_v64_clo32 ((a)))
#define __nds__v_clz8(a) \
  (__builtin_riscv_v64_clz8 ((a)))
#define __nds__v_clz16(a) \
  (__builtin_riscv_v64_clz16 ((a)))
#define __nds__v_clz32(a) \
  (__builtin_riscv_v64_clz32 ((a)))
#define __nds__v_swap8(a) \
  (__builtin_riscv_v64_swap8 ((a)))
#define __nds__v_swap16(a) \
  (__builtin_riscv_v64_swap16 ((a)))
#define __nds__v_pkbb32(a, b) \
  (__builtin_riscv_v64_pkbb32 ((a), (b)))
#define __nds__v_pkbt32(a, b) \
  (__builtin_riscv_v64_pkbt32 ((a), (b)))
#define __nds__v_pktb32(a, b) \
  (__builtin_riscv_v64_pktb32 ((a), (b)))
#define __nds__v_pktt32(a, b) \
  (__builtin_riscv_v64_pktt32 ((a), (b)))
#define __nds__v_kdmbb(a, b) \
  (__builtin_riscv_v64_kdmbb ((a), (b)))
#define __nds__v_kdmbt(a, b) \
  (__builtin_riscv_v64_kdmbt ((a), (b)))
#define __nds__v_kdmtt(a, b) \
  (__builtin_riscv_v64_kdmtt ((a), (b)))
#define __nds__v_khmbb(a, b) \
  (__builtin_riscv_v64_khmbb ((a), (b)))
#define __nds__v_khmbt(a, b) \
  (__builtin_riscv_v64_khmbt ((a), (b)))
#define __nds__v_khmtt(a, b) \
  (__builtin_riscv_v64_khmtt ((a), (b)))
#define __nds__v_smmul(a, b) \
  (__builtin_riscv_v64_smmul ((a), (b)))
#define __nds__v_smmul_u(a, b) \
  (__builtin_riscv_v64_smmul_u ((a), (b)))
#define __nds__v_kwmmul(a, b) \
  (__builtin_riscv_v64_kwmmul ((a), (b)))
#define __nds__v_kwmmul_u(a, b) \
  (__builtin_riscv_v64_kwmmul_u ((a), (b)))
#define __nds__v_kmmac(r, a, b) \
  (__builtin_riscv_v64_kmmac ((r), (a), (b)))
#define __nds__v_kmmac_u(r, a, b) \
  (__builtin_riscv_v64_kmmac_u ((r), (a), (b)))
#define __nds__v_kmmsb(r, a, b) \
  (__builtin_riscv_v64_kmmsb ((r), (a), (b)))
#define __nds__v_kmmsb_u(r, a, b) \
  (__builtin_riscv_v64_kmmsb_u ((r), (a), (b)))
#define __nds__v_uclip32(a, imm) \
  (__builtin_riscv_v64_uclip32 ((a), (imm)))
#define __nds__v_sclip32(a, imm) \
  (__builtin_riscv_v64_sclip32 ((a), (imm)))
#define __nds__v_pbsad(a, b) \
  (__builtin_riscv_v64_pbsad ((a), (b)))
#define __nds__v_pbsada(acc, a, b) \
  (__builtin_riscv_v64_pbsada ((acc), (a), (b)))

#define __nds__v_smar64(r, a, b) \
  (__builtin_riscv_v64_smar64 ((r), (a), (b)))
#define __nds__v_smsr64(r, a, b) \
  (__builtin_riscv_v64_smsr64 ((r), (a), (b)))
#define __nds__v_umar64(r, a, b) \
  (__builtin_riscv_v64_umar64 ((r), (a), (b)))
#define __nds__v_umsr64(r, a, b) \
  (__builtin_riscv_v64_umsr64 ((r), (a), (b)))
#define __nds__v_kmar64(r, a, b) \
  (__builtin_riscv_v64_kmar64 ((r), (a), (b)))
#define __nds__v_kmsr64(r, a, b) \
  (__builtin_riscv_v64_kmsr64 ((r), (a), (b)))
#define __nds__v_ukmar64(r, a, b) \
  (__builtin_riscv_v64_ukmar64 ((r), (a), (b)))
#define __nds__v_ukmsr64(r, a, b) \
  (__builtin_riscv_v64_ukmsr64 ((r), (a), (b)))
#define vec64_all_eq(type, size, vec1, vec2) \
    (__builtin_riscv_vec_ ##type##cmpeq ##size (vec1, vec2) == 0xffffffffffffffffull)
#endif

static unsigned int __nds__rotr(unsigned int val, unsigned int ror) __attribute__((unused));
static unsigned int __nds__wsbh(unsigned int a) __attribute__((unused));

static unsigned int __nds__rotr(unsigned int val, unsigned int ror)
{
  unsigned int rotr = ror & 0x1f;
  return (val >> rotr) | (val << (sizeof(val)*8 - rotr));
}

static unsigned int __nds__wsbh(unsigned int a)
{
  unsigned int b0 = (a >>  0) & 0xff;
  unsigned int b1 = (a >>  8) & 0xff;
  unsigned int b2 = (a >> 16) & 0xff;
  unsigned int b3 = (a >> 24) & 0xff;
  return (b0 << 8) | (b1 << 0) | (b2 << 24) | (b3 << 16);
}

#endif
