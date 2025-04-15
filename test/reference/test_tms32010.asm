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

        cpu     32010
        org     0800H
;;; 0x0
        add     23H, 1
        add     *, 1
        add     *-, 2, ar1
        add     *+, 15, ar0
;;; 0x1
        sub     34H, 2
        sub     *, 1
        sub     *-, 2, ar1
        sub     *+, 15, ar0
;;; 0x2
        lac     45H, 3
        lac     *, 1
        lac     *-, 2, 1
        lac     *+, 15, 0
;;; 0x3
        sar     ar0, 24H
        sar     1, *, ar1
        lar     ar1, 8AH
        lar     ar0, *-, 0
;;; 0x4
        in      67H, 5
        in      *, 7, ar1
        out     080H, 0
        out     *+, 1, ar0
;;; 0x5
        sacl    61H
        sacl    *-, 0, ar1
        sach    69H, 4
        sach    *+, 1, ar0
;;; 0x6
        addh    62H
        addh    *, ar1
        adds    63H
        adds    *-, ar0
        subh    64H
        subh    *+, ar1
        subs    64H
        subs    *, ar0
        subc    65H
        subc    *-, ar1
        zalh    66H
        zalh    *+, ar0
        zals    67H
        zals    *, ar1
        tblr    68H
        tblr    *-, ar0
        larp    ar1
        mar     69H
        mar     *+, ar0
        dmov    6AH
        dmov    *, ar1
        lt      6BH
        lt      *-, ar0
        ltd     6CH
        ltd     *+, ar1
        lta     6DH
        lta     *, ar0
        mpy     6EH
        mpy     *-, ar1
        ldpk    1
        ldp     70H
        ldp     *+, ar0
;;; 0x7
        lark    ar0, 71H
        lark    ar1, 72H
        xor     79H
        xor     *, ar0
        and     7AH
        and     *-, ar1
        or      7BH
        or      *+, ar0
        lst     7CH
        lst     *, ar1
        sst     8DH
        sst     *-, ar0
        tblw    7EH
        tblw    *+, ar1
        lack    7FH
;;; 0x7F
        nop
        dint
        eint
        abs
        zac
        rovm
        sovm
        cala
        ret
        pac
        apac
        spac
        push
        pop
;;; 0x8
        mpyk    9ABH
        mpyk    4095
        mpyk    -4096
;;; 0xF
        banz    456H
        bv      567H
        bioz    678H
        call    89AH
        b       9ABH
        blz     0ABCH
        blez    0BCDH
        bgz     0CDEH
        bgez    0DEFH
        bnz     0EF0H
        bz      0F01H

        end
;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
