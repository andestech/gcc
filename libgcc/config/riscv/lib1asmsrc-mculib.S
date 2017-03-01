#ifdef L_addsub_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__subsf3
	.type	__subsf3,@function
__subsf3:



	li	x12, 2147483648
	xor	x11, x11, x12


	.globl	__addsf3
	.type	__addsf3,@function
__addsf3:
	slli	x12, x10, 1




	slli	x13, x11, 1
	li	x6, 2147483648
	sltu	tp, x12, x13
	beq	tp, zero, .LEcont
	addi	x15, x10, 0
	addi	x10, x11, 0
	addi	x11, x15, 0
	slli	x12, x10, 1
	slli	x13, x11, 1

.LEcont:
	xor	x11, x11, x10
	and	x11, x11, x6
	srli	x14, x12, 24
	srli	x7, x13, 24
	slli	x5, x12, 7
	slli	x28, x13, 7

	li	x17, 255
	beq	x14, x17, .LEinfnan




	beq	x12, zero, .LEzeroP
	beq	x13, zero, .LEretA
	sub	x15, x14, x7
	sltiu	tp, x14, 2
	bne	tp, zero, .LElab2
	slti	tp, x15, 32
	beq	tp, zero, .LEretA
	or	x5, x5, x6
	beq	x7, zero, .LElab1
	or	x28, x28, x6

.LElab1:
	addi	tp, x15, -1
	bne	x7, zero, .Ltmp0
	addi	x15, tp, 0
.Ltmp0:
	addi	tp, x28, 0
	srl	x28, x28, x15
	sll	x15, x28, x15
	beq	x15, tp, .LElab2
	ori	x28, x28, 2

.LElab2:
	bne	x11, zero, .LEsub

	add	x5, x5, x28
	sltu	tp, x5, x28
	beq	tp, zero, .LEaddnoover

	li	x17, 254
	beq	x14, x17, .LEinf




	andi	tp, x5, 1
	ori	x15, x5, 2
	beq	tp, zero, .Ltmp1
	addi	x5, x15, 0
.Ltmp1:
	srli	x5, x5, 1
	addi	x14, x14, 1
	la	x17, .LEround
	jalr	zero, x17, 0

.LEaddnoover:
	bne	x14, zero, .LEround

.LEdenorm:
	srli	x5, x5, 8
	la	x17, .LEpack
	jalr	zero, x17, 0

.LEsub:
	beq	x12, x13, .LEzero
	sltu	tp, x5, x28
	beq	tp, zero, .LEsub2
	srli	x28, x28, 1
	addi	x14, x14, -1

.LEsub2:
	sub	x5, x5, x28
	sltiu	tp, x14, 2
	bne	tp, zero, .LEdenorm

	la	x17, .LEloopC2
	jalr	zero, x17, 0

.LEloopC:
	addi	x14, x14, -1
	beq	x14, zero, .LEround
	add	x5, x5, x5

.LEloopC2:
	sltu	tp, x5, x6
	bne	tp, zero, .LEloopC


.LEround:
	addi	x5, x5, 128
	sltiu	tp, x5, 128
	add	x14, x14, tp
	srli	x15, x5, 8
	andi	x15, x15, 1
	sub	x5, x5, x15

	slli	x5, x5, 1
	srli	x5, x5, 9
	slli	x11, x14, 23
	or	x5, x5, x11
.LEpack:
	and	x10, x10, x6
	or	x10, x10, x5

.LEretA:
.LEret:



	jalr	zero, ra, 0

.LEzeroP:
	beq	x11, zero, .LEretA

.LEzero:
	li	x10, 0
	la	x17, .LEret
	jalr	zero, x17, 0

.LEinfnan:
	bne	x5, x6, .LEnan

	li	x17, 255
	bne	x7, x17, .LEretA




	beq	x11, zero, .LEretA

.LEnan:
	li	x10, 4290772992
	la	x17, .LEret
	jalr	zero, x17, 0

.LEinf:
	li	x5, 2139095040
	la	x17, .LEpack
	jalr	zero, x17, 0
.Ltmp2:
	.size	__subsf3, .Ltmp2-__subsf3
.Ltmp3:
	.size	__addsf3, .Ltmp3-__addsf3
#endif
#ifdef L_divsi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__divsi3
	.type	__divsi3,@function
__divsi3:
	slti	x15, x10, 0
	sub	x14, zero, x10
	beq	x15, zero, .Ltmp0
	addi	x10, x14, 0
.Ltmp0:
.L2:
	bge	x11, zero, .L3
	sub	x11, zero, x11
	li	x17, 1
	sub	x15, x17, x15
.L3:
	li	x12, 0
	beq	x11, zero, .L1
	li	x14, 1

.L6:

	sltu	tp, x11, x10
	beq	tp, zero, .L5







	slli	x11, x11, 1
	slli	x14, x14, 1
	la	x17, .L6
	jalr	zero, x17, 0

.L5:
	sltu	tp, x10, x11
	bne	tp, zero, .L9
	sub	x10, x10, x11
	or	x12, x12, x14
.L9:
	srli	x14, x14, 1
	srli	x11, x11, 1
	bne	x14, zero, .L5
.L1:
	sub	x10, zero, x12
	bne	x15, zero, .Ltmp1
	addi	x10, x12, 0
.Ltmp1:
	jalr	zero, ra, 0
.Ltmp2:
	.size	__divsi3, .Ltmp2-__divsi3
#endif
#ifdef L_divdi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__divdi3
	.type	__divdi3,@function
__divdi3:
	sw	x9, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -8

	xor	x9, x11, x13
	srai	x9, x9, 31
	bge	x13, zero, .L80
	sub	x13, zero, x13
	beq	x12, zero, .L80
	sub	x12, zero, x12
	addi	x13, x13, -1

.L80:
	bge	x11, zero, .L81
	sub	x11, zero, x11
	beq	x10, zero, .L81
	sub	x10, zero, x10
	addi	x11, x11, -1

.L81:
	li	x14, 0
	la	x17, __udivmoddi4
	jalr	ra, x17, 0
	beq	x9, zero, .L82
	or	x14, x11, x10
	beq	x14, zero, .L82
	sub	x11, zero, x11
	beq	x10, zero, .L82
	sub	x10, zero, x10
	addi	x11, x11, -1

	.p2align	2
.L82:
	lw	x9, 0(sp)
	lw	ra, 4(sp)
	addi	sp, sp, 8
	jalr	zero, ra, 0
.Ltmp0:
	.size	__divdi3, .Ltmp0-__divdi3
#endif
#ifdef L_modsi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__modsi3
	.type	__modsi3,@function
__modsi3:
	slti	x15, x10, 0
	sub	x14, zero, x10
	beq	x15, zero, .Ltmp0
	addi	x10, x14, 0
.Ltmp0:

	bge	x11, zero, .L3
	sub	x11, zero, x11
.L3:







	beq	x11, zero, .L1
	li	x14, 1

.L6:

	sltu	tp, x11, x10
	beq	tp, zero, .L5







	slli	x11, x11, 1
	slli	x14, x14, 1
	la	x17, .L6
	jalr	zero, x17, 0

.L5:
	sub	x12, x10, x11
	sltu	tp, x10, x11
	srli	x14, x14, 1
	bne	tp, zero, .Ltmp1
	addi	x10, x12, 0
.Ltmp1:
	srli	x11, x11, 1
	bne	x14, zero, .L5
.L1:
	sub	x13, zero, x10
	beq	x15, zero, .Ltmp2
	addi	x10, x13, 0
.Ltmp2:
	jalr	zero, ra, 0
.Ltmp3:
	.size	__modsi3, .Ltmp3-__modsi3
#endif
#ifdef L_moddi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__moddi3
	.type	__moddi3,@function
__moddi3:
	addi	sp, sp, -16
	sw	x9, 0(sp)
	sw	ra, 4(sp)

	srai	x9, x11, 31
	bge	x13, zero, .L80
	sub	x13, zero, x13
	beq	x12, zero, .L80
	sub	x12, zero, x12
	addi	x13, x13, -1

.L80:
	beq	x9, zero, .L81
	sub	x11, zero, x11
	beq	x10, zero, .L81
	sub	x10, zero, x10
	addi	x11, x11, -1

.L81:
	addi	x14, sp, 8
	la	x17, __udivmoddi4
	jalr	ra, x17, 0
	li	x17, 8
	add	x17, sp, x17
	lw	x10, 0(x17)
	li	x17, 12
	add	x17, sp, x17
	lw	x11, 0(x17)
	beq	x9, zero, .L82
	or	x14, x11, x10
	beq	x14, zero, .L82
	sub	x11, zero, x11
	beq	x10, zero, .L82
	sub	x10, zero, x10
	addi	x11, x11, -1

	.p2align	2
.L82:
	lw	x9, 0(sp)
	lw	ra, 4(sp)
	addi	sp, sp, 16
	jalr	zero, ra, 0
.Ltmp0:
	.size	__moddi3, .Ltmp0-__moddi3
#endif
#ifdef L_mulsi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__mulsi3
	.type	__mulsi3,@function
__mulsi3:
	beq	x10, zero, .L7
	addi	x12, x10, 0
	li	x10, 0
.L8:
	andi	x13, x12, 1
	add	x14, x10, x11
	beq	x13, zero, .Ltmp0
	addi	x10, x14, 0
.Ltmp0:
	srli	x12, x12, 1
	slli	x11, x11, 1
	bne	x12, zero, .L8
.L7:
	jalr	zero, ra, 0
.Ltmp1:
	.size	__mulsi3, .Ltmp1-__mulsi3
#endif
#ifdef L_udivsi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__udivsi3
	.type	__udivsi3,@function
__udivsi3:
	li	x12, 0
	beq	x11, zero, .L1
	li	x14, 1

.L6:

	sltu	tp, x11, x10
	beq	tp, zero, .L5
	blt	x11, zero, .L5







	slli	x11, x11, 1
	slli	x14, x14, 1
	la	x17, .L6
	jalr	zero, x17, 0

.L5:
	sltu	tp, x10, x11
	bne	tp, zero, .L9
	sub	x10, x10, x11
	or	x12, x12, x14
.L9:
	srli	x14, x14, 1
	srli	x11, x11, 1
	bne	x14, zero, .L5
.L1:
	addi	x10, x12, 0
	jalr	zero, ra, 0
.Ltmp0:
	.size	__udivsi3, .Ltmp0-__udivsi3
#endif
#ifdef L_udivdi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__udivdi3
	.type	__udivdi3,@function
__udivdi3:
	li	x14, 0
	la	x17, __udivmoddi4
	jalr	zero, x17, 0
.Ltmp0:
	.size	__udivdi3, .Ltmp0-__udivdi3
#endif
#ifdef L_umul_ppmm
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	umul_ppmm
	.type	umul_ppmm,@function
umul_ppmm:
	slli	x17, x10, 16
	srli	x12, x17, 16
	srli	x13, x10, 16




	slli	x17, x11, 16
	srli	x10, x17, 16
	srli	x11, x11, 16

	mul	x15, x12, x11
	mul	x12, x12, x10
	mul	x10, x13, x10
	add	x15, x15, x10
	sltu	tp, x15, x10
	slli	tp, tp, 16
	mul	x17, x13, x11
	add	tp, tp, x17
	srli	x11, x15, 16
	add	x11, x11, tp
	slli	x10, x15, 16
	add	x10, x10, x12
	sltu	tp, x10, x12
	add	x11, x11, tp
	jalr	zero, ra, 0
.Ltmp0:
	.size	umul_ppmm, .Ltmp0-umul_ppmm
#endif
#ifdef L_udivmoddi4
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.type	fudiv_qrnnd,@function




fudiv_qrnnd:
	srli	x21, x12, 16
	slli	x17, x10, 16
	srli	x15, x17, 16
	srli	x10, x10, 16
	divu	x14, x11, x21
	remu	x11, x11, x21
	slli	x17, x12, 16
	srli	x13, x17, 16
	slli	x11, x11, 16
	or	x11, x11, x10
	mul	x10, x14, x13
	sltu	tp, x11, x10
	beq	tp, zero, .L2

	addi	x14, x14, -1
	add	x11, x11, x12
	sltu	tp, x11, x12
	bne	tp, zero, .L2
	sltu	tp, x11, x10
	beq	tp, zero, .L2

	addi	x14, x14, -1
	add	x11, x11, x12

.L2:

	sub	x11, x11, x10
	divu	x17, x11, x21
	remu	x10, x11, x21
	addi	x11, x17, 0
	slli	x10, x10, 16
	or	x10, x10, x15


	mul	x13, x13, x11
	sltu	tp, x10, x13
	beq	tp, zero, .L5

	add	x10, x10, x12
	addi	x11, x11, -1
	sltu	tp, x10, x12
	bne	tp, zero, .L5
	sltu	tp, x10, x13
	beq	tp, zero, .L5

	add	x10, x10, x12
	addi	x11, x11, -1

.L5:
	sub	x10, x10, x13
	slli	x14, x14, 16
	or	x11, x11, x14
	jalr	zero, ra, 0
.Ltmp0:
	.size	fudiv_qrnnd, .Ltmp0-fudiv_qrnnd

	.p2align	2
	.globl	__udivmoddi4
	.type	__udivmoddi4,@function












__udivmoddi4:

	addi	sp, sp, -40
	sw	x9, 0(sp)
	sw	x18, 4(sp)
	sw	x19, 8(sp)
	sw	x20, 12(sp)
	sw	x21, 16(sp)
	sw	x8, 20(sp)
	sw	ra, 24(sp)




	addi	x9, x10, 0
	addi	x18, x11, 0
	addi	x19, x12, 0
	addi	x20, x13, 0
	addi	x8, x14, 0
	bne	x13, zero, .L9

	sltu	tp, x18, x19
	beq	tp, zero, .L10





	addi	x10, x19, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0


	li	x17, 28
	add	x17, sp, x17
	sw	x10, 0(x17)



	beq	x10, zero, .LZskipnorm1

	sll	x19, x19, x10
	li	x17, 32
	sub	x15, x17, x10
	srl	x15, x9, x15
	sll	x18, x18, x10
	or	x18, x18, x15
	sll	x9, x9, x10

.LZskipnorm1:
	addi	x10, x9, 0
	addi	x11, x18, 0
	addi	x12, x19, 0
	la	x17, fudiv_qrnnd
	jalr	ra, x17, 0

	li	x17, 32
	add	x17, sp, x17
	sw	x11, 0(x17)



	addi	x9, x10, 0

	li	x15, 0
	li	x17, 36
	add	x17, sp, x17
	sw	x15, 0(x17)



	la	x17, .L19
	jalr	zero, x17, 0

.L10:
	beq	x12, zero, .LZdivzero








	addi	x10, x19, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0


	li	x17, 28
	add	x17, sp, x17
	sw	x10, 0(x17)



	bne	x10, zero, .LZnorm1

	sub	x18, x18, x19

	li	x15, 1
	li	x17, 36
	add	x17, sp, x17
	sw	x15, 0(x17)



	la	x17, .L29
	jalr	zero, x17, 0

	.p2align	2
.LZnorm1:
	li	x17, 32
	sub	tp, x17, x10
	sll	x19, x19, x10
	addi	x12, x19, 0
	srl	x14, x9, tp
	sll	x15, x18, x10
	sll	x9, x9, x10
	or	x10, x15, x14
	srl	x11, x18, tp
	la	x17, fudiv_qrnnd
	jalr	ra, x17, 0

	li	x17, 36
	add	x17, sp, x17
	sw	x11, 0(x17)



	addi	x18, x10, 0

.L29:
	addi	x10, x9, 0
	addi	x11, x18, 0
	addi	x12, x19, 0
	la	x17, fudiv_qrnnd
	jalr	ra, x17, 0

	li	x17, 32
	add	x17, sp, x17
	sw	x11, 0(x17)



	addi	x9, x10, 0

	.p2align	2
.L19:
	beq	x8, zero, .LZsetq


	li	x17, 28
	add	x17, sp, x17
	lw	x13, 0(x17)

	li	x18, 0

	srl	x9, x9, x13



	la	x17, .LZsetr
	jalr	zero, x17, 0

	.p2align	2
.LZdivzero:
	divu	x18, x19, x19
	remu	x9, x19, x19

.LZqzero:
	li	x11, 0
	li	x10, 0
	beq	x8, zero, .LZret

	li	x17, 0
	add	x17, x8, x17
	sw	x9, 0(x17)
	li	x17, 4
	add	x17, x8, x17
	sw	x18, 0(x17)
	la	x17, .LZret
	jalr	zero, x17, 0

.L9:
	sltu	tp, x18, x20
	bne	tp, zero, .LZqzero




	addi	x10, x20, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0


	li	x17, 28
	add	x17, sp, x17
	sw	x10, 0(x17)



	beq	x10, zero, .LZskipnorm2

	li	x17, 32
	sub	x14, x17, x10
	srl	x15, x19, x14
	sll	x12, x20, x10
	or	x12, x12, x15
	addi	x20, x12, 0
	sll	x19, x19, x10
	srl	x13, x9, x14
	sll	x9, x9, x10
	sll	x10, x18, x10
	srl	x11, x18, x14
	or	x10, x10, x13
	la	x17, fudiv_qrnnd
	jalr	ra, x17, 0

	li	x17, 32
	add	x17, sp, x17
	sw	x11, 0(x17)



	addi	x18, x10, 0





	mulhu	x17, x11, x19
	mul	x10, x11, x19
	addi	x11, x17, 0

	sltu	tp, x18, x11
	bne	tp, zero, .L46
	bne	x11, x18, .L45
	sltu	tp, x9, x10
	beq	tp, zero, .L45

.L46:

	li	x17, 32
	add	x17, sp, x17
	lw	x13, 0(x17)
	sub	x11, x11, x20
	addi	x13, x13, -1
	li	x17, 32
	add	x17, sp, x17
	sw	x13, 0(x17)




	sub	x13, x10, x19
	sltu	tp, x10, x13
	sub	x11, x11, tp
	addi	x10, x13, 0

.L45:

	li	x13, 0
	li	x17, 36
	add	x17, sp, x17
	sw	x13, 0(x17)



	beq	x8, zero, .LZsetq

	sub	x10, x9, x10
	sub	x11, x18, x11
	sltu	tp, x9, x10
	sub	x11, x11, tp

	li	x17, 28
	add	x17, sp, x17
	lw	x13, 0(x17)
	li	x17, 32
	sub	x14, x17, x13
	sll	x18, x11, x14
	srl	x9, x10, x13
	or	x9, x9, x18
	srl	x18, x11, x13

.LZsetr:
	li	x17, 0
	add	x17, x8, x17
	sw	x9, 0(x17)
	li	x17, 4
	add	x17, x8, x17
	sw	x18, 0(x17)

.LZsetq:

	li	x17, 32
	add	x17, sp, x17
	lw	x10, 0(x17)
	li	x17, 36
	add	x17, sp, x17
	lw	x11, 0(x17)





	.p2align	2
.LZret:

	lw	x9, 0(sp)
	lw	x18, 4(sp)
	lw	x19, 8(sp)
	lw	x20, 12(sp)
	lw	x21, 16(sp)
	lw	x8, 20(sp)
	lw	ra, 24(sp)
	addi	sp, sp, 40



	jalr	zero, ra, 0

.LZskipnorm2:




	li	x13, 0

	sltu	tp, x20, x18
	bne	tp, zero, .L52
	sltu	tp, x9, x19
	bne	tp, zero, .L51

.L52:

	li	x15, 1
	li	x17, 32
	add	x17, sp, x17
	sw	x15, 0(x17)



	sub	x14, x9, x19
	sub	x18, x18, x20
	sltu	tp, x9, x14
	sub	x18, x18, tp
	addi	x9, x14, 0
	la	x17, .L54
	jalr	zero, x17, 0

.L51:

	li	x17, 32
	add	x17, sp, x17
	sw	x13, 0(x17)




.L54:

	li	x17, 36
	add	x17, sp, x17
	sw	x13, 0(x17)



	bne	x8, zero, .LZsetr
	la	x17, .LZsetq
	jalr	zero, x17, 0
.Ltmp1:
	.size	__udivmoddi4, .Ltmp1-__udivmoddi4
#endif
#ifdef L_umodsi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__umodsi3
	.type	__umodsi3,@function
__umodsi3:
	beq	x11, zero, .L1
	li	x14, 1

.L6:

	sltu	tp, x11, x10
	beq	tp, zero, .L5
	blt	x11, zero, .L5







	slli	x11, x11, 1
	slli	x14, x14, 1
	la	x17, .L6
	jalr	zero, x17, 0

.L5:
	sub	x12, x10, x11
	sltu	tp, x10, x11
	srli	x14, x14, 1
	bne	tp, zero, .Ltmp0
	addi	x10, x12, 0
.Ltmp0:
	srli	x11, x11, 1
	bne	x14, zero, .L5
.L1:
	jalr	zero, ra, 0
.Ltmp1:
	.size	__umodsi3, .Ltmp1-__umodsi3
#endif
#ifdef L_umoddi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__umoddi3
	.type	__umoddi3,@function
__umoddi3:
	addi	sp, sp, -12
	li	x17, 0
	add	x17, sp, x17
	sw	ra, 0(x17)

	addi	x14, sp, 4
	la	x17, __udivmoddi4
	jalr	ra, x17, 0
	li	x17, 4
	add	x17, sp, x17
	lw	x10, 0(x17)
	li	x17, 8
	add	x17, sp, x17
	lw	x11, 0(x17)

	lw	ra, 0(sp)
	li	x17, 12
	add	sp, sp, x17
	jalr	zero, ra, 0
.Ltmp0:
	.size	__umoddi3, .Ltmp0-__umoddi3
#endif
#ifdef L_muldi3
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__muldi3
	.type	__muldi3,@function
__muldi3:

	mul	x13, x13, x10
	mul	x17, x11, x12
	add	x13, x13, x17
	mulhu	x11, x10, x12
	mul	x10, x10, x12
	add	x11, x11, x13
	jalr	zero, ra, 0

.Ltmp0:
	.size	__muldi3, .Ltmp0-__muldi3
#endif
#ifdef L_addsub_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__subdf3
	.type	__subdf3,@function
__subdf3:



	li	x14, 2147483648
	xor	x13, x13, x14


	.globl	__adddf3
	.type	__adddf3,@function
__adddf3:
	slli	x14, x11, 1
	sw	x9, -24(sp)
	sw	x18, -20(sp)
	sw	x19, -16(sp)
	sw	x20, -12(sp)
	sw	x21, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -24

	slli	x9, x13, 1
	li	ra, 2147483648
	sltu	tp, x14, x9
	bne	tp, zero, .LEswap
	bne	x14, x9, .LEmain
	sltu	tp, x10, x12
	beq	tp, zero, .LEmain

.LEswap:
	addi	x19, x10, 0
	addi	x20, x11, 0
	addi	x10, x12, 0
	addi	x11, x13, 0
	addi	x12, x19, 0
	addi	x13, x20, 0
	slli	x14, x11, 1
	slli	x9, x13, 1

.LEmain:
	xor	x13, x13, x11
	and	x13, x13, ra
	srli	x18, x14, 21
	srli	x21, x9, 21
	slli	x20, x14, 10
	slli	x19, x9, 10
	li	x15, 2047
	beq	x15, x18, .LEinfnan
	or	tp, x14, x10
	beq	tp, zero, .LEzeroP
	or	tp, x9, x12
	beq	tp, zero, .LEretA
	sub	x9, x18, x21
	sltiu	tp, x9, 64
	beq	tp, zero, .LEretA
	srli	x15, x10, 21
	or	x20, x20, x15
	slli	x10, x10, 11
	srli	x15, x12, 21
	or	x19, x19, x15
	slli	x12, x12, 11
	sltiu	tp, x18, 2
	bne	tp, zero, .LEmain4
	or	x20, x20, ra
	beq	x21, zero, .LEmain1
	or	x19, x19, ra

.LEmain1:
	addi	x15, x9, -1
	bne	x21, zero, .Ltmp0
	addi	x9, x15, 0
.Ltmp0:
	beq	x9, zero, .LEmain4
	li	x17, 32
	sub	x15, x17, x9
	bge	zero, x15, .LEmain2

	sll	x14, x12, x15
	sll	x15, x19, x15
	srl	x19, x19, x9
	srl	x12, x12, x9
	or	x12, x12, x15
	la	x17, .LEmain3
	jalr	zero, x17, 0

.LEmain2:
	sub	x9, zero, x15
	addi	x15, x15, 32
	sll	x14, x19, x15
	or	x14, x14, x12
	bne	x9, zero, .Ltmp1
	addi	x14, x9, 0
.Ltmp1:
	srl	x12, x19, x9
	li	x19, 0

.LEmain3:
	beq	x14, zero, .LEmain4
	ori	x12, x12, 2

.LEmain4:
	beq	x13, zero, .LEadd

	bne	x18, x21, .LEsub1
	bne	x20, x19, .LEsub1
	beq	x10, x12, .LEzero

.LEsub1:
	sltu	tp, x20, x19
	bne	tp, zero, .LEsub2
	bne	x20, x19, .LEsub3
	sltu	tp, x10, x12
	beq	tp, zero, .LEsub3

.LEsub2:
	addi	x18, x18, -1
	slli	x14, x19, 31
	srli	x19, x19, 1
	srli	x12, x12, 1
	or	x12, x12, x14

.LEsub3:
	addi	x14, x10, 0
	sub	x10, x10, x12
	sltu	tp, x14, x10
	sub	x20, x20, tp
	sub	x20, x20, x19
	sltiu	tp, x18, 2
	bne	tp, zero, .LEdenorm
	bne	x20, zero, .LEsub4

	sltiu	tp, x18, 32
	bne	tp, zero, .LEsub4
	addi	x20, x10, 0
	li	x10, 0
	addi	x18, x18, -32
	bne	x18, zero, .LEsub4
	la	x17, .LEround
	jalr	zero, x17, 0

.LEloop:
	addi	x18, x18, -1
	beq	x18, zero, .LEround
	srli	x14, x10, 31
	slli	x20, x20, 1
	slli	x10, x10, 1
	or	x20, x20, x14

.LEsub4:
	sltu	tp, x20, ra
	bne	tp, zero, .LEloop


.LEround:
	addi	x10, x10, 1024
	sltiu	tp, x10, 1024
	add	x20, x20, tp
	sltu	tp, x20, tp
	add	x18, x18, tp
	srli	x15, x10, 11
	andi	x15, x15, 1
	addi	x9, x10, 0
	sub	x10, x10, x15
	sltu	tp, x9, x10
	sub	x20, x20, tp

	slli	x20, x20, 1
	slli	x15, x20, 20
	srli	x20, x20, 12
	srli	x10, x10, 11
	or	x10, x10, x15
	slli	x15, x18, 20
	or	x20, x20, x15

.LEpack:
	and	x11, x11, ra
	or	x11, x11, x20

.LEretA:
.LEret:
	lw	x9, 0(sp)
	lw	x18, 4(sp)
	lw	x19, 8(sp)
	lw	x20, 12(sp)
	lw	x21, 16(sp)
	lw	ra, 20(sp)
	addi	sp, sp, 24
	jalr	zero, ra, 0

.LEadd:
	add	x10, x10, x12
	sltu	tp, x10, x12
	add	x20, x20, tp
	sltu	tp, x20, tp
	add	x20, x20, x19
	bne	tp, zero, .LEaddover
	sltu	tp, x20, x19
	bne	tp, zero, .LEaddover
	bne	x18, zero, .LEround

.LEdenorm:
	srli	x10, x10, 11
	slli	x14, x20, 21
	srli	x20, x20, 11
	or	x10, x10, x14
	la	x17, .LEpack
	jalr	zero, x17, 0

.LEaddover:
	li	x17, 2046
	sub	x15, x17, x18
	beq	x15, zero, .LEinf
	andi	tp, x10, 1
	ori	x15, x10, 2
	beq	tp, zero, .Ltmp2
	addi	x10, x15, 0
.Ltmp2:
	slli	x14, x20, 31
	srli	x20, x20, 1
	srli	x10, x10, 1
	or	x10, x10, x14
	addi	x18, x18, 1
	la	x17, .LEround
	jalr	zero, x17, 0

.LEinf:
	li	x10, 0
	li	x20, 2146435072
	la	x17, .LEpack
	jalr	zero, x17, 0

.LEzeroP:
	beq	x13, zero, .LEretA

.LEzero:
	li	x10, 0
	li	x11, 0
	la	x17, .LEret
	jalr	zero, x17, 0

.LEinfnan:
	or	x20, x20, x10
	bne	x20, ra, .LEnan
	bne	x15, x21, .LEretA
	beq	x13, zero, .LEretA

.LEnan:
	li	x10, 0
	li	x11, 4294443008
	la	x17, .LEret
	jalr	zero, x17, 0
.Ltmp3:
	.size	__subdf3, .Ltmp3-__subdf3
.Ltmp4:
	.size	__adddf3, .Ltmp4-__adddf3
#endif
#ifdef L_mul_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__mulsf3
	.type	__mulsf3,@function
__mulsf3:




	xor	x7, x11, x10
	li	x14, 2147483648
	and	x7, x7, x14
	slli	x13, x10, 1
	slli	x11, x11, 1
	srli	x5, x13, 24
	srli	x6, x11, 24
	slli	x12, x13, 7
	slli	x10, x11, 7

	beq	x5, zero, .LFzeroAexp
	li	x17, 255
	beq	x5, x17, .LFinfnanA





	or	x12, x12, x14

.LFlab1:
	beq	x6, zero, .LFzeroB

	li	x17, 255
	beq	x6, x17, .LFinfnanB



	or	x10, x10, x14

.LFlab2:




	mulhu	x11, x12, x10
	mul	x10, x12, x10

	ori	x12, x11, 1
	bne	x10, zero, .Ltmp0
	addi	x12, x11, 0
.Ltmp0:
	slti	tp, x12, 0
	bne	tp, zero, .Li18
	slli	x12, x12, 1
	addi	x5, x5, -1

.Li18:
	addi	x15, x6, -126
	add	x5, x5, x15
	bge	zero, x5, .LFunder
	sltiu	tp, x5, 255
	beq	tp, zero, .LFinf

.LFround:
	addi	x12, x12, 128
	sltiu	tp, x12, 128
	add	x5, x5, tp
	srli	x15, x12, 8
	andi	x15, x15, 1
	sub	x12, x12, x15

	slli	x12, x12, 1
	srli	x12, x12, 9
	slli	x10, x5, 23
	or	x10, x10, x12
.LFpack:
	or	x10, x10, x7

.LFret:



	jalr	zero, ra, 0

.LFzeroAexp:

	bne	x12, zero, .LFloopA2


	li	x17, 255
	beq	x6, x17, .LFnan




.LFzero:
	addi	x10, x7, 0
	la	x17, .LFret
	jalr	zero, x17, 0

.LFloopA:
	addi	x5, x5, -1
.LFloopA2:
	add	x12, x12, x12
	sltu	tp, x12, x14
	bne	tp, zero, .LFloopA
	la	x17, .LFlab1
	jalr	zero, x17, 0


.LFinfnanA:
	bne	x12, x14, .LFnan

	beq	x11, zero, .LFnan

	li	x17, 255
	bne	x6, x17, .LFinf




.LFinfnanB:
	bne	x10, x14, .LFnan

.LFinf:
	li	x10, 2139095040
	la	x17, .LFpack
	jalr	zero, x17, 0

.LFzeroB:

	bne	x10, zero, .LFloopB2
	la	x17, .LFzero
	jalr	zero, x17, 0
.LFloopB:
	addi	x6, x6, -1
.LFloopB2:
	add	x10, x10, x10
	sltu	tp, x10, x14
	bne	tp, zero, .LFloopB

	la	x17, .LFlab2
	jalr	zero, x17, 0

.LFnan:
	li	x10, 4290772992
	la	x17, .LFret
	jalr	zero, x17, 0

.LFunder:
	li	x17, 1
	sub	x11, x17, x5
	sltiu	tp, x11, 32
	beq	tp, zero, .LFzero
	li	x17, 32
	sub	x15, x17, x11
	sll	x5, x12, x15
	srl	x12, x12, x11
	beq	x5, zero, .LFunder2
	ori	x12, x12, 2
.LFunder2:
	addi	x15, x12, 128
	slti	x5, x15, 0
	la	x17, .LFround
	jalr	zero, x17, 0
.Ltmp1:
	.size	__mulsf3, .Ltmp1-__mulsf3
#endif
#ifdef L_mul_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__muldf3
	.type	__muldf3,@function
__muldf3:
	sw	x9, -32(sp)
	sw	x18, -28(sp)
	sw	x19, -24(sp)
	sw	x20, -20(sp)
	sw	x21, -16(sp)
	sw	x8, -12(sp)
	sw	gp, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -32
	addi	sp, sp, -16

	slli	x14, x11, 1
	srli	x14, x14, 21
	slli	x18, x11, 11
	srli	x9, x10, 21
	or	x18, x18, x9
	slli	x9, x10, 11
	li	x8, 2147483648
	slli	x15, x13, 1
	srli	x15, x15, 21
	slli	x20, x13, 11
	srli	x19, x12, 21
	or	x20, x20, x19
	slli	x19, x12, 11
	xor	x13, x13, x11
	and	x21, x13, x8

	li	x13, 2047
	beq	x14, zero, .LFAexpzero
	beq	x13, x14, .LFAinfnan
	or	x18, x18, x8

.LFmain1:
	beq	x15, zero, .LFBexpzero
	beq	x13, x15, .LFBinfnan
	or	x20, x20, x8

.LFmain2:
	li	x17, 12
	add	x17, sp, x17
	sw	x21, 0(x17)
	addi	x10, x15, -1022
	add	x14, x14, x10

	mulhu	x13, x18, x20
	mul	x12, x18, x20








	mulhu	x11, x9, x20
	mul	x10, x9, x20







	add	x20, x12, x11
	sltu	tp, x20, x11
	add	x21, x13, tp

	mulhu	x13, x18, x19
	mul	x12, x18, x19

	add	x18, x12, x10
	sltu	tp, x18, x10
	add	x20, x20, tp
	sltu	tp, x20, tp
	add	x21, x21, tp
	add	x20, x20, x13
	sltu	tp, x20, x13
	add	x21, x21, tp

	mulhu	x11, x9, x19
	mul	x10, x9, x19






	add	x18, x18, x11
	sltu	tp, x18, x11
	add	x20, x20, tp
	sltu	tp, x20, tp
	add	x11, x21, tp

	or	x18, x18, x10
	ori	x10, x20, 1
	bne	x18, zero, .Ltmp0
	addi	x10, x20, 0
.Ltmp0:
	slti	tp, x11, 0
	bne	tp, zero, .LFmain3
	addi	tp, x10, 0
	add	x10, x10, x10
	sltu	tp, x10, tp
	add	x11, x11, x11
	add	x11, x11, tp
	addi	x14, x14, -1

.LFmain3:
	li	x17, 12
	add	x17, sp, x17
	lw	x21, 0(x17)
	bge	zero, x14, .LFunderflow
	li	x17, 2047
	sub	x15, x17, x14
	bge	zero, x15, .LFinf
	addi	x10, x10, 1024
	sltiu	tp, x10, 1024
	beq	tp, zero, .LFround
	add	x11, x11, tp
	sltu	tp, x11, tp
	add	x14, x14, tp

.LFround:
	srli	x12, x10, 11
	andi	x12, x12, 1
	sub	x10, x10, x12
	srli	x10, x10, 11
	slli	x12, x11, 21
	or	x10, x10, x12

	slli	x11, x11, 1
	srli	x11, x11, 12
	slli	x15, x14, 20
	or	x11, x11, x15

.LFret:
	or	x11, x11, x21
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LFAexpzero:
	or	tp, x18, x9
	beq	tp, zero, .LFAzero
	srli	tp, x9, 31
	add	x18, x18, x18
	add	x18, x18, tp
	add	x9, x9, x9
	bne	x18, zero, .LFAcont
	addi	x18, x9, 0
	li	x9, 0
	addi	x14, x14, -32

.LFAcont:



	li	x10, 0
	addi	x11, x18, 0
	la	x17, .LFAloop2
	jalr	zero, x17, 0

.LFAloop:
	add	x11, x11, x11
	addi	x10, x10, 1

.LFAloop2:
	sltu	tp, x11, x8
	bne	tp, zero, .LFAloop

	beq	x10, zero, .LFmain1
	sub	x14, x14, x10
	li	x17, 32
	sub	x12, x17, x10
	srl	x12, x9, x12
	sll	x9, x9, x10
	sll	x18, x18, x10
	or	x18, x18, x12
	la	x17, .LFmain1
	jalr	zero, x17, 0

.LFAzero:
	beq	x13, x15, .LFnan

.LFsetsign:
	addi	x11, x21, 0
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LFAinfnan:
	or	x18, x18, x9
	bne	x18, x8, .LFnan
	bne	x15, zero, .LFAcont2
	slli	x12, x20, 1
	or	x12, x12, x19
	beq	x12, zero, .LFnan

.LFAcont2:
	bne	x13, x15, .LFinf

.LFBinfnan:
	or	x20, x20, x19
	bne	x20, x8, .LFnan

.LFinf:
	li	x10, 0
	li	x11, 2146435072
	la	x17, .LFret
	jalr	zero, x17, 0

.LFnan:
	li	x10, 0
	li	x11, 4294443008
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LFBexpzero:
	or	x10, x20, x19
	beq	x10, zero, .LFsetsign
	srli	tp, x19, 31
	add	x20, x20, x20
	add	x20, x20, tp
	add	x19, x19, x19
	bne	x20, zero, .LFBcont
	addi	x20, x19, 0
	li	x19, 0
	addi	x15, x15, -32

.LFBcont:



	li	x10, 0
	addi	x11, x20, 0
	la	x17, .LFBloop2
	jalr	zero, x17, 0

.LFBloop:
	add	x11, x11, x11
	addi	x10, x10, 1

.LFBloop2:
	sltu	tp, x11, x8
	bne	tp, zero, .LFBloop

	beq	x10, zero, .LFmain2
	sub	x15, x15, x10
	li	x17, 32
	sub	x12, x17, x10
	srl	x12, x19, x12
	sll	x19, x19, x10
	sll	x20, x20, x10
	or	x20, x20, x12
	la	x17, .LFmain2
	jalr	zero, x17, 0

.LFunderflow:
	li	x9, 0
	li	x17, 1
	sub	x13, x17, x14
	sltiu	tp, x13, 32
	bne	tp, zero, .LFunderflow2
	addi	x9, x10, 0
	addi	x10, x11, 0
	li	x11, 0
	addi	x13, x13, -32
	beq	x10, zero, .LFunderflow2
	sltiu	tp, x13, 32
	beq	tp, zero, .LFignore

.LFunderflow2:
	beq	x13, zero, .LFunderflow3
	li	x17, 32
	sub	x12, x17, x13
	sll	x18, x11, x12
	sll	x15, x10, x12
	srl	x10, x10, x13
	srl	x11, x11, x13
	or	x10, x10, x18
	or	x9, x9, x15
	beq	x9, zero, .LFunderflow3
	ori	x10, x10, 1

.LFunderflow3:
	addi	x10, x10, 1024
	sltiu	tp, x10, 1024
	add	x11, x11, tp
	srli	x14, x11, 31
	la	x17, .LFround
	jalr	zero, x17, 0

.LFignore:
	li	x10, 0
	la	x17, .LFsetsign
	jalr	zero, x17, 0
.Ltmp1:
	.size	__muldf3, .Ltmp1-__muldf3
#endif
#ifdef L_div_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__divsf3
	.type	__divsf3,@function
__divsf3:




	xor	x7, x11, x10
	li	x14, 2147483648
	and	x7, x7, x14
	slli	x12, x10, 1
	slli	x11, x11, 1
	srli	x5, x12, 24
	srli	x6, x11, 24
	slli	x12, x12, 7
	slli	x13, x11, 7
	beq	x5, zero, .LGzeroAexp

	li	x17, 255
	beq	x5, x17, .LGinfnanA




	or	x12, x12, x14

.LGlab1:
	beq	x6, zero, .LGzeroB

	li	x17, 255
	beq	x6, x17, .LGinfnanB



	or	x13, x13, x14

.LGlab2:
	sltu	tp, x12, x13
	bne	tp, zero, .LGlab3
	srli	x12, x12, 1
	addi	x5, x5, 1

.LGlab3:
	srli	x15, x13, 14
	li	x17, 16383
	and	x28, x13, x17
	divu	x11, x12, x15
	remu	x10, x12, x15
	mul	x12, x28, x11
	slli	x10, x10, 14
	sltu	tp, x10, x12
	beq	tp, zero, .LGlab4
	addi	x11, x11, -1
	add	x10, x10, x13

.LGlab4:
	sub	x10, x10, x12
	divu	x12, x10, x15
	remu	x10, x10, x15
	mul	x28, x28, x12
	slli	x10, x10, 14
	sltu	tp, x10, x28
	beq	tp, zero, .LGlab5
	addi	x12, x12, -1
	add	x10, x10, x13

.LGlab5:
	sub	x10, x10, x28
	slli	x28, x11, 14
	add	x12, x12, x28
	slli	x12, x12, 4
	beq	x10, zero, .LGlab6
	ori	x12, x12, 1

.LGlab6:
	li	x17, 126
	sub	x15, x17, x6
	add	x5, x5, x15
	bge	zero, x5, .LGunder
	sltiu	tp, x5, 255
	beq	tp, zero, .LGinf

.LGround:
	addi	x12, x12, 128
	sltiu	tp, x12, 128
	add	x5, x5, tp
	srli	x15, x12, 8
	andi	x15, x15, 1
	sub	x12, x12, x15

	slli	x12, x12, 1
	srli	x12, x12, 9
	slli	x10, x5, 23
	or	x10, x10, x12
.LGpack:
	or	x10, x10, x7

.LGret:



	jalr	zero, ra, 0

.LGzeroAexp:







	bne	x12, zero, .LGloopA2
	la	x17, .LGzeroA
	jalr	zero, x17, 0
.LGloopA:
	addi	x5, x5, -1
.LGloopA2:
	add	x12, x12, x12
	sltu	tp, x12, x14
	bne	tp, zero, .LGloopA

	la	x17, .LGlab1
	jalr	zero, x17, 0

.LGzeroA:
	beq	x11, zero, .LGnan
	li	x15, 4278190080
	sltu	tp, x15, x11
	bne	tp, zero, .LGnan

.LGzero:
	addi	x10, x7, 0
	la	x17, .LGret
	jalr	zero, x17, 0

.LGinfnanA:
	bne	x12, x14, .LGnan

	beq	x6, x5, .LGnan

.LGinf:
	li	x10, 2139095040
	or	x10, x10, x7
	la	x17, .LGret
	jalr	zero, x17, 0

.LGinfnanB:
	beq	x13, x14, .LGzero

.LGnan:
	li	x10, 4290772992
	la	x17, .LGret
	jalr	zero, x17, 0

.LGzeroB:







	bne	x13, zero, .LGloopB2
	la	x17, .LGinf
	jalr	zero, x17, 0
.LGloopB:
	addi	x6, x6, -1
.LGloopB2:
	add	x13, x13, x13
	sltu	tp, x13, x14
	bne	tp, zero, .LGloopB

	la	x17, .LGlab2
	jalr	zero, x17, 0

.LGunder:
	li	x17, 1
	sub	x11, x17, x5
	sltiu	tp, x11, 32
	beq	tp, zero, .LGzero
	li	x17, 32
	sub	x15, x17, x11
	sll	x5, x12, x15
	srl	x12, x12, x11
	beq	x5, zero, .LGunder2
	ori	x12, x12, 2
.LGunder2:
	addi	x15, x12, 128
	slti	x5, x15, 0
	la	x17, .LGround
	jalr	zero, x17, 0
.Ltmp0:
	.size	__divsf3, .Ltmp0-__divsf3
#endif
#ifdef L_div_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__divdf3
	.type	__divdf3,@function
__divdf3:
	sw	x9, -32(sp)
	sw	x18, -28(sp)
	sw	x19, -24(sp)
	sw	x20, -20(sp)
	sw	x21, -16(sp)
	sw	x8, -12(sp)
	sw	gp, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -32
	addi	sp, sp, -16

	slli	x14, x11, 1
	srli	x14, x14, 21
	slli	x18, x11, 11
	srli	x9, x10, 21
	or	x18, x18, x9
	slli	x9, x10, 11
	li	x8, 2147483648
	slli	x15, x13, 1
	srli	x15, x15, 21
	slli	x20, x13, 11
	srli	x19, x12, 21
	or	x20, x20, x19
	slli	x19, x12, 11
	xor	x13, x13, x11
	and	x21, x13, x8

	li	x13, 2047
	beq	x14, zero, .LGAexpzero
	beq	x13, x14, .LGAinfnan
	or	x18, x18, x8

.LGmain1:
	beq	x15, zero, .LGBexpzero
	beq	x13, x15, .LGBinfnan
	or	x20, x20, x8

.LGmain2:
	sub	x14, x14, x15
	addi	x14, x14, 1023
	srli	x9, x9, 1
	slli	x11, x18, 31
	or	x9, x9, x11
	srli	x18, x18, 1
	srli	x12, x20, 16
	divu	x13, x18, x12
	remu	x18, x18, x12
	slli	x17, x20, 16
	srli	x11, x17, 16
	mul	x15, x11, x13
	slli	x18, x18, 16
	srli	x10, x9, 16
	or	x18, x18, x10
	addi	x10, x18, 0
	sub	x18, x18, x15
	sltu	tp, x10, x18
	beq	tp, zero, .LGmain3

.LGloop1:
	addi	x13, x13, -1
	add	x18, x18, x20
	sltu	tp, x18, x20
	beq	tp, zero, .LGloop1

.LGmain3:
	divu	x17, x18, x12
	remu	x18, x18, x12
	addi	x12, x17, 0
	mul	x15, x11, x12
	slli	x18, x18, 16
	slli	x17, x9, 16
	srli	x10, x17, 16
	or	x18, x18, x10
	addi	x10, x18, 0
	sub	x18, x18, x15
	sltu	tp, x10, x18
	beq	tp, zero, .LGmain4

.LGloop2:
	addi	x12, x12, -1
	add	x18, x18, x20
	sltu	tp, x18, x20
	beq	tp, zero, .LGloop2

.LGmain4:
	slli	x13, x13, 16
	add	x13, x13, x12

	mulhu	x11, x13, x19
	mul	x10, x13, x19

	sub	x9, zero, x10
	addi	x10, x18, 0
	sub	x18, x18, x11
	sltu	tp, x10, x18
	beq	x9, zero, .LGmain5
	addi	x10, x18, 0
	addi	x18, x18, -1
	bne	tp, zero, .LGloopA
	sltu	tp, x10, x18

.LGmain5:
	beq	tp, zero, .LGmain6

.LGloopA:
	addi	x13, x13, -1
	add	x9, x9, x19
	sltu	x10, x9, x19
	add	x18, x18, x20
	sltu	tp, x18, x20
	beq	x10, zero, .LGloopA2
	addi	x18, x18, 1
	bne	tp, zero, .LGmain6
	sltiu	tp, x18, 1

.LGloopA2:
	beq	tp, zero, .LGloopA

.LGmain6:
	bne	x18, x20, .Li25
	addi	x11, x19, 0
	addi	x18, x9, 0
	li	x12, 0
	li	x10, 0
	la	x17, .LGmain7
	jalr	zero, x17, 0

.Li25:
	srli	x11, x20, 16
	divu	x12, x18, x11
	remu	x18, x18, x11
	slli	x17, x20, 16
	srli	x10, x17, 16
	mul	tp, x10, x12
	slli	x18, x18, 16
	srli	x15, x9, 16
	or	x18, x18, x15
	addi	x15, x18, 0
	sub	x18, x18, tp
	sltu	tp, x15, x18
	beq	tp, zero, .Li26

.LGloop3:
	addi	x12, x12, -1
	add	x18, x18, x20
	sltu	tp, x18, x20
	beq	tp, zero, .LGloop3

.Li26:
	divu	x17, x18, x11
	remu	x18, x18, x11
	addi	x11, x17, 0
	mul	x15, x10, x11
	slli	x18, x18, 16
	slli	x17, x9, 16
	srli	x10, x17, 16
	or	x18, x18, x10
	addi	x10, x18, 0
	sub	x18, x18, x15
	sltu	tp, x10, x18
	beq	tp, zero, .Li28

.LGloop4:
	addi	x11, x11, -1
	add	x18, x18, x20
	sltu	tp, x18, x20
	beq	tp, zero, .LGloop4

.Li28:
	slli	x12, x12, 16
	add	x12, x12, x11

	mulhu	x11, x12, x19
	mul	x10, x12, x19

.LGmain7:
	sub	x9, zero, x10
	addi	x10, x18, 0
	sub	x18, x18, x11
	sltu	tp, x10, x18
	beq	x9, zero, .LGmain8
	addi	x10, x18, 0
	addi	x18, x18, -1
	bne	tp, zero, .LGloopB
	sltu	tp, x10, x18

.LGmain8:
	beq	tp, zero, .LGmain9

.LGloopB:
	addi	x12, x12, -1
	add	x9, x9, x19
	sltu	x10, x9, x19
	add	x18, x18, x20
	sltu	tp, x18, x20
	beq	x10, zero, .LGloopB2
	addi	x18, x18, 1
	bne	tp, zero, .LGmain9
	sltiu	tp, x18, 1

.LGloopB2:
	beq	tp, zero, .LGloopB

.LGmain9:
	slti	tp, x13, 0



	bne	tp, zero, .LGmain10
	addi	tp, x12, 0
	add	x12, x12, x12
	sltu	tp, x12, tp
	add	x13, x13, x13
	add	x13, x13, tp
	addi	x14, x14, -1

.LGmain10:
	or	x18, x18, x9
	ori	x10, x12, 1
	bne	x18, zero, .Ltmp0
	addi	x10, x12, 0
.Ltmp0:
	addi	x11, x13, 0
	bge	zero, x14, .LGunderflow
	li	x17, 2047
	sub	x15, x17, x14
	bge	zero, x15, .LGinf
	addi	x10, x10, 1024
	sltiu	tp, x10, 1024
	beq	tp, zero, .LGround
	add	x11, x11, tp
	sltu	tp, x11, tp
	add	x14, x14, tp

.LGround:
	srli	x12, x10, 11
	andi	x12, x12, 1
	sub	x10, x10, x12
	srli	x10, x10, 11
	slli	x12, x11, 21
	or	x10, x10, x12

	slli	x11, x11, 1
	srli	x11, x11, 12
	slli	x15, x14, 20
	or	x11, x11, x15

.LGret:
	or	x11, x11, x21
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LGAexpzero:
	or	tp, x18, x9
	beq	tp, zero, .LGAzero
	srli	tp, x9, 31
	add	x18, x18, x18
	add	x18, x18, tp
	add	x9, x9, x9
	bne	x18, zero, .LGAcont
	addi	x18, x9, 0
	li	x9, 0
	addi	x14, x14, -32

.LGAcont:



	li	x10, 0
	addi	x11, x18, 0
	la	x17, .LGAloop2
	jalr	zero, x17, 0

.LGAloop:
	add	x11, x11, x11
	addi	x10, x10, 1

.LGAloop2:
	sltu	tp, x11, x8
	bne	tp, zero, .LGAloop

	beq	x10, zero, .LGmain1
	sub	x14, x14, x10
	li	x17, 32
	sub	x12, x17, x10
	srl	x12, x9, x12
	sll	x9, x9, x10
	sll	x18, x18, x10
	or	x18, x18, x12
	la	x17, .LGmain1
	jalr	zero, x17, 0

.LGAzero:
	beq	x13, x15, .LGAzero2
	bne	x15, zero, .LGsetsign
	or	tp, x20, x19
	beq	tp, zero, .LGnan

.LGsetsign:
	addi	x11, x21, 0
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LGAzero2:
	or	x20, x20, x19
	beq	x20, x8, .LGsetsign

.LGnan:
	li	x10, 0
	li	x11, 4294443008
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LGAinfnan:
	or	x18, x18, x9
	bne	x18, x8, .LGnan
	beq	x13, x15, .LGnan

.LGinf:
	li	x10, 0
	li	x11, 2146435072
	or	x11, x11, x21
	li	x17, 16
	add	x17, sp, x17
	lw	x9, 0(x17)
	lw	x18, 4(x17)
	lw	x19, 8(x17)
	lw	x20, 12(x17)
	lw	x21, 16(x17)
	lw	x8, 20(x17)
	lw	gp, 24(x17)
	lw	ra, 28(x17)
	addi	x17, x17, 32
	addi	sp, x17, 0
	jalr	zero, ra, 0

.LGBinfnan:
	or	x20, x20, x19
	bne	x20, x8, .LGnan
	li	x10, 0
	la	x17, .LGsetsign
	jalr	zero, x17, 0

.LGBexpzero:
	or	tp, x20, x19
	beq	tp, zero, .LGinf
	srli	tp, x19, 31
	add	x20, x20, x20
	add	x20, x20, tp
	add	x19, x19, x19
	bne	x20, zero, .LGBcont
	addi	x20, x19, 0
	li	x19, 0
	addi	x15, x15, -32

.LGBcont:



	li	x10, 0
	addi	x11, x20, 0
	la	x17, .LGBloop2
	jalr	zero, x17, 0

.LGBloop:
	add	x11, x11, x11
	addi	x10, x10, 1

.LGBloop2:
	sltu	tp, x11, x8
	bne	tp, zero, .LGBloop

	beq	x10, zero, .LGmain2
	sub	x15, x15, x10
	li	x17, 32
	sub	x12, x17, x10
	srl	x12, x19, x12
	sll	x19, x19, x10
	sll	x20, x20, x10
	or	x20, x20, x12
	la	x17, .LGmain2
	jalr	zero, x17, 0

.LGunderflow:
	li	x9, 0
	li	x17, 1
	sub	x13, x17, x14
	sltiu	tp, x13, 32
	bne	tp, zero, .LGunderflow2
	addi	x9, x10, 0
	addi	x10, x11, 0
	li	x11, 0
	addi	x13, x13, -32
	beq	x10, zero, .LGunderflow2
	sltiu	tp, x13, 32
	beq	tp, zero, .LGignore

.LGunderflow2:
	beq	x13, zero, .LGunderflow3
	li	x17, 32
	sub	x12, x17, x13
	sll	x18, x11, x12
	sll	x15, x10, x12
	srl	x10, x10, x13
	srl	x11, x11, x13
	or	x10, x10, x18
	or	x9, x9, x15
	beq	x9, zero, .LGunderflow3
	ori	x10, x10, 1

.LGunderflow3:
	addi	x10, x10, 1024
	sltiu	tp, x10, 1024
	add	x11, x11, tp
	srli	x14, x11, 31
	la	x17, .LGround
	jalr	zero, x17, 0

.LGignore:
	li	x10, 0
	la	x17, .LGsetsign
	jalr	zero, x17, 0
.Ltmp1:
	.size	__divdf3, .Ltmp1-__divdf3
#endif
#ifdef L_negate_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__negsf2
	.type	__negsf2,@function
__negsf2:
	li	x11, 2147483648
	xor	x10, x10, x11
	jalr	zero, ra, 0
.Ltmp0:
	.size	__negsf2, .Ltmp0-__negsf2
#endif
#ifdef L_negate_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__negdf2
	.type	__negdf2,@function
__negdf2:
	li	x12, 2147483648
	xor	x11, x11, x12
	jalr	zero, ra, 0
.Ltmp0:
	.size	__negdf2, .Ltmp0-__negdf2
#endif
#ifdef L_sf_to_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__extendsfdf2
	.type	__extendsfdf2,@function
__extendsfdf2:
	slli	x15, x10, 1
	beq	x15, zero, .LJzero

	srli	x13, x15, 24
	li	x11, 2147483648
	and	x12, x11, x10
	slli	x14, x15, 8
	beq	x13, zero, .LJdenorm


	li	x17, 255
	beq	x13, x17, .LJinfnan






.LJlab1:
	addi	x13, x13, 896
	slli	x10, x14, 20
	srli	x11, x14, 12
	or	x11, x11, x12
	slli	x13, x13, 20
	or	x11, x11, x13
	jalr	zero, ra, 0







.LJdenorm2:
	addi	x13, x13, -1
	add	x14, x14, x14
.LJdenorm:
	sltu	tp, x14, x11
	bne	tp, zero, .LJdenorm2

	slli	x14, x14, 1
	la	x17, .LJlab1
	jalr	zero, x17, 0

.LJinfnan:
	beq	x14, zero, .LJinf
	li	x11, 4294443008
	la	x17, .LJcont
	jalr	zero, x17, 0

.LJinf:
	li	x15, 7340032



	or	x10, x10, x15


.LJzero:

	addi	x11, x10, 0


.LJcont:
	li	x10, 0
	jalr	zero, ra, 0
.Ltmp0:
	.size	__extendsfdf2, .Ltmp0-__extendsfdf2
#endif
#ifdef L_df_to_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__truncdfsf2
	.type	__truncdfsf2,@function
__truncdfsf2:
	sw	x9, -12(sp)
	sw	x18, -8(sp)
	sw	x19, -4(sp)
	addi	sp, sp, -12

	slli	x13, x11, 11
	srli	x18, x10, 21
	or	x13, x13, x18
	slli	x12, x10, 11
	li	x18, 2147483648
	or	x13, x13, x18
	and	x15, x11, x18
	slli	x14, x11, 1
	srli	x14, x14, 21
	addi	x14, x14, -896
	addi	x18, x14, -1
	sltiu	tp, x18, 254
	beq	tp, zero, .LKspec

.LKlab1:
	beq	x12, zero, .Li45
	ori	x13, x13, 1
.Li45:
	li	tp, 128
	add	x13, x13, tp
	sltu	tp, x13, tp

	add	x14, x14, tp
	srli	x18, x13, 8
	andi	x18, x18, 1
	sub	x13, x13, x18
	slli	x13, x13, 1
	srli	x13, x13, 9
	slli	x18, x14, 23
	or	x13, x13, x18
	or	x10, x13, x15

.LK999:
	lw	x9, 0(sp)
	lw	x18, 4(sp)
	lw	x19, 8(sp)
	addi	sp, sp, 12
	jalr	zero, ra, 0

.LKspec:
	li	x17, 1151
	sub	tp, x17, x14
	bne	tp, zero, .Li46
	slli	x18, x13, 1
	or	x18, x18, x12
	beq	x18, zero, .Li46
	li	x10, 4290772992
	la	x17, .LK999
	jalr	zero, x17, 0
.Li46:
	slti	tp, x14, 255
	bne	tp, zero, .Li48
	li	x18, 2139095040
	or	x10, x15, x18
	la	x17, .LK999
	jalr	zero, x17, 0
.Li48:
	li	x17, 1
	sub	x9, x17, x14
	li	x18, 32
	sltu	tp, x9, x18
	bne	tp, zero, .Li49
	addi	x10, x15, 0
	la	x17, .LK999
	jalr	zero, x17, 0
.Li49:
	li	x17, 32
	sub	x19, x17, x9
	sll	x18, x13, x19
	or	x12, x12, x18
	srl	x13, x13, x9
	li	x14, 0
	li	x18, 2147483648
	or	x13, x13, x18
	la	x17, .LKlab1
	jalr	zero, x17, 0
.Ltmp0:
	.size	__truncdfsf2, .Ltmp0-__truncdfsf2
#endif
#ifdef L_fixsfdi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__fixsfdi
	.type	__fixsfdi,@function
__fixsfdi:



	srli	x13, x10, 23
	andi	x13, x13, 255
	slli	x12, x10, 8
	li	x15, 2147483648
	or	x12, x12, x15
	li	x11, 0
	slti	tp, x13, 190
	beq	tp, zero, .LCinfnan
	li	x17, 190
	sub	x13, x17, x13
.LL8:
	li	x15, 32
	sltu	tp, x13, x15
	bne	tp, zero, .LL9
	addi	x11, x12, 0
	li	x12, 0
	addi	x13, x13, -32
	bne	x11, zero, .LL8
.LL9:
	beq	x13, zero, .LL10
	addi	x14, x12, 0
	srl	x11, x11, x13
	srl	x12, x12, x13
	li	x17, 32
	sub	x13, x17, x13
	sll	x14, x14, x13
	or	x11, x11, x14
.LL10:
	slti	tp, x10, 0
	beq	tp, zero, .LCret

	sub	x12, zero, x12
	beq	x11, zero, .LL11
	sub	x11, zero, x11
	addi	x12, x12, -1
.LL11:

.LCret:
	addi	x10, x11, 0
	addi	x11, x12, 0
	jalr	zero, ra, 0

.LCinfnan:
	slti	tp, x10, 0
	bne	tp, zero, .LCret3
	li	x17, 255
	sub	tp, x17, x13
	bne	tp, zero, .Li7
	slli	x15, x12, 1
	beq	x15, zero, .Li7

.LCret3:
	li	x12, 2147483648
	la	x17, .LCret
	jalr	zero, x17, 0
.Li7:
	li	x12, 2147483647
	li	x11, -1
	la	x17, .LCret
	jalr	zero, x17, 0
.Ltmp0:
	.size	__fixsfdi, .Ltmp0-__fixsfdi
#endif
#ifdef L_fixsfsi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__fixsfsi
	.type	__fixsfsi,@function
__fixsfsi:





	slli	x11, x10, 1
	slli	x12, x11, 7
	srli	x11, x11, 24
	li	x17, 158
	sub	x11, x17, x11

	li	x15, 2147483648

	bge	zero, x11, .LJover
	slti	tp, x11, 32
	beq	tp, zero, .LJzero




	or	x12, x12, x15

	srl	x12, x12, x11
	slti	tp, x10, 0
	sub	x10, zero, x12
	bne	tp, zero, .Ltmp0
	addi	x10, x12, 0
.Ltmp0:
	jalr	zero, ra, 0

.LJzero:
	li	x10, 0
	jalr	zero, ra, 0

.LJover:
	li	x14, 2139095040
	sltu	tp, x14, x10
	beq	tp, zero, .LJnan



	addi	x10, x15, 0

	jalr	zero, ra, 0
.LJnan:



	addi	x10, x15, -1

	jalr	zero, ra, 0

.Ltmp1:
	.size	__fixsfsi, .Ltmp1-__fixsfsi
#endif
#ifdef L_fixdfdi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__fixdfdi
	.type	__fixdfdi,@function
__fixdfdi:
	sw	x9, -4(sp)
	addi	sp, sp, -4



	slli	x15, x11, 1
	srli	x15, x15, 21
	slli	x14, x11, 11
	srli	x9, x10, 21
	or	x14, x14, x9
	slli	x13, x10, 11
	li	x9, 2147483648
	or	x14, x14, x9
	sltiu	tp, x15, 1086
	beq	tp, zero, .LCnaninf
	li	x17, 1086
	sub	x12, x17, x15
.LL14:
	li	x9, 32
	sltu	tp, x12, x9
	bne	tp, zero, .LL15
	addi	x13, x14, 0
	li	x14, 0
	addi	x12, x12, -32
	bne	x13, zero, .LL14
.LL15:
	beq	x12, zero, .LL16
	addi	x10, x14, 0
	srl	x13, x13, x12
	srl	x14, x14, x12
	li	x17, 32
	sub	x12, x17, x12
	sll	x10, x10, x12
	or	x13, x13, x10
.LL16:
	slti	tp, x11, 0
	beq	tp, zero, .LCret

	sub	x14, zero, x14
	beq	x13, zero, .LL17
	sub	x13, zero, x13
	addi	x14, x14, -1
.LL17:

.LCret:
	addi	x10, x13, 0
	addi	x11, x14, 0
	lw	x9, 0(sp)
	addi	sp, sp, 4
	jalr	zero, ra, 0

.LCnaninf:
	slti	tp, x11, 0
	bne	tp, zero, .LCret3
	li	x17, 2047
	sub	tp, x17, x15
	bne	tp, zero, .Li5
	slli	x9, x14, 1
	or	x9, x9, x13
	beq	x9, zero, .Li5

.LCret3:
	li	x14, 2147483648
	li	x13, 0
	la	x17, .LCret
	jalr	zero, x17, 0
.Li5:
	li	x14, 2147483647
	li	x13, -1
	la	x17, .LCret
	jalr	zero, x17, 0
.Ltmp0:
	.size	__fixdfdi, .Ltmp0-__fixdfdi
#endif
#ifdef L_fixdfsi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.globl	__fixdfsi
	.type	__fixdfsi,@function
__fixdfsi:

	slli	x13, x11, 11
	srli	x14, x10, 21
	or	x13, x13, x14
	li	x14, 2147483648
	or	x13, x13, x14
	slli	x14, x11, 1
	srli	x14, x14, 21
	li	x17, 1054
	sub	x12, x17, x14
	bge	zero, x12, .LLnaninf
	li	x14, 32
	sltu	tp, x12, x14
	bne	tp, zero, .LL72
	li	x13, 0
.LL72:
	srl	x13, x13, x12
	slti	tp, x11, 0
	beq	tp, zero, .Li50
	sub	x13, zero, x13
.Li50:
	addi	x10, x13, 0
	jalr	zero, ra, 0

.LLnaninf:
	beq	x10, zero, .Li51
	ori	x11, x11, 1
.Li51:
	li	x14, 2146435072
	sltu	tp, x14, x11
	beq	tp, zero, .Li52
	li	x10, 2147483648
	jalr	zero, ra, 0
.Li52:
	li	x10, 2147483647
	jalr	zero, ra, 0

.Ltmp0:
	.size	__fixdfsi, .Ltmp0-__fixdfsi
#endif
#ifdef L_fixunssfsi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.globl	__fixunssfsi
	.type	__fixunssfsi,@function
__fixunssfsi:





	blt	x10, zero, .LZero
	srli	x13, x10, 23
	addi	x13, x13, -127
	blt	x13, zero, .LZero
	slti	tp, x13, 32
	beq	tp, zero, .LMax
	slli	x10, x10, 8



	lui	x12, 524288
	or	x11, x10, x12

	li	x17, 31
	sub	x10, x17, x13
	srl	x10, x11, x10
	jalr	zero, ra, 0
.LZero:
	addi	x10, zero, 0
	jalr	zero, ra, 0
.LMax:
	addi	x10, zero, -1
	jalr	zero, ra, 0

.Ltmp0:
	.size	__fixunssfsi, .Ltmp0-__fixunssfsi
#endif
#ifdef L_fixunsdfsi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__fixunsdfsi
	.type	__fixunsdfsi,@function
__fixunsdfsi:

	slli	x13, x11, 11
	srli	x14, x10, 21
	or	x13, x13, x14
	li	x14, 2147483648
	or	x13, x13, x14
	slli	x14, x11, 1
	srli	x14, x14, 21
	li	x17, 1054
	sub	x12, x17, x14
	slti	tp, x12, 0
	bne	tp, zero, .LNnaninf
	li	x14, 32
	sltu	tp, x12, x14
	bne	tp, zero, .LL73
	li	x13, 0
.LL73:
	srl	x13, x13, x12
	slti	tp, x11, 0
	beq	tp, zero, .Li53
	sub	x13, zero, x13
.Li53:
	addi	x10, x13, 0
	jalr	zero, ra, 0

.LNnaninf:
	beq	x10, zero, .Li54
	ori	x11, x11, 1
.Li54:
	li	x14, 2146435072
	sltu	tp, x14, x11
	beq	tp, zero, .Li55
	li	x10, 2147483648
	jalr	zero, ra, 0
.Li55:
	li	x10, -1
	jalr	zero, ra, 0

.Ltmp0:
	.size	__fixunsdfsi, .Ltmp0-__fixunsdfsi
#endif
#ifdef L_fixunssfdi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__fixunssfdi
	.type	__fixunssfdi,@function
__fixunssfdi:

	blt	x10, zero, .LZero

	srli	x12, x10, 23
	addi	x12, x12, -127
	blt	x12, zero, .LZero

	slti	tp, x12, 64
	beq	tp, zero, .LMax

	slli	x10, x10, 8



	lui	x13, 524288
	or	x10, x10, x13

	li	x17, 31
	sub	x13, x17, x12
	blt	x13, zero, .Lgt31

	srl	x10, x10, x13
	addi	x11, zero, 0
	jalr	zero, ra, 0

.Lgt31:
	li	x17, 63
	sub	x12, x17, x12
	sub	x13, zero, x13
	srl	x11, x10, x12
	sll	x10, x10, x13
	li	x17, 32
	beq	x13, x17, .LClrL
	jalr	zero, ra, 0
.LZero:
	addi	x11, zero, 0
.LClrL:
	addi	x10, zero, 0
	jalr	zero, ra, 0
.LMax:
	addi	x10, zero, -1
	addi	x11, zero, -1
	jalr	zero, ra, 0
.Ltmp0:
	.size	__fixunssfdi, .Ltmp0-__fixunssfdi
#endif
#ifdef L_fixunsdfdi
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__fixunsdfdi
	.type	__fixunsdfdi,@function
__fixunsdfdi:
	sw	x9, -4(sp)
	addi	sp, sp, -4



	slli	x15, x11, 1
	srli	x15, x15, 21
	slli	x14, x11, 11
	srli	x9, x10, 21
	or	x14, x14, x9
	slli	x13, x10, 11
	li	x9, 2147483648
	or	x14, x14, x9
	sltiu	tp, x15, 1086
	beq	tp, zero, .LDnaninf
	li	x17, 1086
	sub	x12, x17, x15
.LL18:
	li	x9, 32
	sltu	tp, x12, x9
	bne	tp, zero, .LL19
	addi	x13, x14, 0
	li	x14, 0
	addi	x12, x12, -32
	bne	x13, zero, .LL18
.LL19:
	beq	x12, zero, .LL20
	addi	x10, x14, 0
	srl	x13, x13, x12
	srl	x14, x14, x12
	li	x17, 32
	sub	x12, x17, x12
	sll	x10, x10, x12
	or	x13, x13, x10
.LL20:
	slti	tp, x11, 0
	beq	tp, zero, .LDret

	sub	x14, zero, x14
	beq	x13, zero, .LL21
	sub	x13, zero, x13
	addi	x14, x14, -1
.LL21:

.LDret:
	addi	x10, x13, 0
	addi	x11, x14, 0
	lw	x9, 0(sp)
	addi	sp, sp, 4
	jalr	zero, ra, 0

.LDnaninf:
	li	x14, 2147483648
	li	x13, 0
	la	x17, .LDret
	jalr	zero, x17, 0
.Ltmp0:
	.size	__fixunsdfdi, .Ltmp0-__fixunsdfdi
#endif
#ifdef L_si_to_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatsisf
	.type	__floatsisf,@function
__floatsisf:
	beq	x10, zero, .LKzero
	li	x14, 2147483648
	and	x12, x10, x14
	beq	x12, zero, .LKcont
	sub	x10, zero, x10

.LKcont:
	li	x11, 158





	li	x15, 16
	li	x13, 0
.LKloop:
	add	x13, x13, x15
	srl	tp, x10, x13
	bne	tp, zero, .LKloop2
	sll	x10, x10, x15
	sub	x11, x11, x15
.LKloop2:
	srli	x15, x15, 1
	bne	x15, zero, .LKloop


	srli	x14, x14, 24
	add	x10, x10, x14
	sltu	tp, x10, x14
	add	x11, x11, tp
	srai	x14, x10, 8
	andi	x14, x14, 1
	sub	x10, x10, x14
	slli	x10, x10, 1

	srli	x10, x10, 9
	slli	x14, x11, 23
	or	x10, x10, x14
	or	x10, x10, x12

.LKzero:
	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatsisf, .Ltmp0-__floatsisf
#endif
#ifdef L_si_to_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatsidf
	.type	__floatsidf,@function
__floatsidf:



	sw	x9, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -8


	li	x11, 0
	addi	x15, x11, 0
	addi	x13, x11, 0
	addi	x12, x10, 0
	beq	x12, zero, .Li39
	slti	tp, x12, 0
	beq	tp, zero, .Li40
	li	x15, 2147483648

	sub	x12, zero, x12
	beq	x11, zero, .LL71
	sub	x11, zero, x11
	addi	x12, x12, -1
.LL71:
.Li40:
	li	x13, 1054




	sw	x10, -16(sp)
	sw	x11, -12(sp)
	sw	x12, -8(sp)
	sw	x13, -4(sp)
	addi	sp, sp, -16
	sw	x15, -4(sp)
	addi	sp, sp, -4
	addi	x10, x12, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0
	addi	x14, x10, 0
	lw	x15, 0(sp)
	addi	sp, sp, 4
	lw	x10, 0(sp)
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	addi	sp, sp, 16

	sub	x13, x13, x14
	sll	x12, x12, x14
.Li39:
	srli	x14, x11, 11
	slli	x9, x12, 21
	or	x14, x14, x9
	slli	x9, x12, 1
	srli	x9, x9, 12
	or	x15, x15, x9
	slli	x9, x13, 20
	or	x15, x15, x9
	addi	x10, x14, 0
	addi	x11, x15, 0




	lw	x9, 0(sp)
	lw	ra, 4(sp)
	addi	sp, sp, 8

	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatsidf, .Ltmp0-__floatsidf
#endif
#ifdef L_floatdisf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatdisf
	.type	__floatdisf,@function
__floatdisf:



	sw	x9, -12(sp)
	sw	x18, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -12


	li	x18, 2147483648
	and	x15, x11, x18
	addi	x13, x11, 0
	addi	x12, x10, 0
	or	x18, x11, x10
	beq	x18, zero, .Li1
	slti	tp, x11, 0
	beq	tp, zero, .Li2

	sub	x13, zero, x13
	beq	x12, zero, .LL1
	sub	x12, zero, x12
	addi	x13, x13, -1
.LL1:
.Li2:
	li	x14, 190


	bne	x13, zero, .LL2
	bne	x12, zero, .LL3
	li	x14, 0
	la	x17, .LL4
	jalr	zero, x17, 0
.LL3:
	addi	x13, x12, 0
	li	x12, 0
	li	x9, 32
	sub	x14, x14, x9
.LL2:



	sw	x10, -24(sp)
	sw	x11, -20(sp)
	sw	x12, -16(sp)
	sw	x13, -12(sp)
	sw	x14, -8(sp)
	sw	x15, -4(sp)
	addi	sp, sp, -24
	addi	x10, x13, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0
	addi	x9, x10, 0
	lw	x10, 0(sp)
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	lw	x15, 20(sp)
	addi	sp, sp, 24

	beq	x9, zero, .LL4
	sub	x14, x14, x9
	li	x17, 32
	sub	x10, x17, x9
	srl	x10, x12, x10
	sll	x12, x12, x9
	sll	x13, x13, x9
	or	x13, x13, x10
.LL4:

	beq	x12, zero, .Li3
	ori	x13, x13, 1
.Li3:
	li	tp, 128
	add	x13, x13, tp
	sltu	tp, x13, tp

	add	x14, x14, tp
	srli	x18, x13, 8
	andi	x18, x18, 1
	sub	x13, x13, x18
	slli	x13, x13, 1
	srli	x13, x13, 9
	slli	x18, x14, 23
	or	x13, x13, x18
.Li1:
	or	x10, x13, x15

.LA999:



	lw	x9, 0(sp)
	lw	x18, 4(sp)
	lw	ra, 8(sp)
	addi	sp, sp, 12

	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatdisf, .Ltmp0-__floatdisf
#endif
#ifdef L_floatdidf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatdidf
	.type	__floatdidf,@function
__floatdidf:



	sw	x9, -16(sp)
	sw	x18, -12(sp)
	sw	x19, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -16


	li	x14, 0
	addi	x18, x14, 0
	addi	x13, x11, 0
	addi	x12, x10, 0
	or	x19, x11, x10
	beq	x19, zero, .Li1
	li	x14, 1086
	slti	tp, x11, 0
	beq	tp, zero, .Li2
	li	x18, 2147483648

	sub	x13, zero, x13
	beq	x12, zero, .LL1
	sub	x12, zero, x12
	addi	x13, x13, -1
.LL1:

.Li2:
	bne	x13, zero, .LL2
	bne	x12, zero, .LL3
	li	x14, 0
	la	x17, .LL4
	jalr	zero, x17, 0
.LL3:
	addi	x13, x12, 0
	li	x12, 0
	li	x9, 32
	sub	x14, x14, x9
.LL2:

	sw	x10, -24(sp)
	sw	x11, -20(sp)
	sw	x12, -16(sp)
	sw	x13, -12(sp)
	sw	x14, -8(sp)
	sw	x15, -4(sp)
	addi	sp, sp, -24
	addi	x10, x13, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0
	addi	x9, x10, 0
	lw	x10, 0(sp)
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	lw	x15, 20(sp)
	addi	sp, sp, 24

	beq	x9, zero, .LL4
	sub	x14, x14, x9
	li	x17, 32
	sub	x15, x17, x9
	srl	x15, x12, x15
	sll	x12, x12, x9
	sll	x13, x13, x9
	or	x13, x13, x15
.LL4:

	li	tp, 1024
	add	x12, x12, tp
	sltu	tp, x12, tp


	beq	tp, zero, .LL7
	add	x13, x13, tp
	sltu	tp, x13, tp
.LL7:

	add	x14, x14, tp
	srli	x19, x12, 11
	andi	x19, x19, 1
	sub	x12, x12, x19
.Li1:
	srli	x15, x12, 11
	slli	x19, x13, 21
	or	x15, x15, x19
	slli	x9, x13, 1
	srli	x9, x9, 12
	slli	x19, x14, 20
	or	x9, x9, x19
	or	x9, x9, x18
	addi	x10, x15, 0
	addi	x11, x9, 0

.LA999:



	lw	x9, 0(sp)
	lw	x18, 4(sp)
	lw	x19, 8(sp)
	lw	ra, 12(sp)
	addi	sp, sp, 16

	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatdidf, .Ltmp0-__floatdidf
#endif
#ifdef L_floatunsisf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatunsisf
	.type	__floatunsisf,@function
__floatunsisf:
	beq	x10, zero, .LKzero

	li	x11, 158





	li	x15, 16
	li	x13, 0
.LKloop:
	add	x13, x13, x15
	srl	tp, x10, x13
	bne	tp, zero, .LKloop2
	sll	x10, x10, x15
	sub	x11, x11, x15
.LKloop2:
	srli	x15, x15, 1
	bne	x15, zero, .LKloop


	addi	x10, x10, 128
	sltiu	tp, x10, 128
	add	x11, x11, tp
	srli	x14, x10, 8
	andi	x14, x14, 1
	sub	x10, x10, x14
	slli	x10, x10, 1

	srli	x10, x10, 9
	slli	x14, x11, 23
	or	x10, x10, x14

.LKzero:
	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatunsisf, .Ltmp0-__floatunsisf
#endif
#ifdef L_floatunsidf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatunsidf
	.type	__floatunsidf,@function
__floatunsidf:



	sw	x9, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -8


	li	x11, 0
	addi	x13, x11, 0
	addi	x12, x10, 0
	beq	x12, zero, .Li41
	li	x13, 1054




	sw	x10, -20(sp)
	sw	x11, -16(sp)
	sw	x12, -12(sp)
	sw	x13, -8(sp)
	sw	x14, -4(sp)
	addi	sp, sp, -20
	addi	x10, x12, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0
	addi	x15, x10, 0
	lw	x10, 0(sp)
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	addi	sp, sp, 20

	sub	x13, x13, x15
	sll	x12, x12, x15
.Li41:
	srli	x14, x11, 11
	slli	x9, x12, 21
	or	x14, x14, x9
	slli	x15, x12, 1
	srli	x15, x15, 12
	slli	x9, x13, 20
	or	x15, x15, x9
	addi	x10, x14, 0
	addi	x11, x15, 0




	lw	x9, 0(sp)
	lw	ra, 4(sp)
	addi	sp, sp, 8

	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatunsidf, .Ltmp0-__floatunsidf
#endif
#ifdef L_floatundisf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatundisf
	.type	__floatundisf,@function
__floatundisf:



	sw	x9, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -8


	addi	x13, x11, 0
	addi	x12, x10, 0
	or	x9, x11, x10
	beq	x9, zero, .Li4
	li	x14, 190


	bne	x13, zero, .LL5
	bne	x12, zero, .LL6
	li	x14, 0
	la	x17, .LL7
	jalr	zero, x17, 0
.LL6:
	addi	x13, x12, 0
	li	x12, 0
	li	x15, 32
	sub	x14, x14, x15
.LL5:



	sw	x10, -20(sp)
	sw	x11, -16(sp)
	sw	x12, -12(sp)
	sw	x13, -8(sp)
	sw	x14, -4(sp)
	addi	sp, sp, -20
	addi	x10, x13, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0
	addi	x15, x10, 0
	lw	x10, 0(sp)
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	addi	sp, sp, 20

	beq	x15, zero, .LL7
	sub	x14, x14, x15
	li	x17, 32
	sub	x10, x17, x15
	srl	x10, x12, x10
	sll	x12, x12, x15
	sll	x13, x13, x15
	or	x13, x13, x10
.LL7:

	beq	x12, zero, .Li5
	ori	x13, x13, 1
.Li5:
	li	tp, 128
	add	x13, x13, tp
	sltu	tp, x13, tp

	add	x14, x14, tp
	srli	x9, x13, 8
	andi	x9, x9, 1
	sub	x13, x13, x9
	slli	x13, x13, 1
	srli	x13, x13, 9
	slli	x9, x14, 23
	or	x13, x13, x9
.Li4:
	addi	x10, x13, 0

.LB999:



	lw	x9, 0(sp)
	lw	ra, 4(sp)
	addi	sp, sp, 8

	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatundisf, .Ltmp0-__floatundisf
#endif
#ifdef L_floatundidf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__floatundidf
	.type	__floatundidf,@function
__floatundidf:



	sw	x9, -12(sp)
	sw	x18, -8(sp)
	sw	ra, -4(sp)
	addi	sp, sp, -12


	li	x14, 0
	addi	x13, x11, 0
	addi	x12, x10, 0
	or	x18, x11, x10
	beq	x18, zero, .Li3
	li	x14, 1086


	bne	x13, zero, .LL8
	bne	x12, zero, .LL9
	li	x14, 0
	la	x17, .LL10
	jalr	zero, x17, 0
.LL9:
	addi	x13, x12, 0
	li	x12, 0
	li	x9, 32
	sub	x14, x14, x9
.LL8:

	sw	x10, -24(sp)
	sw	x11, -20(sp)
	sw	x12, -16(sp)
	sw	x13, -12(sp)
	sw	x14, -8(sp)
	sw	x15, -4(sp)
	addi	sp, sp, -24
	addi	x10, x13, 0
	la	x17, __clzsi2
	jalr	ra, x17, 0
	addi	x9, x10, 0
	lw	x10, 0(sp)
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	lw	x15, 20(sp)
	addi	sp, sp, 24

	beq	x9, zero, .LL10
	sub	x14, x14, x9
	li	x17, 32
	sub	x15, x17, x9
	srl	x15, x12, x15
	sll	x12, x12, x9
	sll	x13, x13, x9
	or	x13, x13, x15
.LL10:

	li	tp, 1024
	add	x12, x12, tp
	sltu	tp, x12, tp


	beq	tp, zero, .LL13
	add	x13, x13, tp
	sltu	tp, x13, tp
.LL13:

	add	x14, x14, tp
	srli	x18, x12, 11
	andi	x18, x18, 1
	sub	x12, x12, x18
.Li3:
	srli	x15, x12, 11
	slli	x18, x13, 21
	or	x15, x15, x18
	slli	x9, x13, 1
	srli	x9, x9, 12
	slli	x18, x14, 20
	or	x9, x9, x18
	addi	x10, x15, 0
	addi	x11, x9, 0

.LB999:



	lw	x9, 0(sp)
	lw	x18, 4(sp)
	lw	ra, 8(sp)
	addi	sp, sp, 12

	jalr	zero, ra, 0
.Ltmp0:
	.size	__floatundidf, .Ltmp0-__floatundidf
#endif
#ifdef L_compare_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__gtsf2
	.type	__gtsf2,@function
__gtsf2:
	.globl	__gesf2
	.type	__gesf2,@function
__gesf2:
	li	x14, -1
	la	x17, .LA
	jalr	zero, x17, 0

	.globl	__eqsf2
	.type	__eqsf2,@function
__eqsf2:
	.globl	__nesf2
	.type	__nesf2,@function
__nesf2:
	.globl	__lesf2
	.type	__lesf2,@function
__lesf2:
	.globl	__ltsf2
	.type	__ltsf2,@function
__ltsf2:
	.globl	__cmpsf2
	.type	__cmpsf2,@function
__cmpsf2:
	li	x14, 1

	.p2align	2
.LA:

	li	x15, 4278190080
	slli	x12, x10, 1
	sltu	tp, x15, x12
	bne	tp, zero, .LMnan
	slli	x13, x11, 1
	sltu	tp, x15, x13
	bne	tp, zero, .LMnan

	xor	x15, x10, x11
	bge	x15, zero, .LSameSign

.LDiffSign:




	or	x12, x12, x13
	beq	x12, zero, .LMequ
	li	x12, 1
	bne	x10, zero, .Ltmp0
	addi	x10, x12, 0
.Ltmp0:
	jalr	zero, ra, 0

.LSameSign:
	slti	tp, x10, 0
	bne	tp, zero, .LSameSignNeg
.LSameSignPos:
	sub	x10, x10, x11
	jalr	zero, ra, 0
.LSameSignNeg:
	sub	x10, x11, x10
	jalr	zero, ra, 0

.LMequ:
	li	x10, 0
	jalr	zero, ra, 0


.LMnan:
	addi	x10, x14, 0
	jalr	zero, ra, 0

.Ltmp1:
	.size	__cmpsf2, .Ltmp1-__cmpsf2
.Ltmp2:
	.size	__ltsf2, .Ltmp2-__ltsf2
.Ltmp3:
	.size	__lesf2, .Ltmp3-__lesf2
.Ltmp4:
	.size	__nesf2, .Ltmp4-__nesf2
.Ltmp5:
	.size	__eqsf2, .Ltmp5-__eqsf2
.Ltmp6:
	.size	__gesf2, .Ltmp6-__gesf2
.Ltmp7:
	.size	__gtsf2, .Ltmp7-__gtsf2
#endif
#ifdef L_compare_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__gtdf2
	.type	__gtdf2,@function
__gtdf2:
	.globl	__gedf2
	.type	__gedf2,@function
__gedf2:
	li	x14, -1
	la	x17, .LA
	jalr	zero, x17, 0

	.globl	__eqdf2
	.type	__eqdf2,@function
__eqdf2:
	.globl	__nedf2
	.type	__nedf2,@function
__nedf2:
	.globl	__ledf2
	.type	__ledf2,@function
__ledf2:
	.globl	__ltdf2
	.type	__ltdf2,@function
__ltdf2:
	.globl	__cmpdf2
	.type	__cmpdf2,@function
__cmpdf2:
	li	x14, 1

.LA:
	li	x15, 0




	li	x7, 4292870144
	slli	x5, x11, 1
	sltu	tp, x15, x10
	add	x28, x5, tp
	sltu	tp, x7, x28
	bne	tp, zero, .LMnan
	slli	x6, x13, 1
	sltu	tp, x15, x12
	add	x28, x6, tp
	sltu	tp, x7, x28
	bne	tp, zero, .LMnan
	xor	x14, x11, x13
	blt	x14, zero, .LMdiff

	slti	tp, x11, 0
	bne	tp, zero, .LMsame

	sub	x14, x10, x12
	sltu	tp, x10, x14
	sub	x10, x11, x13
	sub	x10, x10, tp
	sltu	tp, x15, x14
	bne	x10, zero, .Ltmp0
	addi	x10, tp, 0
.Ltmp0:



	jalr	zero, ra, 0


.LMsame:
	sub	x14, x12, x10
	sltu	tp, x12, x14
	sub	x10, x13, x11
	sub	x10, x10, tp
	sltu	tp, x15, x14
	bne	x10, zero, .Ltmp1
	addi	x10, tp, 0
.Ltmp1:



	jalr	zero, ra, 0

	.p2align	2

.LMdiff:



	or	x15, x5, x6

	or	x14, x10, x12



	or	x15, x15, x14

	beq	x15, zero, .LMret


	li	x10, 1
	beq	x11, zero, .Ltmp2
	addi	x10, x11, 0
.Ltmp2:


.LMret:



	jalr	zero, ra, 0

.LMnan:
	addi	x10, x14, 0



	jalr	zero, ra, 0

.Ltmp3:
	.size	__cmpdf2, .Ltmp3-__cmpdf2
.Ltmp4:
	.size	__ltdf2, .Ltmp4-__ltdf2
.Ltmp5:
	.size	__ledf2, .Ltmp5-__ledf2
.Ltmp6:
	.size	__nedf2, .Ltmp6-__nedf2
.Ltmp7:
	.size	__eqdf2, .Ltmp7-__eqdf2
.Ltmp8:
	.size	__gedf2, .Ltmp8-__gedf2
.Ltmp9:
	.size	__gtdf2, .Ltmp9-__gtdf2
#endif
#ifdef L_unord_sf
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__unordsf2
	.type	__unordsf2,@function
__unordsf2:
	slli	x10, x10, 1
	li	x13, 4278190080
	sltu	x10, x13, x10
	bne	x10, zero, .Li67
	slli	x11, x11, 1
	sltu	x10, x13, x11

.Li67:
	jalr	zero, ra, 0
.Ltmp0:
	.size	__unordsf2, .Ltmp0-__unordsf2
#endif
#ifdef L_unord_df
	.text





	.section	.mdebug.abi_nds32,"",@progbits
	.text


	.p2align	2
	.globl	__unorddf2
	.type	__unorddf2,@function
__unorddf2:
	slli	x11, x11, 1
	li	x14, 0
	sltu	tp, x14, x10
	add	x10, x11, tp
	li	x15, 4292870144
	sltu	x10, x15, x10
	bne	x10, zero, .Li69

	slli	x13, x13, 1
	sltu	tp, x14, x12
	add	x13, x13, tp
	sltu	x10, x15, x13

.Li69:
	jalr	zero, ra, 0
.Ltmp0:
	.size	__unorddf2, .Ltmp0-__unorddf2
#endif
