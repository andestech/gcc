/* { dg-do compile } */
/* { dg-options "-O2 -march=rv64gc_zbkb -mabi=lp64" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int64_t foo1(int64_t rs1, int64_t rs2)
{
    return _rv64_ror(rs1,rs2);
}

int64_t foo2(int64_t rs1, int64_t rs2)
{
    return _rv64_rol(rs1,rs2);
}

int64_t foo3(int64_t rs1, int64_t rs2)
{
    return _rv64_brev8(rs1);
}
/* { dg-final { scan-assembler-times "ror" 1 } } */
/* { dg-final { scan-assembler-times "rol" 1 } } */
/* { dg-final { scan-assembler-times "brev8" 1 } } */