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

        include "test_tms9900.asm"

        cpu     TMS9995

;;; TMS9995
        org     0100H

;;; 00X
        lst     r8
        lwp     r9
;;; 01X
        divs    r8
        mpys    r12

        end                     ; comment

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
