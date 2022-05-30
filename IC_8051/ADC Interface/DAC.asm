ORG 000
CLR P0.7
Start:MOV A, #00H
MOV A, #50H
MOV R2, #03H 
MOV R3, #62H
RisingWave: MOV P1, A
CALL DELAY1 
ADD A, #50H
DJNZ R2, RisingWave 
SUBB A, #50H
Ramp:SUBB A, #05H
MOV P1, A
DJNZ R3, Ramp
JMP Start

ORG 300
DELAY1: MOV R5, #05H
AGAIN: DJNZ R5, AGAIN
RET
END