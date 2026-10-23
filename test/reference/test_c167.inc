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

;;; Extended instructions
        ATOMIC  #1
        ATOMIC  #4
        EXTS    R1, #1
        NOP
        EXTP    R2, #2
        NOP
        NOP
        EXTS    #12H, #3
        NOP
        NOP
        NOP
        EXTP    #3FFH, #4
        NOP
        NOP
        NOP
        NOP
        JMPS    0FFH, 2345H
;;; reg and bitoff 80H-EFH address the ESFRs in an EXTR sequence
        EXTR    #3
        MOV     0F012H, #1234H
        BSET    0F102H.3
        BSET    0FD02H.3
        BSET    0FF02H.3
        PUSH    0FE12H
