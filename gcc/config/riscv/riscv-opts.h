/* Definition of RISC-V target for GNU compiler.
   Copyright (C) 2016-2022 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef GCC_RISCV_OPTS_H
#define GCC_RISCV_OPTS_H

enum riscv_abi_type {
  ABI_ILP32,
  ABI_ILP32E,
  ABI_ILP32F,
  ABI_ILP32D,
  ABI_LP64,
  ABI_LP64F,
  ABI_LP64D
};
extern enum riscv_abi_type riscv_abi;

enum riscv_code_model {
  CM_MEDLOW,
  CM_MEDANY,
  CM_PIC,
  CM_LARGE
};
extern enum riscv_code_model riscv_cmodel;

/* Multiply instruction configuration.  */
enum riscv_mul_type
{
  MUL_TYPE_AUTO,
  MUL_TYPE_FAST,
  MUL_TYPE_SLOW
};

enum riscv_isa_spec_class {
  ISA_SPEC_CLASS_NONE,

  ISA_SPEC_CLASS_2P2,
  ISA_SPEC_CLASS_20190608,
  ISA_SPEC_CLASS_20191213
};

extern enum riscv_isa_spec_class riscv_isa_spec;

/* Keep this list in sync with define_attr "tune" in riscv.md.  */
enum riscv_microarchitecture_type {
  generic,
  sifive_7,
  rocket,
  vicuna,
  vicuna2,
  kavalan,
  makatau,
  /* Used to mark the end of the processor table.  */
  riscv_none
};
extern enum riscv_microarchitecture_type riscv_microarchitecture;

enum riscv_align_data {
  riscv_align_data_type_xlen,
  riscv_align_data_type_natural
};

/* Where to get the canary for the stack protector.  */
enum stack_protector_guard {
  SSP_TLS,			/* per-thread canary in TLS block */
  SSP_GLOBAL			/* global canary */
};

/* RISC-V auto-vectorization preference.  */
enum riscv_autovec_preference_enum {
  NO_AUTOVEC,
  RVV_SCALABLE,
  RVV_FIXED_VLMAX
};

/* RISC-V auto-vectorization RVV LMUL.  */
enum riscv_autovec_lmul_enum {
  RVV_M1 = 1,
  RVV_M2 = 2,
  RVV_M4 = 4,
  RVV_M8 = 8
};

enum riscv_multilib_select_kind {
  /* Select multilib by builtin way.  */
  select_by_builtin,
  /* Select multilib by ABI, arch and code model.  */
  select_by_abi_arch_cmodel,
  /* Select multilib by ABI only.  */
  select_by_abi,
};

/* ENTITIES in mode switching.  */
enum riscv_entity
{
  RISCV_VXRM = 0,
  RISCV_FRM,
  MAX_RISCV_ENTITIES
};

#define MASK_ZICSR    (1 << 0)
#define MASK_ZIFENCEI (1 << 1)

#define TARGET_ZICSR    ((riscv_zi_subext & MASK_ZICSR) != 0)
#define TARGET_ZIFENCEI ((riscv_zi_subext & MASK_ZIFENCEI) != 0)

#define MASK_ZBA      (1 << 0)
#define MASK_ZBB      (1 << 1)
#define MASK_ZBC      (1 << 2)
#define MASK_ZBS      (1 << 3)

#define TARGET_ZBA    ((riscv_zb_subext & MASK_ZBA) != 0)
#define TARGET_ZBB    ((riscv_zb_subext & MASK_ZBB) != 0)
#define TARGET_ZBC    ((riscv_zb_subext & MASK_ZBC) != 0)
#define TARGET_ZBS    ((riscv_zb_subext & MASK_ZBS) != 0)

#define MASK_ZCA      (1 << 0)
#define MASK_ZCB      (1 << 1)
#define MASK_ZCD      (1 << 2)
#define MASK_ZCF      (1 << 3)
#define MASK_ZCMP     (1 << 4)
#define MASK_ZCMT     (1 << 5)
#define MASK_ZCMPE    (1 << 6)

#define TARGET_ZCA    ((riscv_zc_subext & MASK_ZCA) != 0)
#define TARGET_ZCB    ((riscv_zc_subext & MASK_ZCB) != 0)
#define TARGET_ZCD    ((riscv_zc_subext & MASK_ZCD) != 0)
#define TARGET_ZCF    ((riscv_zc_subext & MASK_ZCF) != 0)
#define TARGET_ZCMP   ((riscv_zc_subext & MASK_ZCMP) != 0)
#define TARGET_ZCMT   ((riscv_zc_subext & MASK_ZCMT) != 0)
#define TARGET_ZCMPE  ((riscv_zc_subext & MASK_ZCMPE) != 0)

#define MASK_ZBKB     (1 << 0)
#define MASK_ZBKC     (1 << 1)
#define MASK_ZBKX     (1 << 2)
#define MASK_ZKNE     (1 << 3)
#define MASK_ZKND     (1 << 4)
#define MASK_ZKNH     (1 << 5)
#define MASK_ZKR      (1 << 6)
#define MASK_ZKSED    (1 << 7)
#define MASK_ZKSH     (1 << 8)
#define MASK_ZKT      (1 << 9)

#define TARGET_ZBKB   ((riscv_zk_subext & MASK_ZBKB) != 0)
#define TARGET_ZBKC   ((riscv_zk_subext & MASK_ZBKC) != 0)
#define TARGET_ZBKX   ((riscv_zk_subext & MASK_ZBKX) != 0)
#define TARGET_ZKNE   ((riscv_zk_subext & MASK_ZKNE) != 0)
#define TARGET_ZKND   ((riscv_zk_subext & MASK_ZKND) != 0)
#define TARGET_ZKNH   ((riscv_zk_subext & MASK_ZKNH) != 0)
#define TARGET_ZKR    ((riscv_zk_subext & MASK_ZKR) != 0)
#define TARGET_ZKSED  ((riscv_zk_subext & MASK_ZKSED) != 0)
#define TARGET_ZKSH   ((riscv_zk_subext & MASK_ZKSH) != 0)
#define TARGET_ZKT    ((riscv_zk_subext & MASK_ZKT) != 0)

#define MASK_VECTOR_ELEN_32    (1 << 0)
#define MASK_VECTOR_ELEN_64    (1 << 1)
#define MASK_VECTOR_ELEN_FP_32 (1 << 2)
#define MASK_VECTOR_ELEN_FP_64 (1 << 3)
/* Align the bit index to riscv-vector-builtins.h.  */
#define MASK_VECTOR_ELEN_FP_16 (1 << 6)

#define TARGET_VECTOR_ELEN_32 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_32) != 0)
#define TARGET_VECTOR_ELEN_64 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_64) != 0)
#define TARGET_VECTOR_ELEN_FP_32 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_FP_32) != 0)
#define TARGET_VECTOR_ELEN_FP_64 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_FP_64) != 0)
#define TARGET_VECTOR_ELEN_FP_16 \
  ((riscv_vector_elen_flags & MASK_VECTOR_ELEN_FP_16) != 0)

#define MASK_ZVBB      (1 << 0)
#define MASK_ZVBC      (1 << 1)

#define TARGET_ZVBB    ((riscv_zvb_subext & MASK_ZVBB) != 0)
#define TARGET_ZVBC    ((riscv_zvb_subext & MASK_ZVBC) != 0)

#define MASK_ZVKG      (1 << 0)
#define MASK_ZVKNED    (1 << 1)
#define MASK_ZVKNHA    (1 << 2)
#define MASK_ZVKNHB    (1 << 3)
#define MASK_ZVKSED    (1 << 4)
#define MASK_ZVKSH     (1 << 5)
#define MASK_ZVKN      (1 << 6)
#define MASK_ZVKNC     (1 << 7)
#define MASK_ZVKNG     (1 << 8)
#define MASK_ZVKS      (1 << 9)
#define MASK_ZVKSC     (1 << 10)
#define MASK_ZVKSG     (1 << 11)
#define MASK_ZVKT      (1 << 12)

#define TARGET_ZVKG    ((riscv_zvk_subext & MASK_ZVKG) != 0)
#define TARGET_ZVKNED  ((riscv_zvk_subext & MASK_ZVKNED) != 0)
#define TARGET_ZVKNHA  ((riscv_zvk_subext & MASK_ZVKNHA) != 0)
#define TARGET_ZVKNHB  ((riscv_zvk_subext & MASK_ZVKNHB) != 0)
#define TARGET_ZVKSED  ((riscv_zvk_subext & MASK_ZVKSED) != 0)
#define TARGET_ZVKSH   ((riscv_zvk_subext & MASK_ZVKSH) != 0)
#define TARGET_ZVKN    ((riscv_zvk_subext & MASK_ZVKN) != 0)
#define TARGET_ZVKNC   ((riscv_zvk_subext & MASK_ZVKNC) != 0)
#define TARGET_ZVKNG   ((riscv_zvk_subext & MASK_ZVKNG) != 0)
#define TARGET_ZVKS    ((riscv_zvk_subext & MASK_ZVKS) != 0)
#define TARGET_ZVKSC   ((riscv_zvk_subext & MASK_ZVKSC) != 0)
#define TARGET_ZVKSG   ((riscv_zvk_subext & MASK_ZVKSG) != 0)
#define TARGET_ZVKT    ((riscv_zvk_subext & MASK_ZVKT) != 0)

#define MASK_ZVL32B    (1 <<  0)
#define MASK_ZVL64B    (1 <<  1)
#define MASK_ZVL128B   (1 <<  2)
#define MASK_ZVL256B   (1 <<  3)
#define MASK_ZVL512B   (1 <<  4)
#define MASK_ZVL1024B  (1 <<  5)
#define MASK_ZVL2048B  (1 <<  6)
#define MASK_ZVL4096B  (1 <<  7)
#define MASK_ZVL8192B  (1 <<  8)
#define MASK_ZVL16384B (1 <<  9)
#define MASK_ZVL32768B (1 << 10)
#define MASK_ZVL65536B (1 << 11)

#define TARGET_ZVL32B    ((riscv_zvl_flags & MASK_ZVL32B) != 0)
#define TARGET_ZVL64B    ((riscv_zvl_flags & MASK_ZVL64B) != 0)
#define TARGET_ZVL128B   ((riscv_zvl_flags & MASK_ZVL128B) != 0)
#define TARGET_ZVL256B   ((riscv_zvl_flags & MASK_ZVL256B) != 0)
#define TARGET_ZVL512B   ((riscv_zvl_flags & MASK_ZVL512B) != 0)
#define TARGET_ZVL1024B  ((riscv_zvl_flags & MASK_ZVL1024B) != 0)
#define TARGET_ZVL2048B  ((riscv_zvl_flags & MASK_ZVL2048B) != 0)
#define TARGET_ZVL4096B  ((riscv_zvl_flags & MASK_ZVL4096B) != 0)
#define TARGET_ZVL8192B  ((riscv_zvl_flags & MASK_ZVL8192B) != 0)
#define TARGET_ZVL16384B ((riscv_zvl_flags & MASK_ZVL16384B) != 0)
#define TARGET_ZVL32768B ((riscv_zvl_flags & MASK_ZVL32768B) != 0)
#define TARGET_ZVL65536B ((riscv_zvl_flags & MASK_ZVL65536B) != 0)

#define MASK_ZICBOZ   (1 << 0)
#define MASK_ZICBOM   (1 << 1)
#define MASK_ZICBOP   (1 << 2)

#define TARGET_ZICBOZ ((riscv_zicmo_subext & MASK_ZICBOZ) != 0)
#define TARGET_ZICBOM ((riscv_zicmo_subext & MASK_ZICBOM) != 0)
#define TARGET_ZICBOP ((riscv_zicmo_subext & MASK_ZICBOP) != 0)

#define MASK_ZFHMIN   (1 << 0)
#define MASK_ZFH      (1 << 1)
#define MASK_ZVFHMIN  (1 << 2)
#define MASK_ZVFH     (1 << 3)
#define MASK_ZFBFMIN  (1 << 4)
#define MASK_ZVFBFMIN (1 << 5)
#define MASK_ZVFBFWMA (1 << 6)

#define TARGET_ZFHMIN ((riscv_zf_subext & MASK_ZFHMIN) != 0)
#define TARGET_ZFH    ((riscv_zf_subext & MASK_ZFH) != 0)
#define TARGET_ZVFHMIN ((riscv_zf_subext & MASK_ZVFHMIN) != 0)
#define TARGET_ZVFH    ((riscv_zf_subext & MASK_ZVFH) != 0)
#define TARGET_ZFBFMIN  ((riscv_zf_subext & MASK_ZFBFMIN) != 0)
#define TARGET_ZVFBFMIN ((riscv_zf_subext & MASK_ZVFBFMIN) != 0)
#define TARGET_ZVFBFWMA ((riscv_zf_subext & MASK_ZVFBFWMA) != 0)

#define MASK_SVINVAL (1 << 0)
#define MASK_SVNAPOT (1 << 1)

#define TARGET_SVINVAL ((riscv_sv_subext & MASK_SVINVAL) != 0)
#define TARGET_SVNAPOT ((riscv_sv_subext & MASK_SVNAPOT) != 0)

#define MASK_BF16 (1 << 0)
#define TARGET_BF16 ((riscv_xnds_subext & MASK_BF16) != 0)

/* Bit of riscv_zvl_flags will set contintuly, N-1 bit will set if N-bit is
   set, e.g. MASK_ZVL64B has set then MASK_ZVL32B is set, so we can use
   popcount to caclulate the minimal VLEN.  */
#define TARGET_MIN_VLEN \
  ((riscv_zvl_flags == 0) \
   ? 0 \
   : 32 << (__builtin_popcount (riscv_zvl_flags) - 1))

/* The ict model defines the address generation strategy.  */
enum riscv_ict_model_type
{
  ICT_MODEL_TINY,
  ICT_MODEL_SMALL,
  ICT_MODEL_LARGE
};

extern enum riscv_ict_model_type riscv_ict_model;

enum riscv_zve
{
  ZVE32X,
  ZVE32F,
  ZVE64X,
  ZVE64F,
  ZVE64D,
};

#endif /* ! GCC_RISCV_OPTS_H */
