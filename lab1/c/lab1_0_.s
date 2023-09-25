	.file	"main.c"
	.text
	.globl	sin_x
	.type	sin_x, @function
sin_x:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movsd	%xmm0, -40(%rbp)
	movq	%rdi, -48(%rbp)
	movsd	-40(%rbp), %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	movq	$1, -8(%rbp)
	jmp	.L2
.L3:
	movsd	-16(%rbp), %xmm0
	addsd	-24(%rbp), %xmm0
	movsd	%xmm0, -16(%rbp)
	movsd	-40(%rbp), %xmm0
	movq	.LC1(%rip), %xmm1
	xorpd	%xmm1, %xmm0
	mulsd	-40(%rbp), %xmm0
	movq	-8(%rbp), %rax
	addq	%rax, %rax
	addq	$1, %rax
	imulq	-8(%rbp), %rax
	addq	%rax, %rax
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm1, %xmm0
	movsd	-24(%rbp), %xmm1
	mulsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	addq	$1, -8(%rbp)
.L2:
	movq	-8(%rbp), %rax
	cmpq	-48(%rbp), %rax
	jl	.L3
	movsd	-16(%rbp), %xmm0
	movq	%xmm0, %rax
	movq	%rax, %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	sin_x, .-sin_x
	.section	.rodata
.LC2:
	.string	"Time taken: %lf sec.\n"
.LC3:
	.string	"rez %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$144, %rsp
	movl	%edi, -132(%rbp)
	movq	%rsi, -144(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-144(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoll@PLT
	movq	%rax, -120(%rbp)
	movq	-144(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atof@PLT
	movq	%xmm0, %rax
	movq	%rax, -112(%rbp)
	movl	$2, %edi
	call	sysconf@PLT
	movq	%rax, -104(%rbp)
	leaq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	times@PLT
	movq	-120(%rbp), %rdx
	movq	-112(%rbp), %rax
	movq	%rdx, %rdi
	movq	%rax, %xmm0
	call	sin_x
	movq	%xmm0, %rax
	movq	%rax, -96(%rbp)
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	times@PLT
	movq	-48(%rbp), %rax
	movq	-80(%rbp), %rdx
	subq	%rdx, %rax
	movq	%rax, -88(%rbp)
	pxor	%xmm0, %xmm0
	cvtsi2sdq	-88(%rbp), %xmm0
	pxor	%xmm1, %xmm1
	cvtsi2sdq	-104(%rbp), %xmm1
	divsd	%xmm1, %xmm0
	movq	%xmm0, %rax
	movq	%rax, %xmm0
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	stdout(%rip), %rax
	movq	-96(%rbp), %rdx
	movq	%rdx, %xmm0
	leaq	.LC3(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$1, %eax
	call	fprintf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L7
	call	__stack_chk_fail@PLT
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.section	.rodata
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
