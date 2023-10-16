sin_x(double, long long):
        pushq   %rbp
        movq    %rsp, %rbp
        ; формируем новый стековый кадр
        movsd   %xmm0, -40(%rbp)
        movq    %rdi, -48(%rbp)
        ; созраняем x и n на стеке
        movsd   -40(%rbp), %xmm0
        movsd   %xmm0, -8(%rbp)
        ;  double start_sum = x;
        ; в -8(%rbp) x
        pxor    %xmm0, %xmm0
        ; зануляем x
        movsd   %xmm0, -16(%rbp)
        ;  double sum = 0.0;
        movq    $1, -24(%rbp)
        ;long long int i = 1
        jmp     .L2
.L3:
        movsd   -16(%rbp), %xmm0
        ;в xmmo sum
        addsd   -8(%rbp), %xmm0
        ;sum += start_sum;
        movsd   %xmm0, -16(%rbp)
        ;сохраняем результат вычислений на стеке
        movsd   -40(%rbp), %xmm0
        ;в xnm0 x
        movq    .LC1(%rip), %xmm1
        ; в xmm1 double число которое наход по .LC1
        ; в xmm1 -1.0
        xorpd   %xmm1, %xmm0
        ; умножаем на -1
        ; -1.0 * x
        mulsd   -40(%rbp), %xmm0
        ; -1.0 * x * x
        ; %xmm0 = -x * x
        movq    -24(%rbp), %rax
        ; в rax 1
        addq    %rax, %rax
        ; в rax 2
        addq    $1, %rax
        ; в rax 3
        imulq   -24(%rbp), %rax
        ; в rax 3
        addq    %rax, %rax
        ; в rax 6
        ; ((2 * i) * (2 * i + 1))
        ; вообщем он сначала делает левую скобку, потом умножает на i, потом умножает на 2
        pxor    %xmm1, %xmm1
        ; в xmm1 0
        cvtsi2sdq       %rax, %xmm1
        ; перевод ((2 * i) * (2 * i + 1)) в double и запись в xmm1
        divsd   %xmm1, %xmm0
        ; (-1.0 * x * x) / (((2 * i) * (2 * i + 1)))
        movsd   -8(%rbp), %xmm1
        mulsd   %xmm1, %xmm0
        ;start_sum *= ...
        movsd   %xmm0, -8(%rbp)
        ; в -8(%rbp) start_sum
        addq    $1, -24(%rbp)
        ; i++
.L2:
        movq    -24(%rbp), %rax
        ;i <= n
        cmpq    -48(%rbp), %rax
        jle     .L3
        ; повторяем действие
        movsd   -16(%rbp), %xmm0
        movq    %xmm0, %rax
        movq    %rax, %xmm0
        popq    %rbp
        ret
.LC2:
        .string "Time taken: %lf sec.\n"
.LC3:
        .string "rez %lf\n"
main:
        pushq   %rbp
        movq    %rsp, %rbp
        ; формируем новый стековый кадр
        addq    $-128, %rsp
        ; выделяем место под локальные переменные
        movl    %edi, -116(%rbp)
        ;в -116(%rbp) argc
        movq    %rsi, -128(%rbp)
        ;в 128(%rbp) char** argv
        movq    -128(%rbp), %rax
        addq    $8, %rax
        ;выбираем argv[1]
        movq    (%rax), %rax
        movq    %rax, %rdi
        ;даем адресс для atoll
        call    atoll
        movq    %rax, -8(%rbp)
        ;long long int n = atoll(argv[1]);
        movq    -128(%rbp), %rax
        addq    $16, %rax
        movq    (%rax), %rax
        movq    %rax, %rdi
        call    atof
        movq    %xmm0, %rax
        movq    %rax, -16(%rbp)
        ;аналогично для double x = atof(argv[2]);
        movl    $2, %edi
        ; передаем _SC_CLK_TCK в sysconf
        call    sysconf
        movq    %rax, -24(%rbp)
        ; long clocks_per_sec = sysconf(_SC_CLK_TCK);
        leaq    -80(%rbp), %rax
        movq    %rax, %rdi
        ;в rli &start
        call    times
        ;  times(&start);
        movq    -8(%rbp), %rdx
        movq    -16(%rbp), %rax
        movq    %rdx, %rdi
        movq    %rax, %xmm0
        ; передаем x и n для вызова sin_x(x, n)
        call    sin_x(double, long long)
        movq    %xmm0, %rax
        ;double rez = sin_x(x, n);
        movq    %rax, -32(%rbp)
        ; сохраняем rez на стеке
        leaq    -112(%rbp), %rax
        movq    %rax, %rdi
        ; в rdi &end
        call    times
        movq    -112(%rbp), %rdx
        movq    -80(%rbp), %rax
        subq    %rax, %rdx
        ; clocks = end.tms_utime - start.tms_utime;
        movq    %rdx, -40(%rbp)
        ; сохраняем clocks -40(%rbp)
        pxor    %xmm0, %xmm0
        cvtsi2sdq       -40(%rbp), %xmm0
        ; (double) clocks
        pxor    %xmm1, %xmm1
        cvtsi2sdq       -24(%rbp), %xmm1
        ;clocks_per_sec в double
        divsd   %xmm1, %xmm0
        ; в xmm0 (double) clocks / clocks_per_sec
        movq    %xmm0, %rax
        movq    %rax, %xmm0
        movl    $.LC2, %edi
        ; передаем адресс "Time taken: %lf sec.\n"
        movl    $1, %eax
        ; указываем колво double в вызове print
        call    printf
        movq    stdout(%rip), %rax
        movq    -32(%rbp), %rdx
        movq    %rdx, %xmm0
        movl    $.LC3, %esi
        movq    %rax, %rdi
        movl    $1, %eax
        ; аналогично для fprintf
        call    fprintf
        movl    $0, %eax
        ;указываем код возврата
        leave
        ; восстанавливаем стек
        ret
.LC1:
        .long   0
        .long   -2147483648
        .long   0
        .long   0