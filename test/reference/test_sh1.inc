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

;;; Data Transfer

;;; MOV #imm,Rn (ni format)
        MOV     #0,R0                   ;# E000
        MOV     #H'7F,R1                ;# E17F
        MOV     #-H'80,R2               ;# E280
        MOV     #-1,R15                 ;# EFFF

;;; MOV Rm,Rn (nm format: register to register)
        MOV     R0,R1                   ;# 6103
        MOV     R14,R15                 ;# 6FE3

;;; MOV.W @(disp,PC),Rn (nd8 format: PC-relative word load)
;;; d = (N - 4) / 2, position-independent
        MOV.W   $+4,R0            ;# 9000
        MOV.W   $+8,R3            ;# 9302
        MOV.W   $+H'40,R7         ;# 971E

;;; MOV.L @(disp,PC),Rn (nd8 format: PC-relative longword load)
;;; d = (target - ((PC+4) & ~3)) / 4; target must be 4-byte aligned
        MOV.L   $+6,R0            ;# D001
        MOV.L   $+8,R5            ;# D501
        MOV.L   $+H'42,R9         ;# D910

;;; MOV.B @Rm,Rn (nm format: register-indirect byte load)
        MOV.B   @R1,R0                  ;# 6010
        MOV.B   @R3,R2                  ;# 6230

;;; MOV.W @Rm,Rn (nm format: register-indirect word load)
        MOV.W   @R5,R4                  ;# 6451
        MOV.W   @R7,R6                  ;# 6671

;;; MOV.L @Rm,Rn (nm format: register-indirect longword load)
        MOV.L   @R9,R8                  ;# 6892
        MOV.L   @R11,R10                ;# 6AB2

;;; MOV.B Rm,@Rn (nm format: register-indirect byte store)
        MOV.B   R0,@R1                  ;# 2100
        MOV.B   R2,@R3                  ;# 2320

;;; MOV.W Rm,@Rn (nm format: register-indirect word store)
        MOV.W   R4,@R5                  ;# 2541
        MOV.W   R6,@R7                  ;# 2761

;;; MOV.L Rm,@Rn (nm format: register-indirect longword store)
        MOV.L   R8,@R9                  ;# 2982
        MOV.L   R10,@R11                ;# 2BA2

;;; MOV.B @Rm+,Rn (nm format: post-increment byte load)
        MOV.B   @R1+,R0                 ;# 6014
        MOV.B   @R3+,R2                 ;# 6234

;;; MOV.W @Rm+,Rn (nm format: post-increment word load)
        MOV.W   @R5+,R4                 ;# 6455
        MOV.W   @R7+,R6                 ;# 6675

;;; MOV.L @Rm+,Rn (nm format: post-increment longword load)
        MOV.L   @R9+,R8                 ;# 6896
        MOV.L   @R11+,R10               ;# 6AB6

;;; MOV.B Rm,@-Rn (nm format: pre-decrement byte store)
        MOV.B   R0,@-R1                 ;# 2104
        MOV.B   R2,@-R3                 ;# 2324

;;; MOV.W Rm,@-Rn (nm format: pre-decrement word store)
        MOV.W   R4,@-R5                 ;# 2545
        MOV.W   R6,@-R7                 ;# 2765

;;; MOV.L Rm,@-Rn (nm format: pre-decrement longword store)
        MOV.L   R8,@-R9                 ;# 2986
        MOV.L   R10,@-R11               ;# 2BA6

;;; MOV.B @(R0,Rm),Rn (nm format: R0-indexed byte load)
        MOV.B   @(R0,R1),R2             ;# 021C
        MOV.B   @(R0,R13),R14           ;# 0EDC

;;; MOV.W @(R0,Rm),Rn (nm format: R0-indexed word load)
        MOV.W   @(R0,R3),R4             ;# 043D
        MOV.W   @(R0,R12),R15           ;# 0FCD

;;; MOV.L @(R0,Rm),Rn (nm format: R0-indexed longword load)
        MOV.L   @(R0,R5),R6             ;# 065E
        MOV.L   @(R0,R11),R13           ;# 0DBE

;;; MOV.B Rm,@(R0,Rn) (nm format: R0-indexed byte store)
        MOV.B   R0,@(R0,R2)             ;# 0204
        MOV.B   R7,@(R0,R15)            ;# 0F74

;;; MOV.W Rm,@(R0,Rn) (nm format: R0-indexed word store)
        MOV.W   R0,@(R0,R4)             ;# 0405
        MOV.W   R8,@(R0,R14)            ;# 0E85

;;; MOV.L Rm,@(R0,Rn) (nm format: R0-indexed longword store)
        MOV.L   R0,@(R0,R6)             ;# 0606
        MOV.L   R9,@(R0,R13)            ;# 0D96

;;; MOV.B R0,@(disp,Rn) (nd4 format: displaced byte store)
        MOV.B   R0,@(0,R0)              ;# 8000
        MOV.B   R0,@(7,R4)              ;# 8047
        MOV.B   R0,@(15,R15)            ;# 80FF

;;; MOV.W R0,@(disp,Rn) (nd4 format: displaced word store, disp scaled by 2)
        MOV.W   R0,@(0,R0)              ;# 8100
        MOV.W   R0,@(14,R5)             ;# 8157
        MOV.W   R0,@(30,R15)            ;# 81FF

;;; MOV.B @(disp,Rm),R0 (md format: displaced byte load)
        MOV.B   @(0,R0),R0              ;# 8400
        MOV.B   @(7,R6),R0              ;# 8467
        MOV.B   @(15,R15),R0            ;# 84FF

;;; MOV.W @(disp,Rm),R0 (md format: displaced word load, disp scaled by 2)
        MOV.W   @(0,R0),R0              ;# 8500
        MOV.W   @(14,R7),R0             ;# 8577
        MOV.W   @(30,R15),R0            ;# 85FF

;;; MOV.L Rm,@(disp,Rn) (nmd format: displaced longword store, disp scaled by 4)
        MOV.L   R0,@(0,R1)              ;# 1100
        MOV.L   R5,@(8,R6)              ;# 1652
        MOV.L   R14,@(60,R15)           ;# 1FEF

;;; MOV.L @(disp,Rm),Rn (nmd format: displaced longword load, disp scaled by 4)
        MOV.L   @(0,R0),R1              ;# 5100
        MOV.L   @(8,R7),R8              ;# 5872
        MOV.L   @(60,R14),R15           ;# 5FEF

;;; MOV.B R0,@(disp,GBR) (d8 format: GBR-relative byte store)
        MOV.B   R0,@(0,GBR)             ;# C000
        MOV.B   R0,@(H'80,GBR)          ;# C080
        MOV.B   R0,@(H'FF,GBR)          ;# C0FF

;;; MOV.W R0,@(disp,GBR) (d8 format: GBR-relative word store, disp scaled by 2)
        MOV.W   R0,@(0,GBR)             ;# C100
        MOV.W   R0,@(H'100,GBR)         ;# C180
        MOV.W   R0,@(H'1FE,GBR)         ;# C1FF

;;; MOV.L R0,@(disp,GBR) (d8 format: GBR-relative longword store, disp scaled by 4)
        MOV.L   R0,@(0,GBR)             ;# C200
        MOV.L   R0,@(H'200,GBR)         ;# C280
        MOV.L   R0,@(H'3FC,GBR)         ;# C2FF

;;; MOV.B @(disp,GBR),R0 (d8 format: GBR-relative byte load)
        MOV.B   @(0,GBR),R0             ;# C400
        MOV.B   @(H'80,GBR),R0          ;# C480
        MOV.B   @(H'FF,GBR),R0          ;# C4FF

;;; MOV.W @(disp,GBR),R0 (d8 format: GBR-relative word load, disp scaled by 2)
        MOV.W   @(0,GBR),R0             ;# C500
        MOV.W   @(H'100,GBR),R0         ;# C580
        MOV.W   @(H'1FE,GBR),R0         ;# C5FF

;;; MOV.L @(disp,GBR),R0 (d8 format: GBR-relative longword load, disp scaled by 4)
        MOV.L   @(0,GBR),R0             ;# C600
        MOV.L   @(H'200,GBR),R0         ;# C680
        MOV.L   @(H'3FC,GBR),R0         ;# C6FF

;;; MOVA @(disp,PC),R0 (d8 format: compute PC-relative longword-aligned address)
;;; d = (target - ((PC+4) & ~3)) / 4; target must be 4-byte aligned
        MOVA    $+4,R0             ;# C700
        MOVA    $+6,R0             ;# C701
        MOVA    $+H'40,R0          ;# C70F

;;; MOVT Rn (n format: move T bit to register)
        MOVT    R0                       ;# 0029
        MOVT    R7                       ;# 0729
        MOVT    R15                      ;# 0F29

;;; Arithmetic

;;; ADD Rm,Rn (nm format)
        ADD     R0,R1                   ;# 310C
        ADD     R14,R15                 ;# 3FEC

;;; ADD #imm,Rn (ni format: 8-bit signed immediate)
        ADD     #0,R0                   ;# 7000
        ADD     #1,R1                   ;# 7101
        ADD     #H'7F,R7                ;# 777F
        ADD     #-1,R8                  ;# 78FF
        ADD     #-H'80,R15              ;# 7F80

;;; ADDC Rm,Rn (nm format: add with carry)
        ADDC    R0,R1                   ;# 310E
        ADDC    R14,R15                 ;# 3FEE

;;; ADDV Rm,Rn (nm format: add with overflow check)
        ADDV    R0,R1                   ;# 310F
        ADDV    R14,R15                 ;# 3FEF

;;; SUB Rm,Rn (nm format)
        SUB     R0,R1                   ;# 3108
        SUB     R14,R15                 ;# 3FE8

;;; SUBC Rm,Rn (nm format: subtract with carry)
        SUBC    R0,R1                   ;# 310A
        SUBC    R14,R15                 ;# 3FEA

;;; SUBV Rm,Rn (nm format: subtract with overflow check)
        SUBV    R0,R1                   ;# 310B
        SUBV    R14,R15                 ;# 3FEB

;;; CMP/EQ #imm,R0 (i8 format: compare immediate with R0)
        CMP/EQ  #0,R0                   ;# 8800
        CMP/EQ  #H'7F,R0                ;# 887F
        CMP/EQ  #-1,R0                  ;# 88FF

;;; CMP/EQ Rm,Rn (nm format)
        CMP/EQ  R0,R1                   ;# 3100
        CMP/EQ  R14,R15                 ;# 3FE0

;;; CMP/GE Rm,Rn (nm format: signed >=)
        CMP/GE  R0,R1                   ;# 3103
        CMP/GE  R14,R15                 ;# 3FE3

;;; CMP/GT Rm,Rn (nm format: signed >)
        CMP/GT  R0,R1                   ;# 3107
        CMP/GT  R14,R15                 ;# 3FE7

;;; CMP/HI Rm,Rn (nm format: unsigned >)
        CMP/HI  R0,R1                   ;# 3106
        CMP/HI  R14,R15                 ;# 3FE6

;;; CMP/HS Rm,Rn (nm format: unsigned >=)
        CMP/HS  R0,R1                   ;# 3102
        CMP/HS  R14,R15                 ;# 3FE2

;;; CMP/PL Rn (n format: Rn > 0)
        CMP/PL  R0                      ;# 4015
        CMP/PL  R7                      ;# 4715
        CMP/PL  R15                     ;# 4F15

;;; CMP/PZ Rn (n format: Rn >= 0)
        CMP/PZ  R0                      ;# 4011
        CMP/PZ  R7                      ;# 4711
        CMP/PZ  R15                     ;# 4F11

;;; CMP/STR Rm,Rn (nm format: any byte equal)
        CMP/STR R0,R1                   ;# 210C
        CMP/STR R14,R15                 ;# 2FEC

;;; DIV0S Rm,Rn (nm format: initialize signed division)
        DIV0S   R0,R1                   ;# 2107
        DIV0S   R14,R15                 ;# 2FE7

;;; DIV0U (0 format: initialize unsigned division)
        DIV0U                           ;# 0019

;;; DIV1 Rm,Rn (nm format: single-step division)
        DIV1    R0,R1                   ;# 3104
        DIV1    R14,R15                 ;# 3FE4

;;; EXTS.B Rm,Rn (nm format: sign-extend byte)
        EXTS.B  R0,R1                   ;# 610E
        EXTS.B  R14,R15                 ;# 6FEE

;;; EXTS.W Rm,Rn (nm format: sign-extend word)
        EXTS.W  R0,R1                   ;# 610F
        EXTS.W  R14,R15                 ;# 6FEF

;;; EXTU.B Rm,Rn (nm format: zero-extend byte)
        EXTU.B  R0,R1                   ;# 610C
        EXTU.B  R14,R15                 ;# 6FEC

;;; EXTU.W Rm,Rn (nm format: zero-extend word)
        EXTU.W  R0,R1                   ;# 610D
        EXTU.W  R14,R15                 ;# 6FED

;;; MAC.W @Rm+,@Rn+ (nm format: multiply-accumulate word with post-increment)
        MAC.W   @R0+,@R1+               ;# 410F
        MAC.W   @R14+,@R15+             ;# 4FEF

;;; MULS.W Rm,Rn (nm format: signed word multiply)
        MULS.W  R0,R1                   ;# 210F
        MULS.W  R14,R15                 ;# 2FEF

;;; MULU.W Rm,Rn (nm format: unsigned word multiply)
        MULU.W  R0,R1                   ;# 210E
        MULU.W  R14,R15                 ;# 2FEE

;;; NEG Rm,Rn (nm format: negate)
        NEG     R0,R1                   ;# 610B
        NEG     R14,R15                 ;# 6FEB

;;; NEGC Rm,Rn (nm format: negate with carry)
        NEGC    R0,R1                   ;# 610A
        NEGC    R14,R15                 ;# 6FEA

;;; SWAP.B Rm,Rn (nm format: swap bytes in lower word)
        SWAP.B  R0,R1                   ;# 6108
        SWAP.B  R14,R15                 ;# 6FE8

;;; SWAP.W Rm,Rn (nm format: swap upper and lower words)
        SWAP.W  R0,R1                   ;# 6109
        SWAP.W  R14,R15                 ;# 6FE9

;;; XTRCT Rm,Rn (nm format: extract middle 32 bits of 64-bit Rm:Rn)
        XTRCT   R0,R1                   ;# 210D
        XTRCT   R14,R15                 ;# 2FED

;;; Logic

;;; AND Rm,Rn (nm format)
        AND     R0,R1                   ;# 2109
        AND     R14,R15                 ;# 2FE9

;;; AND #imm,R0 (i8 format: logical AND immediate with R0)
        AND     #0,R0                   ;# C900
        AND     #H'AA,R0                ;# C9AA
        AND     #H'FF,R0                ;# C9FF

;;; AND.B #imm,@(R0,GBR) (i8 format: logical AND immediate with @(R0+GBR))
        AND.B   #0,@(R0,GBR)            ;# CD00
        AND.B   #H'AA,@(R0,GBR)         ;# CDAA
        AND.B   #H'FF,@(R0,GBR)         ;# CDFF

;;; NOT Rm,Rn (nm format: bitwise NOT)
        NOT     R0,R1                   ;# 6107
        NOT     R14,R15                 ;# 6FE7

;;; OR Rm,Rn (nm format)
        OR      R0,R1                   ;# 210B
        OR      R14,R15                 ;# 2FEB

;;; OR #imm,R0 (i8 format: logical OR immediate with R0)
        OR      #0,R0                   ;# CB00
        OR      #H'55,R0                ;# CB55
        OR      #H'FF,R0                ;# CBFF

;;; OR.B #imm,@(R0,GBR) (i8 format: logical OR immediate with @(R0+GBR))
        OR.B    #0,@(R0,GBR)            ;# CF00
        OR.B    #H'55,@(R0,GBR)         ;# CF55
        OR.B    #H'FF,@(R0,GBR)         ;# CFFF

;;; TAS.B @Rn (n format: test and set byte)
        TAS.B   @R0                     ;# 401B
        TAS.B   @R7                     ;# 471B
        TAS.B   @R15                    ;# 4F1B

;;; TST Rm,Rn (nm format)
        TST     R0,R1                   ;# 2108
        TST     R14,R15                 ;# 2FE8

;;; TST #imm,R0 (i8 format: logical AND immediate with R0, set T only)
        TST     #0,R0                   ;# C800
        TST     #H'FF,R0                ;# C8FF

;;; TST.B #imm,@(R0,GBR) (i8 format: test @(R0+GBR) with immediate)
        TST.B   #0,@(R0,GBR)            ;# CC00
        TST.B   #H'FF,@(R0,GBR)         ;# CCFF

;;; XOR Rm,Rn (nm format)
        XOR     R0,R1                   ;# 210A
        XOR     R14,R15                 ;# 2FEA

;;; XOR #imm,R0 (i8 format: logical XOR immediate with R0)
        XOR     #0,R0                   ;# CA00
        XOR     #H'0F,R0                ;# CA0F
        XOR     #H'FF,R0                ;# CAFF

;;; XOR.B #imm,@(R0,GBR) (i8 format: logical XOR immediate with @(R0+GBR))
        XOR.B   #0,@(R0,GBR)            ;# CE00
        XOR.B   #H'0F,@(R0,GBR)         ;# CE0F
        XOR.B   #H'FF,@(R0,GBR)         ;# CEFF

;;; Shift and Rotate

;;; ROTL Rn (n format: rotate left through T)
        ROTL    R0                      ;# 4004
        ROTL    R7                      ;# 4704
        ROTL    R15                     ;# 4F04

;;; ROTR Rn (n format: rotate right through T)
        ROTR    R0                      ;# 4005
        ROTR    R7                      ;# 4705
        ROTR    R15                     ;# 4F05

;;; ROTCL Rn (n format: rotate left through carry/T)
        ROTCL   R0                      ;# 4024
        ROTCL   R7                      ;# 4724
        ROTCL   R15                     ;# 4F24

;;; ROTCR Rn (n format: rotate right through carry/T)
        ROTCR   R0                      ;# 4025
        ROTCR   R7                      ;# 4725
        ROTCR   R15                     ;# 4F25

;;; SHAL Rn (n format: arithmetic shift left, same as SHLL)
        SHAL    R0                      ;# 4020
        SHAL    R7                      ;# 4720
        SHAL    R15                     ;# 4F20

;;; SHAR Rn (n format: arithmetic shift right)
        SHAR    R0                      ;# 4021
        SHAR    R7                      ;# 4721
        SHAR    R15                     ;# 4F21

;;; SHLL Rn (n format: logical shift left by 1)
        SHLL    R0                      ;# 4000
        SHLL    R7                      ;# 4700
        SHLL    R15                     ;# 4F00

;;; SHLR Rn (n format: logical shift right by 1)
        SHLR    R0                      ;# 4001
        SHLR    R7                      ;# 4701
        SHLR    R15                     ;# 4F01

;;; SHLL2 Rn (n format: logical shift left by 2)
        SHLL2   R0                      ;# 4008
        SHLL2   R7                      ;# 4708
        SHLL2   R15                     ;# 4F08

;;; SHLR2 Rn (n format: logical shift right by 2)
        SHLR2   R0                      ;# 4009
        SHLR2   R7                      ;# 4709
        SHLR2   R15                     ;# 4F09

;;; SHLL8 Rn (n format: logical shift left by 8)
        SHLL8   R0                      ;# 4018
        SHLL8   R7                      ;# 4718
        SHLL8   R15                     ;# 4F18

;;; SHLR8 Rn (n format: logical shift right by 8)
        SHLR8   R0                      ;# 4019
        SHLR8   R7                      ;# 4719
        SHLR8   R15                     ;# 4F19

;;; SHLL16 Rn (n format: logical shift left by 16)
        SHLL16  R0                      ;# 4028
        SHLL16  R7                      ;# 4728
        SHLL16  R15                     ;# 4F28

;;; SHLR16 Rn (n format: logical shift right by 16)
        SHLR16  R0                      ;# 4029
        SHLR16  R7                      ;# 4729
        SHLR16  R15                     ;# 4F29

;;; Branch

;;; BF label (d8 format: branch if T=0, 8-bit signed PC-relative)
;;; d = (N - 4) / 2, position-independent
        BF      $+4                     ;# 8B00
        BF      $+6                     ;# 8B01
        BF      $+H'84                  ;# 8B40
        BF      $-2                     ;# 8BFD

;;; BT label (d8 format: branch if T=1, 8-bit signed PC-relative)
        BT      $+4                     ;# 8900
        BT      $+6                     ;# 8901
        BT      $+H'84                  ;# 8940
        BT      $-2                     ;# 89FD

;;; BRA label (d12 format: unconditional branch, 12-bit signed PC-relative)
;;; d = (N - 4) / 2, position-independent
        BRA     $+4                     ;# A000
        BRA     $+8                     ;# A002
        BRA     $+H'800                 ;# A3FE
        BRA     $-2                     ;# AFFD

;;; BSR label (d12 format: branch to subroutine, 12-bit signed PC-relative)
        BSR     $+4                     ;# B000
        BSR     $+8                     ;# B002
        BSR     $+H'800                 ;# B3FE

;;; JMP @Rm (m format: unconditional branch to register)
        JMP     @R0                     ;# 402B
        JMP     @R7                     ;# 472B
        JMP     @R15                    ;# 4F2B

;;; JSR @Rm (m format: jump to subroutine via register)
        JSR     @R0                     ;# 400B
        JSR     @R7                     ;# 470B
        JSR     @R15                    ;# 4F0B

;;; RTS (0 format: return from subroutine)
        RTS                             ;# 000B

;;; System Control

;;; CLRMAC (0 format: clear MACH and MACL)
        CLRMAC                          ;# 0028

;;; CLRT (0 format: clear T bit)
        CLRT                            ;# 0010

;;; LDC Rm,SR (m format: load SR from general register)
        LDC     R0,SR                   ;# 400E
        LDC     R7,SR                   ;# 470E
        LDC     R15,SR                  ;# 4F0E

;;; LDC Rm,GBR (m format)
        LDC     R0,GBR                  ;# 401E
        LDC     R7,GBR                  ;# 471E
        LDC     R15,GBR                 ;# 4F1E

;;; LDC Rm,VBR (m format)
        LDC     R0,VBR                  ;# 402E
        LDC     R7,VBR                  ;# 472E
        LDC     R15,VBR                 ;# 4F2E

;;; LDC.L @Rm+,SR (m format: pop SR from stack)
        LDC.L   @R0+,SR                 ;# 4007
        LDC.L   @R7+,SR                 ;# 4707
        LDC.L   @R15+,SR                ;# 4F07

;;; LDC.L @Rm+,GBR
        LDC.L   @R0+,GBR                ;# 4017
        LDC.L   @R7+,GBR                ;# 4717
        LDC.L   @R15+,GBR               ;# 4F17

;;; LDC.L @Rm+,VBR
        LDC.L   @R0+,VBR                ;# 4027
        LDC.L   @R7+,VBR                ;# 4727
        LDC.L   @R15+,VBR               ;# 4F27

;;; LDS Rm,MACH (m format: load MACH from general register)
        LDS     R0,MACH                 ;# 400A
        LDS     R7,MACH                 ;# 470A
        LDS     R15,MACH                ;# 4F0A

;;; LDS Rm,MACL
        LDS     R0,MACL                 ;# 401A
        LDS     R7,MACL                 ;# 471A
        LDS     R15,MACL                ;# 4F1A

;;; LDS Rm,PR
        LDS     R0,PR                   ;# 402A
        LDS     R7,PR                   ;# 472A
        LDS     R15,PR                  ;# 4F2A

;;; LDS.L @Rm+,MACH (m format: pop MACH from stack)
        LDS.L   @R0+,MACH               ;# 4006
        LDS.L   @R7+,MACH               ;# 4706
        LDS.L   @R15+,MACH              ;# 4F06

;;; LDS.L @Rm+,MACL
        LDS.L   @R0+,MACL               ;# 4016
        LDS.L   @R7+,MACL               ;# 4716
        LDS.L   @R15+,MACL              ;# 4F16

;;; LDS.L @Rm+,PR
        LDS.L   @R0+,PR                 ;# 4026
        LDS.L   @R7+,PR                 ;# 4726
        LDS.L   @R15+,PR                ;# 4F26

;;; NOP (0 format: no operation)
        NOP                             ;# 0009

;;; RTE (0 format: return from exception)
        RTE                             ;# 002B

;;; SETT (0 format: set T bit)
        SETT                            ;# 0018

;;; SLEEP (0 format: sleep until interrupt)
        SLEEP                           ;# 001B

;;; STC SR,Rn (n format: store SR to general register)
        STC     SR,R0                   ;# 0002
        STC     SR,R7                   ;# 0702
        STC     SR,R15                  ;# 0F02

;;; STC GBR,Rn
        STC     GBR,R0                  ;# 0012
        STC     GBR,R7                  ;# 0712
        STC     GBR,R15                 ;# 0F12

;;; STC VBR,Rn
        STC     VBR,R0                  ;# 0022
        STC     VBR,R7                  ;# 0722
        STC     VBR,R15                 ;# 0F22

;;; STC.L SR,@-Rn (n format: push SR to stack)
        STC.L   SR,@-R0                 ;# 4003
        STC.L   SR,@-R7                 ;# 4703
        STC.L   SR,@-R15                ;# 4F03

;;; STC.L GBR,@-Rn
        STC.L   GBR,@-R0                ;# 4013
        STC.L   GBR,@-R7                ;# 4713
        STC.L   GBR,@-R15               ;# 4F13

;;; STC.L VBR,@-Rn
        STC.L   VBR,@-R0                ;# 4023
        STC.L   VBR,@-R7                ;# 4723
        STC.L   VBR,@-R15               ;# 4F23

;;; STS MACH,Rn (n format: store MACH to general register)
        STS     MACH,R0                 ;# 000A
        STS     MACH,R7                 ;# 070A
        STS     MACH,R15                ;# 0F0A

;;; STS MACL,Rn
        STS     MACL,R0                 ;# 001A
        STS     MACL,R7                 ;# 071A
        STS     MACL,R15                ;# 0F1A

;;; STS PR,Rn
        STS     PR,R0                   ;# 002A
        STS     PR,R7                   ;# 072A
        STS     PR,R15                  ;# 0F2A

;;; STS.L MACH,@-Rn (n format: push MACH to stack)
        STS.L   MACH,@-R0               ;# 4002
        STS.L   MACH,@-R7               ;# 4702
        STS.L   MACH,@-R15              ;# 4F02

;;; STS.L MACL,@-Rn
        STS.L   MACL,@-R0               ;# 4012
        STS.L   MACL,@-R7               ;# 4712
        STS.L   MACL,@-R15              ;# 4F12

;;; STS.L PR,@-Rn
        STS.L   PR,@-R0                 ;# 4022
        STS.L   PR,@-R7                 ;# 4722
        STS.L   PR,@-R15                ;# 4F22

;;; TRAPA #imm (i8 format: trap to vector)
        TRAPA   #0                      ;# C300
        TRAPA   #H'20                   ;# C320
        TRAPA   #H'FF                   ;# C3FF

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
