        cpu     8080

;;; 0X
        nop
        lxi     b,0101H
        stax    b
        inx     b
        inr     b
        dcr     b
        mvi     b,06H
        rlc
        dad     b
        ldax    b
        dcx     b
        inr     c
        dcr     c
        mvi     c,0eH
        rrc
;;; 1X
        lxi     d,1111H
        stax    d
        inx     d
        inr     d
        dcr     d
        mvi     d,16H
        ral
        dad     d
        ldax    d
        dcx     d
        inr     e
        dcr     e
        mvi     e,1eH
        rar
;;; 2X
        lxi     h,2121H
        shld    2222H
        inx     h
        inr     h
        dcr     h
        mvi     h,26H
        daa
        dad     h
        lhld    2a2aH
        dcx     h
        inr     l
        dcr     l
        mvi     l,2eH
        cma
;;; 3X
        lxi     sp,3131H
        sta     3232H
        inx     sp
        inr     m
        dcr     m
        mvi     m,36H
        stc
        dad     sp
        lda     3a3aH
        dcx     sp
        inr     a
        dcr     a
        mvi     a,3eH
        cmc
;;; 4X
        mov     b,b
        mov     b,c
        mov     b,d
        mov     b,e
        mov     b,h
        mov     b,l
        mov     b,m
        mov     b,a
        mov     c,b
        mov     c,c
        mov     c,d
        mov     c,e
        mov     c,h
        mov     c,l
        mov     c,m
        mov     c,a
;;; 5X
        mov     d,b
        mov     d,c
        mov     d,d
        mov     d,e
        mov     d,h
        mov     d,l
        mov     d,m
        mov     d,a
        mov     e,b
        mov     e,c
        mov     e,d
        mov     e,e
        mov     e,h
        mov     e,l
        mov     e,m
        mov     e,a
;;; 6X
        mov     h,b
        mov     h,c
        mov     h,d
        mov     h,e
        mov     h,h
        mov     h,l
        mov     h,m
        mov     h,a
        mov     l,b
        mov     l,c
        mov     l,d
        mov     l,e
        mov     l,h
        mov     l,l
        mov     l,m
        mov     l,a
;;; 7X
        mov     m,b
        mov     m,c
        mov     m,d
        mov     m,e
        mov     m,h
        mov     m,l
        hlt
        mov     m,a
        mov     a,b
        mov     a,c
        mov     a,d
        mov     a,e
        mov     a,h
        mov     a,l
        mov     a,m
        mov     a,a
;;; 8X
        add     b
        add     c
        add     d
        add     e
        add     h
        add     l
        add     m
        add     a
        adc     b
        adc     c
        adc     d
        adc     e
        adc     h
        adc     l
        adc     m
        adc     a
;;; 9X
        sub     b
        sub     c
        sub     d
        sub     e
        sub     h
        sub     l
        sub     m
        sub     a
        sbb     b
        sbb     c
        sbb     d
        sbb     e
        sbb     h
        sbb     l
        sbb     m
        sbb     a
;;; AX
        ana     b
        ana     c
        ana     d
        ana     e
        ana     h
        ana     l
        ana     m
        ana     a
        xra     b
        xra     c
        xra     d
        xra     e
        xra     h
        xra     l
        xra     m
        xra     a
;;; BX
        ora     b
        ora     c
        ora     d
        ora     e
        ora     h
        ora     l
        ora     m
        ora     a
        cmp     b
        cmp     c
        cmp     d
        cmp     e
        cmp     h
        cmp     l
        cmp     m
        cmp     a
;;; CX
        rnz
        pop     b
        jnz     0c2c2H
        jmp     0c3c3H
        cnz     0c4c4H
        push    b
        adi     0c6H
        rst     0
        rz
        ret
        jz      0cacaH
        cz      0ccccH
        call    0cdcdH
        aci     0ceH
        rst     1
;;; DX
        rnc
        pop     d
        jnc     0d2d2H
        out     0d3h
        cnc     0d4d4H
        push    d
        sui     0d6H
        rst     2
        rc
        jc      0dadaH
        in      0dbH
        cc      0dcdcH
        sbi     0deH
        rst     3
;;; EX
        rpo
        pop     h
        jpo     0e2e2H
        xthl
        cpo     0e4e4H
        push    h
        ani     0e6H
        rst     4
        rpe
        pchl
        jpe     0eaeaH
        xchg
        cpe     0ececH
        xri     0eeH
        rst     5
;;; FX
        rp
        pop     psw
        jp      0f2f2H
        di
        cp      0f4f4H
        push    psw
        ori     0f6H
        rst     6
        rm
        sphl
        jm      0fafaH
        ei
        cm      0fcfcH
        cpi     0feH
        rst     7
