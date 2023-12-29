/* { dg-do compile } */
/* { dg-additional-options "-march=rv32gcv_zvfhmin -mabi=ilp32d --param riscv-autovec-preference=fixed-vlmax -fdump-tree-vect-details" } */
/* { dg-skip-if "" { *64-*-linux* } { "*" } } */

#include "single_rgroup-2.h"

TEST_ALL (test_1)

/* { dg-final { scan-tree-dump-times "vectorized 1 loops in function" 11 "vect" } } */
