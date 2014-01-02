/* This is a test program for unaligned double word access.  */

/* { dg-do run } */
/* { dg-options "-O0 -std=c99" } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

int
main ()
{
  unsigned char data[] = {0x55, 0x66, 0x77, 0x88, 0xAA,
			  0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  unsigned long long* long_long_data = (unsigned long long*) & data[1];
  unsigned long long test_long_long = 0x1122334455667788LL;

#ifdef __NDS32_EL__
  if (__nds32__get_unaligned_dw (long_long_data) != 0xEEDDCCBBAA887766LL)
    abort ();
#else
  if (__nds32__get_unaligned_dw (long_long_data) != 0x667788AABBCCDDEELL)
    abort ();
#endif

  __nds32__put_unaligned_dw (long_long_data, test_long_long);

  if (__nds32__get_unaligned_dw (long_long_data) != 0x1122334455667788LL)
    abort ();
  else
    exit (0);
}
