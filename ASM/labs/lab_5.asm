
.model tiny
org 100h
 
.data
        x db 10
        y db 10
       
        m db 8
        n db 8
        str db 13,10,'$'
 
.code
 
        mov cl,[y]
        mov ah,02h  
       
p_y:
        mov dl,0Ah
        int 21h
        loop p_y
       
        mov cl,[x]
p_x:
        mov dl,' '
        int 21h
        loop p_x
       
        mov cl,[n]
l1:
        mov dl,'h'
        int 21h
        loop l1
       
        mov ah,09h
        lea dx, str
        int 21h
        mov ah,02h
       
        mov cl,[x]
       
        dec [m]
        cmp [m],0
        jne p_x

ret




