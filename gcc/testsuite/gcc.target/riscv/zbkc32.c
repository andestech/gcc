/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32gc_zbkc -mabi=ilp32" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int32_t foo1(int32_t rs1, int32_t rs2)
{
    return _rv32_clmul(rs1,rs2);
}

int32_t foo2(int32_t rs1, int32_t rs2)
{
    return _rv32_clmulh(rs1,rs2);
}

/* { dg-final { scan-assembler-times "clmul" 2 } } */
/* { dg-final { scan-assembler-times "clmulh" 1 } } */