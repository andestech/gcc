/* { dg-do compile } */
/* { dg-options "-mriscv-attribute -march=rv32i -mabi=ilp32 -mext-svinval" } */
int foo()
{
}
/* { dg-final { scan-assembler ".attribute arch, \".*_svinval1p0\"" } } */
