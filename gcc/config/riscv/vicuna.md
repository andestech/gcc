;; Generic DFA-based pipeline description for RISC-V targets.
;; Copyright (C) 2011-2016 Free Software Foundation, Inc.
;; Contributed by Andrew Waterman (andrew@sifive.com).
;; Based on MIPS target for GNU compiler.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.


(define_automaton "vicuna_pipe")
(define_cpu_unit "vicuna_alu" "vicuna_pipe")
(define_cpu_unit "vicuna_mdu" "vicuna_pipe")
(define_cpu_unit "vicuna_ag" "vicuna_pipe")
(define_cpu_unit "vicuna_fpu" "vicuna_pipe")

(define_insn_reservation "vicuna_alu_insn" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "unknown,const,arith,shift,slt,multi,nop,logical,move"))
  "vicuna_alu")

(define_insn_reservation "vicuna_load_wd" 2
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "SI,DI")))
  "vicuna_alu+vicuna_ag")

(define_insn_reservation "vicuna_load_bh" 3
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "QI,HI")))
  "vicuna_alu+vicuna_ag")

(define_insn_reservation "vicuna_store" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "store"))
  "vicuna_alu+vicuna_ag")

(define_insn_reservation "vicuna_branch" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "branch,jump,call"))
  "vicuna_alu+vicuna_ag")

(define_insn_reservation "vicuna_imul" 10
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "imul"))
  "vicuna_mdu*8")

(define_insn_reservation "vicuna_idivsi" 38
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "vicuna_mdu*36")

(define_insn_reservation "vicuna_idivdi" 70
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "vicuna_mdu*68")

(define_insn_reservation "vicuna_xfer" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "mfc,mtc"))
  "vicuna_alu")

(define_insn_reservation "vicuna_fpu_alu" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fadd"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_mul" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fmul"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_mac" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fmadd"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_div" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fdiv"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_sqrt" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fsqrt"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_move" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fmove,mtc,mfc"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_cmp" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fcmp"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_cvt" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fcvt"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_load" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fpload"))
  "vicuna_fpu")

(define_insn_reservation "vicuna_fpu_store" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fpstore"))
  "vicuna_fpu")
