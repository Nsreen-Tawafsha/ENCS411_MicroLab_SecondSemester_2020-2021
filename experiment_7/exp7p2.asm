.model small
.stack 1000h
.data
t equ 20
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
tone 4553,t ; hap (c4)
tone 4553,t ; py (c4)
tone 4057,2*t ; birth (d4)
tone 4553,2*t;c4
tone 6409,2*t;f4
tone 3606,4*t;e4
tone 4553,2*t;c4
tone 4553,2*t;c4
tone 4057,2*t;d4
tone 4553,2*t;c4
tone 3035,2*t;g4
tone 3409,4*t;f4
tone 4553,t;c4
tone 4553,t;c4
tone 2275,2*t;c5
tone 2704,2*t;a4
tone 3409,2*t;f4
tone 3608,2*t;e4
tone 4057,6*t;d4
tone 2553,t;b4b
tone 2553,t;b4b
tone 2704,2*t;a4
tone 3409,2*t;f4
tone 3035,2*t;g4
tone 3409,4*t;f4
; Continue your code here……
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