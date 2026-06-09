;; Machine description for RISC-V Zvfofp8 extension conversions.
;; Copyright (C) 2026 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_mode_iterator VWEXTF_ZVFOFP8MIN [
  (RVVM8BF  "TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN")
  (RVVM4BF  "TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN")
  (RVVM2BF  "TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN")
  (RVVM1BF  "TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN")
  (RVVMF2BF "TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN")
  (RVVMF4BF "TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VWEXT4_ZVFOFP8MIN [
  (RVVM8SF  "TARGET_ZVFOFP8MIN")
  (RVVM4SF  "TARGET_ZVFOFP8MIN")
  (RVVM2SF  "TARGET_ZVFOFP8MIN")
  (RVVM1SF  "TARGET_ZVFOFP8MIN")
  (RVVMF2SF "TARGET_ZVFOFP8MIN")
])

(define_mode_attr VBF_DOUBLE_TRUNC [
  (RVVM8BF "RVVM4QI")
  (RVVM4BF "RVVM2QI")
  (RVVM2BF "RVVM1QI")
  (RVVM1BF "RVVMF2QI")
  (RVVMF2BF "RVVMF4QI")
  (RVVMF4BF "RVVMF8QI")
])

(define_mode_attr VF_QUAD_TRUNC [
  (RVVM8SF "RVVM2QI")
  (RVVM4SF "RVVM1QI")
  (RVVM2SF "RVVMF2QI")
  (RVVM1SF "RVVMF4QI")
  (RVVMF2SF "RVVMF8QI")
])

(define_int_iterator ALTFMT_BASE [UNSPEC_F8E4M3 UNSPEC_F8E5M2])
(define_int_iterator ALTFMT_SAT [UNSPEC_F8E4M3 UNSPEC_F8E5M2 UNSPEC_F8E4M3_SAT UNSPEC_F8E5M2_SAT])

(define_int_attr altfmt_base
  [(UNSPEC_F8E4M3     "f8e4m3")
   (UNSPEC_F8E5M2     "f8e5m2")])

(define_int_attr altfmt_sat
  [(UNSPEC_F8E4M3     "f8e4m3")
   (UNSPEC_F8E5M2     "f8e5m2")
   (UNSPEC_F8E4M3_SAT "f8e4m3_sat")
   (UNSPEC_F8E5M2_SAT "f8e5m2_sat")])

(define_int_attr altfmt_kind
  [(UNSPEC_F8E4M3     "none")
   (UNSPEC_F8E5M2     "alt")
   (UNSPEC_F8E4M3_SAT "none")
   (UNSPEC_F8E5M2_SAT "alt")])

(define_int_attr sat
  [(UNSPEC_F8E4M3     "")
   (UNSPEC_F8E5M2     "")
   (UNSPEC_F8E4M3_SAT ".sat")
   (UNSPEC_F8E5M2_SAT ".sat")])

;; Zvfofp8min extension: FP8 to BF16 widening conversions.

(define_insn "@pred_extend_<altfmt_base>_to_<mode>"
  [(set (match_operand:VWEXTF_ZVFOFP8MIN 0 "register_operand"          "=&vr,  &vr")
    (if_then_else:VWEXTF_ZVFOFP8MIN
      (unspec:<VM>
        [(match_operand:<VM> 1 "vector_mask_operand"       "vmWc1,vmWc1")
         (match_operand 4 "vector_length_operand"          "   rK,   rK")
         (match_operand 5 "const_int_operand"              "    i,    i")
         (match_operand 6 "const_int_operand"              "    i,    i")
         (match_operand 7 "const_int_operand"              "    i,    i")
         (reg:SI VL_REGNUM)
         (reg:SI VTYPE_REGNUM)] UNSPEC_VPREDICATE)
     (unspec:VWEXTF_ZVFOFP8MIN
      [(float_extend:VWEXTF_ZVFOFP8MIN
        (match_operand:<VBF_DOUBLE_TRUNC> 3 "register_operand"    "   vr,   vr"))] ALTFMT_BASE)
        (match_operand:VWEXTF_ZVFOFP8MIN 2 "vector_merge_operand"       "   vu,    0")))]
  "TARGET_VECTOR && TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN"
  "vfwcvtbf16.f.f.v\t%0,%3%p1"
  [(set_attr "type" "vfwcvtbf16")
   (set_attr "mode" "<VBF_DOUBLE_TRUNC>")
   (set_attr "altfmt" "<altfmt_kind>")])

;; Zvfofp8min extension: BF16 to FP8 narrowing conversions.

(define_insn "@pred_trunc_<mode>_to_<altfmt_sat>"
  [(set (match_operand:<VBF_DOUBLE_TRUNC> 0 "register_operand"   "=vd, vd, vr, vr,  &vr,  &vr")
    (if_then_else:<VBF_DOUBLE_TRUNC>
        (unspec:<VM>
        [(match_operand:<VM> 1 "vector_mask_operand"            " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
        (match_operand 4 "vector_length_operand"               " rK, rK, rK, rK,   rK,   rK")
        (match_operand 5 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (match_operand 6 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (match_operand 7 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (match_operand 8 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (reg:SI VL_REGNUM)
        (reg:SI VTYPE_REGNUM)
        (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<VBF_DOUBLE_TRUNC>
        [(float_truncate:<VBF_DOUBLE_TRUNC>
	    (match_operand:VWEXTF_ZVFOFP8MIN 3 "register_operand"      "  0,  0,  0,  0,   vr,   vr"))] ALTFMT_SAT)
        (match_operand:<VBF_DOUBLE_TRUNC> 2 "vector_merge_operand"  " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_VECTOR && TARGET_ZVFOFP8MIN && TARGET_ZVFBFMIN"
  "vfncvtbf16<sat>.f.f.w\t%0,%3%p1"
  [(set_attr "type" "vfncvtbf16")
   (set_attr "mode" "<VBF_DOUBLE_TRUNC>")
   (set_attr "altfmt" "<altfmt_kind>")
   (set (attr "frm_mode")
   (symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])

;; Zvfofp8min extension: F32 to FP8 narrowing conversions.

(define_insn "@pred_quad_trunc_<mode>_to_<altfmt_sat>"
  [(set (match_operand:<VF_QUAD_TRUNC> 0 "register_operand"   "=vd, vd, vr, vr,  &vr,  &vr")
    (if_then_else:<VF_QUAD_TRUNC>
        (unspec:<VM>
        [(match_operand:<VM> 1 "vector_mask_operand"            " vm, vm,Wc1,Wc1,vmWc1,vmWc1")
        (match_operand 4 "vector_length_operand"               " rK, rK, rK, rK,   rK,   rK")
        (match_operand 5 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (match_operand 6 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (match_operand 7 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (match_operand 8 "const_int_operand"                   "  i,  i,  i,  i,    i,    i")
        (reg:SI VL_REGNUM)
        (reg:SI VTYPE_REGNUM)
        (reg:SI FRM_REGNUM)] UNSPEC_VPREDICATE)
	  (unspec:<VF_QUAD_TRUNC>
        [(float_truncate:<VF_QUAD_TRUNC>
        (match_operand:VWEXT4_ZVFOFP8MIN 3 "register_operand"      "  0,  0,  0,  0,   vr,   vr"))] ALTFMT_SAT)
        (match_operand:<VF_QUAD_TRUNC> 2 "vector_merge_operand"  " vu,  0, vu,  0,   vu,    0")))]
  "TARGET_VECTOR && TARGET_ZVFOFP8MIN"
  "vfncvt<sat>.f.f.q\t%0,%3%p1"
  [(set_attr "type" "vfncvtbf16")
   (set_attr "mode" "<VF_QUAD_TRUNC>")
   (set_attr "altfmt" "<altfmt_kind>")
   (set (attr "frm_mode")
   (symbol_ref "riscv_vector::get_frm_mode (operands[8])"))])
