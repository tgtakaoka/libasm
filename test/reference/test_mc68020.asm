;;; Copyright 2025 Tadashi G. Takaoka
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

        cpu     68020
        fpu     on
        org     $10000
        include "test_mc68000.inc"
        include "test_mc68882.inc"
        include "test_mc68010.inc"
        include "test_mc68020.inc"
;;; Module
        rtm     d2
        rtm     a2
        callm   #$FF, (a2)
        callm   #0, ($1234, a2)
        callm   #1, (18,a2,d3.w)
        callm   #2, ($ffffff00).w
        callm   #3, ($12345678).l
        callm   #4, (*+$1234,pc)
        callm   #5, (*+18,pc,d3.l)
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
