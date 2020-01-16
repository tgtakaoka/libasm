	include "test_mc6809.asm"

	cpu     6309
        assume  dpr:$00

;;; HD6309
        org     $02b0

;;; 0X
        oim     #$02,$03
        aim     #$03,$04
        eim     #$06,$07
        tim     #$0c,$0d
;;; 1X
        tfr     w,v
        exg     e,f
;;; 6X
        oim     #$62,3,s
        aim     #$63,4,s
        eim     #$66,7,s
        tim     #$6c,13,s
;;; 7X
        oim     #$72,$7374
        aim     #$73,$7475
        eim     #$76,$7778
        tim     #$7c,$7d7e
;;; CX
        ldq     #$cecfd0d1
;;; 103X
        addr    u,x
        adcr    u,y
        subr    u,u
        sbcr    u,s
        andr    u,pc
        orr     u,w
        eorr    u,v
        cmpr    e,f
        cmpr    v,d
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
        subw    #$8182
        cmpw    #$8283
        sbcd    #$8384
        andd    #$8586
        bitd    #$8788
        ldw     #$8788
        eord    #$898a
        adcd    #$8a8b
        ord     #$8b8c
        addw    #$8c8d
;;; 109X
        subw    $91
        cmpw    $92
        sbcd    $93
        andd    $95
        bitd    $96
        ldw     $97
        stw     $98
        eord    $99
        adcd    $9a
        ord     $9b
        addw    $9c
;;; 10AX
        subw    ,y++
        cmpw    ,-y
        sbcd    ,--y
        andd    b,y
        bitd    a,y
        ldw     e,y
        stw     -87,y
        eord    -21845,y
        adcd    f,y
        ord     d,y
        addw    *-111,pcr
;;; 10BX
        subw    $b1b2
        cmpw    $b2b3
        sbcd    $b3b4
        andd    $b5b6
        bitd    $b6b7
        ldw     $b7b8
        stw     $b8b9
        eord    $b9ba
        adcd    $babb
        ord     $bbbc
        addw    $bcbd
;;; 10DX
        ldq     $dd
        stq     $de
;;; 10EX
        ldq     ,--w
        stq     w,s
;;; 10FX
        ldq     $fdfe
        stq     $feff
;;; 113X
        band    cc.1,$32.6
        biand   cc.2,$33.6
        bor     a.2,$52.0
        bior    a.4,$55.0
        beor    b.4,$c4.6
        bieor   b.5,$a5.2
        ldbt    cc.7,$38.6
        stbt    cc.0,$39.7
        tfm     x+,y+
        tfm     y-,u-
        tfm     u+,s
        tfm     s,d+
        bitmd   #$3d
        ldmd    #$3e
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
        sube    #$81
        cmpe    #$82
        lde     #$87
        adde    #$8c
        divd    #$8e
        divq    #$8f90
        muld    #$9091
;;; 119X
        sube    $91
        cmpe    $92
        lde     $97
        ste     $98
        adde    $9c
        divd    $9e
        divq    $9f
        muld    $a0
;;; 11AX
        sube    ,y++
        cmpe    ,-y
        lde     e,y
        ste     -87,y
        adde    f,y
        divd    w,y
        divq    $b0b1,w
        muld    [,y++]
;;; 11BX
        sube    $b1b2
        cmpe    $b2b3
        lde     $b7b8
        ste     $b8b9
        adde    $bcbd
        divd    $bebf
        divq    $bfc0
        muld    $c0c1
;;; 11CX
        subf    #$c1
        cmpf    #$c2
        ldf     #$c7
        addf    #$cc
;;; 11DX
        subf    $d1
        cmpf    $d2
        ldf     $d7
        stf     $d8
        addf    $de
;;; 11EX
        subf    ,s++
        cmpf    ,-s
        ldf     e,s
        stf     -23,s
        addf    f,s
;;; 11FX
        subf    $f1f2
        cmpf    $f2f3
        ldf     $f7f8
        stf     $f8f9
        addf    $fcfd

        end
;;; comment
