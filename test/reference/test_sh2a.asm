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

;;; SH-2A reference test wrapper. The SH-2A FPU is optional: the SH-2A without
;;; FPU is not upward-compatible with the SH-2E, while the SH-2A with FPU is.
;;; Without the FPU there are no FPU system registers, so the no-FPU section
;;; (.fpu off) covers only SH-1/SH-2 and the SH-2A CPU additions. The FPU
;;; section (.fpu on) adds the SH-2E FPU-related CPU instructions (LDS/STS
;;; FPUL/FPSCR), the SH-2E floating-point instructions, and the SH-2A FPU
;;; additions.
;;;
;;; test_sh2a_disp12.inc (the 32-bit @(disp12,Rn) forms) is not included: those
;;; have no table rows yet and do not assemble.

        .cpu     SH-2A
        .fpu     off
        .org     0
        .include "test_sh1.inc"
        .include "test_sh2.inc"
        .include "test_sh2a.inc"
        .fpu     on
        .include "test_sh2e.inc"
        .include "test_sh2e_fpu.inc"
        .include "test_sh2a_fpu.inc"
        .end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
