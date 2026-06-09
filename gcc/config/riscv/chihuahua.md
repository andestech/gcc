;; Generic DFA-based pipeline description for RISC-V targets.
;; Copyright (C) 2011-2022 Free Software Foundation, Inc.
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

(define_automaton "chihuahua_arch")
(define_cpu_unit
 "chihuahua_alu0, chihuahua_alu1,
  chihuahua_lsu0, chihuahua_lsu1, chihuahua_lsu2,
  chihuahua_dsp"
 "chihuahua_arch")
(define_cpu_unit "chihuahua_mdu" "chihuahua_arch")
(define_cpu_unit "chihuahua_fpu" "chihuahua_arch")

(define_reservation "chihuahua_pipe"
 "chihuahua_alu0 | chihuahua_alu1")

(define_reservation "chihuahua_lsu"
 "chihuahua_lsu0 | chihuahua_lsu1 | chihuahua_lsu2")  

(define_reservation "chihuahua_pipe_unify"
 "chihuahua_alu0 + chihuahua_alu1")

(define_insn_reservation "chihuahua_alu_insn" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "unknown,const,arith,slt,multi,nop,move,
                        shift,logical,mvpair"))
  "chihuahua_pipe")

(define_insn_reservation "chihuahua_load" 3
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "load"))
  "chihuahua_pipe_unify, chihuahua_lsu*3")

(define_insn_reservation "chihuahua_store" 0
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "store"))
  "chihuahua_pipe_unify,chihuahua_lsu*3")

(define_insn_reservation "chihuahua_branch" 0
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "branch,jump,call"))
  "chihuahua_pipe_unify")

(define_insn_reservation "chihuahua_imul" 2
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "imul"))
  "chihuahua_alu0, chihuahua_mdu")

(define_insn_reservation "chihuahua_idivsi" 35
  (and (eq_attr "tune" "chihuahua")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "chihuahua_pipe_unify, chihuahua_mdu* 34")

(define_insn_reservation "chihuahua_idivdi" 35
  (and (eq_attr "tune" "chihuahua")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "chihuahua_pipe_unify, chihuahua_mdu* 34")

(define_insn_reservation "chihuahua_xfer" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "mfc,mtc"))
  "chihuahua_pipe")

(define_insn_reservation "chihuahua_fpu_alu" 4
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fadd"))
  "chihuahua_pipe_unify, chihuahua_fpu")

(define_insn_reservation "chihuahua_fpu_mul" 4
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fmul"))
  "chihuahua_pipe_unify, chihuahua_fpu")

(define_insn_reservation "chihuahua_fpu_mac" 4
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fmadd"))
  "chihuahua_pipe_unify, chihuahua_fpu")

(define_insn_reservation "chihuahua_fpu_div" 33
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fdiv"))
  "chihuahua_pipe_unify, chihuahua_fpu*33")

(define_insn_reservation "chihuahua_fpu_sqrt" 33
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fsqrt"))
  "chihuahua_pipe_unify, chihuahua_fpu*33")

(define_insn_reservation "chihuahua_fpu_move" 2
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fmove,mtc,mfc"))
  "chihuahua_pipe_unify, chihuahua_fpu")

(define_insn_reservation "chihuahua_fpu_cmp" 3
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fcmp"))
  "chihuahua_pipe_unify, chihuahua_fpu")

(define_insn_reservation "chihuahua_fpu_cvt" 3
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fcvt"))
  "chihuahua_pipe_unify, chihuahua_fpu")

(define_insn_reservation "chihuahua_fpu_load" 3
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fpload"))
  "chihuahua_pipe_unify, chihuahua_lsu*3")

(define_insn_reservation "chihuahua_fpu_store" 0
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "fpstore"))
  "chihuahua_pipe_unify, chihuahua_lsu*3")

(define_insn_reservation "chihuahua_dsp_alu" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dalu"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_alu64" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dalu64"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_alu_round" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "daluround"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_cmp" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dcmp"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_clip" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dclip"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_mul" 2
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dmul"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_mac" 2
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dmac"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_insb" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dinsb"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_pack" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dpack"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_bpick" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dbpick"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_dsp_wext" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "dwext"))
  "chihuahua_alu0, chihuahua_dsp")

(define_insn_reservation "chihuahua_bitmanip" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "bitmanip"))
  "chihuahua_alu0")

(define_insn_reservation "chihuahua_crypto" 1
  (and (eq_attr "tune" "chihuahua")
       (eq_attr "type" "crypto"))
  "chihuahua_alu0")

(define_bypass 3
  "chihuahua_fpu_mul"
  "chihuahua_fpu_alu,chihuahua_fpu_mac,
   chihuahua_fpu_div,chihuahua_fpu_sqrt")

(define_bypass 3
  "chihuahua_fpu_alu"
  "chihuahua_fpu_mul,chihuahua_fpu_alu,chihuahua_fpu_mac,
   chihuahua_fpu_div,chihuahua_fpu_sqrt")

(define_bypass 3
  "chihuahua_fpu_mac"
  "chihuahua_fpu_mul,chihuahua_fpu_alu,chihuahua_fpu_mac,
   chihuahua_fpu_div,chihuahua_fpu_sqrt")

(define_bypass 2
  "chihuahua_fpu_load"
  "chihuahua_fpu_div,chihuahua_fpu_sqrt")
