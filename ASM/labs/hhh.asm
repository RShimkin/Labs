.model tiny
org 100h

.data
x db 10
y db 5
m db 6
n db 5
str db 13,10,'$' ;�� ���� ������ � �����

.code

start:

mov cl,[y] ;���� � ���
mov bl,[m] ; ����� ��������������
mov ah,02h  ;����� ������� �� �����

pos_y:

mov dl,0Ah ;������� �� ����� ������
int 21h
loop pos_y  ;��������� ���� � ���

mov cl,[x]
pos_x:
mov dl,20h  ;��� �������
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





