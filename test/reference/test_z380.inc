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

;;; CB 3x
        ex      b, b'
        ex      c, c'
        ex      d, d'
        ex      e, e'
        ex      h, h'
        ex      l, l'
        ex      a, a'
;;; DD 0x
        ld      (bc), ix
        ld      bc, de
        ld      ix, (bc)
        ld      ix, bc
        ld      bc, ix
        ld      bc, (bc)
        ld      bc, (de)
        ld      bc, (hl)
;;; DD 1x
        djnz    $+1215H
        ld      (de), ix
        ld      de, de
        ld      ix, (de)
        ld      ix, de
        jr      $+1A1DH
        ld      de, ix
        ld      de, (bc)
        ld      de, (de)
        ld      de, (hl)
;;; DD 2x
        jr      nz, $+2225H
        inc     ixu
        dec     ixu
        ld      ixu, 27H
        ld      ix, iy
        jr      z, $+2A2DH
        inc     ixl
        dec     ixl
        ld      ixl, 2FH
        cplw    hl
;;; DD 3x
        jr      nc, $+3235H
        ld      (hl), ix
        ld      hl, de
        ld      ix, (hl)
        ld      ix, hl
        jr      c, $+3A3DH
        ld      hl, ix
        ld      hl, (bc)
        ld      hl, (de)
        swap    ix
        ld      hl, (hl)
;;; DD 4x
        inw     bc, (c)
        outw    (c), bc
        ld      b, ixu
        ld      b, ixl
        ldw     i, hl
        ld      c, ixu
        ld      c, ixl
;;; DD 5x
        inw     de, (c)
        outw    (c), de
        ld      d, ixu
        ld      d, ixl
        ldw     hl, i
        ld      e, ixu
        ld      e, ixl
;;; DD 6x
        ld      ixu, b
        ld      ixu, c
        ld      ixu, d
        ld      ixu, e
        ld      ixu, ixu
        ld      ixu, ixl
        ld      ixu, a
        ld      ixl, b
        ld      ixl, c
        ld      ixl, d
        ld      ixl, e
        ld      ixl, ixu
        ld      ixl, ixl
        ld      ixl, a
;;; DD 7x
        inw     hl, (c)
        outw    (c), hl
        ld      a, ixu
        ld      a, ixl
;;; DD 8x
        add     a, ixu
        add     a, ixl
        addw    hl, ix
        adc     a, ixu
        adc     a, ixl
        adcw    hl, ix
;;; DD 9x
        sub     a, ixu
        sub     a, ixl
        subw    hl, ix
        sbc     a, ixu
        sbc     a, ixl
        sbcw    hl, ix
;;; DD Ax
        and     a, ixu
        and     a, ixl
        andw    hl, ix
        xor     a, ixu
        xor     a, ixl
        xorw    hl, ix
;;; DD Bx
        or      a, ixu
        or      a, ixl
        orw     hl, ix
        cp      a, ixu
        cp      a, ixl
        cpw     hl, ix
;;; DD Cx
        calr    nz, $-3937H
        addw    hl, (ix-39H)
        ldctl   sr, a
        ldctl   sr, 0CBH
        calr    z, $-312FH
        calr    $-302EH
        adcw    hl, (ix-31H)
        mtest
;;; DD Dx
        ldctl   a, xsr
        calr    nc, $-2927H
        subw    hl, (ix-29H)
        ldctl   xsr, a
        exxx
        ldctl   xsr, 0DBH
        calr    c, $-211FH
        sbcw    hl, (ix-21H)
;;; DD Ex
        calr    nv, $-1917H
        andw    hl, (ix-19H)
        calr    v, $-110FH
        xorw    hl, (ix-11H)
;;; DD Fx
        di      0F4H
        calr    p, $-0907H
        orw     hl, (ix-09H)
        setc    lw
        ei      0FCH
        calr    m, $-00FFH
        cpw     hl, (ix-1)
        resc    lw
;;; DD CB 0x
        ld      bc, (sp+0)
        rlcw    (ix+1)
        ld      bc, (ix+2)
        ld      (sp+8), bc
        rrcw    (ix+9)
        ld      (ix+10), bc
;;; DD CB 1x
        ld      de, (sp+10H)
        rlw     (ix+11H)
        ld      de, (ix+12H)
        ld      (sp+18H), de
        rrw     (ix+19H)
        ld      (ix+1AH), de
;;; DD CB 2x
        ld      ix, (sp+20H)
        slaw    (ix+21H)
        ld      iy, (ix+22H)
        ld      (sp+28H), ix
        sraw    (ix+29H)
        ld      (ix+2AH), iy
;;; DD CB 3x
        ld      hl, (sp+30H)
        ld      hl, (ix+32H)
        ld      (sp+38H), hl
        srlw    (ix+39H)
        ld      (ix+3AH), hl
;;; DD CB 9x
        multw   hl, (ix-6FH)
        multuw  hl, (ix-67H)
;;; DD CB Bx
        divuw   hl, (ix-47H)
;;; ED 0x
        ld      bc, bc
        ex      bc, ix
        ex      bc, de
        ldw     (bc), 0807H
        ex      a, b
        ex      bc, iy
        ex      bc, hl
        swap    bc
        ex      a, c
;;; ED 1x
        ld      bc, de
        ex      de, ix
        ldw     (de), 1817H
        ex      a, d
        ex      de, iy
        swap    de
        ex      a, e
;;; ED 2x
        ex      ix, iy
        ex      a, l
;;; ED 3x
        in0     (31H)
        ld      bc, hl
        ex      hl, ix
        ldw     (hl), 8938H
        ex      a, (hl)
        ex      hl, iy
        swap    hl
        ex      a, a
;;; ED 4x
        im      3
;;; ED 5x
        negw    hl
        retb
;;; ED 6x
        db      0edh, 63h, 64h, 65h ; ldw (6564H), hl
        exts    a
        db      0edh, 6bh, 6ch, 6dh ; ldw hl, (6D6CH)
;;; ED 7x
        out     (c), 72H
        extsw   hl
;;; ED 8x
        add     sp, 8483H
        addw    hl, bc
        addw    hl, de
        addw    hl, 8887H
        addw    hl, hl
        adcw    hl, bc
        adcw    hl, de
        adcw    hl, 908FH
        adcw    hl, hl
;;; ED 9x
        sub     sp, 9493H
        subw    hl, bc
        subw    hl, de
        subw    hl, 9897H
        subw    hl, hl
        sbcw    hl, bc
        sbcw    hl, de
        sbcw    hl, 0A09FH
        sbcw    hl, hl
;;; ED Ax
        andw    hl, bc
        andw    hl, de
        andw    hl, 0A8A7H
        andw    hl, hl
        xorw    hl, bc
        xorw    hl, de
        xorw    hl, 0B0AFH
        xorw    hl, hl
;;; ED Bx
        orw     hl, bc
        orw     hl, de
        orw     hl, 0B8B7H
        orw     hl, hl
        cpw     hl, bc
        cpw     hl, de
        cpw     hl, 0C0BFH
        cpw     hl, hl
;;; ED Cx
        ldctl   hl, sr
        pop     sr
        calr    nz, $-38H
        push    sr
        add     hl, (0C8C7H)
        ldctl   sr, hl
        calr    z, $-30H
        calr    $-2FH
        btest
;;; ED Dx
        ldctl   a, dsr
        outa    (0D5D4H), a
        calr    nc, $-28H
        sub     hl, (0D8D7H)
        ldctl   dsr, a
        exall
        ldctl   dsr, 0DBH
        ina     a, (0DDDCH)
        calr    c, $-20H
;;; ED Ex
        ldiw
        iniw
        outiw
        calr    nv, $-18H
        lddw
        indw
        outdw
        calr    v, $-10H
;;; ED Fx
        ldirw
        inirw
        otirw
        calr    ns, $-08H
        setc    lck
        lddrw
        indrw
        otdrw
        calr    s, $
        resc    lck
;;; ED CB 0x
        rlcw    bc
        rlcw    de
        rlcw    (hl)
        rlcw    hl
        rlcw    ix
        rlcw    iy
        rrcw    bc
        rrcw    de
        rrcw    (hl)
        rrcw    hl
        rrcw    ix
        rrcw    iy
;;; ED CB 1x
        rlw     bc
        rlw     de
        rlw     (hl)
        rlw     hl
        rlw     ix
        rlw     iy
        rrw     bc
        rrw     de
        rrw     (hl)
        rrw     hl
        rrw     ix
        rrw     iy
;;; ED CB 2x
        slaw    bc
        slaw    de
        slaw    (hl)
        slaw    hl
        slaw    ix
        slaw    iy
        sraw    bc
        sraw    de
        sraw    (hl)
        sraw    hl
        sraw    ix
        sraw    iy
;;; ED CB 3x
        ex      bc, bc'
        ex      de, de'
        ex      hl, hl'
        ex      ix, ix'
        ex      iy, iy'
        srlw    bc
        srlw    de
        srlw    (hl)
        srlw    hl
        srlw    ix
        srlw    iy
;;; ED CB 9x
        multw   hl, bc
        multw   hl, de
        multw   hl, hl
        multw   hl, ix
        multw   hl, iy
        multw   hl, 9998H
        multuw  hl, bc
        multuw  hl, de
        multuw  hl, hl
        multuw  hl, ix
        multuw  hl, iy
        multuw  hl, 0A1A0H
;;; ED CB Ax
        divuw   hl, bc
        divuw   hl, de
        divuw   hl, hl
        divuw   hl, ix
        divuw   hl, iy
        divuw   hl, 0C1C0H
;;; FD 0x
        ld      (bc), iy
        ld      bc, hl
        ld      iy, (bc)
        ld      iy, bc
        ld      bc, iy
        ld      (bc), bc
        ld      (de), bc
        ld      (hl), bc
;;; FD 1x
        djnz    $+131216H
        ld      (de), iy
        ld      de, hl
        ld      iy, (de)
        ld      iy, de
        jr      $+1B1A1EH
        ld      de, iy
        ld      (bc), de
        ld      (de), de
        ld      (hl), de
;;; FD 2x
        jr      nz, $+232226H
        inc     iyu
        dec     iyu
        ld      iyu, 27H
        ld      iy, ix
        jr      z, $+2B2A2EH
        inc     iyl
        dec     iyl
        ld      iyl, 2FH
;;; FD 3x
        jr      nc, $+333236H
        ld      (hl), iy
        ld      hl, hl
        ld      iy, (hl)
        ld      iy, hl
        jr      c, $+3B3A3EH
        ld      hl, iy
        ld      (bc), hl
        ld      (de), hl
        swap    iy
        ld      (hl), hl
;;; FD 4x
        ld      b, iyu
        ld      b, iyl
        ld      c, iyu
        ld      c, iyl
;;; FD 5x
        ld      d, iyu
        ld      d, iyl
        ld      e, iyu
        ld      e, iyl
;;; FD 6x
        ld      iyu, b
        ld      iyu, c
        ld      iyu, d
        ld      iyu, e
        ld      iyu, iyu
        ld      iyu, iyl
        ld      iyu, a
        ld      iyl, b
        ld      iyl, c
        ld      iyl, d
        ld      iyl, e
        ld      iyl, iyu
        ld      iyl, iyl
        ld      iyl, a
;;; FD 7x
        outw    (c), 7B7AH
        ld      a, iyu
        ld      a, iyl
;;; FD 8x
        add     a, iyu
        add     a, iyl
        addw    hl, iy
        adc     a, iyu
        adc     a, iyl
        adcw    hl, iy
;;; FD 9x
        sub     a, iyu
        sub     a, iyl
        subw    hl, iy
        sbc     a, iyu
        sbc     a, iyl
        sbcw    hl, iy
;;; FD Ax
        and     a, iyu
        and     a, iyl
        andw    hl, iy
        xor     a, iyu
        xor     a, iyl
        xorw    hl, iy
;;; FD Bx
        or      a, iyu
        or      a, iyl
        orw     hl, iy
        cp      a, iyu
        cp      a, iyl
        cpw     hl, iy
;;; FD Cx
        calr    nz, $-383936H
        addw    hl, (iy-39H)
        calr    z, $-30312EH
        calr    $-2F302DH
        adcw    hl, (iy-31H)
;;; FD Dx
        ldctl   a, ysr
        outaw   (0D5D4H), HL
        calr    nc, $-282926H
        subw    hl, (iy-29H)
        ldctl   ysr, a
        exxy
        ldctl   ysr, 0DBH
        inaw    hl, (0DDDCH)
        calr    c, $-20211EH
        sbcw    hl, (iy-21H)
;;; FD Ex
        calr    nv, $-181916H
        andw    hl, (iy-19H)
        calr    v, $-10110EH
        xorw    hl, (iy-11H)
;;; FD Fx
        calr    ns, $-080906H
        push    0F7F6H
        orw     hl, (iy-09H)
        setc    xm
        calr    s, $-0F00FEH
        cpw     hl, (iy-1)
;;; FD CB 0x
        rlcw    (iy+1)
        ld      bc, (iy+2)
        rrcw    (iy+9)
        ld      (iy+0AH), bc
;;; FD CB 1x
        rlw     (iy+11H)
        ld      de, (iy+12H)
        rrw     (iy+19H)
        ld      (iy+1AH), de
;;; FD CB 2x
        ld      iy, (sp+20H)
        slaw    (iy+21H)
        ld      ix, (iy+22H)
        ld      (sp+28H), iy
        sraw    (iy+29H)
        ld      (iy+2AH), ix
;;; FD CB 3x
        ld      hl, (iy+32H)
        srlw    (iy+39H)
        ld      (iy+3AH), hl
;;; FD CB 9x
        multw   hl, (iy-6FH)
        multuw  hl, (iy-67H)
;;; FD CB Bx
        divuw   hl, (iy-47H)
;;; E-1
        add     hl, (123456H)
        add     sp, 123456H
        call    123456H
        call    c, 123456H
        call    m, 123456H
        call    nc, 123456H
        call    nz, 123456H
        call    p, 123456H
        call    pe, 123456H
        call    po, 123456H
        jp      123456H
        jp      c, 123456H
        jp      m, 123456H
        jp      nc, 123456H
        jp      nz, 123456H
        jp      p, 123456H
        jp      pe, 123456H
        jp      po, 123456H
        sub     hl, (123456H)
        sub     sp, 123456H
;;; E-2
        ld      (123456H), bc
        ld      (123456H), de
        ld      (123456H), hl
        db      0DDH,0C3H,0EDH,63H,56H,34H,12H  ; ldw (123456H), bc
        ld      (123456H), ix
        ld      (123456H), iy
        ld      (123456H), sp
        ld      (ix+1234H), bc
        ld      (ix+1234H), de
        ld      (ix+1234H), hl
        ld      (ix+1234H), iy
        ld      (iy+1234H), bc
        ld      (iy+1234H), de
        ld      (iy+1234H), hl
        ld      (iy+1234H), ix
        ld      (sp+1234H), bc
        ld      (sp+1234H), de
        ld      (sp+1234H), hl
        ld      (sp+1234H), ix
        ld      (sp+1234H), iy
        ld      bc, (123456H)
        ld      bc, (ix+1234H)
        ld      bc, (iy+1234H)
        ld      bc, (sp+1234H)
        ld      de, (123456H)
        ld      de, (ix+1234H)
        ld      de, (iy+1234H)
        ld      de, (sp+1234H)
        ld      hl, (123456H)
        db      0DDH,0C3H,0EDH,6BH,56H,34H,12H ; ldw hl,(123456H)
        ld      hl, (ix+1234H)
        ld      hl, (iy+1234H)
        ld      hl, (sp+1234H)
        ld      ix, (123456H)
        ld      ix, (iy+1234H)
        ld      ix, (sp+1234H)
        ld      iy, (123456H)
        ld      iy, (ix+1234H)
        ld      iy, (sp+1234H)
        ld      sp, (123456H)
        ldw     (bc), 123456H
        ldw     (de), 123456H
        ldw     (hl), 123456H
;;; E-3
        ld      bc, 123456H
        ld      de, 123456H
        ld      hl, 123456H
        ld      ix, 123456H
        ld      iy, 123456H
        ld      sp, 123456H
        push    123456H
;;; E-4
        adc     a, (ix+1234H)
        adc     a, (iy+1234H)
        adcw    (ix+1234H)
        adcw    (iy+1234H)
        adcw    hl, (ix+1234H)
        adcw    hl, (iy+1234H)
        add     a, (ix+1234H)
        add     a, (iy+1234H)
        addw    (ix+1234H)
        addw    (iy+1234H)
        addw    hl, (ix+1234H)
        addw    hl, (iy+1234H)
        and     (ix+1234H)
        and     (iy+1234H)
        and     a, (ix+1234H)
        and     a, (iy+1234H)
        andw    (ix+1234H)
        andw    (iy+1234H)
        andw    hl, (ix+1234H)
        andw    hl, (iy+1234H)
        bit     0, (ix+1234H)
        bit     0, (iy+1234H)
        bit     1, (ix+1234H)
        bit     1, (iy+1234H)
        bit     2, (ix+1234H)
        bit     2, (iy+1234H)
        bit     3, (ix+1234H)
        bit     3, (iy+1234H)
        bit     4, (ix+1234H)
        bit     4, (iy+1234H)
        bit     5, (ix+1234H)
        bit     5, (iy+1234H)
        bit     6, (ix+1234H)
        bit     6, (iy+1234H)
        bit     7, (ix+1234H)
        bit     7, (iy+1234H)
        cp      (ix+1234H)
        cp      (iy+1234H)
        cp      a, (ix+1234H)
        cp      a, (iy+1234H)
        cpw     (ix+1234H)
        cpw     (iy+1234H)
        cpw     hl, (ix+1234H)
        cpw     hl, (iy+1234H)
        dec     (ix+1234H)
        dec     (iy+1234H)
        divuw   (ix+1234H)
        divuw   (iy+1234H)
        divuw   hl, (ix+1234H)
        divuw   hl, (iy+1234H)
        ina     a,(123456H)
        inaw    hl,(123456H)
        inc     (ix+1234H)
        inc     (iy+1234H)
        ld      (123456H), a
        ld      (ix+1234H), 56H
        ld      (ix+1234H), a
        ld      (ix+1234H), b
        ld      (ix+1234H), c
        ld      (ix+1234H), d
        ld      (ix+1234H), e
        ld      (ix+1234H), h
        ld      (ix+1234H), l
        ld      (iy+1234H), 56H
        ld      (iy+1234H), a
        ld      (iy+1234H), b
        ld      (iy+1234H), c
        ld      (iy+1234H), d
        ld      (iy+1234H), e
        ld      (iy+1234H), h
        ld      (iy+1234H), l
        ld      a, (123456H)
        ld      a, (ix+1234H)
        ld      a, (iy+1234H)
        ld      b, (ix+1234H)
        ld      b, (iy+1234H)
        ld      c, (ix+1234H)
        ld      c, (iy+1234H)
        ld      d, (ix+1234H)
        ld      d, (iy+1234H)
        ld      e, (ix+1234H)
        ld      e, (iy+1234H)
        ld      h, (ix+1234H)
        ld      h, (iy+1234H)
        ld      l, (ix+1234H)
        ld      l, (iy+1234H)
        multuw  (ix+1234H)
        multuw  (iy+1234H)
        multuw  hl, (ix+1234H)
        multuw  hl, (iy+1234H)
        multw   (ix+1234H)
        multw   (iy+1234H)
        multw   hl, (ix+1234H)
        multw   hl, (iy+1234H)
        or      (ix+1234H)
        or      (iy+1234H)
        or      a, (ix+1234H)
        or      a, (iy+1234H)
        orw     (ix+1234H)
        orw     (iy+1234H)
        orw     hl, (ix+1234H)
        orw     hl, (iy+1234H)
        outa    (123456H), a
        outaw   (123456H), hl
        res     0, (ix+1234H)
        res     0, (iy+1234H)
        res     1, (ix+1234H)
        res     1, (iy+1234H)
        res     2, (ix+1234H)
        res     2, (iy+1234H)
        res     3, (ix+1234H)
        res     3, (iy+1234H)
        res     4, (ix+1234H)
        res     4, (iy+1234H)
        res     5, (ix+1234H)
        res     5, (iy+1234H)
        res     6, (ix+1234H)
        res     6, (iy+1234H)
        res     7, (ix+1234H)
        res     7, (iy+1234H)
        rl      (ix+1234H)
        rl      (iy+1234H)
        rlc     (ix+1234H)
        rlc     (iy+1234H)
        rlcw    (ix+1234H)
        rlcw    (iy+1234H)
        rlw     (ix+1234H)
        rlw     (iy+1234H)
        rr      (ix+1234H)
        rr      (iy+1234H)
        rrc     (ix+1234H)
        rrc     (iy+1234H)
        rrcw    (ix+1234H)
        rrcw    (iy+1234H)
        rrw     (ix+1234H)
        rrw     (iy+1234H)
        sbc     a, (ix+1234H)
        sbc     a, (iy+1234H)
        sbcw    (ix+1234H)
        sbcw    (iy+1234H)
        set     0, (ix+1234H)
        set     0, (iy+1234H)
        set     1, (ix+1234H)
        set     1, (iy+1234H)
        set     2, (ix+1234H)
        set     2, (iy+1234H)
        set     3, (ix+1234H)
        set     3, (iy+1234H)
        set     4, (ix+1234H)
        set     4, (iy+1234H)
        set     5, (ix+1234H)
        set     5, (iy+1234H)
        set     6, (ix+1234H)
        set     6, (iy+1234H)
        set     7, (ix+1234H)
        set     7, (iy+1234H)
        sla     (ix+1234H)
        sla     (iy+1234H)
        slaw    (ix+1234H)
        slaw    (iy+1234H)
        sra     (ix+1234H)
        sra     (iy+1234H)
        sraw    (ix+1234H)
        sraw    (iy+1234H)
        srl     (ix+1234H)
        srl     (iy+1234H)
        srlw    (ix+1234H)
        srlw    (iy+1234H)
        sub     a, (ix+1234H)
        sub     a, (iy+1234H)
        subw    hl, (ix+1234H)
        subw    hl, (iy+1234H)
        xor     (ix+1234H)
        xor     (iy+1234H)
        xor     a, (ix+1234H)
        xor     a, (iy+1234H)
        xorw    (ix+1234H)
        xorw    (iy+1234H)
        xorw    hl, (ix+1234H)
        xorw    hl, (iy+1234H)
;;; E-5
        add     hl, (12345678H)
        add     sp, 12345678H
        call    12345678H
        call    c, 12345678H
        call    m, 12345678H
        call    nc, 12345678H
        call    nz, 12345678H
        call    p, 12345678H
        call    pe, 12345678H
        call    po, 12345678H
        jp      12345678H
        jp      c, 12345678H
        jp      m, 12345678H
        jp      nc, 12345678H
        jp      nz, 12345678H
        jp      p, 12345678H
        jp      pe, 12345678H
        jp      po, 12345678H
        sub     hl, (12345678H)
        sub     sp, 12345678H
;;; E-6
        ld      (12345678H), bc
        ld      (12345678H), de
        ld      (12345678H), hl
        db      0DDH,0C3H,0EDH,63H,56H,34H,12H  ; ldw (12345678H), bc
        ld      (12345678H), ix
        ld      (12345678H), iy
        ld      (12345678H), sp
        ld      (ix+123456H), bc
        ld      (ix+123456H), de
        ld      (ix+123456H), hl
        ld      (ix+123456H), iy
        ld      (iy+123456H), bc
        ld      (iy+123456H), de
        ld      (iy+123456H), hl
        ld      (iy+123456H), ix
        ld      (sp+123456H), bc
        ld      (sp+123456H), de
        ld      (sp+123456H), hl
        ld      (sp+123456H), ix
        ld      (sp+123456H), iy
        ld      bc, (12345678H)
        ld      bc, (ix+123456H)
        ld      bc, (iy+123456H)
        ld      bc, (sp+123456H)
        ld      de, (12345678H)
        ld      de, (ix+123456H)
        ld      de, (iy+123456H)
        ld      de, (sp+123456H)
        ld      hl, (12345678H)
        db      0DDH,0C3H,0EDH,6BH,56H,34H,12H ; ldw hl,(12345678H)
        ld      hl, (ix+123456H)
        ld      hl, (iy+123456H)
        ld      hl, (sp+123456H)
        ld      ix, (12345678H)
        ld      ix, (iy+123456H)
        ld      ix, (sp+123456H)
        ld      iy, (12345678H)
        ld      iy, (ix+123456H)
        ld      iy, (sp+123456H)
        ld      sp, (12345678H)
        ldw     (bc), 12345678H
        ldw     (de), 12345678H
        ldw     (hl), 12345678H
;;; E-7
        ld      bc, 12345678H
        ld      de, 12345678H
        ld      hl, 12345678H
        ld      ix, 12345678H
        ld      iy, 12345678H
        ld      sp, 12345678H
        push    12345678H
;;; E-8
        adc     a, (ix+123456H)
        adc     a, (iy+123456H)
        adcw    (ix+123456H)
        adcw    (iy+123456H)
        adcw    hl, (ix+123456H)
        adcw    hl, (iy+123456H)
        add     a, (ix+123456H)
        add     a, (iy+123456H)
        addw    (ix+123456H)
        addw    (iy+123456H)
        addw    hl, (ix+123456H)
        addw    hl, (iy+123456H)
        and     (ix+123456H)
        and     (iy+123456H)
        and     a, (ix+123456H)
        and     a, (iy+123456H)
        andw    (ix+123456H)
        andw    (iy+123456H)
        andw    hl, (ix+123456H)
        andw    hl, (iy+123456H)
        bit     0, (ix+123456H)
        bit     0, (iy+123456H)
        bit     1, (ix+123456H)
        bit     1, (iy+123456H)
        bit     2, (ix+123456H)
        bit     2, (iy+123456H)
        bit     3, (ix+123456H)
        bit     3, (iy+123456H)
        bit     4, (ix+123456H)
        bit     4, (iy+123456H)
        bit     5, (ix+123456H)
        bit     5, (iy+123456H)
        bit     6, (ix+123456H)
        bit     6, (iy+123456H)
        bit     7, (ix+123456H)
        bit     7, (iy+123456H)
        cp      (ix+123456H)
        cp      (iy+123456H)
        cp      a, (ix+123456H)
        cp      a, (iy+123456H)
        cpw     (ix+123456H)
        cpw     (iy+123456H)
        cpw     hl, (ix+123456H)
        cpw     hl, (iy+123456H)
        dec     (ix+123456H)
        dec     (iy+123456H)
        divuw   (ix+123456H)
        divuw   (iy+123456H)
        divuw   hl, (ix+123456H)
        divuw   hl, (iy+123456H)
        ina     a,(12345678H)
        inaw    hl,(12345678H)
        inc     (ix+123456H)
        inc     (iy+123456H)
        ld      (12345678H), a
        ld      (ix+123456H), 56H
        ld      (ix+123456H), a
        ld      (ix+123456H), b
        ld      (ix+123456H), c
        ld      (ix+123456H), d
        ld      (ix+123456H), e
        ld      (ix+123456H), h
        ld      (ix+123456H), l
        ld      (iy+123456H), 56H
        ld      (iy+123456H), a
        ld      (iy+123456H), b
        ld      (iy+123456H), c
        ld      (iy+123456H), d
        ld      (iy+123456H), e
        ld      (iy+123456H), h
        ld      (iy+123456H), l
        ld      a, (12345678H)
        ld      a, (ix+123456H)
        ld      a, (iy+123456H)
        ld      b, (ix+123456H)
        ld      b, (iy+123456H)
        ld      c, (ix+123456H)
        ld      c, (iy+123456H)
        ld      d, (ix+123456H)
        ld      d, (iy+123456H)
        ld      e, (ix+123456H)
        ld      e, (iy+123456H)
        ld      h, (ix+123456H)
        ld      h, (iy+123456H)
        ld      l, (ix+123456H)
        ld      l, (iy+123456H)
        multuw  (ix+123456H)
        multuw  (iy+123456H)
        multuw  hl, (ix+123456H)
        multuw  hl, (iy+123456H)
        multw   (ix+123456H)
        multw   (iy+123456H)
        multw   hl, (ix+123456H)
        multw   hl, (iy+123456H)
        or      (ix+123456H)
        or      (iy+123456H)
        or      a, (ix+123456H)
        or      a, (iy+123456H)
        orw     (ix+123456H)
        orw     (iy+123456H)
        orw     hl, (ix+123456H)
        orw     hl, (iy+123456H)
        outa    (12345678H), a
        outaw   (12345678H), hl
        res     0, (ix+123456H)
        res     0, (iy+123456H)
        res     1, (ix+123456H)
        res     1, (iy+123456H)
        res     2, (ix+123456H)
        res     2, (iy+123456H)
        res     3, (ix+123456H)
        res     3, (iy+123456H)
        res     4, (ix+123456H)
        res     4, (iy+123456H)
        res     5, (ix+123456H)
        res     5, (iy+123456H)
        res     6, (ix+123456H)
        res     6, (iy+123456H)
        res     7, (ix+123456H)
        res     7, (iy+123456H)
        rl      (ix+123456H)
        rl      (iy+123456H)
        rlc     (ix+123456H)
        rlc     (iy+123456H)
        rlcw    (ix+123456H)
        rlcw    (iy+123456H)
        rlw     (ix+123456H)
        rlw     (iy+123456H)
        rr      (ix+123456H)
        rr      (iy+123456H)
        rrc     (ix+123456H)
        rrc     (iy+123456H)
        rrcw    (ix+123456H)
        rrcw    (iy+123456H)
        rrw     (ix+123456H)
        rrw     (iy+123456H)
        sbc     a, (ix+123456H)
        sbc     a, (iy+123456H)
        sbcw    (ix+123456H)
        sbcw    (iy+123456H)
        set     0, (ix+123456H)
        set     0, (iy+123456H)
        set     1, (ix+123456H)
        set     1, (iy+123456H)
        set     2, (ix+123456H)
        set     2, (iy+123456H)
        set     3, (ix+123456H)
        set     3, (iy+123456H)
        set     4, (ix+123456H)
        set     4, (iy+123456H)
        set     5, (ix+123456H)
        set     5, (iy+123456H)
        set     6, (ix+123456H)
        set     6, (iy+123456H)
        set     7, (ix+123456H)
        set     7, (iy+123456H)
        sla     (ix+123456H)
        sla     (iy+123456H)
        slaw    (ix+123456H)
        slaw    (iy+123456H)
        sra     (ix+123456H)
        sra     (iy+123456H)
        sraw    (ix+123456H)
        sraw    (iy+123456H)
        srl     (ix+123456H)
        srl     (iy+123456H)
        srlw    (ix+123456H)
        srlw    (iy+123456H)
        sub     a, (ix+123456H)
        sub     a, (iy+123456H)
        subw    hl, (ix+123456H)
        subw    hl, (iy+123456H)
        xor     (ix+123456H)
        xor     (iy+123456H)
        xor     a, (ix+123456H)
        xor     a, (iy+123456H)
        xorw    (ix+123456H)
        xorw    (iy+123456H)
        xorw    hl, (ix+123456H)
        xorw    hl, (iy+123456H)

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
