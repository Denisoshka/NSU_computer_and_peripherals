sin_x:
        pushq   %rbp
;Сохранение текущего значения регистра %rbp на стеке. Это обычно делается в начале функции для создания стекового фрейма.
        movq    %rsp, %rbp
;Установка регистра %rbp равным текущему значению стекового указателя %rsp. Это устанавливает %rbp как базовый указатель
;для работы со стековым фреймом функции.
;%esp: Stack pointer register — указатель стека. Содержит адрес вершины стека.
;%ebp: Base pointer register — указатель базы кадра стека (англ. stack frame). Предназначен для организации произвольного
;доступа к данным внутри стека.
        movsd   %xmm0, -40(%rbp)
;Копирование значения из регистра xmm0 (регистр для работы с числами с плавающей точкой) в память по адресу -40(%rbp).
        movq    %rdi, -48(%rbp)
;Копирование значения из регистра %rdi (первый аргумент функции) в память по адресу -48(%rbp).
        movsd   -40(%rbp), %xmm0
        movsd   %xmm0, -8(%rbp)
;Копирование значения из регистра xmm0 в память по адресу -8(%rbp).
        pxor    %xmm0, %xmm0
;Инструкция pxor выполняет побитовое исключающее ИЛИ для регистров xmm0 и xmm0, что приводит к обнулению xmm0.
        movsd   %xmm0, -16(%rbp)
        movq    $1, -24(%rbp)
;Загрузка значения 1 в память по адресу -24(%rbp).
        jmp     .L2
;Безусловный переход к метке .L2. Этот переход используется для начала выполнения кода внутри цикла или условия.
.L3:
        movsd   -16(%rbp), %xmm0
;Загрузка значения из памяти по адресу -16(%rbp) в регистр xmm0.
        addsd   -8(%rbp), %xmm0
;Выполнение операции сложения между значением в регистре xmm0 и значением в памяти по адресу -8(%rbp),
;результат сохраняется в xmm0.
        movsd   %xmm0, -16(%rbp)
        movsd   -40(%rbp), %xmm0
        movq    .LC1(%rip), %xmm1
;RIP (Instruction Pointer) - это специальный регистр в архитектуре x86-64 (64-разрядной архитектуре x86), который
;содержит адрес следующей инструкции, которая должна быть выполнена процессором. Точнее говоря, RIP указывает на адрес
;памяти, где расположена следующая инструкция для выполнения.
;В процессе выполнения программы, процессор последовательно выполняет инструкции, изменяя значение RIP на адрес
;следующей инструкции после выполнения текущей инструкции. Это обеспечивает последовательное выполнение кода
;программы.
;Загрузка константного значения из памяти в регистр xmm1.
        xorpd   %xmm1, %xmm0
        mulsd   -40(%rbp), %xmm0
;Умножение значения в xmm0 на значение в памяти по адресу -40(%rbp), результат сохраняется в xmm0.
        movq    -24(%rbp), %rax
;Загрузка значения из памяти по адресу -24(%rbp) в регистр %rax.
        addq    %rax, %rax
;Удвоение значения в %rax путем сложения его самого с собой.
        addq    $1, %rax
;Увеличение значения в %rax на 1.
        imulq   -24(%rbp), %rax
;Выполнение целочисленного умножения между значением в %rax и значением в памяти по адресу -24(%rbp),
;результат сохраняется в %rax.
        addq    %rax, %rax
        pxor    %xmm1, %xmm1
        cvtsi2sdq       %rax, %xmm1
;Конвертация целого значения в %rax в число с плавающей точкой и сохранение результата в xmm1.
        divsd   %xmm1, %xmm0
;Выполнение операции деления xmm0 на xmm1, результат сохраняется в xmm0.
        movsd   -8(%rbp), %xmm1
        mulsd   %xmm1, %xmm0
;Умножение значения в xmm0 на значение в xmm1, результат сохраняется в xmm0.
        movsd   %xmm0, -8(%rbp)
        addq    $1, -24(%rbp)
;Увеличение значения в памяти по адресу -24(%rbp) на 1.
.L2:
        movq    -24(%rbp), %rax
        cmpq    -48(%rbp), %rax
        jle      .L3
        movsd   -16(%rbp), %xmm0
        movq    %xmm0, %rax
        movq    %rax, %xmm0
        popq    %rbp
;Восстанавливает значение регистра %rbp из стека, завершая текущий фрейм стека.
        ret
;Выполняет возврат из функции.
.LC2:
        .string "Time taken: %lf sec.\n"
.LC3:
        .string "rez %lf\n"
main:
        pushq   %rbp
;Сохраняет текущее значение регистра %rbp на стеке. Это часто делается перед созданием нового
;фрейма стека для локальных переменных и сохранения предыдущего значения регистра %rbp.
        movq    %rsp, %rbp
;Устанавливает регистр %rbp в текущее значение регистра %rsp. Это устанавливает фрейм стека для текущей функции.
        addq    $-128, %rsp
;Выделяет 128 байтов памяти на стеке для локальных переменных и других данных. Уменьшение значения
;регистра %rsp означает выделение дополнительной памяти на стеке.
        movl    %edi, -116(%rbp)
;Загружает значение из регистра %edi (первый аргумент функции) в память по адресу -116(%rbp).
        movq    %rsi, -128(%rbp)
;Загружает значение из регистра %rsi (второй аргумент функции) в память по адресу -128(%rbp).
        movq    -128(%rbp), %rax
        addq    $8, %rax
        movq    (%rax), %rax
;Загружает значение, на которое указывает регистр %rax, в сам регистр %rax. Это, вероятно, происходит для получения
;какого-то значения из структуры.
        movq    %rax, %rdi
;Загружает значение из регистра %rax в регистр %rdi. Это значение будет использоваться
;как аргумент при вызове функции.
        call    atoll
        movq    %rax, -8(%rbp)
        movq    -128(%rbp), %rax
        addq    $16, %rax
        movq    (%rax), %rax
        movq    %rax, %rdi
        call    atof
        movq    %xmm0, %rax
        movq    %rax, -16(%rbp)
        movl    $2, %edi
        call    sysconf
        movq    %rax, -24(%rbp)
        leaq    -80(%rbp), %rax
        movq    %rax, %rdi
        call    times
        movq    -8(%rbp), %rdx
        movq    -16(%rbp), %rax
        movq    %rdx, %rdi
        movq    %rax, %xmm0
;movq %rdx, %rdi и movq %rax, %xmm0: Эти инструкции копируют значения rdx и rax в регистры rdi и xmm0, соответственно.
;xmm0 - это регистр для работы с числами с плавающей точкой. rdi будет использоваться как аргумент для функции sin_x.
        call    sin_x
        movq    %xmm0, %rax
        movq    %rax, -32(%rbp)
        leaq    -112(%rbp), %rax
        movq    %rax, %rdi
        call    times
        movq    -112(%rbp), %rax
        movq    -80(%rbp), %rdx
        subq    %rdx, %rax
        movq    %rax, -40(%rbp)
        pxor    %xmm0, %xmm0
        cvtsi2sdq       -40(%rbp), %xmm0
;Инструкция cvtsi2sdq -40(%rbp), %xmm0 в ассемблере x86-64 выполняет преобразование целого числа в число с
;плавающей точкой двойной точности (double) и загружает результат в регистр xmm0.
        pxor    %xmm1, %xmm1
        cvtsi2sdq       -24(%rbp), %xmm1
        divsd   %xmm1, %xmm0
        movq    %xmm0, %rax
        movq    %rax, %xmm0
        movl    $.LC2, %edi
;movl $.LC2, %edi: Эта инструкция загружает адрес строки формата (возможно, это строка формата для printf) в регистр edi.
        movl    $1, %eax
;Эта инструкция загружает значение 1 в регистр eax, которое будет использоваться как код возврата для функции printf
        call    printf
;Эта инструкция загружает указатель на структуру FILE для стандартного вывода (stdout) в регистр rax.
        movq    stdout(%rip), %rax
        movq    -32(%rbp), %rdx
        movq    %rdx, %xmm0
        movl    $.LC3, %esi
;Эта инструкция загружает адрес другой строки формата (возможно, для функции fprintf) в регистр esi.
        movq    %rax, %rdi
        movl    $1, %eax
;Устанавливает значение 1 в регистр %eax, что может быть использовано для определения успешного завершения вызова fprintf.
        call    fprintf
        movl    $0, %eax
;Эта инструкция загружает значение 0 в регистр eax, что обычно используется как код возврата функции main().
        leave
;Это инструкция, которая выполняет действия по завершению функции, возвращая стек в предыдущее состояние.
        ret
;Эта инструкция завершает выполнение функции и возвращает управление из main().
.LC1:
        .long   0
        .long   -2147483648
        .long   0
        .long   0