/* { dg-do compile } */
/* { dg-final { scan-assembler "\t.no_execit_begin" } } */
/* { dg-final { scan-assembler "\t.no_execit_end" } } */

void foo() __attribute__((no_ex9));
void foo()
{
}
