/* This is a test program for unaligned half word access.  */

/* { dg-do run } */
/* { dg-options "-O0" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned char data[] = {0x55,0x66,0x77,0x88};
  unsigned short* short_data = (unsigned short*)& data[1];
  unsigned short test_short = 0x5566;

#ifdef __NDS32_EL__
  if (__nds32__get_unaligned_hw (short_data) != 0x7766)
    abort ();
#else
  if (__nds32__get_unaligned_hw (short_data) != 0x6677)
    abort ();
#endif

  __nds32__put_unaligned_hw (short_data, test_short);

  if (__nds32__get_unaligned_hw (short_data) != 0x5566)
    abort ();
  else
    exit (0);
}
