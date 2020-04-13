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
        lsrd
        asld
        lsld
;;; 2X
        brn     *+$24
;;; 3X
        pulx
        abx
        pshx
        mul
;;; 8X
        subd    #$8485
;;; 9X
        subd    $94
        jsr     $9e
;;; AX
        subd    164,x
;;; BX
        subd    $b4b5
;;; CX
        addd    #$c4c5
        ldd     #$cdce
;;; DX
        addd    $d4
        ldd     $dd
        std     $de
;;; EX
        addd    228,x
        ldd     237,x
        std     238,x
;;; FX
        addd    $f4f5
        ldd     $fdfe
        std     $feff

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
