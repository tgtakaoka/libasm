;;; Copyright 2020 Tadashi G. Takaoka
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

;;; 0X
        nop
        lxi     b,0302H
        stax    b
        inx     b
        inr     b
        dcr     b
        mvi     b,07H
        rlc
        dad     b
        ldax    b
        dcx     b
        inr     c
        dcr     c
        mvi     c,0fH
        rrc
;;; 1X
        lxi     d,1312H
        stax    d
        inx     d
        inr     d
        dcr     d
        mvi     d,17H
        ral
        dad     d
        ldax    d
        dcx     d
        inr     e
        dcr     e
        mvi     e,1fH
        rar
;;; 2X
        lxi     h,2322H
        shld    2423H
        inx     h
        inr     h
        dcr     h
        mvi     h,27H
        daa
        dad     h
        lhld    2c2bH
        dcx     h
        inr     l
        dcr     l
        mvi     l,2fH
        cma
;;; 3X
        lxi     sp,3332H
        sta     3433H
        inx     sp
        inr     m
        dcr     m
        mvi     m,37H
        stc
        dad     sp
        lda     3c3bH
        dcx     sp
        inr     a
        dcr     a
        mvi     a,3fH
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
        jnz     0c4c3H
        jmp     0c5c4H
        cnz     0c6c5H
        push    b
        adi     0c7H
        rst     0
        rz
        ret
        jz      0cccbH
        cz      0cecdH
        call    0cfceH
        aci     0cfH
        rst     1
;;; DX
        rnc
        pop     d
        jnc     0d4d3H
        out     0d4h
        cnc     0d6d5H
        push    d
        sui     0d7H
        rst     2
        rc
        jc      0dcdbH
        in      0dcH
        cc      0deddH
        sbi     0dfH
        rst     3
;;; EX
        rpo
        pop     h
        jpo     0e4e3H
        xthl
        cpo     0e6e5H
        push    h
        ani     0e7H
        rst     4
        rpe
        pchl
        jpe     0ecebH
        xchg
        cpe     0eeedH
        xri     0efH
        rst     5
;;; FX
        rp
        pop     psw
        jp      0f4f3H
        di
        cp      0f6f5H
        push    psw
        ori     0f7H
        rst     6
        rm
        sphl
        jm      0fcfbH
        ei
        cm      0fefdH
        cpi     0ffH
        rst     7

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
