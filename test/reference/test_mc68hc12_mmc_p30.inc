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
far0:   bra     far0x                   ; 20 00
far0x:  lbra    $C000                   ; 18 20 3F FA
        jmp     far0                    ; 06 80 00
        jsr     far0x                   ; 16 80 02
        call    far1                    ; 4A 80 00 31
        call    $8000,$31               ; 4A 80 00 31
        ldaa    far0,pc                 ; A6 F9 E9
        ldab    #page(far1)             ; C6 31
        ldx     #low16(far1)            ; CE 80 00

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
