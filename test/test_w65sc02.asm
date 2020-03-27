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

        include test_m6502.asm

        cpu     65sc02

;;; W65SC02
        org     $0150

;;; 0X
        tsb     $05
        tsb     $0e0d
;;; 1X
        ora     ($13)
        trb     $15
        inc     a
        trb     $1e1d
;;; 3X
        and     ($33)
        bit     $35,x
        dec     a
        bit     $3e3d,x
;;; 5X
        eor     ($53)
        phy
;;; 6X
        stz     $65
;;; 7X
        adc     ($73)
        stz     $75,x
        ply
        jmp     ($7e7d,x)
;;; 8X
        bra     *-125
        bit     #$8a
;;; 9X
        sta     ($93)
        stz     $9e9d
        stz     $a09f,x
;;; BX
        lda     ($b3)
;;; DX
        cmp     ($d3)
        phx
;;; FX
        sbc     ($f3)
        plx

;;; comment

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
