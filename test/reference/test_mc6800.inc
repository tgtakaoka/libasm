;;; Copyright 2020 Tadashi G. Takaoka
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
        nop
        tap
        tpa
        inx
        dex
        clv
        sev
        clc
        sec
        cli
        sei
;;; 1X
        sba
        cba
        tab
        tba
        daa
        aba
;;; 2X
        bra     *+$23
        bhi     *+$25
        bls     *+$26
        bcc     *+$27
        bcs     *+$28
        bhs     *+$27
        blo     *+$28
        bne     *+$29
        beq     *+$2a
        bvc     *+$2b
        bvs     *+$2c
        bpl     *+$2d
        bmi     *+$2e
        bge     *+$2f
        blt     *+$30
        bgt     *+$31
        ble     *+$32
;;; 3X
        tsx
        ins
        pula
        pulb
        des
        txs
        psha
        pshb
        rts
        rti
        wai
        swi
;;; 4X
        nega
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
        negb
        comb
        lsrb
        rorb
        asrb
        lslb
        aslb
        rolb
        decb
        incb
        tstb
        clrb
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
        jmp     15,x
        clr     255,x
;;; 7X
        neg     $7172
        com     $7475
        lsr     $7576
        ror     $7778
        asr     $7879
        lsl     $797a
        asl     $797a
        rol     $7a7b
        dec     $7b7c
        inc     $7d7e
        tst     $7e7f
        jmp     $7f80
        clr     $8081
;;; 8X
        suba    #$81
        cmpa    #$82
        sbca    #$83
        anda    #$85
        bita    #$86
        ldaa    #$87
        eora    #$89
        adca    #$8a
        oraa    #$8b
        adda    #$8c
        cpx     #$8d8e
        bsr     *-$70
        lds     #$8f90
;;; 9X
        suba    $91
        cmpa    $92
        sbca    $93
        anda    $95
        bita    $96
        ldaa    $97
        staa    $98
        eora    $99
        adca    $9a
        oraa    $9b
        adda    $9c
        cpx     $9d
        lds     $9f
        sts     $a0
;;; AX
        suba    0,x
        cmpa    0,x
        sbca    1,x
        anda    2,x
        bita    3,x
        ldaa    4,x
        staa    5,x
        eora    6,x
        adca    127,x
        oraa    128,x
        adda    255,x
        cpx     0,x
        jsr     2,x
        lds     4,x
        sts     6,x
;;; BX
        suba    $b1b2
        suba    $b1b2
        cmpa    $b2b3
        sbca    $b3b4
        anda    $b5b6
        bita    $b6b7
        ldaa    $b7b9
        staa    $b8b9
        eora    $b9ba
        adca    $babb
        oraa    $bbbc
        adda    $bcbd
        cpx     $bdbe
        jsr     $bebf
        lds     $bfc0
        sts     $c0c1
;;; CX
        subb    #$c1
        cmpb    #$c2
        sbcb    #$c3
        andb    #$c5
        bitb    #$c6
        ldab    #$c7
        eorb    #$c8
        adcb    #$c9
        orab    #$ca
        addb    #$cb
        ldx     #$cdce
;;; DX
        subb    $d1
        cmpb    $d2
        sbcb    $d3
        andb    $d5
        bitb    $d6
        ldab    $d7
        stab    $d8
        eorb    $d9
        adcb    $da
        orab    $db
        addb    $dc
        ldx     $df
        stx     $e0
;;; EX
        subb    0,x
        cmpb    0,x
        sbcb    1,x
        andb    2,x
        bitb    3,x
        ldab    4,x
        stab    5,x
        eorb    0,x
        adcb    127,x
        orab    128,x
        addb    255,x
        ldx     4,x
        stx     6,x
;;; FX
        subb    $f1f2
        cmpb    $f2f3
        sbcb    $f3f4
        andb    $f5f6
        bitb    $f6f7
        ldab    $f7f8
        stab    $f8f9
        eorb    $f9fa
        adcb    $fafb
        orab    $fbfc
        addb    $fcfd
        ldx     $ff00
        stx     >$0001

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
