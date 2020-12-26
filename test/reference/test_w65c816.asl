;;; Copyright 2020 Tadashi G. Takaoka
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

        cpu     65816

        org     $100000
        assume  m:1
        assume  x:1
        include "test_mos6502.inc"
        align   32
        include "test_g65sc02.inc"
        align   32
        include "test_w65c02s.inc"
        align   32
        include "test_w65c816.inc"

	org     $200000
        assume  m:1
        assume  x:0
        include "test_mos6502.inc"
        align   32
        include "test_g65sc02.inc"
        align   32
        include "test_w65c02s.inc"
        align   32
        include "test_w65c816.inc"

	org     $300000
        assume  m:0
        assume  x:1
        include "test_mos6502.inc"
        align   32
        include "test_g65sc02.inc"
        align   32
        include "test_w65c02s.inc"
        align   32
        include "test_w65c816.inc"

	org     $400000
        assume  m:0
        assume  x:0
        include "test_mos6502.inc"
        align   32
        include "test_g65sc02.inc"
        align   32
        include "test_w65c02s.inc"
        align   32
        include "test_w65c816.inc"

        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
