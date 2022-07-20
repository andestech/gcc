/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32ifv -mabi=ilp32f -mno-ext-dsp -mno-ext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_f2p0_v1p0_zfh1p0\"" } } */
