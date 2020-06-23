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

        ldc     r2,   -129(rr4)
        ldc     r2,   -128(rr4)
        ldc     r2,     -1(rr4)
        ldc     r2, 0FFFFH(rr4) ; can be short indexed mode
        ldc     r2,   +127(rr4)
        ldc     r2,   +128(rr4)
        lde     r2,   -129(rr4)
        lde     r2,   -128(rr4)
        lde     r2,     -1(rr4)
        lde     r2, 0FFFFH(rr4) ; can be short indexed mode
        lde     r2,   +127(rr4)
        lde     r2,   +128(rr4)
        ldc       -129(rr4), r2
        ldc       -128(rr4), r2
        ldc         -1(rr4), r2
        ldc     0FFFFH(rr4), r2 ; can be short indexed mode
        ldc       +127(rr4), r2
        ldc       +128(rr4), r2
        lde       -129(rr4), r2
        lde       -128(rr4), r2
        lde         -1(rr4), r2
        lde     0FFFFH(rr4), r2 ; can be short indexed mode
        lde       +127(rr4), r2
        lde       +128(rr4), r2
        ldc     r2,5678H
        lde     r2,5678H
        ldc     5678H,r2
        lde     5678H,r2
        ldcd    r2,@rr4
        lded    r2,@rr4
        ldci    r2,@rr4
        ldei    r2,@rr4
        ldcpd   @rr4,r2
        ldepd   @rr4,r2
        ldcpi   @rr4,r2
        ldepi   @rr4,r2

        ldw     24H,46H
        ldw     24H,@45H
        ldw     24H,#4567H
        ldw     rr2,rr4
        ldw     rr2,@r3
        ldw     rr2,#4567H

        pushud  @23H,45H
        pushui  @23H,45H
        popud   23H,@45H
        popui   23H,@45H
        pushud  @r2,r3
        pushui  @r2,r3
        popud   r2,@r3
        popui   r2,@r3

        next
        enter
        exit
        wfi
        sb0
        sb1

        bitc    r2,#3
        bitr    r2,#3
        bits    r2,#3

        bor     r2,34H,#5
        bor     34H,#5,r2
        bcp     r2,34H,#5
        bxor    r2,34H,#5
        bxor    34H,#5,r2
        ldb     r2,34H,#5
        ldb     34H,#5,r4
        band    r2,34H,#5
        band    34H,#5,r4

        btjrf   $+42H,r2,#3
        btjrt   $+42H,r2,#3
        cpije   r2,@r3,$+42H
        cpijne  r2,@r3,$+42H

        div     24H,45H
        div     24H,@45H
        div     24H,#45H
        mult    24H,45H
        mult    24H,45H
        mult    24H,@45H
        mult    22H,#45H
        div     rr2,r5
        div     rr2,@r5
        div     rr2,#45H
        mult    rr2,r5
        mult    rr2,@r5
        mult    rr2,#45H

        srp0    #58H
        srp1    #68H

        call    #0D5H

        align   32
        assume  RP:00H
        ld      21H,R4
        ld      R4,21H
        ld      21H,#66H
        ld      R1,33H(R4)
        ld      33H(R4),R1
        ld      21H,24H
        ld      24H,21H
        ld      21H,@24H
        ld      @21H,24H
        add     21H,24H
        add     21H,@24H
        lde     R1,@RR4
        lde     @RR4,R1
        djnz    R1,$

        align   32
        assume  RP:20H
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
        lde     R1,@24H         ; lde R1,@RR4
        lde     R1,@RR4         ; lde R1,@RR4
        lde     @24H,21H        ; lde @RR4,R1
        djnz    21H,$           ; djnz R1,$

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
