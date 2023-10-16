sin_x(double, long long):
        testq   %rdi, %rdi
        ; проверяем n == 0
        jle     .L4
        movapd  %xmm0, %xmm4
        ; в xmm4 x
        ;  double start_sum = x;
        addq    $1, %rdi
        movl    $3, %ecx
        movl    $1, %eax
        xorpd   .LC1(%rip), %xmm4
        ;-1.0 * x
        pxor    %xmm1, %xmm1
        ;double sum = 0.0;
        ; в xmm1 0
        mulsd   %xmm0, %xmm4
        ;-1.0 * x * x
.L3:
        pxor    %xmm3, %xmm3
        movapd  %xmm4, %xmm2
        ;%xmm2 -1.0 * x * x
        addsd   %xmm0, %xmm1
        ;sum += start_sum;
        movq    %rcx, %rdx
        ; %rdx = (2 * i + 1)
        imulq   %rax, %rdx
        ; %rdx = (i) * (2 * i + 1))
        addq    $1, %rax
        addq    $2, %rcx
        ; (2 * i + 1) здесь $2, %rcx это аналог (2 * i + 1),
        ; с каждой итерацией i * 2, поэтому +2
        addq    %rdx, %rdx
        ; (2 * i) * (2 * i + 1))
        cvtsi2sdq       %rdx, %xmm3
        ;((2 * i) * (2 * i + 1)) в double
        divsd   %xmm3, %xmm2
        ;-1.0 * x * x / ((2 * i) * (2 * i + 1));
        mulsd   %xmm2, %xmm0
        ;start_sum *= -1.0 * x * x / ((2 * i) * (2 * i + 1));
        cmpq    %rdi, %rax
        ;сравниваем счетчик
        jne     .L3
        movapd  %xmm1, %xmm0
        ;sum = ...
        ret
.L4:
        pxor    %xmm1, %xmm1
        movapd  %xmm1, %xmm0
        ; double sum = 0.0;
        ; ret 0
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
        ; сохраняем char** argv
        subq    $88, %rsp
        movq    8(%rsi), %rdi
        ;адресс на argv[1] в rdi
        xorl    %esi, %esi
        call    strtoll
        movq    16(%rbx), %rdi
        ;double x = atof(argv[2]);
        xorl    %esi, %esi
        movq    %rax, %rbp
        ;   long long int n = atoll(argv[1]);
        ; в rbp n
        call    strtod
        movl    $2, %edi
        ; системный вызов _SC_CLK_TCK
        movsd   %xmm0, 8(%rsp)
        ; double x = atof(argv[2]);
        call    sysconf
        ; sysconf(_SC_CLK_TCK);
        leaq    16(%rsp), %rdi
        ;&start
        movq    %rax, %rbx
        call    times
        ; inline sinx
        ; n == 0?
        testq   %rbp, %rbp
        jle     .L10
        movsd   8(%rsp), %xmm0
        ;double start_sum = x;
        leaq    1(%rbp), %rax
        ;long long int i = 1
        movl    $3, %esi
        ;(2 * i + 1)
        movl    $1, %edx
        pxor    %xmm1, %xmm1
        ;double sum = 0.0;
        movapd  %xmm0, %xmm4
        ; %xmm4 = x
        xorpd   .LC1(%rip), %xmm4
        ;-1.0 * x
        mulsd   %xmm0, %xmm4
        ;-1.0 * x * x
.L9:
        pxor    %xmm3, %xmm3
        movapd  %xmm4, %xmm2
        ; xmm2 = -1*x*x
        addsd   %xmm0, %xmm1
        ;sum += start_sum;
        movq    %rsi, %rcx
        imulq   %rdx, %rcx
        ;(i) * (2 * i + 1)
        addq    $1, %rdx
        addq    $2, %rsi
        ;(2 * (i+1) + 1) для след итерации
        addq    %rcx, %rcx
        ;(2*i) * (2 * i + 1)
        cvtsi2sdq       %rcx, %xmm3
        ;(2*i) * (2 * i + 1) в double
        divsd   %xmm3, %xmm2
        ;-1.0 * x * x / ((2 * i) * (2 * i + 1))
        mulsd   %xmm2, %xmm0
        ;start_sum *= -1.0 * x * x / ((2 * i) * (2 * i + 1));
        cmpq    %rax, %rdx
        ;i <= n;
        jne     .L9
.L8:
        leaq    48(%rsp), %rdi
        ;&end
        movsd   %xmm1, 8(%rsp)
        ; в 8(%rsp) sum
        call    times
        movq    48(%rsp), %rax
        pxor    %xmm0, %xmm0
        subq    16(%rsp), %rax
        ;  clocks = end.tms_utime - start.tms_utime;
        pxor    %xmm2, %xmm2
        cvtsi2sdq       %rax, %xmm0
        ;(double) clocks
        movl    $.LC2, %edi
        ;"Time taken: %lf sec.\n"
        movl    $1, %eax
        ;колво парам double
        cvtsi2sdq       %rbx, %xmm2
        ;clocks_per_sec в double
        divsd   %xmm2, %xmm0
        ;(double) clocks / clocks_per_sec
        call    printf
        movsd   8(%rsp), %xmm1
        movl    $.LC3, %esi
        ;"rez %lf\n"
        movq    stdout(%rip), %rdi
        movl    $1, %eax
        movapd  %xmm1, %xmm0
        call    fprintf
        ; восстанавливаем стек
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