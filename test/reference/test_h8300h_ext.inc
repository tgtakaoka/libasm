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

;;; MOV.L register-to-register (0F80|...)
        mov.l   er0,er1                ; 0F81
        mov     er2,er5                ; 0FA5
        mov.l   er7,er0                ; 0FF0

;;; MOV.L register-indirect (0100 69|b4)
        mov.l   @er0,er1               ; 0100 6901
        mov     er1,@er0               ; 0100 6981
        mov.l   @er2,er3               ; 0100 6923
        mov     er3,@er2               ; 0100 69A3

;;; MOV.L post-increment / pre-decrement (0100 6D|b4)
        mov.l   @er0+,er1              ; 0100 6D01
        mov     er0,@-er1              ; 0100 6D90
        mov.l   @er2+,er3              ; 0100 6D23

;;; POP.L / PUSH.L (ER7 = SP, 0100 6D70|rd / F0|rs)
        pop.l   er0                    ; 0100 6D70
        pop     er5                    ; 0100 6D75
        push.l  er0                    ; 0100 6DF0
        push   er6                     ; 0100 6DF6

;;; MOV.L @(d16,ERn),ERd (0100 6E|b4 disp16)
        mov.l   @(0x1234,er0),er1      ; 0100 6F01 1234
        mov     er0,@(0x1234,er1)      ; 0100 6F90 1234
        mov.l   @(-0x20,er2),er3       ; 0100 6F23 FFE0

;;; MOV.L @abs16,ERd (0100 6B|erd abs16)
        mov.l   @0x001234:16,er0       ; 0100 6B00 1234
        mov     @0xFFFFFE:16,er7       ; 0100 6B07 FFFE
        mov.l   er0,@0x001234:16       ; 0100 6B80 1234
        mov     er7,@0xFFFFFE:16       ; 0100 6B87 FFFE

;;; MOV.L #imm32,ERd (7A|erd imm32)
        mov.l   #0x12345678,er0        ; 7A00 1234 5678
        mov     #0xFFFFFFFF,er7        ; 7A07 FFFF FFFF
        mov.l   #0,er3                 ; 7A03 0000 0000

;;; ADD.L / SUB.L Rs,Rd (0A8|b4 / 1A8|b4)
        add.l   er0,er1                ; 0A81
        add     er7,er0                ; 0AF0
        sub.l   er0,er1                ; 1A81
        sub     er7,er0                ; 1AF0

;;; CMP.L Rs,Rd (1F8|b4)
        cmp.l   er0,er1                ; 1F81
        cmp     er3,er5                ; 1FB5

;;; ADD.L/SUB.L/CMP.L #xx:32,ERd (7A|op|erd imm32)
        add.l   #0x12345678,er0        ; 7A10 1234 5678
        add     #0xFFFFFFFF,er7        ; 7A17 FFFF FFFF
        sub.l   #0x12345678,er0        ; 7A30 1234 5678
        sub     #0xFFFFFFFF,er7        ; 7A37 FFFF FFFF
        cmp.l   #0x12345678,er0        ; 7A20 1234 5678
        cmp     #0xFFFFFFFF,er7        ; 7A27 FFFF FFFF

;;; AND.L/OR.L/XOR.L Rs,Rd (01F0 64/65/66|b4)
        or.l    er0,er1                ; 01F0 6401
        or      er2,er7                ; 01F0 6427
        xor.l   er0,er1                ; 01F0 6501
        xor     er3,er4                ; 01F0 6534
        and.l   er0,er1                ; 01F0 6601
        and     er5,er6                ; 01F0 6656

;;; AND.L/OR.L/XOR.L #xx:32,ERd (7A|op|erd imm32)
        or.l    #0x12345678,er0        ; 7A40 1234 5678
        or      #0xFFFFFFFF,er7        ; 7A47 FFFF FFFF
        xor.l   #0x12345678,er0        ; 7A50 1234 5678
        xor     #0xFFFFFFFF,er7        ; 7A57 FFFF FFFF
        and.l   #0x12345678,er0        ; 7A60 1234 5678
        and     #0xFFFFFFFF,er7        ; 7A67 FFFF FFFF

;;; ADDS/SUBS #4,ERd (0B/1B90|erd)
        adds    #4,er0                 ; 0B90
        adds    #4,er7                 ; 0B97
        subs    #4,er0                 ; 1B90
        subs    #4,er5                 ; 1B95

;;; INC.W / DEC.W (0A/1A50|rd / 0A/1AD0|rd)
        inc.w   #1, r0                 ; 0B50
        inc     #2, r7                 ; 0BD7
        inc.w   #1, e0                 ; 0B58
        inc     #2, e7                 ; 0BDF
        dec.w   #1, r0                 ; 1B50
        dec     #2, r7                 ; 1BD7
        dec.w   #1, e0                 ; 1B58
        dec     #2, e3                 ; 1BDB

;;; INC.L / DEC.L (0B/1B70|erd / 0B/1BF0|erd)
        inc.l   #1, er0                ; 0B70
        inc     #2, er0                ; 0BF0
        inc.l   #1, er7                ; 0B77
        inc     #2, er7                ; 0BF7
        dec.l   #1, er0                ; 1B70
        dec     #2, er0                ; 1BF0
        dec.l   #1, er7                ; 1B77
        dec     #2, er7                ; 1BF7

;;; ADD.W / SUB.W / CMP.W #xx:16,Rd (79|op|rd imm16)
        add.w   #0x1234,r0             ; 7910 1234
        add     #0xFFFF,e7             ; 791F FFFF
        sub.w   #0x1234,r0             ; 7930 1234
        sub     #0xFFFF,e7             ; 793F FFFF
        cmp.w   #0x1234,r0             ; 7920 1234
        cmp     #0xFFFF,e7             ; 792F FFFF

;;; AND.W / OR.W / XOR.W Rs,Rd (66/64/65|b2)
        and.w   r0,r1                  ; 6601
        and     e0,r1                  ; 6681
        and.w   r0,e1                  ; 6609
        and     e0,e7                  ; 668F
        or.w    r0,r1                  ; 6401
        or      e0,r1                  ; 6481
        or.w    r0,e1                  ; 6409
        xor.w   r0,r1                  ; 6501
        xor     e0,r1                  ; 6581
        xor.w   r0,e1                  ; 6509

;;; AND.W / OR.W / XOR.W #xx:16,Rd (79|op|rd imm16)
        and.w   #0x1234,r0             ; 7960 1234
        and     #0xFFFF,e7             ; 796F FFFF
        or.w    #0x1234,r0             ; 7940 1234
        or      #0xFFFF,e7             ; 794F FFFF
        xor.w   #0x1234,r0             ; 7950 1234
        xor     #0xFFFF,e7             ; 795F FFFF

;;; NOT.W/L, NEG.W/L, EXTU.W/L, EXTS.W/L (17|b2[7:4] dispatch)
        not.w   r0                     ; 1710
        not     e0                     ; 1718
        not.l   er0                    ; 1730
        neg.w   r0                     ; 1790
        neg     e0                     ; 1798
        neg.l   er0                    ; 17B0
        extu.w  r0                     ; 1750
        extu    e0                     ; 1758
        extu.l  er0                    ; 1770
        exts.w  r0                     ; 17D0
        exts    e0                     ; 17D8
        exts.l  er0                    ; 17F0

;;; Shift/rotate .W and .L (10-13, bits[6:4] of b2)
        shll.w  r0                     ; 1010
        shll    e0                     ; 1018
        shll.l  er0                    ; 1030
        shal.w  r0                     ; 1090
        shal    e0                     ; 1098
        shal.l  er0                    ; 10B0
        shlr.w  r0                     ; 1110
        shlr    e0                     ; 1118
        shlr.l  er0                    ; 1130
        shar.w  r0                     ; 1190
        shar    e0                     ; 1198
        shar.l  er0                    ; 11B0
        rotxl.w r0                     ; 1210
        rotxl   e0                     ; 1218
        rotxl.l er0                    ; 1230
        rotl.w  r0                     ; 1290
        rotl    e0                     ; 1298
        rotl.l  er0                    ; 12B0
        rotxr.w r0                     ; 1310
        rotxr   e0                     ; 1318
        rotxr.l er0                    ; 1330
        rotr.w  r0                     ; 1390
        rotr    e0                     ; 1398
        rotr.l  er0                    ; 13B0

;;; MULXU.B with E-reg destination (50|rs|ed); destination Rd field extends to E0-E7
        mulxu.b r3h,e4                 ; 503C
        mulxu.b r0h,e7                 ; 500F
        divxu.b r3h,e4                 ; 513C
        divxu.b r0h,e7                 ; 510F

;;; MULXU.W Rs16,ERd (52|b2) / DIVXU.W Rs16,ERd (53|b2)
        mulxu.w r0,er1                 ; 5201
        mulxu.w e0,er1                 ; 5281
        mulxu.w r7,er0                 ; 5270
        mulxu.w e7,er7                 ; 52F7
        divxu.w r0,er1                 ; 5301
        divxu.w e0,er1                 ; 5381
        divxu.w r3,er7                 ; 5337
        divxu.w e7,er7                 ; 53F7

;;; MULXS.B Rs8,Rd16 (01C0 50|b2) / MULXS.W Rs16,ERd (01C0 52|b2)
        mulxs.b r0h,r1                 ; 01C0 5001
        mulxs.b r7l,r0                 ; 01C0 50F0
        mulxs.b r3h,e4                 ; 01C0 503C
        mulxs.w r0,er1                 ; 01C0 5201
        mulxs.w e0,er1                 ; 01C0 5281
        mulxs.w r7,er0                 ; 01C0 5270
        mulxs.w e7,er7                 ; 01C0 52F7

;;; DIVXS.B Rs8,Rd16 (01D0 51|b2) / DIVXS.W Rs16,ERd (01D0 53|b2)
        divxs.b r0h,r1                 ; 01D0 5101
        divxs.b r7l,r0                 ; 01D0 51F0
        divxs.b r3h,e4                 ; 01D0 513C
        divxs.w r0,er1                 ; 01D0 5301
        divxs.w e0,er1                 ; 01D0 5381
        divxs.w r3,er7                 ; 01D0 5337
        divxs.w e7,er7                 ; 01D0 53F7

;;; TRAPA #vector (57|vector*16)
        trapa   #0                     ; 5700
        trapa   #1                     ; 5710
        trapa   #2                     ; 5720
        trapa   #3                     ; 5730

;;; LDC/STC ccr,@ERn (0140 69|ern*16|0/8)
        ldc     @er0,ccr               ; 0140 6900
        ldc     @er7,ccr               ; 0140 6970
        stc     ccr,@er0               ; 0140 6980
        stc     ccr,@er7               ; 0140 69F0

;;; LDC/STC ccr,@(d:16,ERn) (0140 6F|ern*16|0/8 disp16)
        ldc     @(0x1234,er0),ccr      ; 0140 6F00 1234
        ldc     @(-1,er7),ccr          ; 0140 6F70 FFFF
        stc     ccr,@(0x1234,er0)      ; 0140 6F80 1234
        stc     ccr,@(0x7FFF,er7)      ; 0140 6FF0 7FFF

;;; LDC/STC ccr,@aa:16 (0140 6B|0/8 abs16)
        ldc     @0x1234:16,ccr         ; 0140 6B00 1234
        ldc     @0xFFFF80:16,ccr       ; 0140 6B00 FF80
        stc     ccr,@0x1234:16         ; 0140 6B80 1234
        stc     ccr,@0xFFFF80:16       ; 0140 6B80 FF80

;;; LDC/STC ccr,@ERn+ / @-ERn (0140 6D|ern*16|0/8)
        ldc     @er0+,ccr              ; 0140 6D00
        ldc     @er7+,ccr              ; 0140 6D70
        stc     ccr,@-er0              ; 0140 6D80
        stc     ccr,@-er7              ; 0140 6DF0

;;; Long branches (58|cc*16 rel16) and BSR (5C00 rel16)
        bra     far_label              ; 5800 09CE
        brn     far_label              ; 5810 09CA
        bhi     far_label              ; 5820 09C6
        bls     far_label              ; 5830 09C2
        bcc     far_label              ; 5840 09BE
        bcs     far_label              ; 5850 09BA
        bne     far_label              ; 5860 09B6
        beq     far_label              ; 5870 09B2
        bvc     far_label              ; 5880 09AE
        bvs     far_label              ; 5890 09AA
        bpl     far_label              ; 58A0 09A6
        bmi     far_label              ; 58B0 09A2
        bge     far_label              ; 58C0 099E
        blt     far_label              ; 58D0 099A
        bgt     far_label              ; 58E0 0996
        ble     far_label              ; 58F0 0992
        bsr     far_label              ; 5C00 098E

;;; JMP/JSR @ERn (ER registers, same encoding as Rn but decoded as ERn)
        jmp     @er0                   ; 5900
        jmp     @er5                   ; 5950
        jsr     @er0                   ; 5D00
        jsr     @er3                   ; 5D30

;;; EEPMOV.W (7BD4 598F)
        eepmov.w                       ; 7BD4 598F

;;; Bit ops Rs8,@ERn (7C|ern*16 op rs*16)
        bset    r0h,@er0               ; 7D00 6000
        bset    r3h,@er1               ; 7D10 6030
        bnot    r0h,@er0               ; 7D00 6100
        bclr    r0h,@er0               ; 7D00 6200
        btst    r0h,@er0               ; 7C00 6300

;;; Bit ops #bit,@ERn (7D|ern*16 op bit*16)
        bset    #0,@er0                ; 7D00 7000
        bset    #7,@er1                ; 7D10 7070
        bnot    #3,@er0                ; 7D00 7130
        bclr    #5,@er0                ; 7D00 7250
        btst    #1,@er0                ; 7C00 7310
        bst     #0,@er0                ; 7D00 6700
        bist    #0,@er0                ; 7D00 6780

;;; Bit ops Rs8,@abs8 (7E|abs8 op rs*16)
        bset    r0h,@0xFFFF10:8        ; 7F10 6000
        btst    r2h,@0xFFFF20:8        ; 7E20 6320

;;; Bit ops #bit,@abs8 (7F|abs8 op bit*16)
        bset    #0,@0xFFFF10:8         ; 7F10 7000
        btst    #5,@0xFFFF20:8         ; 7E20 7350

;;; Extended 16-bit register operations (E0-E7 now valid in 4-bit field)
        add.w   e0,r1                  ; 0981
        add.w   r0,e1                  ; 0909
        add.w   e0,e1                  ; 0989
        mov.w   e0,r1                  ; 0D81
        mov.w   r0,e1                  ; 0D09
        mov.w   e0,e1                  ; 0D89
        sub.w   e0,r1                  ; 1981
        sub.w   r0,e1                  ; 1909
        sub.w   e0,e1                  ; 1989
        cmp.w   e0,r1                  ; 1D81
        cmp.w   r0,e1                  ; 1D09
        cmp.w   e0,e1                  ; 1D89
        mov.w   @er0,e1                ; 6909
        mov.w   e0,@er1                ; 6998
        mov.w   @0x1234:16,e0          ; 6B08 1234
        mov.w   e0,@0x1234:16          ; 6B88 1234
        mov.w   #0x1234,e0             ; 7908 1234

;;; MOV.W with E-reg + displacement / post-inc / pre-dec
        mov.w   @(0x1234,er0),e0       ; 6F08 1234
        mov.w   e0,@(0x1234,er0)       ; 6F88 1234
        mov.w   @er0+,e0               ; 6D08
        mov.w   e0,@-er0               ; 6D88

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
