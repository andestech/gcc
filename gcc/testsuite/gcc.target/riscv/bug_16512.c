/* { dg-do run } */
/* { dg-options "-O2" } */

typedef unsigned uint32_t;
typedef int int32_t;

double logc = 1.0;

double
log2_inline (uint32_t ix) __attribute__((noinline));

int top;
double
log2_inline (uint32_t ix)
{

  uint32_t iz, tmp;
  int k, i, y0;

  tmp = ix - 0x3f330000;
  top = tmp & 0xff800000;
  k = ((int32_t) top) >> (23 - 0);
  y0 = logc + (double) k;
  return y0;
}

int main(){
  uint32_t xx = 0xffff0000u;
  union U{
    double d;
    unsigned long long ull;
  };
  union U x;
  x.d = log2_inline(xx);
  if (x.ull != 0xc05f800000000000ull){
    __builtin_printf("fail\n");
    __builtin_abort();
  }
  return 0;
}
