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
        cpu     68HC12
        org     $0100
;;; 0x
        bgnd
        mem
        iny
        dey
        dbeq    x,*+$09
        jmp     5,x
        bsr     *+$0A
        inx
        dex
        rtc
        rti
        bset    $0D,x,#$0E
        bclr    $0E,x,#$0F
        brset   $0F,x,#$10,*+$15
        brclr   -16,x,#$11,*+$16
;;; 1x
        andcc   #$11
        clc
        cli
        clv
        ediv
        mul
        emul
        orcc    #$15
        sec
        sei
        sev
        jsr     -10,x
        mina    -6,x
        leay    -6,x
        aby
        leax    -5,x
        abx
        leas    -4,x
        des
        ins
        bset    $1D1E,#$1F
        bclr    $1E1F,#$20
        brset   $1F20,#$21,*+$27
        brclr   $2021,#$22,*+$28
;;; 2X
        bra     *+$23
        bhi     *+$25
        bls     *+$26
        bcc     *+$27
        bcs     *+$28
        bhs     *+$27
        blo     *+$28
        bne     *+$29
        beq     *+$2A
        bvc     *+$2B
        bvs     *+$2C
        bpl     *+$2D
        bmi     *+$2E
        bge     *+$2F
        blt     *+$30
        bgt     *+$31
        ble     *+$32
;;; 3x
        pulx
        puly
        pula
        pulb
        pshx
        pshy
        psha
        pshb
        pulc
        pshc
        puld
        pshd
        rts
        wai
        swi
;;; 4x
        nega
        coma
        inca
        deca
        lsra
        rola
        rora
        asra
        asla
        lsla
        lsrd
        call    $4B4C,$4D
        call    12,y,$4D
        bset    $4D,#$4E
        bclr    $4E,#$4F
        brset   $4F,#$50,*+$55
        brclr   $50,#$51,*+$56
;;; 5x
        negb
        comb
        incb
        decb
        lsrb
        rolb
        rorb
        asrb
        aslb
        lslb
        asld
        lsld
        staa    $5B
        stab    $5C
        std     $5D
        sty     $5E
        stx     $5F
        sts     $60
;;; 6x
        neg     2,+y
        com     3,+y
        inc     4,+y
        dec     5,+y
        lsr     6,+y
        rol     7,+y
        ror     8,+y
        asr     8,-y
        asl     7,-y
        lsl     7,-y
        clr     6,-y
        staa    5,-y
        stab    4,-y
        std     3,-y
        sty     2,-y
        stx     1,-y
        sts     1,y+
;;; 7x
        neg     $7172
        com     $7273
        inc     $7374
        dec     $7475
        lsr     $7576
        rol     $7677
        ror     $7778
        asr     $7879
        asl     $797A
        lsl     $797A
        clr     $7A7B
        staa    $7B7C
        stab    $7C7D
        std     $7D7E
        sty     $7E7F
        stx     $7F80
        sts     $8081
;;; 8x
        suba    #$81
        cmpa    #$82
        sbca    #$83
        subd    #$8485
        anda    #$85
        bita    #$86
        ldaa    #$87
        clra
        eora    #$89
        adca    #$8A
        oraa    #$8B
        adda    #$8C
        cpd     #$8D8E
        cpy     #$8E8F
        cpx     #$9F90
        cps     #$9091
;;; 9x
        suba    $91
        cmpa    $92
        sbca    $93
        subd    $94
        anda    $95
        bita    $96
        ldaa    $97
        tsta
        eora    $99
        adca    $9A
        oraa    $9B
        adda    $9C
        cpd     $9D
        cpy     $9E
        cpx     $9F
        cps     $A0
;;; Ax
        suba    2,+sp
        cmpa    3,+sp
        sbca    4,+sp
        subd    5,+sp
        anda    6,+sp
        bita    7,+sp
        ldaa    8,+sp
        nop
        eora    7,-sp
        adca    6,-sp
        oraa    5,-sp
        adda    4,-sp
        cpd     3,-sp
        cpy     2,-sp
        cpx     1,-sp
        cps     1,sp+
;;; Bx
        suba    $B1B2
        cmpa    $B2B3
        sbca    $B3B4
        subd    $B485
        anda    $B5B6
        bita    $B6B7
        ldaa    $B7B8
        exg     d,sp
        eora    $B9BA
        adca    $BABB
        oraa    $BBBC
        adda    $BCBD
        cpd     $BD8E
        cpy     $BE8F
        cpx     $BFC0
        cps     $C0C1
;;; Cx
        subb    #$C1
        cmpb    #$C2
        sbcb    #$C3
        addd    #$C4C5
        andb    #$C5
        bitb    #$C6
        ldab    #$C7
        clrb
        eorb    #$C9
        adcb    #$CA
        orab    #$CB
        addb    #$CC
        ldd     #$CDCE
        ldy     #$CECF
        ldx     #$CFD0
        lds     #$D0D1
;;; Dx
        subb    $D1
        cmpb    $D2
        sbcb    $D3
        addd    $D4
        andb    $D5
        bitb    $D6
        ldab    $D7
        tstb
        eorb    $D9
        adcb    $DA
        orab    $DB
        addb    $DC
        ldd     $DD
        ldy     $DE
        ldx     $DF
        lds     $E0
;;; Ex
        subb    -$1E,x
        cmpb    $E3E4,x
        sbcb    [$E4E5,x]
        addd    a,x
        andb    b,x
        bitb    d,x
        ldab    [d,x]
        tst     $E9,y
        eorb    -$16,y
        adcb    $EBEC,y
        orab    [$ECED,y]
        addb    a,y
        ldd     b,y
        ldy     d,y
        ldx     [d,y]
        lds     $F1,sp
;;; Fx
        subb    $F1F2
        cmpb    $F2F3
        sbcb    $F3F4
        addd    $F4F5
        andb    $F5F6
        bitb    $F6F7
        ldab    $F7F8
        tst     $F8F9
        eorb    $F9FA
        adcb    $FAFB
        orab    $FBFC
        addb    $FCFD
        ldd     $FDFE
        ldy     $FEFF
        ldx     $FF00
        lds     $1001
;;; 04 xx
        dbeq    a,*+$04
        dbeq    b,*+$05
        dbeq    d,*+$08
        dbeq    x,*+$09
        dbeq    y,*+$0A
        dbeq    sp,*+$0B
        dbeq    a,*-$EC
        dbeq    b,*-$EB
        dbeq    d,*-$E8
        dbeq    x,*-$E7
        dbeq    y,*-$E6
        dbeq    sp,*-$E5
        dbne    a,*+$24
        dbne    b,*+$25
        dbne    d,*+$28
        dbne    x,*+$29
        dbne    y,*+$2A
        dbne    sp,*+$2B
        dbne    a,*-$CC
        dbne    b,*-$CB
        dbne    d,*-$C8
        dbne    x,*-$C7
        dbne    y,*-$C6
        dbne    sp,*-$C5
        tbeq    a,*+$44
        tbeq    b,*+$45
        tbeq    d,*+$48
        tbeq    x,*+$49
        tbeq    y,*+$4A
        tbeq    sp,*+$4B
        tbeq    a,*-$AC
        tbeq    b,*-$AB
        tbeq    d,*-$A8
        tbeq    x,*-$A7
        tbeq    y,*-$A6
        tbeq    sp,*-$A5
        tbne    a,*+$64
        tbne    b,*+$65
        tbne    d,*+$68
        tbne    x,*+$69
        tbne    y,*+$6A
        tbne    sp,*+$6B
        tbne    a,*-$8C
        tbne    b,*-$8B
        tbne    d,*-$88
        tbne    x,*-$87
        tbne    y,*-$86
        tbne    sp,*-$85
        ibeq    a,*+$84
        ibeq    b,*+$85
        ibeq    d,*+$88
        ibeq    x,*+$89
        ibeq    y,*+$8A
        ibeq    sp,*+$8B
        ibeq    a,*-$6C
        ibeq    b,*-$6B
        ibeq    d,*-$68
        ibeq    x,*-$67
        ibeq    y,*-$66
        ibeq    sp,*-$65
        ibne    a,*+$A4
        ibne    b,*+$A5
        ibne    d,*+$A8
        ibne    x,*+$A9
        ibne    y,*+$AA
        ibne    sp,*+$AB
        ibne    a,*-$4C
        ibne    b,*-$4B
        ibne    d,*-$48
        ibne    x,*-$47
        ibne    y,*-$46
        ibne    sp,*-$45
;;; B7 xx
        tfr     a,a
        tfr     a,b
        tfr     a,ccr
        tap
        sex     a,d
        sex     a,x
        sex     a,y
        sex     a,sp
        tfr     b,a
        tfr     b,b
        tfr     b,ccr
        sex     b,d
        sex     b,x
        sex     b,y
        sex     b,sp
        tfr     ccr,a
        tpa
        tfr     ccr,b
        tfr     ccr,ccr
        sex     ccr,d
        sex     ccr,x
        sex     ccr,y
        sex     ccr,sp
        tfr     d,d
        tfr     d,x
        tfr     d,y
        tfr     d,sp
        tfr     x,d
        tfr     x,x
        tfr     x,y
        tfr     x,sp
        txs
        tfr     y,d
        tfr     y,x
        tfr     y,y
        tfr     y,sp
        tys
        tfr     sp,d
        tfr     sp,x
        tsx
        tfr     sp,y
        tsy
        tfr     sp,sp
        exg     a,a
        exg     a,b
        exg     a,ccr
        exg     b,a
        exg     b,b
        exg     b,ccr
        exg     ccr,a
        exg     ccr,b
        exg     ccr,ccr
        exg     d,d
        exg     d,x
        xgdx
        exg     d,y
        xgdy
        exg     d,sp
        exg     x,d
        exg     x,x
        exg     x,y
        exg     x,y
        exg     y,d
        exg     y,x
        exg     y,y
        exg     y,sp
        exg     sp,d
        exg     sp,x
        exg     sp,y
        exg     sp,sp
;;; 18 0x
        movw    #$0203,1,x
        movw    $0304,2,x
        movw    3,x,4,x
        movw    #$0405,$0607
        movw    $0506,$0708
        movw    6,x,$0708
        aba
        daa
        movb    #$0A,9,x
        movb    $0B0C,10,x
        movb    11,x,12,x
        movb    #$0C,$0D0E
        movb    $0D0E,$0F10
        movb    14,x,$0F10
        tab
        tba
;;; 18 1x
        idiv
        fdiv
        emacs   $1314
        emuls
        edivs
        idivs
        sba
        cba
        maxa    -7,x
        mina    -6,x
        emaxd   -5,x
        emind   -4,x
        maxm    -3,x
        minm    -2,x
        emaxm   -1,x
        eminm   1,+x
;;; 18 2x
        lbra    *+$2126
        lbrn    *+$2227
        lbhi    *+$2328
        lbls    *+$2429
        lbcc    *+$252A
        lbcs    *+$262B
        lbhs    *+$252A
        lblo    *+$262B
        lbne    *+$272C
        lbeq    *+$282D
        lbvc    *+$292E
        lbvs    *+$2A2F
        lbpl    *+$2B30
        lbmi    *+$2C31
        lbge    *+$2D32
        lblt    *+$2E33
        lbgt    *+$2F34
        lble    *+$3035
;;; 18 3x
        trap    $30
        trap    $31
        trap    $32
        trap    $33
        trap    $34
        trap    $35
        trap    $36
        trap    $37
        trap    $38
        trap    $39
        rev
        revw
        wav
        tbl     2,x-
        stop
        etbl    0,y
;;; 18 xx
        trap    $40
        trap    $FF
;;; indexing
        ldaa    0,x
        ldaa    15,x
        ldaa    -16,x
        ldaa    -1,x
        ldaa    1,+x
        ldaa    8,+x
        ldaa    8,-x
        ldaa    1,-x
        ldaa    1,x+
        ldaa    8,x+
        ldaa    8,x-
        ldaa    1,x-
        ldaa    0,y
        ldaa    15,y
        ldaa    -16,y
        ldaa    -1,y
        ldaa    1,+y
        ldaa    8,+y
        ldaa    8,-y
        ldaa    1,-y
        ldaa    1,y+
        ldaa    8,y+
        ldaa    8,y-
        ldaa    1,y-
        ldaa    0,sp
        ldaa    15,sp
        ldaa    -16,sp
        ldaa    -1,sp
        ldaa    1,+sp
        ldaa    8,+sp
        ldaa    8,-sp
        ldaa    1,-sp
        ldaa    1,sp+
        ldaa    8,sp+
        ldaa    8,sp-
        ldaa    1,sp-
        ldaa    *+2,pc
        ldaa    *+17,pc
        ldaa    *-14,pc
        ldaa    *+1,pc
        ldaa    $FF,x
        ldaa    -$100,x
        ldaa    $E3E4,x
        ldaa    [$E4E5,x]
        ldaa    a,x
        ldaa    b,x
        ldaa    d,x
        ldaa    [d,x]
        ldaa    $FF,y
        ldaa    -$100,y
        ldaa    $EBEC,y
        ldaa    [$ECED,y]
        ldaa    a,y
        ldaa    b,y
        ldaa    d,y
        ldaa    [d,y]
        ldaa    $FF,sp
        ldaa    -$100,sp
        ldaa    $F3F4,sp
        ldaa    [$F4F5,sp]
        ldaa    a,sp
        ldaa    b,sp
        ldaa    d,sp
        ldaa    [d,sp]
        ldaa    *+$102,pc
        ldaa    *-$FD,pc
        ldaa    *-$400,pc
        ldaa    [*-$2FF,pc]
        ldaa    a,pc
        ldaa    b,pc
        ldaa    d,pc
        ldaa    [d,pc]
        end
;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
