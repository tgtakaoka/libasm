        cpu     6800

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
        bge     *+$31
        ble     *+$32
;;; 3X
        tsx
        ins
        pul     a
        pul     b
        des
        txs
        psh     a
        psh     b
        rts
        rti
        wai
        swi
;;; 4X
        neg     a
        com     a
        lsr     a
        ror     a
        asr     a
        lsl     a
        asl     a
        rol     a
        dec     a
        inc     a
        tst     a
        clr     a
;;; 5X
        neg     b
        com     b
        lsr     b
        ror     b
        asr     b
        lsl     b
        asl     b
        rol     b
        dec     b
        inc     b
        tst     b
        clr     b
;;; 6X
        neg     ,x
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
        sub     a,#$81
        cmp     a,#$82
        sbc     a,#$83
        and     a,#$85
        bit     a,#$86
        lda     a,#$87
        eor     a,#$89
        adc     a,#$8a
        ora     a,#$8b
        add     a,#$8c
        cpx     #$8d8e
        bsr     $008e
        lds     #$8f90
;;; 9X
        sub     a,$91
        cmp     a,$92
        sbc     a,$93
        and     a,$95
        bit     a,$96
        lda     a,$97
        sta     a,$98
        eor     a,$99
        adc     a,$9a
        ora     a,$9b
        add     a,$9c
        cpx     $9d
        lds     $9f
        sts     $a0
;;; AX
        sub     a,,x
        cmp     a,0,x
        sbc     a,1,x
        and     a,2,x
        bit     a,3,x
        lda     a,4,x
        sta     a,5,x
        eor     a,6,x
        adc     a,127,x
        ora     a,128,x
        add     a,255,x
        cpx     ,x
        jsr     2,x
        lds     4,x
        sts     6,x
;;; BX
        sub     a,$b1b2
        sub     a,$b1b2
        cmp     a,$b2b3
        sbc     a,$b3b4
        and     a,$b5b6
        bit     a,$b6b7
        lda     a,$b7b9
        sta     a,$b8b9
        eor     a,$b9ba
        adc     a,$babb
        ora     a,$bbbc
        add     a,$bcbd
        cpx     $bdbe
        jsr     $bebf
        lds     $bfc0
        sts     $c0c1
;;; CX
        sub     b,#$c1
        cmp     b,#$c2
        sbc     b,#$c3
        and     b,#$c5
        bit     b,#$c6
        lda     b,#$c7
        eor     b,#$c8
        adc     b,#$c9
        ora     b,#$ca
        add     b,#$cb
        ldx     #$cdce
;;; DX
        sub     b,$d1
        cmp     b,$d2
        sbc     b,$d3
        and     b,$d5
        bit     b,$d6
        lda     b,$d7
        sta     b,$d8
        eor     b,$d9
        adc     b,$da
        ora     b,$db
        add     b,$dc
        ldx     $df
        stx     $e0
;;; EX
        sub     b,,x
        cmp     b,0,x
        sbc     b,1,x
        and     b,2,x
        bit     b,3,x
        lda     b,4,x
        sta     b,5,x
        eor     b,,x
        adc     b,127,x
        ora     b,128,x
        add     b,255,x
        ldx     4,x
        stx     6,x
;;; FX
        sub     b,$f1f2
        cmp     b,$f2f3
        sbc     b,$f3f4
        and     b,$f5f6
        bit     b,$f6f7
        lda     b,$f7f8
        sta     b,$f8f9
        eor     b,$f9fa
        adc     b,$fafb
        ora     b,$fbfc
        add     b,$fcfd
        ldx     $ff00
        stx     >$0001
