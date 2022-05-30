/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32i -mabi=ilp32 -mext-zkn" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*_zbkb1p0_zbkc1p0_zbkx1p0_zkn1p0_zknd1p0_zkne1p0_zknh1p0\"" } } */
/* { dg-final { scan-assembler-not "_zkr1p0" } } */
/* { dg-final { scan-assembler-not "_zkt1p0" } } */
