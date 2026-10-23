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

;;; EXTPR and EXTSR also begin an ESFR sequence
        EXTSR   R3, #1
        PUSH    0F012H
        EXTPR   R15, #1
        BSET    0F102H.3
        EXTSR   #0FFH, #2
        PUSH    R1
        POP     0F012H
        EXTPR   #1, #1
        MOV     0F012H, #1234H
