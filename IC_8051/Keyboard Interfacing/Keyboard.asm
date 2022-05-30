ORG 00
start: MOV R0, #00H ; clear R0 - the first key is key0
;Deactivate Row0, Row1, and Row2
SETB P0.0 ; set row0
SETB P0.1 ; set row1
SETB P0.2 ; set row2
 
 
;Scan Row3 (i.e., Scan Key 1, 2, 3)
CLR P0.3 ; clear row3
CALL Key

work: CJNE R0, #02H, start
CLR P0.7
MOV A, #00H
MOV A, #20H
MOV P1, A
CALL Delay
CALL Key
MOV A, #00H
MOV P1, A
CALL Delay
CALL Key
JMP work
; column-scan subroutine


Key: JNB P0.5, gotKey
CJNE R2, #02H,start ; if col1 is cleared key found (i.e., 2, 5, 8, 0)
JMP work
 ; return from subroutine - key not found
gotKey:
MOV R0, #02H ; key found
RET; and return from subroutine

Delay:
MOV R5, #05H
Again: DJNZ R5, Again
RET
