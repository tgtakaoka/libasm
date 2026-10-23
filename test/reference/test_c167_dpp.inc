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

;;; A mem operand above 0FFFFH is a physical address
        MOV     R1, 1234H
        MOV     R1, 14234H
        MOV     3C010H, R1
        MOVB    RL0, 17FFFH
;;; and in an EXTended sequence it is in the page or segment of the sequence
        EXTP    #3FFH, #1
        MOV     R1, 0FFC010H
        EXTS    #12H, #2
        MOV     R1, 123456H
        MOV     12FFFEH, R2
        MOV     R1, 14234H
