#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"
#include "insn-codes.h"
#include "reload.h"
#include "flags.h"
#include "function.h"
#include "expr.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "df.h"
#include "tm_p.h"
#include "tm-constrs.h"
#include "optabs.h"
#include "target.h"
#include "target-def.h"
#include "langhooks.h"
#include "ggc.h"
static struct nds32_isr_info nds32_isr_vectors[NDS32_N_ISR_VECTORS];
static void
nds32_interrupt_attribute_parse_string (const char *original_str,
     const char *func_name)
{
  char target_str[100];
  enum nds32_isr_save_reg save_reg;
  enum nds32_isr_nested_type nested_type;
  char *save_all_regs_str, *save_caller_regs_str;
  char *nested_str, *not_nested_str, *ready_nested_str, *critical_str;
  char *id_str, *value_str;
  strcpy (target_str, original_str);
  save_all_regs_str = strstr (target_str, "save_all_regs");
  save_caller_regs_str = strstr (target_str, "save_caller_regs");
  if (save_all_regs_str)
    save_reg = NDS32_SAVE_ALL;
  else if (save_caller_regs_str)
    save_reg = NDS32_PARTIAL_SAVE;
  else
    save_reg = NDS32_PARTIAL_SAVE;
  nested_str = strstr (target_str, "nested");
  not_nested_str = strstr (target_str, "not_nested");
  ready_nested_str = strstr (target_str, "ready_nested");
  critical_str = strstr (target_str, "critical");
  if (not_nested_str)
    nested_type = NDS32_NOT_NESTED;
  else if (ready_nested_str)
    nested_type = NDS32_NESTED_READY;
  else if (nested_str)
    nested_type = NDS32_NESTED;
  else if (critical_str)
    nested_type = NDS32_CRITICAL;
  else
    nested_type = NDS32_NOT_NESTED;
  id_str = strstr (target_str, "id=");
  if (id_str == NULL)
    error ("require id argument in the string");
  id_str = strtok (id_str, "=");
  value_str = strtok (NULL, ";");
  value_str = strtok (value_str, ",");
  while (value_str != NULL)
    {
      int i;
      i = atoi (value_str);
      i = i + 9;
      if (i < 9 || i > 72)
 error ("invalid id value for interrupt attribute");
      nds32_isr_vectors[i].category = NDS32_ISR_INTERRUPT;
      strcpy (nds32_isr_vectors[i].func_name, func_name);
      nds32_isr_vectors[i].save_reg = save_reg;
      nds32_isr_vectors[i].nested_type = nested_type;
      value_str = strtok (NULL, ",");
    }
  return;
}
static void
nds32_exception_attribute_parse_string (const char *original_str,
     const char *func_name)
{
  char target_str[100];
  enum nds32_isr_save_reg save_reg;
  enum nds32_isr_nested_type nested_type;
  char *save_all_regs_str, *save_caller_regs_str;
  char *nested_str, *not_nested_str, *ready_nested_str, *critical_str;
  char *id_str, *value_str;
  strcpy (target_str, original_str);
  save_all_regs_str = strstr (target_str, "save_all_regs");
  save_caller_regs_str = strstr (target_str, "save_caller_regs");
  if (save_all_regs_str)
    save_reg = NDS32_SAVE_ALL;
  else if (save_caller_regs_str)
    save_reg = NDS32_PARTIAL_SAVE;
  else
    save_reg = NDS32_PARTIAL_SAVE;
  nested_str = strstr (target_str, "nested");
  not_nested_str = strstr (target_str, "not_nested");
  ready_nested_str = strstr (target_str, "ready_nested");
  critical_str = strstr (target_str, "critical");
  if (not_nested_str)
    nested_type = NDS32_NOT_NESTED;
  else if (ready_nested_str)
    nested_type = NDS32_NESTED_READY;
  else if (nested_str)
    nested_type = NDS32_NESTED;
  else if (critical_str)
    nested_type = NDS32_CRITICAL;
  else
    nested_type = NDS32_NOT_NESTED;
  id_str = strstr (target_str, "id=");
  if (id_str == NULL)
    error ("require id argument in the string");
  id_str = strtok (id_str, "=");
  value_str = strtok (NULL, ";");
  value_str = strtok (value_str, ",");
  while (value_str != NULL)
    {
      int i;
      i = atoi (value_str);
      if (i < 1 || i > 8)
 error ("invalid id value for exception attribute");
      nds32_isr_vectors[i].category = NDS32_ISR_EXCEPTION;
      strcpy (nds32_isr_vectors[i].func_name, func_name);
      nds32_isr_vectors[i].save_reg = save_reg;
      nds32_isr_vectors[i].nested_type = nested_type;
      value_str = strtok (NULL, ",");
    }
  return;
}
static void
nds32_reset_attribute_parse_string (const char *original_str,
        const char *func_name)
{
  char target_str[100];
  char *vectors_str, *nmi_str, *warm_str, *value_str;
  nds32_isr_vectors[0].category = NDS32_ISR_RESET;
  strcpy (target_str, original_str);
  vectors_str = strstr (target_str, "vectors=");
  if (vectors_str != NULL)
    {
      vectors_str = strtok (vectors_str, "=");
      value_str = strtok (NULL, ";");
      nds32_isr_vectors[0].total_n_vectors = atoi (value_str) + 8 + 1;
    }
  else
    nds32_isr_vectors[0].total_n_vectors = 16 + 8 + 1;
  strcpy (nds32_isr_vectors[0].func_name, func_name);
  strcpy (target_str, original_str);
  nmi_str = strstr (target_str, "nmi_func=");
  if (nmi_str != NULL)
    {
      nmi_str = strtok (nmi_str, "=");
      value_str = strtok (NULL, ";");
      strcpy (nds32_isr_vectors[0].nmi_name, value_str);
    }
  strcpy (target_str, original_str);
  warm_str = strstr (target_str, "warm_func=");
  if (warm_str != NULL)
    {
      warm_str = strtok (warm_str, "=");
      value_str = strtok (NULL, ";");
      strcpy (nds32_isr_vectors[0].warm_name, value_str);
    }
  return;
}
static void
nds32_emit_section_head_template (char section_name[],
      char symbol_name[],
      int align_value,
      bool object_p)
{
  const char *flags_str;
  const char *type_str;
  flags_str = (object_p) ? "\"a\"" : "\"ax\"";
  type_str = (object_p) ? "@object" : "@function";
  fprintf (asm_out_file, "\t.section\t%s, %s\n", section_name, flags_str);
  fprintf (asm_out_file, "\t.align\t%d\n", align_value);
  fprintf (asm_out_file, "\t.global\t%s\n", symbol_name);
  fprintf (asm_out_file, "\t.type\t%s, %s\n", symbol_name, type_str);
  fprintf (asm_out_file, "%s:\n", symbol_name);
}
static void
nds32_emit_section_tail_template (char symbol_name[])
{
  fprintf (asm_out_file, "\t.size\t%s, .-%s\n", symbol_name, symbol_name);
}
static void
nds32_emit_isr_jmptbl_section (int vector_id)
{
  char section_name[100];
  char symbol_name[100];
  if (nds32_isr_vectors[vector_id].nested_type == NDS32_CRITICAL)
    {
      fprintf (asm_out_file, "\t! The vector %02d is a critical isr !\n",
        vector_id);
      return;
    }
  snprintf (section_name, sizeof (section_name),
     ".nds32_jmptbl.%02d", vector_id);
  snprintf (symbol_name, sizeof (symbol_name),
     "_nds32_jmptbl_%02d", vector_id);
  nds32_emit_section_head_template (section_name, symbol_name, 2, true);
  fprintf (asm_out_file, "\t.word\t%s\n",
    nds32_isr_vectors[vector_id].func_name);
  nds32_emit_section_tail_template (symbol_name);
}
static void
nds32_emit_isr_vector_section (int vector_id)
{
  unsigned int vector_number_offset = 0;
  const char *c_str = "CATEGORY";
  const char *sr_str = "SR";
  const char *nt_str = "NT";
  char first_level_handler_name[100];
  char section_name[100];
  char symbol_name[100];
  switch (nds32_isr_vectors[vector_id].category)
    {
    case NDS32_ISR_INTERRUPT:
      vector_number_offset = 9;
      c_str = "i";
      break;
    case NDS32_ISR_EXCEPTION:
      vector_number_offset = 0;
      c_str = "e";
      break;
    case NDS32_ISR_NONE:
    case NDS32_ISR_RESET:
      gcc_unreachable ();
      break;
    }
  switch (nds32_isr_vectors[vector_id].save_reg)
    {
    case NDS32_SAVE_ALL:
      sr_str = "sa";
      break;
    case NDS32_PARTIAL_SAVE:
      sr_str = "ps";
      break;
    }
  switch (nds32_isr_vectors[vector_id].nested_type)
    {
    case NDS32_NESTED:
      nt_str = "ns";
      break;
    case NDS32_NOT_NESTED:
      nt_str = "nn";
      break;
    case NDS32_NESTED_READY:
      nt_str = "nr";
      break;
    case NDS32_CRITICAL:
      nt_str = "";
      break;
    }
  snprintf (first_level_handler_name, sizeof (first_level_handler_name),
     "_nds32_%s_%s_%s", c_str, sr_str, nt_str);
  snprintf (section_name, sizeof (section_name),
     ".nds32_vector.%02d", vector_id);
  snprintf (symbol_name, sizeof (symbol_name),
     "_nds32_vector_%02d", vector_id);
  nds32_emit_section_head_template (section_name, symbol_name,
        floor_log2 (nds32_isr_vector_size), false);
  if (nds32_isr_vectors[vector_id].nested_type == NDS32_CRITICAL)
    {
      fprintf (asm_out_file, "\tj\t%s ! jump to user handler directly\n",
        nds32_isr_vectors[vector_id].func_name);
    }
  else if (nds32_isr_vector_size == 4)
    {
      fprintf (asm_out_file, "\tj\t%s ! jump to first level handler\n",
        first_level_handler_name);
    }
  else
    {
      if (TARGET_REDUCED_REGS)
 {
   if (nds32_isr_vectors[vector_id].save_reg == NDS32_SAVE_ALL)
     {
       fprintf (asm_out_file, "\t! reduced set regs + save_all\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r15, [$sp], $r15, 0xf\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r0, [$sp], $r10, 0x0\n");
     }
   else
     {
       fprintf (asm_out_file, "\t! reduced set regs + partial_save\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r15, [$sp], $r15, 0x2\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r0, [$sp], $r5, 0x0\n");
     }
 }
      else
 {
   if (nds32_isr_vectors[vector_id].save_reg == NDS32_SAVE_ALL)
     {
       fprintf (asm_out_file, "\t! full set regs + save_all\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r0, [$sp], $r27, 0xf\n");
     }
   else
     {
       fprintf (asm_out_file, "\t! full set regs + partial_save\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r15, [$sp], $r27, 0x2\n");
       fprintf (asm_out_file, "\tsmw.adm\t$r0, [$sp], $r5, 0x0\n");
     }
 }
      fprintf (asm_out_file, "\tmovi\t$r0, %d ! preparing software vid\n",
        vector_id - vector_number_offset);
      fprintf (asm_out_file, "\tj\t%s ! jump to first level handler\n",
        first_level_handler_name);
    }
  nds32_emit_section_tail_template (symbol_name);
}
static void
nds32_emit_isr_reset_content (void)
{
  unsigned int i;
  unsigned int total_n_vectors;
  char reset_handler_name[100];
  char section_name[100];
  char symbol_name[100];
  total_n_vectors = nds32_isr_vectors[0].total_n_vectors;
  fprintf (asm_out_file, "\t! RESET HANDLER CONTENT - BEGIN !\n");
  fprintf (asm_out_file, "\t.section\t.rodata\n");
  fprintf (asm_out_file, "\t.align\t2\n");
  fprintf (asm_out_file, "\t ! references to jmptbl section entries\n");
  for (i = 0; i < total_n_vectors; i++)
    fprintf (asm_out_file, "\t.word\t_nds32_jmptbl_%02d\n", i);
  fprintf (asm_out_file, "\t ! references to vector section entries\n");
  for (i = 0; i < total_n_vectors; i++)
    fprintf (asm_out_file, "\t.word\t_nds32_vector_%02d\n", i);
  snprintf (section_name, sizeof (section_name), ".nds32_jmptbl.00");
  snprintf (symbol_name, sizeof (symbol_name), "_nds32_jmptbl_00");
  fprintf (asm_out_file, "\t! ....................................\n");
  nds32_emit_section_head_template (section_name, symbol_name, 2, true);
  fprintf (asm_out_file, "\t.word\t%s\n",
    nds32_isr_vectors[0].func_name);
  nds32_emit_section_tail_template (symbol_name);
  snprintf (section_name, sizeof (section_name), ".nds32_vector.00");
  snprintf (symbol_name, sizeof (symbol_name), "_nds32_vector_00");
  snprintf (reset_handler_name, sizeof (reset_handler_name),
     "_nds32_reset");
  fprintf (asm_out_file, "\t! ....................................\n");
  nds32_emit_section_head_template (section_name, symbol_name,
        floor_log2 (nds32_isr_vector_size), false);
  fprintf (asm_out_file, "\tj\t%s ! jump to reset handler\n",
    reset_handler_name);
  nds32_emit_section_tail_template (symbol_name);
  snprintf (section_name, sizeof (section_name), ".nds32_nmih");
  snprintf (symbol_name, sizeof (symbol_name), "_nds32_nmih");
  fprintf (asm_out_file, "\t! ....................................\n");
  nds32_emit_section_head_template (section_name, symbol_name, 2, true);
  fprintf (asm_out_file, "\t.word\t%s\n",
    (strlen (nds32_isr_vectors[0].nmi_name) == 0)
    ? "0"
    : nds32_isr_vectors[0].nmi_name);
  nds32_emit_section_tail_template (symbol_name);
  snprintf (section_name, sizeof (section_name), ".nds32_wrh");
  snprintf (symbol_name, sizeof (symbol_name), "_nds32_wrh");
  fprintf (asm_out_file, "\t! ....................................\n");
  nds32_emit_section_head_template (section_name, symbol_name, 2, true);
  fprintf (asm_out_file, "\t.word\t%s\n",
    (strlen (nds32_isr_vectors[0].warm_name) == 0)
    ? "0"
    : nds32_isr_vectors[0].warm_name);
  nds32_emit_section_tail_template (symbol_name);
  fprintf (asm_out_file, "\t! RESET HANDLER CONTENT - END !\n");
}
void
nds32_check_isr_attrs_conflict (tree func_decl, tree func_attrs)
{
  int save_all_p, partial_save_p;
  int nested_p, not_nested_p, nested_ready_p, critical_p;
  int intr_p, excp_p, reset_p;
  save_all_p = partial_save_p = 0;
  nested_p = not_nested_p = nested_ready_p = critical_p = 0;
  intr_p = excp_p = reset_p = 0;
  if (lookup_attribute ("save_all", func_attrs))
    save_all_p = 1;
  if (lookup_attribute ("partial_save", func_attrs))
    partial_save_p = 1;
  if ((save_all_p + partial_save_p) > 1)
    error ("multiple save reg attributes to function %qD", func_decl);
  if (lookup_attribute ("nested", func_attrs))
    nested_p = 1;
  if (lookup_attribute ("not_nested", func_attrs))
    not_nested_p = 1;
  if (lookup_attribute ("nested_ready", func_attrs))
    nested_ready_p = 1;
  if (lookup_attribute ("critical", func_attrs))
    critical_p = 1;
  if ((nested_p + not_nested_p + nested_ready_p + critical_p) > 1)
    error ("multiple nested types attributes to function %qD", func_decl);
  if (lookup_attribute ("interrupt", func_attrs))
    intr_p = 1;
  if (lookup_attribute ("exception", func_attrs))
    excp_p = 1;
  if (lookup_attribute ("reset", func_attrs))
    reset_p = 1;
  if ((intr_p + excp_p + reset_p) > 1)
    error ("multiple interrupt attributes to function %qD", func_decl);
  if (TARGET_LINUX_ABI && intr_p)
      error ("cannot use interrupt attributes to function %qD "
      "under linux toolchain", func_decl);
  if (TARGET_LINUX_ABI && excp_p)
      error ("cannot use exception attributes to function %qD "
      "under linux toolchain", func_decl);
  if (TARGET_LINUX_ABI && reset_p)
      error ("cannot use reset attributes to function %qD "
      "under linux toolchain", func_decl);
}
void
nds32_construct_isr_vectors_information (tree func_attrs,
      const char *func_name)
{
  tree save_all, partial_save;
  tree nested, not_nested, nested_ready, critical;
  tree intr, excp, reset;
  save_all = lookup_attribute ("save_all", func_attrs);
  partial_save = lookup_attribute ("partial_save", func_attrs);
  nested = lookup_attribute ("nested", func_attrs);
  not_nested = lookup_attribute ("not_nested", func_attrs);
  nested_ready = lookup_attribute ("nested_ready", func_attrs);
  critical = lookup_attribute ("critical", func_attrs);
  intr = lookup_attribute ("interrupt", func_attrs);
  excp = lookup_attribute ("exception", func_attrs);
  reset = lookup_attribute ("reset", func_attrs);
  if (!intr && !excp && !reset)
    return;
  if (intr != NULL_TREE
      && TREE_CODE (TREE_VALUE (TREE_VALUE (intr))) == STRING_CST)
    {
      tree string_arg = TREE_VALUE (TREE_VALUE (intr));
      nds32_interrupt_attribute_parse_string (TREE_STRING_POINTER (string_arg),
           func_name);
      return;
    }
  if (excp != NULL_TREE
      && TREE_CODE (TREE_VALUE (TREE_VALUE (excp))) == STRING_CST)
    {
      tree string_arg = TREE_VALUE (TREE_VALUE (excp));
      nds32_exception_attribute_parse_string (TREE_STRING_POINTER (string_arg),
           func_name);
      return;
    }
  if (reset != NULL_TREE
      && TREE_CODE (TREE_VALUE (TREE_VALUE (reset))) == STRING_CST)
    {
      tree string_arg = TREE_VALUE (TREE_VALUE (reset));
      nds32_reset_attribute_parse_string (TREE_STRING_POINTER (string_arg),
       func_name);
      return;
    }
  if (intr || excp)
    {
      tree id_list;
      id_list = (intr) ? (TREE_VALUE (intr)) : (TREE_VALUE (excp));
      while (id_list)
 {
   tree id;
   int vector_id;
   unsigned int vector_number_offset;
   vector_number_offset = (intr) ? (9) : (0);
   id = TREE_VALUE (id_list);
   vector_id = TREE_INT_CST_LOW (id) + vector_number_offset;
   nds32_isr_vectors[vector_id].category = (intr)
        ? (NDS32_ISR_INTERRUPT)
        : (NDS32_ISR_EXCEPTION);
   strcpy (nds32_isr_vectors[vector_id].func_name, func_name);
   if (save_all)
     nds32_isr_vectors[vector_id].save_reg = NDS32_SAVE_ALL;
   else if (partial_save)
     nds32_isr_vectors[vector_id].save_reg = NDS32_PARTIAL_SAVE;
   if (nested)
     nds32_isr_vectors[vector_id].nested_type = NDS32_NESTED;
   else if (not_nested)
     nds32_isr_vectors[vector_id].nested_type = NDS32_NOT_NESTED;
   else if (nested_ready)
     nds32_isr_vectors[vector_id].nested_type = NDS32_NESTED_READY;
   else if (critical)
     nds32_isr_vectors[vector_id].nested_type = NDS32_CRITICAL;
   id_list = TREE_CHAIN (id_list);
 }
    }
  else
    {
      tree id_list;
      tree id;
      tree nmi, warm;
      nds32_isr_vectors[0].category = NDS32_ISR_RESET;
      id_list = TREE_VALUE (reset);
      id = TREE_VALUE (id_list);
      nds32_isr_vectors[0].total_n_vectors = TREE_INT_CST_LOW (id) + 8 + 1;
      strcpy (nds32_isr_vectors[0].func_name, func_name);
      nmi = lookup_attribute ("nmi", func_attrs);
      warm = lookup_attribute ("warm", func_attrs);
      if (nmi != NULL_TREE)
 {
   tree nmi_func_list;
   tree nmi_func;
   nmi_func_list = TREE_VALUE (nmi);
   nmi_func = TREE_VALUE (nmi_func_list);
   strcpy (nds32_isr_vectors[0].nmi_name,
    IDENTIFIER_POINTER (nmi_func));
 }
      if (warm != NULL_TREE)
 {
   tree warm_func_list;
   tree warm_func;
   warm_func_list = TREE_VALUE (warm);
   warm_func = TREE_VALUE (warm_func_list);
   strcpy (nds32_isr_vectors[0].warm_name,
    IDENTIFIER_POINTER (warm_func));
 }
    }
}
void
nds32_asm_file_start_for_isr (void)
{
  int i;
  for (i = 0; i < NDS32_N_ISR_VECTORS; i++)
    {
      nds32_isr_vectors[i].category = NDS32_ISR_NONE;
      strcpy (nds32_isr_vectors[i].func_name, "");
      nds32_isr_vectors[i].save_reg = NDS32_PARTIAL_SAVE;
      nds32_isr_vectors[i].nested_type = NDS32_NOT_NESTED;
      nds32_isr_vectors[i].total_n_vectors = 0;
      strcpy (nds32_isr_vectors[i].nmi_name, "");
      strcpy (nds32_isr_vectors[i].warm_name, "");
    }
}
void nds32_asm_file_end_for_isr (void)
{
  int i;
  for (i = 0; i < NDS32_N_ISR_VECTORS; i++)
    if (nds32_isr_vectors[i].category != NDS32_ISR_NONE)
      break;
  if (i == NDS32_N_ISR_VECTORS)
    return;
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  fprintf (asm_out_file, "\t! The isr vector information:\n");
  fprintf (asm_out_file, "\t! ------------------------------------\n");
  if (nds32_isr_vectors[0].category == NDS32_ISR_RESET)
    {
      nds32_emit_isr_reset_content ();
      fprintf (asm_out_file, "\t! ------------------------------------\n");
    }
  for (i = 1; i < NDS32_N_ISR_VECTORS; i++)
    {
      if (nds32_isr_vectors[i].category == NDS32_ISR_INTERRUPT
   || nds32_isr_vectors[i].category == NDS32_ISR_EXCEPTION)
 {
   fprintf (asm_out_file, "\t! interrupt/exception vector %02d\n", i);
   fprintf (asm_out_file, "\t! ------------------------------------\n");
   nds32_emit_isr_jmptbl_section (i);
   fprintf (asm_out_file, "\t! ....................................\n");
   nds32_emit_isr_vector_section (i);
   fprintf (asm_out_file, "\t! ------------------------------------\n");
 }
    }
}
bool
nds32_isr_function_p (tree func)
{
  tree t_intr;
  tree t_excp;
  tree t_reset;
  tree attrs;
  if (TREE_CODE (func) != FUNCTION_DECL)
    abort ();
  attrs = DECL_ATTRIBUTES (func);
  t_intr = lookup_attribute ("interrupt", attrs);
  t_excp = lookup_attribute ("exception", attrs);
  t_reset = lookup_attribute ("reset", attrs);
  return ((t_intr != NULL_TREE)
   || (t_excp != NULL_TREE)
   || (t_reset != NULL_TREE));
}
bool
nds32_isr_function_critical_p (tree func)
{
  tree t_intr;
  tree t_excp;
  tree t_critical;
  tree attrs;
  if (TREE_CODE (func) != FUNCTION_DECL)
    abort ();
  attrs = DECL_ATTRIBUTES (func);
  t_intr = lookup_attribute ("interrupt", attrs);
  t_excp = lookup_attribute ("exception", attrs);
  t_critical = lookup_attribute ("critical", attrs);
  if ((t_intr == NULL_TREE) && (t_excp == NULL_TREE))
    return false;
  if (t_critical != NULL_TREE)
    return true;
  if (t_intr != NULL_TREE || t_excp != NULL_TREE)
    {
      char target_str[100];
      char *critical_str;
      tree t_check;
      tree string_arg;
      t_check = t_intr ? t_intr : t_excp;
      if (TREE_CODE (TREE_VALUE (TREE_VALUE (t_check))) == STRING_CST)
 {
   string_arg = TREE_VALUE (TREE_VALUE (t_check));
   strcpy (target_str, TREE_STRING_POINTER (string_arg));
   critical_str = strstr (target_str, "critical");
   if (critical_str)
     return true;
 }
    }
  return false;
}
