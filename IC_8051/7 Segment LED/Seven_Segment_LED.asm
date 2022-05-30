ORG 00
START: JB P2.6, D6_is_not_pressed
	JNB P2.6, D6_is_pressed
D6_is_not_pressed:
	MOV P1,#0FFH
	CLR P3.4
	CLR P3.3
	MOV A, #10000000B
	MOV P1, A
	CPL P3.3
	MOV A, #11111001B
	MOV P1, A
	CLR P3.3
	CPL P3.4
	MOV A, #10010000B
	MOV P1, A
	CPL P3.3
	MOV A, #10110000B
	MOV P1, A
	ACALL START
D6_is_pressed:
	MOV P1,#0FFH 
	CLR P3.4
	CLR P3.3
	CPL P3.4
	CPL P3.3
	MOV A, #10000000B
	MOV P1, A
	CPL P3.3
	MOV A, #11111001B
	MOV P1, A
	CPL P3.4
	CPL P3.3
	MOV A, #10010000B
	MOV P1, A
	CPL P3.3
	MOV A, #10110000B
	MOV P1, A
	ACALL DELAY
	ACALL MOVE
	CLR P3.4
	CLR P3.3
	CPL P3.4
	CPL P3.3
	MOV A, #11111111B
	MOV P1, A
	CPL P3.3
	MOV A, #11111001B
	MOV P1, A
	CPL P3.4
	CPL P3.3
	MOV A, #10010000B
	MOV P1, A
	CPL P3.3
	MOV A, #10110000B
	MOV P1, A
	ACALL DELAY
	ACALL MOVE
	CLR P3.4
	CLR P3.3
	CPL P3.4
	CPL P3.3
	MOV A, #11111111B
	MOV P1, A
	CPL P3.3
	MOV A, #11111111B
	MOV P1, A
	CPL P3.4
	CPL P3.3
	MOV A, #10010000B
	MOV P1, A
	CPL P3.3
	MOV A, #10110000B
	MOV P1, A
	ACALL DELAY
	ACALL MOVE
	CLR P3.4
	CLR P3.3
	CPL P3.4
	CPL P3.3
	MOV A, #11111111B
	MOV P1, A
	CPL P3.3
	MOV A, #11111111B
	MOV P1, A
	CPL P3.4
	CPL P3.3
	MOV A, #11111111B
	MOV P1, A
	CPL P3.3
	MOV A, #10110000B
	MOV P1, A
	ACALL DELAY
	ACALL MOVE
	ACALL D6_is_pressed
ORG 100
MOVE: JB P2.6, START
DELAY: MOV R5, #05H
AGAIN: DJNZ R5, AGAIN
RET
END


