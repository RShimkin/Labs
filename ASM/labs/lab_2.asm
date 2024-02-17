;a/b + 5, if a > b
;-25,     if a = b
;(a-5)/b  if a < b
.model tiny
org 100h
.data
      a dw 10
      b dw 5
      result dd 0
.code
      mov ax,[a]
      mov bx,[b]
      cmp ax,bx ;cravnivaet
      jg metka1
      je metka2
      jmp metka3

metka1:
      cmp bx,0
      je finish_eror
      idiv bx
      CBW
      add ax,5
      adc ax,0
      mov [result],ax
      jmp finish
metka2:
      mov [result],-25
      jmp finish
metka3:
      cmp bx,0
      je finish_eror
      sub ax,5
      sbb ax,0
      CWD
      idiv bx
      mov [result],dx
      jmp finish
finish:
ret
finish_eror:
mov ax,"!"
mov [result],ax
ret