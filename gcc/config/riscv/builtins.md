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

(define_code_iterator any_amo [plus xor and ior smax smin umax umin])
(define_code_attr amo_optab
  [(plus "add") (xor "xor") (and "and") (ior "or")
   (smax "max") (smin "min")  (umax "maxu") (umin "minu")])

(define_int_iterator UVCSR [UNSPECV_CSRW UNSPECV_CSRS UNSPECV_CSRC])
(define_int_iterator UVCSRR [UNSPECV_CSRRW UNSPECV_CSRRS UNSPECV_CSRRC])
(define_int_attr csr_pat [(UNSPECV_CSRRW "rw")
			  (UNSPECV_CSRRS "rs")
			  (UNSPECV_CSRRC "rc")
			  (UNSPECV_CSRW "w")
			  (UNSPECV_CSRS "s")
			  (UNSPECV_CSRC "c")])

(define_insn "riscv_frflags"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec_volatile [(const_int 0)] UNSPECV_FRFLAGS))]
  "TARGET_HARD_FLOAT"
  "frflags %0")

(define_insn "riscv_fsflags"
  [(unspec_volatile [(match_operand:SI 0 "csr_operand" "rK")] UNSPECV_FSFLAGS)]
  "TARGET_HARD_FLOAT"
  "fsflags %0")

(define_insn "riscv_csrr<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "immediate_operand" "i")] UNSPECV_CSRR))]
  ""
  "csrr\t%0, %V1")

(define_insn "riscv_csr<csr_pat><GPR:mode>"
  [(unspec_volatile:GPR [(match_operand:GPR 0 "csr_operand" "r, K")
			 (match_operand:GPR 1 "immediate_operand" "i, i")] UVCSR)]
  ""
  "@
   csr<csr_pat>\t%V1,%0
   csr<csr_pat>i\t%V1,%0")

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

(define_insn "riscv_ebreak<GPR:mode>"
  [(unspec_volatile:GPR [(match_operand:GPR 0 "ecall_register_operand" "e")] UNSPECV_SBREAK)]
  ""
  "ebreak"
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

(define_insn "riscv_frcsr"
   [(set (match_operand:SI 0 "register_operand" "=r")
	 (unspec_volatile:SI [(const_int 0)] UNSPECV_FRCSR))]
  "TARGET_HARD_FLOAT"
  "frcsr\t%0"
)

(define_insn "riscv_fscsr<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "register_operand" "r")] UNSPECV_FSCSR))]
  "TARGET_HARD_FLOAT"
  "fscsr\t%0, %1"
)

(define_insn "riscv_fwcsr<GPR:mode>"
  [(unspec_volatile:GPR [(match_operand:GPR 0 "register_operand" "r")] UNSPECV_FWCSR)]
  "TARGET_HARD_FLOAT"
  "fscsr\tx0, %0"
)

(define_insn "riscv_frrm<GPR:mode>"
   [(set (match_operand:GPR 0 "register_operand" "=r")
	 (unspec_volatile:GPR [(const_int 0)] UNSPECV_FRRM))]
  "TARGET_HARD_FLOAT"
  "frrm\t%0"
)

(define_insn "riscv_fsrm<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r, r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "frm_operand" "r, Lu03")] UNSPECV_FSRM))]
  "TARGET_HARD_FLOAT"
  "@
   fsrm\t%0, %1
   fsrmi\t%0, %1"
)

(define_insn "riscv_fwrm<GPR:mode>"
  [(unspec_volatile:GPR [(match_operand:GPR 0 "frm_operand" "r, Lu03")] UNSPECV_FWRM)]
  "TARGET_HARD_FLOAT"
  "@
   fsrm\tx0, %0
   fsrmi\tx0, %0"
)

(define_insn "riscv_frflags<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec_volatile:GPR [(const_int 0)] UNSPECV_FRFLAGS))]
  "TARGET_HARD_FLOAT"
  "frflags\t%0"
)

(define_insn "riscv_fsflags<GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r, r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "csr_operand" "r, K")] UNSPECV_FSFLAGS))]
  "TARGET_HARD_FLOAT"
  "@
   fsflags\t%0, %1
   fsflagsi\t%0, %1"
)

(define_insn "riscv_fwflags<GPR:mode>"
  [(unspec_volatile:GPR [(match_operand:GPR 0 "csr_operand" "r, K")] UNSPECV_FWFLAGS)]
  "TARGET_HARD_FLOAT"
  "@
   fsflags\tx0, %0
   fsflagsi\tx0, %0"
)

(define_insn "riscv_lrw<GPR:mode>"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(mem:SI (match_operand:GPR 1 "register_operand" "r"))
			     (match_operand:SI 2 "immediate_operand" "i")] UNSPECV_LRW))]
  ""
  "lr.w%B2\t%0, (%1)"
)

(define_insn "riscv_lrd"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(mem:DI (match_operand:DI 1 "register_operand" "r"))
			     (match_operand:DI 2 "immediate_operand" "i")] UNSPECV_LRD))]
  ""
  "lr.d%B2\t%0, (%1)"
)

(define_insn "riscv_scw<GPR:mode>"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "register_operand" "r")
			     (mem:SI (match_operand:GPR 2 "register_operand" "r"))
                             (match_operand:SI 3 "immediate_operand" "i")] UNSPECV_SCW))]
  ""
  "sc.w%B3\t%0, %1, (%2)"
)

(define_insn "riscv_scd"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")
			     (mem:DI (match_operand:DI 2 "register_operand" "r"))
                             (match_operand:DI 3 "immediate_operand" "i")] UNSPECV_SCD))]
  ""
  "sc.d%B3\t%0, %1, (%2)"
)

(define_insn "riscv_amowswap<GPR:mode>"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(match_operand:SI 1 "register_operand" "r")
			     (mem:SI (match_operand:GPR 2 "register_operand" "r"))
                             (match_operand:SI 3 "immediate_operand" "i")] UNSPECV_AMOWSWAP))]
  ""
  "amoswap.w%B3\t%0, %1, (%2)"
)

(define_insn "riscv_amow<amo_optab><GPR:mode>"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec_volatile:SI [(any_amo:SI (match_operand:SI 1 "register_operand" "r")
			     		 (mem:SI (match_operand:GPR 2 "register_operand" "r")))
                             (match_operand:SI 3 "immediate_operand" "i")] UNSPECV_AMOW))]
  ""
  "amo<amo_optab>.w%B3\t%0, %1, (%2)"
)

(define_insn "riscv_amodswap"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(match_operand:DI 1 "register_operand" "r")
			     (mem:DI (match_operand:DI 2 "register_operand" "r"))
                             (match_operand:DI 3 "immediate_operand" "i")] UNSPECV_AMODSWAP))]
  ""
  "amoswap.d%B3\t%0, %1, (%2)"
)


(define_insn "riscv_amod<amo_optab>"
  [(set (match_operand:DI 0 "register_operand" "=r")
        (unspec_volatile:DI [(any_amo:DI (match_operand:DI 1 "register_operand" "r")
			     		 (mem:DI (match_operand:DI 2 "register_operand" "r")))
                             (match_operand:DI 3 "immediate_operand" "i")] UNSPECV_AMOD))]
  ""
  "amo<amo_optab>.d%B3\t%0, %1, (%2)"
)

(define_insn "riscv_csr<csr_pat><GPR:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r, r")
        (unspec_volatile:GPR [(match_operand:GPR 1 "csr_operand" "r, K")
			      (match_operand:SI 2 "immediate_operand" "i, i")] UVCSRR))]
  ""
  "@
   csr<csr_pat>\t%0, %V2, %1
   csr<csr_pat>i\t%0, %V2, %1"
)

;; String Extension

(define_insn "ffb<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec:GPR [(match_operand:GPR 1 "register_operand" "r")
		     (match_operand:GPR 2 "nonmemory_operand" "r")] UNSPEC_FFB))]
  ""
  "ffb\t%0, %1, %2"
  [(set_attr "mode" "<MODE>")])

(define_insn "ffmism<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec:GPR [(match_operand:GPR 1 "register_operand" "r")
		     (match_operand:GPR 2 "register_operand" "r")] UNSPEC_FFMISM))]
  ""
  "ffmism\t%0, %1, %2"
  [(set_attr "mode" "<MODE>")])

(define_insn "flmism<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec:GPR [(match_operand:GPR 1 "register_operand" "r")
		     (match_operand:GPR 2 "register_operand" "r")] UNSPEC_FLMISM))]
  ""
  "flmism\t%0, %1, %2"
  [(set_attr "mode" "<MODE>")])
