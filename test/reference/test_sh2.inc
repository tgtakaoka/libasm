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

;;; SH-2 additional instructions (not present in SH-1)

;;; BF/S label (d8 format: branch if T=0 with delay slot)
;;; d = (N - 4) / 2, position-independent
        BF/S    *+4                     ; 8F00
        BF/S    *+6                     ; 8F01
        BF/S    *+H'84                  ; 8F40
        BF/S    *-2                     ; 8FFD

;;; BT/S label (d8 format: branch if T=1 with delay slot)
        BT/S    *+4                     ; 8D00
        BT/S    *+6                     ; 8D01
        BT/S    *+H'84                  ; 8D40
        BT/S    *-2                     ; 8DFD

;;; BRAF Rm (m format: branch far via register, PC-relative indirect, delay slot)
        BRAF    R0                      ; 0023
        BRAF    R7                      ; 0723
        BRAF    R15                     ; 0F23

;;; BSRF Rm (m format: branch to subroutine far via register, delay slot)
        BSRF    R0                      ; 0003
        BSRF    R7                      ; 0703
        BSRF    R15                     ; 0F03

;;; DMULS.L Rm,Rn (nm format: 32x32 signed multiply to 64-bit MACH:MACL)
        DMULS.L R0,R1                   ; 310D
        DMULS.L R14,R15                 ; 3FED

;;; DMULU.L Rm,Rn (nm format: 32x32 unsigned multiply to 64-bit MACH:MACL)
        DMULU.L R0,R1                   ; 3105
        DMULU.L R14,R15                 ; 3FE5

;;; DT Rn (n format: decrement and test)
        DT      R0                      ; 4010
        DT      R7                      ; 4710
        DT      R15                     ; 4F10

;;; MAC.L @Rm+,@Rn+ (nm format: 32x32 multiply-accumulate with post-increment)
        MAC.L   @R0+,@R1+               ; 010F
        MAC.L   @R14+,@R15+             ; 0FEF

;;; MUL.L Rm,Rn (nm format: 32x32 multiply to MACL)
        MUL.L   R0,R1                   ; 0107
        MUL.L   R14,R15                 ; 0FE7

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
