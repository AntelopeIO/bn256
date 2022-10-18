	.text
	.file	"gfp_amd64.cpp"
	.globl	gfp_neg                         # -- Begin function gfp_neg
	.p2align	4, 0x90
	.type	gfp_neg,@function
gfp_neg:                                # @gfp_neg
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movabsq	$4332616871279656263, %r8       # imm = 0x3C208C16D87CFD47
	movq	%r8, %rdx
	subq	16(%rsp), %rdx
	movabsq	$-7529619929231668595, %r9      # imm = 0x97816A916871CA8D
	movq	%r9, %rsi
	sbbq	24(%rsp), %rsi
	movabsq	$-5165552122434856867, %r10     # imm = 0xB85045B68181585D
	movq	%r10, %rcx
	sbbq	32(%rsp), %rcx
	movabsq	$3486998266802970665, %r11      # imm = 0x30644E72E131A029
	movq	%r11, %rbx
	sbbq	40(%rsp), %rbx
	movq	%rdi, %rax
	movq	%rdx, -72(%rsp)
	movq	%rsi, -64(%rsp)
	movq	%rcx, -56(%rsp)
	movq	%rbx, -48(%rsp)
	xorl	%edi, %edi
	subq	%r8, %rdx
	sbbq	%r9, %rsi
	sbbq	%r10, %rcx
	sbbq	%r11, %rbx
	movq	%rdx, -32(%rsp)
	movq	%rsi, -24(%rsp)
	movq	%rcx, -16(%rsp)
	movq	%rbx, -8(%rsp)
	sbbq	%rdi, %rdi
	leaq	-40(%rsp), %rcx
	leaq	-80(%rsp), %rdx
	cmovaeq	%rcx, %rdx
	movups	8(%rdx), %xmm0
	movups	24(%rdx), %xmm1
	movups	%xmm1, 16(%rax)
	movups	%xmm0, (%rax)
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	gfp_neg, .Lfunc_end0-gfp_neg
	.cfi_endproc
                                        # -- End function
	.globl	gfp_add                         # -- Begin function gfp_add
	.p2align	4, 0x90
	.type	gfp_add,@function
gfp_add:                                # @gfp_add
	.cfi_startproc
# %bb.0:
	movq	8(%rsp), %r8
	addq	40(%rsp), %r8
	movq	16(%rsp), %rdx
	adcq	48(%rsp), %rdx
	movq	24(%rsp), %rsi
	adcq	56(%rsp), %rsi
	movq	32(%rsp), %rcx
	adcq	64(%rsp), %rcx
	movq	%rdi, %rax
	setb	%dil
	movq	%r8, -72(%rsp)
	movq	%rdx, -64(%rsp)
	movq	%rsi, -56(%rsp)
	movq	%rcx, -48(%rsp)
	movzbl	%dil, %r9d
	movabsq	$4332616871279656263, %rdi      # imm = 0x3C208C16D87CFD47
	subq	%rdi, %r8
	movabsq	$-7529619929231668595, %rdi     # imm = 0x97816A916871CA8D
	sbbq	%rdi, %rdx
	movabsq	$-5165552122434856867, %rdi     # imm = 0xB85045B68181585D
	sbbq	%rdi, %rsi
	movabsq	$3486998266802970665, %rdi      # imm = 0x30644E72E131A029
	sbbq	%rdi, %rcx
	movq	%r8, -32(%rsp)
	movq	%rdx, -24(%rsp)
	movq	%rsi, -16(%rsp)
	movq	%rcx, -8(%rsp)
	sbbq	$0, %r9
	leaq	-40(%rsp), %rcx
	leaq	-80(%rsp), %rdx
	cmovaeq	%rcx, %rdx
	movups	8(%rdx), %xmm0
	movups	24(%rdx), %xmm1
	movups	%xmm1, 16(%rax)
	movups	%xmm0, (%rax)
	retq
.Lfunc_end1:
	.size	gfp_add, .Lfunc_end1-gfp_add
	.cfi_endproc
                                        # -- End function
	.globl	gfp_sub                         # -- Begin function gfp_sub
	.p2align	4, 0x90
	.type	gfp_sub,@function
gfp_sub:                                # @gfp_sub
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movabsq	$4332616871279656263, %r8       # imm = 0x3C208C16D87CFD47
	movq	%r8, %rdx
	subq	48(%rsp), %rdx
	movabsq	$-7529619929231668595, %r9      # imm = 0x97816A916871CA8D
	movq	%r9, %rsi
	sbbq	56(%rsp), %rsi
	movabsq	$-5165552122434856867, %r10     # imm = 0xB85045B68181585D
	movq	%r10, %rcx
	sbbq	64(%rsp), %rcx
	movabsq	$3486998266802970665, %r11      # imm = 0x30644E72E131A029
	movq	%r11, %rbx
	sbbq	72(%rsp), %rbx
	addq	16(%rsp), %rdx
	adcq	24(%rsp), %rsi
	adcq	32(%rsp), %rcx
	adcq	40(%rsp), %rbx
	movq	%rdi, %rax
	setb	%dil
	movq	%rdx, -72(%rsp)
	movq	%rsi, -64(%rsp)
	movq	%rcx, -56(%rsp)
	movq	%rbx, -48(%rsp)
	movzbl	%dil, %edi
	subq	%r8, %rdx
	sbbq	%r9, %rsi
	sbbq	%r10, %rcx
	sbbq	%r11, %rbx
	movq	%rdx, -32(%rsp)
	movq	%rsi, -24(%rsp)
	movq	%rcx, -16(%rsp)
	movq	%rbx, -8(%rsp)
	sbbq	$0, %rdi
	leaq	-40(%rsp), %rcx
	leaq	-80(%rsp), %rdx
	cmovaeq	%rcx, %rdx
	movups	8(%rdx), %xmm0
	movups	24(%rdx), %xmm1
	movups	%xmm1, 16(%rax)
	movups	%xmm0, (%rax)
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	gfp_sub, .Lfunc_end2-gfp_sub
	.cfi_endproc
                                        # -- End function
	.ident	"Ubuntu clang version 13.0.1-++20220120110924+75e33f71c2da-1~exp1~20220120231001.58"
	.section	".note.GNU-stack","",@progbits
