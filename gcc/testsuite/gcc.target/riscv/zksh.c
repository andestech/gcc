/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc_zksh -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
long foo1(long rs1)
{
    return _rv_sm3p0(rs1);
}

long foo2(long rs1)
{
    return _rv_sm3p1(rs1);
}


/* { dg-final { scan-assembler-times "sm3p0" 1 } } */
/* { dg-final { scan-assembler-times "sm3p1" 1 } } */