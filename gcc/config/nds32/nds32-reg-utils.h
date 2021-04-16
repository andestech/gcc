/* Prototypes for load-store-opt of Andes NDS32 cpu for GNU compiler
   Copyright (C) 2012-2015 Free Software Foundation, Inc.
   Contributed by Andes Technology Corporation.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef NDS32_REG_UTILS_OPT_H
#define NDS32_REG_UTILS_OPT_H

/* Auxiliary functions for register usage analysis.  */
extern void nds32_live_regs (basic_block, rtx, rtx, bitmap *);
extern void print_hard_reg_set (FILE *, const char *, HARD_REG_SET);
extern void nds32_get_available_reg_set (basic_block, rtx, rtx, HARD_REG_SET *);

static inline bool
in_reg_class_p (unsigned regno, enum reg_class clazz)
{
  return TEST_HARD_REG_BIT (reg_class_contents[clazz], regno);
}

static inline bool
in_reg_class_p (rtx reg, enum reg_class clazz)
{
  gcc_assert (REG_P (reg));
  return in_reg_class_p (REGNO (reg), clazz);
}

static inline unsigned
find_available_reg (HARD_REG_SET *available_regset, enum reg_class clazz)
{
  hard_reg_set_iterator hrsi;
  unsigned regno;
  EXECUTE_IF_SET_IN_HARD_REG_SET (reg_class_contents[clazz], 0, regno, hrsi)
    {
      /* Caller-save register or callee-save register but it's ever live.  */
      if (TEST_HARD_REG_BIT (*available_regset, regno)
	  && (call_used_regs[regno] || df_regs_ever_live_p (regno)))
	return regno;
    }

  return INVALID_REGNUM;
}



#endif
