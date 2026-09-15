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

;;; SH7205 is ASL's SH-2A with both FPU widths; FPU OFF/ON brackets the
;;; sections the same way test_sh2a.asm does.
        cpu     SH7205
        fpu     off
        org     0
        include "test_sh1.inc"
        include "test_sh2.inc"
        include "test_sh2a.inc"
        fpu     on
        include "test_sh2e.inc"
        include "test_sh2e_fpu.inc"
        include "test_sh2a_fpu.inc"
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
