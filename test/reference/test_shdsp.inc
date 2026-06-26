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

;;; SH-DSP additional instructions (DSP register transfers, repeat control)

;;; LDS Rm,DSR (m format: 0100mmmm 01101010) load DSP status register
        LDS     R0,DSR                  ;# 406A
        LDS     R7,DSR                  ;# 476A
        LDS     R15,DSR                 ;# 4F6A

;;; LDS Rm,A0 (m format: 0100mmmm 01111010) load DSP accumulator A0 (low 32 bits)
        LDS     R0,A0                   ;# 407A
        LDS     R7,A0                   ;# 477A
        LDS     R15,A0                  ;# 4F7A

;;; LDS Rm,X0 (m format: 0100mmmm 10001010)
        LDS     R0,X0                   ;# 408A
        LDS     R7,X0                   ;# 478A
        LDS     R15,X0                  ;# 4F8A

;;; LDS Rm,X1 (m format: 0100mmmm 10011010)
        LDS     R0,X1                   ;# 409A
        LDS     R7,X1                   ;# 479A
        LDS     R15,X1                  ;# 4F9A

;;; LDS Rm,Y0 (m format: 0100mmmm 10101010)
        LDS     R0,Y0                   ;# 40AA
        LDS     R7,Y0                   ;# 47AA
        LDS     R15,Y0                  ;# 4FAA

;;; LDS Rm,Y1 (m format: 0100mmmm 10111010)
        LDS     R0,Y1                   ;# 40BA
        LDS     R7,Y1                   ;# 47BA
        LDS     R15,Y1                  ;# 4FBA

;;; LDS.L @Rm+,DSR (m format: 0100mmmm 01100110)
        LDS.L   @R0+,DSR                ;# 4066
        LDS.L   @R7+,DSR                ;# 4766
        LDS.L   @R15+,DSR               ;# 4F66

;;; LDS.L @Rm+,A0 (m format: 0100mmmm 01110110)
        LDS.L   @R0+,A0                 ;# 4076
        LDS.L   @R7+,A0                 ;# 4776
        LDS.L   @R15+,A0                ;# 4F76

;;; LDS.L @Rm+,X0 (m format: 0100mmmm 10000110)
        LDS.L   @R0+,X0                 ;# 4086
        LDS.L   @R7+,X0                 ;# 4786
        LDS.L   @R15+,X0                ;# 4F86

;;; LDS.L @Rm+,X1 (m format: 0100mmmm 10010110)
        LDS.L   @R0+,X1                 ;# 4096
        LDS.L   @R7+,X1                 ;# 4796
        LDS.L   @R15+,X1                ;# 4F96

;;; LDS.L @Rm+,Y0 (m format: 0100mmmm 10100110)
        LDS.L   @R0+,Y0                 ;# 40A6
        LDS.L   @R7+,Y0                 ;# 47A6
        LDS.L   @R15+,Y0                ;# 4FA6

;;; LDS.L @Rm+,Y1 (m format: 0100mmmm 10110110)
        LDS.L   @R0+,Y1                 ;# 40B6
        LDS.L   @R7+,Y1                 ;# 47B6
        LDS.L   @R15+,Y1                ;# 4FB6

;;; STS DSR,Rn (n format: 0000nnnn 01101010)
        STS     DSR,R0                  ;# 006A
        STS     DSR,R7                  ;# 076A
        STS     DSR,R15                 ;# 0F6A

;;; STS A0,Rn (n format: 0000nnnn 01111010)
        STS     A0,R0                   ;# 007A
        STS     A0,R7                   ;# 077A
        STS     A0,R15                  ;# 0F7A

;;; STS X0,Rn (n format: 0000nnnn 10001010)
        STS     X0,R0                   ;# 008A
        STS     X0,R7                   ;# 078A
        STS     X0,R15                  ;# 0F8A

;;; STS X1,Rn (n format: 0000nnnn 10011010)
        STS     X1,R0                   ;# 009A
        STS     X1,R7                   ;# 079A
        STS     X1,R15                  ;# 0F9A

;;; STS Y0,Rn (n format: 0000nnnn 10101010)
        STS     Y0,R0                   ;# 00AA
        STS     Y0,R7                   ;# 07AA
        STS     Y0,R15                  ;# 0FAA

;;; STS Y1,Rn (n format: 0000nnnn 10111010)
        STS     Y1,R0                   ;# 00BA
        STS     Y1,R7                   ;# 07BA
        STS     Y1,R15                  ;# 0FBA

;;; STS.L DSR,@-Rn (n format: 0100nnnn 01100010)
        STS.L   DSR,@-R0                ;# 4062
        STS.L   DSR,@-R7                ;# 4762
        STS.L   DSR,@-R15               ;# 4F62

;;; STS.L A0,@-Rn (n format: 0100nnnn 01110010)
        STS.L   A0,@-R0                 ;# 4072
        STS.L   A0,@-R7                 ;# 4772
        STS.L   A0,@-R15                ;# 4F72

;;; STS.L X0,@-Rn (n format: 0100nnnn 10000010)
        STS.L   X0,@-R0                 ;# 4082
        STS.L   X0,@-R7                 ;# 4782
        STS.L   X0,@-R15                ;# 4F82

;;; STS.L X1,@-Rn (n format: 0100nnnn 10010010)
        STS.L   X1,@-R0                 ;# 4092
        STS.L   X1,@-R7                 ;# 4792
        STS.L   X1,@-R15                ;# 4F92

;;; STS.L Y0,@-Rn (n format: 0100nnnn 10100010)
        STS.L   Y0,@-R0                 ;# 40A2
        STS.L   Y0,@-R7                 ;# 47A2
        STS.L   Y0,@-R15                ;# 4FA2

;;; STS.L Y1,@-Rn (n format: 0100nnnn 10110010)
        STS.L   Y1,@-R0                 ;# 40B2
        STS.L   Y1,@-R7                 ;# 47B2
        STS.L   Y1,@-R15                ;# 4FB2

;;; LDC Rm,MOD (m format: 0100mmmm 01011110) load modulo register
        LDC     R0,MOD                  ;# 405E
        LDC     R7,MOD                  ;# 475E
        LDC     R15,MOD                 ;# 4F5E

;;; LDC Rm,RS (m format: 0100mmmm 01101110) load repeat start
        LDC     R0,RS                   ;# 406E
        LDC     R7,RS                   ;# 476E
        LDC     R15,RS                  ;# 4F6E

;;; LDC Rm,RE (m format: 0100mmmm 01111110) load repeat end
        LDC     R0,RE                   ;# 407E
        LDC     R7,RE                   ;# 477E
        LDC     R15,RE                  ;# 4F7E

;;; LDC.L @Rm+,MOD (m format: 0100mmmm 01010111)
        LDC.L   @R0+,MOD                ;# 4057
        LDC.L   @R7+,MOD                ;# 4757
        LDC.L   @R15+,MOD               ;# 4F57

;;; LDC.L @Rm+,RS (m format: 0100mmmm 01100111)
        LDC.L   @R0+,RS                 ;# 4067
        LDC.L   @R7+,RS                 ;# 4767
        LDC.L   @R15+,RS                ;# 4F67

;;; LDC.L @Rm+,RE (m format: 0100mmmm 01110111)
        LDC.L   @R0+,RE                 ;# 4077
        LDC.L   @R7+,RE                 ;# 4777
        LDC.L   @R15+,RE                ;# 4F77

;;; STC MOD,Rn (n format: 0000nnnn 01010010)
        STC     MOD,R0                  ;# 0052
        STC     MOD,R7                  ;# 0752
        STC     MOD,R15                 ;# 0F52

;;; STC RS,Rn (n format: 0000nnnn 01100010)
        STC     RS,R0                   ;# 0062
        STC     RS,R7                   ;# 0762
        STC     RS,R15                  ;# 0F62

;;; STC RE,Rn (n format: 0000nnnn 01110010)
        STC     RE,R0                   ;# 0072
        STC     RE,R7                   ;# 0772
        STC     RE,R15                  ;# 0F72

;;; STC.L MOD,@-Rn (n format: 0100nnnn 01010011)
        STC.L   MOD,@-R0                ;# 4053
        STC.L   MOD,@-R7                ;# 4753
        STC.L   MOD,@-R15               ;# 4F53

;;; STC.L RS,@-Rn (n format: 0100nnnn 01100011)
        STC.L   RS,@-R0                 ;# 4063
        STC.L   RS,@-R7                 ;# 4763
        STC.L   RS,@-R15                ;# 4F63

;;; STC.L RE,@-Rn (n format: 0100nnnn 01110011)
        STC.L   RE,@-R0                 ;# 4073
        STC.L   RE,@-R7                 ;# 4773
        STC.L   RE,@-R15                ;# 4F73

;;; LDRS @(disp,PC) (d8 format: 1000 1100 dddd dddd) load repeat start
;;; d = (target - (PC+4)) / 2; target should be 2-byte aligned
        LDRS    $+4               ;# 8C00
        LDRS    $+H'40            ;# 8C1E

;;; LDRE @(disp,PC) (d8 format: 1000 1110 dddd dddd) load repeat end
        LDRE    $+4               ;# 8E00
        LDRE    $+H'40            ;# 8E1E

;;; SETRC #imm (i8 format: 1000 0010 iiii iiii) set repeat count to immediate
        SETRC   #0                      ;# 8200
        SETRC   #H'7F                   ;# 827F
        SETRC   #H'FF                   ;# 82FF

;;; SETRC Rm (m format: 0100mmmm 00010100) set repeat count from register
        SETRC   R0                      ;# 4014
        SETRC   R7                      ;# 4714
        SETRC   R15                     ;# 4F14

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
