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

;;; MINC/MDEC (word operand, buffer size must be power of 2)

        MINC1   2,BC
        MINC1   4,DE
        MINC1   8,HL
        MINC2   4,BC
        MINC2   8,WA
        MINC4   8,BC
        MINC4   16,DE
        MDEC1   2,BC
        MDEC2   4,DE
        MDEC4   8,HL

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
