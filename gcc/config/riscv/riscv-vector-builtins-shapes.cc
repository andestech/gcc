/* function_shape implementation for RISC-V 'V' Extension for GNU compiler.
   Copyright (C) 2022-2024 Free Software Foundation, Inc.
   Contributed by Ju-Zhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tm_p.h"
#include "memmodel.h"
#include "insn-codes.h"
#include "optabs.h"
#include "riscv-vector-builtins.h"
#include "riscv-vector-builtins-shapes.h"

namespace riscv_vector {

/* Check whether the RETURN_TYPE and ARGUMENT_TYPES are
   valid for the function.  */

static bool
check_type (tree return_type, vec<tree> &argument_types)
{
  tree arg;
  unsigned i;

  if (!return_type)
    return false;

  FOR_EACH_VEC_ELT (argument_types, i, arg)
    if (!arg)
      return false;

  return true;
}

/* Add one function instance for GROUP, using operand suffix at index OI,
   mode suffix at index PAIR && bi and predication suffix at index pred_idx.  */
static void
build_one (function_builder &b, const function_group_info &group,
	   unsigned int pred_idx, unsigned int vec_type_idx)
{
  /* Byte forms of non-tuple vlxusegei take 21 arguments.  */
  auto_vec<tree, 21> argument_types;
  function_instance function_instance (group.base_name, *group.base,
				       *group.shape,
				       group.ops_infos.types[vec_type_idx],
				       group.preds[pred_idx], &group.ops_infos);
  tree return_type = group.ops_infos.ret.get_tree_type (
    group.ops_infos.types[vec_type_idx].index);
  b.allocate_argument_types (function_instance, argument_types);
  b.apply_predication (function_instance, return_type, argument_types);

  if (TARGET_XTHEADVECTOR && !check_type (return_type, argument_types))
    return;

  b.add_overloaded_function (function_instance, *group.shape,
			     group.required_extensions);
  b.add_unique_function (function_instance, (*group.shape), return_type,
			 argument_types, group.required_extensions);
}

/* Determine whether the intrinsic supports the currently
   processed vector type */
static bool
supports_vectype_p (const function_group_info &group, unsigned int vec_type_idx)
{
  int index = group.ops_infos.types[vec_type_idx].index;
  uint64_t ext = group.ops_infos.types[vec_type_idx].required_extensions;
  if (index < VECTOR_TYPE_vbfloat16mf4_t || index > VECTOR_TYPE_vbfloat16m8_t)
    return true;
  /* Only judge for bf16 vector type  */
  if (*group.shape == shapes::loadstore
      || *group.shape == shapes::indexed_loadstore
      || *group.shape == shapes::vundefined || *group.shape == shapes::misc
      || *group.shape == shapes::vset || *group.shape == shapes::vget
      || *group.shape == shapes::vcreate || *group.shape == shapes::fault_load
      || *group.shape == shapes::seg_loadstore
      || *group.shape == shapes::seg_indexed_loadstore
      || *group.shape == shapes::seg_fault_load
      || *group.shape == shapes::alu_f8e4m3
      || *group.shape == shapes::alu_f8e5m2
      || *group.shape == shapes::narrow_alu_f8e4m3
      || *group.shape == shapes::narrow_alu_f8e5m2
      || *group.shape == shapes::narrow_alu_frm_f8e4m3
      || *group.shape == shapes::narrow_alu_frm_f8e5m2)
    return true;
  /* Only judge for Andes mode switch */
  else if (ext == (RVV_REQUIRE_XANDESBF | RVV_REQUIRE_ELEN_64)
	   || ext == RVV_REQUIRE_XANDESBF
	   || ext
		== (RVV_REQUIRE_ELEN_64 | RVV_REQUIRE_XANDESBF
		    | RVV_REQUIRE_ZVLSIDX)
	   || ext == (RVV_REQUIRE_XANDESBF | RVV_REQUIRE_ZVLSIDX)
	   || ext
		== (RVV_REQUIRE_ELEN_64 | RVV_REQUIRE_XANDESBF
		    | RVV_REQUIRE_ZVLSS)
	   || ext == (RVV_REQUIRE_XANDESBF | RVV_REQUIRE_ZVLSS)
	   || ext
		== (RVV_REQUIRE_XANDESBF | RVV_REQUIRE_ELEN_64
		    | RVV_REQUIRE_ZVLSSEG)
	   || ext == (RVV_REQUIRE_XANDESBF | RVV_REQUIRE_ZVLSSEG)
	   || ext
		== (RVV_REQUIRE_MIN_VLEN_64
		    | RVV_REQUIRE_ZVFBFMIN_OR_XANDESBF_OR_XANDESVBFHCVT)
	   || ext == RVV_REQUIRE_ZVFBFMIN_OR_XANDESBF_OR_XANDESVBFHCVT)
    return true;
  return false;
}

/* Add a function instance for every operand && predicate && args
   combination in GROUP.  Take the function base name from GROUP && operand
   suffix from operand_suffixes && mode suffix from type_suffixes && predication
   suffix from predication_suffixes. Use apply_predication to add in
   the predicate.  */
static void
build_all (function_builder &b, const function_group_info &group)
{
  for (unsigned int pred_idx = 0; group.preds[pred_idx] != NUM_PRED_TYPES;
       ++pred_idx)
    for (unsigned int vec_type_idx = 0;
	 group.ops_infos.types[vec_type_idx].index != NUM_VECTOR_TYPES;
	 ++vec_type_idx)
      {
	if (supports_vectype_p (group, vec_type_idx))
	  build_one (b, group, pred_idx, vec_type_idx);
      }
}

/* Declare the function shape NAME, pointing it to an instance
   of class <NAME>_def.  */
#define SHAPE(DEF, VAR) \
  static CONSTEXPR const DEF##_def VAR##_obj; \
  namespace shapes { const function_shape *const VAR = &VAR##_obj; }

#define BASE_NAME_MAX_LEN 20

/* Base class for build.  */
struct build_base : public function_shape
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    build_all (b, group);
  }
};

/* vsetvl_def class.  */
struct vsetvl_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* vsetvl* instruction doesn't have C++ overloaded functions.  */
    if (overloaded_p)
      return nullptr;
    b.append_base_name (instance.base_name);
    b.append_name (type_suffixes[instance.type.index].vsetvl);
    return b.finish_name ();
  }
};

/* loadstore_def class.  */
struct loadstore_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);
    int sew = GET_MODE_BITSIZE (GET_MODE_INNER (mode));
    /* vop --> vop<sew>.  */
    if (GET_MODE_CLASS (mode) != MODE_VECTOR_BOOL)
      b.append_sew (sew);

    /* vop<sew>_v --> vop<sew>_v_<type>.  */
    if (!overloaded_p)
      {
	/* vop<sew> --> vop<sew>_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<sew>_v --> vop<sew>_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* indexed_loadstore_def class.  */
struct indexed_loadstore_def : public function_shape
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    for (unsigned int pred_idx = 0; group.preds[pred_idx] != NUM_PRED_TYPES;
	 ++pred_idx)
      {
	for (unsigned int vec_type_idx = 0;
	     group.ops_infos.types[vec_type_idx].index != NUM_VECTOR_TYPES;
	     ++vec_type_idx)
	  {
	    tree index_type = group.ops_infos.args[1].get_tree_type (
	      group.ops_infos.types[vec_type_idx].index);
	    if (!index_type)
	      continue;
	    build_one (b, group, pred_idx, vec_type_idx);
	  }
      }
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);
    /* vop<sew>_v --> vop<sew>_v_<type>.  */
    if (!overloaded_p)
      {
	/* vop<sew> --> vop<sew>_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<sew>_v --> vop<sew>_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* Add one function instance for GROUP, using operand suffix at index OI,
   mode suffix at index PAIR && bi and predication suffix at index pred_idx.  */
static void
build_th_loadstore (function_builder &b, const function_group_info &group,
		    unsigned int pred_idx, unsigned int vec_type_idx)
{
  auto_vec<tree, 5> argument_types;
  function_instance function_instance (group.base_name, *group.base,
				       *group.shape,
				       group.ops_infos.types[vec_type_idx],
				       group.preds[pred_idx], &group.ops_infos);
  tree return_type = group.ops_infos.ret.get_tree_type (
    group.ops_infos.types[vec_type_idx].index);
  b.allocate_argument_types (function_instance, argument_types);
  b.apply_predication (function_instance, return_type, argument_types);

  if (TARGET_XTHEADVECTOR && !check_type (return_type, argument_types))
    return;

  tree type = builtin_types[group.ops_infos.types[vec_type_idx].index].vector;
  if (strstr (group.base_name, "l")
      && strstr (group.base_name, "u")
      && !TYPE_UNSIGNED (TREE_TYPE (type)))
    return;

  if (strstr (group.base_name, "l")
      && !strstr (group.base_name, "u")
      && TYPE_UNSIGNED (TREE_TYPE (type)))
    return;

  machine_mode mode = TYPE_MODE (type);
  int sew = GET_MODE_BITSIZE (GET_MODE_INNER (mode));
  if (strstr (group.base_name, "h") && sew == 8)
    return;

  if (strstr (group.base_name, "w") && (sew == 8 || sew ==16))
    return;

  b.add_overloaded_function (function_instance, *group.shape,
			     group.required_extensions);
  b.add_unique_function (function_instance, (*group.shape), return_type,
			 argument_types, group.required_extensions);
}

/* th_loadstore_width_def class.  */
struct th_loadstore_width_def : public build_base
{
   void build (function_builder &b,
	      const function_group_info &group) const override
  {
    for (unsigned int pred_idx = 0; group.preds[pred_idx] != NUM_PRED_TYPES;
       ++pred_idx)
      {
	for (unsigned int vec_type_idx = 0;
	     group.ops_infos.types[vec_type_idx].index != NUM_VECTOR_TYPES;
	     ++vec_type_idx)
	  {
	    build_th_loadstore (b, group, pred_idx, vec_type_idx);
	  }
      }
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_");
    b.append_name (instance.base_name);

    /* vop_v --> vop_v_<type>.  */
    if (!overloaded_p)
      {
	/* vop --> vop_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_v --> vop_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};


/* th_indexed_loadstore_width_def class.  */
struct th_indexed_loadstore_width_def : public function_shape
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    for (unsigned int pred_idx = 0; group.preds[pred_idx] != NUM_PRED_TYPES;
	 ++pred_idx)
      {
	for (unsigned int vec_type_idx = 0;
	     group.ops_infos.types[vec_type_idx].index != NUM_VECTOR_TYPES;
	     ++vec_type_idx)
	  {
	   tree index_type = group.ops_infos.args[1].get_tree_type (
	      group.ops_infos.types[vec_type_idx].index);
	   if (!index_type)
	      continue;
	   build_th_loadstore (b, group, pred_idx, vec_type_idx);
	  }
      }
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_th_");
    b.append_name (instance.base_name);
    /* vop_v --> vop_v_<type>.  */
    if (!overloaded_p)
      {
	/* vop --> vop_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_v --> vop_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* alu_def class.  */
struct alu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }

  bool check (function_checker &c) const override
  {
    /* Check whether rounding mode argument is a valid immediate.  */
    if (c.base->has_rounding_mode_operand_p ())
      {
	/* Some invalid overload intrinsic like below will have zero for
	   c.arg_num ().  Thus, make sure arg_num is big enough here.
	   __riscv_vaadd () will make c.arg_num () == 0.  */
	if (!c.any_type_float_p () && c.arg_num () >= 2)
	  return c.require_immediate (c.arg_num () - 2, VXRM_RNU, VXRM_ROD);
	/* TODO: We will support floating-point intrinsic modeling
	   rounding mode in the future.  */
      }
    return true;
  }
};

/* Replace trailing "_f" with "_bf".  */
static void
replace_suffix_f_with_bf (char *base_name, size_t limit)
{
  size_t len = strlen (base_name);

  if (len < 2 || base_name[len - 2] != '_' || base_name[len - 1] != 'f')
    return;

  gcc_assert (len + 1 < limit);
  base_name[len - 1] = 'b';
  base_name[len] = 'f';
  base_name[len + 1] = '\0';
}

static bool
mode_is_bf_p (machine_mode mode)
{
  if (GET_MODE_INNER (mode) == E_BFmode)
    return true;
  return false;
}

static bool
instance_has_bf_mode_p (const function_instance &instance)
{
  if (mode_is_bf_p (TYPE_MODE (instance.get_return_type ())))
    return true;

  for (unsigned i = 0; instance.op_info->args[i].base_type != NUM_BASE_TYPES;
       ++i)
    if (mode_is_bf_p (TYPE_MODE (instance.get_arg_type (i))))
      return true;

  return false;
}

/* alu_bf_def class.  */
struct alu_bf_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    if (overloaded_p)
      {
	char base_name[BASE_NAME_MAX_LEN] = {};
	gcc_assert (strlen (instance.base_name) < sizeof (base_name));
	strncpy (base_name, instance.base_name, sizeof (base_name) - 1);
	base_name[sizeof (base_name) - 1] = '\0';
	replace_suffix_f_with_bf (base_name, sizeof (base_name));
	b.append_base_name (base_name);
      }
    else
      b.append_base_name (instance.base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

static void
append_f8_suffix (function_builder &b, vector_type_index vti,
		  const char *altfmt)
{
  if (vti == VECTOR_TYPE_INVALID)
    return;

  const char *suffix = type_suffixes[vti].vector;
  if (!suffix)
    return;

  if (strncmp (suffix, "_u8", 3) == 0)
    {
      b.append_name ("_");
      b.append_name (altfmt);
      b.append_name (suffix + 3);
    }
  else
    b.append_name (suffix);
}

static char *
build_f8_name (function_builder &b, const function_instance &instance,
	       bool overloaded_p, const char *altfmt)
{
  if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
    return nullptr;

  b.append_base_name (instance.base_name);

  if (overloaded_p)
    {
      b.append_name ("_");
      b.append_name (altfmt);
      b.append_name ("_bf16");
    }
  else
    {
      b.append_name (operand_suffixes[instance.op_info->op]);
      append_f8_suffix (b,
			instance.op_info->args[0].get_function_type_index (
			  instance.type.index),
			altfmt);
      b.append_name (type_suffixes[instance.type.index].vector);
    }

  if (overloaded_p && instance.pred == PRED_TYPE_m)
    return b.finish_name ();
  b.append_name (predication_suffixes[instance.pred]);
  return b.finish_name ();
}

/* alu_f8e4m3_def class.  */
struct alu_f8e4m3_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    return build_f8_name (b, instance, overloaded_p, "f8e4m3");
  }
};

/* alu_f8e5m2_def class.  */
struct alu_f8e5m2_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    return build_f8_name (b, instance, overloaded_p, "f8e5m2");
  }
};

/* The base class for frm build.  */
struct build_frm_base : public build_base
{
  /* Normalize vf<op>_frm to vf<op>.  */
  static void normalize_base_name (char *to, const char *from, int limit)
  {
    strncpy (to, from, limit - 1);
    char *suffix = strstr (to, "_frm");

    if (suffix)
      *suffix = '\0';

    to[limit - 1] = '\0';
  }

  bool check (function_checker &c) const override
  {
    gcc_assert (c.any_type_float_p ());

    /* Check whether rounding mode argument is a valid immediate.
       Some invalid overload intrinsic like below will have zero for
       c.arg_num ().  Thus, make sure arg_num is big enough here.
       __riscv_vaadd () will make c.arg_num () == 0.  */
    if (c.base->has_rounding_mode_operand_p () && c.arg_num () >= 2)
      {
	unsigned int frm_num = c.arg_num () - 2;

	return c.require_immediate (frm_num, FRM_STATIC_MIN, FRM_STATIC_MAX);
      }

    return true;
  }
};

/* alu_frm_def class.  */
struct alu_frm_def : public build_frm_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    if (overloaded_p && instance_has_bf_mode_p (instance))
      replace_suffix_f_with_bf (base_name, sizeof (base_name));
    b.append_base_name (base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    return b.finish_name ();
  }
};

/* widen_alu_frm_def class.  */
struct widen_alu_frm_def : public build_frm_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    return b.finish_name ();
  }
};

/* narrow_alu_frm_def class.  */
struct narrow_alu_frm_def : public build_frm_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    if (overloaded_p && instance_has_bf_mode_p (instance))
      replace_suffix_f_with_bf (base_name, sizeof (base_name));
    b.append_base_name (base_name);

    if (!overloaded_p)
      {
	/* vop --> vop_<op>.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_<op> --> vop_<op>_<type>.  */
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    return b.finish_name ();
  }
};

/* reduc_alu_frm_def class.  */
struct reduc_alu_frm_def : public build_frm_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    return b.finish_name ();
  }
};

/* widen_alu_def class. Handle vwadd/vwsub. Unlike
   vadd.vx/vadd.vv/vwmul.vv/vwmul.vx, vwadd.vv/vwadd.vx/vwadd.wv/vwadd.wx has
   'OP' suffix in overloaded API.  */
struct widen_alu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* no_mask_policy_def class. Such instructions belong to this class
   doesn't need mask policy.  */
struct no_mask_policy_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* return_mask_def class. Such instructions belong to this class
   is returning mask value.  */
struct return_mask_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type1>_<type2>.  */
    if (!overloaded_p)
      {
	b.append_name (type_suffixes[instance.type.index].vector);
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* narrow_alu_def class. Handle narrowing instructions like vnsrl.wv.  */
struct narrow_alu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    if (overloaded_p && instance_has_bf_mode_p (instance))
      {
	char base_name[BASE_NAME_MAX_LEN] = {};
	gcc_assert (strlen (instance.base_name) < sizeof (base_name));
	strncpy (base_name, instance.base_name, sizeof (base_name) - 1);
	base_name[sizeof (base_name) - 1] = '\0';
	replace_suffix_f_with_bf (base_name, sizeof (base_name));
	b.append_base_name (base_name);
      }
    else
      b.append_base_name (instance.base_name);

    if (!overloaded_p)
      {
	/* vop --> vop_<op>.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_<op> --> vop_<op>_<type>.  */
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }

  bool check (function_checker &c) const override
  {
    /* Check whether rounding mode argument is a valid immediate.  */
    if (c.base->has_rounding_mode_operand_p ())
      {
	/* Some invalid overload intrinsic like below will have zero for
	   c.arg_num ().  Thus, make sure arg_num is big enough here.
	   __riscv_vaadd () will make c.arg_num () == 0.  */
	if (!c.any_type_float_p () && c.arg_num () >= 2)
	  return c.require_immediate (c.arg_num () - 2, VXRM_RNU, VXRM_ROD);
	/* TODO: We will support floating-point intrinsic modeling
	   rounding mode in the future.  */
      }
    return true;
  }
};

static char *
build_f8_narrow_name (function_builder &b, const function_instance &instance,
		      bool overloaded_p, const char *altfmt, bool frm_p)
{
  if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
    return nullptr;

  const char *base_name = instance.base_name;
  char base_name_buf[BASE_NAME_MAX_LEN] = {};
  if (frm_p)
    {
      build_frm_base::normalize_base_name (base_name_buf, instance.base_name,
					   sizeof (base_name_buf));
      base_name = base_name_buf;
    }

  b.append_base_name (base_name);

  if (overloaded_p)
    {
      vector_type_index vti
	= instance.op_info->args[0].get_function_type_index (
	  instance.type.index);
      if (instance.op_info->op == OP_TYPE_f_w)
	{
	  const char *src_scalar
	    = vti == VECTOR_TYPE_INVALID ? nullptr : type_suffixes[vti].scalar;
	  b.append_name (src_scalar ? src_scalar : "_bf16");
	}
      b.append_name ("_");
      b.append_name (altfmt);
    }
  else
    {
      b.append_name (operand_suffixes[instance.op_info->op]);
      vector_type_index vti
	= instance.op_info->args[0].get_function_type_index (
	  instance.type.index);
      /* Additional type suffix before output type suffix.  */
      if (vti != VECTOR_TYPE_INVALID && instance.op_info->op != OP_TYPE_f_q)
	b.append_name (type_suffixes[vti].vector);
      append_f8_suffix (b,
			instance.op_info->ret.get_function_type_index (
			  instance.type.index),
			altfmt);
      if (frm_p)
	b.append_name ("_rm");
    }

  if (overloaded_p && instance.pred == PRED_TYPE_m)
    return b.finish_name ();
  b.append_name (predication_suffixes[instance.pred]);
  return b.finish_name ();
}

/* narrow_alu_f8e4m3_def class.  */
struct narrow_alu_f8e4m3_def : public narrow_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    return build_f8_narrow_name (b, instance, overloaded_p, "f8e4m3", false);
  }
};

/* narrow_alu_f8e5m2_def class.  */
struct narrow_alu_f8e5m2_def : public narrow_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    return build_f8_narrow_name (b, instance, overloaded_p, "f8e5m2", false);
  }
};

/* narrow_alu_frm_f8e4m3_def class.  */
struct narrow_alu_frm_f8e4m3_def : public narrow_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    return build_f8_narrow_name (b, instance, overloaded_p, "f8e4m3", true);
  }
};

/* narrow_alu_frm_f8e5m2_def class.  */
struct narrow_alu_frm_f8e5m2_def : public narrow_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    return build_f8_narrow_name (b, instance, overloaded_p, "f8e5m2", true);
  }
};

/* move_def class. Handle vmv.v.v/vmv.v.x.  */
struct move_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* vmv.v.x/vfmv.v.f (PRED_none) can not be overloaded.  */
    if ((instance.op_info->op == OP_TYPE_x || instance.op_info->op == OP_TYPE_f)
	&& overloaded_p && instance.pred == PRED_TYPE_none)
      return nullptr;

    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* mask_alu_def class.  */
struct mask_alu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    if (instance.op_info->op == OP_TYPE_mm || instance.op_info->op == OP_TYPE_m)
      if (!overloaded_p)
	b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* reduc_alu_def class.  */
struct reduc_alu_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* th_extract_def class.  */
struct th_extract_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
      bool overloaded_p) const override
  {
    b.append_name ("__riscv_th_");
    b.append_name (instance.base_name);

    if (overloaded_p)
      return b.finish_name ();
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name (type_suffixes[instance.type.index].scalar);
    return b.finish_name ();
  }
};

/* scalar_move_def class.  */
struct scalar_move_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);
    if (overloaded_p)
      return b.finish_name ();
    b.append_name (operand_suffixes[instance.op_info->op]);
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name (type_suffixes[instance.type.index].scalar);
    return b.finish_name ();
  }
};

/* vundefined_def class.  */
struct vundefined_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    if (overloaded_p)
      return nullptr;
    b.append_base_name (instance.base_name);
    b.append_name (type_suffixes[instance.type.index].vector);
    return b.finish_name ();
  }
};

/* misc_def class.  */
struct misc_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	vector_type_index arg0_type_idx
	  = instance.op_info->args[0].get_function_type_index (
	    instance.type.index);
	b.append_name (type_suffixes[arg0_type_idx].vector);
      }

    vector_type_index ret_type_idx
      = instance.op_info->ret.get_function_type_index (instance.type.index);
    b.append_name (type_suffixes[ret_type_idx].vector);
    return b.finish_name ();
  }
};

/* vset_def class.  */
struct vset_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	vector_type_index arg_type_idx
	  = instance.op_info->args[2].get_function_type_index (
	    instance.type.index);
	b.append_name (type_suffixes[arg_type_idx].vector);

	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }
    return b.finish_name ();
  }

  bool check (function_checker &c) const override
  {
    poly_int64 outer_size = GET_MODE_SIZE (c.arg_mode (0));
    poly_int64 inner_size = GET_MODE_SIZE (c.arg_mode (2));
    unsigned int nvecs = exact_div (outer_size, inner_size).to_constant ();
    return c.require_immediate (1, 0, nvecs - 1);
  }
};

/* vget_def class.  */
struct vget_def : public misc_def
{
  bool check (function_checker &c) const override
  {
    poly_int64 outer_size = GET_MODE_SIZE (c.arg_mode (0));
    poly_int64 inner_size = GET_MODE_SIZE (c.ret_mode ());
    unsigned int nvecs = exact_div (outer_size, inner_size).to_constant ();
    return c.require_immediate (1, 0, nvecs - 1);
  }
};

/* vcreate_def class.  */
struct vcreate_def : public build_base
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    for (unsigned int vec_type_idx = 0;
	 group.ops_infos.types[vec_type_idx].index != NUM_VECTOR_TYPES;
	 ++vec_type_idx)
      {
	  auto_vec<tree, 8> argument_types;
	  function_instance function_instance (group.base_name, *group.base,
					      *group.shape,
					      group.ops_infos.types[vec_type_idx],
					      group.preds[0], &group.ops_infos);

	  tree return_type = group.ops_infos.ret.get_tree_type (
	    group.ops_infos.types[vec_type_idx].index);

	  if (!return_type)
	    continue;

	  tree arg_type = function_instance.op_info->args[0].get_tree_type (
	    function_instance.type.index);

	  machine_mode outer_mode = TYPE_MODE (return_type);
	  machine_mode inner_mode = TYPE_MODE (arg_type);
	  unsigned int nargs
	    = exact_div (GET_MODE_SIZE (outer_mode), GET_MODE_SIZE (inner_mode))
		.to_constant ();

	  for (unsigned int i = 0; i < nargs; i++)
	    argument_types.quick_push (arg_type);

	  b.add_unique_function (function_instance, (*group.shape), return_type,
	    argument_types, group.required_extensions);
     }
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    if (overloaded_p)
      return nullptr;
    b.append_base_name (instance.base_name);
    b.append_name (operand_suffixes[instance.op_info->op]);

    if (instance.op_info->ret.base_type != RVV_BASE_vector)
      {
	vector_type_index arg_type_idx
	  = instance.op_info->args[0].get_function_type_index (
	    instance.type.index);
	b.append_name (type_suffixes[arg_type_idx].vector);
      }

    vector_type_index ret_type_idx
      = instance.op_info->ret.get_function_type_index (instance.type.index);
    b.append_name (type_suffixes[ret_type_idx].vector);
    return b.finish_name ();
  }
};

/* read_vl_def class.  */
struct read_vl_def : public function_shape
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    auto_vec<tree> argument_types;
    b.add_unique_function (get_read_vl_instance (), (*group.shape),
			   size_type_node, argument_types,
			   group.required_extensions);
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    if (overloaded_p)
      return nullptr;
    b.append_base_name (instance.base_name);
    return b.finish_name ();
  }
};

/* fault_load_def class.  */
struct fault_load_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;
    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);
    int sew = GET_MODE_BITSIZE (GET_MODE_INNER (mode));
    b.append_name ("__riscv_");
    b.append_name ("vle");
    b.append_sew (sew);
    b.append_name ("ff");

    /* vop<sew>_v --> vop<sew>_v_<type>.  */
    if (!overloaded_p)
      {
	/* vop<sew> --> vop<sew>_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<sew>_v --> vop<sew>_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* vlenb_def class.  */
struct vlenb_def : public function_shape
{
  void build (function_builder &b,
	      const function_group_info &group) const override
  {
    auto_vec<tree> argument_types;
    function_instance function_instance (group.base_name, *group.base,
					 *group.shape, group.ops_infos.types[0],
					 group.preds[0], &group.ops_infos);
    b.add_unique_function (function_instance, (*group.shape),
			   long_unsigned_type_node, argument_types,
			   group.required_extensions);
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    if (overloaded_p)
      return nullptr;
    b.append_base_name (instance.base_name);
    return b.finish_name ();
  }
};

/* seg_loadstore_def class.  */
struct seg_loadstore_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>e.  */
    b.append_name ("e");

    int sew = GET_MODE_BITSIZE (GET_MODE_INNER (mode));
    /* vop<nf>e --> vop<nf>e<sew>.  */
    b.append_sew (sew);

    if (!overloaded_p)
      {
	/* vop<nf>e<sew> --> vop<nf>e<sew>_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>e<sew>_v --> vop<nf>e<sew>_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* seg_indexed_loadstore_def class.  */
struct seg_indexed_loadstore_def : public indexed_loadstore_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>ei.  */
    b.append_name ("ei");

    /* vop<nf>ei --> vop<nf>ei<eew>.  */
    vector_type_index arg1_type_idx
      = instance.op_info->args[1].get_function_type_index (instance.type.index);
    tree index_type = builtin_types[arg1_type_idx].vector;
    machine_mode index_mode = TYPE_MODE (index_type);
    int eew = GET_MODE_BITSIZE (GET_MODE_INNER (index_mode));
    b.append_sew (eew);

    if (!overloaded_p)
      {
	/* vop<sew> --> vop<sew>_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<sew>_v --> vop<sew>_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* seg_fault_load_def class.  */
struct seg_fault_load_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_vlseg");

    tree type = builtin_types[instance.type.index].vector;
    machine_mode mode = TYPE_MODE (type);

    int nf = get_nf (mode);
    /* vop --> vop<nf>.  */
    b.append_nf (nf);

    /* vop<nf> --> vop<nf>e.  */
    b.append_name ("e");

    int sew = GET_MODE_BITSIZE (GET_MODE_INNER (mode));
    /* vop<nf>e --> vop<nf>e<sew>.  */
    b.append_sew (sew);

    /* vop<nf>e<sew> --> vop<nf>e<sew>ff.  */
    b.append_name ("ff");

    if (!overloaded_p)
      {
	/* vop<nf>e<sew>ff --> vop<nf>e<sew>ff_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop<nf>e<sew>ff_v --> vop<nf>e<sew>ff_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* vsm4r/vaes* class.  */
struct crypto_vv_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
                  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;
    b.append_base_name (instance.base_name);
    b.append_name (operand_suffixes[instance.op_info->op]);

    if (!overloaded_p)
    {
      if (instance.op_info->op == OP_TYPE_vv)
        b.append_name (type_suffixes[instance.type.index].vector);
      else
      {
        vector_type_index arg0_type_idx
          = instance.op_info->args[1].get_function_type_index
            (instance.type.index);
        b.append_name (type_suffixes[arg0_type_idx].vector);
        vector_type_index ret_type_idx
          = instance.op_info->ret.get_function_type_index
            (instance.type.index);
        b.append_name (type_suffixes[ret_type_idx].vector);
      }
    }

    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* vaeskf1/vaeskf2/vsm4k/vsm3c class.  */
struct crypto_vi_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
                  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;
    b.append_base_name (instance.base_name);
    if (!overloaded_p)
    {
      b.append_name (operand_suffixes[instance.op_info->op]);
      b.append_name (type_suffixes[instance.type.index].vector);
    }
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* vaesz class.  */
struct crypto_vv_no_op_type_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
                  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
    {
      b.append_name (operand_suffixes[instance.op_info->op]);
      vector_type_index arg0_type_idx
        = instance.op_info->args[1].get_function_type_index
          (instance.type.index);
      b.append_name (type_suffixes[arg0_type_idx].vector);
      vector_type_index ret_type_idx
        = instance.op_info->ret.get_function_type_index
          (instance.type.index);
      b.append_name (type_suffixes[ret_type_idx].vector);
    }

    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

/* alu_default_fp_def class.  */
struct alu_default_fp_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>_default_fp.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_fp");

    return b.finish_name ();
  }
};

/* alu_ms_fp_def class.  */
struct alu_ms_fp_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>_<pred>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_fp");

    return b.finish_name ();
  }
};

/* alu_default_bf_def class.  */
struct alu_default_bf_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_bf");

    return b.finish_name ();
  }
};

/* alu_ms_bf_def class.  */
struct alu_ms_bf_def : public alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_base_name (instance.base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_bf");

    return b.finish_name ();
  }
};

/* alu_frm_default_fp_def class.  */
struct alu_frm_default_fp_def : public alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_fp");

    return b.finish_name ();
  }
};

/* alu_frm_ms_fp_def class.  */
struct alu_frm_ms_fp_def : public alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    b.append_name (type_suffixes[instance.type.index].vector);

    /* vop<sew>_<op>_<type> --> vop<sew>_<op>_<type>_rm_<pred>.  */
    b.append_name ("_rm");
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_fp");

    return b.finish_name ();
  }
};

/* alu_frm_default_bf_def class.  */
struct alu_frm_default_bf_def : public alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_bf");

    return b.finish_name ();
  }
};

/* alu_frm_ms_bf_def class.  */
struct alu_frm_ms_bf_def : public alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    b.append_name (type_suffixes[instance.type.index].vector);

    /* vop<sew>_<op>_<type> --> vop<sew>_<op>_<type>_rm<pred>.  */
    b.append_name ("_rm");
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_bf");

    return b.finish_name ();
  }
};

/* widen_alu_default_fp_def class.  */
struct widen_alu_default_fp_def : public widen_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_fp");

    return b.finish_name ();
  }
};

/* widen_alu_ms_fp_def class.  */
struct widen_alu_ms_fp_def : public widen_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>_<pred>.  */
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_fp");

    return b.finish_name ();
  }
};

/* widen_alu_default_bf_def class.  */
struct widen_alu_default_bf_def : public widen_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_bf");

    return b.finish_name ();
  }
};

/* widen_alu_ms_bf_def class.  */
struct widen_alu_ms_bf_def : public widen_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>_<pred>.  */
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_bf");

    return b.finish_name ();
  }
};

/* widen_alu_frm_default_fp_def class.  */
struct widen_alu_frm_default_fp_def : public widen_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_fp");

    return b.finish_name ();
  }
};

/* widen_alu_frm_ms_fp_def class.  */
struct widen_alu_frm_ms_fp_def : public widen_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>_rm_<pred>.  */
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name ("_rm");
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_fp");

    return b.finish_name ();
  }
};

/* widen_alu_frm_default_bf_def class.  */
struct widen_alu_frm_default_bf_def : public widen_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      b.append_name (type_suffixes[instance.type.index].vector);

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_bf");

    return b.finish_name ();
  }
};

/* widen_alu_frm_ms_bf_def class.  */
struct widen_alu_frm_ms_bf_def : public widen_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew> --> vop<sew>_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>_rm_<pred>.  */
    b.append_name (type_suffixes[instance.type.index].vector);
    b.append_name ("_rm");
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_bf");

    return b.finish_name ();
  }
};

/* narrow_alu_default_fp_def class.  */
struct narrow_alu_default_fp_def : public narrow_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      {
	/* vop --> vop_<op>.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_<op> --> vop_<op>_<type>.  */
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_fp");

    return b.finish_name ();
  }
};

/* narrow_alu_ms_fp_def class.  */
struct narrow_alu_ms_fp_def : public narrow_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop --> vop_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    /* vop_<op> --> vop_<op>_<type>.  */
    vector_type_index ret_type_idx
      = instance.op_info->ret.get_function_type_index (instance.type.index);
    b.append_name (type_suffixes[ret_type_idx].vector);
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_fp");

    return b.finish_name ();
  }
};

/* narrow_alu_default_bf_def class.  */
struct narrow_alu_default_bf_def : public narrow_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    if (!overloaded_p)
      {
	/* vop --> vop_<op>.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_<op> --> vop_<op>_<type>.  */
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_bf");

    return b.finish_name ();
  }
};

/* narrow_alu_ms_bf_def class.  */
struct narrow_alu_ms_bf_def : public narrow_alu_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    b.append_base_name (instance.base_name);

    /* vop --> vop_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    /* vop_<op> --> vop_<op>_<type>.  */
    vector_type_index ret_type_idx
      = instance.op_info->ret.get_function_type_index (instance.type.index);
    b.append_name (type_suffixes[ret_type_idx].vector);
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_bf");

    return b.finish_name ();
  }
};

/* narrow_alu_frm_default_fp_def class.  */
struct narrow_alu_frm_default_fp_def : public narrow_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    if (!overloaded_p)
      {
	/* vop --> vop_<op>.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_<op> --> vop_<op>_<type>.  */
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_fp");

    return b.finish_name ();
  }
};

/* narrow_alu_frm_ms_fp_def class.  */
struct narrow_alu_frm_ms_fp_def : public narrow_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop --> vop_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    /* vop_<op> --> vop_<op>_<type>.  */
    vector_type_index ret_type_idx
      = instance.op_info->ret.get_function_type_index (instance.type.index);
    b.append_name (type_suffixes[ret_type_idx].vector);
    b.append_name ("_rm");
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_fp");

    return b.finish_name ();
  }
};

/* narrow_alu_frm_default_bf_def class.  */
struct narrow_alu_frm_default_bf_def : public narrow_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    if (!overloaded_p)
      {
	/* vop --> vop_<op>.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_<op> --> vop_<op>_<type>.  */
	vector_type_index ret_type_idx
	  = instance.op_info->ret.get_function_type_index (instance.type.index);
	b.append_name (type_suffixes[ret_type_idx].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_rm" suffix
       for vop_rm C++ overloaded API.  */
    if (!overloaded_p)
      b.append_name ("_rm");

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();

    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_default_bf");

    return b.finish_name ();
  }
};

/* narrow_alu_frm_ms_bf_def class.  */
struct narrow_alu_frm_ms_bf_def : public narrow_alu_frm_def
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    char base_name[BASE_NAME_MAX_LEN] = {};

    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop --> vop_<op>.  */
    b.append_name (operand_suffixes[instance.op_info->op]);
    /* vop_<op> --> vop_<op>_<type>.  */
    vector_type_index ret_type_idx
      = instance.op_info->ret.get_function_type_index (instance.type.index);
    b.append_name (type_suffixes[ret_type_idx].vector);
    b.append_name ("_rm");
    b.append_name (predication_suffixes[instance.pred]);

    if (!overloaded_p)
      b.append_name ("_ms_bf");

    return b.finish_name ();
  }
};

/* ntlh_def class. Handle NTLH intrinsics.  */
struct ntlh_def : public build_base
{
  /* Normalize <name><postfix> to <name>.  */
  static void normalize_base_name (char *to, const char *from, int limit,
				   const char *postfix)
  {
    strncpy (to, from, limit - 1);
    char *suffix = strstr (to, postfix);

    if (suffix)
      *suffix = '\0';

    to[limit - 1] = '\0';
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    if (!overloaded_p)
      {
	tree type = builtin_types[instance.type.index].vector;
	machine_mode mode = TYPE_MODE (type);
	/* All SCALAR_TYPE_* register scalar types in their vector field.  */
	bool vector_p = VECTOR_MODE_P (mode);

	b.append_base_name (instance.base_name);

	if (vector_p)
	  {
	    /* vop --> vop<sew>.  */
	    if (GET_MODE_CLASS (mode) != MODE_VECTOR_BOOL)
	      b.append_sew (GET_MODE_BITSIZE (GET_MODE_INNER (mode)));
	    /* vop<sew> --> vop<sew>_v.  */
	    b.append_name (operand_suffixes[instance.op_info->op]);
	    /* vop<sew>_v --> vop<sew>_v_<type>.  */
	    b.append_name (type_suffixes[instance.type.index].vector);
	  }
	else
	  {
	    /* Scalar: op --> op<[u]int[8,16,32,64]>.
	       Float:  op --> op<float[16,32,64]>.  */
	    if (!FLOAT_MODE_P (mode))
	      b.append_int (TYPE_UNSIGNED (type),
			    GET_MODE_BITSIZE (mode).to_constant ());
	    else
	      b.append_float (GET_MODE_BITSIZE (mode).to_constant ());
	  }
      }
    else
      {
	char base_name[BASE_NAME_MAX_LEN] = {};

	normalize_base_name (base_name, instance.base_name, sizeof (base_name),
			     "_default");
	b.append_base_name (base_name);
      }

    return b.finish_name ();
  }

  bool check (function_checker &c) const override
  {
    /* Check the domain parameter, but no need to check overloaded
       intrinsics that use a default domain argument.  */
    if (!strstr (c.base_name, "_default"))
      return c.require_immediate (c.arg_num () - 1, 2, 5);
    return true;
  }
};

/* nibbleload_def class.  */
struct nibbleload_def : public build_base
{
  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    b.append_name ("__riscv_nds_");
    b.append_name (instance.base_name);

    /* vop_v --> vop_v_<type>.  */
    if (!overloaded_p)
      {
	/* vop --> vop_v.  */
	b.append_name (operand_suffixes[instance.op_info->op]);
	/* vop_v --> vop_v_<type>.  */
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

struct vcpopv_def : public alu_def
{
  /* Normalize v<op>v to v<op>.  */
  static void normalize_base_name (char *to, const char *from, int limit)
  {
    strncpy (to, from, limit - 1);
    char *suffix = strstr (to + 1, "v");

    if (suffix)
      *suffix = '\0';

    to[limit - 1] = '\0';
  }

  char *get_name (function_builder &b, const function_instance &instance,
		  bool overloaded_p) const override
  {
    /* Return nullptr if it can not be overloaded.  */
    if (overloaded_p && !instance.base->can_be_overloaded_p (instance.pred))
      return nullptr;

    char base_name[BASE_NAME_MAX_LEN] = {};
    normalize_base_name (base_name, instance.base_name, sizeof (base_name));

    b.append_base_name (base_name);

    /* vop<sew>_<op> --> vop<sew>_<op>_<type>.  */
    if (!overloaded_p)
      {
	b.append_name (operand_suffixes[instance.op_info->op]);
	b.append_name (type_suffixes[instance.type.index].vector);
      }

    /* According to rvv-intrinsic-doc, it does not add "_m" suffix
       for vop_m C++ overloaded API.  */
    if (overloaded_p && instance.pred == PRED_TYPE_m)
      return b.finish_name ();
    b.append_name (predication_suffixes[instance.pred]);
    return b.finish_name ();
  }
};

SHAPE(vsetvl, vsetvl)
SHAPE(vsetvl, vsetvlmax)
SHAPE(loadstore, loadstore)
SHAPE(indexed_loadstore, indexed_loadstore)
SHAPE(th_loadstore_width, th_loadstore_width)
SHAPE(th_indexed_loadstore_width, th_indexed_loadstore_width)
SHAPE(alu, alu)
SHAPE(alu_frm, alu_frm)
SHAPE(widen_alu, widen_alu)
SHAPE(widen_alu_frm, widen_alu_frm)
SHAPE(no_mask_policy, no_mask_policy)
SHAPE(return_mask, return_mask)
SHAPE(narrow_alu, narrow_alu)
SHAPE(narrow_alu_frm, narrow_alu_frm)
SHAPE(move, move)
SHAPE(mask_alu, mask_alu)
SHAPE(reduc_alu, reduc_alu)
SHAPE(reduc_alu_frm, reduc_alu_frm)
SHAPE(th_extract, th_extract)
SHAPE(scalar_move, scalar_move)
SHAPE(vundefined, vundefined)
SHAPE(misc, misc)
SHAPE(vset, vset)
SHAPE(vget, vget)
SHAPE(vcreate, vcreate)
SHAPE(read_vl, read_vl)
SHAPE(fault_load, fault_load)
SHAPE(vlenb, vlenb)
SHAPE(seg_loadstore, seg_loadstore)
SHAPE(seg_indexed_loadstore, seg_indexed_loadstore)
SHAPE(seg_fault_load, seg_fault_load)
SHAPE(crypto_vv, crypto_vv)
SHAPE(crypto_vi, crypto_vi)
SHAPE(crypto_vv_no_op_type, crypto_vv_no_op_type)
SHAPE (vcpopv, vcpopv)
/* Andes BF16 Mode Switch. */
SHAPE (alu_bf, alu_bf)
SHAPE (alu_default_fp, alu_default_fp)
SHAPE (alu_ms_fp, alu_ms_fp)
SHAPE (alu_default_bf, alu_default_bf)
SHAPE (alu_ms_bf, alu_ms_bf)
SHAPE (alu_frm_default_fp, alu_frm_default_fp)
SHAPE (alu_frm_ms_fp, alu_frm_ms_fp)
SHAPE (alu_frm_default_bf, alu_frm_default_bf)
SHAPE (alu_frm_ms_bf, alu_frm_ms_bf)
SHAPE (widen_alu_default_fp, widen_alu_default_fp)
SHAPE (widen_alu_ms_fp, widen_alu_ms_fp)
SHAPE (widen_alu_default_bf, widen_alu_default_bf)
SHAPE (widen_alu_ms_bf, widen_alu_ms_bf)
SHAPE (widen_alu_frm_default_fp, widen_alu_frm_default_fp)
SHAPE (widen_alu_frm_ms_fp, widen_alu_frm_ms_fp)
SHAPE (widen_alu_frm_default_bf, widen_alu_frm_default_bf)
SHAPE (widen_alu_frm_ms_bf, widen_alu_frm_ms_bf)
SHAPE (narrow_alu_default_fp, narrow_alu_default_fp)
SHAPE (narrow_alu_ms_fp, narrow_alu_ms_fp)
SHAPE (narrow_alu_default_bf, narrow_alu_default_bf)
SHAPE (narrow_alu_ms_bf, narrow_alu_ms_bf)
SHAPE (narrow_alu_frm_default_fp, narrow_alu_frm_default_fp)
SHAPE (narrow_alu_frm_ms_fp, narrow_alu_frm_ms_fp)
SHAPE (narrow_alu_frm_default_bf, narrow_alu_frm_default_bf)
SHAPE (narrow_alu_frm_ms_bf, narrow_alu_frm_ms_bf)
SHAPE (ntlh, ntlh)
SHAPE (nibbleload, nibbleload)
/* Zvfofp8min */
SHAPE (alu_f8e4m3, alu_f8e4m3)
SHAPE (alu_f8e5m2, alu_f8e5m2)
SHAPE (narrow_alu_f8e4m3, narrow_alu_f8e4m3)
SHAPE (narrow_alu_f8e5m2, narrow_alu_f8e5m2)
SHAPE (narrow_alu_frm_f8e4m3, narrow_alu_frm_f8e4m3)
SHAPE (narrow_alu_frm_f8e5m2, narrow_alu_frm_f8e5m2)
} // end namespace riscv_vector
