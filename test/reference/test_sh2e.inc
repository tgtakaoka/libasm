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

;;; SH-2E additional instructions (single-precision FPU)

;;; FADD FRm,FRn (nm format: 1111nnnnmmmm0000)
        FADD    FR0,FR1                 ; F100
        FADD    FR14,FR15               ; FFE0

;;; FSUB FRm,FRn (nm format: 1111nnnnmmmm0001)
        FSUB    FR0,FR1                 ; F101
        FSUB    FR14,FR15               ; FFE1

;;; FMUL FRm,FRn (nm format: 1111nnnnmmmm0010)
        FMUL    FR0,FR1                 ; F102
        FMUL    FR14,FR15               ; FFE2

;;; FDIV FRm,FRn (nm format: 1111nnnnmmmm0011)
        FDIV    FR0,FR1                 ; F103
        FDIV    FR14,FR15               ; FFE3

;;; FCMP/EQ FRm,FRn (nm format: 1111nnnnmmmm0100)
        FCMP/EQ FR0,FR1                 ; F104
        FCMP/EQ FR14,FR15               ; FFE4

;;; FCMP/GT FRm,FRn (nm format: 1111nnnnmmmm0101)
        FCMP/GT FR0,FR1                 ; F105
        FCMP/GT FR14,FR15               ; FFE5

;;; FABS FRn (n format: 1111nnnn01011101)
        FABS    FR0                     ; F05D
        FABS    FR7                     ; F75D
        FABS    FR15                    ; FF5D

;;; FNEG FRn (n format: 1111nnnn01001101)
        FNEG    FR0                     ; F04D
        FNEG    FR7                     ; F74D
        FNEG    FR15                    ; FF4D

;;; FMOV FRm,FRn (nm format: 1111nnnnmmmm1100)
        FMOV    FR0,FR1                 ; F10C
        FMOV    FR14,FR15               ; FFEC

;;; FMOV.S @Rm,FRn (nm format: 1111nnnnmmmm1000)
        FMOV.S  @R0,FR0                 ; F008
        FMOV.S  @R7,FR8                 ; F878
        FMOV.S  @R15,FR15               ; FFF8

;;; FMOV.S FRm,@Rn (nm format: 1111nnnnmmmm1010)
        FMOV.S  FR0,@R0                 ; F00A
        FMOV.S  FR8,@R7                 ; F78A
        FMOV.S  FR15,@R15               ; FFFA

;;; FMOV.S @Rm+,FRn (nm format: 1111nnnnmmmm1001)
        FMOV.S  @R0+,FR0                ; F009
        FMOV.S  @R15+,FR15              ; FFF9

;;; FMOV.S FRm,@-Rn (nm format: 1111nnnnmmmm1011)
        FMOV.S  FR0,@-R0                ; F00B
        FMOV.S  FR15,@-R15              ; FFFB

;;; FMOV.S @(R0,Rm),FRn (nm format: 1111nnnnmmmm0110)
        FMOV.S  @(R0,R1),FR0            ; F016
        FMOV.S  @(R0,R15),FR15          ; FFF6

;;; FMOV.S FRm,@(R0,Rn) (nm format: 1111nnnnmmmm0111)
        FMOV.S  FR0,@(R0,R1)            ; F107
        FMOV.S  FR15,@(R0,R15)          ; FFF7

;;; FLDI0 FRn (n format: 1111nnnn10001101) -- load 0.0
        FLDI0   FR0                     ; F08D
        FLDI0   FR15                    ; FF8D

;;; FLDI1 FRn (n format: 1111nnnn10011101) -- load 1.0
        FLDI1   FR0                     ; F09D
        FLDI1   FR15                    ; FF9D

;;; FLOAT FPUL,FRn (n format: 1111nnnn00101101) -- integer-to-float
        FLOAT   FPUL,FR0                ; F02D
        FLOAT   FPUL,FR15               ; FF2D

;;; FTRC FRm,FPUL (m-in-n format: 1111mmmm00111101) -- float-to-integer truncate
        FTRC    FR0,FPUL                ; F03D
        FTRC    FR15,FPUL               ; FF3D

;;; FMAC FR0,FRm,FRn (nm format: 1111nnnnmmmm1110) -- FR0*FRm + FRn -> FRn
        FMAC    FR0,FR1,FR2             ; F21E
        FMAC    FR0,FR14,FR15           ; FFEE

;;; FLDS FRm,FPUL (m-in-n format: 1111mmmm00011101)
        FLDS    FR0,FPUL                ; F01D
        FLDS    FR15,FPUL               ; FF1D

;;; FSTS FPUL,FRn (n format: 1111nnnn00001101)
        FSTS    FPUL,FR0                ; F00D
        FSTS    FPUL,FR15               ; FF0D

;;; LDS Rm,FPUL (m format: 0100mmmm01011010)
        LDS     R0,FPUL                 ; 405A
        LDS     R15,FPUL                ; 4F5A

;;; LDS Rm,FPSCR (m format: 0100mmmm01101010)
        LDS     R0,FPSCR                ; 406A
        LDS     R15,FPSCR               ; 4F6A

;;; STS FPUL,Rn (n format: 0000nnnn01011010)
        STS     FPUL,R0                 ; 005A
        STS     FPUL,R15                ; 0F5A

;;; STS FPSCR,Rn (n format: 0000nnnn01101010)
        STS     FPSCR,R0                ; 006A
        STS     FPSCR,R15               ; 0F6A

;;; LDS.L @Rm+,FPUL (m format: 0100mmmm01010110)
        LDS.L   @R0+,FPUL               ; 4056
        LDS.L   @R15+,FPUL              ; 4F56

;;; LDS.L @Rm+,FPSCR (m format: 0100mmmm01100110)
        LDS.L   @R0+,FPSCR              ; 4066
        LDS.L   @R15+,FPSCR             ; 4F66

;;; STS.L FPUL,@-Rn (n format: 0100nnnn01010010)
        STS.L   FPUL,@-R0               ; 4052
        STS.L   FPUL,@-R15              ; 4F52

;;; STS.L FPSCR,@-Rn (n format: 0100nnnn01100010)
        STS.L   FPSCR,@-R0              ; 4062
        STS.L   FPSCR,@-R15             ; 4F62

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
