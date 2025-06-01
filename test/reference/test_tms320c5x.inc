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

        lamm    *, AR1
        smmr    *+, #1234H
        bldp    *-, AR2
        xpl     *0+
        opl     *0-
        apl     *BR0-
        cpl     *BR0+
        xpl     #1234H, *, AR7
        opl     #1234H, *-
        apl     #1234H, 70H
        cpl     #1234H, *+, AR1
        bd      1234H
        calld   1234H
        banzd   1234H, *, AR0
        samm    *+, AR0
        lmmr    *, #1234H, AR2
        blpd    bmar, *+
        mads    *0+
        madd    *0-
        bldd    bmar, *BR0+, AR1
        bldd    *BR0-, bmar, AR2
        addb
        adcb
        andb
        orb
        rolb
        rorb
        sflb
        sfrb
        sbb
        sbbb
        xorb
        crgt
        crlt
        exar
        sacb
        lacb
        baccd
        idle2
        reti
        rete
        calad
        clrc    hm
        setc    hm
        zpr
        zap
        sath
        satl
        rptz    #1234H
        rptb    1234H
        bsar    16
        xc      1, tc
        xc      1, ntc
        xc      1, bio
        xc      1, eq
        xc      1, neq
        xc      1, gt
        xc      1, geq
        xc      1, lt
        xc      1, leq
        xc      1, ov
        xc      1, nov
        xc      1, c
        xc      1, nc
        xc      2, tc
        xc      2, ntc
        xc      2, bio
        xc      2, eq
        xc      2, neq
        xc      2, gt
        xc      2, geq
        xc      2, lt
        xc      2, leq
        xc      2, ov
        xc      2, nov
        xc      2, c
        xc      2, nc
        bcndd   1234H, tc
        bcndd   1234H, ntc
        bcndd   1234H, bio
        bcndd   1234H, eq
        bcndd   1234H, neq
        bcndd   1234H, gt
        bcndd   1234H, geq
        bcndd   1234H, lt
        bcndd   1234H, leq
        bcndd   1234H, ov
        bcndd   1234H, nov
        bcndd   1234H, c
        bcndd   1234H, nc
        bcndd   1234H, unc
        ccd     1234H, tc
        ccd     1234H, ntc
        ccd     1234H, bio
        ccd     1234H, eq
        ccd     1234H, neq
        ccd     1234H, gt
        ccd     1234H, geq
        ccd     1234H, lt
        ccd     1234H, leq
        ccd     1234H, ov
        ccd     1234H, nov
        ccd     1234H, c
        ccd     1234H, nc
        ccd     1234H, unc
        retcd   tc
        retcd   ntc
        retcd   bio
        retcd   eq
        retcd   neq
        retcd   gt
        retcd   geq
        retcd   lt
        retcd   leq
        retcd   ov
        retcd   nov
        retcd   c
        retcd   nc
        retd

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
