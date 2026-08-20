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

;;; Floating-point instructions.  Only the Am29050 executes these in its
;;; on-chip floating-point unit; every other family member traps them to a
;;; software routine.  All of them take register operands only, so there is
;;; no 8-bit constant form.

;;; Arithmetic
        FADD    gr96, gr97, gr98
        DADD    gr96, gr97, gr98
        FSUB    gr96, gr97, gr98
        DSUB    gr96, gr97, gr98
        FMUL    gr96, gr97, gr98
        DMUL    gr96, gr97, gr98
        FDMUL   gr96, gr97, gr98
        FDIV    gr96, gr97, gr98
        DDIV    gr96, gr97, gr98

;;; Compare
        FEQ     gr96, gr97, gr98
        DEQ     gr96, gr97, gr98
        FGT     gr96, gr97, gr98
        DGT     gr96, gr97, gr98
        FGE     gr96, gr97, gr98
        DGE     gr96, gr97, gr98

;;; Convert, square root and classify.  CONVERT's control operands are the
;;; unsigned-integer select, the rounding mode, and the destination and
;;; source formats; SQRT and CLASS take a single format operand, for which the
;;; manual reserves both 0 and 3.
        CONVERT gr96, gr97, 0, 0, 1, 0
        CONVERT gr96, gr97, 1, 4, 2, 1
        SQRT    gr96, gr97, 1
        SQRT    gr96, gr97, 2
        CLASS   gr96, gr97, 1
        CLASS   gr96, gr97, 2

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
