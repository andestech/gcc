/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax" } */
/* { dg-skip-if "" { *64-*-linux* } { "*" } } */

#include "vnot-template.h"

/* { dg-final { scan-assembler-times {\tvnot\.v} 8 } } */
