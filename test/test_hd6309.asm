	cpu     6309
        assume  dpr:$00

;;; 0X
	neg     $01
        com     $03
        lsr     $04
        ror     $06
        asr     $07
        lsl     $08
        asl     $08
        rol     $09
        dec     $0a
        inc     $0c
        tst     $0d
        jmp     $0e
        clr     $0f
;;; 1X
        nop
        sync
        lbra    $16
        lbsr    $17
        daa
        orcc    #$1a
        andcc   #$1c
        sex
        exg     x,pc
        tfr     x,pc
;;; 2X
        bra     $20
        brn     $21
        bhi     $22
        bls     $23
        bcc     $24
        bhs     $24
        bcs     $25
        blo     $25
        bne     $26
        beq     $27
        bvc     $28
        bvs     $29
        bpl     $2a
        bmi     $2b
        bge     $2c
        blt     $2d
        bge     $2e
        ble     $2f
;;; 3X
        leax    $30,y
        leay    [$31,s]
        leas    [$32,pc]
        leau    $33,x
        pshs    y,x,b
        puls    cc,b,x,y
        pshu    y,x,d
        pulu    cc,a,b,x,y
        rts
        abx
        rti
        cwai    #$3c
        mul
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
        neg	$60,x
        com	$63,x
        lsr	$64,x
        ror	$66,x
        asr	$67,x
	lsl     $68,x
        asl	$68,x
        rol	$69,x
        dec	$6a,x
        inc	$6c,x
        tst	$6d,x
        jmp     $6e,x
        clr	$6f,x
;;; 7X
        neg	$7070
        com	$7373
        lsr	$7474
        ror	$7676
        asr	$7777
	lsl     $7878
        asl	$7878
        rol	$7979
        dec	$7a7a
        inc	$7c7c
        tst	$7d7d
        jmp     $7e7e
        clr	$7f7f
;;; 8X
        suba    #$80
        cmpa    #$81
        sbca    #$82
        subd    #$83
        anda    #$84
        bita    #$85
        lda     #$86
        eora    #$88
        adca    #$89
        ora     #$8a
        adda    #$8b
        cmpx    #$8c
        bsr     $8d
        ldx     #$8e
;;; 9X
        suba    $90
        cmpa    $91
        sbca    $92
        subd    $93
        anda    $94
        bita    $95
        lda     $96
        sta     $97
        eora    $98
        adca    $99
        ora     $9a
        adda    $9b
        cmpx    $9c
        jsr     $9d
        ldx     $9e
        stx     $9f
;;; AX
        suba    $a0,y
        cmpa    $a1,y
        sbca    $a2,y
        subd    $a3,y
        anda    $a4,y
        bita    $a5,y
        lda     $a6,y
        sta     $a7,y
        eora    $a8,y
        adca    $a9,y
        ora     $aa,y
        adda    $ab,y
        cmpx    $ac,y
        jsr     $ad,y
        ldx     $ae,y
        stx     $af,y
;;; BX
        suba    $b0b0
        cmpa    $b1b1
        sbca    $b2b2
        subd    $b3b3
        anda    $b4b4
        bita    $b5b5
        lda     $b6b6
        sta     $b7b7
        eora    $b8b8
        adca    $b9b9
        ora     $baba
        adda    $bbbb
        cmpx    $bcbc
        jsr     $bdbd
        ldx     $bebe
        stx     $bfbf
;;; CX
        subb    #$c0
        cmpb    #$c1
        sbcb    #$c2
        addd    #$c3
        andb    #$c4
        bitb    #$c5
        ldb     #$c6
        eorb    #$c8
        adcb    #$c9
        orb     #$ca
        addb    #$cb
        ldd     #$cc
        ldu     #$ce
;;; DX
        subb    $d0
        cmpb    $d1
        sbcb    $d2
        addd    $d3
        andb    $d4
        bitb    $d5
        ldb     $d6
        stb     $d7
        eorb    $d8
        adcb    $d9
        orb     $da
        addb    $db
        ldd     $dc
        std     $dd
        ldu     $de
        stu     $df
;;; EX
        subb    $e0,u
        cmpb    $e1,u
        sbcb    $e2,u
        addd    $e3,u
        andb    $e4,u
        bitb    $e5,u
        ldb     $e6,u
        stb     $e7,u
        eorb    $e8,u
        adcb    $e9,u
        orb     $ea,u
        addb    $eb,u
        ldd     $ec,u
        std     $ed,u
        ldu     $ee,u
        stu     $ef,u
;;; FX
        subb    $f0f0
        cmpb    $f1f1
        sbcb    $f2f2
        addd    $f3f3
        andb    $f4f4
        bitb    $f5f5
        ldb     $f6f6
        stb     $f7f7
        eorb    $f8f8
        adcb    $f9f9
        orb     $fafa
        addb    $fbfb
        ldd     $fcfc
        std     $fdfd
        ldu     $fefe
        stu     $ffff
;;; 102X
        lbrn    $1021
        lbhi    $1022
        lbls    $1023
        lbhs    $1024
        lbcc    $1024
        lblo    $1025
        lbcs    $1025
        lbne    $1026
        lbeq    $1027
        lbvc    $1028
        lbvs    $1029
        lbpl    $102a
        lbmi    $102b
        lbge    $102c
        lblt    $102d
        lbgt    $102e
        lble    $102f
;;; 103X
        swi2
;;; 108X
        cmpd    #$1083
        cmpy    #$108c
        ldy     #$108e
;;; 109X
        cmpd    $93
        cmpy    $9c
        ldy     $9e
        sty     $9f
;;; 10AX
        cmpd    $a3,pc
        cmpy    $ac,pc
        ldy     $ae,pc
        sty     $af,pc
;;; 10BX
        cmpd    $10b3
        cmpy    $10bc
        ldy     $10be
        sty     $10bf
;;; 10CX
        lds     #$10ce
;;; 10DX
        lds     $de
        sts     $df
;;; 10EX
        lds     $ee,s
        sts     $ef,s
;;; 10FX
        lds     $10fe
        sts     $10ff
;;; 113X
        swi3
;;; 118X
        cmpu    #$1183
        cmps    #$118c
;;; 119X
        cmpu    $93
        cmps    $9c
;;; 11AX
        cmpu    [$11a3]
        cmps    [$11ac]
;;; 11BX
        cmpu    $11b3
        cmps    $11bc

;;; HD6309

;;; 0X
        oim     #$01,$01
        aim     #$02,$02
        eim     #$05,$05
        tim     #$0b,$0b
;;; 6X
        oim     #$61,$61,w
        aim     #$62,[$62,w]
        eim     #$65,[$65,pc]
        tim     #$6b,$6b,w
;;; 7X
        oim     #$71,$7171
        aim     #$72,$7272
        eim     #$75,$7575
        tim     #$7b,$7b7b
;;; CX
        ldq     #$cdcdcdcd
;;; 103X
        addr    u,d
        adcr    u,x
        subr    u,y
        sbcr    u,u
        andr    u,s
        orr     u,pc
        eorr    u,w
        cmpr    u,v
        pshsw
        pulsw
        pshuw
        puluw
;;; 104X
        negd
        comd
        lsrd
        rord
        asrd
        lsld
        asld
        rold
        decd
        incd
        tstd
        clrd
;;; 105X
        comw
        lsrw
        rorw
        rolw
        decw
        incw
        tstw
        clrw
;;; 108X
        subw    #$1080
        cmpw    #$1081
        sbcd    #$1082
        andd    #$1084
        bitd    #$1085
        ldw     #$1086
        eord    #$1088
        adcd    #$1089
        ord     #$108a
        addw    #$108b
;;; 109X
        subw    $90
        cmpw    $91
        sbcd    $92
        andd    $94
        bitd    $95
        ldw     $96
        stw     $97
        eord    $98
        adcd    $99
        ord     $9a
        addw    $9b
;;; 10AX
        subw    $10a0,w
        cmpw    $10a1,w
        sbcd    $10a2,w
        andd    $10a4,w
        bitd    $10a5,w
        ldw     $10a6,w
        stw     $10a7,w
        eord    $10a8,w
        adcd    $10a9,w
        ord     $10aa,w
        addw    $10ab,w
;;; 10BX
        subw    $10b0
        cmpw    $10b1
        sbcd    $10b2
        andd    $10b4
        bitd    $10b5
        ldw     $10b6
        stw     $10b7
        eord    $10b8
        adcd    $10b9
        ord     $10ba
        addw    $10bb
;;; 10DX
        ldq     $dc
        stq     $dd
;;; 10EX
        ldq     $10ec,w
        stq     $10ed,w
;;; 10FX
        ldq     $10fc
        stq     $10fd
;;; 113X
        band    a.0,$30.7
        biand   b.1,$31.6
        bor     cc.2,$32.5
        bior    a.3,$33.4
        beor    b.4,$34.3
        bieor   cc.5,$35.2
        ldbt    a.6,$36.1
        stbt    b.7,$37.0
        tfm     x+,y+
        tfm     y-,u-
        tfm     u+,s
;        tfm     s,d+
        tfm     s,x+
        bitmd   #$3c
        ldmd    #$3d
;;; 114X
        come
        dece
        ince
        tste
        clre
;;; 115X
        comf
        decf
        incf
        tstf
        clrf
;;; 118X
        sube    #$80
        cmpe    #$81
        lde     #$86
        adde    #$8b
        divd    #$8d
        divq    #$118e
        muld    #$118f
;;; 119X
        sube    $90
        cmpe    $91
        lde     $96
        ste     $97
        adde    $9b
        divd    $9d
        divq    $9e
        muld    $9f
;;; 11AX
        sube    $11a0,w
        cmpe    $11a1,w
        lde     $11a6,w
        ste     $11a7,w
        adde    $11ab,w
        divd    $11ad,w
        divq    $11ae,w
        muld    $11af,w
;;; 11BX
        sube    $11b0
        cmpe    $11b1
        lde     $11b6
        ste     $11b7
        adde    $11bb
        divd    $11bd
        divq    $11be
        muld    $11bf
;;; 11CX
        subf    #$c0
        cmpf    #$c1
        ldf     #$c6
        addf    #$cb
;;; 11DX
        subf    $d0
        cmpf    $d1
        ldf     $d6
        stf     $d7
        addf    $db
;;; 11EX
        subf    $11e0,w
        cmpf    $11e1,w
        ldf     $11e6,w
        stf     $11e7,w
        addf    $11eb,w
;;; 11FX
        subf    $11f0
        cmpf    $11f1
        ldf     $11f6
        stf     $11f7
        addf    $11fb
