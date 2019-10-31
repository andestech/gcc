;; Builtin function codegen for RISC-V targets.
;; Copyright (C) 2011-2017 Free Software Foundation, Inc.
;; Contributed by Andrew Waterman (andrew@sifive.com).
;; Based on MIPS target for GNU compiler.

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

;; A list of the modes that are up to one word long vector.
(define_mode_iterator VQIHI [(V4QI "!TARGET_64BIT") (V2HI "!TARGET_64BIT")
                             (V8QI "TARGET_64BIT") (V4HI "TARGET_64BIT")])

(define_mode_iterator VECI [(V4QI "!TARGET_64BIT") (V2HI "!TARGET_64BIT")
                            (V8QI "TARGET_64BIT") (V4HI "TARGET_64BIT")
			    (V2SI "TARGET_64BIT")])

(define_mode_iterator VSHI [(V2HI "!TARGET_64BIT") (V2SI "TARGET_64BIT")])

(define_mode_iterator VHI [(V2HI "!TARGET_64BIT") (V4HI "TARGET_64BIT")])

(define_mode_iterator VQI [(V4QI "!TARGET_64BIT") (V8QI "TARGET_64BIT")])

(define_mode_iterator VSI [(V2SI "!TARGET_64BIT") (V4SI "TARGET_64BIT")])

(define_mode_iterator VD_SI [(SI "!TARGET_64BIT") (V2SI "TARGET_64BIT")])

;; Give the number of DSP instructions in the mode
(define_mode_attr bits [(V8QI "8") (V4QI "8") (QI "8") (V4HI "16") (V2HI "16")
			(HI "16") (V2SI "32") (DI "64")])

(define_mode_attr bsize [(HI "8") (SI "16")])

(define_mode_attr VSH_EXT [(V2SI "DI") (V2HI "HI")])

(define_mode_attr VELT [(V4QI "QI") (V2HI "HI") (V8QI "QI") (V4HI "HI")])

(define_mode_attr VEXT [(V4QI "V4HI") (V2HI "V2SI") (V8QI "V8HI") (V4HI "V4SI")
			(V2SI "V2DI")])

(define_mode_attr VNHALF [(V2SI "SI") (V2HI "HI")])

(define_code_iterator all_plus [plus ss_plus us_plus])

(define_code_iterator all_minus [minus ss_minus us_minus])

(define_code_iterator plus_minus [plus minus])

(define_code_iterator sumax [smax umax])

(define_code_iterator sumin [smin umin])

(define_code_iterator sumin_max [smax umax smin umin])

(define_code_iterator unop [clrsb clz])

(define_code_attr shift
  [(ashift "ashl") (ashiftrt "ashr") (lshiftrt "lshr") (rotatert "rotr")])

(define_code_attr su
  [(ashiftrt "") (lshiftrt "u") (sign_extend "s") (zero_extend "u")])

(define_code_attr uk
  [(plus "") (ss_plus "k") (us_plus "uk")
   (minus "") (ss_minus "k") (us_minus "uk")])

(define_code_attr zs
  [(sign_extend "s") (zero_extend "z")])

(define_code_attr add_sub
  [(plus "a") (minus "s")])

(define_code_attr opcode
  [(plus "add") (minus "sub") (smax "smax") (umax "umax") (smin "smin") (umin "umin")])

(define_expand "mov<mode>"
  [(set (match_operand:VQIHI 0 "")
	(match_operand:VQIHI 1 ""))]
  "TARGET_DSP"
{
  if (riscv_legitimize_move (<MODE>mode, operands[0], operands[1]))
    DONE;
})

(define_insn "*mov<mode>_internal"
  [(set (match_operand:VQIHI 0 "nonimmediate_operand" "=r,r,r, m,  *f,*f,*r,*m")
	(match_operand:VQIHI 1 "move_operand"         " r,T,m,rJ,*r*J,*m,*f,*f"))]
  "(register_operand (operands[0], <MODE>mode)
    || reg_or_0_operand (operands[1], <MODE>mode))
   && TARGET_DSP"
  { return riscv_output_move (operands[0], operands[1]); }
  [(set_attr "move_type" "move,const,load,store,mtc,fpload,mfc,fpstore")
   (set_attr "mode" "<MODE>")])

(define_expand "movv2si"
  [(set (match_operand:V2SI 0 "")
	(match_operand:V2SI 1 ""))]
  "TARGET_64BIT && TARGET_DSP"
{
  if (riscv_legitimize_move (V2SImode, operands[0], operands[1]))
    DONE;
})

(define_insn "*movv2si_64bit"
  [(set (match_operand:V2SI 0 "nonimmediate_operand" "=r,r,r, m,  *f,*f,*r,*f,*m")
	(match_operand:V2SI 1 "move_operand"         " r,T,m,rJ,*r*J,*m,*f,*f,*f"))]
  "TARGET_64BIT && TARGET_DSP
   && (register_operand (operands[0], V2SImode)
       || reg_or_0_operand (operands[1], V2SImode))"
  { return riscv_output_move (operands[0], operands[1]); }
  [(set_attr "move_type" "move,const,load,store,mtc,fpload,mfc,fmove,fpstore")
   (set_attr "mode" "V2SI")])

(define_insn "<uk>add<mode>3"
  [(set (match_operand:VECI 0 "register_operand"                "=r")
	(all_plus:VECI (match_operand:VECI 1 "register_operand" " r")
		       (match_operand:VECI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "<uk>add<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "dsp_<uk>adddi3"
  [(set (match_operand:DI 0 "register_even_operand"              "=r")
	(all_plus:DI (match_operand:DI 1 "register_even_operand" " r")
		     (match_operand:DI 2 "register_even_operand" " r")))]
  "TARGET_DSP"
  "<uk>add64 %0, %1, %2"
  [(set_attr "type" "dalu64")
   (set_attr "mode" "DI")])

(define_insn "radd<mode>3"
  [(set (match_operand:VECI 0 "register_operand" "=r")
	(truncate:VECI
	  (ashiftrt:<VEXT>
	    (plus:<VEXT> (sign_extend:<VEXT> (match_operand:VECI 1 "register_operand" " r"))
			 (sign_extend:<VEXT> (match_operand:VECI 2 "register_operand" " r")))
	    (const_int 1))))]
  "TARGET_DSP"
  "radd<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "uradd<mode>3"
  [(set (match_operand:VECI 0 "register_operand" "=r")
	(truncate:VECI
	  (lshiftrt:<VEXT>
	    (plus:<VEXT> (zero_extend:<VEXT> (match_operand:VECI 1 "register_operand" " r"))
			 (zero_extend:<VEXT> (match_operand:VECI 2 "register_operand" " r")))
	    (const_int 1))))]
  "TARGET_DSP"
  "uradd<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "radddi3"
  [(set (match_operand:DI 0 "register_even_operand" "=r")
	(truncate:DI
	  (ashiftrt:TI
	    (plus:TI (sign_extend:TI (match_operand:DI 1 "register_even_operand" " r"))
		     (sign_extend:TI (match_operand:DI 2 "register_even_operand" " r")))
	  (const_int 1))))]
  "TARGET_DSP"
  "radd64\t%0, %1, %2"
  [(set_attr "type" "dalu64")
   (set_attr "mode" "DI")])

(define_insn "uradddi3"
  [(set (match_operand:DI 0 "register_even_operand" "=r")
	(truncate:DI
	  (lshiftrt:TI
	    (plus:TI (zero_extend:TI (match_operand:DI 1 "register_even_operand" " r"))
		     (zero_extend:TI (match_operand:DI 2 "register_even_operand" " r")))
	  (const_int 1))))]
  "TARGET_DSP"
  "uradd64\t%0, %1, %2"
  [(set_attr "type" "dalu64")
   (set_attr "mode" "DI")])

(define_insn "<uk>sub<mode>3"
  [(set (match_operand:VECI 0 "register_operand"                  "=r")
	(all_minus:VECI (match_operand:VECI 1 "register_operand" " r")
			(match_operand:VECI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "<uk>sub<bits> %0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "dsp_<uk>subdi3"
  [(set (match_operand:DI 0 "register_even_operand"               "=r")
	(all_minus:DI (match_operand:DI 1 "register_even_operand" " r")
		      (match_operand:DI 2 "register_even_operand" " r")))]
  "TARGET_DSP"
  "<uk>sub64 %0, %1, %2"
  [(set_attr "type" "dalu64")
   (set_attr "mode" "DI")])

(define_insn "rsub<mode>3"
  [(set (match_operand:VECI 0 "register_operand"                                   "=r")
	(truncate:VECI
	  (ashiftrt:<VEXT>
	    (minus:<VEXT> (sign_extend:<VEXT> (match_operand:VECI 1 "register_operand" " r"))
			  (sign_extend:<VEXT> (match_operand:VECI 2 "register_operand" " r")))
	    (const_int 1))))]
  "TARGET_DSP"
  "rsub<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "ursub<mode>3"
  [(set (match_operand:VECI 0 "register_operand"                                   "=r")
	(truncate:VECI
	  (ashiftrt:<VEXT>
	    (minus:<VEXT> (zero_extend:<VEXT> (match_operand:VECI 1 "register_operand" " r"))
			  (zero_extend:<VEXT> (match_operand:VECI 2 "register_operand" " r")))
	    (const_int 1))))]
  "TARGET_DSP"
  "ursub<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "rsubdi3"
  [(set (match_operand:DI 0 "register_even_operand"                   "=r")
	(truncate:DI
	  (ashiftrt:TI
	    (minus:TI (sign_extend:TI (match_operand:DI 1 "register_even_operand" " r"))
		      (sign_extend:TI (match_operand:DI 2 "register_even_operand" " r")))
	  (const_int 1))))]
  "TARGET_DSP"
  "rsub64\t%0, %1, %2"
  [(set_attr "type" "dalu64")
   (set_attr "mode" "DI")])

(define_insn "ursubdi3"
  [(set (match_operand:DI 0 "register_even_operand"                   "=r")
	(truncate:DI
	  (lshiftrt:TI
	    (minus:TI (zero_extend:TI (match_operand:DI 1 "register_even_operand" " r"))
		      (zero_extend:TI (match_operand:DI 2 "register_even_operand" " r")))
	  (const_int 1))))]
  "TARGET_DSP"
  "ursub64\t%0, %1, %2"
  [(set_attr "type" "dalu64")
   (set_attr "mode" "DI")])

(define_expand "cras<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_cras<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "cras<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:VSHI
	    (plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP"
  "cras<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "kcras<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_kcras<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kcras<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (ss_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:VSHI
	    (ss_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP"
  "kcras<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "ukcras<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_ukcras<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukcras<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (us_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:VSHI
	    (us_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP"
  "ukcras<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "crsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_crsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "crsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP"
  "crsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "kcrsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_kcrsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kcrsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (ss_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (ss_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP"
  "kcrsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "ukcrsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_ukcrsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukcrsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (us_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (us_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP"
  "ukcrsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "rcras<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_rcras<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rcras<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
		(minus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 0)])))
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 0)])))
		  (sign_extend:SI
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 1)))]
  "TARGET_DSP"
  "rcras<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "urcras<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_urcras<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urcras<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
		(minus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 0)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 0)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 1)))]
  "TARGET_DSP"
  "urcras<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "rcrsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_rcrsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rcrsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
	        (minus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 1)])))
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 0)])))
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 2)))]
  "TARGET_DSP"
  "rcrsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "urcrsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_urcrsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urcrsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
	        (minus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 1)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 0)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 2)))]
  "TARGET_DSP"
  "urcrsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "cras16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_cras16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "cras16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (minus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				     (parallel [(const_int 0)]))
		      (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				     (parallel [(const_int 1)])))
	    (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
		     (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
		      (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (plus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
		      (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "cras16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "kcras16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kcras16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kcras16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (ss_minus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
					(parallel [(const_int 0)]))
			 (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
					(parallel [(const_int 1)])))
	    (ss_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (ss_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (ss_plus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kcras16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "ukcras16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_ukcras16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukcras16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (us_minus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
					(parallel [(const_int 0)]))
			 (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
					(parallel [(const_int 1)])))
	    (us_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (us_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (us_plus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "ukcras16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "crsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_crsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "crsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (plus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				    (parallel [(const_int 0)]))
		     (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 1)])))
	    (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
		      (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
		     (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (minus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
		       (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "crsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "kcrsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kcrsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kcrsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (ss_plus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				       (parallel [(const_int 0)]))
			(vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				       (parallel [(const_int 1)])))
	    (ss_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (ss_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (ss_minus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			  (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kcrsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "ukcrsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_ukcrsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukcrsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (us_plus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				       (parallel [(const_int 0)]))
			(vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				       (parallel [(const_int 1)])))
	    (us_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (vec_concat:V2HI
	    (us_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
	    (us_minus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			  (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "ukcrsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "rcras16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_rcras16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rcras16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 1)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "rcras16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "urcras16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_urcras16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urcras16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 1)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "urcras16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "rcrsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_rcrsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rcrsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 1)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "rcrsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "urcrsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_urcrsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urcrsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 1)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "urcrsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "<shift><mode>3"
  [(set (match_operand:VHI 0 "register_operand"                "")
	(any_shift:VHI (match_operand:VHI 1 "register_operand" "")
		       (match_operand:SI   2 "rimm4u_operand"  "")))]
  "TARGET_DSP"
{
  if (operands[2] == const0_rtx)
    {
      emit_move_insn (operands[0], operands[1]);
      DONE;
    }
})

(define_insn "*ashl<mode>3"
  [(set (match_operand:VHI 0 "register_operand"             "=  r, r")
	(ashift:VHI (match_operand:VHI 1 "register_operand" "   r, r")
		    (match_operand:SI 2  "rimm4u_operand"   " u04, r")))]
  "TARGET_DSP"
  "@
   slli16\t%0, %1, %2
   sll16\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "kslli16<mode>"
  [(set (match_operand:VHI 0 "register_operand"                "=  r, r")
	(ss_ashift:VHI (match_operand:VHI 1 "register_operand" "   r, r")
		       (match_operand:SI 2  "rimm4u_operand"   " u04, r")))]
  "TARGET_DSP"
  "@
   kslli16\t%0, %1, %2
   ksll16\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "ksll32"
  [(set (match_operand:V2SI 0 "register_operand"                 "=  r, r")
	(ss_ashift:V2SI (match_operand:V2SI 1 "register_operand" "   r, r")
			(match_operand:SI 2   "rimm5u_operand"   " u05, r")))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   kslli32\t%0, %1, %2
   ksll32\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V2SI")])

(define_insn "*ashr<mode>3"
  [(set (match_operand:VHI 0 "register_operand"               "=   r, r")
	(ashiftrt:VHI (match_operand:VHI 1 "register_operand" "    r, r")
		      (match_operand:SI 2  "rimm4u_operand"   " u04, r")))]
  "TARGET_DSP"
  "@
   srai16\t%0, %1, %2
   sra16\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "sra16_round<mode>"
  [(set (match_operand:VHI 0 "register_operand"                            "=  r, r")
	(unspec:VHI [(ashiftrt:VHI (match_operand:VHI 1 "register_operand" "   r, r")
				   (match_operand:SI 2  "rimm4u_operand"   " u04, r"))]
		     UNSPEC_ROUND))]
  "TARGET_DSP"
  "@
   srai16.u\t%0, %1, %2
   sra16.u\t%0, %1, %2"
  [(set_attr "type" "daluround, daluround")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "sra32_round"
  [(set (match_operand:V2SI 0 "register_operand"                              "=  r, r")
	(unspec:V2SI [(ashiftrt:V2SI (match_operand:V2SI 1 "register_operand" "   r, r")
				     (match_operand:SI 2   "rimm5u_operand"   " u05, r"))]
		       UNSPEC_ROUND))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   srai32.u\t%0, %1, %2
   sra32.u\t%0, %1, %2"
  [(set_attr "type" "daluround, daluround")
   (set_attr "mode" "V2SI,  V2SI")])

(define_insn "*lshr<mode>3"
  [(set (match_operand:VHI 0 "register_operand"               "=  r, r")
	(lshiftrt:VHI (match_operand:VHI 1 "register_operand" "   r, r")
		      (match_operand:SI 2  "rimm4u_operand"   " u04, r")))]
  "TARGET_DSP"
  "@
   srli16\t%0, %1, %2
   srl16\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "srl16_round<mode>"
  [(set (match_operand:VHI 0 "register_operand"                            "=  r, r")
	(unspec:VHI [(lshiftrt:VHI (match_operand:VHI 1 "register_operand" "   r, r")
				   (match_operand:SI 2  "rimm4u_operand"   " u04, r"))]
		     UNSPEC_ROUND))]
  "TARGET_DSP"
  "@
   srli16.u\t%0, %1, %2
   srl16.u\t%0, %1, %2"
  [(set_attr "type" "daluround, daluround")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "srl32_round"
  [(set (match_operand:V2SI 0 "register_operand"                              "=  r, r")
	(unspec:V2SI [(lshiftrt:V2SI (match_operand:V2SI 1 "register_operand" "   r, r")
				     (match_operand:SI 2   "rimm5u_operand"   " u05, r"))]
		       UNSPEC_ROUND))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   srli32.u\t%0, %1, %2
   srl32.u\t%0, %1, %2"
  [(set_attr "type" "daluround, daluround")
   (set_attr "mode" "V2SI,  V2SI")])

(define_insn "kslra<VECI:mode><X:mode>"
  [(set (match_operand:VECI 0 "register_operand"                  "=r")
	(if_then_else:VECI
	  (lt:X (match_operand:X 2 "register_operand"             " r")
		(const_int 0))
	  (ashiftrt:VECI (match_operand:VECI 1 "register_operand" " r")
			 (neg:X (match_dup 2)))
	  (ss_ashift:VECI (match_dup 1)
			  (match_dup 2))))]
  "TARGET_DSP"
  "kslra<VECI:bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<VECI:MODE>")])

(define_insn "kslra<VECI:mode><X:mode>_round"
  [(set (match_operand:VECI 0 "register_operand"                  "=r")
	(if_then_else:VECI
	  (lt:X (match_operand:X 2 "register_operand"             " r")
		(const_int 0))
	  (unspec:VECI [(ashiftrt:VECI (match_operand:VECI 1 "register_operand" " r")
				       (neg:X (match_dup 2)))]
		        UNSPEC_ROUND)
	  (ss_ashift:VECI (match_dup 1)
			  (match_dup 2))))]
  "TARGET_DSP"
  "kslra<VECI:bits>.u\t%0, %1, %2"
  [(set_attr "type" "daluround")
   (set_attr "mode" "<VECI:MODE>")])

(define_insn "<optab>v2si3"
  [(set (match_operand:V2SI 0 "register_operand"                 "=  r, r")
	(any_shift:V2SI (match_operand:V2SI 1 "register_operand" "   r, r")
			(match_operand:SI   2  "rimm5u_operand"  " u05, r")))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   <insn>i32\t%0, %1, %2
   <insn>32\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "V2SI,  V2SI")])

(define_insn "cmpeq<mode>"
  [(set (match_operand:VQIHI 0 "register_operand"                          "=r")
	(unspec:VQIHI [(eq:VQIHI (match_operand:VQIHI 1 "register_operand" " r")
				 (match_operand:VQIHI 2 "register_operand" " r"))]
		       UNSPEC_VEC_COMPARE))]
  "TARGET_DSP"
  "cmpeq<bits>\t%0, %1, %2"
  [(set_attr "type" "dcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "scmplt<mode>"
  [(set (match_operand:VQIHI 0 "register_operand"                          "=r")
	(unspec:VQIHI [(lt:VQIHI (match_operand:VQIHI 1 "register_operand" " r")
				 (match_operand:VQIHI 2 "register_operand" " r"))]
		       UNSPEC_VEC_COMPARE))]
  "TARGET_DSP"
  "scmplt<bits>\t%0, %1, %2"
  [(set_attr "type" "dcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "scmple<mode>"
  [(set (match_operand:VQIHI 0 "register_operand"                          "=r")
	(unspec:VQIHI [(le:VQIHI (match_operand:VQIHI 1 "register_operand" " r")
				 (match_operand:VQIHI 2 "register_operand" " r"))]
		       UNSPEC_VEC_COMPARE))]
  "TARGET_DSP"
  "scmple<bits>\t%0, %1, %2"
  [(set_attr "type" "dcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "ucmplt<mode>"
  [(set (match_operand:VQIHI 0 "register_operand"                          "=r")
	(unspec:VQIHI [(ltu:VQIHI (match_operand:VQIHI 1 "register_operand" " r")
				  (match_operand:VQIHI 2 "register_operand" " r"))]
		       UNSPEC_VEC_COMPARE))]
  "TARGET_DSP"
  "ucmplt<bits>\t%0, %1, %2"
  [(set_attr "type" "dcmp")
   (set_attr "mode" "SI")])

(define_insn "ucmple<mode>"
  [(set (match_operand:VQIHI 0 "register_operand"                          "=r")
	(unspec:VQIHI [(leu:VQIHI (match_operand:VQIHI 1 "register_operand" " r")
				  (match_operand:VQIHI 2 "register_operand" " r"))]
		       UNSPEC_VEC_COMPARE))]
  "TARGET_DSP"
  "ucmple<bits>\t%0, %1, %2"
  [(set_attr "type" "dcmp")
   (set_attr "mode" "<MODE>")])

(define_insn "sclip8<mode>"
  [(set (match_operand:VQI 0 "register_operand"              "=  r")
	(unspec:VQI [(match_operand:VQI 1 "register_operand" "   r")
		      (match_operand:SI 2 "imm3u_operand"    " u03")]
		     UNSPEC_CLIPS))]
  "TARGET_DSP"
  "sclip8\t%0, %1, %2"
  [(set_attr "type" "dclip")
   (set_attr "mode" "<MODE>")])

(define_insn "uclip8<mode>"
  [(set (match_operand:VQI 0 "register_operand"               "=  r")
	(unspec:VQI [(match_operand:VQI 1 "register_operand"  "   r")
		     (match_operand:SI 2 "imm3u_operand"      " u03")]
		     UNSPEC_CLIP))]
  "TARGET_DSP"
  "uclip8\t%0, %1, %2"
  [(set_attr "type" "dclip")
   (set_attr "mode" "<MODE>")])

(define_insn "sclip16<mode>"
  [(set (match_operand:VHI 0 "register_operand"              "=   r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "    r")
		     (match_operand:SI 2 "imm4u_operand"     " u04")]
		     UNSPEC_CLIPS))]
  "TARGET_DSP"
  "sclip16\t%0, %1, %2"
  [(set_attr "type" "dclip")
   (set_attr "mode" "<MODE>")])

(define_insn "uclip16<mode>"
  [(set (match_operand:VHI 0 "register_operand"               "=   r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand"  "    r")
		      (match_operand:SI 2 "imm4u_operand"     " u04")]
		     UNSPEC_CLIP))]
  "TARGET_DSP"
  "uclip16\t%0, %1, %2"
  [(set_attr "type" "dclip")
   (set_attr "mode" "<MODE>")])

(define_insn "khm16<mode>"
  [(set (match_operand:VHI 0 "register_operand"              "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" " r")
		     (match_operand:VHI 2 "register_operand" " r")]
		     UNSPEC_KHM))]
  "TARGET_DSP"
  "khm16\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "khmx16<mode>"
  [(set (match_operand:VHI 0 "register_operand"              "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" " r")
		     (match_operand:VHI 2 "register_operand" " r")]
		     UNSPEC_KHMX))]
  "TARGET_DSP"
  "khmx16\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "khm8<mode>"
  [(set (match_operand:VQI 0 "register_operand"              "=r")
	(unspec:VQI [(match_operand:VQI 1 "register_operand" " r")
		     (match_operand:VQI 2 "register_operand" " r")]
		     UNSPEC_KHM))]
  "TARGET_DSP"
  "khm8\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "khmx8<mode>"
  [(set (match_operand:VQI 0 "register_operand"              "=r")
	(unspec:VQI [(match_operand:VQI 1 "register_operand" " r")
		     (match_operand:VQI 2 "register_operand" " r")]
		     UNSPEC_KHMX))]
  "TARGET_DSP"
  "khmx8\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode" "<MODE>")])

(define_expand "vec_setv4qi"
  [(match_operand:V4QI 0 "register_operand" "")
   (match_operand:QI 1 "register_operand" "")
   (match_operand:SI 2 "immediate_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  HOST_WIDE_INT pos = INTVAL (operands[2]);
  if (pos > 4)
    gcc_unreachable ();
  HOST_WIDE_INT elem = (HOST_WIDE_INT) 1 << pos;
  emit_insn (gen_vec_setv4qi_internal (operands[0], operands[1],
				       operands[0], GEN_INT (elem)));
  DONE;
})

(define_expand "insb"
  [(match_operand:V4QI 0 "register_operand" "")
   (match_operand:V4QI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:SI 3 "const_int_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  if (INTVAL (operands[3]) > 3 || INTVAL (operands[3]) < 0)
    gcc_unreachable ();

  rtx src = gen_reg_rtx (QImode);

  convert_move (src, operands[2], false);

  HOST_WIDE_INT selector_index;
  selector_index = INTVAL (operands[3]);
  rtx selector = gen_int_mode (1 << selector_index, SImode);
  emit_insn (gen_vec_setv4qi_internal (operands[0], src,
				       operands[1], selector));
  DONE;
}
[(set_attr "type" "dinsb")])

(define_expand "insvsi"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand" "")
			 (match_operand:SI 1 "const_int_operand" "")
			 (match_operand:SI 2 "insv_operand" ""))
	(match_operand:SI 3 "register_operand" ""))]
  "TARGET_DSP && !TARGET_64BIT"
{
  if (INTVAL (operands[1]) != 8)
    FAIL;
})

(define_insn "insvsi_internal"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand"   "+r")
			 (const_int 8)
			 (match_operand:SI 1 "insv_operand"        "i"))
	(match_operand:SI 2                  "register_operand"    "r"))]
  "TARGET_DSP && !TARGET_64BIT"
  "insb\t%0, %2, %v1"
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "SI")])

(define_insn "insvsiqi_internal"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand"   "+r")
			 (const_int 8)
			 (match_operand:SI 1 "insv64_operand"     "i"))
	(zero_extend:SI (match_operand:QI 2 "register_operand"    "r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "insb\t%0, %2, %v1"
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "SI")])

;; v0 = (v1 & 0xff00ffff) | ((v2 << 16) | 0xff0000)
(define_insn_and_split "insbsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "0")
			(const_int -16711681))
		(and:SI (ashift:SI (match_operand:SI 2 "register_operand" "r")
				   (const_int 16))
			(const_int 16711680))))]
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  "#"
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  [(const_int 1)]
{
  rtx tmp = gen_reg_rtx (SImode);
  emit_move_insn (tmp, operands[1]);
  emit_insn (gen_insvsi_internal (tmp, gen_int_mode(16, SImode), operands[2]));
  emit_move_insn (operands[0], tmp);
  DONE;
})

(define_insn "vec_setv4qi_internal"
  [(set (match_operand:V4QI 0 "register_operand"    "=   r,    r,    r,    r")
	(vec_merge:V4QI
	  (vec_duplicate:V4QI
	    (match_operand:QI 1 "register_operand"  "    r,    r,    r,    r"))
	  (match_operand:V4QI 2 "register_operand"  "    0,    0,    0,    0")
	  (match_operand:SI 3 "imm_1_2_4_8_operand" " v01, v02, v04, v08")))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   insb\t%0, %1, 0
   insb\t%0, %1, 1
   insb\t%0, %1, 2
   insb\t%0, %1, 3"
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "V4QI")])

(define_insn "vec_setv4qi_internal_vec"
  [(set (match_operand:V4QI 0 "register_operand"       "=  r,   r,   r,   r")
	(vec_merge:V4QI
	  (vec_duplicate:V4QI
	    (vec_select:QI
	      (match_operand:V4QI 1 "register_operand" "   r,   r,   r,   r")
	      (parallel [(const_int 0)])))
	  (match_operand:V4QI 2 "register_operand"     "   0,   0,   0,   0")
	  (match_operand:SI 3 "imm_1_2_4_8_operand"    " v01, v02, v04, v08")))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   insb\t%0, %1, 0
   insb\t%0, %1, 1
   insb\t%0, %1, 2
   insb\t%0, %1, 3"
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "V4QI")])

(define_insn "vec_mergev4qi_and_cv0_1"
  [(set (match_operand:V4QI 0 "register_operand"       "=r")
	(vec_merge:V4QI
	  (vec_duplicate:V4QI
	    (vec_select:QI
	      (match_operand:V4QI 1 "register_operand" " r")
	      (parallel [(const_int 0)])))
	  (const_vector:V4QI [
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)])
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "andi\t%0, %1, 0xff"
  [(set_attr "type" "arith")
   (set_attr "mode" "V4QI")])

(define_insn "vec_mergev4qi_and_cv0_2"
  [(set (match_operand:V4QI 0 "register_operand"       "=r")
	(vec_merge:V4QI
	  (const_vector:V4QI [
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)])
	  (vec_duplicate:V4QI
	    (vec_select:QI
	      (match_operand:V4QI 1 "register_operand" " r")
	      (parallel [(const_int 0)])))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "andi\t%0, %1, 0xff"
  [(set_attr "type" "arith")
   (set_attr "mode" "V4QI")])

(define_insn "vec_mergeqi_and_cv0_1"
  [(set (match_operand:V4QI 0 "register_operand"                     "=r")
	(vec_merge:V4QI
	  (vec_duplicate:V4QI (match_operand:QI 1 "register_operand" " r"))
	  (const_vector:V4QI [
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)])
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "andi\t%0, %1, 0xff"
  [(set_attr "type" "arith")
   (set_attr "mode" "V4QI")])

(define_insn "vec_mergeqi_and_cv0_2"
  [(set (match_operand:V4QI 0 "register_operand"                     "=r")
	(vec_merge:V4QI
	  (const_vector:V4QI [
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)
	    (const_int 0)])
	  (vec_duplicate:V4QI (match_operand:QI 1 "register_operand" " r"))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "andi\t%0, %1, 0xff"
  [(set_attr "type" "arith")
   (set_attr "mode" "V4QI")])

(define_expand "vec_setv8qi"
  [(match_operand:V8QI 0 "register_operand" "")
   (match_operand:QI 1 "register_operand" "")
   (match_operand:SI 2 "immediate_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  HOST_WIDE_INT pos = INTVAL (operands[2]);
  if (pos > 7)
    gcc_unreachable ();
  HOST_WIDE_INT elem = (HOST_WIDE_INT) 1 << pos;
  emit_insn (gen_vec_setv8qi_internal (operands[0], operands[1],
				       operands[0], GEN_INT (elem)));
  DONE;
})

(define_expand "insb64"
  [(match_operand:V8QI 0 "register_operand" "")
   (match_operand:V8QI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:SI 3 "const_insb64_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  if (INTVAL (operands[3]) > 7 || INTVAL (operands[3]) < 0)
    gcc_unreachable ();

  rtx src = gen_reg_rtx (QImode);

  convert_move (src, operands[2], false);

  HOST_WIDE_INT selector_index;
  selector_index = INTVAL (operands[3]);
  rtx selector = gen_int_mode (1 << selector_index, SImode);
  emit_insn (gen_vec_setv8qi_internal (operands[0], src,
				       operands[1], selector));
  DONE;
}
[(set_attr "type" "dinsb")])

(define_insn "vec_setv8qi_internal"
  [(set (match_operand:V8QI 0 "register_operand"    "=r")
	(vec_merge:V8QI
	  (vec_duplicate:V8QI
	    (match_operand:QI 1 "register_operand"  "r"))
	  (match_operand:V8QI 2 "register_operand"  "0")
	  (match_operand:SI 3 "pwr_7_operand" " D07")))]
  "TARGET_DSP && TARGET_64BIT"
{
  operands[3] = GEN_INT (exact_log2 (INTVAL (operands[3])));
  return "insb\t%0, %1, %3";
}
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "V8QI")])

(define_expand "insvdi"
  [(set (zero_extract:DI (match_operand:DI 0 "register_operand" "")
			 (match_operand:SI 1 "const_int_operand" "")
			 (match_operand:SI 2 "insv_operand" ""))
	(match_operand:DI 3 "register_operand" ""))]
  "TARGET_DSP && TARGET_64BIT"
{
  if (INTVAL (operands[1]) != 8)
    FAIL;
})

(define_insn "insvdi_internal"
  [(set (zero_extract:DI (match_operand:DI 0 "register_operand"   "+r")
			 (const_int 8)
			 (match_operand:SI 1 "insv64_operand"      "i"))
	(match_operand:DI 2                  "register_operand"    "r"))]
  "TARGET_DSP && TARGET_64BIT"
  "insb\t%0, %2, %v1"
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "DI")])

(define_insn "insvdiqi_internal"
  [(set (zero_extract:DI (match_operand:DI 0 "register_operand"   "+r")
			 (const_int 8)
			 (match_operand:SI 1 "insv64_operand"     "i"))
	(zero_extend:DI (match_operand:QI 2 "register_operand"    "r")))]
  "TARGET_DSP && TARGET_64BIT"
  "insb\t%0, %2, %v1"
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "DI")])

(define_insn "vec_setv8qi_internal_vec"
  [(set (match_operand:V8QI 0 "register_operand"       "=r")
	(vec_merge:V8QI
	  (vec_duplicate:V8QI
	    (vec_select:QI
	      (match_operand:V8QI 1 "register_operand" "r")
	      (parallel [(const_int 0)])))
	  (match_operand:V8QI 2 "register_operand"     "0")
	  (match_operand:SI 3 "pwr_7_operand"    "D07")))]
  "TARGET_DSP && TARGET_64BIT"
{
  operands[3] = GEN_INT (exact_log2 (INTVAL (operands[3])));
  return "insb\t%0, %1, %3";
}
  [(set_attr "type"  "dinsb")
   (set_attr "mode"  "V8QI")])

(define_expand "vec_set<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:<VNHALF> 1 "register_operand" "")
   (match_operand:SI 2 "immediate_operand" "")]
  "TARGET_DSP"
{
  HOST_WIDE_INT pos = INTVAL (operands[2]);
  if (pos > 2)
    gcc_unreachable ();
  HOST_WIDE_INT elem = (HOST_WIDE_INT) 1 << pos;
  emit_insn (gen_vec_set<mode>_internal (operands[0], operands[1],
					 operands[0], GEN_INT (elem)));
  DONE;
})

(define_insn "vec_set<mode>_internal"
  [(set (match_operand:VSHI 0 "register_operand"   "=  r,   r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (match_operand:<VNHALF> 1 "register_operand" "   r,   r"))
	  (match_operand:VSHI 2 "register_operand" "   r,   r")
	  (match_operand:SI 3 "imm_1_2_operand"    " v01, v02")))]
  "TARGET_DSP"
  "@
   pktb<bits>\t%0, %2, %1
   pkbb<bits>\t%0, %1, %2"
  [(set_attr "type"  "dpack")
   (set_attr "mode"  "<MODE>")])

(define_expand "pkbb<mode>"
  [(match_operand:VSHI 0 "register_operand")
   (match_operand:VSHI 1 "register_operand")
   (match_operand:VSHI 2 "register_operand")]
  "TARGET_DSP"
{
  emit_insn (gen_vec_merge<mode> (operands[0], operands[1], operands[2],
				  GEN_INT (2), GEN_INT (0), GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "pkbbsi_1"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int 65535))
		(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pkbb16\t%0, %2, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pkbbsi_2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI	(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))
		(and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int 65535))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pkbb16\t%0, %2, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pkbbsi_3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (zero_extend:SI	(match_operand:HI 1 "register_operand" "r"))
		(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pkbb16\t%0, %2, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pkbbsi_4"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI	(ashift:SI (match_operand:SI 2 "register_operand" "r")
			   (const_int 16))
		(zero_extend:SI (match_operand:HI 1 "register_operand" "r"))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pkbb16\t%0, %2, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

;; v0 = (v1 & 0xffff0000) | (v2 & 0xffff)
(define_insn "pktbsi_1"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int -65536))
		(zero_extend:SI (match_operand:HI 2 "register_operand" "r"))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pktb16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pktbsi_2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand" "r")
			(const_int -65536))
		(and:SI (match_operand:SI 2 "register_operand" "r")
			(const_int 65535))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pktb16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pktbsi_3"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand" "+r")
			 (const_int 16 )
			 (const_int 0))
	(match_operand:SI 1 "register_operand"                  " r"))]
  "TARGET_DSP && !TARGET_64BIT"
  "pktb16\t%0, %0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pktbsi_4"
  [(set (zero_extract:SI (match_operand:SI 0 "register_operand" "+r")
			 (const_int 16 )
			 (const_int 0))
	(zero_extend:SI (match_operand:HI 1 "register_operand"  " r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "pktb16\t%0, %0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_insn "pkttsi"
  [(set (match_operand:SI 0 "register_operand"                      "=r")
	(ior:SI (and:SI (match_operand:SI 1 "register_operand"      " r")
			(const_int -65536))
		(lshiftrt:SI (match_operand:SI 2 "register_operand" " r")
			     (const_int 16))))]
  "TARGET_DSP && !TARGET_64BIT"
  "pktt16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "SI")])

(define_expand "pkbt<mode>"
  [(match_operand:VSHI 0 "register_operand")
   (match_operand:VSHI 1 "register_operand")
   (match_operand:VSHI 2 "register_operand")]
  "TARGET_DSP"
{
  emit_insn (gen_vec_merge<mode> (operands[0], operands[1], operands[2],
				  GEN_INT (2), GEN_INT (0), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dpack")])

(define_expand "pktt<mode>"
  [(match_operand:VSHI 0 "register_operand")
   (match_operand:VSHI 1 "register_operand")
   (match_operand:VSHI 2 "register_operand")]
  "TARGET_DSP"
{
  emit_insn (gen_vec_merge<mode> (operands[0], operands[1], operands[2],
				  GEN_INT (2), GEN_INT (1), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dpack")])

(define_expand "pktb<mode>"
  [(match_operand:VSHI 0 "register_operand")
   (match_operand:VSHI 1 "register_operand")
   (match_operand:VSHI 2 "register_operand")]
  "TARGET_DSP"
{
  emit_insn (gen_vec_merge<mode> (operands[0], operands[1], operands[2],
				  GEN_INT (2), GEN_INT (1), GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "vec_mergerr"
  [(set (match_operand:V2HI 0 "register_operand"   "=  r,   r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (match_operand:HI 1 "register_operand" "   r,   r"))
	  (vec_duplicate:V2HI
	    (match_operand:HI 2 "register_operand" "   r,   r"))
	  (match_operand:SI 3 "imm_1_2_operand"    " v01, v02")))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   pkbb16\t%0, %2, %1
   pkbb16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V2HI")])


(define_insn "vec_merge"
  [(set (match_operand:V2HI 0 "register_operand"   "=  r,   r")
	(vec_merge:V2HI
	  (match_operand:V2HI 1 "register_operand" "   r,   r")
	  (match_operand:V2HI 2 "register_operand" "   r,   r")
	  (match_operand:SI 3 "imm_1_2_operand"    " v01, v02")))]
  "TARGET_DSP && !TARGET_64BIT"
{
  const char *pats[] = { "pktb16\t%0, %2, %1",
			 "pktb16\t%0, %1, %2" };
  return pats[which_alternative];
}
  [(set_attr "type" "dpack")
   (set_attr "mode" "V2HI")])

(define_insn "vec_mergerv"
  [(set (match_operand:V2HI 0 "register_operand"               "=   r,    r,    r,    r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (match_operand:HI 1 "register_operand"             "    r,    r,    r,    r"))
	  (vec_duplicate:V2HI
	    (vec_select:HI
	      (match_operand:V2HI 2 "register_operand"         "   r,   r,   r,   r")
	      (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v01, v00, v01")])))
	  (match_operand:SI 3 "imm_1_2_operand"                " v01, v01, v02, v02")))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   pkbb16\t%0, %2, %1
   pktb16\t%0, %2, %1
   pkbb16\t%0, %1, %2
   pkbt16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V2HI")])

(define_insn "vec_mergevr"
  [(set (match_operand:V2HI 0 "register_operand"                "=  r,   r,   r,   r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (vec_select:HI
	      (match_operand:V2HI 1 "register_operand"          "   r,   r,   r,   r")
	       (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v01, v00, v01")])))
	  (vec_duplicate:V2HI
	    (match_operand:HI 2 "register_operand"              "   r,   r,   r,   r"))
	  (match_operand:SI 3 "imm_1_2_operand"                 " v01, v01, v02, v02")))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   pkbb16\t%0, %2, %1
   pkbt16\t%0, %2, %1
   pkbb16\t%0, %1, %2
   pktb16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V2HI")])

(define_insn "vec_merge<mode>"
  [(set (match_operand:VSHI 0 "register_operand"               "= r,   r,   r,   r,   r,   r,   r,   r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (vec_select:<VNHALF>
	      (match_operand:VSHI 1 "register_operand"         "  r,   r,   r,   r,   r,   r,   r,   r")
	      (parallel [(match_operand:SI 4 "imm_0_1_operand" "v00, v00, v01, v01, v00, v00, v01, v01")])))
	  (vec_duplicate:VSHI
	    (vec_select:<VNHALF>
	      (match_operand:VSHI 2 "register_operand"         "  r,   r,   r,   r,   r,   r,   r,   r")
	      (parallel [(match_operand:SI 5 "imm_0_1_operand" "v00, v01, v01, v00, v00, v01, v01, v00")])))
	  (match_operand:SI 3 "imm_1_2_operand"                "v01, v01, v01, v01, v02, v02, v02, v02")))]
  "TARGET_DSP"
  "@
   pkbb<bits>\t%0, %2, %1
   pktb<bits>\t%0, %2, %1
   pktt<bits>\t%0, %2, %1
   pkbt<bits>\t%0, %2, %1
   pkbb<bits>\t%0, %1, %2
   pkbt<bits>\t%0, %1, %2
   pktt<bits>\t%0, %1, %2
   pktb<bits>\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "<MODE>")])

(define_insn "vec_extractv4qi0"
  [(set (match_operand:QI 0 "register_operand"         "=r,r")
	(vec_select:QI
	  (match_operand:V4QI 1 "nonimmediate_operand" " r,m")
	  (parallel [(const_int 0)])))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   and\t%0,%1,0xff
   lbu\t%0,%1"
  [(set_attr "move_type" "andi,load")
   (set_attr "mode" "QI")])

(define_insn_and_split "vec_extractv4qi0_se"
  [(set (match_operand:SI 0 "register_operand"         "=r,r")
	(sign_extend:SI
	  (vec_select:QI
	    (match_operand:V4QI 1 "nonimmediate_operand" "r,m")
	    (parallel [(const_int 0)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   bfos\t%0,%1,7,0
   lb\t%0,%1"
  "reload_completed && REG_P (operands[1]) && !TARGET_BFO"
  [(set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (match_dup 2)))]
{
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (SImode)
			 - GET_MODE_BITSIZE (QImode));
}
  [(set_attr "move_type" "shift_shift,load")
   (set_attr "mode" "SI")])

(define_insn "vec_extractv4qi3_se"
  [(set (match_operand:SI 0 "register_operand"       "=r")
	(sign_extend:SI
	  (vec_select:QI
	    (match_operand:V4QI 1 "register_operand" " r")
	    (parallel [(const_int 3)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "srai\t%0, %1, 24"
  [(set_attr "mode" "V4QI")])

(define_insn "vec_extractv4qi0_ze"
  [(set (match_operand:SI 0 "register_operand"         "=r,r")
	(zero_extend:SI
	  (vec_select:QI
	    (match_operand:V4QI 1 "nonimmediate_operand" "r,m")
	    (parallel [(const_int 0)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   and\t%0,%1,0xff
   lbu\t%0,%1"
  [(set_attr "move_type" "andi,load")
   (set_attr "mode" "QI")])

(define_insn "vec_extractv4qi3_ze"
  [(set (match_operand:SI 0 "register_operand"       "=r")
	(zero_extend:SI
	  (vec_select:QI
	    (match_operand:V4QI 1 "register_operand" " r")
	    (parallel [(const_int 3)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "srli\t%0, %1, 24"
  [(set_attr "mode" "V4QI")])

(define_insn_and_split "vec_extractv4qihi0"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(sign_extend:HI
	  (vec_select:QI
	    (match_operand:V4QI 1 "register_operand" " r")
	     (parallel [(const_int 0)]))))]
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  "#"
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  [(const_int 1)]
{
  rtx tmp = gen_reg_rtx (QImode);
  emit_insn (gen_vec_extractv4qi0 (tmp, operands[1]));
  emit_insn (gen_extendqihi2 (operands[0], tmp));
  DONE;
}
  [(set_attr "mode"  "HI")])

(define_insn_and_split "vec_extractv2hi0"
  [(set (match_operand:HI 0 "register_operand"         "=r,r")
	(vec_select:HI
	  (match_operand:V2HI 1 "nonimmediate_operand" " r,m")
	  (parallel [(const_int 0)])))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   bfos\t%0,%1,15,0
   lh\t%0,%1"
  "reload_completed && REG_P (operands[1]) && !TARGET_BFO"
  [(set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (match_dup 2)))]
{
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (SImode)
			 - GET_MODE_BITSIZE (HImode));
}
  [(set_attr "move_type" "shift_shift,load")
   (set_attr "mode" "HI")])

(define_insn_and_split "vec_extractv2hi0_se"
  [(set (match_operand:SI 0 "register_operand"         "=r, r")
	(sign_extend:SI
	  (vec_select:HI
	    (match_operand:V2HI 1 "nonimmediate_operand" "r,m")
	    (parallel [(const_int 0)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   bfos\t%0,%1,15,0
   lh\t%0,%1"
  "reload_completed && REG_P (operands[1]) && !TARGET_BFO"
  [(set (match_dup 0) (ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0) (ashiftrt:SI (match_dup 0) (match_dup 2)))]
{
  operands[0] = gen_lowpart (SImode, operands[0]);
  operands[1] = gen_lowpart (SImode, operands[1]);
  operands[2] = GEN_INT (GET_MODE_BITSIZE (SImode)
			 - GET_MODE_BITSIZE (HImode));
}
  [(set_attr "move_type" "shift_shift,load")
   (set_attr "mode" "SI")])

(define_insn_and_split "vec_extractv2hi0_ze"
  [(set (match_operand:SI 0 "register_operand"         "=r, r")
	(zero_extend:SI
	  (vec_select:HI
	    (match_operand:V2HI 1 "nonimmediate_operand" "r, m")
	    (parallel [(const_int 0)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   bfoz\t%0,%1,15,0
   lhu\t%0,%1"
  "&& reload_completed && REG_P (operands[1]) && !TARGET_BFO"
  [(set (match_dup 0)
	(ashift:SI (match_dup 1) (match_dup 2)))
   (set (match_dup 0)
	(lshiftrt:SI (match_dup 0) (match_dup 2)))]
  {
    operands[1] = gen_lowpart (SImode, operands[1]);
    operands[2] = GEN_INT(GET_MODE_BITSIZE(SImode) - 16);
  }
  [(set_attr "move_type" "shift_shift,load")
   (set_attr "mode" "SI")])


(define_insn "vec_extractv2hi1"
  [(set (match_operand:HI 0 "register_operand"     "=r")
	(vec_select:HI
	  (match_operand:V2HI 1 "register_operand" " r")
	  (parallel [(const_int 1)])))]
  "TARGET_DSP && !TARGET_64BIT"
  "srai\t%0, %1, 16"
  [(set_attr "mode"    "V2HI")])

(define_insn "vec_extractv2hi1_se"
  [(set (match_operand:SI 0 "register_operand"     "=r")
	(sign_extend:SI
	  (vec_select:HI
	    (match_operand:V2HI 1 "register_operand" "r")
	    (parallel [(const_int 1)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "srai\t%0, %1, 16"
)

(define_insn "vec_extractv2hi1_ze"
  [(set (match_operand:SI 0 "register_operand"     "=r")
	(zero_extend:SI
	  (vec_select:HI
	    (match_operand:V2HI 1 "register_operand" "r")
	    (parallel [(const_int 1)]))))]
  "TARGET_DSP && !TARGET_64BIT"
  "srli\t%0, %1, 16"
)

(define_expand "pkbb64"
  [(match_operand:V4HI 0 "register_operand")
   (match_operand:V4HI 1 "register_operand")
   (match_operand:V4HI 2 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vec_pkbb64 (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "vec_pkbb64"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 0) (const_int 4)
		    (const_int 2) (const_int 6)])))]
  "TARGET_DSP && TARGET_64BIT"
  "pkbb16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V4HI")])

(define_expand "pkbt64"
  [(match_operand:V4HI 0 "register_operand")
   (match_operand:V4HI 1 "register_operand")
   (match_operand:V4HI 2 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vec_pkbt64 (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "vec_pkbt64"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 0) (const_int 5)
		    (const_int 2) (const_int 7)])))]
  "TARGET_DSP && TARGET_64BIT"
  "pkbt16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V4HI")])

(define_expand "pktt64"
  [(match_operand:V4HI 0 "register_operand")
   (match_operand:V4HI 1 "register_operand")
   (match_operand:V4HI 2 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vec_pktt64 (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "vec_pktt64"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 1) (const_int 5)
		    (const_int 3) (const_int 7)])))]
  "TARGET_DSP && TARGET_64BIT"
  "pktt16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V4HI")])

(define_expand "pktb64"
  [(match_operand:V4HI 0 "register_operand")
   (match_operand:V4HI 1 "register_operand")
   (match_operand:V4HI 2 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vec_pktb64 (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "vec_pktb64"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_select:V4HI
	 (vec_concat:V8HI (match_operand:V4HI 1 "register_operand" "r")
			  (match_operand:V4HI 2 "register_operand" "r"))
	 (parallel [(const_int 1) (const_int 4)
		    (const_int 3) (const_int 6)])))]
  "TARGET_DSP && TARGET_64BIT"
  "pktb16\t%0, %1, %2"
  [(set_attr "type" "dpack")
   (set_attr "mode" "V4HI")])

(define_insn "<su>mul16"
  [(set (match_operand:V2SI 0 "register_operand"                             "=r")
	(mult:V2SI (any_extend:V2SI (match_operand:V2HI 1 "register_operand" "%r"))
		   (any_extend:V2SI (match_operand:V2HI 2 "register_operand" " r"))))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>mul16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V2SI")])

(define_insn "smul16_64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_SMUL16))]
  "TARGET_DSP && TARGET_64BIT"
  "smul16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "umul16_64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_UMUL16))]
  "TARGET_DSP && TARGET_64BIT"
  "umul16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "smul8"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_SMUL8))]
  "TARGET_DSP"
  "smul8\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "umul8"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_UMUL8))]
  "TARGET_DSP"
  "umul8\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "<su>mulx16"
  [(set (match_operand:V2SI 0 "register_operand"         "=r")
	(vec_merge:V2SI
	  (vec_duplicate:V2SI
	    (mult:SI
	      (any_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 1 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (any_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" " r")
		  (parallel [(const_int 1)])))))
	  (vec_duplicate:V2SI
	    (mult:SI
	      (any_extend:SI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(const_int 1)])))
	      (any_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))))
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>mulx16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V2SI")])

(define_insn "smulx16_64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_SMULX16))]
  "TARGET_DSP && TARGET_64BIT"
  "smulx16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "umulx16_64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_UMULX16))]
  "TARGET_DSP && TARGET_64BIT"
  "umulx16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "smulx8"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_SMULX8))]
  "TARGET_DSP"
  "smulx8\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "umulx8"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(unspec:DI [(match_operand:SI 1 "register_operand" " r")
		    (match_operand:SI 2 "register_operand" " r")]
		    UNSPEC_UMULX8))]
  "TARGET_DSP"
  "umulx8\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "v4qi_dup_10"
  [(set (match_operand:V4QI 0 "register_operand"    "=r")
	(vec_select:V4QI
	   (match_operand:V4QI 1 "register_operand" " r")
	   (parallel [(const_int 0) (const_int 1) (const_int 0) (const_int 1)])))]
  "TARGET_DSP && !TARGET_64BIT"
  "pkbb\t%0, %1, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4QI")])

(define_insn "v4qi_dup_32"
  [(set (match_operand:V4QI 0 "register_operand"    "=r")
	(vec_select:V4QI
	   (match_operand:V4QI 1 "register_operand" " r")
	   (parallel [(const_int 2) (const_int 3) (const_int 2) (const_int 3)])))]
  "TARGET_DSP && !TARGET_64BIT"
  "pktt\t%0, %1, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4QI")])

(define_expand "vec_unpacks_lo_v4qi"
  [(match_operand:V2HI 0 "register_operand" "=r")
   (match_operand:V4QI 1 "register_operand" " r")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sunpkd810 (operands[0], operands[1]));
  DONE;
})

(define_expand "sunpkd810"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sunpkd810_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "<zs>unpkd810_imp"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd810\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_insn "<zs>unpkd810_imp_inv"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd810\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_expand "sunpkd820"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sunpkd820_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "<zs>unpkd820_imp"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 2)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd820\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_insn "<zs>unpkd820_imp_inv"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 2)]))))
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd820\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_expand "sunpkd830"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sunpkd830_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "<zs>unpkd830_imp"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 3)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd830\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_insn "<zs>unpkd830_imp_inv"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 3)]))))
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd830\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_expand "sunpkd831"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sunpkd831_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "<zs>unpkd831_imp"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 3)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd831\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_insn "<zs>unpkd831_imp_inv"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 1)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 3)]))))
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd831\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_expand "sunpkd832"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sunpkd832_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "<zs>unpkd832_imp"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 3)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 2)]))))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd832\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_insn "<zs>unpkd832_imp_inv"
  [(set (match_operand:V2HI 0 "register_operand"                     "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_operand:V4QI 1 "register_operand"             " r")
		(parallel [(const_int 2)]))))
	  (vec_duplicate:V2HI
	    (any_extend:HI
	      (vec_select:QI
		(match_dup 1)
		(parallel [(const_int 3)]))))
	  (const_int 1)))]
  "TARGET_DSP && !TARGET_64BIT"
  "<zs>unpkd832\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V2HI")])

(define_expand "zunpkd810"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_zunpkd810_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_expand "zunpkd820"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_zunpkd820_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_expand "zunpkd830"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_zunpkd830_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_expand "zunpkd831"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_zunpkd831_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_expand "zunpkd832"
  [(match_operand:V2HI 0 "register_operand")
   (match_operand:V4QI 1 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_zunpkd832_imp (operands[0], operands[1]));
  DONE;
}
[(set_attr "type" "dpack")])

(define_insn "<zs>unpkd810_64"
  [(set (match_operand:V4HI 0 "register_operand"                    "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 0) (const_int 1)
		     (const_int 4) (const_int 5)])))]
  "TARGET_DSP && TARGET_64BIT"
  "<zs>unpkd810\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4HI")])

(define_insn "<zs>unpkd820_64"
  [(set (match_operand:V4HI 0 "register_operand"                    "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 0) (const_int 2)
		     (const_int 4) (const_int 6)])))]
  "TARGET_DSP && TARGET_64BIT"
  "<zs>unpkd820\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4HI")])

(define_insn "<zs>unpkd830_64"
  [(set (match_operand:V4HI 0 "register_operand"                    "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 0) (const_int 3)
		     (const_int 4) (const_int 7)])))]
  "TARGET_DSP && TARGET_64BIT"
  "<zs>unpkd830\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4HI")])

(define_insn "<zs>unpkd831_64"
  [(set (match_operand:V4HI 0 "register_operand"                    "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 1) (const_int 3)
		     (const_int 5) (const_int 7)])))]
  "TARGET_DSP && TARGET_64BIT"
  "<zs>unpkd831\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4HI")])

(define_insn "<zs>unpkd832_64"
  [(set (match_operand:V4HI 0 "register_operand"                    "=r")
	(vec_select:V4HI
	  (any_extend:V8HI (match_operand:V8QI 1 "register_operand" "r"))
	  (parallel [(const_int 2) (const_int 3)
		     (const_int 6) (const_int 7)])))]
  "TARGET_DSP && TARGET_64BIT"
  "<zs>unpkd832\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode"  "V4HI")])

(define_expand "smbb"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_mulhisi3v (operands[0], operands[1], operands[2],
			    GEN_INT (0), GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smbt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_mulhisi3v (operands[0], operands[1], operands[2],
			    GEN_INT (0), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smtt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_mulhisi3v (operands[0], operands[1], operands[2],
			    GEN_INT (1), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "mulhisi3v"
  [(set (match_operand:SI 0 "register_operand"                  "=  r,   r,   r,   r")
	(mult:SI
	  (sign_extend:SI
	     (vec_select:HI
	       (match_operand:V2HI 1 "register_operand"         "   r,   r,   r,   r")
	       (parallel [(match_operand:SI 3 "imm_0_1_operand" " v00, v00, v01, v01")])))
	  (sign_extend:SI (vec_select:HI
	       (match_operand:V2HI 2 "register_operand"         "   r,   r,   r,   r")
	       (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v01, v01, v00")])))))]
  "TARGET_DSP && !TARGET_64BIT"
{
  const char *pats[] = { "smbb16\t%0, %1, %2",
			 "smbt16\t%0, %1, %2",
			 "smtt16\t%0, %1, %2",
			 "smbt16\t%0, %2, %1" };
  return pats[which_alternative];
}
  [(set_attr "type"   "dmul")
   (set_attr "mode"   "SI")])

(define_expand "smbb64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_mulv2hiv2si3v (operands[0], operands[1], operands[2],
				GEN_INT (0), GEN_INT (2), GEN_INT (0), GEN_INT (2)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smbt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_mulv2hiv2si3v (operands[0], operands[1], operands[2],
				GEN_INT (0), GEN_INT (2), GEN_INT (1), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smtt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_mulv2hiv2si3v (operands[0], operands[1], operands[2],
				GEN_INT (1), GEN_INT (3), GEN_INT (1), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "mulv2hiv2si3v"
  [(set (match_operand:V2SI 0 "register_operand" "=r, r, r")
        (mult:V2SI
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 1 "register_operand" "r, r, r")
	   (parallel [(match_operand:SI 3 "imm2u_operand" " v00, v00, v01")
		      (match_operand:SI 4 "imm2u_operand" " v02, v02, v03")])))
	 (sign_extend:V2SI
	  (vec_select:V2HI
	   (match_operand:V4HI 2 "register_operand" "r, r, r")
	   (parallel [(match_operand:SI 5 "imm2u_operand" " v00, v01, v01")
		      (match_operand:SI 6 "imm2u_operand" " v02, v03, v03")])))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   smbb16\t%0, %1, %2
   smbt16\t%0, %1, %2
   smtt16\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode"   "DI")])

(define_expand "smbb32"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_mulsidi3v (operands[0], operands[1], operands[2],
			    GEN_INT (0), GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smbt32"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_mulsidi3v (operands[0], operands[1], operands[2],
			    GEN_INT (0), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smtt32"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_mulsidi3v (operands[0], operands[1], operands[2],
			    GEN_INT (1), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "mulsidi3v"
  [(set (match_operand:DI 0 "register_operand"                  "=  r,   r,   r,   r")
	(mult:DI
	  (sign_extend:DI
	     (vec_select:SI
	       (match_operand:V2SI 1 "register_operand"         "   r,   r,   r,   r")
	       (parallel [(match_operand:SI 3 "imm_0_1_operand" " v00, v00, v01, v01")])))
	  (sign_extend:DI (vec_select:SI
	       (match_operand:V2SI 2 "register_operand"         "   r,   r,   r,   r")
	       (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v01, v01, v00")])))))]
  "TARGET_DSP && TARGET_64BIT"
{
  const char *pats[] = { "smbb32\t%0, %1, %2",
			 "smbt32\t%0, %1, %2",
			 "smtt32\t%0, %1, %2",
			 "smbt32\t%0, %2, %1" };
  return pats[which_alternative];
}
  [(set_attr "type"   "dmul")
   (set_attr "mode"   "DI")])

(define_expand "kmabb"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kma_internal (operands[0], operands[2], operands[3],
			       GEN_INT (0), GEN_INT (0),
			       operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmabt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kma_internal (operands[0], operands[2], operands[3],
			       GEN_INT (0), GEN_INT (1),
			       operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmatt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kma_internal (operands[0], operands[2], operands[3],
			       GEN_INT (1), GEN_INT (1),
			       operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kma_internal"
  [(set (match_operand:SI 0 "register_operand"                   "=   r,   r,   r,   r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI
	      (vec_select:HI
		(match_operand:V2HI 1 "register_operand"         "   r,   r,   r,   r")
	        (parallel [(match_operand:SI 3 "imm_0_1_operand" " v00, v00, v01, v01")])))
	    (sign_extend:SI
	      (vec_select:HI
	        (match_operand:V2HI 2 "register_operand"         "   r,   r,   r,   r")
	        (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v01, v01, v00")]))))
	  (match_operand:SI 5 "register_operand"                 "   0,   0,   0,   0")))]
  "TARGET_DSP && !TARGET_64BIT"
{
  const char *pats[] = { "kmabb\t%0, %1, %2",
			 "kmabt\t%0, %1, %2",
			 "kmatt\t%0, %1, %2",
			 "kmabt\t%0, %2, %1" };
  return pats[which_alternative];
}
  [(set_attr "type" "dmac")
   (set_attr "mode" "SI")])

(define_expand "kmabb64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kma64_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), GEN_INT (2), GEN_INT (0),
				 GEN_INT (2), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmabt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kma64_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), GEN_INT (2), GEN_INT (1),
				 GEN_INT (3), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmatt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kma64_internal (operands[0], operands[2], operands[3],
				 GEN_INT (1), GEN_INT (3), GEN_INT (1),
				 GEN_INT (3), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kma64_internal"
  [(set (match_operand:V2SI 0 "register_operand"                 "=   r,   r,   r,   r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI
	      (vec_select:V2HI
		(match_operand:V4HI 1 "register_operand"         "   r,   r,   r,   r")
	        (parallel [(match_operand:SI 3 "imm_0_1_operand" " v00, v00, v01, v01")
			   (match_operand:SI 4 "imm_2_3_operand" " v02, v02, v03, v03")])))
	    (sign_extend:V2SI
	      (vec_select:V2HI
	        (match_operand:V4HI 2 "register_operand"         "   r,   r,   r,   r")
	        (parallel [(match_operand:SI 5 "imm_0_1_operand" " v00, v01, v01, v00")
			   (match_operand:SI 6 "imm_2_3_operand" " v02, v03, v03, v02")]))))
	  (match_operand:V2SI 7 "register_operand"               "   0,   0,   0,   0")))]
  "TARGET_DSP && TARGET_64BIT"
  "@
  kmabb\t%0, %1, %2
  kmabt\t%0, %1, %2
  kmatt\t%0, %1, %2
  kmabt\t%0, %2, %1"
  [(set_attr "type" "dmac")
   (set_attr "mode" "V2SI")])

(define_expand "kmabb32"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V2SI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kma32_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), GEN_INT (0),
				 operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmabt32"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V2SI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kma32_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), GEN_INT (1),
				 operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmatt32"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V2SI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kma32_internal (operands[0], operands[2], operands[3],
				 GEN_INT (1), GEN_INT (1),
				 operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kma32_internal"
  [(set (match_operand:DI 0 "register_operand"                   "=   r,   r,   r,   r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:SI
		(match_operand:V2SI 1 "register_operand"         "   r,   r,   r,   r")
	        (parallel [(match_operand:SI 3 "imm_0_1_operand" " v00, v00, v01, v01")])))
	    (sign_extend:DI
	      (vec_select:SI
	        (match_operand:V2SI 2 "register_operand"         "   r,   r,   r,   r")
	        (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v01, v01, v00")]))))
	  (match_operand:DI 5 "register_operand"                 "   0,   0,   0,   0")))]
  "TARGET_DSP && TARGET_64BIT"
  "@
  kmabb32\t%0, %1, %2
  kmabt32\t%0, %1, %2
  kmatt32\t%0, %1, %2
  kmabt32\t%0, %2, %1"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_expand "smds"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smds_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smds_le"
  [(set (match_operand:SI 0 "register_operand"                         "=r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" " r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" " r")
			      (parallel [(const_int 1)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
{
}
[(set_attr "type" "dmac")])

(define_insn "smds32"
  [(set (match_operand:DI 0 "register_operand"                         "=r")
	(minus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" " r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" " r")
			      (parallel [(const_int 1)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smds32\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "smds64"
  [(set (match_operand:V2SI 0 "register_operand"                      "=r")
	(minus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smds\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_expand "smdrs"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smdrs_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smdrs_le"
  [(set (match_operand:SI 0 "register_operand"                         "=r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" " r")
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" " r")
			      (parallel [(const_int 0)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
{
}
[(set_attr "type" "dmac")])

(define_insn "smdrs32"
  [(set (match_operand:DI 0 "register_operand"                         "=r")
	(minus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" " r")
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" " r")
			      (parallel [(const_int 0)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smdrs32\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "smdrs64"
  [(set (match_operand:V2SI 0 "register_operand"                      "=r")
	(minus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smdrs\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_expand "smxdsv"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smxdsv_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dmac")])


(define_expand "smxdsv_le"
  [(set (match_operand:SI 0 "register_operand"                         "=r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" " r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" " r")
			      (parallel [(const_int 0)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
{
}
[(set_attr "type" "dmac")])

(define_insn "smxds32"
  [(set (match_operand:DI 0 "register_operand"                         "=r")
	(minus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" " r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" " r")
			      (parallel [(const_int 0)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smxds32\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "smxds64"
  [(set (match_operand:V2SI 0 "register_operand"                      "=r")
	(minus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smxds\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "smal1"
  [(set (match_operand:DI 0 "register_even_operand"             "=r")
	(plus:DI (match_operand:DI 1 "register_even_operand"    " r")
	  (sign_extend:DI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal2"
  [(set (match_operand:DI 0 "register_even_operand"           "=r")
	(plus:DI (match_operand:DI 1 "register_even_operand"  " r")
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" " r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_dup 2)
		(parallel [(const_int 1)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal3"
  [(set (match_operand:DI 0 "register_even_operand"             "=r")
	(plus:DI (match_operand:DI 1 "register_even_operand"    " r")
	  (sign_extend:DI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" " r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal4"
  [(set (match_operand:DI 0 "register_even_operand"           "=r")
	(plus:DI (match_operand:DI 1 "register_even_operand"  " r")
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" " r")
		(parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_dup 2)
		(parallel [(const_int 0)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal5"
  [(set (match_operand:DI 0 "register_even_operand"             "=r")
	(plus:DI
	  (sign_extend:DI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 1)])))))
	  (match_operand:DI 1 "register_even_operand"           " r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal6"
  [(set (match_operand:DI 0 "register_even_operand"           "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" " r")
		(parallel [(const_int 0)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (match_operand:DI 1 "register_even_operand"         " r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal7"
  [(set (match_operand:DI 0 "register_even_operand"             "=r")
	(plus:DI
	  (sign_extend:DI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand" " r")
		  (parallel [(const_int 1)])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(const_int 0)])))))
	  (match_operand:DI 1 "register_even_operand"           " r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal8"
  [(set (match_operand:DI 0 "register_even_operand"           "=r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand" " r")
		(parallel [(const_int 1)])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_dup 2)
		(parallel [(const_int 0)]))))
	  (match_operand:DI 1 "register_even_operand"         " r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "smal1_64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(plus:DI (match_operand:DI 1 "register_operand"    " r")
	  (plus:DI
	    (sign_extend:DI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand" " r")
		    (parallel [(const_int 0)])))
		(sign_extend:SI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))))
	    (sign_extend:DI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		(sign_extend:SI
		  (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smal\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_insn "dsp_extendqihi2"
  [(set (match_operand:HI 0 "register_operand"                 "=r")
	(sign_extend:HI (match_operand:QI 1 "register_operand" " r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "sunpkd820\t%0, %1"
  [(set_attr "type" "dpack")
   (set_attr "mode" "HI")])

(define_insn "smulsi3_highpart"
  [(set (match_operand:SI 0 "register_operand"                       "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" " r"))
	      (sign_extend:DI (match_operand:SI 2 "register_operand" " r")))
	    (const_int 32))))]
  "TARGET_DSP && TARGET_64BIT"
  "smmul\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "HI")])

(define_insn "smmul_round"
  [(set (match_operand:SI 0 "register_operand"                       "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (unspec:DI [(mult:DI
		  	  (sign_extend:DI (match_operand:SI 1 "register_operand" " r"))
			  (sign_extend:DI (match_operand:SI 2 "register_operand" " r")))]
		       UNSPEC_ROUND)
	    (const_int 32))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smmul.u\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "HI")])

(define_insn "smulv2si3_highpart"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (mult:V2DI (sign_extend:V2DI (match_operand:V2SI 1 "register_operand" " r"))
		       (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r")))
	    (const_int 32))))]
  "TARGET_DSP && TARGET_64BIT"
  "smmul\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V2SI")])

(define_insn "smmulv2si3_round"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (unspec:V2DI [(mult:V2DI
			    (sign_extend:V2DI (match_operand:V2SI 1 "register_operand" " r"))
			    (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r")))]
			  UNSPEC_ROUND)
	    (const_int 32))))]
  "TARGET_DSP  && TARGET_64BIT"
  "smmul.u\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "kmmac"
  [(set (match_operand:SI 0 "register_operand"                         "=r")
	(ss_plus:SI (match_operand:SI 1 "register_operand"             " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 2 "register_operand" " r"))
		(sign_extend:DI (match_operand:SI 3 "register_operand" " r")))
	      (const_int 32)))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmmac\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmac_64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_plus:V2SI (match_operand:V2SI 1 "register_operand"             " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (mult:V2DI
		(sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r"))
		(sign_extend:V2DI (match_operand:V2SI 3 "register_operand" " r")))
	      (const_int 32)))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmmac\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmac_round"
  [(set (match_operand:SI 0 "register_operand"                                     "=r")
	(ss_plus:SI (match_operand:SI 1 "register_operand"                         " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI [(mult:DI
			    (sign_extend:DI (match_operand:SI 2 "register_operand" " r"))
			    (sign_extend:DI (match_operand:SI 3 "register_operand" " r")))]
			 UNSPEC_ROUND)
	      (const_int 32)))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmmac.u\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmac64_round"
  [(set (match_operand:V2SI 0 "register_operand"                                     "=r")
	(ss_plus:V2SI (match_operand:V2SI 1 "register_operand"                       " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (unspec:V2DI [(mult:V2DI
			    (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r"))
			    (sign_extend:V2DI (match_operand:V2SI 3 "register_operand" " r")))]
			 UNSPEC_ROUND)
	      (const_int 32)))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmmac.u\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmsb"
  [(set (match_operand:SI 0 "register_operand"                         "=r")
	(ss_minus:SI (match_operand:SI 1 "register_operand"            " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 2 "register_operand" " r"))
		(sign_extend:DI (match_operand:SI 3 "register_operand" " r")))
	      (const_int 32)))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmmsb\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmsb_64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_minus:V2SI (match_operand:V2SI 1 "register_operand"             " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (mult:V2DI
		(sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r"))
		(sign_extend:V2DI (match_operand:V2SI 3 "register_operand" " r")))
	      (const_int 32)))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmmsb\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmsb_round"
  [(set (match_operand:SI 0 "register_operand"                                     "=r")
	(ss_minus:SI (match_operand:SI 1 "register_operand"                        " 0")
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI [(mult:DI
			    (sign_extend:DI (match_operand:SI 2 "register_operand" " r"))
			    (sign_extend:DI (match_operand:SI 3 "register_operand" " r")))]
			 UNSPEC_ROUND)
	      (const_int 32)))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmmsb.u\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmmsb64_round"
  [(set (match_operand:V2SI 0 "register_operand"                                     "=r")
	(ss_minus:V2SI (match_operand:V2SI 1 "register_operand"                       " 0")
	  (truncate:V2SI
	    (lshiftrt:V2DI
	      (unspec:V2DI [(mult:V2DI
			    (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r"))
			    (sign_extend:V2DI (match_operand:V2SI 3 "register_operand" " r")))]
			 UNSPEC_ROUND)
	      (const_int 32)))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmmsb.u\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kwmmul"
  [(set (match_operand:SI 0 "register_operand"                       "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (ss_mult:DI
	      (mult:DI (sign_extend:DI (match_operand:SI 1 "register_operand" " r")) (const_int 2))
	      (mult:DI (sign_extend:DI (match_operand:SI 2 "register_operand" " r")) (const_int 2)))
	    (const_int 32))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kwmmul\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_insn "kwmmul_64"
  [(set (match_operand:V2SI 0 "register_operand"                       "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (ss_mult:V2DI
	      (mult:V2DI (sign_extend:V2DI (match_operand:V2SI 1 "register_operand" " r")) (const_int 2))
	      (mult:V2DI (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r")) (const_int 2)))
	    (const_int 32))))]
  "TARGET_DSP && TARGET_64BIT"
  "kwmmul\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_insn "kwmmul_round"
  [(set (match_operand:SI 0 "register_operand"                       "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (unspec:DI [
	      (ss_mult:DI
		(mult:DI (sign_extend:DI (match_operand:SI 1 "register_operand" " r")) (const_int 2))
		(mult:DI (sign_extend:DI (match_operand:SI 2 "register_operand" " r")) (const_int 2)))]
	      UNSPEC_ROUND)
	    (const_int 32))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kwmmul.u\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_insn "kwmmul64_round"
  [(set (match_operand:V2SI 0 "register_operand"                       "=r")
	(truncate:V2SI
	  (lshiftrt:V2DI
	    (unspec:V2DI [
	      (ss_mult:V2DI
		(mult:V2DI (sign_extend:V2DI (match_operand:V2SI 1 "register_operand" " r")) (const_int 2))
		(mult:V2DI (sign_extend:V2DI (match_operand:V2SI 2 "register_operand" " r")) (const_int 2)))]
	      UNSPEC_ROUND)
	    (const_int 32))))]
  "TARGET_DSP && TARGET_64BIT"
  "kwmmul.u\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_expand "smmwb"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smulhisi3_highpart_1 (operands[0], operands[1], operands[2], GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smmwt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smulhisi3_highpart_1 (operands[0], operands[1], operands[2], GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "smulhisi3_highpart_1"
  [(set (match_operand:SI 0 "register_operand"                       "= r,   r")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "  r,   r"))
	      (sign_extend:DI
	        (vec_select:HI
		  (match_operand:V2HI 2 "register_operand"           "  r,   r")
		  (parallel [(match_operand:SI 3 "imm_0_1_operand"   "v00, v01")]))))
	    (const_int 16))))]
  "TARGET_DSP && !TARGET_64BIT"
{
  const char *pats[] = { "smmwb\t%0, %1, %2",
			 "smmwt\t%0, %1, %2" };
  return pats[which_alternative];
}
[(set_attr "type" "dmul")])

(define_insn "smulhisi3_highpart_2"
  [(set (match_operand:SI 0 "register_operand"                       "= r,   r")
	(truncate:SI
	  (lshiftrt:DI
	    (mult:DI
	      (sign_extend:DI
	        (vec_select:HI
		  (match_operand:V2HI 1 "register_operand"           "  r,   r")
		  (parallel [(match_operand:SI 3 "imm_0_1_operand"   "v00, v01")])))
	      (sign_extend:DI (match_operand:SI 2 "register_operand" "  r,   r")))
	    (const_int 16))))]
  "TARGET_DSP && !TARGET_64BIT"
{
  const char *pats[] = { "smmwb\t%0, %1, %2",
			 "smmwt\t%0, %1, %2" };
  return pats[which_alternative];
}
[(set_attr "type" "dmul")])

(define_expand "smmwb64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smulhisi3_highpart_64 (operands[0], operands[1], operands[2],
					GEN_INT (0), GEN_INT (2)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smmwt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smulhisi3_highpart_64 (operands[0], operands[1], operands[2],
					GEN_INT (1), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "smulhisi3_highpart_64"
  [(set (match_operand:V2SI 0 "register_operand"                       "=r,    r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_operand:V2SI 1 "register_operand"           "  r,   r")
		      (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand"           "  r,   r")
		      (parallel [(match_operand:SI 3 "imm_0_1_operand"   "v00, v01")]))))
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_dup 1)
		      (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_dup 2)
		      (parallel [(match_operand:SI 4 "imm_2_3_operand"   "v02, v03")]))))
	      (const_int 16)))))]
  "TARGET_DSP && TARGET_64BIT"
{
  const char *pats[] = { "smmwb\t%0, %1, %2",
			 "smmwt\t%0, %1, %2" };
  return pats[which_alternative];
}
[(set_attr "type" "dmul")])

(define_expand "kmmwb64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmw64 (operands[0], operands[1], operands[2],
			  GEN_INT (0), GEN_INT (2)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "kmmwt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmw64 (operands[0], operands[1], operands[2],
			 GEN_INT (1), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "kmmw64"
  [(set (match_operand:V2SI 0 "register_operand"                       "=r,    r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI [(mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_operand:V2SI 1 "register_operand"           "  r,   r")
		      (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand"           "  r,   r")
		      (parallel [(match_operand:SI 3 "imm_0_1_operand"   "v00, v01")]))))]
	      UNSPEC_KMMW)
	      (const_int 15)))
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI [(mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_dup 1)
		      (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_dup 2)
		      (parallel [(match_operand:SI 4 "imm_2_3_operand"   "v02, v03")]))))]
	      UNSPEC_KMMW)
	      (const_int 15)))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   kmmwb2\t%0, %1, %2
   kmmwt2\t%0, %1, %2"
[(set_attr "type" "dmul")])

(define_expand "smmwb_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smmw_round_internal (operands[0], operands[1], operands[2], GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smmwt_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smmw_round_internal (operands[0], operands[1], operands[2], GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "smmw_round_internal"
  [(set (match_operand:SI 0 "register_operand"                          "=  r,   r")
	(truncate:SI
	  (lshiftrt:DI
	    (unspec:DI
	      [(mult:DI
		 (sign_extend:DI (match_operand:SI 1 "register_operand" "   r,   r"))
		 (sign_extend:DI
		   (vec_select:HI
		     (match_operand:V2HI 2 "register_operand"           "   r,   r")
		     (parallel [(match_operand:SI 3 "imm_0_1_operand"   " v00, v01")]))))]
	      UNSPEC_ROUND)
	    (const_int 16))))]
  "TARGET_DSP && !TARGET_64BIT"
{
  const char *pats[] = { "smmwb.u\t%0, %1, %2",
			 "smmwt.u\t%0, %1, %2" };
  return pats[which_alternative];
}
[(set_attr "type" "dmul")])

(define_expand "smmwb64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smmw64_round_internal (operands[0], operands[1], operands[2],
					GEN_INT (0), GEN_INT (2)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "smmwt64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smmw64_round_internal (operands[0], operands[1], operands[2],
					GEN_INT (1), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "smmw64_round_internal"
  [(set (match_operand:V2SI 0 "register_operand"                       "=r,    r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
	        [(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand"           "  r,   r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand"           "  r,   r")
		      (parallel [(match_operand:SI 3 "imm_0_1_operand"   "v00, v01")]))))]
	        UNSPEC_ROUND)
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
		[(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
		      (parallel [(match_operand:SI 4 "imm_2_3_operand"   "v02, v03")]))))]
	        UNSPEC_ROUND)
	      (const_int 16)))))]
  "TARGET_DSP && TARGET_64BIT"
{
  const char *pats[] = { "smmwb.u\t%0, %1, %2",
			 "smmwt.u\t%0, %1, %2" };
  return pats[which_alternative];
}
[(set_attr "type" "dmul")])

(define_expand "kmmwb2"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmw2_internal (operands[0], operands[1], operands[2], GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "kmmwt2"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmw2_internal (operands[0], operands[1], operands[2], GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "kmmw2_internal"
  [(set (match_operand:SI 0 "register_operand"                       "=   r,    r")
	(truncate:SI
	  (lshiftrt:DI
	    (unspec:DI [(mult:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" "    r,    r"))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand"           "    r,    r")
		  (parallel [(match_operand:SI 3 "imm_0_1_operand"   "  v00,  v01")]))))]
	    UNSPEC_KMMW)
	    (const_int 15))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   kmmwb2\t%0, %1, %2
   kmmwt2\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_expand "kmmwb2_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmw2_round_internal (operands[0], operands[1],
				       operands[2], GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "kmmwt2_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmw2_round_internal (operands[0], operands[1],
				       operands[2], GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "kmmw2_round_internal"
  [(set (match_operand:SI 0 "register_operand"                          "=   r,    r")
	(truncate:SI
	  (lshiftrt:DI
	    (unspec:DI
	      [(ss_mult:DI
		 (sign_extend:DI (match_operand:SI 1 "register_operand" "    r,    r"))
		 (sign_extend:DI
		   (vec_select:HI
		     (match_operand:V2HI 2 "register_operand"           "    r,    r")
		     (parallel [(match_operand:SI 3 "imm_0_1_operand"   "  v00,  v01")]))))]
	      UNSPEC_KMMWU)
	    (const_int 15))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   kmmwb2.u\t%0, %1, %2
   kmmwt2.u\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_expand "kmmwb64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmw64_round (operands[0], operands[1], operands[2],
			       GEN_INT (0), GEN_INT (2)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_expand "kmmwt64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmw64_round (operands[0], operands[1], operands[2],
			       GEN_INT (1), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmul")])

(define_insn "kmmw64_round"
  [(set (match_operand:V2SI 0 "register_operand"                       "=r,    r")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
		[(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand"           "  r,   r")
		      (parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand"           "  r,   r")
		      (parallel [(match_operand:SI 3 "imm_0_1_operand"   "v00, v01")]))))]
		UNSPEC_KMMWU)
	      (const_int 15)))
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
		[(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
		      (parallel [(match_operand:SI 4 "imm_2_3_operand"   "v02, v03")]))))]
		UNSPEC_KMMWU)
	      (const_int 15)))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   kmmwb2.u\t%0, %1, %2
   kmmwt2.u\t%0, %1, %2"
  [(set_attr "type" "dmul")])

(define_expand "kmmawb"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), operands[1], GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_internal (operands[0], operands[2], operands[3],
				 GEN_INT (1), operands[1], GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawb2"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), operands[1], GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt2"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_internal (operands[0], operands[2], operands[3],
				 GEN_INT (1), operands[1], GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kmmaw_internal"
  [(set (match_operand:SI 0 "register_operand"                         "=  r,   r,   r,   r")
	(ss_plus:SI
	  (match_operand:SI 4 "register_operand"                       "   0,   0,   0,   0")
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "   r,   r,   r,   r"))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V2HI 2 "register_operand"         "   r,   r,   r,   r")
		      (parallel [(match_operand:SI 3 "imm_0_1_operand" " v00, v01, v00, v01")]))))
	      (match_operand:SI 5 "imm_15_16_operand"                  " v16, v16, v15, v15" )))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
  kmmawb\t%0, %1, %2
  kmmawt\t%0, %1, %2
  kmmawb2\t%0, %1, %2
  kmmawt2\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_expand "kmmawb64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_internal (operands[0], operands[2], operands[3],
				   GEN_INT (0), GEN_INT (2), operands[1],
				   GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_internal (operands[0], operands[2], operands[3],
				   GEN_INT (1), GEN_INT (3), operands[1],
				   GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawb2_64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_internal (operands[0], operands[2], operands[3],
				   GEN_INT (0), GEN_INT (2), operands[1],
				   GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt2_64"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_internal (operands[0], operands[2], operands[3],
				   GEN_INT (1), GEN_INT (3), operands[1],
				   GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kmmaw64_internal"
[(set (match_operand:V2SI 0 "register_operand" "=r, r, r, r")
      (ss_plus:V2SI
	(match_operand:V2SI 5 "register_operand" "0, 0, 0, 0")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_operand:V2SI 1 "register_operand" "r, r, r, r")
		      (parallel [(const_int 0)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand"           "  r,   r,   r,   r")
		      (parallel [(match_operand:SI 3 "imm_0_1_operand" "v00, v01, v00, v01")]))))
	      (match_operand:SI 6 "imm_15_16_operand"                  "v16, v16, v15, v15")))
	  (truncate:SI
	    (lshiftrt:DI
	      (mult:DI
		(sign_extend:DI
		  (vec_select:SI
		    (match_dup 1)
		      (parallel [(const_int 1)])))
		(sign_extend:DI
		  (vec_select:HI
		    (match_dup 2)
		      (parallel [(match_operand:SI 4 "imm_2_3_operand" "v02, v03, v02, v03")]))))
	      (match_dup 6))))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
  kmmawb\t%0, %1, %2
  kmmawt\t%0, %1, %2
  kmmawb2\t%0, %1, %2
  kmmawt2\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_expand "kmmawb_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_round_internal (operands[0], operands[2], operands[3],
				       GEN_INT (0), operands[1], GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_round_internal (operands[0], operands[2], operands[3],
				       GEN_INT (1), operands[1], GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawb2_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_round_internal (operands[0], operands[2], operands[3],
				       GEN_INT (0), operands[1], GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt2_round"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kmmaw_round_internal (operands[0], operands[2], operands[3],
				       GEN_INT (1), operands[1], GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kmmaw_round_internal"
  [(set (match_operand:SI 0 "register_operand"                            "=  r,   r,   r,   r")
	(ss_plus:SI
	  (match_operand:SI 4 "register_operand"                          "   0,   0,   0,   0")
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
		[(mult:DI
		   (sign_extend:DI (match_operand:SI 1 "register_operand" "   r,   r,   r,   r"))
		   (sign_extend:DI
		     (vec_select:HI
		       (match_operand:V2HI 2 "register_operand"           "   r,   r,   r,   r")
		       (parallel [(match_operand:SI 3 "imm_0_1_operand"   " v00, v01, v00, v01")]))))]
		UNSPEC_ROUND)
	      (match_operand:SI 5 "imm_15_16_operand"                     " v16, v16, v15, v15")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
  kmmawb.u\t%0, %1, %2
  kmmawt.u\t%0, %1, %2
  kmmawb2.u\t%0, %1, %2
  kmmawt2.u\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_expand "kmmawb64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_round_internal (operands[0], operands[2], operands[3],
					 GEN_INT (0), GEN_INT (2), operands[1],
					 GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_round_internal (operands[0], operands[2], operands[3],
					 GEN_INT (1), GEN_INT (3), operands[1],
					 GEN_INT (16)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawb2_64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_round_internal (operands[0], operands[2], operands[3],
					 GEN_INT (0), GEN_INT (2), operands[1],
					 GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kmmawt2_64_round"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V2SI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kmmaw64_round_internal (operands[0], operands[2], operands[3],
					 GEN_INT (1), GEN_INT (3), operands[1],
					 GEN_INT (15)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kmmaw64_round_internal"
[(set (match_operand:V2SI 0 "register_operand"                           "=r,    r,   r,   r")
      (ss_plus:V2SI
	(match_operand:V2SI 5 "register_operand"                         "  0,   0,   0,   0")
	(vec_concat:V2SI
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
		[(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_operand:V2SI 1 "register_operand"           "  r,   r,   r,   r")
			(parallel [(const_int 0)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_operand:V4HI 2 "register_operand"           "  r,   r,   r,   r")
			(parallel [(match_operand:SI 3 "imm_0_1_operand" "v00, v01, v00, v01")]))))]
		UNSPEC_ROUND)
	      (const_int 16)))
	  (truncate:SI
	    (lshiftrt:DI
	      (unspec:DI
		[(mult:DI
		  (sign_extend:DI
		    (vec_select:SI
		      (match_dup 1)
			(parallel [(const_int 1)])))
		  (sign_extend:DI
		    (vec_select:HI
		      (match_dup 2)
			(parallel [(match_operand:SI 4 "imm_2_3_operand" "v02, v03, v02, v03")]))))]
		  UNSPEC_ROUND)
	      (match_operand:SI 6 "imm_15_16_operand"                    "v16, v16, v15, v15"))))))]
"TARGET_DSP && TARGET_64BIT"
  "@
  kmmawb.u\t%0, %1, %2
  kmmawt.u\t%0, %1, %2
  kmmawb2.u\t%0, %1, %2
  kmmawt2.u\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_expand "smalbb"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smaddhidi (operands[0], operands[2],
			    operands[3], operands[1],
			    GEN_INT (0), GEN_INT (0)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smalbt"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smaddhidi (operands[0], operands[2],
			    operands[3], operands[1],
			    GEN_INT (0), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smaltt"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smaddhidi (operands[0], operands[2],
			    operands[3], operands[1],
			    GEN_INT (1), GEN_INT (1)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "smaddhidi"
  [(set (match_operand:DI 0 "register_even_operand"                   "=  r,   r,   r,   r")
	(plus:DI
	  (match_operand:DI 3 "register_even_operand"                 "   0,   0,   0,   0")
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 1 "register_operand"         "   r,   r,   r,   r")
		(parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v00, v01, v01")])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand"         "   r,   r,   r,   r")
		(parallel [(match_operand:SI 5 "imm_0_1_operand" " v00, v01, v01, v00")]))))))]
  "TARGET_DSP && !TARGET_64BIT"
{
    const char *pats[] = { "smalbb\t%0, %1, %2",
			   "smalbt\t%0, %1, %2",
			   "smaltt\t%0, %1, %2",
			   "smalbt\t%0, %2, %1" };
    return pats[which_alternative];
}
[(set_attr "type" "dmac")])

(define_insn "smaddhidi2"
  [(set (match_operand:DI 0 "register_even_operand"                   "=  r,   r,   r,   r")
	(plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 1 "register_operand"         "   r,   r,   r,   r")
		(parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v00, v01, v01")])))
	    (sign_extend:DI
	      (vec_select:HI
		(match_operand:V2HI 2 "register_operand"         "   r,   r,   r,   r")
		(parallel [(match_operand:SI 5 "imm_0_1_operand" " v00, v01, v01, v00")]))))
	  (match_operand:DI 3 "register_even_operand"                 "   0,   0,   0,   0")))]
  "TARGET_DSP && !TARGET_64BIT"
{
    const char *pats[] = { "smalbb\t%0, %1, %2",
			   "smalbt\t%0, %1, %2",
			   "smaltt\t%0, %1, %2",
			   "smalbt\t%0, %2, %1" };
    return pats[which_alternative];
}
[(set_attr "type" "dmac")])

(define_expand "smalbb64"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smaddhidi64 (operands[0], operands[2],
			      operands[3], operands[1],
			      GEN_INT (0), GEN_INT (0),
			      GEN_INT (2), GEN_INT (2)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smalbt64"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smaddhidi64 (operands[0], operands[2],
			      operands[3], operands[1],
			      GEN_INT (0), GEN_INT (1),
			      GEN_INT (2), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smaltt64"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smaddhidi64 (operands[0], operands[2],
			      operands[3], operands[1],
			      GEN_INT (1), GEN_INT (1),
			      GEN_INT (3), GEN_INT (3)));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "smaddhidi64"
  [(set (match_operand:DI 0 "register_operand"                   "=  r,   r,   r,   r")
	(plus:DI
	  (match_operand:DI 3 "register_operand"                 "   0,   0,   0,   0")
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 1 "register_operand"         "   r,   r,   r,   r")
		  (parallel [(match_operand:SI 4 "imm_0_1_operand" " v00, v00, v01, v01")])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_operand:V4HI 2 "register_operand"         "   r,   r,   r,   r")
		  (parallel [(match_operand:SI 5 "imm_0_1_operand" " v00, v01, v01, v00")]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 1)
		  (parallel [(match_operand:SI 6 "imm_2_3_operand" " v02, v02, v03, v03")])))
	      (sign_extend:DI
		(vec_select:HI
		  (match_dup 2)
		  (parallel [(match_operand:SI 7 "imm_2_3_operand" " v02, v03, v03, v02")])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   smalbb\t%0, %1, %2
   smalbt\t%0, %1, %2
   smaltt\t%0, %1, %2
   smalbt\t%0, %2, %1"
  [(set_attr "type" "dmac")])

(define_expand "smalda1"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" " r")
   (match_operand:V2HI 3 "register_operand" " r")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smalda1_le (operands[0], operands[1], operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smalds1"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" " r")
   (match_operand:V2HI 3 "register_operand" " r")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smalds1_le (operands[0], operands[1], operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "smalda1_le"
  [(set (match_operand:DI 0 "register_even_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_even_operand"                           " 0")
	  (sign_extend:DI
	    (plus:SI
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 2 "register_operand" " r")
				  (parallel [(const_int 1)])))
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 3 "register_operand" " r")
				  (parallel [(const_int 1)]))))
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_dup 2)
				  (parallel [(const_int 0)])))
		(sign_extend:SI (vec_select:HI
				  (match_dup 3)
				  (parallel [(const_int 0)]))))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smalda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smalda64"
  [(set (match_operand:DI 0 "register_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_operand"                           " 0")
	  (plus:DI
	    (sign_extend:DI
	      (plus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 3 "register_operand" " r")
				    (parallel [(const_int 0)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 1)]))))))
	    (sign_extend:DI
	      (plus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 3)])))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smalda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smalds1_le"
  [(set (match_operand:DI 0 "register_even_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_even_operand"                           " 0")
	  (sign_extend:DI
	    (minus:SI
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 2 "register_operand" " r")
				  (parallel [(const_int 1)])))
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 3 "register_operand" " r")
				  (parallel [(const_int 1)]))))
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_dup 2)
				  (parallel [(const_int 0)])))
		(sign_extend:SI (vec_select:HI
				  (match_dup 3)
				  (parallel [(const_int 0)]))))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smalds\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smalds64"
  [(set (match_operand:DI 0 "register_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_operand"                           " 0")
	  (plus:DI
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 3 "register_operand" " r")
				    (parallel [(const_int 1)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 0)]))))))
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 2)])))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smalds\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_expand "smaldrs3"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" " r")
   (match_operand:V2HI 3 "register_operand" " r")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smaldrs3_le (operands[0], operands[1], operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "smaldrs3_le"
  [(set (match_operand:DI 0 "register_even_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_even_operand"                           " 0")
	  (sign_extend:DI
	    (minus:SI
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 2 "register_operand" " r")
				  (parallel [(const_int 0)])))
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 3 "register_operand" " r")
				  (parallel [(const_int 0)]))))
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_dup 2)
				  (parallel [(const_int 1)])))
		(sign_extend:SI (vec_select:HI
				  (match_dup 3)
				  (parallel [(const_int 1)]))))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smaldrs\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smaldrs64"
  [(set (match_operand:DI 0 "register_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_operand"                           " 0")
	  (plus:DI
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 3 "register_operand" " r")
				    (parallel [(const_int 0)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 1)]))))))
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 3)])))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smaldrs\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_expand "smalxda1"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" " r")
   (match_operand:V2HI 3 "register_operand" " r")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smalxda1_le (operands[0], operands[1], operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "smalxds1"
  [(match_operand:DI 0 "register_even_operand" "")
   (match_operand:DI 1 "register_even_operand" "")
   (match_operand:V2HI 2 "register_operand" " r")
   (match_operand:V2HI 3 "register_operand" " r")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smalxds1_le (operands[0], operands[1], operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "smalxd<add_sub>1_le"
  [(set (match_operand:DI 0 "register_even_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_even_operand"                           " 0")
	  (sign_extend:DI
	    (plus_minus:SI
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 2 "register_operand" " r")
				  (parallel [(const_int 1)])))
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 3 "register_operand" " r")
				  (parallel [(const_int 0)]))))
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_dup 2)
				  (parallel [(const_int 0)])))
		(sign_extend:SI (vec_select:HI
				  (match_dup 3)
				  (parallel [(const_int 1)]))))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smalxd<add_sub>\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smalxd<add_sub>64"
  [(set (match_operand:DI 0 "register_operand"                             "=r")
	(plus:DI
	  (match_operand:DI 1 "register_operand"                           " 0")
	  (plus:DI
	    (sign_extend:DI
	      (plus_minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 3 "register_operand" " r")
				    (parallel [(const_int 0)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 1)]))))))
	    (sign_extend:DI
	      (plus_minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 3)])))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smalxd<add_sub>\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smslda1"
  [(set (match_operand:DI 0 "register_even_operand"                             "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "register_even_operand"                         " 0")
	    (sign_extend:DI
	      (mult:SI
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 2 "register_operand" " r")
				  (parallel [(const_int 1)])))
		(sign_extend:SI (vec_select:HI
				  (match_operand:V2HI 3 "register_operand" " r")
				  (parallel [(const_int 1)]))))))
	  (sign_extend:DI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smslda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smslda64"
  [(set (match_operand:DI 0 "register_operand"                             "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "register_operand"                           " 0")
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 3 "register_operand" " r")
				    (parallel [(const_int 0)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 1)])))))))
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smslda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smslxda1"
  [(set (match_operand:DI 0 "register_even_operand"                               "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "register_even_operand"                           " 0")
	      (sign_extend:DI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V2HI 2 "register_operand" " r")
				    (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V2HI 3 "register_operand" " r")
				    (parallel [(const_int 0)]))))))
	  (sign_extend:DI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smslxda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "smslxda64"
  [(set (match_operand:DI 0 "register_operand"                             "=r")
	(minus:DI
	  (minus:DI
	    (match_operand:DI 1 "register_operand"                           " 0")
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI
				    (match_operand:V4HI 3 "register_operand" " r")
				    (parallel [(const_int 1)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 0)])))))))
	    (sign_extend:DI
	      (minus:SI
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 3)]))))
		(mult:SI
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 3) (parallel [(const_int 2)]))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smslxda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

;; mada for synthetize smalda
(define_insn_and_split "mada1"
  [(set (match_operand:SI 0 "register_operand"                          "=r")
	(plus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(match_operand:SI 3 "imm_0_1_operand" " u01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(match_operand:SI 4 "imm_0_1_operand" " u01")]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(match_operand:SI 5 "imm_0_1_operand" " u01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(match_operand:SI 6 "imm_0_1_operand" " u01")]))))))]
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  "#"
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  [(const_int 1)]
{
  rtx result0 = gen_reg_rtx (SImode);
  rtx result1 = gen_reg_rtx (SImode);
  emit_insn (gen_mulhisi3v (result0, operands[1], operands[2],
			    operands[3], operands[4]));
  emit_insn (gen_mulhisi3v (result1, operands[1], operands[2],
			    operands[5], operands[6]));
  emit_insn (gen_addsi3 (operands[0], result0, result1));
  DONE;
})

(define_insn_and_split "mada2"
  [(set (match_operand:SI 0 "register_operand"                                 "=r")
	(plus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(match_operand:SI 3 "imm_0_1_operand" " Iu01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(match_operand:SI 4 "imm_0_1_operand" " Iu01")]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(match_operand:SI 5 "imm_0_1_operand" " Iu01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(match_operand:SI 6 "imm_0_1_operand" " Iu01")]))))))]
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  "#"
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  [(const_int 1)]
{
  rtx result0 = gen_reg_rtx (SImode);
  rtx result1 = gen_reg_rtx (SImode);
  emit_insn (gen_mulhisi3v (result0, operands[1], operands[2],
			    operands[3], operands[4]));
  emit_insn (gen_mulhisi3v (result1, operands[1], operands[2],
			    operands[6], operands[5]));
  emit_insn (gen_addsi3 (operands[0], result0, result1));
  DONE;
})

;; sms for synthetize smalds
(define_insn_and_split "sms1"
  [(set (match_operand:SI 0 "register_operand"                                 "=  r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand"         "   r")
			      (parallel [(match_operand:SI 3 "imm_0_1_operand" " u01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand"         "   r")
			      (parallel [(match_operand:SI 4 "imm_0_1_operand" " u01")]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(match_operand:SI 5 "imm_0_1_operand" " u01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(match_operand:SI 6 "imm_0_1_operand" " u01")]))))))]
  "TARGET_DSP
   && !TARGET_64BIT
   && (!reload_completed
       || !riscv_need_split_sms_p (operands[3], operands[4],
				   operands[5], operands[6]))"

{
  return riscv_output_sms (operands[3], operands[4],
			   operands[5], operands[6]);
}
  "TARGET_DSP
   && !TARGET_64BIT
   && !reload_completed
   && riscv_need_split_sms_p (operands[3], operands[4],
			      operands[5], operands[6])"
  [(const_int 1)]
{
  riscv_split_sms (operands[0], operands[1], operands[2],
		   operands[3], operands[4],
		   operands[5], operands[6]);
  DONE;
})

(define_insn_and_split "sms2"
  [(set (match_operand:SI 0 "register_operand"                                 "=  r")
	(minus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand"         "   r")
			      (parallel [(match_operand:SI 3 "imm_0_1_operand" " u01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand"         "   r")
			      (parallel [(match_operand:SI 4 "imm_0_1_operand" " u01")]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(match_operand:SI 5 "imm_0_1_operand" " u01")])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(match_operand:SI 6 "imm_0_1_operand" " u01")]))))))]
  "TARGET_DSP
   && !TARGET_64BIT
   && (!reload_completed
       || !riscv_need_split_sms_p (operands[3], operands[4],
				   operands[6], operands[5]))"
{
  return riscv_output_sms (operands[3], operands[4],
			   operands[6], operands[5]);
}
  "TARGET_DSP
   && !TARGET_64BIT
   && !reload_completed
   && riscv_need_split_sms_p (operands[3], operands[4],
			      operands[6], operands[5])"
  [(const_int 1)]
{
  riscv_split_sms (operands[0], operands[1], operands[2],
		   operands[3], operands[4],
		   operands[6], operands[5]);
  DONE;
})

(define_insn "kmda"
  [(set (match_operand:SI 0 "register_operand"                         "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 1)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmda\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "kmda32"
  [(set (match_operand:DI 0 "register_operand"                         "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 1)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 0)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmda32\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "kmda64"
  [(set (match_operand:V2SI 0 "register_operand"                      "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmda\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "kmxda"
  [(set (match_operand:SI 0 "register_operand"                        "=r")
	(ss_plus:SI
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:SI (vec_select:HI
			      (match_operand:V2HI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:SI
	    (sign_extend:SI (vec_select:HI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:SI (vec_select:HI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmxda\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "kmxda32"
  [(set (match_operand:DI 0 "register_operand"                        "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 1 "register_operand" "r")
			      (parallel [(const_int 1)])))
	    (sign_extend:DI (vec_select:SI
			      (match_operand:V2SI 2 "register_operand" "r")
			      (parallel [(const_int 0)]))))
	  (mult:DI
	    (sign_extend:DI (vec_select:SI
			      (match_dup 1)
			      (parallel [(const_int 0)])))
	    (sign_extend:DI (vec_select:SI
			      (match_dup 2)
			      (parallel [(const_int 1)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmxda32\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "kmxda64"
  [(set (match_operand:V2SI 0 "register_operand"                      "=r")
	(ss_plus:V2SI
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 1 "register_operand" "r")
				(parallel [(const_int 1) (const_int 3)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" "r")
				(parallel [(const_int 0) (const_int 2)]))))
	  (mult:V2SI
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 1)
				(parallel [(const_int 0) (const_int 2)])))
	    (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmxda\t%0, %1, %2"
  [(set_attr "type" "dmac")])

(define_insn "kmada"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_plus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 1)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmada\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmada32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_plus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmada32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmada64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_plus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 0) (const_int 2)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmada\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmada2"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_plus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmada\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmaxda"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_plus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmaxda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmaxda32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_plus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmaxda32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmaxda64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_plus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmaxda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmads"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 1)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmads\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmads32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmads32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmads64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 0) (const_int 2)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmads\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmadrs"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmadrs\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmadrs32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmadrs32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmadrs64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmadrs\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmaxds"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_plus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmaxds\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmaxds32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmaxds32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmaxds64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_plus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmaxds\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmsda"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_minus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 1)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmsda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmsda32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_minus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 1)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 0)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmsda32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmsda64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_minus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 0) (const_int 2)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmsda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmsxda"
  [(set (match_operand:SI 0 "register_operand"                           "=r")
	(ss_minus:SI
	  (match_operand:SI 1 "register_operand"                         " 0")
	  (ss_minus:SI
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:SI (vec_select:HI
				(match_operand:V2HI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:SI
	      (sign_extend:SI (vec_select:HI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:SI (vec_select:HI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmsxda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmsxda32"
  [(set (match_operand:DI 0 "register_operand"                           "=r")
	(ss_minus:DI
	  (match_operand:DI 1 "register_operand"                         " 0")
	  (ss_minus:DI
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 2 "register_operand" " r")
				(parallel [(const_int 1)])))
	      (sign_extend:DI (vec_select:SI
				(match_operand:V2SI 3 "register_operand" " r")
				(parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI (vec_select:SI
				(match_dup 2)
				(parallel [(const_int 0)])))
	      (sign_extend:DI (vec_select:SI
				(match_dup 3)
				(parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmsxda32\t%0, %2, %3"
  [(set_attr "type" "dmac")])

(define_insn "kmsxda64"
  [(set (match_operand:V2SI 0 "register_operand"                         "=r")
	(ss_minus:V2SI
	  (match_operand:V2SI 1 "register_operand"                       " 0")
	  (ss_minus:V2SI
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 2 "register_operand" " r")
				(parallel [(const_int 1) (const_int 3)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_operand:V4HI 3 "register_operand" " r")
				(parallel [(const_int 0) (const_int 2)]))))
	    (mult:V2SI
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 2)
				(parallel [(const_int 0) (const_int 2)])))
	      (sign_extend:V2SI (vec_select:V2HI
				(match_dup 3)
				(parallel [(const_int 1) (const_int 3)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmsxda\t%0, %2, %3"
  [(set_attr "type" "dmac")])

;; smax[8|16] and umax[8|16]
(define_insn "<opcode><mode>3"
  [(set (match_operand:VECI 0 "register_operand"             "=r")
	(sumax:VECI (match_operand:VECI 1 "register_operand" " r")
		    (match_operand:VECI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "<opcode><bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")])

;; smin[8|16] and umin[8|16]
(define_insn "<opcode><mode>3"
  [(set (match_operand:VECI 0 "register_operand"             "=r")
	(sumin:VECI (match_operand:VECI 1 "register_operand" " r")
		    (match_operand:VECI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "<opcode><bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")])

(define_insn "<opcode><mode>3_bb"
  [(set (match_operand:<VELT> 0 "register_operand"                    "=r")
	(sumin_max:<VELT> (vec_select:<VELT>
			    (match_operand:VQIHI 1 "register_operand" " r")
			    (parallel [(const_int 0)]))
			  (vec_select:<VELT>
			    (match_operand:VQIHI 2 "register_operand" " r")
			    (parallel [(const_int 0)]))))]
  "TARGET_DSP"
  "<opcode><bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")])

(define_insn_and_split "<opcode>v2hi3_bbtt"
  [(set (match_operand:V2HI 0 "register_operand"                         "=r")
	(vec_merge:V2HI
	  (vec_duplicate:V2HI
	    (sumin_max:HI (vec_select:HI
			    (match_operand:V2HI 1 "register_operand" " r")
			    (parallel [(const_int 1)]))
			  (vec_select:HI
			    (match_operand:V2HI 2 "register_operand" " r")
			    (parallel [(const_int 1)]))))
	  (vec_duplicate:V2HI
	    (sumin_max:HI (vec_select:HI
			    (match_dup 1)
			    (parallel [(const_int 0)]))
			  (vec_select:HI
			    (match_dup 2)
			    (parallel [(const_int 0)]))))
	  (const_int 2)))]
  "TARGET_DSP && !TARGET_64BIT"
  "#"
  "TARGET_DSP && !TARGET_64BIT"
  [(const_int 0)]
{
  emit_insn (gen_<opcode>v2hi3 (operands[0], operands[1], operands[2]));
  DONE;
}
  [(set_attr "type" "dalu")
   (set_attr "mode" "V2HI")])

(define_expand "abs<mode>2"
  [(set (match_operand:VQIHI 0 "register_operand"               "=r")
	(ss_abs:VQIHI (match_operand:VQIHI 1 "register_operand" " r")))]
  "TARGET_DSP && !flag_wrapv"
{
})

(define_insn "kabs<mode>2"
  [(set (match_operand:VECI 0 "register_operand"              "=r")
	(ss_abs:VECI (match_operand:VECI 1 "register_operand" " r")))]
  "TARGET_DSP"
  "kabs<bits>\t%0, %1"
  [(set_attr "type"   "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "<su>mar64_1"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(plus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (mult:DI
	    (any_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (any_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>mar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "v<su>mar64_1"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(plus:DI (match_operand:DI 1 "register_operand"    " 0")
	  (plus:DI
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" " r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (any_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "<su>mar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode" "DI")])

(define_insn "<su>mar64_2"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(plus:DI
	  (mult:DI
	    (any_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (any_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))
	  (match_operand:DI 1 "register_even_operand"     " 0")))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>mar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "<su>mar64_3"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(plus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (any_extend:DI
	    (mult:SI
	      (match_operand:SI 2 "register_operand" " r")
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>mar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "<su>mar64_4"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(plus:DI
	  (any_extend:DI
	  (mult:SI
	      (match_operand:SI 2 "register_operand" " r")
	      (match_operand:SI 3 "register_operand" " r")))
	  (match_operand:DI 1 "register_even_operand"     " 0")))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>mar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "<su>msr64"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(minus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (mult:DI
	    (any_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (any_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>msr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "v<su>msr64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(minus:DI
	  (minus:DI
	  (match_operand:DI 1 "register_operand"    " 0")
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (any_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" " r")
		  (parallel [(const_int 0)])))))
	    (mult:DI
	      (any_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (any_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "<su>msr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode" "DI")])

(define_insn "<su>msr64_2"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(minus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (any_extend:DI
	    (mult:SI
	      (match_operand:SI 2 "register_operand" " r")
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>msr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

;; kmar64, kmsr64, ukmar64 and ukmsr64
(define_insn "kmar64_1"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(ss_plus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (mult:DI
	    (sign_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (sign_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "kmar64_2"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(ss_plus:DI
	  (mult:DI
	    (sign_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (sign_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))
	  (match_operand:DI 1 "register_even_operand"     " 0")))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "vkmar64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(ss_plus:DI (match_operand:DI 1 "register_operand"    " 0")
	  (plus:DI
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" " r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode" "DI")])

(define_insn "kmsr64"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(ss_minus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (mult:DI
	    (sign_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (sign_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "kmsr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "vkmsr64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(ss_minus:DI
	  (minus:DI
	  (match_operand:DI 1 "register_operand"    " 0")
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (sign_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" " r")
		  (parallel [(const_int 0)])))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kmsr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode" "DI")])

(define_insn "ukmar64_1"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(us_plus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (mult:DI
	    (zero_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (zero_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "ukmar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "vukmar64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(us_plus:DI (match_operand:DI 1 "register_operand"    " 0")
	  (plus:DI
	    (mult:DI
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" " r")
		  (parallel [(const_int 0)]))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)])))))))]
  "TARGET_DSP && TARGET_64BIT"
  "ukmar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode" "DI")])

(define_insn "ukmar64_2"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(us_plus:DI
	  (mult:DI
	    (zero_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (zero_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))
	  (match_operand:DI 1 "register_even_operand"     " 0")))]
  "TARGET_DSP && !TARGET_64BIT"
  "ukmar64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "ukmsr64"
  [(set (match_operand:DI 0 "register_even_operand"       "=r")
	(us_minus:DI
	  (match_operand:DI 1 "register_even_operand"     " 0")
	  (mult:DI
	    (zero_extend:DI
	      (match_operand:SI 2 "register_operand" " r"))
	    (zero_extend:DI
	      (match_operand:SI 3 "register_operand" " r")))))]
  "TARGET_DSP && !TARGET_64BIT"
  "ukmsr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "DI")])

(define_insn "vukmsr64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(us_minus:DI
	  (minus:DI
	    (match_operand:DI 1 "register_operand"    " 0")
	    (mult:DI
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 2 "register_operand" " r")
		  (parallel [(const_int 0)])))
	      (zero_extend:DI
		(vec_select:SI
		  (match_operand:V2SI 3 "register_operand" " r")
		  (parallel [(const_int 0)])))))
	    (mult:DI
	      (sign_extend:DI
		(vec_select:SI (match_dup 2) (parallel [(const_int 1)])))
	      (sign_extend:DI
		(vec_select:SI (match_dup 3) (parallel [(const_int 1)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "ukmsr64\t%0, %2, %3"
  [(set_attr "type"   "dmac")
   (set_attr "mode" "DI")])

(define_insn "bpick1<mode>"
  [(set (match_operand:GPR 0 "register_operand"       "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand" " r")
	      (match_operand:GPR 3 "register_operand" " r"))
	    (and:GPR
	      (match_operand:GPR 2 "register_operand" " r")
	      (not:GPR (match_dup 3)))))]
  "TARGET_DSP"
  "bpick\t%0, %1, %2, %3"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick2<mode>"
  [(set (match_operand:GPR 0 "register_operand"       "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand" " r")
	      (match_operand:GPR 2 "register_operand" " r"))
	    (and:GPR
	      (not:GPR (match_dup 2))
	      (match_operand:GPR 3 "register_operand" " r"))))]
  "TARGET_DSP"
  "bpick\t%0, %1, %3, %2"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick3<mode>"
  [(set (match_operand:GPR 0 "register_operand"       "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand" " r")
	      (match_operand:GPR 2 "register_operand" " r"))
	    (and:GPR
	      (match_operand:GPR 3 "register_operand" " r")
	      (not:GPR (match_dup 1)))))]
  "TARGET_DSP"
  "bpick\t%0, %2, %3, %1"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick4<mode>"
  [(set (match_operand:GPR 0 "register_operand"       "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand" " r")
	      (match_operand:GPR 2 "register_operand" " r"))
	    (and:GPR
	      (not:GPR (match_dup 1))
	      (match_operand:GPR 3 "register_operand" " r"))))]
  "TARGET_DSP"
  "bpick\t%0, %2, %3, %1"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick5<mode>"
  [(set (match_operand:GPR 0 "register_operand"               "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand"         " r")
	      (not:GPR (match_operand:GPR 2 "register_operand" " r")))
	    (and:GPR
	      (match_operand:GPR 3 "register_operand"         " r")
	      (match_dup 2))))]
  "TARGET_DSP"
  "bpick\t%0, %3, %1, %2"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick6<mode>"
  [(set (match_operand:GPR 0 "register_operand"               "=r")
	  (ior:GPR
	    (and:GPR
	      (not:GPR (match_operand:GPR 1 "register_operand" " r"))
	      (match_operand:GPR 2 "register_operand"         " r"))
	    (and:GPR
	      (match_operand:GPR 3 "register_operand" " r")
	      (match_dup 1))))]
  "TARGET_DSP"
  "bpick\t%0, %3, %2, %1"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick7<mode>"
  [(set (match_operand:GPR 0 "register_operand"               "=r")
	  (ior:GPR
	    (and:GPR
	      (match_operand:GPR 1 "register_operand"         " r")
	      (not:GPR (match_operand:GPR 2 "register_operand" " r")))
	    (and:GPR
	      (match_dup 2)
	      (match_operand:GPR 3 "register_operand"         " r"))))]
  "TARGET_DSP"
  "bpick\t%0, %3, %1, %2"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick8<mode>"
  [(set (match_operand:GPR 0 "register_operand"               "=r")
	  (ior:GPR
	    (and:GPR
	      (not:GPR (match_operand:GPR 1 "register_operand" " r"))
	      (match_operand:GPR 2 "register_operand"         " r"))
	    (and:GPR
	      (match_dup 1)
	      (match_operand:GPR 3 "register_operand"         " r"))))]
  "TARGET_DSP"
  "bpick\t%0, %3, %2, %1"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "<MODE>")])

(define_insn "bpick8"
  [(set (match_operand:SI 0 "register_operand"               "=r")
	  (ior:SI
	    (and:SI
	      (not:SI (match_operand:SI 1 "register_operand" " r"))
	      (match_operand:SI 2 "register_operand"         " r"))
	    (and:SI
	      (match_dup 1)
	      (match_operand:SI 3 "register_operand"         " r"))))]
  "TARGET_DSP && !TARGET_64BIT"
  "bpick\t%0, %3, %2, %1"
  [(set_attr "type"   "dbpick")
   (set_attr "mode"   "SI")])

(define_insn "sraiu"
  [(set (match_operand:SI 0 "register_operand"                          "=  r, r")
	(unspec:SI [(ashiftrt:SI (match_operand:SI 1 "register_operand" "   r, r")
				 (match_operand:SI 2 "rimm5u_operand"   " u05, r"))]
		    UNSPEC_ROUND))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   srai.u\t%0, %1, %2
   sra.u\t%0, %1, %2"
  [(set_attr "type"   "daluround")
   (set_attr "mode"   "SI")])

(define_insn "sraiu64"
  [(set (match_operand:DI 0 "register_operand"                          "=  r, r")
	(unspec:DI [(ashiftrt:DI (match_operand:DI 1 "register_operand" "   r, r")
				 (match_operand:DI 2 "rimm6u_operand"   " u06, r"))]
		    UNSPEC_ROUND))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   srai.u\t%0, %1, %2
   sra.u\t%0, %1, %2"
  [(set_attr "type"   "daluround")
   (set_attr "mode"   "DI")])

(define_insn "sraiw_u"
  [(set (match_operand:DI 0 "register_operand"             "=  r")
	(unspec:DI [(match_operand:SI 1 "register_operand" "   r")
		    (match_operand:SI 2 "imm5u_operand"    " u05")]
		    UNSPEC_ROUND64))]
  "TARGET_DSP && TARGET_64BIT"
  "sraiw.u\t%0, %1, %2"
  [(set_attr "type"   "daluround")
   (set_attr "mode"   "DI")])

(define_insn "kssl"
  [(set (match_operand:SI 0 "register_operand"               "=   r, r")
	(ss_ashift:SI (match_operand:SI 1 "register_operand" "    r, r")
		      (match_operand:SI 2 "rimm5u_operand"   " Iu05, r")))]
  "TARGET_DSP"
  "@
   kslliw\t%0, %1, %2
   ksllw\t%0, %1, %2"
  [(set_attr "type"   "dalu")
   (set_attr "mode"   "SI")])

(define_insn "kslraw_round"
  [(set (match_operand:SI 0 "register_operand"                  "=r")
	(if_then_else:SI
	  (lt:SI (match_operand:SI 2 "register_operand"        " r")
		 (const_int 0))
	  (unspec:SI [(ashiftrt:SI (match_operand:SI 1 "register_operand" " r")
				   (neg:SI (match_dup 2)))]
		     UNSPEC_ROUND)
	  (ss_ashift:SI (match_dup 1)
			(match_dup 2))))]
  "TARGET_DSP"
  "kslraw.u\t%0, %1, %2"
  [(set_attr "type"   "daluround")
   (set_attr "mode"   "SI")])

(define_insn "sclip32<VD_SI:mode><X:mode>"
  [(set (match_operand:VD_SI 0 "register_operand" "=r")
	(unspec:VD_SI [(match_operand:VD_SI 1 "register_operand" "r")
		       (match_operand:X 2 "immediate_operand" "i")] UNSPEC_CLIPS_OV))]
  "TARGET_DSP"
  "sclip32\t%0, %1, %2"
  [(set_attr "type"   "dclip")
   (set_attr "mode" "<VD_SI:MODE>")])

(define_insn "uclip32<VD_SI:mode><X:mode>"
  [(set (match_operand:VD_SI 0 "register_operand" "=r")
	(unspec:VD_SI [(match_operand:VD_SI 1 "register_operand" "r")
		       (match_operand:X 2 "immediate_operand" "i")] UNSPEC_CLIP_OV))]
  "TARGET_DSP"
  "uclip32\t%0, %1, %2"
  [(set_attr "type"   "dclip")
   (set_attr "mode" "<VD_SI:MODE>")])

(define_insn "bitrev"
  [(set (match_operand:SI 0 "register_operand"             "=r,   r")
	(unspec:SI [(match_operand:SI 1 "register_operand" " r,   r")
		    (match_operand:SI 2 "rimm5u_operand"   " r, u05")]
		   UNSPEC_BITREV))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   bitrev\t%0, %1, %2
   bitrevi\t%0, %1, %2"
  [(set_attr "type"   "dalu")
   (set_attr "mode"   "SI")])

(define_insn "bitrev64"
  [(set (match_operand:DI 0 "register_operand"             "=r,   r")
	(unspec:DI [(match_operand:DI 1 "register_operand" " r,   r")
		    (match_operand:SI 2 "rimm6u_operand"   " r, u06")]
		   UNSPEC_BITREV))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   bitrev\t%0, %1, %2
   bitrevi\t%0, %1, %2"
  [(set_attr "type"   "dalu")
   (set_attr "mode"   "SI")])

;; wext, wexti
(define_insn "<su>wext"
  [(set (match_operand:SI 0 "register_operand"     "=r,  r")
	(truncate:SI
	  (any_shiftrt:DI
	    (match_operand:DI 1 "register_even_operand" " r,  r")
	    (match_operand:SI 2 "rimm5u_operand"   " r,u05"))))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   wext\t%0, %1, %2
   wexti\t%0, %1, %2"
  [(set_attr "type"   "dwext")
   (set_attr "mode"   "SI")])

(define_insn "wext64"
  [(set (match_operand:DI 0 "register_operand"     "=r,  r")
	(sign_extend:DI
	  (truncate:SI
	    (ashiftrt:DI
	      (match_operand:DI 1 "register_operand" " r,  r")
	      (match_operand:DI 2 "rimm5u_operand"   " r,u05")))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   wext\t%0, %1, %2
   wexti\t%0, %1, %2"
  [(set_attr "type" "dwext")
   (set_attr "mode" "DI")])

(define_insn_and_split "wext_<shift>di3"
  [(set (match_operand:DI 0 "register_operand" "")
	(any_shift:DI (match_operand:DI 1 "register_operand" "")
		      (match_operand:DI 2 "imm6u_operand" "")))]
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  "#"
  "TARGET_DSP && !TARGET_64BIT && !reload_completed"
  [(const_int 0)]
{
  riscv_split_<code>di3 (operands[0], operands[1], operands[2]);
  DONE;
})

;; 32-bit add/sub instruction: raddw and rsubw.
(define_insn "r<opcode>si3"
  [(set (match_operand:SI 0 "register_operand"                       "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (plus_minus:DI
	      (sign_extend:DI (match_operand:SI 1 "register_operand" " r"))
	      (sign_extend:DI (match_operand:SI 2 "register_operand" " r")))
	    (const_int 1))))]
  "TARGET_DSP && !TARGET_64BIT"
  "r<opcode>w\t%0, %1, %2"
  [(set_attr "type" "dalu")])

;; 64-bit add/sub instruction: raddw and rsubw.
(define_insn "r<opcode>disi3"
  [(set (match_operand:DI 0 "register_operand"                       "=r")
	(sign_extend:DI
	  (truncate:SI
	    (ashiftrt:DI
	      (plus_minus:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" " r"))
		(sign_extend:DI (match_operand:SI 2 "register_operand" " r")))
	      (const_int 1)))))]
  "TARGET_DSP && TARGET_64BIT"
  "r<opcode>w\t%0, %1, %2"
  [(set_attr "type" "dalu")])

;; 32-bit add/sub instruction: uraddw and ursubw.
(define_insn "ur<opcode>si3"
  [(set (match_operand:SI 0 "register_operand"                       "=r")
	(truncate:SI
	  (lshiftrt:DI
	    (plus_minus:DI
	      (zero_extend:DI (match_operand:SI 1 "register_operand" " r"))
	      (zero_extend:DI (match_operand:SI 2 "register_operand" " r")))
	    (const_int 1))))]
  "TARGET_DSP && !TARGET_64BIT"
  "ur<opcode>w\t%0, %1, %2"
  [(set_attr "type" "dalu")])

;; 64-bit add/sub instruction: uraddw and ursubw.
(define_insn "ur<opcode>disi3"
  [(set (match_operand:DI 0 "register_operand"                       "=r")
	(sign_extend:DI
	  (truncate:SI
	    (lshiftrt:DI
	      (plus_minus:DI
		(zero_extend:DI (match_operand:SI 1 "register_operand" " r"))
		(zero_extend:DI (match_operand:SI 2 "register_operand" " r")))
	      (const_int 1)))))]
  "TARGET_DSP && TARGET_64BIT"
  "ur<opcode>w\t%0, %1, %2"
  [(set_attr "type" "dalu")])

;; SATURATION

(define_insn "kabs"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")] UNSPEC_KABS))]
  "TARGET_DSP"
  "kabsw\t%0, %1"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "kaddw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KADDW))]
  "TARGET_DSP && !TARGET_64BIT"
  "kaddw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "kaddw64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KADDW)))]
  "TARGET_DSP && TARGET_64BIT"
  "kaddw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "ksubw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSUBW))]
  "TARGET_DSP && !TARGET_64BIT"
  "ksubw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "ksubw64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSUBW)))]
  "TARGET_DSP && TARGET_64BIT"
  "ksubw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "kaddh"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(sign_extend:SI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KADDH)))]
  "TARGET_DSP"
  "kaddh\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "ksubh"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(sign_extend:SI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSUBH)))]
  "TARGET_DSP"
  "ksubh\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "ukaddw"
  [(set (match_operand:SI 0 "register_operand"             "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKADDW))]
  "TARGET_DSP && !TARGET_64BIT"
  "ukaddw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "ukaddw64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(sign_extend:DI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKADDW64)))]
  "TARGET_DSP && TARGET_64BIT"
  "ukaddw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "uksubw"
  [(set (match_operand:SI 0 "register_operand"             "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKSUBW))]
  "TARGET_DSP && !TARGET_64BIT"
  "uksubw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "uksubw64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(sign_extend:DI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKSUBW64)))]
  "TARGET_DSP && TARGET_64BIT"
  "uksubw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "ukaddh"
  [(set (match_operand:SI 0 "register_operand"             "=r")
	(sign_extend:SI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKADDH)))]
  "TARGET_DSP && !TARGET_64BIT"
  "ukaddh\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "ukaddh64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(sign_extend:DI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKADDH)))]
  "TARGET_DSP && TARGET_64BIT"
  "ukaddh\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "uksubh"
  [(set (match_operand:SI 0 "register_operand"             "=r")
	(sign_extend:SI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKSUBH)))]
  "TARGET_DSP && !TARGET_64BIT"
  "uksubh\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "uksubh64"
  [(set (match_operand:DI 0 "register_operand"             "=r")
	(sign_extend:DI (unspec:SI [(match_operand:SI 1 "register_operand" "r")
				    (match_operand:SI 2 "register_operand" "r")] UNSPEC_UKSUBH)))]
  "TARGET_DSP && TARGET_64BIT"
  "uksubh\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "kdmbb<mode>"
  [(set (match_operand:VHI 0 "register_operand" "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "r")
		     (match_operand:VHI 2 "register_operand" "r")] UNSPEC_KDMBB))]
  "TARGET_DSP"
  "kdmbb\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "kdmbt<mode>"
  [(set (match_operand:VHI 0 "register_operand" "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "r")
		     (match_operand:VHI 2 "register_operand" "r")] UNSPEC_KDMBT))]
  "TARGET_DSP"
  "kdmbt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "kdmtt<mode>"
  [(set (match_operand:VHI 0 "register_operand" "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "r")
		     (match_operand:VHI 2 "register_operand" "r")] UNSPEC_KDMTT))]
  "TARGET_DSP"
  "kdmtt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "kdmbb64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KDMBB)))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmbb\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "kdmbt64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KDMBT)))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmbt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "kdmtt64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KDMTT)))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmtt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "kdmbb16"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")
		      (match_operand:V4HI 2 "register_operand" "r")] UNSPEC_KDMBB16))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmbb16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "kdmbt16"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")
		      (match_operand:V4HI 2 "register_operand" "r")] UNSPEC_KDMBT16))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmbt16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "kdmtt16"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")
		      (match_operand:V4HI 2 "register_operand" "r")] UNSPEC_KDMTT16))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmtt16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "khmbb<mode>"
  [(set (match_operand:VHI 0 "register_operand" "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "r")
		     (match_operand:VHI 2 "register_operand" "r")] UNSPEC_KHMBB))]
  "TARGET_DSP"
  "khmbb\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "khmbt<mode>"
  [(set (match_operand:VHI 0 "register_operand" "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "r")
		     (match_operand:VHI 2 "register_operand" "r")] UNSPEC_KHMBT))]
  "TARGET_DSP"
  "khmbt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "khmtt<mode>"
  [(set (match_operand:VHI 0 "register_operand" "=r")
	(unspec:VHI [(match_operand:VHI 1 "register_operand" "r")
		     (match_operand:VHI 2 "register_operand" "r")] UNSPEC_KHMTT))]
  "TARGET_DSP"
  "khmtt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "<MODE>")])

(define_insn "khmbb64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KHMBB)))]
  "TARGET_DSP && TARGET_64BIT"
  "khmbb\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "khmbt64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KHMBT)))]
  "TARGET_DSP && TARGET_64BIT"
  "khmbt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "khmtt64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KHMTT)))]
  "TARGET_DSP && TARGET_64BIT"
  "khmtt\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "DI")])

(define_insn "khmbb16"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")
		      (match_operand:V4HI 2 "register_operand" "r")] UNSPEC_KHMBB16))]
  "TARGET_DSP && TARGET_64BIT"
  "khmbb16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "khmbt16"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")
		      (match_operand:V4HI 2 "register_operand" "r")] UNSPEC_KHMBT16))]
  "TARGET_DSP && TARGET_64BIT"
  "khmbt16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "khmtt16"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")
		      (match_operand:V4HI 2 "register_operand" "r")] UNSPEC_KHMTT16))]
  "TARGET_DSP && TARGET_64BIT"
  "khmtt16\t%0, %1, %2"
  [(set_attr "type" "dmul")
   (set_attr "mode" "V4HI")])

(define_insn "kslraw"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSLRAW))]
  "TARGET_DSP && !TARGET_64BIT"
  "kslraw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "kslraw64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSLRAW)))]
  "TARGET_DSP && TARGET_64BIT"
  "kslraw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "unspec_kslrawu"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSLRAWU))]
  "TARGET_DSP && !TARGET_64BIT"
  "kslraw.u\t%0, %1, %2"
  [(set_attr "type" "daluround")
   (set_attr "mode" "SI")])

(define_insn "unspec_kslrawu64"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI
	  (unspec:SI [(match_operand:SI 1 "register_operand" "r")
		      (match_operand:SI 2 "register_operand" "r")] UNSPEC_KSLRAWU)))]
  "TARGET_DSP && TARGET_64BIT"
  "kslraw.u\t%0, %1, %2"
  [(set_attr "type" "daluround")
   (set_attr "mode" "DI")])

(define_expand "unspec_ave"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:SI 2 "register_operand" "")]
 "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_ave (operands[0], operands[1], operands[2]));
  DONE;
})

(define_insn "ave"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(truncate:SI
	  (ashiftrt:DI
	    (plus:DI
	      (plus:DI
		(sign_extend:DI (match_operand:SI 1 "register_operand" "r"))
		(sign_extend:DI (match_operand:SI 2 "register_operand" "r")))
	      (const_int 1))
	  (const_int 1))))]
  "TARGET_DSP && !TARGET_64BIT"
  "ave\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_expand "unspec_avedi"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:DI 2 "register_operand" "")]
 "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_avedi (operands[0], operands[1], operands[2]));
  DONE;
})

(define_insn "avedi"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(truncate:DI
	  (ashiftrt:TI
	    (plus:TI
	      (plus:TI
		(sign_extend:TI (match_operand:DI 1 "register_operand" "r"))
		(sign_extend:TI (match_operand:DI 2 "register_operand" "r")))
	      (const_int 1))
	  (const_int 1))))]
  "TARGET_DSP && TARGET_64BIT"
  "ave\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "DI")])

(define_insn "smaxsi3"
  [(set (match_operand:SI 0 "register_operand"          "=r")
	(smax:SI (match_operand:SI 1 "register_operand" " r")
		 (match_operand:SI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "maxw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "sminsi3"
  [(set (match_operand:SI 0 "register_operand"          "=r")
	(smin:SI (match_operand:SI 1 "register_operand" " r")
		 (match_operand:SI 2 "register_operand" " r")))]
  "TARGET_DSP"
  "minw\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])


(define_expand "<shift><mode>3"
  [(set (match_operand:VQI 0 "register_operand"                "")
	(any_shift:VQI (match_operand:VQI 1 "register_operand" "")
			(match_operand:SI 2 "rimm3u_operand" "")))]
  "TARGET_DSP"
{
  if (operands[2] == const0_rtx)
    {
      emit_move_insn (operands[0], operands[1]);
      DONE;
    }
})

(define_insn "*ashl<mode>3"
  [(set (match_operand:VQI 0 "register_operand"             "=  r, r")
	(ashift:VQI (match_operand:VQI 1 "register_operand" "   r, r")
		     (match_operand:SI 2   "rimm3u_operand" " u03, r")))]
  "TARGET_DSP"
  "@
   slli8\t%0, %1, %2
   sll8\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "kslli8<VQI:mode><X:mode>"
  [(set (match_operand:VQI 0 "register_operand"                "=  r, r")
	(ss_ashift:VQI (match_operand:VQI 1 "register_operand" "   r, r")
		       (match_operand:X 2   "rimm3u_operand"   " u03, r")))]
  "TARGET_DSP"
  "@
   kslli8\t%0, %1, %2
   ksll8\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<VQI:MODE>, <VQI:MODE>")])

(define_insn "*ashr<mode>3"
  [(set (match_operand:VQI 0 "register_operand"               "=  r, r")
	(ashiftrt:VQI (match_operand:VQI 1 "register_operand" "   r, r")
		       (match_operand:SI 2 "rimm3u_operand"   " u03, r")))]
  "TARGET_DSP"
  "@
   srai8\t%0, %1, %2
   sra8\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "sra8_round<mode>"
  [(set (match_operand:VQI 0 "register_operand"                            "=  r, r")
	(unspec:VQI [(ashiftrt:VQI (match_operand:VQI 1 "register_operand" "   r, r")
				   (match_operand:SI 2  "rimm3u_operand"   " u03, r"))]
		      UNSPEC_ROUND))]
  "TARGET_DSP"
  "@
   srai8.u\t%0, %1, %2
   sra8.u\t%0, %1, %2"
  [(set_attr "type" "daluround, daluround")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "*lshr<mode>3"
  [(set (match_operand:VQI 0 "register_operand"               "=  r, r")
	(lshiftrt:VQI (match_operand:VQI 1 "register_operand" "   r, r")
		       (match_operand:SI 2 "rimm3u_operand"   " u03, r")))]
  "TARGET_DSP"
  "@
   srli8\t%0, %1, %2
   srl8\t%0, %1, %2"
  [(set_attr "type" "dalu, dalu")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "srl8_round<mode>"
  [(set (match_operand:VQI 0 "register_operand"                            "=  r, r")
	(unspec:VQI [(lshiftrt:VQI (match_operand:VQI 1 "register_operand" "   r, r")
				   (match_operand:SI 2  "rimm3u_operand"   " u03, r"))]
		      UNSPEC_ROUND))]
  "TARGET_DSP"
  "@
   srli8.u\t%0, %1, %2
   srl8.u\t%0, %1, %2"
  [(set_attr "type" "daluround, daluround")
   (set_attr "mode" "<MODE>, <MODE>")])

(define_insn "pbsad<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec:GPR [(match_operand:GPR 1 "register_operand" "r")
		     (match_operand:GPR 2 "register_operand" "r")] UNSPEC_PBSAD))]
  "TARGET_DSP"
  "pbsad\t%0, %1, %2"
  [(set_attr "type" "dmac")
   (set_attr "mode" "<MODE>")])

(define_insn "pbsada<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec:GPR [(match_operand:GPR 1 "register_operand" "0")
		     (match_operand:GPR 2 "register_operand" "r")
		     (match_operand:GPR 3 "register_operand" "r")] UNSPEC_PBSADA))]
  ""
  "pbsada\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "<MODE>")])

(define_insn "dsp_mulsidi3"
  [(set (match_operand:DI 0 "register_operand"                          "=r")
	(mult:DI (sign_extend:DI (match_operand:SI 1 "register_operand" " r"))
		 (sign_extend:DI (match_operand:SI 2 "register_operand" " r"))))]
  "TARGET_DSP"
  "mulsr64\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode"   "DI")])

(define_insn "dsp_umulsidi3"
  [(set (match_operand:DI 0 "register_operand"                          "=r")
	(mult:DI (zero_extend:DI (match_operand:SI 1 "register_operand" " r"))
		 (zero_extend:DI (match_operand:SI 2 "register_operand" " r"))))]
  "TARGET_DSP"
  "mulr64\t%0, %1, %2"
  [(set_attr "type"   "dmul")
   (set_attr "mode"   "DI")])

;; Multiply-accumulate instructions.

(define_insn "*maddr32_0"
  [(set (match_operand:SI 0 "register_operand"                   "=r")
	(plus:SI (match_operand:SI 3 "register_operand"          " 0")
		 (mult:SI (match_operand:SI 1 "register_operand" " r")
			  (match_operand:SI 2 "register_operand" " r"))))]
  "TARGET_DSP"
  "maddr32\t%0, %1, %2"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "SI")])

(define_insn "*maddr32_1"
  [(set (match_operand:SI 0 "register_operand"                   "=r")
	(plus:SI (mult:SI (match_operand:SI 1 "register_operand" " r")
			  (match_operand:SI 2 "register_operand" " r"))
		 (match_operand:SI 3 "register_operand"          " 0")))]
  "TARGET_DSP"
  "maddr32\t%0, %1, %2"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "SI")])

(define_insn "*msubr32"
  [(set (match_operand:SI 0 "register_operand"                    "=r")
	(minus:SI (match_operand:SI 3 "register_operand"          " 0")
		  (mult:SI (match_operand:SI 1 "register_operand" " r")
			   (match_operand:SI 2 "register_operand" " r"))))]
  "TARGET_DSP"
  "msubr32\t%0, %1, %2"
  [(set_attr "type"   "dmac")
   (set_attr "mode"   "SI")])

(define_insn "bswaphi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
	(bswap:HI (match_operand:HI 1 "register_operand" "r")))]
  "TARGET_DSP"
  "swap8\t%0, %1"
  [(set_attr "type"  "dalu")
   (set_attr "mode"  "HI")])

(define_insn "unspec_bswap8"
  [(set (match_operand:V4QI 0 "register_operand" "=r")
	(unspec:V4QI [(match_operand:V4QI 1 "register_operand" "r")] UNSPEC_BSWAP))]
  "TARGET_DSP"
  "swap8\t%0, %1"
  [(set_attr "type"  "dalu")
   (set_attr "mode"  "V4QI")])

(define_insn "unspec_bswap8_64"
  [(set (match_operand:V8QI 0 "register_operand" "=r")
	(unspec:V8QI [(match_operand:V8QI 1 "register_operand" "r")] UNSPEC_BSWAP))]
  "TARGET_DSP"
  "swap8\t%0, %1"
  [(set_attr "type"  "dalu")
   (set_attr "mode"  "V8QI")])

(define_insn "unspec_bswap16"
  [(set (match_operand:V2HI 0 "register_operand" "=r")
	(unspec:V2HI [(match_operand:V2HI 1 "register_operand" "r")] UNSPEC_BSWAP))]
  "TARGET_DSP"
  "swap16\t%0, %1"
  [(set_attr "type"  "dalu")
   (set_attr "mode"  "V2HI")])

(define_insn "unspec_bswap16_64"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(unspec:V4HI [(match_operand:V4HI 1 "register_operand" "r")] UNSPEC_BSWAP))]
  "TARGET_DSP"
  "swap16\t%0, %1"
  [(set_attr "type"  "dalu")
   (set_attr "mode"  "V4HI")])

(define_insn "riscv_rdov<mode>"
  [(set (match_operand:GPR 0 "register_operand" "=r")
	(unspec_volatile:GPR [(const_int 0)] UNSPEC_RDOV))]
  "TARGET_DSP"
  "csrrs\t%0, ucode, zero"
  [(set_attr "mode" "<MODE>")])

(define_insn "riscv_clrov<mode>"
  [(unspec_volatile:GPR [(const_int 0)] UNSPEC_CLROV)]
  "TARGET_DSP"
  "csrrci zero, ucode, 1"
  [(set_attr "mode" "<MODE>")])

(define_expand "kdmabb"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kdma_internal (operands[0], operands[2], operands[3],
				GEN_INT (0), GEN_INT (0), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kdmabb32"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "r")
		    (match_operand:SI 3 "register_operand" "r")] UNSPEC_KDMABB))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmabb\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "SI")])

(define_expand "kdmabb64"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kdma64_internal (operands[0], operands[2], operands[3],
				  GEN_INT (0), GEN_INT (0), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kdmabt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kdma_internal (operands[0], operands[2], operands[3],
				GEN_INT (0), GEN_INT (1), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kdmabt32"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "r")
		    (match_operand:SI 3 "register_operand" "r")] UNSPEC_KDMABT))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmabt\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "SI")])

(define_expand "kdmabt64"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kdma64_internal (operands[0], operands[2], operands[3],
				  GEN_INT (0), GEN_INT (1), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kdmatt"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:SI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:V2HI 3 "register_operand" "")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_kdma_internal (operands[0], operands[2], operands[3],
				GEN_INT (1), GEN_INT (1), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kdmatt32"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "r")
		    (match_operand:SI 3 "register_operand" "r")] UNSPEC_KDMATT))]
  "TARGET_DSP && TARGET_64BIT"
  "kdmatt\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "SI")])

(define_expand "kdmatt64"
  [(match_operand:DI 0 "register_operand" "")
   (match_operand:DI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kdma64_internal (operands[0], operands[2], operands[3],
				  GEN_INT (1), GEN_INT (1), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "kdma_internal"
  [(set (match_operand:SI 0 "register_operand"                      "=   r,   r,   r,   r")
	(ss_plus:SI
	  (ashift:SI
	    (mult:SI
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 1 "register_operand"          "   r,   r,   r,   r")
		  (parallel [(match_operand:SI 3 "imm_0_1_operand"  " v00, v00, v01, v01")])))
	      (sign_extend:SI
		(vec_select:HI
		  (match_operand:V2HI 2 "register_operand"          "   r,   r,   r,   r")
		  (parallel [(match_operand:SI 4 "imm_0_1_operand"  " v00, v01, v01, v00")]))))
	    (const_int 1))
	  (match_operand:SI 5 "register_operand"                    "   0,   0,   0,   0")))]
  "TARGET_DSP && !TARGET_64BIT"
  "@
   kdmabb\t%0, %1, %2
   kdmabt\t%0, %1, %2
   kdmatt\t%0, %1, %2
   kdmabt\t%0, %2, %1"
  [(set_attr "type" "dmac")
   (set_attr "mode" "SI")])

(define_insn "kdma64_internal"
  [(set (match_operand:DI 0 "register_operand"                        "=   r,   r,   r,   r")
	(sign_extend:DI
	  (ss_plus:SI
	    (ashift:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 1 "register_operand"          "   r,   r,   r,   r")
		    (parallel [(match_operand:SI 3 "imm_0_1_operand"  " v00, v00, v01, v01")])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand"          "   r,   r,   r,   r")
		    (parallel [(match_operand:SI 4 "imm_0_1_operand"  " v00, v01, v01, v00")]))))
	      (const_int 1))
	    (truncate:SI
	      (match_operand:DI 5 "register_operand"                  "   0,   0,   0,   0")))))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   kdmabb\t%0, %1, %2
   kdmabt\t%0, %1, %2
   kdmatt\t%0, %1, %2
   kdmabt\t%0, %2, %1"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_expand "kdmabb16"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vkdma_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), GEN_INT (0), GEN_INT (2),
				 GEN_INT (2), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kdmabt16"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vkdma_internal (operands[0], operands[2], operands[3],
				 GEN_INT (0), GEN_INT (1), GEN_INT (2),
				 GEN_INT (3), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "kdmatt16"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V2SI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V4HI 3 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_vkdma_internal (operands[0], operands[2], operands[3],
				 GEN_INT (1), GEN_INT (1), GEN_INT (3),
				 GEN_INT (3), operands[1]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "vkdma_internal"
  [(set (match_operand:V2SI 0 "register_operand"                      "=   r,   r,   r,   r")
	(ss_plus:V2SI
	  (vec_concat:V2SI
	    (ashift:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 1 "register_operand"          "   r,   r,   r,   r")
		    (parallel [(match_operand:SI 3 "imm_0_1_operand"  " v00, v00, v01, v01")])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_operand:V4HI 2 "register_operand"          "   r,   r,   r,   r")
		    (parallel [(match_operand:SI 4 "imm_0_1_operand"  " v00, v01, v01, v00")]))))
	      (const_int 1))
	    (ashift:SI
	      (mult:SI
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 1)
		    (parallel [(match_operand:SI 5 "imm_2_3_operand"  " v02, v02, v03, v03")])))
		(sign_extend:SI
		  (vec_select:HI
		    (match_dup 2)
		    (parallel [(match_operand:SI 6 "imm_2_3_operand"  " v02, v03, v03, v02")]))))
	      (const_int 1)))
	  (match_operand:V2SI 7 "register_operand"                    "   0,   0,   0,   0")))]
  "TARGET_DSP && TARGET_64BIT"
  "@
   kdmabb16\t%0, %1, %2
   kdmabt16\t%0, %1, %2
   kdmatt16\t%0, %1, %2
   kdmabt16\t%0, %2, %1"
  [(set_attr "type" "dmac")
   (set_attr "mode" "V2SI")])

(define_expand "smaqa"
  [(match_operand:SI 0 "register_operand")
   (match_operand:SI 1 "register_operand")
   (match_operand:V4QI 2 "register_operand")
   (match_operand:V4QI 3 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_smaqav4qi (operands[0], operands[1],
			    operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "umaqa"
  [(match_operand:SI 0 "register_operand")
   (match_operand:SI 1 "register_operand")
   (match_operand:V4QI 2 "register_operand")
   (match_operand:V4QI 3 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_umaqav4qi (operands[0], operands[1],
			    operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "<su>maqav4qi"
  [(set (match_operand:SI 0 "register_operand"             "=r")
	(plus:SI (match_operand:SI 1 "register_operand"    " 0")
	  (plus:SI (plus:SI
	    (sign_extend:SI
	      (mult:HI
		(any_extend:HI
		  (vec_select:QI
		    (match_operand:V4QI 2 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(any_extend:HI
		  (vec_select:QI
		    (match_operand:V4QI 3 "register_operand" "r")
		    (parallel [(const_int 0)])))))
	    (sign_extend:SI
	      (mult:HI
		(any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		(any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 1)]))))))
	  (plus:SI
	    (sign_extend:SI
	      (mult:HI
		(any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		(any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 2)])))))
	    (sign_extend:SI
	      (mult:HI
		(any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		(any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 3)])))))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "<su>maqa\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "DI")])

(define_expand "smaqa64"
  [(match_operand:V2SI 0 "register_operand")
   (match_operand:V2SI 1 "register_operand")
   (match_operand:V8QI 2 "register_operand")
   (match_operand:V8QI 3 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_smaqav8qi (operands[0], operands[1],
			    operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_expand "umaqa64"
  [(match_operand:V2SI 0 "register_operand")
   (match_operand:V2SI 1 "register_operand")
   (match_operand:V8QI 2 "register_operand")
   (match_operand:V8QI 3 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_umaqav8qi (operands[0], operands[1],
			    operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "<su>maqav8qi"
  [(set (match_operand:V2SI 0 "register_operand"             "=r")
	(plus:V2SI (match_operand:V2SI 1 "register_operand"  " 0")
	  (vec_concat:V2SI
	    (plus:SI
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI
		      (vec_select:QI
			(match_operand:V8QI 2 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (any_extend:HI
		      (vec_select:QI
			(match_operand:V8QI 3 "register_operand" "r")
			(parallel [(const_int 0)])))))
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 1)]))))))
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 2)])))))
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 3)])))))))
	    (plus:SI
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 4)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 4)])))))
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 5)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 5)]))))))
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 6)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 6)])))))
		(sign_extend:SI
		  (mult:HI
		    (any_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 7)])))
		    (any_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 7)]))))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "<su>maqa\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "V2SI")])

(define_expand "sumaqa"
  [(match_operand:SI 0 "register_operand")
   (match_operand:SI 1 "register_operand")
   (match_operand:V4QI 2 "register_operand")
   (match_operand:V4QI 3 "register_operand")]
  "TARGET_DSP && !TARGET_64BIT"
{
  emit_insn (gen_sumaqav4qi (operands[0], operands[1],
			     operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "sumaqav4qi"
  [(set (match_operand:SI 0 "register_operand"             "=r")
	(plus:SI (match_operand:SI 1 "register_operand"    " 0")
	  (plus:SI (plus:SI
	    (sign_extend:SI
	      (mult:HI
		(sign_extend:HI
		  (vec_select:QI
		    (match_operand:V4QI 2 "register_operand" "r")
		    (parallel [(const_int 0)])))
		(zero_extend:HI
		  (vec_select:QI
		    (match_operand:V4QI 3 "register_operand" "r")
		    (parallel [(const_int 0)])))))
	    (sign_extend:SI
	      (mult:HI
		(sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		(zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 1)]))))))

	  (plus:SI
	    (sign_extend:SI
	      (mult:HI
		(sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		(zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 2)])))))
	    (sign_extend:SI
	      (mult:HI
		(sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		(zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 3)])))))))))]
  "TARGET_DSP && !TARGET_64BIT"
  "smaqa.su\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "SI")])

(define_expand "sumaqa64"
  [(match_operand:V2SI 0 "register_operand")
   (match_operand:V2SI 1 "register_operand")
   (match_operand:V8QI 2 "register_operand")
   (match_operand:V8QI 3 "register_operand")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_sumaqav8qi (operands[0], operands[1],
			     operands[2], operands[3]));
  DONE;
}
[(set_attr "type" "dmac")])

(define_insn "sumaqav8qi"
  [(set (match_operand:V2SI 0 "register_operand"             "=r")
	(plus:V2SI (match_operand:V2SI 1 "register_operand"  " 0")
	  (vec_concat:V2SI
	    (plus:SI
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI
		      (vec_select:QI
			(match_operand:V8QI 2 "register_operand" "r")
			(parallel [(const_int 0)])))
		    (zero_extend:HI
		      (vec_select:QI
			(match_operand:V8QI 3 "register_operand" "r")
			(parallel [(const_int 0)])))))
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 1)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 1)]))))))
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 2)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 2)])))))
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 3)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 3)])))))))
	    (plus:SI
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 4)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 4)])))))
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 5)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 5)]))))))
	      (plus:SI
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 6)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 6)])))))
		(sign_extend:SI
		  (mult:HI
		    (sign_extend:HI (vec_select:QI (match_dup 2) (parallel [(const_int 7)])))
		    (zero_extend:HI (vec_select:QI (match_dup 3) (parallel [(const_int 7)]))))))))))]
  "TARGET_DSP && TARGET_64BIT"
  "smaqa.su\t%0, %2, %3"
  [(set_attr "type" "dmac")
   (set_attr "mode" "V2SI")])

(define_insn "<optab><mode>2"
  [(set (match_operand:VECI 0 "register_operand" "=r")
        (unop:VECI (match_operand:VECI 1 "register_operand" "r")))]
  "TARGET_DSP"
  "<insn><bits>\t%0, %1"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "<optab>si2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unop:SI (match_operand:SI 1 "register_operand" "r")))]
  "TARGET_DSP && !TARGET_64BIT"
  "<insn>32\t%0, %1"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_insn "clo<mode>2"
  [(set (match_operand:VECI 0 "register_operand"               "=r")
	(unspec:VECI [(match_operand:VECI 1 "register_operand" " r")]
		      UNSPEC_CLO))]
  "TARGET_DSP"
  "clo<bits>\t%0, %1"
  [(set_attr "type" "dalu")
   (set_attr "mode" "<MODE>")])

(define_insn "closi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
        (unspec:SI [(match_operand:SI 1 "register_operand" "r")]
		    UNSPEC_CLO))]
  "TARGET_DSP && !TARGET_64BIT"
  "clo32\t%0, %1"
  [(set_attr "type" "dalu")
   (set_attr "mode" "SI")])

(define_expand "stas<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_stas<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "stas<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP"
  "stas<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "kstas<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_kstas<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kstas<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (ss_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (ss_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP"
  "kstas<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "ukstas<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_ukstas<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukstas<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (us_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (us_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))))
	  (const_int 1)))]
  "TARGET_DSP"
  "ukstas<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "stsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_stsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "stsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP"
  "stsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "kstsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_kstsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kstsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (ss_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (ss_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP"
  "kstsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "ukstsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_ukstsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukstsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"         "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (us_minus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_operand:VSHI 1 "register_operand" " r")
		(parallel [(const_int 0)]))
	      (vec_select:<VNHALF>
		(match_operand:VSHI 2 "register_operand" " r")
		(parallel [(const_int 0)]))))
	  (vec_duplicate:VSHI
	    (us_plus:<VNHALF>
	      (vec_select:<VNHALF>
		(match_dup 1)
		(parallel [(const_int 1)]))
	      (vec_select:<VNHALF>
		(match_dup 2)
		(parallel [(const_int 1)]))))
	  (const_int 2)))]
  "TARGET_DSP"
  "ukstsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "rstas<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_rstas<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rstas<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
		(minus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 0)])))
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 1)])))
		  (sign_extend:SI
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 1)))]
  "TARGET_DSP"
  "rstas<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "urstas<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_urstas<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urstas<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
		(minus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 0)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 1)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 1)))]
  "TARGET_DSP"
  "urstas<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "rstsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_rstsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rstsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
	        (minus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 0)])))
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (ashiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (sign_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 2)))]
  "TARGET_DSP"
  "rstsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "urstsa<mode>"
  [(match_operand:VSHI 0 "register_operand" "")
   (match_operand:VSHI 1 "register_operand" "")
   (match_operand:VSHI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_urstsa<mode>_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urstsa<mode>_le"
  [(set (match_operand:VSHI 0 "register_operand"           "=r")
	(vec_merge:VSHI
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
	        (minus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 1 "register_operand" " r")
		      (parallel [(const_int 0)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_operand:VSHI 2 "register_operand" " r")
		      (parallel [(const_int 0)]))))
		(const_int 1))))
	  (vec_duplicate:VSHI
	    (truncate:<VNHALF>
	      (lshiftrt:<VSH_EXT>
		(plus:<VSH_EXT>
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 1)
		      (parallel [(const_int 1)])))
		  (zero_extend:<VSH_EXT>
		    (vec_select:<VNHALF>
		      (match_dup 2)
		      (parallel [(const_int 1)]))))
		(const_int 1))))
	  (const_int 2)))]
  "TARGET_DSP"
  "urstsa<bits>\t%0, %1, %2"
  [(set_attr "type" "dalu")]
)

(define_expand "stas16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_stas16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "stas16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (minus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				     (parallel [(const_int 0)]))
		      (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				     (parallel [(const_int 0)])))
	    (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
		     (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
		      (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (plus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
		      (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "stas16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "kstas16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kstas16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kstas16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (ss_minus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
					(parallel [(const_int 0)]))
			 (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
					(parallel [(const_int 0)])))
	    (ss_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (ss_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (ss_plus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kstas16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "ukstas16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP"
{
  emit_insn (gen_ukstas16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukstas16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (us_minus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
					(parallel [(const_int 0)]))
			 (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
					(parallel [(const_int 0)])))
	    (us_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (us_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (us_plus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "ukstas16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "stsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_stsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "stsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (plus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				    (parallel [(const_int 0)]))
		     (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				    (parallel [(const_int 0)])))
	    (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
		      (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
		     (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (minus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
		       (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "stsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "kstsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_kstsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "kstsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (ss_plus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				       (parallel [(const_int 0)]))
			(vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				       (parallel [(const_int 0)])))
	    (ss_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (ss_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (ss_minus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			  (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "kstsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "ukstsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_ukstsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "ukstsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (us_plus:HI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
				       (parallel [(const_int 0)]))
			(vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
				       (parallel [(const_int 0)])))
	    (us_minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
			 (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	  (vec_concat:V2HI
	    (us_plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
			(vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	    (us_minus:HI  (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
			  (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))))]
  "TARGET_DSP && TARGET_64BIT"
  "ukstsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "rstas16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_rstas16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rstas16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 0)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "rstas16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "urstas16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_urstas16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urstas16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 0)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "urstas16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "rstsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_rstsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "rstsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (sign_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 0)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (ashiftrt:SI
		(plus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1)))
	    (truncate:HI
	      (ashiftrt:SI
		(minus:SI
		  (sign_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (sign_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "rstsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])

(define_expand "urstsa16_64"
  [(match_operand:V4HI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")]
  "TARGET_DSP && TARGET_64BIT"
{
  emit_insn (gen_urstsa16_64_le (operands[0], operands[1], operands[2]));
  DONE;
}
[(set_attr "type" "dalu")])

(define_insn "urstsa16_64_le"
  [(set (match_operand:V4HI 0 "register_operand"         "=r")
	(vec_concat:V4HI
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 1 "register_operand" " r")
						 (parallel [(const_int 0)])))
		  (zero_extend:SI (vec_select:HI (match_operand:V4HI 2 "register_operand" " r")
						  (parallel [(const_int 0)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
		(const_int 1))))
	  (vec_concat:V2HI
	    (truncate:HI
	      (lshiftrt:SI
		(plus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 2)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 2)]))))
		(const_int 1)))
	    (truncate:HI
	      (lshiftrt:SI
		(minus:SI
		  (zero_extend:SI (vec_select:HI (match_dup 1) (parallel [(const_int 3)])))
		  (zero_extend:SI (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
		(const_int 1))))))]
  "TARGET_DSP && TARGET_64BIT"
  "urstsa16\t%0, %1, %2"
  [(set_attr "type" "dalu")
   (set_attr "mode" "V4HI")])
