/* { dg-do compile } */
/* { dg-require-effective-target rv32 } */
/* { dg-options "-O1 -march=rv32i_zilsd -mabi=ilp32" } */

#include <stdint.h>

void test (int64_t *src, int64_t *dest)
{
  *(dest + 1) = *(src + 3);
}

/* { dg-final { scan-assembler {ld\s+a[0,2,4,6],\s*[0-9]+\(a0\)} } } */
/* { dg-final { scan-assembler {sd\s+a[0,2,4,6],\s*[0-9]+\(a1\)} } } */
