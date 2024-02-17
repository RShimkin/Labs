    .model tiny

    org 100h

    .data
    x db 1  
    y db 1  ;kol strok i pervi stmbol
    .code

    mov ah,02h
    mov cl,5

    start:
    mov dl,30h
    add dl,[x]
    ;mov dl,bl
    int 21h
    inc x
    loop start ; if CL=0, prodolzhaem dalsh

    mov dl,10    ;symbol perehoda na sled stroky
    int 21h
    mov dl,13    ;symbol perenosa karetki vlevo
    int 21h

    mov bl,[y]
    inc bl
    mov [y],bl
    mov [x],bl

    mov cl,5    ;schetchic symbol

    cmp bl,5   ; cravnivaem
    jle start  ;if <= pereghodit na metky

    ret






