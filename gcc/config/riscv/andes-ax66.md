;;This is a variant of kavalan scheduling model.
;;It doesn't represent the actual architecture of andes_ax66
(define_automaton "andes_ax66, andes_ax66_vector")

(define_cpu_unit "andes_ax66_pipe0" "andes_ax66")
(define_cpu_unit "andes_ax66_pipe1" "andes_ax66")
(define_cpu_unit "andes_ax66_vpu_pipe0" "andes_ax66_vector")
(define_cpu_unit "andes_ax66_vpu_pipe1" "andes_ax66_vector")

(define_reservation "andes_ax66_vpu_pipe" "(andes_ax66_vpu_pipe0 + andes_ax66_pipe0 | andes_ax66_vpu_pipe1 + andes_ax66_pipe1)")

(define_cpu_unit "andes_ax66_mdu,andes_ax66_alu0,andes_ax66_alu1,andes_ax66_bru0,andes_ax66_bru1,andes_ax66_lsu" "andes_ax66")
(define_cpu_unit "andes_ax66_dsp" "andes_ax66")
(define_cpu_unit "andes_ax66_fpu_fmac,andes_ax66_fpu_fdiv,andes_ax66_fpu_fmis,andes_ax66_fpu_fmv" "andes_ax66")
(define_cpu_unit "andes_ax66_vpu_alu,andes_ax66_vpu_mac,andes_ax66_vpu_fmis,andes_ax66_vpu_permut,
                  andes_ax66_vpu_div,andes_ax66_vpu_fdiv,andes_ax66_vpu_mask,andes_ax66_vpu_lsu" "andes_ax66_vector")
                  
(define_reservation "andes_ax66_fpu_arith"
 "andes_ax66_pipe0+andes_ax66_fpu_fmac | andes_ax66_pipe1+andes_ax66_fpu_fmac")

(define_insn_reservation "andes_ax66_alu_insn_s" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "shift,nop,logical"))
  "andes_ax66_pipe0+andes_ax66_alu0 | andes_ax66_pipe1+andes_ax66_alu1")
  
(define_insn_reservation "andes_ax66_alu_insn_l" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "unknown,const,arith,multi,slt,move,cmov"))
  "andes_ax66_pipe0+andes_ax66_alu0 | andes_ax66_pipe1+andes_ax66_alu1")

(define_insn_reservation "andes_ax66_load_wd" 4
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "load")
            (not (eq_attr "mode" "QI,HI"))))
  "andes_ax66_pipe0 + andes_ax66_lsu | andes_ax66_pipe1 + andes_ax66_lsu")

(define_insn_reservation "andes_ax66_load_bh" 4
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "load")
            (eq_attr "mode" "QI,HI")))
  "andes_ax66_pipe0 + andes_ax66_lsu | andes_ax66_pipe1 + andes_ax66_lsu")

(define_insn_reservation "andes_ax66_store" 0
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "store"))
  "andes_ax66_pipe0 + andes_ax66_lsu | andes_ax66_pipe1 + andes_ax66_lsu")

(define_insn_reservation "andes_ax66_branch" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "branch,jump,call,branch_imm"))
  "andes_ax66_pipe0 + andes_ax66_bru0 | andes_ax66_pipe1 + andes_ax66_bru1")

(define_insn_reservation "andes_ax66_imul" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "imul"))
  "andes_ax66_pipe0 +andes_ax66_alu0 | andes_ax66_pipe1 +andes_ax66_alu1 , andes_ax66_mdu*2")

(define_insn_reservation "andes_ax66_idivsi" 38
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "SI")))
  "andes_ax66_pipe0 +andes_ax66_alu0 | andes_ax66_pipe1 +andes_ax66_alu1 , andes_ax66_mdu*2")

(define_insn_reservation "andes_ax66_idivdi" 70
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "idiv")
            (eq_attr "mode" "DI")))
  "andes_ax66_pipe0  +andes_ax66_alu0 | andes_ax66_pipe1 +andes_ax66_alu1 , andes_ax66_mdu*2")

(define_insn_reservation "andes_ax66_xfer" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "mfc,mtc"))
  "andes_ax66_pipe0+andes_ax66_alu0 | andes_ax66_pipe1+andes_ax66_alu1")

;; DSP alu
 (define_insn_reservation "andes_ax66_dsp_alu" 1
   (and (eq_attr "tune" "andes_ax66")
        (eq_attr "type" "dalu"))
   "andes_ax66_pipe0 + andes_ax66_dsp| andes_ax66_pipe1 + andes_ax66_dsp")

 (define_insn_reservation "andes_ax66_dsp_alu64" 1
   (and (eq_attr "tune" "andes_ax66")
        (eq_attr "type" "dalu64"))
   "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

 (define_insn_reservation "andes_ax66_dsp_alu_round" 1
   (and (eq_attr "tune" "andes_ax66")
        (eq_attr "type" "daluround"))
   "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_cmp" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "dcmp"))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_clip" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "dclip"))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_mulqi" 1
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "dmul")
            (eq_attr "mode" "DSPV4QI, DSPV8QI")))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_mul" 2
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "dmul")
            (not (eq_attr "mode" "DSPV4QI, DSPV8QI"))))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_mac" 2
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "dmac")
            (not (eq_attr "mode" "DI"))))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_kmar64" 3
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "dmac")
            (eq_attr "mode" "DI")))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_insb" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "dinsb"))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_pack" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "dpack"))
  "andes_ax66_pipe0 | andes_ax66_pipe1")

(define_insn_reservation "andes_ax66_dsp_bpick" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "dbpick"))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_dsp_wext" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "dwext"))
  "andes_ax66_pipe0 + andes_ax66_dsp | andes_ax66_pipe1 + andes_ax66_dsp")

(define_insn_reservation "andes_ax66_fpu_alu_s" 3
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "fadd")
            (eq_attr "mode" "SF")))
  "andes_ax66_fpu_arith")

(define_insn_reservation "andes_ax66_fpu_alu_d" 4
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "fadd")
            (eq_attr "mode" "DF")))
  "andes_ax66_fpu_arith")

(define_insn_reservation "andes_ax66_fpu_mul_s" 3
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "fmul")
            (eq_attr "mode" "SF")))
  "andes_ax66_fpu_arith")

(define_insn_reservation "andes_ax66_fpu_mul_d" 4
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "fmul")
            (eq_attr "mode" "DF")))
  "andes_ax66_fpu_arith")

(define_insn_reservation "andes_ax66_fpu_mac_s" 3
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "fmadd")
            (eq_attr "mode" "SF")))
  "andes_ax66_pipe1 + andes_ax66_fpu_fmac")

(define_insn_reservation "andes_ax66_fpu_mac_d" 4
  (and (eq_attr "tune" "andes_ax66")
       (and (eq_attr "type" "fmadd")
            (eq_attr "mode" "DF")))
  "andes_ax66_pipe1 + andes_ax66_fpu_fmac")

(define_insn_reservation "andes_ax66_fpu_div" 33
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fdiv"))
  "andes_ax66_pipe0 + andes_ax66_fpu_fdiv | andes_ax66_pipe1 + andes_ax66_fpu_fdiv, andes_ax66_fpu_fdiv * 27")

(define_insn_reservation "andes_ax66_fpu_sqrt" 33
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fsqrt"))
  "andes_ax66_pipe0 + andes_ax66_fpu_fdiv | andes_ax66_pipe1 + andes_ax66_fpu_fdiv, andes_ax66_fpu_fdiv * 27")

(define_insn_reservation "andes_ax66_fpu_move" 1
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fmove,mtc,mfc"))
  "andes_ax66_pipe0 + andes_ax66_fpu_fmv | andes_ax66_pipe1 + andes_ax66_fpu_fmv")

(define_insn_reservation "andes_ax66_fpu_cmp" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fcmp"))
  "andes_ax66_pipe0 + andes_ax66_fpu_fmis | andes_ax66_pipe1 + andes_ax66_fpu_fmis")

(define_insn_reservation "andes_ax66_fpu_cvt" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fcvt"))
  "andes_ax66_pipe0 + andes_ax66_fpu_fmis | andes_ax66_pipe1 + andes_ax66_fpu_fmis")

(define_insn_reservation "andes_ax66_fpu_load" 4
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fpload"))
  "andes_ax66_pipe0 + andes_ax66_lsu | andes_ax66_pipe1 + andes_ax66_lsu")

(define_insn_reservation "andes_ax66_fpu_store" 0
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "fpstore"))
  "andes_ax66_pipe0 + andes_ax66_lsu | andes_ax66_pipe1 + andes_ax66_lsu")
  
(define_insn_reservation "andes_ax66_vpu_load" 8
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vlde,vldm,vldr"))
  "(andes_ax66_vpu_pipe + andes_ax66_vpu_lsu),andes_ax66_vpu_lsu*2")
  
(define_insn_reservation "andes_ax66_vpu_store" 0
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vste,vstm,vstr"))
  "andes_ax66_vpu_pipe + andes_ax66_lsu + andes_ax66_vpu_lsu")
  
(define_insn_reservation "andes_ax66_vpu_alu" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vialu,viwalu,vicalu,vsalu,vaalu"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")
  
(define_insn_reservation "andes_ax66_vpu_ext" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vext"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_permut")

(define_insn_reservation "andes_ax66_vpu_shift" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vshift,vnshift,vsshift"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")

(define_insn_reservation "andes_ax66_vpu_minmax" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "viminmax"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")

(define_insn_reservation "andes_ax66_vpu_cmp" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vicmp"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")

(define_insn_reservation "andes_ax66_vpu_mul" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vimul,viwmul,vsmul"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mac")

(define_insn_reservation "andes_ax66_vpu_div" 36
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vidiv"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_div*35")

(define_insn_reservation "andes_ax66_vpu_madd" 4
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vimuladd,viwmuladd"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mac")

(define_insn_reservation "andes_ax66_vpu_merge" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vimerge"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")

(define_insn_reservation "andes_ax66_vpu_move" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vimov,vimovvx,vimovxv,vmov,vslideup,vislide1up,vislide1down"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_permut")

(define_insn_reservation "andes_ax66_vpu_clip" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vnclip"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")

(define_insn_reservation "andes_ax66_vpu_falu" 4
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfalu,vfwalu,vfmul"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mac")

(define_insn_reservation "andes_ax66_vpu_fdiv" 38
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfdiv,vfsqrt"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fdiv")

(define_insn_reservation "andes_ax66_vpu_fmadd" 5
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfmuladd,vfwmuladd"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mac")

(define_insn_reservation "andes_ax66_vpu_fminmax" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfminmax"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fmis")

(define_insn_reservation "andes_ax66_vpu_fcmp" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfcmp"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fmis")

(define_insn_reservation "andes_ax66_vpu_fsgnj" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfsgnj"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fmis")

(define_insn_reservation "andes_ax66_vpu_fclass" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfclass"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fmis")

(define_insn_reservation "andes_ax66_vpu_fmerge" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfmerge"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fmis")

(define_insn_reservation "andes_ax66_vpu_fmove" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfmov,vfmovvf,vfmovfv,vfslide1up,vfslide1down"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_permut")

(define_insn_reservation "andes_ax66_vpu_fcvt" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfcvtitof,vfcvtftoi,vfwcvtitof,vfwcvtftoi,vfwcvtftof,vfncvtitof,vfncvtftoi,vfncvtftof"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_fmis")

(define_insn_reservation "andes_ax66_vpu_red" 9
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vired,viwred"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_alu")

(define_insn_reservation "andes_ax66_vpu_fredu" 6
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfredu,vfwredu"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mac")

(define_insn_reservation "andes_ax66_vpu_fredo" 34
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vfredo,vfwredo"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mac")

(define_insn_reservation "andes_ax66_vpu_malu" 3
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vmalu"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mask")

(define_insn_reservation "andes_ax66_vpu_mask" 4
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vmpop,vmffs,vmsfs,vmiota,vmidx"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_mask")

(define_insn_reservation "andes_ax66_vpu_gather" 2
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vgather"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_permut")

(define_insn_reservation "andes_ax66_vpu_compress" 4
  (and (eq_attr "tune" "andes_ax66")
       (eq_attr "type" "vcompress"))
  "andes_ax66_vpu_pipe + andes_ax66_vpu_permut")

(define_bypass 4
  "andes_ax66_load_wd, andes_ax66_load_bh"
  "andes_ax66_dsp_alu, andes_ax66_dsp_alu64, andes_ax66_dsp_alu_round,\
   andes_ax66_dsp_cmp, andes_ax66_dsp_clip, andes_ax66_dsp_insb, andes_ax66_dsp_pack,\
   andes_ax66_dsp_bpick, andes_ax66_dsp_wext, andes_ax66_dsp_mul, andes_ax66_dsp_mac")
