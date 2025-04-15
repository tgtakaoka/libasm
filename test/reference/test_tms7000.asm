;;; Copyright 2023 Tadashi G. Takaoka
;;;
;;; Licensed under the Apache License, Version 2.0 (the "License");
;;; you may not use this file except in compliance with the License.
;;; You may obtain a copy of the License at
;;;
;;;     http://www.apache.org/licenses/LICENSE-2.0
;;;
;;; Unless required by applicable law or agreed to in writing, software
;;; distributed under the License is distributed on an "AS IS" BASIS,
;;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;;; See the License for the specific language governing permissions and
;;; limitations under the License.

        CPU     TMS70C00
        ORG     0200h

;;; 0x
	NOP
	IDLE
	EINT
	DINT
	SETC
	POP	ST
	STSP
	RETS
	RETI
	LDSP
	PUSH	ST

;;; 1x
	MOV	R19, A
	AND	R20, A
        OR      R21, A
	XOR	R22, A
	BTJO	R23, A, $+27
	BTJZ	R24, A, $+28
	ADD	R25, A
	ADC	R26, A
	SUB	R27, A
	SBB	R28, A
	MPY	R29, A
	CMP	R30, A
	DAC	R31, A
	DSB	R32, A

;;; 2x
	MOV	%23H, A
	AND	%24H, A
	OR	%25H, A
	XOR	%26H, A
	BTJO	%27H, A, $+43
	BTJZ	%28H, A, $+44
	ADD	%29H, A
	ADC	%2AH, A
	SUB	%2BH, A
	SBB	%2CH, A
	MPY	%2DH, A
	CMP	%2EH, A
	DAC	%2FH, A
	DSB	%30H, A

;;; 3x
	MOV	R51, B
	AND	R52, B
	OR	R53, B
	XOR	R54, B
	BTJO	R55, B, $+59
	BTJZ	R56, B, $+60
	ADD	R57, B
	ADC	R58, B
        SUB     R59, B
	SBB	R60, B
	MPY	R61, B
	CMP	R62, B
	DAC	R63, B
	DSB	R64, B

;;; 4x
	MOV	R67, R68
	AND	R68, R69
	OR	R69, R70
	XOR	R70, R71
	BTJO	R71, R72, $+77
	BTJZ	R72, R73, $+78
	ADD	R73, R74
	ADC	R74, R75
	SUB	R75, R76
	SBB	R76, R77
	MPY	R77, R78
	CMP	R78, R79
	DAC	R79, R80
	DSB	R80, R81

;;; 5x
	MOV	%53H, B
	AND	%54H, B
	OR	%55H, B
	XOR	%56H, B
	BTJO	%57H, B, $+91
	BTJZ	%58H, B, $+92
	ADD	%59H, B
	ADC	%5AH, B
	SUB	%5BH, B
	SBB	%5CH, B
	MPY	%5DH, B
	CMP	%5EH, B
	DAC	%5FH, B
	DSB	%60H, B

;;; 6x
	MOV	B, A
	AND	B, A
	OR	B, A
	XOR	B, A
	BTJO	B, A, $+105
	BTJZ	B, A, $+106
	ADD	B, A
	ADC	B, A
	SUB	B, A
	SBB	B, A
	MPY	B, A
	CMP	B, A
	DAC	B, A
	DSB	B, A

;;; 7x
        MOV     %73H, R116
	AND	%74H, R117
	OR	%75H, R118
	XOR	%76H, R119
	BTJO	%77H, R120, $+125
	BTJZ	%78H, R121, $+126
	ADD	%79H, R122
	ADC	%7AH, R123
	SUB	%7BH, R124
        SBB     %7CH, R125
	MPY	%7DH, R126
	CMP	%7EH, R127
	DAC	%7FH, R128
	DSB	%80H, R129

;;; 8x
        MOVP    P129, A
	MOVP    A, P131
	ANDP    A, P132
	ORP	A, P133
	XORP	A, P134
	BTJOP	A, P135, $-117
	BTJZP	A, P136, $-116
	MOVD	%898AH, R139
	LDA	@8B8CH
	STA	@8C8DH
	BR	@8D8EH
	CMPA	@8E8FH
	CALL	@8F90H

;;; 9x
	MOVP	P146, B
	MOVP	B, P147
        ANDP    B, P148
	ORP	B, P149
	XORP	B, P150
	BTJOP	B, P151, $-101
	BTJZP	B, P152, $-100
	MOVD	R153, R154
	LDA	*R155
	STA	*R156
	BR	*R157
	CMPA	*R158
	CALL	*R159

;;; Ax
        MOVP    %0A3H, P164
	ANDP	%0A4H, P165
	ORP	%0A5H, P166
	XORP	%0A6H, P167
	BTJOP	%0A7H, P168, $-83
	BTJZP	%0A8H, P169, $-82
	MOVD	%0A9AAH(B), R171
	LDA	@0ABACH(B)
	STA	@0ACADH(B)
	BR	@0ADAEH(B)
	CMPA	@0AEAFH(B)
	CALL	@0AFB0H(B)

;;; Bx
	CLRC
	TSTA
	DEC	A
	INC	A
	INV	A
	CLR	A
	XCHB	A
	SWAP	A
	PUSH	A
	POP	A
	DJNZ	A, $-67
	DECD	A
	RR	A
	RRC	A
	RL	A
	RLC	A

;;; Cx
	MOV	A, B
	TSTB
	DEC	B
	INC	B
	INV	B
	CLR	B
	XCHB	B
	SWAP	B
	PUSH	B
	POP	B
	DJNZ	B, $-51
	DECD	B
	RR	B
	RRC	B
	RL	B
	RLC	B

;;; Dx
	MOV	A, R209
	MOV	B, R210
	DEC	R211
	INC	R212
	INV	R213
	CLR	R214
	XCHB	R215
	SWAP	R216
	PUSH	R217
	POP	R218
	DJNZ	R219, $-33
	DECD	R220
	RR	R221
	RRC	R222
	RL	R223

;;; Ex
	JMP	$-29
	JN	$-28
        JLT     $-28
	JEQ	$-27
	JZ	$-27
	JC	$-26
	JHS	$-26
	JP	$-25
        JGT     $-25
	JPZ	$-24
        JGE     $-24
	JNE	$-23
	JNZ	$-23
	JL	$-22
	JNC	$-22
	TRAP	23
	TRAP	22
	TRAP	21
	TRAP	20
	TRAP	19
	TRAP	18
	TRAP	17
	TRAP	16

;;; Fx
	TRAP	15
	TRAP	14
	TRAP	13
	TRAP	12
	TRAP	11
	TRAP	10
	TRAP	9
	TRAP	8
	TRAP	7
	TRAP	6
	TRAP	5
	TRAP	4
	TRAP	3
	TRAP	2
	TRAP	1
	TRAP	0

        END

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
