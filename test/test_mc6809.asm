	cpu     6809
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
        leas    [$32,pcr]
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
        cmpd    $a3,pcr
        cmpy    $ac,pcr
        ldy     $ae,pcr
        sty     $af,pcr
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

        end
;;; comment
