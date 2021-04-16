/* { dg-do compile } */
/* { dg-final { scan-assembler-not "ret" } } */

void __attribute__ ((naked))
naked (void)
{
}
