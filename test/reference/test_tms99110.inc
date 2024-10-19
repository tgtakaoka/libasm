;;; Copyright 2024 Tadashi G. Takaoka
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

        cr      @1234h(r13), @3456h(r5)
        mm      @1234h(r13), @3456h(r5)

        lds
        mov     *r3+, r4
        ldd
        mov     r5, *r6
        lds
        ldd
        mov     *r3+, *r4+

        cri
        negr
        cre
        cer

        ar      r2
        cir     *r3
        sr      @1234h
        mr      @1234h(r4)
        dr      *r5+
        lr      r6
        str     *r7

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
