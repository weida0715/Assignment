IN R0
OUT R0
IN R1
OUT R1
IN R2
OUT R2

MOV R0, 10
MOV R1, 666
MOV R2, -513
MOV R3, R2
MOV R4, [0]

ADD R0, R1
ADD R1, 800
ADD R2, -666
ADD R3, 8

SUB R3, R4
SUB R2, 800
SUB R1, -666
SUB R0, 8

MUL R0, R1
MUL R1, 800
MUL R2, -666
MUL R3, 8

DIV R3, R4
DIV R2, 800
DIV R1, -666
DIV R0, 8

MOV R4, 69
MOV R5, 127
MOV R6, -128
INC R4
DEC R4
INC R5
DEC R6

ROL R0, 1
ROR R2, 3
SHL R3, 5
SHR R6, 7

LOAD R1, 0
LOAD R3, [2]
STORE R5, 4
STORE R7, [6]