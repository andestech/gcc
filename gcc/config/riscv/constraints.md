;; Constraint definitions for RISC-V target.
;; Copyright (C) 2011-2022 Free Software Foundation, Inc.
;; Contributed by Andrew Waterman (andrew@sifive.com).
;; Based on MIPS target for GNU compiler.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Register constraints

(define_register_constraint "f" "TARGET_HARD_FLOAT ? FP_REGS : NO_REGS"
  "A floating-point register (if available).")

(define_register_constraint "j" "SIBCALL_REGS"
  "@internal")

(define_register_constraint "e" "TARGET_RVE ? T0_REG : A7_REG"
  "A syscall register.")

;; Avoid using register t0 for JALR's argument, because for some
;; microarchitectures that is a return-address stack hint.
(define_register_constraint "l" "JALR_REGS"
  "@internal")

;; General constraints

(define_constraint "I"
  "An I-type 12-bit signed immediate."
  (and (match_code "const_int")
       (match_test "SMALL_OPERAND (ival)")))

(define_constraint "J"
  "Integer zero."
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_constraint "K"
  "A 5-bit unsigned immediate for CSR access instructions."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 31)")))

(define_constraint "L"
  "A U-type 20-bit signed immediate."
  (and (match_code "const_int")
       (match_test "LUI_OPERAND (ival)")))

(define_constraint "Ds3"
  "@internal
   1, 2 or 3 immediate"
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 1, 3)")))

(define_constraint "DsS"
  "@internal
   31 immediate"
  (and (match_code "const_int")
       (match_test "(ival & 31) == 31")))

(define_constraint "DsD"
  "@internal
   63 immediate"
  (and (match_code "const_int")
       (match_test "(ival & 63) == 63")))

(define_constraint "DbS"
  "@internal"
  (and (match_code "const_int")
       (match_test "SINGLE_BIT_MASK_OPERAND (ival)")))

(define_constraint "DnS"
  "@internal"
  (and (match_code "const_int")
       (match_test "SINGLE_BIT_MASK_OPERAND (~ival)")))

(define_constraint "D03"
  "0, 1, 2 or 3 immediate"
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 3)")))

(define_constraint "DsA"
  "0 - 10 immediate"
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 10)")))

;; Floating-point constant +0.0, used for FCVT-based moves when FMV is
;; not available in RV32.
(define_constraint "G"
  "@internal"
  (and (match_code "const_double")
       (match_test "op == CONST0_RTX (mode)")))

(define_memory_constraint "A"
  "An address that is held in a general-purpose register."
  (and (match_code "mem")
       (match_test "GET_CODE(XEXP(op,0)) == REG")))

(define_constraint "S"
  "A constraint that matches an absolute symbolic address."
  (match_operand 0 "absolute_symbolic_operand"))

(define_constraint "U"
  "@internal
   A PLT-indirect call address."
  (match_operand 0 "plt_symbolic_operand"))

(define_constraint "T"
  "@internal
   A constant @code{move_operand}."
  (and (match_operand 0 "move_operand")
       (match_test "CONSTANT_P (op)")))

(define_constraint "Bz07"
  "Zero extended immediate 7-bit value"
  (and (match_test "TARGET_BIMM")
       (and (match_code "const_int")
	    (match_test "ival < (1 << 7) && ival >= 0"))))

(define_constraint "u01"
  "Unsigned immediate 1-bit value"
  (and (match_code "const_int")
       (match_test "ival == 1 || ival == 0")))

(define_constraint "u02"
  "Unsigned immediate 2-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 2) && ival >= 0")))

(define_constraint "u03"
  "Unsigned immediate 3-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 3) && ival >= 0")))

(define_constraint "u04"
  "Unsigned immediate 4-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 4) && ival >= 0")))

(define_constraint "u05"
  "Unsigned immediate 5-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 5) && ival >= 0")))

(define_constraint "u06"
  "Unsigned immediate 6-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 6) && ival >= 0")))

(define_constraint "u08"
  "Unsigned immediate 8-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 8) && ival >= 0")))

(define_constraint "D07"
  "A constraint that matches the integers 2^(0...7)."
  (and (match_code "const_int")
       (match_test "(unsigned) exact_log2 (ival) <= 7")))

(define_constraint "M00"
  "Constant value 1"
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_constraint "M01"
  "Constant value 1"
  (and (match_code "const_int")
       (match_test "ival == 1")))

(define_constraint "M02"
  "Constant value 2"
  (and (match_code "const_int")
       (match_test "ival == 2")))

(define_constraint "M03"
  "Constant value 3"
  (and (match_code "const_int")
       (match_test "ival == 3")))

(define_constraint "M04"
  "Constant value 4"
  (and (match_code "const_int")
       (match_test "ival == 4")))

(define_constraint "M08"
  "Constant value 8"
  (and (match_code "const_int")
       (match_test "ival == 8")))

(define_constraint "Bext"
  "Sequence bit extract."
  (and (match_code "const_int")
       (match_test "(ival & (ival + 1)) == 0")))

(define_constraint "M15"
  "Constant value 15"
  (and (match_code "const_int")
       (match_test "ival == 15")))

(define_constraint "M16"
  "Constant value 16"
  (and (match_code "const_int")
       (match_test "ival == 16")))

(define_address_constraint "Zpf05"
  "An address valid for a prefetch instruction."
 (match_test "riscv_address_valid_for_prefetch_p (op)"))

;; Vector constraints.

(define_register_constraint "vr" "TARGET_VECTOR ? V_REGS : NO_REGS"
  "A vector register (if available).")

(define_register_constraint "vd" "TARGET_VECTOR ? VD_REGS : NO_REGS"
  "A vector register except mask register (if available).")

(define_register_constraint "vm" "TARGET_VECTOR ? VM_REGS : NO_REGS"
  "A vector mask register (if available).")

;; This constraint is used to match instruction "csrr %0, vlenb" which is generated in "mov<mode>".
;; VLENB is a run-time constant which represent the vector register length in bytes.
;; BYTES_PER_RISCV_VECTOR represent runtime invariant of vector register length in bytes.
;; We should only allow the poly equal to BYTES_PER_RISCV_VECTOR.
(define_constraint "vp"
  "POLY_INT"
  (and (match_code "const_poly_int")
       (match_test "known_eq (rtx_to_poly_int64 (op), BYTES_PER_RISCV_VECTOR)")))

(define_constraint "vu"
  "A undefined vector value."
  (and (match_code "unspec")
       (match_test "XINT (op, 1) == UNSPEC_VUNDEF")))

(define_constraint "vi"
  "A vector 5-bit signed immediate."
  (and (match_code "const_vector")
       (match_test "riscv_vector::const_vec_all_same_in_range_p (op, -16, 15)")))

(define_constraint "vj"
  "A vector negated 5-bit signed immediate."
  (and (match_code "const_vector")
       (match_test "riscv_vector::const_vec_all_same_in_range_p (op, -15, 16)")))

(define_constraint "vk"
  "A vector 5-bit unsigned immediate."
  (and (match_code "const_vector")
       (match_test "riscv_vector::const_vec_all_same_in_range_p (op, 0, 31)")))

(define_constraint "Wc0"
  "@internal
 A constraint that matches a vector of immediate all zeros."
 (and (match_code "const_vector")
      (match_test "op == CONST0_RTX (GET_MODE (op))")))

(define_constraint "Wc1"
  "@internal
 A constraint that matches a vector of immediate all ones."
 (and (match_code "const_vector")
      (match_test "op == CONSTM1_RTX (GET_MODE (op))")))

(define_constraint "Wb1"
  "@internal
 A constraint that matches a BOOL vector of {...,0,...0,1}"
 (and (match_code "const_vector")
      (match_test "rtx_equal_p (op, riscv_vector::gen_scalar_move_mask (GET_MODE (op)))")))

(define_memory_constraint "Wdm"
  "Vector duplicate memory operand"
  (and (match_code "mem")
       (match_code "reg" "0")))
