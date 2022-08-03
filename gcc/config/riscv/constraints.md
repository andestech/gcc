;; Constraint definitions for RISC-V target.
;; Copyright (C) 2011-2020 Free Software Foundation, Inc.
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
  "@internal
   A constant call address."
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
  (and (match_code "const_int")
       (match_test "ival < (1 << 7) && ival >= 0")))

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

(define_constraint "u08"
  "Unsigned immediate 8-bit value"
  (and (match_code "const_int")
       (match_test "ival < (1 << 8) && ival >= 0")))

(define_constraint "v00"
  "Constant value 1"
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_constraint "v01"
  "Constant value 1"
  (and (match_code "const_int")
       (match_test "ival == 1")))

(define_constraint "v02"
  "Constant value 2"
  (and (match_code "const_int")
       (match_test "ival == 2")))

(define_constraint "v03"
  "Constant value 3"
  (and (match_code "const_int")
       (match_test "ival == 3")))

(define_constraint "v04"
  "Constant value 4"
  (and (match_code "const_int")
       (match_test "ival == 4")))

(define_constraint "v08"
  "Constant value 8"
  (and (match_code "const_int")
       (match_test "ival == 8")))

(define_constraint "Bext"
  "Sequence bit extract."
  (and (match_code "const_int")
       (match_test "(ival & (ival + 1)) == 0")))

(define_constraint "v15"
  "Constant value 15"
  (and (match_code "const_int")
       (match_test "ival == 15")))

(define_constraint "v16"
  "Constant value 16"
  (and (match_code "const_int")
       (match_test "ival == 16")))
