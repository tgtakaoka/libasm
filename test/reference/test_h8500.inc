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

;;; Base instructions for the H8/500 family (H8/520 intersection).
;;; Excludes E-clock (MOVTPE/MOVFPE) and page-jump (PJMP/PJSR/PRTS/PRTD)
;;; instructions covered by separate .inc files.

;;; Data Transfer

;;; MOV:G register-register
        mov:g.b r0,r1                   ; A0 81
        mov:g.w r0,r1                   ; A8 81
        mov:g.b r7,r0                   ; A7 80
        mov:g.w r7,r0                   ; AF 80

;;; MOV:G register indirect
        mov:g.b @r0,r1                  ; D0 81
        mov:g.w @r1,r2                  ; D9 82
        mov:g.b r1,@r0                  ; D0 91
        mov:g.w r2,@r1                  ; D9 92

;;; MOV:G pre-decrement / post-increment
        mov:g.b @-r0,r1                 ; B0 81
        mov:g.w @-r1,r2                 ; B9 82
        mov:g.b @r0+,r1                 ; C0 81
        mov:g.w @r1+,r2                 ; C9 82
        mov:g.b r1,@-r0                 ; B0 91
        mov:g.b r1,@r0+                 ; C0 91

;;; MOV:G displacement (8-bit and 16-bit)
;;; Note: ASL bug (see ~/asl_bugs_h8500.md) requires `((-N), reg)` for negatives.
        mov:g.b @(16,r0),r1             ; E0 10 81
        mov:g.b @((-16),r0),r1          ; E0 F0 81
        mov:g.w @(16,r1),r2             ; E9 10 82
        mov:g.b @(0x1234,r0),r1         ; F0 12 34 81
        mov:g.w @((-0x20),r1),r2        ; E9 E0 82
;;; Explicit 16-bit displacement form
        mov:g.w @(0x100,r1),r2          ; F9 01 00 82
        mov:g.b r1,@(16,r0)             ; E0 10 91
        mov:g.b r1,@(0x1234,r0)         ; F0 12 34 91

;;; MOV:G absolute 8-bit / 16-bit
        mov:g.b @0x30:8,r0              ; 05 30 80
        mov:g.b @0xff:8,r1              ; 05 FF 81
        mov:g.w @0x40:8,r0              ; 0D 40 80
        mov:g.b @0x1234:16,r0           ; 15 12 34 80
        mov:g.w @0x1234:16,r1           ; 1D 12 34 81
        mov:g.b r0,@0x30:8              ; 05 30 90
        mov:g.b r0,@0x1234:16           ; 15 12 34 90

;;; MOV:G immediate
        mov:g.b #0x55,r0                ; 04 55 80
        mov:g.w #0x1234,r0              ; 0C 12 34 80
        mov:g.b #0x55,@r0               ; D0 06 55
        mov:g.w #0x1234,@r1             ; D9 07 12 34

;;; MOV:E (short immediate byte to register)
        mov:e   #0x55,r0                ; 50 55
        mov:e   #-1,r7                  ; 57 FF

;;; MOV:I (short immediate word to register)
        mov:i   #0x1234,r0              ; 58 12 34
        mov:i   #-1,r7                  ; 5F FF FF

;;; MOV:L (short load byte/word @aa:8 -> Rd)
        mov:l.b @0xa0:8,r0              ; 60 A0
        mov:l.w @0xa2:8,r0              ; 68 A2

;;; MOV:S (short store Rs -> @aa:8)
        mov:s.b r0,@0xa0:8              ; 70 A0
        mov:s.w r0,@0xa2:8              ; 78 A2

;;; MOV:F (short stack-frame @(d:8,fp))
;;; Note: ASL bug requires `((-N), fp)` for negative displacements.
        mov:f.b @(4,fp),r0              ; 80 04
        mov:f.w @(4,fp),r0              ; 88 04
        mov:f.b @((-4),fp),r0           ; 80 FC
        mov:f.b r0,@(4,fp)              ; 90 04
        mov:f.w r0,@(4,fp)              ; 98 04

;;; LDM / STM (stack multi-register)
        ldm     @sp+,(r0)               ; 02 01
        ldm     @sp+,(r0-r3)            ; 02 0F
        ldm     @sp+,(r0,r2-r4)         ; 02 1D
        stm     (r0),@-sp               ; 12 01
        stm     (r0-r3),@-sp            ; 12 0F
        stm     (r0,r2-r4),@-sp         ; 12 1D

;;; XCH / SWAP
        xch     r0,r1                   ; A8 91
        swap    r0                      ; A0 10

;;; Arithmetic

;;; ADD:G
        add:g.b r0,r1                   ; A0 21
        add:g.w r0,r1                   ; A8 21
        add:g.w @r0,r1                  ; D8 21
        add:g.b #0x55,r0                ; 04 55 20
        add:g.w #0x1234,r0              ; 0C 12 34 20

;;; ADD:Q (quick: +1/-1/+2/-2)
        add:q   #1,r0                   ; A8 08
        add:q   #2,r0                   ; A8 09
        add:q   #-1,r0                  ; A8 0C
        add:q   #-2,r0                  ; A8 0D
        add:q   #1,@r0                  ; D8 08
        add:q   #-2,@0x30:8             ; 0D 30 0D

;;; ADDS / SUBS (sign-extending word add/sub)
        adds.b  r0,r1                   ; A0 29
        adds.w  r0,r1                   ; A8 29
        subs.b  r0,r1                   ; A0 39
        subs.w  r0,r1                   ; A8 39

;;; ADDX / SUBX (with carry/borrow)
        addx.b  r0,r1                   ; A0 A1
        addx.w  r0,r1                   ; A8 A1
        subx.b  r0,r1                   ; A0 B1
        subx.w  r0,r1                   ; A8 B1

;;; DADD / DSUB (BCD)
        dadd    r0,r1                   ; A0 00 A1
        dsub    r0,r1                   ; A0 00 B1

;;; SUB
        sub.b   r0,r1                   ; A0 31
        sub.w   r0,r1                   ; A8 31

;;; MULXU / DIVXU
        mulxu.b r0,r1                   ; A0 A9
        mulxu.w r0,r1                   ; A8 A9
        divxu.b r0,r1                   ; A0 B9
        divxu.w r0,r1                   ; A8 B9

;;; CMP:G / CMP:E / CMP:I
        cmp:g.b r0,r1                   ; A0 71
        cmp:g.w r0,r1                   ; A8 71
        cmp:g.b #0x55,r0                ; 04 55 70
        cmp:g.w #0x1234,r0              ; 0C 12 34 70
        cmp:e   #0x55,r0                ; 40 55
        cmp:e   #-1,r0                  ; 40 FF
        cmp:i   #0x1234,r0              ; 48 12 34
        cmp:i   #-1,r0                  ; 48 FF FF

;;; EXTS / EXTU (sign / zero extend byte to word)
        exts    r0                      ; A0 11
        extu    r0                      ; A0 12

;;; TST / NEG / CLR / TAS
        tst.b   r0                      ; A0 16
        tst.w   r0                      ; A8 16
        neg.b   r0                      ; A0 14
        neg.w   r0                      ; A8 14
        clr.b   r0                      ; A0 13
        clr.w   r0                      ; A8 13
        tas     r0                      ; A0 17

;;; Logic

        and.b   r0,r1                   ; A0 51
        and.w   r0,r1                   ; A8 51
        or.b    r0,r1                   ; A0 41
        or.w    r0,r1                   ; A8 41
        xor.b   r0,r1                   ; A0 61
        xor.w   r0,r1                   ; A8 61
        not.b   r0                      ; A0 15
        not.w   r0                      ; A8 15

;;; Shift / Rotate (all act on EA destination)

        shal.b  r0                      ; A0 18
        shal.w  r0                      ; A8 18
        shar.b  r0                      ; A0 19
        shar.w  r0                      ; A8 19
        shll.b  r0                      ; A0 1A
        shll.w  r0                      ; A8 1A
        shlr.b  r0                      ; A0 1B
        shlr.w  r0                      ; A8 1B
        rotl.b  r0                      ; A0 1C
        rotl.w  r0                      ; A8 1C
        rotr.b  r0                      ; A0 1D
        rotr.w  r0                      ; A8 1D
        rotxl.b r0                      ; A0 1E
        rotxl.w r0                      ; A8 1E
        rotxr.b r0                      ; A0 1F
        rotxr.w r0                      ; A8 1F

;;; Bit Manipulation

        bset    r0,r1                   ; A9 48
        bclr    r0,r1                   ; A9 58
        bnot    r0,r1                   ; A9 68
        btst    r0,r1                   ; A9 78
        bset    #5,r0                   ; A8 C5
        bclr    #5,r0                   ; A8 D5
        bnot    #5,r0                   ; A8 E5
        btst    #5,r0                   ; A8 F5
;;; Byte-size bit ops on byte-sized destination
        bset.b  r0,@r1                  ; D1 48
        bclr.b  #3,@r1                  ; D1 D3

;;; Branch (Bcc d:8)

start:
        bra     start                   ; 20 FE
        brn     start                   ; 21 FC
        bhi     start                   ; 22 FA
        bls     start                   ; 23 F8
        bcc     start                   ; 24 F6
        bhs     start                   ; 24 F4
        bcs     start                   ; 25 F2
        blo     start                   ; 25 F0
        bne     start                   ; 26 EE
        beq     start                   ; 27 EC
        bvc     start                   ; 28 EA
        bvs     start                   ; 29 E8
        bpl     start                   ; 2A E6
        bmi     start                   ; 2B E4
        bge     start                   ; 2C E2
        blt     start                   ; 2D E0
        bgt     start                   ; 2E DE
        ble     start                   ; 2F DC
        bt      start                   ; 20 DA
        bf      start                   ; 21 D8
;;; BSR (subroutine d:8)
        bsr     start                   ; 0E D6

;;; Jumps

        jmp     @0x1234:16              ; 10 12 34
        jmp     @r0                     ; 11 D0
        jmp     @(0x10,r0)              ; 11 E0 10
        jmp     @(0x1234,r0)            ; 11 F0 12 34
        jsr     @0x1234:16              ; 18 12 34
        jsr     @r0                     ; 11 D8
        jsr     @(0x10,r0)              ; 11 E8 10
        jsr     @(0x1234,r0)            ; 11 F8 12 34

;;; Returns / system

        rts                             ; 19
        rtd     #4                      ; 14 04
        rtd     #0x1234                 ; 1C 12 34
;;; Note: rte (0x0A) not recognized by h8500-hms-as; see ~/gas_bugs_h8500.md
        rte                             ; 0A
        sleep                           ; 1A
        nop                             ; 00
        trapa   #0                      ; 08 10
        trapa   #3                      ; 08 13
        trap/vs                         ; 09

;;; LINK / UNLK
        link    fp,#-4                  ; 17 FC
        link    fp,#-256                ; 1F FF 00
        unlk    fp                      ; 0F

;;; SCB loop control
loop:   scb/f   r0,loop                 ; 01 B8 FD
        scb/ne  r1,loop                 ; 06 B9 FA
        scb/eq  r2,loop                 ; 07 BA F7

;;; Control register access

        ldc.b   r0,ccr                  ; A0 89
        ldc.w   r0,sr                   ; A8 88
        ldc.b   r0,br                   ; A0 8B
        ldc.b   r0,ep                   ; A0 8C
        ldc.b   r0,dp                   ; A0 8D
        ldc.b   r0,tp                   ; A0 8F
        ldc.b   #0x55,ccr               ; 04 55 89
        ldc.w   #0x1234,sr              ; 0C 12 34 88
        stc.b   ccr,r0                  ; A0 99
        stc.w   sr,r0                   ; A8 98
        stc.b   br,r0                   ; A0 9B
        stc.b   ep,r0                   ; A0 9C
        stc.b   dp,r0                   ; A0 9D
        stc.b   tp,r0                   ; A0 9F

        andc.b  #0x55,ccr               ; 04 55 59
        andc.w  #0x1234,sr              ; 0C 12 34 58
        orc.b   #0x55,ccr               ; 04 55 49
        orc.w   #0x1234,sr              ; 0C 12 34 48
        xorc.b  #0x55,ccr               ; 04 55 69
        xorc.w  #0x1234,sr              ; 0C 12 34 68

;;; Local Variables:
;;; mode: asm
;;; End:
