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
        lbra    far0                    ; 18 20 BF FC
        jsr     far0                    ; 16 80 00
        ldaa    $0FD567                 ; B6 15 67
        staa    $13F856                 ; 7A 08 56
        ldab    $0FD234                 ; D6 34
        gldaa   $408123                 ; 18 B6 81 23
        gstaa   $1234                   ; 18 5A 34

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
