/* { dg-do compile } */
/* { dg-additional-options "-std=c99 -fno-vect-cost-model -march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable" } */

#include "vfncvt-itof-template.h"

/* { dg-final { scan-assembler-times {\tvfncvt\.f\.x\.w} 2 } } */
/* { dg-final { scan-assembler-times {\tvfncvt\.f\.xu\.w} 2 } } */
