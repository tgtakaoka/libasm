;;; Copyright 2020 Tadashi G. Takaoka
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

        ld      21H,R4          ; ld R1,R4
        ld      R4,21H          ; ld R4,R1
        ld      21H,#66H        ; ld R1,#66H
        ld      21H,33H(R4)     ; ld R1,33H(R4)
        ld      33H(R4),21H     ; ld 33H(R4),R1
        ld      21H,24H         ; ld R1,R4
        ld      24H,21H         ; ld R4,R1
        ld      21H,@24H        ; ld R1,@R4
        ld      @21H,24H        ; ld @R4,R1
        add     21H,24H         ; add R1,R4
        add     21H,@24H        ; add R1,@R4
        lde     21H,@24H        ; lde R1,@RR4
        lde     @24H,21H        ; lde @RR4,R1
;       ldei    @21H,@24H       ; ldei @R1,@RR4
;       ldei    @24H,@21H       ; ldei @RR4,@R1
        djnz    21H,$           ; djnz R1,$

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
