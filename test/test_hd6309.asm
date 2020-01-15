	include "test_mc6809.asm"

	cpu     6309
        assume  dpr:$00

;;; HD6309
        org     $02f0

;;; 0X
        oim     #$01,$01
        aim     #$02,$02
        eim     #$05,$05
        tim     #$0b,$0b
;;; 6X
        oim     #$61,$61,w
        aim     #$62,[$62,w]
        eim     #$65,[$65,pcr]
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
        tfm     s,d+
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

        end
;;; comment
