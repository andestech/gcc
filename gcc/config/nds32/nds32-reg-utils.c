
/* lmwsmw pass of Andes NDS32 cpu for GNU compiler
   Copyright (C) 2012-2016 Free Software Foundation, Inc.
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

/* ------------------------------------------------------------------------ */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "wide-int.h"
#include "inchash.h"
#include "tree.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"	/* Required by recog.h.  */
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"		/* For DFA state_t.  */
#include "insn-codes.h"		/* For CODE_FOR_xxx.  */
#include "reload.h"		/* For push_reload().  */
#include "flags.h"
#include "input.h"
#include "function.h"
#include "expr.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "dominance.h"
#include "cfg.h"
#include "cfgrtl.h"
#include "cfganal.h"
#include "lcm.h"
#include "cfgbuild.h"
#include "cfgcleanup.h"
#include "predict.h"
#include "basic-block.h"
#include "bitmap.h"
#include "df.h"
#include "tm_p.h"
#include "tm-constrs.h"
#include "optabs.h"		/* For GEN_FCN.  */
#include "target.h"
#include "langhooks.h"		/* For add_builtin_function().  */
#include "ggc.h"
#include "tree-pass.h"
#include "target-globals.h"
#include "ira.h"
#include "ira-int.h"
#include "nds32-reg-utils.h"

#define NDS32_GPR_NUM 32

static bool debug_live_reg = false;

void
nds32_live_regs (basic_block bb, rtx_insn *first, rtx_insn *last, bitmap *live)
{
  df_ref def;
  rtx_insn *insn;
  bitmap_copy (*live, DF_LR_IN (bb));
  df_simulate_initialize_forwards (bb, *live);
  rtx_insn *first_insn = BB_HEAD (bb);

  for (insn = first_insn; insn != first; insn = NEXT_INSN (insn))
    df_simulate_one_insn_forwards (bb, insn, *live);

  if (dump_file && debug_live_reg)
    {
      fprintf (dump_file, "scan live regs:\nfrom:\n");
      print_rtl_single (dump_file, first);

      fprintf (dump_file, "to:\n");
      print_rtl_single (dump_file, last);

      fprintf (dump_file, "bb lr in:\n");
      dump_bitmap (dump_file, DF_LR_IN (bb));

      fprintf (dump_file, "init:\n");
      dump_bitmap (dump_file, *live);
    }

  for (insn = first; insn != last; insn = NEXT_INSN (insn))
    {
      if (!INSN_P (insn))
	continue;

      FOR_EACH_INSN_DEF (def, insn)
	bitmap_set_bit (*live, DF_REF_REGNO (def));

      if (dump_file && debug_live_reg)
	{
	  fprintf (dump_file, "scaning:\n");
	  print_rtl_single (dump_file, insn);
	  dump_bitmap (dump_file, *live);
	}
    }

  gcc_assert (INSN_P (insn));

  FOR_EACH_INSN_DEF (def, insn)
    bitmap_set_bit (*live, DF_REF_REGNO (def));

  if (dump_file && debug_live_reg)
    {
      fprintf (dump_file, "scaning:\n");
      print_rtl_single (dump_file, last);
      dump_bitmap (dump_file, *live);
    }
}

void
print_hard_reg_set (FILE *file, const char *prefix, HARD_REG_SET set)
{
  int i;
  bool first = true;
  fprintf (file, "%s{ ", prefix);

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (TEST_HARD_REG_BIT (set, i))
	{
	  if (first)
	    {
	      fprintf (file, "%s", reg_names[i]);
	      first = false;
	    }
	  else
	    fprintf (file, ", %s", reg_names[i]);
	}
    }
  fprintf (file, "}\n");
}

void
nds32_get_available_reg_set (basic_block bb,
			     rtx_insn *first,
			     rtx_insn *last,
			     HARD_REG_SET *available_regset)
{
  bitmap live;
  HARD_REG_SET live_regset;
  unsigned i;
  live = BITMAP_ALLOC (&reg_obstack);

  nds32_live_regs (bb, first, last, &live);

  REG_SET_TO_HARD_REG_SET (live_regset, live);

  /* Reverse available_regset. */
  COMPL_HARD_REG_SET (*available_regset, live_regset);

  /* We only care $r0-$r31, so mask $r0-$r31.  */
  AND_HARD_REG_SET (*available_regset, reg_class_contents[GENERAL_REGS]);

  /* Fixed register also not available.  */
  for (i = NDS32_FIRST_GPR_REGNUM; i <= NDS32_LAST_GPR_REGNUM; ++i)
    {
      if (fixed_regs[i])
	CLEAR_HARD_REG_BIT (*available_regset, i);
    }

  BITMAP_FREE (live);
}
