/* { dg-do compile } */
/* { dg-options "-march=rv32gv_xandes_xandesvqmac -O3" { target { rv32 } } } */
/* { dg-options "-march=rv64gv_xandes_xandesvqmac -O3" { target { rv64 } } } */

#include <riscv_vector.h>

vint32mf2_t
test_nds_vqmacc_vv_i32mf2 (vint32mf2_t vd, vint8mf8_t vs1, vint8mf8_t vs2,
			   size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vx_i32mf2 (vint32mf2_t vd, int8_t rs1, vint8mf8_t vs2,
			   size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vv_i32m1 (vint32m1_t vd, vint8mf4_t vs1, vint8mf4_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vx_i32m1 (vint32m1_t vd, int8_t rs1, vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vv_i32m2 (vint32m2_t vd, vint8mf2_t vs1, vint8mf2_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vx_i32m2 (vint32m2_t vd, int8_t rs1, vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vv_i32m4 (vint32m4_t vd, vint8m1_t vs1, vint8m1_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vx_i32m4 (vint32m4_t vd, int8_t rs1, vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vv_i32m8 (vint32m8_t vd, vint8m2_t vs1, vint8m2_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vx_i32m8 (vint32m8_t vd, int8_t rs1, vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vv_i64m1 (vint64m1_t vd, vint16mf4_t vs1, vint16mf4_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vx_i64m1 (vint64m1_t vd, int16_t rs1, vint16mf4_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vv_i64m2 (vint64m2_t vd, vint16mf2_t vs1, vint16mf2_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vx_i64m2 (vint64m2_t vd, int16_t rs1, vint16mf2_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vv_i64m4 (vint64m4_t vd, vint16m1_t vs1, vint16m1_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vx_i64m4 (vint64m4_t vd, int16_t rs1, vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vv_i64m8 (vint64m8_t vd, vint16m2_t vs1, vint16m2_t vs2,
			  size_t vl)
{
  return __riscv_nds_vqmacc (vd, vs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vx_i64m8 (vint64m8_t vd, int16_t rs1, vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (vd, rs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vv_i32mf2_m (vbool64_t mask, vint32mf2_t vd, vint8mf8_t vs1,
			     vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint32mf2_t
test_nds_vqmacc_vx_i32mf2_m (vbool64_t mask, vint32mf2_t vd, int8_t rs1,
			     vint8mf8_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vv_i32m1_m (vbool32_t mask, vint32m1_t vd, vint8mf4_t vs1,
			    vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint32m1_t
test_nds_vqmacc_vx_i32m1_m (vbool32_t mask, vint32m1_t vd, int8_t rs1,
			    vint8mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vv_i32m2_m (vbool16_t mask, vint32m2_t vd, vint8mf2_t vs1,
			    vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint32m2_t
test_nds_vqmacc_vx_i32m2_m (vbool16_t mask, vint32m2_t vd, int8_t rs1,
			    vint8mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vv_i32m4_m (vbool8_t mask, vint32m4_t vd, vint8m1_t vs1,
			    vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint32m4_t
test_nds_vqmacc_vx_i32m4_m (vbool8_t mask, vint32m4_t vd, int8_t rs1,
			    vint8m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vv_i32m8_m (vbool4_t mask, vint32m8_t vd, vint8m2_t vs1,
			    vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint32m8_t
test_nds_vqmacc_vx_i32m8_m (vbool4_t mask, vint32m8_t vd, int8_t rs1,
			    vint8m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vv_i64m1_m (vbool64_t mask, vint64m1_t vd, vint16mf4_t vs1,
			    vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint64m1_t
test_nds_vqmacc_vx_i64m1_m (vbool64_t mask, vint64m1_t vd, int16_t rs1,
			    vint16mf4_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vv_i64m2_m (vbool32_t mask, vint64m2_t vd, vint16mf2_t vs1,
			    vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint64m2_t
test_nds_vqmacc_vx_i64m2_m (vbool32_t mask, vint64m2_t vd, int16_t rs1,
			    vint16mf2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vv_i64m4_m (vbool16_t mask, vint64m4_t vd, vint16m1_t vs1,
			    vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint64m4_t
test_nds_vqmacc_vx_i64m4_m (vbool16_t mask, vint64m4_t vd, int16_t rs1,
			    vint16m1_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vv_i64m8_m (vbool8_t mask, vint64m8_t vd, vint16m2_t vs1,
			    vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, vs1, vs2, vl);
}

vint64m8_t
test_nds_vqmacc_vx_i64m8_m (vbool8_t mask, vint64m8_t vd, int16_t rs1,
			    vint16m2_t vs2, size_t vl)
{
  return __riscv_nds_vqmacc (mask, vd, rs1, vs2, vl);
}
