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

;;; W65C816

;;; 0X
        cop     #$03
        ora     $04,s
        oral    ($08)
        phd
        ora     $121110
;;; 1X
        ora     ($14,s),y
        oral    ($18),y
        tcs
        ora     $222120,x
;;; 2X
        jsl     $252423
        and     $24,s
        andl    ($28)
        pld
        and     $323130
;;; 3X
        and     ($34,s),y
        andl    ($38),y
        tsc
        and     $424140,x
;;; 4X
        ;; wdm
        ;; fcb     $42,$43
        eor     $44,s
        mvp     $46789A,$456789
        eorl    ($48)
        phk
        eor     $525150
;;; 5X
        eor     ($54,s),y
        mvn     $56789A,$556789
        eorl    ($58),y
        tcd
        jmp     $5F5E5D
        eor     $626160,x
;;; 6X
        per     *+$6466
        adc     $64,s
        adcl    ($68)
        rtl
        adc     $727170
;;; 7X
        adc     ($74,s),y
        adcl    ($78),y
        tdc
        adc     $828180,x
;;; 8X
        brl     *+$7E83
        sta     $84,s
        stal    ($88)
        phb
        sta     $929190
;;; 9X
        sta     ($94,s),y
        stal    ($98),y
        txy
        sta     $A2A1A0,x
;;; AX
        lda     $A4,s
        ldal    ($A8)
        plb
        lda     $B2B1B0
;;; BX
        lda     ($b4,s),y
        ldal    ($b8),y
        tyx
        lda     $C2C1C0,x
;;; CX
        rep     #$C3
        cmp     $C4,s
        cmpl    ($C8)
        cmp     $D2D1D0
;;; DX
        cmp     ($d4,s),y
        pei     ($D5)
        cmpl    ($D8),y
        jmpl    ($DEDD)
        cmp     $E2E1E0,x
;;; EX
        sep     #$E3
        sbc     $E4,s
        sbcl    ($E8)
        xba
        sbc     $F2F1F0
;;; FX
        sbc     ($F4,s),y
        pea     $F6F5
        sbcl    ($F8),y
        xce
        jsr     ($FEFD,x)
        sbc     $020100,x

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
