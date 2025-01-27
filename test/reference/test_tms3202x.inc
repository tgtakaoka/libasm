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

        add     23H, 1
        sub     34H, 2
        lac     45H, 3
        lar     AR4, 56H
        mpy     *-, AR2
        sqra    *+, 3
        lt      *0-, AR6
        lta     *0+, 7
        ltp     *BR0+
        ltd     01H
        zalh    12H
        zals    23H
        lact    34H
        subh    56H
        subs    67H
        subt    78H
        subc    *, 1
        addh    *-, AR2
        adds    *+, 3
        addt    4BH
        rpt     *BR0-, AR5
        xor     *0-, 6
        or      *0+, AR7
        and     *BR0+
        lst     12H
        lst1    23H
        ldp     34H
        lph     45H
        pshd    56H
        nop
        mar     67H
        larp    AR0
        larp    7
        dmov    78H
        bitt    *, 1
        tblr    *-, AR2
        tblw    *+, 3
        sqrs    5BH
        lts     *BR0-, AR5
        macd    0F012H, *0-, 6
        mac     0123H, *0+, AR7
        sacl    *, 7, AR1
        sach    *+, 0, AR4
        sar     AR1, 23H
        sst     *-, 2
        sst1    *+, AR3
        popd    7BH
        in      *+, 9, 3
        bit     76H, 8
        mpyk    0BCDH
        lark    AR7, 0C8H
        ldpk    1CAH
        zac
        lack    0CBH
        rptk    0CCH
        eint
        dint
        rovm
        sovm
        rsxm
        ssxm
        spm     0
        spm     3
        rxf
        sxf
        fort    0
        fort    1
        pac
        apac
        spac
        sfl
        sfr
        abs
        push
        pop
        trap
        idle
        rtxm
        stxm
        neg
        cala
        bacc
        ret
        cmpl
        cmpr    0
        cmpr    3
        norm    *
        lrlk    0, 0D1D2H
        lrlk    AR7, 0D8D9H
        lalk    2345H
        lalk    2345H, 14
        adlk    3456H
        adlk    3456H, 14
        sblk    4567H
        sblk    4567H, 14
        andk    5678H
        andk    5678H, 14
        ork     6789H
        ork     6789H, 14
        xork    789AH
        xork    789AH, 14
        out     *BR0-, 13, AR3
        bv      0ABCDH, *, 1
        bgz     0BCDEH, *-, AR2
        blez    0CDEFH, *+, 3
        blz     0EF01H, *BR0-, AR5
        bgez    0F012H, *0-, 6
        bnz     00123H, *0+, AR7
        bz      01234H, *BR0+
        bnv     0ABCDH, *, 1
        bbz     0BCDEH, *-, AR2
        bbnz    0CDEFH, *+, 3
        bioz    0FCFDH, *BR0+, AR3
        banz    0EF01H, *BR0-, 5
        blkp    0F012H, *0-, AR6
        blkd    00123H, *0+, 7
        call    0BA98H, *0-, AR4
        b       0ABCDH, *, 1

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
