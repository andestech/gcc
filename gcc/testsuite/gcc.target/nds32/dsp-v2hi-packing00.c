/* { dg-do run } */

#include <nds32_intrinsic.h>

int16x2_t packing01(int16x2_t x, int16x2_t y) __attribute__ ((noinline));
int16x2_t packing01(int16x2_t x, int16x2_t y)
{
  int16x2_t ret;
  ret[0] = x[0];
  ret[1] = y[1];
  return ret;
}

int16x2_t packing10(int16x2_t x, int16x2_t y) __attribute__ ((noinline));
int16x2_t packing10(int16x2_t x, int16x2_t y)
{
  int16x2_t ret;
  ret[0] = x[1];
  ret[1] = y[0];
  return ret;
}

int16x2_t packing00(int16x2_t x, int16x2_t y) __attribute__ ((noinline));
int16x2_t packing00(int16x2_t x, int16x2_t y)
{
  int16x2_t ret;
  ret[0] = x[0];
  ret[1] = y[0];
  return ret;
}

int16x2_t packing0cv0(int16x2_t x) __attribute__ ((noinline));
int16x2_t packing0cv0(int16x2_t x)
{
  int16x2_t ret = {0, 0};
  ret[0] = x[0];
  return ret;
}

int16x2_t packingcv00(int16x2_t x) __attribute__ ((noinline));
int16x2_t packingcv00(int16x2_t x)
{
  int16x2_t ret = {0, 0};
  ret[1] = x[0];
  return ret;
}

int16x2_t packing11(int16x2_t x, int16x2_t y) __attribute__ ((noinline));
int16x2_t packing11(int16x2_t x, int16x2_t y)
{
  int16x2_t ret;
  ret[0] = x[1];
  ret[1] = y[1];
  return ret;
}
int16x2_t packing1cv0(int16x2_t x) __attribute__ ((noinline));
int16x2_t packing1cv0(int16x2_t x)
{
  int16x2_t ret = {0, 0};
  ret[0] = x[1];
  return ret;
}

int16x2_t packingcv01(int16x2_t x) __attribute__ ((noinline));
int16x2_t packingcv01(int16x2_t x)
{
  int16x2_t ret = {0, 0};
  ret[1] = x[1];
  return ret;
}

int main() {
  int16x2_t a = {0x11, 0x22};
  int16x2_t b = {0x33, 0x44};

  int16x2_t ret00, ret01, ret10, ret11;
  int16x2_t ret0cv0, retcv00, ret1cv0, retcv01;
  ret00 = packing00 (a, b);

  if (ret00[0] != 0x11
      || ret00[1] != 0x33)
    return 1;

  ret0cv0 = packing0cv0 (a);

  if (ret0cv0[0] != 0x11
      || ret0cv0[1] != 0)
    return 1;

  retcv00 = packingcv00 (a);

  if (retcv00[0] != 0
      || retcv00[1] != 0x11)
    return 1;

  ret11 = packing11 (a, b);

  if (ret11[0] != 0x22
      || ret11[1] != 0x44)
    return 1;

  ret1cv0 = packing1cv0 (a);

  if (ret1cv0[0] != 0x22
      || ret1cv0[1] != 0)
    return 1;

  retcv01 = packingcv01 (a);

  if (retcv01[0] != 0
      || retcv01[1] != 0x22)
    return 1;

  ret01 = packing01 (a, b);

  if (ret01[0] != 0x11
      || ret01[1] != 0x44)
    return 1;

  ret10 = packing10 (a, b);

  if (ret10[0] != 0x22
      || ret10[1] != 0x33)
    return 1;

  return 0;
}
