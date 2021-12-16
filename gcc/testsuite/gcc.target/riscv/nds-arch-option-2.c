/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32g -mabi=ilp32 -mno-atomic -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_m2p0_f2p0_d2p0\"" } } */
