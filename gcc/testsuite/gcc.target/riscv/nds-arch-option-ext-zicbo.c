/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32i -mabi=ilp32 -mext-cbo -mno-ext-dsp -mno-ext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*_zicbom1p0_zicbop1p0_zicboz1p0\"" } } */
