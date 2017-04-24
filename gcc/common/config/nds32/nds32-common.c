#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "diagnostic-core.h"
#include "tm.h"
#include "common/common-target.h"
#include "common/common-target-def.h"
#include "opts.h"
#include "flags.h"
static bool
nds32_handle_option (struct gcc_options *opts ATTRIBUTE_UNUSED,
       struct gcc_options *opts_set ATTRIBUTE_UNUSED,
       const struct cl_decoded_option *decoded,
       location_t loc)
{
  size_t code = decoded->opt_index;
  int value = decoded->value;
  switch (code)
    {
    case OPT_misr_vector_size_:
      if (value != 4 && value != 16)
 {
   error_at (loc, "for the option -misr-vector-size=X, the valid X "
    "must be: 4 or 16");
   return false;
 }
      return true;
    case OPT_mcache_block_size_:
      if (exact_log2 (value) < 2 || exact_log2 (value) > 9)
 {
   error_at (loc, "for the option -mcache-block-size=X, the valid X "
    "must be: 4, 8, 16, 32, 64, 128, 256, or 512");
   return false;
 }
      return true;
    default:
      return true;
    }
}
static const struct default_options nds32_option_optimization_table[] =
{
#ifdef TARGET_DEFAULT_NO_MATH_ERRNO
  { OPT_LEVELS_ALL, OPT_fmath_errno, NULL, 0 },
#endif
#if TARGET_LINUX_ABI == 0
  { OPT_LEVELS_ALL, OPT_fdelete_null_pointer_checks,
          NULL, 0 },
#endif
  { OPT_LEVELS_ALL, OPT_fomit_frame_pointer, NULL, 1 },
  { OPT_LEVELS_ALL, OPT_mrelax_hint, NULL, 1 },
  { OPT_LEVELS_1_PLUS_SPEED_ONLY, OPT_malways_align, NULL, 1 },
  { OPT_LEVELS_SIZE, OPT_mv3push, NULL, 1 },
  { OPT_LEVELS_SIZE, OPT_mload_store_opt, NULL, 1 },
  { OPT_LEVELS_1_PLUS, OPT_mregrename, NULL, 1 },
  { OPT_LEVELS_1_PLUS, OPT_mgcse, NULL, 1 },
#ifdef TARGET_OS_DEFAULT_IFC
  { OPT_LEVELS_SIZE, OPT_mifc, NULL, 1 },
#endif
#ifdef TARGET_OS_DEFAULT_EX9
  { OPT_LEVELS_SIZE, OPT_mex9, NULL, 1 },
#endif
  { OPT_LEVELS_NONE, 0, NULL, 0 }
};
static enum unwind_info_type
nds32_except_unwind_info (struct gcc_options *opts ATTRIBUTE_UNUSED)
{
  if (TARGET_LINUX_ABI)
    return UI_DWARF2;
  return UI_SJLJ;
}
#undef TARGET_DEFAULT_TARGET_FLAGS
#define TARGET_DEFAULT_TARGET_FLAGS \
  (TARGET_CPU_DEFAULT \
   | TARGET_DEFAULT_FPU_ISA \
   | TARGET_DEFAULT_FPU_FMA \
   | MASK_16_BIT \
   | MASK_EXT_PERF \
   | MASK_EXT_PERF2 \
   | MASK_EXT_STRING \
   | MASK_HW_ABS \
   | MASK_CMOV)
#undef TARGET_HANDLE_OPTION
#define TARGET_HANDLE_OPTION nds32_handle_option
#undef TARGET_OPTION_OPTIMIZATION_TABLE
#define TARGET_OPTION_OPTIMIZATION_TABLE nds32_option_optimization_table
#undef TARGET_EXCEPT_UNWIND_INFO
#define TARGET_EXCEPT_UNWIND_INFO nds32_except_unwind_info
struct gcc_targetm_common targetm_common = TARGETM_COMMON_INITIALIZER;
