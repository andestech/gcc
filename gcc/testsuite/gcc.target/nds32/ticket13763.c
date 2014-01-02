/* { dg-do compile } */
/* { dg-options "-O0 -fjump-tables" } */
/* { dg-final { scan-assembler ".no_ifc_begin" } } */
/* { dg-final { scan-assembler ".no_ifc_end" } } */

int lala;
void foo (int a)
{
  switch (a)
    {
    case 'a':
      lala = 1;
      break;
    case 'b':
      lala = 2;
      break;
    case 'c':
      lala = 4;
      break;
    case 'k':
      lala = 2;
      break;
    case 'z':
      lala = 5;
      break;
    default:
      lala = 10;
      break;
    }
}
