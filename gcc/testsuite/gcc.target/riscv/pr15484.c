/* { dg-do run } */
/* { dg-options "-O2" } */

#include <stdio.h>

unsigned long long foo (const unsigned int op003)
     __attribute__ ((noinline, noclone));
unsigned long long
foo (const unsigned int op003)
{
  return (0xFFFF & (0xd066 << ((0x2f ^ (0x1 & op003)) % 16)));
}

unsigned long long op0 = 0;

int
main ()
{
  if (foo (0x1) == 0x8000)
    return 0;

  __builtin_abort ();
}


