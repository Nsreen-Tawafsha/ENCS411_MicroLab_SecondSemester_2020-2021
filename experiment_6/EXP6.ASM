.MODEL TINY 
.CODE 
.STACK 100H 
JMP INSTALL 
MESS DB "WELCOME TO COOLEST LAB ON EARTH ENCS411 :D $"
MYINT PROC 
MOV BX,OFFSET MESS 
MOV DL,44 ;column 20 (# of mess chars) 
L1:
 MOV AL ,CS:[BX] 
 CMP al, '$'
 JZ L2
 PUSH BX 
 MOV BH,0 ;page number 0
 MOV AH,2 ;set the curser position 
 MOV DH,20 ;row 0 
 INC DL 
 INT 10H ;to access video memory  
 MOV AH,9H ;Write character and attribute at cursor position 
 MOV BH,0 ;page number 0
 MOV BL,84H ;color of the character
 MOV CX,1 ;number of times to write the character
 INT 10H ;video memory access
 POP BX
 INC BX
 JMP L1
L2:
 IRET 
MYINT ENDP
INSTALL: ;install new interrupt vector
 MOV AH,25H ;set interrupt vector
 MOV AL,62H ;desired interrupt vector
 MOV DX,CS
 MOV DS,DX
 MOV DX, OFFSET MYINT
 INT 21H ; Terminate & stay resident
 MOV AX,3100H
 INT 21H
 END