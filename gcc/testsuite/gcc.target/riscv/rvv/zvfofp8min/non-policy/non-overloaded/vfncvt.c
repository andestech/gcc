/* { dg-do compile } */
/* { dg-options "-march=rv64gcv_zvfh_zvfofp8min -mabi=lp64d -O3" } */

#include "riscv_vector.h"

vuint8mf8_t test_vfncvt_f_f_q_f8e4m3mf8(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf8(vs2, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e4m3mf8(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf8(vs2, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e4m3mf4(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf4(vs2, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e4m3mf4(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf4(vs2, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e4m3mf2(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf2(vs2, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e4m3mf2(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf2(vs2, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e4m3m1(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m1(vs2, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e4m3m1(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m1(vs2, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e4m3m2(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m2(vs2, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e4m3m2(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m2(vs2, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e4m3mf8_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf8_m(vm, vs2, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e4m3mf8_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf8_m(vm, vs2, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e4m3mf4_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf4_m(vm, vs2, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e4m3mf4_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf4_m(vm, vs2, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e4m3mf2_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf2_m(vm, vs2, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e4m3mf2_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf2_m(vm, vs2, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e4m3m1_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m1_m(vm, vs2, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e4m3m1_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m1_m(vm, vs2, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e4m3m2_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m2_m(vm, vs2, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e4m3m2_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m2_m(vm, vs2, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e4m3mf8_rm(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf8_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e4m3mf8_rm(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf8_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e4m3mf4_rm(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf4_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e4m3mf4_rm(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf4_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e4m3mf2_rm(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e4m3mf2_rm(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e4m3m1_rm(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m1_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e4m3m1_rm(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m1_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e4m3m2_rm(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e4m3m2_rm(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e4m3mf8_rm_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf8_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e4m3mf8_rm_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf8_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e4m3mf4_rm_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf4_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e4m3mf4_rm_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf4_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e4m3mf2_rm_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3mf2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e4m3mf2_rm_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3mf2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e4m3m1_rm_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m1_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e4m3m1_rm_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m1_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e4m3m2_rm_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e4m3m2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e4m3m2_rm_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e4m3m2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e5m2mf8(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf8(vs2, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e5m2mf8(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf8(vs2, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e5m2mf4(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf4(vs2, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e5m2mf4(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf4(vs2, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e5m2mf2(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf2(vs2, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e5m2mf2(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf2(vs2, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e5m2m1(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m1(vs2, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e5m2m1(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m1(vs2, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e5m2m2(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m2(vs2, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e5m2m2(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m2(vs2, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e5m2mf8_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf8_m(vm, vs2, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e5m2mf8_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf8_m(vm, vs2, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e5m2mf4_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf4_m(vm, vs2, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e5m2mf4_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf4_m(vm, vs2, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e5m2mf2_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf2_m(vm, vs2, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e5m2mf2_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf2_m(vm, vs2, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e5m2m1_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m1_m(vm, vs2, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e5m2m1_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m1_m(vm, vs2, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e5m2m2_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m2_m(vm, vs2, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e5m2m2_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m2_m(vm, vs2, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e5m2mf8_rm(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf8_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e5m2mf8_rm(vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf8_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e5m2mf4_rm(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf4_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e5m2mf4_rm(vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf4_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e5m2mf2_rm(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e5m2mf2_rm(vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e5m2m1_rm(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m1_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e5m2m1_rm(vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m1_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e5m2m2_rm(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e5m2m2_rm(vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m2_rm(vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_f_f_q_f8e5m2mf8_rm_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf8_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf8_t test_vfncvt_sat_f_f_q_f8e5m2mf8_rm_m(vbool64_t vm, vfloat32mf2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf8_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_f_f_q_f8e5m2mf4_rm_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf4_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf4_t test_vfncvt_sat_f_f_q_f8e5m2mf4_rm_m(vbool32_t vm, vfloat32m1_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf4_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_f_f_q_f8e5m2mf2_rm_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2mf2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8mf2_t test_vfncvt_sat_f_f_q_f8e5m2mf2_rm_m(vbool16_t vm, vfloat32m2_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2mf2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_f_f_q_f8e5m2m1_rm_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m1_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m1_t test_vfncvt_sat_f_f_q_f8e5m2m1_rm_m(vbool8_t vm, vfloat32m4_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m1_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_f_f_q_f8e5m2m2_rm_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_f_f_q_f8e5m2m2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}

vuint8m2_t test_vfncvt_sat_f_f_q_f8e5m2m2_rm_m(vbool4_t vm, vfloat32m8_t vs2, size_t vl) {
  return __riscv_vfncvt_sat_f_f_q_f8e5m2m2_rm_m(vm, vs2, __RISCV_FRM_RNE, vl);
}
/* { dg-final { scan-assembler-times {vseti?vli\s+[a-z0-9]+,\s*[a-z0-9]+,\s*e[0-9]+(alt)?,\s*mf?[1248],\s*t[au],\s*m[au]\s+} 80 } } */
/* { dg-final { scan-assembler-times {vfncvt\.(sat\.)?[ivxfswumq.]+\s+} 80 } } */
