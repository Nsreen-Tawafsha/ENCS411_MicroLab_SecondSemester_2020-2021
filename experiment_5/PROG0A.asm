.MODEL SMALL
.386
.STACK 64
.DATA
MESS DB 'Hello World!',13,10,'$'
.CODE
BEGIN PROC FAR
MOV AX,@DATA 
MOV DS,AX
MOV AH,9H
MOV DX,OFFSET MESS
INT 21H
MOV AH,4CH
INT 21H
BEGIN ENDP
END BEGIN
