/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -march=rv32gcv -mabi=ilp32d -fno-vect-cost-model --param=riscv-autovec-preference=fixed-vlmax -fno-builtin" } */
/* { dg-skip-if "" { *64-*-linux* } { "*" } } */

#include "vmv-imm-template.h"

/* { dg-final { scan-assembler-times {vmv.v.i} 32 } } */
/* { dg-final { scan-assembler-times {vmv.v.x} 8 } } */
