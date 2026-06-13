;;; Copyright 2026 Tadashi G. Takaoka
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


;;; Internal Control
        HLT
        EIS
        DIS
        TCI
        CLRC
        SETC
        ;; SDBD is tested below in the SDBD prefix sections.

;;; Single Register
        GSWD    R0
        GSWD    R1
        GSWD    R2
        GSWD    R3
        NOP
        NOP     2
        SIN
        SIN     2
        RSWD    R0
        RSWD    R1
        RSWD    R2
        RSWD    R3
        RSWD    R4
        RSWD    R5
        RSWD    R6
        RSWD    R7
        INCR    R0
        INCR    R1
        INCR    R2
        INCR    R3
        INCR    R4
        INCR    R5
        INCR    R6
        INCR    R7
        DECR    R0
        DECR    R1
        DECR    R2
        DECR    R3
        DECR    R4
        DECR    R5
        DECR    R6
        DECR    R7
        COMR    R0
        COMR    R1
        COMR    R2
        COMR    R3
        COMR    R4
        COMR    R5
        COMR    R6
        COMR    R7
        NEGR    R0
        NEGR    R1
        NEGR    R2
        NEGR    R3
        NEGR    R4
        NEGR    R5
        NEGR    R6
        NEGR    R7
        ADCR    R0
        ADCR    R1
        ADCR    R2
        ADCR    R3
        ADCR    R4
        ADCR    R5
        ADCR    R6
        ADCR    R7

;;; Register Shift (R0-R3 only)
        SWAP    R0
        SWAP    R1
        SWAP    R2
        SWAP    R3
        SWAP    R0, 2
        SWAP    R1, 2
        SWAP    R2, 2
        SWAP    R3, 2
        SLL     R0
        SLL     R1
        SLL     R2
        SLL     R3
        SLL     R0, 2
        SLL     R1, 2
        SLL     R2, 2
        SLL     R3, 2
        RLC     R0
        RLC     R1
        RLC     R2
        RLC     R3
        RLC     R0, 2
        RLC     R1, 2
        RLC     R2, 2
        RLC     R3, 2
        SLLC    R0
        SLLC    R1
        SLLC    R2
        SLLC    R3
        SLLC    R0, 2
        SLLC    R1, 2
        SLLC    R2, 2
        SLLC    R3, 2
        SLR     R0
        SLR     R1
        SLR     R2
        SLR     R3
        SLR     R0, 2
        SLR     R1, 2
        SLR     R2, 2
        SLR     R3, 2
        SAR     R0
        SAR     R1
        SAR     R2
        SAR     R3
        SAR     R0, 2
        SAR     R1, 2
        SAR     R2, 2
        SAR     R3, 2
        RRC     R0
        RRC     R1
        RRC     R2
        RRC     R3
        RRC     R0, 2
        RRC     R1, 2
        RRC     R2, 2
        RRC     R3, 2
        SARC    R0
        SARC    R1
        SARC    R2
        SARC    R3
        SARC    R0, 2
        SARC    R1, 2
        SARC    R2, 2
        SARC    R3, 2

;;; Register-to-Register
        MOVR    R0, R0
        MOVR    R0, R7
        MOVR    R7, R0
        MOVR    R7, R7
        MOVR    R1, R2
        MOVR    R3, R4
        MOVR    R5, R6
        ADDR    R0, R0
        ADDR    R0, R7
        ADDR    R7, R0
        ADDR    R7, R7
        ADDR    R1, R2
        ADDR    R3, R4
        ADDR    R5, R6
        SUBR    R0, R0
        SUBR    R0, R7
        SUBR    R7, R0
        SUBR    R7, R7
        SUBR    R1, R2
        SUBR    R3, R4
        SUBR    R5, R6
        CMPR    R0, R0
        CMPR    R0, R7
        CMPR    R7, R0
        CMPR    R7, R7
        CMPR    R1, R2
        CMPR    R3, R4
        CMPR    R5, R6
        ANDR    R0, R0
        ANDR    R0, R7
        ANDR    R7, R0
        ANDR    R7, R7
        ANDR    R1, R2
        ANDR    R3, R4
        ANDR    R5, R6
        XORR    R0, R0
        XORR    R0, R7
        XORR    R7, R0
        XORR    R7, R7
        XORR    R1, R2
        XORR    R3, R4
        XORR    R5, R6

;;; Alias
        TSTR    R0
        TSTR    R7
        TSTR    R3
        CLRR    R0
        CLRR    R7
        CLRR    R3
        JR      R0
        JR      R7
        JR      R3

;;; Jump / JSR
L_JUMP:
        J       L_JUMP
        JE      L_JUMP
        JD      L_JUMP
        JSR     R4, L_JUMP
        JSR     R5, L_JUMP
        JSR     R6, L_JUMP
        JSRE    R4, L_JUMP
        JSRE    R5, L_JUMP
        JSRE    R6, L_JUMP
        JSRD    R4, L_JUMP
        JSRD    R5, L_JUMP
        JSRD    R6, L_JUMP

;;; Data Access - Direct
D_DATA: EQU     X'1234'
        MVO     R0, D_DATA
        MVO     R1, D_DATA
        MVO     R2, D_DATA
        MVO     R3, D_DATA
        MVO     R4, D_DATA
        MVO     R5, D_DATA
        MVI     D_DATA, R0
        MVI     D_DATA, R1
        MVI     D_DATA, R2
        MVI     D_DATA, R3
        MVI     D_DATA, R4
        MVI     D_DATA, R5
        MVI     D_DATA, R6
        MVI     D_DATA, R7
        ADD     D_DATA, R0
        ADD     D_DATA, R1
        ADD     D_DATA, R2
        ADD     D_DATA, R3
        ADD     D_DATA, R4
        ADD     D_DATA, R5
        ADD     D_DATA, R6
        ADD     D_DATA, R7
        SUB     D_DATA, R0
        SUB     D_DATA, R1
        SUB     D_DATA, R2
        SUB     D_DATA, R3
        SUB     D_DATA, R4
        SUB     D_DATA, R5
        SUB     D_DATA, R6
        SUB     D_DATA, R7
        CMP     D_DATA, R0
        CMP     D_DATA, R1
        CMP     D_DATA, R2
        CMP     D_DATA, R3
        CMP     D_DATA, R4
        CMP     D_DATA, R5
        CMP     D_DATA, R6
        CMP     D_DATA, R7
        AND     D_DATA, R0
        AND     D_DATA, R1
        AND     D_DATA, R2
        AND     D_DATA, R3
        AND     D_DATA, R4
        AND     D_DATA, R5
        AND     D_DATA, R6
        AND     D_DATA, R7
        XOR     D_DATA, R0
        XOR     D_DATA, R1
        XOR     D_DATA, R2
        XOR     D_DATA, R3
        XOR     D_DATA, R4
        XOR     D_DATA, R5
        XOR     D_DATA, R6
        XOR     D_DATA, R7

;;; Data Access - Indirect via R1-R3 (no auto-increment)
        MVO@    R0, R1
        MVO@    R1, R1
        MVO@    R2, R1
        MVO@    R3, R1
        MVO@    R4, R1
        MVO@    R5, R1
        MVO@    R0, R2
        MVO@    R0, R3
        MVI@    R1, R0
        MVI@    R1, R1
        MVI@    R1, R2
        MVI@    R1, R3
        MVI@    R1, R4
        MVI@    R1, R5
        MVI@    R1, R6
        MVI@    R1, R7
        MVI@    R2, R0
        MVI@    R3, R0
        ADD@    R1, R0
        ADD@    R2, R1
        ADD@    R3, R2
        SUB@    R1, R0
        SUB@    R2, R1
        SUB@    R3, R2
        CMP@    R1, R0
        CMP@    R2, R1
        CMP@    R3, R2
        AND@    R1, R0
        AND@    R2, R1
        AND@    R3, R2
        XOR@    R1, R0
        XOR@    R2, R1
        XOR@    R3, R2

;;; Data Access - Indirect via R4-R5 (post-increment)
        MVO@    R0, R4
        MVO@    R1, R5
        MVI@    R4, R0
        MVI@    R5, R1
        ADD@    R4, R2
        ADD@    R5, R3
        SUB@    R4, R4
        SUB@    R5, R5
        CMP@    R4, R6
        CMP@    R5, R7
        AND@    R4, R0
        AND@    R5, R1
        XOR@    R4, R2
        XOR@    R5, R3

;;; Data Access - Stack via R6
        MVO@    R0, R6          ; push R0
        MVO@    R1, R6          ; push R1
        MVI@    R6, R0          ; pull to R0
        MVI@    R6, R1          ; pull to R1
        PSHR    R2
        PSHR    R3
        PULR    R4
        PULR    R5
        ADD@    R6, R0
        SUB@    R6, R1
        CMP@    R6, R2
        AND@    R6, R3
        XOR@    R6, R4

;;; Data Access - Immediate
        MVII    X'00', R0
        MVII    X'FF', R1
        MVII    X'00', R7
        MVII    X'FF', R7
        MVOI    R0, X'00'
        MVOI    R1, X'FF'
        ADDI    X'00', R0
        ADDI    X'FF', R1
        SUBI    X'00', R2
        SUBI    X'FF', R3
        CMPI    X'00', R4
        CMPI    X'FF', R5
        ANDI    X'00', R6
        ANDI    X'FF', R7
        XORI    X'00', R0
        XORI    X'FF', R1

;;; SDBD prefix - double-byte immediate (16-bit values)
        SDBD
        MVII    X'1234', R0
        SDBD
        MVII    -1, R1
        SDBD
        ADDI    X'ABCD', R2
        SDBD
        SUBI    X'1234', R3
        SDBD
        CMPI    X'5678', R4
        SDBD
        ANDI    X'9ABC', R5
        SDBD
        XORI    X'DEF0', R6

;;; SDBD prefix - double-byte indirect (16-bit values via MVI@/ADD@/etc.)
        SDBD
        MVI@    R1, R0
        SDBD
        ADD@    R2, R1
        SDBD
        SUB@    R3, R2
        SDBD
        CMP@    R4, R3
        SDBD
        AND@    R5, R4
        SDBD
        XOR@    R5, R5

;;; Branches - forward and backward
L_BRA:
        B       L_BRA           ; backward unconditional
        NOPP
        B       L_BRA_END       ; forward unconditional
        BC      L_BRA
        BNC     L_BRA
        BOV     L_BRA
        BNOV    L_BRA
        BPL     L_BRA
        BMI     L_BRA
        BZE     L_BRA
        BNZE    L_BRA
        BLT     L_BRA
        BGE     L_BRA
        BLE     L_BRA
        BGT     L_BRA
        BUSC    L_BRA
        BESC    L_BRA
        BEXT    L_BRA, 0
        BEXT    L_BRA, 1
        BEXT    L_BRA, 7
        BEXT    L_BRA, 14
        BEXT    L_BRA, 15
L_BRA_END:
        BC      L_BRA_END       ; forward
        BNC     L_BRA_END
        BOV     L_BRA_END
        BNOV    L_BRA_END
        BPL     L_BRA_END
        BMI     L_BRA_END
        BZE     L_BRA_END
        BNZE    L_BRA_END
        BLT     L_BRA_END
        BGE     L_BRA_END
        BLE     L_BRA_END
        BGT     L_BRA_END
        BUSC    L_BRA_END
        BESC    L_BRA_END
        BEXT    L_BRA_END, 0
        BEXT    L_BRA_END, 7
        BEXT    L_BRA_END, 15

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
