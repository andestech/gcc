/* { dg-do compile } */
/* { dg-options "-O2 -mcmov" } */

long
foo (float f1, float f2, long *l)
{
  return (f1 <= f2) * (*l--);
}
