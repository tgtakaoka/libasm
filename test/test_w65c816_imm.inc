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

        ora     #$0a
        and     #$2a
        eor     #$4a
        adc     #$6a
        bit     #$8a
        lda     #$aa
        cmp     #$ca
        sbc     #$ea

        ldy     #$a1
        ldx     #$a3
        cpy     #$c1
        cpx     #$e1

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
