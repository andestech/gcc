/* { dg-do run } */
/* { dg-options "-O2 -fno-inline" } */

#include <stdlib.h>

unsigned int a;
unsigned long long bar ()
{
  a = a << 25;
  return (unsigned long long) a;
}
int main(void) {

  a = 0xffffffff;
  if (bar () == 0x00000000fe000000UL)
    exit (0);
  else
    abort();
}
