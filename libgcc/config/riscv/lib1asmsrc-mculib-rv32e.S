#ifdef L_addsub_sf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__subsf3
	.type	__subsf3,@function
__subsf3:
	li	x12, 2147483648         # 	move	$r2, #2147483648
	xor	x11, x11, x12           # 	xor	$r1, $r1, $r2
	.globl	__addsf3
	.type	__addsf3,@function
__addsf3:
	slli	x12, x10, 1             # 	slli	$r2, $r0, #1
	addi	sp, sp, -16             # 	push.s	$r6, $r9, {}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	slli	x13, x11, 1             # 	slli	$r3, $r1, #1
	li	x5, 2147483648          # 	move	$r7, #2147483648
	la	x7, __r7
	sw	x5, 0(x7)
	sltu	tp, x12, x13            # 	slt	$ta, $r2, $r3
	beqz	tp, .LEcont             # 	beqz	$ta, .LEcont
	mv	x15, x10                # 	move	$r5, $r0
	mv	x10, x11                # 	move	$r0, $r1
	mv	x11, x15                # 	move	$r1, $r5
	slli	x12, x10, 1             # 	slli	$r2, $r0, #1
	slli	x13, x11, 1             # 	slli	$r3, $r1, #1
.LEcont:
	xor	x11, x11, x10           # 	xor	$r1, $r1, $r0
	la	x6, __r7                # 	and	$r1, $r1, $r7
	lw	x6, 0(x6)
	and	x11, x11, x6
	srli	x14, x12, 24            # 	srli	$r4, $r2, #24
	srli	x5, x13, 24             # 	srli	$r8, $r3, #24
	la	x7, __r8
	sw	x5, 0(x7)
	slli	x9, x12, 7              # 	slli	$r6, $r2, #7
	slli	x5, x13, 7              # 	slli	$r9, $r3, #7
	la	x7, __r9
	sw	x5, 0(x7)
	li	x6, 255                 # 	beqc	$r4, #255, .LEinfnan
	beq	x14, x6, .LEinfnan
	beqz	x12, .LEzeroP           # 	beqz	$r2, .LEzeroP
	beqz	x13, .LEretA            # 	beqz	$r3, .LEretA
	la	x6, __r8                # 	sub	$r5, $r4, $r8
	lw	x6, 0(x6)
	sub	x15, x14, x6
	sltiu	tp, x14, 2              # 	slti	$ta, $r4, #2
	bnez	tp, .LElab2             # 	bnez	$ta, .LElab2
	slti	tp, x15, 32             # 	sltsi	$ta, $r5, #32
	beqz	tp, .LEretA             # 	beqz	$ta, .LEretA
	la	x6, __r7                # 	or	$r6, $r6, $r7
	lw	x6, 0(x6)
	or	x9, x9, x6
	la	x5, __r8                # 	beqz	$r8, .LElab1
	lw	x5, 0(x5)
	beqz	x5, .LElab1
	la	x6, __r7                # 	or	$r9, $r9, $r7
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
.LElab1:
	addi	tp, x15, -1             # 	addi	$ta, $r5, #-1
	la	x6, __r8                # 	cmovz	$r5, $ta, $r8
	lw	x6, 0(x6)
	bne	x6, zero, .Ltmp0
	addi	x15, tp, 0
.Ltmp0:
	la	x5, __r9                # 	move	$ta, $r9
	lw	x5, 0(x5)
	mv	tp, x5
	la	x5, __r9                # 	srl	$r9, $r9, $r5
	lw	x5, 0(x5)
	srl	x5, x5, x15
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	sll	$r5, $r9, $r5
	lw	x5, 0(x5)
	sll	x15, x5, x15
	beq	x15, tp, .LElab2        # 	beq	$r5, $ta, .LElab2
	la	x5, __r9                # 	ori	$r9, $r9, #2
	lw	x5, 0(x5)
	ori	x5, x5, 2
	la	x7, __r9
	sw	x5, 0(x7)
.LElab2:
	bnez	x11, .LEsub             # 	bnez	$r1, .LEsub
	la	x6, __r9                # 	add	$r6, $r6, $r9
	lw	x6, 0(x6)
	add	x9, x9, x6
	la	x6, __r9                # 	slt	$ta, $r6, $r9
	lw	x6, 0(x6)
	sltu	tp, x9, x6
	beqz	tp, .LEaddnoover        # 	beqz	$ta, .LEaddnoover
	li	x6, 254                 # 	beqc	$r4, #254, .LEinf
	beq	x14, x6, .LEinf
	andi	tp, x9, 1               # 	andi	$ta, $r6, #1
	ori	x15, x9, 2              # 	ori	$r5, $r6, #2
	beq	tp, zero, .Ltmp1        # 	cmovn	$r6, $r5, $ta
	addi	x9, x15, 0
.Ltmp1:
	srli	x9, x9, 1               # 	srli	$r6, $r6, #1
	addi	x14, x14, 1             # 	addi	$r4, $r4, #1
	tail	.LEround                # 	b	.LEround
.LEaddnoover:
	bnez	x14, .LEround           # 	bnez	$r4, .LEround
.LEdenorm:
	srli	x9, x9, 8               # 	srli	$r6, $r6, #8
	tail	.LEpack                 # 	b	.LEpack
.LEsub:
	beq	x12, x13, .LEzero       # 	beq	$r2, $r3, .LEzero
	la	x6, __r9                # 	slt	$ta, $r6, $r9
	lw	x6, 0(x6)
	sltu	tp, x9, x6
	beqz	tp, .LEsub2             # 	beqz	$ta, .LEsub2
	la	x5, __r9                # 	srli	$r9, $r9, #1
	lw	x5, 0(x5)
	srli	x5, x5, 1
	la	x7, __r9
	sw	x5, 0(x7)
	addi	x14, x14, -1            # 	addi	$r4, $r4, #-1
.LEsub2:
	la	x6, __r9                # 	sub	$r6, $r6, $r9
	lw	x6, 0(x6)
	sub	x9, x9, x6
	sltiu	tp, x14, 2              # 	slti	$ta, $r4, #2
	bnez	tp, .LEdenorm           # 	bnez	$ta, .LEdenorm
	tail	.LEloopC2               # 	b	.LEloopC2
.LEloopC:
	addi	x14, x14, -1            # 	addi	$r4, $r4, #-1
	beqz	x14, .LEround           # 	beqz	$r4, .LEround
	add	x9, x9, x9              # 	add	$r6, $r6, $r6
.LEloopC2:
	la	x6, __r7                # 	slt	$ta, $r6, $r7
	lw	x6, 0(x6)
	sltu	tp, x9, x6
	bnez	tp, .LEloopC            # 	bnez	$ta, .LEloopC
.LEround:
	addi	x9, x9, 128             # 	addi	$r6, $r6, #128
	sltiu	tp, x9, 128             # 	slti	$ta, $r6, #128
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
	srli	x15, x9, 8              # 	srli	$r5, $r6, #8
	andi	x15, x15, 1             # 	andi	$r5, $r5, #1
	sub	x9, x9, x15             # 	sub	$r6, $r6, $r5
	slli	x9, x9, 1               # 	slli	$r6, $r6, #1
	srli	x9, x9, 9               # 	srli	$r6, $r6, #9
	slli	x11, x14, 23            # 	slli	$r1, $r4, #23
	or	x9, x9, x11             # 	or	$r6, $r6, $r1
.LEpack:
	la	x6, __r7                # 	and	$r0, $r0, $r7
	lw	x6, 0(x6)
	and	x10, x10, x6
	or	x10, x10, x9            # 	or	$r0, $r0, $r6
.LEretA:
.LEret:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r9, {}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	addi	sp, sp, 16
	ret                             # 	ret5	$lp
.LEzeroP:
	beqz	x11, .LEretA            # 	beqz	$r1, .LEretA
.LEzero:
	li	x10, 0                  # 	move	$r0, #0
	tail	.LEret                  # 	b	.LEret
.LEinfnan:
	la	x6, __r7                # 	bne	$r6, $r7, .LEnan
	lw	x6, 0(x6)
	bne	x9, x6, .LEnan
	la	x5, __r8                # 	bnec	$r8, #255, .LEretA
	lw	x5, 0(x5)
	li	x6, 255
	bne	x5, x6, .LEretA
	beqz	x11, .LEretA            # 	beqz	$r1, .LEretA
.LEnan:
	li	x10, 4290772992         # 	move	$r0, #4290772992
	tail	.LEret                  # 	b	.LEret
.LEinf:
	li	x9, 2139095040          # 	move	$r6, #2139095040
	tail	.LEpack                 # 	b	.LEpack
.Ltmp2:
	.size	__subsf3, .Ltmp2-__subsf3
.Ltmp3:
	.size	__addsf3, .Ltmp3-__addsf3
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
#endif

#ifdef L_divsi3
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__divsi3
	.type	__divsi3,@function
__divsi3:
	slti	x15, x10, 0             # 	sltsi	$r5, $r0, #0
	sub	x14, zero, x10          # 	subri	$r4, $r0, #0
	beq	x15, zero, .Ltmp0       # 	cmovn	$r0, $r4, $r5
	addi	x10, x14, 0
.Ltmp0:
.L2:
	bgez	x11, .L3                # 	bgez	$r1, .L3
	sub	x11, zero, x11          # 	subri	$r1, $r1, #0
	li	x6, 1                   # 	subri	$r5, $r5, #1
	sub	x15, x6, x15
.L3:
	li	x12, 0                  # 	movi	$r2, #0
	beqz	x11, .L1                # 	beqz	$r1, .L1
	li	x14, 1                  # 	movi	$r4, #1
.L6:
	sltu	tp, x11, x10            # 	slt	$ta, $r1, $r0
	beqz	tp, .L5                 # 	beqz	$ta, .L5
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	slli	x14, x14, 1             # 	slli	$r4, $r4, #1
	tail	.L6                     # 	b	.L6
.L5:
	sltu	tp, x10, x11            # 	slt	$ta, $r0, $r1
	bnez	tp, .L9                 # 	bnez	$ta, .L9
	sub	x10, x10, x11           # 	sub	$r0, $r0, $r1
	or	x12, x12, x14           # 	or	$r2, $r2, $r4
.L9:
	srli	x14, x14, 1             # 	srli	$r4, $r4, #1
	srli	x11, x11, 1             # 	srli	$r1, $r1, #1
	bnez	x14, .L5                # 	bnez	$r4, .L5
.L1:
	sub	x10, zero, x12          # 	subri	$r0, $r2, #0
	bne	x15, zero, .Ltmp1       # 	cmovz	$r0, $r2, $r5
	addi	x10, x12, 0
.Ltmp1:
	ret                             # 	ret	$lp
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
	addi	sp, sp, -8              # 	push.s	$r6, $r6, {$lp}
	sw	x9, 0(sp)
	sw	ra, 4(sp)
	xor	x9, x11, x13            # 	xor	$r6, $r1, $r3
	srai	x9, x9, 31              # 	srai45	$r6, #31
	bgez	x13, .L80               # 	bgez	$r3, .L80
	sub	x13, zero, x13          # 	neg	$r3, $r3
	beqz	x12, .L80               # 	beqz	$r2, .L80
	sub	x12, zero, x12          # 	neg	$r2, $r2
	addi	x13, x13, -1            # 	addi	$r3, $r3, #-1
.L80:
	bgez	x11, .L81               # 	bgez	$r1, .L81
	sub	x11, zero, x11          # 	neg	$r1, $r1
	beqz	x10, .L81               # 	beqz	$r0, .L81
	sub	x10, zero, x10          # 	neg	$r0, $r0
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
.L81:
	li	x14, 0                  # 	movi	$r4, #0
	call	__udivmoddi4            # 	bal	__udivmoddi4
	beqz	x9, .L82                # 	beqz	$r6, .L82
	or	x14, x11, x10           # 	or	$r4, $r1, $r0
	beqz	x14, .L82               # 	beqz	$r4, .L82
	sub	x11, zero, x11          # 	neg	$r1, $r1
	beqz	x10, .L82               # 	beqz	$r0, .L82
	sub	x10, zero, x10          # 	neg	$r0, $r0
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
	.p2align	2
.L82:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r6, {$lp}
	lw	ra, 4(sp)
	addi	sp, sp, 8
	ret                             # 	ret	$lp
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
	slti	x15, x10, 0             # 	sltsi	$r5, $r0, #0
	sub	x14, zero, x10          # 	subri	$r4, $r0, #0
	beq	x15, zero, .Ltmp0       # 	cmovn	$r0, $r4, $r5
	addi	x10, x14, 0
.Ltmp0:
	bgez	x11, .L3                # 	bgez	$r1, .L3
	sub	x11, zero, x11          # 	subri	$r1, $r1, #0
.L3:
	beqz	x11, .L1                # 	beqz	$r1, .L1
	li	x14, 1                  # 	movi	$r4, #1
.L6:
	sltu	tp, x11, x10            # 	slt	$ta, $r1, $r0
	beqz	tp, .L5                 # 	beqz	$ta, .L5
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	slli	x14, x14, 1             # 	slli	$r4, $r4, #1
	tail	.L6                     # 	b	.L6
.L5:
	sub	x12, x10, x11           # 	sub	$r2, $r0, $r1
	sltu	tp, x10, x11            # 	slt	$ta, $r0, $r1
	srli	x14, x14, 1             # 	srli	$r4, $r4, #1
	bne	tp, zero, .Ltmp1        # 	cmovz	$r0, $r2, $ta
	addi	x10, x12, 0
.Ltmp1:
	srli	x11, x11, 1             # 	srli	$r1, $r1, #1
	bnez	x14, .L5                # 	bnez	$r4, .L5
.L1:
	sub	x13, zero, x10          # 	subri	$r3, $r0, #0
	beq	x15, zero, .Ltmp2       # 	cmovn	$r0, $r3, $r5
	addi	x10, x13, 0
.Ltmp2:
	ret                             # 	ret	$lp
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
	addi	sp, sp, -16             # 	addi	$sp, $sp, #-16
	sw	x9, 0(sp)               # 	smw.bi	$r6, [$sp], $r6, #2
	sw	ra, 4(sp)
	srai	x9, x11, 31             # 	srai	$r6, $r1, #31
	bgez	x13, .L80               # 	bgez	$r3, .L80
	sub	x13, zero, x13          # 	neg	$r3, $r3
	beqz	x12, .L80               # 	beqz	$r2, .L80
	sub	x12, zero, x12          # 	neg	$r2, $r2
	addi	x13, x13, -1            # 	addi	$r3, $r3, #-1
.L80:
	beqz	x9, .L81                # 	beqz	$r6, .L81
	sub	x11, zero, x11          # 	neg	$r1, $r1
	beqz	x10, .L81               # 	beqz	$r0, .L81
	sub	x10, zero, x10          # 	neg	$r0, $r0
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
.L81:
	addi	x14, sp, 8              # 	addi	$r4, $sp, #8
	call	__udivmoddi4            # 	bal	__udivmoddi4
	lw	x10, 8(sp)              # 	lwi	$r0, [$sp + #8]
	lw	x11, 12(sp)             # 	lwi	$r1, [$sp + #12]
	beqz	x9, .L82                # 	beqz	$r6, .L82
	or	x14, x11, x10           # 	or	$r4, $r1, $r0
	beqz	x14, .L82               # 	beqz	$r4, .L82
	sub	x11, zero, x11          # 	neg	$r1, $r1
	beqz	x10, .L82               # 	beqz	$r0, .L82
	sub	x10, zero, x10          # 	neg	$r0, $r0
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
	.p2align	2
.L82:
	lw	x9, 0(sp)               # 	lmw.bi	$r6, [$sp], $r6, #2
	lw	ra, 4(sp)
	addi	sp, sp, 16              # 	addi	$sp, $sp, #16
	ret                             # 	ret	$lp
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
	beqz	x10, .L7                # 	beqz	$r0, .L7
	mv	x12, x10                # 	move	$r2, $r0
	li	x10, 0                  # 	movi	$r0, #0
.L8:
	andi	x13, x12, 1             # 	andi	$r3, $r2, #1
	add	x14, x10, x11           # 	add	$r4, $r0, $r1
	beq	x13, zero, .Ltmp0       # 	cmovn	$r0, $r4, $r3
	addi	x10, x14, 0
.Ltmp0:
	srli	x12, x12, 1             # 	srli	$r2, $r2, #1
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	bnez	x12, .L8                # 	bnez	$r2, .L8
.L7:
	ret                             # 	ret	$lp
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
	li	x12, 0                  # 	movi	$r2, #0
	beqz	x11, .L1                # 	beqz	$r1, .L1
	li	x14, 1                  # 	movi	$r4, #1
.L6:
	sltu	tp, x11, x10            # 	slt	$ta, $r1, $r0
	beqz	tp, .L5                 # 	beqz	$ta, .L5
	bltz	x11, .L5                # 	bltz	$r1, .L5
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	slli	x14, x14, 1             # 	slli	$r4, $r4, #1
	tail	.L6                     # 	b	.L6
.L5:
	sltu	tp, x10, x11            # 	slt	$ta, $r0, $r1
	bnez	tp, .L9                 # 	bnez	$ta, .L9
	sub	x10, x10, x11           # 	sub	$r0, $r0, $r1
	or	x12, x12, x14           # 	or	$r2, $r2, $r4
.L9:
	srli	x14, x14, 1             # 	srli	$r4, $r4, #1
	srli	x11, x11, 1             # 	srli	$r1, $r1, #1
	bnez	x14, .L5                # 	bnez	$r4, .L5
.L1:
	mv	x10, x12                # 	move	$r0, $r2
	ret                             # 	ret	$lp
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
	li	x14, 0                  # 	movi	$r4, #0
	tail	__udivmoddi4            # 	b	__udivmoddi4
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
	slli	x6, x10, 16             # 	zeh	$r2, $r0
	srli	x12, x6, 16
	srli	x13, x10, 16            # 	srli	$r3, $r0, #16
	slli	x6, x11, 16             # 	zeh	$r0, $r1
	srli	x10, x6, 16
	srli	x11, x11, 16            # 	srli	$r1, $r1, #16
	mul	x15, x12, x11           # 	mul	$r5, $r2, $r1
	mul	x12, x12, x10           # 	mul	$r2, $r2, $r0
	mul	x10, x13, x10           # 	mul	$r0, $r3, $r0
	add	x15, x15, x10           # 	add	$r5, $r5, $r0
	sltu	tp, x15, x10            # 	slt	$ta, $r5, $r0
	slli	tp, tp, 16              # 	slli	$ta, $ta, #16
	mul	x6, x13, x11            # 	maddr32	$ta, $r3, $r1
	add	tp, tp, x6
	srli	x11, x15, 16            # 	srli	$r1, $r5, #16
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	slli	x10, x15, 16            # 	slli	$r0, $r5, #16
	add	x10, x10, x12           # 	add	$r0, $r0, $r2
	sltu	tp, x10, x12            # 	slt	$ta, $r0, $r2
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	ret                             # 	ret	$lp
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
	srli	x5, x12, 16             # 	srli	$r10, $r2, #16
	la	x7, __r10
	sw	x5, 0(x7)
	slli	x6, x10, 16             # 	zeh	$r5, $r0
	srli	x15, x6, 16
	srli	x10, x10, 16            # 	srli	$r0, $r0, #16
	la	x7, __r10               # 	divr	$r4, $r1, $r1, $r10
	lw	x7, 0(x7)
	divu	x14, x11, x7
	remu	x11, x11, x7
	slli	x6, x12, 16             # 	zeh	$r3, $r2
	srli	x13, x6, 16
	slli	x11, x11, 16            # 	slli	$r1, $r1, #16
	or	x11, x11, x10           # 	or	$r1, $r1, $r0
	mul	x10, x14, x13           # 	mul	$r0, $r4, $r3
	sltu	tp, x11, x10            # 	slt	$ta, $r1, $r0
	beqz	tp, .L2                 # 	beqz	$ta, .L2
	addi	x14, x14, -1            # 	addi	$r4, $r4, #-1
	add	x11, x11, x12           # 	add	$r1, $r1, $r2
	sltu	tp, x11, x12            # 	slt	$ta, $r1, $r2
	bnez	tp, .L2                 # 	bnez	$ta, .L2
	sltu	tp, x11, x10            # 	slt	$ta, $r1, $r0
	beqz	tp, .L2                 # 	beqz	$ta, .L2
	addi	x14, x14, -1            # 	addi	$r4, $r4, #-1
	add	x11, x11, x12           # 	add	$r1, $r1, $r2
.L2:
	sub	x11, x11, x10           # 	sub	$r1, $r1, $r0
	la	x7, __r10               # 	divr	$r1, $r0, $r1, $r10
	lw	x7, 0(x7)
	remu	x10, x11, x7
	divu	x11, x11, x7
	slli	x10, x10, 16            # 	slli	$r0, $r0, #16
	or	x10, x10, x15           # 	or	$r0, $r0, $r5
	mul	x13, x13, x11           # 	mul	$r3, $r3, $r1
	sltu	tp, x10, x13            # 	slt	$ta, $r0, $r3
	beqz	tp, .L5                 # 	beqz	$ta, .L5
	add	x10, x10, x12           # 	add	$r0, $r0, $r2
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
	sltu	tp, x10, x12            # 	slt	$ta, $r0, $r2
	bnez	tp, .L5                 # 	bnez	$ta, .L5
	sltu	tp, x10, x13            # 	slt	$ta, $r0, $r3
	beqz	tp, .L5                 # 	beqz	$ta, .L5
	add	x10, x10, x12           # 	add	$r0, $r0, $r2
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
.L5:
	sub	x10, x10, x13           # 	sub	$r0, $r0, $r3
	slli	x14, x14, 16            # 	slli	$r4, $r4, #16
	or	x11, x11, x14           # 	or	$r1, $r1, $r4
	ret                             # 	ret	$lp
.Ltmp0:
	.size	fudiv_qrnnd, .Ltmp0-fudiv_qrnnd
	.p2align	2
	.globl	__udivmoddi4
	.type	__udivmoddi4,@function
__udivmoddi4:
	addi	sp, sp, -40             # 	addi	$sp, $sp, #-40
	sw	x9, 0(sp)               # 	smw.bi	$r6, [$sp], $r10, #10
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	la	x7, __r10
	lw	x7, 0(x7)
	sw	x7, 16(sp)
	sw	x8, 20(sp)
	sw	ra, 24(sp)
	mv	x9, x10                 # 	movd44	$r6, $r0
	mv	x5, x11
	la	x7, __r7
	sw	x5, 0(x7)
	mv	x5, x12                 # 	movd44	$r8, $r2
	la	x7, __r8
	sw	x5, 0(x7)
	mv	x5, x13
	la	x7, __r9
	sw	x5, 0(x7)
	mv	x8, x14                 # 	move	$fp, $r4
	bnez	x13, .L9                # 	bnez	$r3, .L9
	la	x6, __r8                # 	slt	$ta, $r7, $r8
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	tp, .L10                # 	beqz	$ta, .L10
	la	x5, __r8                # 	move	$r0, $r8
	lw	x5, 0(x5)
	mv	x10, x5
	call	__clzsi2                # 	bal	__clzsi2
	sw	x10, 28(sp)             # 	swi	$r0, [$sp + #28]
	beqz	x10, .LZskipnorm1       # 	beqz	$r0, .LZskipnorm1
	la	x5, __r8                # 	sll	$r8, $r8, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r8
	sw	x5, 0(x7)
	li	x6, 32                  # 	subri	$r5, $r0, #32
	sub	x15, x6, x10
	srl	x15, x9, x15            # 	srl	$r5, $r6, $r5
	la	x5, __r7                # 	sll	$r7, $r7, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	or	$r7, $r7, $r5
	lw	x5, 0(x5)
	or	x5, x5, x15
	la	x7, __r7
	sw	x5, 0(x7)
	sll	x9, x9, x10             # 	sll	$r6, $r6, $r0
.LZskipnorm1:
	mv	x10, x9                 # 	movd44	$r0, $r6
	la	x5, __r7
	lw	x5, 0(x5)
	mv	x11, x5
	la	x5, __r8                # 	move	$r2, $r8
	lw	x5, 0(x5)
	mv	x12, x5
	call	fudiv_qrnnd             # 	bal	fudiv_qrnnd
	sw	x11, 32(sp)             # 	swi	$r1, [$sp + #32]
	mv	x9, x10                 # 	move	$r6, $r0
	li	x15, 0                  # 	move	$r5, #0
	sw	x15, 36(sp)             # 	swi	$r5, [$sp + #36]
	tail	.L19                    # 	b	.L19
.L10:
	beqz	x12, .LZdivzero         # 	beqz	$r2, .LZdivzero
	la	x5, __r8                # 	move	$r0, $r8
	lw	x5, 0(x5)
	mv	x10, x5
	call	__clzsi2                # 	bal	__clzsi2
	sw	x10, 28(sp)             # 	swi	$r0, [$sp + #28]
	bnez	x10, .LZnorm1           # 	bnez	$r0, .LZnorm1
	la	x6, __r8                # 	sub	$r7, $r7, $r8
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sub	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	li	x15, 1                  # 	movi	$r5, #1
	sw	x15, 36(sp)             # 	swi	$r5, [$sp + #36]
	tail	.L29                    # 	b	.L29
	.p2align	2
.LZnorm1:
	li	x6, 32                  # 	subri	$ta, $r0, #32
	sub	tp, x6, x10
	la	x5, __r8                # 	sll	$r8, $r8, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r8                # 	move	$r2, $r8
	lw	x5, 0(x5)
	mv	x12, x5
	srl	x14, x9, tp             # 	srl	$r4, $r6, $ta
	la	x5, __r7                # 	sll	$r5, $r7, $r0
	lw	x5, 0(x5)
	sll	x15, x5, x10
	sll	x9, x9, x10             # 	sll	$r6, $r6, $r0
	or	x10, x15, x14           # 	or	$r0, $r5, $r4
	la	x5, __r7                # 	srl	$r1, $r7, $ta
	lw	x5, 0(x5)
	srl	x11, x5, tp
	call	fudiv_qrnnd             # 	bal	fudiv_qrnnd
	sw	x11, 36(sp)             # 	swi	$r1, [$sp + #36]
	mv	x5, x10                 # 	move	$r7, $r0
	la	x7, __r7
	sw	x5, 0(x7)
.L29:
	mv	x10, x9                 # 	movd44	$r0, $r6
	la	x5, __r7
	lw	x5, 0(x5)
	mv	x11, x5
	la	x5, __r8                # 	move	$r2, $r8
	lw	x5, 0(x5)
	mv	x12, x5
	call	fudiv_qrnnd             # 	bal	fudiv_qrnnd
	sw	x11, 32(sp)             # 	swi	$r1, [$sp + #32]
	mv	x9, x10                 # 	move	$r6, $r0
	.p2align	2
.L19:
	beqz	x8, .LZsetq             # 	beqz	$fp, .LZsetq
	lw	x13, 28(sp)             # 	lwi	$r3, [$sp + #28]
	li	x5, 0                   # 	movi	$r7, #0
	la	x7, __r7
	sw	x5, 0(x7)
	srl	x9, x9, x13             # 	srl	$r6, $r6, $r3
	tail	.LZsetr                 # 	b	.LZsetr
	.p2align	2
.LZdivzero:
	la	x7, __r8                # 	divr	$r7, $r6, $r8, $r8
	lw	x7, 0(x7)
	la	x6, __r8
	lw	x6, 0(x6)
	divu	x5, x6, x7
	remu	x9, x6, x7
	la	x7, __r7
	sw	x5, 0(x7)
.LZqzero:
	li	x11, 0                  # 	movi	$r1, #0
	li	x10, 0                  # 	movi	$r0, #0
	beqz	x8, .LZret              # 	beqz	$fp, .LZret
	sw	x9, 0(x8)               # 	swi	$r6, [$fp + #0]
	la	x5, __r7                # 	swi	$r7, [$fp + #4]
	lw	x5, 0(x5)
	sw	x5, 4(x8)
	tail	.LZret                  # 	b	.LZret
.L9:
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	bnez	tp, .LZqzero            # 	bnez	$ta, .LZqzero
	la	x5, __r9                # 	move	$r0, $r9
	lw	x5, 0(x5)
	mv	x10, x5
	call	__clzsi2                # 	bal	__clzsi2
	sw	x10, 28(sp)             # 	swi	$r0, [$sp + #28]
	beqz	x10, .LZskipnorm2       # 	beqz	$r0, .LZskipnorm2
	li	x6, 32                  # 	subri	$r4, $r0, #32
	sub	x14, x6, x10
	la	x5, __r8                # 	srl	$r5, $r8, $r4
	lw	x5, 0(x5)
	srl	x15, x5, x14
	la	x5, __r9                # 	sll	$r2, $r9, $r0
	lw	x5, 0(x5)
	sll	x12, x5, x10
	or	x12, x12, x15           # 	or	$r2, $r2, $r5
	mv	x5, x12                 # 	move	$r9, $r2
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r8                # 	sll	$r8, $r8, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r8
	sw	x5, 0(x7)
	srl	x13, x9, x14            # 	srl	$r3, $r6, $r4
	sll	x9, x9, x10             # 	sll	$r6, $r6, $r0
	la	x5, __r7                # 	sll	$r0, $r7, $r0
	lw	x5, 0(x5)
	sll	x10, x5, x10
	la	x5, __r7                # 	srl	$r1, $r7, $r4
	lw	x5, 0(x5)
	srl	x11, x5, x14
	or	x10, x10, x13           # 	or	$r0, $r0, $r3
	call	fudiv_qrnnd             # 	bal	fudiv_qrnnd
	sw	x11, 32(sp)             # 	swi	$r1, [$sp + #32]
	mv	x5, x10                 # 	move	$r7, $r0
	la	x7, __r7
	sw	x5, 0(x7)
	la	x7, __r8                # 	mulr64	$r0, $r1, $r8
	lw	x7, 0(x7)
	mul	x10, x11, x7
	mulhu	x11, x11, x7
	la	x5, __r7                # 	slt	$ta, $r7, $r1
	lw	x5, 0(x5)
	sltu	tp, x5, x11
	bnez	tp, .L46                # 	bnez	$ta, .L46
	la	x6, __r7                # 	bne	$r1, $r7, .L45
	lw	x6, 0(x6)
	bne	x11, x6, .L45
	sltu	tp, x9, x10             # 	slt	$ta, $r6, $r0
	beqz	tp, .L45                # 	beqz	$ta, .L45
.L46:
	lw	x13, 32(sp)             # 	lwi	$r3, [$sp + #32]
	la	x6, __r9                # 	sub	$r1, $r1, $r9
	lw	x6, 0(x6)
	sub	x11, x11, x6
	addi	x13, x13, -1            # 	addi	$r3, $r3, #-1
	sw	x13, 32(sp)             # 	swi	$r3, [$sp + #32]
	la	x6, __r8                # 	sub	$r3, $r0, $r8
	lw	x6, 0(x6)
	sub	x13, x10, x6
	sltu	tp, x10, x13            # 	slt	$ta, $r0, $r3
	sub	x11, x11, tp            # 	sub	$r1, $r1, $ta
	mv	x10, x13                # 	move	$r0, $r3
.L45:
	li	x13, 0                  # 	movi	$r3, #0
	sw	x13, 36(sp)             # 	swi	$r3, [$sp + #36]
	beqz	x8, .LZsetq             # 	beqz	$fp, .LZsetq
	sub	x10, x9, x10            # 	sub	$r0, $r6, $r0
	la	x5, __r7                # 	sub	$r1, $r7, $r1
	lw	x5, 0(x5)
	sub	x11, x5, x11
	sltu	tp, x9, x10             # 	slt	$ta, $r6, $r0
	sub	x11, x11, tp            # 	sub	$r1, $r1, $ta
	lw	x13, 28(sp)             # 	lwi	$r3, [$sp + #28]
	li	x6, 32                  # 	subri	$r4, $r3, #32
	sub	x14, x6, x13
	sll	x5, x11, x14            # 	sll	$r7, $r1, $r4
	la	x7, __r7
	sw	x5, 0(x7)
	srl	x9, x10, x13            # 	srl	$r6, $r0, $r3
	la	x6, __r7                # 	or	$r6, $r6, $r7
	lw	x6, 0(x6)
	or	x9, x9, x6
	srl	x5, x11, x13            # 	srl	$r7, $r1, $r3
	la	x7, __r7
	sw	x5, 0(x7)
.LZsetr:
	sw	x9, 0(x8)               # 	swi	$r6, [$fp + #0]
	la	x5, __r7                # 	swi	$r7, [$fp + #4]
	lw	x5, 0(x5)
	sw	x5, 4(x8)
.LZsetq:
	lw	x10, 32(sp)             # 	lwi	$r0, [$sp + #32]
	lw	x11, 36(sp)             # 	lwi	$r1, [$sp + #36]
	.p2align	2
.LZret:
	lw	x9, 0(sp)               # 	lmw.bi	$r6, [$sp], $r10, #10
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	ra, 24(sp)
	addi	sp, sp, 40              # 	addi	$sp, $sp, #40
	ret                             # 	ret	$lp
.LZskipnorm2:
	li	x13, 0                  # 	move	$r3, #0
	la	x6, __r7                # 	slt	$ta, $r9, $r7
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	bnez	tp, .L52                # 	bnez	$ta, .L52
	la	x6, __r8                # 	slt	$ta, $r6, $r8
	lw	x6, 0(x6)
	sltu	tp, x9, x6
	bnez	tp, .L51                # 	bnez	$ta, .L51
.L52:
	li	x15, 1                  # 	move	$r5, #1
	sw	x15, 32(sp)             # 	swi	$r5, [$sp + #32]
	la	x6, __r8                # 	sub	$r4, $r6, $r8
	lw	x6, 0(x6)
	sub	x14, x9, x6
	la	x6, __r9                # 	sub	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sub	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	sltu	tp, x9, x14             # 	slt	$ta, $r6, $r4
	la	x5, __r7                # 	sub	$r7, $r7, $ta
	lw	x5, 0(x5)
	sub	x5, x5, tp
	la	x7, __r7
	sw	x5, 0(x7)
	mv	x9, x14                 # 	move	$r6, $r4
	tail	.L54                    # 	b	.L54
.L51:
	sw	x13, 32(sp)             # 	swi	$r3, [$sp + #32]
.L54:
	sw	x13, 36(sp)             # 	swi	$r3, [$sp + #36]
	bnez	x8, .LZsetr             # 	bnez	$fp, .LZsetr
	tail	.LZsetq                 # 	b	.LZsetq
.Ltmp1:
	.size	__udivmoddi4, .Ltmp1-__udivmoddi4
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
	.comm	__r10,4,4
#endif

#ifdef L_umodsi3
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__umodsi3
	.type	__umodsi3,@function
__umodsi3:
	beqz	x11, .L1                # 	beqz	$r1, .L1
	li	x14, 1                  # 	movi	$r4, #1
.L6:
	sltu	tp, x11, x10            # 	slt	$ta, $r1, $r0
	beqz	tp, .L5                 # 	beqz	$ta, .L5
	bltz	x11, .L5                # 	bltz	$r1, .L5
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	slli	x14, x14, 1             # 	slli	$r4, $r4, #1
	tail	.L6                     # 	b	.L6
.L5:
	sub	x12, x10, x11           # 	sub	$r2, $r0, $r1
	sltu	tp, x10, x11            # 	slt	$ta, $r0, $r1
	srli	x14, x14, 1             # 	srli	$r4, $r4, #1
	bne	tp, zero, .Ltmp0        # 	cmovz	$r0, $r2, $ta
	addi	x10, x12, 0
.Ltmp0:
	srli	x11, x11, 1             # 	srli	$r1, $r1, #1
	bnez	x14, .L5                # 	bnez	$r4, .L5
.L1:
	ret                             # 	ret	$lp
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
	addi	sp, sp, -12             # 	addi	$sp, $sp, #-12
	sw	ra, 0(sp)               # 	swi	$lp, [$sp + #0]
	addi	x14, sp, 4              # 	addi	$r4, $sp, #4
	call	__udivmoddi4            # 	bal	__udivmoddi4
	lw	x10, 4(sp)              # 	lwi	$r0, [$sp + #4]
	lw	x11, 8(sp)              # 	lwi	$r1, [$sp + #8]
	lw	ra, 0(sp)               # 	lwi.bi	$lp, [$sp], #12
	addi	sp, sp, 12
	ret                             # 	ret	$lp
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
	mul	x13, x13, x10           # 	mul	$r3, $r3, $r0
	mul	x6, x11, x12            # 	maddr32	$r3, $r1, $r2
	add	x13, x13, x6
	mulhu	x11, x10, x12           # 	mulr64	$r0, $r0, $r2
	mul	x10, x10, x12
	add	x11, x11, x13           # 	add	$r1, $r1, $r3
	ret                             # 	ret	$lp
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
	li	x14, 2147483648         # 	move	$r4, #2147483648
	xor	x13, x13, x14           # 	xor	$r3, $r3, $r4
	.globl	__adddf3
	.type	__adddf3,@function
__adddf3:
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	addi	sp, sp, -24             # 	push.s	$r6, $r10, {$lp}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	la	x7, __r10
	lw	x7, 0(x7)
	sw	x7, 16(sp)
	sw	ra, 20(sp)
	slli	x9, x13, 1              # 	slli	$r6, $r3, #1
	li	ra, 2147483648          # 	move	$lp, #2147483648
	sltu	tp, x14, x9             # 	slt	$ta, $r4, $r6
	bnez	tp, .LEswap             # 	bnez	$ta, .LEswap
	bne	x14, x9, .LEmain        # 	bne	$r4, $r6, .LEmain
	sltu	tp, x10, x12            # 	slt	$ta, $r0, $r2
	beqz	tp, .LEmain             # 	beqz	$ta, .LEmain
.LEswap:
	mv	x5, x10                 # 	movd44	$r8, $r0
	la	x7, __r8
	sw	x5, 0(x7)
	mv	x5, x11
	la	x7, __r9
	sw	x5, 0(x7)
	mv	x10, x12                # 	movd44	$r0, $r2
	mv	x11, x13
	la	x5, __r8                # 	movd44	$r2, $r8
	lw	x5, 0(x5)
	mv	x12, x5
	la	x5, __r9
	lw	x5, 0(x5)
	mv	x13, x5
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	slli	x9, x13, 1              # 	slli	$r6, $r3, #1
.LEmain:
	xor	x13, x13, x11           # 	xor	$r3, $r3, $r1
	and	x13, x13, ra            # 	and	$r3, $r3, $lp
	srli	x5, x14, 21             # 	srli	$r7, $r4, #21
	la	x7, __r7
	sw	x5, 0(x7)
	srli	x5, x9, 21              # 	srli	$r10, $r6, #21
	la	x7, __r10
	sw	x5, 0(x7)
	slli	x5, x14, 10             # 	slli	$r9, $r4, #10
	la	x7, __r9
	sw	x5, 0(x7)
	slli	x5, x9, 10              # 	slli	$r8, $r6, #10
	la	x7, __r8
	sw	x5, 0(x7)
	li	x15, 2047               # 	move	$r5, #2047
	la	x6, __r7                # 	beq	$r5, $r7, .LEinfnan
	lw	x6, 0(x6)
	beq	x15, x6, .LEinfnan
	or	tp, x14, x10            # 	or	$ta, $r4, $r0
	beqz	tp, .LEzeroP            # 	beqz	$ta, .LEzeroP
	or	tp, x9, x12             # 	or	$ta, $r6, $r2
	beqz	tp, .LEretA             # 	beqz	$ta, .LEretA
	la	x6, __r10               # 	sub	$r6, $r7, $r10
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sub	x9, x5, x6
	sltiu	tp, x9, 64              # 	slti	$ta, $r6, #64
	beqz	tp, .LEretA             # 	beqz	$ta, .LEretA
	srli	x15, x10, 21            # 	srli	$r5, $r0, #21
	la	x5, __r9                # 	or	$r9, $r9, $r5
	lw	x5, 0(x5)
	or	x5, x5, x15
	la	x7, __r9
	sw	x5, 0(x7)
	slli	x10, x10, 11            # 	slli	$r0, $r0, #11
	srli	x15, x12, 21            # 	srli	$r5, $r2, #21
	la	x5, __r8                # 	or	$r8, $r8, $r5
	lw	x5, 0(x5)
	or	x5, x5, x15
	la	x7, __r8
	sw	x5, 0(x7)
	slli	x12, x12, 11            # 	slli	$r2, $r2, #11
	la	x5, __r7                # 	slti	$ta, $r7, #2
	lw	x5, 0(x5)
	sltiu	tp, x5, 2
	bnez	tp, .LEmain4            # 	bnez	$ta, .LEmain4
	la	x5, __r9                # 	or	$r9, $r9, $lp
	lw	x5, 0(x5)
	or	x5, x5, ra
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r10               # 	beqz	$r10, .LEmain1
	lw	x5, 0(x5)
	beqz	x5, .LEmain1
	la	x5, __r8                # 	or	$r8, $r8, $lp
	lw	x5, 0(x5)
	or	x5, x5, ra
	la	x7, __r8
	sw	x5, 0(x7)
.LEmain1:
	addi	x15, x9, -1             # 	addi	$r5, $r6, #-1
	la	x6, __r10               # 	cmovz	$r6, $r5, $r10
	lw	x6, 0(x6)
	bne	x6, zero, .Ltmp0
	addi	x9, x15, 0
.Ltmp0:
	beqz	x9, .LEmain4            # 	beqz	$r6, .LEmain4
	li	x6, 32                  # 	subri	$r5, $r6, #32
	sub	x15, x6, x9
	blez	x15, .LEmain2           # 	blez	$r5, .LEmain2
	sll	x14, x12, x15           # 	sll	$r4, $r2, $r5
	la	x5, __r8                # 	sll	$r5, $r8, $r5
	lw	x5, 0(x5)
	sll	x15, x5, x15
	la	x5, __r8                # 	srl	$r8, $r8, $r6
	lw	x5, 0(x5)
	srl	x5, x5, x9
	la	x7, __r8
	sw	x5, 0(x7)
	srl	x12, x12, x9            # 	srl	$r2, $r2, $r6
	or	x12, x12, x15           # 	or	$r2, $r2, $r5
	tail	.LEmain3                # 	b	.LEmain3
.LEmain2:
	sub	x9, zero, x15           # 	subri	$r6, $r5, #0
	addi	x15, x15, 32            # 	addi	$r5, $r5, #32
	la	x5, __r8                # 	sll	$r4, $r8, $r5
	lw	x5, 0(x5)
	sll	x14, x5, x15
	or	x14, x14, x12           # 	or	$r4, $r4, $r2
	bne	x9, zero, .Ltmp1        # 	cmovz	$r4, $r6, $r6
	addi	x14, x9, 0
.Ltmp1:
	la	x5, __r8                # 	srl	$r2, $r8, $r6
	lw	x5, 0(x5)
	srl	x12, x5, x9
	li	x5, 0                   # 	move	$r8, #0
	la	x7, __r8
	sw	x5, 0(x7)
.LEmain3:
	beqz	x14, .LEmain4           # 	beqz	$r4, .LEmain4
	ori	x12, x12, 2             # 	ori	$r2, $r2, #2
.LEmain4:
	beqz	x13, .LEadd             # 	beqz	$r3, .LEadd
	la	x6, __r10               # 	bne	$r7, $r10, .LEsub1
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	bne	x5, x6, .LEsub1
	la	x6, __r8                # 	bne	$r9, $r8, .LEsub1
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	bne	x5, x6, .LEsub1
	beq	x10, x12, .LEzero       # 	beq	$r0, $r2, .LEzero
.LEsub1:
	la	x6, __r8                # 	slt	$ta, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	bnez	tp, .LEsub2             # 	bnez	$ta, .LEsub2
	la	x6, __r8                # 	bne	$r9, $r8, .LEsub3
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	bne	x5, x6, .LEsub3
	sltu	tp, x10, x12            # 	slt	$ta, $r0, $r2
	beqz	tp, .LEsub3             # 	beqz	$ta, .LEsub3
.LEsub2:
	la	x5, __r7                # 	addi	$r7, $r7, #-1
	lw	x5, 0(x5)
	addi	x5, x5, -1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r8                # 	slli	$r4, $r8, #31
	lw	x5, 0(x5)
	slli	x14, x5, 31
	la	x5, __r8                # 	srli	$r8, $r8, #1
	lw	x5, 0(x5)
	srli	x5, x5, 1
	la	x7, __r8
	sw	x5, 0(x7)
	srli	x12, x12, 1             # 	srli	$r2, $r2, #1
	or	x12, x12, x14           # 	or	$r2, $r2, $r4
.LEsub3:
	mv	x14, x10                # 	move	$r4, $r0
	sub	x10, x10, x12           # 	sub	$r0, $r0, $r2
	sltu	tp, x14, x10            # 	slt	$ta, $r4, $r0
	la	x5, __r9                # 	sub	$r9, $r9, $ta
	lw	x5, 0(x5)
	sub	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x6, __r8                # 	sub	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	sub	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r7                # 	slti	$ta, $r7, #2
	lw	x5, 0(x5)
	sltiu	tp, x5, 2
	bnez	tp, .LEdenorm           # 	bnez	$ta, .LEdenorm
	la	x5, __r9                # 	bnez	$r9, .LEsub4
	lw	x5, 0(x5)
	bnez	x5, .LEsub4
	la	x5, __r7                # 	slti	$ta, $r7, #32
	lw	x5, 0(x5)
	sltiu	tp, x5, 32
	bnez	tp, .LEsub4             # 	bnez	$ta, .LEsub4
	mv	x5, x10                 # 	move	$r9, $r0
	la	x7, __r9
	sw	x5, 0(x7)
	li	x10, 0                  # 	move	$r0, #0
	la	x5, __r7                # 	addi	$r7, $r7, #-32
	lw	x5, 0(x5)
	addi	x5, x5, -32
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	bnez	$r7, .LEsub4
	lw	x5, 0(x5)
	bnez	x5, .LEsub4
	tail	.LEround                # 	b	.LEround
.LEloop:
	la	x5, __r7                # 	addi	$r7, $r7, #-1
	lw	x5, 0(x5)
	addi	x5, x5, -1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	beqz	$r7, .LEround
	lw	x5, 0(x5)
	beqz	x5, .LEround
	srli	x14, x10, 31            # 	srli	$r4, $r0, #31
	la	x5, __r9                # 	slli	$r9, $r9, #1
	lw	x5, 0(x5)
	slli	x5, x5, 1
	la	x7, __r9
	sw	x5, 0(x7)
	slli	x10, x10, 1             # 	slli	$r0, $r0, #1
	la	x5, __r9                # 	or	$r9, $r9, $r4
	lw	x5, 0(x5)
	or	x5, x5, x14
	la	x7, __r9
	sw	x5, 0(x7)
.LEsub4:
	la	x5, __r9                # 	slt	$ta, $r9, $lp
	lw	x5, 0(x5)
	sltu	tp, x5, ra
	bnez	tp, .LEloop             # 	bnez	$ta, .LEloop
.LEround:
	addi	x10, x10, 1024          # 	addi	$r0, $r0, #1024
	sltiu	tp, x10, 1024           # 	slti	$ta, $r0, #1024
	la	x5, __r9                # 	add	$r9, $r9, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slt	$ta, $r9, $ta
	lw	x5, 0(x5)
	sltu	tp, x5, tp
	la	x5, __r7                # 	add	$r7, $r7, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r7
	sw	x5, 0(x7)
	srli	x15, x10, 11            # 	srli	$r5, $r0, #11
	andi	x15, x15, 1             # 	andi	$r5, $r5, #1
	mv	x9, x10                 # 	move	$r6, $r0
	sub	x10, x10, x15           # 	sub	$r0, $r0, $r5
	sltu	tp, x9, x10             # 	slt	$ta, $r6, $r0
	la	x5, __r9                # 	sub	$r9, $r9, $ta
	lw	x5, 0(x5)
	sub	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slli	$r9, $r9, #1
	lw	x5, 0(x5)
	slli	x5, x5, 1
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slli	$r5, $r9, #20
	lw	x5, 0(x5)
	slli	x15, x5, 20
	la	x5, __r9                # 	srli	$r9, $r9, #12
	lw	x5, 0(x5)
	srli	x5, x5, 12
	la	x7, __r9
	sw	x5, 0(x7)
	srli	x10, x10, 11            # 	srli	$r0, $r0, #11
	or	x10, x10, x15           # 	or	$r0, $r0, $r5
	la	x5, __r7                # 	slli	$r5, $r7, #20
	lw	x5, 0(x5)
	slli	x15, x5, 20
	la	x5, __r9                # 	or	$r9, $r9, $r5
	lw	x5, 0(x5)
	or	x5, x5, x15
	la	x7, __r9
	sw	x5, 0(x7)
.LEpack:
	and	x11, x11, ra            # 	and	$r1, $r1, $lp
	la	x6, __r9                # 	or	$r1, $r1, $r9
	lw	x6, 0(x6)
	or	x11, x11, x6
.LEretA:
.LEret:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r10, {$lp}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	ra, 20(sp)
	addi	sp, sp, 24
	ret                             # 	ret5	$lp
.LEadd:
	add	x10, x10, x12           # 	add	$r0, $r0, $r2
	sltu	tp, x10, x12            # 	slt	$ta, $r0, $r2
	la	x5, __r9                # 	add	$r9, $r9, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slt	$ta, $r9, $ta
	lw	x5, 0(x5)
	sltu	tp, x5, tp
	la	x6, __r8                # 	add	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	bnez	tp, .LEaddover          # 	bnez	$ta, .LEaddover
	la	x6, __r8                # 	slt	$ta, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	bnez	tp, .LEaddover          # 	bnez	$ta, .LEaddover
	la	x5, __r7                # 	bnez	$r7, .LEround
	lw	x5, 0(x5)
	bnez	x5, .LEround
.LEdenorm:
	srli	x10, x10, 11            # 	srli	$r0, $r0, #11
	la	x5, __r9                # 	slli	$r4, $r9, #21
	lw	x5, 0(x5)
	slli	x14, x5, 21
	la	x5, __r9                # 	srli	$r9, $r9, #11
	lw	x5, 0(x5)
	srli	x5, x5, 11
	la	x7, __r9
	sw	x5, 0(x7)
	or	x10, x10, x14           # 	or	$r0, $r0, $r4
	tail	.LEpack                 # 	b	.LEpack
.LEaddover:
	la	x5, __r7                # 	subri	$r5, $r7, #2046
	lw	x5, 0(x5)
	li	x6, 2046
	sub	x15, x6, x5
	beqz	x15, .LEinf             # 	beqz	$r5, .LEinf
	andi	tp, x10, 1              # 	andi	$ta, $r0, #1
	ori	x15, x10, 2             # 	ori	$r5, $r0, #2
	beq	tp, zero, .Ltmp2        # 	cmovn	$r0, $r5, $ta
	addi	x10, x15, 0
.Ltmp2:
	la	x5, __r9                # 	slli	$r4, $r9, #31
	lw	x5, 0(x5)
	slli	x14, x5, 31
	la	x5, __r9                # 	srli	$r9, $r9, #1
	lw	x5, 0(x5)
	srli	x5, x5, 1
	la	x7, __r9
	sw	x5, 0(x7)
	srli	x10, x10, 1             # 	srli	$r0, $r0, #1
	or	x10, x10, x14           # 	or	$r0, $r0, $r4
	la	x5, __r7                # 	addi	$r7, $r7, #1
	lw	x5, 0(x5)
	addi	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	tail	.LEround                # 	b	.LEround
.LEinf:
	li	x10, 0                  # 	move	$r0, #0
	li	x5, 2146435072          # 	move	$r9, #2146435072
	la	x7, __r9
	sw	x5, 0(x7)
	tail	.LEpack                 # 	b	.LEpack
.LEzeroP:
	beqz	x13, .LEretA            # 	beqz	$r3, .LEretA
.LEzero:
	li	x10, 0                  # 	move	$r0, #0
	li	x11, 0                  # 	move	$r1, #0
	tail	.LEret                  # 	b	.LEret
.LEinfnan:
	la	x5, __r9                # 	or	$r9, $r9, $r0
	lw	x5, 0(x5)
	or	x5, x5, x10
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	bne	$r9, $lp, .LEnan
	lw	x5, 0(x5)
	bne	x5, ra, .LEnan
	la	x6, __r10               # 	bne	$r5, $r10, .LEretA
	lw	x6, 0(x6)
	bne	x15, x6, .LEretA
	beqz	x13, .LEretA            # 	beqz	$r3, .LEretA
.LEnan:
	li	x10, 0                  # 	move	$r0, #0
	li	x11, 4294443008         # 	move	$r1, #4294443008
	tail	.LEret                  # 	b	.LEret
.Ltmp3:
	.size	__subdf3, .Ltmp3-__subdf3
.Ltmp4:
	.size	__adddf3, .Ltmp4-__adddf3
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
	.comm	__r10,4,4
#endif

#ifdef L_mul_sf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__mulsf3
	.type	__mulsf3,@function
__mulsf3:
	addi	sp, sp, -16             # 	push.s	$r6, $r8, {$lp}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	sw	ra, 12(sp)
	xor	x5, x11, x10            # 	xor	$r8, $r1, $r0
	la	x7, __r8
	sw	x5, 0(x7)
	li	x14, 2147483648         # 	move	$r4, #2147483648
	la	x5, __r8                # 	and	$r8, $r8, $r4
	lw	x5, 0(x5)
	and	x5, x5, x14
	la	x7, __r8
	sw	x5, 0(x7)
	slli	x13, x10, 1             # 	slli	$r3, $r0, #1
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	srli	x9, x13, 24             # 	srli	$r6, $r3, #24
	srli	x5, x11, 24             # 	srli	$r7, $r1, #24
	la	x7, __r7
	sw	x5, 0(x7)
	slli	x12, x13, 7             # 	slli	$r2, $r3, #7
	slli	x10, x11, 7             # 	slli	$r0, $r1, #7
	beqz	x9, .LFzeroAexp         # 	beqz	$r6, .LFzeroAexp
	li	x6, 255                 # 	beqc	$r6, #255, .LFinfnanA
	beq	x9, x6, .LFinfnanA
	or	x12, x12, x14           # 	or	$r2, $r2, $r4
.LFlab1:
	la	x5, __r7                # 	beqz	$r7, .LFzeroB
	lw	x5, 0(x5)
	beqz	x5, .LFzeroB
	la	x5, __r7                # 	beqc	$r7, #255, .LFinfnanB
	lw	x5, 0(x5)
	li	x6, 255
	beq	x5, x6, .LFinfnanB
	or	x10, x10, x14           # 	or	$r0, $r0, $r4
.LFlab2:
	mulhu	x11, x12, x10           # 	mulr64	$r0, $r2, $r0
	mul	x10, x12, x10
	ori	x12, x11, 1             # 	ori	$r2, $r1, #1
	bne	x10, zero, .Ltmp0       # 	cmovz	$r2, $r1, $r0
	addi	x12, x11, 0
.Ltmp0:
	slti	tp, x12, 0              # 	sltsi	$ta, $r2, #0
	bnez	tp, .Li18               # 	bnezs8	.Li18
	slli	x12, x12, 1             # 	slli	$r2, $r2, #1
	addi	x9, x9, -1              # 	addi	$r6, $r6, #-1
.Li18:
	la	x5, __r7                # 	addi	$r5, $r7, #-126
	lw	x5, 0(x5)
	addi	x15, x5, -126
	add	x9, x9, x15             # 	add	$r6, $r6, $r5
	blez	x9, .LFunder            # 	blez	$r6, .LFunder
	sltiu	tp, x9, 255             # 	slti	$ta, $r6, #255
	beqz	tp, .LFinf              # 	beqz	$ta, .LFinf
.LFround:
	addi	x12, x12, 128           # 	addi	$r2, $r2, #128
	sltiu	tp, x12, 128            # 	slti	$ta, $r2, #128
	add	x9, x9, tp              # 	add	$r6, $r6, $ta
	srli	x15, x12, 8             # 	srli	$r5, $r2, #8
	andi	x15, x15, 1             # 	andi	$r5, $r5, #1
	sub	x12, x12, x15           # 	sub	$r2, $r2, $r5
	slli	x12, x12, 1             # 	slli	$r2, $r2, #1
	srli	x12, x12, 9             # 	srli	$r2, $r2, #9
	slli	x10, x9, 23             # 	slli	$r0, $r6, #23
	or	x10, x10, x12           # 	or	$r0, $r0, $r2
.LFpack:
	la	x6, __r8                # 	or	$r0, $r0, $r8
	lw	x6, 0(x6)
	or	x10, x10, x6
.LFret:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r8, {$lp}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret                             # 	ret5	$lp
.LFzeroAexp:
	bnez	x12, .LFloopA2          # 	bnez	$r2, .LFloopA2
	la	x5, __r7                # 	beqc	$r7, #255, .LFnan
	lw	x5, 0(x5)
	li	x6, 255
	beq	x5, x6, .LFnan
.LFzero:
	la	x5, __r8                # 	move	$r0, $r8
	lw	x5, 0(x5)
	mv	x10, x5
	tail	.LFret                  # 	b	.LFret
.LFloopA:
	addi	x9, x9, -1              # 	addi	$r6, $r6, #-1
.LFloopA2:
	add	x12, x12, x12           # 	add	$r2, $r2, $r2
	sltu	tp, x12, x14            # 	slt	$ta, $r2, $r4
	bnez	tp, .LFloopA            # 	bnez	$ta, .LFloopA
	tail	.LFlab1                 # 	b	.LFlab1
.LFinfnanA:
	bne	x12, x14, .LFnan        # 	bne	$r2, $r4, .LFnan
	beqz	x11, .LFnan             # 	beqz	$r1, .LFnan
	la	x5, __r7                # 	bnec	$r7, #255, .LFinf
	lw	x5, 0(x5)
	li	x6, 255
	bne	x5, x6, .LFinf
.LFinfnanB:
	bne	x10, x14, .LFnan        # 	bne	$r0, $r4, .LFnan
.LFinf:
	li	x10, 2139095040         # 	move	$r0, #2139095040
	tail	.LFpack                 # 	b	.LFpack
.LFzeroB:
	bnez	x10, .LFloopB2          # 	bnez	$r0, .LFloopB2
	tail	.LFzero                 # 	b	.LFzero
.LFloopB:
	la	x5, __r7                # 	addi	$r7, $r7, #-1
	lw	x5, 0(x5)
	addi	x5, x5, -1
	la	x7, __r7
	sw	x5, 0(x7)
.LFloopB2:
	add	x10, x10, x10           # 	add	$r0, $r0, $r0
	sltu	tp, x10, x14            # 	slt	$ta, $r0, $r4
	bnez	tp, .LFloopB            # 	bnez	$ta, .LFloopB
	tail	.LFlab2                 # 	b	.LFlab2
.LFnan:
	li	x10, 4290772992         # 	move	$r0, #4290772992
	tail	.LFret                  # 	b	.LFret
.LFunder:
	li	x6, 1                   # 	subri	$r1, $r6, #1
	sub	x11, x6, x9
	sltiu	tp, x11, 32             # 	slti	$ta, $r1, #32
	beqz	tp, .LFzero             # 	beqzs8	.LFzero
	li	x6, 32                  # 	subri	$r5, $r1, #32
	sub	x15, x6, x11
	sll	x9, x12, x15            # 	sll	$r6, $r2, $r5
	srl	x12, x12, x11           # 	srl	$r2, $r2, $r1
	beqz	x9, .LFunder2           # 	beqz	$r6, .LFunder2
	ori	x12, x12, 2             # 	ori	$r2, $r2, #2
.LFunder2:
	addi	x15, x12, 128           # 	addi	$r5, $r2, #128
	slti	x9, x15, 0              # 	sltsi	$r6, $r5, #0
	tail	.LFround                # 	b	.LFround
.Ltmp1:
	.size	__mulsf3, .Ltmp1-__mulsf3
	.comm	__r7,4,4
	.comm	__r8,4,4
#endif

#ifdef L_mul_df
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__muldf3
	.type	__muldf3,@function
__muldf3:
	addi	sp, sp, -32             # 	push25	$r10, #16
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	la	x7, __r10
	lw	x7, 0(x7)
	sw	x7, 16(sp)
	sw	x8, 20(sp)
	sw	gp, 24(sp)
	sw	ra, 28(sp)
	addi	sp, sp, -16
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	srli	x14, x14, 21            # 	srli	$r4, $r4, #21
	slli	x5, x11, 11             # 	slli	$r7, $r1, #11
	la	x7, __r7
	sw	x5, 0(x7)
	srli	x9, x10, 21             # 	srli	$r6, $r0, #21
	la	x5, __r7                # 	or	$r7, $r7, $r6
	lw	x5, 0(x5)
	or	x5, x5, x9
	la	x7, __r7
	sw	x5, 0(x7)
	slli	x9, x10, 11             # 	slli	$r6, $r0, #11
	li	x8, 2147483648          # 	move	$fp, #2147483648
	slli	x15, x13, 1             # 	slli	$r5, $r3, #1
	srli	x15, x15, 21            # 	srli	$r5, $r5, #21
	slli	x5, x13, 11             # 	slli	$r9, $r3, #11
	la	x7, __r9
	sw	x5, 0(x7)
	srli	x5, x12, 21             # 	srli	$r8, $r2, #21
	la	x7, __r8
	sw	x5, 0(x7)
	la	x6, __r8                # 	or	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	slli	x5, x12, 11             # 	slli	$r8, $r2, #11
	la	x7, __r8
	sw	x5, 0(x7)
	xor	x13, x13, x11           # 	xor	$r3, $r3, $r1
	and	x5, x13, x8             # 	and	$r10, $r3, $fp
	la	x7, __r10
	sw	x5, 0(x7)
	li	x13, 2047               # 	move	$r3, #2047
	beqz	x14, .LFAexpzero        # 	beqz	$r4, .LFAexpzero
	beq	x13, x14, .LFAinfnan    # 	beq	$r3, $r4, .LFAinfnan
	la	x5, __r7                # 	or	$r7, $r7, $fp
	lw	x5, 0(x5)
	or	x5, x5, x8
	la	x7, __r7
	sw	x5, 0(x7)
.LFmain1:
	beqz	x15, .LFBexpzero        # 	beqz	$r5, .LFBexpzero
	beq	x13, x15, .LFBinfnan    # 	beq	$r3, $r5, .LFBinfnan
	la	x5, __r9                # 	or	$r9, $r9, $fp
	lw	x5, 0(x5)
	or	x5, x5, x8
	la	x7, __r9
	sw	x5, 0(x7)
.LFmain2:
	la	x5, __r10               # 	swi	$r10, [$sp + #12]
	lw	x5, 0(x5)
	sw	x5, 12(sp)
	addi	x10, x15, -1022         # 	addi	$r0, $r5, #-1022
	add	x14, x14, x10           # 	add	$r4, $r4, $r0
	la	x7, __r9                # 	mulr64	$r2, $r7, $r9
	lw	x7, 0(x7)
	la	x6, __r7
	lw	x6, 0(x6)
	mulhu	x13, x6, x7
	mul	x12, x6, x7
	la	x7, __r9                # 	mulr64	$r0, $r6, $r9
	lw	x7, 0(x7)
	mulhu	x11, x9, x7
	mul	x10, x9, x7
	add	x5, x12, x11            # 	add	$r9, $r2, $r1
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slt	$ta, $r9, $r1
	lw	x5, 0(x5)
	sltu	tp, x5, x11
	add	x5, x13, tp             # 	add	$r10, $r3, $ta
	la	x7, __r10
	sw	x5, 0(x7)
	la	x7, __r8                # 	mulr64	$r2, $r7, $r8
	lw	x7, 0(x7)
	la	x6, __r7
	lw	x6, 0(x6)
	mulhu	x13, x6, x7
	mul	x12, x6, x7
	add	x5, x12, x10            # 	add	$r7, $r2, $r0
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	slt	$ta, $r7, $r0
	lw	x5, 0(x5)
	sltu	tp, x5, x10
	la	x5, __r9                # 	add	$r9, $r9, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slt	$ta, $r9, $ta
	lw	x5, 0(x5)
	sltu	tp, x5, tp
	la	x5, __r10               # 	add	$r10, $r10, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r10
	sw	x5, 0(x7)
	la	x5, __r9                # 	add	$r9, $r9, $r3
	lw	x5, 0(x5)
	add	x5, x5, x13
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slt	$ta, $r9, $r3
	lw	x5, 0(x5)
	sltu	tp, x5, x13
	la	x5, __r10               # 	add	$r10, $r10, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r10
	sw	x5, 0(x7)
	la	x7, __r8                # 	mulr64	$r0, $r6, $r8
	lw	x7, 0(x7)
	mulhu	x11, x9, x7
	mul	x10, x9, x7
	la	x5, __r7                # 	add	$r7, $r7, $r1
	lw	x5, 0(x5)
	add	x5, x5, x11
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	slt	$ta, $r7, $r1
	lw	x5, 0(x5)
	sltu	tp, x5, x11
	la	x5, __r9                # 	add	$r9, $r9, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	slt	$ta, $r9, $ta
	lw	x5, 0(x5)
	sltu	tp, x5, tp
	la	x5, __r10               # 	add	$r1, $r10, $ta
	lw	x5, 0(x5)
	add	x11, x5, tp
	la	x5, __r7                # 	or	$r7, $r7, $r0
	lw	x5, 0(x5)
	or	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r9                # 	ori	$r0, $r9, #1
	lw	x5, 0(x5)
	ori	x10, x5, 1
	la	x6, __r7                # 	cmovz	$r0, $r9, $r7
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	bne	x6, zero, .Ltmp0
	addi	x10, x5, 0
.Ltmp0:
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	bnez	tp, .LFmain3            # 	bnez	$ta, .LFmain3
	mv	tp, x10                 # 	move	$ta, $r0
	add	x10, x10, x10           # 	add	$r0, $r0, $r0
	sltu	tp, x10, tp             # 	slt	$ta, $r0, $ta
	add	x11, x11, x11           # 	add	$r1, $r1, $r1
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	addi	x14, x14, -1            # 	addi	$r4, $r4, #-1
.LFmain3:
	lw	x5, 12(sp)              # 	lwi	$r10, [$sp + #12]
	la	x7, __r10
	sw	x5, 0(x7)
	blez	x14, .LFunderflow       # 	blez	$r4, .LFunderflow
	li	x6, 2047                # 	subri	$r5, $r4, #2047
	sub	x15, x6, x14
	blez	x15, .LFinf             # 	blez	$r5, .LFinf
	addi	x10, x10, 1024          # 	addi	$r0, $r0, #1024
	sltiu	tp, x10, 1024           # 	slti	$ta, $r0, #1024
	beqz	tp, .LFround            # 	beqz	$ta, .LFround
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	sltu	tp, x11, tp             # 	slt	$ta, $r1, $ta
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
.LFround:
	srli	x12, x10, 11            # 	srli	$r2, $r0, #11
	andi	x12, x12, 1             # 	andi	$r2, $r2, #1
	sub	x10, x10, x12           # 	sub	$r0, $r0, $r2
	srli	x10, x10, 11            # 	srli	$r0, $r0, #11
	slli	x12, x11, 21            # 	slli	$r2, $r1, #21
	or	x10, x10, x12           # 	or	$r0, $r0, $r2
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	srli	x11, x11, 12            # 	srli	$r1, $r1, #12
	slli	x15, x14, 20            # 	slli	$r5, $r4, #20
	or	x11, x11, x15           # 	or	$r1, $r1, $r5
.LFret:
	la	x6, __r10               # 	or	$r1, $r1, $r10
	lw	x6, 0(x6)
	or	x11, x11, x6
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LFAexpzero:
	la	x5, __r7                # 	or	$ta, $r7, $r6
	lw	x5, 0(x5)
	or	tp, x5, x9
	beqz	tp, .LFAzero            # 	beqz	$ta, .LFAzero
	srli	tp, x9, 31              # 	srli	$ta, $r6, #31
	la	x6, __r7                # 	add	$r7, $r7, $r7
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	add	$r7, $r7, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r7
	sw	x5, 0(x7)
	add	x9, x9, x9              # 	add	$r6, $r6, $r6
	la	x5, __r7                # 	bnez	$r7, .LFAcont
	lw	x5, 0(x5)
	bnez	x5, .LFAcont
	mv	x5, x9                  # 	move	$r7, $r6
	la	x7, __r7
	sw	x5, 0(x7)
	li	x9, 0                   # 	move	$r6, #0
	addi	x14, x14, -32           # 	addi	$r4, $r4, #-32
.LFAcont:
	li	x10, 0                  # 	move	$r0, #0
	la	x5, __r7                # 	move	$r1, $r7
	lw	x5, 0(x5)
	mv	x11, x5
	tail	.LFAloop2               # 	b	.LFAloop2
.LFAloop:
	add	x11, x11, x11           # 	add	$r1, $r1, $r1
	addi	x10, x10, 1             # 	addi	$r0, $r0, #1
.LFAloop2:
	sltu	tp, x11, x8             # 	slt	$ta, $r1, $fp
	bnez	tp, .LFAloop            # 	bnez	$ta, .LFAloop
	beqz	x10, .LFmain1           # 	beqz	$r0, .LFmain1
	sub	x14, x14, x10           # 	sub	$r4, $r4, $r0
	li	x6, 32                  # 	subri	$r2, $r0, #32
	sub	x12, x6, x10
	srl	x12, x9, x12            # 	srl	$r2, $r6, $r2
	sll	x9, x9, x10             # 	sll	$r6, $r6, $r0
	la	x5, __r7                # 	sll	$r7, $r7, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	or	$r7, $r7, $r2
	lw	x5, 0(x5)
	or	x5, x5, x12
	la	x7, __r7
	sw	x5, 0(x7)
	tail	.LFmain1                # 	b	.LFmain1
.LFAzero:
	beq	x13, x15, .LFnan        # 	beq	$r3, $r5, .LFnan
.LFsetsign:
	la	x5, __r10               # 	move	$r1, $r10
	lw	x5, 0(x5)
	mv	x11, x5
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LFAinfnan:
	la	x5, __r7                # 	or	$r7, $r7, $r6
	lw	x5, 0(x5)
	or	x5, x5, x9
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	bne	$r7, $fp, .LFnan
	lw	x5, 0(x5)
	bne	x5, x8, .LFnan
	bnez	x15, .LFAcont2          # 	bnez	$r5, .LFAcont2
	la	x5, __r9                # 	slli	$r2, $r9, #1
	lw	x5, 0(x5)
	slli	x12, x5, 1
	la	x6, __r8                # 	or	$r2, $r2, $r8
	lw	x6, 0(x6)
	or	x12, x12, x6
	beqz	x12, .LFnan             # 	beqz	$r2, .LFnan
.LFAcont2:
	bne	x13, x15, .LFinf        # 	bne	$r3, $r5, .LFinf
.LFBinfnan:
	la	x6, __r8                # 	or	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	bne	$r9, $fp, .LFnan
	lw	x5, 0(x5)
	bne	x5, x8, .LFnan
.LFinf:
	li	x10, 0                  # 	move	$r0, #0
	li	x11, 2146435072         # 	move	$r1, #2146435072
	tail	.LFret                  # 	b	.LFret
.LFnan:
	li	x10, 0                  # 	move	$r0, #0
	li	x11, 4294443008         # 	move	$r1, #4294443008
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LFBexpzero:
	la	x6, __r8                # 	or	$r0, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x10, x5, x6
	beqz	x10, .LFsetsign         # 	beqz	$r0, .LFsetsign
	la	x5, __r8                # 	srli	$ta, $r8, #31
	lw	x5, 0(x5)
	srli	tp, x5, 31
	la	x6, __r9                # 	add	$r9, $r9, $r9
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	add	$r9, $r9, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x6, __r8                # 	add	$r8, $r8, $r8
	lw	x6, 0(x6)
	la	x5, __r8
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r9                # 	bnez	$r9, .LFBcont
	lw	x5, 0(x5)
	bnez	x5, .LFBcont
	la	x5, __r8                # 	move	$r9, $r8
	lw	x5, 0(x5)
	mv	x5, x5
	la	x7, __r9
	sw	x5, 0(x7)
	li	x5, 0                   # 	move	$r8, #0
	la	x7, __r8
	sw	x5, 0(x7)
	addi	x15, x15, -32           # 	addi	$r5, $r5, #-32
.LFBcont:
	li	x10, 0                  # 	move	$r0, #0
	la	x5, __r9                # 	move	$r1, $r9
	lw	x5, 0(x5)
	mv	x11, x5
	tail	.LFBloop2               # 	b	.LFBloop2
.LFBloop:
	add	x11, x11, x11           # 	add	$r1, $r1, $r1
	addi	x10, x10, 1             # 	addi	$r0, $r0, #1
.LFBloop2:
	sltu	tp, x11, x8             # 	slt	$ta, $r1, $fp
	bnez	tp, .LFBloop            # 	bnez	$ta, .LFBloop
	beqz	x10, .LFmain2           # 	beqz	$r0, .LFmain2
	sub	x15, x15, x10           # 	sub	$r5, $r5, $r0
	li	x6, 32                  # 	subri	$r2, $r0, #32
	sub	x12, x6, x10
	la	x5, __r8                # 	srl	$r2, $r8, $r2
	lw	x5, 0(x5)
	srl	x12, x5, x12
	la	x5, __r8                # 	sll	$r8, $r8, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r9                # 	sll	$r9, $r9, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	or	$r9, $r9, $r2
	lw	x5, 0(x5)
	or	x5, x5, x12
	la	x7, __r9
	sw	x5, 0(x7)
	tail	.LFmain2                # 	b	.LFmain2
.LFunderflow:
	li	x9, 0                   # 	move	$r6, #0
	li	x6, 1                   # 	subri	$r3, $r4, #1
	sub	x13, x6, x14
	sltiu	tp, x13, 32             # 	slti	$ta, $r3, #32
	bnez	tp, .LFunderflow2       # 	bnez	$ta, .LFunderflow2
	mv	x9, x10                 # 	move	$r6, $r0
	mv	x10, x11                # 	move	$r0, $r1
	li	x11, 0                  # 	move	$r1, #0
	addi	x13, x13, -32           # 	addi	$r3, $r3, #-32
	beqz	x10, .LFunderflow2      # 	beqz	$r0, .LFunderflow2
	sltiu	tp, x13, 32             # 	slti	$ta, $r3, #32
	beqz	tp, .LFignore           # 	beqz	$ta, .LFignore
.LFunderflow2:
	beqz	x13, .LFunderflow3      # 	beqz	$r3, .LFunderflow3
	li	x6, 32                  # 	subri	$r2, $r3, #32
	sub	x12, x6, x13
	sll	x5, x11, x12            # 	sll	$r7, $r1, $r2
	la	x7, __r7
	sw	x5, 0(x7)
	sll	x15, x10, x12           # 	sll	$r5, $r0, $r2
	srl	x10, x10, x13           # 	srl	$r0, $r0, $r3
	srl	x11, x11, x13           # 	srl	$r1, $r1, $r3
	la	x6, __r7                # 	or	$r0, $r0, $r7
	lw	x6, 0(x6)
	or	x10, x10, x6
	or	x9, x9, x15             # 	or	$r6, $r6, $r5
	beqz	x9, .LFunderflow3       # 	beqz	$r6, .LFunderflow3
	ori	x10, x10, 1             # 	ori	$r0, $r0, #1
.LFunderflow3:
	addi	x10, x10, 1024          # 	addi	$r0, $r0, #1024
	sltiu	tp, x10, 1024           # 	slti	$ta, $r0, #1024
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	srli	x14, x11, 31            # 	srli	$r4, $r1, #31
	tail	.LFround                # 	b	.LFround
.LFignore:
	li	x10, 0                  # 	move	$r0, #0
	tail	.LFsetsign              # 	b	.LFsetsign
.Ltmp1:
	.size	__muldf3, .Ltmp1-__muldf3
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
	.comm	__r10,4,4
#endif

#ifdef L_div_sf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__divsf3
	.type	__divsf3,@function
__divsf3:
	addi	sp, sp, -16             # 	push.s	$r6, $r9, {}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	xor	x5, x11, x10            # 	xor	$r8, $r1, $r0
	la	x7, __r8
	sw	x5, 0(x7)
	li	x14, 2147483648         # 	move	$r4, #2147483648
	la	x5, __r8                # 	and	$r8, $r8, $r4
	lw	x5, 0(x5)
	and	x5, x5, x14
	la	x7, __r8
	sw	x5, 0(x7)
	slli	x12, x10, 1             # 	slli	$r2, $r0, #1
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	srli	x9, x12, 24             # 	srli	$r6, $r2, #24
	srli	x5, x11, 24             # 	srli	$r7, $r1, #24
	la	x7, __r7
	sw	x5, 0(x7)
	slli	x12, x12, 7             # 	slli	$r2, $r2, #7
	slli	x13, x11, 7             # 	slli	$r3, $r1, #7
	beqz	x9, .LGzeroAexp         # 	beqz	$r6, .LGzeroAexp
	li	x6, 255                 # 	beqc	$r6, #255, .LGinfnanA
	beq	x9, x6, .LGinfnanA
	or	x12, x12, x14           # 	or	$r2, $r2, $r4
.LGlab1:
	la	x5, __r7                # 	beqz	$r7, .LGzeroB
	lw	x5, 0(x5)
	beqz	x5, .LGzeroB
	la	x5, __r7                # 	beqc	$r7, #255, .LGinfnanB
	lw	x5, 0(x5)
	li	x6, 255
	beq	x5, x6, .LGinfnanB
	or	x13, x13, x14           # 	or	$r3, $r3, $r4
.LGlab2:
	sltu	tp, x12, x13            # 	slt	$ta, $r2, $r3
	bnez	tp, .LGlab3             # 	bnez	$ta, .LGlab3
	srli	x12, x12, 1             # 	srli	$r2, $r2, #1
	addi	x9, x9, 1               # 	addi	$r6, $r6, #1
.LGlab3:
	srli	x15, x13, 14            # 	srli	$r5, $r3, #14
	li	x6, 16383               # 	andi	$r9, $r3, #16383
	and	x5, x13, x6
	la	x7, __r9
	sw	x5, 0(x7)
	divu	x11, x12, x15           # 	divr	$r1, $r0, $r2, $r5
	remu	x10, x12, x15
	la	x5, __r9                # 	mul	$r2, $r9, $r1
	lw	x5, 0(x5)
	mul	x12, x5, x11
	slli	x10, x10, 14            # 	slli	$r0, $r0, #14
	sltu	tp, x10, x12            # 	slt	$ta, $r0, $r2
	beqz	tp, .LGlab4             # 	beqz	$ta, .LGlab4
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
	add	x10, x10, x13           # 	add	$r0, $r0, $r3
.LGlab4:
	sub	x10, x10, x12           # 	sub	$r0, $r0, $r2
	divu	x12, x10, x15           # 	divr	$r2, $r0, $r0, $r5
	remu	x10, x10, x15
	la	x5, __r9                # 	mul	$r9, $r9, $r2
	lw	x5, 0(x5)
	mul	x5, x5, x12
	la	x7, __r9
	sw	x5, 0(x7)
	slli	x10, x10, 14            # 	slli	$r0, $r0, #14
	la	x6, __r9                # 	slt	$ta, $r0, $r9
	lw	x6, 0(x6)
	sltu	tp, x10, x6
	beqz	tp, .LGlab5             # 	beqz	$ta, .LGlab5
	addi	x12, x12, -1            # 	addi	$r2, $r2, #-1
	add	x10, x10, x13           # 	add	$r0, $r0, $r3
.LGlab5:
	la	x6, __r9                # 	sub	$r0, $r0, $r9
	lw	x6, 0(x6)
	sub	x10, x10, x6
	slli	x5, x11, 14             # 	slli	$r9, $r1, #14
	la	x7, __r9
	sw	x5, 0(x7)
	la	x6, __r9                # 	add	$r2, $r2, $r9
	lw	x6, 0(x6)
	add	x12, x12, x6
	slli	x12, x12, 4             # 	slli	$r2, $r2, #4
	beqz	x10, .LGlab6            # 	beqz	$r0, .LGlab6
	ori	x12, x12, 1             # 	ori	$r2, $r2, #1
.LGlab6:
	la	x5, __r7                # 	subri	$r5, $r7, #126
	lw	x5, 0(x5)
	li	x6, 126
	sub	x15, x6, x5
	add	x9, x9, x15             # 	add	$r6, $r6, $r5
	blez	x9, .LGunder            # 	blez	$r6, .LGunder
	sltiu	tp, x9, 255             # 	slti	$ta, $r6, #255
	beqz	tp, .LGinf              # 	beqz	$ta, .LGinf
.LGround:
	addi	x12, x12, 128           # 	addi	$r2, $r2, #128
	sltiu	tp, x12, 128            # 	slti	$ta, $r2, #128
	add	x9, x9, tp              # 	add	$r6, $r6, $ta
	srli	x15, x12, 8             # 	srli	$r5, $r2, #8
	andi	x15, x15, 1             # 	andi	$r5, $r5, #1
	sub	x12, x12, x15           # 	sub	$r2, $r2, $r5
	slli	x12, x12, 1             # 	slli	$r2, $r2, #1
	srli	x12, x12, 9             # 	srli	$r2, $r2, #9
	slli	x10, x9, 23             # 	slli	$r0, $r6, #23
	or	x10, x10, x12           # 	or	$r0, $r0, $r2
.LGpack:
	la	x6, __r8                # 	or	$r0, $r0, $r8
	lw	x6, 0(x6)
	or	x10, x10, x6
.LGret:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r9, {}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	addi	sp, sp, 16
	ret                             # 	ret5	$lp
.LGzeroAexp:
	bnez	x12, .LGloopA2          # 	bnez	$r2, .LGloopA2
	tail	.LGzeroA                # 	b	.LGzeroA
.LGloopA:
	addi	x9, x9, -1              # 	addi	$r6, $r6, #-1
.LGloopA2:
	add	x12, x12, x12           # 	add	$r2, $r2, $r2
	sltu	tp, x12, x14            # 	slt	$ta, $r2, $r4
	bnez	tp, .LGloopA            # 	bnez	$ta, .LGloopA
	tail	.LGlab1                 # 	b	.LGlab1
.LGzeroA:
	beqz	x11, .LGnan             # 	beqz	$r1, .LGnan
	li	x15, 4278190080         # 	move	$r5, #4278190080
	sltu	tp, x15, x11            # 	slt	$ta, $r5, $r1
	bnez	tp, .LGnan              # 	bnez	$ta, .LGnan
.LGzero:
	la	x5, __r8                # 	move	$r0, $r8
	lw	x5, 0(x5)
	mv	x10, x5
	tail	.LGret                  # 	b	.LGret
.LGinfnanA:
	bne	x12, x14, .LGnan        # 	bne	$r2, $r4, .LGnan
	la	x5, __r7                # 	beq	$r7, $r6, .LGnan
	lw	x5, 0(x5)
	beq	x5, x9, .LGnan
.LGinf:
	li	x10, 2139095040         # 	move	$r0, #2139095040
	la	x6, __r8                # 	or	$r0, $r0, $r8
	lw	x6, 0(x6)
	or	x10, x10, x6
	tail	.LGret                  # 	b	.LGret
.LGinfnanB:
	beq	x13, x14, .LGzero       # 	beq	$r3, $r4, .LGzero
.LGnan:
	li	x10, 4290772992         # 	move	$r0, #4290772992
	tail	.LGret                  # 	b	.LGret
.LGzeroB:
	bnez	x13, .LGloopB2          # 	bnez	$r3, .LGloopB2
	tail	.LGinf                  # 	b	.LGinf
.LGloopB:
	la	x5, __r7                # 	addi	$r7, $r7, #-1
	lw	x5, 0(x5)
	addi	x5, x5, -1
	la	x7, __r7
	sw	x5, 0(x7)
.LGloopB2:
	add	x13, x13, x13           # 	add	$r3, $r3, $r3
	sltu	tp, x13, x14            # 	slt	$ta, $r3, $r4
	bnez	tp, .LGloopB            # 	bnez	$ta, .LGloopB
	tail	.LGlab2                 # 	b	.LGlab2
.LGunder:
	li	x6, 1                   # 	subri	$r1, $r6, #1
	sub	x11, x6, x9
	sltiu	tp, x11, 32             # 	slti	$ta, $r1, #32
	beqz	tp, .LGzero             # 	beqzs8	.LGzero
	li	x6, 32                  # 	subri	$r5, $r1, #32
	sub	x15, x6, x11
	sll	x9, x12, x15            # 	sll	$r6, $r2, $r5
	srl	x12, x12, x11           # 	srl	$r2, $r2, $r1
	beqz	x9, .LGunder2           # 	beqz	$r6, .LGunder2
	ori	x12, x12, 2             # 	ori	$r2, $r2, #2
.LGunder2:
	addi	x15, x12, 128           # 	addi	$r5, $r2, #128
	slti	x9, x15, 0              # 	sltsi	$r6, $r5, #0
	tail	.LGround                # 	b	.LGround
.Ltmp0:
	.size	__divsf3, .Ltmp0-__divsf3
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
#endif

#ifdef L_div_df
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__divdf3
	.type	__divdf3,@function
__divdf3:
	addi	sp, sp, -32             # 	push25	$r10, #16
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	la	x7, __r10
	lw	x7, 0(x7)
	sw	x7, 16(sp)
	sw	x8, 20(sp)
	sw	gp, 24(sp)
	sw	ra, 28(sp)
	addi	sp, sp, -16
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	srli	x14, x14, 21            # 	srli	$r4, $r4, #21
	slli	x5, x11, 11             # 	slli	$r7, $r1, #11
	la	x7, __r7
	sw	x5, 0(x7)
	srli	x9, x10, 21             # 	srli	$r6, $r0, #21
	la	x5, __r7                # 	or	$r7, $r7, $r6
	lw	x5, 0(x5)
	or	x5, x5, x9
	la	x7, __r7
	sw	x5, 0(x7)
	slli	x9, x10, 11             # 	slli	$r6, $r0, #11
	li	x8, 2147483648          # 	move	$fp, #2147483648
	slli	x15, x13, 1             # 	slli	$r5, $r3, #1
	srli	x15, x15, 21            # 	srli	$r5, $r5, #21
	slli	x5, x13, 11             # 	slli	$r9, $r3, #11
	la	x7, __r9
	sw	x5, 0(x7)
	srli	x5, x12, 21             # 	srli	$r8, $r2, #21
	la	x7, __r8
	sw	x5, 0(x7)
	la	x6, __r8                # 	or	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	slli	x5, x12, 11             # 	slli	$r8, $r2, #11
	la	x7, __r8
	sw	x5, 0(x7)
	xor	x13, x13, x11           # 	xor	$r3, $r3, $r1
	and	x5, x13, x8             # 	and	$r10, $r3, $fp
	la	x7, __r10
	sw	x5, 0(x7)
	li	x13, 2047               # 	move	$r3, #2047
	beqz	x14, .LGAexpzero        # 	beqz	$r4, .LGAexpzero
	beq	x13, x14, .LGAinfnan    # 	beq	$r3, $r4, .LGAinfnan
	la	x5, __r7                # 	or	$r7, $r7, $fp
	lw	x5, 0(x5)
	or	x5, x5, x8
	la	x7, __r7
	sw	x5, 0(x7)
.LGmain1:
	beqz	x15, .LGBexpzero        # 	beqz	$r5, .LGBexpzero
	beq	x13, x15, .LGBinfnan    # 	beq	$r3, $r5, .LGBinfnan
	la	x5, __r9                # 	or	$r9, $r9, $fp
	lw	x5, 0(x5)
	or	x5, x5, x8
	la	x7, __r9
	sw	x5, 0(x7)
.LGmain2:
	sub	x14, x14, x15           # 	sub	$r4, $r4, $r5
	addi	x14, x14, 1023          # 	addi	$r4, $r4, #1023
	srli	x9, x9, 1               # 	srli	$r6, $r6, #1
	la	x5, __r7                # 	slli	$r1, $r7, #31
	lw	x5, 0(x5)
	slli	x11, x5, 31
	or	x9, x9, x11             # 	or	$r6, $r6, $r1
	la	x5, __r7                # 	srli	$r7, $r7, #1
	lw	x5, 0(x5)
	srli	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r9                # 	srli	$r2, $r9, #16
	lw	x5, 0(x5)
	srli	x12, x5, 16
	la	x6, __r7                # 	divr	$r3, $r7, $r7, $r2
	lw	x6, 0(x6)
	divu	x13, x6, x12
	remu	x6, x6, x12
	la	x7, __r7
	sw	x6, 0(x7)
	la	x5, __r9                # 	zeh	$r1, $r9
	lw	x5, 0(x5)
	slli	x6, x5, 16
	srli	x11, x6, 16
	mul	x15, x11, x13           # 	mul	$r5, $r1, $r3
	la	x5, __r7                # 	slli	$r7, $r7, #16
	lw	x5, 0(x5)
	slli	x5, x5, 16
	la	x7, __r7
	sw	x5, 0(x7)
	srli	x10, x9, 16             # 	srli	$r0, $r6, #16
	la	x5, __r7                # 	or	$r7, $r7, $r0
	lw	x5, 0(x5)
	or	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	sub	$r7, $r7, $r5
	lw	x5, 0(x5)
	sub	x5, x5, x15
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
	beqz	tp, .LGmain3            # 	beqz	$ta, .LGmain3
.LGloop1:
	addi	x13, x13, -1            # 	addi	$r3, $r3, #-1
	la	x6, __r9                # 	add	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	tp, .LGloop1            # 	beqz	$ta, .LGloop1
.LGmain3:
	la	x6, __r7                # 	divr	$r2, $r7, $r7, $r2
	lw	x6, 0(x6)
	divu	x5, x6, x12
	remu	x6, x6, x12
	add	x12, x5, 0
	la	x7, __r7
	sw	x6, 0(x7)
	mul	x15, x11, x12           # 	mul	$r5, $r1, $r2
	la	x5, __r7                # 	slli	$r7, $r7, #16
	lw	x5, 0(x5)
	slli	x5, x5, 16
	la	x7, __r7
	sw	x5, 0(x7)
	slli	x6, x9, 16              # 	zeh	$r0, $r6
	srli	x10, x6, 16
	la	x5, __r7                # 	or	$r7, $r7, $r0
	lw	x5, 0(x5)
	or	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	sub	$r7, $r7, $r5
	lw	x5, 0(x5)
	sub	x5, x5, x15
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
	beqz	tp, .LGmain4            # 	beqz	$ta, .LGmain4
.LGloop2:
	addi	x12, x12, -1            # 	addi	$r2, $r2, #-1
	la	x6, __r9                # 	add	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	tp, .LGloop2            # 	beqz	$ta, .LGloop2
.LGmain4:
	slli	x13, x13, 16            # 	slli	$r3, $r3, #16
	add	x13, x13, x12           # 	add	$r3, $r3, $r2
	la	x7, __r8                # 	mulr64	$r0, $r3, $r8
	lw	x7, 0(x7)
	mulhu	x11, x13, x7
	mul	x10, x13, x7
	sub	x9, zero, x10           # 	subri	$r6, $r0, #0
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	sub	$r7, $r7, $r1
	lw	x5, 0(x5)
	sub	x5, x5, x11
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
	beqz	x9, .LGmain5            # 	beqz	$r6, .LGmain5
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	addi	$r7, $r7, #-1
	lw	x5, 0(x5)
	addi	x5, x5, -1
	la	x7, __r7
	sw	x5, 0(x7)
	bnez	tp, .LGloopA            # 	bnez	$ta, .LGloopA
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
.LGmain5:
	beqz	tp, .LGmain6            # 	beqz	$ta, .LGmain6
.LGloopA:
	addi	x13, x13, -1            # 	addi	$r3, $r3, #-1
	la	x6, __r8                # 	add	$r6, $r6, $r8
	lw	x6, 0(x6)
	add	x9, x9, x6
	la	x6, __r8                # 	slt	$r0, $r6, $r8
	lw	x6, 0(x6)
	sltu	x10, x9, x6
	la	x6, __r9                # 	add	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	x10, .LGloopA2          # 	beqz	$r0, .LGloopA2
	la	x5, __r7                # 	addi	$r7, $r7, #1
	lw	x5, 0(x5)
	addi	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	bnez	tp, .LGmain6            # 	bnez	$ta, .LGmain6
	la	x5, __r7                # 	slti	$ta, $r7, #1
	lw	x5, 0(x5)
	sltiu	tp, x5, 1
.LGloopA2:
	beqz	tp, .LGloopA            # 	beqz	$ta, .LGloopA
.LGmain6:
	la	x6, __r9                # 	bne	$r7, $r9, .Li25
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	bne	x5, x6, .Li25
	la	x5, __r8                # 	move	$r1, $r8
	lw	x5, 0(x5)
	mv	x11, x5
	mv	x5, x9                  # 	move	$r7, $r6
	la	x7, __r7
	sw	x5, 0(x7)
	li	x12, 0                  # 	move	$r2, #0
	li	x10, 0                  # 	move	$r0, #0
	tail	.LGmain7                # 	b	.LGmain7
.Li25:
	la	x5, __r9                # 	srli	$r1, $r9, #16
	lw	x5, 0(x5)
	srli	x11, x5, 16
	la	x6, __r7                # 	divr	$r2, $r7, $r7, $r1
	lw	x6, 0(x6)
	divu	x12, x6, x11
	remu	x6, x6, x11
	la	x7, __r7
	sw	x6, 0(x7)
	la	x5, __r9                # 	zeh	$r0, $r9
	lw	x5, 0(x5)
	slli	x6, x5, 16
	srli	x10, x6, 16
	mul	tp, x10, x12            # 	mul	$ta, $r0, $r2
	la	x5, __r7                # 	slli	$r7, $r7, #16
	lw	x5, 0(x5)
	slli	x5, x5, 16
	la	x7, __r7
	sw	x5, 0(x7)
	srli	x15, x9, 16             # 	srli	$r5, $r6, #16
	la	x5, __r7                # 	or	$r7, $r7, $r5
	lw	x5, 0(x5)
	or	x5, x5, x15
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	move	$r5, $r7
	lw	x5, 0(x5)
	mv	x15, x5
	la	x5, __r7                # 	sub	$r7, $r7, $ta
	lw	x5, 0(x5)
	sub	x5, x5, tp
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r5, $r7
	lw	x6, 0(x6)
	sltu	tp, x15, x6
	beqz	tp, .Li26               # 	beqz	$ta, .Li26
.LGloop3:
	addi	x12, x12, -1            # 	addi	$r2, $r2, #-1
	la	x6, __r9                # 	add	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	tp, .LGloop3            # 	beqz	$ta, .LGloop3
.Li26:
	la	x6, __r7                # 	divr	$r1, $r7, $r7, $r1
	lw	x6, 0(x6)
	divu	x5, x6, x11
	remu	x6, x6, x11
	add	x11, x5, 0
	la	x7, __r7
	sw	x6, 0(x7)
	mul	x15, x10, x11           # 	mul	$r5, $r0, $r1
	la	x5, __r7                # 	slli	$r7, $r7, #16
	lw	x5, 0(x5)
	slli	x5, x5, 16
	la	x7, __r7
	sw	x5, 0(x7)
	slli	x6, x9, 16              # 	zeh	$r0, $r6
	srli	x10, x6, 16
	la	x5, __r7                # 	or	$r7, $r7, $r0
	lw	x5, 0(x5)
	or	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	sub	$r7, $r7, $r5
	lw	x5, 0(x5)
	sub	x5, x5, x15
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
	beqz	tp, .Li28               # 	beqz	$ta, .Li28
.LGloop4:
	addi	x11, x11, -1            # 	addi	$r1, $r1, #-1
	la	x6, __r9                # 	add	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	tp, .LGloop4            # 	beqz	$ta, .LGloop4
.Li28:
	slli	x12, x12, 16            # 	slli	$r2, $r2, #16
	add	x12, x12, x11           # 	add	$r2, $r2, $r1
	la	x7, __r8                # 	mulr64	$r0, $r2, $r8
	lw	x7, 0(x7)
	mulhu	x11, x12, x7
	mul	x10, x12, x7
.LGmain7:
	sub	x9, zero, x10           # 	subri	$r6, $r0, #0
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	sub	$r7, $r7, $r1
	lw	x5, 0(x5)
	sub	x5, x5, x11
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
	beqz	x9, .LGmain8            # 	beqz	$r6, .LGmain8
	la	x5, __r7                # 	move	$r0, $r7
	lw	x5, 0(x5)
	mv	x10, x5
	la	x5, __r7                # 	addi	$r7, $r7, #-1
	lw	x5, 0(x5)
	addi	x5, x5, -1
	la	x7, __r7
	sw	x5, 0(x7)
	bnez	tp, .LGloopB            # 	bnez	$ta, .LGloopB
	la	x6, __r7                # 	slt	$ta, $r0, $r7
	lw	x6, 0(x6)
	sltu	tp, x10, x6
.LGmain8:
	beqz	tp, .LGmain9            # 	beqz	$ta, .LGmain9
.LGloopB:
	addi	x12, x12, -1            # 	addi	$r2, $r2, #-1
	la	x6, __r8                # 	add	$r6, $r6, $r8
	lw	x6, 0(x6)
	add	x9, x9, x6
	la	x6, __r8                # 	slt	$r0, $r6, $r8
	lw	x6, 0(x6)
	sltu	x10, x9, x6
	la	x6, __r9                # 	add	$r7, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r7, $r9
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	beqz	x10, .LGloopB2          # 	beqz	$r0, .LGloopB2
	la	x5, __r7                # 	addi	$r7, $r7, #1
	lw	x5, 0(x5)
	addi	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	bnez	tp, .LGmain9            # 	bnez	$ta, .LGmain9
	la	x5, __r7                # 	slti	$ta, $r7, #1
	lw	x5, 0(x5)
	sltiu	tp, x5, 1
.LGloopB2:
	beqz	tp, .LGloopB            # 	beqz	$ta, .LGloopB
.LGmain9:
	slti	tp, x13, 0              # 	sltsi	$ta, $r3, #0
	bnez	tp, .LGmain10           # 	bnez	$ta, .LGmain10
	mv	tp, x12                 # 	move	$ta, $r2
	add	x12, x12, x12           # 	add	$r2, $r2, $r2
	sltu	tp, x12, tp             # 	slt	$ta, $r2, $ta
	add	x13, x13, x13           # 	add	$r3, $r3, $r3
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	addi	x14, x14, -1            # 	addi	$r4, $r4, #-1
.LGmain10:
	la	x5, __r7                # 	or	$r7, $r7, $r6
	lw	x5, 0(x5)
	or	x5, x5, x9
	la	x7, __r7
	sw	x5, 0(x7)
	ori	x10, x12, 1             # 	ori	$r0, $r2, #1
	la	x6, __r7                # 	cmovz	$r0, $r2, $r7
	lw	x6, 0(x6)
	bne	x6, zero, .Ltmp0
	addi	x10, x12, 0
.Ltmp0:
	mv	x11, x13                # 	move	$r1, $r3
	blez	x14, .LGunderflow       # 	blez	$r4, .LGunderflow
	li	x6, 2047                # 	subri	$r5, $r4, #2047
	sub	x15, x6, x14
	blez	x15, .LGinf             # 	blez	$r5, .LGinf
	addi	x10, x10, 1024          # 	addi	$r0, $r0, #1024
	sltiu	tp, x10, 1024           # 	slti	$ta, $r0, #1024
	beqz	tp, .LGround            # 	beqz	$ta, .LGround
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	sltu	tp, x11, tp             # 	slt	$ta, $r1, $ta
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
.LGround:
	srli	x12, x10, 11            # 	srli	$r2, $r0, #11
	andi	x12, x12, 1             # 	andi	$r2, $r2, #1
	sub	x10, x10, x12           # 	sub	$r0, $r0, $r2
	srli	x10, x10, 11            # 	srli	$r0, $r0, #11
	slli	x12, x11, 21            # 	slli	$r2, $r1, #21
	or	x10, x10, x12           # 	or	$r0, $r0, $r2
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	srli	x11, x11, 12            # 	srli	$r1, $r1, #12
	slli	x15, x14, 20            # 	slli	$r5, $r4, #20
	or	x11, x11, x15           # 	or	$r1, $r1, $r5
.LGret:
	la	x6, __r10               # 	or	$r1, $r1, $r10
	lw	x6, 0(x6)
	or	x11, x11, x6
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LGAexpzero:
	la	x5, __r7                # 	or	$ta, $r7, $r6
	lw	x5, 0(x5)
	or	tp, x5, x9
	beqz	tp, .LGAzero            # 	beqz	$ta, .LGAzero
	srli	tp, x9, 31              # 	srli	$ta, $r6, #31
	la	x6, __r7                # 	add	$r7, $r7, $r7
	lw	x6, 0(x6)
	la	x5, __r7
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	add	$r7, $r7, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r7
	sw	x5, 0(x7)
	add	x9, x9, x9              # 	add	$r6, $r6, $r6
	la	x5, __r7                # 	bnez	$r7, .LGAcont
	lw	x5, 0(x5)
	bnez	x5, .LGAcont
	mv	x5, x9                  # 	move	$r7, $r6
	la	x7, __r7
	sw	x5, 0(x7)
	li	x9, 0                   # 	move	$r6, #0
	addi	x14, x14, -32           # 	addi	$r4, $r4, #-32
.LGAcont:
	li	x10, 0                  # 	move	$r0, #0
	la	x5, __r7                # 	move	$r1, $r7
	lw	x5, 0(x5)
	mv	x11, x5
	tail	.LGAloop2               # 	b	.LGAloop2
.LGAloop:
	add	x11, x11, x11           # 	add	$r1, $r1, $r1
	addi	x10, x10, 1             # 	addi	$r0, $r0, #1
.LGAloop2:
	sltu	tp, x11, x8             # 	slt	$ta, $r1, $fp
	bnez	tp, .LGAloop            # 	bnez	$ta, .LGAloop
	beqz	x10, .LGmain1           # 	beqz	$r0, .LGmain1
	sub	x14, x14, x10           # 	sub	$r4, $r4, $r0
	li	x6, 32                  # 	subri	$r2, $r0, #32
	sub	x12, x6, x10
	srl	x12, x9, x12            # 	srl	$r2, $r6, $r2
	sll	x9, x9, x10             # 	sll	$r6, $r6, $r0
	la	x5, __r7                # 	sll	$r7, $r7, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	or	$r7, $r7, $r2
	lw	x5, 0(x5)
	or	x5, x5, x12
	la	x7, __r7
	sw	x5, 0(x7)
	tail	.LGmain1                # 	b	.LGmain1
.LGAzero:
	beq	x13, x15, .LGAzero2     # 	beq	$r3, $r5, .LGAzero2
	bnez	x15, .LGsetsign         # 	bnez	$r5, .LGsetsign
	la	x6, __r8                # 	or	$ta, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	tp, x5, x6
	beqz	tp, .LGnan              # 	beqz	$ta, .LGnan
.LGsetsign:
	la	x5, __r10               # 	move	$r1, $r10
	lw	x5, 0(x5)
	mv	x11, x5
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LGAzero2:
	la	x6, __r8                # 	or	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	beq	$r9, $fp, .LGsetsign
	lw	x5, 0(x5)
	beq	x5, x8, .LGsetsign
.LGnan:
	li	x10, 0                  # 	move	$r0, #0
	li	x11, 4294443008         # 	move	$r1, #4294443008
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LGAinfnan:
	la	x5, __r7                # 	or	$r7, $r7, $r6
	lw	x5, 0(x5)
	or	x5, x5, x9
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	bne	$r7, $fp, .LGnan
	lw	x5, 0(x5)
	bne	x5, x8, .LGnan
	beq	x13, x15, .LGnan        # 	beq	$r3, $r5, .LGnan
.LGinf:
	li	x10, 0                  # 	move	$r0, #0
	li	x11, 2146435072         # 	move	$r1, #2146435072
	la	x6, __r10               # 	or	$r1, $r1, $r10
	lw	x6, 0(x6)
	or	x11, x11, x6
	addi	sp, sp, 16              # 	pop25	$r10, #16
	lw	x9, 0(sp)
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	lw	x5, 16(sp)
	la	x7, __r10
	sw	x5, 0(x7)
	lw	x8, 20(sp)
	lw	gp, 24(sp)
	lw	ra, 28(sp)
	addi	sp, sp, 32
	jalr	zero, ra, 0
.LGBinfnan:
	la	x6, __r8                # 	or	$r9, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	bne	$r9, $fp, .LGnan
	lw	x5, 0(x5)
	bne	x5, x8, .LGnan
	li	x10, 0                  # 	move	$r0, #0
	tail	.LGsetsign              # 	b	.LGsetsign
.LGBexpzero:
	la	x6, __r8                # 	or	$ta, $r9, $r8
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	or	tp, x5, x6
	beqz	tp, .LGinf              # 	beqz	$ta, .LGinf
	la	x5, __r8                # 	srli	$ta, $r8, #31
	lw	x5, 0(x5)
	srli	tp, x5, 31
	la	x6, __r9                # 	add	$r9, $r9, $r9
	lw	x6, 0(x6)
	la	x5, __r9
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	add	$r9, $r9, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x6, __r8                # 	add	$r8, $r8, $r8
	lw	x6, 0(x6)
	la	x5, __r8
	lw	x5, 0(x5)
	add	x5, x5, x6
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r9                # 	bnez	$r9, .LGBcont
	lw	x5, 0(x5)
	bnez	x5, .LGBcont
	la	x5, __r8                # 	move	$r9, $r8
	lw	x5, 0(x5)
	mv	x5, x5
	la	x7, __r9
	sw	x5, 0(x7)
	li	x5, 0                   # 	move	$r8, #0
	la	x7, __r8
	sw	x5, 0(x7)
	addi	x15, x15, -32           # 	addi	$r5, $r5, #-32
.LGBcont:
	li	x10, 0                  # 	move	$r0, #0
	la	x5, __r9                # 	move	$r1, $r9
	lw	x5, 0(x5)
	mv	x11, x5
	tail	.LGBloop2               # 	b	.LGBloop2
.LGBloop:
	add	x11, x11, x11           # 	add	$r1, $r1, $r1
	addi	x10, x10, 1             # 	addi	$r0, $r0, #1
.LGBloop2:
	sltu	tp, x11, x8             # 	slt	$ta, $r1, $fp
	bnez	tp, .LGBloop            # 	bnez	$ta, .LGBloop
	beqz	x10, .LGmain2           # 	beqz	$r0, .LGmain2
	sub	x15, x15, x10           # 	sub	$r5, $r5, $r0
	li	x6, 32                  # 	subri	$r2, $r0, #32
	sub	x12, x6, x10
	la	x5, __r8                # 	srl	$r2, $r8, $r2
	lw	x5, 0(x5)
	srl	x12, x5, x12
	la	x5, __r8                # 	sll	$r8, $r8, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r9                # 	sll	$r9, $r9, $r0
	lw	x5, 0(x5)
	sll	x5, x5, x10
	la	x7, __r9
	sw	x5, 0(x7)
	la	x5, __r9                # 	or	$r9, $r9, $r2
	lw	x5, 0(x5)
	or	x5, x5, x12
	la	x7, __r9
	sw	x5, 0(x7)
	tail	.LGmain2                # 	b	.LGmain2
.LGunderflow:
	li	x9, 0                   # 	move	$r6, #0
	li	x6, 1                   # 	subri	$r3, $r4, #1
	sub	x13, x6, x14
	sltiu	tp, x13, 32             # 	slti	$ta, $r3, #32
	bnez	tp, .LGunderflow2       # 	bnez	$ta, .LGunderflow2
	mv	x9, x10                 # 	move	$r6, $r0
	mv	x10, x11                # 	move	$r0, $r1
	li	x11, 0                  # 	move	$r1, #0
	addi	x13, x13, -32           # 	addi	$r3, $r3, #-32
	beqz	x10, .LGunderflow2      # 	beqz	$r0, .LGunderflow2
	sltiu	tp, x13, 32             # 	slti	$ta, $r3, #32
	beqz	tp, .LGignore           # 	beqz	$ta, .LGignore
.LGunderflow2:
	beqz	x13, .LGunderflow3      # 	beqz	$r3, .LGunderflow3
	li	x6, 32                  # 	subri	$r2, $r3, #32
	sub	x12, x6, x13
	sll	x5, x11, x12            # 	sll	$r7, $r1, $r2
	la	x7, __r7
	sw	x5, 0(x7)
	sll	x15, x10, x12           # 	sll	$r5, $r0, $r2
	srl	x10, x10, x13           # 	srl	$r0, $r0, $r3
	srl	x11, x11, x13           # 	srl	$r1, $r1, $r3
	la	x6, __r7                # 	or	$r0, $r0, $r7
	lw	x6, 0(x6)
	or	x10, x10, x6
	or	x9, x9, x15             # 	or	$r6, $r6, $r5
	beqz	x9, .LGunderflow3       # 	beqz	$r6, .LGunderflow3
	ori	x10, x10, 1             # 	ori	$r0, $r0, #1
.LGunderflow3:
	addi	x10, x10, 1024          # 	addi	$r0, $r0, #1024
	sltiu	tp, x10, 1024           # 	slti	$ta, $r0, #1024
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	srli	x14, x11, 31            # 	srli	$r4, $r1, #31
	tail	.LGround                # 	b	.LGround
.LGignore:
	li	x10, 0                  # 	move	$r0, #0
	tail	.LGsetsign              # 	b	.LGsetsign
.Ltmp1:
	.size	__divdf3, .Ltmp1-__divdf3
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
	.comm	__r10,4,4
#endif

#ifdef L_negate_sf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__negsf2
	.type	__negsf2,@function
__negsf2:
	li	x11, 2147483648         # 	move	$r1, #2147483648
	xor	x10, x10, x11           # 	xor	$r0, $r0, $r1
	ret                             # 	ret5	$lp
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
	li	x12, 2147483648         # 	move	$r2, #2147483648
	xor	x11, x11, x12           # 	xor	$r1, $r1, $r2
	ret                             # 	ret5	$lp
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
	slli	x15, x10, 1             # 	slli	$r5, $r0, #1
	beqz	x15, .LJzero            # 	beqz	$r5, .LJzero
	srli	x13, x15, 24            # 	srli	$r3, $r5, #24
	li	x11, 2147483648         # 	move	$r1, #2147483648
	and	x12, x11, x10           # 	and	$r2, $r1, $r0
	slli	x14, x15, 8             # 	slli	$r4, $r5, #8
	beqz	x13, .LJdenorm          # 	beqz	$r3, .LJdenorm
	li	x6, 255                 # 	beqc	$r3, #255, .LJinfnan
	beq	x13, x6, .LJinfnan
.LJlab1:
	addi	x13, x13, 896           # 	addi	$r3, $r3, #896
	slli	x10, x14, 20            # 	slli	$r0, $r4, #20
	srli	x11, x14, 12            # 	srli	$r1, $r4, #12
	or	x11, x11, x12           # 	or	$r1, $r1, $r2
	slli	x13, x13, 20            # 	slli	$r3, $r3, #20
	or	x11, x11, x13           # 	or	$r1, $r1, $r3
	ret                             # 	ret5	$lp
.LJdenorm2:
	addi	x13, x13, -1            # 	addi	$r3, $r3, #-1
	add	x14, x14, x14           # 	add	$r4, $r4, $r4
.LJdenorm:
	sltu	tp, x14, x11            # 	slt	$ta, $r4, $r1
	bnez	tp, .LJdenorm2          # 	bnezs8	.LJdenorm2
	slli	x14, x14, 1             # 	slli	$r4, $r4, #1
	tail	.LJlab1                 # 	b	.LJlab1
.LJinfnan:
	beqz	x14, .LJinf             # 	beqz	$r4, .LJinf
	li	x11, 4294443008         # 	move	$r1, #4294443008
	tail	.LJcont                 # 	b	.LJcont
.LJinf:
	li	x15, 7340032            # 	move	$r5, #7340032
	or	x10, x10, x15           # 	or	$r0, $r0, $r5
.LJzero:
	mv	x11, x10                # 	move	$r1, $r0
.LJcont:
	li	x10, 0                  # 	move	$r0, #0
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -12             # 	pushm	$r6, $r8
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	slli	x13, x11, 11            # 	slli	$r3, $r1, #11
	srli	x5, x10, 21             # 	srli	$r7, $r0, #21
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r3, $r3, $r7
	lw	x6, 0(x6)
	or	x13, x13, x6
	slli	x12, x10, 11            # 	slli	$r2, $r0, #11
	li	x5, 2147483648          # 	move	$r7, #2147483648
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r3, $r3, $r7
	lw	x6, 0(x6)
	or	x13, x13, x6
	la	x6, __r7                # 	and	$r5, $r1, $r7
	lw	x6, 0(x6)
	and	x15, x11, x6
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	srli	x14, x14, 21            # 	srli	$r4, $r4, #21
	addi	x14, x14, -896          # 	addi	$r4, $r4, #-896
	addi	x5, x14, -1             # 	addi	$r7, $r4, #-1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	slti	$ta, $r7, #254
	lw	x5, 0(x5)
	sltiu	tp, x5, 254
	beqz	tp, .LKspec             # 	beqzs8	.LKspec
.LKlab1:
	beqz	x12, .Li45              # 	beqz	$r2, .Li45
	ori	x13, x13, 1             # 	ori	$r3, $r3, #1
.Li45:
	li	tp, 128                 # 	move	$ta, #128
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	sltu	tp, x13, tp             # 	slt	$ta, $r3, $ta
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
	srli	x5, x13, 8              # 	srli	$r7, $r3, #8
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	andi	$r7, $r7, #1
	lw	x5, 0(x5)
	andi	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	sub	$r3, $r3, $r7
	lw	x6, 0(x6)
	sub	x13, x13, x6
	slli	x13, x13, 1             # 	slli	$r3, $r3, #1
	srli	x13, x13, 9             # 	srli	$r3, $r3, #9
	slli	x5, x14, 23             # 	slli	$r7, $r4, #23
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r3, $r3, $r7
	lw	x6, 0(x6)
	or	x13, x13, x6
	or	x10, x13, x15           # 	or	$r0, $r3, $r5
.LK999:
	lw	x9, 0(sp)               # 	popm	$r6, $r8
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	addi	sp, sp, 12
	ret                             # 	ret5	$lp
.LKspec:
	li	x6, 1151                # 	subri	$ta, $r4, #1151
	sub	tp, x6, x14
	bnez	tp, .Li46               # 	bnezs8	.Li46
	slli	x5, x13, 1              # 	slli	$r7, $r3, #1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	or	$r7, $r7, $r2
	lw	x5, 0(x5)
	or	x5, x5, x12
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	beqz	$r7, .Li46
	lw	x5, 0(x5)
	beqz	x5, .Li46
	li	x10, 4290772992         # 	move	$r0, #4290772992
	tail	.LK999                  # 	b	.LK999
.Li46:
	slti	tp, x14, 255            # 	sltsi	$ta, $r4, #255
	bnez	tp, .Li48               # 	bnezs8	.Li48
	li	x5, 2139095040          # 	move	$r7, #2139095040
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r0, $r5, $r7
	lw	x6, 0(x6)
	or	x10, x15, x6
	tail	.LK999                  # 	b	.LK999
.Li48:
	li	x6, 1                   # 	subri	$r6, $r4, #1
	sub	x9, x6, x14
	li	x5, 32                  # 	move	$r7, #32
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	slt	$ta, $r6, $r7
	lw	x6, 0(x6)
	sltu	tp, x9, x6
	bnez	tp, .Li49               # 	bnezs8	.Li49
	mv	x10, x15                # 	move	$r0, $r5
	tail	.LK999                  # 	b	.LK999
.Li49:
	li	x6, 32                  # 	subri	$r8, $r6, #32
	sub	x5, x6, x9
	la	x7, __r8
	sw	x5, 0(x7)
	la	x6, __r8                # 	sll	$r7, $r3, $r8
	lw	x6, 0(x6)
	sll	x5, x13, x6
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r2, $r2, $r7
	lw	x6, 0(x6)
	or	x12, x12, x6
	srl	x13, x13, x9            # 	srl	$r3, $r3, $r6
	li	x14, 0                  # 	move	$r4, #0
	li	x5, 2147483648          # 	move	$r7, #2147483648
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r3, $r3, $r7
	lw	x6, 0(x6)
	or	x13, x13, x6
	tail	.LKlab1                 # 	b	.LKlab1
.Ltmp0:
	.size	__truncdfsf2, .Ltmp0-__truncdfsf2
	.comm	__r7,4,4
	.comm	__r8,4,4
#endif

#ifdef L_fixsfdi
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__fixsfdi
	.type	__fixsfdi,@function
__fixsfdi:
	srli	x13, x10, 23            # 	srli	$r3, $r0, #23
	andi	x13, x13, 255           # 	andi	$r3, $r3, #255
	slli	x12, x10, 8             # 	slli	$r2, $r0, #8
	li	x15, 2147483648         # 	move	$r5, #2147483648
	or	x12, x12, x15           # 	or	$r2, $r2, $r5
	li	x11, 0                  # 	move	$r1, #0
	slti	tp, x13, 190            # 	sltsi	$ta, $r3, #190
	beqz	tp, .LCinfnan           # 	beqzs8	.LCinfnan
	li	x6, 190                 # 	subri	$r3, $r3, #190
	sub	x13, x6, x13
.LL8:
	li	x15, 32                 # 	move	$r5, #32
	sltu	tp, x13, x15            # 	slt	$ta, $r3, $r5
	bnez	tp, .LL9                # 	bnezs8	.LL9
	mv	x11, x12                # 	move	$r1, $r2
	li	x12, 0                  # 	move	$r2, #0
	addi	x13, x13, -32           # 	addi	$r3, $r3, #-32
	bnez	x11, .LL8               # 	bnez	$r1, .LL8
.LL9:
	beqz	x13, .LL10              # 	beqz	$r3, .LL10
	mv	x14, x12                # 	move	$r4, $r2
	srl	x11, x11, x13           # 	srl	$r1, $r1, $r3
	srl	x12, x12, x13           # 	srl	$r2, $r2, $r3
	li	x6, 32                  # 	subri	$r3, $r3, #32
	sub	x13, x6, x13
	sll	x14, x14, x13           # 	sll	$r4, $r4, $r3
	or	x11, x11, x14           # 	or	$r1, $r1, $r4
.LL10:
	slti	tp, x10, 0              # 	sltsi	$ta, $r0, #0
	beqz	tp, .LCret              # 	beqzs8	.LCret
	sub	x12, zero, x12          # 	subri	$r2, $r2, #0
	beqz	x11, .LL11              # 	beqz	$r1, .LL11
	sub	x11, zero, x11          # 	subri	$r1, $r1, #0
	addi	x12, x12, -1            # 	subi45	$r2, #1
.LL11:
.LCret:
	mv	x10, x11                # 	move	$r0, $r1
	mv	x11, x12                # 	move	$r1, $r2
	ret                             # 	ret5	$lp
.LCinfnan:
	slti	tp, x10, 0              # 	sltsi	$ta, $r0, #0
	bnez	tp, .LCret3             # 	bnezs8	.LCret3
	li	x6, 255                 # 	subri	$ta, $r3, #255
	sub	tp, x6, x13
	bnez	tp, .Li7                # 	bnezs8	.Li7
	slli	x15, x12, 1             # 	slli	$r5, $r2, #1
	beqz	x15, .Li7               # 	beqz	$r5, .Li7
.LCret3:
	li	x12, 2147483648         # 	move	$r2, #2147483648
	tail	.LCret                  # 	b	.LCret
.Li7:
	li	x12, 2147483647         # 	move	$r2, #2147483647
	li	x11, -1                 # 	move	$r1, #-1
	tail	.LCret                  # 	b	.LCret
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
	slli	x11, x10, 1             # 	slli	$r1, $r0, #1
	slli	x12, x11, 7             # 	slli	$r2, $r1, #7
	srli	x11, x11, 24            # 	srli	$r1, $r1, #24
	li	x6, 158                 # 	subri	$r1, $r1, #158
	sub	x11, x6, x11
	li	x15, 2147483648         # 	move	$r5, #2147483648
	blez	x11, .LJover            # 	blez	$r1, .LJover
	slti	tp, x11, 32             # 	sltsi	$ta, $r1, #32
	beqz	tp, .LJzero             # 	beqz	$ta, .LJzero
	or	x12, x12, x15           # 	or	$r2, $r2, $r5
	srl	x12, x12, x11           # 	srl	$r2, $r2, $r1
	slti	tp, x10, 0              # 	sltsi	$ta, $r0, #0
	sub	x10, zero, x12          # 	subri	$r0, $r2, #0
	bne	tp, zero, .Ltmp0        # 	cmovz	$r0, $r2, $ta
	addi	x10, x12, 0
.Ltmp0:
	ret                             # 	ret5	$lp
.LJzero:
	li	x10, 0                  # 	move	$r0, #0
	ret                             # 	ret5	$lp
.LJover:
	li	x14, 2139095040         # 	move	$r4, #2139095040
	sltu	tp, x14, x10            # 	slt	$ta, $r4, $r0
	beqz	tp, .LJnan              # 	beqzs8	.LJnan
	mv	x10, x15                # 	move	$r0, $r5
	ret                             # 	ret5	$lp
.LJnan:
	addi	x10, x15, -1            # 	addi	$r0, $r5, #-1
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -4              # 	pushm	$r6, $r6
	sw	x9, 0(sp)
	slli	x15, x11, 1             # 	slli	$r5, $r1, #1
	srli	x15, x15, 21            # 	srli	$r5, $r5, #21
	slli	x14, x11, 11            # 	slli	$r4, $r1, #11
	srli	x9, x10, 21             # 	srli	$r6, $r0, #21
	or	x14, x14, x9            # 	or	$r4, $r4, $r6
	slli	x13, x10, 11            # 	slli	$r3, $r0, #11
	li	x9, 2147483648          # 	move	$r6, #2147483648
	or	x14, x14, x9            # 	or	$r4, $r4, $r6
	sltiu	tp, x15, 1086           # 	slti	$ta, $r5, #1086
	beqz	tp, .LCnaninf           # 	beqzs8	.LCnaninf
	li	x6, 1086                # 	subri	$r2, $r5, #1086
	sub	x12, x6, x15
.LL14:
	li	x9, 32                  # 	move	$r6, #32
	sltu	tp, x12, x9             # 	slt	$ta, $r2, $r6
	bnez	tp, .LL15               # 	bnezs8	.LL15
	mv	x13, x14                # 	move	$r3, $r4
	li	x14, 0                  # 	move	$r4, #0
	addi	x12, x12, -32           # 	addi	$r2, $r2, #-32
	bnez	x13, .LL14              # 	bnez	$r3, .LL14
.LL15:
	beqz	x12, .LL16              # 	beqz	$r2, .LL16
	mv	x10, x14                # 	move	$r0, $r4
	srl	x13, x13, x12           # 	srl	$r3, $r3, $r2
	srl	x14, x14, x12           # 	srl	$r4, $r4, $r2
	li	x6, 32                  # 	subri	$r2, $r2, #32
	sub	x12, x6, x12
	sll	x10, x10, x12           # 	sll	$r0, $r0, $r2
	or	x13, x13, x10           # 	or	$r3, $r3, $r0
.LL16:
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	beqz	tp, .LCret              # 	beqzs8	.LCret
	sub	x14, zero, x14          # 	subri	$r4, $r4, #0
	beqz	x13, .LL17              # 	beqz	$r3, .LL17
	sub	x13, zero, x13          # 	subri	$r3, $r3, #0
	addi	x14, x14, -1            # 	subi45	$r4, #1
.LL17:
.LCret:
	mv	x10, x13                # 	move	$r0, $r3
	mv	x11, x14                # 	move	$r1, $r4
	lw	x9, 0(sp)               # 	popm	$r6, $r6
	addi	sp, sp, 4
	ret                             # 	ret5	$lp
.LCnaninf:
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	bnez	tp, .LCret3             # 	bnezs8	.LCret3
	li	x6, 2047                # 	subri	$ta, $r5, #2047
	sub	tp, x6, x15
	bnez	tp, .Li5                # 	bnezs8	.Li5
	slli	x9, x14, 1              # 	slli	$r6, $r4, #1
	or	x9, x9, x13             # 	or	$r6, $r6, $r3
	beqz	x9, .Li5                # 	beqz	$r6, .Li5
.LCret3:
	li	x14, 2147483648         # 	move	$r4, #2147483648
	li	x13, 0                  # 	move	$r3, #0
	tail	.LCret                  # 	b	.LCret
.Li5:
	li	x14, 2147483647         # 	move	$r4, #2147483647
	li	x13, -1                 # 	move	$r3, #-1
	tail	.LCret                  # 	b	.LCret
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
	slli	x13, x11, 11            # 	slli	$r3, $r1, #11
	srli	x14, x10, 21            # 	srli	$r4, $r0, #21
	or	x13, x13, x14           # 	or	$r3, $r3, $r4
	li	x14, 2147483648         # 	move	$r4, #2147483648
	or	x13, x13, x14           # 	or	$r3, $r3, $r4
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	srli	x14, x14, 21            # 	srli	$r4, $r4, #21
	li	x6, 1054                # 	subri	$r2, $r4, #1054
	sub	x12, x6, x14
	blez	x12, .LLnaninf          # 	blez	$r2, .LLnaninf
	li	x14, 32                 # 	move	$r4, #32
	sltu	tp, x12, x14            # 	slt	$ta, $r2, $r4
	bnez	tp, .LL72               # 	bnezs8	.LL72
	li	x13, 0                  # 	move	$r3, #0
.LL72:
	srl	x13, x13, x12           # 	srl	$r3, $r3, $r2
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	beqz	tp, .Li50               # 	beqzs8	.Li50
	sub	x13, zero, x13          # 	subri	$r3, $r3, #0
.Li50:
	mv	x10, x13                # 	move	$r0, $r3
	ret                             # 	ret5	$lp
.LLnaninf:
	beqz	x10, .Li51              # 	beqz	$r0, .Li51
	ori	x11, x11, 1             # 	ori	$r1, $r1, #1
.Li51:
	li	x14, 2146435072         # 	move	$r4, #2146435072
	sltu	tp, x14, x11            # 	slt	$ta, $r4, $r1
	beqz	tp, .Li52               # 	beqzs8	.Li52
	li	x10, 2147483648         # 	move	$r0, #2147483648
	ret                             # 	ret5	$lp
.Li52:
	li	x10, 2147483647         # 	move	$r0, #2147483647
	ret                             # 	ret5	$lp
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
	bltz	x10, .LZero             # 	bltz	$r0, .LZero
	srli	x13, x10, 23            # 	srli	$r3, $r0, #23
	addi	x13, x13, -127          # 	addi	$r3, $r3, #-127
	bltz	x13, .LZero             # 	bltz	$r3, .LZero
	slti	tp, x13, 32             # 	sltsi	$ta, $r3, #32
	beqz	tp, .LMax               # 	beqzs8	.LMax
	slli	x10, x10, 8             # 	slli	$r0, $r0, #8
	lui	x12, 524288             # 	sethi	$r2, #524288
	or	x11, x10, x12           # 	or	$r1, $r0, $r2
	li	x6, 31                  # 	subri	$r0, $r3, #31
	sub	x10, x6, x13
	srl	x10, x11, x10           # 	srl	$r0, $r1, $r0
	ret                             # 	ret5	$lp
.LZero:
	li	x10, 0                  # 	movi55	$r0, #0
	ret                             # 	ret5	$lp
.LMax:
	li	x10, -1                 # 	movi55	$r0, #-1
	ret                             # 	ret5	$lp
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
	slli	x13, x11, 11            # 	slli	$r3, $r1, #11
	srli	x14, x10, 21            # 	srli	$r4, $r0, #21
	or	x13, x13, x14           # 	or	$r3, $r3, $r4
	li	x14, 2147483648         # 	move	$r4, #2147483648
	or	x13, x13, x14           # 	or	$r3, $r3, $r4
	slli	x14, x11, 1             # 	slli	$r4, $r1, #1
	srli	x14, x14, 21            # 	srli	$r4, $r4, #21
	li	x6, 1054                # 	subri	$r2, $r4, #1054
	sub	x12, x6, x14
	slti	tp, x12, 0              # 	sltsi	$ta, $r2, #0
	bnez	tp, .LNnaninf           # 	bnezs8	.LNnaninf
	li	x14, 32                 # 	move	$r4, #32
	sltu	tp, x12, x14            # 	slt	$ta, $r2, $r4
	bnez	tp, .LL73               # 	bnezs8	.LL73
	li	x13, 0                  # 	move	$r3, #0
.LL73:
	srl	x13, x13, x12           # 	srl	$r3, $r3, $r2
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	beqz	tp, .Li53               # 	beqzs8	.Li53
	sub	x13, zero, x13          # 	subri	$r3, $r3, #0
.Li53:
	mv	x10, x13                # 	move	$r0, $r3
	ret                             # 	ret5	$lp
.LNnaninf:
	beqz	x10, .Li54              # 	beqz	$r0, .Li54
	ori	x11, x11, 1             # 	ori	$r1, $r1, #1
.Li54:
	li	x14, 2146435072         # 	move	$r4, #2146435072
	sltu	tp, x14, x11            # 	slt	$ta, $r4, $r1
	beqz	tp, .Li55               # 	beqzs8	.Li55
	li	x10, 2147483648         # 	move	$r0, #2147483648
	ret                             # 	ret5	$lp
.Li55:
	li	x10, -1                 # 	move	$r0, #-1
	ret                             # 	ret5	$lp
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
	bltz	x10, .LZero             # 	bltz	$r0, .LZero
	srli	x12, x10, 23            # 	srli	$r2, $r0, #23
	addi	x12, x12, -127          # 	addi	$r2, $r2, #-127
	bltz	x12, .LZero             # 	bltz	$r2, .LZero
	slti	tp, x12, 64             # 	sltsi	$ta, $r2, #64
	beqz	tp, .LMax               # 	beqzs8	.LMax
	slli	x10, x10, 8             # 	slli	$r0, $r0, #8
	lui	x13, 524288             # 	sethi	$r3, #524288
	or	x10, x10, x13           # 	or33	$r0, $r3
	li	x6, 31                  # 	subri	$r3, $r2, #31
	sub	x13, x6, x12
	bltz	x13, .Lgt31             # 	bltz	$r3, .Lgt31
	srl	x10, x10, x13           # 	srl	$r0, $r0, $r3
	li	x11, 0                  # 	movi55	$r1, #0
	ret                             # 	ret5	$lp
.Lgt31:
	li	x6, 63                  # 	subri	$r2, $r2, #63
	sub	x12, x6, x12
	sub	x13, zero, x13          # 	neg33	$r3, $r3
	srl	x11, x10, x12           # 	srl	$r1, $r0, $r2
	sll	x10, x10, x13           # 	sll	$r0, $r0, $r3
	li	x6, 32                  # 	beqc	$r3, #32, .LClrL
	beq	x13, x6, .LClrL
	ret                             # 	ret5	$lp
.LZero:
	li	x11, 0                  # 	movi55	$r1, #0
.LClrL:
	li	x10, 0                  # 	movi55	$r0, #0
	ret                             # 	ret5	$lp
.LMax:
	li	x10, -1                 # 	movi55	$r0, #-1
	li	x11, -1                 # 	movi55	$r1, #-1
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -4              # 	pushm	$r6, $r6
	sw	x9, 0(sp)
	slli	x15, x11, 1             # 	slli	$r5, $r1, #1
	srli	x15, x15, 21            # 	srli	$r5, $r5, #21
	slli	x14, x11, 11            # 	slli	$r4, $r1, #11
	srli	x9, x10, 21             # 	srli	$r6, $r0, #21
	or	x14, x14, x9            # 	or	$r4, $r4, $r6
	slli	x13, x10, 11            # 	slli	$r3, $r0, #11
	li	x9, 2147483648          # 	move	$r6, #2147483648
	or	x14, x14, x9            # 	or	$r4, $r4, $r6
	sltiu	tp, x15, 1086           # 	slti	$ta, $r5, #1086
	beqz	tp, .LDnaninf           # 	beqzs8	.LDnaninf
	li	x6, 1086                # 	subri	$r2, $r5, #1086
	sub	x12, x6, x15
.LL18:
	li	x9, 32                  # 	move	$r6, #32
	sltu	tp, x12, x9             # 	slt	$ta, $r2, $r6
	bnez	tp, .LL19               # 	bnezs8	.LL19
	mv	x13, x14                # 	move	$r3, $r4
	li	x14, 0                  # 	move	$r4, #0
	addi	x12, x12, -32           # 	addi	$r2, $r2, #-32
	bnez	x13, .LL18              # 	bnez	$r3, .LL18
.LL19:
	beqz	x12, .LL20              # 	beqz	$r2, .LL20
	mv	x10, x14                # 	move	$r0, $r4
	srl	x13, x13, x12           # 	srl	$r3, $r3, $r2
	srl	x14, x14, x12           # 	srl	$r4, $r4, $r2
	li	x6, 32                  # 	subri	$r2, $r2, #32
	sub	x12, x6, x12
	sll	x10, x10, x12           # 	sll	$r0, $r0, $r2
	or	x13, x13, x10           # 	or	$r3, $r3, $r0
.LL20:
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	beqz	tp, .LDret              # 	beqzs8	.LDret
	sub	x14, zero, x14          # 	subri	$r4, $r4, #0
	beqz	x13, .LL21              # 	beqz	$r3, .LL21
	sub	x13, zero, x13          # 	subri	$r3, $r3, #0
	addi	x14, x14, -1            # 	subi45	$r4, #1
.LL21:
.LDret:
	mv	x10, x13                # 	move	$r0, $r3
	mv	x11, x14                # 	move	$r1, $r4
	lw	x9, 0(sp)               # 	popm	$r6, $r6
	addi	sp, sp, 4
	ret                             # 	ret5	$lp
.LDnaninf:
	li	x14, 2147483648         # 	move	$r4, #2147483648
	li	x13, 0                  # 	move	$r3, #0
	tail	.LDret                  # 	b	.LDret
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
	beqz	x10, .LKzero            # 	beqz	$r0, .LKzero
	li	x14, 2147483648         # 	move	$r4, #2147483648
	and	x12, x10, x14           # 	and	$r2, $r0, $r4
	beqz	x12, .LKcont            # 	beqz	$r2, .LKcont
	sub	x10, zero, x10          # 	subri	$r0, $r0, #0
.LKcont:
	li	x11, 158                # 	move	$r1, #158
	li	x15, 16                 # 	move	$r5, #16
	li	x13, 0                  # 	move	$r3, #0
.LKloop:
	add	x13, x13, x15           # 	add	$r3, $r3, $r5
	srl	tp, x10, x13            # 	srl	$ta, $r0, $r3
	bnez	tp, .LKloop2            # 	bnez	$ta, .LKloop2
	sll	x10, x10, x15           # 	sll	$r0, $r0, $r5
	sub	x11, x11, x15           # 	sub	$r1, $r1, $r5
.LKloop2:
	srli	x15, x15, 1             # 	srli	$r5, $r5, #1
	bnez	x15, .LKloop            # 	bnez	$r5, .LKloop
	srli	x14, x14, 24            # 	srli	$r4, $r4, #24
	add	x10, x10, x14           # 	add	$r0, $r0, $r4
	sltu	tp, x10, x14            # 	slt	$ta, $r0, $r4
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	srai	x14, x10, 8             # 	srai	$r4, $r0, #8
	andi	x14, x14, 1             # 	andi	$r4, $r4, #1
	sub	x10, x10, x14           # 	sub	$r0, $r0, $r4
	slli	x10, x10, 1             # 	slli	$r0, $r0, #1
	srli	x10, x10, 9             # 	srli	$r0, $r0, #9
	slli	x14, x11, 23            # 	slli	$r4, $r1, #23
	or	x10, x10, x14           # 	or	$r0, $r0, $r4
	or	x10, x10, x12           # 	or	$r0, $r0, $r2
.LKzero:
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -8              # 	push.s	$r6, $r6, {$lp}
	sw	x9, 0(sp)
	sw	ra, 4(sp)
	li	x11, 0                  # 	move	$r1, #0
	mv	x15, x11                # 	move	$r5, $r1
	mv	x13, x11                # 	move	$r3, $r1
	mv	x12, x10                # 	move	$r2, $r0
	beqz	x12, .Li39              # 	beqz	$r2, .Li39
	slti	tp, x12, 0              # 	sltsi	$ta, $r2, #0
	beqz	tp, .Li40               # 	beqzs8	.Li40
	li	x15, 2147483648         # 	move	$r5, #2147483648
	sub	x12, zero, x12          # 	subri	$r2, $r2, #0
	beqz	x11, .LL71              # 	beqz	$r1, .LL71
	sub	x11, zero, x11          # 	subri	$r1, $r1, #0
	addi	x12, x12, -1            # 	subi45	$r2, #1
.LL71:
.Li40:
	li	x13, 1054               # 	move	$r3, #1054
	addi	sp, sp, -16             # 	pushm	$r0, $r3
	sw	x10, 0(sp)
	sw	x11, 4(sp)
	sw	x12, 8(sp)
	sw	x13, 12(sp)
	addi	sp, sp, -4              # 	push	$r5
	sw	x15, 0(sp)
	mv	x10, x12                # 	move	$r0, $r2
	call	__clzsi2                # 	bal	__clzsi2
	mv	x14, x10                # 	move	$r4, $r0
	lw	x15, 0(sp)              # 	pop	$r5
	addi	sp, sp, 4
	lw	x10, 0(sp)              # 	popm	$r0, $r3
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	addi	sp, sp, 16
	sub	x13, x13, x14           # 	sub	$r3, $r3, $r4
	sll	x12, x12, x14           # 	sll	$r2, $r2, $r4
.Li39:
	srli	x14, x11, 11            # 	srli	$r4, $r1, #11
	slli	x9, x12, 21             # 	slli	$r6, $r2, #21
	or	x14, x14, x9            # 	or	$r4, $r4, $r6
	slli	x9, x12, 1              # 	slli	$r6, $r2, #1
	srli	x9, x9, 12              # 	srli	$r6, $r6, #12
	or	x15, x15, x9            # 	or	$r5, $r5, $r6
	slli	x9, x13, 20             # 	slli	$r6, $r3, #20
	or	x15, x15, x9            # 	or	$r5, $r5, $r6
	mv	x10, x14                # 	move	$r0, $r4
	mv	x11, x15                # 	move	$r1, $r5
	lw	x9, 0(sp)               # 	pop.s	$r6, $r6, {$lp}
	lw	ra, 4(sp)
	addi	sp, sp, 8
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -12             # 	push.s	$r6, $r7, {$lp}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	sw	ra, 8(sp)
	li	x5, 2147483648          # 	move	$r7, #2147483648
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	and	$r5, $r1, $r7
	lw	x6, 0(x6)
	and	x15, x11, x6
	mv	x13, x11                # 	move	$r3, $r1
	mv	x12, x10                # 	move	$r2, $r0
	or	x5, x11, x10            # 	or	$r7, $r1, $r0
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	beqz	$r7, .Li1
	lw	x5, 0(x5)
	beqz	x5, .Li1
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	beqz	tp, .Li2                # 	beqzs8	.Li2
	sub	x13, zero, x13          # 	subri	$r3, $r3, #0
	beqz	x12, .LL1               # 	beqz	$r2, .LL1
	sub	x12, zero, x12          # 	subri	$r2, $r2, #0
	addi	x13, x13, -1            # 	subi45	$r3, #1
.LL1:
.Li2:
	li	x14, 190                # 	move	$r4, #190
	bnez	x13, .LL2               # 	bnez	$r3, .LL2
	bnez	x12, .LL3               # 	bnez	$r2, .LL3
	li	x14, 0                  # 	move	$r4, #0
	tail	.LL4                    # 	b	.LL4
.LL3:
	mv	x13, x12                # 	move	$r3, $r2
	li	x12, 0                  # 	move	$r2, #0
	li	x9, 32                  # 	move	$r6, #32
	sub	x14, x14, x9            # 	sub	$r4, $r4, $r6
.LL2:
	addi	sp, sp, -24             # 	pushm	$r0, $r5
	sw	x10, 0(sp)
	sw	x11, 4(sp)
	sw	x12, 8(sp)
	sw	x13, 12(sp)
	sw	x14, 16(sp)
	sw	x15, 20(sp)
	mv	x10, x13                # 	move	$r0, $r3
	call	__clzsi2                # 	bal	__clzsi2
	mv	x9, x10                 # 	move	$r6, $r0
	lw	x10, 0(sp)              # 	popm	$r0, $r5
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	lw	x15, 20(sp)
	addi	sp, sp, 24
	beqz	x9, .LL4                # 	beqz	$r6, .LL4
	sub	x14, x14, x9            # 	sub	$r4, $r4, $r6
	li	x6, 32                  # 	subri	$r0, $r6, #32
	sub	x10, x6, x9
	srl	x10, x12, x10           # 	srl	$r0, $r2, $r0
	sll	x12, x12, x9            # 	sll	$r2, $r2, $r6
	sll	x13, x13, x9            # 	sll	$r3, $r3, $r6
	or	x13, x13, x10           # 	or	$r3, $r3, $r0
.LL4:
	beqz	x12, .Li3               # 	beqz	$r2, .Li3
	ori	x13, x13, 1             # 	ori	$r3, $r3, #1
.Li3:
	li	tp, 128                 # 	move	$ta, #128
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	sltu	tp, x13, tp             # 	slt	$ta, $r3, $ta
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
	srli	x5, x13, 8              # 	srli	$r7, $r3, #8
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	andi	$r7, $r7, #1
	lw	x5, 0(x5)
	andi	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	sub	$r3, $r3, $r7
	lw	x6, 0(x6)
	sub	x13, x13, x6
	slli	x13, x13, 1             # 	slli	$r3, $r3, #1
	srli	x13, x13, 9             # 	srli	$r3, $r3, #9
	slli	x5, x14, 23             # 	slli	$r7, $r4, #23
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r3, $r3, $r7
	lw	x6, 0(x6)
	or	x13, x13, x6
.Li1:
	or	x10, x13, x15           # 	or	$r0, $r3, $r5
.LA999:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r7, {$lp}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	ra, 8(sp)
	addi	sp, sp, 12
	ret                             # 	ret5	$lp
.Ltmp0:
	.size	__floatdisf, .Ltmp0-__floatdisf
	.comm	__r7,4,4
#endif

#ifdef L_floatdidf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__floatdidf
	.type	__floatdidf,@function
__floatdidf:
	addi	sp, sp, -16             # 	push.s	$r6, $r8, {$lp}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	sw	ra, 12(sp)
	li	x14, 0                  # 	move	$r4, #0
	mv	x5, x14                 # 	move	$r7, $r4
	la	x7, __r7
	sw	x5, 0(x7)
	mv	x13, x11                # 	move	$r3, $r1
	mv	x12, x10                # 	move	$r2, $r0
	or	x5, x11, x10            # 	or	$r8, $r1, $r0
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r8                # 	beqz	$r8, .Li1
	lw	x5, 0(x5)
	beqz	x5, .Li1
	li	x14, 1086               # 	move	$r4, #1086
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	beqz	tp, .Li2                # 	beqzs8	.Li2
	li	x5, 2147483648          # 	move	$r7, #2147483648
	la	x7, __r7
	sw	x5, 0(x7)
	sub	x13, zero, x13          # 	subri	$r3, $r3, #0
	beqz	x12, .LL1               # 	beqz	$r2, .LL1
	sub	x12, zero, x12          # 	subri	$r2, $r2, #0
	addi	x13, x13, -1            # 	subi45	$r3, #1
.LL1:
.Li2:
	bnez	x13, .LL2               # 	bnez	$r3, .LL2
	bnez	x12, .LL3               # 	bnez	$r2, .LL3
	li	x14, 0                  # 	move	$r4, #0
	tail	.LL4                    # 	b	.LL4
.LL3:
	mv	x13, x12                # 	move	$r3, $r2
	li	x12, 0                  # 	move	$r2, #0
	li	x9, 32                  # 	move	$r6, #32
	sub	x14, x14, x9            # 	sub	$r4, $r4, $r6
.LL2:
	addi	sp, sp, -24             # 	pushm	$r0, $r5
	sw	x10, 0(sp)
	sw	x11, 4(sp)
	sw	x12, 8(sp)
	sw	x13, 12(sp)
	sw	x14, 16(sp)
	sw	x15, 20(sp)
	mv	x10, x13                # 	move	$r0, $r3
	call	__clzsi2                # 	bal	__clzsi2
	mv	x9, x10                 # 	move	$r6, $r0
	lw	x10, 0(sp)              # 	popm	$r0, $r5
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	lw	x15, 20(sp)
	addi	sp, sp, 24
	beqz	x9, .LL4                # 	beqz	$r6, .LL4
	sub	x14, x14, x9            # 	sub	$r4, $r4, $r6
	li	x6, 32                  # 	subri	$r5, $r6, #32
	sub	x15, x6, x9
	srl	x15, x12, x15           # 	srl	$r5, $r2, $r5
	sll	x12, x12, x9            # 	sll	$r2, $r2, $r6
	sll	x13, x13, x9            # 	sll	$r3, $r3, $r6
	or	x13, x13, x15           # 	or	$r3, $r3, $r5
.LL4:
	li	tp, 1024                # 	move	$ta, #1024
	add	x12, x12, tp            # 	add	$r2, $r2, $ta
	sltu	tp, x12, tp             # 	slt	$ta, $r2, $ta
	beqz	tp, .LL7                # 	beqzs8	.LL7
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	sltu	tp, x13, tp             # 	slt	$ta, $r3, $ta
.LL7:
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
	srli	x5, x12, 11             # 	srli	$r8, $r2, #11
	la	x7, __r8
	sw	x5, 0(x7)
	la	x5, __r8                # 	andi	$r8, $r8, #1
	lw	x5, 0(x5)
	andi	x5, x5, 1
	la	x7, __r8
	sw	x5, 0(x7)
	la	x6, __r8                # 	sub	$r2, $r2, $r8
	lw	x6, 0(x6)
	sub	x12, x12, x6
.Li1:
	srli	x15, x12, 11            # 	srli	$r5, $r2, #11
	slli	x5, x13, 21             # 	slli	$r8, $r3, #21
	la	x7, __r8
	sw	x5, 0(x7)
	la	x6, __r8                # 	or	$r5, $r5, $r8
	lw	x6, 0(x6)
	or	x15, x15, x6
	slli	x9, x13, 1              # 	slli	$r6, $r3, #1
	srli	x9, x9, 12              # 	srli	$r6, $r6, #12
	slli	x5, x14, 20             # 	slli	$r8, $r4, #20
	la	x7, __r8
	sw	x5, 0(x7)
	la	x6, __r8                # 	or	$r6, $r6, $r8
	lw	x6, 0(x6)
	or	x9, x9, x6
	la	x6, __r7                # 	or	$r6, $r6, $r7
	lw	x6, 0(x6)
	or	x9, x9, x6
	mv	x10, x15                # 	move	$r0, $r5
	mv	x11, x9                 # 	move	$r1, $r6
.LA999:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r8, {$lp}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	ra, 12(sp)
	addi	sp, sp, 16
	ret                             # 	ret5	$lp
.Ltmp0:
	.size	__floatdidf, .Ltmp0-__floatdidf
	.comm	__r7,4,4
	.comm	__r8,4,4
#endif

#ifdef L_floatunsisf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__floatunsisf
	.type	__floatunsisf,@function
__floatunsisf:
	beqz	x10, .LKzero            # 	beqz	$r0, .LKzero
	li	x11, 158                # 	move	$r1, #158
	li	x15, 16                 # 	move	$r5, #16
	li	x13, 0                  # 	move	$r3, #0
.LKloop:
	add	x13, x13, x15           # 	add	$r3, $r3, $r5
	srl	tp, x10, x13            # 	srl	$ta, $r0, $r3
	bnez	tp, .LKloop2            # 	bnez	$ta, .LKloop2
	sll	x10, x10, x15           # 	sll	$r0, $r0, $r5
	sub	x11, x11, x15           # 	sub	$r1, $r1, $r5
.LKloop2:
	srli	x15, x15, 1             # 	srli	$r5, $r5, #1
	bnez	x15, .LKloop            # 	bnez	$r5, .LKloop
	addi	x10, x10, 128           # 	addi	$r0, $r0, #128
	sltiu	tp, x10, 128            # 	slti	$ta, $r0, #128
	add	x11, x11, tp            # 	add	$r1, $r1, $ta
	srli	x14, x10, 8             # 	srli	$r4, $r0, #8
	andi	x14, x14, 1             # 	andi	$r4, $r4, #1
	sub	x10, x10, x14           # 	sub	$r0, $r0, $r4
	slli	x10, x10, 1             # 	slli	$r0, $r0, #1
	srli	x10, x10, 9             # 	srli	$r0, $r0, #9
	slli	x14, x11, 23            # 	slli	$r4, $r1, #23
	or	x10, x10, x14           # 	or	$r0, $r0, $r4
.LKzero:
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -8              # 	push.s	$r6, $r6, {$lp}
	sw	x9, 0(sp)
	sw	ra, 4(sp)
	li	x11, 0                  # 	move	$r1, #0
	mv	x13, x11                # 	move	$r3, $r1
	mv	x12, x10                # 	move	$r2, $r0
	beqz	x12, .Li41              # 	beqz	$r2, .Li41
	li	x13, 1054               # 	move	$r3, #1054
	addi	sp, sp, -20             # 	pushm	$r0, $r4
	sw	x10, 0(sp)
	sw	x11, 4(sp)
	sw	x12, 8(sp)
	sw	x13, 12(sp)
	sw	x14, 16(sp)
	mv	x10, x12                # 	move	$r0, $r2
	call	__clzsi2                # 	bal	__clzsi2
	mv	x15, x10                # 	move	$r5, $r0
	lw	x10, 0(sp)              # 	popm	$r0, $r4
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	addi	sp, sp, 20
	sub	x13, x13, x15           # 	sub	$r3, $r3, $r5
	sll	x12, x12, x15           # 	sll	$r2, $r2, $r5
.Li41:
	srli	x14, x11, 11            # 	srli	$r4, $r1, #11
	slli	x9, x12, 21             # 	slli	$r6, $r2, #21
	or	x14, x14, x9            # 	or	$r4, $r4, $r6
	slli	x15, x12, 1             # 	slli	$r5, $r2, #1
	srli	x15, x15, 12            # 	srli	$r5, $r5, #12
	slli	x9, x13, 20             # 	slli	$r6, $r3, #20
	or	x15, x15, x9            # 	or	$r5, $r5, $r6
	mv	x10, x14                # 	move	$r0, $r4
	mv	x11, x15                # 	move	$r1, $r5
	lw	x9, 0(sp)               # 	pop.s	$r6, $r6, {$lp}
	lw	ra, 4(sp)
	addi	sp, sp, 8
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -8              # 	push.s	$r6, $r6, {$lp}
	sw	x9, 0(sp)
	sw	ra, 4(sp)
	mv	x13, x11                # 	move	$r3, $r1
	mv	x12, x10                # 	move	$r2, $r0
	or	x9, x11, x10            # 	or	$r6, $r1, $r0
	beqz	x9, .Li4                # 	beqz	$r6, .Li4
	li	x14, 190                # 	move	$r4, #190
	bnez	x13, .LL5               # 	bnez	$r3, .LL5
	bnez	x12, .LL6               # 	bnez	$r2, .LL6
	li	x14, 0                  # 	move	$r4, #0
	tail	.LL7                    # 	b	.LL7
.LL6:
	mv	x13, x12                # 	move	$r3, $r2
	li	x12, 0                  # 	move	$r2, #0
	li	x15, 32                 # 	move	$r5, #32
	sub	x14, x14, x15           # 	sub	$r4, $r4, $r5
.LL5:
	addi	sp, sp, -20             # 	pushm	$r0, $r4
	sw	x10, 0(sp)
	sw	x11, 4(sp)
	sw	x12, 8(sp)
	sw	x13, 12(sp)
	sw	x14, 16(sp)
	mv	x10, x13                # 	move	$r0, $r3
	call	__clzsi2                # 	bal	__clzsi2
	mv	x15, x10                # 	move	$r5, $r0
	lw	x10, 0(sp)              # 	popm	$r0, $r4
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	addi	sp, sp, 20
	beqz	x15, .LL7               # 	beqz	$r5, .LL7
	sub	x14, x14, x15           # 	sub	$r4, $r4, $r5
	li	x6, 32                  # 	subri	$r0, $r5, #32
	sub	x10, x6, x15
	srl	x10, x12, x10           # 	srl	$r0, $r2, $r0
	sll	x12, x12, x15           # 	sll	$r2, $r2, $r5
	sll	x13, x13, x15           # 	sll	$r3, $r3, $r5
	or	x13, x13, x10           # 	or	$r3, $r3, $r0
.LL7:
	beqz	x12, .Li5               # 	beqz	$r2, .Li5
	ori	x13, x13, 1             # 	ori	$r3, $r3, #1
.Li5:
	li	tp, 128                 # 	move	$ta, #128
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	sltu	tp, x13, tp             # 	slt	$ta, $r3, $ta
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
	srli	x9, x13, 8              # 	srli	$r6, $r3, #8
	andi	x9, x9, 1               # 	andi	$r6, $r6, #1
	sub	x13, x13, x9            # 	sub	$r3, $r3, $r6
	slli	x13, x13, 1             # 	slli	$r3, $r3, #1
	srli	x13, x13, 9             # 	srli	$r3, $r3, #9
	slli	x9, x14, 23             # 	slli	$r6, $r4, #23
	or	x13, x13, x9            # 	or	$r3, $r3, $r6
.Li4:
	mv	x10, x13                # 	move	$r0, $r3
.LB999:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r6, {$lp}
	lw	ra, 4(sp)
	addi	sp, sp, 8
	ret                             # 	ret5	$lp
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
	addi	sp, sp, -12             # 	push.s	$r6, $r7, {$lp}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	sw	ra, 8(sp)
	li	x14, 0                  # 	move	$r4, #0
	mv	x13, x11                # 	move	$r3, $r1
	mv	x12, x10                # 	move	$r2, $r0
	or	x5, x11, x10            # 	or	$r7, $r1, $r0
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	beqz	$r7, .Li3
	lw	x5, 0(x5)
	beqz	x5, .Li3
	li	x14, 1086               # 	move	$r4, #1086
	bnez	x13, .LL8               # 	bnez	$r3, .LL8
	bnez	x12, .LL9               # 	bnez	$r2, .LL9
	li	x14, 0                  # 	move	$r4, #0
	tail	.LL10                   # 	b	.LL10
.LL9:
	mv	x13, x12                # 	move	$r3, $r2
	li	x12, 0                  # 	move	$r2, #0
	li	x9, 32                  # 	move	$r6, #32
	sub	x14, x14, x9            # 	sub	$r4, $r4, $r6
.LL8:
	addi	sp, sp, -24             # 	pushm	$r0, $r5
	sw	x10, 0(sp)
	sw	x11, 4(sp)
	sw	x12, 8(sp)
	sw	x13, 12(sp)
	sw	x14, 16(sp)
	sw	x15, 20(sp)
	mv	x10, x13                # 	move	$r0, $r3
	call	__clzsi2                # 	bal	__clzsi2
	mv	x9, x10                 # 	move	$r6, $r0
	lw	x10, 0(sp)              # 	popm	$r0, $r5
	lw	x11, 4(sp)
	lw	x12, 8(sp)
	lw	x13, 12(sp)
	lw	x14, 16(sp)
	lw	x15, 20(sp)
	addi	sp, sp, 24
	beqz	x9, .LL10               # 	beqz	$r6, .LL10
	sub	x14, x14, x9            # 	sub	$r4, $r4, $r6
	li	x6, 32                  # 	subri	$r5, $r6, #32
	sub	x15, x6, x9
	srl	x15, x12, x15           # 	srl	$r5, $r2, $r5
	sll	x12, x12, x9            # 	sll	$r2, $r2, $r6
	sll	x13, x13, x9            # 	sll	$r3, $r3, $r6
	or	x13, x13, x15           # 	or	$r3, $r3, $r5
.LL10:
	li	tp, 1024                # 	move	$ta, #1024
	add	x12, x12, tp            # 	add	$r2, $r2, $ta
	sltu	tp, x12, tp             # 	slt	$ta, $r2, $ta
	beqz	tp, .LL13               # 	beqzs8	.LL13
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	sltu	tp, x13, tp             # 	slt	$ta, $r3, $ta
.LL13:
	add	x14, x14, tp            # 	add	$r4, $r4, $ta
	srli	x5, x12, 11             # 	srli	$r7, $r2, #11
	la	x7, __r7
	sw	x5, 0(x7)
	la	x5, __r7                # 	andi	$r7, $r7, #1
	lw	x5, 0(x5)
	andi	x5, x5, 1
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	sub	$r2, $r2, $r7
	lw	x6, 0(x6)
	sub	x12, x12, x6
.Li3:
	srli	x15, x12, 11            # 	srli	$r5, $r2, #11
	slli	x5, x13, 21             # 	slli	$r7, $r3, #21
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r5, $r5, $r7
	lw	x6, 0(x6)
	or	x15, x15, x6
	slli	x9, x13, 1              # 	slli	$r6, $r3, #1
	srli	x9, x9, 12              # 	srli	$r6, $r6, #12
	slli	x5, x14, 20             # 	slli	$r7, $r4, #20
	la	x7, __r7
	sw	x5, 0(x7)
	la	x6, __r7                # 	or	$r6, $r6, $r7
	lw	x6, 0(x6)
	or	x9, x9, x6
	mv	x10, x15                # 	move	$r0, $r5
	mv	x11, x9                 # 	move	$r1, $r6
.LB999:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r7, {$lp}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	ra, 8(sp)
	addi	sp, sp, 12
	ret                             # 	ret5	$lp
.Ltmp0:
	.size	__floatundidf, .Ltmp0-__floatundidf
	.comm	__r7,4,4
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
	li	x14, -1                 # 	move	$r4, #-1
	tail	.LA                     # 	b	.LA
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
	li	x14, 1                  # 	move	$r4, #1
	.p2align	2
.LA:
	li	x15, 4278190080         # 	move	$r5, #4278190080
	slli	x12, x10, 1             # 	slli	$r2, $r0, #1
	sltu	tp, x15, x12            # 	slt	$ta, $r5, $r2
	bnez	tp, .LMnan              # 	bnez	$ta, .LMnan
	slli	x13, x11, 1             # 	slli	$r3, $r1, #1
	sltu	tp, x15, x13            # 	slt	$ta, $r5, $r3
	bnez	tp, .LMnan              # 	bnez	$ta, .LMnan
	xor	x15, x10, x11           # 	xor	$r5, $r0, $r1
	bgez	x15, .LSameSign         # 	bgez	$r5, .LSameSign
.LDiffSign:
	or	x12, x12, x13           # 	or	$r2, $r2, $r3
	beqz	x12, .LMequ             # 	beqz	$r2, .LMequ
	li	x12, 1                  # 	move	$r2, #1
	bne	x10, zero, .Ltmp0       # 	cmovz	$r0, $r2, $r0
	addi	x10, x12, 0
.Ltmp0:
	ret                             # 	ret5	$lp
.LSameSign:
	slti	tp, x10, 0              # 	sltsi	$ta, $r0, #0
	bnez	tp, .LSameSignNeg       # 	bnez	$ta, .LSameSignNeg
.LSameSignPos:
	sub	x10, x10, x11           # 	sub	$r0, $r0, $r1
	ret                             # 	ret5	$lp
.LSameSignNeg:
	sub	x10, x11, x10           # 	sub	$r0, $r1, $r0
	ret                             # 	ret5	$lp
.LMequ:
	li	x10, 0                  # 	move	$r0, #0
	ret                             # 	ret5	$lp
.LMnan:
	mv	x10, x14                # 	move	$r0, $r4
	ret                             # 	ret5	$lp
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
	li	x14, -1                 # 	move	$r4, #-1
	tail	.LA                     # 	b	.LA
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
	li	x14, 1                  # 	move	$r4, #1
.LA:
	li	x15, 0                  # 	move	$r5, #0
	addi	sp, sp, -16             # 	push.s	$r6, $r9, {}
	sw	x9, 0(sp)
	la	x7, __r7
	lw	x7, 0(x7)
	sw	x7, 4(sp)
	la	x7, __r8
	lw	x7, 0(x7)
	sw	x7, 8(sp)
	la	x7, __r9
	lw	x7, 0(x7)
	sw	x7, 12(sp)
	li	x5, 4292870144          # 	move	$r8, #4292870144
	la	x7, __r8
	sw	x5, 0(x7)
	slli	x9, x11, 1              # 	slli	$r6, $r1, #1
	sltu	tp, x15, x10            # 	slt	$ta, $r5, $r0
	add	x5, x9, tp              # 	add	$r9, $r6, $ta
	la	x7, __r9
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r8, $r9
	lw	x6, 0(x6)
	la	x5, __r8
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	bnez	tp, .LMnan              # 	bnez	$ta, .LMnan
	slli	x5, x13, 1              # 	slli	$r7, $r3, #1
	la	x7, __r7
	sw	x5, 0(x7)
	sltu	tp, x15, x12            # 	slt	$ta, $r5, $r2
	la	x5, __r7                # 	add	$r9, $r7, $ta
	lw	x5, 0(x5)
	add	x5, x5, tp
	la	x7, __r9
	sw	x5, 0(x7)
	la	x6, __r9                # 	slt	$ta, $r8, $r9
	lw	x6, 0(x6)
	la	x5, __r8
	lw	x5, 0(x5)
	sltu	tp, x5, x6
	bnez	tp, .LMnan              # 	bnez	$ta, .LMnan
	xor	x14, x11, x13           # 	xor	$r4, $r1, $r3
	bltz	x14, .LMdiff            # 	bltz	$r4, .LMdiff
	slti	tp, x11, 0              # 	sltsi	$ta, $r1, #0
	bnez	tp, .LMsame             # 	bnez	$ta, .LMsame
	sub	x14, x10, x12           # 	sub	$r4, $r0, $r2
	sltu	tp, x10, x14            # 	slt	$ta, $r0, $r4
	sub	x10, x11, x13           # 	sub	$r0, $r1, $r3
	sub	x10, x10, tp            # 	sub	$r0, $r0, $ta
	sltu	tp, x15, x14            # 	slt	$ta, $r5, $r4
	bne	x10, zero, .Ltmp0       # 	cmovz	$r0, $ta, $r0
	addi	x10, tp, 0
.Ltmp0:
	tail	.LMret                  # 	b	.LMret
.LMsame:
	sub	x14, x12, x10           # 	sub	$r4, $r2, $r0
	sltu	tp, x12, x14            # 	slt	$ta, $r2, $r4
	sub	x10, x13, x11           # 	sub	$r0, $r3, $r1
	sub	x10, x10, tp            # 	sub	$r0, $r0, $ta
	sltu	tp, x15, x14            # 	slt	$ta, $r5, $r4
	bne	x10, zero, .Ltmp1       # 	cmovz	$r0, $ta, $r0
	addi	x10, tp, 0
.Ltmp1:
	tail	.LMret                  # 	b	.LMret
.LMdiff:
	la	x6, __r7                # 	or	$r6, $r6, $r7
	lw	x6, 0(x6)
	or	x9, x9, x6
	or	x14, x10, x12           # 	or	$r4, $r0, $r2
	or	x9, x9, x14             # 	or	$r6, $r6, $r4
	beqz	x9, .LMret              # 	beqz	$r6, .LMret
	li	x10, 1                  # 	movi	$r0, #1
	beq	x11, zero, .Ltmp2       # 	cmovn	$r0, $r1, $r1
	addi	x10, x11, 0
.Ltmp2:
.LMret:
	lw	x9, 0(sp)               # 	pop.s	$r6, $r9, {}
	lw	x5, 4(sp)
	la	x7, __r7
	sw	x5, 0(x7)
	lw	x5, 8(sp)
	la	x7, __r8
	sw	x5, 0(x7)
	lw	x5, 12(sp)
	la	x7, __r9
	sw	x5, 0(x7)
	addi	sp, sp, 16
	ret                             # 	ret5	$lp
.LMnan:
	mv	x10, x14                # 	move	$r0, $r4
	tail	.LMret                  # 	b	.LMret
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
	.comm	__r7,4,4
	.comm	__r8,4,4
	.comm	__r9,4,4
#endif

#ifdef L_unord_sf
	.text
	.section	.mdebug.abi_nds32,"",@progbits
	.text
	.p2align	2
	.globl	__unordsf2
	.type	__unordsf2,@function
__unordsf2:
	slli	x10, x10, 1             # 	slli	$r0, $r0, #1
	li	x13, 4278190080         # 	move	$r3, #4278190080
	sltu	x10, x13, x10           # 	slt	$r0, $r3, $r0
	bnez	x10, .Li67              # 	bnez	$r0, .Li67
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	sltu	x10, x13, x11           # 	slt	$r0, $r3, $r1
.Li67:
	ret                             # 	ret5	$lp
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
	slli	x11, x11, 1             # 	slli	$r1, $r1, #1
	li	x14, 0                  # 	move	$r4, #0
	sltu	tp, x14, x10            # 	slt	$ta, $r4, $r0
	add	x10, x11, tp            # 	add	$r0, $r1, $ta
	li	x15, 4292870144         # 	move	$r5, #4292870144
	sltu	x10, x15, x10           # 	slt	$r0, $r5, $r0
	bnez	x10, .Li69              # 	bnez	$r0, .Li69
	slli	x13, x13, 1             # 	slli	$r3, $r3, #1
	sltu	tp, x14, x12            # 	slt	$ta, $r4, $r2
	add	x13, x13, tp            # 	add	$r3, $r3, $ta
	sltu	x10, x15, x13           # 	slt	$r0, $r5, $r3
.Li69:
	ret                             # 	ret5	$lp
.Ltmp0:
	.size	__unorddf2, .Ltmp0-__unorddf2
#endif

