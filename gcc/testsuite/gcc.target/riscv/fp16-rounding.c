/* Test that rounding double to __fp16 happens directly, using an example
   of a number that would round differently if it went from double to
   __fp16 via float.  */

/* { dg-do run } */
/* { dg-require-effective-target riscv_hard_fp } */
/* { dg-require-effective-target riscv_xv5 } */
/* { dg-options "-mfp16" } */

#include <stdlib.h>

/* The original double value.  */
#define ORIG 0x1.0020008p0

/* The expected (double)((__fp16)ORIG) value, by rounding toward zero.  */
#define ROUNDED 0x1.0040000p0

typedef union u {
  __fp16 f;
  unsigned short h;
} ufh;

ufh r = { ROUNDED };

double d = ORIG;

int
main (void)
{
  ufh x;

  /* Test that the rounding is correct for round towards zero.  */
  x.f = (__fp16)d;
  if (x.h != r.h)
    abort ();

  return 0;
}
