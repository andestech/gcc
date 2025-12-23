/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */
/* { dg-skip-if "" { *64-*-linux* } { "*" } } */

#include "vneg-template.h"

/* { dg-final { scan-assembler-times {\tvneg\.v} 4 } } */
/* { dg-final { scan-assembler-times {\tvfneg\.v} 3 } } */
