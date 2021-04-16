;; Predicate description for RISC-V target.
;; Copyright (C) 2011-2017 Free Software Foundation, Inc.
;; Contributed by Andrew Waterman (andrew@sifive.com).
;; Based on MIPS target for GNU compiler.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_predicate "movecc_comparison_operator"
  (match_code "eq,ne,le,leu,ge,geu"))

(define_predicate "const_arith_operand"
  (and (match_code "const_int")
       (match_test "SMALL_OPERAND (INTVAL (op))")))

(define_predicate "arith_operand"
  (ior (match_operand 0 "const_arith_operand")
       (match_operand 0 "register_operand")))

(define_predicate "const_csr_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 31)")))

(define_predicate "csr_operand"
  (ior (match_operand 0 "const_csr_operand")
       (match_operand 0 "register_operand")))

(define_predicate "const_frm_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 7)")))

(define_predicate "const_insb64_operand"
  (and (match_code "const_int")
       (match_test "IN_RANGE (INTVAL (op), 0, 7)")))

(define_predicate "frm_operand"
  (ior (match_operand 0 "const_frm_operand")
       (match_operand 0 "register_operand")))

(define_predicate "sle_operand"
  (and (match_code "const_int")
       (match_test "SMALL_OPERAND (INTVAL (op) + 1)")))

(define_predicate "sleu_operand"
  (and (match_operand 0 "sle_operand")
       (match_test "INTVAL (op) + 1 != 0")))

(define_predicate "const_0_operand"
  (and (match_code "const_int,const_wide_int,const_double,const_vector")
       (match_test "op == CONST0_RTX (GET_MODE (op))")))

(define_predicate "reg_or_0_operand"
  (ior (match_operand 0 "const_0_operand")
       (match_operand 0 "register_operand")))

;; Only use branch-on-bit sequences when the mask is not an ANDI immediate.
(define_predicate "branch_on_bit_operand"
  (and (match_code "const_int")
       (match_test "INTVAL (op) >= IMM_BITS - 1")))

;; Only use branch-on-one-bit when TARGET_BBCS is on.
(define_predicate "branch_bbcs_operand"
  (match_code "const_int")
{
  if (TARGET_BBCS && (INTVAL (op) >= 0))
    {
      if (TARGET_64BIT && INTVAL (op) <= 63)
        return true;
      else if (INTVAL (op) <=31)
        return true;
      else
	return false;
    }

  return (INTVAL (op) >= IMM_BITS - 1);
})

;; A legitimate CONST_INT operand that takes more than one instruction
;; to load.
(define_predicate "splittable_const_int_operand"
  (match_code "const_int")
{
  /* Don't handle multi-word moves this way; we don't want to introduce
     the individual word-mode moves until after reload.  */
  if (GET_MODE_SIZE (mode) > UNITS_PER_WORD)
    return false;

  /* Otherwise check whether the constant can be loaded in a single
     instruction.  */
  return !LUI_OPERAND (INTVAL (op)) && !SMALL_OPERAND (INTVAL (op));
})

(define_predicate "movehf_operand"
  (match_operand 0 "general_operand")
{
  enum riscv_symbol_type symbol_type;

  /* The thinking here is as follows:

     (1) The move expanders should split complex load sequences into
	 individual instructions.  Those individual instructions can
	 then be optimized by all rtl passes.

     (2) The target of pre-reload load sequences should not be used
	 to store temporary results.  If the target register is only
	 assigned one value, reload can rematerialize that value
	 on demand, rather than spill it to the stack.

     (3) If we allowed pre-reload passes like combine and cse to recreate
	 complex load sequences, we would want to be able to split the
	 sequences before reload as well, so that the pre-reload scheduler
	 can see the individual instructions.  This falls foul of (2);
	 the splitter would be forced to reuse the target register for
	 intermediate results.

     (4) We want to define complex load splitters for combine.  These
	 splitters can request a temporary scratch register, which avoids
	 the problem in (2).  They allow things like:

	      (set (reg T1) (high SYM))
	      (set (reg T2) (low (reg T1) SYM))
	      (set (reg X) (plus (reg T2) (const_int OFFSET)))

	 to be combined into:

	      (set (reg T3) (high SYM+OFFSET))
	      (set (reg X) (lo_sum (reg T3) SYM+OFFSET))

	 if T2 is only used this once.  */
  switch (GET_CODE (op))
    {
    case CONST_INT:
    case CONST_DOUBLE:
      return false;

    case CONST:
    case SYMBOL_REF:
    case LABEL_REF:
      return riscv_symbolic_constant_p (op, &symbol_type)
	      && !riscv_split_symbol_type (symbol_type);

    case HIGH:
      op = XEXP (op, 0);
      return riscv_symbolic_constant_p (op, &symbol_type)
	      && riscv_split_symbol_type (symbol_type)
	      && symbol_type != SYMBOL_PCREL;

    default:
      return true;
    }
})

(define_predicate "move_operand"
  (match_operand 0 "general_operand")
{
  enum riscv_symbol_type symbol_type;

  /* The thinking here is as follows:

     (1) The move expanders should split complex load sequences into
	 individual instructions.  Those individual instructions can
	 then be optimized by all rtl passes.

     (2) The target of pre-reload load sequences should not be used
	 to store temporary results.  If the target register is only
	 assigned one value, reload can rematerialize that value
	 on demand, rather than spill it to the stack.

     (3) If we allowed pre-reload passes like combine and cse to recreate
	 complex load sequences, we would want to be able to split the
	 sequences before reload as well, so that the pre-reload scheduler
	 can see the individual instructions.  This falls foul of (2);
	 the splitter would be forced to reuse the target register for
	 intermediate results.

     (4) We want to define complex load splitters for combine.  These
	 splitters can request a temporary scratch register, which avoids
	 the problem in (2).  They allow things like:

	      (set (reg T1) (high SYM))
	      (set (reg T2) (low (reg T1) SYM))
	      (set (reg X) (plus (reg T2) (const_int OFFSET)))

	 to be combined into:

	      (set (reg T3) (high SYM+OFFSET))
	      (set (reg X) (lo_sum (reg T3) SYM+OFFSET))

	 if T2 is only used this once.  */
  switch (GET_CODE (op))
    {
    case CONST_INT:
      return !splittable_const_int_operand (op, mode);

    case CONST:
    case SYMBOL_REF:
    case LABEL_REF:
      return riscv_symbolic_constant_p (op, &symbol_type)
	     && !riscv_split_symbol_type (symbol_type)
	     && symbol_type != SYMBOL_FORCE_TO_MEM;

    case HIGH:
      op = XEXP (op, 0);
      return riscv_symbolic_constant_p (op, &symbol_type)
	      && riscv_split_symbol_type (symbol_type)
	      && symbol_type != SYMBOL_PCREL;

    default:
      return true;
    }
})

(define_predicate "symbolic_operand"
  (match_code "const,symbol_ref,label_ref")
{
  enum riscv_symbol_type type;
  return riscv_symbolic_constant_p (op, &type);
})

(define_predicate "absolute_symbolic_operand"
  (match_code "const,symbol_ref,label_ref")
{
  enum riscv_symbol_type type;
  return (riscv_symbolic_constant_p (op, &type)
	  && (type == SYMBOL_ABSOLUTE || type == SYMBOL_PCREL));
})

(define_predicate "pcrel_symbol_operand"
  (match_code "symbol_ref")
{
  enum riscv_symbol_type type;
  return (riscv_symbolic_constant_p (op, &type)
	  && type == SYMBOL_PCREL);
})

(define_predicate "plt_symbolic_operand"
  (match_code "const,symbol_ref,label_ref")
{
  enum riscv_symbol_type type;
  return (riscv_symbolic_constant_p (op, &type)
	  && type == SYMBOL_GOT_DISP && !SYMBOL_REF_WEAK (op) && TARGET_PLT);
})

(define_predicate "call_insn_operand"
  (match_operand 0 "general_operand")
{
  if (riscv_cmodel == CM_LARGE)
    return register_operand (op, mode);
  else
    return (absolute_symbolic_operand (op, mode) ||
	    plt_symbolic_operand (op, mode) ||
	    register_operand (op, mode));
})

(define_predicate "modular_operator"
  (match_code "plus,minus,mult,ashift"))

(define_predicate "equality_operator"
  (match_code "eq,ne"))

(define_predicate "order_operator"
  (match_code "eq,ne,lt,ltu,le,leu,ge,geu,gt,gtu"))

(define_predicate "signed_order_operator"
  (match_code "eq,ne,lt,le,ge,gt"))

(define_predicate "fp_native_comparison"
  (match_code "eq,lt,le,gt,ge"))

(define_predicate "fp_scc_comparison"
  (match_code "unordered,ordered,unlt,unge,unle,ungt,ltgt,ne,eq,lt,le,gt,ge"))

(define_predicate "fp_branch_comparison"
  (match_code "unordered,ordered,unlt,unge,unle,ungt,uneq,ltgt,ne,eq,lt,le,gt,ge"))

(define_predicate "ecall_register_operand"
  (match_code "reg,subreg")
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  return (REG_P (op)
	  && ((TARGET_RVE && (REGNO (op) == T0_REGNUM))
	      || (!TARGET_RVE && (REGNO (op) == A7_REGNUM))));
})

(define_predicate "extract_size_imm_si"
  (and (match_code "const_int")
         (match_test "IN_RANGE (INTVAL (op), 1, 32)")))

(define_predicate "extract_loc_imm_si"
  (and (match_code "const_int")
         (match_test "IN_RANGE (INTVAL (op), 0, 31)")))

(define_predicate "extract_size_imm_di"
  (and (match_code "const_int")
         (match_test "IN_RANGE (INTVAL (op), 1, 64)")))

(define_predicate "extract_loc_imm_di"
  (and (match_code "const_int")
         (match_test "IN_RANGE (INTVAL (op), 0, 63)")))

(define_predicate "branch_bimm_operand"
  (match_code "const_int")
{
  if (TARGET_BIMM)
    return satisfies_constraint_Bz07 (op);
  else
    return true;
})

(define_predicate "reg_or_imm7u_operand"
  (ior (match_operand 0 "reg_or_0_operand")
       (match_code "const_int"))
{
  if (CONST_INT_P (op))
    {
      if (op == CONST0_RTX(mode))
	return true;
      if (TARGET_BIMM)
        return satisfies_constraint_Bz07 (op);
    }
  return true;
})

(define_predicate "imm2u_operand"
  (and (match_operand 0 "const_int_operand")
       (match_test "satisfies_constraint_u02 (op)")))

(define_predicate "imm3u_operand"
  (and (match_operand 0 "const_int_operand")
       (match_test "satisfies_constraint_u03 (op)")))

(define_predicate "imm4u_operand"
  (and (match_operand 0 "const_int_operand")
       (match_test "satisfies_constraint_u04 (op)")))

(define_predicate "imm5u_operand"
  (and (match_operand 0 "const_int_operand")
       (match_test "satisfies_constraint_u05 (op)")))

(define_predicate "imm6u_operand"
  (and (match_operand 0 "const_int_operand")
       (match_test "satisfies_constraint_u06 (op)")))

(define_predicate "rimm3u_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "imm3u_operand")))

(define_predicate "rimm4u_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "imm4u_operand")))

(define_predicate "rimm5u_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "imm5u_operand")))

(define_predicate "rimm6u_operand"
  (ior (match_operand 0 "register_operand")
       (match_operand 0 "imm6u_operand")))

(define_predicate "pwr_7_operand"
  (and (match_code "const_int")
       (match_test "INTVAL (op) != 0
		    && (unsigned) exact_log2 (INTVAL (op)) <= 7")))

(define_predicate "insv_operand"
  (match_code "const_int")
{
  return INTVAL (op) == 0
	 || INTVAL (op) == 8
	 || INTVAL (op) == 16
	 || INTVAL (op) == 24;
})

(define_predicate "insv64_operand"
  (match_code "const_int")
{
  return INTVAL (op) == 0
	 || INTVAL (op) == 8
	 || INTVAL (op) == 16
	 || INTVAL (op) == 24
	 || INTVAL (op) == 32
	 || INTVAL (op) == 40
	 || INTVAL (op) == 48
	 || INTVAL (op) == 56;
})

(define_predicate "imm_0_1_operand"
  (and (match_operand 0 "const_int_operand")
       (ior (match_test "satisfies_constraint_v00 (op)")
	    (match_test "satisfies_constraint_v01 (op)"))))

(define_predicate "imm_1_2_operand"
  (and (match_operand 0 "const_int_operand")
       (ior (match_test "satisfies_constraint_v01 (op)")
	    (match_test "satisfies_constraint_v02 (op)"))))

(define_predicate "imm_2_3_operand"
  (and (match_operand 0 "const_int_operand")
       (ior (match_test "satisfies_constraint_v02 (op)")
	    (match_test "satisfies_constraint_v03 (op)"))))

(define_predicate "imm_1_2_4_8_operand"
  (and (match_operand 0 "const_int_operand")
       (ior (ior (match_test "satisfies_constraint_v01 (op)")
		 (match_test "satisfies_constraint_v02 (op)"))
	    (ior (match_test "satisfies_constraint_v04 (op)")
		 (match_test "satisfies_constraint_v08 (op)")))))

(define_predicate "imm_extract_operand"
  (match_test "satisfies_constraint_Bext (op)"))

(define_predicate "imm_15_16_operand"
  (and (match_operand 0 "const_int_operand")
       (ior (match_test "satisfies_constraint_v15 (op)")
	    (match_test "satisfies_constraint_v16 (op)"))))

(define_predicate "register_even_operand"
  (match_operand 0 "register_operand")
{
  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op); /* Possibly a MEM */

  if (!REG_P (op))
    return false;

  if (REGNO (op) >= FIRST_PSEUDO_REGISTER)
    return true;

  return ((!TARGET_64BIT
	   && GP_REG_P (REGNO (op))
	   && (REGNO (op) & 1) == 0)
	  || (TARGET_64BIT && GP_REG_P (REGNO (op))));
})

(define_predicate "reg_even_or_0_operand"
  (ior (match_operand 0 "const_0_operand")
       (match_operand 0 "register_even_operand")))
