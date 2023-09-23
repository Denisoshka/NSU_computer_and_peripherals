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
;	Это организация кадра стека: pushq %rbp сохраняет текущий указатель кадра стека, movq %rsp, %rbp устанавливает новый.
;   Аннотации начинающиеся с .cfi_ управляют отладочной информацией, которой пользуются дебагер и механизм раскручивания стека при исключениях:
;   .cfi_startproc задаёт начало процедуры и устанавливает начальный регистр и смещение для расчёта адреса CFA (Canonical Frame Address).
;   .cfi_def_cfa_offset 16 обновляет смещение CFA, говоря, что оно стало равно 16 относительно заданного (директивой .cfi_startproc) регистра (%rsp).
;   .cfi_offset 6, -16 говорит, что теперь регистр 6 лежит по смещению -16 от CFA (таким образом была описана инструкция pushq %rbp).
;   .cfi_def_cfa_register 6 говорит, что теперь для расчёта адреса CFA используется регистр 6.
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movsd	%xmm0, -56(%rbp)
	movq	%rdi, -64(%rbp)
	fldl	-56(%rbp)
	fstpt	-32(%rbp)
	fldz
	fstpt	-16(%rbp)
	movq	$1, -40(%rbp)
	jmp	.L2
.L3:
	fldt	-16(%rbp)
	fldt	-32(%rbp)
	faddp	%st, %st(1)
;	Инструкция faddp используется для выполнения операции сложения (addition) над значениями, хранящимися в регистрах FPU (Floating-Point Unit), и затем удаления верхнего элемента стека FPU.
;    В данной инструкции:
;    %st - это указатель на текущий верхний элемент стека FPU. %st(0) представляет верхний элемент, %st(1) - элемент, который находится ниже него, и так далее.
;    %st, %st(1) - эта инструкция выполняет сложение верхнего элемента стека FPU (%st) с элементом, находящимся ниже него (%st(1)).
;    После выполнения операции сложения, верхний элемент стека FPU содержит результат сложения, и вершина стека сдвигается на одну позицию вниз, так что более низкий элемент становится новым верхним элементом.
;    Инструкция faddp полезна при выполнении последовательных операций с плавающей запятой в стеке FPU, так как она выполняет операцию и одновременно удаляет верхний элемент, что позволяет сэкономить место в стеке.
	fstpt	-16(%rbp)
	movsd	-56(%rbp), %xmm0
	movq	.LC1(%rip), %xmm1
	xorpd	%xmm1, %xmm0
	mulsd	-56(%rbp), %xmm0
	movq	-40(%rbp), %rax
	addq	%rax, %rax
	addq	$1, %rax
	imulq	-40(%rbp), %rax
	addq	%rax, %rax
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -72(%rbp)
	fldl	-72(%rbp)
	fldt	-32(%rbp)
	fmulp	%st, %st(1)
	fstpt	-32(%rbp)
	addq	$1, -40(%rbp)
.L2:
	movq	-40(%rbp), %rax
	cmpq	-64(%rbp), %rax
	jl	.L3
	fldt	-16(%rbp)
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	sin_x, .-sin_x
	.section	.rodata
.LC3:
	.string	"Time taken: %lf sec.\n"
.LC4:
	.string	"rez %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB7:
	.cfi_startproc
	endbr64
;	Эта инструкция служит для защиты от атаки "Return-Oriented Programming" (ROP) и указывает,
;   что следующая инструкция должна быть корректной инструкцией x86-64. Это мера безопасности.
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$144, %rsp
	movl	%edi, -132(%rbp)
	movq	%rsi, -144(%rbp)
	movq	%fs:40, %rax
;	%fs - это сегментный регистр, который указывает на сегмент данных (Data Segment) сегмента
;   системного уровня, известного как FS (или GS). В архитектуре x86-64, сегментные регистры
;   %fs и %gs могут быть использованы для доступа к специфическим данным, например, для доступа
;   к потоковой локальной памяти (Thread-Local Storage, TLS).
;   40 - это смещение (offset) внутри сегмента %fs, откуда будет загружено значение.
;%fs: F segment — дополнительный сегментный регистр без специального назначения.
;%gs: G segment — дополнительный сегментный регистр без специального назначения.
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
	fstpl	-96(%rbp)
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
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf@PLT
	movq	stdout(%rip), %rax
	movq	-96(%rbp), %rdx
	movq	%rdx, %xmm0
	leaq	.LC4(%rip), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$1, %eax
	call	fprintf@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
;	movq -8(%rbp), %rdx: Загрузка значения из памяти по адресу -8(%rbp) в регистр %rdx. Это,
;   вероятно, используется для проверки стековой защиты.
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
