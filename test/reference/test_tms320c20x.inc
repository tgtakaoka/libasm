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

        lar     AR4, 56H
        subc    *, 1
        rpt     *BR0-, AR5
        out     *BR0-, 1234H, AR3
        ldp     34H
        lst     #0, 12H
        lst     #1, 23H
        lacc    45H, 3
        add     23H, 1
        sub     34H, 2
        bit     76H, 8
        mpya    3BH
        mpys    *BR0-, AR5
        sqra    *+, 3
        sqrs    5BH
        mpy     *-, AR2
        mpyu    *0+, AR5
        addc    21H
        add     *-, 16, AR2
        adds    *+, 3
        addt    4BH
        subb    *0+, 5
        sub     56H, 16
        subs    67H
        subt    78H
        zalr    *BR0-, AR5
        lacl    23H
        lacc    12H, 16
        xor     *0-, 6
        or      *0+, AR7
        and     *BR0+
        bitt    *, 1
        lta     *0+, 7
        ltp     *BR0+
        ltd     01H
        lt      *0-, AR6
        lts     *BR0-, AR5
        lph     45H
        pshd    56H
        dmov    78H
        adrk    #7FH
        b       1234H
        sar     AR1, 23H
        popd    7BH
        mar     67H
        nop
        spl     *0-, 6
        sph     *0+, AR7
        sst     #0, *-, 2
        sst     #1, *+, AR3
        sacl    *, 7, AR1
        sach    *+, 0, AR4
        norm    *
        mac     0123H, *0+, AR7
        macd    0F012H, *0-, 6
        blpd    #0F012H, *0-, AR6
        tblr    *-, AR2
        tblw    *+, 3
        bldd    #0123H, *0+, 7
        bldd    *0+, #0123H, 7
        splk    #1234H, *+, AR4
        in      *+, 1234H, 3
        lar     AR7, #0C8H
        add     #0CDH
        lacc    #0CBH
        sub     #0CEH
        rpt     #0CCH
        ldp     #1CAH
        abs
        cmpl
        neg
        pac
        apac
        spac
        sfl
        sfr
        rol
        ror
        bacc
        idle
        cala
        pop
        push
        clrc    intm
        setc    intm
        clrc    ovm
        setc    ovm
        clrc    cnf
        setc    cnf
        clrc    sxm
        setc    sxm
        clrc    tc
        setc    tc
        clrc    xf
        setc    xf
        clrc    c
        setc    c
        trap
        nmi
        intr    31
        and     #1234H, 16
        or      #4567H, 16
        xor     #89ABH, 16
        spm     3
        lar     0, #0D1D2H
        cmpr    3
        lacc    #2345H, 2
        add     #3456H, 14
        sub     #4567H, 14
        and     #5678H, 14
        or      #6789H, 14
        xor     #789AH, 14
        mpy     #456H
        mpy     #-456H
        bcnd    1234H, tc
        bcnd    1234H, ntc
        bcnd    1234H, bio
        bcnd    1234H, eq
        bcnd    1234H, neq
        bcnd    1234H, gt
        bcnd    1234H, geq
        bcnd    1234H, lt
        bcnd    1234H, leq
        bcnd    1234H, ov
        bcnd    1234H, nov
        bcnd    1234H, c
        bcnd    1234H, nc
        bcnd    1234H, unc
        cc      1234H, tc
        cc      1234H, ntc
        cc      1234H, bio
        cc      1234H, eq
        cc      1234H, neq
        cc      1234H, gt
        cc      1234H, geq
        cc      1234H, lt
        cc      1234H, leq
        cc      1234H, ov
        cc      1234H, nov
        cc      1234H, c
        cc      1234H, nc
        cc      1234H, unc
        retc    tc
        retc    ntc
        retc    bio
        retc    eq
        retc    neq
        retc    gt
        retc    geq
        retc    lt
        retc    leq
        retc    ov
        retc    nov
        retc    c
        retc    nc
        ret

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
