/* This is a test program for smbb instruction.  */

/* { dg-do run } */

#include <nds32_intrinsic.h>
#include <stdlib.h>

#ifdef __NDS32_EXT_DSP__
int
main (void)
{
  char data[] = {0x55,0x66,0x77,0x88};
  unsigned char* char_data = (char*)& data[1];
  uint8x4_t test_char = {0x11, 0x22, 0xaa, 0xbb};
  uint8x4_t vecdata =  __nds32__get_unaligned_u8x4 (char_data);

#ifdef __NDS32_EL__
  if (vecdata[0] != 0x66)
    abort ();
#else
  if (vecdata[0] != 0x66)
    abort ();
#endif

  __nds32__put_unaligned_u8x4 (char_data, test_char);
  vecdata =  __nds32__get_unaligned_u8x4 (char_data);

  if (vecdata[0] != 0x11
      & vecdata[3] != 0xaa)
    abort ();
  else
    exit (0);
}
#else
int main(){return 0;}
#endif
