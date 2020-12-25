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
        idl
        ldn     1
        ldn     2
        ldn     3
        ldn     4
        ldn     5
        ldn     6
        ldn     7
        ldn     8
        ldn     9
        ldn     10
        ldn     11
        ldn     12
        ldn     13
        ldn     14
        ldn     15
;;; 1X
        inc     0
        inc     1
        inc     2
        inc     3
        inc     4
        inc     5
        inc     6
        inc     7
        inc     8
        inc     9
        inc     10
        inc     11
        inc     12
        inc     13
        inc     14
        inc     15
;;; 2X
        dec     0
        dec     1
        dec     2
        dec     3
        dec     4
        dec     5
        dec     6
        dec     7
        dec     8
        dec     9
        dec     10
        dec     11
        dec     12
        dec     13
        dec     14
        dec     15
;;; 3X
        br      131H
        bq      132H
        bz      133H
        bdf     134H
        bpz     134H
        bge     134H
        b1      135H
        b2      136H
        b3      137H
        b4      138H
        nbr     139H
        skp
        bnq     13AH
        bnz     13BH
        bnf     13CH
        bm      13CH
        bl      13CH
        bn1     13DH
        bn2     13EH
        bn3     13FH
        bn4     140H
;;; 4XX
        lda     0
        lda     1
        lda     2
        lda     3
        lda     4
        lda     5
        lda     6
        lda     7
        lda     8
        lda     9
        lda     10
        lda     11
        lda     12
        lda     13
        lda     14
        lda     15
;;; 5X
        str     0
        str     1
        str     2
        str     3
        str     4
        str     5
        str     6
        str     7
        str     8
        str     9
        str     10
        str     11
        str     12
        str     13
        str     14
        str     15
;;; 6X
        irx
        out     1
        out     2
        out     3
        out     4
        out     5
        out     6
        out     7
        inp     1
        inp     2
        inp     3
        inp     4
        inp     5
        inp     6
        inp     7
;;; 7X
        ret
        dis
        ldxa
        stxd
        adc
        sdb
        shrc
        rshr
        smb
        sav
        mark
        req
        seq
        adci    7DH
        sdbi    7EH
        shlc
        rshl
        smbi    80H
;;; 8X
        glo     0
        glo     1
        glo     2
        glo     3
        glo     4
        glo     5
        glo     6
        glo     7
        glo     8
        glo     9
        glo     10
        glo     11
        glo     12
        glo     13
        glo     14
        glo     15
;;; 9X
        ghi     0
        ghi     1
        ghi     2
        ghi     3
        ghi     4
        ghi     5
        ghi     6
        ghi     7
        ghi     8
        ghi     9
        ghi     10
        ghi     11
        ghi     12
        ghi     13
        ghi     14
        ghi     15
;;; AX
        plo     0
        plo     1
        plo     2
        plo     3
        plo     4
        plo     5
        plo     6
        plo     7
        plo     8
        plo     9
        plo     10
        plo     11
        plo     12
        plo     13
        plo     14
        plo     15
;;; BX
        phi     0
        phi     1
        phi     2
        phi     3
        phi     4
        phi     5
        phi     6
        phi     7
        phi     8
        phi     9
        phi     10
        phi     11
        phi     12
        phi     13
        phi     14
        phi     15
;;; CX
        lbr     0C1C2H
        lbq     0C2C3H
        lbz     0C3C4H
        lbdf    0C4C5H
        nop
        lsnq
        lsnz
        lsnf
        nlbr    0C9CAH
        lskp
        lbnq    0CACBH
        lbnz    0CBCCH
        lbnf    0CCCDH
        lsie
        lsq
        lsz
        lsdf
;;; DX
        sep     0
        sep     1
        sep     2
        sep     3
        sep     4
        sep     5
        sep     6
        sep     7
        sep     8
        sep     9
        sep     10
        sep     11
        sep     12
        sep     13
        sep     14
        sep     15
;;; EX
        ghi     0
        ghi     1
        ghi     2
        ghi     3
        ghi     4
        ghi     5
        ghi     6
        ghi     7
        ghi     8
        ghi     9
        ghi     10
        ghi     11
        ghi     12
        ghi     13
        ghi     14
        ghi     15
;;; EX
        sex     0
        sex     1
        sex     2
        sex     3
        sex     4
        sex     5
        sex     6
        sex     7
        sex     8
        sex     9
        sex     10
        sex     11
        sex     12
        sex     13
        sex     14
        sex     15
;;; FX
        ldx
        or
        and
        xor
        add
        sd
        shr
        sm
        ldi     0F9H
        ori     0FAH
        ani     0FBH
        xri     0FCH
        adi     0FDH
        sdi     0FEH
        shl
        smi     00H

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
