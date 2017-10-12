/* Target macros for riscv*-elf targets.
   Copyright (C) 1994-2020 Free Software Foundation, Inc.

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

#ifdef TARGET_OS_DEFAULT_EX9
#define NDS32_EX9_SPEC " %{Os3|Os|mex9:%{!mno-ex9:--mex9}}"
#else
#define NDS32_EX9_SPEC " %{mex9:--mex9}"
#endif

#ifdef TARGET_DEFAULT_GP_RELAX
#define NDS32_GP_RELAX_SPEC " %{!mno-gp-insn-relax:--mgp-insn}"
#else
#define NDS32_GP_RELAX_SPEC " %{mgp-insn-relax:--mgp-insn}"
#endif

#define LINK_SPEC "\
-melf" XLEN_SPEC "lriscv \
%{mno-relax:--no-relax} \
%{minnermost-loop:-mexecit-loop-aware} \
%{shared}" \
NDS32_EX9_SPEC \
NDS32_GP_RELAX_SPEC

/* Link against Newlib libraries, because the ELF backend assumes Newlib.
   Handle the circular dependence between libc and libgloss. */
#undef  LIB_SPEC
#define LIB_SPEC "--start-group -lc %{!specs=nosys.specs:-lgloss} --end-group"

#ifdef TARGET_MCULIB

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC \
    " %{mctor-dtor|coverage:crt1.o%s;:crt0.o%s}" \
    " %{mctor-dtor|coverage:crtbegin.o%s}"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC " %{mctor-dtor|coverage:crtend.o%s}"

#define STARTFILE_CXX_SPEC \
  " %{!mno-ctor-dtor:crt1.o%s;:crt0.o%s}" \
  " %{!mno-ctor-dtor:crtbegin.o%s}"

#define ENDFILE_CXX_SPEC \
  " %{!mno-ctor-dtor:crtend.o%s}"
#else

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crt0%O%s crtbegin%O%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend%O%s"

#endif /* TARGET_MCULIB */
