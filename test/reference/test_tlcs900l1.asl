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

;;; TLCS-900/L1 CPU core is identical to TLCS-900/H per Toshiba family spec.
;;; ASL has no /L1-specific cpu name; use 96c141 (= /H) with MAXMODE ON.

        cpu     96c141
        org     0100h
        SUPMODE ON
        MAXMODE ON
        include "test_tlcs900_common.inc"
        include "test_tlcs900_ldx.inc"
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
