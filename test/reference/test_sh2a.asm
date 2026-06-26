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

;;; SH-2A reference test wrapper. The 32-bit "long" forms in test_sh2a.inc
;;; (MOV.B/W/L @(disp12,Rn), MOVU.B/W, FMOV.S @(disp12,Rn), the bit-
;;; manipulation memory variants) are excluded from the libasm assembler
;;; for now; this wrapper exercises everything else.

        cpu     SH-2A
        .option  "fpu", "true"
        .org     0
        include "test_sh1.inc"
        include "test_sh2.inc"
        include "test_sh2e.inc"
;;; test_sh2a.inc not yet included: contains 12-bit-displacement and 20-bit-
;;; immediate forms that need M_D12N/M_D12M encoders.
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
