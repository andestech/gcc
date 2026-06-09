;; Machine description for Andes CMOV (Conditional MOV/ALU).
;; Copyright (C) 2023-2024 Free Software Foundation, Inc.

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

;; Patterns for Conditional CMOV/ALU.

(define_insn "cmov<optab><ANYI:mode><X:mode>"
  [(set (match_operand:ANYI 0 "register_operand"                                  "=r,  r,    r,    r,  r,  r,    r,    r")
        (if_then_else:ANYI (equality_op (match_operand:X 1 "register_operand"     " r,  r,    r,    r,  r,  0,    r,    r")
					(match_operand:X 4 "reg_or_imm7u_operand" "rJ, rJ, Bz07, Bz07, rJ, rJ, Bz07, Bz07"))
			   (match_operand:ANYI 2 "arith_operand"                  " r,  I,    r,    I,  0,  0,    0,    0")
			   (match_operand:ANYI 3 "arith_operand"                  " 0,  0,    0,    0,  r,  I,    r,    I")))]
  "TARGET_CMOV"
  "@
   <rev_br_insn> %1, %z4, 0f\n\tadd %0, %2, zero\n\t.align 2\n0:
   <rev_br_insn> %1, %z4, 0f\n\tadd %0, zero, %2\n\t.align 2\n0:
   nds.<rev_br_insn>c %1,  %4, 0f\n\tadd %0, %2, zero\n\t.align 2\n0:
   nds.<rev_br_insn>c %1,  %4, 0f\n\tadd %0, zero, %2\n\t.align 2\n0:
   <br_insn> %1, %z4, 0f\n\tadd %0, %3, zero\n\t.align 2\n0:
   <br_insn> %1, %z4, 0f\n\tadd %0, zero, %3\n\t.align 2\n0:
   nds.<br_insn>c %1,  %4, 0f\n\tadd %0, %3, zero\n\t.align 2\n0:
   nds.<br_insn>c %1,  %4, 0f\n\tadd %0, zero, %3\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<X:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><ANYI:mode><X:mode>"
  [(set (match_operand:ANYI 0 "register_operand"                             "=r,  r,  r,  r")
        (if_then_else:ANYI (inequal_op (match_operand:X 1 "register_operand" " r,  r,  r,  r")
				       (match_operand:X 4 "reg_or_0_operand" "rJ, rJ, rJ, rJ"))
			   (match_operand:ANYI 2 "arith_operand"             " r,  I,  0,  0")
			   (match_operand:ANYI 3 "arith_operand"             " 0,  0,  r,  I")))]
  "TARGET_CMOV"
  "@
   <rev_br_insn> %1, %z4, 0f\n\tadd %0, %2, zero\n\t.align 2\n0:
   <rev_br_insn> %1, %z4, 0f\n\tadd %0, zero, %2\n\t.align 2\n0:
   <br_insn> %1, %z4, 0f\n\tadd %0, %3, zero\n\t.align 2\n0:
   <br_insn> %1, %z4, 0f\n\tadd %0, zero, %3\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<X:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab><ANYI:mode><X:mode>"
  [(set (match_operand:ANYI 0 "register_operand" "=r, r, r, r")
        (if_then_else:ANYI (equality_op
			     (zero_extract:X (match_operand:X 4 "register_operand" "r, r, r, r")
			     (const_int 1)
			     (match_operand 1 "branch_bbcs_operand"))
			     (const_int 0))
			    (match_operand:ANYI 2 "arith_operand" " r, I, 0, 0")
			    (match_operand:ANYI 3 "arith_operand" " 0, 0, r, I")))]
  "TARGET_CMOV && TARGET_BBCS"
  "@
   <rev_bbcs> %4, %1, 0f\n\tadd %0, %2, zero\n\t.align 2\n0:
   <rev_bbcs> %4, %1, 0f\n\tadd %0, zero, %2\n\t.align 2\n0:
   <bbcs> %4, %1, 0f\n\tadd %0, %3, zero\n\t.align 2\n0:
   <bbcs> %4, %1, 0f\n\tadd %0, zero, %3\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<ANYI:MODE>")
   (set (attr "length") (const_int 8))])

;;Combine for conditional alu instructions.

;;branch + alu
(define_insn "cmov<equality_op:optab><mode>_<cond_alu:optab>"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(cond_alu:X (match_operand:X 4 "register_operand"      " r,    r")
				    (match_operand:X 5 "const_arith_operand"   " I,    I"))
			(match_operand:X 3 "arith_operand"                     " 0,    0")))]
  "TARGET_CMOV"
  "@
   <equality_op:rev_br_insn> %1, %z2, 0f\n\t<cond_alu:insn> %0, %4, %5\n\t.align 2\n0:
   nds.<equality_op:rev_br_insn>c %1, %2, 0f\n\t<cond_alu:insn> %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmoveq<equality_op:optab><mode>_<cond_alu:optab>_rev"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(match_operand:X 3  "arith_operand"                    " 0,    0")
			(cond_alu:X (match_operand:X 4 "register_operand"      " r,    r")
				    (match_operand:X 5 "const_arith_operand"   " I,    I"))))]
  "TARGET_CMOV"
  "@
   <equality_op:br_insn> %1, %z2, 0f\n\t<cond_alu:insn> %0, %4, %5\n\t.align 2\n0:
   nds.<equality_op:br_insn>c %1, %2, 0f\n\t<cond_alu:insn> %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_<cond_alu:optab>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(cond_alu:X (match_operand:X 4 "register_operand" "r")
				    (match_operand:X 5 "const_arith_operand" "I"))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV"
  "<inequal_op:rev_br_insn> %1, %z2, 0f\n\t<cond_alu:insn> %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_<cond_alu:optab>_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(match_operand:X 3 "arith_operand" "0")
			(cond_alu:X (match_operand:X 4 "register_operand" "r")
				    (match_operand:X 5 "const_arith_operand" "I"))))]
  "TARGET_CMOV"
  "<inequal_op:br_insn> %1, %z2, 0f\n\t<cond_alu:insn> %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

;;branch + ext-zbb
(define_insn "cmov<equality_op:optab><mode>_<cond_bitwise:optab>n"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(cond_bitwise:X (not:X (match_operand:X 4 "register_operand"      " r,    r"))
				    (match_operand:X 5 "register_operand"   " r,    r"))
			(match_operand:X 3 "arith_operand"                     " 0,    0")))]
  "TARGET_CMOV && TARGET_ZBB"
  "@
   <equality_op:rev_br_insn> %1, %z2, 0f\n\t<cond_bitwise:insn>n %0, %5, %4\n\t.align 2\n0:
   nds.<equality_op:rev_br_insn>c %1, %2, 0f\n\t<cond_bitwise:insn>n %0, %5, %4\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmoveq<equality_op:optab><mode>_<cond_bitwise:optab>n_rev"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(match_operand:X 3  "arith_operand"                    " 0,    0")
			(cond_bitwise:X (not:X (match_operand:X 4 "register_operand"      " r,    r"))
				    (match_operand:X 5 "register_operand"   " r,    r"))))]
  "TARGET_CMOV && TARGET_ZBB"
  "@
   <equality_op:br_insn> %1, %z2, 0f\n\t<cond_bitwise:insn>n %0, %5, %4\n\t.align 2\n0:
   nds.<equality_op:br_insn>c %1, %2, 0f\n\t<cond_bitwise:insn>n %0, %5, %4\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_<cond_bitwise:optab>n"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(cond_bitwise:X (not:X (match_operand:X 4 "register_operand" "r"))
				    (match_operand:X 5 "register_operand" "r"))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_ZBB"
  "<inequal_op:rev_br_insn> %1, %z2, 0f\n\t<cond_bitwise:insn>n %0, %5, %4\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_<cond_bitwise:optab>n_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(match_operand:X 3 "arith_operand" "0")
			(cond_bitwise:X (not:X (match_operand:X 4 "register_operand" "r"))
				    (match_operand:X 5 "register_operand" "r"))))]
  "TARGET_CMOV && TARGET_ZBB"
  "<inequal_op:br_insn> %1, %z2, 0f\n\t<cond_bitwise:insn>n %0, %5, %4\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<equality_op:optab><mode>_xnor"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(not:X (xor:X (match_operand:X 4 "register_operand"      " r,    r")
				    (match_operand:X 5 "register_operand"   " r,    r")))
			(match_operand:X 3 "arith_operand"                     " 0,    0")))]
  "TARGET_CMOV && TARGET_ZBB"
  "@
   <equality_op:rev_br_insn> %1, %z2, 0f\n\txnor %0, %4, %5\n\t.align 2\n0:
   nds.<equality_op:rev_br_insn>c %1, %2, 0f\n\txnor %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmoveq<equality_op:optab><mode>_xnor_rev"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(match_operand:X 3  "arith_operand"                    " 0,    0")
			(not:X (xor:X (match_operand:X 4 "register_operand"      " r,    r")
				    (match_operand:X 5 "register_operand"   " r,    r")))))]
  "TARGET_CMOV && TARGET_ZBB"
  "@
   <equality_op:br_insn> %1, %z2, 0f\n\txnor %0, %4, %5\n\t.align 2\n0:
   nds.<equality_op:br_insn>c %1, %2, 0f\n\txnor %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_xnor"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(not:X (xor:X (match_operand:X 4 "register_operand" "r")
				    (match_operand:X 5 "register_operand" "r")))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_ZBB"
  "<inequal_op:rev_br_insn> %1, %z2, 0f\n\txnor %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_xnor_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(match_operand:X 3 "arith_operand" "0")
			(not:X (xor:X (match_operand:X 4 "register_operand" "r")
				    (match_operand:X 5 "register_operand" "r")))))]
  "TARGET_CMOV && TARGET_ZBB"
  "<inequal_op:br_insn> %1, %z2, 0f\n\txnor %0, %4, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

;; bbc|s + alu
(define_insn "cmov_bb<equality_op:optab><mode>_<cond_alu:optab>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(cond_alu:X (match_operand:X 2 "register_operand"    "r")
				    (match_operand:X 5 "const_arith_operand" "I"))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS"
  "<equality_op:rev_bbcs> %4, %1, 0f\n\t<cond_alu:insn> %0, %2, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<equality_op:optab><mode>_<cond_alu:optab>_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(match_operand:X 3 "arith_operand" "0")
			(cond_alu:X (match_operand:X 2 "register_operand"    "r")
				    (match_operand:X 5 "const_arith_operand" "I"))))]
  "TARGET_CMOV && TARGET_BBCS"
  "<bbcs> %4, %1, 0f\n\t<cond_alu:insn> %0, %2, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

;; bbc|s + ext-zbb
(define_insn "cmov_bb<equality_op:optab><mode>_<cond_bitwise:optab>n"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(cond_bitwise:X (not:X (match_operand:X 2 "register_operand"    "r"))
				    (match_operand:X 5 "register_operand" "r"))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_ZBB"
  "<equality_op:rev_bbcs> %4, %1, 0f\n\t<cond_bitwise:insn>n %0, %5, %2\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<equality_op:optab><mode>_<cond_bitwise:optab>n_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(match_operand:X 3 "arith_operand" "0")
			(cond_bitwise:X (not:X (match_operand:X 2 "register_operand"    "r"))
				    (match_operand:X 5 "register_operand" "r"))))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_ZBB"
  "<bbcs> %4, %1, 0f\n\t<cond_bitwise:optab>n %0, %5, %2\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<equality_op:optab><mode>_xnor"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(not:X  (xor:X (match_operand:X 2 "register_operand"    "r")
				    (match_operand:X 5 "register_operand" "r")))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_ZBB"
  "<equality_op:rev_bbcs> %4, %1, 0f\n\txnor %0, %2, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<equality_op:optab><mode>_xnor_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(match_operand:X 3 "arith_operand" "0")
			(not:X (xor:X (match_operand:X 2 "register_operand"    "r")
				    (match_operand:X 5 "register_operand" "r")))))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_ZBB"
  "<bbcs> %4, %1, 0f\n\txnor %0, %2, %5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

;;Combine for conditional any shift instructions.

;;branch + shift
(define_insn "cmov<equality_op:optab><mode>_<any_shift:optab>"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(any_shift:X (match_operand:X 4 "register_operand"     " r,    r")
				     (match_operand:X 5 "const_arith_operand"  " I,    I"))
			(match_operand:X 3 "arith_operand"                     " 0,    0")))]
  "TARGET_CMOV"
  "@
   <equality_op:rev_br_insn> %1, %z2, 0f\n\t<any_shift:insn> %0, %4, %s5\n\t.align 2\n0:
   nds.<equality_op:rev_br_insn>c %1, %2, 0f\n\t<any_shift:insn> %0, %4, %s5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<equality_op:optab><mode>_<any_shift:optab>_rev"
  [(set (match_operand:X 0 "register_operand"                                  "=r,    r")
        (if_then_else:X (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				     (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(match_operand:X 3  "arith_operand"                    " 0,    0")
			(any_shift:X (match_operand:X 4 "register_operand"     " r,    r")
				     (match_operand:X 5 "const_arith_operand"  " I,    I"))))]
  "TARGET_CMOV"
  "@
   <equality_op:br_insn> %1, %z2, 0f\n\t<any_shift:insn> %0, %4, %s5\n\t.align 2\n0:
   nds.<equality_op:br_insn>c %1, %2, 0f\n\t<any_shift:insn> %0, %4, %s5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode>_<any_shift:optab>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(any_shift:X (match_operand:X 4 "register_operand" "r")
				     (match_operand:X 5 "const_arith_operand" "I"))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV"
  "<inequal_op:rev_br_insn> %1, %z2, 0f\n\t<any_shift:insn> %0, %4, %s5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<inequal_op:optab><mode><any_shift:optab>_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (inequal_op (match_operand:X 1 "register_operand" " r")
				    (match_operand:X 2 "reg_or_0_operand" "rJ"))
			(match_operand:X 3 "arith_operand" "0")
			(any_shift:X (match_operand:X 4 "register_operand" "r")
				     (match_operand:X 5 "const_arith_operand" "I"))))]
  "TARGET_CMOV"
  "<inequal_op:br_insn> %1, %z2, 0f\n\t<any_shift:insn> %0, %4, %s5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

;;bbc|s + shift
(define_insn "cmov_bb<equality_op:optab><mode>_<any_shift:optab>"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(any_shift:X (match_operand:X 2 "register_operand"    "r")
				     (match_operand:X 5 "const_arith_operand" "I"))
			(match_operand:X 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS"
  "<rev_bbcs> %4, %1, 0f\n\t<any_shift:insn> %0, %2, %s5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<equality_op:optab><mode>_<any_shift:optab>_rev"
  [(set (match_operand:X 0 "register_operand" "=r")
        (if_then_else:X (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(match_operand:X 3 "arith_operand" "0")
			(any_shift:X (match_operand:X 2 "register_operand"    "r")
				     (match_operand:X 5 "const_arith_operand" "I"))))]
  "TARGET_CMOV && TARGET_BBCS"
  "<bbcs> %4, %1, 0f\n\t<any_shift:insn> %0, %2, %s5\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<MODE>")
   (set (attr "length") (const_int 8))])

;;bbcs + bfoz|s
(define_insn "cmov_bb<optab><X:mode>_<any_extract:sz>extra<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				       (const_int 1)
				       (match_operand 1 "branch_bbcs_operand"))
				       (const_int 0))
			  (any_extract:GPR (match_operand:GPR 2 "register_operand" "r")
					   (match_operand 5 "extract_size_imm_<X:mode>" "n")
					   (match_operand 6 "extract_loc_imm_<X:mode>" "n"))
			  (match_operand:GPR 3 "register_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO
   && IN_RANGE (INTVAL (operands[5]) + INTVAL (operands[6]),
		1, GET_MODE_BITSIZE (<X:MODE>mode))"
  {
    operands[5] = GEN_INT (INTVAL (operands[5]) + INTVAL (operands[6]) - 1);
    return "<rev_bbcs> %4, %1, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab><X:mode>_<any_extract:sz>extra<GPR:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				       (const_int 1)
				       (match_operand 1 "branch_bbcs_operand"))
				       (const_int 0))
			  (match_operand:GPR 3 "register_operand" "0")
			  (any_extract:GPR (match_operand:GPR 2 "register_operand" "r")
					   (match_operand 5 "extract_size_imm_<X:mode>" "n")
					   (match_operand 6 "extract_loc_imm_<X:mode>" "n"))))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO
   && IN_RANGE (INTVAL (operands[5]) + INTVAL (operands[6]),
		1, GET_MODE_BITSIZE (<X:MODE>mode))"
  {
    operands[5] = GEN_INT (INTVAL (operands[5]) + INTVAL (operands[6]) - 1);
    return "<bbcs> %4, %1, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

;;brach + bfoz|s
(define_insn "cmov<optab><X:mode>_<any_extract:sz>extra<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (inequal_op (match_operand:X 1 "register_operand" " r")
				      (match_operand:X 4 "reg_or_0_operand" "rJ"))
			  (any_extract:GPR (match_operand:GPR 2 "register_operand" "r")
					   (match_operand 5 "extract_size_imm_<X:mode>" "n")
					   (match_operand 6 "extract_loc_imm_<X:mode>" "n"))
			  (match_operand:GPR 3 "register_operand" "0")))]
  "TARGET_CMOV && TARGET_BFO
   && IN_RANGE (INTVAL (operands[5]) + INTVAL (operands[6]),
		1, GET_MODE_BITSIZE (<X:MODE>mode))"
  {
    operands[5] = GEN_INT (INTVAL (operands[5]) + INTVAL (operands[6]) - 1);
    return "<rev_br_insn> %1, %z4, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><X:mode>_<any_extract:sz>extra<GPR:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (inequal_op (match_operand:X 1 "register_operand" " r")
				      (match_operand:X 4 "reg_or_0_operand" "rJ"))
			  (match_operand:GPR 3 "register_operand" "0")
			  (any_extract:GPR (match_operand:GPR 2 "register_operand" "r")
					   (match_operand 5 "extract_size_imm_<X:mode>" "n")
					   (match_operand 6 "extract_loc_imm_<X:mode>" "n"))))]
  "TARGET_CMOV && TARGET_BFO
   && IN_RANGE (INTVAL (operands[5]) + INTVAL (operands[6]),
		1, GET_MODE_BITSIZE (<X:MODE>mode))"
  {
    operands[5] = GEN_INT (INTVAL (operands[5]) + INTVAL (operands[6]) - 1);
    return "<br_insn> %1, %z4, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><X:mode>_<any_extract:sz>extra<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand"                                       "=r,    r")
        (if_then_else:GPR (equality_op (match_operand:X 1 "register_operand"          " r,    r")
				       (match_operand:X 4 "reg_or_imm7u_operand"      "rJ, Bz07"))
			  (any_extract:GPR (match_operand:GPR 2 "register_operand"    " r,    r")
					   (match_operand 5 "extract_size_imm_<X:mode>" " n,    n")
					   (match_operand 6 "extract_loc_imm_<X:mode>"  " n,    n"))
			  (match_operand:GPR 3 "register_operand"                     " 0,    0")))]
  "TARGET_CMOV && TARGET_BFO
   && IN_RANGE (INTVAL (operands[5]) + INTVAL (operands[6]),
		1, GET_MODE_BITSIZE (<X:MODE>mode))"
  {
    operands[5] = GEN_INT (INTVAL (operands[5]) + INTVAL (operands[6]) - 1);

    switch (which_alternative)
      {
      case 0:
	return "<rev_br_insn> %1, %z4, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
      case 1:
	return "nds.<rev_br_insn>c %1, %4, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
      default:
	gcc_unreachable ();
    }
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><X:mode>_<any_extract:sz>extra<GPR:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand"                                       "=r,    r")
        (if_then_else:GPR (equality_op (match_operand:X 1 "register_operand"          " r,    r")
				       (match_operand:X 4 "reg_or_imm7u_operand"      "rJ, Bz07"))
			  (match_operand:GPR 3 "register_operand"                     " 0,    0")
			  (any_extract:GPR (match_operand:GPR 2 "register_operand"    " r,    r")
					   (match_operand 5 "extract_size_imm_<X:mode>" " n,    n")
					   (match_operand 6 "extract_loc_imm_<X:mode>"  " n,    n"))))]
  "TARGET_CMOV && TARGET_BFO
   && IN_RANGE (INTVAL (operands[5]) + INTVAL (operands[6]),
		1, GET_MODE_BITSIZE (<X:MODE>mode))"
  {
    operands[5] = GEN_INT (INTVAL (operands[5]) + INTVAL (operands[6]) - 1);

    switch (which_alternative)
      {
      case 0:
	return "<br_insn> %1, %z4, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
      case 1:
	return "nds.<br_insn>c %1, %4, 0f\n\tnds.bfo<any_extract:sz> %0, %2, %5, %6\n\t.align 2\n0:";
      default:
	gcc_unreachable ();
    }
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

;;branch + zero_extend
(define_insn "cmov<optab>_zext<GPR:mode><X:mode>"
  [(set (match_operand:GPR 0 "register_operand"                                   "=r,    r")
        (if_then_else:GPR (equality_op (match_operand:X 1 "register_operand"      " r,    r")
				       (match_operand:X 2 "reg_or_imm7u_operand"  "rJ, Bz07"))
			  (zero_extend:GPR (match_operand:HI 4 "register_operand" " r,    r"))
			  (match_operand:GPR 3 "arith_operand"                    " 0,    0")))]
  "TARGET_CMOV && TARGET_BFO"
  "@
   <rev_br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, 15, 0\n\t.align 2\n0:
   nds.<rev_br_insn>c %1, %2, 0f\n\tnds.bfoz %0, %4, 15, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab>_zext<GPR:mode><X:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand"                                  "=r,    r")
        (if_then_else:GPR (equality_op (match_operand:X 1 "register_operand"     " r,    r")
				       (match_operand:X 2 "reg_or_imm7u_operand" "rJ, Bz07"))
			(match_operand:GPR 3  "arith_operand"                    " 0,    0")
			(zero_extend:GPR (match_operand:HI 4 "register_operand"  " r,    r"))))]
  "TARGET_CMOV && TARGET_BFO"
  "@
   <br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, 15, 0\n\t.align 2\n0:
   nds.<br_insn>c %1, %2, 0f\n\tnds.bfoz %0, %4, 15, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab>_zext<GPR:mode><X:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (inequal_op (match_operand:X 1 "register_operand" " r")
				      (match_operand:X 2 "reg_or_0_operand" "rJ"))
			  (zero_extend:GPR (match_operand:HI 4 "register_operand"  "r"))
			  (match_operand:GPR 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BFO"
  "<rev_br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, 15, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab>_zext<GPR:mode><X:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (inequal_op (match_operand:X 1 "register_operand" " r")
				      (match_operand:X 2 "reg_or_0_operand" "rJ"))
			  (match_operand:GPR 3 "arith_operand" "0")
			  (zero_extend:GPR (match_operand:HI 4 "register_operand" "r"))))]
  "TARGET_CMOV && TARGET_BFO"
  "<br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, 15, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab>_zext<GPR:mode><X:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				       (const_int 1)
				       (match_operand 1 "branch_bbcs_operand"))
				       (const_int 0))
			  (zero_extend:GPR (match_operand:HI 2 "register_operand" "r"))
			  (match_operand:GPR 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO"
  "<rev_bbcs> %4, %1, 0f\n\tnds.bfoz %0, %2, 15, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab>_zext<GPR:mode><X:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				     (const_int 1)
				     (match_operand 1 "branch_bbcs_operand"))
				     (const_int 0))
			(match_operand:GPR 3 "arith_operand" "0")
			(zero_extend:GPR (match_operand:HI 2 "register_operand" "r"))))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO"
  "<bbcs> %4, %1, 0f\n\tnds.bfoz %0, %2, 15, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

;;branch + sign_extend
(define_insn "cmov<optab><X:mode>_extend<SHORT:mode><SUPERQI:mode>2"
  [(set (match_operand:SUPERQI 0 "register_operand"                                           "=r,    r")
        (if_then_else:SUPERQI (equality_op (match_operand:X 1 "register_operand"              " r,    r")
					   (match_operand:X 2 "reg_or_imm7u_operand"          "rJ, Bz07"))
			      (sign_extend:SUPERQI (match_operand:SHORT 4 "register_operand"  " r,    r"))
			      (match_operand:SUPERQI 3 "arith_operand"                        " 0,    0")))]
  "TARGET_CMOV && TARGET_BFO"
  "@
   <rev_br_insn> %1, %z2, 0f\n\tnds.bfos %0, %4, <SHORT:sh_limit>, 0\n\t.align 2\n0:
   nds.<rev_br_insn>c %1, %2, 0f\n\tnds.bfos %0, %4, <SHORT:sh_limit>, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "SI")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><X:mode>_extend<SHORT:mode><SUPERQI:mode>2_rev"
  [(set (match_operand:SUPERQI 0 "register_operand"                                     "=r,    r")
        (if_then_else:SUPERQI (equality_op (match_operand:X 1 "register_operand"        " r,    r")
					   (match_operand:X 2 "reg_or_imm7u_operand"    "rJ, Bz07"))
			(match_operand:SUPERQI 3  "arith_operand"                       " 0,    0")
			(sign_extend:SUPERQI (match_operand:SHORT 4 "register_operand"  " r,    r"))))]
  "TARGET_CMOV && TARGET_BFO"
  "@
   <br_insn> %1, %z2, 0f\n\tnds.bfos %0, %4, <SHORT:sh_limit>, 0\n\t.align 2\n0:
   nds.<br_insn>c %1, %2, 0f\n\tnds.bfos %0, %4, <SHORT:sh_limit>, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "SI")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><X:mode>_extend<SHORT:mode><SUPERQI:mode>2"
  [(set (match_operand:SUPERQI 0 "register_operand" "=r")
        (if_then_else:SUPERQI (inequal_op (match_operand:X 1 "register_operand" " r")
					  (match_operand:X 2 "reg_or_0_operand" "rJ"))
			      (zero_extend:SUPERQI (match_operand:SHORT 4 "register_operand"  "r"))
			      (match_operand:SUPERQI 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BFO"
  "<rev_br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, <SHORT:sh_limit>, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "SI")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab><X:mode>_extend<SHORT:mode><SUPERQI:mode>2_rev"
  [(set (match_operand:SUPERQI 0 "register_operand" "=r")
        (if_then_else:SUPERQI (inequal_op (match_operand:X 1 "register_operand" " r")
					  (match_operand:X 2 "reg_or_0_operand" "rJ"))
			      (match_operand:SUPERQI 3 "arith_operand" "0")
			      (zero_extend:SUPERQI (match_operand:SHORT 4 "register_operand" "r"))))]
  "TARGET_CMOV && TARGET_BFO"
  "<br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, <SHORT:sh_limit>, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "SI")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab><X:mode>_extend<SHORT:mode><SUPERQI:mode>2"
  [(set (match_operand:SUPERQI 0 "register_operand" "=r")
        (if_then_else:SUPERQI (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
					   (const_int 1)
					   (match_operand 1 "branch_bbcs_operand"))
					   (const_int 0))
			      (zero_extend:SUPERQI (match_operand:SHORT 2 "register_operand" "r"))
			      (match_operand:SUPERQI 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO"
  "<rev_bbcs> %4, %1, 0f\n\tnds.bfoz %0, %2, <SHORT:sh_limit>, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "SI")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab><X:mode>_extend<SHORT:mode><SUPERQI:mode>2_rev"
  [(set (match_operand:SUPERQI 0 "register_operand" "=r")
        (if_then_else:SUPERQI (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
					   (const_int 1)
					   (match_operand 1 "branch_bbcs_operand"))
					   (const_int 0))
			      (match_operand:SUPERQI 3 "arith_operand" "0")
			      (zero_extend:SUPERQI (match_operand:SHORT 2 "register_operand" "r"))))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO"
  "<bbcs> %4, %1, 0f\n\tnds.bfoz %0, %2, <SHORT:sh_limit>, 0\n\t.align 2\n0:"
  [(set_attr "type" "cmov")
   (set_attr "mode" "SI")
   (set (attr "length") (const_int 8))])

;;branch + mask
(define_insn "cmov<optab>_mask<GPR:mode><X:mode>"
  [(set (match_operand:GPR 0 "register_operand"                                   "=  r,      r")
        (if_then_else:GPR (equality_op (match_operand:X 1 "register_operand"      "   r,      r")
				       (match_operand:X 2 "reg_or_imm7u_operand"  "  rJ,   Bz07"))
			  (and:GPR (match_operand:GPR 4 "register_operand"        "   r,      r")
				   (match_operand:GPR 5 "imm_extract_operand"     "Bext,   Bext"))
			  (match_operand:GPR 3 "arith_operand"                    "   0,      0")))]
  "TARGET_CMOV && TARGET_BFO"
  {
    operands[5] = GEN_INT (__builtin_popcountll (INTVAL (operands[5])) - 1);

    switch (which_alternative)
      {
      case 0:
	return "<rev_br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, %5, 0\n\t.align 2\n0:";
      case 1:
	return "nds.<rev_br_insn>c %1, %2, 0f\n\tnds.bfoz %0, %4, %5, 0\n\t.align 2\n0:";
      default:
	gcc_unreachable ();
    }
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab>_mask<GPR:mode><X:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand"                                  "=  r,    r")
        (if_then_else:GPR (equality_op (match_operand:X 1 "register_operand"     "   r,    r")
				       (match_operand:X 2 "reg_or_imm7u_operand" "  rJ, Bz07"))
			(match_operand:GPR 3  "arith_operand"                    "   0,    0")
			(and:GPR (match_operand:GPR 4 "register_operand"         "   r,    r")
				 (match_operand:GPR 5 "imm_extract_operand"      "Bext, Bext"))))]
  "TARGET_CMOV && TARGET_BFO"
  {
    operands[5] = GEN_INT (__builtin_popcountll (INTVAL (operands[5])) - 1);

    switch (which_alternative)
      {
      case 0:
	return "<br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, %5, 0\n\t.align 2\n0:";
      case 1:
	return "nds.<br_insn>c %1, %2, 0f\n\tnds.bfoz %0, %4, %5, 0\n\t.align 2\n0:";
      default:
	gcc_unreachable ();
    }
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab>_mask<GPR:mode><X:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (inequal_op (match_operand:X 1 "register_operand" " r")
				      (match_operand:X 2 "reg_or_0_operand" "rJ"))
			  (and:GPR (match_operand:GPR 4 "register_operand"  "r")
				   (match_operand:GPR 5 "imm_extract_operand" "Bext"))
			  (match_operand:GPR 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BFO"
  {
    operands[5] = GEN_INT (__builtin_popcountll (INTVAL (operands[5])) - 1);
    return "<rev_br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, %5, 0\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov<optab>_mask<GPR:mode><X:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (inequal_op (match_operand:X 1 "register_operand" " r")
				      (match_operand:X 2 "reg_or_0_operand" "rJ"))
			  (match_operand:GPR 3 "arith_operand" "0")
			  (and:GPR (match_operand:GPR 4 "register_operand" "r")
				   (match_operand:GPR 5 "imm_extract_operand" "Bext"))))]
  "TARGET_CMOV && TARGET_BFO"
  {
    operands[5] = GEN_INT (__builtin_popcountll (INTVAL (operands[5])) - 1);
    return "<br_insn> %1, %z2, 0f\n\tnds.bfoz %0, %4, %5, 0\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab>_mask<GPR:mode><X:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				       (const_int 1)
				       (match_operand 1 "branch_bbcs_operand"))
				       (const_int 0))
			  (and:GPR (match_operand:GPR 2 "register_operand" "r")
				   (match_operand:GPR 5 "imm_extract_operand" "Bext"))
			  (match_operand:GPR 3 "arith_operand" "0")))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO"
  {
    operands[5] = GEN_INT (__builtin_popcountll (INTVAL (operands[5])) - 1);
    return "<rev_bbcs> %4, %1, 0f\n\tnds.bfoz %0, %2, %5, 0\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])

(define_insn "cmov_bb<optab>_mask<GPR:mode><X:mode>_rev"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (if_then_else:GPR (equality_op (zero_extract:X (match_operand:X 4 "register_operand" "r")
				       (const_int 1)
				       (match_operand 1 "branch_bbcs_operand"))
				       (const_int 0))
			(match_operand:GPR 3 "arith_operand" "0")
			(and:GPR (match_operand:GPR 2 "register_operand" "r")
				 (match_operand:GPR 5 "imm_extract_operand" "Bext"))))]
  "TARGET_CMOV && TARGET_BBCS && TARGET_BFO"
  {
    operands[5] = GEN_INT (__builtin_popcountll (INTVAL (operands[5])) - 1);
    return "<bbcs> %4, %1, 0f\n\tnds.bfoz %0, %2, %5, 0\n\t.align 2\n0:";
  }
  [(set_attr "type" "cmov")
   (set_attr "mode" "<GPR:MODE>")
   (set (attr "length") (const_int 8))])
