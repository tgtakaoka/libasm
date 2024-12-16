;;; Copyright 2024 Tadashi G. Takaoka
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

;;; X1
        cbeq    $32, *+$36
        cbeqa   #$42, *+$46
        cbeqx   #$52, *+$56
        cbeq    $62,x+, *+$66
        cbeq    x+, *+$74
;;; X2
        div
        nsa
        daa
;;; X5
        sthx    $36
        ldhx    #$4647
        ldhx    $56
        cphx    #$6667
        cphx    $76
;;; XB
        dbnz    $3c, *+$40
        dbnza   *+$4e
        dbnzx   *+$5e
        dbnz    $6c,x, *+$70
        dbnz    ,x, *+$7e
;;; XE
        mov     $4f, $50
        mov     $5f, x+
        mov     #$6f, $70
        mov     x+, $7f
;;; 8X
        tap
        tpa
        pula
        psha
        pulx
        pshx
        pulh
        pshh
        clrh
;;; 9X
        bge     *-$6d
        blt     *-$6c
        bgt     *-$6b
        ble     *-$6a
        txs
        tsx
;;; AX
        ais     #-$58
        aix     #-$50
;;; 9E 6X
        neg     $61,sp
        cbeq    $62,sp, *+$67
        com     $64,sp
        lsr     $65,sp
        ror     $67,sp
        asr     $68,sp
        lsl     $69,sp
        asl     $69,sp
        rol     $6a,sp
        dec     $6b,sp
        dbnz    $6c,sp, *+$71
        inc     $6d,sp
        tst     $6e,sp
        clr     $70,sp
;;; 9E DX
        sub     $d1d2,sp
        cmp     $d2d3,sp
        sbc     $d3d4,sp
        cpx     $d4d5,sp
        and     $d5d6,sp
        bit     $d6d7,sp
        lda     $d7d8,sp
        sta     $d8d9,sp
        eor     $d9da,sp
        adc     $dadb,sp
        ora     $dbdc,sp
        add     $dcdd,sp
        ldx     $dfe0,sp
        stx     $e0e1,sp
;;; 9E EX
        sub     $e1,sp
        cmp     $e2,sp
        sbc     $e3,sp
        cpx     $e4,sp
        and     $e5,sp
        bit     $e6,sp
        lda     $e7,sp
        sta     $e8,sp
        eor     $e9,sp
        adc     $ea,sp
        ora     $eb,sp
        add     $ec,sp
        ldx     $ef,sp
        stx     $f0,sp

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
