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

;;; Single-byte instructions

        NOP
        RETI
        INCF
        DECF
        LDF     0
        LDF     3
        SWI     0
        SWI     7
        SWI                     ; bare SWI defaults to vec 7
        RCF
        SCF
        CCF
        ZCF
        EI      0
        EI      7
        EI                      ; bare EI defaults to level 0
        DI
        PUSH    SR
        POP     SR
        PUSH    A
        POP     A
        EX      F,F'
        HALT

;;; LD (n8), #n8 / LDW (n8), #n16

        LD      (034H),056H
        LDW     (034H),01234H

;;; PUSH #n8 / PUSHW #n16

        PUSH    056H
        PUSHW   01234H

;;; LD: 8-bit register (current bank) <- register

        LD      A,A
        LD      A,W
        LD      A,B
        LD      A,C
        LD      A,D
        LD      A,E
        LD      A,H
        LD      A,L
        LD      W,A
        LD      B,A
        LD      C,A
        LD      D,A
        LD      E,A
        LD      H,A
        LD      L,A

;;; LD: 8-bit register <- immediate

        LD      A,034H
        LD      W,034H
        LD      B,034H
        LD      C,034H
        LD      D,034H
        LD      E,034H
        LD      H,034H
        LD      L,034H

;;; LD: 8-bit register <- memory (various addressing modes)

        LD      A,(XIX)
        LD      A,(XIY)
        LD      A,(XIZ)
        LD      A,(XSP)
        LD      A,(-XIX)
        LD      A,(XIX+)
        LD      A,(XIX+20H)
        LD      A,(XIX-20H)
        LD      A,(XIX+1234H)
        LD      A,(XIX-1234H)
        LD      A,(034H)
        LD      A,(01234H)
        LD      A,(0123456H)
        LD      A,(XIX+A)
        LD      A,(XIX+B)

;;; LD: memory <- 8-bit register

        LD      (XIX),A
        LD      (XIY),A
        LD      (-XIX),A
        LD      (XIX+),A
        LD      (XIX+20H),A
        LD      (XIX+1234H),A
        LD      (034H),A
        LD      (01234H),A
        LD      (0123456H),A
        LD      (XIX+A),A

;;; LD: memory <- 8-bit immediate

        LD      (XIX),034H
        LD      (-XIX),034H
        LD      (XIX+),034H
        LD      (XIX+20H),034H
        LD      (XIX+1234H),034H
        LD      (034H),034H
        LD      (01234H),034H
        LD      (0123456H),034H
        LD      (XIX+A),034H

;;; LD: 16-bit register (current bank) <- register

        LD      WA,WA
        LD      WA,BC
        LD      WA,DE
        LD      WA,HL
        LD      WA,IX
        LD      WA,IY
        LD      WA,IZ
        LD      WA,SP
        LD      BC,WA
        LD      DE,WA
        LD      HL,WA
        LD      IX,WA
        LD      IY,WA
        LD      IZ,WA
        LD      SP,WA

;;; LD: 16-bit register <- immediate

        LD      WA,01234H
        LD      BC,01234H
        LD      DE,01234H
        LD      HL,01234H
        LD      IX,01234H
        LD      IY,01234H
        LD      IZ,01234H
        LD      SP,01234H

;;; LD: 16-bit register <- memory

        LD      WA,(XIX)
        LD      WA,(XIY)
        LD      WA,(-XIX)
        LD      WA,(XIX+)
        LD      WA,(XIX+20H)
        LD      WA,(XIX+1234H)
        LD      WA,(034H)
        LD      WA,(01234H)
        LD      WA,(0123456H)
        LD      WA,(XIX+A)

;;; LD: memory <- 16-bit register

        LD      (XIX),WA
        LD      (XIY),WA
        LD      (-XIX),WA
        LD      (XIX+),WA
        LD      (XIX+20H),WA
        LD      (XIX+1234H),WA
        LD      (034H),WA
        LD      (01234H),WA
        LD      (0123456H),WA
        LD      (XIX+A),WA

;;; LD: 32-bit register <- register (MAX mode)

        MAXMODE ON

        LD      XWA,XWA
        LD      XWA,XBC
        LD      XWA,XDE
        LD      XWA,XHL
        LD      XWA,XIX
        LD      XWA,XIY
        LD      XWA,XIZ
        LD      XWA,XSP
        LD      XIX,XWA
        LD      XIY,XWA
        LD      XIZ,XWA
        LD      XSP,XWA

;;; LD: 32-bit register <- immediate

        LD      XWA,012345678H
        LD      XBC,012345678H
        LD      XIX,012345678H
        LD      XIY,012345678H

;;; LD: 32-bit register <- memory

        LD      XWA,(XIX)
        LD      XWA,(-XIX)
        LD      XWA,(XIX+)
        LD      XWA,(XIX+20H)
        LD      XWA,(XIX+1234H)
        LD      XWA,(034H)
        LD      XWA,(01234H)
        LD      XWA,(0123456H)
        LD      XWA,(XIX+A)

;;; LD: memory <- 32-bit register

        LD      (XIX),XWA
        LD      (-XIX),XWA
        LD      (XIX+),XWA
        LD      (XIX+20H),XWA
        LD      (XIX+1234H),XWA
        LD      (034H),XWA
        LD      (01234H),XWA
        LD      (0123456H),XWA
        LD      (XIX+A),XWA


;;; LD: absolute addressing (16-bit/24-bit absolute <- mem and vice versa)

        LD      (01234H),(056H)
        LD      (01234H),(0ABCDH)

;;; PUSH / POP

        PUSH    WA
        PUSH    BC
        PUSH    DE
        PUSH    HL
        PUSH    IX
        PUSH    IY
        PUSH    IZ
        PUSH    SP
        PUSH    F
        PUSH    034H
        PUSH    (XIX)
        PUSH    SR

        POP     WA
        POP     BC
        POP     DE
        POP     HL
        POP     IX
        POP     IY
        POP     IZ
        POP     SP
        POP     F
        POP     (XIX)
        POP     SR

        MAXMODE ON
        PUSH    XWA
        PUSH    XBC
        PUSH    XIX
        POP     XWA
        POP     XIX

;;; LDA

        LDA     WA,(XIX)
        LDA     WA,(XIX+20H)
        LDA     WA,(XIX+1234H)
        LDA     WA,(034H)
        LDA     WA,(01234H)
        LDA     WA,(0123456H)
        LDA     WA,(XIX+A)
        LDA     IX,(XIX+20H)
        MAXMODE ON
        LDA     XIX,(XIY)
        LDA     XIX,(XIY+20H)
        LDA     XIX,(XIY+1234H)
        LDA     XIX,(034H)
        LDA     XIX,(01234H)
        LDA     XIX,(0123456H)
        LDA     XIX,(XIY+A)

;;; LDAR

        LDAR    WA,$
        LDAR    WA,$+7FFFH
        LDAR    IX,$
        MAXMODE ON
        LDAR    XIX,$
        LDAR    XIX,$+7FFFH

;;; EX

        EX      F,F'
        EX      WA,BC
        EX      BC,WA
        EX      WA,(XIX)
        EX      WA,(XIX+20H)
        EX      WA,(034H)
        EX      WA,(01234H)
        EX      WA,(0123456H)

;;; MIRR (word only)

        MIRR    WA
        MIRR    BC
        MIRR    HL

;;; Block transfer instructions

        LDI
        LDIR
        LDD
        LDDR
        CPI
        CPIR
        CPD
        CPDR

;;; ADD / ADC / SUB / SBC / AND / OR / XOR / CP

        ADD     A,A
        ADD     A,B
        ADD     A,034H
        ADD     A,(XIX)
        ADD     A,(XIX+20H)
        ADD     A,(034H)
        ADD     A,(01234H)
        ADD     A,(0123456H)
        ADD     A,(XIX+A)

        ADD     WA,WA
        ADD     WA,BC
        ADD     WA,01234H
        ADD     WA,(XIX)
        ADD     WA,(XIX+20H)
        ADD     WA,(034H)
        ADD     WA,(01234H)
        ADD     WA,(0123456H)
        ADD     WA,(XIX+A)

        MAXMODE ON
        ADD     XWA,XWA
        ADD     XWA,XBC
        ADD     XWA,012345678H
        ADD     XWA,(XIX)
        ADD     XWA,(XIX+20H)
        ADD     XWA,(034H)
        ADD     XWA,(01234H)
        ADD     XWA,(0123456H)
        ADD     XWA,(XIX+A)

        ADC     A,A
        ADC     A,034H
        ADC     A,(XIX)
        ADC     WA,WA
        ADC     WA,01234H
        ADC     WA,(XIX)

        SUB     A,A
        SUB     A,034H
        SUB     A,(XIX)
        SUB     WA,WA
        SUB     WA,01234H
        SUB     WA,(XIX)

        SBC     A,A
        SBC     A,034H
        SBC     A,(XIX)
        SBC     WA,WA
        SBC     WA,01234H
        SBC     WA,(XIX)

        AND     A,A
        AND     A,034H
        AND     A,(XIX)
        AND     WA,WA
        AND     WA,01234H
        AND     WA,(XIX)

        OR      A,A
        OR      A,034H
        OR      A,(XIX)
        OR      WA,WA
        OR      WA,01234H
        OR      WA,(XIX)

        XOR     A,A
        XOR     A,034H
        XOR     A,(XIX)
        XOR     WA,WA
        XOR     WA,01234H
        XOR     WA,(XIX)

        CP      A,A
        CP      A,034H
        CP      A,(XIX)
        CP      WA,WA
        CP      WA,01234H
        CP      WA,(XIX)
        CP      (XIX),034H

;;; INC / DEC

        INC     1,A
        INC     2,A
        INC     4,A
        INC     1,WA
        INC     2,WA
        INC     4,WA
        INC     1,(XIX)
        INC     2,(XIX)
        INC     4,(XIX)
        INC     1,(XIX+20H)
        INC     1,(034H)
        INC     1,(01234H)
        INC     1,(0123456H)

        DEC     1,A
        DEC     2,A
        DEC     4,A
        DEC     1,WA
        DEC     2,WA
        DEC     4,WA
        DEC     1,(XIX)
        DEC     2,(XIX)
        DEC     4,(XIX)
        DEC     1,(XIX+20H)
        DEC     1,(034H)

        MAXMODE ON
        INC     1,XWA
        INC     2,XWA
        INC     4,XWA
        DEC     1,XWA

;;; MUL / MULS / DIV / DIVS

        MUL     WA,A
        MUL     WA,B
        MUL     WA,034H
        MUL     WA,(XIX)
        MUL     WA,(XIX+20H)
        MUL     WA,(034H)

        MULS    WA,034H
        MULS    WA,(XIX)

        DIV     WA,A
        DIV     WA,B
        DIV     WA,(XIX)
        DIV     WA,(034H)

        DIVS    WA,A
        DIVS    WA,(XIX)

;;; MULA (32-bit register only)

        MAXMODE ON
        MULA    XWA
        MULA    XIX

;;; MINC / MDEC (word operand, buffer size must be power of 2)

;;;     MINC1   04H,WA
;;;     MINC1   04H,BC
;;;     MINC2   04H,WA
;;;     MINC4   08H,WA
;;;     MDEC1   04H,WA
;;;     MDEC2   04H,WA
;;;     MDEC4   08H,WA

;;; NEG / CPL

        NEG     A
        NEG     B
        NEG     WA
        NEG     BC

        CPL     A
        CPL     B
        CPL     WA
        CPL     BC

;;; EXTZ / EXTS (word or longword)

        EXTZ    WA
        EXTZ    BC
        EXTS    WA
        EXTS    BC
        MAXMODE ON
        EXTZ    XWA
        EXTS    XWA
        EXTZ    XIX
        EXTS    XIX

;;; DAA (byte only)

        DAA     A
        DAA     B

;;; PAA (word or longword)

        PAA     WA
        PAA     BC
        MAXMODE ON
        PAA     XWA
        PAA     XIX

;;; BS1F / BS1B (word register)

        BS1F    A,WA
        BS1F    A,BC
        BS1B    A,WA
        BS1B    A,BC

;;; Bit operations

        LDCF    3,A
        LDCF    3,B
        LDCF    3,(XIX)
        LDCF    3,(XIX+20H)
        LDCF    3,(034H)
        LDCF    3,(01234H)

        STCF    3,A
        STCF    3,B
        STCF    3,(XIX)

        ANDCF   3,A
        ANDCF   3,(XIX)
        ANDCF   3,(034H)

        ORCF    3,A
        ORCF    3,(XIX)

        XORCF   3,A
        XORCF   3,(XIX)

        BIT     3,A
        BIT     3,B
        BIT     3,(XIX)
        BIT     3,(XIX+20H)
        BIT     3,(034H)
        BIT     3,(01234H)
        BIT     3,(0123456H)

        RES     3,A
        RES     3,B
        RES     3,(XIX)
        RES     3,(XIX+20H)
        RES     3,(034H)
        RES     3,(01234H)
        RES     3,(0123456H)

        SET     3,A
        SET     3,B
        SET     3,(XIX)
        SET     3,(XIX+20H)
        SET     3,(034H)
        SET     3,(01234H)
        SET     3,(0123456H)

        CHG     3,A
        CHG     3,B
        CHG     3,(XIX)
        CHG     3,(XIX+20H)
        CHG     3,(034H)
        CHG     3,(01234H)
        CHG     3,(0123456H)

        TSET    3,A
        TSET    3,B
        TSET    3,(XIX)
        TSET    3,(XIX+20H)
        TSET    3,(034H)
        TSET    3,(01234H)
        TSET    3,(0123456H)

;;; Rotate / Shift

        RLC     1,A
        RLC     1,WA
        RRC     1,A
        RRC     1,WA
        RL      1,A
        RL      1,WA
        RR      1,A
        RR      1,WA
        SLA     1,A
        SLA     1,WA
        SRA     1,A
        SRA     1,WA
        SLL     1,A
        SLL     1,WA
        SRL     1,A
        SRL     1,WA

        RLC     2,A
        RLC     2,WA
        RRC     2,WA
        RL      2,WA
        RR      2,WA
        SLA     2,A
        SLA     2,WA

        RLD     A,(XIX)
        RLD     A,(034H)
        RLD     A,(01234H)

        RRD     A,(XIX)
        RRD     A,(034H)
        RRD     A,(01234H)

;;; SCC

        SCC     F,A
        SCC     LT,A
        SCC     LE,A
        SCC     ULE,A
        SCC     OV,A
        SCC     MI,A
        SCC     Z,A
        SCC     C,A
        SCC     T,A
        SCC     GE,A
        SCC     GT,A
        SCC     UGT,A
        SCC     NOV,A
        SCC     PL,A
        SCC     NZ,A
        SCC     NC,A

        SCC     F,WA
        SCC     T,WA
        SCC     Z,WA
        SCC     NZ,WA

;;; JP / JR / JRL / CALL / CALR

        JR      F,$+2
        JR      LT,$+2
        JR      LE,$+2
        JR      ULE,$+2
        JR      OV,$+2
        JR      MI,$+2
        JR      Z,$+2
        JR      C,$+2
        JR      T,$+2
        JR      GE,$+2
        JR      GT,$+2
        JR      UGT,$+2
        JR      NOV,$+2
        JR      PL,$+2
        JR      NZ,$+2
        JR      NC,$+2

        JRL     T,$+3
        JRL     Z,$+3
        JRL     C,$+3
        JRL     NZ,$+3

        JP      T,(XIX)
        JP      T,(XIY)
        JP      T,(034H)
        JP      T,(01234H)
        JP      T,(0123456H)
        JP      T,(XIX+20H)
        JP      T,(XIX+1234H)
        JP      Z,(XIX)
        JP      C,(XIX)
        JP      NZ,(XIX)
        JP      NC,(XIX)

        CALR    $+3

        CALL    T,(XIX)
        CALL    T,(034H)
        CALL    T,(01234H)
        CALL    T,(0123456H)
        CALL    Z,(XIX)
        CALL    C,(XIX)
        CALL    NZ,(XIX)

;;; DJNZ

        DJNZ    $+2
        DJNZ    WA,$+2
        DJNZ    BC,$+2
        DJNZ    IX,$+2

;;; RET / RETD / RETI

        RET     F
        RET     LT
        RET     LE
        RET     ULE
        RET     OV
        RET     MI
        RET     Z
        RET     C
        RET     T
        RET     GE
        RET     GT
        RET     UGT
        RET     NOV
        RET     PL
        RET     NZ
        RET     NC

        RETD    0
        RETD    01234H

;;; LINK / UNLK (longword register)

        MAXMODE ON
        LINK    XIX,-4
        LINK    XIY,0
        UNLK    XIX
        UNLK    XIY

;;; LDF / INCF / DECF

        LDF     0
        LDF     3
        INCF
        DECF

;;; MINC/MDEC (modulo increment/decrement; buffer size must be power of 2)

        MINC1   2,BC
        MINC1   4,DE
        MINC1   8,HL
        MINC2   4,BC
        MINC2   8,WA
        MINC4   8,BC
        MINC4   16,DE
        MDEC1   2,BC
        MDEC2   4,DE
        MDEC4   8,HL

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
