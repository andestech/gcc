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

(define_insn "riscv_frflags<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec_volatile [(const_int 0)] UNSPECV_FRFLAGS))]
  "TARGET_HARD_FLOAT"
  "frflags %0")

(define_insn "riscv_fsflags<GPR:mode>"
  [(unspec_volatile [(match_operand:GPR 0 "csr_operand" "rK")] UNSPECV_FSFLAGS)]
  "TARGET_HARD_FLOAT"
  "fsflags %0")

(define_insn "riscv_csrr<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "immediate_operand" "i")] UNSPECV_CSRR))]
  ""
  "csrr\t%0, %V1")

(define_insn "riscv_csrw<GPR:mode>"
  [(unspec_volatile:GPR [(match_operand:GPR 0 "register_operand" "r")
                        (match_operand:GPR 1 "immediate_operand" "i")] UNSPECV_CSRW)]
  ""
  "csrw\t%V1, %0")

(define_expand "riscv_get_current_sp<GPR:mode>"
  [(match_operand:GPR 0 "register_operand" "")]
  ""
{
  emit_move_insn (operands[0], gen_rtx_REG (<GPR:MODE>mode, SP_REGNUM));
  DONE;
})

(define_expand "riscv_set_current_sp<GPR:mode>"
  [(match_operand:GPR 0 "register_operand" "")]
  ""
{
  emit_move_insn (gen_rtx_REG (<GPR:MODE>mode, SP_REGNUM), operands[0]);
  DONE;
})

(define_insn "riscv_ecall<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")] UNSPECV_SCALL))
   (use (reg:GPR A0_REGNUM))]

  ""
  "ecall"
)

(define_insn "riscv_ecall1<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")
			     (match_operand:GPR 2 "register_operand" "r")] UNSPECV_SCALL))]
  ""
  "ecall"
)

(define_insn "riscv_ecall2<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")
			     (match_operand:GPR 2 "register_operand" "r")
			     (match_operand:GPR 3 "register_operand" "r")] UNSPECV_SCALL))
   (use (reg:GPR A0_REGNUM))
   (use (reg:GPR A1_REGNUM))]
  ""
  "ecall"
)

(define_insn "riscv_ecall3<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")
			     (match_operand:GPR 2 "register_operand" "r")
			     (match_operand:GPR 3 "register_operand" "r")
			     (match_operand:GPR 4 "register_operand" "r")] UNSPECV_SCALL))
   (use (reg:GPR A0_REGNUM))
   (use (reg:GPR A1_REGNUM))
   (use (reg:GPR A2_REGNUM))]

  ""
  "ecall"
)

(define_insn "riscv_ecall4<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")
			     (match_operand:GPR 2 "register_operand" "r")
			     (match_operand:GPR 3 "register_operand" "r")
			     (match_operand:GPR 4 "register_operand" "r")
			     (match_operand:GPR 5 "register_operand" "r")] UNSPECV_SCALL))
   (use (reg:GPR A0_REGNUM))
   (use (reg:GPR A1_REGNUM))
   (use (reg:GPR A2_REGNUM))
   (use (reg:GPR A3_REGNUM))]

  ""
  "ecall"
)

(define_insn "riscv_ecall5<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")
			     (match_operand:GPR 2 "register_operand" "r")
			     (match_operand:GPR 3 "register_operand" "r")
			     (match_operand:GPR 4 "register_operand" "r")
			     (match_operand:GPR 5 "register_operand" "r")
			     (match_operand:GPR 6 "register_operand" "r")] UNSPECV_SCALL))
   (use (reg:GPR A0_REGNUM))
   (use (reg:GPR A1_REGNUM))
   (use (reg:GPR A2_REGNUM))
   (use (reg:GPR A3_REGNUM))
   (use (reg:GPR A4_REGNUM))]

  ""
  "ecall"
)

(define_insn "riscv_ecall6<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "ecall_register_operand" "e")
			     (match_operand:GPR 2 "register_operand" "r")
			     (match_operand:GPR 3 "register_operand" "r")
			     (match_operand:GPR 4 "register_operand" "r")
			     (match_operand:GPR 5 "register_operand" "r")
			     (match_operand:GPR 6 "register_operand" "r")
			     (match_operand:GPR 7 "register_operand" "r")] UNSPECV_SCALL))
   (use (reg:GPR A0_REGNUM))
   (use (reg:GPR A1_REGNUM))
   (use (reg:GPR A2_REGNUM))
   (use (reg:GPR A3_REGNUM))
   (use (reg:GPR A4_REGNUM))
   (use (reg:GPR A5_REGNUM))]
  ""
  "ecall"
)

(define_insn "riscv_fence"
  [(unspec_volatile [(match_operand:SI 0 "immediate_operand" "i")
		     (match_operand:SI 1 "immediate_operand" "i")] UNSPECV_FENCE)]
  ""
  "fence\t%E0, %E1")

(define_insn "riscv_fencei"
  [(unspec_volatile [(const_int 0)] UNSPECV_FENCE_I)]
  ""
  "fence.i")

