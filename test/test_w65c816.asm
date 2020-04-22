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
        assume  m:1
        assume  x:1

        org     $100000
        include "test_mos6502.inc"
        org     $100150
        include "test_w65sc02.inc"
        org     $100188
        include "test_w65c02s.inc"
        org     $100190
        include "test_w65c816.inc"

        org     $110000
        assume  m:0
        assume  x:0
        include "test_w65c816_imm.inc"

        org     $110100
        assume  m:0
        assume  x:1
        include "test_w65c816_imm.inc"

        org     $110200
        assume  m:1
        assume  x:0
        include "test_w65c816_imm.inc"

        org     $110300
        assume  m:1
        assume  x:1
        include "test_w65c816_imm.inc"

        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
