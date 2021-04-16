/* { dg-do compile } */
/* { dg-final { scan-assembler "\t.no_ifc_begin" } } */
/* { dg-final { scan-assembler "\t.no_ifc_end" } } */

void foo() __attribute__((no_ifc));
void foo()
{
}
