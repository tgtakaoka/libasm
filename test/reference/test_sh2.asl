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

        cpu     SH7600
        org     0

;;; Data Transfer

;;; MOV #imm,Rn (1110 nnnn iiii iiii)
        MOV     #0,R0                   ;;; E000
        MOV     #127,R1                 ;;; E17F
        MOV     #-128,R2                ;;; E280
        MOV     #-1,R15                 ;;; EFFF

;;; MOV Rm,Rn (0110 nnnn mmmm 0011)
        MOV     R0,R1                   ;;; 6103
        MOV     R14,R15                 ;;; 6FE3

;;; MOV.W @(disp,PC),Rn (1001 nnnn dddd dddd)
;;; note: ASL encodes d = (current_addr + N) / 2 (absolute); GAS is authoritative
        MOV.W   @(*+4,PC),R0            ;;; 9008
        MOV.W   @(*+6,PC),R3            ;;; 930A
        MOV.W   @(*+8,PC),R7            ;;; 970C

;;; MOV.L @(disp,PC),Rn (1101 nnnn dddd dddd) -- instruction must be 4-byte aligned
        align   4
        MOV.L   @(*+4,PC),R0            ;;; D006
        align   4
        MOV.L   @(*+8,PC),R5            ;;; D508

;;; MOVA @(disp,PC),R0 (1100 0111 dddd dddd) -- instruction must be 4-byte aligned
        align   4
        MOVA    @(*+4,PC),R0            ;;; C708

;;; MOV.B @Rm,Rn (0110 nnnn mmmm 0000)
        MOV.B   @R1,R0                  ;;; 6010
        MOV.B   @R3,R2                  ;;; 6230

;;; MOV.W @Rm,Rn (0110 nnnn mmmm 0001)
        MOV.W   @R5,R4                  ;;; 6451
        MOV.W   @R7,R6                  ;;; 6671

;;; MOV.L @Rm,Rn (0110 nnnn mmmm 0010)
        MOV.L   @R9,R8                  ;;; 6892
        MOV.L   @R11,R10                ;;; 6AB2

;;; MOV.B Rm,@Rn (0010 nnnn mmmm 0000)
        MOV.B   R0,@R1                  ;;; 2100
        MOV.B   R2,@R3                  ;;; 2320

;;; MOV.W Rm,@Rn (0010 nnnn mmmm 0001)
        MOV.W   R4,@R5                  ;;; 2541
        MOV.W   R6,@R7                  ;;; 2761

;;; MOV.L Rm,@Rn (0010 nnnn mmmm 0010)
        MOV.L   R8,@R9                  ;;; 2982
        MOV.L   R10,@R11                ;;; 2BA2

;;; MOV.B @Rm+,Rn (0110 nnnn mmmm 0100)
        MOV.B   @R1+,R0                 ;;; 6014
        MOV.B   @R3+,R2                 ;;; 6234

;;; MOV.W @Rm+,Rn (0110 nnnn mmmm 0101)
        MOV.W   @R5+,R4                 ;;; 6455
        MOV.W   @R7+,R6                 ;;; 6675

;;; MOV.L @Rm+,Rn (0110 nnnn mmmm 0110)
        MOV.L   @R9+,R8                 ;;; 6896
        MOV.L   @R11+,R10               ;;; 6AB6

;;; MOV.B Rm,@-Rn (0010 nnnn mmmm 0100)
        MOV.B   R0,@-R1                 ;;; 2104
        MOV.B   R2,@-R3                 ;;; 2324

;;; MOV.W Rm,@-Rn (0010 nnnn mmmm 0101)
        MOV.W   R4,@-R5                 ;;; 2545
        MOV.W   R6,@-R7                 ;;; 2765

;;; MOV.L Rm,@-Rn (0010 nnnn mmmm 0110)
        MOV.L   R8,@-R9                 ;;; 2986
        MOV.L   R10,@-R11               ;;; 2BA6

;;; MOV.B @(R0,Rm),Rn (0000 nnnn mmmm 1100)
        MOV.B   @(R0,R1),R2             ;;; 021C
        MOV.B   @(R0,R13),R14           ;;; 0EDC

;;; MOV.W @(R0,Rm),Rn (0000 nnnn mmmm 1101)
        MOV.W   @(R0,R3),R4             ;;; 043D
        MOV.W   @(R0,R12),R15           ;;; 0FCD

;;; MOV.L @(R0,Rm),Rn (0000 nnnn mmmm 1110)
        MOV.L   @(R0,R5),R6             ;;; 065E
        MOV.L   @(R0,R11),R13           ;;; 0DBE

;;; MOV.B Rm,@(R0,Rn) (0000 nnnn mmmm 0100)
        MOV.B   R0,@(R0,R2)             ;;; 0204
        MOV.B   R7,@(R0,R15)            ;;; 0F74

;;; MOV.W Rm,@(R0,Rn) (0000 nnnn mmmm 0101)
        MOV.W   R0,@(R0,R4)             ;;; 0405
        MOV.W   R8,@(R0,R14)            ;;; 0E85

;;; MOV.L Rm,@(R0,Rn) (0000 nnnn mmmm 0110)
        MOV.L   R0,@(R0,R6)             ;;; 0606
        MOV.L   R9,@(R0,R13)            ;;; 0D96

;;; MOV.B R0,@(disp,Rn) (1000 0000 nnnn dddd) disp in bytes [0,15]
        MOV.B   R0,@(0,R0)              ;;; 8000
        MOV.B   R0,@(7,R4)              ;;; 8047
        MOV.B   R0,@(15,R15)            ;;; 80FF

;;; MOV.W R0,@(disp,Rn) (1000 0001 nnnn dddd) disp in bytes/2 [0,30]
        MOV.W   R0,@(0,R0)              ;;; 8100
        MOV.W   R0,@(14,R5)             ;;; 8157
        MOV.W   R0,@(30,R15)            ;;; 81FF

;;; MOV.B @(disp,Rm),R0 (1000 0100 mmmm dddd) disp in bytes [0,15]
        MOV.B   @(0,R0),R0              ;;; 8400
        MOV.B   @(7,R6),R0              ;;; 8467
        MOV.B   @(15,R15),R0            ;;; 84FF

;;; MOV.W @(disp,Rm),R0 (1000 0101 mmmm dddd) disp in bytes/2 [0,30]
        MOV.W   @(0,R0),R0              ;;; 8500
        MOV.W   @(14,R7),R0             ;;; 8577
        MOV.W   @(30,R15),R0            ;;; 85FF

;;; MOV.L Rm,@(disp,Rn) (0001 nnnn mmmm dddd) disp in bytes/4 [0,60]
        MOV.L   R0,@(0,R1)              ;;; 1100
        MOV.L   R5,@(8,R6)              ;;; 1652
        MOV.L   R14,@(60,R15)           ;;; 1FEF

;;; MOV.L @(disp,Rm),Rn (0101 nnnn mmmm dddd) disp in bytes/4 [0,60]
        MOV.L   @(0,R0),R1              ;;; 5100
        MOV.L   @(8,R7),R8              ;;; 5872
        MOV.L   @(60,R14),R15           ;;; 5FEF

;;; MOV.B R0,@(disp,GBR) (1100 0000 dddd dddd) disp = byte offset [0,255]
        MOV.B   R0,@(0,GBR)             ;;; C000
        MOV.B   R0,@(128,GBR)           ;;; C080
        MOV.B   R0,@(255,GBR)           ;;; C0FF

;;; MOV.W R0,@(disp,GBR) (1100 0001 dddd dddd) disp = byte offset/2 [0,510]
        MOV.W   R0,@(0,GBR)             ;;; C100
        MOV.W   R0,@(256,GBR)           ;;; C180
        MOV.W   R0,@(510,GBR)           ;;; C1FF

;;; MOV.L R0,@(disp,GBR) (1100 0010 dddd dddd) disp = byte offset/4 [0,1020]
        MOV.L   R0,@(0,GBR)             ;;; C200
        MOV.L   R0,@(512,GBR)           ;;; C280
        MOV.L   R0,@(1020,GBR)          ;;; C2FF

;;; MOV.B @(disp,GBR),R0 (1100 0100 dddd dddd)
        MOV.B   @(0,GBR),R0             ;;; C400
        MOV.B   @(128,GBR),R0           ;;; C480
        MOV.B   @(255,GBR),R0           ;;; C4FF

;;; MOV.W @(disp,GBR),R0 (1100 0101 dddd dddd)
        MOV.W   @(0,GBR),R0             ;;; C500
        MOV.W   @(256,GBR),R0           ;;; C580
        MOV.W   @(510,GBR),R0           ;;; C5FF

;;; MOV.L @(disp,GBR),R0 (1100 0110 dddd dddd)
        MOV.L   @(0,GBR),R0             ;;; C600
        MOV.L   @(512,GBR),R0           ;;; C680
        MOV.L   @(1020,GBR),R0          ;;; C6FF

;;; MOVT Rn (0000 nnnn 0010 1001)
        MOVT    R0                      ;;; 0029
        MOVT    R7                      ;;; 0729
        MOVT    R15                     ;;; 0F29

;;; Arithmetic

;;; ADD Rm,Rn (0011 nnnn mmmm 1100)
        ADD     R0,R1                   ;;; 310C
        ADD     R14,R15                 ;;; 3FEC

;;; ADD #imm,Rn (0111 nnnn iiii iiii) 8-bit signed
        ADD     #0,R0                   ;;; 7000
        ADD     #1,R1                   ;;; 7101
        ADD     #127,R7                 ;;; 777F
        ADD     #-1,R8                  ;;; 78FF
        ADD     #-128,R15               ;;; 7F80

;;; ADDC Rm,Rn (0011 nnnn mmmm 1110)
        ADDC    R0,R1                   ;;; 310E
        ADDC    R14,R15                 ;;; 3FEE

;;; ADDV Rm,Rn (0011 nnnn mmmm 1111)
        ADDV    R0,R1                   ;;; 310F
        ADDV    R14,R15                 ;;; 3FEF

;;; SUB Rm,Rn (0011 nnnn mmmm 1000)
        SUB     R0,R1                   ;;; 3108
        SUB     R14,R15                 ;;; 3FE8

;;; SUBC Rm,Rn (0011 nnnn mmmm 1010)
        SUBC    R0,R1                   ;;; 310A
        SUBC    R14,R15                 ;;; 3FEA

;;; SUBV Rm,Rn (0011 nnnn mmmm 1011)
        SUBV    R0,R1                   ;;; 310B
        SUBV    R14,R15                 ;;; 3FEB

;;; CMP/EQ #imm,R0 (1000 1000 iiii iiii)
        CMP/EQ  #0,R0                   ;;; 8800
        CMP/EQ  #127,R0                 ;;; 887F
        CMP/EQ  #-1,R0                  ;;; 88FF

;;; CMP/EQ Rm,Rn (0011 nnnn mmmm 0000)
        CMP/EQ  R0,R1                   ;;; 3100
        CMP/EQ  R14,R15                 ;;; 3FE0

;;; CMP/GE Rm,Rn (0011 nnnn mmmm 0011) signed >=
        CMP/GE  R0,R1                   ;;; 3103
        CMP/GE  R14,R15                 ;;; 3FE3

;;; CMP/GT Rm,Rn (0011 nnnn mmmm 0111) signed >
        CMP/GT  R0,R1                   ;;; 3107
        CMP/GT  R14,R15                 ;;; 3FE7

;;; CMP/HI Rm,Rn (0011 nnnn mmmm 0110) unsigned >
        CMP/HI  R0,R1                   ;;; 3106
        CMP/HI  R14,R15                 ;;; 3FE6

;;; CMP/HS Rm,Rn (0011 nnnn mmmm 0010) unsigned >=
        CMP/HS  R0,R1                   ;;; 3102
        CMP/HS  R14,R15                 ;;; 3FE2

;;; CMP/PL Rn (0100 nnnn 0001 0101)
        CMP/PL  R0                      ;;; 4015
        CMP/PL  R7                      ;;; 4715
        CMP/PL  R15                     ;;; 4F15

;;; CMP/PZ Rn (0100 nnnn 0001 0001)
        CMP/PZ  R0                      ;;; 4011
        CMP/PZ  R7                      ;;; 4711
        CMP/PZ  R15                     ;;; 4F11

;;; CMP/STR Rm,Rn (0010 nnnn mmmm 1100) any byte equal
        CMP/STR R0,R1                   ;;; 210C
        CMP/STR R14,R15                 ;;; 2FEC

;;; DIV0S Rm,Rn (0010 nnnn mmmm 0111)
        DIV0S   R0,R1                   ;;; 2107
        DIV0S   R14,R15                 ;;; 2FE7

;;; DIV0U (0000 0000 0001 1001)
        DIV0U                           ;;; 0019

;;; DIV1 Rm,Rn (0011 nnnn mmmm 0100)
        DIV1    R0,R1                   ;;; 3104
        DIV1    R14,R15                 ;;; 3FE4

;;; EXTS.B Rm,Rn (0110 nnnn mmmm 1110)
        EXTS.B  R0,R1                   ;;; 610E
        EXTS.B  R14,R15                 ;;; 6FEE

;;; EXTS.W Rm,Rn (0110 nnnn mmmm 1111)
        EXTS.W  R0,R1                   ;;; 610F
        EXTS.W  R14,R15                 ;;; 6FEF

;;; EXTU.B Rm,Rn (0110 nnnn mmmm 1100)
        EXTU.B  R0,R1                   ;;; 610C
        EXTU.B  R14,R15                 ;;; 6FEC

;;; EXTU.W Rm,Rn (0110 nnnn mmmm 1101)
        EXTU.W  R0,R1                   ;;; 610D
        EXTU.W  R14,R15                 ;;; 6FED

;;; DMULS.L Rm,Rn (0011 nnnn mmmm 1101) SH-2: 32x32 signed multiply -> 64-bit
        DMULS.L R0,R1                   ;;; 310D
        DMULS.L R14,R15                 ;;; 3FED

;;; DMULU.L Rm,Rn (0011 nnnn mmmm 0101) SH-2: 32x32 unsigned multiply -> 64-bit
        DMULU.L R0,R1                   ;;; 3105
        DMULU.L R14,R15                 ;;; 3FE5

;;; MAC.L @Rm+,@Rn+ (0000 nnnn mmmm 1111) SH-2: 32x32 multiply-accumulate
        MAC.L   @R0+,@R1+               ;;; 010F
        MAC.L   @R14+,@R15+             ;;; 0FEF

;;; MAC.W @Rm+,@Rn+ (0100 nnnn mmmm 1111) 16x16 multiply-accumulate
        MAC.W   @R0+,@R1+               ;;; 410F
        MAC.W   @R14+,@R15+             ;;; 4FEF

;;; MULS.W Rm,Rn (0010 nnnn mmmm 1111)
        MULS.W  R0,R1                   ;;; 210F
        MULS.W  R14,R15                 ;;; 2FEF

;;; MULU.W Rm,Rn (0010 nnnn mmmm 1110)
        MULU.W  R0,R1                   ;;; 210E
        MULU.W  R14,R15                 ;;; 2FEE

;;; NEG Rm,Rn (0110 nnnn mmmm 1011)
        NEG     R0,R1                   ;;; 610B
        NEG     R14,R15                 ;;; 6FEB

;;; NEGC Rm,Rn (0110 nnnn mmmm 1010)
        NEGC    R0,R1                   ;;; 610A
        NEGC    R14,R15                 ;;; 6FEA

;;; SWAP.B Rm,Rn (0110 nnnn mmmm 1000)
        SWAP.B  R0,R1                   ;;; 6108
        SWAP.B  R14,R15                 ;;; 6FE8

;;; SWAP.W Rm,Rn (0110 nnnn mmmm 1001)
        SWAP.W  R0,R1                   ;;; 6109
        SWAP.W  R14,R15                 ;;; 6FE9

;;; XTRCT Rm,Rn (0010 nnnn mmmm 1101)
        XTRCT   R0,R1                   ;;; 210D
        XTRCT   R14,R15                 ;;; 2FED

;;; Logic

;;; AND Rm,Rn (0010 nnnn mmmm 1001)
        AND     R0,R1                   ;;; 2109
        AND     R14,R15                 ;;; 2FE9

;;; AND #imm,R0 (1100 1001 iiii iiii)
        AND     #0,R0                   ;;; C900
        AND     #$AA,R0                 ;;; C9AA
        AND     #255,R0                 ;;; C9FF

;;; AND.B #imm,@(R0,GBR) (1100 1101 iiii iiii)
        AND.B   #0,@(R0,GBR)            ;;; CD00
        AND.B   #$AA,@(R0,GBR)          ;;; CDAA
        AND.B   #255,@(R0,GBR)          ;;; CDFF

;;; NOT Rm,Rn (0110 nnnn mmmm 0111)
        NOT     R0,R1                   ;;; 6107
        NOT     R14,R15                 ;;; 6FE7

;;; OR Rm,Rn (0010 nnnn mmmm 1011)
        OR      R0,R1                   ;;; 210B
        OR      R14,R15                 ;;; 2FEB

;;; OR #imm,R0 (1100 1011 iiii iiii)
        OR      #0,R0                   ;;; CB00
        OR      #$55,R0                 ;;; CB55
        OR      #255,R0                 ;;; CBFF

;;; OR.B #imm,@(R0,GBR) (1100 1111 iiii iiii)
        OR.B    #0,@(R0,GBR)            ;;; CF00
        OR.B    #$55,@(R0,GBR)          ;;; CF55
        OR.B    #255,@(R0,GBR)          ;;; CFFF

;;; TAS.B @Rn (0100 nnnn 0001 1011)
        TAS.B   @R0                     ;;; 401B
        TAS.B   @R7                     ;;; 471B
        TAS.B   @R15                    ;;; 4F1B

;;; TST Rm,Rn (0010 nnnn mmmm 1000)
        TST     R0,R1                   ;;; 2108
        TST     R14,R15                 ;;; 2FE8

;;; TST #imm,R0 (1100 1000 iiii iiii)
        TST     #0,R0                   ;;; C800
        TST     #255,R0                 ;;; C8FF

;;; TST.B #imm,@(R0,GBR) (1100 1100 iiii iiii)
        TST.B   #0,@(R0,GBR)            ;;; CC00
        TST.B   #255,@(R0,GBR)          ;;; CCFF

;;; XOR Rm,Rn (0010 nnnn mmmm 1010)
        XOR     R0,R1                   ;;; 210A
        XOR     R14,R15                 ;;; 2FEA

;;; XOR #imm,R0 (1100 1010 iiii iiii)
        XOR     #0,R0                   ;;; CA00
        XOR     #15,R0                  ;;; CA0F
        XOR     #255,R0                 ;;; CAFF

;;; XOR.B #imm,@(R0,GBR) (1100 1110 iiii iiii)
        XOR.B   #0,@(R0,GBR)            ;;; CE00
        XOR.B   #15,@(R0,GBR)           ;;; CE0F
        XOR.B   #255,@(R0,GBR)          ;;; CEFF

;;; Shift and Rotate

;;; ROTL Rn (0100 nnnn 0000 0100)
        ROTL    R0                      ;;; 4004
        ROTL    R7                      ;;; 4704
        ROTL    R15                     ;;; 4F04

;;; ROTR Rn (0100 nnnn 0000 0101)
        ROTR    R0                      ;;; 4005
        ROTR    R7                      ;;; 4705
        ROTR    R15                     ;;; 4F05

;;; ROTCL Rn (0100 nnnn 0010 0100)
        ROTCL   R0                      ;;; 4024
        ROTCL   R7                      ;;; 4724
        ROTCL   R15                     ;;; 4F24

;;; ROTCR Rn (0100 nnnn 0010 0101)
        ROTCR   R0                      ;;; 4025
        ROTCR   R7                      ;;; 4725
        ROTCR   R15                     ;;; 4F25

;;; SHAL Rn (0100 nnnn 0010 0000)
        SHAL    R0                      ;;; 4020
        SHAL    R7                      ;;; 4720
        SHAL    R15                     ;;; 4F20

;;; SHAR Rn (0100 nnnn 0010 0001)
        SHAR    R0                      ;;; 4021
        SHAR    R7                      ;;; 4721
        SHAR    R15                     ;;; 4F21

;;; SHLL Rn (0100 nnnn 0000 0000)
        SHLL    R0                      ;;; 4000
        SHLL    R7                      ;;; 4700
        SHLL    R15                     ;;; 4F00

;;; SHLR Rn (0100 nnnn 0000 0001)
        SHLR    R0                      ;;; 4001
        SHLR    R7                      ;;; 4701
        SHLR    R15                     ;;; 4F01

;;; SHLL2 Rn (0100 nnnn 0000 1000)
        SHLL2   R0                      ;;; 4008
        SHLL2   R7                      ;;; 4708
        SHLL2   R15                     ;;; 4F08

;;; SHLR2 Rn (0100 nnnn 0000 1001)
        SHLR2   R0                      ;;; 4009
        SHLR2   R7                      ;;; 4709
        SHLR2   R15                     ;;; 4F09

;;; SHLL8 Rn (0100 nnnn 0001 1000)
        SHLL8   R0                      ;;; 4018
        SHLL8   R7                      ;;; 4718
        SHLL8   R15                     ;;; 4F18

;;; SHLR8 Rn (0100 nnnn 0001 1001)
        SHLR8   R0                      ;;; 4019
        SHLR8   R7                      ;;; 4719
        SHLR8   R15                     ;;; 4F19

;;; SHLL16 Rn (0100 nnnn 0010 1000)
        SHLL16  R0                      ;;; 4028
        SHLL16  R7                      ;;; 4728
        SHLL16  R15                     ;;; 4F28

;;; SHLR16 Rn (0100 nnnn 0010 1001)
        SHLR16  R0                      ;;; 4029
        SHLR16  R7                      ;;; 4729
        SHLR16  R15                     ;;; 4F29

;;; Branch

;;; BF label (1000 1011 dddd dddd)
        BF      *+4                     ;;; 8B00
        BF      *+6                     ;;; 8B01
        BF      *-2                     ;;; 8BFD

;;; BT label (1000 1001 dddd dddd)
        BT      *+4                     ;;; 8900
        BT      *+6                     ;;; 8901
        BT      *-2                     ;;; 89FD

;;; BRA label (1010 dddd dddd dddd) delay slot
        BRA     *+4                     ;;; A000
        NOP
        BRA     *+8                     ;;; A002
        NOP
        BRA     *-2                     ;;; AFFD
        NOP

;;; BSR label (1011 dddd dddd dddd) delay slot
        BSR     *+4                     ;;; B000
        NOP
        BSR     *+8                     ;;; B002
        NOP
        BSR     *-2                     ;;; BFFD
        NOP

;;; JMP @Rm (0100 mmmm 0010 1011) delay slot
        JMP     @R0                     ;;; 402B
        NOP
        JMP     @R7                     ;;; 472B
        NOP
        JMP     @R15                    ;;; 4F2B
        NOP

;;; JSR @Rm (0100 mmmm 0000 1011) delay slot
        JSR     @R0                     ;;; 400B
        NOP
        JSR     @R7                     ;;; 470B
        NOP
        JSR     @R15                    ;;; 4F0B
        NOP

;;; RTS (0000 0000 0000 1011) delay slot
        RTS                             ;;; 000B
        NOP

;;; System Control

;;; CLRMAC (0000 0000 0010 1000)
        CLRMAC                          ;;; 0028

;;; CLRT (0000 0000 0001 0000)
        CLRT                            ;;; 0010

;;; LDC Rm,SR (0100 mmmm 0000 1110)
        LDC     R0,SR                   ;;; 400E
        LDC     R7,SR                   ;;; 470E
        LDC     R15,SR                  ;;; 4F0E

;;; LDC Rm,GBR (0100 mmmm 0001 1110)
        LDC     R0,GBR                  ;;; 401E
        LDC     R7,GBR                  ;;; 471E
        LDC     R15,GBR                 ;;; 4F1E

;;; LDC Rm,VBR (0100 mmmm 0010 1110)
        LDC     R0,VBR                  ;;; 402E
        LDC     R7,VBR                  ;;; 472E
        LDC     R15,VBR                 ;;; 4F2E

;;; LDC.L @Rm+,SR (0100 mmmm 0000 0111)
        LDC.L   @R0+,SR                 ;;; 4007
        LDC.L   @R7+,SR                 ;;; 4707
        LDC.L   @R15+,SR                ;;; 4F07

;;; LDC.L @Rm+,GBR (0100 mmmm 0001 0111)
        LDC.L   @R0+,GBR                ;;; 4017
        LDC.L   @R7+,GBR                ;;; 4717
        LDC.L   @R15+,GBR               ;;; 4F17

;;; LDC.L @Rm+,VBR (0100 mmmm 0010 0111)
        LDC.L   @R0+,VBR                ;;; 4027
        LDC.L   @R7+,VBR                ;;; 4727
        LDC.L   @R15+,VBR               ;;; 4F27

;;; LDS Rm,MACH (0100 mmmm 0000 1010)
        LDS     R0,MACH                 ;;; 400A
        LDS     R7,MACH                 ;;; 470A
        LDS     R15,MACH                ;;; 4F0A

;;; LDS Rm,MACL (0100 mmmm 0001 1010)
        LDS     R0,MACL                 ;;; 401A
        LDS     R7,MACL                 ;;; 471A
        LDS     R15,MACL                ;;; 4F1A

;;; LDS Rm,PR (0100 mmmm 0010 1010)
        LDS     R0,PR                   ;;; 402A
        LDS     R7,PR                   ;;; 472A
        LDS     R15,PR                  ;;; 4F2A

;;; LDS.L @Rm+,MACH (0100 mmmm 0000 0110)
        LDS.L   @R0+,MACH               ;;; 4006
        LDS.L   @R7+,MACH               ;;; 4706
        LDS.L   @R15+,MACH              ;;; 4F06

;;; LDS.L @Rm+,MACL (0100 mmmm 0001 0110)
        LDS.L   @R0+,MACL               ;;; 4016
        LDS.L   @R7+,MACL               ;;; 4716
        LDS.L   @R15+,MACL              ;;; 4F16

;;; LDS.L @Rm+,PR (0100 mmmm 0010 0110)
        LDS.L   @R0+,PR                 ;;; 4026
        LDS.L   @R7+,PR                 ;;; 4726
        LDS.L   @R15+,PR                ;;; 4F26

;;; NOP (0000 0000 0000 1001)
        NOP                             ;;; 0009

;;; RTE (0000 0000 0010 1011) delay slot
        RTE                             ;;; 002B
        NOP

;;; SETT (0000 0000 0001 1000)
        SETT                            ;;; 0018

;;; SLEEP (0000 0000 0001 1011)
        SLEEP                           ;;; 001B

;;; STC SR,Rn (0000 nnnn 0000 0010)
        STC     SR,R0                   ;;; 0002
        STC     SR,R7                   ;;; 0702
        STC     SR,R15                  ;;; 0F02

;;; STC GBR,Rn (0000 nnnn 0001 0010)
        STC     GBR,R0                  ;;; 0012
        STC     GBR,R7                  ;;; 0712
        STC     GBR,R15                 ;;; 0F12

;;; STC VBR,Rn (0000 nnnn 0010 0010)
        STC     VBR,R0                  ;;; 0022
        STC     VBR,R7                  ;;; 0722
        STC     VBR,R15                 ;;; 0F22

;;; STC.L SR,@-Rn (0100 nnnn 0000 0011)
        STC.L   SR,@-R0                 ;;; 4003
        STC.L   SR,@-R7                 ;;; 4703
        STC.L   SR,@-R15                ;;; 4F03

;;; STC.L GBR,@-Rn (0100 nnnn 0001 0011)
        STC.L   GBR,@-R0                ;;; 4013
        STC.L   GBR,@-R7                ;;; 4713
        STC.L   GBR,@-R15               ;;; 4F13

;;; STC.L VBR,@-Rn (0100 nnnn 0010 0011)
        STC.L   VBR,@-R0                ;;; 4023
        STC.L   VBR,@-R7                ;;; 4723
        STC.L   VBR,@-R15               ;;; 4F23

;;; STS MACH,Rn (0000 nnnn 0000 1010)
        STS     MACH,R0                 ;;; 000A
        STS     MACH,R7                 ;;; 070A
        STS     MACH,R15                ;;; 0F0A

;;; STS MACL,Rn (0000 nnnn 0001 1010)
        STS     MACL,R0                 ;;; 001A
        STS     MACL,R7                 ;;; 071A
        STS     MACL,R15                ;;; 0F1A

;;; STS PR,Rn (0000 nnnn 0010 1010)
        STS     PR,R0                   ;;; 002A
        STS     PR,R7                   ;;; 072A
        STS     PR,R15                  ;;; 0F2A

;;; STS.L MACH,@-Rn (0100 nnnn 0000 0010)
        STS.L   MACH,@-R0               ;;; 4002
        STS.L   MACH,@-R7               ;;; 4702
        STS.L   MACH,@-R15              ;;; 4F02

;;; STS.L MACL,@-Rn (0100 nnnn 0001 0010)
        STS.L   MACL,@-R0               ;;; 4012
        STS.L   MACL,@-R7               ;;; 4712
        STS.L   MACL,@-R15              ;;; 4F12

;;; STS.L PR,@-Rn (0100 nnnn 0010 0010)
        STS.L   PR,@-R0                 ;;; 4022
        STS.L   PR,@-R7                 ;;; 4722
        STS.L   PR,@-R15                ;;; 4F22

;;; TRAPA #imm (1100 0011 iiii iiii)
        TRAPA   #0                      ;;; C300
        TRAPA   #32                     ;;; C320
        TRAPA   #255                    ;;; C3FF

        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
