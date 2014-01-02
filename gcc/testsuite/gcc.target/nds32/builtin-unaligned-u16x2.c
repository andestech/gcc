/* This is a test program for smbb instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
int
main (void)
{
  unsigned char data[] = {0x55,0x66,0x77,0x88};
  unsigned short* short_data = (unsigned short*)& data[1];
  uint16x2_t test_short = {0x1111, 0xaaaa};
  uint16x2_t vecdata =  __nds32__get_unaligned_u16x2 (short_data);

#ifdef __NDS32_EL__
  if (vecdata[0] != 0x7766)
    abort ();
#else
  if (vecdata[0] != 0x6677)
    abort ();
#endif

  __nds32__put_unaligned_u16x2 (short_data, test_short);
  vecdata =  __nds32__get_unaligned_u16x2 (short_data);

  if (vecdata[0] != 0x1111
      & vecdata[1] != 0xaaaa)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
