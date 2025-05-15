;; Machine description for Zilsd extension.
;; Copyright (C) 2021 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.


;; Load Multiple Insns.
;;
;; operands[0] is the first of the consecutive registers.
;; operands[1] is the first memory location.
;; operands[2] is the number of consecutive registers.

(define_expand "load_multiple"
  [(match_par_dup 3 [(set (match_operand:SI 0 "" "")
                          (match_operand:SI 1 "" ""))
                     (use (match_operand:SI 2 "" ""))])]
  "TARGET_ZILSD && !TARGET_64BIT"
  {
    int regno, count, i;
    rtx result, mem;

    /* Zilsd only allows 2 SI load. */
    if (GET_CODE (operands[2]) != CONST_INT
        || INTVAL (operands[2]) != 2
        || GET_CODE (operands[0]) != REG
        || GET_CODE (operands[1]) != MEM
        || MEM_VOLATILE_P (operands[1]))
      FAIL;

    count = INTVAL (operands[2]);
    regno = REGNO (operands[0]);
    mem = replace_equiv_address (operands[1],
                                 force_reg (SImode, XEXP (operands[1], 0)));
    result = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count));

    for (i = 0; i < count; i++)
      XVECEXP (result, 0, i)
        = gen_rtx_SET (gen_rtx_REG (SImode, regno + i),
                       adjust_address_nv (mem, SImode, i * 4));

    operands[3] = result;
  })

(define_expand "store_multiple"
  [(match_par_dup 3 [(set (match_operand:SI 0 "")
                          (match_operand:SI 1 ""))
                     (use (match_operand:SI 2 ""))])]
  "TARGET_ZILSD && !TARGET_64BIT"
  {
    int regno, count, i;
    rtx result, mem;

    /* Zilsd only allows 2 SI store. */
    if (GET_CODE (operands[2]) != CONST_INT
        || INTVAL (operands[2]) != 2
        || GET_CODE (operands[0]) != MEM
        || GET_CODE (operands[1]) != REG
        || MEM_VOLATILE_P (operands[0]))
      FAIL;

    count = INTVAL (operands[2]);
    regno = REGNO (operands[1]);
    mem = replace_equiv_address (operands[0],
                                 force_reg (SImode, XEXP (operands[0], 0)));
    result = gen_rtx_PARALLEL (VOIDmode, rtvec_alloc (count));

    for (i = 0; i < count; i++)
      XVECEXP (result, 0, i)
        = gen_rtx_SET (adjust_address_nv (mem, SImode, i * 4),
                       gen_rtx_REG (SImode, regno + i));

    operands[3] = result;
  })

(define_insn "*load_pair"
  [(match_parallel 0 "zilsd_load_multiple_operation"
    [(set (match_operand:SI 1 "register_even_operand" "=r")
          (mem:SI (match_operand:SI 2 "register_operand" "r")))
     (set (match_operand:SI 3 "register_operand" "=r")
          (mem:SI (plus:SI (match_dup 2) (const_int 4))))])]
  "(XVECLEN (operands[0], 0) == 2) && TARGET_ZILSD && !TARGET_64BIT"
  "ld\t%1,%a2"
  [(set_attr "type" "load")
   (set_attr "mode" "SI")])

(define_insn "*store_pair"
  [(match_parallel 0 "zilsd_store_multiple_operation"
    [(set (mem:SI (match_operand:SI 1 "register_operand" "r"))
          (match_operand:SI 2 "register_even_operand" "r"))
     (set (mem:SI (plus:SI (match_dup 1) (const_int 4)))
          (match_operand:SI 3 "register_operand" "r"))])]
  "(XVECLEN (operands[0], 0) == 2) && TARGET_ZILSD && !TARGET_64BIT"
  "sd\t%2,%a1"
  [(set_attr "type" "store")
   (set_attr "mode" "SI")])
