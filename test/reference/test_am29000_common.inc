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

;;; Instructions common to every Am29000 family member.  The M bit of the
;;; operation code selects between a register operand (M=0) and an 8-bit
;;; constant (M=1), so each three-operand instruction is exercised both ways.

;;; Integer Arithmetic
        ADD     gr96, gr97, gr98
        ADD     gr96, gr97, 15
        ADDS    gr96, gr97, gr98
        ADDS    gr96, gr97, 15
        ADDU    gr96, gr97, gr98
        ADDU    gr96, gr97, 15
        ADDC    gr96, gr97, gr98
        ADDC    gr96, gr97, 15
        ADDCS   gr96, gr97, gr98
        ADDCS   gr96, gr97, 15
        ADDCU   gr96, gr97, gr98
        ADDCU   gr96, gr97, 15
        SUB     gr96, gr97, gr98
        SUB     gr96, gr97, 15
        SUBS    gr96, gr97, gr98
        SUBS    gr96, gr97, 15
        SUBU    gr96, gr97, gr98
        SUBU    gr96, gr97, 15
        SUBC    gr96, gr97, gr98
        SUBC    gr96, gr97, 15
        SUBCS   gr96, gr97, gr98
        SUBCS   gr96, gr97, 15
        SUBCU   gr96, gr97, gr98
        SUBCU   gr96, gr97, 15
        SUBR    gr96, gr97, gr98
        SUBR    gr96, gr97, 15
        SUBRS   gr96, gr97, gr98
        SUBRS   gr96, gr97, 15
        SUBRU   gr96, gr97, gr98
        SUBRU   gr96, gr97, 15
        SUBRC   gr96, gr97, gr98
        SUBRC   gr96, gr97, 15
        SUBRCS  gr96, gr97, gr98
        SUBRCS  gr96, gr97, 15
        SUBRCU  gr96, gr97, gr98
        SUBRCU  gr96, gr97, 15

;;; Multiply and divide steps
        MUL     gr96, gr97, gr98
        MUL     gr96, gr97, 15
        MULL    gr96, gr97, gr98
        MULL    gr96, gr97, 15
        MULU    gr96, gr97, gr98
        MULU    gr96, gr97, 15
        DIV0    gr96, gr98
        DIV0    gr96, 15
        DIV     gr96, gr97, gr98
        DIV     gr96, gr97, 15
        DIVL    gr96, gr97, gr98
        DIVL    gr96, gr97, 15
        DIVREM  gr96, gr97, gr98
        DIVREM  gr96, gr97, 15

;;; Compare
        CPEQ    gr96, gr97, gr98
        CPEQ    gr96, gr97, 15
        CPNEQ   gr96, gr97, gr98
        CPNEQ   gr96, gr97, 15
        CPLT    gr96, gr97, gr98
        CPLT    gr96, gr97, 15
        CPLTU   gr96, gr97, gr98
        CPLTU   gr96, gr97, 15
        CPLE    gr96, gr97, gr98
        CPLE    gr96, gr97, 15
        CPLEU   gr96, gr97, gr98
        CPLEU   gr96, gr97, 15
        CPGT    gr96, gr97, gr98
        CPGT    gr96, gr97, 15
        CPGTU   gr96, gr97, gr98
        CPGTU   gr96, gr97, 15
        CPGE    gr96, gr97, gr98
        CPGE    gr96, gr97, 15
        CPGEU   gr96, gr97, gr98
        CPGEU   gr96, gr97, 15
        CPBYTE  gr96, gr97, gr98
        CPBYTE  gr96, gr97, 15

;;; Assert.  A vector number below 64 is reserved for the operating system,
;;; so the user-mode forms tested here use vector numbers 64 and above.
        ASEQ    64, gr97, gr98
        ASEQ    64, gr97, 15
        ASNEQ   65, gr97, gr98
        ASNEQ   65, gr97, 15
        ASLT    66, gr97, gr98
        ASLT    66, gr97, 15
        ASLTU   67, gr97, gr98
        ASLTU   67, gr97, 15
        ASLE    68, gr97, gr98
        ASLE    68, gr97, 15
        ASLEU   69, gr97, gr98
        ASLEU   69, gr97, 15
        ASGT    70, gr97, gr98
        ASGT    70, gr97, 15
        ASGTU   71, gr97, gr98
        ASGTU   71, gr97, 15
        ASGE    72, gr97, gr98
        ASGE    72, gr97, 15
        ASGEU   255, gr97, gr98
        ASGEU   255, gr97, 15

;;; Logical
        AND     gr96, gr97, gr98
        AND     gr96, gr97, 15
        ANDN    gr96, gr97, gr98
        ANDN    gr96, gr97, 15
        NAND    gr96, gr97, gr98
        NAND    gr96, gr97, 15
        OR      gr96, gr97, gr98
        OR      gr96, gr97, 15
        NOR     gr96, gr97, gr98
        NOR     gr96, gr97, 15
        XOR     gr96, gr97, gr98
        XOR     gr96, gr97, 15
        XNOR    gr96, gr97, gr98
        XNOR    gr96, gr97, 15

;;; Shift
        SLL     gr96, gr97, gr98
        SLL     gr96, gr97, 31
        SRL     gr96, gr97, gr98
        SRL     gr96, gr97, 31
        SRA     gr96, gr97, gr98
        SRA     gr96, gr97, 31
        EXTRACT gr96, gr97, gr98
        EXTRACT gr96, gr97, 31

;;; Data Movement: load and store.  Only the Am29050 has a coprocessor
;;; interface, so on every other family member the first operand is a
;;; reserved bit which must be zero and the manuals spell it as a literal 0.
;;; The second operand is the CNTL access control field.
        LOAD    0, 0, gr96, gr97
        LOAD    0, 0, gr96, 15
        LOAD    0, 0x40, gr96, gr97
        LOADL   0, 0, gr96, gr97
        LOADL   0, 0, gr96, 15
        LOADSET 0, 0, gr96, gr97
        LOADSET 0, 0, gr96, 15
        LOADM   0, 0, gr96, gr97
        LOADM   0, 0, gr96, 15
        STORE   0, 0, gr96, gr97
        STORE   0, 0, gr96, 15
        STORE   0, 0x40, gr96, gr97
        STOREL  0, 0, gr96, gr97
        STOREL  0, 0, gr96, 15
        STOREM  0, 0, gr96, gr97
        STOREM  0, 0, gr96, 15

;;; Data Movement: byte and half-word
        EXBYTE  gr96, gr97, gr98
        EXBYTE  gr96, gr97, 15
        EXHW    gr96, gr97, gr98
        EXHW    gr96, gr97, 15
        EXHWS   gr96, gr97
        INBYTE  gr96, gr97, gr98
        INBYTE  gr96, gr97, 15
        INHW    gr96, gr97, gr98
        INHW    gr96, gr97, 15

;;; Data Movement: special-purpose registers
        MFSR    gr96, VAB
        MFSR    gr96, OPS
        MFSR    gr96, CPS
        MFSR    gr96, CFG
        MFSR    gr96, CHA
        MFSR    gr96, CHD
        MFSR    gr96, CHC
        MFSR    gr96, RBP
        MFSR    gr96, TMC
        MFSR    gr96, TMR
        MFSR    gr96, PC0
        MFSR    gr96, PC1
        MFSR    gr96, PC2
        MFSR    gr96, IPC
        MFSR    gr96, IPA
        MFSR    gr96, IPB
        MFSR    gr96, Q
        MFSR    gr96, ALU
        MFSR    gr96, BP
        MFSR    gr96, FC
        MFSR    gr96, CR
        MTSR    VAB, gr96
        MTSR    OPS, gr96
        MTSR    CPS, gr96
        MTSR    CFG, gr96
        MTSR    CHA, gr96
        MTSR    CHD, gr96
        MTSR    CHC, gr96
        MTSR    RBP, gr96
        MTSR    TMC, gr96
        MTSR    TMR, gr96
        MTSR    PC0, gr96
        MTSR    PC1, gr96
        MTSR    PC2, gr96
        MTSR    IPC, gr96
        MTSR    IPA, gr96
        MTSR    IPB, gr96
        MTSR    Q, gr96
        MTSR    ALU, gr96
        MTSR    BP, gr96
        MTSR    FC, gr96
        MTSR    CR, gr96
        MTSRIM  Q, 0x1234
        MTSRIM  CR, 0

;;; Data Movement: translation look-aside buffer
        MFTLB   gr96, gr97
        MTTLB   gr96, gr97

;;; Branch.  Targets are absolute addresses; the A bit of the operation code
;;; selects Program-Counter relative (A=0) or absolute (A=1) encoding.
back:
        JMP     back
        JMPI    gr98
        CALL    gr96, back
        CALLI   gr96, gr98
        JMPT    gr97, back
        JMPTI   gr97, gr98
        JMPF    gr97, back
        JMPFI   gr97, gr98
        JMPFDEC gr97, back

;;; Miscellaneous
        CLZ     gr96, gr98
        CLZ     gr96, 15
        SETIP   gr96, gr97, gr98
        EMULATE 64, gr97, gr98
        INV
        IRET
        IRETINV
        HALT

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
