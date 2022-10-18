	.text
	.file	"gfp_mul_amd64.cpp"
	.globl	gfp_mul                         # -- Begin function gfp_mul
	.p2align	4, 0x90
	.type	gfp_mul,@function
gfp_mul:                                # @gfp_mul
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	subq	$48, %rsp
	.cfi_def_cfa_offset 104
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movq	104(%rsp), %rsi
	movq	152(%rsp), %rax
	movq	%rax, -120(%rsp)                # 8-byte Spill
	movq	%rax, %rdx
	mulxq	%rsi, %r10, %rbp
	movq	160(%rsp), %r8
	movq	%r8, %rdx
	movq	%r8, -128(%rsp)                 # 8-byte Spill
	mulxq	%rsi, %rcx, %rbx
	addq	%rbp, %rcx
	adcq	$0, %rbx
	movq	112(%rsp), %rbp
	movq	%rax, %rdx
	mulxq	%rbp, %r15, %rax
	addq	%rcx, %r15
	adcq	%rbx, %rax
	movq	%r8, %rdx
	mulxq	%rbp, %r14, %r9
	setb	%cl
	movzbl	%cl, %ecx
	addq	%rax, %r14
	adcq	%rcx, %r9
	movq	136(%rsp), %rax
	movq	%rax, %rdx
	mulxq	%rsi, %rcx, %r12
	movq	%rcx, -104(%rsp)                # 8-byte Spill
	movq	144(%rsp), %r13
	movq	%r13, %rdx
	mulxq	%rsi, %rsi, %rbx
	addq	%r12, %rsi
	adcq	$0, %rbx
	movq	%rax, %rdx
	mulxq	%rbp, %rdx, %rcx
	addq	%rsi, %rdx
	movq	%rdx, -112(%rsp)                # 8-byte Spill
	adcq	%rbx, %rcx
	movq	%r13, %rdx
	mulxq	%rbp, %rsi, %r12
	setb	%dl
	movzbl	%dl, %edx
	addq	%rcx, %rsi
	adcq	%rdx, %r12
	addq	%r10, %rsi
	adcq	%r15, %r12
	adcq	$0, %r14
	adcq	$0, %r9
	movq	120(%rsp), %r15
	movq	%rax, %rdx
	mulxq	%r15, %r11, %r10
	movq	%r13, %rdx
	mulxq	%r15, %rcx, %rbp
	addq	%r10, %rcx
	adcq	$0, %rbp
	movq	128(%rsp), %r10
	movq	%rax, %rdx
	mulxq	%r10, %r8, %rbx
	addq	%rcx, %r8
	adcq	%rbp, %rbx
	movq	%r13, %rdx
	mulxq	%r10, %rcx, %rax
	setb	%dl
	addq	%rbx, %rcx
	movzbl	%dl, %edx
	adcq	%rdx, %rax
	addq	%rsi, %r11
	movq	%r8, %r13
	adcq	%r12, %r13
	movq	%r13, -56(%rsp)                 # 8-byte Spill
	adcq	$0, %rcx
	adcq	$0, %rax
	addq	%r14, %rcx
	adcq	%r9, %rax
	movq	-120(%rsp), %rsi                # 8-byte Reload
	movq	%rsi, %rdx
	mulxq	%r15, %r8, %r9
	movq	-128(%rsp), %r12                # 8-byte Reload
	movq	%r12, %rdx
	mulxq	%r15, %rbp, %rbx
	setb	%dl
	addq	%r9, %rbp
	adcq	$0, %rbx
	movzbl	%dl, %r9d
	movq	%rsi, %rdx
	mulxq	%r10, %rdx, %rsi
	addq	%rbp, %rdx
	movq	%rdx, %rbp
	adcq	%rbx, %rsi
	setb	%dl
	movzbl	%dl, %r14d
	movq	%r12, %rdx
	mulxq	%r10, %rbx, %rdx
	addq	%rsi, %rbx
	adcq	%r14, %rdx
	addq	%rcx, %r8
	movq	%r8, -120(%rsp)                 # 8-byte Spill
	adcq	%rax, %rbp
	movq	%rbp, -128(%rsp)                # 8-byte Spill
	adcq	%r9, %rbx
	movq	%rbx, -40(%rsp)                 # 8-byte Spill
	adcq	$0, %rdx
	movq	%rdx, -32(%rsp)                 # 8-byte Spill
	movabsq	$-6999018897625421111, %r8      # imm = 0x9EDE7D651ECA6AC9
	movq	%r11, %rdx
	movq	%r11, -48(%rsp)                 # 8-byte Spill
	movq	%r11, %rax
	movabsq	$-8659850874718887031, %r11     # imm = 0x87D20782E4866389
	mulxq	%r11, %r9, %rdx
	imulq	%r8, %rax
	addq	%rax, %rdx
	movq	%r13, %rcx
	imulq	%r11, %rcx
	addq	%rdx, %rcx
	movabsq	$-2832821545973065088, %rax     # imm = 0xD8AFCBD01833DA80
	movq	-104(%rsp), %rdx                # 8-byte Reload
	mulxq	%rax, %r10, %rbp
	movabsq	$-758255415442502549, %rbx      # imm = 0xF57A22B791888C6B
	imulq	%rdx, %rbx
	movq	%rdx, %rsi
	addq	%rbx, %rbp
	movq	-112(%rsp), %rdx                # 8-byte Reload
	imulq	%rdx, %rax
	addq	%rbp, %rax
	addq	%r9, %r10
	adcq	%rcx, %rax
	movq	%rdx, %rcx
	mulxq	%r11, %rbp, %rbx
	movq	%rsi, %rdx
	mulxq	%r11, %r13, %rsi
	addq	%rbp, %rsi
	adcq	$0, %rbx
	mulxq	%r8, %r14, %rbp
	addq	%rsi, %r14
	adcq	%rbx, %rbp
	movq	%rcx, %rdx
	mulxq	%r8, %rsi, %rcx
	setb	%dl
	addq	%rbp, %rsi
	movzbl	%dl, %edx
	adcq	%rdx, %rcx
	addq	%r10, %rsi
	adcq	%rax, %rcx
	movabsq	$-5165552122434856867, %rax     # imm = 0xB85045B68181585D
	movq	%rcx, %rdx
	mulxq	%rax, %r8, %rbp
	movq	%rsi, %rdx
	mulxq	%rax, %rdx, %rbx
	movq	%rdx, -64(%rsp)                 # 8-byte Spill
	addq	%r8, %rbx
	adcq	$0, %rbp
	movabsq	$3486998266802970665, %r8       # imm = 0x30644E72E131A029
	movq	%rsi, %rdx
	mulxq	%r8, %rdx, %rax
	addq	%rbx, %rdx
	movq	%rdx, -72(%rsp)                 # 8-byte Spill
	adcq	%rbp, %rax
	movq	%rcx, %rdx
	mulxq	%r8, %rbp, %r15
	setb	%dl
	addq	%rax, %rbp
	movq	%rbp, -80(%rsp)                 # 8-byte Spill
	movzbl	%dl, %eax
	adcq	%rax, %r15
	movabsq	$4332616871279656263, %r8       # imm = 0x3C208C16D87CFD47
	movq	%rcx, %rdx
	mulxq	%r8, %rax, %rbx
	movq	%rsi, %rdx
	mulxq	%r8, %r12, %rbp
	addq	%rax, %rbp
	adcq	$0, %rbx
	movabsq	$-7529619929231668595, %r10     # imm = 0x97816A916871CA8D
	movq	%rsi, %rdx
	mulxq	%r10, %rax, %rsi
	addq	%rbp, %rax
	adcq	%rbx, %rsi
	movq	%rcx, %rdx
	mulxq	%r10, %r11, %r9
	setb	%cl
	addq	%rsi, %r11
	movzbl	%cl, %ecx
	adcq	%rcx, %r9
	movq	%r14, %rdx
	mulxq	%r8, %rcx, %rbp
	movq	%r13, %rdx
	mulxq	%r8, %rbx, %rsi
	movq	%rbx, -88(%rsp)                 # 8-byte Spill
	addq	%rcx, %rsi
	adcq	$0, %rbp
	mulxq	%r10, %r8, %rcx
	addq	%rsi, %r8
	adcq	%rbp, %rcx
	movq	%r14, %rdx
	movq	%r14, -96(%rsp)                 # 8-byte Spill
	mulxq	%r10, %r14, %r10
	setb	%bl
	addq	%rcx, %r14
	movzbl	%bl, %ecx
	adcq	%rcx, %r10
	addq	%r12, %r14
	adcq	%rax, %r10
	adcq	$0, %r11
	adcq	$0, %r9
	movabsq	$-5165552122434856867, %rsi     # imm = 0xB85045B68181585D
	mulxq	%rsi, %rax, %rcx
	movq	%r13, %rdx
	mulxq	%rsi, %r12, %rsi
	addq	%rax, %rsi
	adcq	$0, %rcx
	movabsq	$3486998266802970665, %rbx      # imm = 0x30644E72E131A029
	mulxq	%rbx, %rax, %rbp
	addq	%rsi, %rax
	adcq	%rcx, %rbp
	movq	-96(%rsp), %rdx                 # 8-byte Reload
	mulxq	%rbx, %rcx, %rdx
	movq	%rbx, %r13
	setb	%bl
	addq	%rbp, %rcx
	movzbl	%bl, %esi
	adcq	%rsi, %rdx
	addq	%r14, %r12
	adcq	%r10, %rax
	adcq	$0, %rcx
	adcq	$0, %rdx
	addq	%r11, %rcx
	adcq	%r9, %rdx
	setb	%bl
	addq	-64(%rsp), %rcx                 # 8-byte Folded Reload
	adcq	-72(%rsp), %rdx                 # 8-byte Folded Reload
	movzbl	%bl, %ebx
	adcq	-80(%rsp), %rbx                 # 8-byte Folded Reload
	adcq	$0, %r15
	movq	-88(%rsp), %rsi                 # 8-byte Reload
	addq	-104(%rsp), %rsi                # 8-byte Folded Reload
	adcq	-112(%rsp), %r8                 # 8-byte Folded Reload
	adcq	-48(%rsp), %r12                 # 8-byte Folded Reload
	adcq	-56(%rsp), %rax                 # 8-byte Folded Reload
	adcq	-120(%rsp), %rcx                # 8-byte Folded Reload
	adcq	-128(%rsp), %rdx                # 8-byte Folded Reload
	adcq	-40(%rsp), %rbx                 # 8-byte Folded Reload
	adcq	-32(%rsp), %r15                 # 8-byte Folded Reload
	setb	%sil
	movq	%rcx, -24(%rsp)
	movabsq	$4332616871279656263, %rax      # imm = 0x3C208C16D87CFD47
	subq	%rax, %rcx
	movq	%rdx, -16(%rsp)
	movabsq	$-7529619929231668595, %rax     # imm = 0x97816A916871CA8D
	sbbq	%rax, %rdx
	movq	%rbx, -8(%rsp)
	movabsq	$-5165552122434856867, %rax     # imm = 0xB85045B68181585D
	sbbq	%rax, %rbx
	movq	%r15, (%rsp)
	sbbq	%r13, %r15
	movq	%rdi, %rax
	movq	%rcx, 16(%rsp)
	movq	%rdx, 24(%rsp)
	movq	%rbx, 32(%rsp)
	movzbl	%sil, %ecx
	sbbq	$0, %rcx
	leaq	16(%rsp), %rcx
	movq	%r15, 40(%rsp)
	leaq	-24(%rsp), %rdx
	cmovaeq	%rcx, %rdx
	movups	(%rdx), %xmm0
	movups	16(%rdx), %xmm1
	movups	%xmm1, 16(%rdi)
	movups	%xmm0, (%rdi)
	addq	$48, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	gfp_mul, .Lfunc_end0-gfp_mul
	.cfi_endproc
                                        # -- End function
	.ident	"Ubuntu clang version 13.0.1-++20220120110924+75e33f71c2da-1~exp1~20220120231001.58"
	.section	".note.GNU-stack","",@progbits
