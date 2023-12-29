/* { dg-do compile } */
/* { dg-options "-march=rv64gczve32x -mabi=lp64d --param=riscv-autovec-preference=fixed-vlmax -Wno-psabi -mext-zvlsseg" } */
/* { dg-skip-if "" { *32-*-linux* } { "*" } } */

#include <stdint.h>
#include "riscv_vector.h"

__attribute__ ((noipa)) vint32m1x3_t
foo1 (int32_t *in, int vl)
{
  vint32m1x3_t v = __riscv_vlseg3e32_v_i32m1x3 (in, vl);
  return v;
}

__attribute__ ((noipa)) void
foo2 (vint32m1x3_t a, int32_t *out, int vl)
{
  __riscv_vsseg3e32_v_i32m1x3 (out, a, vl);
}

__attribute__ ((noipa)) vint32m1x3_t
foo3 (vint32m1x3_t a, int32_t *out, int32_t *in, int vl)
{
  __riscv_vsseg3e32_v_i32m1x3 (out, a, vl);
  vint32m1x3_t v = __riscv_vlseg3e32_v_i32m1x3 (in, vl);
  return v;
}
