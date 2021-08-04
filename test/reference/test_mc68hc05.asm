;;; Copyright 2021 Tadashi G. Takaoka
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

        cpu     68HC05
        org     $0100
;;; 0X
        brset0  $01, *+5
        brclr0  $02, *+6
        brset1  $03, *+7
        brclr1  $04, *+8
        brset2  $05, *+9
        brclr2  $06, *+10
        brset3  $07, *+11
        brclr3  $08, *+12
        brset4  $09, *+13
        brclr4  $0a, *+14
        brset5  $0b, *+15
        brclr5  $0c, *+16
        brset6  $0d, *+17
        brclr6  $0e, *+18
        brset7  $0f, *+19
        brclr7  $10, *+20
;;; 1X
        bset0   $11
        bclr0   $12
        bset1   $13
        bclr1   $14
        bset2   $15
        bclr2   $16
        bset3   $17
        bclr3   $18
        bset4   $19
        bclr4   $1a
        bset5   $1b
        bclr5   $1c
        bset6   $1d
        bclr6   $1e
        bset7   $1f
        bclr7   $20
;;; 2X
        bra     *+$23
        brn     *+$23
        bhi     *+$25
        bls     *+$26
        bcc     *+$27
        bcs     *+$28
        bhs     *+$27
        blo     *+$28
        bne     *+$29
        beq     *+$2a
        bhcc    *+$2b
        bhcs    *+$2c
        bpl     *+$2d
        bmi     *+$2e
        bmc     *+$2f
        bms     *+$30
        bil     *+$31
        bih     *+$32
;;; 3X
        neg     $31
        com     $34
        lsr     $35
        ror     $37
        asr     $38
        lsl     $39
        asl     $39
        rol     $3a
        dec     $3b
        inc     $3d
        tst     $3e
        clr     $40
;;; 4X
        nega
        mul
        coma
        lsra
        rora
        asra
        lsla
        asla
        rola
        deca
        inca
        tsta
        clra
;;; 5X
        negx
        comx
        lsrx
        rorx
        asrx
        lslx
        aslx
        rolx
        decx
        incx
        tstx
        clrx
;;; 6X
        neg     0,x
        com     0,x
        lsr     1,x
        ror     6,x
        asr     8,x
        lsl     9,x
        asl     9,x
        rol     10,x
        dec     11,x
        inc     13,x
        tst     14,x
        clr     255,x
;;; 7X
        neg     ,x
        com     ,x
        lsr     ,x
        ror     ,x
        asr     ,x
        lsl     ,x
        asl     ,x
        rol     ,x
        dec     ,x
        inc     ,x
        tst     ,x
        clr     ,x
;;; 8X
        rti
        rts
        swi
        stop
        wait
;;; 9X
        tax
        clc
        sec
        cli
        sei
        rsp
        nop
        txa
;;; AX
        sub     #$a1
        cmp     #$a2
        sbc     #$a3
        cpx     #$a4
        and     #$a5
        bit     #$a6
        lda     #$a7
        eor     #$a9
        adc     #$aa
        ora     #$ab
        add     #$ac
        bsr     *-80
        ldx     #$af
;;; BX
        sub     $b1
        cmp     $b2
        sbc     $b3
        cpx     $b4
        and     $b5
        bit     $b6
        lda     $b7
        sta     $b8
        eor     $b9
        adc     $ba
        ora     $bb
        add     $bc
        jmp     $bd
        jsr     $be
        ldx     $bf
        stx     $c0
;;; CX
        sub     $11c2
        cmp     $12c3
        sbc     $13c4
        cpx     $14c5
        and     $15c6
        bit     $16c7
        lda     $17c8
        sta     $18c9
        eor     $19ca
        adc     $1acb
        ora     $1bcc
        add     $1ccd
        jmp     $1dce
        jsr     $1ecf
        ldx     $1fd0
        stx     $10d1
;;; DX
        sub     $d1d2,x
        cmp     $d2d3,x
        sbc     $d3d4,x
        cpx     $d4d5,x
        and     $d5d6,x
        bit     $d6d7,x
        lda     $d7d8,x
        sta     $d8d9,x
        eor     $d9da,x
        adc     $dadb,x
        ora     $dbdc,x
        add     $dcdd,x
        jmp     $ddde,x
        jsr     $dedf,x
        ldx     $dfe0,x
        stx     $e0e1,x
;;; EX
        sub     $e1,x
        cmp     $e2,x
        sbc     $e3,x
        cpx     $e4,x
        and     $e5,x
        bit     $e6,x
        lda     $e7,x
        sta     $e8,x
        eor     $e9,x
        adc     $ea,x
        ora     $eb,x
        add     $ec,x
        jmp     $ed,x
        jsr     $ee,x
        ldx     $ef,x
        stx     $f0,x
;;; FX
        sub     ,x
        cmp     ,x
        sbc     ,x
        cpx     ,x
        and     ,x
        bit     ,x
        lda     ,x
        sta     ,x
        eor     ,x
        adc     ,x
        ora     ,x
        add     ,x
        jmp     ,x
        jsr     ,x
        ldx     ,x
        stx     ,x
