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

;;; SH-2A additional instructions (CPU additions and FPU additions FCNVDS/FCNVSD)

;;; ====== 16-bit CPU additions ======

;;; CLIPS.B Rn (n format: 0100nnnn10010001) saturate signed byte
        CLIPS.B R0                      ; 4091
        CLIPS.B R15                     ; 4F91

;;; CLIPS.W Rn (n format: 0100nnnn10010101) saturate signed word
        CLIPS.W R0                      ; 4095
        CLIPS.W R15                     ; 4F95

;;; CLIPU.B Rn (n format: 0100nnnn10000001) saturate unsigned byte
        CLIPU.B R0                      ; 4081
        CLIPU.B R15                     ; 4F81

;;; CLIPU.W Rn (n format: 0100nnnn10000101) saturate unsigned word
        CLIPU.W R0                      ; 4085
        CLIPU.W R15                     ; 4F85

;;; DIVS R0,Rn (n format: 0100nnnn10010100) signed batch divide
        DIVS    R0,R1                   ; 4194
        DIVS    R0,R15                  ; 4F94

;;; DIVU R0,Rn (n format: 0100nnnn10000100) unsigned batch divide
        DIVU    R0,R1                   ; 4184
        DIVU    R0,R15                  ; 4F84

;;; MULR R0,Rn (n format: 0100nnnn10000000) Rn = R0 * Rn
        MULR    R0,R1                   ; 4180
        MULR    R0,R15                  ; 4F80

;;; SHAD Rm,Rn (nm format: 0100nnnnmmmm1100) arithmetic shift dynamic
        SHAD    R0,R1                   ; 410C
        SHAD    R14,R15                 ; 4FEC

;;; SHLD Rm,Rn (nm format: 0100nnnnmmmm1101) logical shift dynamic
        SHLD    R0,R1                   ; 410D
        SHLD    R14,R15                 ; 4FED

;;; MAC.W @Rm+,@Rn+ (nm format: 0000nnnnmmmm1111) 16x16+64 multiply-accumulate
;;; Note: SH-2A reuses 0x000F (which is RTE on SH-2); the assembler selects
;;; by CPU type. SH-2 has MAC.W at 0x400F (n format); SH-2A keeps that too.
        MAC.W   @R0+,@R1+               ; 010F
        MAC.W   @R14+,@R15+             ; 0FEF

;;; BCLR #imm3,Rn (n format: 10000110nnnniii0) clear bit
        BCLR    #0,R0                   ; 8600
        BCLR    #7,R15                  ; 86FE

;;; BSET #imm3,Rn (n format: 10000110nnnniii1) set bit
        BSET    #0,R0                   ; 8601
        BSET    #7,R15                  ; 86FF

;;; BLD #imm3,Rn (n format: 10000111nnnniii1) load bit to T
        BLD     #0,R0                   ; 8701
        BLD     #7,R15                  ; 87FF

;;; BST #imm3,Rn (n format: 10000111nnnniii0) store T to bit
        BST     #0,R0                   ; 8700
        BST     #7,R15                  ; 87FE

;;; MOVRT Rn (n format: 0000nnnn00111001) T -> Rn
        MOVRT   R0                      ; 0039
        MOVRT   R15                     ; 0F39

;;; NOTT (0000000001101000) invert T bit
        NOTT                            ; 0068

;;; MOVML.L Rn,@-R15 (n format: 0100nnnn11110001) save R0..Rn to stack
        MOVML.L R0,@-R15                ; 40F1
        MOVML.L R15,@-R15               ; 4FF1

;;; MOVML.L @R15+,Rn (n format: 0100nnnn11110101) restore R0..Rn from stack
        MOVML.L @R15+,R0                ; 40F5
        MOVML.L @R15+,R15               ; 4FF5

;;; MOVMU.L Rn,@-R15 (n format: 0100nnnn11110000) save Rn..R14 and PR to stack
        MOVMU.L R0,@-R15                ; 40F0
        MOVMU.L R15,@-R15               ; 4FF0

;;; MOVMU.L @R15+,Rn (n format: 0100nnnn11110100) restore Rn..R14 and PR
        MOVMU.L @R15+,R0                ; 40F4
        MOVMU.L @R15+,R15               ; 4FF4

;;; JSR/N @Rm (m format: 0100mmmm01001011) jump to subroutine, no delay slot
        JSR/N   @R0                     ; 404B
        JSR/N   @R15                    ; 4F4B

;;; RTS/N (0000000001101011) return from subroutine, no delay slot
        RTS/N                           ; 006B

;;; RTV/N Rm (m format: 0000mmmm01111011) Rm -> R0, RTS/N
        RTV/N   R0                      ; 007B
        RTV/N   R15                     ; 0F7B

;;; PREF @Rn (n format: 0000nnnn10000011) prefetch
        PREF    @R0                     ; 0083
        PREF    @R15                    ; 0F83

;;; RESBANK (0000000001011011) restore register bank
        RESBANK                         ; 005B

;;; LDBANK @Rm,R0 (m format: 0100mmmm11100101) load from register bank
        LDBANK  @R0,R0                  ; 40E5
        LDBANK  @R15,R0                 ; 4FE5

;;; STBANK R0,@Rn (n format: 0100nnnn11100001) store to register bank
        STBANK  R0,@R0                  ; 40E1
        STBANK  R0,@R15                 ; 4FE1

;;; ====== 32-bit CPU additions ======

;;; MOVI20 #imm,Rn (n format: 0000nnnniiii0000 iiiiiiiiiiiiiiii) 20-bit immediate
        MOVI20  #0,R0                   ; 00000000
        MOVI20  #H'7FFFF,R1             ; 0170FFFF
        MOVI20  #-H'80000,R2            ; 02800000

;;; MOVI20S #imm,Rn (n format: 0000nnnniiii0001 iiiiiiiiiiiiiiii) 28-bit, low 8 zero
        MOVI20S #0,R0                   ; 00010000
        MOVI20S #H'FF0000,R1            ; 0101FF00

;;; MOV.B Rm,@(disp,Rn) (12-bit disp): 0011nnnnmmmm0001 0001dddddddddddd
        MOV.B   R0,@(H'FFF,R1)          ; 31010FFF

;;; MOV.B @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 0100dddddddddddd
        MOV.B   @(0,R0),R1              ; 31014000

;;; MOV.W Rm,@(disp,Rn) (12-bit disp): 0011nnnnmmmm0001 0010dddddddddddd
;;; MOV.W @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 0101dddddddddddd
        MOV.W   @(0,R0),R1              ; 31015000

;;; MOVU.B @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 1000dddddddddddd
        MOVU.B  @(0,R0),R1              ; 31018000

;;; MOVU.W @(disp,Rm),Rn (12-bit disp): 0011nnnnmmmm0001 1001dddddddddddd
        MOVU.W  @(0,R0),R1              ; 31019000

;;; FMOV.S @(disp,Rm),FRn (12-bit disp): 0011nnnnmmmm0001 0111dddddddddddd
        FMOV.S  @(0,R0),FR1             ; 31017000

;;; Bit-manipulation memory variants: 0011nnnn00001001 OPiiidddddddddddd
;;; BCLR.B #imm3,@(disp,Rn)
        BCLR.B  #0,@(0,R0)              ; 30090000
;;; BSET.B #imm3,@(disp,Rn)
        BSET.B  #0,@(0,R0)              ; 30091000
;;; BST.B #imm3,@(disp,Rn)
        BST.B   #0,@(0,R0)              ; 30092000
;;; BLD.B #imm3,@(disp,Rn)
        BLD.B   #0,@(0,R0)              ; 30093000
;;; BAND.B #imm3,@(disp,Rn)
        BAND.B  #0,@(0,R0)              ; 30094000
;;; BOR.B #imm3,@(disp,Rn)
        BOR.B   #0,@(0,R0)              ; 30095000
;;; BXOR.B #imm3,@(disp,Rn)
        BXOR.B  #0,@(0,R0)              ; 30096000
;;; BLDNOT.B #imm3,@(disp,Rn)
        BLDNOT.B #0,@(0,R0)             ; 3009B000
;;; BANDNOT.B #imm3,@(disp,Rn)
        BANDNOT.B #0,@(0,R0)            ; 3009C000
;;; BORNOT.B #imm3,@(disp,Rn)
        BORNOT.B #0,@(0,R0)             ; 3009D000

;;; ====== FPU additions ======

;;; FCNVDS DRm,FPUL (m format: 1111mmm010111101) double -> single+truncate -> FPUL
        FCNVDS  DR0,FPUL                ; F0BD
        FCNVDS  DR14,FPUL               ; FEBD

;;; FCNVSD FPUL,DRn (n format: 1111nnn010101101) FPUL -> single -> double
        FCNVSD  FPUL,DR0                ; F0AD
        FCNVSD  FPUL,DR14               ; FEAD

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
