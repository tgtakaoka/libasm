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

;;; SH-2A FPU additions (double <-> single conversion). Require the optional
;;; SH-2A FPU (.fpu on).

;;; FCNVDS DRm,FPUL (m format: 1111mmm010111101) double -> single+truncate -> FPUL
        FCNVDS  DR0,FPUL                ;# F0BD
        FCNVDS  DR14,FPUL               ;# FEBD

;;; FCNVSD FPUL,DRn (n format: 1111nnn010101101) FPUL -> single -> double
        FCNVSD  FPUL,DR0                ;# F0AD
        FCNVSD  FPUL,DR14               ;# FEAD

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
