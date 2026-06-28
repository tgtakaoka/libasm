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

        cpu     CP-1600
        org     X'0100'
        include "test_cp1600.inc"
;;; CP1600 branch-condition aliases BLGE/BLLT/BEQ/BNEQ (= BC/BNC/BZE/BNZE
;;; branching to L_ALIAS).  ASL's CP-1600 target rejects these mnemonics with
;;; "unknown instruction" (see ~/asl_bug_cp1600.md), so emit the exact bytes
;;; libasm assembles from test_cp1600_brnalias.inc.
        WORD    X'0221', X'0001'    ; BLGE   L_ALIAS
        WORD    X'0229', X'0003'    ; BLLT   L_ALIAS
        WORD    X'0224', X'0005'    ; BEQ    L_ALIAS
        WORD    X'022C', X'0007'    ; BNEQ   L_ALIAS
        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
