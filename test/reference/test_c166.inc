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

;;; Arithmetic and logical
        ADD     R1, R2
        ADD     R1, [R2]
        ADD     R1, [R3+]
        ADD     R1, #7
        ADD     0FE12H, #1234H
        ADD     R4, #1234H
        ADD     0FE12H, 1234H
        ADD     1234H, 0FE12H
        ADDB    RL1, RH2
        ADDB    RH0, [R0]
        ADDB    RL0, [R1+]
        ADDB    RH7, #5
        ADDB    0FE12H, #34H
        ADDB    0FE12H, 1234H
        ADDB    1234H, RH1
        ADDC    R1, R2
        ADDC    R1, #0
        ADDC    R1, [R2]
        ADDC    R1, [R3+]
        ADDC    0FE12H, #1234H
        ADDC    0FE12H, 1234H
        ADDC    1234H, 0FE12H
        ADDCB   RL1, [R3]
        ADDCB   RL1, RH2
        ADDCB   RL0, [R1+]
        ADDCB   RH7, #5
        ADDCB   0FE12H, #34H
        ADDCB   0FE12H, 1234H
        ADDCB   1234H, RH1
        SUB     R15, [R0+]
        SUB     1234H, R1
        SUB     R1, R2
        SUB     R1, [R2]
        SUB     R1, #7
        SUB     0FE12H, #1234H
        SUB     0FE12H, 1234H
        SUBB    0FE12H, #0FFH
        SUBB    RL1, RH2
        SUBB    RH0, [R0]
        SUBB    RL0, [R1+]
        SUBB    RH7, #5
        SUBB    0FE12H, 1234H
        SUBB    1234H, RH1
        SUBC    0FFDEH, 1234H
        SUBC    R1, R2
        SUBC    R1, [R2]
        SUBC    R1, [R3+]
        SUBC    R1, #7
        SUBC    0FE12H, #1234H
        SUBC    1234H, 0FE12H
        SUBCB   RH7, RL0
        SUBCB   RH0, [R0]
        SUBCB   RL0, [R1+]
        SUBCB   RH7, #5
        SUBCB   0FE12H, #34H
        SUBCB   0FE12H, 1234H
        SUBCB   1234H, RH1
        CMP     R1, #1234H
        CMP     0FE00H, 1234H
        CMP     R1, R2
        CMP     R1, [R2]
        CMP     R1, [R3+]
        CMP     R1, #7
        CMPB    RL0, [R2]
        CMPB    0FE12H, #12H
        CMPB    RL1, RH2
        CMPB    RL0, [R1+]
        CMPB    RH7, #5
        CMPB    0FE12H, 1234H
        XOR     R2, #3
        XOR     R1, R2
        XOR     R1, [R2]
        XOR     R1, [R3+]
        XOR     0FE12H, #1234H
        XOR     0FE12H, 1234H
        XOR     1234H, 0FE12H
        XORB    1234H, 0FE12H
        XORB    RL1, RH2
        XORB    RH0, [R0]
        XORB    RL0, [R1+]
        XORB    RH7, #5
        XORB    0FE12H, #34H
        XORB    0FE12H, 1234H
        AND     R3, [R1+]
        AND     R1, R2
        AND     R1, [R2]
        AND     R1, #7
        AND     0FE12H, #1234H
        AND     0FE12H, 1234H
        AND     1234H, 0FE12H
        ANDB    RL3, RH3
        ANDB    RH0, [R0]
        ANDB    RL0, [R1+]
        ANDB    RH7, #5
        ANDB    0FE12H, #34H
        ANDB    0FE12H, 1234H
        ANDB    1234H, RH1
        OR      0FE12H, #0FFFFH
        OR      R1, R2
        OR      R1, [R2]
        OR      R1, [R3+]
        OR      R1, #7
        OR      0FE12H, 1234H
        OR      1234H, 0FE12H
        ORB     RH1, #7
        ORB     RL1, RH2
        ORB     RH0, [R0]
        ORB     RL0, [R1+]
        ORB     0FE12H, #34H
        ORB     0FE12H, 1234H
        ORB     1234H, RH1
        NEG     R1
        CPL     R2
        NEGB    RH0
        CPLB    RL1
;;; Multiply and divide
        MUL     R1, R2
        MULU    R3, R4
        PRIOR   R5, R6
        DIV     R3
        DIVU    R4
        DIVL    R5
        DIVLU   R15
;;; Shift and rotate
        ROL     R1, R2
        ROL     R1, #15
        ROR     R3, R4
        ROR     R3, #1
        SHL     R5, R6
        SHL     R5, #0
        SHR     R7, R8
        SHR     R7, #8
        ASHR    R9, R10
        ASHR    R9, #10
;;; Compare and increment/decrement
        CMPI1   R1, #3
        CMPI1   R2, #1234H
        CMPI1   R3, 1234H
        CMPI2   R4, #15
        CMPI2   R5, #1234H
        CMPI2   R6, 1234H
        CMPD1   R7, #0
        CMPD1   R8, #1234H
        CMPD1   R9, 1234H
        CMPD2   R10, #1
        CMPD2   R11, #1234H
        CMPD2   R12, 1234H
;;; Move
        MOV     R1, R2
        MOV     R1, #5
        MOV     0FE12H, #1234H
        MOV     R1, [R2]
        MOV     R1, [R2+]
        MOV     [R2], R1
        MOV     [-R2], R1
        MOV     [R1], [R2]
        MOV     [R1+], [R2]
        MOV     [R1], [R2+]
        MOV     R1, [R2+1234H]
        MOV     [R2+1234H], R1
        MOV     [R1], 1234H
        MOV     1234H, [R1]
        MOV     0FE12H, 1234H
        MOV     1234H, 0FE12H
        MOVB    RL1, RH2
        MOVB    RH0, #5
        MOVB    0FE12H, #34H
        MOVB    RH0, [R2]
        MOVB    RH0, [R2+]
        MOVB    [R2], RH0
        MOVB    [-R2], RH0
        MOVB    [R1], [R2]
        MOVB    [R1+], [R2]
        MOVB    [R1], [R2+]
        MOVB    RH0, [R2+1234H]
        MOVB    [R2+1234H], RH0
        MOVB    [R1], 1234H
        MOVB    1234H, [R1]
        MOVB    0FE12H, 1234H
        MOVB    1234H, 0FE12H
        MOVBZ   R1, RH1
        MOVBZ   0FE12H, 1234H
        MOVBZ   1234H, RL0
        MOVBS   R2, RL1
        MOVBS   R1, 1234H
        MOVBS   1234H, 0FE12H
        SCXT    0FE12H, #1234H
        SCXT    R1, 1234H
        PUSH    0FE12H
        POP     R1
;;; Bit
        BCLR    0FD02H.3
        BSET    R1.15
        BSET    0FF02H.0
        BCLR    0FFDEH.9
        BFLDL   0FD02H, #12H, #34H
        BFLDH   0FD02H, #12H, #34H
        BFLDL   R2, #0FFH, #0
        BCMP    0FD02H.3, 0FD04H.5
        BMOVN   R1.0, 0FF00H.15
        BMOV    0FD02H.3, 0FD04H.5
        BOR     0FD02H.3, 0FD04H.5
        BAND    0FD02H.3, 0FD04H.5
        BXOR    0FD02H.3, 0FD04H.5
;;; Jump and call
LABEL1: JMPR    UC, LABEL1
        JMPR    Z, LABEL2
        JMPR    NZ, LABEL1
        JMPR    ULE, LABEL2
        JMPR    NET, LABEL2
        JMPR    SGT, LABEL1
        CALLR   LABEL2
        JB      0FD02H.3, LABEL1
        JNB     R1.0, LABEL2
        JBC     0FF02H.15, LABEL1
        JNBS    0FD00H.1, LABEL2
LABEL2: JMPA    NZ, LABEL2
        CALLA   UC, LABEL2
        JMPI    UC, [R1]
        JMPI    NET, [R1]
        JMPI    Z, [R1]
        JMPI    NZ, [R1]
        JMPI    V, [R1]
        JMPI    NV, [R1]
        JMPI    N, [R1]
        JMPI    NN, [R1]
        JMPI    C, [R1]
        JMPI    NC, [R1]
        JMPI    SGT, [R1]
        JMPI    SLE, [R1]
        JMPI    SLT, [R1]
        JMPI    SGE, [R1]
        JMPI    UGT, [R1]
        JMPI    ULE, [R1]
        CALLI   Z, [R15]
        JMPS    1, 2345H
        CALLS   3, 0FFFEH
        PCALL   0FE12H, LABEL1
        TRAP    #7FH
        TRAP    #0
        RET
        RETS
        RETI
        RETP    0FE12H
        RETP    R5
;;; System control
        NOP
        IDLE
        PWRDN
        SRVWDT
        SRST
        DISWDT
        EINIT
