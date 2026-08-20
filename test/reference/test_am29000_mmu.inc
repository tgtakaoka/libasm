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

;;; Special-purpose registers of the memory management unit.  The Am29200 and
;;; Am29205 have none, and do not implement these two register numbers.

        MFSR    gr96, MMU
        MFSR    gr96, LRU
        MTSR    MMU, gr96
        MTSR    LRU, gr96
        MTSRIM  MMU, 0x0400

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
