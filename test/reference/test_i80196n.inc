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

;;; 1X
        est     52, [32]
        est     52, 123456h[32]
        estb    35, [32]
        estb    35, 123456h[32]
;;; EX
        ebr     [50h]
        ebmovi  40h, 32
        ejmp    $
        ejmp    $+1234h
        ejmp    $-1234h
        eld     52, [32]
        eld     52, 123456h[32]
        eldb    35, [32]
        eldb    35, 123456h[32]
;;; FX
        ecall   $
        ecall   $+1234h
        ecall   $-1234h

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
