(define_automaton "makatau")

(define_cpu_unit "makatau_pipe0" "makatau")
(define_cpu_unit "makatau_pipe1" "makatau")
(define_cpu_unit "makatau_pipe2" "makatau")
(define_cpu_unit "makatau_pipe3" "makatau")

(define_cpu_unit "makatau_alu0,makatau_alu1,makatau_alu2,makatau_alu3" "makatau")
(define_cpu_unit "makatau_lsu0,makatau_lsu1" "makatau")
(define_cpu_unit "makatau_dsp" "makatau")
(define_cpu_unit "makatau_fpu0,makatau_fpu1" "makatau")

(define_reservation "makatau_pipe"
 "makatau_pipe0 | makatau_pipe1 | makatau_pipe2 | makatau_pipe3")

(define_reservation "makatau_alu"
 "makatau_alu0 | makatau_alu1 | makatau_alu2 | makatau_alu3")
 
(define_reservation "makatau_alu_crypto"
 "makatau_alu0 | makatau_alu1")

(define_reservation "makatau_lsu"
 "makatau_lsu0 | makatau_lsu1 ")

(define_reservation "makatau_fpu"
 "makatau_fpu0 | makatau_fpu1 ")

(define_insn_reservation "makatau_alu_insn" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "shift,nop,logical,unknown,
                        const,arith,multi,slt,move"))
  "makatau_pipe + makatau_alu")

(define_insn_reservation "makatau_load_wd" 3
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "load"))
  "makatau_pipe + makatau_lsu")

(define_insn_reservation "makatau_store" 0
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "store"))
  "makatau_pipe + makatau_lsu")

(define_insn_reservation "makatau_branch" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "branch,jump,call,branch_imm"))
  "makatau_pipe + makatau_alu3")

(define_insn_reservation "makatau_imul" 3
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "imul"))
  "makatau_pipe + makatau_alu3")

(define_insn_reservation "makatau_idivsi" 38
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "makatau_pipe + makatau_alu3")

(define_insn_reservation "makatau_idivdi" 70
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "makatau_pipe + makatau_alu3")

(define_insn_reservation "makatau_xfer" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "mfc,mtc"))
  "makatau_pipe + makatau_alu")

(define_insn_reservation "makatau_dsp_alu" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dalu"))
  "makatau_pipe0 + makatau_dsp| makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_alu64" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dalu64"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_alu_round" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "daluround"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_cmp" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dcmp"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_clip" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dclip"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_mul" 2
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dmul"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_mac" 2
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dmac"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_insb" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dinsb"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_pack" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dpack"))
  "makatau_pipe0 | makatau_pipe1")

(define_insn_reservation "makatau_dsp_bpick" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dbpick"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_dsp_wext" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "dwext"))
  "makatau_pipe0 + makatau_dsp | makatau_pipe1 + makatau_dsp")

(define_insn_reservation "makatau_fpu_alu" 2
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fadd")
            (eq_attr "mode" "SF,DF")))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_mul" 3
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fmul")
            (eq_attr "mode" "SF,DF")))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_mac" 4
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fmadd")
            (eq_attr "mode" "SF")))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_div_h" 5
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fdiv")
            (eq_attr "mode" "HF")))
  "makatau_pipe + makatau_fpu1")

(define_insn_reservation "makatau_fpu_div_s" 8
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fdiv")
            (eq_attr "mode" "SF")))
  "makatau_pipe + makatau_fpu1")

(define_insn_reservation "makatau_fpu_div_d" 12
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fdiv")
            (eq_attr "mode" "DF")))
  "makatau_pipe + makatau_fpu1")
  
(define_insn_reservation "makatau_fpu_sqrt_h" 6
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fsqrt")
            (eq_attr "mode" "HF")))
  "makatau_pipe + makatau_fpu1")

(define_insn_reservation "makatau_fpu_sqrt_s" 10
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fsqrt")
            (eq_attr "mode" "SF")))
  "makatau_pipe + makatau_fpu1")

(define_insn_reservation "makatau_fpu_sqrt_d" 17
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "type" "fsqrt")
            (eq_attr "mode" "DF")))
  "makatau_pipe + makatau_fpu1")

(define_insn_reservation "makatau_fpu_move" 2
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "fmove,mtc,mfc"))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_output_move" 6
  (and (eq_attr "tune" "makatau")
       (and (eq_attr "move_type" "fmove")
            (eq_attr "mode" "BF,HF,SF,DF")))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_cmp" 2
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "fcmp"))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_cvt" 6
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "fcvt"))
  "makatau_pipe + makatau_fpu")

(define_insn_reservation "makatau_fpu_load" 4
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "fpload"))
  "makatau_pipe + makatau_lsu")

(define_insn_reservation "makatau_fpu_store" 0
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "fpstore"))
  "makatau_pipe + makatau_lsu")

(define_insn_reservation "makatau_bitmanip" 1
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "bitmanip"))
  "makatau_pipe + makatau_alu")

(define_insn_reservation "makatau_crypto" 2
  (and (eq_attr "tune" "makatau")
       (eq_attr "type" "crypto"))
  "makatau_pipe + makatau_alu_crypto")
