	.file	"main.c"
	.text
	.p2align 4
	.globl	sin_x
	.type	sin_x, @function
sin_x:
.LFB51:
	.cfi_startproc
	endbr64
	movsd	%xmm0, -16(%rsp)
	fldl	-16(%rsp)
	movapd	%xmm0, %xmm1
	cmpq	$1, %rdi
	jle	.L4
	xorpd	.LC1(%rip), %xmm0
	fldz
	movl	$3, %ecx
	movl	$1, %eax
	mulsd	%xmm1, %xmm0
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L7:
	fxch	%st(1)
.L3:
	pxor	%xmm1, %xmm1
	fadd	%st(1), %st
	fxch	%st(1)
	movapd	%xmm0, %xmm2
	movq	%rcx, %rdx
	imulq	%rax, %rdx
	addq	$1, %rax
	addq	$2, %rcx
	addq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm1
	divsd	%xmm1, %xmm2
	movsd	%xmm2, -16(%rsp)
	fmull	-16(%rsp)
	cmpq	%rax, %rdi
	jne	.L7
	fstp	%st(0)
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	fstp	%st(0)
	fldz
	ret
	.cfi_endproc
.LFE51:
	.size	sin_x, .-sin_x
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"Time taken: %lf sec.\n"
.LC4:
	.string	"rez %lf\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB52:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$10, %edx
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$104, %rsp
	.cfi_def_cfa_offset 128
	movq	8(%rsi), %rdi
	xorl	%esi, %esi
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	call	strtoll@PLT
	movq	16(%rbp), %rdi
	xorl	%esi, %esi
	movq	%rax, %rbx
	call	strtod@PLT
	movl	$2, %edi
	movsd	%xmm0, (%rsp)
	call	sysconf@PLT
	leaq	16(%rsp), %rdi
	movq	%rax, %rbp
	call	times@PLT
	fldl	(%rsp)
	cmpq	$1, %rbx
	jle	.L12
	movsd	(%rsp), %xmm3
	movl	$3, %ecx
	fldz
	movl	$1, %eax
	movapd	%xmm3, %xmm1
	xorpd	.LC1(%rip), %xmm1
	mulsd	%xmm3, %xmm1
	jmp	.L10
	.p2align 4,,10
	.p2align 3
.L16:
	fxch	%st(1)
.L10:
	pxor	%xmm0, %xmm0
	fadd	%st(1), %st
	fxch	%st(1)
	movapd	%xmm1, %xmm2
	movq	%rcx, %rdx
	imulq	%rax, %rdx
	addq	$1, %rax
	addq	$2, %rcx
	addq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm0
	divsd	%xmm0, %xmm2
	movsd	%xmm2, (%rsp)
	fmull	(%rsp)
	cmpq	%rax, %rbx
	jne	.L16
	fstp	%st(0)
.L9:
	leaq	48(%rsp), %rdi
	fstpt	(%rsp)
	call	times@PLT
	movq	48(%rsp), %rax
	pxor	%xmm0, %xmm0
	subq	16(%rsp), %rax
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %edi
	movl	$1, %eax
	cvtsi2sdq	%rbp, %xmm1
	leaq	.LC3(%rip), %rsi
	divsd	%xmm1, %xmm0
	call	__printf_chk@PLT
	fldt	(%rsp)
	movl	$1, %esi
	movq	stdout(%rip), %rdi
	leaq	.LC4(%rip), %rdx
	movl	$1, %eax
	fstpl	(%rsp)
	movsd	(%rsp), %xmm0
	call	__fprintf_chk@PLT
	movq	88(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L15
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L12:
	.cfi_restore_state
	fstp	%st(0)
	fldz
	jmp	.L9
.L15:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE52:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	0
	.long	-2147483648
	.long	0
	.long	0
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
