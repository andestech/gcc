/* { dg-do compile } */
/* { dg-options "-mzfh -O3" } */
/* { dg-final { scan-assembler "fmadd.h" } } */

#include <stdint.h>

typedef __fp16 float16_t;

float16_t fmadd_f16(float16_t * src1, float16_t * src2, uint32_t size)
 {
     float16_t sum = 0.0;
     while (size != 0)
     {
         sum += (*src1++) * *src2;
         src2++;
         size--;
     }
     return sum;
 }
