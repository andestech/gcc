#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "basic-block.h"
#include "tm.h"
#include "function.h"
#include "cfg.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "tree-pass.h"
#include "gimple.h"
#include "gimple-ssa.h"
#include "gimple-iterator.h"
#include "stringpool.h"
#include "langhooks.h"
#include "domwalk.h"

const pass_data pass_data_riscv_iprintf =
{
  GIMPLE_PASS,
  "iprintf",
  OPTGROUP_NONE,
  TV_NONE,
  PROP_cfg,
  0,
  0,
  0,
  0,
};

class iprintf_dom_walker : public dom_walker
{
public:
  iprintf_dom_walker () : dom_walker (CDI_DOMINATORS) {}
  ~iprintf_dom_walker () {}

  edge before_dom_children (basic_block) FINAL OVERRIDE;
  tree handle_gimple_call (gimple *);
};

class pass_riscv_iprintf : public gimple_opt_pass
{
public:
  pass_riscv_iprintf (gcc::context *ctxt)
    : gimple_opt_pass (pass_data_riscv_iprintf, ctxt) {}

  virtual bool gate (function *);
  virtual unsigned int execute (function *func);

};

bool
pass_riscv_iprintf::gate (function*)
{
  return riscv_iprintf;
}

static tree
decl_constant_value (tree decl)
{
  if (current_function_decl != 0
      && TREE_CODE (decl) != PARM_DECL
      && !TREE_THIS_VOLATILE (decl)
      && TREE_READONLY (decl)
      && DECL_INITIAL (decl) != 0
      && TREE_CODE (DECL_INITIAL (decl)) != ERROR_MARK
      && TREE_CONSTANT (DECL_INITIAL (decl))
      && TREE_CODE (DECL_INITIAL (decl)) != CONSTRUCTOR)
    return DECL_INITIAL (decl);
  return decl;
}

static const char*
get_format_string (tree format, location_t *ploc)
{
  if (VAR_P (format))
    {
      format = decl_constant_value (format);
      STRIP_NOPS (format);
    }
  if (integer_zerop (format))
    {
      return NULL;
    }
  HOST_WIDE_INT offset = 0;
  if (TREE_CODE (format) == POINTER_PLUS_EXPR)
    {
      tree arg0 = TREE_OPERAND (format, 0);
      tree arg1 = TREE_OPERAND (format, 1);
      STRIP_NOPS (arg0);
      STRIP_NOPS (arg1);
      if (TREE_CODE (arg1) != INTEGER_CST)
	return NULL;
      format = arg0;
      if (!cst_and_fits_in_hwi (arg1))
	return NULL;
      offset = int_cst_value (arg1);
    }
  if (TREE_CODE (format) != ADDR_EXPR)
    return NULL;
  *ploc = EXPR_LOC_OR_LOC (format, input_location);
  format = TREE_OPERAND (format, 0);
  if (TREE_CODE (format) == ARRAY_REF
      && tree_fits_shwi_p (TREE_OPERAND (format, 1))
      && (offset += tree_to_shwi (TREE_OPERAND (format, 1))) >= 0)
    format = TREE_OPERAND (format, 0);
  if (offset < 0)
    return NULL;
  tree array_init;
  tree array_size = NULL_TREE;
  if (VAR_P (format)
      && TREE_CODE (TREE_TYPE (format)) == ARRAY_TYPE
      && (array_init = decl_constant_value (format)) != format
      && TREE_CODE (array_init) == STRING_CST)
    {
      array_size = DECL_SIZE_UNIT (format);
      format = array_init;
    }
  if (TREE_CODE (format) != STRING_CST)
    return NULL;

  const char *fmtstr = TREE_STRING_POINTER (format);
  unsigned fmtlen = TREE_STRING_LENGTH (format);
  if (array_size)
    {
      gcc_assert (TREE_CODE (array_size) == INTEGER_CST);
      if (tree_fits_shwi_p (array_size))
	{
	  HOST_WIDE_INT array_size_value = tree_to_shwi (array_size);
	  if (array_size_value > 0
	      && array_size_value == (int) array_size_value
	      && fmtlen > array_size_value)
	    fmtlen = array_size_value;
	}
    }
  if (offset)
    {
      if (offset >= fmtlen)
	return NULL;
      fmtstr += offset;
      fmtlen -= offset;
    }
  if (fmtlen < 1 || fmtstr[--fmtlen] != 0)
    return NULL;
  return fmtstr;
}

edge
iprintf_dom_walker::before_dom_children (basic_block bb)
{
  gimple_stmt_iterator si;
  for (si = gsi_start_bb (bb); !gsi_end_p (si); gsi_next (&si))
    {
      gimple *gs = gsi_stmt (si);
      if (GIMPLE_CALL != gimple_code (gs))
	continue;

      tree new_fndecl = handle_gimple_call (gs);
      if (new_fndecl)
	{
	  gimple_call_set_fndecl(gs, new_fndecl);
	  update_stmt (gs);
	}
    }
  return NULL;
}

tree
iprintf_dom_walker::handle_gimple_call (gimple *gs)
{
  char new_fnname[16];
  int fmtstr_pos;
  tree fndecl = gimple_call_fndecl (gs);
  if (fndecl == NULL
      || lookup_attribute ("always_inline", DECL_ATTRIBUTES (fndecl)))
    return NULL;

  memset (new_fnname, 0, 16);
  switch (DECL_FUNCTION_CODE (fndecl))
    {
    case BUILT_IN_PRINTF:
      strncpy (new_fnname, "iprintf", 8);
      fmtstr_pos = 0;
      break;
    case BUILT_IN_SPRINTF:
      strncpy (new_fnname, "siprintf", 9);
      fmtstr_pos = 1;
      break;
    case BUILT_IN_SNPRINTF:
      strncpy (new_fnname, "sniprintf", 10);
      fmtstr_pos = 2;
      break;
    default:
      return NULL;
    }

  tree new_fndecl;
  tree format = gimple_call_arg (gs, fmtstr_pos);
  location_t loc = gimple_location (gs);
  const char *s = get_format_string (format, &loc);
  if (s)
    {
      // Parse the format string.
      for (; *s != '\0'; s++)
	{
	  if (*s == '%')
	    {
	      // Flags
	      for (++s; ; ++s)
		if (*s != ' ' && *s != '+' && *s != '-'
		    && *s != '#' && *s != '0')
		  break;

	      for (; *s >= '0' && *s <= '9'; ++s);

	      // Precision
	      if (*s == '.')
		{
		  if (*++s == '*')
		    ++s;
		  else
		    for (; *s >= '0' && *s <= '9'; ++s);
		}

	      // Length
	      if (*s == 'h')
		{
		  if (*++s == 'h')
		    ++s;
		}
	      else if (*s == 'l')
		{
		  if (*++s == 'l')
		    ++s;
		}
	      else if (*s == 'L' || *s == 'z'
		       || *s == 'j' || *s == 't')
		++s;

	      // Type
	      if (*s == 'A' || *s == 'a'
		  || *s == 'E' || *s == 'e'
		  || *s == 'F' || *s == 'f'
		  || *s == 'G' || *s == 'g')
		return  NULL;
	    }
	}
    }
  else
    {
      // Check by type of arguments.
      int argc = gimple_call_num_args (gs);
      for (int i = 1; i < argc; i++)
	{
	  tree farg = gimple_call_arg (gs, i);
	  switch (TREE_CODE (TREE_TYPE (farg)))
	    {
	    case REAL_TYPE:
	    case COMPLEX_TYPE:
	      return NULL;
	    default:
	      continue;
	    }
	}
    }

  new_fndecl = copy_list (fndecl);
  DECL_NAME (new_fndecl) = get_identifier (new_fnname);
  return new_fndecl;
}

unsigned int 
pass_riscv_iprintf::execute (function *func)
{
  iprintf_dom_walker iprintf_dom_walker;
  iprintf_dom_walker.walk (ENTRY_BLOCK_PTR_FOR_FN (func));
  return 0;
}

gimple_opt_pass *
make_pass_riscv_iprintf (gcc::context *ctxt)
{
  return new pass_riscv_iprintf (ctxt);
}
