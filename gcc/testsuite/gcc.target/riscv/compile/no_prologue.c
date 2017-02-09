/* { dg-do compile } */
/* { dg-final { scan-assembler "ret" } } */

void __attribute__ ((no_prologue))
naked (void)
{
}
