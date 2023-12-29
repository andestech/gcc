/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfh -mabi=ilp32d --param=riscv-autovec-preference=scalable -fdump-tree-optimized-details -fno-schedule-insns" } */

#include "ternop-3.c"

/* { dg-final { scan-assembler-times {\tvmacc\.vv} 8 } } */
/* { dg-final { scan-assembler-times {\tvfmacc\.vv} 6 } } */
/* { dg-final { scan-assembler-times {\tvmv} 11 } } */
/* { dg-final { scan-tree-dump-times "COND_LEN_FMA" 6 "optimized" } } */
