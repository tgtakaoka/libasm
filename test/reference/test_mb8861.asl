;;; Copyright 2022 Tadashi G. Takaoka
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

        cpu     6800
        org     $0100
        include "test_mc6800.inc"
        align   32
        fcb     $71, $72, $73   ; nim #$72,$73,x
        fcb     $72, $73, $74   ; oim #$73,$74,x
        fcb     $75, $76, $77   ; xim #$76,$77,x
        fcb     $7b, $7c, $7d   ; tmm #$7c,$7d,x
        fcb     $ec, $ed        ; adx #$ed
        fcb     $fc, $fd, $fe   ; adx $fdfe
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
