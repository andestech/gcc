(define_automaton "kavalan")

(define_cpu_unit "kavalan_pipe0" "kavalan")
(define_cpu_unit "kavalan_pipe1" "kavalan")

(define_cpu_unit "kavalan_mdu,kavalan_alu0,kavalan_alu1,kavalan_bru0,kavalan_bru1,kavalan_lsu" "kavalan")
(define_cpu_unit "kavalan_dsp" "kavalan")
(define_cpu_unit "kavalan_fpu_fmac,kavalan_fpu_fdiv,kavalan_fpu_fmis,kavalan_fpu_fmv" "kavalan")

(define_reservation "kavalan_fpu_arith"
 "kavalan_pipe0+kavalan_fpu_fmac | kavalan_pipe1+kavalan_fpu_fmac")

(define_insn_reservation "kavalan_alu_insn_s" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "shift,nop,logical"))
  "kavalan_pipe0+kavalan_alu0 | kavalan_pipe1+kavalan_alu1")
  
(define_insn_reservation "kavalan_alu_insn_l" 2
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "unknown,const,arith,multi,slt,move,cmov"))
  "kavalan_pipe0+kavalan_alu0 | kavalan_pipe1+kavalan_alu1")

(define_insn_reservation "kavalan_load_wd" 3
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "SI,DI")))
  "kavalan_pipe0 + kavalan_lsu | kavalan_pipe1 + kavalan_lsu")

(define_insn_reservation "kavalan_load_bh" 4
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "QI,HI")))
  "kavalan_pipe0 + kavalan_lsu | kavalan_pipe1 + kavalan_lsu")

(define_insn_reservation "kavalan_store" 0
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "store"))
  "kavalan_pipe0 + kavalan_lsu | kavalan_pipe1 + kavalan_lsu")

(define_insn_reservation "kavalan_branch" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "branch,jump,call,branch_imm"))
  "kavalan_pipe0 + kavalan_bru0 | kavalan_pipe1 + kavalan_bru1")

(define_insn_reservation "kavalan_imul" 10
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "imul"))
  "kavalan_pipe0 +kavalan_alu0 | kavalan_pipe1 +kavalan_alu1 , kavalan_mdu*2")

(define_insn_reservation "kavalan_idivsi" 38
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "kavalan_pipe0 +kavalan_alu0 | kavalan_pipe1 +kavalan_alu1 , kavalan_mdu*2")

(define_insn_reservation "kavalan_idivdi" 70
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "kavalan_pipe0  +kavalan_alu0 | kavalan_pipe1 +kavalan_alu1 , kavalan_mdu*2")

(define_insn_reservation "kavalan_xfer" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "mfc,mtc"))
  "kavalan_pipe0+kavalan_alu0 | kavalan_pipe1+kavalan_alu1")

(define_insn_reservation "kavalan_dsp_alu" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dalu"))
  "kavalan_pipe0 + kavalan_dsp| kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_alu64" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dalu64"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_alu_round" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "daluround"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_cmp" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dcmp"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_clip" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dclip"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_mul" 2
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dmul"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_mac" 2
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dmac"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_insb" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dinsb"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_pack" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dpack"))
  "kavalan_pipe0 | kavalan_pipe1")

(define_insn_reservation "kavalan_dsp_bpick" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dbpick"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_dsp_wext" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "dwext"))
  "kavalan_pipe0 + kavalan_dsp | kavalan_pipe1 + kavalan_dsp")

(define_insn_reservation "kavalan_fpu_alu_s" 3
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "fadd")
            (eq_attr "mode" "SF")))
  "kavalan_fpu_arith")

(define_insn_reservation "kavalan_fpu_alu_d" 4
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "fadd")
            (eq_attr "mode" "DF")))
  "kavalan_fpu_arith")

(define_insn_reservation "kavalan_fpu_mul_s" 3
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "fmul")
            (eq_attr "mode" "SF")))
  "kavalan_fpu_arith")

(define_insn_reservation "kavalan_fpu_mul_d" 4
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "fmul")
            (eq_attr "mode" "DF")))
  "kavalan_fpu_arith")

(define_insn_reservation "kavalan_fpu_mac_s" 3
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "fmadd")
            (eq_attr "mode" "SF")))
  "kavalan_pipe1+kavalan_fpu_fmac")

(define_insn_reservation "kavalan_fpu_mac_d" 4
  (and (eq_attr "tune" "kavalan")
       (and (eq_attr "type" "fmadd")
            (eq_attr "mode" "DF")))
  "kavalan_pipe1+kavalan_fpu_fmac")

(define_insn_reservation "kavalan_fpu_div" 33
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fdiv"))
  "kavalan_pipe0+kavalan_fpu_fdiv | kavalan_pipe1+kavalan_fpu_fdiv, kavalan_fpu_fdiv * 27")

(define_insn_reservation "kavalan_fpu_sqrt" 33
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fsqrt"))
  "kavalan_pipe0+kavalan_fpu_fdiv | kavalan_pipe1+kavalan_fpu_fdiv, kavalan_fpu_fdiv * 27")

(define_insn_reservation "kavalan_fpu_move" 1
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fmove,mtc,mfc"))
  "kavalan_pipe0+kavalan_fpu_fmv | kavalan_pipe1+kavalan_fpu_fmv")

(define_insn_reservation "kavalan_fpu_cmp" 2
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fcmp"))
  "kavalan_pipe0+kavalan_fpu_fmis | kavalan_pipe1+kavalan_fpu_fmis")

(define_insn_reservation "kavalan_fpu_cvt" 2
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fcvt"))
  "kavalan_pipe0+kavalan_fpu_fmis | kavalan_pipe1+kavalan_fpu_fmis")

(define_insn_reservation "kavalan_fpu_load" 3
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fpload"))
  "kavalan_pipe0 + kavalan_lsu | kavalan_pipe1 + kavalan_lsu")

(define_insn_reservation "kavalan_fpu_store" 0
  (and (eq_attr "tune" "kavalan")
       (eq_attr "type" "fpstore"))
  "kavalan_pipe0 + kavalan_lsu | kavalan_pipe1 + kavalan_lsu")
