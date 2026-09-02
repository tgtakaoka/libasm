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

;;; MEMB mode 5, IP-relative: the effective address is IP + 8 + displacement,
;;; and the operand is the DISPLACEMENT itself.  Table 5-1 of the manual gives
;;; the syntax as "exp (IP)" and 5-8 says "the displacement plus a constant of
;;; 8 is added to the IP of the instruction"; the Assembler User's Guide writes
;;; "st g5, 4(ip)" the same way.  GNU as agrees.
;;;
;;; ASL does not: it takes the operand as the target address and subtracts
;;; IP + 8 from it, so it encodes every line here differently.  That is one of
;;; the reasons i960 is in ASL_exclude; see ~/asl_bugs/asl_bug_i960.md.

        ldob    -8(ip), g8
        ldos    0(ip), g8
        ldib    4(ip), g8
        ldis    0x40(ip), g8
        ld      -0x40(ip), g8
        ldl     0x1234(ip), g8
        ldt     -0x1234(ip), g8
        ldq     0x12345678(ip), g8
        stob    g8, -8(ip)
        st      g8, 0x40(ip)
        lda     0(ip), g8
        bx      -0x10(ip)
        balx    0x20(ip), g8
        callx   -0x7FFFFFF0(ip)
