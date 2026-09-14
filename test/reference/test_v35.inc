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

;# Register bank switching
        movspa
        brkcs   ax
        brkcs   cx
        brkcs   di
        tsksw   ax
        tsksw   bx
        tsksw   sp
        movspb  ax
        movspb  bp
        movspb  sp

;# Interrupt and standby
        retrbi
        fint
        stop

;# Bit test, clear and branch
        btclr   0EAH, 0, $+5
        btclr   0EAH, 7, $+5
        btclr   0FFH, 3, $+5

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
