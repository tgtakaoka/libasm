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

;;; SH-2E FPU-related CPU instructions (manual table 5.10): LDS/STS to the FPU
;;; system registers FPUL/FPSCR. They are System Control (CPU) instructions by
;;; encoding but access FPU registers, so they require the FPU (mandatory on the
;;; SH-2E, .fpu on for the SH-2A).

;;; LDS Rm,FPUL (m format: 0100mmmm01011010)
        LDS     R0,FPUL                 ;# 405A
        LDS     R15,FPUL                ;# 4F5A

;;; LDS Rm,FPSCR (m format: 0100mmmm01101010)
        LDS     R0,FPSCR                ;# 406A
        LDS     R15,FPSCR               ;# 4F6A

;;; STS FPUL,Rn (n format: 0000nnnn01011010)
        STS     FPUL,R0                 ;# 005A
        STS     FPUL,R15                ;# 0F5A

;;; STS FPSCR,Rn (n format: 0000nnnn01101010)
        STS     FPSCR,R0                ;# 006A
        STS     FPSCR,R15               ;# 0F6A

;;; LDS.L @Rm+,FPUL (m format: 0100mmmm01010110)
        LDS.L   @R0+,FPUL               ;# 4056
        LDS.L   @R15+,FPUL              ;# 4F56

;;; LDS.L @Rm+,FPSCR (m format: 0100mmmm01100110)
        LDS.L   @R0+,FPSCR              ;# 4066
        LDS.L   @R15+,FPSCR             ;# 4F66

;;; STS.L FPUL,@-Rn (n format: 0100nnnn01010010)
        STS.L   FPUL,@-R0               ;# 4052
        STS.L   FPUL,@-R15              ;# 4F52

;;; STS.L FPSCR,@-Rn (n format: 0100nnnn01100010)
        STS.L   FPSCR,@-R0              ;# 4062
        STS.L   FPSCR,@-R15             ;# 4F62

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
