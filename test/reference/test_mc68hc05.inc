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

;;; 0X
        brset   0,$01, *+5
        brclr   0,$02, *+6
        brset   1,$03, *+7
        brclr   1,$04, *+8
        brset   2,$05, *+9
        brclr   2,$06, *+10
        brset   3,$07, *+11
        brclr   3,$08, *+12
        brset   4,$09, *+13
        brclr   4,$0a, *+14
        brset   5,$0b, *+15
        brclr   5,$0c, *+16
        brset   6,$0d, *+17
        brclr   6,$0e, *+18
        brset   7,$0f, *+19
        brclr   7,$10, *+20
;;; 1X
        bset    0,$11
        bclr    0,$12
        bset    1,$13
        bclr    1,$14
        bset    2,$15
        bclr    2,$16
        bset    3,$17
        bclr    3,$18
        bset    4,$19
        bclr    4,$1a
        bset    5,$1b
        bclr    5,$1c
        bset    6,$1d
        bclr    6,$1e
        bset    7,$1f
        bclr    7,$20
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
        neg     $61,x
        com     $64,x
        lsr     $65,x
        ror     $67,x
        asr     $68,x
        lsl     $69,x
        asl     $69,x
        rol     $6a,x
        dec     $6b,x
        inc     $6d,x
        tst     $6e,x
        clr     $70,x
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
        sub     abs+$01c2
        cmp     abs+$02c3
        sbc     abs+$03c4
        cpx     abs+$04c5
        and     abs+$05c6
        bit     abs+$06c7
        lda     abs+$07c8
        sta     abs+$08c9
        eor     abs+$09ca
        adc     abs+$0acb
        ora     abs+$0bcc
        add     abs+$0ccd
        jmp     abs+$0dce
        jsr     abs+$0ecf
        ldx     abs+$0fd0
        stx     abs+$00d1
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

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
