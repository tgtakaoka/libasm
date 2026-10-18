;;; Copyright 2026 Tadashi G. Takaoka
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
        cpu     68HC12X
        org     $0100
        include "test_mc68hc12.inc"
        include "test_mc68hc12_trapcommon.inc"
;;; ASL does not encode these as the manual does; emit the bytes directly.
;;; The libasm wrapper assembles the equivalent mnemonics.
        dc.b    $18,$08,$C0,$12                 ;;; movb    #$12,*+4,pc
        dc.b    $18,$09,$C0,$12,$34             ;;; movb    $1234,*+5,pc
        dc.b    $18,$0A,$C0,$C0                 ;;; movb    *+4,pc,*+4,pc
        dc.b    $18,$0D,$C0,$12,$34             ;;; movb    *+5,pc,$1234
        dc.b    $18,$00,$C0,$12,$34             ;;; movw    #$1234,*+5,pc
        dc.b    $18,$01,$C0,$12,$34             ;;; movw    $1234,*+5,pc
        dc.b    $18,$02,$C0,$C0                 ;;; movw    *+4,pc,*+4,pc
        dc.b    $18,$05,$C0,$12,$34             ;;; movw    *+5,pc,$1234
        include "test_mc68hcs12x.inc"
;;; ASL does not encode these as the manual does; emit the bytes directly.
;;; The libasm wrapper assembles the equivalent mnemonics.
        dc.b    $18,$A7                         ;;; sys
        dc.b    $18,$37,$F2,$12,$34,$34         ;;; btas    $1234,sp,#$34
        dc.b    $B7,$4D                         ;;; sex     d,x
        dc.b    $B7,$4E                         ;;; sex     d,y
        dc.b    $B7,$4F                         ;;; sex     d,sp
        dc.b    $B7,$D9                         ;;; exg     xl,b
        dc.b    $B7,$E9                         ;;; exg     yl,b
        dc.b    $B7,$F9                         ;;; exg     spl,b
        dc.b    $18,$08,$F8,$80,$12             ;;; movb    #$12,*+$85,pc
        dc.b    $18,$08,$FA,$0F,$FF,$12         ;;; movb    #$12,*+$1005,pc
        dc.b    $18,$0A,$C1,$E0,$64             ;;; movb    *+5,pc,100,x
        dc.b    $18,$0A,$C0,$E2,$12,$34         ;;; movb    *+4,pc,$1234,x
        dc.b    $18,$0A,$C0,$E3,$12,$34         ;;; movb    *+4,pc,[$1234,x]
        dc.b    $18,$0A,$C0,$E7                 ;;; movb    *+4,pc,[d,x]
        dc.b    $18,$0A,$F8,$80,$02             ;;; movb    *+$85,pc,2,x
        dc.b    $18,$0A,$FA,$0F,$FF,$02         ;;; movb    *+$1005,pc,2,x
        dc.b    $18,$0A,$01,$F8,$80             ;;; movb    1,x,*+$85,pc
        dc.b    $18,$0A,$01,$FA,$0F,$FF         ;;; movb    1,x,*+$1005,pc
        dc.b    $18,$0A,$FA,$0F,$FF,$FA,$1F,$F8 ;;; movb    *+$1005,pc,*+$2000,pc
        dc.b    $18,$00,$FA,$0F,$FF,$12,$34     ;;; movw    #$1234,*+$1006,pc
        dc.b    $18,$02,$F8,$80,$EA,$12,$34     ;;; movw    *+$85,pc,$1234,y
        dc.b    $18,$02,$FA,$0F,$FF,$EA,$12,$34 ;;; movw    *+$1005,pc,$1234,y
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
