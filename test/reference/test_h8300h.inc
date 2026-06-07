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

;;; MOV.B register
        mov     r0h,r1h                 ; 0C01
        mov.b   r0l,r1l                 ; 0C89
        mov     #0x56,r2h               ; F256
        mov.b   #0xA5,r3l               ; FBA5

;;; MOV.B indirect
        mov.b   @er0,r1h                ; 6801
        mov     r1h,@er0                ; 6881
        mov     @er1,r0l                ; 6818
        mov.b   r0l,@er1                ; 6898

;;; MOV.B displacement
        mov     @(0x1234,er0),r1h       ; 6E01 1234
        mov.b   r1h,@(0x1234,er0)       ; 6E81 1234
        mov.b   @(-0x20,er2),r3h        ; 6E23 FFE0
        mov     r3h,@(-0x20,er2)        ; 6EA3 FFE0

;;; MOV.B post-increment / pre-decrement
        mov.b   @er0+,r1h               ; 6C01
        mov     @er0+,r7l               ; 6C0F
        mov     r7l,@-er0               ; 6C8F
        mov.b   r1h,@-er0               ; 6C81

;;; MOV.B absolute 8-bit
        mov     @0xFFFF34:8,r0h         ; 2034
        mov.b   r0h,@0xFFFF34:8         ; 3034
        mov.b   @0xFFFFFF:8,r1h         ; 21FF
        mov     r1h,@0xFFFFFF:8         ; 31FF
        mov     @0xFFFF00:8,r7l         ; 2F00
        mov     r7l,@0xFFFF00:8         ; 3F00

;;; MOV.B absolute 16-bit
        mov.b   @0x1234:16,r0h          ; 6A00 1234
        mov     r0h,@0x1234:16          ; 6A80 1234
        mov     @0x5678:16,r1h          ; 6A01 5678
        mov.b   r1h,@0xFFFF00:16        ; 6A81 FF00
        mov     @0x0000:16,r0h          ; 6A00 0000
        mov     r7l,@0x7FFF:16          ; 6A8F 7FFF
        mov.b   @0xFFFF80:16,r7l        ; 6A0F FF80

;;; MOV.W register
        mov     r0,r1                   ; 0D01
        mov.w   r2,r3                   ; 0D23
        mov.w   #0x1234,r0              ; 7900 1234
        mov     #0xABCD,r7              ; 7907 ABCD

;;; MOV.W indirect
        mov.w   @er0,r1                 ; 6901
        mov     r1,@er0                 ; 6981
        mov     @er0,r7                 ; 6907
        mov.w   r7,@er7                 ; 69F7

;;; MOV.W displacement
        mov     @(0x1234,er0),r1        ; 6F01 1234
        mov.w   r1,@(0x1234,er0)        ; 6F81 1234
        mov.w   @(-1,er0),r1            ; 6F01 FFFF
        mov     r1,@(+2,er0)            ; 6F81 0002

;;; MOV.W post-increment / pre-decrement
        mov.w   @er0+,r1                ; 6D01
        mov.w   r1,@-er0                ; 6D81
        mov.w   @er7+,r1                ; 6D71
        mov.w   r1,@-er7                ; 6DF1

;;; MOV.W absolute 16-bit
        mov.w   @0x1234,r0              ; 6B00 1234
        mov.w   r0,@0xFFFF80:16         ; 6B80 FF80
        mov     @0x0000:16,r0           ; 6B00 0000
        mov     r7,@0x7FFE:16           ; 6B87 7FFE

;;; MOVFPE / MOVTPE
        movfpe  @0x1234:16,r0h          ; 6A40 1234
        movtpe  r0h,@0xFFFF80:16        ; 6AC0 FF80

;;; POP / PUSH
        pop     r0                      ; 6D70
        push    r1                      ; 6DF1
        pop.w   r0                      ; 6D70
        push.w  r1                      ; 6DF1

;;; Arithmetic

;;; ADD.B
        add.b   r0h,r1h                 ; 0801
        add     r0l,r1l                 ; 0889
        add     #0x34,r0h               ; 8034
        add.b   #0xFF,r7l               ; 8FFF

;;; ADD.W
        add.w   r0,r1                   ; 0901
        add     r2,r3                   ; 0923

;;; ADDS
        adds    #1,er0                  ; 0B00
        adds.l  #1,er7                  ; 0B07
        adds    #2,er0                  ; 0B80
        adds    #2,er7                  ; 0B87

;;; ADDX
        addx    r0h,r1h                 ; 0E01
        addx.b  r0l,r1l                 ; 0E89
        addx    #0x34,r0h               ; 9034
        addx.b  #0xFF,r7l               ; 9FFF

;;; INC
        inc     r0h                     ; 0A00
        inc.b   r7l                     ; 0A0F

;;; SUB.B
        sub     r0h,r1h                 ; 1801
        sub.b   r0l,r1l                 ; 1889

;;; SUB.W
        sub.w   r0,r1                   ; 1901
        sub     r2,r3                   ; 1923

;;; SUBS
        subs    #1,er0                  ; 1B00
        subs    #1,er7                  ; 1B07
        subs.l  #2,er0                  ; 1B80
        subs    #2,er7                  ; 1B87

;;; SUBX
        subx    r0h,r1h                 ; 1E01
        subx.b  r0l,r1l                 ; 1E89
        subx    #0x34,r0h               ; B034
        subx.b  #0xFF,r7l               ; BFFF

;;; DEC
        dec     r0h                     ; 1A00
        dec.b   r7l                     ; 1A0F

;;; DAA / DAS
        daa     r0h                     ; 0F00
        daa.b   r7l                     ; 0F0F
        das.b   r0h                     ; 1F00
        das     r7l                     ; 1F0F

;;; NEG / NOT
        neg     r0h                     ; 1780
        neg.b   r7l                     ; 178F
        not.b   r0h                     ; 1700
        not     r7l                     ; 170F

;;; CMP.B
        cmp     r0h,r1h                 ; 1C01
        cmp.b   r0l,r1l                 ; 1C89
        cmp     #0x34,r0h               ; A034
        cmp.b   #0xFF,r7l               ; AFFF

;;; CMP.W
        cmp.w   r0,r1                   ; 1D01
        cmp     r2,r3                   ; 1D23

;;; MULXU / DIVXU
        mulxu.b r0h,r0                  ; 5000
        mulxu   r1l,r1                  ; 5091
        mulxu.b r7l,r7                  ; 50F7
        divxu.b r0h,r0                  ; 5100
        divxu   r1l,r1                  ; 5191
        divxu.b r7l,r7                  ; 51F7

;;; Logic

;;; AND.B
        and     r0h,r1h                 ; 1601
        and.b   r0l,r1l                 ; 1689
        and     #0xF0,r0h               ; E0F0
        and.b   #0x0F,r7l               ; EF0F

;;; OR.B
        or      r0h,r1h                 ; 1401
        or.b    r0l,r1l                 ; 1489
        or      #0xF0,r0h               ; C0F0
        or.b    #0x0F,r7l               ; CF0F

;;; XOR.B
        xor     r0h,r1h                 ; 1501
        xor.b   r0l,r1l                 ; 1589
        xor     #0xF0,r0h               ; D0F0
        xor.b   #0x0F,r7l               ; DF0F

;;; Shift / Rotate

        shll.b  r0h                     ; 1000
        shll    r7l                     ; 100F
        shal.b  r0h                     ; 1080
        shal    r7l                     ; 108F
        shlr.b  r0h                     ; 1100
        shlr    r7l                     ; 110F
        shar.b  r0h                     ; 1180
        shar    r7l                     ; 118F
        rotxl.b r0h                     ; 1200
        rotxl   r7l                     ; 120F
        rotl.b  r0h                     ; 1280
        rotl    r7l                     ; 128F
        rotxr.b r0h                     ; 1300
        rotxr   r7l                     ; 130F
        rotr.b  r0h                     ; 1380
        rotr    r7l                     ; 138F

;;; Bit Manipulation register
        bset    #3,r0h                  ; 7030
        bset.b  #7,r7l                  ; 707F
        bset    r1h,r2h                 ; 6012
        bset.b  r1h,r2h                 ; 6012
        bclr    #3,r0h                  ; 7230
        bclr.b  #7,r7l                  ; 727F
        bclr    r1h,r2h                 ; 6212
        bclr.b  r1h,r2h                 ; 6212
        bnot    #3,r0h                  ; 7130
        bnot.b  #7,r7l                  ; 717F
        bnot    r1h,r2h                 ; 6112
        bnot.b  r1h,r2h                 ; 6112
        btst    #3,r0h                  ; 7330
        btst.b  #7,r7l                  ; 737F
        btst    r1h,r2h                 ; 6312
        btst.b  r1h,r2h                 ; 6312

;;; Bit Manipulation indirect
        bset    #3,@er0                 ; 7D00 7030
        bset.b  #7,@er7                 ; 7D70 7070
        bset    r1h,@er2                ; 7D20 6010
        bset.b  r1h,@er2                ; 7D20 6010
        bclr    #3,@er0                 ; 7D00 7230
        bclr.b  #7,@er7                 ; 7D70 7270
        bclr    r1h,@er2                ; 7D20 6210
        bclr.b  r1h,@er2                ; 7D20 6210
        bnot    #3,@er0                 ; 7D00 7130
        bnot.b  #7,@er7                 ; 7D70 7170
        bnot    r1h,@er2                ; 7D20 6110
        bnot.b  r1h,@er2                ; 7D20 6110
        btst    #3,@er0                 ; 7C00 7330
        btst.b  #7,@er7                 ; 7C70 7370
        btst    r1h,@er7                ; 7C70 6310
        btst.b  r1h,@er7                ; 7C70 6310

;;; Bit Manipulation absolute 8-bit
        bset.b  #7,@0xFFFF81:8          ; 7F81 7070
        bset    r1h,@0xFFFF82:8         ; 7F82 6010
        bclr    #3,@0xFFFF00:8          ; 7F00 7230
        bclr.b  r1h,@0xFFFF03:8         ; 7F03 6210
        bnot.b  #7,@0xFFFFE1:8          ; 7FE1 7170
        bnot    r1h,@0xFFFFE2:8         ; 7FE2 6110
        btst    #3,@0xFFFFFC:8          ; 7EFC 7330
        btst.b  r1h,@0xFFFFFF:8         ; 7EFF 6310
        bset    #0,@0xFFFF00:8          ; 7F00 7000
        bclr.b  #7,@0xFFFFFF:8          ; 7FFF 7270
        bnot    #0,@0xFFFF00:8          ; 7F00 7100
        btst    r7h,@0xFFFF00:8         ; 7E00 6370

;;; Bit to carry register
        bld     #0,r0h                  ; 7700
        bld.b   #1,r0h                  ; 7710
        bild    #2,r0h                  ; 77A0
        bild.b  #3,r0h                  ; 77B0
        bst     #4,r0h                  ; 6740
        bst.b   #5,r0h                  ; 6750
        bist    #6,r0h                  ; 67E0
        bist.b  #7,r0h                  ; 67F0
        band    #0,r0h                  ; 7600
        band.b  #1,r0h                  ; 7610
        biand   #2,r0h                  ; 76A0
        biand.b #3,r0h                  ; 76B0
        bor     #4,r0h                  ; 7440
        bor.b   #5,r0h                  ; 7450
        bior    #6,r0h                  ; 74E0
        bior.b  #7,r0h                  ; 74F0
        bxor    #0,r0h                  ; 7500
        bxor.b  #1,r0h                  ; 7510
        bixor   #2,r0h                  ; 75A0
        bixor.b #3,r0h                  ; 75B0

;;; Bit to carry indirect
        bld     #0,@er7                 ; 7C70 7700
        bld.b   #1,@er6                 ; 7C60 7710
        bild    #2,@er5                 ; 7C50 77A0
        bild.b  #3,@er4                 ; 7C40 77B0
        bst     #4,@er3                 ; 7D30 6740
        bst.b   #5,@er2                 ; 7D20 6750
        bist    #6,@er1                 ; 7D10 67E0
        bist.b  #7,@er0                 ; 7D00 67F0
        band    #0,@er7                 ; 7C70 7600
        band.b  #1,@er6                 ; 7C60 7610
        biand   #2,@er5                 ; 7C50 76A0
        biand.b #3,@er4                 ; 7C40 76B0
        bor     #4,@er3                 ; 7C30 7440
        bor.b   #5,@er2                 ; 7C20 7450
        bior    #6,@er1                 ; 7C10 74E0
        bior.b  #7,@er0                 ; 7C00 74F0
        bxor    #0,@er7                 ; 7C70 7500
        bxor.b  #1,@er6                 ; 7C60 7510
        bixor   #2,@er5                 ; 7C50 75A0
        bixor.b #3,@er0                 ; 7C00 75B0

;;; Bit to carry absolute 8-bit
        bld     #0,@0xFFFF07:8          ; 7E07 7700
        bld.b   #1,@0xFFFFFF:8          ; 7EFF 7710
        bild    #2,@0xFFFF00:8          ; 7E00 77A0
        bild.b  #3,@0xFFFF04:8          ; 7E04 77B0
        bst     #4,@0xFFFFFF:8          ; 7FFF 6740
        bst.b   #5,@0xFFFF02:8          ; 7F02 6750
        bist    #6,@0xFFFF01:8          ; 7F01 67E0
        bist.b  #7,@0xFFFF00:8          ; 7F00 67F0
        band    #0,@0xFFFF87:8          ; 7E87 7600
        band.b  #1,@0xFFFFFF:8          ; 7EFF 7610
        biand   #2,@0xFFFF00:8          ; 7E00 76A0
        biand.b #3,@0xFFFF84:8          ; 7E84 76B0
        bor     #4,@0xFFFF00:8          ; 7E00 7440
        bor.b   #5,@0xFFFF82:8          ; 7E82 7450
        bior    #6,@0xFFFF81:8          ; 7E81 74E0
        bior.b  #7,@0xFFFFFF:8          ; 7EFF 74F0
        bxor    #0,@0xFFFFFC:8          ; 7EFC 7500
        bxor.b  #1,@0xFFFFFF:8          ; 7EFF 7510
        bixor   #2,@0xFFFF00:8          ; 7E00 75A0
        bixor.b #3,@0xFFFFFF:8          ; 7EFF 75B0

;;; Control Registers

        stc     ccr,r0h                 ; 0200
        stc.b   ccr,r7l                 ; 020F
        ldc     r0h,ccr                 ; 0300
        ldc.b   r7l,ccr                 ; 030F
        ldc     #0x80,ccr               ; 0780
        ldc.b   #0x00,ccr               ; 0700
        andc    #0xF0,ccr               ; 06F0
        andc.b  #0xF0,ccr               ; 06F0
        orc     #0x0F,ccr               ; 040F
        orc.b   #0x0F,ccr               ; 040F
        xorc    #0x55,ccr               ; 0555
        xorc.b  #0x55,ccr               ; 0555

;;; Branch

        bra     label                   ; 4012
        brn     label                   ; 4110
        bhi     label                   ; 420E
        bls     label                   ; 430C
        bcc     label                   ; 440A
        bhs     label                   ; 4408
        bcs     label                   ; 4506
        blo     label                   ; 4504
        bne     label                   ; 4602
        beq     label                   ; 4700
label:
        bvc     label                   ; 48FE
        bvs     label                   ; 49FC
        bpl     label                   ; 4AFA
        bmi     label                   ; 4BF8
        bge     label                   ; 4CF6
        blt     label                   ; 4DF4
        bgt     label                   ; 4EF2
        ble     label                   ; 4FF0
        bsr     label                   ; 55EE
        bra     label-108               ; 4080

;;; JMP / JSR

        jmp     @er0                    ; 5900
        jmp     @er7                    ; 5970
        jmp     @0x1234                 ; 5A00 1234
        jmp     @0x1234:16              ; 5A00 1234
        jmp     @@0x0034                ; 5B34
        jsr     @er0                    ; 5D00
        jsr     @er7                    ; 5D70
        jsr     @0x1234                 ; 5E00 1234
        jsr     @0x1234:16              ; 5E00 1234
        jsr     @@0x0034                ; 5F34

;;; System

        rts                             ; 5470
        rte                             ; 5670
        sleep                           ; 0180
        nop                             ; 0000
        eepmov.b                        ; 7B5C 598F

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
