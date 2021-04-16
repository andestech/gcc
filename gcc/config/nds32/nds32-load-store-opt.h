/* Prototypes for load-store-opt of Andes NDS32 cpu for GNU compiler
   Copyright (C) 2012-2015 Free Software Foundation, Inc.
   Contributed by Andes Technology Corporation.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef NDS32_LOAD_STORE_OPT_H
#define NDS32_LOAD_STORE_OPT_H

/* Define the type of a set of hard registers.  */

enum nds32_memory_post_type
{
  NDS32_NONE,
  NDS32_POST_INC,
  NDS32_POST_DEC
};

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
  int new_reg;
  int order;
  int group;
  bool place;
  enum nds32_memory_post_type post_type;
} load_store_info_t;

typedef struct {
  HOST_WIDE_INT max_offset;
  HOST_WIDE_INT min_offset;
  /* How many different offset.  */
  int num_offset;
} offset_info_t;

typedef struct {
  rtx set_insns[2];
  int n_set_insns;
  rtx reg;
  bool need_adjust_offset_p;
  HOST_WIDE_INT adjust_offset;
} new_base_reg_info_t;

typedef struct {
  unsigned int amount;
  unsigned int start;
  unsigned int end;
} available_reg_info_t;

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

#endif /* ! NDS32_LOAD_STORE_OPT_H */
