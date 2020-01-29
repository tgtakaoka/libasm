        cpu     68000

;;; 00XX
        ori.b   #$23,d1
        ori     #$3d,ccr
        ori.w   #$6789,d5
        ori.w   #$5152,(a0)
        ori     #$7d7e,sr
        ori.l   #$bcdef012,(a2)+
;;; 01XX
        btst    d0,d2
        btst    d0,-(a3)
        btst    d0,($7654).w
        btst    d0,($87654321).l
        movep.w ($0a0b,a1),d0
;;; 02XX
        andi.b  #$04,d3
        andi    #$3d,ccr
        andi.w  #$6789,d5
        andi.w  #$789a,(a6)
        andi    #$027d,sr
        andi.l  #$a8a9aaab,-(a7)
        andi.l  #$a8a9aaab,($12345678).l
;;; 03XX
        btst    d1,-(a1)
        bchg    d1,d5
        movep.l d1,(-$5679,a3)
;;; 04XX
        subi.b  #6,d5
        subi.w  #$5667,d5
        subi.w  #$789a,(a6)
        subi.l  #$04850486,d4
;;; 05XX
        bchg    d2,d3
        bchg    d2,-(a7)
        bchg    d2,($abcdef01).l
        movep.w d2,(-$5433,a1)
;;; 06XX
        addi.b  #8,d7
        addi.w  #$3210,(a4)
        addi.w  #$9abc,($ffffdef0).w
        addi.l  #$06870688,d6
;;; 07XX
        movep.l ($5868,a0),d3
        bchg    d3,-(a5)
        bchg    d3,(-$66,a7,d7.l)
        bclr    d3,(a2)+
;;; 08XX
        btst.l  #31,d0
        btst.b  #6,-(a4)
        bchg    #2,(a1)
        bclr    #3,(a2)+
        bset    #4,-(a3)
;;; 09XX
        movep.w ($0b0c,a2),d4
        bclr    d4,d7
        bclr    d4,(-$3211,a3)
;;; 0AXX
        eori.b  #$02,d1
        eori    #$3d,ccr
        eori.w  #$5152,(a0)
        eori    #$7d7e,sr
        eori.l  #$9a9b9c9d,(a1)+
;;; 0BXX
        movep.w ($0d0e,a4),d5
        bclr    d5,(-$789b,a1)
;;; 0CXX
        cmpi.b  #$02,d1
        cmpi.w  #$5152,(a0)
        cmpi.l  #$9a9b9c9d,(a1)+
;;; 0DXX
        movep.l d6,(-$5679,a3)
        bset    d6,($0123,a7)
;;; 0FXX
        movep.w ($0d0c,a6),d7
        bset    d7,(-$3457,a5)
;;; 1XXX
        move.b  (a1),-(a0)
        move.b  (-$66,a4,d7.l),d1
;;; 2XXX
        move.l  -(a2),d1
        movea.l ($4000,pc),a1
        move.l  d5,($6789,a1)
	move.l  #$12345678,($89abcde0).l
;;; 3XXX
        move.w  (a0),d1
        move.w  ($33,a3,d3),-(a1)
        movea.w (a6),a2
;; ;;; 40XX
        move    sr,d0
        negx.b  ($1000)
;; ;;; 42XX
        clr.b   ($1000).l
;; ;;; 44XX
        move    #$12,ccr
        neg.b   d0
;; ;;; 46XX
        move    #$3456,sr
        not.w   d0
;; ;;; 48XX
        ext.w   d0
        ext.l   d0
        nbcd    d0
        swap    d0
        pea     (a0)
        movem.w d0-d7/a1-a7,-(a0)
;;; 4AXX
        illegal
        tas     (a0)
        tst.l   d0
;;; 4CXX
        movem.l (a1)+,d7-a0/a6-a7
;;; 4EXX
        trap    #3
        link    a0,#10
        unlk    a0
        move    usp,a0
        move    a0,usp
        reset
        nop
        stop    #$1234
        rte
        rts
        trapv
        rtr
        jsr     ($1000).w
        jmp     ($2000).l
;; ;;; 4XXX
        lea     ($00,a1,d3.w),a0
        chk.w   #$22,d2     
;; ;;; 5XXX
        addq.l  #1,(a1)
        addq.w  #8,a2
        subq.b  #1,d0
        subq.l  #8,a7
        st      d0
        sf      d1
        shi     d2
        sls     d3
        scc     d4
        shs     d4
        scs     d5
        slo     d5
        sne     d6
        seq     d7
        svc     (a0)
        svs     (a1)+
        spl     -(a2)
        smi     (a3)
        sge     (a4)+
        slt     -(a5)
        sgt     (a6)
        sle     (a7)+
        dbt     d0,*+0
        dbf     d1,*+32768
        dbra    d1,*-32766
        dbhi    d2,*+2
        dbls    d3,*+3
        dbcc    d4,*+4
        dbhs    d4,*+4
        dbcs    d5,*+5
        dblo    d5,*+5
        dbne    d6,*+6
        dbeq    d7,*+7
        dbvc    d0,*+8
        dbvs    d1,*+9
        dbpl    d2,*+10
        dbmi    d3,*+11
        dbge    d4,*+12
        dblt    d5,*+13
        dbgt    d6,*+14
        dble    d7,*+15
;; ;;; 6XXX
        bra     *+128
        bsr     *-126
        bhi     *+130
        bls     *-128
        bcc     *+32768
        bhs     *-32766
        bcs     *+5
        blo     *+5
        bne     *+6
        beq     *+7
        bvc     *+8
        bvs     *+9
        bpl     *+10
        bmi     *+11
        bge     *+12
        blt     *+13
        bgt     *+14
        ble     *+15
;; ;;; 7XXX
        moveq   #1,d0
        moveq   #-1,d7
;; ;;; 8XXX
        divu.w  (a0),d0
        divs.w  d1,d2
        sbcd    d0,d1
        sbcd    -(a0),-(a1)
        or.w    (a0),d0
        or.l    d0,(a0)
;; ;;; 9XXX
        sub.b   (a0),d0
        sub.l   d0,(a0)
	subx.l  d0,d1
        subx.b  -(a1),-(a2)
        suba.w  (a0),a1
;; ;;; BXXX
        eor.b   d0,(a0)
        cmpm.l  (a0)+,(a1)+
        cmp.b   (a0),d0
        cmpa.w  d0,a0
;; ;;; CXXX
        mulu.w  d1,d2
        muls.w  (a0),d0
        abcd    d0,d1
        abcd    -(a0),-(a1)
        exg.l   d1,d2
        exg.l   a3,a4
        exg.l   d5,a6
        exg.l   a6,d5
        and.b   (a0),d0
        and.l   d0,(a1)
;; ;;; DXXX
        add.b   (a0),d0
        add.l   d0,(a0)
        addx.l  d0,d1
        addx.b  -(a1),-(a2)
        adda.w  (a0),a1
;; ;;; EXXX
        asr.w   (a0)
        asl     (a1)
        lsr.l   #8,d1
        lsl.b   #1,d0
        roxr.w  d1,d2
        roxl.l  d3,d4
        ror.w   (a3)+     
        rol     -(a3)
