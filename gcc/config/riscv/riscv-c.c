/* RISC-V-specific code for C family languages.
   Copyright (C) 2011-2020 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define IN_TARGET_CODE 1

#define INCLUDE_STRING
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "c-family/c-common.h"
#include "cpplib.h"
#include "riscv-subset.h"

#define builtin_define(TXT) cpp_define (pfile, TXT)

/* Implement TARGET_CPU_CPP_BUILTINS.  */

void
riscv_cpu_cpp_builtins (cpp_reader *pfile)
{
  builtin_define ("__ANDES");
  builtin_define ("__riscv");

  if (riscv_virtual_hosting)
    builtin_define ("__riscv_virtual_hosting");

  if (TARGET_RVC)
    builtin_define ("__riscv_compressed");

  if (TARGET_RVE)
    builtin_define ("__riscv_32e");

  if (TARGET_RVV)
    builtin_define ("__riscv_vector");

  if (TARGET_ATOMIC)
    builtin_define ("__riscv_atomic");

  if (TARGET_DSP)
    builtin_define ("__riscv_dsp");

  if (TARGET_MUL)
    builtin_define ("__riscv_mul");
  if (TARGET_DIV)
    builtin_define ("__riscv_div");
  if (TARGET_DIV && TARGET_MUL)
    builtin_define ("__riscv_muldiv");

  builtin_define_with_int_value ("__riscv_xlen", UNITS_PER_WORD * 8);
  if (TARGET_HARD_FLOAT)
    builtin_define_with_int_value ("__riscv_flen", UNITS_PER_FP_REG * 8);

  if (TARGET_HARD_FLOAT && TARGET_FDIV)
    {
      builtin_define ("__riscv_fdiv");
      builtin_define ("__riscv_fsqrt");
    }

  switch (riscv_abi)
    {
    case ABI_ILP32E:
      builtin_define ("__riscv_abi_rve");
      gcc_fallthrough ();

    case ABI_ILP32:
    case ABI_LP64:
      builtin_define ("__riscv_float_abi_soft");
      break;

    case ABI_ILP32F:
    case ABI_LP64F:
      builtin_define ("__riscv_float_abi_single");
      break;

    case ABI_ILP32D:
    case ABI_LP64D:
      builtin_define ("__riscv_float_abi_double");
      break;
    }

  switch (riscv_cmodel)
    {
    case CM_MEDLOW:
      builtin_define ("__riscv_cmodel_medlow");
      break;

    case CM_MEDANY:
      builtin_define ("__riscv_cmodel_medany");
      break;

    case CM_LARGE:
      builtin_define ("__riscv_cmodel_large");
      break;

    case CM_PIC:
      builtin_define ("__riscv_cmodel_pic");
      break;

    default:
      break;
    }

  if (TARGET_V5)
    {
      builtin_define ("__nds_v5");

      if (TARGET_EXECIT && TARGET_RVC)
	{
	  builtin_define ("__nds_execit");

	  /* Also define __nds_ex9 for backward compatibility.  */
	  builtin_define ("__nds_ex9");
	}
    }

  if (TARGET_FP16)
    builtin_define ("__nds_fp16");

  if (TARGET_SOFT_FP16)
    builtin_define ("__nds_soft_fp16");

  if (TARGET_BF16) 
    builtin_define ("__nds_bf16");
  
  if (TARGET_BFO)
    builtin_define ("__nds_bfo");
  if (TARGET_LEA)
    builtin_define ("__nds_lea");
  if (TARGET_BBCS)
    builtin_define ("__nds_bbcs");
  if (TARGET_BIMM)
    builtin_define ("__nds_bimm");

  /* Define architecture extension test macros.  */
  builtin_define_with_int_value ("__riscv_arch_test", 1);

  const riscv_subset_list *subset_list = riscv_current_subset_list ();
  size_t max_ext_len = 0;

  /* Figure out the max length of extension name for reserving buffer.   */
  for (const riscv_subset_t *subset = subset_list->begin ();
       subset != subset_list->end ();
       subset = subset->next)
    max_ext_len = MAX (max_ext_len, subset->name.length ());

  char *buf = (char *)alloca (max_ext_len + 10 /* For __riscv_ and '\0'.  */);

  for (const riscv_subset_t *subset = subset_list->begin ();
       subset != subset_list->end ();
       subset = subset->next)
    {
      int version_value = (subset->major_version * 1000000)
			   + (subset->minor_version * 1000);
      /* Special rule for zicsr and zifencei, it's used for ISA spec 2.2 or
	 earlier.  */
      if ((subset->name == "zicsr" || subset->name == "zifencei")
	  && version_value == 0)
	version_value = 2000000;

      sprintf (buf, "__riscv_%s", subset->name.c_str ());
      builtin_define_with_int_value (buf, version_value);
    }
}
