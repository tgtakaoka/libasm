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
        nop                     ;comment
        ld      bc,0302H
        ld      (bc),a
        inc     bc
        inc     b
        dec     b
        ld      b,07H
        rlca
        add     hl,bc
        ld      a,(bc)
        dec     bc
        inc     c
        dec     c
        ld      c,0fH
        rrca
;;; 1X
        ld      de,1312H
        ld      (de),a
        inc     de
        inc     d
        dec     d
        ld      d,17H
        rla
        add     hl,de
        ld      a,(de)
        dec     de
        inc     e
        dec     e
        ld      e,1fH
        rra
;;; 2X
        ld      hl,2322H
        ld      (2423H),hl
        inc     hl
        inc     h
        dec     h
        ld      h,27H
        daa
        add     hl,hl
        ld      hl,(2c2bH)
        dec     hl
        inc     l
        dec     l
        ld      l,2fH
        cpl
;;; 3X
        ld      sp,3332H
        ld      (3433H),a
        inc     sp
        inc     (hl)
        dec     (hl)
        ld      (hl),37H
        scf
        add     hl,sp
        ld      a,(3c3bH)
        dec     sp
        inc     a
        dec     a
        ld      a,3fH
        ccf
;;; 4X
        ld      b,b
        ld      b,c
        ld      b,d
        ld      b,e
        ld      b,h
        ld      b,l
        ld      b,(hl)
        ld      b,a
        ld      c,b
        ld      c,c
        ld      c,d
        ld      c,e
        ld      c,h
        ld      c,l
        ld      c,(hl)
        ld      c,a
;;; 5X
        ld      d,b
        ld      d,c
        ld      d,d
        ld      d,e
        ld      d,h
        ld      d,l
        ld      d,(hl)
        ld      d,a
        ld      e,b
        ld      e,c
        ld      e,d
        ld      e,e
        ld      e,h
        ld      e,l
        ld      e,(hl)
        ld      e,a
;;; 6X
        ld      h,b
        ld      h,c
        ld      h,d
        ld      h,e
        ld      h,h
        ld      h,l
        ld      h,(hl)
        ld      h,a
        ld      l,b
        ld      l,c
        ld      l,d
        ld      l,e
        ld      l,h
        ld      l,l
        ld      l,(hl)
        ld      l,a
;;; 7X
        ld      (hl),b
        ld      (hl),c
        ld      (hl),d
        ld      (hl),e
        ld      (hl),h
        ld      (hl),l
        halt
        ld      (hl),a
        ld      a,b
        ld      a,c
        ld      a,d
        ld      a,e
        ld      a,h
        ld      a,l
        ld      a,(hl)
        ld      a,a
;;; 8X
        add     a,b
        add     a,c
        add     a,d
        add     a,e
        add     a,h
        add     a,l
        add     a,(hl)
        add     a,a
        adc     a,b
        adc     a,c
        adc     a,d
        adc     a,e
        adc     a,h
        adc     a,l
        adc     a,(hl)
        adc     a,a
;;; 9X
        sub     b
        sub     c
        sub     d
        sub     e
        sub     h
        sub     l
        sub     (hl)
        sub     a
        sbc     a,b
        sbc     a,c
        sbc     a,d
        sbc     a,e
        sbc     a,h
        sbc     a,l
        sbc     a,(hl)
        sbc     a,a
;;; AX
        and     b
        and     c
        and     d
        and     e
        and     h
        and     l
        and     (hl)
        and     a
        xor     b
        xor     c
        xor     d
        xor     e
        xor     h
        xor     l
        xor     (hl)
        xor     a
;;; BX
        or      b
        or      c
        or      d
        or      e
        or      h
        or      l
        or      (hl)
        or      a
        cp      b
        cp      c
        cp      d
        cp      e
        cp      h
        cp      l
        cp      (hl)
        cp      a
;;; CX
        ret     nz
        pop     bc
        jp      nz,0c4c3H
        jp      0c5c4H
        call    nz,0c6c5H
        push    bc
        add     a,0c7H
        rst     00h
        ret     z
        ret
        jp      z,0cccbH
        call    z,0cecdH
        call    0cfceH
        adc     a,0cfH
        rst     08H
;;; DX
        ret     nc
        pop     de
        jp      nc,0d4d3H
        out     (0d4h),a
        call    nc,0d6d5H
        push    de
        sub     0d7H
        rst     10h
        ret     c
        jp      c,0dcdbH
        in      a,(0dcH)
        call    c,0deddH
        sbc     a,0dfH
        rst     18H
;;; EX
        ret     po
        pop     hl
        jp      po,0e4e3H
        ex      (sp),hl
        call    po,0e6e5H
        push    hl
        and     0e7H
        rst     20H
        ret     pe
        jp      (hl)
        jp      pe,0ecebH
        ex      de,hl
        call    pe,0eeedH
        xor     0efH
        rst     28H
;;; FX
        ret     p
        pop     af
        jp      p,0f4f3H
        di
        call    p,0f6f5H
        push    af
        or      0f7H
        rst     30H
        ret     m
        ld      sp,hl
        jp      m,0fcfbH
        ei
        call    m,0fefdH
        cp      0ffH
        rst     38H

;;; SUB/AND/XOR/OR/CP instruction can omit destination A register.

        sub     b
        sub     c
        sub     d
        sub     e
        sub     h
        sub     l
        sub     (hl)
        sub     a
        and     b
        and     c
        and     d
        and     e
        and     h
        and     l
        and     (hl)
        and     a
        xor     b
        xor     c
        xor     d
        xor     e
        xor     h
        xor     l
        xor     (hl)
        xor     a
        or      b
        or      c
        or      d
        or      e
        or      h
        or      l
        or      (hl)
        or      a
        cp      b
        cp      c
        cp      d
        cp      e
        cp      h
        cp      l
        cp      (hl)
        cp      a
        sub     0d7H
        and     0e7H
        xor     0efH
        or      0f7H
        cp      0ffH

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
