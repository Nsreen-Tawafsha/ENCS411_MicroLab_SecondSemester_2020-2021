.model small
.stack 1000h
.data
t equ 10
.code
tone macro div,dur
mov al,0b6h
out 43h,al
mov ax,div
out 42h,al
mov al,ah
out 42h,al
mov al,00000011b
out 61h,al
mov cx,dur
call delay1
mov al,00000000b
out 61h,al
call delay2
endm
.startup

lab:
mov ah,1h
int 21h
cmp al,'a'
jz A
cmp al,'b'
jz B
cmp al,'c'
jz Ce
cmp al,'d'
jz n5
jmp s5
n5:
jmp D
s5:
cmp al,'e'
jz n1
jmp s1
n1:
jmp E
s1:
cmp al,'f'
jz n2
jmp s2
n2:
jmp F
s2:
cmp al,'g'
jz n3
jmp s3
n3:
jmp G
s3:
cmp al,'h'

jz n4
jmp s4
n4:
jmp H
s4:
A:
tone 2704,t;a4
jmp lab
B:
tone 2553,t;b4b
jmp lab
Ce:
tone 4553,t;c4
jmp lab
D:
tone 4057,t;d4
jmp lab
E:
tone 3608,t;e4
jmp lab
F:
tone 3409,t;f4
jmp lab
G:
tone 3035,t;g4
jmp lab
H:
tone 2275,t;c5
jmp lab
; Continue your code here……
q:
mov ah,4ch
int 21h
delay1 proc near
d1:
push cx
mov cx,38000
d2:
loop d2
pop cx
loop d1
ret
delay1 endp
delay2 proc near
mov cx,65000
d3:
loop d3
ret
delay2 endp
end