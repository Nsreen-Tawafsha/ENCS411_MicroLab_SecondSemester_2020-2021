;This code is to program Int 70h to add the last two digits from my university id (1+9 = 10)
;1182319-Nsreen Tawafsha

.MODEL TINY 
.CODE 
.STACK 100H 

JMP INSTALL 
MSG DB "The summation of the numbers 1 and 9 =  $" 
NUM1 DW 1
NUM2 DW 9

MYINT PROC 
  mov Ax,@data
  mov ds,Ax
 
  MOV DX,OFFSET MSG
  MOV AH, 09H
  INT 21H
  
  MOV AX, NUM1
  MOV BX, NUM2
  ADD AX, BX
  
  MOV BX, 10
  DIV BX         ;AX=QUOTIENT, DX = REMINDER
  MOV BX, DX  
  
  ADD AX, 30H  
  MOV DL, AL
  MOV AH, 02H
  INT 21H 
  
  ADD BX, 30H
  MOV DL, BL
  MOV AH, 02H
  INT 21H

  MOV AH,4CH
  INT 21H

  IRET
  MYINT ENDP 

INSTALL:
 ;install new interrupt vector
  MOV AH,25H ;set interrupt vector 
  MOV AL,70H ;desired interrupt vector
  MOV DX,CS
  MOV DS,DX
  MOV DX, OFFSET MYINT
  INT 21H

 ; Terminate & stay resident
  MOV AX,3100H
  INT 21H
END