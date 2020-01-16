	cpu     6809
        assume  dpr:$00

;;; 0X
	neg     $01
        com     $04
        lsr     $05
        ror     $07
        asr     $08
        lsl     $09
        asl     $09
        rol     $0a
        dec     $0b
        inc     $0d
        tst     $0e
        jmp     $0f
        clr     $10
;;; 1X
        nop
        sync
        lbra    *+$171b
        lbsr    *+$181c
        daa
        orcc    #$1b
        andcc   #$1d
        sex
        exg     y,d
        tfr     a,dp
;;; 2X
        bra     *+$23
        brn     *+$24
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
        leax    -15,y
        leay    -14,y
        leas    -13,y
        leau    -12,y
        pshs    y,x,b,cc
        puls    d,x,y
        pshu    y,x,b,a,cc
        pulu    dp,x,y
        rts
        abx
        rti
        cwai    #$3d
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
        neg	1,s
        com	4,s
        lsr	5,s
        ror	6,s
        asr	8,s
	lsl     9,s
        asl	9,s
        rol	10,s
        dec	11,s
        inc	13,s
        tst	14,s
        jmp     15,s
        clr	-16,s
;;; 7X
        neg	$7172
        com	$7475
        lsr	$7576
        ror	$7778
        asr	$7879
	lsl     $797a
        asl	$797a
        rol	$7a7b
        dec	$7b7c
        inc	$7d7e
        tst	$7e7f
        jmp     $7f80
        clr	$8081
;;; 8X
        suba    #$81
        cmpa    #$82
        sbca    #$83
        subd    #$8485
        anda    #$85
        bita    #$86
        lda     #$87
        eora    #$89
        adca    #$8a
        ora     #$8b
        adda    #$8c
        cmpx    #$8d8e
        bsr     $8e
        ldx     #$8f90
;;; 9X
        suba    $91
        cmpa    $92
        sbca    $93
        subd    $94
        anda    $95
        bita    $96
        lda     $97
        sta     $98
        eora    $99
        adca    $9a
        ora     $9b
        adda    $9c
        cmpx    $9d
        jsr     $9e
        ldx     $9f
        stx     $a0
;;; AX
        suba    ,y++
        cmpa    ,-y
        sbca    ,--y
        subd    ,y
        anda    b,y
        bita    a,y
        lda     [a,y]
        sta     -87,y
        eora    -21845,y
        adca    [-17733,y]
        ora     d,y
        adda    [d,y]
        cmpx    [,--y]
        jsr     ,y+
        ldx     [$a0a1]
        stx     [,y++]
;;; BX
        suba    $b1b2
        cmpa    $b2b3
        sbca    $b3b4
        subd    $b4b5
        anda    $b5b6
        bita    $b6b7
        lda     $b7b9
        sta     $b8b9
        eora    $b9ba
        adca    $babb
        ora     $bbbc
        adda    $bcbd
        cmpx    $bdbe
        jsr     $bebf
        ldx     $bfc0
        stx     $c0c1
;;; CX
        subb    #$c1
        cmpb    #$c2
        sbcb    #$c3
        addd    #$c4c5
        andb    #$c5
        bitb    #$c6
        ldb     #$c7
        eorb    #$c8
        adcb    #$c9
        orb     #$ca
        addb    #$cb
        ldd     #$cdce
        ldu     #$cfd0
;;; DX
        subb    $d1
        cmpb    $d2
        sbcb    $d3
        addd    $d4
        andb    $d5
        bitb    $d6
        ldb     $d7
        stb     $d8
        eorb    $d9
        adcb    $da
        orb     $db
        addb    $dc
        ldd     $dd
        std     $de
        ldu     $df
        stu     $e0
;;; EX
        subb    ,s++
        cmpb    ,-s
        sbcb    ,--y
        addd    ,s
        andb    b,s
        bitb    a,s
        ldb     [a,s]
        stb     -23,s
        eorb    -5397,s
        adcb    [-1285,s]
        orb     d,s
        addb    [d,s]
        ldd     d,s
        std     [,--s]
        ldu     [$a0a1]
        stu     [,s++]
;;; FX
        subb    $f1f2
        cmpb    $f2f3
        sbcb    $f3f4
        addd    $f4f5
        andb    $f5f6
        bitb    $f6f7
        ldb     $f7f8
        stb     $f8f9
        eorb    $f9fa
        adcb    $fafb
        orb     $fbfc
        addb    $fcfd
        ldd     $fdfe
        std     $feff
        ldu     $ff00
        stu     >$0001
;;; 102X
        lbrn    *+$2227
        lbhi    *+$2328
        lbls    *+$2429
        lbhs    *+$252a
        lblo    *+$26ab
        lbcc    *+$252a
        lbcs    *+$262b
        lbne    *+$272c
        lbeq    *+$282d
        lbvc    *+$292e
        lbvs    *+$2a2f
        lbpl    *+$2b30
        lbmi    *+$2c31
        lbge    *+$2d32
        lblt    *+$2e33
        lbgt    *+$2f34
        lble    *+$3035
;;; 103X
        swi2
;;; 108X
        cmpd    #$8485
        cmpy    #$8d8e
        ldy     #$8f90
;;; 109X
        cmpd    $94
        cmpy    $9d
        ldy     $9f
        sty     $a0
;;; 10AX
        cmpd    ,y
        cmpy    [,--y]
        ldy     [$a0a1]
        sty     [,y++]
;;; 10BX
        cmpd    $b4b5
        cmpy    $bdbe
        ldy     $bfc0
        sty     $c0c1
;;; 10CX
        lds     #$cfd0
;;; 10DX
        lds     $df
        sts     $e0
;;; 10EX
        lds     [$a0a1]
        sts     [,s++]
;;; 10FX
        lds     $ff10
        sts     >$0001
;;; 113X
        swi3
;;; 118X
        cmpu    #$8485
        cmps    #$8d8e
;;; 119X
        cmpu    $94
        cmps    $9d
;;; 11AX
        cmpu    [$11a4]
        cmps    [$11ad]
;;; 11BX
        cmpu    $b4b5
        cmps    $bdbe
