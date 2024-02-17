;(c*2 - d/4)/(a*a + 1)
.model tiny
org 100h

.data
      a db 1
      c db 4
      d db 8
      
      result dw 0
      
.code
      mov al,[c]
      mov bl, 2
      imul bl
      push ax
      
      mov al,[d]
      CBW
      mov bl,4d
      idiv bl
      CBW
      
      pop bx
      sub bx,ax
      
      mov al,a
      imul al
      inc ax

      xchg ax,bx
      CWD
      idiv bx
      mov [result],ax
      
      


ret





