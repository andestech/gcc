
/* { dg-do compile } */
/* { dg-require-effective-target riscv64 } */
/* { dg-options "-O2 -march=rv64gc_zksed -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
long foo1(int32_t rs1, int32_t rs2, int bs)
{
    bs = 1;
    return _rv_sm4ks(rs1,rs2,bs);
}

long foo2(int32_t rs1, int32_t rs2, int bs)
{
    bs = 2;
    return _rv_sm4ed(rs1,rs2,bs);
}


/* { dg-final { scan-assembler-times "sm4ks" 1 } } */
/* { dg-final { scan-assembler-times "sm4ed" 1 } } */