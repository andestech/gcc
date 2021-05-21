/* { dg-do compile } */
/* { dg-options "-march=rv32ifv -mabi=ilp32f -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_f2p0_v1p0_zfh0p1\"" } } */
