/* { dg-do compile } */
/* { dg-options "-march=rv32gv_xandes_xandesvqmac -O3" { target { rv32 } } } */
/* { dg-options "-march=rv64gv_xandes_xandesvqmac -O3" { target { rv64 } } } */

#include <riscv_vector.h>

vint32mf2_t
test_nds_vqmacc_vv_i32mf2_tu (vint32mf2_t vd, vint8mf8_t vs1, vint8mf8_t vs2,
			      size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32mf2_tu (vd, vs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vx_i32mf2_tu (vint32mf2_t vd, int8_t rs1, vint8mf8_t vs2,
			      size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32mf2_tu (vd, rs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vv_i32m1_tu (vint32m1_t vd, vint8mf4_t vs1, vint8mf4_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m1_tu (vd, vs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vx_i32m1_tu (vint32m1_t vd, int8_t rs1, vint8mf4_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m1_tu (vd, rs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vv_i32m2_tu (vint32m2_t vd, vint8mf2_t vs1, vint8mf2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m2_tu (vd, vs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vx_i32m2_tu (vint32m2_t vd, int8_t rs1, vint8mf2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m2_tu (vd, rs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vv_i32m4_tu (vint32m4_t vd, vint8m1_t vs1, vint8m1_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m4_tu (vd, vs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vx_i32m4_tu (vint32m4_t vd, int8_t rs1, vint8m1_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m4_tu (vd, rs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vv_i32m8_tu (vint32m8_t vd, vint8m2_t vs1, vint8m2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m8_tu (vd, vs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vx_i32m8_tu (vint32m8_t vd, int8_t rs1, vint8m2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m8_tu (vd, rs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vv_i64m1_tu (vint64m1_t vd, vint16mf4_t vs1, vint16mf4_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m1_tu (vd, vs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vx_i64m1_tu (vint64m1_t vd, int16_t rs1, vint16mf4_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m1_tu (vd, rs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vv_i64m2_tu (vint64m2_t vd, vint16mf2_t vs1, vint16mf2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m2_tu (vd, vs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vx_i64m2_tu (vint64m2_t vd, int16_t rs1, vint16mf2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m2_tu (vd, rs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vv_i64m4_tu (vint64m4_t vd, vint16m1_t vs1, vint16m1_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m4_tu (vd, vs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vx_i64m4_tu (vint64m4_t vd, int16_t rs1, vint16m1_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m4_tu (vd, rs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vv_i64m8_tu (vint64m8_t vd, vint16m2_t vs1, vint16m2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m8_tu (vd, vs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vx_i64m8_tu (vint64m8_t vd, int16_t rs1, vint16m2_t vs2,
			     size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m8_tu (vd, rs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vv_i32mf2_tum (vbool64_t mask, vint32mf2_t vd, vint8mf8_t vs1,
			       vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32mf2_tum (mask, vd, vs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vx_i32mf2_tum (vbool64_t mask, vint32mf2_t vd, int8_t rs1,
			       vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32mf2_tum (mask, vd, rs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vv_i32m1_tum (vbool32_t mask, vint32m1_t vd, vint8mf4_t vs1,
			      vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m1_tum (mask, vd, vs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vx_i32m1_tum (vbool32_t mask, vint32m1_t vd, int8_t rs1,
			      vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m1_tum (mask, vd, rs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vv_i32m2_tum (vbool16_t mask, vint32m2_t vd, vint8mf2_t vs1,
			      vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m2_tum (mask, vd, vs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vx_i32m2_tum (vbool16_t mask, vint32m2_t vd, int8_t rs1,
			      vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m2_tum (mask, vd, rs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vv_i32m4_tum (vbool8_t mask, vint32m4_t vd, vint8m1_t vs1,
			      vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m4_tum (mask, vd, vs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vx_i32m4_tum (vbool8_t mask, vint32m4_t vd, int8_t rs1,
			      vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m4_tum (mask, vd, rs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vv_i32m8_tum (vbool4_t mask, vint32m8_t vd, vint8m2_t vs1,
			      vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m8_tum (mask, vd, vs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vx_i32m8_tum (vbool4_t mask, vint32m8_t vd, int8_t rs1,
			      vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m8_tum (mask, vd, rs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vv_i64m1_tum (vbool64_t mask, vint64m1_t vd, vint16mf4_t vs1,
			      vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m1_tum (mask, vd, vs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vx_i64m1_tum (vbool64_t mask, vint64m1_t vd, int16_t rs1,
			      vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m1_tum (mask, vd, rs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vv_i64m2_tum (vbool32_t mask, vint64m2_t vd, vint16mf2_t vs1,
			      vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m2_tum (mask, vd, vs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vx_i64m2_tum (vbool32_t mask, vint64m2_t vd, int16_t rs1,
			      vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m2_tum (mask, vd, rs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vv_i64m4_tum (vbool16_t mask, vint64m4_t vd, vint16m1_t vs1,
			      vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m4_tum (mask, vd, vs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vx_i64m4_tum (vbool16_t mask, vint64m4_t vd, int16_t rs1,
			      vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m4_tum (mask, vd, rs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vv_i64m8_tum (vbool8_t mask, vint64m8_t vd, vint16m2_t vs1,
			      vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m8_tum (mask, vd, vs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vx_i64m8_tum (vbool8_t mask, vint64m8_t vd, int16_t rs1,
			      vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m8_tum (mask, vd, rs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vv_i32mf2_tumu (vbool64_t mask, vint32mf2_t vd, vint8mf8_t vs1,
				vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32mf2_tumu (mask, vd, vs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vx_i32mf2_tumu (vbool64_t mask, vint32mf2_t vd, int8_t rs1,
				vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32mf2_tumu (mask, vd, rs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vv_i32m1_tumu (vbool32_t mask, vint32m1_t vd, vint8mf4_t vs1,
			       vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m1_tumu (mask, vd, vs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vx_i32m1_tumu (vbool32_t mask, vint32m1_t vd, int8_t rs1,
			       vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m1_tumu (mask, vd, rs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vv_i32m2_tumu (vbool16_t mask, vint32m2_t vd, vint8mf2_t vs1,
			       vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m2_tumu (mask, vd, vs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vx_i32m2_tumu (vbool16_t mask, vint32m2_t vd, int8_t rs1,
			       vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m2_tumu (mask, vd, rs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vv_i32m4_tumu (vbool8_t mask, vint32m4_t vd, vint8m1_t vs1,
			       vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m4_tumu (mask, vd, vs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vx_i32m4_tumu (vbool8_t mask, vint32m4_t vd, int8_t rs1,
			       vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m4_tumu (mask, vd, rs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vv_i32m8_tumu (vbool4_t mask, vint32m8_t vd, vint8m2_t vs1,
			       vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m8_tumu (mask, vd, vs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vx_i32m8_tumu (vbool4_t mask, vint32m8_t vd, int8_t rs1,
			       vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m8_tumu (mask, vd, rs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vv_i64m1_tumu (vbool64_t mask, vint64m1_t vd, vint16mf4_t vs1,
			       vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m1_tumu (mask, vd, vs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vx_i64m1_tumu (vbool64_t mask, vint64m1_t vd, int16_t rs1,
			       vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m1_tumu (mask, vd, rs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vv_i64m2_tumu (vbool32_t mask, vint64m2_t vd, vint16mf2_t vs1,
			       vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m2_tumu (mask, vd, vs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vx_i64m2_tumu (vbool32_t mask, vint64m2_t vd, int16_t rs1,
			       vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m2_tumu (mask, vd, rs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vv_i64m4_tumu (vbool16_t mask, vint64m4_t vd, vint16m1_t vs1,
			       vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m4_tumu (mask, vd, vs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vx_i64m4_tumu (vbool16_t mask, vint64m4_t vd, int16_t rs1,
			       vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m4_tumu (mask, vd, rs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vv_i64m8_tumu (vbool8_t mask, vint64m8_t vd, vint16m2_t vs1,
			       vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m8_tumu (mask, vd, vs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vx_i64m8_tumu (vbool8_t mask, vint64m8_t vd, int16_t rs1,
			       vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m8_tumu (mask, vd, rs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vv_i32mf2_mu (vbool64_t mask, vint32mf2_t vd, vint8mf8_t vs1,
			      vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32mf2_mu (mask, vd, vs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vx_i32mf2_mu (vbool64_t mask, vint32mf2_t vd, int8_t rs1,
			      vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32mf2_mu (mask, vd, rs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vv_i32m1_mu (vbool32_t mask, vint32m1_t vd, vint8mf4_t vs1,
			     vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m1_mu (mask, vd, vs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vx_i32m1_mu (vbool32_t mask, vint32m1_t vd, int8_t rs1,
			     vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m1_mu (mask, vd, rs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vv_i32m2_mu (vbool16_t mask, vint32m2_t vd, vint8mf2_t vs1,
			     vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m2_mu (mask, vd, vs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vx_i32m2_mu (vbool16_t mask, vint32m2_t vd, int8_t rs1,
			     vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m2_mu (mask, vd, rs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vv_i32m4_mu (vbool8_t mask, vint32m4_t vd, vint8m1_t vs1,
			     vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m4_mu (mask, vd, vs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vx_i32m4_mu (vbool8_t mask, vint32m4_t vd, int8_t rs1,
			     vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m4_mu (mask, vd, rs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vv_i32m8_mu (vbool4_t mask, vint32m8_t vd, vint8m2_t vs1,
			     vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i32m8_mu (mask, vd, vs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vx_i32m8_mu (vbool4_t mask, vint32m8_t vd, int8_t rs1,
			     vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i32m8_mu (mask, vd, rs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vv_i64m1_mu (vbool64_t mask, vint64m1_t vd, vint16mf4_t vs1,
			     vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m1_mu (mask, vd, vs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vx_i64m1_mu (vbool64_t mask, vint64m1_t vd, int16_t rs1,
			     vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m1_mu (mask, vd, rs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vv_i64m2_mu (vbool32_t mask, vint64m2_t vd, vint16mf2_t vs1,
			     vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m2_mu (mask, vd, vs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vx_i64m2_mu (vbool32_t mask, vint64m2_t vd, int16_t rs1,
			     vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m2_mu (mask, vd, rs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vv_i64m4_mu (vbool16_t mask, vint64m4_t vd, vint16m1_t vs1,
			     vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m4_mu (mask, vd, vs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vx_i64m4_mu (vbool16_t mask, vint64m4_t vd, int16_t rs1,
			     vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m4_mu (mask, vd, rs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vv_i64m8_mu (vbool8_t mask, vint64m8_t vd, vint16m2_t vs1,
			     vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vv_i64m8_mu (mask, vd, vs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vx_i64m8_mu (vbool8_t mask, vint64m8_t vd, int16_t rs1,
			     vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc_vx_i64m8_mu (mask, vd, rs1, vs2, vl);
}
