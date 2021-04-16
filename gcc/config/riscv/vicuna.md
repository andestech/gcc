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
(define_cpu_unit
 "vicuna_ii, vicuna_ex, vicuna_mm, vicuna_wb"
 "vicuna_pipe")
(define_cpu_unit "vicuna_mdu" "vicuna_pipe")
(define_cpu_unit
 "vicuna_fpu_rf, vicuna_fpu_f1, vicuna_fpu_f2, vicuna_fpu_eu"
 "vicuna_pipe")

(define_reservation "vicuna_pipe"
 "vicuna_ii, vicuna_ex, vicuna_mm, vicuna_wb")

(define_reservation "vicuna_fpu_arith"
 "vicuna_ii, vicuna_fpu_rf + vicuna_ex,
  vicuna_fpu_f1 + vicuna_mm, vicuna_fpu_f2 + vicuna_wb, vicuna_fpu_eu * 2")

(define_reservation "vicuna_fpu_pipe"
 "vicuna_ii, vicuna_fpu_rf + vicuna_ex,
  vicuna_fpu_f1 + vicuna_mm, vicuna_fpu_f2 + vicuna_wb")

(define_insn_reservation "vicuna_alu_insn" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "unknown,const,arith,shift,slt,multi,nop,logical,move"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_load_wd" 2
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "SI,DI,V2HI,V4HI,V4QI,V8QI,V2SI")))
  "vicuna_pipe")

(define_insn_reservation "vicuna_load_bh" 3
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "QI,HI")))
  "vicuna_pipe")

(define_insn_reservation "vicuna_store" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "store"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_branch" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "branch,jump,call"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_imul" 10
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "imul"))
  "vicuna_pipe, vicuna_mdu * 6")

(define_insn_reservation "vicuna_idivsi" 38
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "vicuna_pipe, vicuna_mdu * 34")

(define_insn_reservation "vicuna_idivdi" 70
  (and (eq_attr "tune" "vicuna")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "vicuna_pipe, vicuna_mdu * 66")

(define_insn_reservation "vicuna_xfer" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "mfc,mtc"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_fpu_alu" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fadd"))
  "vicuna_fpu_arith")

(define_insn_reservation "vicuna_fpu_mul" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fmul"))
  "vicuna_fpu_arith")

(define_insn_reservation "vicuna_fpu_mac" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fmadd"))
  "vicuna_fpu_arith")

(define_insn_reservation "vicuna_fpu_div" 33
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fdiv"))
  "vicuna_fpu_arith, vicuna_fpu_eu * 27")

(define_insn_reservation "vicuna_fpu_sqrt" 33
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fsqrt"))
  "vicuna_fpu_arith, vicuna_fpu_eu * 27")

(define_insn_reservation "vicuna_fpu_move" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fmove,mtc,mfc"))
  "vicuna_fpu_pipe")

(define_insn_reservation "vicuna_fpu_cmp" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fcmp"))
  "vicuna_fpu_pipe")

(define_insn_reservation "vicuna_fpu_cvt" 6
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fcvt"))
  "vicuna_fpu_arith, vicuna_fpu_eu")

(define_insn_reservation "vicuna_fpu_load" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fpload"))
  "vicuna_fpu_pipe")

(define_insn_reservation "vicuna_fpu_store" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "fpstore"))
  "vicuna_fpu_pipe")

(define_insn_reservation "vicuna_dsp_alu" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dalu"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_alu64" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dalu64"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_alu_round" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "daluround"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_cmp" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dcmp"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_clip" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dclip"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_mul" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dmul"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_mac" 2
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dmac"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_insb" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dinsb"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_pack" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dpack"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_bpick" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dbpick"))
  "vicuna_pipe")

(define_insn_reservation "vicuna_dsp_wext" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "dwext"))
  "vicuna_pipe")

;; Load-to-DSP has 2 bubbles.
(define_bypass 3
  "vicuna_load_wd, vicuna_load_bh"
  "vicuna_dsp_alu, vicuna_dsp_alu64, vicuna_dsp_alu_round,\
   vicuna_dsp_cmp, vicuna_dsp_clip, vicuna_dsp_insb, vicuna_dsp_pack,\
   vicuna_dsp_bpick, vicuna_dsp_wext, vicuna_dsp_mul, vicuna_dsp_mac")

(define_bypass 2
 "vicuna_dsp_mul"
 "vicuna_dsp_alu, vicuna_dsp_alu64, vicuna_dsp_alu_round,\
  vicuna_dsp_cmp, vicuna_dsp_clip, vicuna_dsp_insb, vicuna_dsp_pack,\
  vicuna_dsp_bpick, vicuna_dsp_wext, vicuna_dsp_mul, vicuna_dsp_mac")

(define_bypass 3
 "vicuna_dsp_mac"
 "vicuna_dsp_alu, vicuna_dsp_alu64, vicuna_dsp_alu_round,\
  vicuna_dsp_cmp, vicuna_dsp_clip, vicuna_dsp_insb, vicuna_dsp_pack,\
  vicuna_dsp_bpick, vicuna_dsp_wext, vicuna_dsp_mul, vicuna_dsp_mac")
