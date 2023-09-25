sin_x:
        testq   %rdi, %rdi
;Команда test выполняет побитовое И над операндами, как и команда and, но, в отличие от неё, операнды не изменяет, а
;только устанавливает флаги. Её также называют командой логического сравнения, потому что с её помощью удобно
;проверять, установлены ли определённые биты.
        jle     .L4
        movapd  %xmm0, %xmm4
        addq    $1, %rdi
        movl    $3, %ecx
        movl    $1, %eax
        xorpd   .LC1(%rip), %xmm4
        pxor    %xmm1, %xmm1
        mulsd   %xmm0, %xmm4
.L3:
        pxor    %xmm3, %xmm3
        movapd  %xmm4, %xmm2
        addsd   %xmm0, %xmm1
        movq    %rcx, %rdx
        imulq   %rax, %rdx
        addq    $1, %rax
        addq    $2, %rcx
        addq    %rdx, %rdx
        cvtsi2sdq       %rdx, %xmm3
        divsd   %xmm3, %xmm2
        mulsd   %xmm2, %xmm0
        cmpq    %rdi, %rax
        jne     .L3
        movapd  %xmm1, %xmm0
        ret
.L4:
        pxor    %xmm1, %xmm1
        movapd  %xmm1, %xmm0
        ret
.LC2:
        .string "Time taken: %lf sec.\n"
.LC3:
        .string "rez %lf\n"
main:
        pushq   %rbp
        movl    $10, %edx
        pushq   %rbx
        movq    %rsi, %rbx
        subq    $88, %rsp
        movq    8(%rsi), %rdi
        xorl    %esi, %esi
        call    strtoll
        movq    16(%rbx), %rdi
        xorl    %esi, %esi
        movq    %rax, %rbp
        call    strtod
        movl    $2, %edi
        movsd   %xmm0, 8(%rsp)
        call    sysconf
        leaq    16(%rsp), %rdi
        movq    %rax, %rbx
        call    times
        testq   %rbp, %rbp
        jle     .L10
        movsd   8(%rsp), %xmm0
        leaq    1(%rbp), %rax
        movl    $3, %esi
        movl    $1, %edx
        pxor    %xmm1, %xmm1
        movapd  %xmm0, %xmm4
        xorpd   .LC1(%rip), %xmm4
        mulsd   %xmm0, %xmm4
.L9:
        pxor    %xmm3, %xmm3
        movapd  %xmm4, %xmm2
        addsd   %xmm0, %xmm1
        movq    %rsi, %rcx
        imulq   %rdx, %rcx
        addq    $1, %rdx
        addq    $2, %rsi
        addq    %rcx, %rcx
        cvtsi2sdq       %rcx, %xmm3
        divsd   %xmm3, %xmm2
        mulsd   %xmm2, %xmm0
        cmpq    %rax, %rdx
        jne     .L9
.L8:
        leaq    48(%rsp), %rdi
        movsd   %xmm1, 8(%rsp)
        call    times
        movq    48(%rsp), %rax
        pxor    %xmm0, %xmm0
        subq    16(%rsp), %rax
        pxor    %xmm2, %xmm2
        cvtsi2sdq       %rax, %xmm0
        movl    $.LC2, %edi
        movl    $1, %eax
        cvtsi2sdq       %rbx, %xmm2
        divsd   %xmm2, %xmm0
        call    printf
        movsd   8(%rsp), %xmm1
        movl    $.LC3, %esi
        movq    stdout(%rip), %rdi
        movl    $1, %eax
        movapd  %xmm1, %xmm0
        call    fprintf
        addq    $88, %rsp
        xorl    %eax, %eax
        popq    %rbx
        popq    %rbp
        ret
.L10:
        pxor    %xmm1, %xmm1
        jmp     .L8
.LC1:
        .long   0
        .long   -2147483648
        .long   0
        .long   0
