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

;;; @Rn,#IM
        add     @23H,#45H
        adc     @23H,#45H
        sub     @23H,#45H
        sbc     @23H,#45H
        or      @23H,#45H
        and     @23H,#45H
        tcm     @23H,#45H
        tm      @23H,#45H
        cp      @23H,#45H
        xor     @23H,#45H

        ldei    @r2,@rr4
        ldei    @rr4,@r2
        ldci    @r2,@rr4
        ldci    @rr4,@r2

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
