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
        lb      ssbr, 1
        lb      tsr0, 2
        lb      tsr1, 3
        lb      osr0, 4
        lb      osr1, 5
        lb      osr2, 6
        lb      osr3, 7
        setb    r1, ssbr
        setb    r2, tsr0
        setb    r3, tsr1
        setb    r4, osr0
        setb    x0, osr1
        setb    x1, osr2
        setb    sp, osr3
        ls      sbrb, 8
        ls      icb, 9
        ls      npp, 10
        sets    x0, sbrb
        sets    x1, icb
        sets    r2, npp
        stb     ssbr, 11
        stb     tsr0, 12
        stb     tsr1, 13
        stb     osr0, 14
        stb     osr1, 15
        stb     osr2, 16
        stb     osr3, 17
        cpyb    r1, ssbr
        cpyb    r2, tsr0
        cpyb    r3, tsr1
        cpyb    r4, osr0
        cpyb    x0, osr1
        cpyb    x1, osr2
        cpyb    sp, osr3
        sts     sbrb, 18
        sts     icb, 19
        sts     npp, 20
        cpys    x0, sbrb
        cpys    x1, icb
        cpys    r2, npp
;;; 0x1000
        popm
        pshm
        trst    r0, 21
        trst    r1, 22, skp
        trst    r2, 23, z
        trst    r3, 24, nz
        trst    r4, 25, p
        trst    x0, 26, m
        trst    x1, 27, e
        trst    sp, 28, ne
        trst    str, 29, mz
        tset    r0, 21
        tset    r1, 22, skp
        tset    r2, 23, z
        tset    r3, 24, nz
        tset    r4, 25, p
        tset    x0, 26, m
        tset    x1, 27, e
        tset    sp, 28, ne
        tset    str, 29, mz
        flt     dr0, r0
        flt     dr0, r0, nz
        fix     r0, dr0, z
        fix     r0, dr0, skp
        neg     r0, c
        neg     r1, 0, z
;;; 0x2000
        bd      0xffff
        bald    0x8000
        br      (x0)
        br      (r1)
        bl      (0x1000)
        balr    (r4)
        balr    (x1)
        ld      r0, csbr, 0x1234
        ld      sp, 0x2345
        ld      sp, tsr1, 0x2345
        std     r1, ssbr, 0x1234
        std     sp, 0x2345
        std     sp, csbr, 0x2345
        rdr     r0, (r4)
        rdr     sp, (r1)
        rdr     r0, (x1)
        lr      r0, (r1)
        lr      r0, csbr, (r1)
        lr      r1, ssbr, (r2)+
        lr      sp, tsr1, -(x1)
        wtr     r0, (r4)
        wtr     sp, (r1)
        wtr     r0, (x1)
        str     r0, (r1)
        str     r0, csbr, (r1)
        str     r1, ssbr, (r2)+
        str     sp, tsr1, -(r4)
;;; 0x3000
        retl
        blk     (r2), (r1), r0
        seth    r0, tcr
        seth    r1, tir
        seth    r2, tsr
        seth    r3, scr
        seth    r4, ssr
        seth    x0, sor
        seth    x1, iisr
        srbt    r0, r0
        srbt    r0, str
        cpyh    r0, tcr
        cpyh    r1, tir
        cpyh    r2, tsr
        cpyh    r3, scr
        cpyh    r4, ssr
        cpyh    x0, sir
        cpyh    x1, iisr
        debp    r1, r0
        debp    str, r0
;;; 0x4000
        sd      dr0, (r1), c, z
        sd      dr0, (r4)
        ad      dr0, (r2), z
        ad      dr0, (x1), 0, nz
;;; 0x5000
        cbr     r0, (r1)
        cbr     r0, (r4), ne
        cwr     r0, (r1)
        cwr     r0, (r4), ne
        das     r0, (r1), c
        das     r0, (r4), 0, ne
        cbi     r1, 0x2345
        cbi     sp, 0x2345, e
        cwi     r1, 0x2345
        cwi     sp, 0x2345, e
        swr     r0, (r1)
        swr     r0, (r4), z
        awr     r0, (r1)
        awr     r0, (r4), z
        daa     r0, (r2), c, z
        daa     r0, (r3)
        swi     r1, 0x1234, z
        swi     sp, 1
        awi     r1, 0x1234, z
        awi     sp, 1
;;; 0x6000
        fd      dr0, (r1)
        fd      dr0, (r4), z
        fm      dr0, (r1)
        fm      dr0, (r4), z
        eorr    r0, (r1)
        eorr    r0, (r4), z
        eori    r1, 0x1234, z
        eori    str, 1
        orr     r0, (r1)
        orr     r0, (r4), z
        ori     r1, 0x1234, z
        ori     str, 1
        ladr    r0, (r1)
        ladr    r0, (r4), z
        fs      dr0, (r1)
        fs      dr0, (r4), z
        fa      dr0, (r1)
        fa      dr0, (r4), z
        andr    r0, (r1)
        andr    r0, (r4), z
        andi    r1, 0x1234, z
        andi    str, 1
;;; 0x7000
        dswr    r0, (r1)
        dswr    r0, (r4), z
        bswr    r0, (r1)
        bswr    r0, (r4), z
        d       dr0, (r1)
        d       dr0, (r4), z
        mvbr    r0, (r1)
        mvbr    r0, (r4), z
        mvwr    r0, (r1)
        mvwr    r0, (r4), z
        m       dr0, (r1)
        m       dr0, (r4), z
        mvwi    r0, 0x1234
        mvwi    str, 0x5678, skp

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
