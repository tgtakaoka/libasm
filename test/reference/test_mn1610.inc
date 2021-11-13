;;; Copyright 2021 Tadashi G. Takaoka
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

;;; 0x0000
        mvi     r0, 0
        mvi     r1, 1
        mvi     r2, 2
        mvi     r3, 3
        mvi     r4, 4
        mvi     x0, 5
        mvi     x1, 6
        mvi     sp, 0xff
        mvi     str, 0xff
;;; 0x1000
        wt      r0, 0
        wt      r1, 1
        wt      r2, 2
        wt      r3, 3
        wt      r4, 4
        wt      x0, 5
        wt      x1, 6
        wt      sp, 0xff
        wt      str, 0xff
        rd      r0, 0
        rd      r1, 1
        rd      r2, 2
        rd      r3, 3
        rd      r4, 4
        rd      x0, 5
        rd      x1, 6
        rd      sp, 0xff
        rd      str, 0xff
;;; 0x2000
        lpsw    0
        lpsw    3
        h
        ret
        push    r0
        push    r1
        push    r2
        push    r3
        push    r4
        push    x0
        push    x1
        push    sp
        push    str
        pop     r0
        pop     r1
        pop     r2
        pop     r3
        pop     r4
        pop     x0
        pop     x1
        pop     sp
        pop     str
        sl      r0
        sl      r0, re, skp
        sl      r0, se, m
        sl      r0, ce, pz
        sl      r1, z
        sl      r1, re, e
        sl      r1, se, nz
        sl      r1, ce, ne
        sl      r2, mz
        sl      r2, re, p
        sl      r2, se, ez
        sl      r2, ce, enz
        sl      r3, oz
        sl      r3, re, onz
        sl      r3, se, lmz
        sl      r3, ce, lp
        sl      x1, lpz
        sl      x1, re, lm
        sl      x1, se
        sl      x1, ce, skp
        sr      r0
        sr      r0, re, skp
        sr      r0, se, m
        sr      r0, ce, pz
        sr      r1, z
        sr      r1, re, e
        sr      r1, se, nz
        sr      r1, ce, ne
        sr      r2, mz
        sr      r2, re, p
        sr      r2, se, ez
        sr      r2, ce, enz
        sr      x0, oz
        sr      x0, re, onz
        sr      x0, se, lmz
        sr      x0, ce, lp
        sr      r4, lpz
        sr      r4, re, lm
        sr      r4, se
        sr      r4, ce, skp
        tbit    r0, 0
        tbit    r1, 1, z
        tbit    r2, 2, nz
        tbit    r3, 3
        tbit    r4, 4, e
        tbit    x0, 5, ne
        tbit    x1, 6
        tbit    sp, 7
        tbit    str, 15
;;; 0x3000
        rbit    r0, 0
        rbit    r1, 1, z
        rbit    r2, 2, nz
        rbit    r3, 3
        rbit    r4, 4, e
        rbit    x0, 5, ne
        rbit    x1, 6
        rbit    sp, 7
        rbit    str, 15
        sbit    r0, 0
        sbit    r1, 1, z
        sbit    r2, 2, nz
        sbit    r3, 3
        sbit    r4, 4, e
        sbit    x0, 5, ne
        sbit    x1, 6
        sbit    sp, 7
        sbit    str, 15
;;; 0x4000
        si      r0, 0
        si      r1, 1, z
        si      r2, 2, nz
        si      r3, 3
        si      r4, 4, e
        si      x0, 5, ne
        si      x1, 6
        si      sp, 7
        si      str, 15
        ai      r0, 0
        ai      r1, 1, z
        ai      r2, 2, nz
        ai      r3, 3
        ai      r4, 4, e
        ai      x0, 5, ne
        ai      x1, 6
        ai      sp, 7
        ai      str, 15
;;; 0x5000
        cb      r0, r1
        cb      r1, r2, skp
        cb      r2, r3, m
        cb      r3, r4, pz
        cb      x1, x0, e
        cb      x0, sp, nz
        c       r0, r1
        c       r1, r2, skp
        c       r2, r3, m
        c       r3, r4, pz
        c       x1, x0, e
        c       x0, sp, nz
        s       r0, r1
        s       r1, r2, skp
        s       r2, r3, m
        s       r3, r4, pz
        s       x1, x0, e
        s       x0, sp, nz
        a       r0, r1
        a       r1, r2, skp
        a       r2, r3, m
        a       r3, r4, pz
        a       x1, x0, e
        a       x0, sp, nz
;;; 0x6000
        eor     r0, r1
        eor     r1, r2, skp
        eor     r2, r3, m
        eor     r3, r4, pz
        eor     x1, x0, e
        eor     x0, sp, nz
        or      r0, r1
        or      r1, r2, skp
        or      r2, r3, m
        or      r3, r4, pz
        or      x1, x0, e
        or      x0, sp, nz
        lad     r0, r1
        lad     r1, r2, skp
        lad     r2, r3, m
        lad     r3, r4, pz
        lad     x1, x0, e
        lad     x0, sp, nz
        and     r0, r1
        and     r1, r2, skp
        and     r2, r3, m
        and     r3, r4, pz
        and     x1, x0, e
        and     x0, sp, nz
;;; 0x7000
        dswp    r0, r1
        dswp    r1, r2, skp
        dswp    r2, r3, m
        dswp    r3, r4, pz
        dswp    x1, x0, e
        dswp    x0, sp, nz
        bswp    r0, r1
        bswp    r1, r2, skp
        bswp    r2, r3, m
        bswp    r3, r4, pz
        bswp    x1, x0, e
        bswp    x0, sp, nz
        mvb     r0, r1
        mvb     r1, r2, skp
        mvb     r2, r3, m
        mvb     r3, r4, pz
        mvb     x1, x0, e
        mvb     x0, sp, nz
        mv      r0, r1
        mv      r1, r2, skp
        mv      r2, r3, m
        mv      r3, r4, pz
        mv      x1, x0, e
        mv      x0, sp, nz
;;; 0x8000
        st      r0, 1
        st      r1, -2(ic)
        st      r2, (3)
        st      r3, (-4(ic))
        st      r4, 5(x0)
        st      x0, 6(x1)
        st      x1, (7)(x0)
        st      sp, (8)(x1)
        dms     1
        dms     -2(ic)
        dms     (3)
        dms     (-4(ic))
        dms     5(x0)
        dms     6(x1)
        dms     (7)(x0)
        dms     (8)(x1)
        bal     1
        bal     -2(ic)
        bal     (3)
        bal     (-4(ic))
        bal     5(x0)
        bal     6(x1)
        bal     (7)(x0)
        bal     (8)(x1)
;;; 0xC000
        l       r0, 1
        l       r1, -2(ic)
        l       r2, (3)
        l       r3, (-4(ic))
        l       r4, 5(x0)
        l       x0, 6(x1)
        l       x1, (7)(x0)
        l       sp, (8)(x1)
        ims     1
        ims     -2(ic)
        ims     (3)
        ims     (-4(ic))
        ims     5(x0)
        ims     6(x1)
        ims     (7)(x0)
        ims     (8)(x1)
        b       1
        b       -2(ic)
        b       (3)
        b       (-4(ic))
        b       5(x0)
        b       6(x1)
        b       (7)(x0)
        b       (8)(x1)

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
