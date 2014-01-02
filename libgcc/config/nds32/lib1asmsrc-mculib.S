/* mculib libgcc routines of Andes NDS32 cpu for GNU compiler
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

   Under Section 7 of GPL version 3, you are granted additional
   permissions described in the GCC Runtime Library Exception, version
   3.1, as published by the Free Software Foundation.

   You should have received a copy of the GNU General Public License and
   a copy of the GCC Runtime Library Exception along with this program;
   see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
   <http://www.gnu.org/licenses/>.  */

	.section	.mdebug.abi_nds32
	.previous


/* ------------------------------------------- */
/* FPBIT floating point operations for libgcc  */
/* ------------------------------------------- */

#ifdef L_addsub_sf

#define VALUA	$r2	// A<<1
#define VALUB	$r3	// B<<1
#define EXPOA	$r4	// exponent(A)
#ifdef __NDS32_REDUCE_REGS__
#define EXPOB	$r8	// exponent(B)
#define MANTA	$r6	// mantissa(A) related
#define MANTB	$r9	// mantissa(B) related
#define SIGN	$r7	// 0x80000000
#else
#define EXPOB	$r18	// exponent(B)
#define MANTA	$r16	// mantissa(A) related
#define MANTB	$r19	// mantissa(B) related
#define SIGN	$r17	// 0x80000000
#endif
#define RB	$r5

	.text
	.align	2
	.global	__subsf3
	.type	__subsf3, @function
__subsf3:
#ifdef __NDS32_EXT_PERF__
	btgl	$r1, $r1, 31
#else
	move	$r2, #0x80000000
	xor	$r1, $r1, $r2		! A-B is now A+(-B)
#endif

	.global	__addsf3
	.type	__addsf3, @function
__addsf3:
	slli	VALUA, $r0, 1		! A<<1
#ifdef __NDS32_REDUCE_REGS__
	smw.adm	$r6, [$sp], $r9, 0
#endif

	slli	VALUB, $r1, 1		! B<<1
	move	SIGN, #0x80000000
	slt	$r15, VALUA, VALUB	! absolute value(A)<absolute value(B)?
	beqz	$r15, .LEcont
	move	RB, $r0			! yes, swap A and B
	move	$r0, $r1
	move	$r1, RB
	slli	VALUA, $r0, 1		! A<<1
	slli	VALUB, $r1, 1		! B<<1

	! ---------------------------------------------------------------------
	! absolute value(A) >= absolute value(B)
	! ---------------------------------------------------------------------
.LEcont:
	xor	$r1, $r1, $r0
	and	$r1, $r1, SIGN		! sign(A xor B)
	srli	EXPOA, VALUA, 24	! exponent(A)
	srli	EXPOB, VALUB, 24	! exponent(B)
	slli	MANTA, VALUA, 7		! mantissa(A)<<8
	slli	MANTB, VALUB, 7		! mantissa(B)<<8
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOA, 0xff, .LEinfnan	! A is inf or NaN, goto .LEinfnan
#else
	move	RB, #0xff
	beq	RB, EXPOA, .LEinfnan	! A is inf or NaN, goto .LEinfnan
#endif
	! A is finite and thus B can only be finite
	beqz	VALUA, .LEzeroP		! A is zero, simply return zero
	beqz	VALUB, .LEretA		! B is zero, simply return A
	sub	RB, EXPOA, EXPOB	! exponent(A)-exponent(B)
	slti	$r15, EXPOA, #0x2
	bnez	$r15, .LElab2		! exponent(A) is 0 or 1, goto .LElab2
	sltsi	$r15, RB, #0x20
	beqz	$r15, .LEretA		! B is insignificant, simply return A
	or	MANTA, MANTA, SIGN	! decimal-part(A)
	beqz	EXPOB, .LElab1
	or	MANTB, MANTB, SIGN	! decimal-part(B)

.LElab1:
	addi	$r15, RB, -1
	cmovz	RB, $r15, EXPOB
	move	$r15, MANTB
	srl	MANTB, MANTB, RB
	sll	RB, MANTB, RB
	beq	RB, $r15, .LElab2
	ori	MANTB, MANTB, #2	! B is quite small comapre to A

.LElab2:
	bnez	$r1, .LEsub		! different sign, do subtraction

	! ---------------------------------------------------------------------
	! same sign, do addition
	! ---------------------------------------------------------------------
	add	MANTA, MANTA, MANTB
	slt	$r15, MANTA, MANTB
	beqz	$r15, .LEaddnoover	! no overflow, goto .LEaddnoover
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOA, #0xfe, .LEinf
#else
	subri	$r15, EXPOA, #0xfe
	beqz	$r15, .LEinf
#endif
	andi	$r15, MANTA, #1
	ori	RB, MANTA, #2
	cmovn	MANTA, RB, $r15
	srli	MANTA, MANTA, #1
	addi	EXPOA, EXPOA, #1
	b	.LEround

.LEaddnoover:
	bnez	EXPOA, .LEround		! special handling when exponent(A) is zero

.LEdenorm:
	srli	MANTA, MANTA, #8	! 0x008nnnnn-0x00fnnnnn
	b	.LEpack

	! ---------------------------------------------------------------------
	! different sign, do subtraction
	! ---------------------------------------------------------------------
.LEsub:
	beq	VALUA, VALUB, .LEzero
	slt	$r15, MANTA, MANTB
	beqz	$r15, .LEsub2
	srli	MANTB, MANTB, 1
	addi	EXPOA, EXPOA, -1

.LEsub2:
	sub	MANTA, MANTA, MANTB
	slti	$r15, EXPOA, 2
	bnez	$r15, .LEdenorm		! only when exponent(A,B) is (0,0) or (1,0/1)
#ifdef __NDS32_EXT_PERF__
	clz	RB, MANTA
	slt	$r15, RB, EXPOA
	subri	$r15, $r15, #1
	min	RB, RB, EXPOA
	sub	EXPOA, EXPOA, RB
	sub	RB, RB, $r15
	sll	MANTA, MANTA, RB
#else
	b	.LEloopC2

.LEloopC:
	addi	EXPOA, EXPOA, #-1
	beqz	EXPOA, .LEround
	add	MANTA, MANTA, MANTA

.LEloopC2:
	slt	$r15, MANTA, SIGN
	bnez	$r15, .LEloopC
#endif

	! ---------------------------------------------------------------------
	! do rounding
	! ---------------------------------------------------------------------
.LEround:
	addi	MANTA, MANTA, #128
	slti	$r15, MANTA, #128
	add	EXPOA, EXPOA, $r15
	srli	RB, MANTA, 8
	andi	RB, RB, 1
	sub	MANTA, MANTA, RB

	! ---------------------------------------------------------------------
	! pack result
	! ---------------------------------------------------------------------
	slli	MANTA, MANTA, #1	! shift out implied 1
	srli	MANTA, MANTA, #9
	slli	$r1, EXPOA, #23
	or	MANTA, MANTA, $r1
.LEpack:
	and	$r0, $r0, SIGN
	or	$r0, $r0, MANTA

.LEretA:
.LEret:
#ifdef __NDS32_REDUCE_REGS__
	lmw.bim	$r6, [$sp], $r9, 0
#endif
	ret5	$lp

	! 0.0f and -0.0f handling: both A and B are zeroes
.LEzeroP:
	beqz	$r1, .LEretA		! A and B same sign: return A

.LEzero:
	move	$r0, #0			! return +0.0f
	b	.LEret

	! ---------------------------------------------------------------------
	! exponent(A) is 0xff: A is inf or NaN
	! ---------------------------------------------------------------------
.LEinfnan:
	bne	MANTA, SIGN, .LEnan	! A is NaN, goto .LEnan
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	bnec	EXPOB, #0xff, .LEretA	! B is finite, return A
#else
	bne	RB, EXPOB, .LEretA	! B is finite, return A
#endif

	! both A and B are inf
	beqz	$r1, .LEretA		! same sign, return inf

.LEnan:
	move	$r0, #0xffc00000	! return NaN
	b	.LEret

.LEinf:
	move	MANTA, 0x7f800000	! return inf
	b	.LEpack
	.size	__subsf3, .-__subsf3
	.size	__addsf3, .-__addsf3
#endif /* L_addsub_sf */



#ifdef L_fixsfsi

#define VALUA	$r1	// A<<1
#define EXPOA	VALUA	// exponent(A)
#define MANTA	$r2	// mantissa(A) related
#define RA	$r4
#define RB	$r5

	.text
	.align	2
	.global	__fixsfsi
	.type	__fixsfsi, @function
__fixsfsi:
#if defined(__NDS32_EXT_FPU_SP)
        fs2si.z   $fs0, $fs0
        fmfsr   $r0, $fs0
        ret5    $lp
#else
	slli	VALUA, $r0, #1
	slli	MANTA, VALUA, #7
	srli	EXPOA, VALUA, #24
	subri	EXPOA, EXPOA, #0x9e
#if defined(__OPTIMIZE_SIZE__)||!defined(__NDS32_EXT_PERF__)
	move	RB, #0x80000000
#endif
	blez	EXPOA, .LJover		! number is too big
	sltsi	$r15, EXPOA, #0x20
	beqz	$r15, .LJzero		! number is too small

#if defined(__NDS32_EXT_PERF__)&&!defined(__OPTIMIZE_SIZE__)
	bset	MANTA, MANTA, 31
#else
	or	MANTA, MANTA, RB
#endif
	srl	MANTA, MANTA, EXPOA
	sltsi	$r15, $r0, #0
	subri	$r0, MANTA, #0
	cmovz	$r0, MANTA, $r15
	ret5	$lp

.LJzero:
	move	$r0, #0
	ret5	$lp

.LJover:
	move	RA, #0x7f800000
	slt	$r15, RA, $r0
	beqzs8	.LJnan
#if defined(__NDS32_EXT_PERF__)&&!defined(__OPTIMIZE_SIZE__)
	move	$r0, #0x80000000
#else
	move	$r0, RB
#endif
	ret5	$lp
.LJnan:
#if defined(__NDS32_EXT_PERF__)&&!defined(__OPTIMIZE_SIZE__)
	move	$r0, #0x7fffffff
#else
	addi	$r0, RB, -1
#endif
	ret5	$lp
#endif
	.size	__fixsfsi, .-__fixsfsi
#endif /* L_fixsfsi */



#ifdef L_divsi3

	.text
	.align	2
	.globl	__divsi3
	.type	__divsi3, @function
__divsi3:
	! ---------------------------------------------------------------------
	! neg = 0;
	! if (a < 0)
	! {   a = -a;
	!     neg = !neg;
	! }
	! ---------------------------------------------------------------------
	sltsi	$r5, $r0, 0			! $r5  <- neg = (a < 0) ? 1 : 0
	subri	$r4, $r0, 0			! $r4  <- a = -a
	cmovn	$r0, $r4, $r5			! $r0  <- a = neg ? -a : a
.L2:
	! ---------------------------------------------------------------------
	! if (b < 0)
	! ---------------------------------------------------------------------
	bgez	$r1, .L3			! if b >= 0, skip
	! ---------------------------------------------------------------------
	! {   b=-b;
	!     neg=!neg;
	! }
	! ---------------------------------------------------------------------
	subri	$r1, $r1, 0			! $r1  <- b = -b
	subri	$r5, $r5, 1			! $r5  <- neg = !neg
.L3:
	! ---------------------------------------------------------------------
	!!res = udivmodsi4 (a, b, 1);
	! res = 0;
	! if (den != 0)
	! ---------------------------------------------------------------------
	movi	$r2, 0				! $r2  <- res = 0
	beqz	$r1, .L1			! if den == 0, skip
	! ---------------------------------------------------------------------
	! bit = 1;
	! ---------------------------------------------------------------------
	movi	$r4, 1				! $r4  <- bit = 1
#ifndef __OPTIMIZE_SIZE__
.L6:
#endif
	! ---------------------------------------------------------------------
	! while (den < num && bit && !(den & (1L << 31)))
	! ---------------------------------------------------------------------
	slt	$ta, $r1, $r0			! $ta  <- den < num ?
	beqz	$ta, .L5			! if no, skip
	! ---------------------------------------------------------------------
	! {   den << = 1;
	!     bit << = 1;
	! }
	! ---------------------------------------------------------------------
#if defined (__OPTIMIZE_SIZE__) && !defined (__NDS32_ISA_V3M__)
	clz	$r3, $r1			! $r3  <- leading zero count for den
	clz	$ta, $r0			! $ta  <- leading zero count for num
	sub	$r3, $r3, $ta			! $r3  <- number of bits to shift
	sll	$r1, $r1, $r3			! $r1  <- den
	sll	$r4, $r4, $r3			! $r2  <- bit
#else
	slli	$r1, $r1, 1			! $r1  <- den << = 1
	slli	$r4, $r4, 1			! $r4  <- bit << = 1
	b	.L6				! continue loop
#endif
.L5:
	! ---------------------------------------------------------------------
	! while (bit)
	! {   if (num >= den)
	! ---------------------------------------------------------------------
	slt	$ta, $r0, $r1			! $ta  <- num < den ?
	bnez	$ta, .L9			! if yes, skip
	! ---------------------------------------------------------------------
	!     {   num -= den;
	!         res |= bit;
	!     }
	! ---------------------------------------------------------------------
	sub	$r0, $r0, $r1			! $r0  <- num -= den
	or	$r2, $r2, $r4			! $r2  <- res |= bit
.L9:
	! ---------------------------------------------------------------------
	!     bit >> = 1;
	!     den >> = 1;
	! }
	!!if (modwanted)
	!!    return num;
	!!return res;
	! ---------------------------------------------------------------------
	srli	$r4, $r4, 1			! $r4  <- bit >> = 1
	srli	$r1, $r1, 1			! $r1  <- den >> = 1
	bnez	$r4, .L5			! if bit != 0, continue loop
.L1:
	! ---------------------------------------------------------------------
	! if (neg)
	!     res = -res;
	! return res;
	! ---------------------------------------------------------------------
	subri	$r0, $r2, 0			! $r0  <- -res
	cmovz	$r0, $r2, $r5			! $r0  <- neg ? -res : res
	! ---------------------------------------------------------------------
	ret
	.size	__divsi3, .-__divsi3
#endif /* L_divsi3 */



#ifdef L_divdi3

#ifdef __big_endian__
#define RI	$r0
#define RH	$r1
#define RK	$r2
#define RJ	$r3
#else
#define RI	$r1
#define RH	$r0
#define RK	$r3
#define RJ	$r2
#endif

	.text
	.align	2
	.globl	__divdi3
	.type	__divdi3, @function
__divdi3:
	! =====================================================================
	! uint64_t __divdi3(uint64_t n, uint64-t d)
	!
	! This function divides n by d and returns the quotient.
	!
	! stack allocation:
	! sp+8  +-----------------------+
	!       | $lp                   |
	! sp+4  +-----------------------+
	!       | $r6                   |
	! sp    +-----------------------+
	! =====================================================================
	smw.adm	$r6, [$sp], $r6, 2

	xor	$r6, RI, RK
	srai45	$r6, 31			! signof(numerator xor denominator)
	! abs(denominator)
	bgez	RK, .L80
	neg	RK, RK
	beqz	RJ, .L80
	neg	RJ, RJ
	addi	RK, RK, -1

.L80:
	! abs(numerator)
	bgez	RI, .L81
	neg	RI, RI
	beqz	RH, .L81
	neg	RH, RH
	addi	RI, RI, -1

.L81:
	! abs(numerator) / abs(denominator)
	movi	$r4, 0			! ignore remainder
	bal	__udivmoddi4
	! numerator / denominator
	beqz	$r6, .L82
	or	$r4, RI, RH
	beqz	$r4, .L82
	neg	RI, RI
	beqz	RH, .L82
	neg	RH, RH
	addi	RI, RI, -1

	! to eliminate unaligned branch target
	.align	2
.L82:
	lmw.bim	$r6, [$sp], $r6, 2
	ret
	.size	__divdi3, .-__divdi3
#endif /* L_divdi3 */



#ifdef L_modsi3

	.text
	.align	2
	.globl	__modsi3
	.type	__modsi3, @function
__modsi3:
	! ---------------------------------------------------------------------
	! neg=0;
	! if (a<0)
	! {   a=-a;
	!     neg=1;
	! }
	! ---------------------------------------------------------------------
	sltsi	$r5, $r0, 0			! $r5  <- neg < 0 ? 1 : 0
	subri	$r4, $r0, 0			! $r4  <- -a
	cmovn	$r0, $r4, $r5			! $r0  <- |a|
	! ---------------------------------------------------------------------
	! if (b < 0)
#ifndef __NDS32_EXT_PERF__
	! ---------------------------------------------------------------------
	bgez	$r1, .L3			! if b >= 0, skip
	! ---------------------------------------------------------------------
	!     b = -b;
	! ---------------------------------------------------------------------
	subri	$r1, $r1, 0			! $r1  <- |b|
.L3:
	! ---------------------------------------------------------------------
	!!res = udivmodsi4 (a, b, 1);
	! if (den != 0)
	! ---------------------------------------------------------------------
#else /* __NDS32_EXT_PERF__ */
	!     b = -b;
	!!res = udivmodsi4 (a, b, 1);
	! if (den != 0)
	! ---------------------------------------------------------------------
	abs	$r1, $r1			! $r1  <- |b|
#endif /* __NDS32_EXT_PERF__ */
	beqz	$r1, .L1			! if den == 0, skip
	! ---------------------------------------------------------------------
	! {   bit = 1;
	!     res = 0;
	! ---------------------------------------------------------------------
	movi	$r4, 1				! $r4  <- bit = 1
#if !(defined (__OPTIMIZE_SIZE__) && ! defined (__NDS32_ISA_V3M__))
.L6:
#endif
	! ---------------------------------------------------------------------
	!     while (den < num&&bit && !(den & (1L << 31)))
	! ---------------------------------------------------------------------
	slt	$ta, $r1, $r0			! $ta  <- den < num ?
	beqz	$ta, .L5			! if no, skip
	! ---------------------------------------------------------------------
	!     {   den << = 1;
	!         bit << = 1;
	!     }
	! ---------------------------------------------------------------------
#if defined (__OPTIMIZE_SIZE__) && ! defined (__NDS32_ISA_V3M__)
	clz	$r3, $r1			! $r3  <- leading zero count for den
	clz	$ta, $r0			! $ta  <- leading zero count for num
	sub	$r3, $r3, $ta			! $r3  <- number of bits to shift
	sll	$r1, $r1, $r3			! $r1  <- den
	sll	$r4, $r4, $r3			! $r2  <- bit
#else
	slli	$r1, $r1, 1			! $r1  <- den << = 1
	slli	$r4, $r4, 1			! $r4  <- bit << = 1
	b	.L6				! continue loop
#endif
.L5:
	! ---------------------------------------------------------------------
	!     while (bit)
	!     {   if (num >= den)
	!         {   num -= den;
	!             res |= bit;
	!         }
	!         bit >> = 1;
	!         den >> = 1;
	!     }
	! }
	!!if (modwanted)
	!!    return num;
	!!return res;
	! ---------------------------------------------------------------------
	sub	$r2, $r0, $r1			! $r2  <- num - den
	slt	$ta, $r0, $r1			! $ta  <- num < den ?
	srli	$r4, $r4, 1			! $r4  <- bit >> = 1
	cmovz	$r0, $r2, $ta			! $r0  <- num = (num < den) ? num : num - den
	srli	$r1, $r1, 1			! $r1  <- den >> = 1
	bnez	$r4, .L5			! if bit != 0, continue loop
.L1:
	! ---------------------------------------------------------------------
	! if (neg)
	!     res = -res;
	! return res;
	! ---------------------------------------------------------------------
	subri	$r3, $r0, 0			! $r3  <- -res
	cmovn	$r0, $r3, $r5			! $r0  <- neg ? -res : res
	! ---------------------------------------------------------------------
	ret
	.size	__modsi3, .-__modsi3
#endif /* L_modsi3 */



#ifdef L_moddi3

#ifdef __big_endian__
#define RI		$r0
#define RH		$r1
#define RK		$r2
#define RJ		$r3
#define OFFSET_H	0
#define OFFSET_L	4
#else
#define RI		$r1
#define RH		$r0
#define RK		$r3
#define RJ		$r2
#define OFFSET_H	4
#define OFFSET_L	0
#endif

	.text
	.align	2
	.globl	__moddi3
	.type	__moddi3, @function
__moddi3:
	! =====================================================================
	! uint64_t __moddi3(uint64_t n, uint64-t d)
	!
	! This function divides n by d and returns the remainder.
	!
	! stack allocation:
	! sp+16 +-----------------------+
	!       | remainder             |
	! sp+8  +-----------------------+
	!       | $lp                   |
	! sp+4  +-----------------------+
	!       | $r6                   |
	! sp    +-----------------------+
	! =====================================================================
	addi	$sp, $sp, -16
	smw.bi	$r6, [$sp], $r6, 2

	srai	$r6, RI, 31		! signof(numerator)
	! abs(denominator)
	bgez	RK, .L80
	neg	RK, RK
	beqz	RJ, .L80
	neg	RJ, RJ
	addi	RK, RK, -1

.L80:
	! abs(numerator)
	beqz	$r6, .L81
	neg	RI, RI
	beqz	RH, .L81
	neg	RH, RH
	addi	RI, RI, -1

.L81:
	! abs(numerator) % abs(denominator)
	addi	$r4, $sp, 8
	bal	__udivmoddi4
	! numerator % denominator
	lwi	RH, [$sp+(8+OFFSET_L)]
	lwi	RI, [$sp+(8+OFFSET_H)]
	beqz	$r6, .L82
	or	$r4, RI, RH
	beqz	$r4, .L82
	neg	RI, RI
	beqz	RH, .L82
	neg	RH, RH
	addi	RI, RI, -1

	! to eliminate unaligned branch target
	.align	2
.L82:
	lmw.bi	$r6, [$sp], $r6, 2
	addi	$sp, $sp, 16
	ret
	.size	__moddi3, .-__moddi3
#endif /* L_moddi3 */



#ifdef L_mulsi3

	.text
	.align	2
	.globl	__mulsi3
	.type	__mulsi3, @function
__mulsi3:
	! ---------------------------------------------------------------------
	! r = 0;
	! while (a)
	! $r0:       r
	! $r1:       b
	! $r2:       a
	! ---------------------------------------------------------------------
	beqz	$r0, .L7			! if a == 0, done
	move	$r2, $r0			! $r2  <- a
	movi	$r0, 0				! $r0  <- r <- 0
.L8:
	! ---------------------------------------------------------------------
	! {   if (a & 1)
	!         r += b;
	!     a >> = 1;
	!     b << = 1;
	! }
	! $r0:       r
	! $r1:       b
	! $r2:       a
	! $r3:       scratch
	! $r4:       scratch
	! ---------------------------------------------------------------------
	andi	$r3, $r2, 1			! $r3  <- a & 1
	add	$r4, $r0, $r1			! $r4  <- r += b
	cmovn	$r0, $r4, $r3			! $r0  <- r
	srli	$r2, $r2, 1			! $r2  <- a >> = 1
	slli	$r1, $r1, 1			! $r1  <- b << = 1
	bnez	$r2, .L8			! if a != 0, continue loop
.L7:
	! ---------------------------------------------------------------------
	! $r0:       return code
	! ---------------------------------------------------------------------
	ret
	.size	__mulsi3, .-__mulsi3
#endif /* L_mulsi3 */



#ifdef L_udivsi3

	.text
	.align	2
	.globl	__udivsi3
	.type	__udivsi3, @function
__udivsi3:
	! ---------------------------------------------------------------------
	!!res=udivmodsi4(a,b,0);
	! res=0;
	! if (den!=0)
	! ---------------------------------------------------------------------
	movi	$r2, 0				! $r2  <- res=0
	beqz	$r1, .L1			! if den==0, skip
	! ---------------------------------------------------------------------
	! {   bit=1;
	! ---------------------------------------------------------------------
	movi	$r4, 1				! $r4  <- bit=1
#ifndef __OPTIMIZE_SIZE__
.L6:
#endif
	! ---------------------------------------------------------------------
	!     while (den<num
	! ---------------------------------------------------------------------
	slt	$ta, $r1, $r0			! $ta  <- den<num?
	beqz	$ta, .L5			! if no, skip
	! ---------------------------------------------------------------------
	!          &&bit&&!(den&(1L<<31)))
	! ---------------------------------------------------------------------
	bltz	$r1, .L5			! if den<0, skip
	! ---------------------------------------------------------------------
	!     {   den<<=1;
	!         bit<<=1;
	!     }
	! ---------------------------------------------------------------------
#if defined (__OPTIMIZE_SIZE__) && ! defined (__NDS32_ISA_V3M__)
	clz	$r3, $r1			! $r3  <- leading zero count for den
	clz	$ta, $r0			! $ta  <- leading zero count for num
	sub	$r3, $r3, $ta			! $r3  <- number of bits to shift
	sll	$r1, $r1, $r3			! $r1  <- den
	sll	$r2, $r2, $r3			! $r2  <- bit
#else
	slli	$r1, $r1, 1			! $r1  <- den<<=1
	slli	$r4, $r4, 1			! $r4  <- bit<<=1
	b	.L6				! continue loop
#endif
.L5:
	! ---------------------------------------------------------------------
	!     while (bit)
	!     {   if (num>=den)
	! ---------------------------------------------------------------------
	slt	$ta, $r0, $r1			! $ta  <- num<den?
	bnez	$ta, .L9			! if yes, skip
	! ---------------------------------------------------------------------
	!         {   num-=den;
	!             res|=bit;
	!         }
	! ---------------------------------------------------------------------
	sub	$r0, $r0, $r1			! $r0  <- num-=den
	or	$r2, $r2, $r4			! $r2  <- res|=bit
.L9:
	! ---------------------------------------------------------------------
	!         bit>>=1;
	!         den>>=1;
	!     }
	! }
	!!if (modwanted)
	!!    return num;
	!!return res;
	! ---------------------------------------------------------------------
	srli	$r4, $r4, 1			! $r4  <- bit>>=1
	srli	$r1, $r1, 1			! $r1  <- den>>=1
	bnez	$r4, .L5			! if bit!=0, continue loop
.L1:
	! ---------------------------------------------------------------------
	! return res;
	! ---------------------------------------------------------------------
	move	$r0, $r2			! $r0  <- return value
	! ---------------------------------------------------------------------
	! ---------------------------------------------------------------------
	ret
	.size	__udivsi3, .-__udivsi3
#endif /* L_udivsi3 */



#ifdef L_udivdi3

	.text
	.align	2
	.globl	__udivdi3
	.type	__udivdi3, @function
__udivdi3:
	movi	$r4, 0			! ignore remainder
	b	__udivmoddi4
	.size	__udivdi3, .-__udivdi3
#endif /* L_udivdi3 */



#ifdef L_umul_ppmm

#ifdef __big_endian__
	#define RI		$r0
	#define RH		$r1
	#define RK		$r2
	#define RJ		$r3
#else
	#define RI		$r1
	#define RH		$r0
	#define RK		$r3
	#define RJ		$r2
#endif
#define RB	$r5

	.text
	.align	2
	.globl	umul_ppmm
	.type	umul_ppmm, @function
	! =====================================================================
	! uint64_t umul_ppmm(uint32_t a, uint32_t b)
	!
	! This function multiplies `a' by `b' to obtain a 64-bit product. The
	! product is broken into two 32-bit pieces which are stored in the zl
	! (low-part at RH) and zh (high-part at RI).
	! =====================================================================
umul_ppmm:
	! ---------------------------------------------------------------------
	! uint16_t ah, al, bh, bl;
	! uint32_t zh, zA, zB, zl;
	! al = a&0xffff;
	! ah = a>>16;
	! bl = b&0xffff;
	! bh = b>>16;
	! ---------------------------------------------------------------------
	zeh	RJ, $r0	! al=a&0xffff
	srli	RK, $r0, 16	! ah=a>>16
#ifdef __NDS32_EB__
	srli	RI, $r1, 16	! bh=b>>16
	zeh	RH, $r1	! bl=b&0xffff
#else
	zeh	RH, $r1	! bl=b&0xffff
	srli	RI, $r1, 16	! bh=b>>16
#endif
	! ---------------------------------------------------------------------
	! zA = ( (uint32_t) al ) * bh;
	! zl = ( (uint32_t) al ) * bl;
	! zB = ( (uint32_t) ah ) * bl;
	! ---------------------------------------------------------------------
	mul	RB, RJ, RI	! zA=al*bh
	mul	RJ, RJ, RH	! zl=al*bl
	mul	RH, RK, RH	! zB=ah*bl
	! ---------------------------------------------------------------------
	! zh = ( (uint32_t) ah ) * bh;
	! zA += zB;
	! zh += ( ( (uint32_t) ( zA < zB ) )<<16 ) + ( zA>>16 );
	! ---------------------------------------------------------------------
	add	RB, RB, RH	! zA+=zB
	slt	$ta, RB, RH	! zA<zB
	slli	$ta, $ta, 16	! (zA<zB)<<16
	maddr32 $ta, RK, RI	! zh=ah*bh+((zA<zB)<<16)
	srli	RI, RB, 16	! zA>>16
	add	RI, RI, $ta	! zh+=(zA>>16)
	! ---------------------------------------------------------------------
	! zA <<= 16;
	! zl += zA;
	! zh += ( zl < zA );
	! *zlPtr = zl;
	! *zhPtr = zh;
	! ---------------------------------------------------------------------
	slli	RH, RB, 16	! zA<<=16
	add	RH, RH, RJ	! zl+=zA
	slt	$ta, RH, RJ	! zl<zA
	add	RI, RI, $ta	! zh+=(zl<zA)
	ret
	.size	umul_ppmm, .-umul_ppmm
#endif /* L_umul_ppmm */



#ifdef L_udivmoddi4

#ifdef __big_endian__
	#define RI		$r0
	#define RH		$r1
#else
	#define RI		$r1
	#define RH		$r0
#endif
#define RA	$r4
#define RB	$r5
#define RC	RH
#define NHI	RI		// n1
#define NLO	RH		// n0
#define D	$r2		// d
#define DLO	$r3		// d0
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
#define DHI	$r10		// d1
#else
#define DHI	$r20		// d1
#endif
#define Q	NHI		// q/q0
#define QHI	RA		// q1
#define R	NLO		// r/r0
#define RHI	NHI		// r1
#define M	RC		// m

	.text
	.align	2
	.type	fudiv_qrnnd, @function
	! =====================================================================
	! uint64_t fudiv_qrnnd(uint64_t n, uint32_t d)
	!
	! This function divides 64-bit numerator n by 32-bit denominator d. The
	! 64-bit return value contains remainder (low-part at RH) and quotient
	! (high-part at RI).
	! Caller has to make sure that DHI is saved if necessary.
	! =====================================================================
	!------------------------------------------------------
	!  in regs: ($r0,$r1) - NUMERATOR, $r2 - DENOMINATOR
	! out regs: ($r0,$r1) - (Q,R)
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	!  scratch: $r3-$r5, $r10, $ta
#else
	!  scratch: $r3-$r5, $ta, $r20
#endif
	!------------------------------------------------------
fudiv_qrnnd:
	!------------------------------------------------------
	!  __d1 = ((USItype) (d) >> (W_TYPE_SIZE / 2));
	!  __d0 = ((USItype) (d) & (((USItype) 1 << (W_TYPE_SIZE / 2)) - 1));
	!  __r1 = (n1) % __d1; __q1 = (n1) / __d1;
	!  __m = (USItype) __q1 * __d0;
	!  __r1 = __r1 * ((USItype) 1 << (W_TYPE_SIZE / 2)) | ((USItype) (n0) >> (W_TYPE_SIZE / 2));
	!  if (__r1 < __m) {
	!------------------------------------------------------
	srli	DHI, D, 16		! d1 = ll_highpart (d)
	zeh	RB, NLO			! ll_lowpart (n0)
	srli	RC, NLO, 16		! ll_highpart (n0)
	divr	QHI, RHI, NHI, DHI	! q1 = n1 / __d1, r1 = n1 % __d1
	zeh	DLO, D			! d0 = ll_lowpart (d)
	slli	RHI, RHI, 16		! r1 << 16
	or	RHI, RHI, RC		! __r1 = (__r1 << 16) | ll_highpart(n0)
	mul	M, QHI, DLO		! m =  __q1*__d0
	slt	$ta, RHI, M		! __r1 < __m
	beqz	$ta, .L2		! if no, skip

	!------------------------------------------------------
	!    __q1--, __r1 += (d);
	!    if (__r1 >= (d) && __r1 < __m) {
	!------------------------------------------------------
	addi	QHI, QHI, -1		! __q1--
	add	RHI, RHI, D		! __r1 += d
	slt	$ta, RHI, D		! __r1 < d
	bnez	$ta, .L2		! if yes, skip
	slt	$ta, RHI, M		! __r1 < __m
	beqz	$ta, .L2		! if no, skip

	!------------------------------------------------------
	!       __q1--, __r1 += (d);
	!    }
	!  }
	!------------------------------------------------------
	addi	QHI, QHI, -1		! __q1--
	add	RHI, RHI, D		! __r1 += d

.L2:
	!------------------------------------------------------
	!  __r1 -= __m;
	!  __r0 = __r1 % __d1; __q0 = __r1 / __d1;
	!  __m = (USItype) __q0 * __d0;
	!  __r0 = __r0 * ((USItype) 1 << (W_TYPE_SIZE / 2)) \
	!        | ((USItype) (n0) & (((USItype) 1 << (W_TYPE_SIZE / 2)) - 1));
	!  if (__r0 < __m) {
	!------------------------------------------------------
	sub	RHI, RHI, M		! __r1 -= __m
	divr	Q, RC, RHI, DHI		! __q0 = r1 / __d1, __r0 = r1 % __d1
	slli	RC, RC, 16		! __r0 << 16
	or	R, RC, RB		! __r0 = (__r0 << 16) | ll_lowpart(n0)
#undef M
#define M	DLO
	mul	M, DLO, Q		! __m = __q0 * __d0
	slt	$ta, R, M		! __r0 < __m
	beqz	$ta, .L5		! if no, skip

	!------------------------------------------------------
	!    __q0--, __r0 += (d);
	!    if (__r0 >= (d) && __r0 < __m) {
	!------------------------------------------------------
	add	R, R, D			! __r0 += d
	addi	Q, Q, -1		! __q0--
	slt	$ta, R, D		! __r0 < d
	bnez	$ta, .L5		! if yes, skip
	slt	$ta, R, M		! __r0 < __m
	beqz	$ta, .L5		! if no, skip

	!------------------------------------------------------
	!      __q0--, __r0 += (d);
	!    }
	!  }
	!------------------------------------------------------
	add	R, R, D			! __r0 += d
	addi	Q, Q, -1		! __q0--

.L5:
	!------------------------------------------------------
	!  __r0 -= __m;
	!  *q = (USItype) __q1 * ((USItype) 1 << (W_TYPE_SIZE / 2)) | __q0;
	!  *r = __r0;
	!}
	!------------------------------------------------------
	sub	R, R, M			! r = r0 = __r0 - __m
	slli	QHI, QHI, 16		! __q1 << 16
	or	Q, Q, QHI		! q = (__q1 << 16) | __q0
	ret
	.size	fudiv_qrnnd, .-fudiv_qrnnd



#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
#define NREGS	$r6
#define DREGS	$r8
#else
#define NREGS	$r16
#define DREGS	$r18
#endif
#ifdef __big_endian__
	#define RK		$r2
	#define RJ		$r3
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	#define NUMHI		$r6
	#define NUMLO		$r7
	#define DENHI		$r8
	#define DENLO		$r9
#else
	#define NUMHI		$r16
	#define NUMLO		$r17
	#define DENHI		$r18
	#define DENLO		$r19
	#define RDH		$r22
	#define RDL		$r23
#endif
	#define OFFSET_L	4
	#define OFFSET_H	0
#else
	#define RK		$r3
	#define RJ		$r2
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	#define NUMHI		$r7
	#define NUMLO		$r6
	#define DENHI		$r9
	#define DENLO		$r8
#else
	#define NUMHI		$r17
	#define NUMLO		$r16
	#define DENHI		$r19
	#define DENLO		$r18
	#define RDH		$r23
	#define RDL		$r22
#endif
	#define OFFSET_L	0
	#define OFFSET_H	4
#endif
#define MHI	RI	// m1
#define MLO	RH	// m0
#if defined(__NDS32_EXT_PERF__)||!defined(__NDS32_REDUCE_REGS__)
#define BM	$r21	// bm
#endif
#undef RC
#define RC	$r3

	.align	2
	.globl	__udivmoddi4
	.type	__udivmoddi4, @function
	! =====================================================================
	! uint64_t __udivmoddi4(uint64_t n, uint64_t d, uint64_t *r)
	!
	! This function divides 64-bit numerator n by 64-bit denominator d. The
	! quotient is returned as 64-bit return value and the 64-bit remainder
	! is stored at the input address r.
	! stack allocation:
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	! sp+40 +------------------+
	!       | q                |
	! sp+32 +------------------+
	!       | bm               |
	! sp+28 +------------------+
	!       | $lp              |
	! sp+24 +------------------+
	!       | $fp              |
	! sp+20 +------------------+
	!       | $r10             |
	! sp+16 +------------------+
	!       | $r6 - $r9        |
	! sp    +------------------+
#else
	! sp+8  +------------------+
	!       | $lp              |
	! sp+4  +------------------+
	!       | $fp              |
	! sp    +------------------+
#endif
	! =====================================================================
	!------------------------------------------------------
	!UDWtype __udivmoddi4 (UDWtype n, UDWtype d, UDWtype *rp)
	!{
	!  const DWunion nn = {.ll = n};
	!  const DWunion dd = {.ll = d};
	!  DWunion rr;
	!  UWtype d0, d1, n0, n1, n2;
	!  UWtype q0, q1;
	!  UWtype b, bm;
	!------------------------------------------------------
	!  in regs: ($r0,$r1) - NUMERATOR, ($r2,$r3) - DENOMINATOR,
	!           $r4 - pointer to REMAINDER
	! out regs: ($r0,$r1) - QUOTIENT
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	!  scratch: $r2-$r9 , $ta, $fp, $lp
#else
	!  scratch: $r2-$r5, $ta, $r16-$r21 $fp, $lp
#endif
	!------------------------------------------------------
__udivmoddi4:
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	addi	$sp, $sp, -40
	smw.bi	$r6, [$sp], $r10 , 10
#else
	smw.adm	$sp, [$sp], $sp, 10
#endif

	!------------------------------------------------------
	!  d0 = dd.s.low;
	!  d1 = dd.s.high;
	!  n0 = nn.s.low;
	!  n1 = nn.s.high;
	!  if (d1 == 0) {
	!------------------------------------------------------
	movd44	NREGS, $r0		! (n1,n0)
	movd44	DREGS, $r2		! (d1,d0)
	move	$fp, $r4		! rp
	bnez	RK, .L9		! if d1 != 0, skip

	!------------------------------------------------------
	!      if (d0 > n1) {
	!          /* 0q = nn / 0D */
	!------------------------------------------------------
	slt	$ta, NUMHI, DENLO	! n1 < d0
	beqz	$ta, .L10		! if no, skip

	!------------------------------------------------------
	!          count_leading_zeros (bm, d0);
	!          if (bm != 0) {
	!              /* Normalize, i.e. make the most significant bit of the
	!                 denominator set.  */
	!------------------------------------------------------
#ifdef __NDS32_EXT_PERF__
	clz	$r0, DENLO
#else
	move	$r0, DENLO
	bal	__clzsi2
#endif
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	$r0,  [$sp+(28)]	! bm
#else
	move	BM, $r0			! bm
#endif
	beqz	$r0, .LZskipnorm1	! if bm == 0, skip

	!------------------------------------------------------
	!              d0 = d0 << bm;
	!              n1 = (n1 << bm) | (n0 >> (W_TYPE_SIZE - bm));
	!              n0 = n0 << bm;
	!          }
	!------------------------------------------------------
	sll	DENLO, DENLO, $r0	! d0 <<= bm
	subri	RB, $r0, 32		! 32 - bm
	srl	RB, NUMLO, RB		! n0 >> (32 - bm)
	sll	NUMHI, NUMHI, $r0	! n1 << bm
	or	NUMHI, NUMHI, RB	! n1 =  (n1 << bm) | (n0 >> (32 - bm))
	sll	NUMLO, NUMLO, $r0	! n0 <<= bm

.LZskipnorm1:
	!------------------------------------------------------
	!          fudiv_qrnnd (&q0, &n0, n1, n0, d0);
	!          q1 = 0;
	!          /* Remainder in n0 >> bm.  */
	!      }
	!------------------------------------------------------
	movd44	$r0, NREGS		! (n1,n0)
	move	$r2, DENLO		! d0
	bal	fudiv_qrnnd		! calcaulte q0 n0
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	RI, [$sp+(32+OFFSET_L)]! q0
#else
	move	RDL, RI		! q0
#endif
	move	NUMLO, RH		! n0
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	move	RB, 0
	swi	RB, [$sp+(32+OFFSET_H)]	! q1 = 0
#else
	move	RDH, 0			! q1 = 0
#endif
	b	.L19

.L10:
	!------------------------------------------------------
	!      else {
	!          if (d0 == 0)
	!            d0 = 1 / d0; /* Divide intentionally by zero.  */
	!------------------------------------------------------
	beqz	RJ, .LZdivzero		! if d0 != 0, skip

	!------------------------------------------------------
	!          count_leading_zeros (bm, d0);
	!          if (bm == 0) {
	!              /* From (n1 >= d0), (the most significant bit of d0 is set),
	!                 conclude (the most significant bit of n1 is set) and (the
	!                 leading quotient digit q1 = 1).
	!                 This special case is necessary, not an optimization.
	!                 (Shifts counts of W_TYPE_SIZE are undefined.)  */
	!------------------------------------------------------
#ifdef __NDS32_EXT_PERF__
	clz	$r0, DENLO
#else
	move	$r0, DENLO
	bal	__clzsi2
#endif
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	$r0, [$sp+(28)]		! bm
#else
	move	BM, $r0			! bm
#endif
	bnez	$r0, .LZnorm1		! if bm != 0, skip

	!------------------------------------------------------
	!              n1 -= d0;
	!              q1 = 1;
	!          }
	!------------------------------------------------------
	sub	NUMHI, NUMHI, DENLO	! n1 -= d0
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	movi	RB, 1
	swi	RB, [$sp+(32+OFFSET_H)]	! q1 = 1
#else
	movi	RDH, 1			! q1 = 1
#endif
	b	.L29

	! to eliminate unaligned branch target
	.align	2
.LZnorm1:
	!------------------------------------------------------
	!          else {
	!              /* Normalize.  */
	!              b = W_TYPE_SIZE - bm;
	!              d0 = d0 << bm;
	!              n2 = n1 >> b;
	!              n1 = (n1 << bm) | (n0 >> b);
	!              n0 = n0 << bm;
	!              fudiv_qrnnd (&q1, &n1, n2, n1, d0);
	!          }
	!------------------------------------------------------
	subri	$ta, $r0, 32		! b = 32 - bm
	sll	DENLO, DENLO, $r0	! d0 <<= bm
	move	$r2, DENLO
	srl	RA, NUMLO, $ta		! n0 >> b
	sll	RB, NUMHI, $r0		! n1 << bm
	sll	NUMLO, NUMLO, $r0	! n0 <<= bm
	or	RH, RB, RA		! n1 = (n1 << bm) | (n0 >> b)
	srl	RI, NUMHI, $ta		! n2 = n1 >> b
	bal	fudiv_qrnnd		! caculate q1, n1
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	RI, [$sp+(32+OFFSET_H)]! q1
#else
	move	RDH, RI		! q1
#endif
	move	NUMHI, RH		! n1

.L29:
	!------------------------------------------------------
	!          /* n1 != d0...  */
	!          fudiv_qrnnd (&q0, &n0, n1, n0, d0);
	!          /* Remainder in n0 >> bm.  */
	!      }
	!------------------------------------------------------
	movd44	$r0, NREGS		! (n1,n0)
	move	$r2, DENLO		! d0
	bal	fudiv_qrnnd		! calcuate q0, n0
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	RI, [$sp+(32+OFFSET_L)]
#else
	move	RDL, RI
#endif
	move	NUMLO, RH

	! to eliminate unaligned branch target
	.align	2
.L19:
	!------------------------------------------------------
	!      if (rp != 0) {
	!------------------------------------------------------
	beqz	$fp, .LZsetq		! if rp == 0, skip

	!------------------------------------------------------
	!          rr.s.low = n0 >> bm; rr.s.high = 0;
	!          *rp = rr.ll;
	!      }
	!  }
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	lwi	RC, [$sp+(28)]		! bm
#endif
	movi	NUMHI, 0
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	srl	NUMLO, NUMLO, RC	! n0 >> bm
#else
	srl	NUMLO, NUMLO, BM	! n0 >> bm
#endif
	b	.LZsetr

	! to eliminate unaligned branch target
	.align	2
.LZdivzero:
	! divide-by-zero exception or quotient = 0 and remainder = 0 returned
	divr	NUMHI, NUMLO, DENLO, DENLO

.LZqzero:
	movi	RI, 0
	movi	RH, 0
	beqz	$fp, .LZret		! if rp == NULL, skip

	swi	NUMLO, [$fp+OFFSET_L]	! *rp
	swi	NUMHI, [$fp+OFFSET_H]
	b	.LZret

.L9:
	!------------------------------------------------------
	!  else { 
	!      if (d1 > n1) {
	!          /* 00 = nn / DD */
	!          q0 = 0; q1 = 0;
	!          /* Remainder in n1n0.  */
	!          if (rp != 0) {
	!              rr.s.low = n0; rr.s.high = n1;
	!              *rp = rr.ll;
	!          }
	!      }
	!------------------------------------------------------
	slt	$ta, NUMHI, DENHI	! n1 < d1
	bnez	$ta, .LZqzero		! if yes, skip

	!------------------------------------------------------
	!      else {
	!          /* 0q = NN / dd */
	!          count_leading_zeros (bm, d1);
	!          if (bm != 0) {
	!              /* Normalize.  */
	!              UWtype m1, m0;
	!------------------------------------------------------
#ifdef __NDS32_EXT_PERF__
	clz	$r0, DENHI
#else
	move	$r0, DENHI
	bal	__clzsi2
#endif
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	$r0, [$sp+(28)]		! bm
#else
	move	BM, $r0			! bm
#endif
	beqz	$r0, .LZskipnorm2	! if bm == 0, skip

	!------------------------------------------------------
	!              b = W_TYPE_SIZE - bm;
	!              d1 = (d0 >> b) | (d1 << bm);
	!              d0 = d0 << bm;
	!              n2 = n1 >> b;
	!              n1 = (n0 >> b) | (n1 << bm);
	!              n0 = n0 << bm;
	!              fudiv_qrnnd (&q0, &n1, n2, n1, d1);
	!------------------------------------------------------
	subri	RA, $r0, 32		! b = 32 - bm
	srl	RB, DENLO, RA		! d0 >> b
	sll	$r2, DENHI, $r0		! d1 << bm
	or	$r2, $r2, RB		! d1 = (d0 >> b) | (d1 << bm)
	move	DENHI, $r2
	sll	DENLO, DENLO, $r0	! d0 <<= bm
	srl	RC, NUMLO, RA		! n0 >> b
	sll	NUMLO, NUMLO, $r0	! n0 <<= bm
	sll	RH, NUMHI, $r0		! n1 << bm
	srl	RI, NUMHI, RA		! n2 = n1 >> b
	or	RH, RH, RC		! n1 = (n0 >> b) | (n1 << bm)
	bal	fudiv_qrnnd		! calculate  q0, n1
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	RI, [$sp+(32+OFFSET_L)]
#else
	move	RDL, RI
#endif
	move	NUMHI, RH

	!----------------------------------------------------
	!              umul_ppmm (m1, m0, q0, d0);
	!!!!!!         do {
	!!!!!!             USItype __x0, __x1, __x2, __x3;
	!!!!!!             USItype __ul, __vl, __uh, __vh;
	!!!!!!             __ul = ((USItype) (q0) & (((USItype) 1 << (W_TYPE_SIZE / 2)) - 1));
	!!!!!!             __uh = ((USItype) (q0) >> (W_TYPE_SIZE / 2));
	!!!!!!             __vl = ((USItype) (d0) & (((USItype) 1 << (W_TYPE_SIZE / 2)) - 1));
	!!!!!!             __vh = ((USItype) (d0) >> (W_TYPE_SIZE / 2));
	!!!!!!             __x0 = (USItype) __ul * __vl;
	!!!!!!             __x1 = (USItype) __ul * __vh;
	!!!!!!             __x2 = (USItype) __uh * __vl;
	!!!!!!             __x3 = (USItype) __uh * __vh;
	!!!!!!             __x1 += ((USItype) (__x0) >> (W_TYPE_SIZE / 2));
	!!!!!!             __x1 += __x2;
	!!!!!!             if (__x1 < __x2)
	!!!!!!               __x3 += ((USItype) 1 << (W_TYPE_SIZE / 2));
	!!!!!!             (m1) = __x3 + ((USItype) (__x1) >> (W_TYPE_SIZE / 2));
	!!!!!!             (m0) = (USItype)(q0*d0);
	!!!!!!         }
	!              if (m1 > n1 || (m1 == n1 && m0 > n0)) {
	!---------------------------------------------------
#ifdef __NDS32_ISA_V3M__
	move	RH, DENLO		! d0
	bal	umul_ppmm
#else
	mulr64	$r0, RI, DENLO
#endif
	slt	$ta, NUMHI, MHI		! n1 < m1
	bnez	$ta, .L46		! if yes, skip
	bne	MHI, NUMHI, .L45	! if m1 != n1, skip
	slt	$ta, NUMLO, MLO		! n0 < m0
	beqz	$ta, .L45		! if no, skip

.L46:
	!------------------------------------------------------
	!                  q0--;
	!                  sub_ddmmss (m1, m0, m1, m0, d1, d0);
	!!!!!!             do {
	!!!!!!                 USItype __x;
	!!!!!!                 __x = (m0) - (d0);
	!!!!!!                 (m1) = (m1) - (d1) - (__x > (m0));
	!!!!!!                 (m0) = __x;
	!!!!!!             }
	!              }
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	lwi	RC, [$sp+(32+OFFSET_L)]
	sub	MHI, MHI, DENHI		! m1 - d1
	addi	RC, RC, -1		! q0--
	swi	RC, [$sp+(32+OFFSET_L)]
#else
	addi	RDL, RDL, -1		! q0--
	sub	MHI, MHI, DENHI		! m1 - d1
#endif
	sub	RC, MLO, DENLO		! __x = m0 - d0
	slt	$ta, MLO, RC		! m0 < __x
	sub	MHI, MHI, $ta		! m1 = m1 - d1 - (__x > m0)
	move	MLO, RC			! m0 = __x

.L45:
	!------------------------------------------------------
	!                q1 = 0;
	!                if (rp != 0) {
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	movi	RC, 0
	swi	RC, [$sp+(32+OFFSET_H)]	! q1 = 0
#else
	movi	RDH, 0			! q1 = 0
#endif
	beqz	$fp, .LZsetq		! if yes, skip

	!------------------------------------------------------
	!                    sub_ddmmss (n1, n0, n1, n0, m1, m0);
	!!!!!!               do {
	!!!!!!                   USItype __x;
	!!!!!!                   __x = (n0) - (m0);
	!!!!!!                   (n1) = (n1) - (m1) - (__x > (n0));
	!!!!!!                   (n0) = __x;
	!!!!!!               }
	!                    rr.s.low = (n1 << b) | (n0 >> bm);
	!                    rr.s.high = n1 >> bm;
	!                    *rp = rr.ll;
	!                }
	!            }
	!------------------------------------------------------
	sub	RH, NUMLO, MLO		! __x = n0 - m0
	sub	RI, NUMHI, MHI		! n1 - m1
	slt	$ta, NUMLO, RH		! n0 < __x
	sub	RI, RI, $ta		! n1 = n1 - m1 - (__x > n0)
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	lwi	RC, [$sp+(28)]		! bm
	subri	RA, RC, 32		! b
	sll	NUMHI, RI, RA		! n1 << b
	srl	NUMLO, RH, RC		! n0 >> bm
	or	NUMLO, NUMLO, NUMHI	! (n1 << b) | (n0 >> bm)
	srl	NUMHI, RI, RC		! n1 >> bm
#else
	subri	RA, BM, 32		! b
	sll	NUMHI, RI, RA		! n1 << b
	srl	NUMLO, RH, BM		! n0 >> bm
	or	NUMLO, NUMLO, NUMHI	! (n1 << b) | (n0 >> bm)
	srl	NUMHI, RI, BM		! n1 >> bm
#endif

.LZsetr:
	swi	NUMLO, [$fp+OFFSET_L]	! remainder
	swi	NUMHI, [$fp+OFFSET_H]

.LZsetq:
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	lwi	RH, [$sp+(32+OFFSET_L)]! quotient
	lwi	RI, [$sp+(32+OFFSET_H)]
#else
	move	RH, RDL		! quotient
	move	RI, RDH
#endif

	! to eliminate unaligned branch target
	.align	2
.LZret:
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	lmw.bi	$r6, [$sp], $r10 , 10
	addi	$sp, $sp, 40
#else
	lmw.bim	$sp, [$sp], $sp, 10
#endif
	ret

.LZskipnorm2:
	!------------------------------------------------------
	!            else {
	!                /* From (n1 >= d1) /\ (the most significant bit of d1 is set),
	!                   conclude (the most significant bit of n1 is set) /\ (the
	!                   quotient digit q0 = 0 or 1).
	!                   This special case is necessary, not an optimization.  */
	!                if (n1 > d1 || n0 >= d0) {
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	move	RC, 0
#endif
	slt	$ta, DENHI, NUMHI	! n1 > d1
	bnez	$ta, .L52		! if yes, skip
	slt	$ta, NUMLO, DENLO	! n0 < d0
	bnez	$ta, .L51		! if yes, skip

.L52:
	!------------------------------------------------------
	!                    q0 = 1;
	!                    sub_ddmmss (n1, n0, n1, n0, d1, d0);
	!!!!!!               do {
	!!!!!!                   USItype __x;
	!!!!!!                   __x = (n0) - (d0);
	!!!!!!                   (n1) = (n1) - (d1) - (__x > (n0));
	!!!!!!                   (n0) = __x;
	!!!!!!               }
	!                }
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	move	RB, 1
	swi	RB, [$sp+(32+OFFSET_L)]	! q0 = 1
#else
	movi	RDL, 1			! q0 = 1
#endif
	sub	RA, NUMLO, DENLO	! __x = n0 - d0
	sub	NUMHI, NUMHI, DENHI	! n1 - d1
	slt	$ta, NUMLO, RA		! n0 < __x
	sub	NUMHI, NUMHI, $ta	! n1 = n1 -d1 - (_-x > n0)
	move	NUMLO, RA		! n0 = __x
	b	.L54

.L51:
	!------------------------------------------------------
	!                else
	!                  q0 = 0;
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	RC, [$sp+(32+OFFSET_L)]	! q0 = 0
#else
	movi	RDL, 0			! q0 = 0
#endif

.L54:
	!------------------------------------------------------
	!                q1 = 0;
	!                if (rp != 0) {
	!                    rr.s.low = n0; rr.s.high = n1;
	!                    *rp = rr.ll;
	!                }
	!            }
	!        }
	!    }
	!  const DWunion ww = {{.low = q0, .high = q1}};
	!  return ww.ll;
	!}
	!------------------------------------------------------
#if defined(__NDS32_REDUCE_REGS__)||!defined(__NDS32_EXT_PERF__)
	swi	RC, [$sp+(32+OFFSET_H)]	! q1 = 0
#else
	movi	RDH, 0
#endif
	bnez	$fp, .LZsetr
	b	.LZsetq
	.size	__udivmoddi4, .-__udivmoddi4
#endif /* L_udivmoddi4 */



#ifdef L_umodsi3

	! =====================================================================
	.text
	.align	2
	.globl	__umodsi3
	.type	__umodsi3, @function
__umodsi3:
	! ---------------------------------------------------------------------
	!!res=udivmodsi4(a,b,1);
	! if (den==0)
	!     return num;
	! ---------------------------------------------------------------------
	beqz	$r1, .L1			! if den==0, skip
	! ---------------------------------------------------------------------
	! bit=1;
	! res=0;
	! ---------------------------------------------------------------------
	movi	$r4, 1				! $r4  <- bit=1
#ifndef __OPTIMIZE_SIZE__
.L6:
#endif
	! ---------------------------------------------------------------------
	! while (den<num
	! ---------------------------------------------------------------------
	slt	$ta, $r1, $r0			! $ta  <- den<num?
	beqz	$ta, .L5			! if no, skip
	! ---------------------------------------------------------------------
	!      &&bit&&!(den&(1L<<31)))
	! ---------------------------------------------------------------------
	bltz	$r1, .L5			! if den<0, skip
	! ---------------------------------------------------------------------
	! {   den<<=1;
	!     bit<<=1;
	! }
	! ---------------------------------------------------------------------
#if defined (__OPTIMIZE_SIZE__) && ! defined (__NDS32_ISA_V3M__)
	clz	$r3, $r1			! $r3  <- leading zero count for den
	clz	$ta, $r0			! $ta  <- leading zero count for num
	sub	$r3, $r3, $ta			! $r3  <- number of bits to shift
	sll	$r1, $r1, $r3			! $r1  <- den
	sll	$r4, $r4, $r3			! $r2  <- bit
#else
	slli	$r1, $r1, 1			! $r1  <- den<<=1
	slli	$r4, $r4, 1			! $r4  <- bit<<=1
	b	.L6				! continue loop
#endif
.L5:
	! ---------------------------------------------------------------------
	! while (bit)
	! {   if (num>=den)
	!     {   num-=den;
	!         res|=bit;
	!     }
	!     bit>>=1;
	!     den>>=1;
	! }
	!!if (modwanted)
	!!    return num;
	!!return res;
	! ---------------------------------------------------------------------
	sub	$r2, $r0, $r1			! $r2  <- num-den
	slt	$ta, $r0, $r1			! $ta  <- num<den?
	srli	$r4, $r4, 1			! $r4  <- bit>>=1
	cmovz	$r0, $r2, $ta			! $r0  <- num=(num<den)?num:num-den
	srli	$r1, $r1, 1			! $r1  <- den>>=1
	bnez	$r4, .L5			! if bit!=0, continue loop
.L1:
	! ---------------------------------------------------------------------
	! return res;
	! ---------------------------------------------------------------------
	ret
	.size	__umodsi3, .-__umodsi3
#endif /* L_umodsi3 */



#ifdef L_umoddi3

#ifdef __big_endian__
#define RI		$r0
#define RH		$r1
#define OFFSET_H	0
#define OFFSET_L	4
#else
#define RI		$r1
#define RH		$r0
#define OFFSET_H	4
#define OFFSET_L	0
#endif

	.text
	.align	2
	.globl	__umoddi3
	.type	__umoddi3, @function
__umoddi3:
	! =====================================================================
	! stack allocation:
	! sp+12 +-----------------------+
	!       | remainder             |
	! sp+4  +-----------------------+
	!       | $lp                   |
	! sp    +-----------------------+
	! =====================================================================
	addi	$sp, $sp, -12
	swi $lp, [$sp+(0)]

	addi	$r4, $sp, 4
	bal	__udivmoddi4
	lwi	RH, [$sp+(4+OFFSET_L)]
	lwi	RI, [$sp+(4+OFFSET_H)]

	! epilogue
	lwi.bi $lp, [$sp], 12
	ret
	.size	__umoddi3, .-__umoddi3
#endif /* L_umoddi3 */



#ifdef L_muldi3

#ifdef __big_endian__
#define RI	$r0
#define RH	$r1
#define RK	$r2
#define RJ	$r3
#else
#define RI	$r1
#define RH	$r0
#define RK	$r3
#define RJ	$r2
#endif
	.text
	.align	2
	.globl	__muldi3
	.type	__muldi3, @function
__muldi3:
#ifdef __NDS32_ISA_V3M__
	! There is no mulr64 instruction in Andes ISA V3M.
	! So we must provide a sequence of calculations to complete the job.
	mul	$r5, RI, RJ	! (ah=a>>31)*(bl=b&0xffffffff)
	srli	RI, RH, 16	! alh=al>>16
	maddr32 $r5, RH, RK	! ah*bl+(bh=b>>31)*(al=a&0xffffffff)
	zeh	RH, RH	! all=al&0xffff
	srli	RK, RJ, 16	! blh=bl>>16
	zeh	RJ, RJ	! bll=bl&0xffff

	mul     $ta, RH, RK	! zA=all*blh
	mul	$r4, RH, RJ	! zl=all*bll
	mul	RJ, RI, RJ	! zB=alh*bll
	add     RH, $ta, RJ	! zA+=zB
	slt	$ta, RH, RJ	! zA<zB
	slli	$ta, $ta, 16	! (zA<zB)<<16
	slli	RJ, RH, 16	! zA<<16
	maddr32 $ta, RI, RK	! zh=alh*blh+((zA<zB)<<16)
	srli	RI, RH, 16	! zA>>16
	add	RI, RI, $ta	! zh+=(zA>>16)
	add     RH, $r4, RJ	! zl+=(zA<<16)
	slt	$ta, RH, $r4	! zl<zA
	add	RI, RI, $ta	! zh+=(zl<zA)
	add	RI, RI, $r5	! zh+=ah*bl+bh*al
	ret
#else /* not  __NDS32_ISA_V3M__ */
	mul	RK, RK, RH
	maddr32	RK, RI, RJ
	mulr64	$r0, RH, RJ
	add	RI, RI, RK
	ret
#endif /* not __NDS32_ISA_V3M__ */
	.size	__muldi3, .-__muldi3
#endif /* L_muldi3 */



#ifdef L_addsub_df

#ifdef __big_endian__
	#define RI	$r0
	#define RH	$r1
	#define RK	$r2
	#define RJ	$r3
#else
	#define RH	$r0
	#define RI	$r1
	#define RJ	$r2
	#define RK	$r3
#endif
#define VALAH	$r4
#define EXPOA	$r7
#define MANAH	$r9
#define MANAL	RH
#define VALBH	$r6
#define EXPOB	$r10
#define MANBH	$r8
#define MANBL	RJ
#define SIGN	$lp
#define RB	$r5
#define RA	$r4
#define RC	$r6
#define AXORB	RK		// sign of a xor b

	.text
	.align	2
	.global	__subdf3
	.type	__subdf3, @function
__subdf3:
#ifdef __NDS32_EXT_PERF__
	btgl	RK, RK, 31
#else
	move	$r4, #0x80000000
	xor	RK, RK, $r4		! A-B is now A+(-B)
#endif

	.global	__adddf3
	.type	__adddf3, @function
__adddf3:
	slli	VALAH, RI, 1		! hi-part(A)<<1
	smw.adm	$r6, [$sp], $r10, 2

	slli	VALBH, RK, 1		! hi-part(B)<<1
	move	SIGN, #0x80000000
	slt	$r15, VALAH, VALBH
	bnez	$r15, .LEswap
	bne	VALAH, VALBH, .LEmain
	slt	$r15, RH, RJ
	beqz	$r15, .LEmain

	! |A|<|B|, do swap
.LEswap:
	movd44	$r8, $r0
	movd44	$r0, $r2
	movd44	$r2, $r8
	slli	VALAH, RI, 1		! hi-part(A)<<1
	slli	VALBH, RK, 1		! hi-part(B)<<1

	! ---------------------------------------------------------------------
	! |A|>=|B|
	! ---------------------------------------------------------------------
.LEmain:
	xor	RK, RK, RI
	and	AXORB, RK, SIGN	! sign of (A xor B)
	srli	EXPOA, VALAH, #21	! exponent(A)
	srli	EXPOB, VALBH, #21	! exponent(B)
	slli	MANAH, VALAH, #10	! (dirty) hi-part of mantissa(A)
	slli	MANBH, VALBH, #10	! (dirty) hi-part of mantissa(B)
	move	RB, #0x7ff
	beq	RB, EXPOA, .LEinfnan	! if A is NaN or inf, goto .LEinfnan
	! A is finite, thus B must be finite
	or	$r15, VALAH, RH
	beqz	$r15, .LEzeroP		! if A is zero, return zero
	or	$r15, VALBH, RJ
	beqz	$r15, .LEretA		! if B is zero, return A
	sub	RC, EXPOA, EXPOB	! exponent(A)-exponent(B)
	slti	$r15, RC, #0x40
	beqz	$r15, .LEretA		! B is insignificant, return A
	srli	RB, RH, #21		! (dirty) mantissa(A)<<11
	or	MANAH, MANAH, RB
	slli	MANAL, RH, #11
	srli	RB, RJ, #21		! (dirty) mantissa(B)<<11
	or	MANBH, MANBH, RB
	slli	MANBL, RJ, #11
	slti	$r15, EXPOA, #0x2
	bnez	$r15, .LEmain4		! if exponent(A) is 0 or 1, got .LEmain4
	or	MANAH, MANAH, SIGN	! mantissa(A)<<11
!	or	RB, MANBH, SIGN
!	cmovn	MANBH, RB, EXPOB
	beqz	EXPOB, .LEmain1
	or	MANBH, MANBH, SIGN	! mantissa(A)<<11

.LEmain1:
	addi	RB, RC, #-1		! adjusted shift amount
	cmovz	RC, RB, EXPOB
	beqz	RC, .LEmain4		! shift amount is zero, simply skip
	! mantissa(b)>>shift amount
	subri	RB, RC, #0x20		! 32-exponent(sum)
	blez	RB, .LEmain2		! if exponent(sum)>=32, goto .LEmain2

	! exponent(sum)<32
	sll	RA, MANBL, RB		! shift-out portion
	sll	RB, MANBH, RB
	srl	MANBH, MANBH, RC
	srl	MANBL, MANBL, RC
	or	MANBL, MANBL, RB
	b	.LEmain3

.LEmain2:
	! exponent(sum)>=32
	subri	RC, RB, #0
	addi	RB, RB, #0x20
	sll	RA, MANBH, RB
	or	RA, RA, MANBL		! shift-out portion
	cmovz	RA, RC, RC
	srl	MANBL, MANBH, RC
	move	MANBH, #0

.LEmain3:
!	ori	$r15, MANBL, #2
!	cmovn	MANBL, RA, $r15
	beqz	RA, .LEmain4
	ori	MANBL, MANBL, #2	! B is quite small compare to A

.LEmain4:
	beqz	AXORB, .LEadd		! same sign, do addition

	! ---------------------------------------------------------------------
	! differnet sign, do subtraction
	! ---------------------------------------------------------------------
	bne	EXPOA, EXPOB, .LEsub1
	bne	MANAH, MANBH, .LEsub1
	beq	MANAL, MANBL, .LEzero	! |A|==|B|, return zero

	! |A|>|B|
.LEsub1:
	! is mantissa(|A|)<mantissa(|B|)?
	slt	$r15, MANAH, MANBH
	bnez	$r15, .LEsub2		! mantissa(|A|)<mantissa(|B|), continue
	bne	MANAH, MANBH, .LEsub3	! mantissa(|A|)!=mantissa(|B|), skip
	slt	$r15, MANAL, MANBL
	beqz	$r15, .LEsub3		! mantissa(|A|)>=mantissa(|B|), skip

.LEsub2:
	! mantissa(|A|)<mantissa(|B|), adjust
	addi	EXPOA, EXPOA, #-1
	! mantissa(|B|)>>1
	slli	RA, MANBH, #31
	srli	MANBH, MANBH, #1
	srli	MANBL, MANBL, #1
	or	MANBL, MANBL, RA

.LEsub3:
	! calculate mantissa(|A|)-mantissa(|B|)
	move	RA, MANAL
	sub	MANAL, MANAL, MANBL
	slt	$r15, RA, MANAL
	sub	MANAH, MANAH, $r15	! no undeflow issue
	sub	MANAH, MANAH, MANBH
	slti	$r15, EXPOA, #2
	bnez	$r15, .LEdenorm		! when exponent(A,B) is (0,0) or (1,0/1)
	! count leading zero of mantissa(|A|)
	bnez	MANAH, .LEsub4
#ifdef __NDS32_EXT_PERF__
	move	RA, #0x20
	slt	RA, EXPOA, RA
	move	RB, EXPOA
	bnez	RA, .LEsub5
	move	MANAH, MANAL
	move	MANAL, #0
	addi	EXPOA, EXPOA, #-32

.LEsub4:
	clz	RB, MANAH		! leading zero count
	slt	$r15, RB, EXPOA		! leading zero count>=exponent(A)?
	subri	RA, $r15, #1
	min	RB, RB, EXPOA		! calculated shift amount
	beqz	RB, .LEround		! shift amount is 0, skip
.LEsub5:
	sub	EXPOA, EXPOA, RB
	! mantissa(diff)<<adjusted shift amount
	sub	RB, RB, RA		! adjusted shift amount
	subri	RC, RB, #0x20
	srl	RA, MANAL, RC
	sll	MANAH, MANAH, RB
	sll	MANAL, MANAL, RB
	or	MANAH, MANAH, RA
#else
	slti	$r15, EXPOA, #0x20
	bnez	$r15, .LEsub4
	move	MANAH, MANAL
	move	MANAL, #0
	addi	EXPOA, EXPOA, #-32
	bnez	EXPOA, .LEsub4
	b	.LEround

.LEloop:
	addi	EXPOA, EXPOA, #-1
	beqz	EXPOA, .LEround
	srli	RA, MANAL, #31
	slli	MANAH, MANAH, #1
	slli	MANAL, MANAL, #1
	or	MANAH, MANAH, RA

.LEsub4:
	slt	$r15, MANAH, SIGN
	bnez	$r15, .LEloop
#endif

	! ---------------------------------------------------------------------
	! do rounding
	! ---------------------------------------------------------------------
.LEround:
	addi	MANAL, MANAL, #0x400
	slti	$r15, MANAL, #0x400
	add	MANAH, MANAH, $r15
	slt	$r15, MANAH, $r15
	add	EXPOA, EXPOA, $r15
	srli	RB, MANAL, #11
	andi	RB, RB, #1
	move	RC, MANAL
	sub	MANAL, MANAL, RB
	slt	$r15, RC, MANAL
	sub	MANAH, MANAH, $r15

	! ---------------------------------------------------------------------
	! pack result
	! ---------------------------------------------------------------------
	slli	MANAH, MANAH, #1	! shift our implied 1
	slli	RB, MANAH, #20
	srli	MANAH, MANAH, #12
	srli	MANAL, MANAL, #11
	or	MANAL, MANAL, RB
	slli	RB, EXPOA, #20
	or	MANAH, MANAH, RB

.LEpack:
	and	RI, RI, SIGN
	or	RI, RI, MANAH

.LEretA:
.LEret:
	lmw.bim	$r6, [$sp], $r10, 2
	ret5	$lp

.LEadd:
	! ---------------------------------------------------------------------
	! same sign, do addition
	! ---------------------------------------------------------------------
	add	MANAL, MANAL, MANBL
	slt	$r15, MANAL, MANBL
	add	MANAH, MANAH, $r15
	slt	r15, MANAH, $r15
	add	MANAH, MANAH, MANBH
	bnez	$r15, .LEaddover	! overflow, goto .LEaddover
	slt	$r15, MANAH, MANBH
	bnez	$r15, .LEaddover	! overflow, goto .LEaddover
	! all works fine without overflow
	bnez	EXPOA, .LEround

.LEdenorm:
	! mantissa(sum)>>11
	srli	MANAL, MANAL, #11
	slli	RA, MANAH, #21
	srli	MANAH, MANAH, #11
	or	MANAL, MANAL, RA
	b	.LEpack

	! handle overflow
.LEaddover:
	subri	RB, EXPOA, #0x7fe
	beqz	RB, .LEinf
	andi	$r15, MANAL, #1
	ori	RB, MANAL, #2
	cmovn	MANAL, RB, $r15
	! mantissa(sum)>>1
	slli	RA, MANAH, #31
	srli	MANAH, MANAH, #1
	srli	MANAL, MANAL, #1
	or	MANAL, MANAL, RA
	addi	EXPOA, EXPOA, #1
	b	.LEround

.LEinf:
	move	RH, #0
	move	MANAH, 0x7ff00000	! return inf
	b	.LEpack

	! handle 0.0f or -0.0f
.LEzeroP:
	beqz	AXORB, .LEretA		! A and B same sign, return A

.LEzero:
	move	RH, #0
	move	RI, #0			! otherwise, return 0.0f
	b	.LEret

	! ---------------------------------------------------------------------
	! exponent(A) is 0x7ff
	! ---------------------------------------------------------------------
.LEinfnan:
	or	MANAH, MANAH, MANAL
	bne	MANAH, SIGN, .LEnan	! if A is NaN, goto .LEnan
	! A is inf
	bne	RB, EXPOB, .LEretA	! B is finite, return A
	! B is also inf
	beqz	AXORB, .LEretA		! same sign, return A

.LEnan:
	move	RH, #0
	move	RI, #0xfff80000	! return NaN
	b	.LEret
	.size   __subdf3, .-__subdf3
	.size   __adddf3, .-__adddf3
#endif /* L_addsub_df */


#ifdef L_mul_df

#ifdef __big_endian__
	#define RI	$r0
	#define RH	$r1
	#define RK	$r2
	#define RJ	$r3
	#define MANAH	$r6
	#define MANAL	$r7
	#define MANBH	$r8
	#define MANBL	$r9
#else
	#define RH	$r0
	#define RI	$r1
	#define RJ	$r2
	#define RK	$r3
	#define MANAL	$r6
	#define MANAH	$r7
	#define MANBL	$r8
	#define MANBH	$r9
#endif
#define EXPOA	$r4
#define EXPOB	$r5
#define SIGN	$fp
#define RB	$r5
#define RC	RJ
#define RD	RK
#define RE	RH
#define RF	RI
#define RG	$r10
#define AXORB	$r10

	.text
	.align	2
	.global	__muldf3
	.type	__muldf3, @function
__muldf3:
	push25	$r10, #16

	slli	EXPOA, RI, #1
	srli	EXPOA, EXPOA, #21	! exponent(a)
	slli	MANAH, RI, #11		! (dirty) mantissa(a)
	srli	MANAL, RH, #21
	or	MANAH, MANAH, MANAL
	slli	MANAL, RH, #11
	move	SIGN, #0x80000000
	slli	EXPOB, RK, #1
	srli	EXPOB, EXPOB, #21	! exponent(b)
	slli	MANBH, RK, #11		! (dirty) mantissa(b)
	srli	MANBL, RJ, #21
	or	MANBH, MANBH, MANBL
	slli	MANBL, RJ, #11
	xor	RD, RK, RI
	and	AXORB, RD, SIGN		! sign of (A xor B)

	move	RD, 0x7ff
	beqz	EXPOA, .LFAexpzero	! exponent(A) is 0x000
	beq	RD, EXPOA, .LFAinfnan	! exponent(A) is 0x7ff
	or	MANAH, MANAH, SIGN

.LFmain1:
	beqz	EXPOB, .LFBexpzero	! exponent(B) is 0x000
	beq	RD, EXPOB, .LFBinfnan	! exponent(B) is 0x7ff
	or	MANBH, MANBH, SIGN

	! ---------------------------------------------------------------------
	! multiply two 64-bit unsigned integers for 128-bit product.
	! ---------------------------------------------------------------------
.LFmain2:
	! exponent(product) = exponent(A) + exponent(B) - 0x3fe
	swi	AXORB, [$sp+(12)]
	addi	RE, EXPOB, #0xfffffc02
	add	EXPOA, EXPOA, RE
	! PHH: hi-part of mantissa(A) * hi-part of mantissa(B)
	! This is a 64-bit multiplication: (RK, RJ) is (high, low).
#ifndef __NDS32_ISA_V3M__
	mulr64	$r2, MANAH, MANBH
#else
	swi	EXPOA, [$sp+(8)]
	move	$r0, MANAH
	move	$r1, MANBH
	bal	umul_ppmm
	movd44	$r2, $r0
#endif
	! PLH: lo-part of mantissa(A) * hi-part of mantissa(B)
	! This is a 64-bit multiplication: (RI, RH) is (high, low).
#ifndef __NDS32_ISA_V3M__
	mulr64	$r0, MANAL, MANBH
#else
	smw.bi	$r2, [$sp], $r3, #0
	move	$r0, MANAL
	move	$r1, MANBH
	bal	umul_ppmm
	lmw.bi	$r2, [$sp], $r3, #0
#endif
	! add PLH and PHH for 96-bit result in (RG, MANBH, RH).
	add	MANBH, RJ, RI
	slt	$r15, MANBH, RI
	add	RG, RK, $r15
	! PHL: hi-part of mantissa(A) * lo-part of mantissa(B)
	! This is a 64-bit multiplication: (RK, RJ) is (high, low).
#ifndef __NDS32_ISA_V3M__
	mulr64	$r2, MANAH, MANBL
#else
	swi	RH, [$sp+(4)]
	move	$r0, MANAH
	move	$r1, MANBL
	bal	umul_ppmm
	movd44	$r2, $r0
	lwi	RH, [$sp+(4)]
#endif
	! add PHL, PLH, and PHH for 96-bit result in (RG, MANBH, MANAH).
	add	MANAH, RJ, RH
	slt	$r15, MANAH, RH
	add	MANBH, MANBH, $r15
	slt	$r15, MANBH, $r15
	add	RG, RG, $r15
	add	MANBH, MANBH, RK
	slt	$r15, MANBH, RK
	add	RG, RG, $r15
	! PLL: lo-part of mantissa(A) * lo-part of mantissa(B)
	! This is a 64-bit multiplication: (RI, RH) is (high, low).
#ifndef __NDS32_ISA_V3M__
	mulr64	$r0, MANAL, MANBL
#else
	move	$r0, MANAL
	move	$r1, MANBL
	bal	umul_ppmm
	lwi	EXPOA, [$sp+(8)]
#endif
	! add PLL, PHL, PLH, and PHH for 128-bit result in (RI, MANBH, MANAH, RH).
	add	MANAH, MANAH, RI
	slt	$r15, MANAH, RI
	add	MANBH, MANBH, $r15
	slt	$r15, MANBH, $r15
	add	RI, RG, $r15

	! take high 64-bit part of the product into (RI, RH).
	or	MANAH, MANAH, RH
	ori	RH, MANBH, #1		! adjust if low 64-bit is non-zero.
	cmovz	RH, MANBH, MANAH
	sltsi	$r15, RI, #0
	bnez	$r15, .LFmain3
	! MSB is zero, adjust
	move	$r15, RH
	add	RH, RH, RH
	slt	$r15, RH, $r15
	add	RI, RI, RI
	add	RI, RI, $r15
	addi	EXPOA, EXPOA, #-1

.LFmain3:
	lwi	AXORB, [$sp+(12)]
	blez	EXPOA, .LFunderflow	! exponent(product) is too small
	subri	RB, EXPOA, #0x7ff
	blez	RB, .LFinf		! exponent(product) is too big, return inf
	addi	RH, RH, #0x400
	slti	$r15, RH, #0x400
	beqz	$r15, .LFround
	add	RI, RI, $r15
	slt	$r15, RI, $r15
	add	EXPOA, EXPOA, $r15

	! do rounding
.LFround:
	srli	RC, RH, #11
	andi	RC, RC, #1
	sub	RH, RH, RC
	srli	RH, RH, #11
	slli	RC, RI, #21
	or	RH, RH, RC

	! do packing
	slli	RI, RI, #1
	srli	RI, RI, #12
	slli	RB, EXPOA, #20
	or	RI, RI, RB

.LFret:
	or	RI, RI, AXORB
	pop25	$r10, #16

	! ---------------------------------------------------------------------
	! exponent(A) is 0x000
	! ---------------------------------------------------------------------
.LFAexpzero:
	or	$r15, MANAH, MANAL
	beqz	$r15, .LFAzero		! A is zero
	! A is subnormal
	srli	$r15, MANAL, #31
	add	MANAH, MANAH, MANAH
	add	MANAH, MANAH, $r15
	add	MANAL, MANAL, MANAL
	! count leading zeros of A
	bnez	MANAH, .LFAcont
	move	MANAH, MANAL
	move	MANAL, #0
	addi	EXPOA, EXPOA, #-32

	! MANAH is non-zero
.LFAcont:
#ifdef __NDS32_EXT_PERF__
	clz	RE, MANAH
#else
	move	RE, #0
	move	RF, MANAH
	b	.LFAloop2

.LFAloop:
	add	RF, RF, RF
	addi	RE, RE, #1

.LFAloop2:
	slt	$r15, RF, SIGN
	bnez	$r15, .LFAloop
#endif
	beqz	RE, .LFmain1
	sub	EXPOA, EXPOA, RE
	subri	RC, RE, #32
	srl	RC, MANAL, RC
	sll	MANAL, MANAL, RE
	sll	MANAH, MANAH, RE
	or	MANAH, MANAH, RC
	b	.LFmain1

.LFAzero:
	beq	RD, EXPOB, .LFnan	! B is NaN or inf, return NaN

.LFsetsign:
	move	RI, AXORB
	pop25	$r10, #16

	! ---------------------------------------------------------------------
	! exponent(A) is 0x7ff
	! ---------------------------------------------------------------------
.LFAinfnan:
	or	MANAH, MANAH, MANAL
	bne	MANAH, SIGN, .LFnan	! A is NaN, return NaN
	! A is inf: check whether B is zero.
	bnez	EXPOB, .LFAcont2
	slli	RC, MANBH, #1
	or	RC, RC, MANBL
	beqz	RC, .LFnan		! inf*zero is NaN

.LFAcont2:
	bne	RD, EXPOB, .LFinf	! B is finite, return inf

	! ---------------------------------------------------------------------
	! exponent(B) is 0x7ff
	! ---------------------------------------------------------------------
.LFBinfnan:
	or	MANBH, MANBH, MANBL
	bne	MANBH, SIGN, .LFnan	! B is NaN, return NaN
	! B is inf

.LFinf:
	move	RH, #0
	move	RI, #0x7ff00000
	b	.LFret

.LFnan:
	move	RH, #0
	move	RI, #0xfff80000
	pop25	$r10, #16

	! ---------------------------------------------------------------------
	! exponent(B) is 0x000
	! ---------------------------------------------------------------------
.LFBexpzero:
	or	RH, MANBH, MANBL
	beqz	RH, .LFsetsign		! B is zero, return zero
	! B is subnormal
	srli	$r15, MANBL, #31
	add	MANBH, MANBH, MANBH
	add	MANBH, MANBH, $r15
	add	MANBL, MANBL, MANBL
	! count leading zeros of B
	bnez	MANBH, .LFBcont
	move	MANBH, MANBL
	move	MANBL, #0
	addi	EXPOB, EXPOB, #-32

	! MANBH is non-zero
.LFBcont:
#ifdef __NDS32_EXT_PERF__
	clz	RE, MANBH
#else
	move	RE, #0
	move	RF, MANBH
	b	.LFBloop2

.LFBloop:
	add	RF, RF, RF
	addi	RE, RE, #1

.LFBloop2:
	slt	$r15, RF, SIGN
	bnez	$r15, .LFBloop
#endif
	beqz	RE, .LFmain2
	sub	EXPOB, EXPOB, RE
	subri	RC, RE, #32
	srl	RC, MANBL, RC
	sll	MANBL, MANBL, RE
	sll	MANBH, MANBH, RE
	or	MANBH, MANBH, RC
	b	.LFmain2

	! ---------------------------------------------------------------------
	! handle underflow
	! ---------------------------------------------------------------------
.LFunderflow:
	move	MANAL, #0
	subri	RD, EXPOA, #1
	slti	$r15, RD, #0x20
	bnez	$r15, .LFunderflow2
	move	MANAL, RH
	move	RH, RI
	move	RI, #0
	addi	RD, RD, #0xffffffe0
	beqz	RH, .LFunderflow2
	slti	$r15, RD, #0x20
	beqz	$r15, .LFignore		! result too small, return zero

	! 1-exponent(A), in RD, is 0-31
.LFunderflow2:
	beqz	RD, .LFunderflow3	! it is zero, skip
	subri	RC, RD, #0x20
	sll	MANAH, RI, RC
	sll	RB, RH, RC
	srl	RH, RH, RD
	srl	RI, RI, RD
	or	RH, RH, MANAH
	or	MANAL, MANAL, RB
!	ori	RD, RH, #1
!	cmovn	RH, RD, MANAL
	beqz	MANAL, .LFunderflow3
	ori	RH, RH, #1

.LFunderflow3:
	addi	RH, RH, #0x400
	slti	$r15, RH, #0x400
	add	RI, RI, $r15
	srli	EXPOA, RI, #31
	b	.LFround

.LFignore:
	move	RH, #0
	b	.LFsetsign
	.size __muldf3, .-__muldf3
#endif /* L_mul_df */


#ifdef L_div_df

#ifdef __big_endian__
	#define RI	$r0
	#define RH	$r1
	#define RK	$r2
	#define RJ	$r3
	#define MANAH	$r6
	#define MANAL	$r7
	#define MANBH	$r8
	#define MANBL	$r9
#else
	#define RH	$r0
	#define RI	$r1
	#define RJ	$r2
	#define RK	$r3
	#define MANAL	$r6
	#define MANAH	$r7
	#define MANBL	$r8
	#define MANBH	$r9
#endif
#define EXPOA	$r4
#define EXPOB	$r5
#define SIGN	$fp
#define RB	$r5
#define RC	RJ
#define RD	RK
#define RE	RH
#define RF	RI
#define AXORB	$r10

	.text
	.align	2
	.global	__divdf3
	.type	__divdf3, @function
__divdf3:
	push25	$r10, #16

	slli	EXPOA, RI, #1
	srli	EXPOA, EXPOA, #21	! exponent(a)
	slli	MANAH, RI, #11		! (dirty) mantissa(a)
	srli	MANAL, RH, #21
	or	MANAH, MANAH, MANAL
	slli	MANAL, RH, #11
	move	SIGN, #0x80000000
	slli	EXPOB, RK, #1
	srli	EXPOB, EXPOB, #21	! exponent(b)
	slli	MANBH, RK, #11		! (dirty) mantissa(b)
	srli	MANBL, RJ, #21
	or	MANBH, MANBH, MANBL
	slli	MANBL, RJ, #11
	xor	RD, RK, RI
	and	AXORB, RD, SIGN		! sign of (A xor B)

	move	RD, 0x7ff
	beqz	EXPOA, .LGAexpzero	! exponent(A) is 0x000
	beq	RD, EXPOA, .LGAinfnan	! exponent(A) is 0x7ff
	or	MANAH, MANAH, SIGN

.LGmain1:
	beqz	EXPOB, .LGBexpzero	! exponent(B) is 0x000
	beq	RD, EXPOB, .LGBinfnan	! exponent(B) is 0x7ff
	or	MANBH, MANBH, SIGN

	! ---------------------------------------------------------------------
	! divide two 64-bit unsigned integers for 64-bit quotient.
	! ---------------------------------------------------------------------
.LGmain2:
	! exponent(quotient) = exponent(A) - exponent(B) + 0x3ff
	sub	EXPOA, EXPOA, EXPOB
	addi	EXPOA, EXPOA, #0x3ff
	! Use mantissa(A)>>1: hi-part 31 bits and lo-part 22 bits
	srli	MANAL, MANAL, #1
	slli	RF, MANAH, #31
	or	MANAL, MANAL, RF
	srli	MANAH, MANAH, #1
	! Split divisor into four 16-bit parts to do division:
	! HH in RC, HL in RF
	srli	RC, MANBH, #16
	divr	RD, MANAH, MANAH, RC
	zeh	RF, MANBH
	mul	RB, RF, RD
	slli	MANAH, MANAH, #16
	srli	RE, MANAL, #16
	or	MANAH, MANAH, RE
	move	RE, MANAH
	sub	MANAH, MANAH, RB
	slt	$r15, RE, MANAH
	beqz	$r15, .LGmain3

.LGloop1:
	addi	RD, RD, #-1
	add	MANAH, MANAH, MANBH
	slt	$r15, MANAH, MANBH
	beqz	$r15, .LGloop1

.LGmain3:
	divr	RC, MANAH, MANAH, RC
	mul	RB, RF, RC
	slli	MANAH, MANAH, #16
	zeh	RE, MANAL
	or	MANAH, MANAH, RE
	move	RE, MANAH
	sub	MANAH, MANAH, RB
	slt	$r15, RE, MANAH
	beqz	$r15, .LGmain4

.LGloop2:
	addi	RC, RC, #-1
	add	MANAH, MANAH, MANBH
	slt	$r15, MANAH, MANBH
	beqz	$r15, .LGloop2

.LGmain4:
	slli	RD, RD, #16
	add	RD, RD, RC
	! This is a 64-bit multiplication: (RI, RH) is (high, low).
#ifndef __NDS32_ISA_V3M__
	mulr64	$r0, RD, MANBL
#else
	swi	EXPOA, [$sp+(8)]
	swi	RD, [$sp+(4)]
	move	$r0, RD
	move	$r1, MANBL
	bal	umul_ppmm
	lwi	RD, [$sp+(4)]
#endif
	subri	MANAL, RH, #0
	move	RE, MANAH
	sub	MANAH, MANAH, RI
	slt	$r15, RE, MANAH
	beqz	MANAL, .LGmain5
	move	RE, MANAH
	addi	MANAH, MANAH, #-1
	bnez	$r15, .LGloopA
	slt	$r15, RE, MANAH

.LGmain5:
	beqz	$r15, .LGmain6

.LGloopA:
	addi	RD, RD, #-1
	add	MANAL, MANAL, MANBL
	slt	RE, MANAL, MANBL
	add	MANAH, MANAH, MANBH
	slt	$r15, MANAH, MANBH
	beqz	RE, .LGloopA2
	addi	MANAH, MANAH, #1
	bnez	$r15, .LGmain6
	slti	$r15, MANAH, #1

.LGloopA2:
	beqz	$r15, .LGloopA

.LGmain6:
	bne	MANAH, MANBH, .Li25
	move	RI, MANBL
	move	MANAH, MANAL
	move	RC, #0
	move	RH, #0
	b	.LGmain7

.Li25:
	srli	RF, MANBH, #16
	divr	RC, MANAH, MANAH, RF
	zeh	RE, MANBH
	mul	$r15, RE, RC
	slli	MANAH, MANAH, #16
	srli	RB, MANAL, #16
	or	MANAH, MANAH, RB
	move	RB, MANAH
	sub	MANAH, MANAH, $r15
	slt	$r15, RB, MANAH
	beqz	$r15, .Li26

.LGloop3:
	addi	RC, RC, #-1
	add	MANAH, MANAH, MANBH
	slt	$r15, MANAH, MANBH
	beqz	$r15, .LGloop3

.Li26:
	divr	RF, MANAH, MANAH, RF
	mul	RB, RE, RF
	slli	MANAH, MANAH, #16
	zeh	RE, MANAL
	or	MANAH, MANAH, RE
	move	RE, MANAH
	sub	MANAH, MANAH, RB
	slt	$r15, RE, MANAH
	beqz	$r15, .Li28

.LGloop4:
	addi	RF, RF, #-1
	add	MANAH, MANAH, MANBH
	slt	$r15, MANAH, MANBH
	beqz	$r15, .LGloop4

.Li28:
	slli	RC, RC, #16
	add	RC, RC, RF
	! This is a 64-bit multiplication: (RI, RH) is (high, low).
#ifndef __NDS32_ISA_V3M__
	mulr64	$r0, RC, MANBL
#else
	smw.bi	$r2, [$sp], $r3, #0
	move	$r0, RC
	move	$r1, MANBL
	bal	umul_ppmm
	lmw.bi	$r2, [$sp], $r3, #0
#endif

.LGmain7:
	subri	MANAL, RH, #0
	move	RE, MANAH
	sub	MANAH, MANAH, RI
	slt	$r15, RE, MANAH
	beqz	MANAL, .LGmain8
	move	RE, MANAH
	addi	MANAH, MANAH, #-1
	bnez	$r15, .LGloopB
	slt	$r15, RE, MANAH

.LGmain8:
	beqz	$r15, .LGmain9

.LGloopB:
	addi	RC, RC, #-1
	add	MANAL, MANAL, MANBL
	slt	RE, MANAL, MANBL
	add	MANAH, MANAH, MANBH
	slt	$r15, MANAH, MANBH
	beqz	RE, .LGloopB2
	addi	MANAH, MANAH, #1
	bnez	$r15, .LGmain9
	slti	$r15, MANAH, #1

.LGloopB2:
	beqz	$r15, .LGloopB

.LGmain9:
	sltsi	$r15, RD, #0
#ifdef __NDS32_ISA_V3M__
	lwi	EXPOA, [$sp+(8)]
#endif
	bnez	$r15, .LGmain10
	move	$r15, RC
	add	RC, RC, RC
	slt	$r15, RC, $r15
	add	RD, RD, RD
	add	RD, RD, $r15
	addi	EXPOA, EXPOA, #-1

.LGmain10:
	or	MANAH, MANAH, MANAL
	ori	RH, RC, #1
	cmovz	RH, RC, MANAH
	move	RI, RD
	blez	EXPOA, .LGunderflow	! exponent(quotient) is too small
	subri	RB, EXPOA, #0x7ff
	blez	RB, .LGinf		! exponent(quotient) is too big, return inf
	addi	RH, RH, #0x400
	slti	$r15, RH, #0x400
	beqz	$r15, .LGround
	add	RI, RI, $r15
	slt	$r15, RI, $r15
	add	EXPOA, EXPOA, $r15

	! do rounding
.LGround:
	srli	RC, RH, #11
	andi	RC, RC, #1
	sub	RH, RH, RC
	srli	RH, RH, #11
	slli	RC, RI, #21
	or	RH, RH, RC

	! do packing
	slli	RI, RI, #1
	srli	RI, RI, #12
	slli	RB, EXPOA, #20
	or	RI, RI, RB

.LGret:
	or	RI, RI, AXORB
	pop25	$r10, #16

	! ---------------------------------------------------------------------
	! exponent(A) is 0x000
	! ---------------------------------------------------------------------
.LGAexpzero:
	or	$r15, MANAH, MANAL
	beqz	$r15, .LGAzero		! A is zero
	! A is subnormal
	srli	$r15, MANAL, #31
	add	MANAH, MANAH, MANAH
	add	MANAH, MANAH, $r15
	add	MANAL, MANAL, MANAL
	! count leading zeros of A
	bnez	MANAH, .LGAcont
	move	MANAH, MANAL
	move	MANAL, #0
	addi	EXPOA, EXPOA, #-32

	! MANAH is non-zero
.LGAcont:
#ifdef __NDS32_EXT_PERF__
	clz	RE, MANAH
#else
	move	RE, #0
	move	RF, MANAH
	b	.LGAloop2

.LGAloop:
	add	RF, RF, RF
	addi	RE, RE, #1

.LGAloop2:
	slt	$r15, RF, SIGN
	bnez	$r15, .LGAloop
#endif
	beqz	RE, .LGmain1
	sub	EXPOA, EXPOA, RE
	subri	RC, RE, #32
	srl	RC, MANAL, RC
	sll	MANAL, MANAL, RE
	sll	MANAH, MANAH, RE
	or	MANAH, MANAH, RC
	b	.LGmain1

.LGAzero:
	beq	RD, EXPOB, .LGAzero2	! B is NaN or inf, goto .LGAzero2
	! B is finite: check whether B is zero.
	bnez	EXPOB, .LGsetsign
	or	$r15, MANBH, MANBL
	beqz	$r15, .LGnan	! zero/zero is NaN

.LGsetsign:
	move	RI, AXORB
	pop25	$r10, #16

.LGAzero2:
	or	MANBH, MANBH, MANBL
	beq	MANBH, SIGN, .LGsetsign	! zero/inf is zero
	! zero/NaN is NaN

.LGnan:
	move	RH, #0
	move	RI, #0xfff80000
	pop25	$r10, #16

	! ---------------------------------------------------------------------
	! exponent(A) is 0x7ff
	! ---------------------------------------------------------------------
.LGAinfnan:
	or	MANAH, MANAH, MANAL
	bne	MANAH, SIGN, .LGnan		! A is NaN, return NaN
	! A is inf: check whether B is finite.
	beq	RD, EXPOB, .LGnan	! both inf/inf and inf/NaN are NaN
	! inf/finite is inf

.LGinf:
	move	RH, #0
	move	RI, #0x7ff00000
	or	RI, RI, AXORB
	pop25	$r10, #16

	! ---------------------------------------------------------------------
	! exponent(B) is 0x7ff
	! ---------------------------------------------------------------------
.LGBinfnan:
	or	MANBH, MANBH, MANBL
	bne	MANBH, SIGN, .LGnan		! B is NaN, return NaN
	! B is inf: finite/inf is zero
	move RH, #0
	b	.LGsetsign

	! ---------------------------------------------------------------------
	! exponent(B) is 0x000
	! ---------------------------------------------------------------------
.LGBexpzero:
	or	$r15, MANBH, MANBL
	beqz	$r15, .LGinf		! finite/zero is inf
	! B is subnormal
	srli	$r15, MANBL, #31
	add	MANBH, MANBH, MANBH
	add	MANBH, MANBH, $r15
	add	MANBL, MANBL, MANBL
	! count leading zeros of B
	bnez	MANBH, .LGBcont
	move	MANBH, MANBL
	move	MANBL, #0
	addi	EXPOB, EXPOB, #-32

	! MANBH is non-zero
.LGBcont:
#ifdef __NDS32_EXT_PERF__
	clz	RE, MANBH
#else
	move	RE, #0
	move	RF, MANBH
	b	.LGBloop2

.LGBloop:
	add	RF, RF, RF
	addi	RE, RE, #1

.LGBloop2:
	slt	$r15, RF, SIGN
	bnez	$r15, .LGBloop
#endif
	beqz	RE, .LGmain2
	sub	EXPOB, EXPOB, RE
	subri	RC, RE, #32
	srl	RC, MANBL, RC
	sll	MANBL, MANBL, RE
	sll	MANBH, MANBH, RE
	or	MANBH, MANBH, RC
	b	.LGmain2

	! ---------------------------------------------------------------------
	! handle underflow
	! ---------------------------------------------------------------------
.LGunderflow:
	move	MANAL, #0
	subri	RD, EXPOA, #1
	slti	$r15, RD, #0x20
	bnez	$r15, .LGunderflow2
	move	MANAL, RH
	move	RH, RI
	move	RI, #0
	addi	RD, RD, #0xffffffe0
	beqz	RH, .LGunderflow2
	slti	$r15, RD, #0x20
	beqz	$r15, .LGignore		! result too small, return zero

	! 1-exponent(A), in RD, is 0-31
.LGunderflow2:
	beqz	RD, .LGunderflow3	! it is zero, skip
	subri	RC, RD, #0x20
	sll	MANAH, RI, RC
	sll	RB, RH, RC
	srl	RH, RH, RD
	srl	RI, RI, RD
	or	RH, RH, MANAH
	or	MANAL, MANAL, RB
!	ori	RD, RH, #1
!	cmovn	RH, RD, MANAL
	beqz	MANAL, .LGunderflow3
	ori	RH, RH, #1

.LGunderflow3:
	addi	RH, RH, #0x400
	slti	$r15, RH, #0x400
	add	RI, RI, $r15
	srli	EXPOA, RI, #31
	b	.LGround

.LGignore:
	move	RH, #0
	b	.LGsetsign
	.size __divdf3, .-__divdf3
#endif /* L_div_df */



#ifdef L_mul_sf

#if !defined (__big_endian__)
	#define RH	$r0
	#define RI	$r1
	#define RJ	$r2
	#define RK	$r3
#else
	#define RI	$r0
	#define RH	$r1
	#define RK	$r2
	#define RJ	$r3
#endif
#define SIGN	$r4
#ifdef __NDS32_REDUCE_REGS__
#define EXPOA	$r6
#define MANTA	RJ
#define VALUA	RK
#define EXPOB	$r7
#define MANTB	RH
#define VALUB	RI
#define SPROD	$r8
#else
#define EXPOA	$r16
#define MANTA	RJ
#define VALUA	RK
#define EXPOB	$r17
#define MANTB	RH
#define VALUB	RI
#define SPROD	$r18
#endif
#define RA	VALUB
#define RB	$r5

	.text
	.align	2
	.global	__mulsf3
	.type	__mulsf3, @function
__mulsf3:
#ifdef __NDS32_REDUCE_REGS__
	smw.adm	$r6, [$sp], $r8, 2
#endif

	xor	SPROD, $r1, $r0
	move	SIGN, #0x80000000
	and	SPROD, SPROD, SIGN	! sign(A xor B)
	slli	VALUA, $r0, 1		! A<<1
	slli	VALUB, $r1, 1		! B<<1
	srli	EXPOA, VALUA, 24	! exponent(A)
	srli	EXPOB, VALUB, 24	! exponent(B)
	slli	MANTA, VALUA, 7		! mantissa(A)<<8
	slli	MANTB, VALUB, 7		! mantissa(B)<<8
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqz	EXPOA, .LFzeroAexp	! exponent(A) is zero, goto .LFzeroAexp
	beqc	EXPOA, 0xff, .LFinfnanA	! A is inf or NaN, goto .LFinfnanA
#else
	move	RB, #0xff
	beqz	EXPOA, .LFzeroAexp	! exponent(A) is zero, goto .LFzeroAexp
	beq	RB, EXPOA, .LFinfnanA	! A is inf or NaN, goto .LFinfnanA
#endif
	or	MANTA, MANTA, SIGN

.LFlab1:
	beqz	EXPOB, .LFzeroB		! exponent(B) is zero, goto .LFzeroB
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOB, 0xff, .LFinfnanB	! B is inf or NaN, goto .LFinfnanB
#else
	beq	RB, EXPOB, .LFinfnanB	! B is inf or NaN, goto .LFinfnanB
#endif
	or	MANTB, MANTB, SIGN

	! ---------------------------------------------------------------------
	! This is a 64-bit multiplication.
	! ---------------------------------------------------------------------
.LFlab2:
#ifdef __NDS32_ISA_V3M__
	move	RI, MANTA
	bal	umul_ppmm
#else
	mulr64	$r0, MANTA, MANTB
#endif
	ori	MANTA, RI, #1
	cmovz	MANTA, RI, RH
	sltsi	$r15, MANTA, #0
	bnezs8	.Li18
	slli	MANTA, MANTA, #1
	addi	EXPOA, EXPOA, #-1

.Li18:
	addi	RB, EXPOB, #0xffffff82
	add	EXPOA, EXPOA, RB
	blez	EXPOA, .LFunder		! A*B underflow, goto .LFunder
	slti	$r15, EXPOA, #0xff
	beqz	$r15, .LFinf		! A*B overflow, goto .LFinf

	! ---------------------------------------------------------------------
	! do rounding
	! ---------------------------------------------------------------------
.LFround:
	addi	MANTA, MANTA, #128
	slti	$r15, MANTA, #128
	add	EXPOA, EXPOA, $r15
	srli	RB, MANTA, #8
	andi	RB, RB, #1
	sub	MANTA, MANTA, RB

	! ---------------------------------------------------------------------
	! pack result
	! ---------------------------------------------------------------------
	slli	MANTA, MANTA, #1
	srli	MANTA, MANTA, #9
	slli	$r0, EXPOA, #23
	or	$r0, $r0, MANTA
.LFpack:
	or	$r0, $r0, SPROD

.LFret:
#ifdef __NDS32_REDUCE_REGS__
	lmw.bim	$r6, [$sp], $r8, 2
#endif
	ret5	$lp

	! ---------------------------------------------------------------------
	! exponent(A) is 0x00
	! ---------------------------------------------------------------------
.LFzeroAexp:
#ifdef __NDS32_EXT_PERF__
	beqz	MANTA, .LFzeroA		! A is zero

	! A is denorm
	add	MANTA, MANTA, MANTA
	clz	$r15, MANTA
	sub	EXPOA, EXPOA, $r15
	sll	MANTA, MANTA, $r15
	b	.LFlab1

.LFzeroA:
	! A is zero
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOB, 0xff, .LFnan	! zero * inf = zero * NaN = NaN
#else
	beq	RB, EXPOB,.LFnan	! zero * inf = zero * NaN = NaN
#endif

.LFzero:
	move	$r0, SPROD
	b	.LFret
#else
	bnez	MANTA, .LFloopA2	! A is denorm

	! A is zero
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOB, 0xff, .LFnan	! zero * inf = zero * NaN = NaN
#else
	beq	RB, EXPOB,.LFnan	! zero * inf = zero * NaN = NaN
#endif

.LFzero:
	move	$r0, SPROD
	b	.LFret

.LFloopA:
	addi	EXPOA, EXPOA, #-1
.LFloopA2:
	add	MANTA, MANTA, MANTA
	slt	$r15, MANTA, SIGN
	bnez	$r15, .LFloopA
	b	.LFlab1
#endif

	! ---------------------------------------------------------------------
	! exponent(A) is 0xff
	! ---------------------------------------------------------------------
.LFinfnanA:
	bne	MANTA, SIGN, .LFnan	! A is NaN: NaN * B = NaN

	! A is inf
	beqz	VALUB, .LFnan		! B is zero: inf * zero = NaN
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	bnec	EXPOB, 0xff, .LFinf	! B is finite: inf * B = inf
#else
	bne	RB, EXPOB, .LFinf	! B is finite: inf * B = inf
#endif

	! ---------------------------------------------------------------------
	! exponent(B) is 0xff
	! ---------------------------------------------------------------------
.LFinfnanB:
	bne	MANTB, SIGN, .LFnan	! B is NaN: A * NaN = NaN

.LFinf:
	move	$r0, #0x7f800000
	b	.LFpack

	! ---------------------------------------------------------------------
	! exponent(B) is 0x00
	! ---------------------------------------------------------------------
.LFzeroB:
#ifdef __NDS32_EXT_PERF__
	beqz	MANTB, .LFzero		! B is zero

	! B is denorm
	add	MANTB, MANTB, MANTB
	clz	$r15, MANTB
	sub	EXPOB, EXPOB, $r15
	sll	MANTB, MANTB, $r15
#else
	bnez	MANTB, .LFloopB2	! B is denorm
	b	.LFzero			! B is zero
.LFloopB:
	addi	EXPOB, EXPOB, #-1
.LFloopB2:
	add	MANTB, MANTB, MANTB
	slt	$r15, MANTB, SIGN
	bnez	$r15, .LFloopB
#endif
	b	.LFlab2

.LFnan:
	move	$r0, #0xffc00000
	b	.LFret

	! ---------------------------------------------------------------------
	! A*B underflow
	! ---------------------------------------------------------------------
.LFunder:
	subri	RA, EXPOA, #1
	slti	$r15, RA, #0x20
	beqzs8	.LFzero
	subri	RB, RA, #0x20
	sll	EXPOA, MANTA, RB
	srl	MANTA, MANTA, RA
	beqz	EXPOA, .LFunder2
	ori	MANTA, MANTA, #2
.LFunder2:
	addi	RB, MANTA, #0x80
	sltsi	EXPOA, RB, #0
	b	.LFround
	.size	__mulsf3, .-__mulsf3
#endif /* L_mul_sf */



#ifdef L_div_sf

#define SIGN	$r4
#ifdef __NDS32_REDUCE_REGS__
#define EXPOA	$r6
#define MANTA	$r2
#define VALUA	$r2
#define EXPOB	$r7
#define MANTB	$r3
#define VALUB	$r1
#define SQUOT	$r8
#define RA	VALUB
#define RB	$r5
#define RC	$r0
#define RD	$r9
#else
#define EXPOA	$r16
#define MANTA	$r2
#define VALUA	$r2
#define EXPOB	$r17
#define MANTB	$r3
#define VALUB	$r1
#define SQUOT	$r18
#define RA	VALUB
#define RB	$r5
#define RC	$r0
#define RD	$r19
#endif

#define DHI	RB	// high18(MANTB)
#define DLO	RD	// low14(MANTB)
#define QHI	RA	// MANTA / MANTB
#define REM	RC	// MANTA % MANTB

	.text
	.align	2
	.global	__divsf3
	.type	__divsf3, @function
__divsf3:
#ifdef __NDS32_REDUCE_REGS__
	smw.adm	$r6, [$sp], $r9, 0
#endif

	xor	SQUOT, $r1, $r0
	move	SIGN, #0x80000000
	and	SQUOT, SQUOT, SIGN	! sign(A xor B)
	slli	VALUA, $r0, 1		! A<<1
	slli	VALUB, $r1, 1		! B<<1
	srli	EXPOA, VALUA, 24	! exponent(A)
	srli	EXPOB, VALUB, 24	! exponent(B)
	slli	MANTA, VALUA, 7		! mantissa(A)<<8
	slli	MANTB, VALUB, 7		! mantissa(B)<<8
	beqz	EXPOA, .LGzeroAexp	! exponent(A) is zero, goto .LGzeroAexp
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOA, 0xff, .LGinfnanA	! A is inf or NaN, goto .LGinfnanA
#else
	move	RB, #0xff
	beq	RB, EXPOA, .LGinfnanA	! A is inf or NaN, goto .LGinfnanA
#endif
	or	MANTA, MANTA, SIGN

.LGlab1:
	beqz	EXPOB, .LGzeroB		! exponent(B) is zero, goto .LGzeroB
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPOB, 0xff, .LGinfnanB	! B is inf or NaN, goto .LGinfnanB
#else
	beq	RB, EXPOB, .LGinfnanB	! B is inf or NaN, goto .LGinfnanB
#endif
	or	MANTB, MANTB, SIGN

.LGlab2:
	slt	$r15, MANTA, MANTB
	bnez	$r15, .LGlab3
	srli	MANTA, MANTA, #1
	addi	EXPOA, EXPOA, #1

	! ---------------------------------------------------------------------
	! This is a 64-bit division.
	! high part of dividend, MANTA, is smaller than divisor MANTB.
	! ---------------------------------------------------------------------
.LGlab3:
	srli	DHI, MANTB, #14
	andi	DLO, MANTB, #0x3fff
	divr	QHI, REM, MANTA, DHI
	mul	MANTA, DLO, QHI
	slli	REM, REM, #14
	slt	$r15, REM, MANTA
	beqz	$r15, .LGlab4
	addi	QHI, QHI, #-1
	add	REM, REM, MANTB

.LGlab4:
	sub	REM, REM, MANTA
	divr	MANTA, REM, REM, DHI
	mul	DLO, DLO, MANTA
	slli	REM, REM, #14
	slt	$r15, REM, DLO
	beqz	$r15, .LGlab5
	addi	MANTA, MANTA, #-1
	add	REM, REM, MANTB

.LGlab5:
	sub	REM, REM, DLO
	slli	RD, QHI, #14
	add	MANTA, MANTA, RD
	slli	MANTA, MANTA, #4
	beqz	REM, .LGlab6
	ori	MANTA, MANTA, #1

.LGlab6:
	subri	RB, EXPOB, #0x7e
	add	EXPOA, EXPOA, RB
	blez	EXPOA, .LGunder		! A/B underflow, goto .LGunder
	slti	$r15, EXPOA, #0xff
	beqz	$r15, .LGinf		! A/B overflow, goto .LGinf

	! ---------------------------------------------------------------------
	! do rounding
	! ---------------------------------------------------------------------
.LGround:
	addi	MANTA, MANTA, #128
	slti	$r15, MANTA, #128
	add	EXPOA, EXPOA, $r15
	srli	RB, MANTA, #8
	andi	RB, RB, #1
	sub	MANTA, MANTA, RB

	! ---------------------------------------------------------------------
	! pack result
	! ---------------------------------------------------------------------
	slli	MANTA, MANTA, #1
	srli	MANTA, MANTA, #9
	slli	$r0, EXPOA, #23
	or	$r0, $r0, MANTA
.LGpack:
	or	$r0, $r0, SQUOT

.LGret:
#ifdef __NDS32_REDUCE_REGS__
	lmw.bim	$r6, [$sp], $r9, 0
#endif
	ret5	$lp

	! ---------------------------------------------------------------------
	! exponent(A) is 0x00
	! ---------------------------------------------------------------------
.LGzeroAexp:
#ifdef __NDS32_EXT_PERF__
	add	MANTA, MANTA, MANTA
	beqz	MANTA, .LGzeroA
	clz	$r15, MANTA
	sub	EXPOA, EXPOA, $r15
	sll	MANTA, MANTA, $r15
#else
	bnez	MANTA, .LGloopA2
	b	.LGzeroA
.LGloopA:
	addi	EXPOA, EXPOA, #-1
.LGloopA2:
	add	MANTA, MANTA, MANTA
	slt	$r15, MANTA, SIGN
	bnez	$r15, .LGloopA
#endif
	b	.LGlab1

	! A is 0.0f
.LGzeroA:
	beqz	VALUB, .LGnan		! 0.0f / 0.0f = NaN
	move	RB, 0xff000000
	slt	$r15, RB, VALUB
	bnez	$r15, .LGnan		! 0.0f / NaN = NaN

.LGzero:
	move	$r0, SQUOT
	b	.LGret

	! ---------------------------------------------------------------------
	! exponent(A) is 0xff
	! ---------------------------------------------------------------------
.LGinfnanA:
	bne	MANTA, SIGN, .LGnan	! A is NaN: NaN / B = NaN

	! A if inf
	beq	EXPOB, EXPOA, .LGnan	! no matter B is inf or NaN

.LGinf:
	move	$r0, #0x7f800000
	or	$r0, $r0, SQUOT
	b	.LGret

	! ---------------------------------------------------------------------
	! exponent(B) is 0xff
	! ---------------------------------------------------------------------
.LGinfnanB:
	beq	MANTB, SIGN, .LGzero	! B is inf: A / inf = 0.0f

.LGnan:
	move	$r0, #0xffc00000
	b	.LGret

	! ---------------------------------------------------------------------
	! exponent(B) is 0x00
	! ---------------------------------------------------------------------
.LGzeroB:
#ifdef __NDS32_EXT_PERF__
	add	MANTB, MANTB, MANTB
	beqz	MANTB, .LGinf
	clz	$r15, MANTB
	sub	EXPOB, EXPOB, $r15
	sll	MANTB, MANTB, $r15
#else
	bnez	MANTB, .LGloopB2
	b	.LGinf
.LGloopB:
	addi	EXPOB, EXPOB, #-1
.LGloopB2:
	add	MANTB, MANTB, MANTB
	slt	$r15, MANTB, SIGN
	bnez	$r15, .LGloopB
#endif
	b	.LGlab2

	! ---------------------------------------------------------------------
	! A/B underflow
	! ---------------------------------------------------------------------
.LGunder:
	subri	RA, EXPOA, #1
	slti	$r15, RA, #0x20
	beqzs8	.LGzero
	subri	RB, RA, #0x20
	sll	EXPOA, MANTA, RB
	srl	MANTA, MANTA, RA
	beqz	EXPOA, .LGunder2
	ori	MANTA, MANTA, #2
.LGunder2:
	addi	RB, MANTA, #0x80
	sltsi	EXPOA, RB, #0
	b	.LGround
	.size	__divsf3, .-__divsf3
#endif /* L_div_sf */



#ifdef L_negate_sf

	.text
	.align	2
	.global	__negsf2
	.type	__negsf2, @function
__negsf2:
	move    $r1, #0x80000000
	xor     $r0, $r0, $r1
	ret5    $lp
	.size __negsf2, .-__negsf2
#endif /* L_negate_sf */



#ifdef L_negate_df

#ifndef __big_endian__
	#define RI     $r1
#else
	#define RI     $r0
#endif
	.text
	.align	2
	.global	__negdf2
	.type	__negdf2, @function
__negdf2:
	move    $r2, #0x80000000
	xor     RI, RI, $r2
	ret5    $lp
	.size __negdf2, .-__negdf2
#endif /* L_negate_df */



#ifdef L_sf_to_df

#ifndef __big_endian__
	#define RH	$r0
	#define RI	$r1
#else
	#define RI	$r0
	#define RH	$r1
#endif
#define SIGN	$r2
#define EXPO	$r3
#define MANT	$r4
	.text
	.align	2
	.global	__extendsfdf2
	.type	__extendsfdf2, @function
__extendsfdf2:
	slli	$r5, $r0, 1
	beqz	$r5, .LJzero		! A-in is zero, goto .LJzero

	srli	EXPO, $r5, #24		! exponent(A-in)
	move	$r1, #0x80000000
	and	SIGN, $r1, $r0		! sign(A-in)
	slli	MANT, $r5, #8		! mantissa(A-in)
	beqz	EXPO, .LJdenorm		! exponent(A-in) is zerop, goto .LJdenorm
#ifndef __FAST_MATH__
#if defined(__NDS32_ISA_V3__)||defined(__NDS32_ISA_V3M__)
	beqc	EXPO, #0xff, .LJinfnan	! exponent(A-in) is 0xff, goto .LJinfnan
#else
	slti	$r15, EXPO, #0xff
	beqzs8	.LJinfnan		! exponent(A-in) is 0xff, goto .LJinfnan
#endif
#endif // end of __FAST_MATH__

.LJlab1:
	addi	EXPO, EXPO, #0x380	! exponent(A-out)
	slli	RH, MANT, #20		! low 32-bit(A-out)
	srli	RI, MANT, #12		! high 20-bit mantissa(A-out)
	or	RI, RI, SIGN
	slli	EXPO, EXPO, #20
	or	RI, RI, EXPO		! high 32-bit(-out)
	ret5	$lp

#ifdef __NDS32_EXT_PERF__
.LJdenorm:
	clz	$r1, MANT
	sub	EXPO, EXPO, $r1
	sll	MANT, MANT, $r1
#else
.LJdenorm2:
	addi	EXPO, EXPO, #-1
	add	MANT, MANT, MANT
.LJdenorm:
	slt	$r15, MANT, $r1
	bnezs8	.LJdenorm2
#endif
	slli	MANT, MANT, 1		! shift out implied 1
	b	.LJlab1
#ifndef __FAST_MATH__
.LJinfnan:
	beqz	MANT, .LJinf
	move	RI, 0xfff80000
	b	.LJcont

.LJinf:
	move	$r5, 0x700000
#ifdef __big_endian__
	or	RI, $r0, $r5
#else
	or	$r0, $r0, $r5
#endif
#endif // end of __FAST_MATH__
.LJzero:
#ifndef __big_endian__
	move	RI, $r0
#endif

.LJcont:
	move	RH, 0
	ret5	$lp
	.size __extendsfdf2, .-__extendsfdf2
#endif /* L_sf_to_df */



#ifdef L_df_to_sf

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RJ     $r2
	#define RK     $r3
#else
	#define RI     $r0
	#define RH     $r1
	#define RK     $r2
	#define RJ     $r3
#endif
	.text
	.align	2
	.global	__truncdfsf2
	.type	__truncdfsf2, @function
__truncdfsf2:
	pushm   $r6, $r8

	slli    RK, RI, #11
	srli    $r7, RH, #21
	or      RK, RK, $r7
	slli    RJ, RH, #11
	move    $r7, #0x80000000
	or      RK, RK, $r7
	and     $r5, RI, $r7
	slli    $r4, RI, #1
	srli    $r4, $r4, #21
	addi    $r4, $r4, #0xfffffc80
	addi    $r7, $r4, #-1
	slti    $r15, $r7, #0xfe
	beqzs8  .LKspec

.LKlab1:
	beqz    RJ, .Li45
	ori     RK, RK, #1
.Li45:
	#ADD(RK, $0x80)
	move    $r15, #0x80
	add     RK, RK, $r15
	slt     $r15, RK, $r15

	#ADDC($r4, $0x0)
	add     $r4, $r4, $r15
	srli    $r7, RK, #8
	andi    $r7, $r7, #1
	sub     RK, RK, $r7
	slli    RK, RK, #1
	srli    RK, RK, #9
	slli    $r7, $r4, #23
	or      RK, RK, $r7
	or      $r0, RK, $r5

.LK999:
	popm    $r6, $r8
	ret5    $lp

.LKspec:
	subri   $r15, $r4, #0x47f
	bnezs8  .Li46
	slli    $r7, RK, #1
	or      $r7, $r7, RJ
	beqz    $r7, .Li46
	move    $r0, #0xffc00000
	b       .LK999
.Li46:
	sltsi   $r15, $r4, #0xff
	bnezs8  .Li48
	move    $r7, #0x7f800000
	or      $r0, $r5, $r7
	b       .LK999
.Li48:
	subri   $r6, $r4, #1
	move    $r7, #0x20
	slt     $r15, $r6, $r7
	bnezs8  .Li49
	move    $r0, $r5
	b       .LK999
.Li49:
	subri   $r8, $r6, #0x20
	sll     $r7, RK, $r8
	or      RJ, RJ, $r7
	srl     RK, RK, $r6
	move    $r4, #0
	move    $r7, #0x80000000
	or      RK, RK, $r7
	b       .LKlab1
	.size __truncdfsf2, .-__truncdfsf2
#endif /* L_df_to_sf */



#ifdef L_fixdfsi

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
#else
	#define RI     $r0
	#define RH     $r1
#endif
	.global	__fixdfsi
	.type	__fixdfsi, @function
__fixdfsi:
#if defined(__NDS32_EXT_FPU_DP)
        fd2si.z   $fs0, $fd0
        fmfsr   $r0, $fs0
        ret5    $lp
#else
#if defined(__NDS32_EXT_FPU_SP)
        fmfdr   $r0, $fd0
#endif
	slli    $r3, RI, #11
	srli    $r4, RH, #21
	or      $r3, $r3, $r4
	move    $r4, #0x80000000
	or      $r3, $r3, $r4
	slli    $r4, RI, #1
	srli    $r4, $r4, #21
	subri   $r2, $r4, #0x41e
	blez    $r2, .LLnaninf
	move    $r4, #0x20
	slt     $r15, $r2, $r4
	bnezs8  .LL72
	move    $r3, #0
.LL72:
	srl     $r3, $r3, $r2
	sltsi   $r15, RI, #0
	beqzs8  .Li50
	subri   $r3, $r3, #0
.Li50:
	move    $r0, $r3
	ret5    $lp

.LLnaninf:
	beqz    RH, .Li51
	ori     RI, RI, #1
.Li51:
	move    $r4, #0x7ff00000
	slt     $r15, $r4, RI
	beqzs8  .Li52
	move    $r0, #0x80000000
	ret5    $lp
.Li52:
	move    $r0, #0x7fffffff
	ret5    $lp
#endif
	.size __fixdfsi, .-__fixdfsi
#endif /* L_fixdfsi */



#ifdef L_fixsfdi

#ifndef __big_endian__
	#define RL     $r1
	#define RM     $r2
#else
	#define RM     $r1
	#define RL     $r2
#endif
	.text
	.align	2
	.global	__fixsfdi
	.type	__fixsfdi, @function
__fixsfdi:
#if defined(__NDS32_EXT_FPU_SP)
        fmfsr   $r0, $fs0
#endif
	srli    $r3, $r0, #23
	andi    $r3, $r3, #0xff
	slli    RM, $r0, #8
	move    $r5, #0x80000000
	or      RM, RM, $r5
	move    RL, #0
	sltsi   $r15, $r3, #0xbe
	beqzs8  .LCinfnan
	subri   $r3, $r3, #0xbe
.LL8:
	move    $r5, #0x20
	slt     $r15, $r3, $r5
	bnezs8  .LL9
	move    RL, RM
	move    RM, #0
	addi    $r3, $r3, #0xffffffe0
	bnez    RL, .LL8
.LL9:
	beqz    $r3, .LL10
	move    $r4, RM
	srl     RL, RL, $r3
	srl     RM, RM, $r3
	subri   $r3, $r3, #0x20
	sll     $r4, $r4, $r3
	or      RL, RL, $r4
.LL10:
	sltsi   $r15, $r0, #0
	beqzs8  .LCret

	subri   RM, RM, #0
	beqz    RL, .LL11
	subri   RL, RL, #0
	subi45  RM, #1
.LL11:

.LCret:
	move    $r0, $r1
	move    $r1, $r2
	ret5    $lp

.LCinfnan:
	sltsi   $r15, $r0, #0
	bnezs8  .LCret3
	subri   $r15, $r3, #0xff
	bnezs8  .Li7
	slli    $r5, RM, #1
	beqz    $r5, .Li7

.LCret3:
	move    RM, #0x80000000
	b       .LCret
.Li7:
	move    RM, #0x7fffffff
	move    RL, #-1
	b       .LCret
	.size	__fixsfdi, .-__fixsfdi
#endif /* L_fixsfdi */



#ifdef L_fixdfdi

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RL     $r3
	#define RM     $r4
#else
	#define RI     $r0
	#define RH     $r1
	#define RM     $r3
	#define RL     $r4
#endif
	.text
	.align	2
	.global	__fixdfdi
	.type	__fixdfdi, @function
__fixdfdi:
	pushm   $r6, $r6
#if defined(__NDS32_EXT_FPU_SP)
        fmfdr   $r0, $fd0
#endif
	slli    $r5, RI, #1
	srli    $r5, $r5, #21
	slli    RM, RI, #11
	srli    $r6, RH, #21
	or      RM, RM, $r6
	slli    RL, RH, #11
	move    $r6, #0x80000000
	or      RM, RM, $r6
	slti    $r15, $r5, #0x43e
	beqzs8  .LCnaninf
	subri   $r2, $r5, #0x43e
.LL14:
	move    $r6, #0x20
	slt     $r15, $r2, $r6
	bnezs8  .LL15
	move    RL, RM
	move    RM, #0
	addi    $r2, $r2, #0xffffffe0
	bnez    RL, .LL14
.LL15:
	beqz    $r2, .LL16
	move    RH, RM
	srl     RL, RL, $r2
	srl     RM, RM, $r2
	subri   $r2, $r2, #0x20
	sll     RH, RH, $r2
	or      RL, RL, RH
.LL16:
	sltsi   $r15, RI, #0
	beqzs8  .LCret

	subri   RM, RM, #0
	beqz    RL, .LL17
	subri   RL, RL, #0
	subi45  RM, #1
.LL17:

.LCret:
	move    RH, RL
	move    RI, RM
	popm    $r6, $r6
	ret5    $lp

.LCnaninf:
	sltsi   $r15, RI, #0
	bnezs8  .LCret3
	subri   $r15, $r5, #0x7ff
	bnezs8  .Li5
	slli    $r6, RM, #1
	or      $r6, $r6, RL
	beqz    $r6, .Li5

.LCret3:
	move    RM, #0x80000000
	move    RL, #0
	b       .LCret
.Li5:
	move    RM, #0x7fffffff
	move    RL, #-1
	b       .LCret
	.size	__fixdfdi, .-__fixdfdi
#endif /* L_fixdfdi */



#ifdef L_fixunssfsi
	.global	__fixunssfsi
	.type	__fixunssfsi, @function
__fixunssfsi:
#if defined(__NDS32_EXT_FPU_SP)
        fs2ui.z   $fs0, $fs0
        fmfsr   $r0, $fs0
	ret5 $lp
#else
	bltz $r0,  .LZero	/* negative, return 0 */
	srli $r3,$r0,#0x17
	addi $r3,$r3,#-127
	bltz $r3,  .LZero	/* too small, return 0 */
	sltsi $r15,$r3,#0x20
	beqzs8   .LMax		/* too big, return MAX */
	slli $r0,$r0,#0x8
#ifdef __NDS32_EXT_PERF__
	bset $r1,$r0,#0x1f
#else
	sethi $r2,#0x80000
	or $r1,$r0,$r2
#endif
	subri $r0,$r3,#0x1f
	srl $r0,$r1,$r0
	ret5 $lp
.LZero:
	movi55 $r0,#0x0
	ret5 $lp
.LMax:
	movi55 $r0,#-1
	ret5 $lp
#endif
	.size	__fixunssfsi, .-__fixunssfsi
#endif /* L_fixunssfsi */



#ifdef L_fixunsdfsi

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
#else
	#define RI     $r0
	#define RH     $r1
#endif
	.text
	.align	2
	.global	__fixunsdfsi
	.type	__fixunsdfsi, @function
__fixunsdfsi:
#if defined(__NDS32_EXT_FPU_DP)
        fd2ui.z  $fs0, $fd0
        fmfsr   $r0, $fs0
        ret5    $lp
#else
#if defined(__NDS32_EXT_FPU_SP)
        fmfdr   $r0, $fd0
#endif
	slli    $r3, RI, #11
	srli    $r4, RH, #21
	or      $r3, $r3, $r4
	move    $r4, #0x80000000
	or      $r3, $r3, $r4
	slli    $r4, RI, #1
	srli    $r4, $r4, #21
	subri   $r2, $r4, #0x41e
	sltsi   $r15, $r2, #0
	bnezs8  .LNnaninf
	move    $r4, #0x20
	slt     $r15, $r2, $r4
	bnezs8  .LL73
	move    $r3, #0
.LL73:
	srl     $r3, $r3, $r2
	sltsi   $r15, RI, #0
	beqzs8  .Li53
	subri   $r3, $r3, #0
.Li53:
	move    $r0, $r3
	ret5    $lp

.LNnaninf:
	beqz    RH, .Li54
	ori     RI, RI, #1
.Li54:
	move    $r4, #0x7ff00000
	slt     $r15, $r4, RI
	beqzs8  .Li55
	move    $r0, #0x80000000
	ret5    $lp
.Li55:
	move    $r0, #-1
	ret5    $lp
#endif
	.size __fixunsdfsi, .-__fixunsdfsi
#endif /* L_fixunsdfsi */



#ifdef L_fixunssfdi
	.text
	.align	2
	.global	__fixunssfdi
	.type	__fixunssfdi, @function
__fixunssfdi:
#define INPUT $r0
#define EXP $r2
#define TMP $r3
#define REAL_EXP $r2
#ifndef __big_endian__
#define MANL $r0
#define MANH $r1
#else
#define MANL $r1
#define MANH $r0
#endif
#if defined(__NDS32_EXT_FPU_SP)
        fmfsr   $r0, $fs0
#endif
	bltz INPUT, .LZero !negative, return 0

	srli EXP,INPUT,#0x17
	addi REAL_EXP, EXP,#-127
	bltz REAL_EXP, .LZero  ! too small, return 0

	sltsi $r15,REAL_EXP,#0x40 ! too large, return Max
	beqzs8 .LMax

	slli MANL,INPUT,#0x8
#ifdef	__NDS32_EXT_PERF__
	bset MANL,MANL,#0x1f
#else
	sethi TMP,#0x80000
	or33 MANL,TMP
#endif
	subri TMP,REAL_EXP,#0x1f
	bltz TMP,.Lgt31  ! real_exp > 32

	! real_exp <= 31
	srl MANL,MANL,TMP
	movi55 MANH,#0x0
	ret5 $lp

.Lgt31:
	subri REAL_EXP,REAL_EXP,#0x3f
	neg33 TMP,TMP
	srl MANH,MANL,REAL_EXP
	sll MANL,MANL,TMP
	beqc TMP, #0x20, .LClrL
	ret5 $lp
.LZero:
	movi55 MANH,#0x0
.LClrL:
	movi55 MANL,#0x0
	ret5 $lp
.LMax:
	movi55 MANL,#-1
	movi55 MANH,#-1
	ret5 $lp
	.size	__fixunssfdi, .-__fixunssfdi
#endif /* L_fixunssfdi */



#ifdef L_fixunsdfdi

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RL     $r3
	#define RM     $r4
#else
	#define RI     $r0
	#define RH     $r1
	#define RM     $r3
	#define RL     $r4
#endif
	.text
	.align	2
	.global	__fixunsdfdi
	.type	__fixunsdfdi, @function
__fixunsdfdi:
	pushm   $r6, $r6
#if defined(__NDS32_EXT_FPU_SP)
        fmfdr   $r0, $fd0
#endif
	slli    $r5, RI, #1
	srli    $r5, $r5, #21
	slli    RM, RI, #11
	srli    $r6, RH, #21
	or      RM, RM, $r6
	slli    RL, RH, #11
	move    $r6, #0x80000000
	or      RM, RM, $r6
	slti    $r15, $r5, #0x43e
	beqzs8  .LDnaninf
	subri   $r2, $r5, #0x43e
.LL18:
	move    $r6, #0x20
	slt     $r15, $r2, $r6
	bnezs8  .LL19
	move    RL, RM
	move    RM, #0
	addi    $r2, $r2, #0xffffffe0
	bnez    RL, .LL18
.LL19:
	beqz    $r2, .LL20
	move    RH, RM
	srl     RL, RL, $r2
	srl     RM, RM, $r2
	subri   $r2, $r2, #0x20
	sll     RH, RH, $r2
	or      RL, RL, RH
.LL20:
	sltsi   $r15, RI, #0
	beqzs8  .LDret

	subri   RM, RM, #0
	beqz    RL, .LL21
	subri   RL, RL, #0
	subi45  RM, #1
.LL21:

.LDret:
	move    RH, RL
	move    RI, RM
	popm    $r6, $r6
	ret5    $lp

.LDnaninf:
	move    RM, #0x80000000
	move    RL, #0
	b       .LDret
	.size	__fixunsdfdi, .-__fixunsdfdi
#endif /* L_fixunsdfdi */



#ifdef L_si_to_sf

#define MANTA	$r0
#define EXPOA	$r1

	.text
	.align	2
	.global	__floatsisf
	.type	__floatsisf, @function
__floatsisf:
	beqz	$r0, .LKzero		! A is zero
	move	$r4, #0x80000000
	and	$r2, $r0, $r4		! sign(A)
	beqz	$r2, .LKcont
	subri	$r0, $r0, #0

	! abs(A)
.LKcont:
	move	EXPOA, #0x9e
#ifdef __NDS32_EXT_PERF__
	clz	$r3, $r0
	sll	MANTA, MANTA, $r3
	sub	EXPOA, EXPOA, $r3
#else
	move	$r5, 16
	move	$r3, 0
.LKloop:
	add	$r3, $r3, $r5
	srl	$r15, MANTA, $r3
	bnez	$r15, .LKloop2
	sll	MANTA, MANTA, $r5
	sub	EXPOA, EXPOA, $r5
.LKloop2:
	srli	$r5, $r5, #1
	bnez	$r5, .LKloop
#endif

	! do rounding
	srli	$r4, $r4, #24		! 0x80
	add	MANTA, MANTA, $r4
	slt	$r15, MANTA, $r4
	add	EXPOA, EXPOA, $r15
	srai	$r4, MANTA, #8
	andi	$r4, $r4, #1
	sub	MANTA, MANTA, $r4
	slli	MANTA, MANTA, #1	! shift out implied 1

	! pack
	srli	MANTA, MANTA, #9
	slli	$r4, EXPOA, #23
	or	$r0, MANTA, $r4
	or	$r0, $r0, $r2

.LKzero:
	ret5	$lp
	.size	__floatsisf, .-__floatsisf
#endif /* L_si_to_sf */



#ifdef L_si_to_df

#ifndef __big_endian__
	#define RL     $r1
	#define RM     $r2
	#define RN     $r4
	#define RO	$r5
#else
	#define RM     $r1
	#define RL     $r2
	#define RO     $r4
	#define RN	$r5
#endif
	.text
	.align	2
	.global	__floatsidf
	.type	__floatsidf, @function
__floatsidf:
#ifdef __NDS32_EXT_PERF__
	smw.adm	$r6, [$sp], $r6, 0
#else
	smw.adm	$r6, [$sp], $r6, 2
#endif

	move    RL, #0
	move    RO, RL
	move    $r3, RL
	move    RM, $r0
	beqz    RM, .Li39
	sltsi   $r15, RM, #0
	beqzs8  .Li40
	move    RO, #0x80000000

	subri   RM, RM, #0
	beqz    RL, .LL71
	subri   RL, RL, #0
	subi45  RM, #1
.LL71:
.Li40:
	move    $r3, #0x41e
#ifndef __big_endian__
#ifdef __NDS32_EXT_PERF__
	clz	$r4, $r2
#else
	pushm	$r0, $r3
	push	$r5
	move	$r0, $r2
	bal	__clzsi2
	move	$r4, $r0
	pop	$r5
	popm	$r0, $r3
#endif
#else /* __big_endian__ */
#ifdef __NDS32_EXT_PERF__
	clz	$r5, $r1
#else
	pushm	$r0, $r4
	move	$r0, $r1
	bal	__clzsi2
	move	$r5, $r0
	popm	$r0, $r4
#endif
#endif /* __big_endian__ */
	sub     $r3, $r3, RN
	sll     RM, RM, RN
.Li39:
	srli    RN, RL, #11
	slli    $r6, RM, #21
	or      RN, RN, $r6
	slli    $r6, RM, #1
	srli    $r6, $r6, #12
	or      RO, RO, $r6
	slli    $r6, $r3, #20
	or      RO, RO, $r6
	move    $r0, $r4
	move    $r1, $r5

#ifdef __NDS32_EXT_PERF__
	lmw.bim	$r6, [$sp], $r6, 0
#else
	lmw.bim	$r6, [$sp], $r6, 2
#endif
	ret5    $lp
	.size __floatsidf, .-__floatsidf
#endif /* L_si_to_df */



#ifdef L_floatdisf

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RJ     $r2
	#define RK     $r3
#else
	#define RI     $r0
	#define RH     $r1
	#define RK     $r2
	#define RJ     $r3
#endif
	.text
	.align	2
	.global	__floatdisf
	.type	__floatdisf, @function
__floatdisf:
#ifdef __NDS32_EXT_PERF__
	smw.adm	$r6, [$sp], $r7, 0
#else
	smw.adm	$r6, [$sp], $r7, 2
#endif

	move    $r7, #0x80000000
	and     $r5, RI, $r7
	move    RK, RI
	move    RJ, RH
	or      $r7, RI, RH
	beqz    $r7, .Li1
	sltsi   $r15, RI, #0
	beqzs8  .Li2

	subri   RK, RK, #0
	beqz    RJ, .LL1
	subri   RJ, RJ, #0
	subi45  RK, #1
.LL1:
.Li2:
	move    $r4, #0xbe


	#NORMd($r2, $r6, RH)
	bnez    RK, .LL2
	bnez    RJ, .LL3
	move    $r4, #0
	b       .LL4
.LL3:
	move    RK, RJ
	move    RJ, #0
	move    $r6, #32
	sub     $r4, $r4, $r6
.LL2:
#ifdef __NDS32_EXT_PERF__
	clz	$r6, RK
#else
	pushm	$r0, $r5
	move	$r0, RK
	bal	__clzsi2
	move	$r6, $r0
	popm	$r0, $r5
#endif
	beqz    $r6, .LL4
	sub     $r4, $r4, $r6
	subri   RH, $r6, #32
	srl     RH, RJ, RH
	sll     RJ, RJ, $r6
	sll     RK, RK, $r6
	or      RK, RK, RH
.LL4:
	#NORMd End

	beqz    RJ, .Li3
	ori     RK, RK, #1
.Li3:
	#ADD(RK, $0x80)
	move    $r15, #0x80
	add     RK, RK, $r15
	slt     $r15, RK, $r15

	#ADDC($r4, $0x0)
	add     $r4, $r4, $r15
	srli    $r7, RK, #8
	andi    $r7, $r7, #1
	sub     RK, RK, $r7
	slli    RK, RK, #1
	srli    RK, RK, #9
	slli    $r7, $r4, #23
	or      RK, RK, $r7
.Li1:
	or      $r0, RK, $r5

.LA999:
#ifdef __NDS32_EXT_PERF__
	lmw.bim	$r6, [$sp], $r7, 0
#else
	lmw.bim	$r6, [$sp], $r7, 2
#endif
	ret5    $lp
	.size	__floatdisf, .-__floatdisf
#endif /* L_floatdisf */



#ifdef L_floatdidf

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RJ     $r2
	#define RK     $r3
	#define RL     $r5
	#define RM     $r6
#else
	#define RI     $r0
	#define RH     $r1
	#define RK     $r2
	#define RJ     $r3
	#define RM     $r5
	#define RL     $r6
#endif
	.text
	.align	2
	.global	__floatdidf
	.type	__floatdidf, @function
__floatdidf:
#ifdef __NDS32_EXT_PERF__
	smw.adm	$r6, [$sp], $r8, 0
#else
	smw.adm	$r6, [$sp], $r8, 2
#endif

	move    $r4, #0
	move    $r7, $r4
	move    RK, RI
	move    RJ, RH
	or      $r8, RI, RH
	beqz    $r8, .Li1
	move    $r4, #0x43e
	sltsi   $r15, RI, #0
	beqzs8  .Li2
	move    $r7, #0x80000000

	subri   RK, RK, #0
	beqz    RJ, .LL1
	subri   RJ, RJ, #0
	subi45  RK, #1
.LL1:

.Li2:
	#NORMd($r2, RM, RL)
	bnez    RK, .LL2
	bnez    RJ, .LL3
	move    $r4, #0
	b       .LL4
.LL3:
	move    RK, RJ
	move    RJ, #0
	move    RM, #32
	sub     $r4, $r4, RM
.LL2:
#ifdef __NDS32_EXT_PERF__
	clz	RM, RK
#else /* not __NDS32_EXT_PERF__ */
/*
  Replace clz with function call.
	clz     RM, RK
  EL:	clz     $r6, $r3
  EB:	clz	$r5, $r2
*/
#ifndef __big_endian__
	pushm	$r0, $r5
	move	$r0, $r3
	bal	__clzsi2
	move	$r6, $r0
	popm	$r0, $r5
#else
	pushm	$r0, $r4
	move	$r0, $r2
	bal	__clzsi2
	move	$r5, $r0
	popm	$r0, $r4
#endif
#endif /* not __NDS32_EXT_PERF__ */
	beqz    RM, .LL4
	sub     $r4, $r4, RM
	subri   RL, RM, #32
	srl     RL, RJ, RL
	sll     RJ, RJ, RM
	sll     RK, RK, RM
	or      RK, RK, RL
.LL4:
	#NORMd End

	#ADD(RJ, $0x400)
	move    $r15, #0x400
	add     RJ, RJ, $r15
	slt     $r15, RJ, $r15


	#ADDCC(RK, $0x0)
	beqzs8  .LL7
	add     RK, RK, $r15
	slt     $r15, RK, $r15
.LL7:

	#ADDC($r4, $0x0)
	add     $r4, $r4, $r15
	srli    $r8, RJ, #11
	andi    $r8, $r8, #1
	sub     RJ, RJ, $r8
.Li1:
	srli    RL, RJ, #11
	slli    $r8, RK, #21
	or      RL, RL, $r8
	slli    RM, RK, #1
	srli    RM, RM, #12
	slli    $r8, $r4, #20
	or      RM, RM, $r8
	or      RM, RM, $r7
	move    RH, RL
	move    RI, RM

.LA999:
#ifdef __NDS32_EXT_PERF__
	lmw.bim	$r6, [$sp], $r8, 0
#else
	lmw.bim	$r6, [$sp], $r8, 2
#endif
	ret5    $lp
	.size	__floatdidf, .-__floatdidf
#endif /* L_floatdidf */



#ifdef L_floatunsisf

#define MANTA	$r0
#define EXPOA	$r1

	.text
	.align	2
	.global	__floatunsisf
	.type	__floatunsisf, @function
__floatunsisf:
	beqz	$r0, .LKzero		! A is zero

	move	EXPOA, #0x9e
#ifdef __NDS32_EXT_PERF__
	clz	$r5, $r0
	sll	MANTA, MANTA, $r5
	sub	EXPOA, EXPOA, $r5
#else
	move	$r5, 16
	move	$r3, 0
.LKloop:
	add	$r3, $r3, $r5
	srl	$r15, MANTA, $r3
	bnez	$r15, .LKloop2
	sll	MANTA, MANTA, $r5
	sub	EXPOA, EXPOA, $r5
.LKloop2:
	srli	$r5, $r5, #1
	bnez	$r5, .LKloop
#endif

	! do rounding
	addi	MANTA, MANTA, #128
	slti	$r15, MANTA, #128
	add	EXPOA, EXPOA, $r15
	srli	$r4, MANTA, #8
	andi	$r4, $r4, #1
	sub	MANTA, MANTA, $r4
	slli	MANTA, MANTA, #1	! shift out implied 1

	! pack
	srli	MANTA, MANTA, #9
	slli	$r4, EXPOA, #23
	or	$r0, MANTA, $r4

.LKzero:
	ret5	$lp
	.size	__floatunsisf, .-__floatunsisf
#endif /* L_floatunsisf */



#ifdef L_floatunsidf

#ifndef __big_endian__
	#define RL     $r1
	#define RM     $r2
	#define RN     $r4
	#define RO	$r5
#else
	#define RM     $r1
	#define RL     $r2
	#define RO     $r4
	#define RN	$r5
#endif
	.text
	.align	2
	.global	__floatunsidf
	.type	__floatunsidf, @function
__floatunsidf:
#ifdef __NDS32_EXT_PERF__
	smw.adm	$r6, [$sp], $r6, 0
#else
	smw.adm	$r6, [$sp], $r6, 2
#endif

	move    RL, #0
	move    $r3, RL
	move    RM, $r0
	beqz    RM, .Li41
	move    $r3, #0x41e
#ifndef __big_endian__
#ifdef __NDS32_EXT_PERF__
	clz	$r5, $r2
#else
	pushm	$r0, $r4
	move	$r0, $r2
	bal	__clzsi2
	move	$r5, $r0
	popm	$r0, $r4
#endif
#else /* __big_endian__ */
#ifdef __NDS32_EXT_PERF__
	clz	$r4, $r1
#else
	pushm	$r0, $r3
	push	$r5
	move	$r0, $r1
	bal	__clzsi2
	move	$r4, $r0
	pop	$r5
	popm	$r0, $r3
#endif
#endif /* __big_endian__ */
	sub     $r3, $r3, RO
	sll     RM, RM, RO
.Li41:
	srli    RN, RL, #11
	slli    $r6, RM, #21
	or      RN, RN, $r6
	slli    RO, RM, #1
	srli    RO, RO, #12
	slli    $r6, $r3, #20
	or      RO, RO, $r6
	move    $r0, $r4
	move    $r1, $r5

#ifdef __NDS32_EXT_PERF__
	lmw.bim	$r6, [$sp], $r6, 0
#else
	lmw.bim	$r6, [$sp], $r6, 2
#endif
	ret5    $lp
	.size __floatunsidf, .-__floatunsidf
#endif /* L_floatunsidf */



#ifdef L_floatundisf

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RJ     $r2
	#define RK     $r3
#else
	#define RI     $r0
	#define RH     $r1
	#define RK     $r2
	#define RJ     $r3
#endif
	.text
	.align	2
	.global	__floatundisf
	.type	__floatundisf, @function
__floatundisf:
#ifdef __NDS32_EXT_PERF__
	smw.adm	$r6, [$sp], $r6, 0
#else
	smw.adm	$r6, [$sp], $r6, 2
#endif

	move    RK, RI
	move    RJ, RH
	or      $r6, RI, RH
	beqz    $r6, .Li4
	move    $r4, #0xbe


	#NORMd($r2, $r5, RH)
	bnez    RK, .LL5
	bnez    RJ, .LL6
	move    $r4, #0
	b       .LL7
.LL6:
	move    RK, RJ
	move    RJ, #0
	move    $r5, #32
	sub     $r4, $r4, $r5
.LL5:
#ifdef __NDS32_EXT_PERF__
	clz	$r5, RK
#else
	pushm	$r0, $r4
	move	$r0, RK
	bal	__clzsi2
	move	$r5, $r0
	popm	$r0, $r4
#endif
	beqz    $r5, .LL7
	sub     $r4, $r4, $r5
	subri   RH, $r5, #32
	srl     RH, RJ, RH
	sll     RJ, RJ, $r5
	sll     RK, RK, $r5
	or      RK, RK, RH
.LL7:
	#NORMd End

	beqz    RJ, .Li5
	ori     RK, RK, #1
.Li5:
	#ADD(RK, $0x80)
	move    $r15, #0x80
	add     RK, RK, $r15
	slt     $r15, RK, $r15

	#ADDC($r4, $0x0)
	add     $r4, $r4, $r15
	srli    $r6, RK, #8
	andi    $r6, $r6, #1
	sub     RK, RK, $r6
	slli    RK, RK, #1
	srli    RK, RK, #9
	slli    $r6, $r4, #23
	or      RK, RK, $r6
.Li4:
	move    $r0, RK

.LB999:
#ifdef __NDS32_EXT_PERF__
	lmw.bim	$r6, [$sp], $r6, 0
#else
	lmw.bim	$r6, [$sp], $r6, 2
#endif
	ret5    $lp
	.size	__floatundisf, .-__floatundisf
#endif /* L_floatundisf */



#ifdef L_floatundidf

#ifndef __big_endian__
	#define RH     $r0
	#define RI     $r1
	#define RJ     $r2
	#define RK     $r3
	#define RL     $r5
	#define RM     $r6
#else
	#define RI     $r0
	#define RH     $r1
	#define RK     $r2
	#define RJ     $r3
	#define RM     $r5
	#define RL     $r6
#endif
	.text
	.align	2
	.global	__floatundidf
	.type	__floatundidf, @function
__floatundidf:
#ifdef __NDS32_EXT_PERF__
	smw.adm	$r6, [$sp], $r7, 0
#else
	smw.adm	$r6, [$sp], $r7, 2
#endif

	move    $r4, #0
	move    RK, RI
	move    RJ, RH
	or      $r7, RI, RH
	beqz    $r7, .Li3
	move    $r4, #0x43e


	#NORMd($r2, RM, RL)
	bnez    RK, .LL8
	bnez    RJ, .LL9
	move    $r4, #0
	b       .LL10
.LL9:
	move    RK, RJ
	move    RJ, #0
	move    RM, #32
	sub     $r4, $r4, RM
.LL8:
#ifdef __NDS32_EXT_PERF__
	clz	RM, RK
#else /* not __NDS32_EXT_PERF__ */
/*
  Replace clz with function call.
	clz     RM, RK
  EL:	clz     $r6, $r3
  EB:	clz	$r5, $r2
*/
#ifndef __big_endian__
	pushm	$r0, $r5
	move	$r0, $r3
	bal	__clzsi2
	move	$r6, $r0
	popm	$r0, $r5
#else
	pushm	$r0, $r4
	move	$r0, $r2
	bal	__clzsi2
	move	$r5, $r0
	popm	$r0, $r4
#endif
#endif /* not __NDS32_EXT_PERF__ */
	beqz    RM, .LL10
	sub     $r4, $r4, RM
	subri   RL, RM, #32
	srl     RL, RJ, RL
	sll     RJ, RJ, RM
	sll     RK, RK, RM
	or      RK, RK, RL
.LL10:
	#NORMd End

	#ADD(RJ, $0x400)
	move    $r15, #0x400
	add     RJ, RJ, $r15
	slt     $r15, RJ, $r15


	#ADDCC(RK, $0x0)
	beqzs8  .LL13
	add     RK, RK, $r15
	slt     $r15, RK, $r15
.LL13:

	#ADDC($r4, $0x0)
	add     $r4, $r4, $r15
	srli    $r7, RJ, #11
	andi    $r7, $r7, #1
	sub     RJ, RJ, $r7
.Li3:
	srli    RL, RJ, #11
	slli    $r7, RK, #21
	or      RL, RL, $r7
	slli    RM, RK, #1
	srli    RM, RM, #12
	slli    $r7, $r4, #20
	or      RM, RM, $r7
	move    RH, RL
	move    RI, RM

.LB999:
#ifdef __NDS32_EXT_PERF__
	lmw.bim	$r6, [$sp], $r7, 0
#else
	lmw.bim	$r6, [$sp], $r7, 2
#endif
	ret5    $lp
	.size	__floatundidf, .-__floatundidf
#endif /* L_floatundidf */



#ifdef L_compare_sf

	.text
	.align	2
	.global	__gtsf2
	.type	__gtsf2, @function
__gtsf2:
	! ---------------------------------------------------------------------
	! int __gtsf2(float a, float b):
	! This function returns a value greater than zero if neither argument
	! is NaN and a is strictly greater than b.
	! ---------------------------------------------------------------------
	.global	__gesf2
	.type	__gesf2, @function
__gesf2:
	! ---------------------------------------------------------------------
	! int __gesf2(float a, float b):
	! This function returns a value greater than or equal to zero if
	! neither argument is NaN and a is greater than or equal to b.
	! ---------------------------------------------------------------------
	move	$r4, #-1
	b	.LA

	.global	__eqsf2
	.type	__eqsf2, @function
__eqsf2:
	! ---------------------------------------------------------------------
	! int __eqsf2(float a, float b):
	! This function returns zero value if neither argument is NaN,
	! and a and b are equal.
	! ---------------------------------------------------------------------
	.global	__nesf2
	.type	__nesf2, @function
__nesf2:
	! ---------------------------------------------------------------------
	! int __nesf2(float a, float b):
	! This function returns a nonzero value if either argument is NaN or if
	! a and b are unequal.
	! ---------------------------------------------------------------------
	.global	__lesf2
	.type	__lesf2, @function
__lesf2:
	! ---------------------------------------------------------------------
	! int __lesf2(float a, float b):
	! This function returns a value less than or equal to zero if neither
	! argument is NaN and a is less than b.
	! ---------------------------------------------------------------------
	.global	__ltsf2
	.type	__ltsf2, @function
__ltsf2:
	! ---------------------------------------------------------------------
	! int __ltsf2(float a, float b):
	! This function returns a value less than zero if neither argument is
	! NaN and a is strictly less than b.
	! ---------------------------------------------------------------------
	.global	__cmpsf2
	.type	__cmpsf2, @function
__cmpsf2:
	! ---------------------------------------------------------------------
	! int __cmpsf2(float a, float b);
	! This function calculates a <=> b. That is, if a is less than b, it
	! returns -1; if a if greater than b, it returns 1; and if a and b are
	! equal, it returns 0. If either argument is NaN, it returns 1, But you
	! should not rely on this; If NaN is a possibility, use higher-level
	! comparison function __unordsf2().
	! ---------------------------------------------------------------------
	move    $r4, #1

	.align	2
.LA:
#ifndef __FAST_MATH__
	move    $r5, #0xff000000
	slli	$r2, $r0, #1
	slt	$r15, $r5, $r2
	bnez    $r15, .LMnan		! a is NaN
	slli	$r3, $r1, #1
	slt	$r15, $r5, $r3
	bnez    $r15, .LMnan		! b is NaN
#endif
	xor     $r5, $r0, $r1	        ! a and b have same sign?
	bgez    $r5, .LSameSign

.LDiffSign:
#ifdef __FAST_MATH__
	slli    $r2, $r0, #1
	slli    $r3, $r1, #1
#endif
	or      $r2, $r2, $r3
	beqz	$r2, .LMequ		! 0.0f and -0.0f are equal
	move    $r2, #1			! when a==0.0f, return 1
	cmovz   $r0, $r2, $r0		! otherwise, simply return a
	ret5    $lp

.LSameSign:
	sltsi   $r15, $r0, 0		! a < 0 ?
	bnez	$r15, .LSameSignNeg
.LSameSignPos:
	! a >= 0 && b >= 0, return a - b
	sub     $r0, $r0, $r1
	ret5    $lp
.LSameSignNeg:
	! a < 0 && b < 0, return b - a
	sub     $r0, $r1, $r0
	ret5    $lp

.LMequ:
	move    $r0, #0
	ret5    $lp

#ifndef __FAST_MATH__
.LMnan:
	move    $r0, $r4
	ret5    $lp
#endif
	.size   __cmpsf2, .-__cmpsf2
	.size   __ltsf2, .-__ltsf2
	.size   __lesf2, .-__lesf2
	.size   __nesf2, .-__nesf2
	.size   __eqsf2, .-__eqsf2
	.size   __gesf2, .-__gesf2
	.size   __gtsf2, .-__gtsf2
#endif /* L_compare_sf */



#ifdef L_compare_df

#ifdef __big_endian__
	#define RI     $r0
	#define RH     $r1
	#define RK     $r2
	#define RJ     $r3
#else
	#define RI     $r1
	#define RH     $r0
	#define RK     $r3
	#define RJ     $r2
#endif
#define RB	$r5
#define RA	$r4
#ifdef __NDS32_REDUCE_REGS__
	#define RC	$r6
	#define RD	$r7
	#define RE	$r8
	#define RF	$r9
#else
	#define RC	$r16
	#define RD	$r17
	#define RE	$r18
	#define RF	$r19
#endif

	.text
	.align	2
	.globl	__gtdf2
	.type	__gtdf2, @function
__gtdf2:
	! ---------------------------------------------------------------------
	! int __gtdf2(double a, double b):
	! This function returns a value greater than zero if neither argument
	! is NaN and a is strictly greater than b.
	! ---------------------------------------------------------------------
	.globl	__gedf2
	.type	__gedf2, @function
__gedf2:
	! ---------------------------------------------------------------------
	! int __gedf2(double a, double b):
	! This function returns a value greater than or equal to zero if
	! neither argument is NaN and a is greater than or equal to b.
	! ---------------------------------------------------------------------
	move	$r4, #-1
	b	.LA

	.globl	__eqdf2
	.type	__eqdf2, @function
__eqdf2:
	! ---------------------------------------------------------------------
	! int __eqdf2(double a, double b):
	! This function returns zero value if neither argument is NaN and and b
	! are equal.
	! ---------------------------------------------------------------------
	.globl	__nedf2
	.type	__nedf2, @function
__nedf2:
	! ---------------------------------------------------------------------
	! int __nedf2(double a, double b):
	! This function returns a nonzero value if either argument is NaN or if
	! a and b are unequal.
	! ---------------------------------------------------------------------
	.globl	__ledf2
	.type	__ledf2, @function
__ledf2:
	! ---------------------------------------------------------------------
	! int __ledf2(double a, double b):
	! This function returns a value less than or equal to zero if neither
	! argument is NaN and a is less than b.
	! ---------------------------------------------------------------------
	.globl	__ltdf2
	.type	__ltdf2, @function
__ltdf2:
	! ---------------------------------------------------------------------
	! int __ltdf2(double a, double b):
	! This function returns a value less than zero if neither argument is
	! NaN and a is strictly less than b.
	! ---------------------------------------------------------------------
	.globl	__cmpdf2
	.type	__cmpdf2, @function
__cmpdf2:
	! ---------------------------------------------------------------------
	! int __cmpdf2(double a, double b);
	! This function calculates a <=> b. That is, if a is less than b, it
	! returns -1; if a if greater than b, it returns 1; and if a and b are
	! equal, it returns 0. If either argument is NaN, it returns 1, But you
	! should not rely on this; If NaN is a possibility, use higher-level
	! comparison function __unordsf2().
	! ---------------------------------------------------------------------
	move	$r4, #1

.LA:
	move	RB, #0
#ifdef __NDS32_REDUCE_REGS__
	smw.adm	$r6, [$sp], $r9, 0
#endif

	move	RE, #0xffe00000
	slli	RC, RI, #1
	slt	$r15, RB, RH
	add	RF, RC, $r15
	slt	$r15, RE, RF
	bnez	$r15, .LMnan		! a is NaN
	slli	RD, RK, #1
	slt	$r15, RB, RJ
	add	RF, RD, $r15
	slt	$r15, RE, RF
	bnez	$r15, .LMnan		! b is NaN
	xor	RA, RI, RK
	bltz    RA, .LMdiff		! a and b same sign?

	! same sign
	sltsi	$r15, RI, 0		! a<0?
	bnez	$r15, .LMsame

	sub	RA, RH, RJ
	slt	$r15, RH, RA
	sub	$r0, RI, RK
	sub	$r0, $r0, $r15		! return a-b
	slt	$r15, RB, RA
	cmovz	$r0, $r15, $r0
#ifdef __NDS32_REDUCE_REGS__
	b	.LMret
#else
	ret5	$lp
#endif

.LMsame:
	sub	RA, RJ, RH
	slt	$r15, RJ, RA
	sub	$r0, RK, RI
	sub	$r0, $r0, $r15		! return b-a
	slt	$r15, RB, RA
	cmovz	$r0, $r15, $r0
#ifdef __NDS32_REDUCE_REGS__
	b	.LMret
#else
	ret5	$lp

	.align	2
#endif
	! different sign
.LMdiff:
#ifdef __NDS32_REDUCE_REGS__
	or	RC, RC, RD		! 0.0f and -0.0f are equal
#else
	or	RB, RC, RD		! 0.0f and -0.0f are equal
#endif
	or	RA, RH, RJ
#ifdef __NDS32_REDUCE_REGS__
	or	RC, RC, RA
#else
	or	RB, RB, RA
#endif
#ifdef __big_endian__
#ifdef __NDS32_REDUCE_REGS__
	beqz	RC, .LMequ
#else
	beqz	RB, .LMequ
#endif

	movi	$r2, #1			! when high-part(a) is 0, return 1
	cmovz	$r0, $r2, RI		! otherwise, simply return high-part(a)
#else
#ifdef __NDS32_REDUCE_REGS__
	beqz	RC, .LMret
#else
	beqz	RB, .LMret
#endif

	movi	$r0, #1			! when high-part(a) is 0, return 1
	cmovn	$r0, RI, RI		! otherwise, simply return high-part(a)
#endif

.LMret:
#ifdef __NDS32_REDUCE_REGS__
	lmw.bim	$r6, [$sp], $r9, 0
#endif
	ret5	$lp

#ifdef __big_endian__
.LMequ:
	movi	$r0, #0
#ifdef __NDS32_REDUCE_REGS__
	b	.LMret
#else
	ret5	$lp
#endif
#endif

.LMnan:
	move	$r0, $r4
#ifdef __NDS32_REDUCE_REGS__
	b	.LMret
#else
	ret5	$lp
#endif
	.size	__cmpdf2, .-__cmpdf2
	.size	__ltdf2, .-__ltdf2
	.size	__ledf2, .-__ledf2
	.size	__nedf2, .-__nedf2
	.size	__eqdf2, .-__eqdf2
	.size	__gedf2, .-__gedf2
	.size	__gtdf2, .-__gtdf2
#endif /* L_compare_df */



#ifdef L_unord_sf

	.text
	.align	2
	.global	__unordsf2
	.type	__unordsf2, @function
__unordsf2:
	! ---------------------------------------------------------------------
	! int __unordsf2(float a, float b):
	! This function returns 1 if either argument is NaN, otherwise 0.
	! ---------------------------------------------------------------------
	! Is a NaN?
	slli	$r0, $r0, #1
	move	$r3, #0xff000000
	slt	$r0, $r3, $r0
	bnez	$r0, .Li67
	! a is not NaN. Is b NaN?
	slli	$r1, $r1, #1
	slt	$r0, $r3, $r1

.Li67:
	ret5	$lp
	.size	__unordsf2, .-__unordsf2
#endif /* L_unord_sf */



#ifdef L_unord_df

#ifdef __big_endian__
	#define RI	$r0
	#define RH	$r1
	#define RK	$r2
	#define RJ	$r3
#else
	#define RI	$r1
	#define RH	$r0
	#define RK	$r3
	#define RJ	$r2
#endif
#define RB	$r5
#define RA	$r4

	.text
	.align	2
	.global	__unorddf2
	.type	__unorddf2, @function
__unorddf2:
	! ---------------------------------------------------------------------
	! int __unorddf2(double a, double b):
	! This function returns 1 if either argument is NaN, otherwise 0. 
	! ---------------------------------------------------------------------
	! Is a NaN?
	slli	RI, RI, #1
	move	RA, #0
	slt	$r15, RA, RH
	add	$r0, RI, $r15
	move	RB, #0xffe00000
	slt	$r0, RB, $r0
	bnez	$r0, .Li69		! it is NaN

	! a is not NaN. Is b NaN?
	slli	RK, RK, #1
	slt	$r15, RA, RJ
	add	RK, RK, $r15
	slt	$r0, RB, RK

.Li69:
	ret5	$lp
	.size	__unorddf2, .-__unorddf2
#endif /* L_unord_df */
/* ------------------------------------------- */
/* DPBIT floating point operations for libgcc  */
/* ------------------------------------------- */
