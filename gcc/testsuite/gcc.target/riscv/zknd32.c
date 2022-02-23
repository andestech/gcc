/* { dg-do compile } */
/* { dg-options "-O2 -march=rv32gc_zknd -mabi=ilp32" } */
/* { dg-skip-if "" { *-*-* } { "-g" "-flto"} } */
#include"riscv_crypto.h"
int32_t foo1(int32_t rs1, int32_t rs2, int bs)
{
    bs = 1;
    return _rv32_aes32dsi(rs1,rs2,bs);
}

int32_t foo2(int32_t rs1, int32_t rs2, int bs)
{
    bs = 0;
    return _rv32_aes32dsmi(rs1,rs2,bs);
}

/* { dg-final { scan-assembler-times "aes32dsi" 1 } } */
/* { dg-final { scan-assembler-times "aes32dsmi" 1 } } */