	.file	"main.c"
	.text
	.p2align 4
	.globl	sin_x
	.type	sin_x, @function
sin_x:
.LFB51:
	.cfi_startproc
	endbr64
	cmpq	$1, %rdi
	jle	.L4
	movapd	%xmm0, %xmm4
	xorpd	.LC1(%rip), %xmm4
	movl	$3, %ecx
	movl	$1, %eax
	pxor	%xmm1, %xmm1
	mulsd	%xmm0, %xmm4
	.p2align 4,,10
	.p2align 3
.L3:
	pxor	%xmm3, %xmm3
	movapd	%xmm4, %xmm2
	addsd	%xmm0, %xmm1
	movq	%rcx, %rdx
	imulq	%rax, %rdx
	addq	$1, %rax
	addq	$2, %rcx
	addq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm3
	divsd	%xmm3, %xmm2
	mulsd	%xmm2, %xmm0
	cmpq	%rax, %rdi
	jne	.L3
	movapd	%xmm1, %xmm0
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	pxor	%xmm1, %xmm1
	movapd	%xmm1, %xmm0
	ret
	.cfi_endproc
.LFE51:
	.size	sin_x, .-sin_x
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"Time taken: %lf sec.\n"
.LC3:
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
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rsi, %rbx
	subq	$104, %rsp
	.cfi_def_cfa_offset 128
	movq	8(%rsi), %rdi
	xorl	%esi, %esi
	movq	%fs:40, %rax
	movq	%rax, 88(%rsp)
	xorl	%eax, %eax
	call	strtoll@PLT
	movq	16(%rbx), %rdi
	xorl	%esi, %esi
	movq	%rax, %rbp
	call	strtod@PLT
	movl	$2, %edi
	movsd	%xmm0, 8(%rsp)
	call	sysconf@PLT
	leaq	16(%rsp), %rdi
	movq	%rax, %rbx
	call	times@PLT
	cmpq	$1, %rbp
	jle	.L11
	movsd	8(%rsp), %xmm0
	movl	$3, %ecx
	movl	$1, %eax
	pxor	%xmm1, %xmm1
	movapd	%xmm0, %xmm4
	xorpd	.LC1(%rip), %xmm4
	mulsd	%xmm0, %xmm4
	.p2align 4,,10
	.p2align 3
.L9:
	pxor	%xmm3, %xmm3
	movapd	%xmm4, %xmm2
	addsd	%xmm0, %xmm1
	movq	%rcx, %rdx
	imulq	%rax, %rdx
	addq	$1, %rax
	addq	$2, %rcx
	addq	%rdx, %rdx
	cvtsi2sdq	%rdx, %xmm3
	divsd	%xmm3, %xmm2
	mulsd	%xmm2, %xmm0
	cmpq	%rax, %rbp
	jne	.L9
.L8:
	leaq	48(%rsp), %rdi
	movsd	%xmm1, 8(%rsp)
	call	times@PLT
	movq	48(%rsp), %rax
	pxor	%xmm0, %xmm0
	subq	16(%rsp), %rax
	pxor	%xmm2, %xmm2
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %edi
	movl	$1, %eax
	cvtsi2sdq	%rbx, %xmm2
	leaq	.LC2(%rip), %rsi
	divsd	%xmm2, %xmm0
	call	__printf_chk@PLT
	movsd	8(%rsp), %xmm1
	movl	$1, %esi
	movq	stdout(%rip), %rdi
	leaq	.LC3(%rip), %rdx
	movl	$1, %eax
	movapd	%xmm1, %xmm0
	call	__fprintf_chk@PLT
	movq	88(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L14
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L11:
	.cfi_restore_state
	pxor	%xmm1, %xmm1
	jmp	.L8
.L14:
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
