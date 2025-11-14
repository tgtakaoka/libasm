;;; Copyright 2025 Tadashi G. Takaoka
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

;;; Load and Store
        ldi     r0, r1
        ldiu    @1234H, r2
        ldi     1234H, r3
        ldilo   *+ar0(56H), r4
        ldils   *-ar1(78H), r5
        ldihi   *++ar2(12H), r6
        ldihs   *--ar3(34H), r7
        ldieq   *ar4++(56H), r0
        ldine   *ar5--(78H), r1
        ldilt   *ar6++(12H)%, r2
        ldile   *ar7--(34H)%, r3
        ldigt   *+ar0(ir0), r4
        ldige   *-ar1(ir0), r5
        ldf     *++ar2(ir0), r6
        ldf     1.23, r7
        ldfu    *--ar3(ir0), r0
        ldflo   *ar4++(ir0), r1
        ldfls   *ar5--(ir0), r2
        ldfhi   *ar6++(ir0)%, r3
        ldfhs   *ar7--(ir0)%, r4
        ldfeq   *+ar0(ir1), r5
        ldfne   *-ar1(ir1), r6
        ldflt   *++ar2(ir1), r7
        ldfle   *--ar3(ir1), r0
        ldfgt   *ar4++(ir1), r1
        ldfge   *ar5--(ir1), r2
        lde     *ar6++(ir1)%, r3
        ldf     *ar7--(ir1)%, r4
        lde     r5, r6
        lde     0FFH, r7
        ldm     @1234, r0
        ldm     0FFH, r7
        ldp     0FFH, dp
        pop     ar0
        popf    r0
        push    ar1
        pushf   r1
        sti     r0, *ar4++(12H)
        stf     r1, *ar5--(34H)
;;; 2-Operand
        absi    *ar6+(12H)%, r2
        absi    @1234H, r3
        absi    r4
        absf    *+ar7(ir0), r5
        absf    -1.25, r6
        absf    r7
        sti     r5, *-ar1(ir0)
        sti     r6, *++ar2(ir0)
        sti     r7, *--ar3(ir0)
        sti     r0, *ar4++(ir0)
        sti     r1, *ar5--(ir0)
        sti     r2, *ar6++(ir0)%
        sti     r3, *ar7--(ir0)%
        sti     r4, *+ar0(ir1)
        sti     r5, *-ar1(ir1)
        sti     r6, *++ar2(ir1)
        sti     r7, *--ar3(ir1)
        sti     r0, *ar4++(ir1)
        sti     r1, *ar5--(ir1)
        sti     r2, *ar6++(ir1)%
        sti     r3, *ar7--(ir1)%
        stf     r4, @1234H
        stf     r5, *ar7--(ir1)%
        absf    r3
        absf    r4, r5
        addi    r6, r7
        addc    r0, r1
        addf    r2, r3
        and     r4, r5
        andn    r6, r7
        ash     1, r0
        ash     -1, r1
        cmpi    r2, r3
        cmpf    r4, r5
        fix     r6
        fix     r7, r0
        float   r1
        float   r2, r3
        lsh     2, r4
        lsh     -2, r5
        mpyi    r6, r7
        mpyf    r0, r1
        negi    r2
        negi    r3, r4
        negb    r5
        negb    r6, r7
        negf    r0
        negf    r1, r2
        norm    r3
        norm    r4, r5
        not     r6
        not     r7, r0
        or      r1, r2
        rnd     r3
        rnd     r4, r5
        rol     r6
        rolc    r7
        ror     r0
        rorc    r1
        

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
