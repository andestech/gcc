/* { dg-do compile } */
/* { dg-options "-O -mriscv-attribute -march=rv32ip -mabi=ilp32" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \"rv32i2p0_p0p5\"" } } */
