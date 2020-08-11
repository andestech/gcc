/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32i2p0xv5_xabc -mabi=ilp32 -mno-ext-dsp" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_xv52p0_xabc2p0\"" } } */
