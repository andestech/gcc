/* target-combine pass of Andes RISCV cpu for GNU compiler
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

/* ------------------------------------------------------------------------ */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "function.h"
#include "basic-block.h"
#include "cfg.h"
#include "rtl.h"
#include "cfgrtl.h"
#include "tree-pass.h"
#include "insn-config.h"
#include "recog.h"
/* ------------------------------------------------------------------------ */

static bool find_defined_insn (const auto_vec <rtx> *set, unsigned int regno,
                               rtx *pat)
{
  int i;
  rtx candidate;

  FOR_EACH_VEC_ELT (*set, i, candidate)
    {
      if (REGNO (SET_DEST (candidate)) == regno)
        {
          *pat = candidate;
          return true;
        }
    }
  return false;
}

static void try_merge_lea_with_zext(rtx_insn *insn, auto_vec <rtx> *zext_set)
{
  rtx pat = PATTERN (insn);
  rtx zext;
  if (find_defined_insn (zext_set, REGNO (XEXP (XEXP (SET_SRC (pat), 0), 0)),
                         &zext))
    {
      if (dump_file)
        {
          fprintf(dump_file, "\nTransform %d with pattern:\n", INSN_UID (insn));
          print_rtl_single (dump_file, zext);
        }
      rtx r0 = SET_DEST (pat);
      rtx r1 = XEXP (SET_SRC (zext), 0);
      rtx r2 = XEXP (SET_SRC (pat), 1);

      if (REG_P (r1))
        {
          switch (recog_memoized (insn))
            {
            default:
              return;
            case CODE_FOR_lea_hdi:
              emit_insn_after (gen_lea_h_ze (r0, r1, r2), insn);
              break;
            case CODE_FOR_lea_wdi:
              emit_insn_after (gen_lea_w_ze (r0, r1, r2), insn);
              break;
            case CODE_FOR_lea_ddi:
              emit_insn_after (gen_lea_d_ze (r0, r1, r2), insn);
              break;
            }
        }
      else
        {
          /* r1 is a subreg */
          switch (recog_memoized (insn))
            {
            default:
              return;
            case CODE_FOR_lea_hdi:
              emit_insn_after (gen_lea_h_ze2 (r0, SUBREG_REG (r1), r2), insn);
              break;
            case CODE_FOR_lea_wdi:
              emit_insn_after (gen_lea_w_ze2 (r0, SUBREG_REG (r1), r2), insn);
              break;
            case CODE_FOR_lea_ddi:
              emit_insn_after (gen_lea_d_ze2 (r0, SUBREG_REG (r1), r2), insn);
              break;
            }
        }
      delete_insn (insn);
    }
}

static unsigned int
riscv_target_combine (void)
{

  rtx_insn *insn;
  rtx_def *r;
  basic_block bb;

  auto_vec <rtx> zext_set;

  FOR_EACH_BB_FN (bb, cfun)
      FOR_BB_INSNS (bb, insn)
        {
          if (!NONJUMP_INSN_P (insn))
            continue;

          switch (recog_memoized (insn))
            {
            case CODE_FOR_zero_extendsidi2:
              // Only handle register and subreg of DI register with 0 bytenum
              r = XEXP (SET_SRC (PATTERN (insn)), 0);
              if (REG_P (r) ||
                  (SUBREG_P (r) && GET_MODE (SUBREG_REG (r)) == DImode &&
                   SUBREG_BYTE (r) == 0))
                  zext_set.safe_push (PATTERN (insn));
              break;
            case CODE_FOR_lea_hdi:
            case CODE_FOR_lea_wdi:
            case CODE_FOR_lea_ddi:
              try_merge_lea_with_zext (insn, &zext_set);
              break;
            }
        }

  return 1;
}

const pass_data pass_data_riscv_target_combine =
{
  RTL_PASS,				/* type */
  "target_combine",				/* name */
  OPTGROUP_NONE,			/* optinfo_flags */
  TV_MACH_DEP,				/* tv_id */
  0,					/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_df_finish,		        /* todo_flags_finish */
};

class pass_riscv_target_combine : public rtl_opt_pass
{
public:
  pass_riscv_target_combine (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_riscv_target_combine, ctxt)
  {}

  /* opt_pass methods: */
  virtual bool gate (function *) { return true; }
  virtual unsigned int execute (function *) { return riscv_target_combine (); }
};

rtl_opt_pass *
make_pass_riscv_target_combine (gcc::context *ctxt)
{
  return new pass_riscv_target_combine (ctxt);
}

/* ------------------------------------------------------------------------ */
