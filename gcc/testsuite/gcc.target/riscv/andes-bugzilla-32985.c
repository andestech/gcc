/* { dg-do compile } */
/* { dg-options "-Wno-psabi -O3 -fno-schedule-insns -fno-schedule-insns2" } */

typedef unsigned char uchar;
#define LOBYTE(x) ((uchar)((x) & 0xFF))
#define HIBYTE(x) ((uchar)((x) >> 8))

unsigned short bug_32985(unsigned short crc, uchar a) {
  static unsigned short icrctb[256];
  static uchar rchr[256];
  unsigned short tmp;

  tmp = rchr[a] ^ HIBYTE(crc);
  crc = icrctb[tmp] ^ LOBYTE(crc) << 8;
  return crc;
}

/* { dg-final { scan-assembler-times "nds.bfoz" 1 } } */
