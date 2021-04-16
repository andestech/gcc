/* This is a test program for unaligned word access.  */

/* { dg-do run } */
/* { dg-options "-O0 -std=c99" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned char data[] = {0x55,0x66,0x77,0x88,0xAA,0xBB,0xCC,0xDD};
  unsigned int* int_data = (unsigned int*)& data[1];
  unsigned int test_int = 0x55667788;

#ifdef __NDS32_EL__
  if (__nds32__get_unaligned_w (int_data) != 0xAA887766)
    abort ();
#else
  if (__nds32__get_unaligned_w (int_data) != 0x667788AA)
    abort ();
#endif

  __nds32__put_unaligned_w (int_data, test_int);

  if (__nds32__get_unaligned_w (int_data) != 0x55667788)
    abort ();
  else
    exit (0);
}
