;; Copyright (C) 2024-2024 Free Software Foundation, Inc.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.
;; Vector load/store

(define_automaton "vicuna_vector")

;; Separate issue queue for vector instructions.
;;(define_cpu_unit "vxu_ooo_issue" "vector_ooo")

;; Vector execution unit.
(define_cpu_unit "vicuna_vpu_lsu, vicuna_vpu_alu, vicuna_vpu_mac, vicuna_vpu_msk,
  vicuna_vpu_div, vicuna_vpu_fmac, vicuna_vpu_fmis,
  vicuna_vpu_perm, vicuna_vpu_pipe" "vicuna_vector")

;; Vector subunit that does mult/div/sqrt.
;;(define_cpu_unit "vxu_ooo_multicycle" "vector_ooo")

(define_insn_reservation "vicuna_vload" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vlde,vldm,vlds,vldff,vldr"))
  "(vicuna_vpu_pipe + vicuna_vpu_lsu)*3")

(define_insn_reservation "vicuna_index_vload" 8
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vldux,vldox"))
  "(vicuna_vpu_pipe + vicuna_vpu_lsu)*3")

(define_insn_reservation "vicuna_seg_vload" 16
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vlsegde,vlsegds,vlsegdux,vlsegdox,vlsegdff"))
  "(vicuna_vpu_pipe + vicuna_vpu_lsu)*3")

(define_insn_reservation "vicuna_vstore" 0
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vste,vstm,vsts,vstux,vstox,vstr,vssegte,\
                        vssegts,vssegtux,vssegtox"))
  "(vicuna_vpu_pipe + vicuna_vpu_lsu)*3")

(define_insn_reservation "vicuna_vialu" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vialu,vicalu,vshift,viminmax,vicmp,vimov,\
                        vsalu,vaalu,vmov,vector,vimerge"))
  "vicuna_vpu_pipe + vicuna_vpu_alu")
  
(define_insn_reservation "vicuna_widen_vialu" 2
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "viwalu, vext, vsshift"))
  "vicuna_vpu_pipe + vicuna_vpu_alu")
  
(define_insn_reservation "vicuna_narrow_vialu" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vnshift,vnclip"))
  "vicuna_vpu_pipe + vicuna_vpu_alu")
  
(define_insn_reservation "vicuna_vimul" 2
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vimul,vimuladd,vsmul"))
  "vicuna_vpu_pipe + vicuna_vpu_mac")

(define_insn_reservation "vicuna_widen_vimul" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "viwmul,viwmuladd"))
  "vicuna_vpu_pipe + vicuna_vpu_mac")

(define_insn_reservation "vicuna_vperm" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vslideup,vslidedown,vislide1up,vislide1down,\
                        vfslide1up,vfslide1down,vgather"))
  "vicuna_vpu_pipe + vicuna_vpu_perm")
  
(define_insn_reservation "vicuna_vcompress" 4
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vcompress"))
  "vicuna_vpu_pipe + vicuna_vpu_perm")

(define_insn_reservation "vicuna_vmovv" 7
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vimovvx,vfmovvf"))
  "(vicuna_vpu_pipe + vicuna_vpu_perm)*5")

(define_insn_reservation "vicuna_vmovx" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vimovxv,vfmovfv"))
  "vicuna_vpu_pipe + vicuna_vpu_perm")

(define_insn_reservation "vicuna_vreduction" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vired,viwred"))
  "vicuna_vpu_pipe + vicuna_vpu_alu*5")

(define_insn_reservation "vicuna_vidiv" 35
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vidiv"))
  "vicuna_vpu_pipe + vicuna_vpu_div*34")

(define_insn_reservation "vicuna_vmask_2" 2
  (eq_attr "type" "vmalu,vmsfs")
  "vicuna_vpu_pipe + vicuna_vpu_msk")

(define_insn_reservation "vicuna_vmask_3" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vmiota,vmidx"))
  "vicuna_vpu_pipe + vicuna_vpu_msk")

(define_insn_reservation "vicuna_vpopc" 6
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vmpop"))
  "vicuna_vpu_pipe + vicuna_vpu_msk")

(define_insn_reservation "vicuna_vffs" 7
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vmffs"))
  "vicuna_vpu_pipe + vicuna_vpu_msk")

(define_insn_reservation "vicuna_vfadd" 4
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfalu,vfwalu,vfmul,vfwmul,vfmuladd,\
                        vfwmuladd"))
  "vicuna_vpu_pipe + vicuna_vpu_fmac")

(define_insn_reservation "vicuna_vfdiv" 39
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfdiv,vfsqrt"))
  "vicuna_vpu_pipe + vicuna_vpu_div*19")

(define_insn_reservation "vicuna_vfmis" 2
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfminmax,vfcmp,vfsgnj,vfclass,vfmerge"))
  "vicuna_vpu_pipe + vicuna_vpu_fmis")

(define_insn_reservation "vicuna_vfrecp" 3
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfrecp"))
  "vicuna_vpu_pipe + vicuna_vpu_div")

(define_insn_reservation "vicuna_vfcvt" 2
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfcvtitof,vfcvtftoi"))
  "vicuna_vpu_pipe + vicuna_vpu_fmis")

(define_insn_reservation "vicuna_widen_vfcvt" 5
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfwcvtitof,vfwcvtftoi,vfwcvtftof"))
  "vicuna_vpu_pipe + vicuna_vpu_fmis")

(define_insn_reservation "vicuna_narrow_vfcvt" 4
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfncvtitof,vfncvtftoi,vfncvtftof"))
  "vicuna_vpu_pipe + vicuna_vpu_fmis")  

(define_insn_reservation "vicuna_vfreduction" 6
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vfredu,vfwredu,vfredo,vfwredo"))
  "vicuna_vpu_pipe + vicuna_vpu_fmac*24")

(define_insn_reservation "vicuna_vesetvl" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "vsetvl,vsetvl_pre"))
  "vicuna_vpu_pipe")

(define_insn_reservation "vicuna_vcsr" 1
  (and (eq_attr "tune" "vicuna")
       (eq_attr "type" "wrvxrm,wrfrm,rdvlenb,rdvl"))
  "vicuna_vpu_pipe")

