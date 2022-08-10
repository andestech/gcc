/*===---- nds_vector_0p10.h - Andes vector instrinsic wrapper --------------===*/

/* Subroutines used for code generation for RISC-V.
   Copyright (C) 2011-2023 Free Software Foundation, Inc.

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

#ifndef _NDS_VECTOR_INTRINSIC_WRAPPER_H
#define _NDS_VECTOR_INTRINSIC_WRAPPER_H

#if __riscv_v_intrinsic >= 11000
#include <rvv-non-policy_intrin0p10.h>
#include <rvv-overloaded-non-policy_intrin0p10.h>
#include <rvv-overloaded-policy_intrin0p10.h>
#include <rvv-policy_intrin0p10.h>
#endif

#endif
