.model tiny
org 100h

.data
x db 10
y db 5
m db 6
n db 5
str db 13,10,'$' ;на след строку и влево

.code

start:

mov cl,[y] ;вниз у раз
mov bl,[m] ; длина прямоугольника
mov ah,02h  ;вывод символа на экран

pos_y:

mov dl,0Ah ;переход на новую строку
int 21h
loop pos_y  ;опустимся вниз у раз

mov cl,[x]
pos_x:
mov dl,20h  ;код пробела
int 21h
loop pos_x

mov cl,[n]
cout:
mov dl,'h'
int 21h
loop cout

mov ah,09h
lea dx, str
int 21h
mov ah,02h

mov cl,[x]

dec bl
cmp bl,0
jne pos_x

ret





