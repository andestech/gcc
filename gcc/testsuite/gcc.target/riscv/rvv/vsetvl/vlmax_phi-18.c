/* { dg-do compile } */
/* { dg-options "-march=rv32gcv -mabi=ilp32" } */

#include "riscv_vector.h"

/* The for loop body should not have vsetvl instruction.  */
void f (void * restrict in, void * restrict out, int n, int cond)
{
  switch (cond)
  {
  case 1:{
    vuint8mf4_t v = *(vuint8mf4_t*)(in + 100);
    *(vuint8mf4_t*)(out + 100) = v;
    break;
  }
  case 2:{
    vuint8mf4_t v = *(vuint8mf4_t*)(in + 200);
    *(vuint8mf4_t*)(out + 100) = v;
    break;
  }
  case 3:{
    vuint8mf4_t v = *(vuint8mf4_t*)(in + 300);
    *(vuint8mf4_t*)(out + 100) = v;
    break;
  }
  default:{
    vuint8mf4_t v = *(vuint8mf4_t*)(in + 400);
    *(vuint8mf4_t*)(out + 400) = v;
    break;
  }
  }
  for (int i = 0; i < n; i++)
    {
      vuint8mf4_t v = *(vuint8mf4_t*)(in + i);
      *(vuint8mf4_t*)(out + i) = v;
    }
}

/* { dg-final { scan-assembler-times {vsetvli\s+[a-x0-9]+,\s*zero,\s*e8,\s*mf4,\s*t[au],\s*m[au]} 4 { target { no-opts "-O0"  no-opts "-Os" no-opts "-Oz" no-opts "-funroll-loops" no-opts "-g" no-opts "-flto"  } } } } */
/* { dg-final { scan-assembler-times {ble\t[a-x0-9]+,zero,.L[0-9]+\s*\.align\s+[12]\s+\.L[0-9]+\:\s*vle8\.v\s+v[0-9]+,0\([a-x0-9]+\)} 1 { target { no-opts "-O0"  no-opts "-Os" no-opts "-Oz" no-opts "-funroll-loops" no-opts "-g" no-opts "-flto"  } } } } */
