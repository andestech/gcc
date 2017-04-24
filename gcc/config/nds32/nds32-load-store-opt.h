#ifndef NDS32_LOAD_STORE_OPT_H
#define NDS32_LOAD_STORE_OPT_H 
typedef struct {
  rtx reg;
  rtx base_reg;
  rtx offset;
  HOST_WIDE_INT shift;
  bool load_p;
  rtx insn;
} rr_load_store_info_t;
typedef struct {
  rtx reg;
  rtx base_reg;
  HOST_WIDE_INT offset;
  bool load_p;
  rtx insn;
  rtx mem;
} load_store_info_t;
typedef struct {
  HOST_WIDE_INT max_offset;
  HOST_WIDE_INT min_offset;
  int num_offset;
} offset_info_t;
typedef struct {
  rtx set_insns[2];
  int n_set_insns;
  rtx reg;
  bool need_adjust_offset_p;
  HOST_WIDE_INT adjust_offset;
} new_base_reg_info_t;
typedef auto_vec<load_store_info_t, 64> load_store_infos_t;
class load_store_optimize_pass
{
public:
  load_store_optimize_pass (enum reg_class,
       enum reg_class,
       HOST_WIDE_INT,
       HOST_WIDE_INT,
       bool,
       const char *);
  const char *name () const { return m_name; };
  int calc_gain (HARD_REG_SET *,
   offset_info_t,
   load_store_infos_t *) const;
  void do_optimize (HARD_REG_SET *,
      offset_info_t,
      load_store_infos_t *) const;
private:
  enum reg_class m_allow_regclass;
  enum reg_class m_new_base_regclass;
  HOST_WIDE_INT m_offset_lower_bound;
  HOST_WIDE_INT m_offset_upper_bound;
  bool m_load_only_p;
  const char *m_name;
};
static inline bool
in_reg_class_p (unsigned regno, enum reg_class clazz)
{
  return TEST_HARD_REG_BIT (reg_class_contents[clazz], regno);
}
static inline bool
in_reg_class_p (rtx reg, enum reg_class clazz)
{
  gcc_assert (REG_P (reg));
  return in_reg_class_p (REGNO (reg), clazz);
}
static inline rtx
gen_reg_plus_imm_load_store (rtx reg, rtx base_reg,
        HOST_WIDE_INT offset, bool load_p, rtx oldmem)
{
  rtx addr = plus_constant(Pmode, base_reg, offset);
  rtx mem = gen_rtx_MEM (SImode, addr);
  MEM_COPY_ATTRIBUTES (mem, oldmem);
  if (load_p)
    return gen_movsi (reg, mem);
  else
    return gen_movsi (mem, reg);
}
static inline unsigned
find_available_reg (HARD_REG_SET *available_regset, enum reg_class clazz)
{
  hard_reg_set_iterator hrsi;
  unsigned regno;
  EXECUTE_IF_SET_IN_HARD_REG_SET (reg_class_contents[clazz], 0, regno, hrsi)
    {
      if (TEST_HARD_REG_BIT (*available_regset, regno)
   && (call_used_regs[regno] || df_regs_ever_live_p (regno)))
 return regno;
    }
  return INVALID_REGNUM;
}
#endif
