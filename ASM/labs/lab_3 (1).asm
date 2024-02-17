;proizvedenie elem mas<d, mas>c
.model tiny
org 100h

.data
        n db 5
        c dw 1
        d dw 5
        mas dw n dup(0)
        er db "EROR!!!$"
        res dw 1
.code
        xor cx,cx ;obnylaem cx
        mov cl, [n] ;kolichestvo elem v chetchik
        
; ввод массива       
cin_mas_i:
        mov ah, 08h ;vvod simvola 
        int 21h
        cmp al,0Dh ;kod "enter"
        je finish_cin_mas_i ; if = 
        
        cmp al,30h ;kod '0'
        jb eror  ; if<
        cmp al, 39h ;kod '9'
        ja eror  ; if>
        
        sub al,30h ;perevod symbol v zifry
        mov bl,al ; сохр. число
        mov ax, mas[si] 
        mov dx,10
        mul dx
        mov dx,ax
        mov al,bl   ; преобразование цифр в число
        CBW
        add ax,dx
        adc ax,0
        mov mas[si],ax
        jmp cin_mas_i  ;преход
        
finish_cin_mas_i:

        add si,2
        loop cin_mas_i
        
        xor cx,cx
        xor si,si
        mov cl, [n]
        
start_loop:
        mov ax,mas[si]
        mov bx,[c]
        cmp ax,bx
        jae above
        jmp finish_loop  ; сравниваем элемент с 'c'
above:
        mov bx,[d]
        cmp ax,bx
        jbe mix
        jmp finish_loop  ; сравниваем с d
mix:
        mov bx,ax
        mov ax,[res]
        mul bx
        mov [res],ax  ; перемножаем числа удволетворившие неравества

finish_loop:
        add si,2
        loop start_loop  ; если не удволетворило, то переходим к след элементу
        
        jmp finish
eror:
        mov ah,09h  ; выводим ошибку
        lea dx, er
        int 21h
finish:
    
ret