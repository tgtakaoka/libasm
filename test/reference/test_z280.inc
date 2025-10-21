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

;;; 0x
        ldw     bc, 0302H
        incw    bc
        decw    bc
;;; 1x
        ldw     de, 1312H
        incw    de
        decw    de
;;; 2x
        lda     hl, (2322H)
        ldw     hl, 2322H
        ldw     (2423H), hl
        incw    hl
        ldw     hl, (2C2BH)
        decw    hl
;;; 3x
        ldw     sp, 3332H
        incw    sp
        decw    sp
;;; CB3x
        tset    b
        tset    c
        tset    d
        tset    e
        tset    h
        tset    l
        tset    (hl)
        tset    a
;;; DD:CB:3x
        tset    (ix+35H)
;;; FD:CB:3x
        tset    (iy+35H)
;;; DD:0x
        ldw     (hl), 0302H
        incw    (hl)
        inc     (sp+0605H)
        dec     (sp+0706H)
        ld      (sp+0807H), 9
        decw    (hl)
        inc     (hl+ix)
        dec     (hl+ix)
        ld      (hl+ix), 0FH
;;; DD:1x
        ldw     (1312H), 1514H  ; DD:11:12:13:14:15
        incw    (1514H)
        inc     (hl+iy)
        dec     (hl+iy)
        ld      (hl+iy), 17H
        decw    (1D1CH)
        inc     (ix+iy)
        dec     (ix+iy)
        ld      (ix+iy), 1FH
;;; DD:2x
        jar     $+21H+3
        ldw     ix, 2322H
        lda     ix, (2322H)
        ldw     (2423H), ix
        incw    ix
        inc     ixh
        dec     ixh
        ld      ixh, 27H
        jaf     $+29H+3
        ldw     ix, (2C2BH)
        decw    ix
        inc     ixl
        dec     ixl
        ld      ixl, 2FH
;;; DD:3x
        ldw     <$+3332H+6>, 3534H
        incw    <$+3534H+4>
        decw    <$+3D3BH+4>
        inc     (3E3DH)
        dec     (3F3EH)
        ld      (403FH), 41H
;;; DD:4x
        ld      b, ixh
        ld      b, ixl
        ld      c, ixh
        ld      c, ixl
;;; DD:5x
        ld      d, ixh
        ld      d, ixl
        ld      e, ixh
        ld      e, ixl
;;; DD:6x
        ld      ixh, b
        ld      ixh, c
        ld      ixh, d
        ld      ixh, e
        ld      ixh, ixh
        ld      ixh, ixl
        ld      ixh, a
        ld      ixl, b
        ld      ixl, c
        ld      ixl, d
        ld      ixl, e
        ld      ixl, ixh
        ld      ixl, ixl
        ld      ixl, a
;;; DD:7x
        ld      a, (sp+7A79H)
        ld      a, (hl+ix)
        ld      a, (hl+iy)
        ld      a, (ix+iy)
        ld      a, ixh
        ld      a, ixl
;;; DD:8x
        add     a, (sp-7D7FH)
        add     a, (hl+ix)
        add     a, (hl+iy)
        add     a, (ix+iy)
        add     a, ixh
        add     a, ixl
        add     a, (8988H)
        adc     a, (sp-7577H)
        adc     a, (hl+ix)
        adc     a, (hl+iy)
        adc     a, (ix+iy)
        adc     a, ixh
        adc     a, ixl
        adc     a, (9190H)
;;; DD:9x
        sub     a, (sp-6D6FH)
        sub     a, (hl+ix)
        sub     a, (hl+iy)
        sub     a, (ix+iy)
        sub     a, ixh
        sub     a, ixl
        sub     a, (9998H)
        sbc     a, (sp-6567H)
        sbc     a, (hl+ix)
        sbc     a, (hl+iy)
        sbc     a, (ix+iy)
        sbc     a, ixh
        sbc     a, ixl
        sbc     a, (0A1A0H)
;;; DD:Ax
        and     a, (sp-5D5FH)
        and     a, (hl+ix)
        and     a, (hl+iy)
        and     a, (ix+iy)
        and     a, ixh
        and     a, ixl
        and     a, (0A9A8H)
        xor     a, (sp-5557H)
        xor     a, (hl+ix)
        xor     a, (hl+iy)
        xor     a, (ix+iy)
        xor     a, ixh
        xor     a, ixl
        xor     a, (0B1B0H)
;;; DD:Bx
        or      a, (sp-4D4FH)
        or      a, (hl+ix)
        or      a, (hl+iy)
        or      a, (ix+iy)
        or      a, ixh
        or      a, ixl
        or      a, (0B9B8H)
        cp      a, (sp-4547H)
        cp      a, (hl+ix)
        cp      a, (hl+iy)
        cp      a, (ix+iy)
        cp      a, ixh
        cp      a, ixl
        cp      a, (0C1C0H)
;;; DD:Cx
        pop     (hl)
        jp      nz, (hl)
        call    nz, (hl)
        push    (hl)
        jp      z, (hl)
        call    z, (hl)
        call    (hl)
;;; DD:Dx
        pop     (0D3D2H)
        jp      nc, (hl)
        call    nc, (hl)
        push    (0D7D6H)
        jp      c, (hl)
        call    c, (hl)
;;; DD:Ex
        jp      po, (hl)
        call    po, (hl)
        jp      pe, (hl)
        ex      hl, ix
        call    pe, (hl)
;;; DD:Fx
        pop     <$-0C0EH+4>
        jp      p, (hl)
        call    p, (hl)
        push    <$-080AH+4>
        jp      m, (hl)
        call    m, (hl)
;;; DD:ED:0x
        lda     ix, (sp+0403H)
        ld      ix, (sp+0605H)
        ldw     ix, (sp+0605H)
        ld      (sp+0706H), ix
        ldw     (sp+0706H), ix
        ld      bc, (ix+07H)
        ldw     bc, (ix+07H)
        ex      a, (sp+0908H)
        lda     ix, (hl+ix)
        ld      ix, (hl+ix)
        ldw     ix, (hl+ix)
        ld      (hl+ix), ix
        ldw     (hl+ix), ix
        ld      (ix+0FH), bc
        ldw     (ix+0FH), bc
        ex      a, (hl+ix)
;;; DD:ED:1x
        lda     ix, (hl+iy)
        ld      ix, (hl+iy)
        ldw     ix, (hl+iy)
        ld      (hl+iy), ix
        ldw     (hl+iy), ix
        ld      de, (ix+17H)
        ldw     de, (ix+17H)
        ex      a, (ix+iy)
        lda     ix, (ix+iy)
        ld      ix, (ix+iy)
        ldw     ix, (ix+iy)
        ld      (ix+iy), ix
        ldw     (ix+iy), ix
        ld      (ix+1FH), de
        ldw     (ix+1FH), de
        ex      a, (ix+iy)
;;; DD:ED:2x
        lda     ix, <$+2423H+5>
        ld      ix, <$+2625H+5>
        ldw     ix, <$+2625H+5>
        ld      <$+2726H+5>, ix
        ldw     <$+2726H+5>, ix
        ld      hl, (ix+27H)
        ldw     hl, (ix+27H)
        ex      a, ixh
        lda     ix, (ix+2C2BH)
        ld      ix, (ix+2E2DH)
        ldw     ix, (ix+2E2DH)
        ld      (ix+2F2EH), ix
        ldw     (ix+2F2EH), ix
        ld      (ix+2FH), hl
        ldw     (ix+2FH), hl
        ex      a, ixl
;;; DD:ED:3x
        lda     ix, (iy+3433H)
        ld      ix, (iy+3635H)
        ldw     ix, (iy+3635H)
        ld      (iy+3736H), ix
        ldw     (iy+3736H), ix
        ld      sp, (ix+37H)
        ldw     sp, (ix+37H)
        ex      a, (ix+38H)
        lda     ix, (hl+3C3BH)
        ld      ix, (hl+3E3DH)
        ldw     ix, (hl+3E3DH)
        ld      (hl+3F3EH), ix
        ldw     (hl+3F3EH), ix
        ld      (ix+3FH), sp
        ldw     (ix+3FH), sp
        ex      a, (4140H)
;;; DD:ED:4x
        in      (sp+4241H), (c)
        out     (c), (sp+4342H)
        sbc     ix, bc
        in      (hl+ix), (c)
        out     (c), (hl+ix)
        adc     ix, bc
;;; DD:ED:5x
        in      (hl+iy), (c)
        out     (c), (hl+iy)
        sbc     ix, de
        in      (ix+iy), (c)
        out     (c), (ix+iy)
        adc     ix, de
;;; DD:ED:6x
        in      ixh, (c)
        out     (c), ixh
        sbc     ix, ix
        ldctl   ix, (c)
        in      ixl, (c)
        out     (c), ixl
        adc     ix, ix
        add     ix, a
        ldctl   (c), ix
;;; DD:ED:7x
        sbc     ix, sp
        in      (7A79H), (c)
        out     (c), (7B7AH)
        adc     ix, sp
;;; DD:ED:8x
        ldud    a, (ix-79H)
        ldctl   ix, usp
        ldud    (ix-71H), a
        ldctl   usp, ix
;;; DD:ED:9x
        ldup    a, (ix-69H)
        ldup    (ix-61H), a
;;; DD:ED:Cx
        mult    a, (sp+0C2C1H)
        multu   a, (sp+0C3C2H)
        multw   hl, (hl)
        multuw  hl, (hl)
        div     hl, (sp+0C6C5H)
        divu    hl, (sp+0C7C6H)
        addw    hl, (hl)
        cpw     hl, (hl)
        mult    a, (hl+ix)
        multu   a, (hl+ix)
        divw    dehl, (hl)
        divuw   dehl, (hl)
        div     hl, (hl+ix)
        divu    hl, (hl+ix)
        subw    hl, (hl)
;;; DD:ED:Dx
        mult    a, (hl+iy)
        multu   a, (hl+iy)
        multw   hl, (0D4D3H)
        multuw  hl, (0D5D4H)
        div     hl, (hl+iy)
        divu    hl, (hl+iy)
        addw    hl, (0D8D7H)
        cpw     hl, (0D9D8H)
        mult    a, (ix+iy)
        multu   a, (ix+iy)
        divw    dehl, (0DCDBH)
        divuw   dehl, (0DDDCH)
        div     hl, (ix+iy)
        divu    hl, (ix+iy)
        subw    hl, (0E0DFH)
;;; DD:ED:Ex
        mult    a, ixh
        multu   a, ixh
        multw   hl, ix
        multuw  hl, ix
        div     hl, ixh
        divu    hl, ixh
        addw    hl, ix
        cpw     hl, ix
        mult    a, ixl
        multu   a, ixl
        divw    dehl, ix
        divuw   dehl, ix
        div     hl, ixl
        divu    hl, ixl
        subw    hl, ix
;;; DD:ED:Fx
        mult    a, (ix-0FH)
        multu   a, (ix-0EH)
        multw   hl, <$-0B0DH+5>
        multuw  hl, <$-0A0CH+5>
        div     hl, (ix-0BH)
        divu    hl, (ix-0AH)
        addw    hl, <$-0709H+5>
        cpw     hl, <$-0608H+5>
        mult    a, (0FAF9H)
        multu   a, (0FBFAH)
        divw    dehl, <$-0305H+5>
        divuw   dehl, <$-0204H+5>
        div     hl, (0FEFDH)
        divu    hl, (0FFFEH)
        subw    hl, <$+00FFH+5>
;;; ED:0x
        lda     hl, (sp+0403H)
        ld      (sp+0504H), a
        ld      hl, (sp+0605H)
        ldw     hl, (sp+0605H)
        ld      (sp+0706H), hl
        ldw     (sp+0706H), hl
        ld      bc, (hl)
        ldw     bc, (hl)
        ex      a, b
        lda     hl, (hl+ix)
        ld      (hl+ix), a
        ld      hl, (hl+ix)
        ldw     hl, (hl+ix)
        ld      (hl+ix), hl
        ldw     (hl+ix), hl
        ld      (hl), bc
        ldw     (hl), bc
        ex      a, c
;;; ED:1x
        lda     hl, (hl+iy)
        ld      (hl+iy), a
        ld      hl, (hl+iy)
        ldw     hl, (hl+iy)
        ld      (hl+iy), hl
        ldw     (hl+iy), hl
        ld      de, (hl)
        ldw     de, (hl)
        ex      a, d
        lda     hl, (ix+iy)
        ld      (ix+iy), a
        ld      hl, (ix+iy)
        ldw     hl, (ix+iy)
        ld      (ix+iy), hl
        ldw     (ix+iy), hl
        ld      (hl), de
        ldw     (hl), de
        ex      a, e
;;; ED:2x
        lda     hl, <$+2423H+4>
        ld      <$+2524H+4>, a
        ld      hl, <$+2625H+4>
        ldw     hl, <$+2625H+4>
        ld      <$+2726H+4>, hl
        ldw     <$+2726H+4>, hl
        ld      hl, (hl)
        ldw     hl, (hl)
        ex      a, h
        lda     hl, (ix+2C2BH)
        ld      (ix+2D2CH), a
        ld      hl, (ix+2E2DH)
        ldw     hl, (ix+2E2DH)
        ld      (ix+2F2EH), hl
        ldw     (ix+2F2EH), hl
        ld      (hl), hl
        ldw     (hl), hl
        ex      a, l
;;; ED:3x
        lda     hl, (iy+3433H)
        ld      (iy+3534H), a
        ld      hl, (iy+3635H)
        ldw     hl, (iy+3635H)
        ld      (iy+3736H), hl
        ldw     (iy+3736H), hl
        ld      sp, (hl)
        ldw     sp, (hl)
        ex      a, (hl)
        lda     hl, (hl+3C3BH)
        ld      (hl+3D3CH), a
        ld      hl, (hl+3E3DH)
        ldw     hl, (hl+3E3DH)
        ld      (hl+3F3EH), hl
        ldw     (hl+3F3EH), hl
        ld      (hl), sp
        ldw     (hl), sp
        ex      a, a
;;; ED:4x
        ldw     (4544H), bc
        neg     a
        ldw     bc, (4D4CH)
        neg     hl
        im      3
;;; ED:5x
        ldw     (5554H), de
        retil
        ldw     de, (5D5CH)
;;; ED:6x
        exts    a
        pcache
        ldctl   hl, (c)
        exts    hl
        add     hl, a
        ldctl   (c), hl
;;; ED:7x
        tsti    (c)
        sc      7372H
        ldw     (7574H), sp
        di      78H
        ldw     sp, (7D7CH)
        ei      00H
;;; ED:8x
        iniw
        outiw
        ldud    a, (hl)
        ldctl   hl, usp
        indw
        outdw
        ldud    (hl), a
        ldctl   usp, hl
;;; ED:9x
        inirw
        otirw
        ldup    a, (hl)
        indrw
        otdrw
        ldup    (hl), a
;;; ED:Bx
        in      hl, (c)
        inw     hl, (c)
        out     (c), hl
        outw    (c), hl
;;; ED:Cx
        mult    a, b
        multu   a, b
        multw   hl, bc
        multuw  hl, bc
        div     hl, b
        divu    hl, b
        addw    hl, bc
        cpw     hl, bc
        mult    a, c
        multu   a, c
        divw    dehl, bc
        divuw   dehl, bc
        div     hl, c
        divu    hl, c
        subw    hl, bc
;;; ED:Dx
        mult    a, d
        multu   a, d
        multw   hl, de
        multuw  hl, de
        div     hl, d
        divu    hl, d
        addw    hl, de
        cpw     hl, de
        mult    a, e
        multu   a, e
        divw    dehl, de
        divuw   dehl, de
        div     hl, e
        divu    hl, e
        subw    hl, de
;;; ED:Ex
        mult    a, h
        multu   a, h
        multw   hl, hl
        multuw  hl, hl
        div     hl, h
        divu    hl, h
        addw    hl, hl
        cpw     hl, hl
        mult    a, l
        multu   a, l
        divw    dehl, hl
        divuw   dehl, hl
        div     hl, l
        divu    hl, l
        subw    hl, hl
        ex      h, l
;;; ED:Fx
        mult    a, (hl)
        multu   a, (hl)
        multw   hl, sp
        multuw  hl, sp
        div     hl, (hl)
        divu    hl, (hl)
        addw    hl, sp
        cpw     hl, sp
        mult    a, a
        multu   a, a
        divw    dehl, sp
        divuw   dehl, sp
        div     hl, a
        divu    hl, a
        subw    hl, sp
;;; F9
        ldw     sp, hl
;;; FD:0x
        incw    (ix+0504H)
        inc     <$+0605H+4>
        dec     <$+0706H+4>
        ld      <$+0807H+5>, 9
        decw    (ix+0D0CH)
        inc     (ix+0E0DH)
        dec     (ix+0F0EH)
        ld      (ix+100FH), 11H
;;; FD:1x
        incw    (iy+1514H)
        inc     (iy+1615H)
        dec     (iy+1716H)
        ld      (iy+1817H), 19H
        decw    (iy+1D1CH)
        inc     (hl+1E1DH)
        dec     (hl+1F1EH)
        ld      (hl+201FH), 21H
;;; FD:2x
        ldw     iy, 2322H
        lda     iy, (2322H)
        ldw     (2423H), iy
        incw    iy
        inc     iyh
        dec     iyh
        ld      iyh, 27H
        ldw     iy, (2C2BH)
        decw    iy
        inc     iyl
        dec     iyl
        ld      iyl, 2FH
;;; FD:4x
        ld      b, iyh
        ld      b, iyl
        ld      c, iyh
        ld      c, iyl
;;; FD:5x
        ld      d, iyh
        ld      d, iyl
        ld      e, iyh
        ld      e, iyl
;;; FD:6x
        ld      iyh, b
        ld      iyh, c
        ld      iyh, d
        ld      iyh, e
        ld      iyh, iyh
        ld      iyh, iyl
        ld      iyh, a
        ld      iyl, b
        ld      iyl, c
        ld      iyl, d
        ld      iyl, e
        ld      iyl, iyh
        ld      iyl, iyl
        ld      iyl, a
;;; FD:7x
        ld      a, <$+7A78H+4>
        ld      a, (ix+7B7AH)
        ld      a, (iy+7C7BH)
        ld      a, (hl+7D7CH)
        ld      a, iyh
        ld      a, iyl
;;; FD:8x
        add     a, <$+0201H+4>
        add     a, (ix-7C7EH)
        add     a, (iy-7B7DH)
        add     a, (hl-7A7CH)
        add     a, iyh
        add     a, iyl
        adc     a, <$+0A09H+4>
        adc     a, (ix-7476H)
        adc     a, (iy-7375H)
        adc     a, (hl-7274H)
        adc     a, iyh
        adc     a, iyl
;;; FD:9x
        sub     a, <$+0201H+4>
        sub     a, (ix-6C6EH)
        sub     a, (iy-6B6DH)
        sub     a, (hl-6A6CH)
        sub     a, iyh
        sub     a, iyl
        sbc     a, <$+0A09H+4>
        sbc     a, (ix-6466H)
        sbc     a, (iy-6365H)
        sbc     a, (hl-6264H)
        sbc     a, iyh
        sbc     a, iyl
;;; FD:Ax
        and     a, <$+0201H+4>
        and     a, (ix-5C5EH)
        and     a, (iy-5B5DH)
        and     a, (hl-5A5CH)
        and     a, iyh
        and     a, iyl
        xor     a, <$+0A09H+4>
        xor     a, (ix-5456H)
        xor     a, (iy-5355H)
        xor     a, (hl-5254H)
        xor     a, iyh
        xor     a, iyl
;;; FD:Vx
        or      a, <$+0201H+4>
        or      a, (ix-4C4EH)
        or      a, (iy-4B4DH)
        or      a, (hl-4A4CH)
        or      a, iyh
        or      a, iyl
        cp      a, <$+0A09H+4>
        cp      a, (ix-4446H)
        cp      a, (iy-4345H)
        cp      a, (hl-4244H)
        cp      a, iyh
        cp      a, iyl
;;; FD:Cx
        jp      nz, <$+0403H+4>
        jp      <$+0504H+4>
        call    nz, <$+0605H+4>
        jp      z, <$+0D0CH+4>
        call    z, <$+0E0DH+4>
        call    <$+0F0EH+4>
;;; FD:Dx
        jp      nc, <$+0403H+4>
        call    nc, <$+0605H+4>
        jp      c, <$+0D0CH+4>
        call    c, <$+0E0DH+4>
;;; FD:Ex
        jp      nv, <$+0403H+4>
        call    nv, <$+0605H+4>
        jp      v, <$+0D0CH+4>
        ex      hl, iy
        call    v, <$+0E0DH+4>
;;; FD:Fx
        jp      p, <$-0B0DH+4>
        call    p, <$-090BH+4>
        jp      m, <$-0305H+4>
        call    m, <$-0103H+4>
;;; FD:ED:0x
        lda     iy, (sp+0403H)
        ld      iy, (sp+0605H)
        ldw     iy, (sp+0605H)
        ld      (sp+0706H), iy
        ldw     (sp+0706H), iy
        ld      bc, (iy+07H)
        ldw     bc, (iy+07H)
        ex      a, <$+090AH+5>
        lda     iy, (hl+ix)
        ld      iy, (hl+ix)
        ldw     iy, (hl+ix)
        ld      (hl+ix), iy
        ldw     (hl+ix), iy
        ld      (iy+0FH), bc
        ldw     (iy+0FH), bc
        ex      a, (ix+1110H)
;;; FD:ED:1x
        lda     iy, (hl+iy)
        ld      iy, (hl+iy)
        ldw     iy, (hl+iy)
        ld      (hl+iy), iy
        ldw     (hl+iy), iy
        ld      de, (iy+17H)
        ldw     de, (iy+17H)
        ex      a, (iy+1918H)
        lda     iy, (ix+iy)
        ld      iy, (ix+iy)
        ldw     iy, (ix+iy)
        ld      (ix+iy), iy
        ldw     (ix+iy), iy
        ld      (iy+1FH), de
        ldw     (iy+1FH), de
        ex      a, (hl+2120H)
;;; FD:ED:2x
        lda     iy, <$+2423H+5>
        ld      iy, <$+2625H+5>
        ldw     iy, <$+2625H+5>
        ld      <$+2726H+5>, iy
        ldw     <$+2726H+5>, iy
        ld      hl, (iy+27H)
        ldw     hl, (iy+27H)
        ex      a, iyh
        lda     iy, (ix+2C2BH)
        ld      iy, (ix+2E2DH)
        ldw     iy, (ix+2E2DH)
        ld      (ix+2F2EH), iy
        ldw     (ix+2F2EH), iy
        ld      (iy+2FH), hl
        ldw     (iy+2FH), hl
        ex      a, iyl
;;; FD:ED:3x
        lda     iy, (iy+3433H)
        ld      iy, (iy+3635H)
        ldw     iy, (iy+3635H)
        ld      (iy+3736H), iy
        ldw     (iy+3736H), iy
        ld      sp, (iy+37H)
        ldw     sp, (iy+37H)
        ex      a, (iy+38H)
        lda     iy, (hl+3C3BH)
        ld      iy, (hl+3E3DH)
        ldw     iy, (hl+3E3DH)
        ld      (hl+3F3EH), iy
        ldw     (hl+3F3EH), iy
        ld      (iy+3FH), sp
        ldw     (iy+3FH), sp
        ex      a, (4140H)
;;; FD:ED:4x
        in      <$+4241H+5>, (c)
        out     (c), <$+4342H+5>
        sbc     iy, bc
        in      (ix+4A49H), (c)
        out     (c), (ix+4B4AH)
        adc     iy, bc
;;; FD:ED:5x
        in      (iy+5251H), (c)
        out     (c), (iy+5352H)
        sbc     iy, de
        in      (hl+5A59H), (c)
        out     (c), (hl+5B5AH)
        adc     iy, de
;;; FD:ED:6x
        in      iyh, (c)
        out     (c), iyh
        sbc     iy, iy
        ldctl   iy, (c)
        in      iyl, (c)
        out     (c), iyl
        adc     iy, iy
        add     iy, a
        ldctl   (c), iy
;;; FD:ED:7x
        sbc     iy, sp
        adc     iy, sp
;;; FD:ED:8x
        ldud    a, (iy-79H)
        ldctl   iy, usp
        ldud    (iy-71H), a
        ldctl   usp, iy
;;; FD:ED:9x
        ldup    a, (iy-69H)
        ldup    (iy-61H), a
;;; FD:ED:Cx
        mult    a, <$+0201H+5>
        multu   a, <$+0302H+5>
        multw   hl, (ix-3B3DH)
        multuw  hl, (ix-3A3CH)
        div     hl, <$+0605H+5>
        divu    hl, <$+0706H+5>
        addw    hl, (ix-3739H)
        cpw     hl, (ix-3638H)
        mult    a, (ix-3537H)
        multu   a, (ix-3436H)
        divw    dehl, (ix-3335H)
        divuw   dehl, (ix-3234H)
        div     hl, (ix-3133H)
        divu    hl, (ix-3032H)
        subw    hl, (ix-2F31H)
;;; FD:ED:Dx
        mult    a, (iy-2D2FH)
        multu   a, (iy-2C2EH)
        multw   hl, (iy-2B2DH)
        multuw  hl, (iy-2A2CH)
        div     hl, (iy-292BH)
        divu    hl, (iy-282AH)
        addw    hl, (iy-2729H)
        cpw     hl, (iy-2628H)
        mult    a, (iy-2527H)
        multu   a, (iy-2426H)
        divw    dehl, (iy-2325H)
        divuw   dehl, (iy-2224H)
        div     hl, (iy-2123H)
        divu    hl, (iy-2022H)
        subw    hl, (iy+1F21H)
;;; FD:ED:Ex
        mult    a, iyh
        multu   a, iyh
        multw   hl, iy
        multuw  hl, iy
        div     hl, iyh
        divu    hl, iyh
        addw    hl, iy
        cpw     hl, iy
        mult    a, iyl
        multu   a, iyl
        divw    dehl, iy
        divuw   dehl, iy
        div     hl, iyl
        divu    hl, iyl
        subw    hl, iy
;;; FD:ED:Fx
        mult    a, (iy-0FH)
        multu   a, (iy-0EH)
        multw   hl, 0F4F3H
        multuw  hl, 0F5F4H
        div     hl, (iy-0BH)
        divu    hl, (iy-0AH)
        addw    hl, 0F8F7H
        cpw     hl, 0F9F8H
        mult    a, 0F9H
        multu   a, 0FAH
        divw    dehl, 0FCFBH
        divuw   dehl, 0FDFCH
        div     hl, 0FDH
        divu    hl, 0FEH
        subw    hl, 00FFH

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
