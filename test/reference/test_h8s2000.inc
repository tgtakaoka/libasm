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

;;; H8S/2000 instructions over H8/300H, mode-shared (normal+advanced)

;;; TAS @ERn (01E0 7B|ern*16|0C); architectural set is {ER0,ER1,ER4,ER5}
        tas     @er0                   ; 01E0 7B0C
        tas     @er1                   ; 01E0 7B1C
        tas     @er4                   ; 01E0 7B4C
        tas     @er5                   ; 01E0 7B5C
        tas     @er7                   ; 01E0 7B7C

;;; LDM.L @ER7+,ER-list (01|count_minus_1*16 6D 7|first_reg)
;;; 2-reg: ER0-ER1, ER2-ER3, ER4-ER5; 3-reg: ER0-ER2, ER4-ER6; 4-reg: ER0-ER3
        ldm.l   @er7+,er0-er1          ; 0110 6D71
        ldm.l   @er7+,er2-er3          ; 0110 6D73
        ldm     @er7+,er4-er5          ; 0110 6D75
        ldm     @er7+,er0-er2          ; 0120 6D72
        ldm.l   @er7+,er4-er6          ; 0120 6D76
        ldm.l   @er7+,er0-er3          ; 0130 6D73

;;; STM.L ER-list,@-ER7 (01|count_minus_1*16 6D F|first_reg)
;;; 2-reg: ER0-ER1, ER2-ER3, ER4-ER5; 3-reg: ER0-ER2, ER4-ER6; 4-reg: ER0-ER3
        stm.l   er0-er1,@-er7          ; 0110 6DF0
        stm.l   er2-er3,@-er7          ; 0110 6DF2
        stm.l   er4-er5,@-er7          ; 0110 6DF4
        stm.l   er0-er2,@-er7          ; 0120 6DF0
        stm.l   er4-er6,@-er7          ; 0120 6DF4
        stm     er0-er3,@-er7          ; 0130 6DF0

;;; LDC/STC EXR register-to-register (031|reg / 021|reg)
        ldc     r0l,exr                ; 0318
        ldc     r3h,exr                ; 0313
        stc     exr,r1l                ; 0219

;;; LDC/ANDC/ORC/XORC #imm,EXR (0141 0|op imm)
        ldc     #0xA5,exr              ; 0141 07A5
        andc    #0xF0,exr              ; 0141 06F0
        orc     #0xA5,exr              ; 0141 04A5
        xorc    #0x0F,exr              ; 0141 050F

;;; LDC/STC exr,@ERn (0141 69|ern*16|0/8)
        ldc     @er0,exr               ; 0141 6900
        ldc     @er7,exr               ; 0141 6970
        stc     exr,@er0               ; 0141 6980
        stc     exr,@er7               ; 0141 69F0

;;; LDC/STC exr,@(d:16,ERn) (0141 6F|ern*16|0/8 disp16)
        ldc     @(0x1234,er0),exr      ; 0141 6F00 1234
        ldc     @(-1,er7),exr          ; 0141 6F70 FFFF
        stc     exr,@(0x1234,er0)      ; 0141 6F80 1234
        stc     exr,@(0x7FFF,er7)      ; 0141 6FF0 7FFF

;;; LDC/STC exr,@ERn+ / @-ERn (0141 6D|ern*16|0/8)
        ldc     @er0+,exr              ; 0141 6D00
        ldc     @er7+,exr              ; 0141 6D70
        stc     exr,@-er0              ; 0141 6D80
        stc     exr,@-er7              ; 0141 6DF0

;;; LDC/STC exr,@aa:16 (0141 6B|0/8 abs16)
        ldc     @0x1234:16,exr         ; 0141 6B00 1234
        ldc     @0xFFFF80:16,exr       ; 0141 6B00 FF80
        stc     exr,@0x1234:16         ; 0141 6B80 1234
        stc     exr,@0xFFFF80:16       ; 0141 6B80 FF80

;;; LDC/STC exr,@(d:32,ERn) (0141 78|ern*16 6B 20/A0 disp32)
        ldc     @(0x12345678:32,er0),exr ; 0141 7800 6B20 1234 5678
        ldc     @(0x7FFFFFFF:32,er7),exr ; 0141 7870 6B20 7FFF FFFF
        stc     exr,@(0x12345678:32,er0) ; 0141 7800 6BA0 1234 5678
        stc     exr,@(0x7FFFFFFF:32,er7) ; 0141 7870 6BA0 7FFF FFFF

;;; MOV.B Rs,@(d:32,ERn) / MOV.B @(d:32,ERn),Rd (78|ern*16 6A 2|rd/A|rs disp32)
        mov.b   @(0x12345678:32,er0),r0h ; 7800 6A20 1234 5678
        mov.b   @(0x7FFFFFFF:32,er7),r7l ; 7870 6A2F 7FFF FFFF
        mov.b   r0h,@(0x12345678:32,er0) ; 7800 6AA0 1234 5678
        mov.b   r7l,@(0x7FFFFFFF:32,er7) ; 7870 6AAF 7FFF FFFF

;;; MOV.W Rs,@(d:32,ERn) / MOV.W @(d:32,ERn),Rd (78|ern*16 6B 2|rd/A|rs disp32)
        mov.w   @(0x12345678:32,er0),r0  ; 7800 6B20 1234 5678
        mov     @(0x7FFFFFFF:32,er7),r7  ; 7870 6B27 7FFF FFFF
        mov.w   r0,@(0x12345678:32,er0)  ; 7800 6BA0 1234 5678
        mov     r7,@(0x7FFFFFFF:32,er7)  ; 7870 6BA7 7FFF FFFF

;;; MOV.L ERs,@(d:32,ERn) / MOV.L @(d:32,ERn),ERd
;;; (0100 78|0/8|ern*16 6B 2|erd/A|ers disp32)
        mov.l   @(0x12345678:32,er0),er1 ; 0100 7800 6B21 1234 5678
        mov     @(0x7FFFFFFF:32,er7),er0 ; 0100 7870 6B20 7FFF FFFF
        mov.l   er0,@(0x12345678:32,er0) ; 0100 7880 6BA0 1234 5678
        mov     er6,@(0x7FFFFFFF:32,er7) ; 0100 78F0 6BA6 7FFF FFFF

;;; Shift/rotate by 2 (1|op*4|count_size_dir|reg); H8S/2000-only over H8/300H
;;; ROTL/ROTR (#2 high bit = 4 over #1 form)
        rotl.b  #2,r0h                 ; 12C0
        rotl.b  #2,r7l                 ; 12CF
        rotl.w  #2,r0                  ; 12D0
        rotl.w  #2,e7                  ; 12DF
        rotl.l  #2,er0                 ; 12F0
        rotl.l  #2,er7                 ; 12F7
        rotr.b  #2,r0h                 ; 13C0
        rotr.w  #2,r0                  ; 13D0
        rotr.l  #2,er0                 ; 13F0
;;; ROTXL/ROTXR (#2 high bit = 4 over #1 form)
        rotxl.b #2,r0h                 ; 1240
        rotxl.w #2,r0                  ; 1250
        rotxl.l #2,er0                 ; 1270
        rotxr.b #2,r0h                 ; 1340
        rotxr.w #2,r0                  ; 1350
        rotxr.l #2,er0                 ; 1370
;;; SHAL/SHAR (#2 high bit = 4 over #1 form)
        shal.b  #2,r0h                 ; 10C0
        shal.w  #2,r0                  ; 10D0
        shal.l  #2,er0                 ; 10F0
        shar.b  #2,r0h                 ; 11C0
        shar.w  #2,r0                  ; 11D0
        shar.l  #2,er0                 ; 11F0
;;; SHLL/SHLR (#2 high bit = 4 over #1 form)
        shll.b  #2,r0h                 ; 1040
        shll.w  #2,r0                  ; 1050
        shll.l  #2,er0                 ; 1070
        shlr.b  #2,r0h                 ; 1140
        shlr.w  #2,r0                  ; 1150
        shlr.l  #2,er0                 ; 1170

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
