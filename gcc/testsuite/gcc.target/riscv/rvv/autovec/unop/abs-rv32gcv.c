/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include "abs-template.h"

/* { dg-final { scan-assembler-times {\tvseti?vli\s+[a-z0-9,]+,ta,mu} 4 } } */
/* { dg-final { scan-assembler-times {\tvmslt\.vi} 4 } } */
/* { dg-final { scan-assembler-times {\tvneg.v\sv[0-9]+,v[0-9]+,v0\.t} 4 } } */
/* { dg-final { scan-assembler-times {\tvfabs.v} 3 } } */
