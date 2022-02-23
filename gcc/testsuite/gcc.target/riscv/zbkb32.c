/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32gc_zbkb -mabi=ilp32" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int32_t foo1(int32_t rs1, int32_t rs2)
{
    return _rv32_ror(rs1,rs2);
}

int32_t foo2(int32_t rs1, int32_t rs2)
{
    return _rv32_rol(rs1,rs2);
}

int32_t foo3(int32_t rs1)
{
    return _rv32_brev8(rs1);
}

int32_t foo4(int32_t rs1)
{
    return _rv32_zip(rs1);
}

int32_t foo5(int32_t rs1)
{
    return _rv32_unzip(rs1);
}

/* { dg-final { scan-assembler-times "ror" 1 } } */
/* { dg-final { scan-assembler-times "rol" 1 } } */
/* { dg-final { scan-assembler-times "brev8" 1 } } */
/* { dg-final { scan-assembler-times "zip" 2 } } */
/* { dg-final { scan-assembler-times "unzip" 1 } } */