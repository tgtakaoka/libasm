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

;;; SH-2A 32-bit @(disp12,Rn) instruction forms. These are NOT assembled yet
;;; (no table rows), so this file is intentionally not included by any wrapper;
;;; it records the expected encodings as a reference for future work.

;;; MOV.B Rm,@(disp,Rn) (12-bit disp): 0011nnnnmmmm0001 0001dddddddddddd
        MOV.B   R0,@(H'FFF,R1)          ;# 31010FFF

;;; MOV.B @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 0100dddddddddddd
        MOV.B   @(0,R0),R1              ;# 31014000

;;; MOV.W Rm,@(disp,Rn) (12-bit disp): 0011nnnnmmmm0001 0010dddddddddddd
;;; MOV.W @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 0101dddddddddddd
        MOV.W   @(0,R0),R1              ;# 31015000

;;; MOVU.B @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 1000dddddddddddd
        MOVU.B  @(0,R0),R1              ;# 31018000

;;; MOVU.W @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 1001dddddddddddd
        MOVU.W  @(0,R0),R1              ;# 31019000

;;; FMOV.S @(disp,Rm),FRn (12-bit disp): 0011nnnnmmmm0001 0111dddddddddddd
        FMOV.S  @(0,R0),FR1             ;# 31017000

;;; Bit-manipulation memory variants: 0011nnnn00001001 OPiiidddddddddddd
;;; BCLR.B #imm3,@(disp,Rn)
        BCLR.B  #0,@(0,R0)              ;# 30090000
;;; BSET.B #imm3,@(disp,Rn)
        BSET.B  #0,@(0,R0)              ;# 30091000
;;; BST.B #imm3,@(disp,Rn)
        BST.B   #0,@(0,R0)              ;# 30092000
;;; BLD.B #imm3,@(disp,Rn)
        BLD.B   #0,@(0,R0)              ;# 30093000
;;; BAND.B #imm3,@(disp,Rn)
        BAND.B  #0,@(0,R0)              ;# 30094000
;;; BOR.B #imm3,@(disp,Rn)
        BOR.B   #0,@(0,R0)              ;# 30095000
;;; BXOR.B #imm3,@(disp,Rn)
        BXOR.B  #0,@(0,R0)              ;# 30096000
;;; BLDNOT.B #imm3,@(disp,Rn)
        BLDNOT.B #0,@(0,R0)             ;# 3009B000
;;; BANDNOT.B #imm3,@(disp,Rn)
        BANDNOT.B #0,@(0,R0)            ;# 3009C000
;;; BORNOT.B #imm3,@(disp,Rn)
        BORNOT.B #0,@(0,R0)             ;# 3009D000

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
