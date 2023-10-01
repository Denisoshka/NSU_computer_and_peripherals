sin_x:
        testq   %rdi, %rdi
;сравниваем n==0, если да то перепрыгиваем в конец
;Команда test выполняет побитовое И над операндами, как и команда and, но, в отличие от неё, операнды не изменяет, а
;только устанавливает флаги. Её также называют командой логического сравнения, потому что с её помощью удобно
;проверять, установлены ли определённые биты.
        jle     .L4
        movapd  %xmm0, %xmm4
;double start_sum = x;
        addq    $1, %rdi
;long long int i = 1
        movl    $3, %ecx
        movl    $1, %eax
        xorpd   .LC1(%rip), %xmm4
;исключающее или c числом по адресу .LC1(%rip)
;гдк rip адрес текущей инструкции
        pxor    %xmm1, %xmm1
;sum = 0.0
        mulsd   %xmm0, %xmm4
;x * x
.L3:
        pxor    %xmm3, %xmm3
        movapd  %xmm4, %xmm2
        addsd   %xmm0, %xmm1
;sum += start_sum;
        movq    %rcx, %rdx
;в rdx находится 3
        imulq   %rax, %rdx
;3*1 и записать в rdx
;начинаем вычислять ((2 * i) * (2 * i + 1))
        addq    $1, %rax
        addq    $2, %rcx
        addq    %rdx, %rdx
        cvtsi2sdq       %rdx, %xmm3
;((2 * i) * (2 * i + 1)) в double
        divsd   %xmm3, %xmm2
        mulsd   %xmm2, %xmm0
        cmpq    %rdi, %rax
;i <= n
;где rdi = n, rax = i,
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
;резервируем 88 байтов
        movq    8(%rsi), %rdi
;записать в rdi адрес значения который находится в регистве rsi смещенный на 8 байт
;argv[1]
        xorl    %esi, %esi
        call    strtoll
        movq    16(%rbx), %rdi
;записать в rdi адрес значения который находится в регистве rsi смещенный на 8 байт
;argv[2]
        xorl    %esi, %esi
        movq    %rax, %rbp
;в rbp лежит n
;вызов atof использую %esi и %rdi как аргументы
        call    strtod
        movl    $2, %edi
        movsd   %xmm0, 8(%rsp)
;вызов sysconf используя %edi как аргумент(_SC_CLK_TCK)
        call    sysconf
        leaq    16(%rsp), %rdi
;&start
        movq    %rax, %rbx
        call    times
        testq   %rbp, %rbp
;n==0
        jle     .L10
        movsd   8(%rsp), %xmm0
;кладем в xmm0 находится x
        leaq    1(%rbp), %rax
        movl    $3, %esi
        movl    $1, %edx
        pxor    %xmm1, %xmm1
        movapd  %xmm0, %xmm4
        xorpd   .LC1(%rip), %xmm4
        mulsd   %xmm0, %xmm4
.L9:
;вычисляем start_sum
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
;адрес на start (&end)
        movsd   %xmm1, 8(%rsp)
; сохраняем x по адресу 8 + rsp
        call    times
        movq    48(%rsp), %rax
        pxor    %xmm0, %xmm0
        subq    16(%rsp), %rax
        pxor    %xmm2, %xmm2
        cvtsi2sdq       %rax, %xmm0
        movl    $.LC2, %edi
;строка "Time taken: %lf sec.\n" для printf
        movl    $1, %eax
        cvtsi2sdq       %rbx, %xmm2
        divsd   %xmm2, %xmm0
;clocks / clocks_per_sec
        call    printf
        movsd   8(%rsp), %xmm1
;в xmm1 rez
        movl    $.LC3, %esi
        movq    stdout(%rip), %rdi
        movl    $1, %eax
        movapd  %xmm1, %xmm0
        call    fprintf
;восстанавливаем стековый фрейм
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
