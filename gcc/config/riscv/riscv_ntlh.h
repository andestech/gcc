/* RISC-V 'V' Extension intrinsics include file.
   Copyright (C) 2022-2024 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef __RISCV_NTLH_H
#define __RISCV_NTLH_H

#include <stdint.h>
#include <stddef.h>

#ifndef __riscv_zihintntl
#error "NTLH intrinsics require the NTLH extension."
#else
#ifdef __cplusplus
extern "C" {
#endif

#pragma riscv intrinsic "ntlh"

enum
{
  __RISCV_NTLH_INNERMOST_PRIVATE = 2,
  __RISCV_NTLH_ALL_PRIVATE,
  __RISCV_NTLH_INNERMOST_SHARED,
  __RISCV_NTLH_ALL
};

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __riscv_zihintntl
#endif // __RISCV_NTLH_H
