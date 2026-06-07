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

;;; H8/300H advanced-mode-only additions (24-bit addressing forms)

;;; MOV.B @(d:24,ERn) (78|ern*16 6A|2/A|rd disp24)
;;; Direction is encoded in the 6Ax nibble (2=load, A=store); 78xx prefix has no
;;; direction bit for byte/word forms.
        mov.b   @(0x000000:24,er0),r0h ; 7800 6A20 0000 0000
        mov     @(0xFFFFFF:24,er7),r0h ; 7870 6A20 00FF FFFF
        mov.b   r0h,@(0x000000:24,er0) ; 7800 6AA0 0000 0000
        mov     r7l,@(0xFFFFFF:24,er7) ; 7870 6AAF 00FF FFFF

;;; MOV.W @(d:24,ERn) (78|ern*16 6B|2/A|rd disp24)
        mov.w   @(0x000100:24,er0),r0  ; 7800 6B20 0000 0100
        mov     @(0xFFFFFE:24,er7),r0  ; 7870 6B20 00FF FFFE
        mov.w   r0,@(0x000100:24,er0)  ; 7800 6BA0 0000 0100
        mov     r7,@(0xFFFFFE:24,er7)  ; 7870 6BA7 00FF FFFE

;;; MOV.L @(d:24,ERn) (0100 78|0/8|ern*16 6B|2/A|erd disp24)
;;; Note: ASL has a bug encoding the store form — omits the direction bit in the
;;; 78xx prefix byte (writes 7800/7870 instead of 7880/78F0). Comments below show
;;; the manual-correct encoding (matches GAS); ASL output will diverge here.
        mov.l   @(0x000100:24,er0),er1 ; 0100 7800 6B21 0000 0100
        mov     @(0xFFFFFE:24,er7),er0 ; 0100 7870 6B20 00FF FFFE
        mov.l   er0,@(0x000100:24,er0) ; 0100 7880 6BA0 0000 0100
        mov     er7,@(0xFFFFFE:24,er7) ; 0100 78F0 6BA7 00FF FFFE

;;; MOV.B @aa:24 (6A20|rd abs24) / (6AA0|rs abs24)
        mov.b   @0x000000:24,r0h       ; 6A20 0000 0000
        mov     @0xFFFFFF:24,r7l       ; 6A2F 00FF FFFF
        mov.b   r0h,@0x000000:24       ; 6AA0 0000 0000
        mov     r7l,@0xFFFFFF:24       ; 6AAF 00FF FFFF

;;; MOV.W @aa:24 (6B20|rd abs24) / (6BA0|rs abs24)
        mov.w   @0x000100:24,r0        ; 6B20 0000 0100
        mov     @0xFFFFFE:24,r7        ; 6B27 00FF FFFE
        mov.w   r0,@0x000100:24        ; 6BA0 0000 0100
        mov     r7,@0xFFFFFE:24        ; 6BA7 00FF FFFE

;;; MOV.L @aa:24 (0100 6B20|erd abs24) / (0100 6BA0|ers abs24)
        mov.l   @0x000100:24,er0       ; 0100 6B20 0000 0100
        mov     @0xFFFFFE:24,er7       ; 0100 6B27 00FF FFFE
        mov.l   er0,@0x000100:24       ; 0100 6BA0 0000 0100
        mov     er7,@0xFFFFFE:24       ; 0100 6BA7 00FF FFFE

;;; JMP/JSR @aa:24 (5A abs24 / 5E abs24)
        jmp     @0x000100:24           ; 5A00 0100
        jmp     @0xFFFFE0:24           ; 5AFF FFE0
        jsr     @0x000100:24           ; 5E00 0100
        jsr     @0xFFFFE0:24           ; 5EFF FFE0

;;; LDC/STC ccr,@aa:24 (0140 6B20 abs24 / 0140 6BA0 abs24)
        ldc     @0x000100:24,ccr       ; 0140 6B20 0000 0100
        ldc     @0xFFFFE0:24,ccr       ; 0140 6B20 00FF FFE0
        stc     ccr,@0x000100:24       ; 0140 6BA0 0000 0100
        stc     ccr,@0xFFFFE0:24       ; 0140 6BA0 00FF FFE0

;;; LDC/STC ccr,@(d:24,ERn) (0140 78|ern*16 6B|2/A|0 disp24)
;;; STC has no direction bit in the 78xx prefix (same shape as MOV.B/W stores).
        ldc     @(0x000100:24,er0),ccr ; 0140 7800 6B20 0000 0100
        ldc     @(0xFFFFFE:24,er7),ccr ; 0140 7870 6B20 00FF FFFE
        stc     ccr,@(0x000100:24,er0) ; 0140 7800 6BA0 0000 0100
        stc     ccr,@(0xFFFFFE:24,er7) ; 0140 7870 6BA0 00FF FFFE

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
