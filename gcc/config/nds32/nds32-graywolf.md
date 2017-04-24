;; Pipeline descriptions of Andes NDS32 cpu for GNU compiler
;; Copyright (C) 2012-2013 Free Software Foundation, Inc.
;; Contributed by Andes Technology Corporation.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; ------------------------------------------------------------------------
;; Define Graywolf pipeline settings.
;; ------------------------------------------------------------------------

(define_automaton "nds32_graywolf_machine")

(define_cpu_unit "gw_ii_0" "nds32_graywolf_machine")
(define_cpu_unit "gw_ii_1" "nds32_graywolf_machine")
(define_cpu_unit "gw_ex_p0" "nds32_graywolf_machine")
(define_cpu_unit "gw_mm_p0" "nds32_graywolf_machine")
(define_cpu_unit "gw_wb_p0" "nds32_graywolf_machine")
(define_cpu_unit "gw_ex_p1" "nds32_graywolf_machine")
(define_cpu_unit "gw_mm_p1" "nds32_graywolf_machine")
(define_cpu_unit "gw_wb_p1" "nds32_graywolf_machine")

(define_reservation "gw_ii" "gw_ii_0 | gw_ii_1")
(define_reservation "gw_ex" "gw_ex_p0 | gw_ex_p1")
(define_reservation "gw_mm" "gw_mm_p0 | gw_mm_p1")
(define_reservation "gw_wb" "gw_wb_p0 | gw_wb_p1")

(define_insn_reservation "nds_gw_unknown" 1
  (and (eq_attr "type" "unknown")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_misc" 1
  (and (eq_attr "type" "misc")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_mmu" 1
  (and (eq_attr "type" "mmu")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_alu" 1
  (and (eq_attr "type" "alu")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_alu_shift" 1
  (and (eq_attr "type" "alu_shift")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex*2, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_pbsad" 1
  (and (eq_attr "type" "pbsad")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex*3, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_pbsada" 1
  (and (eq_attr "type" "pbsada")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii, gw_ex*3, gw_mm, gw_wb")

(define_insn_reservation "nds_gw_load" 1
  (and (eq_attr "type" "load")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store" 1
  (and (eq_attr "type" "store")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_1" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "1"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_2" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "1"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*2, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_3" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "3"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*3, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_4" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "4"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_5" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "5"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_6" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "6"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_7" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "7"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_8" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "8"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_load_multiple_12" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "12"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_1" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "1"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_2" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "1"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*2, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_3" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "3"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*3, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_4" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "4"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_5" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "5"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_6" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "6"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_7" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "7"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_8" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "8"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_store_multiple_12" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "12"))
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_1, gw_ex_p1*4, gw_mm_p1, gw_wb_p1")

(define_insn_reservation "nds_gw_mul_fast1" 1
  (and (match_test "nds32_mul_config == MUL_TYPE_FAST_1")
       (and (eq_attr "type" "mul")
       (eq_attr "pipeline_model" "graywolf")))
  "gw_ii_0, gw_ex_p0, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_mul_fast2" 1
  (and (match_test "nds32_mul_config == MUL_TYPE_FAST_2")
       (and (eq_attr "type" "mul")
       (eq_attr "pipeline_model" "graywolf")))
  "gw_ii_0, gw_ex_p0*2, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_mul_slow" 1
  (and (match_test "nds32_mul_config == MUL_TYPE_SLOW")
       (and (eq_attr "type" "mul")
       (eq_attr "pipeline_model" "graywolf")))
  "gw_ii_0, gw_ex_p0*4, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_mac_fast1" 1
  (and (match_test "nds32_mul_config == MUL_TYPE_FAST_1")
       (and (eq_attr "type" "mac")
       (eq_attr "pipeline_model" "graywolf")))
  "gw_ii_0, gw_ex_p0, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_mac_fast2" 1
  (and (match_test "nds32_mul_config == MUL_TYPE_FAST_2")
       (and (eq_attr "type" "mac")
       (eq_attr "pipeline_model" "graywolf")))
  "gw_ii_0, gw_ex_p0*2, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_mac_slow" 1
  (and (match_test "nds32_mul_config == MUL_TYPE_SLOW")
       (and (eq_attr "type" "mac")
       (eq_attr "pipeline_model" "graywolf")))
  "gw_ii_0, gw_ex_p0*4, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_div" 1
  (and (eq_attr "type" "div")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_0, gw_ex_p0*4, gw_mm_p0, gw_wb_p0")

(define_insn_reservation "nds_gw_branch" 1
  (and (eq_attr "type" "branch")
       (eq_attr "pipeline_model" "graywolf"))
  "gw_ii_0, gw_ex_p0, gw_mm_p0, gw_wb_p0")

;; LD, MUL, MAC, DIV
;;   -> ALU, ALU_SHIFT_Rb, PBSAD, PBSADA_RaRb, MOVD44, MUL, MAC_RaRb, DIV, ADDR_IN, BR, MMU
(define_bypass 2
  "nds_gw_load,\
   nds_gw_mul_fast1, nds_gw_mul_fast2, nds_gw_mul_slow,\
   nds_gw_mac_fast1, nds_gw_mac_fast2, nds_gw_mac_slow,\
   nds_gw_div"
  "nds_gw_alu, nds_gw_alu_shift,\
   nds_gw_pbsad, nds_gw_pbsada,\
   nds_gw_mul_fast1, nds_gw_mul_fast2, nds_gw_mul_slow,\
   nds_gw_mac_fast1, nds_gw_mac_fast2, nds_gw_mac_slow,\
   nds_gw_branch,\
   nds_gw_div,\
   nds_gw_load,nds_gw_store,\
   nds_gw_load_multiple_1,nds_gw_load_multiple_2, nds_gw_load_multiple_3,\
   nds_gw_load_multiple_4,nds_gw_load_multiple_5, nds_gw_load_multiple_6,\
   nds_gw_load_multiple_7,nds_gw_load_multiple_8, nds_gw_load_multiple_12,\
   nds_gw_store_multiple_1,nds_gw_store_multiple_2, nds_gw_store_multiple_3,\
   nds_gw_store_multiple_4,nds_gw_store_multiple_5, nds_gw_store_multiple_6,\
   nds_gw_store_multiple_7,nds_gw_store_multiple_8, nds_gw_store_multiple_12,\
   nds_gw_mmu"
  "nds32_gw_mm_to_ex_p"
)

;; LMW(N, N)
;;   -> ALU, ALU_SHIFT_Rb, PBSAD, PBSADA_RaRb, MOVD44, MUL, MAC_RaRb, DIV, ADDR_IN, BR, MMU
(define_bypass 2
  "nds_gw_load_multiple_1,nds_gw_load_multiple_2, nds_gw_load_multiple_3,\
   nds_gw_load_multiple_4,nds_gw_load_multiple_5, nds_gw_load_multiple_6,\
   nds_gw_load_multiple_7,nds_gw_load_multiple_8, nds_gw_load_multiple_12"
  "nds_gw_alu, nds_gw_alu_shift,\
   nds_gw_pbsad, nds_gw_pbsada,\
   nds_gw_mul_fast1, nds_gw_mul_fast2, nds_gw_mul_slow,\
   nds_gw_mac_fast1, nds_gw_mac_fast2, nds_gw_mac_slow,\
   nds_gw_branch,\
   nds_gw_div,\
   nds_gw_load,nds_gw_store,\
   nds_gw_load_multiple_1,nds_gw_load_multiple_2, nds_gw_load_multiple_3,\
   nds_gw_load_multiple_4,nds_gw_load_multiple_5, nds_gw_load_multiple_6,\
   nds_gw_load_multiple_7,nds_gw_load_multiple_8, nds_gw_load_multiple_12,\
   nds_gw_store_multiple_1,nds_gw_store_multiple_2, nds_gw_store_multiple_3,\
   nds_gw_store_multiple_4,nds_gw_store_multiple_5, nds_gw_store_multiple_6,\
   nds_gw_store_multiple_7,nds_gw_store_multiple_8, nds_gw_store_multiple_12,\
   nds_gw_mmu"
  "nds32_gw_last_load_to_ex_p"
)
