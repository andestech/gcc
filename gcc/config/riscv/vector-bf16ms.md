;; Machine description for RISC-V 'V' Extension for GNU compiler.
;; Copyright (C) 2022-2024 Free Software Foundation, Inc.
;; Contributed by Juzhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Above message is copy from vector.md and the file implement Andes BF16 mode switch.

;; -------------------------------------------------------------------------------
;; ---- Predicated floating-point widen binary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 13.3 Vector Widening Floating-Point Add/Subtract Instructions
;; - 13.5 Vector Widening Floating-Point Multiply
;; -------------------------------------------------------------------------------

;; Vector Widening Add/Subtract/Multiply.
;; Andes BF16 mode switch.
(define_insn "@pred_dual_widen_<optab>bf<mode>"
  [(set (match_operand:VWEXTBF 0 "register_operand"                  "=&vr,  &vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK,   rK")
	     (match_operand 6 "const_int_operand"                  "    i,    i")
	     (match_operand 7 "const_int_operand"                  "    i,    i")
	     (match_operand 8 "const_int_operand"                  "    i,    i")
	     (match_operand 9 "const_int_operand"                  "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (any_widen_binop:VWEXTBF
	    (float_extend:VWEXTBF
	      (match_operand:<V_DOUBLE_TRUNC_BF> 3 "register_operand" "   vr,   vr"))
	    (float_extend:VWEXTBF
	      (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr,   vr")))
	  (match_operand:VWEXTBF 2 "vector_merge_operand"           "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<insn>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "vf<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_dual_widen_<optab>bf<mode>_scalar"
  [(set (match_operand:VWEXTBF 0 "register_operand"                  "=&vr,  &vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK,   rK")
	     (match_operand 6 "const_int_operand"                  "    i,    i")
	     (match_operand 7 "const_int_operand"                  "    i,    i")
	     (match_operand 8 "const_int_operand"                  "    i,    i")
	     (match_operand 9 "const_int_operand"                  "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (any_widen_binop:VWEXTBF
	    (float_extend:VWEXTBF
	      (match_operand:<V_DOUBLE_TRUNC_BF> 3 "register_operand" "   vr,   vr"))
	    (float_extend:VWEXTBF
	      (vec_duplicate:<V_DOUBLE_TRUNC_BF>
		(match_operand:<VSUBEL_BF> 4 "register_operand"       "    f,    f"))))
	  (match_operand:VWEXTBF 2 "vector_merge_operand"           "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<insn>.vf\t%0,%3,%4%p1"
  [(set_attr "type" "vf<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_single_widen_addbf<mode>"
  [(set (match_operand:VWEXTBF 0 "register_operand"                  "=&vr,  &vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK,   rK")
	     (match_operand 6 "const_int_operand"                  "    i,    i")
	     (match_operand 7 "const_int_operand"                  "    i,    i")
	     (match_operand 8 "const_int_operand"                  "    i,    i")
	     (match_operand 9 "const_int_operand"                  "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (plus:VWEXTBF
	    (float_extend:VWEXTBF
	      (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr,   vr"))
	    (match_operand:VWEXTBF 3 "register_operand"             "   vr,   vr"))
	  (match_operand:VWEXTBF 2 "vector_merge_operand"           "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfwadd.wv\t%0,%3,%4%p1"
  [(set_attr "type" "vfwalu")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_single_widen_subbf<mode>"
  [(set (match_operand:VWEXTBF 0 "register_operand"                  "=&vr,  &vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK,   rK")
	     (match_operand 6 "const_int_operand"                  "    i,    i")
	     (match_operand 7 "const_int_operand"                  "    i,    i")
	     (match_operand 8 "const_int_operand"                  "    i,    i")
	     (match_operand 9 "const_int_operand"                  "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (minus:VWEXTBF
	    (match_operand:VWEXTBF 3 "register_operand"             "   vr,   vr")
	    (float_extend:VWEXTBF
	      (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr,   vr")))
	  (match_operand:VWEXTBF 2 "vector_merge_operand"           "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfwsub.wv\t%0,%3,%4%p1"
  [(set_attr "type" "vfwalu")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_single_widen_<plus_minus:optab>bf<mode>_scalar"
  [(set (match_operand:VWEXTBF 0 "register_operand"                  "=&vr,  &vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           "vmWc1,vmWc1")
	     (match_operand 5 "vector_length_operand"              "   rK,   rK")
	     (match_operand 6 "const_int_operand"                  "    i,    i")
	     (match_operand 7 "const_int_operand"                  "    i,    i")
	     (match_operand 8 "const_int_operand"                  "    i,    i")
	     (match_operand 9 "const_int_operand"                  "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTBF
	    (match_operand:VWEXTBF 3 "register_operand"             "   vr,   vr")
	    (float_extend:VWEXTBF
	      (vec_duplicate:<V_DOUBLE_TRUNC_BF>
		(match_operand:<VSUBEL_BF> 4 "register_operand"       "    f,    f"))))
	  (match_operand:VWEXTBF 2 "vector_merge_operand"           "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<insn>.wf\t%0,%3,%4%p1"
  [(set_attr "type" "vf<widen_binop_insn_type>")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

;; -------------------------------------------------------------------------------
;; ---- Predicated widen floating-point ternary operations
;; -------------------------------------------------------------------------------
;; Includes:
;; - 13.7 Vector Widening Floating-Point Fused Multiply-Add Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_widen_mul_<optab>bf<mode>"
  [(set (match_operand:VWEXTBF 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 5 "vector_length_operand"                "   rK")
	     (match_operand 6 "const_int_operand"                    "    i")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTBF
	    (mult:VWEXTBF
	      (float_extend:VWEXTBF
	        (match_operand:<V_DOUBLE_TRUNC_BF> 3 "register_operand" "   vr"))
	      (float_extend:VWEXTBF
	        (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTBF 2 "register_operand"               "    0"))
	  (match_dup 2)))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<macc_msac>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "vfwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_widen_mul_<optab>bf<mode>_scalar"
  [(set (match_operand:VWEXTBF 0 "register_operand"                    "=&vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"             "vmWc1")
	     (match_operand 5 "vector_length_operand"                "   rK")
	     (match_operand 6 "const_int_operand"                    "    i")
	     (match_operand 7 "const_int_operand"                    "    i")
	     (match_operand 8 "const_int_operand"                    "    i")
	     (match_operand 9 "const_int_operand"                    "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTBF
	    (mult:VWEXTBF
	      (float_extend:VWEXTBF
	        (vec_duplicate:<V_DOUBLE_TRUNC_BF>
	          (match_operand:<VSUBEL_BF> 3 "register_operand"       "    f")))
	      (float_extend:VWEXTBF
	        (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr")))
	    (match_operand:VWEXTBF 2 "register_operand"               "    0"))
	  (match_dup 2)))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<macc_msac>.vf\t%0,%3,%4%p1"
  [(set_attr "type" "vfwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_widen_mul_neg_<optab>bf<mode>"
  [(set (match_operand:VWEXTBF 0 "register_operand"                      "=&vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               "vmWc1")
	     (match_operand 5 "vector_length_operand"                  "   rK")
	     (match_operand 6 "const_int_operand"                      "    i")
	     (match_operand 7 "const_int_operand"                      "    i")
	     (match_operand 8 "const_int_operand"                      "    i")
	     (match_operand 9 "const_int_operand"                      "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTBF
	    (neg:VWEXTBF
	      (mult:VWEXTBF
	        (float_extend:VWEXTBF
	          (match_operand:<V_DOUBLE_TRUNC_BF> 3 "register_operand" "   vr"))
	        (float_extend:VWEXTBF
	          (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr"))))
	      (match_operand:VWEXTBF 2 "register_operand"               "    0"))
	  (match_dup 2)))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<nmsac_nmacc>.vv\t%0,%3,%4%p1"
  [(set_attr "type" "vfwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

(define_insn "@pred_widen_mul_neg_<optab>bf<mode>_scalar"
  [(set (match_operand:VWEXTBF 0 "register_operand"                      "=&vr")
	(if_then_else:VWEXTBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"               "vmWc1")
	     (match_operand 5 "vector_length_operand"                  "   rK")
	     (match_operand 6 "const_int_operand"                      "    i")
	     (match_operand 7 "const_int_operand"                      "    i")
	     (match_operand 8 "const_int_operand"                      "    i")
	     (match_operand 9 "const_int_operand"                      "    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (plus_minus:VWEXTBF
	    (neg:VWEXTBF
	      (mult:VWEXTBF
	        (float_extend:VWEXTBF
	          (vec_duplicate:<V_DOUBLE_TRUNC_BF>
	            (match_operand:<VSUBEL_BF> 3 "register_operand"       "    f")))
	        (float_extend:VWEXTBF
	          (match_operand:<V_DOUBLE_TRUNC_BF> 4 "register_operand" "   vr"))))
	    (match_operand:VWEXTBF 2 "register_operand"                 "    0"))
	  (match_dup 2)))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfw<nmsac_nmacc>.vf\t%0,%3,%4%p1"
  [(set_attr "type" "vfwmuladd")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])

;; -------------------------------------------------------------------------------
;; ---- Predicated floating-point widen conversions
;; -------------------------------------------------------------------------------
;; Includes:
;; - 13.18 Widening Floating-Point/Integer Type-Convert Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_widen_fcvt_x<v_su>_bf<mode>"
  [(set (match_operand:VWCONVERTI_BF 0 "register_operand"         "=&vr,  &vr")
	(if_then_else:VWCONVERTI_BF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      "vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"         "   rK,   rK")
	     (match_operand 5 "const_int_operand"             "    i,    i")
	     (match_operand 6 "const_int_operand"             "    i,    i")
	     (match_operand 7 "const_int_operand"             "    i,    i")
	     (match_operand 8 "const_int_operand"             "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VWCONVERTI_BF
	     [(match_operand:<VNCONVERT_BF> 3 "register_operand" "   vr,   vr")] VFCVTS)
	  (match_operand:VWCONVERTI_BF 2 "vector_merge_operand"  "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfwcvt.x<v_su>.f.v\t%0,%3%p1"
  [(set_attr "type" "vfwcvtftoi")
   (set_attr "mode" "<VNCONVERT_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_widen_<fix_cvt>bf<mode>"
  [(set (match_operand:VWCONVERTI_BF 0 "register_operand"        "=&vr,  &vr")
	(if_then_else:VWCONVERTI_BF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     "vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"        "   rK,   rK")
	     (match_operand 5 "const_int_operand"            "    i,    i")
	     (match_operand 6 "const_int_operand"            "    i,    i")
	     (match_operand 7 "const_int_operand"            "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_fix:VWCONVERTI_BF
	     (match_operand:<VNCONVERT_BF> 3 "register_operand" "   vr,   vr"))
	  (match_operand:VWCONVERTI_BF 2 "vector_merge_operand" "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfwcvt.rtz.x<u>.f.v\t%0,%3%p1"
  [(set_attr "type" "vfwcvtftoi")
   (set_attr "mode" "<VNCONVERT_BF>")])

(define_insn "@pred_widen_<float_cvt>bf<mode>"
  [(set (match_operand:V_VLSBF 0 "register_operand"                "=&vr,  &vr")
	(if_then_else:V_VLSBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"     "vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"        "   rK,   rK")
	     (match_operand 5 "const_int_operand"            "    i,    i")
	     (match_operand 6 "const_int_operand"            "    i,    i")
	     (match_operand 7 "const_int_operand"            "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (any_float:V_VLSBF
	     (match_operand:<VNCONVERT_BF> 3 "register_operand" "   vr,   vr"))
	  (match_operand:V_VLSBF 2 "vector_merge_operand"         "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfwcvt.f.x<u>.v\t%0,%3%p1"
  [(set_attr "type" "vfwcvtitof")
   (set_attr "mode" "<VNCONVERT_BF>")])

;; Conflic with zvfbfmin, so add suffix ms(mode switch) to separate.
(define_insn "@pred_extend_bfms<mode>"
  [(set (match_operand:VWEXTF_XANDESBF 0 "register_operand"                 "=&vr,  &vr")
	(if_then_else:VWEXTF_XANDESBF
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"          "vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"             "   rK,   rK")
	     (match_operand 5 "const_int_operand"                 "    i,    i")
	     (match_operand 6 "const_int_operand"                 "    i,    i")
	     (match_operand 7 "const_int_operand"                 "    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (float_extend:VWEXTF_XANDESBF
	     (match_operand:<V_DOUBLE_TRUNC_BF> 3 "register_operand" "   vr,   vr"))
	  (match_operand:VWEXTF_XANDESBF 2 "vector_merge_operand"          "   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfwcvt.f.f.v\t%0,%3%p1"
  [(set_attr "type" "vfwcvtftof")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")])

;; -------------------------------------------------------------------------------
;; ---- Predicated floating-point narrow conversions
;; -------------------------------------------------------------------------------
;; Includes:
;; - 13.19 Narrowing Floating-Point/Integer Type-Convert Instructions
;; -------------------------------------------------------------------------------

(define_insn "@pred_narrow_<float_cvt>bf<mode>"
  [(set (match_operand:<VNCONVERT_BF> 0 "register_operand"       "=vd, vd, vr, vr,  &vr,  &vr")
	(if_then_else:<VNCONVERT_BF>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"      " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"         " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"             "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (any_float:<VNCONVERT_BF>
	     (match_operand:VWCONVERTI_BF 3 "register_operand"   "  0,  0,  0,  0,   vr,   vr"))
	  (match_operand:<VNCONVERT_BF> 2 "vector_merge_operand" " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfncvt.f.x<u>.w\t%0,%3%p1"
  [(set_attr "type" "vfncvtitof")
   (set_attr "mode" "<VNCONVERT_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_trunc_bf<mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC_BF> 0 "register_operand"       "=vd, vd, vr, vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC_BF>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"              " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 8 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (float_truncate:<V_DOUBLE_TRUNC_BF>
	     (match_operand:VWEXTF_XANDESBF 3 "register_operand"            "  0,  0,  0,  0,   vr,   vr"))
	  (match_operand:<V_DOUBLE_TRUNC_BF> 2 "vector_merge_operand" " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfncvt.f.f.w\t%0,%3%p1"
  [(set_attr "type" "vfncvtftof")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

(define_insn "@pred_rod_trunc_bf<mode>"
  [(set (match_operand:<V_DOUBLE_TRUNC_BF> 0 "register_operand"       "=vd, vd, vr, vr,  &vr,  &vr")
	(if_then_else:<V_DOUBLE_TRUNC_BF>
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"           " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
	     (match_operand 4 "vector_length_operand"              " rK, rK, rK, rK,   rK,   rK")
	     (match_operand 5 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 6 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (match_operand 7 "const_int_operand"                  "  i,  i,  i,  i,    i,    i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<V_DOUBLE_TRUNC_BF>
	    [(float_truncate:<V_DOUBLE_TRUNC_BF>
	       (match_operand:VWEXTF_XANDESBF 3 "register_operand"          "  0,  0,  0,  0,   vr,   vr"))] UNSPEC_ROD)
	  (match_operand:<V_DOUBLE_TRUNC_BF> 2 "vector_merge_operand" " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_VECTOR && TARGET_BF16MS"
  "vfncvt.rod.f.f.w\t%0,%3%p1"
  [(set_attr "type" "vfncvtftof")
   (set_attr "mode" "<V_DOUBLE_TRUNC_BF>")])

;; -------------------------------------------------------------------------------
;; Andes V5 ISA
;; -------------------------------------------------------------------------------

(define_insn "@pred_vfpmad<tb><mode>"
  [(set (match_operand:VBF16 0 "register_operand"               "=&vr, &vr")
	(if_then_else:VBF16
	  (unspec:<VM>
	    [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1, vmWc1")
	     (match_operand 5 "vector_length_operand"          "   rK,    rK")
	     (match_operand 6 "const_int_operand"              "    i,     i")
	     (match_operand 7 "const_int_operand"              "    i,     i")
	     (match_operand 8 "const_int_operand"              "    i,     i")
	     (match_operand 9 "const_int_operand"              "    i,     i")
	     (reg:SI VL_REGNUM)
	     (reg:SI VTYPE_REGNUM)
	     (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:VBF16
	    [(match_operand:VBF16 3 "register_operand" "vr, vr")
	     (match_operand:SF 4 "register_operand"   " f,  f")] VFPMAD)
	  (match_operand:VBF16 2 "vector_merge_operand"   "vu,  0")))]
  "TARGET_BF16MS && TARGET_V5"
  "nds.vfpmad<tb>.vf\t%0,%4,%3%p1"
  [(set_attr "type" "vfpmad")
   (set_attr "mode" "<MODE>")
   (set (attr "frm_mode")
	(symbol_ref "riscv_vector::get_frm_mode (operands[9])"))])
