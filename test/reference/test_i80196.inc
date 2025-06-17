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

;;; 0X
        xch     08h, 06h
        xch     10h, 0f0eh[1ch]
;;; 1X
        xchb    16h, 15h
        xchb    20h, 1f1eh[1ch]
;;; CX
        bmov    0c4h, 0c2h
        cmpl    0cch, 0c8h
        bmovi   0d0h, 0ceh
;;; EX
        djnzw   0e2h, $-1ah
        tijmp   0e8h,0e4h,#0e6h
        dpts
        epts
;;; FX
        pusha
        popa
        idlpd   #0f7h

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
