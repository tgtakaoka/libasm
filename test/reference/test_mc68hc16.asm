;;; Copyright 2025 Tadashi G. Takaoka
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
        cpu     68HC16
        org     $10000
;;; 0x
        com     $01,x
        dec     $02,x
        neg     $03,x
        inc     $04,x
        asl     $05,x
        lsl     $05,x
        clr     $06,x
        tst     $07,x
        bclr    $0A0B,x,#$09
        bset    $0B0C,x,#$0A
        brclr   $0C0D,x,#$0B,*+$0E16
        brset   $0D0E,x,#$0C,*+$0F16
        rol     $0D,x
        asr     $0E,x
        ror     $0F,x
        lsr     $10,x
;;; 1x
        com     $11,y
        dec     $12,y
        neg     $13,y
        inc     $14,y
        asl     $15,y
        lsl     $15,y
        clr     $16,y
        tst     $17,y
        bclr    $1A1B,y,#$19
        bset    $1B1C,y,#$1A
        brclr   $1C1D,y,#$1B,*+$1E26
        brset   $1D1E,y,#$1C,*+$1F26
        rol     $1D,y
        asr     $1E,y
        ror     $1F,y
        lsr     $20,y
;;; 2x
        com     $21,z
        dec     $22,z
        neg     $23,z
        inc     $24,z
        asl     $25,z
        lsl     $25,z
        clr     $26,z
        tst     $27,z
        bclr    $2A2B,z,#$29
        bset    $2B2C,z,#$2A
        brclr   $2C2D,z,#$2B,*+$2E36
        brset   $2D2E,z,#$2C,*+$2F36
        rol     $2D,z
        asr     $2E,z
        ror     $2F,z
        lsr     $30,z
;;; 3x
        movb    $31,x,$3233
        movw    $32,x,$3334
        movb    $3435,$33,x
        movw    $3536,$34,x
        pshm    k,z,x,d
        pshx
        pshy
        pulm    e,x,z,k
        pulx
        puly
        bsr     *+$3E
        bclr    $3A3B,#$39
        bset    $3B3C,#$3A
        aix     #$3D
        dex
        inx
        aiy     #$3E
        dey
        iny
        aiz     #$3F
        ais     #$40
        des
        ins
;;; 4x
        suba    $41,x
        adda    $42,x
        sbca    $43,x
        adca    $44,x
        eora    $45,x
        ldaa    $46,x
        anda    $47,x
        oraa    $48,x
        cmpa    $49,x
        bita    $4A,x
        staa    $4B,x
        jmp     -$3B2B2,x
        cpx     $4D,x
        cpy     $4E,x
        cpy     $4F,x
        cps     $50,x
;;; 5x
        suba    $51,y
        adda    $52,y
        sbca    $53,y
        adca    $54,y
        eora    $55,y
        ldaa    $56,y
        anda    $57,y
        oraa    $58,y
        cmpa    $59,y
        bita    $5A,y
        staa    $5B,y
        jmp     -$3a2a2,y
        cpx     $5D,y
        cpy     $5E,y
        cpy     $5F,y
        cps     $70,y
;;; 6x
        suba    $61,z
        adda    $62,z
        sbca    $63,z
        adca    $64,z
        eora    $65,z
        ldaa    $66,z
        anda    $67,z
        oraa    $68,z
        cmpa    $69,z
        bita    $6A,z
        staa    $6B,z
        jmp     -$39292,z
        cpx     $6D,z
        cpy     $6E,z
        cpy     $6F,z
        cps     $70,z
;;; 7x
        suba    #$71
        adda    #$72
        sbca    #$73
        adca    #$74
        eora    #$75
        ldaa    #$76
        anda    #$77
        oraa    #$78
        cmpa    #$79
        bita    #$7A
        jmp     $0B7C7D
        mac     7,-4
        adde    #$7D
;;; 8x
        subd    $81,x
        addd    $82,x
        sbcd    $83,x
        adcd    $84,x
        eord    $85,x
        ldd     $86,x
        andd    $87,x
        ord     $88,x
        cpd     $89,x
        jsr     -$57474,x
        std     $8B,x
        brset   $8D,x,#$8C,*-$6C
        stx     $8D,x
        sty     $8E,x
        sty     $8F,x
        sts     $90,x
;;; 9x
        subd    $91,y
        addd    $92,y
        sbcd    $93,y
        adcd    $94,y
        eord    $95,y
        ldd     $96,y
        andd    $97,y
        ord     $98,y
        cpd     $99,y
        jsr     -$56464,y
        std     $9B,y
        brset   $9D,y,#$9C,*-$5C
        stx     $9D,y
        sty     $9E,y
        sty     $9F,y
        sts     $A0,y
;;; Ax
        subd    $A1,z
        addd    $A2,z
        sbcd    $A3,z
        adcd    $A4,z
        eord    $A5,z
        ldd     $A6,z
        andd    $A7,z
        ord     $A8,z
        cpd     $A9,z
        jsr     -$55454,z
        std     $AB,z
        brset   $AD,z,#$AC,*-$4C
        stx     $AD,z
        sty     $AE,z
        sty     $AF,z
        sts     $B0,z
;;; Bx
        bra     *-$4A
        brn     *-$48
        bhi     *-$48
        bls     *-$46
        bcc     *-$46
        bhs     *-$46
        bcs     *-$44
        blo     *-$44
        bne     *-$44
        beq     *-$42
        bvc     *-$42
        bvs     *-$40
        bpl     *-$40
        bmi     *-$3E
        bge     *-$3E
        blt     *-$3C
        bgt     *-$3C
        ble     *-$3A
;;; Cx
        subb    $C1,x
        addb    $C2,x
        sbcb    $C3,x
        adcb    $C4,x
        eorb    $C5,x
        ldab    $C6,x
        andb    $C7,x
        orab    $C8,x
        cmpb    $C9,x
        bitb    $CA,x
        stab    $CB,x
        brclr   $CD,x,#$CC,*-$2C
        ldx     $CD,x
        ldy     $CE,x
        ldy     $CF,x
        lds     $C0,x
;;; Dx
        subb    $D1,y
        addb    $D2,y
        sbcb    $D3,y
        adcb    $D4,y
        eorb    $D5,y
        ldab    $D6,y
        andb    $D7,y
        orab    $D8,y
        cmpb    $D9,y
        bitb    $DA,y
        stab    $DB,y
        brclr   $DD,y,#$DC,*-$1C
        ldx     $DD,y
        ldy     $DE,y
        ldy     $DF,y
        lds     $D0,y
;;; Ex
        subb    $E1,z
        addb    $E2,z
        sbcb    $E3,z
        adcb    $E4,z
        eorb    $E5,z
        ldab    $E6,z
        andb    $E7,z
        orab    $E8,z
        cmpb    $E9,z
        bitb    $EA,z
        stab    $EB,z
        brclr   $ED,z,#$EC,*-$0C
        ldx     $ED,z
        ldy     $EE,z
        ldy     $EF,z
        lds     $E0,z
;;; Fx
        subb    #$F1
        addb    #$F2
        sbcb    #$F3
        adcb    #$F4
        eorb    #$F5
        ldab    #$F6
        andb    #$F7
        orab    #$F8
        cmpb    #$F9
        bitb    #$FA
        rmac    -1,-4
;;; 17 0x
        com     $0102,x
        dec     $0203,x
        neg     $0304,x
        inc     $0405,x
        asl     $0506,x
        lsl     $0506,x
        clr     $0607,x
        tst     $0708,x
        bclr    $0A,x,#$09
        bset    $0B,x,#$0A
        rol     $0D0C,x
        asr     $0E0D,x
        ror     $0F10,x
        lsr     $1011,x
;;; 17 1x
        com     $1112,y
        dec     $1213,y
        neg     $1314,y
        inc     $1415,y
        lsl     $1516,y
        clr     $1617,y
        tst     $1718,y
        bclr    $1A,y,#$19
        bset    $1B,y,#$1A
        rol     $1D1C,y
        asr     $1E1D,y
        ror     $1F20,y
        lsr     $2021,y
;;; 17 2x
        com     $2122,z
        dec     $2223,z
        neg     $2324,z
        inc     $2425,z
        asl     $2526,z
        lsl     $2526,z
        clr     $2627,z
        tst     $2728,z
        bclr    $2A,z,#$29
        bset    $2B,z,#$2A
        rol     $2D2C,z
        asr     $2E2D,z
        ror     $2F30,z
        lsr     $3031,z
;;; 17 3x
        com     $3132
        dec     $3233
        neg     $3334
        inc     $3435
        asl     $3536
        lsl     $3536
        clr     $3637
        tst     $3738
        rol     $3D3C
        asr     $3E3D
        ror     $3F30
        lsr     $4041
;;; 17 4x
        suba    $4142,x
        adda    $4243,x
        sbca    $4344,x
        adca    $4445,x
        eora    $4546,x
        ldaa    $4647,x
        anda    $4748,x
        oraa    $4849,x
        cmpa    $494A,x
        bita    $4A4B,x
        staa    $4B4C,x
        cpx     $4D4E,x
        cpy     $4E4F,x
        cpy     $4F50,x
        cps     $5051,x
;;; 17 5x
        suba    $5152,y
        adda    $5253,y
        sbca    $5354,y
        adca    $5455,y
        eora    $5556,y
        ldaa    $5657,y
        anda    $5758,y
        oraa    $5859,y
        cmpa    $595A,y
        bita    $5A5B,y
        staa    $5B5C,y
        cpx     $5D5E,y
        cpy     $5E5F,y
        cpy     $5F60,y
        cps     $6061,y
;;; 17 6x
        suba    $6162,z
        adda    $6263,z
        sbca    $6364,z
        adca    $6465,z
        eora    $6566,z
        ldaa    $6667,z
        anda    $6768,z
        oraa    $6869,z
        cmpa    $696A,z
        bita    $6A6B,z
        staa    $6B6C,z
        cpx     $6D6E,z
        cpy     $6E6F,z
        cpy     $6F70,z
        cps     $7071,z
;;; 17 7x
        suba    $7172
        adda    $7273
        sbca    $7374
        adca    $7475
        eora    $7576
        ldaa    $7677
        anda    $7778
        oraa    $7879
        cmpa    $797A
        bita    $7A7B
        staa    $7B7C
        cpx     $7D7E
        cpy     $7E7F
        cpy     $7F80
        cps     $8081
;;; 17 8x
        stx     -$7272,x
        sty     -$7171,x
        stz     -$7070,x
        sts     -$6F6F,x
;;; 17 9x
        stx     -$6262,y
        sty     -$6161,y
        stz     -$6060,y
        sts     -$5F5F,y
;;; 17 Ax
        stx     -$5252,z
        sty     -$5151,z
        stz     -$5050,z
        sts     -$4F4F,z
;;; 17 Bx
        stx     $BDBE
        sty     $BEBF
        stz     $BFC0
        sts     $C0C1
;;; 17 Cx
        subb    -$3E3E,x
        addb    -$3D3D,x
        sbcb    -$3C3C,x
        adcb    -$3B3B,x
        eorb    -$3A3A,x
        ldab    -$3939,x
        andb    -$3838,x
        orab    -$3737,x
        cmpb    -$3636,x
        bitb    -$3535,x
        stab    -$3434,x
        ldx     -$3232,x
        ldy     -$3131,x
        ldy     -$3030,x
        lds     -$2F2F,x
;;; 17 Dx
        subb    -$2E2E,y
        addb    -$2D2D,y
        sbcb    -$2C2C,y
        adcb    -$2B2B,y
        eorb    -$2A2A,y
        ldab    -$2929,y
        andb    -$2828,y
        orab    -$2727,y
        cmpb    -$2626,y
        bitb    -$2525,y
        stab    -$2424,y
        ldx     -$2222,y
        ldy     -$2121,y
        ldy     -$2020,y
        lds     -$1F1F,y
;;; 17 Ex
        subb    -$1E1E,z
        addb    -$1D1D,z
        sbcb    -$1C1C,z
        adcb    -$1B1B,z
        eorb    -$1A1A,z
        ldab    -$1919,z
        andb    -$1818,z
        orab    -$1717,z
        cmpb    -$1616,z
        bitb    -$1515,z
        stab    -$1414,z
        ldx     -$1212,z
        ldy     -$1111,z
        ldy     -$1010,z
        lds     -$0F0F,z
;;; 17 Fx
        subb    $F1F2
        addb    $F2F3
        sbcb    $F3F4
        adcb    $F4F5
        eorb    $F5F6
        ldab    $F6F7
        andb    $F7F8
        orab    $F8F9
        cmpb    $F9FA
        bitb    $FAFB
        stab    $FBFC
        ldx     $FDFE
        ldy     $FEFF
        ldy     $FFF0
        lds     $0001
;;; 27 0x
        comw    $0102,x
        decw    $0203,x
        negw    $0304,x
        incw    $0405,x
        aslw    $0506,x
        lslw    $0506,x
        clrw    $0607,x
        tstw    $0708,x
        bclrw   $090A,x,#$0B0C
        bsetw   $0A0B,x,#$0C0D
        rolw    $0D0E,x
        asrw    $0E0F,x
        rorw    $0F10,x
        lsrw    $1011,x
;;; 27 1x
        comw    $1112,y
        decw    $1213,y
        negw    $1314,y
        incw    $1415,y
        aslw    $1516,y
        lslw    $1516,y
        clrw    $1617,y
        tstw    $1718,y
        bclrw   $191A,y,#$1B1C
        bsetw   $1A1B,y,#$1C1D
        rolw    $1D1E,y
        asrw    $1E1F,y
        rorw    $1F20,y
        lsrw    $2021,y
;;; 27 2x
        comw    $2122,z
        decw    $2223,z
        negw    $2324,z
        incw    $2425,z
        aslw    $2526,z
        lslw    $2526,z
        clrw    $2627,z
        tstw    $2728,z
        bclrw   $292A,z,#$2B2C
        bsetw   $2A2B,z,#$2C2D
        rolw    $2D2E,z
        asrw    $2E2F,z
        rorw    $2F30,z
        lsrw    $3031,z
;;; 27 3x
        comw    $3132
        decw    $3233
        negw    $3334
        incw    $3435
        aslw    $3536
        lslw    $3536
        clrw    $3637
        tstw    $3738
        bclrw   $393A,#$3B3C
        bsetw   $3A3B,#$3C3D
        rolw    $3D3E
        asrw    $3E3F
        rorw    $3F40
        lsrw    $4041
;;; 27 4x
        suba    e,x
        adda    e,x
        sbca    e,x
        adca    e,x
        eora    e,x
        ldaa    e,x
        anda    e,x
        oraa    e,x
        cmpa    e,x
        bita    e,x
        staa    e,x
        nop
        tyx
        tzx
        tsx
;;; 27 5x
        suba    e,y
        adda    e,y
        sbca    e,y
        adca    e,y
        eora    e,y
        ldaa    e,y
        anda    e,y
        oraa    e,y
        cmpa    e,y
        bita    e,y
        staa    e,y
        txy
        tzy
        tsy
;;; 27 6x
        suba    e,z
        adda    e,z
        sbca    e,z
        adca    e,z
        eora    e,z
        ldaa    e,z
        anda    e,z
        oraa    e,z
        cmpa    e,z
        bita    e,z
        staa    e,z
        txz
        tyz
        tsz
;;; 27 7x
        come
        lded    $7273
        nege
        sted    $7475
        asle
        lsle
        clre
        tste
        rti
        ade
        sde
        xgde
        tde
        role
        asre
        rore
        lsre
;;; 27 8x
        subd    e,x
        addd    e,x
        sbcd    e,x
        adcd    e,x
        eord    e,x
        ldd     e,x
        andd    e,x
        ord     e,x
        cpd     e,x
        std     e,x
;;; 27 9x
        subd    e,y
        addd    e,y
        sbcd    e,y
        adcd    e,y
        eord    e,y
        ldd     e,y
        andd    e,y
        ord     e,y
        cpd     e,y
        std     e,y
;;; 27 Ax
        subd    e,z
        addd    e,z
        sbcd    e,z
        adcd    e,z
        eord    e,z
        ldd     e,z
        andd    e,z
        ord     e,z
        cpd     e,z
        std     e,z
;;; 27 Bx
        ldhi    $B1B2
        tedm
        tem
        tmxed
        tmer
        tmet
        aslm
        clrm
        pshmac
        pulmac
        asrm
        tekb
;;; 27 Cx
        subb    e,x
        addb    e,x
        sbcb    e,x
        adcb    e,x
        eorb    e,x
        ldab    e,x
        andb    e,x
        orab    e,x
        cmpb    e,x
        bitb    e,x
        stab    e,x
;;; 27 Dx
        subb    e,y
        addb    e,y
        sbcb    e,y
        adcb    e,y
        eorb    e,y
        ldab    e,y
        andb    e,y
        orab    e,y
        cmpb    e,y
        bitb    e,y
        stab    e,y
;;; 27 Ex
        subb    e,z
        addb    e,z
        sbcb    e,z
        adcb    e,z
        eorb    e,z
        ldab    e,z
        andb    e,z
        orab    e,z
        cmpb    e,z
        bitb    e,z
        stab    e,z
;;; 27 Fx
        comd
        lpstop
        negd
        wai
        asld
        lsld
        clrd
        tstd
        rts
        sxt
        lbsr    *-$0500
        tbek
        ted
        rold
        asrd
        rord
        lsrd
;;; 37 0x
        coma
        deca
        nega
        inca
        asla
        lsla
        clra
        tsta
        tba
        psha
        pula
        sba
        aba
        rola
        asra
        rora
        lsra
;;; 37 1x
        comb
        decb
        negb
        incb
        aslb
        lslb
        clrb
        tstb
        tab
        pshb
        pulb
        xgab
        cba
        rolb
        asrb
        rorb
        lsrb
;;; 37 2x
        swi
        daa
        ace
        aced
        mul
        emul
        emuls
        fmuls
        ediv
        edivs
        idiv
        fdiv
        tpd
        tdp
        tdmsk
;;; 37 3x
        sube    #$3132
        adde    #$3233
        sbce    #$3334
        adce    #$3435
        eore    #$3536
        lde     #$3637
        ande    #$3738
        ore     #$3839
        cpe     #$393A
        andp    #$3B3C
        clc
        cli
        clv
        orp     #$3C3D
        sec
        sei
        sev
        aix     #$3D3E
        dex
        inx
        aiy     #$3E3F
        dey
        iny
        aiz     #$3F40
        ais     #$4041
        des
        ins
;;; 37 4x
        sube    $4142,x
        adde    $4243,x
        sbce    $4344,x
        adce    $4445,x
        eore    $4546,x
        lde     $4647,x
        ande    $4748,x
        ore     $4849,x
        cpe     $494A,x
        ste     $4B4C,x
        xgex
        aex
        txs
        abx
;;; 37 5x
        sube    $5152,y
        adde    $5253,y
        sbce    $5354,y
        adce    $5455,y
        eore    $5556,y
        lde     $5657,y
        ande    $5758,y
        ore     $5859,y
        cpe     $595A,y
        ste     $5B5C,y
        xgey
        aey
        tys
        aby
;;; 37 6x
        sube    $6162,z
        adde    $6263,z
        sbce    $6364,z
        adce    $6465,z
        eore    $6566,z
        lde     $6667,z
        ande    $6768,z
        ore     $6869,z
        cpe     $696A,z
        ste     $6B6C,z
        xgez
        aez
        tzs
        abz
;;; 37 7x
        sube    $7172
        adde    $7273
        sbce    $7374
        adce    $7475
        eore    $7576
        lde     $7677
        ande    $7778
        ore     $7879
        cpe     $797A
        ste     $7B7C
        cpx     #$7D7E
        cpy     #$7E7F
        cpz     #$7F80
        cps     #$8081
;;; 37 8x
        lbra    *+$1118
        lbrn    *+$1218
        lbhi    *+$131A
        lbls    *+$141A
        lbcc    *+$151C
        lbhs    *+$151C
        lbcs    *+$161C
        lblo    *+$161C
        lbne    *+$171E
        lbeq    *+$181E
        lbvc    *+$1920
        lbvs    *+$1A20
        lbpl    *+$1B22
        lbmi    *+$1C22
        lbge    *+$1D24
        lblt    *+$1E24
        lbgt    *+$1F26
        lble    *+$2026
;;; 37 9x
        lbmv    *+$2128
        lbev    *+$2228
        tbxk
        tbyk
        tbzk
        tbsk
;;; 37 Ax
        bgnd
        txkb
        tykb
        tzkb
        tskb
;;; 37 Bx
        subd    #$B1B2
        addd    #$B2B3
        sbcd    #$B3B4
        adcd    #$B4B5
        eord    #$B5B6
        ldd     #$B6B7
        andd    #$B7B8
        ord     #$B8B9
        cpd     #$B9BA
        ldx     #$BDBE
        ldy     #$BEBF
        ldz     #$BFC0
        lds     #$C0C1
;;; 37 Cx
        subd    -$3E3E,x
        addd    -$3D3D,x
        sbcd    -$3C3C,x
        adcd    -$3B3B,x
        eord    -$3A3A,x
        ldd     -$3939,x
        andd    -$3838,x
        ord     -$3737,x
        cpd     -$3636,x
        std     -$3434,x
        xgdx
        adx
;;; 37 Dx
        subd    -$2E2E,y
        addd    -$2D2D,y
        sbcd    -$2C2C,y
        adcd    -$2B2B,y
        eord    -$2A2A,y
        ldd     -$2929,y
        andd    -$2828,y
        ord     -$2727,y
        cpd     -$2626,y
        std     -$2424,y
        xgdy
        ady
;;; 37 Ex
        subd    -$1E1E,z
        addd    -$1D1D,z
        sbcd    -$1C1C,z
        adcd    -$1B1B,z
        eord    -$1A1A,z
        ldd     -$1919,z
        andd    -$1818,z
        ord     -$1717,z
        cpd     -$1616,z
        std     -$1414,z
        xgdz
        adz
;;; 37 Fx
        subd    $F1F2
        addd    $F2F3
        sbcd    $F3F4
        adcd    $F4F5
        eord    $F5F6
        ldd     $F6F7
        andd    $F7F8
        ord     $F8F9
        cpd     $F9FA
        std     $FBFC
        tpa
        tap
        movb    $FF00,$0102
        movw    $0001,$0203
        end
;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
