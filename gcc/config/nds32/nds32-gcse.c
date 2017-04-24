#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "diagnostic-core.h"
#include "hash-table.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "flags.h"
#include "insn-config.h"
#include "recog.h"
#include "basic-block.h"
#include "function.h"
#include "expr.h"
#include "except.h"
#include "intl.h"
#include "obstack.h"
#include "hashtab.h"
#include "params.h"
#include "target.h"
#include "tree-pass.h"
#include "dbgcnt.h"
#include "df.h"
#include "reload.h"
struct expr
{
  rtx expr;
  hashval_t hash;
  struct occr *antic_occr;
  unsigned int count;
};
struct occr
{
  struct occr *next;
  rtx insn;
  char deleted_p;
};
struct reg_avail_info
{
  basic_block last_bb;
  int first_set;
  int first_use;
};
struct expr_hasher : typed_noop_remove <expr>
{
  typedef expr value_type;
  typedef expr compare_type;
  static inline hashval_t hash (const value_type *);
  static inline bool equal (const value_type *, const compare_type *);
};
inline hashval_t
expr_hasher::hash (const value_type *exp)
{
  return exp->hash;
}
inline bool
expr_hasher::equal (const value_type *exp1, const compare_type *exp2)
{
  int equiv_p = exp_equiv_p (exp1->expr, exp2->expr, 0, true);
  gcc_assert (!equiv_p || exp1->hash == exp2->hash);
  return equiv_p;
}
static hashval_t
hash_expr (rtx x, int *do_not_record_p)
{
  *do_not_record_p = 0;
  return hash_rtx (x, GET_MODE (x), do_not_record_p,
     NULL, false);
}
static void alloc_mem (void);
static void free_mem (void);
static void compute_hash_table (void);
static void hash_scan_set (rtx);
static void insert_expr_in_table (rtx, rtx);
static void dump_hash_table (FILE *);
static struct obstack expr_obstack;
static hash_table <expr_hasher> expr_table;
static struct reg_avail_info *reg_avail_info;
static sbitmap *hoist_vbein;
static sbitmap *hoist_vbeout;
static void
alloc_mem (void)
{
  expr_table.create (MAX (get_max_insn_count () / 4, 13));
  gcc_obstack_init (&expr_obstack);
}
static void
free_mem (void)
{
  expr_table.dispose ();
  obstack_free (&expr_obstack, NULL);
}
int
nds32_dump_expr_hash_table_entry (expr **slot, FILE *file)
{
  struct expr *exprs = *slot;
  struct occr *occr;
  fprintf (file, "expr: ");
  print_rtl (file, exprs->expr);
  fprintf (file,"\nhashcode: %u\n", exprs->hash);
  fprintf (file,"list of occurrences:\n");
  occr = exprs->antic_occr;
  while (occr)
    {
      rtx insn = occr->insn;
      print_rtl_single (file, insn);
      fprintf (file, "\n");
      occr = occr->next;
    }
  fprintf (file, "\n");
  return 1;
}
static void
dump_hash_table (FILE *file)
{
  fprintf (file, "\n\nexpression hash table\n");
  fprintf (file, "size %ld, %ld elements, %f collision/search ratio\n",
    (long) expr_table.size (),
    (long) expr_table.elements (),
    expr_table.collisions ());
  if (expr_table.elements () > 0)
    {
      fprintf (file, "\n\ntable entries:\n");
      expr_table.traverse <FILE *, nds32_dump_expr_hash_table_entry> (file);
    }
  fprintf (file, "\n");
}
static void
insert_expr_in_table (rtx x, rtx insn)
{
  int do_not_record_p;
  hashval_t hash;
  struct expr *cur_expr, **slot;
  struct occr *antic_occr, *last_occr = NULL;
  hash = hash_expr (x, &do_not_record_p);
  if (do_not_record_p)
    return;
  cur_expr = (struct expr *) obstack_alloc (&expr_obstack,
         sizeof (struct expr));
  cur_expr->expr = x;
  cur_expr->hash = hash;
  cur_expr->antic_occr = NULL;
  slot = expr_table.find_slot_with_hash (cur_expr, hash, INSERT);
  if (! (*slot))
    *slot = cur_expr;
  else
    {
      obstack_free (&expr_obstack, cur_expr);
      cur_expr = *slot;
    }
  antic_occr = cur_expr->antic_occr;
  cur_expr->count++;
  while (antic_occr
  && BLOCK_FOR_INSN (antic_occr->insn) != BLOCK_FOR_INSN (insn))
    {
      last_occr = antic_occr;
      antic_occr = antic_occr->next;
    }
  if (antic_occr)
    antic_occr->insn = insn;
  else
    {
      antic_occr = (struct occr *) obstack_alloc (&expr_obstack,
        sizeof (struct occr));
      if (cur_expr->antic_occr == NULL)
 cur_expr->antic_occr = antic_occr;
      else
 last_occr->next = antic_occr;
      antic_occr->insn = insn;
      antic_occr->next = NULL;
      antic_occr->deleted_p = 0;
    }
}
static void
hash_scan_set (rtx insn)
{
  rtx pat = PATTERN (insn);
  rtx src = SET_SRC (pat);
  rtx dest = SET_DEST (pat);
  int regno;
  struct reg_avail_info *info;
  if (JUMP_P (insn) || set_noop_p (pat))
    return;
  if (REG_P (dest) && REGNO (dest) <= SP_REGNUM)
    {
      regno = REGNO (dest);
      info = &reg_avail_info[regno];
      if (BLOCK_FOR_INSN (insn) == info->last_bb
   && info->first_set == DF_INSN_LUID (insn)
   && info->first_use >= info->first_set)
 {
   if (CONST_INT_P (src) || CONST_DOUBLE_P (src))
     insert_expr_in_table (PATTERN (insn), insn);
 }
    }
}
static void
record_first_reg_use_info (rtx insn, int regno)
{
  struct reg_avail_info *info = &reg_avail_info[regno];
  int luid = DF_INSN_LUID (insn);
  if (info->last_bb != BLOCK_FOR_INSN (insn))
    {
      info->last_bb = BLOCK_FOR_INSN (insn);
      info->first_use = luid;
      info->first_set = luid + 1;
    }
}
static void
record_first_use_info (rtx *dest, void *data)
{
  rtx last_set_insn = (rtx) data;
  int i, j;
  enum rtx_code code;
  const char *fmt;
  rtx x = *dest;
  if (x == 0)
    return;
  code = GET_CODE (x);
  if (REG_P (x) && REGNO (x) <= SP_REGNUM)
    {
      record_first_reg_use_info (last_set_insn, REGNO (x));
      if (GET_MODE_SIZE (GET_MODE (x)) == 8)
 record_first_reg_use_info (last_set_insn, REGNO (x) + 1);
    }
  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
 record_first_use_info (&XEXP (x, i), data);
      else if (fmt[i] == 'E')
 for (j = 0; j < XVECLEN (x, i); j++)
   record_first_use_info (&XVECEXP (x, i, j), data);
    }
}
static void
record_first_reg_set_info (rtx insn, int regno)
{
  struct reg_avail_info *info = &reg_avail_info[regno];
  int luid = DF_INSN_LUID (insn);
  if (info->last_bb != BLOCK_FOR_INSN (insn))
    {
      info->last_bb = BLOCK_FOR_INSN (insn);
      info->first_set = luid;
      info->first_use = luid + 1;
    }
}
static void
record_first_set_info (rtx dest, const_rtx setter ATTRIBUTE_UNUSED, void *data)
{
  rtx last_set_insn = (rtx) data;
  if (GET_CODE (dest) == SUBREG)
    dest = SUBREG_REG (dest);
  if (REG_P (dest) && REGNO (dest) <= SP_REGNUM)
    {
      record_first_reg_set_info (last_set_insn, REGNO (dest));
      if (GET_MODE_SIZE (GET_MODE (dest)) == 8)
 record_first_reg_set_info (last_set_insn, REGNO (dest) + 1);
    }
}
static void
compute_hash_table (void)
{
  basic_block bb;
  int i;
  reg_avail_info =
    (struct reg_avail_info *) xmalloc (sizeof (struct reg_avail_info) *
           (SP_REGNUM + 1));
  for (i = 0; i < 32; i++)
    reg_avail_info[i].last_bb = NULL;
  FOR_EACH_BB_FN (bb, cfun)
    {
      rtx insn;
      if (EDGE_COUNT (bb->preds) > 1)
 continue;
      FOR_BB_INSNS (bb, insn)
 {
   if (!NONDEBUG_INSN_P (insn))
     continue;
   if (CALL_P (insn))
     {
       for (i = 0; i <= SP_REGNUM; i++)
  if (call_used_regs[i])
    record_first_reg_use_info (insn, i);
     }
   note_uses (&PATTERN (insn), record_first_use_info, insn);
   note_stores (PATTERN (insn), record_first_set_info, insn);
 }
      FOR_BB_INSNS (bb, insn)
 if (INSN_P (insn) && GET_CODE (PATTERN (insn)) == SET)
   hash_scan_set (insn);
    }
}
int
nds32_find_gcse_expr_table (expr **slot, void *data ATTRIBUTE_UNUSED)
{
  struct expr *exprs = *slot;
  struct occr *occr;
  rtx insn;
  rtx last_insn;
  basic_block bb;
  edge e;
  unsigned ix;
  unsigned emit_done;
  unsigned cover, regno;
  df_ref *use_rec;
  enum machine_mode mode;
  if (exprs->count < 2)
    return 1;
  bitmap_vector_clear (hoist_vbeout, last_basic_block_for_fn (cfun));
  bitmap_vector_clear (hoist_vbein, last_basic_block_for_fn (cfun));
  occr = exprs->antic_occr;
  while (occr)
    {
      insn = occr->insn;
      bb = BLOCK_FOR_INSN (insn);
      if (!occr->deleted_p)
 bitmap_set_bit (hoist_vbein[bb->index], 0);
      occr = occr->next;
    }
  FOR_EACH_BB_REVERSE_FN (bb, cfun)
    {
      if (bb->next_bb != EXIT_BLOCK_PTR_FOR_FN (cfun))
 bitmap_intersection_of_succs (hoist_vbeout[bb->index], hoist_vbein, bb);
      if (bitmap_bit_p (hoist_vbeout[bb->index], 0)
   && EDGE_COUNT (bb->succs) > 1)
 {
   emit_done = 0;
   cover = FALSE;
   for (e = NULL, ix = 0; ix < EDGE_COUNT (bb->succs); ix++)
     {
       e = EDGE_SUCC (bb, ix);
       if (e->dest == EXIT_BLOCK_PTR_FOR_FN (cfun))
  continue;
       occr = exprs->antic_occr;
       while (occr)
  {
    insn = occr->insn;
    if (!occr->deleted_p && e->dest == BLOCK_FOR_INSN (insn))
      break;
    occr = occr->next;
  }
       if (!emit_done)
  {
    last_insn = BB_END (bb);
    regno = REGNO (SET_DEST (PATTERN (insn)));
    mode = GET_MODE (SET_DEST (PATTERN (insn)));
    for (use_rec = DF_INSN_USES (last_insn); *use_rec; use_rec++)
      {
        if (DF_REF_REGNO (*use_rec) == regno
     || regno_clobbered_p (regno, last_insn, mode, 2))
   {
     cover = TRUE;
     break;
   }
      }
    if (cover)
      break;
    else if (JUMP_P (last_insn))
      {
        emit_insn_before_noloc (PATTERN (insn), last_insn, bb);
        emit_done = TRUE;
      }
    else
      break;
  }
       if (emit_done)
  {
    delete_insn (insn);
    occr->deleted_p = TRUE;
  }
     }
 }
    }
  return 1;
}
static int
hoist_code (void)
{
  hoist_vbein = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), 1);
  hoist_vbeout = sbitmap_vector_alloc (last_basic_block_for_fn (cfun), 1);
  expr_table.traverse <void *, nds32_find_gcse_expr_table> (NULL);
  sbitmap_vector_free (hoist_vbein);
  sbitmap_vector_free (hoist_vbeout);
  return 0;
}
static unsigned int
nds32_gcse_opt (void)
{
  if (n_basic_blocks_for_fn (cfun) <= NUM_FIXED_BLOCKS + 1)
    return 0;
  alloc_mem ();
  df_chain_add_problem (DF_DU_CHAIN);
  df_insn_rescan_all ();
  df_analyze ();
  compute_hash_table ();
  if (dump_file)
    dump_hash_table (dump_file);
  hoist_code ();
  df_insn_rescan_all ();
  free_mem ();
  return 0;
}
const pass_data pass_data_nds32_gcse_opt =
{
  RTL_PASS,
  "gcse_opt",
  OPTGROUP_NONE,
  true,
  true,
  TV_MACH_DEP,
  0,
  0,
  0,
  0,
  (TODO_df_finish | TODO_verify_rtl_sharing),
};
class pass_nds32_gcse_opt : public rtl_opt_pass
{
public:
  pass_nds32_gcse_opt (gcc::context *ctxt)
    : rtl_opt_pass (pass_data_nds32_gcse_opt, ctxt)
  {}
  bool gate () { return TARGET_GCSE_OPT; }
  unsigned int execute () { return nds32_gcse_opt (); }
};
rtl_opt_pass *
make_pass_nds32_gcse_opt (gcc::context *ctxt)
{
  return new pass_nds32_gcse_opt (ctxt);
}
