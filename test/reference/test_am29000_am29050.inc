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

;;; Instructions unique to the Am29050.  These operation codes are reserved
;;; for instruction emulation on every other family member.  ASL does not
;;; implement any of them, so this file is only used by the libasm and GNU
;;; assembler sources.

;;; OR-NOT Logical.  Not a floating-point instruction, unlike the rest here.
        ORN     gr96, gr97, gr98
        ORN     gr96, gr97, 15

;;; Floating-point multiply-accumulate and multiply-sum.  FMAC and DMAC take
;;; a function select and an accumulator register number.
        FMAC    0, 0, gr97, gr98
        FMAC    1, 3, gr97, gr98
        DMAC    0, 0, gr97, gr98
        DMAC    2, 1, gr97, gr98
        FMAC    4, 0, gr97, gr98
        FMAC    15, 3, gr97, gr98
        DMAC    8, 2, gr97, gr98
        FMSM    gr96, gr97, gr98
        DMSM    gr96, gr97, gr98

;;; Move to and from a floating-point accumulator.
        MTACC   gr97, 1, 0
        MTACC   gr97, 2, 3
        MFACC   gr96, 1, 0
        MFACC   gr96, 2, 3

;;; Special-purpose registers unique to the Am29050: the reason vector, region
;;; mapping, the shadow program counters, the instruction breakpoints and the
;;; exception opcode.
        MFSR    gr96, RSN
        MFSR    gr96, RMA0
        MFSR    gr96, RMC0
        MFSR    gr96, RMA1
        MFSR    gr96, RMC1
        MFSR    gr96, SPC0
        MFSR    gr96, SPC1
        MFSR    gr96, SPC2
        MFSR    gr96, IBA0
        MFSR    gr96, IBC0
        MFSR    gr96, IBA1
        MFSR    gr96, IBC1
        MFSR    gr96, EXOP
        MTSR    RSN, gr96
        MTSR    RMA0, gr96
        MTSR    IBC1, gr96
        MTSR    EXOP, gr96
        MTSRIM  RMC0, 0x0400

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
