/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=fixed-vlmax -ffast-math" } */

#include "vfsqrt-template.h"

/* We cannot link this without the Zvfh extension so define
   it here instead of in the template directly (which is also
   included by the run test that might need to be linked without
   the extension).  */
TEST_TYPE3(_Float16)

/* { dg-final { scan-assembler-times {\tvfsqrt\.v} 3 } } */
