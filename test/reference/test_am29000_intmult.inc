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

;;; Integer multiply instructions executed by the on-chip 32-by-32 multiplier
;;; of the Am29240, Am29243, Am29040 and Am29050.  The Am29200, Am29205,
;;; Am29245, Am29030 and Am29035 trap these to a software routine instead.

        MULTIPLY gr96, gr97, gr98
        MULTIPLU gr96, gr97, gr98
        MULTM   gr96, gr97, gr98
        MULTMU  gr96, gr97, gr98

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
