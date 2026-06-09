/* Subroutines used for expanding RISC-V builtins.
   Copyright (C) 2011-2024 Free Software Foundation, Inc.
   Contributed by Andrew Waterman (andrew@sifive.com).

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "gimple-expr.h"
#include "memmodel.h"
#include "expmed.h"
#include "profile-count.h"
#include "optabs.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "stor-layout.h"
#include "stringpool.h"
#include "expr.h"
#include "langhooks.h"
#include "tm_p.h"
#include "backend.h"
#include "gimple.h"
#include "gimple-iterator.h"
#include "function.h"
#include "emit-rtl.h"
#include "expr.h"
#include "explow.h"

/* Macros to create an enumeration identifier for a function prototype.  */
#define RISCV_FTYPE_NAME0(A) RISCV_##A##_FTYPE
#define RISCV_FTYPE_NAME1(A, B) RISCV_##A##_FTYPE_##B
#define RISCV_FTYPE_NAME2(A, B, C) RISCV_##A##_FTYPE_##B##_##C
#define RISCV_FTYPE_NAME3(A, B, C, D) RISCV_##A##_FTYPE_##B##_##C##_##D
#define RISCV_FTYPE_NAME4(A, B, C, D, E) \
  RISCV_##A##_FTYPE_##B##_##C##_##D##_##E
#define RISCV_FTYPE_NAME5(A, B, C, D, E, F) \
  RISCV_##A##_FTYPE_##B##_##C##_##D##_##E##_##F
#define RISCV_FTYPE_NAME6(A, B, C, D, E, F, G) \
  RISCV_##A##_FTYPE_##B##_##C##_##D##_##E##_##F##_##G
#define RISCV_FTYPE_NAME7(A, B, C, D, E, F, G, H) \
  RISCV_##A##_FTYPE_##B##_##C##_##D##_##E##_##F##_##G##_##H

/* Classifies the prototype of a built-in function.  */
enum riscv_function_type {
#define DEF_RISCV_FTYPE(NARGS, LIST) RISCV_FTYPE_NAME##NARGS LIST,
#include "config/riscv/riscv-ftypes.def"
#undef DEF_RISCV_FTYPE
  RISCV_MAX_FTYPE_MAX
};

/* Specifies how a built-in function should be converted into rtl.  */
enum riscv_builtin_type {
  /* The function corresponds directly to an .md pattern.  */
  RISCV_BUILTIN_DIRECT,

  /* Likewise, but with return type VOID.  */
  RISCV_BUILTIN_DIRECT_NO_TARGET,

  /* Ecall functions store return value in A0 register, and save the first argument
     in A7/T0 register, and pass other arguments in A0 to A6 register.  */
  RISCV_BUILTIN_ECALL,

  /* EBREAK save the first argument in A7/T0 register.  */
  RISCV_BUILTIN_EBREAK,
  /* Chech arguments.  */
  RISCV_BUILTIN_PREFETCH,

  /* The last argument for mop intrinsics should be a constant.  */
  RISCV_BUILTIN_MOP,
};

/* Declare an availability predicate for built-in functions.  */
#define AVAIL(NAME, COND)		\
 static unsigned int			\
 riscv_builtin_avail_##NAME (void)	\
 {					\
   return (COND);			\
 }

/* This structure describes a single built-in function.  */
struct riscv_builtin_description {
  /* The code of the main .md file instruction.  See riscv_builtin_type
     for more information.  */
  enum insn_code icode;

  /* The name of the built-in function.  */
  const char *name;

  /* Specifies how the function should be expanded.  */
  enum riscv_builtin_type builtin_type;

  /* The function's prototype.  */
  enum riscv_function_type prototype;

  /* Whether the function is available.  */
  unsigned int (*avail) (void);
};

AVAIL (hard_float, TARGET_HARD_FLOAT || TARGET_ZFINX)
AVAIL (clean32, TARGET_ZICBOM && !TARGET_64BIT)
AVAIL (clean64, TARGET_ZICBOM && TARGET_64BIT)
AVAIL (flush32, TARGET_ZICBOM && !TARGET_64BIT)
AVAIL (flush64, TARGET_ZICBOM && TARGET_64BIT)
AVAIL (inval32, TARGET_ZICBOM && !TARGET_64BIT)
AVAIL (inval64, TARGET_ZICBOM && TARGET_64BIT)
AVAIL (zero32,  TARGET_ZICBOZ && !TARGET_64BIT)
AVAIL (zero64,  TARGET_ZICBOZ && TARGET_64BIT)
AVAIL (prefetchi32, TARGET_ZICBOP && !TARGET_64BIT)
AVAIL (prefetchi64, TARGET_ZICBOP && TARGET_64BIT)
AVAIL (crypto_zbkb, TARGET_ZBKB)
AVAIL (crypto_zbkb32, TARGET_ZBKB && !TARGET_64BIT)
AVAIL (crypto_zbkb64, TARGET_ZBKB && TARGET_64BIT)
AVAIL (crypto_zbkx32, TARGET_ZBKX && !TARGET_64BIT)
AVAIL (crypto_zbkx64, TARGET_ZBKX && TARGET_64BIT)
AVAIL (crypto_zknd32, TARGET_ZKND && !TARGET_64BIT)
AVAIL (crypto_zknd64, TARGET_ZKND && TARGET_64BIT)
AVAIL (crypto_zkne32, TARGET_ZKNE && !TARGET_64BIT)
AVAIL (crypto_zkne64, TARGET_ZKNE && TARGET_64BIT)
AVAIL (crypto_zkne_or_zknd, (TARGET_ZKNE || TARGET_ZKND) && TARGET_64BIT)
AVAIL (crypto_zknh, TARGET_ZKNH)
AVAIL (crypto_zknh32, TARGET_ZKNH && !TARGET_64BIT)
AVAIL (crypto_zknh64, TARGET_ZKNH && TARGET_64BIT)
AVAIL (crypto_zksh, TARGET_ZKSH)
AVAIL (crypto_zksed, TARGET_ZKSED)
AVAIL (clmul_zbkc_or_zbc, (TARGET_ZBKC || TARGET_ZBC))
AVAIL (clmul_zbkc32_or_zbc32, (TARGET_ZBKC || TARGET_ZBC) && !TARGET_64BIT)
AVAIL (clmul_zbkc64_or_zbc64, (TARGET_ZBKC || TARGET_ZBC) && TARGET_64BIT)
AVAIL (clmulr_zbc32, TARGET_ZBC && !TARGET_64BIT)
AVAIL (clmulr_zbc64, TARGET_ZBC && TARGET_64BIT)
AVAIL (mop, TARGET_ZIMOP)
AVAIL (mop64, TARGET_ZIMOP && TARGET_64BIT)
AVAIL (zbb, TARGET_ZBB)
AVAIL (zbb64, TARGET_ZBB && TARGET_64BIT)
AVAIL (zbb64_or_zbkb64, (TARGET_ZBKB || TARGET_ZBB) && TARGET_64BIT)
AVAIL (zbb_or_zbkb, (TARGET_ZBKB || TARGET_ZBB))
AVAIL (hint_pause, (!0))
AVAIL (normal, 1)
AVAIL (rv32, !TARGET_64BIT)
AVAIL (rv64, TARGET_64BIT)
AVAIL (hard_float32, (TARGET_HARD_FLOAT || TARGET_ZFINX) && !TARGET_64BIT)
AVAIL (hard_float64, (TARGET_HARD_FLOAT || TARGET_ZFINX) && TARGET_64BIT)
AVAIL (atomic, TARGET_ATOMIC)
AVAIL (atomic32, TARGET_ATOMIC && !TARGET_64BIT)
AVAIL (atomic64, TARGET_ATOMIC && TARGET_64BIT)
AVAIL (andes32, !TARGET_64BIT && TARGET_V5)
AVAIL (andes64, TARGET_64BIT && TARGET_V5)
AVAIL (bf16, TARGET_BF16)
AVAIL (dsp32, !TARGET_64BIT && TARGET_DSP)
AVAIL (dsp64, TARGET_64BIT && TARGET_DSP)
AVAIL (dsp, TARGET_DSP)

// CORE-V AVAIL
AVAIL (cvmac, TARGET_XCVMAC && !TARGET_64BIT)
AVAIL (cvalu, TARGET_XCVALU && !TARGET_64BIT)
AVAIL (cvelw, TARGET_XCVELW && !TARGET_64BIT)
AVAIL (cvsimd, TARGET_XCVSIMD && !TARGET_64BIT)

/* Construct a riscv_builtin_description from the given arguments.

   INSN is the name of the associated instruction pattern, without the
   leading CODE_FOR_riscv_.

   NAME is the name of the function itself, without the leading
   "__builtin_riscv_".

   BUILTIN_TYPE and FUNCTION_TYPE are riscv_builtin_description fields.

   AVAIL is the name of the availability predicate, without the leading
   riscv_builtin_avail_.  */
#define RISCV_BUILTIN(INSN, NAME, BUILTIN_TYPE,	FUNCTION_TYPE, AVAIL)	\
  { CODE_FOR_riscv_ ## INSN, "__builtin_riscv_" NAME,			\
    BUILTIN_TYPE, FUNCTION_TYPE, riscv_builtin_avail_ ## AVAIL }

/* Construct a riscv_builtin_description from the given arguments like RISCV_BUILTIN.

   INSN is the name of the associated instruction pattern, without the
   leading CODE_FOR_.

   NAME is the name of the function itself, without the leading
   "__builtin_riscv_".

   BUILTIN_TYPE and FUNCTION_TYPE are riscv_builtin_description fields.

   AVAIL is the name of the availability predicate, without the leading
   riscv_builtin_avail_.  */
#define RISCV_BUILTIN_NO_PREFIX(INSN, NAME, BUILTIN_TYPE,	FUNCTION_TYPE, AVAIL)	\
  { CODE_FOR_ ## INSN, "__builtin_riscv_" NAME,			\
    BUILTIN_TYPE, FUNCTION_TYPE, riscv_builtin_avail_ ## AVAIL }

/* Define __builtin_riscv_<INSN>, which is a RISCV_BUILTIN_DIRECT function
   mapped to instruction CODE_FOR_riscv_<INSN>,  FUNCTION_TYPE and AVAIL
   are as for RISCV_BUILTIN.  */
#define DIRECT_BUILTIN(INSN, FUNCTION_TYPE, AVAIL)			\
  RISCV_BUILTIN (INSN, #INSN, RISCV_BUILTIN_DIRECT, FUNCTION_TYPE, AVAIL)

/* Define __builtin_riscv_<INSN>, which is a RISCV_BUILTIN_DIRECT_NO_TARGET
   function mapped to instruction CODE_FOR_riscv_<INSN>,  FUNCTION_TYPE
   and AVAIL are as for RISCV_BUILTIN.  */
#define DIRECT_NO_TARGET_BUILTIN(INSN, FUNCTION_TYPE, AVAIL)		\
  RISCV_BUILTIN (INSN, #INSN, RISCV_BUILTIN_DIRECT_NO_TARGET,		\
		FUNCTION_TYPE, AVAIL)

#define RISCV_DSP_BUILTIN(INSN, NAME, BUILTIN_TYPE, FUNCTION_TYPE, AVAIL) \
  { CODE_FOR_ ## INSN, "__builtin_riscv_" NAME,			\
    BUILTIN_TYPE, FUNCTION_TYPE, riscv_builtin_avail_ ## AVAIL }

#define DIRECT_DSP_BUILTIN(INSN, FUNCTION_TYPE, AVAIL)			\
  RISCV_DSP_BUILTIN (INSN, #INSN, RISCV_BUILTIN_DIRECT, FUNCTION_TYPE, AVAIL)

/* For the Andes extension, the prefix __riscv_nds_* is used in the naming
 * convention. */
#define RISCV_NDS_BUILTIN(INSN, NAME, BUILTIN_TYPE, FUNCTION_TYPE, AVAIL)      \
  {                                                                            \
    CODE_FOR_riscv_##INSN, "__riscv_nds_" NAME, BUILTIN_TYPE, FUNCTION_TYPE,   \
      riscv_builtin_avail_##AVAIL                                              \
  }

#define DIRECT_NDS_BUILTIN(INSN, FUNCTION_TYPE, AVAIL)                         \
  RISCV_NDS_BUILTIN (INSN, #INSN, RISCV_BUILTIN_DIRECT, FUNCTION_TYPE, AVAIL)

/* Argument types.  */
#define RISCV_ATYPE_VOID void_type_node
#define RISCV_ATYPE_UQI unsigned_intQI_type_node
#define RISCV_ATYPE_UHI unsigned_intHI_type_node
#define RISCV_ATYPE_CONST_VOID_PTR const_ptr_type_node
#define RISCV_ATYPE_USI unsigned_intSI_type_node
#define RISCV_ATYPE_UDI unsigned_intDI_type_node
#define RISCV_ATYPE_QI intQI_type_node
#define RISCV_ATYPE_HI intHI_type_node
#define RISCV_ATYPE_SI intSI_type_node
#define RISCV_ATYPE_DI intDI_type_node
#define RISCV_ATYPE_BF bfloat16_type_node
#define RISCV_ATYPE_SF float_type_node
#define RISCV_ATYPE_VOID_PTR ptr_type_node
#define RISCV_ATYPE_INT_PTR integer_ptr_type_node
#define RISCV_ATYPE_ULONG long_unsigned_type_node
#define RISCV_ATYPE_LONG long_integer_type_node
#define RISCV_ATYPE_LLONG long_long_integer_type_node
#define RISCV_ATYPE_ULLONG  long_long_unsigned_type_node
#define RISCV_ATYPE_PSI build_pointer_type (integer_type_node)
#define RISCV_ATYPE_PUSI build_pointer_type (unsigned_type_node)
#define RISCV_ATYPE_PLLONG build_pointer_type (long_long_integer_type_node)
#define RISCV_ATYPE_PULLONG build_pointer_type (long_long_unsigned_type_node)
#define RISCV_ATYPE_DSPV4QI build_vector_type (intQI_type_node, 4)
#define RISCV_ATYPE_DSPUV4QI build_vector_type (unsigned_intQI_type_node, 4)
#define RISCV_ATYPE_DSPV8QI build_vector_type (intQI_type_node, 8)
#define RISCV_ATYPE_DSPUV8QI build_vector_type (unsigned_intQI_type_node, 8)
#define RISCV_ATYPE_DSPV2HI build_vector_type (intHI_type_node, 2)
#define RISCV_ATYPE_DSPUV2HI build_vector_type (unsigned_intHI_type_node, 2)
#define RISCV_ATYPE_DSPV4HI build_vector_type (intHI_type_node, 4)
#define RISCV_ATYPE_DSPUV4HI build_vector_type (unsigned_intHI_type_node, 4)
#define RISCV_ATYPE_DSPV2SI build_vector_type (intSI_type_node, 2)
#define RISCV_ATYPE_DSPUV2SI build_vector_type (unsigned_intSI_type_node, 2)
#define RISCV_ATYPE_DSPV4SI build_vector_type (intSI_type_node, 4)
#define RISCV_ATYPE_DSPUV4SI build_vector_type (unsigned_intSI_type_node, 4)
#define RISCV_ATYPE_DSPV8HI build_vector_type (intHI_type_node, 8)
#define RISCV_ATYPE_DSPUV8HI build_vector_type (unsigned_intHI_type_node, 8)

/* RISCV_FTYPE_ATYPESN takes N RISCV_FTYPES-like type codes and lists
   their associated RISCV_ATYPEs.  */
#define RISCV_FTYPE_ATYPES0(A) \
  RISCV_ATYPE_##A
#define RISCV_FTYPE_ATYPES1(A, B) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B
#define RISCV_FTYPE_ATYPES2(A, B, C) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B, RISCV_ATYPE_##C
#define RISCV_FTYPE_ATYPES3(A, B, C, D) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B, RISCV_ATYPE_##C, RISCV_ATYPE_##D
#define RISCV_FTYPE_ATYPES4(A, B, C, D, E) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B, RISCV_ATYPE_##C, RISCV_ATYPE_##D, \
  RISCV_ATYPE_##E
#define RISCV_FTYPE_ATYPES5(A, B, C, D, E, F) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B, RISCV_ATYPE_##C, RISCV_ATYPE_##D, \
  RISCV_ATYPE_##E, RISCV_ATYPE_##F
#define RISCV_FTYPE_ATYPES6(A, B, C, D, E, F, G) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B, RISCV_ATYPE_##C, RISCV_ATYPE_##D, \
  RISCV_ATYPE_##E, RISCV_ATYPE_##F, RISCV_ATYPE_##G
#define RISCV_FTYPE_ATYPES7(A, B, C, D, E, F, G, H) \
  RISCV_ATYPE_##A, RISCV_ATYPE_##B, RISCV_ATYPE_##C, RISCV_ATYPE_##D, \
  RISCV_ATYPE_##E, RISCV_ATYPE_##F, RISCV_ATYPE_##G, RISCV_ATYPE_##H

static const struct riscv_builtin_description riscv_builtins[] = {
  #include "riscv-cmo.def"
  #include "riscv-mop.def"
  #include "riscv-scalar-crypto.def"
  #include "corev.def"
  #include "riscv-nds.def"

  DIRECT_BUILTIN (frflags_fenv, RISCV_USI_FTYPE, hard_float),
  DIRECT_NO_TARGET_BUILTIN (fsflags_fenv, RISCV_VOID_FTYPE_USI, hard_float),
  RISCV_BUILTIN (pause, "pause", RISCV_BUILTIN_DIRECT_NO_TARGET, RISCV_VOID_FTYPE, hint_pause),
};

/* Index I is the function declaration for riscv_builtins[I], or null if the
   function isn't defined on this target.  */
static GTY(()) tree riscv_builtin_decls[ARRAY_SIZE (riscv_builtins)];

/* Get the index I of the function declaration for riscv_builtin_decls[I]
   using the instruction code or return null if not defined for the target.  */
static GTY(()) int riscv_builtin_decl_index[NUM_INSN_CODES];

#define GET_BUILTIN_DECL(CODE) \
  riscv_builtin_decls[riscv_builtin_decl_index[(CODE)]]

tree riscv_float16_type_node = NULL_TREE;

/* Return the function type associated with function prototype TYPE.  */

static tree
riscv_build_function_type (enum riscv_function_type type)
{
  static tree types[(int) RISCV_MAX_FTYPE_MAX];

  if (types[(int) type] == NULL_TREE)
    switch (type)
      {
#define DEF_RISCV_FTYPE(NUM, ARGS)					\
  case RISCV_FTYPE_NAME##NUM ARGS:					\
    types[(int) type]							\
      = build_function_type_list (RISCV_FTYPE_ATYPES##NUM ARGS,		\
				  NULL_TREE);				\
    break;
#include "config/riscv/riscv-ftypes.def"
#undef DEF_RISCV_FTYPE
      default:
	gcc_unreachable ();
      }

  return types[(int) type];
}

static void
riscv_register_float16_builtin_type (void)
{
  /* Provide the _Float16 type and float16_type_node if needed.  */
  if (!float16_type_node)
    {
      riscv_float16_type_node = make_node (REAL_TYPE);
      TYPE_PRECISION (riscv_float16_type_node) = 16;
      SET_TYPE_MODE (riscv_float16_type_node, HFmode);
      layout_type (riscv_float16_type_node);
    }
  else
    riscv_float16_type_node = float16_type_node;

  if (!maybe_get_identifier ("_Float16"))
    lang_hooks.types.register_builtin_type (riscv_float16_type_node,
					    "_Float16");
  if (!maybe_get_identifier ("__fp16"))
    lang_hooks.types.register_builtin_type (riscv_float16_type_node,
					    "__fp16");
}

static void
riscv_register_bf16_builtin_type (void)
{
  if (bfloat16_type_node == NULL_TREE)
    {
      bfloat16_type_node = make_node (REAL_TYPE);
      TYPE_PRECISION (bfloat16_type_node) = 16;
      SET_TYPE_MODE (bfloat16_type_node, BFmode);
      layout_type (bfloat16_type_node);
    }

  if (!maybe_get_identifier ("__bf16"))
    lang_hooks.types.register_builtin_type (bfloat16_type_node, "__bf16");
}

static void
riscv_init_builtin_types (void)
{
  riscv_register_float16_builtin_type ();

  riscv_register_bf16_builtin_type ();
}

/* Implement TARGET_INIT_BUILTINS.  */

void
riscv_init_builtins (void)
{
  riscv_init_builtin_types ();
  riscv_vector::init_builtins ();

  for (size_t i = 0; i < ARRAY_SIZE (riscv_builtins); i++)
    {
      const struct riscv_builtin_description *d = &riscv_builtins[i];
      if (d->avail ())
	{
	  tree type = riscv_build_function_type (d->prototype);
	  riscv_builtin_decls[i]
	    = add_builtin_function (d->name, type,
				    (i << RISCV_BUILTIN_SHIFT)
				      + RISCV_BUILTIN_GENERAL,
				    BUILT_IN_MD, NULL, NULL);
	  riscv_builtin_decl_index[d->icode] = i;
	}
    }
}

/* Implement TARGET_BUILTIN_DECL.  */

tree
riscv_builtin_decl (unsigned int code, bool initialize_p ATTRIBUTE_UNUSED)
{
  unsigned int subcode = code >> RISCV_BUILTIN_SHIFT;
  switch (code & RISCV_BUILTIN_CLASS)
    {
    case RISCV_BUILTIN_GENERAL:
      if (subcode >= ARRAY_SIZE (riscv_builtins))
	return error_mark_node;
      return riscv_builtin_decls[subcode];

    case RISCV_BUILTIN_VECTOR:
      return riscv_vector::builtin_decl (subcode, initialize_p);
    }
  return error_mark_node;
}

/* Take argument ARGNO from EXP's argument list and convert it into
   an expand operand.  Store the operand in *OP.  */

static void
riscv_prepare_builtin_arg (struct expand_operand *op, tree exp, unsigned argno,
			   enum insn_code icode, bool has_target_p)
{
  enum machine_mode mode = insn_data[icode].operand[argno + has_target_p].mode;
  rtx arg = expand_normal (CALL_EXPR_ARG (exp, argno));
  rtx tmp_rtx = gen_reg_rtx (mode);

  if (!(*insn_data[icode].operand[argno + has_target_p].predicate) (arg, mode))
    {
      if (GET_MODE_SIZE (mode).to_constant ()
	  < GET_MODE_SIZE (GET_MODE (arg)).to_constant ())
	{
	  tmp_rtx = simplify_gen_subreg (mode, arg, GET_MODE (arg), 0);
	  arg = tmp_rtx;
	}
      else if (VECTOR_MODE_P (mode) && CONST_INT_P (arg))
	{
	  /* Handle CONST_INT covert to CONST_VECTOR.  */
	  int nunits = GET_MODE_NUNITS (mode).to_constant ();
	  int i, shift = 0;
	  rtvec v = rtvec_alloc (nunits);
	  HOST_WIDE_INT val = INTVAL (arg);
	  enum machine_mode val_mode = GET_MODE_INNER (mode);
	  int shift_acc = GET_MODE_BITSIZE (val_mode).to_constant ();
	  unsigned HOST_WIDE_INT mask = GET_MODE_MASK (val_mode);
	  HOST_WIDE_INT tmp_val = val;

	  for (i = 0; i < nunits; i++)
	    {
	      tmp_val = (val >> shift) & mask;
	      RTVEC_ELT (v, i) = gen_int_mode (tmp_val, val_mode);
	      shift += shift_acc;
	    }

	  arg = copy_to_mode_reg (mode, gen_rtx_CONST_VECTOR (mode, v));
	}
      else
	{
	  convert_move (tmp_rtx, arg, false);
	  arg = tmp_rtx;
	}
    }

  create_input_operand (op, arg, mode);
}

/* Expand instruction ICODE as part of a built-in function sequence.
   Use the first NOPS elements of OPS as the instruction's operands.
   HAS_TARGET_P is true if operand 0 is a target; it is false if the
   instruction has no target.

   Return the target rtx if HAS_TARGET_P, otherwise return const0_rtx.  */

static rtx
riscv_expand_builtin_insn (enum insn_code icode, unsigned int n_ops,
			   struct expand_operand *ops, bool has_target_p)
{
  if (!maybe_expand_insn (icode, n_ops, ops))
    {
      error ("invalid argument to built-in function");
      return has_target_p ? gen_reg_rtx (ops[0].mode) : const0_rtx;
    }

  return has_target_p ? ops[0].value : const0_rtx;
}

/* Expand a RISCV_BUILTIN_DIRECT or RISCV_BUILTIN_DIRECT_NO_TARGET function;
   HAS_TARGET_P says which.  EXP is the CALL_EXPR that calls the function
   and ICODE is the code of the associated .md pattern.  TARGET, if nonnull,
   suggests a good place to put the result.  */

static rtx
riscv_expand_builtin_direct (enum insn_code icode, rtx target, tree exp,
			     bool has_target_p)
{
  struct expand_operand ops[MAX_RECOG_OPERANDS];

  /* Map any target to operand 0.  */
  int opno = 0;
  enum machine_mode mode = insn_data[icode].operand[opno].mode;

  if (has_target_p)
    {
      if (! target
	  || GET_MODE (target) != mode
	  || ! (*insn_data[icode].operand[opno].predicate) (target, mode))
	target = gen_reg_rtx (mode);

      create_output_operand (&ops[opno++], target, mode);
    }

  /* Map the arguments to the other operands.  */
  gcc_assert (opno + call_expr_nargs (exp)
	      == insn_data[icode].n_generator_args);

  for (int argno = 0; argno < call_expr_nargs (exp); argno++)
    riscv_prepare_builtin_arg (&ops[opno++], exp, argno, icode, has_target_p);

  return riscv_expand_builtin_insn (icode, opno, ops, has_target_p);
}

/* Implement TARGET_GIMPLE_FOLD_BUILTIN.  */

bool
riscv_gimple_fold_builtin (gimple_stmt_iterator *gsi)
{
  gcall *stmt = as_a<gcall *> (gsi_stmt (*gsi));
  tree fndecl = gimple_call_fndecl (stmt);
  unsigned int code = DECL_MD_FUNCTION_CODE (fndecl);
  unsigned int subcode = code >> RISCV_BUILTIN_SHIFT;
  gimple *new_stmt = NULL;
  switch (code & RISCV_BUILTIN_CLASS)
    {
    case RISCV_BUILTIN_GENERAL:
      new_stmt = NULL;
      break;

    case RISCV_BUILTIN_VECTOR:
      new_stmt = riscv_vector::gimple_fold_builtin (subcode, gsi, stmt);
      break;
    }

  if (!new_stmt)
    return false;

  gsi_replace (gsi, new_stmt, false);
  return true;
}

/* Expand a RISCV_BUILTIN_ECALL function, the ecall return value
   store in A0 register, the first pass argument save in A7/T0 register,
   and other pass argument store in A0 to A6 register.  */
static rtx
riscv_expand_builtin_ecall (enum insn_code icode, tree exp)
{
  struct expand_operand ops[MAX_RECOG_OPERANDS];
  rtx reg_arg0 = TARGET_RVE ? gen_rtx_REG (Pmode, T0_REGNUM)
			     : gen_rtx_REG (Pmode, A7_REGNUM);
  rtx retval = gen_rtx_REG (Pmode, A0_REGNUM);
  /* Map any target to operand 0.  */
  int opno = 0;

  /* Store return value in A0 register.  */
  create_output_operand (&ops[opno++], retval, TYPE_MODE (TREE_TYPE (exp)));

  /* Map the arguments to the other operands.  */
  gcc_assert (opno + call_expr_nargs (exp)
	      == insn_data[icode].n_generator_args);

  /* Process first argument save in T0/A7 register.  */
  emit_move_insn (reg_arg0, expand_normal (CALL_EXPR_ARG (exp, 0)));
  create_input_operand (&ops[opno++], reg_arg0,
			TYPE_MODE (TREE_TYPE (CALL_EXPR_ARG (exp, 0))));

  /* Process other arguments.  */
  for (int argno = 1; argno < call_expr_nargs (exp); argno++)
    {
      rtx reg_argno = gen_rtx_REG (Pmode, S1_REGNUM + argno);
      tree arg = CALL_EXPR_ARG (exp, argno);
      emit_move_insn (reg_argno, expand_normal (arg));
      create_input_operand (&ops[opno++], reg_argno,
			    TYPE_MODE (TREE_TYPE (arg)));
    }

  if (!maybe_expand_insn (icode, opno, ops))
    error ("invalid argument to built-in function");

  return retval;
}

/* Expand a RISCV_BUILTIN_ECALL function, the ebreak instruction
   pass argument save in A7/T0 register.  */
static rtx
riscv_expand_builtin_ebreak (enum insn_code icode, tree exp)
{
  struct expand_operand ops[MAX_RECOG_OPERANDS];
  rtx reg_arg0 = TARGET_RVE ? gen_rtx_REG (Pmode, T0_REGNUM)
			     : gen_rtx_REG (Pmode, A7_REGNUM);
  int opno = 0;

  /* Process first argument save in T0/A7 register.  */
  emit_move_insn (reg_arg0, expand_normal (CALL_EXPR_ARG (exp, 0)));
  create_input_operand (&ops[opno++], reg_arg0,
			TYPE_MODE (TREE_TYPE (CALL_EXPR_ARG (exp, 0))));

  if (!maybe_expand_insn (icode, opno, ops))
    error ("invalid argument to built-in function");

  return const0_rtx;
}

/* Check the arguments for __builtin_riscv_prefetch. */

bool
riscv_check_builtin_prefetch (tree exp)
{
  /* rtx addr = expand_normal (CALL_EXPR_ARG (exp, 0)); */
  rtx rw = expand_normal (CALL_EXPR_ARG (exp, 1));
  rtx locality = expand_normal (CALL_EXPR_ARG (exp, 2));
  rtx isData = expand_normal (CALL_EXPR_ARG (exp, 3));

  /* rw: 0 read, 1 write.  */
  if (!CONST_INT_P (rw) || (INTVAL (rw) < 0 || INTVAL (rw) > 1))
    {
      error ("second argument to %<__builtin_prefetch%> must be a constant "
	     "within the range [0, 1]");
      return false;
    }

  /* Use Zihintntl to implement locality.
     locality: 0 ntl.all, 1 ntl.pall, 2 ntl.p1, 3 --.
     Note that ntl.s1 cannot be generated.
  */
  if (!CONST_INT_P (locality)
      || (INTVAL (locality) < 0 || INTVAL (locality) > 3))
    {
      error ("third argument to %<__builtin_prefetch%> must be a constant "
	     "within the range [0, 3]");
      return false;
    }

  /* isData: 0 I-cache, 1 D-cache.  */
  if (!CONST_INT_P (isData) || (INTVAL (isData) < 0 || INTVAL (isData) > 1))
    {
      error ("fourth argument to %<__builtin_prefetch%> must be a constant "
	     "within the range [0, 1]");
      return false;
    }
  return true;
}

/* Check the arguments for __builtin_riscv_mop. */

bool
riscv_check_builtin_mop (tree exp)
{
  int nargs = call_expr_nargs (exp);
  rtx last_arg = expand_normal (CALL_EXPR_ARG (exp, nargs - 1));

  if (nargs == 2)
    if (!CONST_INT_P (last_arg)
	|| (INTVAL (last_arg) < 0 || INTVAL (last_arg) > 31))
      {
	error ("second argument to %<__riscv_mopr%> must be a constant "
	       "within the range [0, 31]");
	return false;
      }

  if (nargs == 3)
    if (!CONST_INT_P (last_arg)
	|| (INTVAL (last_arg) < 0 || INTVAL (last_arg) > 7))
      {
	error ("third argument to %<__riscv_moprr%> must be a constant "
	       "within the range [0, 7]");
	return false;
      }

  return true;
}

/* Implement TARGET_EXPAND_BUILTIN.  */

rtx
riscv_expand_builtin (tree exp, rtx target, rtx subtarget ATTRIBUTE_UNUSED,
		      machine_mode mode ATTRIBUTE_UNUSED,
		      int ignore ATTRIBUTE_UNUSED)
{
  tree fndecl = TREE_OPERAND (CALL_EXPR_FN (exp), 0);
  unsigned int fcode = DECL_MD_FUNCTION_CODE (fndecl);
  unsigned int subcode = fcode >> RISCV_BUILTIN_SHIFT;
  switch (fcode & RISCV_BUILTIN_CLASS)
    {
      case RISCV_BUILTIN_VECTOR:
	return riscv_vector::expand_builtin (subcode, exp, target);
      case RISCV_BUILTIN_GENERAL: {
	const struct riscv_builtin_description *d = &riscv_builtins[subcode];


	switch (d->builtin_type)
	  {
	  case RISCV_BUILTIN_DIRECT:
	    return riscv_expand_builtin_direct (d->icode, target, exp, true);

	  case RISCV_BUILTIN_DIRECT_NO_TARGET:
	    return riscv_expand_builtin_direct (d->icode, target, exp, false);

	  case RISCV_BUILTIN_ECALL:
	    return riscv_expand_builtin_ecall (d->icode, exp);

	  case RISCV_BUILTIN_EBREAK:
	    return riscv_expand_builtin_ebreak (d->icode, exp);

	  case RISCV_BUILTIN_PREFETCH:
	    if (riscv_check_builtin_prefetch (exp))
	      return riscv_expand_builtin_direct (d->icode, target, exp, false);

	  case RISCV_BUILTIN_MOP:
	    if (riscv_check_builtin_mop (exp))
	      return riscv_expand_builtin_direct (d->icode, target, exp, true);

	  default:
	    break;
	  }
      }

      default:
	break;
    }

  gcc_unreachable ();
}

/* Implement TARGET_ATOMIC_ASSIGN_EXPAND_FENV.  */

void
riscv_atomic_assign_expand_fenv (tree *hold, tree *clear, tree *update)
{
  if (!(TARGET_HARD_FLOAT || TARGET_ZFINX))
    return;

  tree frflags = GET_BUILTIN_DECL (CODE_FOR_riscv_frflags_fenv);
  tree fsflags = GET_BUILTIN_DECL (CODE_FOR_riscv_fsflags_fenv);
  tree old_flags = create_tmp_var_raw (RISCV_ATYPE_USI);

  *hold = build4 (TARGET_EXPR, RISCV_ATYPE_USI, old_flags,
		  build_call_expr (frflags, 0), NULL_TREE, NULL_TREE);
  *clear = build_call_expr (fsflags, 1, old_flags);
  *update = NULL_TREE;
}
