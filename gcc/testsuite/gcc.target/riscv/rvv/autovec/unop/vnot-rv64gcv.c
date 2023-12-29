/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv64gcv -mabi=lp64d --param=riscv-autovec-preference=fixed-vlmax" } */
/* { dg-skip-if "" { *32-*-linux* } { "*" } } */

#include "vnot-template.h"

/* { dg-final { scan-assembler-times {\tvnot\.v} 8 } } */
