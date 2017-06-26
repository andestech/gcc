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

(define_insn "riscv_frflags"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec_volatile [(const_int 0)] UNSPECV_FRFLAGS))]
  "TARGET_HARD_FLOAT"
  "frflags %0")

(define_insn "riscv_fsflags"
  [(unspec_volatile [(match_operand:SI 0 "csr_operand" "rK")] UNSPECV_FSFLAGS)]
  "TARGET_HARD_FLOAT"
  "fsflags %0")

(define_insn "riscv_csrr"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "immediate_operand" "i")] UNSPECV_CSRR))]
  ""
  "csrr\t%0, %V1")

(define_insn "riscv_csrw"
  [(unspec_volatile:SI [(match_operand:SI 0 "register_operand" "r")
                        (match_operand:SI 1 "immediate_operand" "i")] UNSPECV_CSRW)]
  ""
  "csrw\t%V1, %0")

(define_expand "riscv_get_current_sp"
  [(match_operand:SI 0 "register_operand" "")]
  ""
{
  emit_move_insn (operands[0], gen_rtx_REG (SImode, SP_REGNUM));
  DONE;
})

(define_expand "riscv_get_current_spdi"
  [(match_operand:DI 0 "register_operand" "")]
  ""
{
  emit_move_insn (operands[0], gen_rtx_REG (DImode, SP_REGNUM));
  DONE;
})

(define_expand "riscv_set_current_sp"
  [(match_operand:SI 0 "register_operand" "")]
  ""
{
  emit_move_insn (gen_rtx_REG (SImode, SP_REGNUM), operands[0]);
  DONE;
})

(define_expand "riscv_set_current_spdi"
  [(match_operand:DI 0 "register_operand" "")]
  ""
{
  emit_move_insn (gen_rtx_REG (DImode, SP_REGNUM), operands[0]);
  DONE;
})
