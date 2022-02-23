/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc_zbkc -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int64_t foo1(int64_t rs1, int64_t rs2)
{
    return _rv64_clmul(rs1,rs2);
}

int64_t foo2(int64_t rs1, int64_t rs2)
{
    return _rv64_clmulh(rs1,rs2);
}

/* { dg-final { scan-assembler-times "clmul" 2 } } */
/* { dg-final { scan-assembler-times "clmulh" 1 } } */