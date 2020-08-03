/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32ixv5 -mabi=ilp32 -mext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_xv52p0_xdsp2p0\"" } } */
