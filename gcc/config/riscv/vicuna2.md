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


(define_automaton "vicuna2_pipe")
(define_cpu_unit "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb" "vicuna2_pipe")
(define_cpu_unit "vicuna2_mdu,vicuna2_alu,vicuna2_ag" "vicuna2_pipe")

(define_insn_reservation "vicuna2_alu_insn" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "unknown,const,arith,shift,slt,multi,nop,logical,move"))
  "vicuna2_ii,vicuna2_ex+vicuna2_alu,vicuna2_wb")

(define_insn_reservation "vicuna2_load_wd" 2
  (and (eq_attr "tune" "vicuna2")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "SI,DI")))
  "vicuna2_ii,vicuna2_ex+vicuna2_ag,vicuna2_mm*2,vicuna2_wb")

(define_insn_reservation "vicuna2_load_bh" 3
  (and (eq_attr "tune" "vicuna2")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "QI,HI")))
  "vicuna2_ii,vicuna2_ex+vicuna2_ag,vicuna2_mm*3,vicuna2_wb")

(define_insn_reservation "vicuna2_store" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "store"))
  "vicuna2_ii,vicuna2_ex+vicuna2_ag,vicuna2_mm,vicuna2_wb")

(define_insn_reservation "vicuna2_branch" 0
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "branch,jump,call"))
  "vicuna2_ii,vicuna2_ex+vicuna2_ag,vicuna2_mm,vicuna2_wb")

(define_insn_reservation "vicuna2_imul" 10
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "imul"))
  "vicuna2_ii,(vicuna2_ex+vicuna2_mdu)*8,vicuna2_mm,vicuna2_wb")

(define_insn_reservation "vicuna2_idivsi" 38
  (and (eq_attr "tune" "vicuna2")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "vicuna2_ii,(vicuna2_ex+vicuna2_mdu)*36,vicuna2_mm,vicuna2_wb")

(define_insn_reservation "vicuna2_idivdi" 70
  (and (eq_attr "tune" "vicuna2")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "vicuna2_ii,(vicuna2_ex+vicuna2_mdu)*68,vicuna2_mm,vicuna2_wb")

(define_insn_reservation "vicuna2_xfer" 3
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "mfc,mtc"))
  "vicuna2_ii,vicuna2_ex+vicuna2_alu,vicuna2_mm,vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_alu" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dalu"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_alu64" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dalu64"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_alu_round" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "daluround"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_cmp" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dcmp"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_clip" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dclip"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_mul" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dmul"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_mac" 2
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dmac"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_insb" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dinsb"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_pack" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dpack"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_bpick" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dbpick"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

(define_insn_reservation "vicuna2_dsp_wext" 1
  (and (eq_attr "tune" "vicuna2")
       (eq_attr "type" "dwext"))
  "vicuna2_ii, vicuna2_ex, vicuna2_mm, vicuna2_wb")

;; Load-to-DSP has 2 bubbles.
(define_bypass 3
 "vicuna2_load_wd, vicuna2_load_bh"
 "vicuna2_dsp_alu, vicuna2_dsp_alu64, vicuna2_dsp_alu_round,\
  vicuna2_dsp_cmp, vicuna2_dsp_clip, vicuna2_dsp_insb, vicuna2_dsp_pack,\
  vicuna2_dsp_bpick, vicuna2_dsp_wext, vicuna2_dsp_mul, vicuna2_dsp_mac")

(define_bypass 2
 "vicuna2_dsp_mul"
 "vicuna2_dsp_alu, vicuna2_dsp_alu64, vicuna2_dsp_alu_round,\
  vicuna2_dsp_cmp, vicuna2_dsp_clip, vicuna2_dsp_insb, vicuna2_dsp_pack,\
  vicuna2_dsp_bpick, vicuna2_dsp_wext, vicuna2_dsp_mul, vicuna2_dsp_mac")

(define_bypass 3
 "vicuna2_dsp_mac"
 "vicuna2_dsp_alu, vicuna2_dsp_alu64, vicuna2_dsp_alu_round,\
  vicuna2_dsp_cmp, vicuna2_dsp_clip, vicuna2_dsp_insb, vicuna2_dsp_pack,\
  vicuna2_dsp_bpick, vicuna2_dsp_wext, vicuna2_dsp_mul, vicuna2_dsp_mac")
