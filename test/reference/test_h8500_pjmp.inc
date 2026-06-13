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

;;; Page-crossing branch instructions.
;;; Valid only on H8/532 (page jumps are invalid in minimum mode).
;;; Requires the wrapper to enable "maxmode on" for ASL.

;;; PJMP - Page Jump (unconditional)
        pjmp    @0x010000               ; 13 01 00 00
        pjmp    @0x123456               ; 13 12 34 56
        pjmp    @r0                     ; 11 C0
        pjmp    @r2                     ; 11 C2
        pjmp    @r6                     ; 11 C6

;;; PJSR - Page Jump to Subroutine
        pjsr    @0x010000               ; 03 01 00 00
        pjsr    @0x654321               ; 03 65 43 21
        pjsr    @r0                     ; 11 C8
        pjsr    @r2                     ; 11 CA
        pjsr    @r6                     ; 11 CE

;;; PRTS - Page ReTurn from Subroutine
        prts                            ; 11 19

;;; PRTD - Page ReTurn and Deallocate stack
        prtd    #4                      ; 11 14 04
        prtd    #-1                     ; 11 14 FF
        prtd    #0x1234                 ; 11 1C 12 34
        prtd    #-256                   ; 11 1C FF 00

;;; Local Variables:
;;; mode: asm
;;; End:
