        cpu     6502

;;; 0X
        brk
        ora     ($01,x)
        ora     $05
        asl     $06
        php
        ora     #$09
        asl     a
        ora     $0d0d
        asl     $0e0e
;;; 1X
        bpl     $10
        ora     ($11),y
        ora     $15,x
        asl     $16,x
        clc
        ora     $1919,y
        ora     $1d1d,x
        asl     $1e1e,x
;;; 2X
        jsr     $2020
        and     ($21,x)
        bit     $24
        and     $25
        rol     $26
        plp
        and     #$29
        rol     a
        bit     $2c2c
        and     $2d2d
        rol     $2e2e
;;; 3X
        bmi     $30
        and     ($31),y
        and     $35,x
        rol     $36,x
        sec
        and     $3939,y
        and     $3d3d,x
        rol     $3e3e,x
;;; 4X
        rti
        eor     ($41,x)
        eor     $45
        lsr     $46
        pha
        eor     #$49
        lsr     a
        jmp     $4c4c
        eor     $4d4d
        lsr     $4e4e
;;; 5X
        bvc     $50
        eor     ($51),y
        eor     $55,x
        lsr     $56,x
        cli
        eor     $5959,y
        eor     $5d5d,x
        lsr     $5e5e,x
;;; 6X
        rts
        adc     ($61,x)
        adc     $65
        ror     $66
        pla
        adc     #$69
        ror     a
        jmp     ($6c6c)
        adc     $6d6d
        ror     $6e6e
;;; 7X
        bvs     $70
        adc     ($71),y
        adc     $75,x
        ror     $76,x
        sei
        adc     $7979,y
        adc     $7d7d,x
        ror     $7e7e,x
;;; 8X
        sta     ($81,x)
        sty     $84
        sta     $85
        stx     $86
        dey
        txa
        sty     $8c8c
        sta     $8d8d
        stx     $8e8e
;;; 9X
        bcc     $90
        sta     ($91),y
        sty     $94,x
        sta     $95,x
        stx     $96,y
        tya
        sta     $9999,y
        txs
        sta     $9d9d,x
;;; AX
        ldy     #$a0
        lda     ($a1,x)
        ldx     #$a2
        ldy     $a4
        lda     $a5
        ldx     $a6
        tay
        lda     #$a9
        tax
        ldy     $acac
        lda     $adad
        ldx     $aeae
;;; BX
        bcs     $b0
        lda     ($b1),y
        ldy     $b4,x
        lda     $b5,x
        ldx     $b6,y
        clv
        lda     $b9b9,y
        tsx
        ldy     $bcbc,x
        lda     $bdbd,x
        ldx     $bebe,y
;;; CX
        cpy     #$c0
        cmp     ($c1,x)
        cpy     $c4
        cmp     $c5
        dec     $c6
        iny
        cmp     #$c9
        dex
        cpy     $cccc
        cmp     $cdcd
        dec     $cece
;;; DX
        bne     $d0
        cmp     ($d1),y
        cmp     $d5,x
        dec     $d6,x
        cld
        cmp     $d9d9,y
        cmp     $dddd,x
        dec     $dede,x
;;; EX
        cpx     #$e0
        sbc     ($e1,x)
        cpx     $e4
        sbc     $e5
        inc     $e6
        inx
        sbc     #$e9
        nop
        cpx     $ecec
        sbc     $eded
        inc     $eeee
;;; FX
        beq     $f0
        sbc     ($f1),y
        sbc     $f5,x
        inc     $f6,x
        sed
        sbc     $f9f9,y
        sbc     $fdfd,x
        inc     $fefe,x
