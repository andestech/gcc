/* { dg-do run { target { riscv_vector && riscv_zvfh_hw } } } */
/* { dg-additional-options "-std=c99 -march=rv64gcv_zvfh -mabi=lp64d -fno-vect-cost-model --param=riscv-autovec-preference=scalable" } */

#include "vfwcvt-itof-template.h"

#define RUN(TYPE1, TYPE2, NUM)                                                 \
  TYPE1 src##TYPE1##TYPE2##NUM[NUM];                                           \
  TYPE2 dst##TYPE1##TYPE2##NUM[NUM];                                           \
  for (int i = 0; i < NUM; i++)                                                \
    {                                                                          \
      src##TYPE1##TYPE2##NUM[i] = i * -3 - 8;                                  \
    }                                                                          \
  vfwcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM); \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

#define RUN2(TYPE1, TYPE2, NUM)                                                \
  TYPE1 src##TYPE1##TYPE2##NUM[NUM];                                           \
  TYPE2 dst##TYPE1##TYPE2##NUM[NUM];                                           \
  for (int i = 0; i < NUM; i++)                                                \
    {                                                                          \
      src##TYPE1##TYPE2##NUM[i] = i * 3 + 8;                                   \
    }                                                                          \
  vfwcvt_##TYPE1##TYPE2 (dst##TYPE1##TYPE2##NUM, src##TYPE1##TYPE2##NUM, NUM); \
  for (int i = 0; i < NUM; i++)                                                \
    if (dst##TYPE1##TYPE2##NUM[i] != (TYPE2) src##TYPE1##TYPE2##NUM[i])        \
      __builtin_abort ();

int
main ()
{
  RUN (int8_t, _Float16, 3)
  RUN (int8_t, _Float16, 4)
  RUN (int8_t, _Float16, 7)
  RUN (int8_t, _Float16, 12)
  RUN (int8_t, _Float16, 20)
  RUN (int8_t, _Float16, 27)

  RUN (int8_t, _Float16, 4)
  RUN (int8_t, _Float16, 8)
  RUN (int8_t, _Float16, 11)
  RUN (int8_t, _Float16, 29)
  RUN (int8_t, _Float16, 49)
}
