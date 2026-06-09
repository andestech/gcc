/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-phiopt" } */
#include <stdint.h>
#define RGBMAX 255

uint8_t
three_min (uint8_t xr, uint8_t xg, uint8_t xb)
{
  uint8_t xk;
  uint8_t xc = (uint8_t) (RGBMAX - xr);
  uint8_t xm = (uint8_t) (RGBMAX - xg);
  uint8_t xy = (uint8_t) (RGBMAX - xb);
  if (xc < xm)
    {
      xk = (uint8_t) (xc < xy ? xc : xy);
    }
  else
    {
      xk = (uint8_t) (xm < xy ? xm : xy);
    }
  return xk;
}

/* { dg-final { scan-tree-dump-times "MAX_EXPR" 2 "phiopt1" } } */
