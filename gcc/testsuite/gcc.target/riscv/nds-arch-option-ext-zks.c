/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32i -mabi=ilp32 -mext-zks" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*_zbkb1p0_zbkc1p0_zbkx1p0_zks1p0_zksed1p0_zksh1p0\"" } } */
