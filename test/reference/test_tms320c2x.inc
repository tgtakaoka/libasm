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

        mpya    3BH
        mpys    *BR0-, AR5
        addc    21H
        subb    *0+, 5
        bc      0BA98H, *0-, AR4
        bnc     0CBA9H, *0+, 5
        zalr    *BR0-, AR5
        spl     *0-, 6
        sph     *0+, AR7
        adrk    7FH
        sbrk    80H
        addk    0CDH
        subk    0CEH
        rc
        sc
        rtc
        stc
        rol
        ror
        rfsm
        sfsm
        rhm
        shm
        norm    *-
        norm    *+
        norm    *0+
        norm    *BR0-
        mpyu    *0+, AR5

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
