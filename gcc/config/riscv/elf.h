/* Target macros for riscv*-elf targets.
   Copyright (C) 1994-2017 Free Software Foundation, Inc.

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

#define TARGET_LINUX_ABI 0

#define LINK_SPEC "\
-melf" XLEN_SPEC "lriscv \
%{mno-relax:--no-relax} \
%{minnermost-loop:-mexecit-loop-aware} \
%{shared}" \
NDS32_EX9_SPEC \
NDS32_GP_RELAX_SPEC \
BTB_FIXUP_SPEC

/* Link against Newlib libraries, because the ELF backend assumes Newlib.
   Handle the circular dependence between libc and libgloss. */
#undef  LIB_SPEC
#define LIB_SPEC "--start-group -lc %{!specs=nosys.specs:%{mvh:-lgloss_vh;:-lgloss}} --end-group"

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

#define NO_IMPLICIT_EXTERN_C 1

#undef TARGET_LIBC_HAS_FUNCTION
#define TARGET_LIBC_HAS_FUNCTION riscv_libc_has_function
