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
        idiv
        fdiv
;;; 1X
        brset   $13,#$14,*+$19
        brclr   $14,#$15,*+$1A
        bset    $15,#$16
        bclr    $16,#$17
        bset    $1D,x,#$1E
        bclr    $1E,x,#$1F
        brset   $1F,x,#$20,*+$25
        brclr   $20,x,#$21,*+$26
;;; 8X
        xgdx
;;; CX
        stop
;;; 180X
        iny
        dey
;;; 181X
        bset    $1D,y,#$1E
        bclr    $1E,y,#$1F
        brset   $1F,y,#$20,*+$25
        brclr   $20,y,#$21,*+$26
;;; 183X
        tsy
        tys
        puly
        aby
        pshy
;;; 186X
        neg     0,y
        com     0,y
        lsr     1,y
        ror     6,y
        asr     8,y
        lsl     9,y
        asl     9,y
        rol     10,y
        dec     11,y
        inc     13,y
        tst     14,y
        jmp     15,y
        clr     255,y
;;; 188X
        cpy     #$8d8e
        xgdy
;;; 189X
        cpy     $9d
;;; 18AX
        suba    0,y
        cmpa    0,y
        sbca    0,y
        subd    1,y
        anda    2,y
        bita    3,y
        ldaa    4,y
        staa    5,y
        eora    6,y
        adca    127,y
        oraa    128,y
        adda    255,y
        cpy     0,y
        jsr     2,y
        lds     4,y
        sts     6,y
;;; 18BX
        cpy     $bdbe
;;; 18CX
        ldy     #$cfd0
;;; 18DX
        ldy     $df
        sty     $e0
;;; 18EX
        addd    $e4,y
        ldd     $ed,y
        ldy     $ef,y
        sty     $f0,y
;;; 10FX
        ldy     $ff00
        sty     $1234
;;; 1AXX
        cpd     #$8485
        cpd     $94
        cpd     $a4,x
        cpy     $ad,x
        cpd     $b4b5
        ldy     $ef,x
        sty     $f0,x
;;; CDXX
        cpd     $a4,y
        cpx     $ad,y
        ldx     $ef,y
        stx     $f0,y

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
