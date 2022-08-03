/* Subroutines for the gcc driver.
   Copyright (C) 2011-2017 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).
   Based on MIPS target for GNU compiler.

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "configargs.h"
#include <string>
#include "riscv-protos.h"

#define MAX_ARCH_STRING_LEN 256
static char riscv_arch_string[MAX_ARCH_STRING_LEN];

struct arch_options_t
{
  const char *short_option;
  const char *long_option;
  bool *val;
};

static bool nds_ext = false;
static bool dsp_ext = false;
static bool _dsp_ext = false;
static bool std_ext[26];
static  bool rv64_p = false;

static arch_options_t arch_options[] = {
  {"m", NULL, &std_ext['m' - 'a']},
  {"a", "atomic", &std_ext['a' - 'a']},
  {"f", NULL, &std_ext['f' - 'a']},
  {"d", NULL, &std_ext['d' - 'a']},
  {"c", "16-bit", &std_ext['c' - 'a']},
  {"xv5", "nds", &nds_ext},
  {"xdsp", "ext-dsp", &dsp_ext},
  {"_xdsp", "ext-dsp", &_dsp_ext},
  {NULL, NULL, NULL},
};

static bool
std_ext_end_p (const arch_options_t *opt)
{
  return (opt->short_option == NULL) &&
	 (opt->long_option == NULL) &&
	 (opt->val == NULL);
}

static void
set_std_ext (char c, bool val)
{
  std_ext[c - 'a'] = val;
}

static bool
get_std_ext (char c)
{
  return std_ext[c - 'a'];
}

static void
parsing_march (const char *march)
{
  const char *p = march;
  if (strncmp (p, "rv64i", 5) == 0)
    rv64_p = true;
  else if (strncmp (p, "rv32", 4) == 0)
    {
      set_std_ext ('e', p[4] == 'e');
    }
  else
    /* Wrong arch string.  */
    return;
  p += 5;

  arch_options_t *opt = &arch_options[0];

  while (*p)
    {
      bool found = false;
      for (;!std_ext_end_p (opt); ++opt)
	{
	  if (!opt->short_option)
	    continue;

	  int opt_len = strlen (opt->short_option);
	  if (strncmp (opt->short_option, p, opt_len) == 0)
	    {
	      *opt->val = true;
	      p += opt_len;
	      found = true;
	      break;
	    }
	}
      if (!found)
	break;
    }
}

void
parsing_long_option (const char *long_opt)
{
  arch_options_t *opt = &arch_options[0];

  bool val = true;

  if (strncmp(long_opt, "mno-", 4) == 0)
    {
      val = false;
      long_opt += 4;
    }
  else
    long_opt += 1;

  for (;!std_ext_end_p (opt); ++opt)
    if (opt->long_option
	&& (strncmp (opt->long_option, long_opt,
		     strlen (opt->long_option)) == 0))
      {
	*opt->val = val;
	return;
      }
}

const char *
riscv_arch (int argc ATTRIBUTE_UNUSED, const char **argv ATTRIBUTE_UNUSED)
{
  std::string str;
  int i;

  // Only deal with the last march=
  for (i = argc - 1; i >= 0; --i)
    {
      if (strncmp (argv[i], "march=", 6) == 0)
        {
          parsing_march (argv[i] + 6);
          break;
        }
    }

  for (i = 0; i <argc; ++i)
    {
      if (strcmp (argv[i], "-") == 0)
	continue;
      parsing_long_option (argv[i]);
    }

  arch_options_t *opt = &arch_options[0];

  if (rv64_p)
    str = "rv64i";
  else if (get_std_ext ('e'))
    str = "rv32e";
  else
    str = "rv32i";

  if (dsp_ext)
    {
      if (nds_ext)
	{
	  dsp_ext = false;
	  _dsp_ext = true;
	}
      else
	_dsp_ext = false;
    }

  for (;!std_ext_end_p (opt); ++opt)
    if (*opt->val && opt->short_option)
      str += opt->short_option;

  gcc_assert (str.length () < MAX_ARCH_STRING_LEN);
  strncpy (riscv_arch_string, str.c_str(), MAX_ARCH_STRING_LEN);
  return &riscv_arch_string[0];
}
