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

;;; H8S/2000 advanced-mode-only additions. Per the H8S manual table 1.5,
;;; advanced-mode data uses @aa:32 (the H8/300H @aa:24 / @(d:24,ERn) data
;;; forms don't apply to H8S). JMP/JSR @aa:24 is shared with H8/300H for
;;; code addresses and lives here so this file is self-contained.

;;; MOV.B @aa:32 (6A20|rd abs32) / (6AA0|rs abs32)
        mov.b   @0x00000000:32,r0h     ; 6A20 0000 0000
        mov     @0xFFFFFFFF:32,r7l     ; 6A2F FFFF FFFF
        mov.b   r0h,@0x00000000:32     ; 6AA0 0000 0000
        mov     r7l,@0xFFFFFFFF:32     ; 6AAF FFFF FFFF

;;; MOV.W @aa:32 (6B20|rd abs32) / (6BA0|rs abs32)
        mov.w   @0x00000100:32,r0      ; 6B20 0000 0100
        mov     @0xFFFFFFFE:32,r7      ; 6B27 FFFF FFFE
        mov.w   r0,@0x00000100:32      ; 6BA0 0000 0100
        mov     r7,@0xFFFFFFFE:32      ; 6BA7 FFFF FFFE

;;; MOV.L @aa:32 (0100 6B20|erd abs32) / (0100 6BA0|ers abs32)
        mov.l   @0x00000100:32,er0     ; 0100 6B20 0000 0100
        mov     @0xFFFFFFFE:32,er7     ; 0100 6B27 FFFF FFFE
        mov.l   er0,@0x00000100:32     ; 0100 6BA0 0000 0100
        mov     er7,@0xFFFFFFFE:32     ; 0100 6BA7 FFFF FFFE

;;; JMP/JSR @aa:24 (5A abs24 / 5E abs24); program-address form shared with
;;; H8/300H but listed here so this file stands alone.
        jmp     @0x000100:24           ; 5A00 0100
        jmp     @0xFFFFE0:24           ; 5AFF FFE0
        jsr     @0x000100:24           ; 5E00 0100
        jsr     @0xFFFFE0:24           ; 5EFF FFE0

;;; LDC/STC ccr,@aa:32 (0140 6B20 abs32 / 0140 6BA0 abs32)
        ldc     @0x00000100:32,ccr     ; 0140 6B20 0000 0100
        ldc     @0xFFFFFFE0:32,ccr     ; 0140 6B20 FFFF FFE0
        stc     ccr,@0x00000100:32     ; 0140 6BA0 0000 0100
        stc     ccr,@0xFFFFFFE0:32     ; 0140 6BA0 FFFF FFE0

;;; LDC/STC exr,@aa:32 (0141 6B20 abs32 / 0141 6BA0 abs32)
        ldc     @0x00000100:32,exr     ; 0141 6B20 0000 0100
        ldc     @0xFFFFFFE0:32,exr     ; 0141 6B20 FFFF FFE0
        stc     exr,@0x00000100:32     ; 0141 6BA0 0000 0100
        stc     exr,@0xFFFFFFE0:32     ; 0141 6BA0 FFFF FFE0

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
