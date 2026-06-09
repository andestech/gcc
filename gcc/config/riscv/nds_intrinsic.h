#ifndef _NDS_INTRINSIC_H
#define _NDS_INTRINSIC_H

#define __NDS_STR(X) #X
#ifdef __NDS_SUPPRESS_DEPRECATED
#define __DEPRECATED_NDS_MACRO(X)
#else
#define __DEPRECATED_NDS_MACRO(MSG) _Pragma (__NDS_STR (GCC warning MSG))
#endif

#include "riscv_csr.h"

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

// Intrinsics for RV32I and RV64I

#define __nds__fence(a, b) \
  (__builtin_riscv_fence ((a), (b)))
#define __nds__fencei() \
  (__builtin_riscv_fencei ())
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
#define __nds__ebreak(a) \
  (__builtin_riscv_ebreak ((a)))

#define __nds__mfsr(srname) \
  (__builtin_riscv_csrr ((srname)))
#define __nds__mtsr(val, srname) \
  (__builtin_riscv_csrw ((val), (srname)))
#define __nds__csrrw(val, srname) \
  (__builtin_riscv_csrrw ((val), (srname)))
#define __nds__csrrs(val, srname) \
  (__builtin_riscv_csrrs ((val), (srname)))
#define __nds__csrrc(val, srname) \
  (__builtin_riscv_csrrc ((val), (srname)))
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

#define __nds__get_current_sp() \
  (__builtin_riscv_get_current_sp ())
#define __nds__set_current_sp(a) \
  (__builtin_riscv_set_current_sp ((a)))

// Intrinsics for Andes Performance Extension ffb, ffmism and flmism

#define __nds__ffb(a, b)                                                       \
  __DEPRECATED_NDS_MACRO ("'__nds__ffb' will be deprecated in next 2 "         \
			  "AndeSight release, please "                         \
			  "use '__riscv_nds_ffb' instead")                     \
  (__riscv_nds_ffb ((a), (b)))

#define __nds__ffmism(a, b)                                                    \
  __DEPRECATED_NDS_MACRO ("'__nds__ffmism' will be deprecated in next 2 "      \
			  "AndeSight release, please "                         \
			  "use '__riscv_nds_ffmism' instead")                  \
  (__riscv_nds_ffmism ((a), (b)))

#define __nds__flmism(a, b)                                                    \
  __DEPRECATED_NDS_MACRO ("'__nds__flmism' will be deprecated in next 2 "      \
			  "AndeSight release, please "                         \
			  "use '__riscv_nds_flmism' instead")                  \
  (__riscv_nds_flmism ((a), (b)))

// Intrinsics for floating-point

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

// Intrinsics for RV32 & RV64 atomic extension

#define __nds__lrw(a, b) \
  (__builtin_riscv_lrw ((a), (b)))
#define __nds__scw(a, b, c) \
  (__builtin_riscv_scw ((a), (b), (c)))
#define __nds__amoswapw(a, b, c) \
  (__builtin_riscv_amoswapw ((a), (b), (c)))
#define __nds__amoaddw(a, b, c) \
  (__builtin_riscv_amoaddw ((a), (b), (c)))
#define __nds__amoxorw(a, b, c) \
  (__builtin_riscv_amoxorw ((a), (b), (c)))
#define __nds__amoandw(a, b, c) \
  (__builtin_riscv_amoandw ((a), (b), (c)))
#define __nds__amoorw(a, b, c) \
  (__builtin_riscv_amoorw ((a), (b), (c)))
#define __nds__amominw(a, b, c) \
  (__builtin_riscv_amominw ((a), (b), (c)))
#define __nds__amomaxw(a, b, c) \
  (__builtin_riscv_amomaxw ((a), (b), (c)))
#define __nds__amominuw(a, b, c) \
  (__builtin_riscv_amominuw ((a), (b), (c)))
#define __nds__amomaxuw(a, b, c) \
  (__builtin_riscv_amomaxuw ((a), (b), (c)))

// Intrinsics for RV64-only atomic extension

#define __nds__lrd(a, b) \
  (__builtin_riscv_lrd ((a), (b)))
#define __nds__scd(a, b, c) \
  (__builtin_riscv_scd ((a), (b), (c)))
#define __nds__amoswapd(a, b, c) \
  (__builtin_riscv_amoswapd ((a), (b), (c)))
#define __nds__amoaddd(a, b, c) \
  (__builtin_riscv_amoaddd ((a), (b), (c)))
#define __nds__amoxord(a, b, c) \
  (__builtin_riscv_amoxord ((a), (b), (c)))
#define __nds__amoandd(a, b, c) \
  (__builtin_riscv_amoandd ((a), (b), (c)))
#define __nds__amoord(a, b, c) \
  (__builtin_riscv_amoord ((a), (b), (c)))
#define __nds__amomind(a, b, c) \
  (__builtin_riscv_amomind ((a), (b), (c)))
#define __nds__amomaxd(a, b, c) \
  (__builtin_riscv_amomaxd ((a), (b), (c)))
#define __nds__amominud(a, b, c) \
  (__builtin_riscv_amominud ((a), (b), (c)))
#define __nds__amomaxud(a, b, c) \
  (__builtin_riscv_amomaxud ((a), (b), (c)))

// Intrinsics for Andes Scalar BFLOAT16 Conversion Extension (XAndesBFHCvt)
#define __nds_fcvt_s_bf16(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds_fcvt_s_bf16' will be deprecated in next 2 "                        \
	"AndeSight release, please "                                               \
	"use '__riscv_nds_fcvt_s_bf16' instead")                                   \
  (__riscv_nds_fcvt_s_bf16 ((a)))

#define __nds_fcvt_bf16_s(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds_fcvt_bf16_s' will be deprecated in next 2 "                        \
	"AndeSight release, please "                                               \
	"use '__riscv_nds_fcvt_bf16_s' instead")                                   \
  (__riscv_nds_fcvt_bf16_s ((a)))

// Intrinsics for Andes Vector BFLOAT16 Conversion Extension (XAndesVBFHCvt)
#if defined(__riscv_vector) && defined(__riscv_f) && defined(__nds_bf16)
#include <riscv_vector.h>
#define vfwcvtsbf16_bf16m1(src)                                                \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'vfwcvtsbf16_bf16m1' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_nds_vfwcvt_s_bf16_f32m2' instead")                    \
  (__riscv_nds_vfwcvt_s_bf16_f32m2 ((src), 0))
#define vfwcvtsbf16_bf16m2(src)                                                \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'vfwcvtsbf16_bf16m2' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_nds_vfwcvt_s_bf16_f32m4' instead")                    \
  (__riscv_nds_vfwcvt_s_bf16_f32m4 ((src), 0))
#define vfwcvtsbf16_bf16m4(src)                                                \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'vfwcvtsbf16_bfvfwcvtsbf16_bf16m416m1' will be deprecated in next 2 "     \
    "AndeSight release, "                                                      \
    "please use '__riscv_nds_vfwcvt_s_bf16_f32m8' instead")                    \
  (__riscv_nds_vfwcvt_s_bf16_f32m8 ((src), 0))
#define vfncvtbf16s_bf16m1(src)                                                \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'vfncvtbf16s_bf16m1' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_nds_vfncvt_bf16_s_bf16m1' instead")                   \
  (__riscv_nds_vfncvt_bf16_s_bf16m1 ((src), 0))
#define vfncvtbf16s_bf16m2(src)                                                \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'vfncvtbf16s_bf16m2' will be deprecated in next 2 "                       \
	"AndeSight release, please "                                               \
	"use '__riscv_nds_vfncvt_bf16_s_bf16m2' instead")                          \
  (__riscv_nds_vfncvt_bf16_s_bf16m2 ((src), 0))
#define vfncvtbf16s_bf16m4(src)                                                \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'vfncvtbf16s_bf16m4' will be deprecated in next 2 "                       \
	"AndeSight release, please "                                               \
	"use '__riscv_nds_vfncvt_bf16_s_bf16m4' instead")                          \
  (__riscv_nds_vfncvt_bf16_s_bf16m4 ((src), 0))
#define __riscv_vfwcvt_s_bf16_f32m2                                            \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__riscv_vfwcvt_s_bf16_f32m2' will be deprecated in next 2 "              \
    "AndeSight release, please "                                               \
    "use '__riscv_nds_vfwcvt_s_bf16_f32m2' instead")                           \
  __riscv_nds_vfwcvt_s_bf16_f32m2
#define __riscv_vfwcvt_s_bf16_f32m4                                            \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__riscv_vfwcvt_s_bf16_f32m4' will be deprecated in next 2 "              \
    "AndeSight release, please "                                               \
    "use '__riscv_nds_vfwcvt_s_bf16_f32m4' instead")                           \
  __riscv_nds_vfwcvt_s_bf16_f32m4
#define __riscv_vfwcvt_s_bf16_f32m8                                            \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__riscv_vfwcvt_s_bf16_f32m8' will be deprecated in next 2 "              \
    "AndeSight release, please "                                               \
    "use '__riscv_nds_vfwcvt_s_bf16_f32m8' instead")                           \
  __riscv_nds_vfwcvt_s_bf16_f32m8
#define __riscv_vfncvt_bf16_s_bf16m1                                           \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__riscv_vfncvt_bf16_s_bf16m1' will be deprecated in next 2 "             \
    "AndeSight release, please "                                               \
    "use '__riscv_nds_vfncvt_bf16_s_bf16m1' instead")                          \
  __riscv_nds_vfncvt_bf16_s_bf16m1
#define __riscv_vfncvt_bf16_s_bf16m2                                           \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__riscv_vfncvt_bf16_s_bf16m2' will be deprecated in next 2 "             \
    "AndeSight release, please "                                               \
    "use '__riscv_nds_vfncvt_bf16_s_bf16m2' instead")                          \
  __riscv_nds_vfncvt_bf16_s_bf16m2
#define __riscv_vfncvt_bf16_s_bf16m4                                           \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__riscv_vfncvt_bf16_s_bf16m4' will be deprecated in next 2 "             \
    "AndeSight release, please "                                               \
    "use '__riscv_nds_vfncvt_bf16_s_bf16m4' instead")                          \
  __riscv_nds_vfncvt_bf16_s_bf16m4
#endif

// Intrinsics for Zbb

#define __nds__clz_32(a)                                                       \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clz_32' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_clz_32' instead")                                            \
  (__builtin_clz (a))
#define __nds__ctz_32(a)                                                       \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__ctz_32' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_ctz_32' instead")                                            \
  (__builtin_ctz (a))
#define __nds__cpop_32(a)                                                      \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__cpop_32' will be deprecated in next 2 AndeSight release, please " \
    "use '__riscv_cpop_32' instead")                                           \
  (__builtin_popcount (a))

#if __riscv_xlen == 64
#define __nds__clz_64(a)                                                       \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clz_64' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_clz_64' instead")                                            \
  (__builtin_clzl (a))
#define __nds__ctz_64(a)                                                       \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__ctz_64' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_ctz_64' instead")                                            \
  (__builtin_ctzl (a))
#define __nds__cpop_64(a)                                                      \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__cpop_64' will be deprecated in next 2 AndeSight release, please " \
    "use '__riscv_cpop_64' instead")                                           \
  (__builtin_popcountl (a))
#endif

// Intrinsics for Zbc or Zbkc

#if __riscv_xlen == 32
#define __nds__clmul(a, b)                                                     \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmul' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_clmul_32' instead")                                          \
  (__builtin_riscv_clmul_32 (a, b))
#define __nds__clmulh(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulh' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_clmulh_32' instead")                                         \
  (__builtin_riscv_clmulh_32 (a, b))
#define __nds__clmulr(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulr' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_clmulr_32' instead")                                         \
  (__builtin_riscv_clmulr_32 (a, b))
#else
#define __nds__clmul(a, b)                                                     \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmul' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_clmul_64' instead")                                          \
  (__builtin_riscv_clmul_64 (a, b))
#define __nds__clmulh(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulh' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_clmulh_64' instead")                                         \
  (__builtin_riscv_clmulh_64 (a, b))
#define __nds__clmulr(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulr' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_clmulr_64' instead")                                         \
  (__builtin_riscv_clmulr_64 (a, b))
#endif

#define __nds__clmul_32(a, b)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmul_32' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_clmul_32' instead")                                   \
  (__builtin_riscv_clmul_32 (a, b))
#define __nds__clmulh_32(a, b)                                                 \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulh_32' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_clmulh_32' instead")                                  \
  (__builtin_riscv_clmulh_32 (a, b))
#define __nds__clmulr_32(a, b)                                                 \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulr_32' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_clmulr_32' instead")                                  \
  (__builtin_riscv_clmulr_32 (a, b))

#define __nds__clmul_64(a, b)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmul_64' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_clmul_64' instead")                                   \
  (__builtin_riscv_clmul_64 (a, b))
#define __nds__clmulh_64(a, b)                                                 \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulh_64' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_clmulh_64' instead")                                  \
  (__builtin_riscv_clmulh_64 (a, b))
#define __nds__clmulr_64(a, b)                                                 \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__clmulr_64' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_clmulr_64' instead")                                  \
  (__builtin_riscv_clmulr_64 (a, b))

// Intrinsics for Zbkx

#if __riscv_xlen == 32
#define __nds__xperm4(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__xperm4' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_xperm4_32' instead")                                         \
  (__builtin_riscv_xperm4 (a, b))
#define __nds__xperm8(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__xperm8' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_xperm8_32' instead")                                         \
  (__builtin_riscv_xperm8 (a, b))
#else
#define __nds__xperm4(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__xperm4' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_xperm4_64' instead")                                         \
  (__builtin_riscv_xperm4 (a, b))
#define __nds__xperm8(a, b)                                                    \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__xperm8' will be deprecated in next 2 AndeSight release, please "  \
    "use '__riscv_xperm8_64' instead")                                         \
  (__builtin_riscv_xperm8 (a, b))
#endif

// Intrinsics for Zbkb

#if __riscv_xlen == 32
#define __nds__brev8(a)                                                        \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__brev8' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_brev8_32' instead")                                          \
  (__builtin_riscv_brev8_32 (a))
#else
#define __nds__brev8(a)                                                        \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__brev8' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_brev8_64' instead")                                          \
  (__builtin_riscv_brev8_64 (a))
#endif

#define __nds__brev8_32(a)                                                     \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__brev8_32' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_brev8_32' instead")                                   \
  (__builtin_riscv_brev8_32 (a))
#define __nds__brev8_64(a)                                                     \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__brev8_64' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_brev8_64' instead")                                   \
  (__builtin_riscv_brev8_64 (a))

#if __riscv_xlen == 32
#define __nds__zip(a)                                                          \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__zip' will be deprecated in next 2 AndeSight release, please use " \
    "'__riscv_zip_32' instead")                                                \
  (__builtin_riscv_zip (a))
#define __nds__unzip(a)                                                        \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__unzip' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_unzip_32' instead")                                          \
  (__builtin_riscv_unzip (a))
#endif

// Intrinsics for Zknd

#if __riscv_xlen == 32
#define __nds__aes32dsi(a, b, c)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes32dsi' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_aes32dsi' instead")                                   \
  (__builtin_riscv_aes32dsi (a, b, c))
#define __nds__aes32dsmi(a, b, c)                                              \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes32dsmi' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_aes32dsmi' instead")                                  \
  (__builtin_riscv_aes32dsmi (a, b, c))
#endif

#if __riscv_xlen == 64
#define __nds__aes64ds(a, b)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64ds' will be deprecated in next 2 AndeSight release, please " \
    "use '__riscv_aes64ds' instead")                                           \
  (__builtin_riscv_aes64ds (a, b))
#define __nds__aes64dsm(a, b)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64dsm' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_aes64dsm' instead")                                   \
  (__builtin_riscv_aes64dsm (a, b))
#define __nds__aes64im(a)                                                      \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64im' will be deprecated in next 2 AndeSight release, please " \
    "use '__riscv_aes64im' instead")                                           \
  (__builtin_riscv_aes64im (a))
#endif

// Intrinsics for Zknd & Zkne

#if __riscv_xlen == 64
#define __nds__aes64ks1i(a, b)                                                 \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64ks1i' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_aes64ks1i' instead")                                  \
  (__builtin_riscv_aes64ks1i (a, b))
#define __nds__aes64ks2(a, b)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64ks2' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_aes64ks2' instead")                                   \
  (__builtin_riscv_aes64ks2 (a, b))
#endif

// Intrinsics for Zkne

#if __riscv_xlen == 32
#define __nds__aes32esi(a, b, c)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes32esi' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_aes32esi' instead")                                   \
  (__builtin_riscv_aes32esi (a, b, c))
#define __nds__aes32esmi(a, b, c)                                              \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes32esmi' will be deprecated in next 2 AndeSight release, "      \
    "please use '__riscv_aes32esmi' instead")                                  \
  (__builtin_riscv_aes32esmi (a, b, c))
#endif

#if __riscv_xlen == 64
#define __nds__aes64es(a, b)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64es' will be deprecated in next 2 AndeSight release, please " \
    "use '__riscv_aes64es' instead")                                           \
  (__builtin_riscv_aes64es (a, b))
#define __nds__aes64esm(a, b)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__aes64esm' will be deprecated in next 2 AndeSight release, "       \
    "please use '__riscv_aes64esm' instead")                                   \
  (__builtin_riscv_aes64esm (a, b))
#endif

// Intrinsics for Zknh

#define __nds__sha256sig0(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha256sig0' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha256sig0' instead")                                 \
  (__builtin_riscv_sha256sig0 (a))
#define __nds__sha256sig1(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha256sig1' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha256sig1' instead")                                 \
  (__builtin_riscv_sha256sig1 (a))
#define __nds__sha256sum0(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha256sum0' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha256sum0' instead")                                 \
  (__builtin_riscv_sha256sum0 (a))
#define __nds__sha256sum1(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha256sum1' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha256sum1' instead")                                 \
  (__builtin_riscv_sha256sum1 (a))

#if __riscv_xlen == 32
#define __nds__sha512sig0h(a, b)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sig0h' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_sha512sig0h' instead")                                \
  (__builtin_riscv_sha512sig0h (a, b))
#define __nds__sha512sig0l(a, b)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sig0l' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_sha512sig0l' instead")                                \
  (__builtin_riscv_sha512sig0l (a, b))
#define __nds__sha512sig1h(a, b)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sig1h' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_sha512sig1h' instead")                                \
  (__builtin_riscv_sha512sig1h (a, b))
#define __nds__sha512sig1l(a, b)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sig1l' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_sha512sig1l' instead")                                \
  (__builtin_riscv_sha512sig1l (a, b))
#define __nds__sha512sum0r(a, b)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sum0r' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_sha512sum0r' instead")                                \
  (__builtin_riscv_sha512sum0r (a, b))
#define __nds__sha512sum1r(a, b)                                               \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sum1r' will be deprecated in next 2 AndeSight release, "    \
    "please use '__riscv_sha512sum1r' instead")                                \
  (__builtin_riscv_sha512sum1r (a, b))
#endif

#if __riscv_xlen == 64
#define __nds__sha512sig0(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sig0' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha512sig0' instead")                                 \
  (__builtin_riscv_sha512sig0 (a))
#define __nds__sha512sig1(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sig1' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha512sig1' instead")                                 \
  (__builtin_riscv_sha512sig1 (a))
#define __nds__sha512sum0(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sum0' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha512sum0' instead")                                 \
  (__builtin_riscv_sha512sum0 (a))
#define __nds__sha512sum1(a)                                                   \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sha512sum1' will be deprecated in next 2 AndeSight release, "     \
    "please use '__riscv_sha512sum1' instead")                                 \
  (__builtin_riscv_sha512sum1 (a))
#endif

// Intrinsics for Zksed

#define __nds__sm4ed(a, b, c)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sm4ed' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_sm4ed' instead")                                             \
  (__builtin_riscv_sm4ed (a, b, c))
#define __nds__sm4ks(a, b, c)                                                  \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sm4ks' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_sm4ks' instead")                                             \
  (__builtin_riscv_sm4ks (a, b, c))

// Intrinsics for Zksh

#define __nds__sm3p0(a)                                                        \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sm3p0' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_sm3p0' instead")                                             \
  (__builtin_riscv_sm3p0 (a))
#define __nds__sm3p1(a)                                                        \
  __DEPRECATED_NDS_MACRO (                                                     \
    "'__nds__sm3p1' will be deprecated in next 2 AndeSight release, please "   \
    "use '__riscv_sm3p1' instead")                                             \
  (__builtin_riscv_sm3p1 (a))

// Intrinsics for prefetch (Zicmop and Zihintntl)

#define __nds__prefetch(a, b, c, d) (__builtin_riscv_prefetch (a, b, c, d))

  // Intrinsics for DSP extension

  typedef signed char int8x4_t __attribute__ ((vector_size (4)));
typedef signed char int8x8_t __attribute__((vector_size(8)));
typedef short int16x2_t __attribute__((vector_size(4)));
typedef short int16x4_t __attribute__((vector_size(8)));
typedef short int16x8_t __attribute__((vector_size(16)));
typedef int int32x2_t __attribute__((vector_size(8)));
typedef int int32x4_t __attribute__((vector_size(16)));
typedef unsigned char uint8x4_t __attribute__((vector_size(4)));
typedef unsigned char uint8x8_t __attribute__((vector_size(8)));
typedef unsigned short uint16x2_t __attribute__((vector_size(4)));
typedef unsigned short uint16x4_t __attribute__((vector_size(8)));
typedef unsigned short uint16x8_t __attribute__((vector_size(16)));
typedef unsigned int uint32x2_t __attribute__((vector_size(8)));
typedef unsigned int uint32x4_t __attribute__((vector_size(16)));

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

/* intrinsic has the same vector argument types on RV32 and RV64. */
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

#define __nds__v_kwmmul(a, b) \
  (__builtin_riscv_v_kwmmul ((a), (b)))
#define __nds__v_kwmmul_u(a, b) \
  (__builtin_riscv_v_kwmmul_u ((a), (b)))
#define __nds__v_kmmac(r, a, b) \
  (__builtin_riscv_v_kmmac ((r), (a), (b)))
#define __nds__v_kmmac_u(r, a, b) \
  (__builtin_riscv_v_kmmac_u ((r), (a), (b)))
#define __nds__v_kmmsb(r, a, b) \
  (__builtin_riscv_v_kmmsb ((r), (a), (b)))
#define __nds__v_kmmsb_u(r, a, b) \
  (__builtin_riscv_v_kmmsb_u ((r), (a), (b)))

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

#if __riscv_xlen == 64
#define __nds__v_clz32(a) \
  (__builtin_riscv_v_clz32 ((a)))
#define __nds__v_sclip32(a, imm) \
  (__builtin_riscv_v_sclip32 ((a), (imm)))
#define __nds__v_uclip32(a, imm) \
  (__builtin_riscv_v_uclip32 ((a), (imm)))
#define __nds__v_clrs32(a) \
  (__builtin_riscv_v_clrs32 ((a)))
#define __nds__v_clo32(a) \
  (__builtin_riscv_v_clo32 ((a)))
#define __nds__v_smar64(r, a, b) \
  (__builtin_riscv_v_smar64 ((r), (a), (b)))
#define __nds__v_smsr64(r, a, b) \
  (__builtin_riscv_v_smsr64 ((r), (a), (b)))
#define __nds__v_umar64(r, a, b) \
  (__builtin_riscv_v_umar64 ((r), (a), (b)))
#define __nds__v_umsr64(r, a, b) \
  (__builtin_riscv_v_umsr64 ((r), (a), (b)))
#define __nds__v_kmar64(r, a, b) \
  (__builtin_riscv_v_kmar64 ((r), (a), (b)))
#define __nds__v_kmsr64(r, a, b) \
  (__builtin_riscv_v_kmsr64 ((r), (a), (b)))
#define __nds__v_ukmar64(r, a, b) \
  (__builtin_riscv_v_ukmar64 ((r), (a), (b)))
#define __nds__v_ukmsr64(r, a, b) \
  (__builtin_riscv_v_ukmsr64 ((r), (a), (b)))
#define __nds__v_smmul(a, b) \
  (__builtin_riscv_v_smmul ((a), (b)))
#define __nds__v_smmul_u(a, b) \
  (__builtin_riscv_v_smmul_u ((a), (b)))
#define __nds__v_uadd32(a, b) \
  (__builtin_riscv_v_uadd32 ((a), (b)))
#define __nds__v_sadd32(a, b) \
  (__builtin_riscv_v_sadd32 ((a), (b)))
#define __nds__v_radd32(a, b) \
  (__builtin_riscv_v_radd32 ((a), (b)))
#define __nds__v_uradd32(a, b) \
  (__builtin_riscv_v_uradd32 ((a), (b)))
#define __nds__v_kadd32(a, b) \
  (__builtin_riscv_v_kadd32 ((a), (b)))
#define __nds__v_ukadd32(a, b) \
  (__builtin_riscv_v_ukadd32 ((a), (b)))
#define __nds__v_usub32(a, b) \
  (__builtin_riscv_v_usub32 ((a), (b)))
#define __nds__v_ssub32(a, b) \
  (__builtin_riscv_v_ssub32 ((a), (b)))
#define __nds__v_rsub32(a, b) \
  (__builtin_riscv_v_rsub32 ((a), (b)))
#define __nds__v_ursub32(a, b) \
  (__builtin_riscv_v_ursub32 ((a), (b)))
#define __nds__v_ksub32(a, b) \
  (__builtin_riscv_v_ksub32 ((a), (b)))
#define __nds__v_uksub32(a, b) \
  (__builtin_riscv_v_uksub32 ((a), (b)))
#define __nds__v_ucras32(a, b) \
  (__builtin_riscv_v_ucras32 ((a), (b)))
#define __nds__v_scras32(a, b) \
  (__builtin_riscv_v_scras32 ((a), (b)))
#define __nds__v_ucrsa32(a, b) \
  (__builtin_riscv_v_ucrsa32 ((a), (b)))
#define __nds__v_scrsa32(a, b) \
  (__builtin_riscv_v_scrsa32 ((a), (b)))
#define __nds__v_rcras32(a, b) \
  (__builtin_riscv_v_rcras32 ((a), (b)))
#define __nds__v_rcrsa32(a, b) \
  (__builtin_riscv_v_rcrsa32 ((a), (b)))
#define __nds__v_urcras32(a, b) \
  (__builtin_riscv_v_urcras32 ((a), (b)))
#define __nds__v_urcrsa32(a, b) \
  (__builtin_riscv_v_urcrsa32 ((a), (b)))
#define __nds__v_kcras32(a, b) \
  (__builtin_riscv_v_kcras32 ((a), (b)))
#define __nds__v_kcrsa32(a, b) \
  (__builtin_riscv_v_kcrsa32 ((a), (b)))
#define __nds__v_ukcras32(a, b) \
  (__builtin_riscv_v_ukcras32 ((a), (b)))
#define __nds__v_ukcrsa32(a, b) \
  (__builtin_riscv_v_ukcrsa32 ((a), (b)))
#define __nds__v_ustas32(a, b) \
  (__builtin_riscv_v_ustas32 ((a), (b)))
#define __nds__v_sstas32(a, b) \
  (__builtin_riscv_v_sstas32 ((a), (b)))
#define __nds__v_ustsa32(a, b) \
  (__builtin_riscv_v_ustsa32 ((a), (b)))
#define __nds__v_sstsa32(a, b) \
  (__builtin_riscv_v_sstsa32 ((a), (b)))
#define __nds__v_rstas32(a, b) \
  (__builtin_riscv_v_rstas32 ((a), (b)))
#define __nds__v_rstsa32(a, b) \
  (__builtin_riscv_v_rstsa32 ((a), (b)))
#define __nds__v_urstas32(a, b) \
  (__builtin_riscv_v_urstas32 ((a), (b)))
#define __nds__v_urstsa32(a, b) \
  (__builtin_riscv_v_urstsa32 ((a), (b)))
#define __nds__v_kstas32(a, b) \
  (__builtin_riscv_v_kstas32 ((a), (b)))
#define __nds__v_kstsa32(a, b) \
  (__builtin_riscv_v_kstsa32 ((a), (b)))
#define __nds__v_ukstas32(a, b) \
  (__builtin_riscv_v_ukstas32 ((a), (b)))
#define __nds__v_ukstsa32(a, b) \
  (__builtin_riscv_v_ukstsa32 ((a), (b)))
#define __nds__v_sra32(a, b) \
  (__builtin_riscv_v_sra32 ((a), (b)))
#define __nds__v_sra32_u(a, b) \
  (__builtin_riscv_v_sra32_u ((a), (b)))
#define __nds__v_srl32(a, b) \
  (__builtin_riscv_v_srl32 ((a), (b)))
#define __nds__v_srl32_u(a, b) \
  (__builtin_riscv_v_srl32_u ((a), (b)))
#define __nds__v_sll32(a, b) \
  (__builtin_riscv_v_sll32 ((a), (b)))
#define __nds__v_ksll32(a, b) \
  (__builtin_riscv_v_ksll32 ((a), (b)))
#define __nds__v_kslra32(a, b) \
  (__builtin_riscv_v_kslra32 ((a), (b)))
#define __nds__v_kslra32_u(a, b) \
  (__builtin_riscv_v_kslra32_u ((a), (b)))
#define __nds__v_smin32(a, b) \
  (__builtin_riscv_v_smin32 ((a), (b)))
#define __nds__v_umin32(a, b) \
  (__builtin_riscv_v_umin32 ((a), (b)))
#define __nds__v_smax32(a, b) \
  (__builtin_riscv_v_smax32 ((a), (b)))
#define __nds__v_umax32(a, b) \
  (__builtin_riscv_v_umax32 ((a), (b)))
#define __nds__v_kabs32(a) \
  (__builtin_riscv_v_kabs32 ((a)))
#define __nds__v_khmbb16(a, b) \
  (__builtin_riscv_v_khmbb16 ((a), (b)))
#define __nds__v_khmbt16(a, b) \
  (__builtin_riscv_v_khmbt16 ((a), (b)))
#define __nds__v_khmtt16(a, b) \
  (__builtin_riscv_v_khmtt16 ((a), (b)))
#define __nds__v_kdmbb16(a, b) \
  (__builtin_riscv_v_kdmbb16 ((a), (b)))
#define __nds__v_kdmbt16(a, b) \
  (__builtin_riscv_v_kdmbt16 ((a), (b)))
#define __nds__v_kdmtt16(a, b) \
  (__builtin_riscv_v_kdmtt16 ((a), (b)))
#define __nds__v_kdmabb16(r, a, b) \
  (__builtin_riscv_v_kdmabb16 ((r), (a), (b)))
#define __nds__v_kdmabt16(r, a, b) \
  (__builtin_riscv_v_kdmabt16 ((r), (a), (b)))
#define __nds__v_kdmatt16(r, a, b) \
  (__builtin_riscv_v_kdmatt16 ((r), (a), (b)))
#define __nds__v_smbb32(a, b) \
  (__builtin_riscv_v_smbb32 ((a), (b)))
#define __nds__v_smbt32(a, b) \
  (__builtin_riscv_v_smbt32 ((a), (b)))
#define __nds__v_smtt32(a, b) \
  (__builtin_riscv_v_smtt32 ((a), (b)))
#define __nds__v_kmabb32(r, a, b) \
  (__builtin_riscv_v_kmabb32 ((r), (a), (b)))
#define __nds__v_kmabt32(r, a, b) \
  (__builtin_riscv_v_kmabt32 ((r), (a), (b)))
#define __nds__v_kmatt32(r, a, b) \
  (__builtin_riscv_v_kmatt32 ((r), (a), (b)))
#define __nds__v_kmda32(a, b) \
  (__builtin_riscv_v_kmda32 ((a), (b)))
#define __nds__v_kmxda32(a, b) \
  (__builtin_riscv_v_kmxda32 ((a), (b)))
#define __nds__v_kmada32(r, a, b) \
  (__builtin_riscv_v_kmada32 ((r), (a), (b)))
#define __nds__v_kmaxda32(r, a, b) \
  (__builtin_riscv_v_kmaxda32 ((r), (a), (b)))
#define __nds__v_kmads32(r, a, b) \
  (__builtin_riscv_v_kmads32 ((r), (a), (b)))
#define __nds__v_kmadrs32(r, a, b) \
  (__builtin_riscv_v_kmadrs32 ((r), (a), (b)))
#define __nds__v_kmaxds32(r, a, b) \
  (__builtin_riscv_v_kmaxds32 ((r), (a), (b)))
#define __nds__v_kmsda32(r, a, b) \
  (__builtin_riscv_v_kmsda32 ((r), (a), (b)))
#define __nds__v_kmsxda32(r, a, b) \
  (__builtin_riscv_v_kmsxda32 ((r), (a), (b)))
#define __nds__v_smds32(a, b) \
  (__builtin_riscv_v_smds32 ((a), (b)))
#define __nds__v_smdrs32(a, b) \
  (__builtin_riscv_v_smdrs32 ((a), (b)))
#define __nds__v_smxds32(a, b) \
  (__builtin_riscv_v_smxds32 ((a), (b)))
#define __nds__v_pkbb32(a, b) \
  (__builtin_riscv_v_pkbb32 ((a), (b)))
#define __nds__v_pkbt32(a, b) \
  (__builtin_riscv_v_pkbt32 ((a), (b)))
#define __nds__v_pktb32(a, b) \
  (__builtin_riscv_v_pktb32 ((a), (b)))
#define __nds__v_pktt32(a, b) \
  (__builtin_riscv_v_pktt32 ((a), (b)))
#endif

/* Support __rv__ prfix macro for DSP instructions */
#define __rv__add16 __nds__add16
#define __rv__radd16 __nds__radd16
#define __rv__uradd16 __nds__uradd16
#define __rv__kadd16 __nds__kadd16
#define __rv__ukadd16 __nds__ukadd16
#define __rv__sub16 __nds__sub16
#define __rv__rsub16 __nds__rsub16
#define __rv__ursub16 __nds__ursub16
#define __rv__ksub16 __nds__ksub16
#define __rv__uksub16 __nds__uksub16
#define __rv__cras16 __nds__cras16
#define __rv__rcras16 __nds__rcras16
#define __rv__urcras16 __nds__urcras16
#define __rv__kcras16 __nds__kcras16
#define __rv__ukcras16 __nds__ukcras16
#define __rv__crsa16 __nds__crsa16
#define __rv__rcrsa16 __nds__rcrsa16
#define __rv__urcrsa16 __nds__urcrsa16
#define __rv__kcrsa16 __nds__kcrsa16
#define __rv__ukcrsa16 __nds__ukcrsa16
#define __rv__stas16 __nds__stas16
#define __rv__rstas16 __nds__rstas16
#define __rv__urstas16 __nds__urstas16
#define __rv__kstas16 __nds__kstas16
#define __rv__ukstas16 __nds__ukstas16
#define __rv__stsa16 __nds__stsa16
#define __rv__rstsa16 __nds__rstsa16
#define __rv__urstsa16 __nds__urstsa16
#define __rv__kstsa16 __nds__kstsa16
#define __rv__ukstsa16 __nds__ukstsa16
#define __rv__add8 __nds__add8
#define __rv__radd8 __nds__radd8
#define __rv__uradd8 __nds__uradd8
#define __rv__kadd8 __nds__kadd8
#define __rv__ukadd8 __nds__ukadd8
#define __rv__sub8 __nds__sub8
#define __rv__rsub8 __nds__rsub8
#define __rv__ursub8 __nds__ursub8
#define __rv__ksub8 __nds__ksub8
#define __rv__uksub8 __nds__uksub8

#define __rv__sra16 __nds__sra16
#define __rv__sra16_u __nds__sra16_u
#define __rv__srl16 __nds__srl16
#define __rv__srl16_u __nds__srl16_u
#define __rv__sll16 __nds__sll16
#define __rv__ksll16 __nds__ksll16
#define __rv__kslra16 __nds__kslra16
#define __rv__kslra16_u __nds__kslra16_u

#define __rv__cmpeq16 __nds__cmpeq16
#define __rv__scmplt16 __nds__scmplt16
#define __rv__scmple16 __nds__scmple16
#define __rv__ucmplt16 __nds__ucmplt16
#define __rv__ucmple16 __nds__ucmple16

#define __rv__cmpeq8 __nds__cmpeq8
#define __rv__scmplt8 __nds__scmplt8
#define __rv__scmple8 __nds__scmple8
#define __rv__ucmplt8 __nds__ucmplt8
#define __rv__ucmple8 __nds__ucmple8

#define __rv__smin16 __nds__smin16
#define __rv__umin16 __nds__umin16
#define __rv__smax16 __nds__smax16
#define __rv__umax16 __nds__umax16
#define __rv__sclip16 __nds__sclip16
#define __rv__uclip16 __nds__uclip16
#define __rv__khm16 __nds__khm16
#define __rv__khmx16 __nds__khmx16
#define __rv__khm8 __nds__khm8
#define __rv__khmx8 __nds__khmx8
#define __rv__kabs16 __nds__kabs16
#define __rv__smin8 __nds__smin8
#define __rv__umin8 __nds__umin8
#define __rv__smax8 __nds__smax8
#define __rv__umax8 __nds__umax8
#define __rv__kabs8 __nds__kabs8

#define __rv__sunpkd810 __nds__sunpkd810
#define __rv__sunpkd820 __nds__sunpkd820
#define __rv__sunpkd830 __nds__sunpkd830
#define __rv__sunpkd831 __nds__sunpkd831
#define __rv__sunpkd832 __nds__sunpkd832

#define __rv__zunpkd810 __nds__zunpkd810
#define __rv__zunpkd820 __nds__zunpkd820
#define __rv__zunpkd830 __nds__zunpkd830
#define __rv__zunpkd831 __nds__zunpkd831
#define __rv__zunpkd832 __nds__zunpkd832

#define __rv__raddw __nds__raddw
#define __rv__uraddw __nds__uraddw
#define __rv__rsubw __nds__rsubw
#define __rv__ursubw __nds__ursubw

#define __rv__sra_u __nds__sra_u
#define __rv__ksllw __nds__ksllw
#define __rv__pkbb16 __nds__pkbb16
#define __rv__pkbt16 __nds__pkbt16
#define __rv__pktb16 __nds__pktb16
#define __rv__pktt16 __nds__pktt16
#define __rv__pkbb32 __nds__pkbb32
#define __rv__pkbt32 __nds__pkbt32
#define __rv__pktb32 __nds__pktb32
#define __rv__pktt32 __nds__pktt32

#define __rv__smmul __nds__smmul
#define __rv__smmul_u __nds__smmul_u
#define __rv__kmmac __nds__kmmac
#define __rv__kmmac_u __nds__kmmac_u
#define __rv__kmmsb __nds__kmmsb
#define __rv__kmmsb_u __nds__kmmsb_u
#define __rv__kwmmul __nds__kwmmul
#define __rv__kwmmul_u __nds__kwmmul_u

#define __rv__smmwb __nds__smmwb
#define __rv__smmwb_u __nds__smmwb_u
#define __rv__smmwt __nds__smmwt
#define __rv__smmwt_u __nds__smmwt_u
#define __rv__kmmwb2 __nds__kmmwb2
#define __rv__kmmwb2_u __nds__kmmwb2_u
#define __rv__kmmwt2 __nds__kmmwt2
#define __rv__kmmwt2_u __nds__kmmwt2_u
#define __rv__kmmawb __nds__kmmawb
#define __rv__kmmawb_u __nds__kmmawb_u
#define __rv__kmmawt __nds__kmmawt
#define __rv__kmmawt_u __nds__kmmawt_u
#define __rv__kmmawb2 __nds__kmmawb2
#define __rv__kmmawb2_u __nds__kmmawb2_u
#define __rv__kmmawt2 __nds__kmmawt2
#define __rv__kmmawt2_u __nds__kmmawt2_u
#define __rv__smbb16 __nds__smbb16
#define __rv__smbt16 __nds__smbt16
#define __rv__smtt16 __nds__smtt16
#define __rv__kmda __nds__kmda
#define __rv__kmxda __nds__kmxda
#define __rv__smds __nds__smds
#define __rv__smdrs __nds__smdrs
#define __rv__smxds __nds__smxds
#define __rv__kmabb __nds__kmabb
#define __rv__kmabt __nds__kmabt
#define __rv__kmatt __nds__kmatt
#define __rv__kmada __nds__kmada
#define __rv__kmaxda __nds__kmaxda
#define __rv__kmads __nds__kmads
#define __rv__kmadrs __nds__kmadrs
#define __rv__kmaxds __nds__kmaxds
#define __rv__kmsda __nds__kmsda
#define __rv__kmsxda __nds__kmsxda
#define __rv__smal __nds__smal

#define __rv__bitrev __nds__bitrev
#define __rv__wext __nds__wext
#define __rv__bpick __nds__bpick
#define __rv__insb __nds__insb

#define __rv__sadd64 __nds__sadd64
#define __rv__uadd64 __nds__uadd64
#define __rv__radd64 __nds__radd64
#define __rv__uradd64 __nds__uradd64
#define __rv__kadd64 __nds__kadd64
#define __rv__ukadd64 __nds__ukadd64
#define __rv__ssub64 __nds__ssub64
#define __rv__usub64 __nds__usub64
#define __rv__rsub64 __nds__rsub64
#define __rv__ursub64 __nds__ursub64
#define __rv__ksub64 __nds__ksub64
#define __rv__uksub64 __nds__uksub64

#define __rv__smalbb __nds__smalbb
#define __rv__smalbt __nds__smalbt
#define __rv__smaltt __nds__smaltt

#define __rv__smalda __nds__smalda
#define __rv__smalxda __nds__smalxda
#define __rv__smalds __nds__smalds
#define __rv__smaldrs __nds__smaldrs
#define __rv__smalxds __nds__smalxds
#define __rv__smslda __nds__smslda
#define __rv__smslxda __nds__smslxda

#define __rv__uclip32 __nds__uclip32
#define __rv__sclip32 __nds__sclip32
#define __rv__kabsw __nds__kabsw

#define __rv__kaddw __nds__kaddw
#define __rv__kaddh __nds__kaddh
#define __rv__ksubw __nds__ksubw
#define __rv__ksubh __nds__ksubh
#define __rv__ukaddw __nds__ukaddw
#define __rv__ukaddh __nds__ukaddh
#define __rv__uksubw __nds__uksubw
#define __rv__uksubh __nds__uksubh
#define __rv__kdmbb __nds__kdmbb
#define __rv__kdmbt __nds__kdmbt
#define __rv__kdmtt __nds__kdmtt
#define __rv__khmbb __nds__khmbb
#define __rv__khmbt __nds__khmbt
#define __rv__khmtt __nds__khmtt
#define __rv__kslraw __nds__kslraw
#define __rv__kslraw_u __nds__kslraw_u
#define __rv__ave __nds__ave
#define __rv__maxw __nds__maxw
#define __rv__minw __nds__minw

#define __rv__sra8 __nds__sra8
#define __rv__sra8_u __nds__sra8_u
#define __rv__srl8 __nds__srl8
#define __rv__srl8_u __nds__srl8_u
#define __rv__sll8 __nds__sll8
#define __rv__ksll8 __nds__ksll8
#define __rv__kslra8 __nds__kslra8
#define __rv__kslra8_u __nds__kslra8_u

#define __rv__pbsad __nds__pbsad
#define __rv__pbsada __nds__pbsada
#define __rv__swap8 __nds__swap8
#define __rv__swap16 __nds__swap16
#define __rv__sclip8 __nds__sclip8
#define __rv__uclip8 __nds__uclip8

#define __rv__sraw_u __nds__sraw_u
#define __rv__add32 __nds__add32
#define __rv__radd32 __nds__radd32
#define __rv__uradd32 __nds__uradd32
#define __rv__kadd32 __nds__kadd32
#define __rv__ukadd32 __nds__ukadd32
#define __rv__sub32 __nds__sub32
#define __rv__rsub32 __nds__rsub32
#define __rv__ursub32 __nds__ursub32
#define __rv__ksub32 __nds__ksub32
#define __rv__uksub32 __nds__uksub32
#define __rv__cras32 __nds__cras32
#define __rv__crsa32 __nds__crsa32
#define __rv__rcras32 __nds__rcras32
#define __rv__rcrsa32 __nds__rcrsa32
#define __rv__urcras32 __nds__urcras32
#define __rv__urcrsa32 __nds__urcrsa32
#define __rv__kcras32 __nds__kcras32
#define __rv__kcrsa32 __nds__kcrsa32
#define __rv__ukcras32 __nds__ukcras32
#define __rv__ukcrsa32 __nds__ukcrsa32
#define __rv__stas32 __nds__stas32
#define __rv__stsa32 __nds__stsa32
#define __rv__rstas32 __nds__rstas32
#define __rv__rstsa32 __nds__rstsa32
#define __rv__urstas32 __nds__urstas32
#define __rv__urstsa32 __nds__urstsa32
#define __rv__kstas32 __nds__kstas32
#define __rv__kstsa32 __nds__kstsa32
#define __rv__ukstas32 __nds__ukstas32
#define __rv__ukstsa32 __nds__ukstsa32
#define __rv__sra32 __nds__sra32
#define __rv__sra32_u __nds__sra32_u
#define __rv__srl32 __nds__srl32
#define __rv__srl32_u __nds__srl32_u
#define __rv__sll32 __nds__sll32
#define __rv__ksll32 __nds__ksll32
#define __rv__kslra32 __nds__kslra32
#define __rv__kslra32_u __nds__kslra32_u
#define __rv__smin32 __nds__smin32
#define __rv__umin32 __nds__umin32
#define __rv__smax32 __nds__smax32
#define __rv__umax32 __nds__umax32
#define __rv__kabs32 __nds__kabs32
#define __rv__khmbb16 __nds__khmbb16
#define __rv__khmbt16 __nds__khmbt16
#define __rv__khmtt16 __nds__khmtt16
#define __rv__kdmbb16 __nds__kdmbb16
#define __rv__kdmbt16 __nds__kdmbt16
#define __rv__kdmtt16 __nds__kdmtt16
#define __rv__smbb32 __nds__smbb32
#define __rv__smbt32 __nds__smbt32
#define __rv__smtt32 __nds__smtt32
#define __rv__kmabb32 __nds__kmabb32
#define __rv__kmabt32 __nds__kmabt32
#define __rv__kmatt32 __nds__kmatt32

#define __rv__kmda32 __nds__kmda32
#define __rv__kmxda32 __nds__kmxda32
#define __rv__kmada32 __nds__kmada32
#define __rv__kmaxda32 __nds__kmaxda32
#define __rv__kmads32 __nds__kmads32
#define __rv__kmadrs32 __nds__kmadrs32
#define __rv__kmaxds32 __nds__kmaxds32
#define __rv__kmsda32 __nds__kmsda32
#define __rv__kmsxda32 __nds__kmsxda32
#define __rv__smds32 __nds__smds32
#define __rv__smdrs32 __nds__smdrs32
#define __rv__smxds32 __nds__smxds32
#define __rv__rdov __nds__rdov
#define __rv__clrov __nds__clrov

#define __rv__kdmabb16 __nds__kdmabb16
#define __rv__kdmabt16 __nds__kdmabt16
#define __rv__kdmatt16 __nds__kdmatt16
#define __rv__kdmabb __nds__kdmabb
#define __rv__kdmabt __nds__kdmabt
#define __rv__kdmatt __nds__kdmatt

#define __rv__smaqa __nds__smaqa
#define __rv__umaqa __nds__umaqa
#define __rv__smaqa_su __nds__smaqa_su

#define __rv__clrs8 __nds__clrs8
#define __rv__clrs16 __nds__clrs16
#define __rv__clrs32 __nds__clrs32
#define __rv__clo8 __nds__clo8
#define __rv__clo16 __nds__clo16
#define __rv__clo32 __nds__clo32
#define __rv__clz8 __nds__clz8
#define __rv__clz16 __nds__clz16
#define __rv__clz32 __nds__clz32
#define __rv__smul16 __nds__smul16
#define __rv__umul16 __nds__umul16
#define __rv__smul8 __nds__smul8
#define __rv__umul8 __nds__umul8
#define __rv__smulx16 __nds__smulx16
#define __rv__smulx8 __nds__smulx8
#define __rv__umulx16 __nds__umulx16
#define __rv__umulx8 __nds__umulx8
#define __rv__smar64 __nds__smar64
#define __rv__smsr64 __nds__smsr64
#define __rv__umar64 __nds__umar64
#define __rv__umsr64 __nds__umsr64
#define __rv__kmar64 __nds__kmar64
#define __rv__kmsr64 __nds__kmsr64
#define __rv__ukmar64 __nds__ukmar64
#define __rv__ukmsr64 __nds__ukmsr64

#if __riscv_xlen == 32
#define __rv__v_uadd16 __nds__v_uadd16
#define __rv__v_sadd16 __nds__v_sadd16
#define __rv__v_radd16 __nds__v_radd16
#define __rv__v_uradd16 __nds__v_uradd16
#define __rv__v_kadd16 __nds__v_kadd16
#define __rv__v_ukadd16 __nds__v_ukadd16
#define __rv__v_usub16 __nds__v_usub16
#define __rv__v_ssub16 __nds__v_ssub16
#define __rv__v_rsub16 __nds__v_rsub16
#define __rv__v_ursub16 __nds__v_ursub16
#define __rv__v_ksub16 __nds__v_ksub16
#define __rv__v_uksub16 __nds__v_uksub16
#define __rv__v_ucras16 __nds__v_ucras16
#define __rv__v_scras16 __nds__v_scras16
#define __rv__v_rcras16 __nds__v_rcras16
#define __rv__v_urcras16 __nds__v_urcras16
#define __rv__v_kcras16 __nds__v_kcras16
#define __rv__v_ukcras16 __nds__v_ukcras16
#define __rv__v_ucrsa16 __nds__v_ucrsa16
#define __rv__v_scrsa16 __nds__v_scrsa16
#define __rv__v_rcrsa16 __nds__v_rcrsa16
#define __rv__v_urcrsa16 __nds__v_urcrsa16
#define __rv__v_kcrsa16 __nds__v_kcrsa16
#define __rv__v_ukcrsa16 __nds__v_ukcrsa16
#define __rv__v_ustas16 __nds__v_ustas16
#define __rv__v_sstas16 __nds__v_sstas16
#define __rv__v_rstas16 __nds__v_rstas16
#define __rv__v_urstas16 __nds__v_urstas16
#define __rv__v_kstas16 __nds__v_kstas16
#define __rv__v_ukstas16 __nds__v_ukstas16
#define __rv__v_ustsa16 __nds__v_ustsa16
#define __rv__v_sstsa16 __nds__v_sstsa16
#define __rv__v_rstsa16 __nds__v_rstsa16
#define __rv__v_urstsa16 __nds__v_urstsa16
#define __rv__v_kstsa16 __nds__v_kstsa16
#define __rv__v_ukstsa16 __nds__v_ukstsa16
#define __rv__v_uadd8 __nds__v_uadd8
#define __rv__v_sadd8 __nds__v_sadd8
#define __rv__v_radd8 __nds__v_radd8
#define __rv__v_uradd8 __nds__v_uradd8
#define __rv__v_kadd8 __nds__v_kadd8
#define __rv__v_ukadd8 __nds__v_ukadd8
#define __rv__v_usub8 __nds__v_usub8
#define __rv__v_ssub8 __nds__v_ssub8
#define __rv__v_rsub8 __nds__v_rsub8
#define __rv__v_ursub8 __nds__v_ursub8
#define __rv__v_ksub8 __nds__v_ksub8
#define __rv__v_uksub8 __nds__v_uksub8
#define __rv__v_sra16 __nds__v_sra16
#define __rv__v_sra16_u __nds__v_sra16_u
#define __rv__v_srl16 __nds__v_srl16
#define __rv__v_srl16_u __nds__v_srl16_u
#define __rv__v_sll16 __nds__v_sll16
#define __rv__v_ksll16 __nds__v_ksll16
#define __rv__v_kslra16 __nds__v_kslra16
#define __rv__v_kslra16_u __nds__v_kslra16_u
#define __rv__v_scmpeq16 __nds__v_scmpeq16
#define __rv__v_ucmpeq16 __nds__v_ucmpeq16
#define __rv__v_scmplt16 __nds__v_scmplt16
#define __rv__v_scmple16 __nds__v_scmple16
#define __rv__v_ucmplt16 __nds__v_ucmplt16
#define __rv__v_ucmple16 __nds__v_ucmple16
#define __rv__v_scmpeq8 __nds__v_scmpeq8
#define __rv__v_ucmpeq8 __nds__v_ucmpeq8
#define __rv__v_scmplt8 __nds__v_scmplt8
#define __rv__v_scmple8 __nds__v_scmple8
#define __rv__v_ucmplt8 __nds__v_ucmplt8
#define __rv__v_ucmple8 __nds__v_ucmple8
#define __rv__v_smin16 __nds__v_smin16
#define __rv__v_umin16 __nds__v_umin16
#define __rv__v_smax16 __nds__v_smax16
#define __rv__v_umax16 __nds__v_umax16
#define __rv__v_sclip16 __nds__v_sclip16
#define __rv__v_uclip16 __nds__v_uclip16
#define __rv__v_khm16 __nds__v_khm16
#define __rv__v_khmx16 __nds__v_khmx16
#define __rv__v_khm8 __nds__v_khm8
#define __rv__v_khmx8 __nds__v_khmx8
#define __rv__v_kabs16 __nds__v_kabs16
#define __rv__v_smul16 __nds__v_smul16
#define __rv__v_smulx16 __nds__v_smulx16
#define __rv__v_umul16 __nds__v_umul16
#define __rv__v_umulx16 __nds__v_umulx16
#define __rv__v_smul8 __nds__v_smul8
#define __rv__v_smulx8 __nds__v_smulx8
#define __rv__v_umul8 __nds__v_umul8
#define __rv__v_umulx8 __nds__v_umulx8
#define __rv__v_smin8 __nds__v_smin8
#define __rv__v_umin8 __nds__v_umin8
#define __rv__v_smax8 __nds__v_smax8
#define __rv__v_umax8 __nds__v_umax8
#define __rv__v_kabs8 __nds__v_kabs8
#define __rv__v_sunpkd810 __nds__v_sunpkd810
#define __rv__v_sunpkd820 __nds__v_sunpkd820
#define __rv__v_sunpkd830 __nds__v_sunpkd830
#define __rv__v_sunpkd831 __nds__v_sunpkd831
#define __rv__v_sunpkd832 __nds__v_sunpkd832
#define __rv__v_zunpkd810 __nds__v_zunpkd810
#define __rv__v_zunpkd820 __nds__v_zunpkd820
#define __rv__v_zunpkd830 __nds__v_zunpkd830
#define __rv__v_zunpkd831 __nds__v_zunpkd831
#define __rv__v_zunpkd832 __nds__v_zunpkd832
#define __rv__v_pkbb16 __nds__v_pkbb16
#define __rv__v_pkbt16 __nds__v_pkbt16
#define __rv__v_pktb16 __nds__v_pktb16
#define __rv__v_pktt16 __nds__v_pktt16
#define __rv__v_smmwb __nds__v_smmwb
#define __rv__v_smmwb_u __nds__v_smmwb_u
#define __rv__v_smmwt __nds__v_smmwt
#define __rv__v_smmwt_u __nds__v_smmwt_u
#define __rv__v_kmmwb2 __nds__v_kmmwb2
#define __rv__v_kmmwb2_u __nds__v_kmmwb2_u
#define __rv__v_kmmwt2 __nds__v_kmmwt2
#define __rv__v_kmmwt2_u __nds__v_kmmwt2_u
#define __rv__v_kmmawb __nds__v_kmmawb
#define __rv__v_kmmawb_u __nds__v_kmmawb_u
#define __rv__v_kmmawt __nds__v_kmmawt
#define __rv__v_kmmawt_u __nds__v_kmmawt_u
#define __rv__v_kmmawb2 __nds__v_kmmawb2
#define __rv__v_kmmawb2_u __nds__v_kmmawb2_u
#define __rv__v_kmmawt2 __nds__v_kmmawt2
#define __rv__v_kmmawt2_u __nds__v_kmmawt2_u
#define __rv__v_smbb16 __nds__v_smbb16
#define __rv__v_smbt16 __nds__v_smbt16
#define __rv__v_smtt16 __nds__v_smtt16
#define __rv__v_kmda __nds__v_kmda
#define __rv__v_kmxda __nds__v_kmxda
#define __rv__v_smds __nds__v_smds
#define __rv__v_smdrs __nds__v_smdrs
#define __rv__v_smxds __nds__v_smxds
#define __rv__v_kmabb __nds__v_kmabb
#define __rv__v_kmabt __nds__v_kmabt
#define __rv__v_kmatt __nds__v_kmatt
#define __rv__v_kmada __nds__v_kmada
#define __rv__v_kmaxda __nds__v_kmaxda
#define __rv__v_kmads __nds__v_kmads
#define __rv__v_kmadrs __nds__v_kmadrs
#define __rv__v_kmaxds __nds__v_kmaxds
#define __rv__v_kmsda __nds__v_kmsda
#define __rv__v_kmsxda __nds__v_kmsxda
#define __rv__v_smal __nds__v_smal
#define __rv__v_smalbb __nds__v_smalbb
#define __rv__v_smalbt __nds__v_smalbt
#define __rv__v_smaltt __nds__v_smaltt
#define __rv__v_smalda __nds__v_smalda
#define __rv__v_smalxda __nds__v_smalxda
#define __rv__v_smalds __nds__v_smalds
#define __rv__v_smaldrs __nds__v_smaldrs
#define __rv__v_smalxds __nds__v_smalxds
#define __rv__v_smslda __nds__v_smslda
#define __rv__v_smslxda __nds__v_smslxda
#define __rv__v_sra8 __nds__v_sra8
#define __rv__v_sra8_u __nds__v_sra8_u
#define __rv__v_srl8 __nds__v_srl8
#define __rv__v_srl8_u __nds__v_srl8_u
#define __rv__v_sll8 __nds__v_sll8
#define __rv__v_ksll8 __nds__v_ksll8
#define __rv__v_kslra8 __nds__v_kslra8
#define __rv__v_kslra8_u __nds__v_kslra8_u
#define __rv__v_swap8 __nds__v_swap8
#define __rv__v_swap16 __nds__v_swap16
#define __rv__v_sclip8 __nds__v_sclip8
#define __rv__v_uclip8 __nds__v_uclip8
#define __rv__v_kdmabb __nds__v_kdmabb
#define __rv__v_kdmabt __nds__v_kdmabt
#define __rv__v_kdmatt __nds__v_kdmatt
#define __rv__v_smaqa __nds__v_smaqa
#define __rv__v_umaqa __nds__v_umaqa
#define __rv__v_smaqa_su __nds__v_smaqa_su
#define __rv__v_clrs8 __nds__v_clrs8
#define __rv__v_clrs16 __nds__v_clrs16
#define __rv__v_clo8 __nds__v_clo8
#define __rv__v_clo16 __nds__v_clo16
#define __rv__v_clz8 __nds__v_clz8
#define __rv__v_clz16 __nds__v_clz16
#define __rv__v_kdmbb __nds__v_kdmbb
#define __rv__v_kdmbt __nds__v_kdmbt
#define __rv__v_kdmtt __nds__v_kdmtt
#define __rv__v_khmbb __nds__v_khmbb
#define __rv__v_khmbt __nds__v_khmbt
#define __rv__v_khmtt __nds__v_khmtt
#define __rv__v_pbsad __nds__v_pbsad
#define __rv__v_pbsada __nds__v_pbsada
#else
#define __rv__v_smul16 __nds__v_smul16
#define __rv__v_umul16 __nds__v_umul16
#define __rv__v_smul8 __nds__v_smul8
#define __rv__v_umul8 __nds__v_umul8
#define __rv__v_smulx16 __nds__v_smulx16
#define __rv__v_smulx8 __nds__v_smulx8
#define __rv__v_umulx16 __nds__v_umulx16
#define __rv__v_umulx8 __nds__v_umulx8
#define __rv__v_uadd16 __nds__v_uadd16
#define __rv__v_sadd16 __nds__v_sadd16
#define __rv__v_radd16 __nds__v_radd16
#define __rv__v_uradd16 __nds__v_uradd16
#define __rv__v_kadd16 __nds__v_kadd16
#define __rv__v_ukadd16 __nds__v_ukadd16
#define __rv__v_usub16 __nds__v_usub16
#define __rv__v_ssub16 __nds__v_ssub16
#define __rv__v_rsub16 __nds__v_rsub16
#define __rv__v_ursub16 __nds__v_ursub16
#define __rv__v_ksub16 __nds__v_ksub16
#define __rv__v_uksub16 __nds__v_uksub16
#define __rv__v_ucras16 __nds__v_ucras16
#define __rv__v_scras16 __nds__v_scras16
#define __rv__v_rcras16 __nds__v_rcras16
#define __rv__v_urcras16 __nds__v_urcras16
#define __rv__v_kcras16 __nds__v_kcras16
#define __rv__v_ukcras16 __nds__v_ukcras16
#define __rv__v_ucrsa16 __nds__v_ucrsa16
#define __rv__v_scrsa16 __nds__v_scrsa16
#define __rv__v_rcrsa16 __nds__v_rcrsa16
#define __rv__v_urcrsa16 __nds__v_urcrsa16
#define __rv__v_kcrsa16 __nds__v_kcrsa16
#define __rv__v_ukcrsa16 __nds__v_ukcrsa16
#define __rv__v_ustas16 __nds__v_ustas16
#define __rv__v_sstas16 __nds__v_sstas16
#define __rv__v_rstas16 __nds__v_rstas16
#define __rv__v_urstas16 __nds__v_urstas16
#define __rv__v_kstas16 __nds__v_kstas16
#define __rv__v_ukstas16 __nds__v_ukstas16
#define __rv__v_ustsa16 __nds__v_ustsa16
#define __rv__v_sstsa16 __nds__v_sstsa16
#define __rv__v_rstsa16 __nds__v_rstsa16
#define __rv__v_urstsa16 __nds__v_urstsa16
#define __rv__v_kstsa16 __nds__v_kstsa16
#define __rv__v_ukstsa16 __nds__v_ukstsa16
#define __rv__v_uadd8 __nds__v_uadd8
#define __rv__v_sadd8 __nds__v_sadd8
#define __rv__v_radd8 __nds__v_radd8
#define __rv__v_uradd8 __nds__v_uradd8
#define __rv__v_kadd8 __nds__v_kadd8
#define __rv__v_ukadd8 __nds__v_ukadd8
#define __rv__v_usub8 __nds__v_usub8
#define __rv__v_ssub8 __nds__v_ssub8
#define __rv__v_rsub8 __nds__v_rsub8
#define __rv__v_ursub8 __nds__v_ursub8
#define __rv__v_ksub8 __nds__v_ksub8
#define __rv__v_uksub8 __nds__v_uksub8
#define __rv__v_sra16 __nds__v_sra16
#define __rv__v_sra16_u __nds__v_sra16_u
#define __rv__v_srl16 __nds__v_srl16
#define __rv__v_srl16_u __nds__v_srl16_u
#define __rv__v_sll16 __nds__v_sll16
#define __rv__v_ksll16 __nds__v_ksll16
#define __rv__v_kslra16 __nds__v_kslra16
#define __rv__v_kslra16_u __nds__v_kslra16_u
#define __rv__v_scmpeq16 __nds__v_scmpeq16
#define __rv__v_ucmpeq16 __nds__v_ucmpeq16
#define __rv__v_scmplt16 __nds__v_scmplt16
#define __rv__v_scmple16 __nds__v_scmple16
#define __rv__v_ucmplt16 __nds__v_ucmplt16
#define __rv__v_ucmple16 __nds__v_ucmple16
#define __rv__v_scmpeq8 __nds__v_scmpeq8
#define __rv__v_ucmpeq8 __nds__v_ucmpeq8
#define __rv__v_scmplt8 __nds__v_scmplt8
#define __rv__v_scmple8 __nds__v_scmple8
#define __rv__v_ucmplt8 __nds__v_ucmplt8
#define __rv__v_ucmple8 __nds__v_ucmple8
#define __rv__v_smin16 __nds__v_smin16
#define __rv__v_umin16 __nds__v_umin16
#define __rv__v_smax16 __nds__v_smax16
#define __rv__v_umax16 __nds__v_umax16
#define __rv__v_sclip16 __nds__v_sclip16
#define __rv__v_uclip16 __nds__v_uclip16
#define __rv__v_khm16 __nds__v_khm16
#define __rv__v_khmx16 __nds__v_khmx16
#define __rv__v_khm8 __nds__v_khm8
#define __rv__v_khmx8 __nds__v_khmx8
#define __rv__v_kabs16 __nds__v_kabs16
#define __rv__v_smul16 __nds__v_smul16
#define __rv__v_smulx16 __nds__v_smulx16
#define __rv__v_umul16 __nds__v_umul16
#define __rv__v_umulx16 __nds__v_umulx16
#define __rv__v_smul8 __nds__v_smul8
#define __rv__v_smulx8 __nds__v_smulx8
#define __rv__v_umul8 __nds__v_umul8
#define __rv__v_umulx8 __nds__v_umulx8
#define __rv__v_smin8 __nds__v_smin8
#define __rv__v_umin8 __nds__v_umin8
#define __rv__v_smax8 __nds__v_smax8
#define __rv__v_umax8 __nds__v_umax8
#define __rv__v_kabs8 __nds__v_kabs8
#define __rv__v_sunpkd810 __nds__v_sunpkd810
#define __rv__v_sunpkd820 __nds__v_sunpkd820
#define __rv__v_sunpkd830 __nds__v_sunpkd830
#define __rv__v_sunpkd831 __nds__v_sunpkd831
#define __rv__v_sunpkd832 __nds__v_sunpkd832
#define __rv__v_zunpkd810 __nds__v_zunpkd810
#define __rv__v_zunpkd820 __nds__v_zunpkd820
#define __rv__v_zunpkd830 __nds__v_zunpkd830
#define __rv__v_zunpkd831 __nds__v_zunpkd831
#define __rv__v_zunpkd832 __nds__v_zunpkd832
#define __rv__v_pkbb16 __nds__v_pkbb16
#define __rv__v_pkbt16 __nds__v_pkbt16
#define __rv__v_pktb16 __nds__v_pktb16
#define __rv__v_pktt16 __nds__v_pktt16
#define __rv__v_smmwb __nds__v_smmwb
#define __rv__v_smmwb_u __nds__v_smmwb_u
#define __rv__v_smmwt __nds__v_smmwt
#define __rv__v_smmwt_u __nds__v_smmwt_u
#define __rv__v_kmmwb2 __nds__v_kmmwb2
#define __rv__v_kmmwb2_u __nds__v_kmmwb2_u
#define __rv__v_kmmwt2 __nds__v_kmmwt2
#define __rv__v_kmmwt2_u __nds__v_kmmwt2_u
#define __rv__v_kmmawb __nds__v_kmmawb
#define __rv__v_kmmawb_u __nds__v_kmmawb_u
#define __rv__v_kmmawt __nds__v_kmmawt
#define __rv__v_kmmawt_u __nds__v_kmmawt_u
#define __rv__v_kmmawb2 __nds__v_kmmawb2
#define __rv__v_kmmawb2_u __nds__v_kmmawb2_u
#define __rv__v_kmmawt2 __nds__v_kmmawt2
#define __rv__v_kmmawt2_u __nds__v_kmmawt2_u
#define __rv__v_smbb16 __nds__v_smbb16
#define __rv__v_smbt16 __nds__v_smbt16
#define __rv__v_smtt16 __nds__v_smtt16
#define __rv__v_kmda __nds__v_kmda
#define __rv__v_kmxda __nds__v_kmxda
#define __rv__v_smds __nds__v_smds
#define __rv__v_smdrs __nds__v_smdrs
#define __rv__v_smxds __nds__v_smxds
#define __rv__v_kmabb __nds__v_kmabb
#define __rv__v_kmabt __nds__v_kmabt
#define __rv__v_kmatt __nds__v_kmatt
#define __rv__v_kmada __nds__v_kmada
#define __rv__v_kmaxda __nds__v_kmaxda
#define __rv__v_kmads __nds__v_kmads
#define __rv__v_kmadrs __nds__v_kmadrs
#define __rv__v_kmaxds __nds__v_kmaxds
#define __rv__v_kmsda __nds__v_kmsda
#define __rv__v_kmsxda __nds__v_kmsxda
#define __rv__v_smal __nds__v_smal
#define __rv__v_smalbb __nds__v_smalbb
#define __rv__v_smalbt __nds__v_smalbt
#define __rv__v_smaltt __nds__v_smaltt
#define __rv__v_smalda __nds__v_smalda
#define __rv__v_smalxda __nds__v_smalxda
#define __rv__v_smalds __nds__v_smalds
#define __rv__v_smaldrs __nds__v_smaldrs
#define __rv__v_smalxds __nds__v_smalxds
#define __rv__v_smslda __nds__v_smslda
#define __rv__v_smslxda __nds__v_smslxda
#define __rv__v_sra8 __nds__v_sra8
#define __rv__v_sra8_u __nds__v_sra8_u
#define __rv__v_srl8 __nds__v_srl8
#define __rv__v_srl8_u __nds__v_srl8_u
#define __rv__v_sll8 __nds__v_sll8
#define __rv__v_ksll8 __nds__v_ksll8
#define __rv__v_kslra8 __nds__v_kslra8
#define __rv__v_kslra8_u __nds__v_kslra8_u
#define __rv__v_sclip8 __nds__v_sclip8
#define __rv__v_uclip8 __nds__v_uclip8
#define __rv__v_uadd32 __nds__v_uadd32
#define __rv__v_sadd32 __nds__v_sadd32
#define __rv__v_radd32 __nds__v_radd32
#define __rv__v_uradd32 __nds__v_uradd32
#define __rv__v_kadd32 __nds__v_kadd32
#define __rv__v_ukadd32 __nds__v_ukadd32
#define __rv__v_usub32 __nds__v_usub32
#define __rv__v_ssub32 __nds__v_ssub32
#define __rv__v_rsub32 __nds__v_rsub32
#define __rv__v_ursub32 __nds__v_ursub32
#define __rv__v_ksub32 __nds__v_ksub32
#define __rv__v_uksub32 __nds__v_uksub32
#define __rv__v_ucras32 __nds__v_ucras32
#define __rv__v_scras32 __nds__v_scras32
#define __rv__v_ucrsa32 __nds__v_ucrsa32
#define __rv__v_scrsa32 __nds__v_scrsa32
#define __rv__v_rcras32 __nds__v_rcras32
#define __rv__v_rcrsa32 __nds__v_rcrsa32
#define __rv__v_urcras32 __nds__v_urcras32
#define __rv__v_urcrsa32 __nds__v_urcrsa32
#define __rv__v_kcras32 __nds__v_kcras32
#define __rv__v_kcrsa32 __nds__v_kcrsa32
#define __rv__v_ukcras32 __nds__v_ukcras32
#define __rv__v_ukcrsa32 __nds__v_ukcrsa32
#define __rv__v_ustas32 __nds__v_ustas32
#define __rv__v_sstas32 __nds__v_sstas32
#define __rv__v_ustsa32 __nds__v_ustsa32
#define __rv__v_sstsa32 __nds__v_sstsa32
#define __rv__v_rstas32 __nds__v_rstas32
#define __rv__v_rstsa32 __nds__v_rstsa32
#define __rv__v_urstas32 __nds__v_urstas32
#define __rv__v_urstsa32 __nds__v_urstsa32
#define __rv__v_kstas32 __nds__v_kstas32
#define __rv__v_kstsa32 __nds__v_kstsa32
#define __rv__v_ukstas32 __nds__v_ukstas32
#define __rv__v_ukstsa32 __nds__v_ukstsa32
#define __rv__v_sra32 __nds__v_sra32
#define __rv__v_sra32_u __nds__v_sra32_u
#define __rv__v_srl32 __nds__v_srl32
#define __rv__v_srl32_u __nds__v_srl32_u
#define __rv__v_sll32 __nds__v_sll32
#define __rv__v_ksll32 __nds__v_ksll32
#define __rv__v_kslra32 __nds__v_kslra32
#define __rv__v_kslra32_u __nds__v_kslra32_u
#define __rv__v_smin32 __nds__v_smin32
#define __rv__v_umin32 __nds__v_umin32
#define __rv__v_smax32 __nds__v_smax32
#define __rv__v_umax32 __nds__v_umax32
#define __rv__v_kabs32 __nds__v_kabs32
#define __rv__v_khmbb16 __nds__v_khmbb16
#define __rv__v_khmbt16 __nds__v_khmbt16
#define __rv__v_khmtt16 __nds__v_khmtt16
#define __rv__v_kdmbb16 __nds__v_kdmbb16
#define __rv__v_kdmbt16 __nds__v_kdmbt16
#define __rv__v_kdmtt16 __nds__v_kdmtt16
#define __rv__v_smbb32 __nds__v_smbb32
#define __rv__v_smbt32 __nds__v_smbt32
#define __rv__v_smtt32 __nds__v_smtt32
#define __rv__v_kmabb32 __nds__v_kmabb32
#define __rv__v_kmabt32 __nds__v_kmabt32
#define __rv__v_kmatt32 __nds__v_kmatt32
#define __rv__v_kmda32 __nds__v_kmda32
#define __rv__v_kmxda32 __nds__v_kmxda32
#define __rv__v_kmada32 __nds__v_kmada32
#define __rv__v_kmaxda32 __nds__v_kmaxda32
#define __rv__v_kmads32 __nds__v_kmads32
#define __rv__v_kmadrs32 __nds__v_kmadrs32
#define __rv__v_kmaxds32 __nds__v_kmaxds32
#define __rv__v_kmsda32 __nds__v_kmsda32
#define __rv__v_kmsxda32 __nds__v_kmsxda32
#define __rv__v_smds32 __nds__v_smds32
#define __rv__v_smdrs32 __nds__v_smdrs32
#define __rv__v_smxds32 __nds__v_smxds32
#define __rv__v_kdmabb __nds__v_kdmabb
#define __rv__v_kdmabt __nds__v_kdmabt
#define __rv__v_kdmatt __nds__v_kdmatt
#define __rv__v_kdmabb16 __nds__v_kdmabb16
#define __rv__v_kdmabt16 __nds__v_kdmabt16
#define __rv__v_kdmatt16 __nds__v_kdmatt16
#define __rv__v_smaqa __nds__v_smaqa
#define __rv__v_umaqa __nds__v_umaqa
#define __rv__v_smaqa_su __nds__v_smaqa_su
#define __rv__v_clrs8 __nds__v_clrs8
#define __rv__v_clrs16 __nds__v_clrs16
#define __rv__v_clrs32 __nds__v_clrs32
#define __rv__v_clo8 __nds__v_clo8
#define __rv__v_clo16 __nds__v_clo16
#define __rv__v_clo32 __nds__v_clo32
#define __rv__v_clz8 __nds__v_clz8
#define __rv__v_clz16 __nds__v_clz16
#define __rv__v_clz32 __nds__v_clz32
#define __rv__v_swap8 __nds__v_swap8
#define __rv__v_swap16 __nds__v_swap16
#define __rv__v_pkbb32 __nds__v_pkbb32
#define __rv__v_pkbt32 __nds__v_pkbt32
#define __rv__v_pktb32 __nds__v_pktb32
#define __rv__v_pktt32 __nds__v_pktt32
#define __rv__v_kdmbb __nds__v_kdmbb
#define __rv__v_kdmbt __nds__v_kdmbt
#define __rv__v_kdmtt __nds__v_kdmtt
#define __rv__v_khmbb __nds__v_khmbb
#define __rv__v_khmbt __nds__v_khmbt
#define __rv__v_khmtt __nds__v_khmtt
#define __rv__v_smmul __nds__v_smmul
#define __rv__v_smmul_u __nds__v_smmul_u
#define __rv__v_kwmmul __nds__v_kwmmul
#define __rv__v_kwmmul_u __nds__v_kwmmul_u
#define __rv__v_kmmac __nds__v_kmmac
#define __rv__v_kmmac_u __nds__v_kmmac_u
#define __rv__v_kmmsb __nds__v_kmmsb
#define __rv__v_kmmsb_u __nds__v_kmmsb_u
#define __rv__v_uclip32 __nds__v_uclip32
#define __rv__v_sclip32 __nds__v_sclip32
#define __rv__v_pbsad __nds__v_pbsad
#define __rv__v_pbsada __nds__v_pbsada

#define __rv__v_smar64 __nds__v_smar64
#define __rv__v_smsr64 __nds__v_smsr64
#define __rv__v_umar64 __nds__v_umar64
#define __rv__v_umsr64 __nds__v_umsr64
#define __rv__v_kmar64 __nds__v_kmar64
#define __rv__v_kmsr64 __nds__v_kmsr64
#define __rv__v_ukmar64 __nds__v_ukmar64
#define __rv__v_ukmsr64 __nds__v_ukmsr64
#endif // __riscv_xlen == 64

// End of DSP extensions.

#endif // _NDS_INTRINSIC_H
