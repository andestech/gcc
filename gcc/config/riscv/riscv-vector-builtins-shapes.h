/* function_shape declaration for RISC-V 'V' Extension for GNU compiler.
   Copyright (C) 2022-2024 Free Software Foundation, Inc.
   Contributed by Ju-Zhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_RISCV_VECTOR_BUILTINS_SHAPES_H
#define GCC_RISCV_VECTOR_BUILTINS_SHAPES_H

namespace riscv_vector {

namespace shapes {
extern const function_shape *const vsetvl;
extern const function_shape *const vsetvlmax;
extern const function_shape *const loadstore;
extern const function_shape *const indexed_loadstore;
extern const function_shape *const th_loadstore_width;
extern const function_shape *const th_indexed_loadstore_width;
extern const function_shape *const alu;
extern const function_shape *const alu_frm;
extern const function_shape *const widen_alu;
extern const function_shape *const widen_alu_frm;
extern const function_shape *const no_mask_policy;
extern const function_shape *const return_mask;
extern const function_shape *const narrow_alu;
extern const function_shape *const narrow_alu_frm;
extern const function_shape *const move;
extern const function_shape *const mask_alu;
extern const function_shape *const reduc_alu;
extern const function_shape *const reduc_alu_frm;
extern const function_shape *const scalar_move;
extern const function_shape *const th_extract;
extern const function_shape *const vundefined;
extern const function_shape *const misc;
extern const function_shape *const vset;
extern const function_shape *const vget;
extern const function_shape *const vcreate;
extern const function_shape *const read_vl;
extern const function_shape *const fault_load;
extern const function_shape *const vlenb;
extern const function_shape *const seg_loadstore;
extern const function_shape *const seg_indexed_loadstore;
extern const function_shape *const seg_fault_load;
/* Below function_shape are Vectro Crypto*/
extern const function_shape *const crypto_vv;
extern const function_shape *const crypto_vi;
extern const function_shape *const crypto_vv_no_op_type;
extern const function_shape *const vcpopv;
/* Andes BF16 Mode Switch. In rvv-intrinsic-doc, some of non-overloaded base
 * will happen collide issue in BF/HF mode switch. E.g.,
 * __riscv_vfwadd_vv_f32m1(op1, op2, vl), type of op1 and op2 could BF or HF.
 * List these instance base vfwadd, vfwadd_frm, vfwsub, vfwsub_frm, vfwmul,
 * vfwmul_frm, vfwmacc, vfwmacc_frm, vfwnmacc, vfwnmacc_frm, vfwmsac,
 * vfwmsac_frm, vfwnmsac, vfwnmsac_frm, vfclass, vfcvt_x, vfcvt_xu, vfcvt_x_frm,
 * vfcvt_xu_frm, vfcvt_rtz_x, vfwcvt_rtz_xu, vfncvt_x, vfncvt_xu, vfncvt_rtz_x,
 * vfncvt_rtz_xu, vfncvt_x_frm, vfncvt_xu_frm. */
extern const function_shape *const alu_default_fp;
extern const function_shape *const alu_ms_fp;
extern const function_shape *const alu_default_bf;
extern const function_shape *const alu_ms_bf;
extern const function_shape *const alu_frm_default_fp;
extern const function_shape *const alu_frm_ms_fp;
extern const function_shape *const alu_frm_default_bf;
extern const function_shape *const alu_frm_ms_bf;
extern const function_shape *const widen_alu_default_fp;
extern const function_shape *const widen_alu_ms_fp;
extern const function_shape *const widen_alu_default_bf;
extern const function_shape *const widen_alu_ms_bf;
extern const function_shape *const widen_alu_frm_default_fp;
extern const function_shape *const widen_alu_frm_ms_fp;
extern const function_shape *const widen_alu_frm_default_bf;
extern const function_shape *const widen_alu_frm_ms_bf;
extern const function_shape *const narrow_alu_default_fp;
extern const function_shape *const narrow_alu_ms_fp;
extern const function_shape *const narrow_alu_default_bf;
extern const function_shape *const narrow_alu_ms_bf;
extern const function_shape *const narrow_alu_frm_default_fp;
extern const function_shape *const narrow_alu_frm_ms_fp;
extern const function_shape *const narrow_alu_frm_default_bf;
extern const function_shape *const narrow_alu_frm_ms_bf;
/* NTLH */
extern const function_shape *const ntlh;
/* Andes Vector INT4 Load Extension */
extern const function_shape *const nibbleload;
}

} // end namespace riscv_vector

#endif
