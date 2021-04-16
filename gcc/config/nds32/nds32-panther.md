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
;; Define Panther pipeline settings.
;; ------------------------------------------------------------------------

(define_automaton "nds32_pn_machine")

(define_cpu_unit "pn_i3_0" "nds32_pn_machine")
(define_cpu_unit "pn_i3_1" "nds32_pn_machine")
(define_cpu_unit "pn_e1_p0" "nds32_pn_machine")
(define_cpu_unit "pn_e2_p0" "nds32_pn_machine")
(define_cpu_unit "pn_e3_p0" "nds32_pn_machine")
(define_cpu_unit "pn_e4_p0" "nds32_pn_machine")
(define_cpu_unit "pn_wb_p0" "nds32_pn_machine")
(define_cpu_unit "pn_e1_p1" "nds32_pn_machine")
(define_cpu_unit "pn_e2_p1" "nds32_pn_machine")
(define_cpu_unit "pn_e3_p1" "nds32_pn_machine")
(define_cpu_unit "pn_e4_p1" "nds32_pn_machine")
(define_cpu_unit "pn_wb_p1" "nds32_pn_machine")
(define_cpu_unit "pn_e1_p2" "nds32_pn_machine")
(define_cpu_unit "pn_e2_p2" "nds32_pn_machine")
(define_cpu_unit "pn_e3_p2" "nds32_pn_machine")
(define_cpu_unit "pn_e4_p2" "nds32_pn_machine")
(define_cpu_unit "pn_wb_p2" "nds32_pn_machine")

(define_reservation "pn_i3" "pn_i3_0 | pn_i3_1")
(define_reservation "pn_e1" "pn_e1_p0 | pn_e1_p1")
(define_reservation "pn_e2" "pn_e2_p0 | pn_e2_p1")
(define_reservation "pn_e3" "pn_e3_p0 | pn_e3_p1")
(define_reservation "pn_e4" "pn_e4_p0 | pn_e4_p1")
(define_reservation "pn_wb" "pn_wb_p0 | pn_wb_p1")

(define_insn_reservation "nds_pn_unknown" 1
  (and (eq_attr "type" "unknown")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_misc" 1
  (and (eq_attr "type" "misc")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_mmu" 1
  (and (eq_attr "type" "mmu")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_movd44" 1
  (and (and (and (eq_attr "type" "alu")
		 (eq_attr "subtype" "simple"))
	    (match_test "nds32::movd44_insn_p (insn)"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p1, pn_e2_p1, pn_e3_p1, pn_e4_p1, pn_wb_p1")

(define_insn_reservation "nds_pn_alu" 1
  (and (and (and (eq_attr "type" "alu")
		 (eq_attr "subtype" "simple"))
	    (match_test "!nds32::movd44_insn_p (insn)"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_shift" 1
  (and (and (eq_attr "type" "alu")
	    (eq_attr "subtype" "shift"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_alu_shift" 1
  (and (eq_attr "type" "alu_shift")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_pbsad" 1
  (and (eq_attr "type" "pbsad")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3*2, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_pbsada" 1
  (and (eq_attr "type" "pbsada")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1, pn_e2, pn_e3*3, pn_e4, pn_wb")

(define_insn_reservation "nds_pn_load_full_word" 1
  (and (match_test "nds32::load_full_word_p (insn)")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_partial_word" 1
  (and (match_test "nds32::load_partial_word_p (insn)")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store" 1
  (and (match_test "nds32::store_single_p (insn)")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_1" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "1"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_2" 1
  (and (ior (and (eq_attr "type" "load_multiple")
		 (eq_attr "combo" "2"))
	    (match_test "nds32::load_double_p (insn)"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_3" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "3"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*3, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_4" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "4"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*4, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_5" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "5"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*5, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_6" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "5"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*6, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_7" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "7"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*7, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_8" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "8"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*8, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_load_multiple_12" 1
  (and (and (eq_attr "type" "load_multiple")
            (eq_attr "combo" "12"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*12, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_1" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "1"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_2" 1
  (and (ior (and (eq_attr "type" "store_multiple")
		 (eq_attr "combo" "2"))
	    (match_test "nds32::store_double_p (insn)"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*2, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_3" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "3"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*3, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_4" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "4"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*4, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_5" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "5"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*5, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_6" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "5"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*6, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_7" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "7"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*7, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_8" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "8"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*8, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_store_multiple_12" 1
  (and (and (eq_attr "type" "store_multiple")
            (eq_attr "combo" "12"))
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p2*12, pn_e2_p2, pn_e3_p2, pn_e4_p2, pn_wb_p2")

(define_insn_reservation "nds_pn_mul" 1
  (and (eq_attr "type" "mul")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p1, pn_e2_p1, pn_e3_p1, pn_e4_p1, pn_wb_p1")

(define_insn_reservation "nds_pn_mac" 1
  (and (eq_attr "type" "mac")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p1, pn_e2_p1, pn_e3_p1, pn_e4_p1, pn_wb_p1")

;; The cycles consumed in E4 stage is 32 - CLZ(abs(Ra)) + 2,
;; so the worst case is 34.
(define_insn_reservation "nds_pn_div" 1
  (and (eq_attr "type" "div")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p1, pn_e2_p1, pn_e3_p1, pn_e4_p1*34, pn_wb_p1")

(define_insn_reservation "nds_pn_branch" 1
  (and (eq_attr "type" "branch")
       (eq_attr "pipeline_model" "panther"))
  "pn_i3, pn_e1_p0, pn_e2_p0, pn_e3_p0, pn_e4_p0, pn_wb_p0")

;; SHIFT -> ADDR_IN
(define_bypass 2
  "nds_pn_shift"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_e2_to_e1_p"
)

;; ALU, MOVD44 -> ADDR_IN
(define_bypass 3
  "nds_pn_alu, nds_pn_movd44"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_e3_to_e1_p"
)

;; ALU, MOVD44 -> SHIFT, MUL, MAC_RaRb
(define_bypass 2
  "nds_pn_alu, nds_pn_movd44"
  "nds_pn_shift, nds_pn_mul, nds_pn_mac"
  "nds32_pn_e3_to_e2_p"
)

;; MUL, MAC, DIV, LW, ADDR_OUT -> ADDR_IN
(define_bypass 4
  "nds_pn_mul, nds_pn_mac, nds_pn_div,\
   nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_e4_to_e1_p"
)

;; MUL, MAC, DIV, LW, ADDR_OUT -> SHIFT, MUL, MAC_RaRb
(define_bypass 3
  "nds_pn_mul, nds_pn_mac, nds_pn_div,\
   nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds_pn_shift, nds_pn_mul, nds_pn_mac"
  "nds32_pn_e4_to_e2_p"
)

;; MUL, MAC, DIV, LW, ADDR_OUT -> ALU, MOVD44, BR_COND, ST, SMW(N, 1)
(define_bypass 2
  "nds_pn_mul, nds_pn_mac, nds_pn_div,\
   nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds_pn_alu, nds_pn_movd44, nds_pn_branch,\
   nds_pn_store,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_e4_to_e3_p"
)

;; LH, LB -> ADDR_IN
(define_bypass 5
  "nds_pn_load_partial_word"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_wb_to_e1_p"
)

;; LH, LB -> SHIFT, MUL, MAC_RaRb
(define_bypass 4
  "nds_pn_load_partial_word"
  "nds_pn_shift, nds_pn_mul, nds_pn_mac"
  "nds32_pn_wb_to_e2_p"
)

;; LH, LB -> ALU, MOVD44, BR_COND, ST, SMW(N, 1)
(define_bypass 3
  "nds_pn_load_partial_word"
  "nds_pn_alu, nds_pn_movd44, nds_pn_branch,\
   nds_pn_store,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_wb_to_e3_p"
)

;; LH, LB -> DIV
(define_bypass 2
  "nds_pn_load_partial_word"
  "nds_pn_div"
  "nds32_pn_wb_to_e4_p"
)

;; LMW(N, N) -> ADDR_IN
(define_bypass 4
  "nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_last_load_to_e1_p"
)

;; LMW(N, N) -> SHIFT, MUL, MAC_RaRb
(define_bypass 3
  "nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12"
  "nds_pn_shift, nds_pn_mul, nds_pn_mac"
  "nds32_pn_last_load_to_e2_p"
)

;; LMW(N, N - 1) -> ADDR_IN
(define_bypass 3
  "nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_last_two_load_to_e1_p"
)

;; LMW(N, N - 2) -> ADDR_IN
(define_bypass 2
  "nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12"
  "nds_pn_load_full_word, nds_pn_load_partial_word, nds_pn_store,\
   nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_last_three_load_to_e1_p"
)

;; LMW(N, N - 1) -> SHIFT, MUL, MAC_RaRb
(define_bypass 2
  "nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12"
  "nds_pn_shift, nds_pn_mul, nds_pn_mac"
  "nds32_pn_last_two_load_to_e2_p"
)

;; LMW(N, N) -> ALU, MOVD44, BR_COND
(define_bypass 2
  "nds_pn_load_multiple_1, nds_pn_load_multiple_2, nds_pn_load_multiple_3,\
   nds_pn_load_multiple_4, nds_pn_load_multiple_5, nds_pn_load_multiple_6,\
   nds_pn_load_multiple_7, nds_pn_load_multiple_8, nds_pn_load_multiple_12"
  "nds_pn_alu, nds_pn_movd44, nds_pn_branch,\
   nds_pn_store,\
   nds_pn_store_multiple_1, nds_pn_store_multiple_2, nds_pn_store_multiple_3,\
   nds_pn_store_multiple_4, nds_pn_store_multiple_5, nds_pn_store_multiple_6,\
   nds_pn_store_multiple_7, nds_pn_store_multiple_8, nds_pn_store_multiple_12"
  "nds32_pn_last_load_to_e3_p"
)
