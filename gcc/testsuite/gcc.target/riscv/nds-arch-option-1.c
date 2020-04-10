/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32i2p0 -mabi=ilp32 -mext-vector -mno-ext-dsp -mno-ext-zbabcs" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_v1p0\"" } } */
