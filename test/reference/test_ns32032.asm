; Copyright 2020 Tadashi G. Takaoka
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

        cpu     ns32032
        org     0x10000
        include "test_ns32000.inc"

        fpu     ns32081
        align   32
        include "test_ns32000_float.inc"

        pmmu    ns32082
        align   32
        include "test_ns32000_mmu.inc"

        end

; Local Variables:
; mode: asm
; End:
; vim: set ft=asm:
