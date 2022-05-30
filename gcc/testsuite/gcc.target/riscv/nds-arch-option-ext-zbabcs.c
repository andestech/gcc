/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32i -mabi=ilp32 -mext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*_zba1p0_zbb1p0_zbc1p0_zbs1p0\"" } } */
