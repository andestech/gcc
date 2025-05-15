/* { dg-do compile } */
/* { dg-options "-march=rv32gv_xandes_xandesvqmac -O3" { target { rv32 } } } */
/* { dg-options "-march=rv64gv_xandes_xandesvqmac -O3" { target { rv64 } } } */
/* { dg-prune-output "warning: '.*' will be deprecated in next 2" } */

#include <riscv_vector.h>

vuint32mf2_t test_vqmaccu_vv_u32mf2_tu(vuint32mf2_t vd, vuint8mf8_t vs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vx_u32mf2_tu(vuint32mf2_t vd, uint8_t rs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vv_u32m1_tu(vuint32m1_t vd, vuint8mf4_t vs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vx_u32m1_tu(vuint32m1_t vd, uint8_t rs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vv_u32m2_tu(vuint32m2_t vd, vuint8mf2_t vs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vx_u32m2_tu(vuint32m2_t vd, uint8_t rs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vv_u32m4_tu(vuint32m4_t vd, vuint8m1_t vs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vx_u32m4_tu(vuint32m4_t vd, uint8_t rs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vv_u32m8_tu(vuint32m8_t vd, vuint8m2_t vs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vx_u32m8_tu(vuint32m8_t vd, uint8_t rs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vv_u64m1_tu(vuint64m1_t vd, vuint16mf4_t vs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vx_u64m1_tu(vuint64m1_t vd, uint16_t rs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vv_u64m2_tu(vuint64m2_t vd, vuint16mf2_t vs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vx_u64m2_tu(vuint64m2_t vd, uint16_t rs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vv_u64m4_tu(vuint64m4_t vd, vuint16m1_t vs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vx_u64m4_tu(vuint64m4_t vd, uint16_t rs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vv_u64m8_tu(vuint64m8_t vd, vuint16m2_t vs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, vs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vx_u64m8_tu(vuint64m8_t vd, uint16_t rs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tu(vd, rs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vv_u32mf2_tum(vbool64_t mask, vuint32mf2_t vd, vuint8mf8_t vs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vx_u32mf2_tum(vbool64_t mask, vuint32mf2_t vd, uint8_t rs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vv_u32m1_tum(vbool32_t mask, vuint32m1_t vd, vuint8mf4_t vs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vx_u32m1_tum(vbool32_t mask, vuint32m1_t vd, uint8_t rs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vv_u32m2_tum(vbool16_t mask, vuint32m2_t vd, vuint8mf2_t vs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vx_u32m2_tum(vbool16_t mask, vuint32m2_t vd, uint8_t rs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vv_u32m4_tum(vbool8_t mask, vuint32m4_t vd, vuint8m1_t vs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vx_u32m4_tum(vbool8_t mask, vuint32m4_t vd, uint8_t rs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vv_u32m8_tum(vbool4_t mask, vuint32m8_t vd, vuint8m2_t vs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vx_u32m8_tum(vbool4_t mask, vuint32m8_t vd, uint8_t rs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vv_u64m1_tum(vbool64_t mask, vuint64m1_t vd, vuint16mf4_t vs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vx_u64m1_tum(vbool64_t mask, vuint64m1_t vd, uint16_t rs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vv_u64m2_tum(vbool32_t mask, vuint64m2_t vd, vuint16mf2_t vs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vx_u64m2_tum(vbool32_t mask, vuint64m2_t vd, uint16_t rs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vv_u64m4_tum(vbool16_t mask, vuint64m4_t vd, vuint16m1_t vs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vx_u64m4_tum(vbool16_t mask, vuint64m4_t vd, uint16_t rs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vv_u64m8_tum(vbool8_t mask, vuint64m8_t vd, vuint16m2_t vs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, vs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vx_u64m8_tum(vbool8_t mask, vuint64m8_t vd, uint16_t rs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tum(mask, vd, rs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vv_u32mf2_tumu(vbool64_t mask, vuint32mf2_t vd, vuint8mf8_t vs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vx_u32mf2_tumu(vbool64_t mask, vuint32mf2_t vd, uint8_t rs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vv_u32m1_tumu(vbool32_t mask, vuint32m1_t vd, vuint8mf4_t vs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vx_u32m1_tumu(vbool32_t mask, vuint32m1_t vd, uint8_t rs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vv_u32m2_tumu(vbool16_t mask, vuint32m2_t vd, vuint8mf2_t vs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vx_u32m2_tumu(vbool16_t mask, vuint32m2_t vd, uint8_t rs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vv_u32m4_tumu(vbool8_t mask, vuint32m4_t vd, vuint8m1_t vs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vx_u32m4_tumu(vbool8_t mask, vuint32m4_t vd, uint8_t rs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vv_u32m8_tumu(vbool4_t mask, vuint32m8_t vd, vuint8m2_t vs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vx_u32m8_tumu(vbool4_t mask, vuint32m8_t vd, uint8_t rs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vv_u64m1_tumu(vbool64_t mask, vuint64m1_t vd, vuint16mf4_t vs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vx_u64m1_tumu(vbool64_t mask, vuint64m1_t vd, uint16_t rs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vv_u64m2_tumu(vbool32_t mask, vuint64m2_t vd, vuint16mf2_t vs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vx_u64m2_tumu(vbool32_t mask, vuint64m2_t vd, uint16_t rs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vv_u64m4_tumu(vbool16_t mask, vuint64m4_t vd, vuint16m1_t vs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vx_u64m4_tumu(vbool16_t mask, vuint64m4_t vd, uint16_t rs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vv_u64m8_tumu(vbool8_t mask, vuint64m8_t vd, vuint16m2_t vs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, vs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vx_u64m8_tumu(vbool8_t mask, vuint64m8_t vd, uint16_t rs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_tumu(mask, vd, rs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vv_u32mf2_mu(vbool64_t mask, vuint32mf2_t vd, vuint8mf8_t vs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint32mf2_t test_vqmaccu_vx_u32mf2_mu(vbool64_t mask, vuint32mf2_t vd, uint8_t rs1, vuint8mf8_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vv_u32m1_mu(vbool32_t mask, vuint32m1_t vd, vuint8mf4_t vs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint32m1_t test_vqmaccu_vx_u32m1_mu(vbool32_t mask, vuint32m1_t vd, uint8_t rs1, vuint8mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vv_u32m2_mu(vbool16_t mask, vuint32m2_t vd, vuint8mf2_t vs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint32m2_t test_vqmaccu_vx_u32m2_mu(vbool16_t mask, vuint32m2_t vd, uint8_t rs1, vuint8mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vv_u32m4_mu(vbool8_t mask, vuint32m4_t vd, vuint8m1_t vs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint32m4_t test_vqmaccu_vx_u32m4_mu(vbool8_t mask, vuint32m4_t vd, uint8_t rs1, vuint8m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vv_u32m8_mu(vbool4_t mask, vuint32m8_t vd, vuint8m2_t vs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint32m8_t test_vqmaccu_vx_u32m8_mu(vbool4_t mask, vuint32m8_t vd, uint8_t rs1, vuint8m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vv_u64m1_mu(vbool64_t mask, vuint64m1_t vd, vuint16mf4_t vs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint64m1_t test_vqmaccu_vx_u64m1_mu(vbool64_t mask, vuint64m1_t vd, uint16_t rs1, vuint16mf4_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vv_u64m2_mu(vbool32_t mask, vuint64m2_t vd, vuint16mf2_t vs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint64m2_t test_vqmaccu_vx_u64m2_mu(vbool32_t mask, vuint64m2_t vd, uint16_t rs1, vuint16mf2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vv_u64m4_mu(vbool16_t mask, vuint64m4_t vd, vuint16m1_t vs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint64m4_t test_vqmaccu_vx_u64m4_mu(vbool16_t mask, vuint64m4_t vd, uint16_t rs1, vuint16m1_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vv_u64m8_mu(vbool8_t mask, vuint64m8_t vd, vuint16m2_t vs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, vs1, vs2, vl);
}

vuint64m8_t test_vqmaccu_vx_u64m8_mu(vbool8_t mask, vuint64m8_t vd, uint16_t rs1, vuint16m2_t vs2, size_t vl) {
  return __riscv_vqmaccu_mu(mask, vd, rs1, vs2, vl);
}
