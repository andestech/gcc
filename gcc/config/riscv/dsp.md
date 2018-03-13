;; Builtin function codegen for RISC-V targets.
;; Copyright (C) 2011-2017 Free Software Foundation, Inc.
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

;; A list of the modes that are up to one word long vector.
(define_mode_iterator VQIHI [V4QI V2HI])

;; Give the number of DSP instructions in the mode
(define_mode_attr bits [(V4QI "8") (QI "8") (V2HI "16") (HI "16") (DI "64")])

(define_code_iterator all_plus [plus ss_plus us_plus])

(define_code_attr uk
  [(plus "") (ss_plus "k") (us_plus "uk")
   (minus "") (ss_minus "k") (us_minus "uk")])

(define_insn "<uk>add<mode>3"
  [(set (match_operand:VQIHI 0 "register_operand"                 "=r")
	(all_plus:VQIHI (match_operand:VQIHI 1 "register_operand" " r")
			(match_operand:VQIHI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "<uk>add<bits>\t%0, %1, %2"
  [(set_attr "type" "arith")
   (set_attr "mode" "<MODE>")])
