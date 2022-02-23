/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32gc_zbkx -mabi=ilp32" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int32_t foo3(int32_t rs1, int32_t rs2)
{
    return _rv32_xperm8(rs1,rs2);
}

int32_t foo4(int32_t rs1, int32_t rs2)
{
    return _rv32_xperm4(rs1,rs2);
}

/* { dg-final { scan-assembler-times "xperm8" 1 } } */
/* { dg-final { scan-assembler-times "xperm4" 1 } } */