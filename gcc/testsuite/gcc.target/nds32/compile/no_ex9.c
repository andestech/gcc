/* { dg-do compile } */
/* { dg-final { scan-assembler "\t.no_ex9_begin" } } */
/* { dg-final { scan-assembler "\t.no_ex9_end" } } */

void foo() __attribute__((no_ex9));
void foo()
{
}
