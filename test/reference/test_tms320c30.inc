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

;;; Load
        lde     r0, r1
        lde     @1234H, r2
        lde     *+ar1(2), r3
        lde     *-ar2(3), r4
        lde     *++ar3(4), r5
        lde     *--ar4(5), r6
        lde     *ar5++(6), r7
        lde     *ar6--(7), r0
        lde     *ar7++(8)%, r1
        lde     *ar0--(9)%, r2
        lde     *+ar1(ir0), r3
        lde     *-ar2(ir1), r4
        lde     *++ar3(ir0), r5
        lde     *--ar4(ir1), r6
        lde     *ar5++(ir0), r7
        lde     *ar6--(ir1), r0
        lde     *ar7++(ir0)%, r1
        lde     *ar0--(ir1)%, r2
        lde     *ar1, r3
        lde     *ar2++(ir0)b, r4
        lde     1.0, r5
        lde     -1.5, r6
        lde     0.0, r7
        lde     -0.50024, r0
        ldf     r0, r1
        ldf     @1234H, r2
        ldf     *+ar1(2), r3
        ldf     *-ar2(3), r4
        ldf     *++ar3(4), r5
        ldf     *--ar4(5), r6
        ldf     *ar5++(6), r7
        ldf     *ar6--(7), r0
        ldf     *ar7++(8)%, r1
        ldf     *ar0--(9)%, r2
        ldf     *+ar1(ir0), r3
        ldf     *-ar2(ir1), r4
        ldf     *++ar3(ir0), r5
        ldf     *--ar4(ir1), r6
        ldf     *ar5++(ir0), r7
        ldf     *ar6--(ir1), r0
        ldf     *ar7++(ir0)%, r1
        ldf     *ar0--(ir1)%, r2
        ldf     *ar1, r3
        ldf     *ar2++(ir0)b, r4
        ldf     1.0, r5
        ldf     -1.5, r6
        ldf     0.0, r7
        ldf     -0.50024, r0
        ldi     r0, r1
        ldi     @1234H, ar2
        ldi     *+ar1(2), r3
        ldi     *-ar2(3), r4
        ldi     *++ar3(4), r5
        ldi     *--ar4(5), r6
        ldi     *ar5++(6), r7
        ldi     *ar6--(7), r0
        ldi     *ar7++(8)%, r1
        ldi     *ar0--(9)%, r2
        ldi     *+ar1(ir0), r3
        ldi     *-ar2(ir1), r4
        ldi     *++ar3(ir0), r5
        ldi     *--ar4(ir1), r6
        ldi     *ar5++(ir0), r7
        ldi     *ar6--(ir1), r0
        ldi     *ar7++(ir0)%, r1
        ldi     *ar0--(ir1)%, r2
        ldi     *ar1, r3
        ldi     *ar2++(ir0)b, r4
        ldi     0, r5
        ldi     0c00H, r6
        ldi     -8000H, r7
        ldi     -1, r0
        ldp     345678H
        ldm     r0, r1
        ldm     @1234H, r2
        ldm     *+ar1(2), r3
        ldm     *-ar2(3), r4
        ldm     *++ar3(4), r5
        ldm     *--ar4(5), r6
        ldm     *ar5++(6), r7
        ldm     *ar6--(7), r0
        ldm     *ar7++(8)%, r1
        ldm     *ar0--(9)%, r2
        ldm     *+ar1(ir0), r3
        ldm     *-ar2(ir1), r4
        ldm     *++ar3(ir0), r5
        ldm     *--ar4(ir1), r6
        ldm     *ar5++(ir0), r7
        ldm     *ar6--(ir1), r0
        ldm     *ar7++(ir0)%, r1
        ldm     *ar0--(ir1)%, r2
        ldm     *ar1, r3
        ldm     *ar2++(ir0)b, r4
        ldm     1.0, r5
        ldm     -1.5, r6
        ldm     0.0, r7
        ldm     -0.50024, r0
        ldm     2.5508, r1
        pop     r0
        pop     ar7
        pop     dp
        pop     ir1
        pop     rc
        popf    r7
        ldfu    r0, r1
        ldflo   r1, r2
        ldfls   *+ar1(2), r3
        ldfhi   *-ar2(3), r4
        ldfhs   *++ar3(4), r5
        ldfeq   *--ar4(5), r6
        ldfne   *ar5++(6), r7
        ldflt   *ar6--(7), r0
        ldfle   *ar7++(8)%, r1
        ldfgt   *ar0--(9)%, r2
        ldfge   *+ar1(ir0), r3
        ldfnv   *-ar2(ir1), r4
        ldfv    *++ar3(ir0), r5
        ldfnuf  *--ar4(ir1), r6
        ldfuf   *ar5++(ir0), r7
        ldfnlv  *ar6--(ir1), r0
        ldflv   *ar7++(ir0)%, r1
        ldfnluf *ar0--(ir1)%, r2
        ldfluf  *ar1, r3
        ldfzuf  *ar2++(ir0)b, r4
        ldfeq   1.0, r5
        ldfne   -1.5, r6
        ldfhi   0.0, r7
        ldfhs   -0.50024, r0
        ldiu    r0, r1
        ldilo   r1, ar2
        ldils   *+ar1(2), r3
        ldihi   *-ar2(3), r4
        ldihs   *++ar3(4), r5
        ldieq   *--ar4(5), r6
        ldine   *ar5++(6), r7
        ldilt   *ar6--(7), r0
        ldile   *ar7++(8)%, r1
        ldigt   *ar0--(9)%, r2
        ldige   *+ar1(ir0), r3
        ldinv   *-ar2(ir1), r4
        ldiv    *++ar3(ir0), r5
        ldinuf  *--ar4(ir1), r6
        ldiuf   *ar5++(ir0), r7
        ldinlv  *ar6--(ir1), r0
        ldilv   *ar7++(ir0)%, r1
        ldinluf *ar0--(ir1)%, r2
        ldiluf  *ar1, r3
        ldizuf  *ar2++(ir0)b, r4
        ldieq   0, r5
        ldine   -32768, r6
        ldihs   -1, r0
;;; Store
        stf     r2, @1234H
        stf     r3, *+ar4(5)
        stf     r4, *-ar5(6)
        stf     r5, *++ar6(7)
        stf     r6, *--ar7(8)
        stf     r7, *ar0++(9)
        stf     r0, *ar1--(10)
        stf     r1, *ar2++(11)%
        stf     r2, *ar3--(12)%
        stf     r3, *+ar4(ir1)
        stf     r4, *-ar5(ir0)
        stf     r5, *++ar6(ir1)
        stf     r6, *--ar7(ir0)
        stf     r7, *ar0++(ir1)
        stf     r0, *ar1--(ir0)
        stf     r1, *ar2++(ir1)%
        stf     r2, *ar3--(ir0)%
        stf     r3, *ar4
        stf     r4, *ar5++(ir0)b
        sti     r2, @1234H
        sti     ar3, *+ar4(5)
        sti     r4, *-ar5(6)
        sti     r5, *++ar6(7)
        sti     r6, *--ar7(8)
        sti     r7, *ar0++(9)
        sti     r0, *ar1--(10)
        sti     r1, *ar2++(11)%
        sti     r2, *ar3--(12)%
        sti     r3, *+ar4(ir1)
        sti     r4, *-ar5(ir0)
        sti     r5, *++ar6(ir1)
        sti     r6, *--ar7(ir0)
        sti     r7, *ar0++(ir1)
        sti     r0, *ar1--(ir0)
        sti     r1, *ar2++(ir1)%
        sti     r2, *ar3--(ir0)%
        sti     r3, *ar4
        sti     r4, *ar5++(ir0)b
        push    r7
        push    ar0
        push    dp
        push    ir0
        push    rc
        pushf   r0
;;; 2op
        absf    r0, r1
        absf    1.0, r2
        absf    -5.0024, r3
        absi    r0, r1
        absi    ar2, r3
        absi    -1, r4
        addc    r0, r1
        addc    ar1, r2
        addc    -1, r3
        addf    r0, r1
        addf    *ar2, r3
        addf    -1, r4
        addi    r0, r1
        addi    ar2, ar3
        addi    -1, r4
        and     r5, r6
        and     ar7, r0
        and     ffffH, r1
        andn    r2, r3
        andn    r4, ar5
        andn    ffffH, ir1
        ash     r0, ir1
        ash     *ar2++, r3
        ash     -64, r4
        cmpf    r6, r7
        cmpf    *+ar0, r0
        cmpf    -1, r1
        cmpi    r2, ar3
        cmpi    *ar4, ir1
        cmpi    -1, ar5
        fix     r0, r1
        fix     *--ar2, r3
        fix     -0.50024, r4
        float   r5, r6
        float   *ar7++, r0
        float   -1, r1
        lsh     ir1, ar2
        lsh     *ar3, r4
        lsh     15, r5
        mpyf    r6, r7
        mpyf    *+ar0(255), r1
        mpyf    2, r2
        mpyi    ar3, ar4
        mpyi    *-ar5(255), r6
        mpyi    -1, r7
        negb    r0, r1
        negb    *ar2++(128), r3
        negb    32767, r4
        negf    r5, r6
        negf    *ar7--(128), r0
        negf    -1.5, r1
        negi    r0, r1
        negi    *ar2++(128), r3
        negi    32767, r4
        norm    r5, r6
        norm    *ar7--(128), r0
        norm    -1.5, r1
        not     r2, r3
        not     *++ar4(128), ar5
        not     ffffH, r6
        or      r2, r3
        or      *++ar4(128), ar5
        or      ffffH, r6
        rnd     r5, r6
        rnd     *ar7--(128), r0
        rnd     -1.5, r1
        rol     ar7
        rolc    r0
        ror     ir1
        rorc    r2
        subb    r3, ar4
        subb    *ar5++(ir0)b, r6
        subb    -1, r7
        subc    ar0, r1
        subc    *+ar2(3), r4
        subc    ffffH ,r5
        subf    r6, r7
        subf    *+ar0(0), r1
        subf    -1.5, r2
        subi    r3, ar4
        subi    *++ar5(1), r6
        subi    -1, r7
        subrb   r3, ar4
        subrb   *ar5++(ir0)b, r6
        subrb   -1, r7
        subrf   r6, r7
        subrf   *+ar0(0), r1
        subrf   -1.5, r2
        subri   r3, ar4
        subri   *++ar5(1), r6
        subri   -1, r7
        tstb    r0, r1
        tstb    *ar2--(3), r3
        tstb    ffffH, r4
        xor     r2, r3
        xor     *++ar4(128), ar5
        xor     ffffH, r6
;;; unary
        absf    r1
        absi    r2
        fix     r3
        float   r4
        negb    r5
        negf    r6
        negi    r7
        norm    r0
        not     r1
        rnd     r2
;;; 3op
        addc3   r0, ar1, r2
        addc3   *+ar1, r2, ar3
        addc3   *-ar2(0), *ar3++(ir0)b, r4
        addc3   *++ar3, *ar4, r5
        addc3   *--ar4(1), *ar5--(ir1)%, r6
        addc3   *ar3++(1), *ar6++(ir0)%, r7
        addc3   *ar4--(1), *ar7--(ir1), r0
        addc3   *+ar5(ir1), *ar0++(ir0), r1
        addc3   r6, *+ar1(ir1), ar2
        addc3   ir1, *ar2++(ir0), r3
        addc3   *+ar1, *-ar2, r3
        addf3   r0, r1, r2
        addf3   *+ar1, r2, r3
        addf3   *-ar2(0), *ar3++(ir0)b, r4
        addf3   *++ar3, *ar4, r5
        addf3   *--ar4(1), *ar5--(ir1)%, r6
        addf3   *ar3++(1), *ar6++(ir0)%, r7
        addf3   *ar4--(1), *ar7--(ir1), r0
        addf3   *+ar5(ir1), *ar0++(ir0), r1
        addf3   r6, *+ar1(ir1), r2
        addf3   r1, *ar2++(ir0), r3
        addf3   *+ar1, *-ar2, r3
        addi3   r0, ar1, r2
        addi3   *+ar1, r2, ar3
        addi3   *-ar2(0), *ar3++(ir0)b, r4
        addi3   *++ar3, *ar4, r5
        addi3   *--ar4(1), *ar5--(ir1)%, r6
        addi3   *ar3++(1), *ar6++(ir0)%, r7
        addi3   *ar4--(1), *ar7--(ir1), r0
        addi3   *+ar5(ir1), *ar0++(ir0), r1
        addi3   r6, *+ar1(ir1), ar2
        addi3   ir1, *ar2++(ir0), r3
        addi3   *+ar1, *-ar2, r3
        and3    r0, ar1, r2
        and3    *+ar1, r2, ar3
        and3    *-ar2(0), *ar3++(ir0)b, r4
        and3    *++ar3, *ar4, r5
        and3    *--ar4(1), *ar5--(ir1)%, r6
        and3    *ar3++(1), *ar6++(ir0)%, r7
        and3    *ar4--(1), *ar7--(ir1), r0
        and3    *+ar5(ir1), *ar0++(ir0), r1
        and3    r6, *+ar1(ir1), ar2
        and3    ir1, *ar2++(ir0), r3
        and3    *+ar1, *-ar2, r3
        andn3   r0, ar1, r2
        andn3   *+ar1, r2, ar3
        andn3   *-ar2(0), *ar3++(ir0)b, r4
        andn3   *++ar3, *ar4, r5
        andn3   *--ar4(1), *ar5--(ir1)%, r6
        andn3   *ar3++(1), *ar6++(ir0)%, r7
        andn3   *ar4--(1), *ar7--(ir1), r0
        andn3   *+ar5(ir1), *ar0++(ir0), r1
        andn3   r6, *+ar1(ir1), ar2
        andn3   ir1, *ar2++(ir0), r3
        andn3   *+ar1, *-ar2, r3
        ash3    r0, ar1, r2
        ash3    *+ar1, r2, ar3
        ash3    *-ar2(0), *ar3++(ir0)b, r4
        ash3    *++ar3, *ar4, r5
        ash3    *--ar4(1), *ar5--(ir1)%, r6
        ash3    *ar3++(1), *ar6++(ir0)%, r7
        ash3    *ar4--(1), *ar7--(ir1), r0
        ash3    *+ar5(ir1), *ar0++(ir0), r1
        ash3    r6, *+ar1(ir1), ar2
        ash3    ir1, *ar2++(ir0), r3
        ash3    *+ar1, *-ar2, r3
        cmpf3   r0, r1
        cmpf3   *+ar1, r2
        cmpf3   *-ar2(0), *ar3++(ir0)b
        cmpf3   *++ar3, *ar4
        cmpf3   *--ar4(1), *ar5--(ir1)%
        cmpf3   *ar3++(1), *ar6++(ir0)%
        cmpf3   *ar4--(1), *ar7--(ir1)
        cmpf3   *+ar5(ir1), *ar0++(ir0)
        cmpf3   r6, *+ar1(ir1)
        cmpf3   r1, *ar2++(ir0)
        cmpf3   *+ar1, *-ar2
        cmpi3   r0, ar1
        cmpi3   *+ar1, r2
        cmpi3   *-ar2(0), *ar3++(ir0)b
        cmpi3   *++ar3, *ar4
        cmpi3   *--ar4(1), *ar5--(ir1)%
        cmpi3   *ar3++(1), *ar6++(ir0)%
        cmpi3   *ar4--(1), *ar7--(ir1)
        cmpi3   *+ar5(ir1), *ar0++(ir0)
        cmpi3   r6, *+ar1(ir1)
        cmpi3   ir1, *ar2++(ir0)
        cmpi3   *+ar1, *-ar2
        lsh3    r0, ar1, r2
        lsh3    *+ar1, r2, ar3
        lsh3    *-ar2(0), *ar3++(ir0)b, r4
        lsh3    *++ar3, *ar4, r5
        lsh3    *--ar4(1), *ar5--(ir1)%, r6
        lsh3    *ar3++(1), *ar6++(ir0)%, r7
        lsh3    *ar4--(1), *ar7--(ir1), r0
        lsh3    *+ar5(ir1), *ar0++(ir0), r1
        lsh3    r6, *+ar1(ir1), ar2
        lsh3    ir1, *ar2++(ir0), r3
        lsh3    *+ar1, *-ar2, r3
        mpyf3   r0, r1, r2
        mpyf3   *+ar1, r2, r3
        mpyf3   *-ar2(0), *ar3++(ir0)b, r4
        mpyf3   *++ar3, *ar4, r5
        mpyf3   *--ar4(1), *ar5--(ir1)%, r6
        mpyf3   *ar3++(1), *ar6++(ir0)%, r7
        mpyf3   *ar4--(1), *ar7--(ir1), r0
        mpyf3   *+ar5(ir1), *ar0++(ir0), r1
        mpyf3   r6, *+ar1(ir1), r2
        mpyf3   r1, *ar2++(ir0), r3
        mpyf3   *+ar1, *-ar2, r3
        mpyi3   r0, ar1, r2
        mpyi3   *+ar1, r2, ar3
        mpyi3   *-ar2(0), *ar3++(ir0)b, r4
        mpyi3   *++ar3, *ar4, r5
        mpyi3   *--ar4(1), *ar5--(ir1)%, r6
        mpyi3   *ar3++(1), *ar6++(ir0)%, r7
        mpyi3   *ar4--(1), *ar7--(ir1), r0
        mpyi3   *+ar5(ir1), *ar0++(ir0), r1
        mpyi3   r6, *+ar1(ir1), ar2
        mpyi3   ir1, *ar2++(ir0), r3
        mpyi3   *+ar1, *-ar2, r3
        or3     r0, ar1, r2
        or3     *+ar1, r2, ar3
        or3     *-ar2(0), *ar3++(ir0)b, r4
        or3     *++ar3, *ar4, r5
        or3     *--ar4(1), *ar5--(ir1)%, r6
        or3     *ar3++(1), *ar6++(ir0)%, r7
        or3     *ar4--(1), *ar7--(ir1), r0
        or3     *+ar5(ir1), *ar0++(ir0), r1
        or3     r6, *+ar1(ir1), ar2
        or3     ir1, *ar2++(ir0), r3
        or3     *+ar1, *-ar2, r3
        subb3   r0, ar1, r2
        subb3   *+ar1, r2, ar3
        subb3   *-ar2(0), *ar3++(ir0)b, r4
        subb3   *++ar3, *ar4, r5
        subb3   *--ar4(1), *ar5--(ir1)%, r6
        subb3   *ar3++(1), *ar6++(ir0)%, r7
        subb3   *ar4--(1), *ar7--(ir1), r0
        subb3   *+ar5(ir1), *ar0++(ir0), r1
        subb3   r6, *+ar1(ir1), ar2
        subb3   ir1, *ar2++(ir0), r3
        subb3   *+ar1, *-ar2, r3
        subf3   r0, r1, r2
        subf3   *+ar1, r2, r3
        subf3   *-ar2(0), *ar3++(ir0)b, r4
        subf3   *++ar3, *ar4, r5
        subf3   *--ar4(1), *ar5--(ir1)%, r6
        subf3   *ar3++(1), *ar6++(ir0)%, r7
        subf3   *ar4--(1), *ar7--(ir1), r0
        subf3   *+ar5(ir1), *ar0++(ir0), r1
        subf3   r6, *+ar1(ir1), r2
        subf3   r1, *ar2++(ir0), r3
        subf3   *+ar1, *-ar2, r3
        subi3   r0, ar1, r2
        subi3   *+ar1, r2, ar3
        subi3   *-ar2(0), *ar3++(ir0)b, r4
        subi3   *++ar3, *ar4, r5
        subi3   *--ar4(1), *ar5--(ir1)%, r6
        subi3   *ar3++(1), *ar6++(ir0)%, r7
        subi3   *ar4--(1), *ar7--(ir1), r0
        subi3   *+ar5(ir1), *ar0++(ir0), r1
        subi3   r6, *+ar1(ir1), ar2
        subi3   ir1, *ar2++(ir0), r3
        subi3   *+ar1, *-ar2, r3
        tstb3   r0, ar1
        tstb3   *+ar1, r2
        tstb3   *-ar2(0), *ar3++(ir0)b
        tstb3   *++ar3, *ar4
        tstb3   *--ar4(1), *ar5--(ir1)%
        tstb3   *ar3++(1), *ar6++(ir0)%
        tstb3   *ar4--(1), *ar7--(ir1)
        tstb3   *+ar5(ir1), *ar0++(ir0)
        tstb3   r6, *+ar1(ir1)
        tstb3   ir1, *ar2++(ir0)
        tstb3   *+ar1, *-ar2
        xor3    r0, ar1, r2
        xor3    *+ar1, r2, ar3
        xor3    *-ar2(0), *ar3++(ir0)b, r4
        xor3    *++ar3, *ar4, r5
        xor3    *--ar4(1), *ar5--(ir1)%, r6
        xor3    *ar3++(1), *ar6++(ir0)%, r7
        xor3    *ar4--(1), *ar7--(ir1), r0
        xor3    *+ar5(ir1), *ar0++(ir0), r1
        xor3    r6, *+ar1(ir1), ar2
        xor3    ir1, *ar2++(ir0), r3
        xor3    *+ar1, *-ar2, r3
;;; Program
        bu      r0
        blo     r1
        bls     r2
        bhi     r3
        bhs     r4
        beq     r5
        bne     r6
        blt     r7
        ble     ar0
        bgt     ar1
        bge     ar2
        b       ar3
        bc      ar4
        bnc     ar5
        bz      ar6
        bnz     ar7
        bnv     r0
        bv      r1
        bnuf    r2
        buf     r3
        bnlv    r4
        blv     r5
        bnluf   r6
        bluf    r7
        bzuf    ar0
        bu      $+1000H
        blo     $+1000H
        bls     $+1000H
        bhi     $+1000H
        bhs     $+1000H
        beq     $+1000H
        bne     $+1000H
        blt     $+1000H
        ble     $+1000H
        bgt     $+1000H
        bge     $+1000H
        b       $+1000H
        bc      $+1000H
        bnc     $+1000H
        bz      $+1000H
        bnz     $+1000H
        bnv     $+1000H
        bv      $+1000H
        bnuf    $+1000H
        buf     $+1000H
        bnlv    $+1000H
        blv     $+1000H
        bnluf   $+1000H
        bluf    $+1000H
        bzuf    $+1000H
        bud     r0
        blod    r1
        blsd    r2
        bhid    r3
        bhsd    r4
        beqd    r5
        bned    r6
        bltd    r7
        bled    ar0
        bgtd    ar1
        bged    ar2
        b       ar3
        bcd     ar4
        bncd    ar5
        bzd     ar6
        bnzd    ar7
        bnvd    r0
        bvd     r1
        bnufd   r2
        bufd    r3
        bnlvd   r4
        blvd    r5
        bnlufd  r6
        blufd   r7
        bzufd   ar0
        bud     $+1000H
        blod    $+1000H
        blsd    $+1000H
        bhid    $+1000H
        bhsd    $+1000H
        beqd    $+1000H
        bned    $+1000H
        bltd    $+1000H
        bled    $+1000H
        bgtd    $+1000H
        bged    $+1000H
        b       $+1000H
        bcd     $+1000H
        bncd    $+1000H
        bzd     $+1000H
        bnzd    $+1000H
        bnvd    $+1000H
        bvd     $+1000H
        bnufd   $+1000H
        bufd    $+1000H
        bnlvd   $+1000H
        blvd    $+1000H
        bnlufd  $+1000H
        blufd   $+1000H
        bzufd   $+1000H
        br      123456H
        brd     123456H
        call    123456H
        callu   r0
        calllo  r1
        callls  r2
        callhi  r3
        callhs  r4
        calleq  r5
        callne  r6
        calllt  r7
        callle  ar0
        callgt  ar1
        callge  ar2
        callc   ar4
        callnc  ar5
        callz   ar6
        callnz  ar7
        callnv  r0
        callv   r1
        callnuf r2
        calluf  r3
        callnlv r4
        calllv  r5
        callnluf r6
        callluf r7
        callzuf ar0
        callu   $+1000H
        calllo  $+1000H
        callls  $+1000H
        callhi  $+1000H
        callhs  $+1000H
        calleq  $+1000H
        callne  $+1000H
        calllt  $+1000H
        callle  $+1000H
        callgt  $+1000H
        callge  $+1000H
        callc   $+1000H
        callnc  $+1000H
        callz   $+1000H
        callnz  $+1000H
        callnv  $+1000H
        callv   $+1000H
        callnuf $+1000H
        calluf  $+1000H
        callnlv $+1000H
        calllv  $+1000H
        callnluf $+1000H
        callluf $+1000H
        callzuf $+1000H
        dbu     ar7, r0
        dblo    ar6, r1
        dbls    ar5, r2
        dbhi    ar4, r3
        dbhs    ar3, r4
        dbeq    ar2, r5
        dbne    ar1, r6
        dblt    ar0, r7
        dble    ar7, ar0
        dbgt    ar6, ar1
        dbge    ar5, ar2
        db      ar4, ar3
        dbc     ar3, ar4
        dbnc    ar2, ar5
        dbz     ar1, ar6
        dbnz    ar0, ar7
        dbnv    ar7, r0
        dbv     ar6, r1
        dbnuf   ar5, r2
        dbuf    ar4, r3
        dbnlv   ar3, r4
        dblv    ar2, r5
        dbnluf  ar1, r6
        dbluf   ar0, r7
        dbzuf   ar7, ar0
        dbu     ar6, $+1000H
        dblo    ar5, $+1000H
        dbls    ar4, $+1000H
        dbhi    ar3, $+1000H
        dbhs    ar2, $+1000H
        dbeq    ar1, $+1000H
        dbne    ar0, $+1000H
        dblt    ar7, $+1000H
        dble    ar6, $+1000H
        dbgt    ar5, $+1000H
        dbge    ar4, $+1000H
        db      ar3, $+1000H
        dbc     ar2, $+1000H
        dbnc    ar1, $+1000H
        dbz     ar0, $+1000H
        dbnz    ar7, $+1000H
        dbnv    ar6, $+1000H
        dbv     ar5, $+1000H
        dbnuf   ar4, $+1000H
        dbuf    ar3, $+1000H
        dbnlv   ar2, $+1000H
        dblv    ar1, $+1000H
        dbnluf  ar0, $+1000H
        dbluf   ar7, $+1000H
        dbzuf   ar6, $+1000H
        dbud    ar7, r0
        dblod   ar6, r1
        dblsd   ar5, r2
        dbhid   ar4, r3
        dbhsd   ar3, r4
        dbeqd   ar2, r5
        dbned   ar1, r6
        dbltd   ar0, r7
        dbled   ar7, ar0
        dbgtd   ar6, ar1
        dbged   ar5, ar2
        db      ar4, ar3
        dbcd    ar3, ar4
        dbncd   ar2, ar5
        dbzd    ar1, ar6
        dbnzd   ar0, ar7
        dbnvd   ar7, r0
        dbvd    ar6, r1
        dbnufd  ar5, r2
        dbufd   ar4, r3
        dbnlvd  ar3, r4
        dblvd   ar2, r5
        dbnlufd ar1, r6
        dblufd  ar0, r7
        dbzufd  ar7, ar0
        dbud    ar6, $+1000H
        dblod   ar5, $+1000H
        dblsd   ar4, $+1000H
        dbhid   ar3, $+1000H
        dbhsd   ar2, $+1000H
        dbeqd   ar1, $+1000H
        dbned   ar0, $+1000H
        dbltd   ar7, $+1000H
        dbled   ar6, $+1000H
        dbgtd   ar5, $+1000H
        dbged   ar4, $+1000H
        db      ar3, $+1000H
        dbcd    ar2, $+1000H
        dbncd   ar1, $+1000H
        dbzd    ar0, $+1000H
        dbnzd   ar7, $+1000H
        dbnvd   ar6, $+1000H
        dbvd    ar5, $+1000H
        dbnufd  ar4, $+1000H
        dbufd   ar3, $+1000H
        dbnlvd  ar2, $+1000H
        dblvd   ar1, $+1000H
        dbnlufd ar0, $+1000H
        dblufd  ar7, $+1000H
        dbzufd  ar6, $+1000H
        iack    @1234H
        iack    *+ar1(2)
        iack    *-ar2(3)
        iack    *++ar3(4)
        iack    *--ar4(5)
        iack    *ar5++(6)
        iack    *ar6--(7)
        iack    *ar7++(8)%
        iack    *ar0--(9)%
        iack    *+ar1(ir0)
        iack    *-ar2(ir1)
        iack    *++ar3(ir0)
        iack    *--ar4(ir1)
        iack    *ar5++(ir0)
        iack    *ar6--(ir1)
        iack    *ar7++(ir0)%
        iack    *ar0--(ir1)%
        iack    *ar1
        iack    *ar2++(ir0)b
        idle
        nop     r0
        nop     ar0
        nop     *+ar1(2)
        nop     *-ar2(3)
        nop     *++ar3(4)
        nop     *--ar4(5)
        nop     *ar5++(6)
        nop     *ar6--(7)
        nop     *ar7++(8)%
        nop     *ar0--(9)%
        nop     *+ar1(ir0)
        nop     *-ar2(ir1)
        nop     *++ar3(ir0)
        nop     *--ar4(ir1)
        nop     *ar5++(ir0)
        nop     *ar6--(ir1)
        nop     *ar7++(ir0)%
        nop     *ar0--(ir1)%
        nop     *ar1
        nop     *ar2++(ir0)b
        retiu
        retilo
        retils
        retihi
        retihs
        retieq
        retine
        retilt
        retile
        retigt
        retige
        reti
        retic
        retinc
        retiz
        retinz
        retinv
        retiv
        retinuf
        retiuf
        retinlv
        retilv
        retinluf
        retiluf
        retizuf
        retsu
        retslo
        retsls
        retshi
        retshs
        retseq
        retsne
        retslt
        retsle
        retsgt
        retsge
        rets
        retsc
        retsnc
        retsz
        retsnz
        retsnv
        retsv
        retsnuf
        retsuf
        retsnlv
        retslv
        retsnluf
        retsluf
        retszuf
        rptb    123456H
        rpts    r0
        rpts    ar0
        rpts    @1234H
        rpts    *+ar1(2)
        rpts    *-ar2(3)
        rpts    *++ar3(4)
        rpts    *--ar4(5)
        rpts    *ar5++(6)
        rpts    *ar6--(7)
        rpts    *ar7++(8)%
        rpts    *ar0--(9)%
        rpts    *+ar1(ir0)
        rpts    *-ar2(ir1)
        rpts    *++ar3(ir0)
        rpts    *--ar4(ir1)
        rpts    *ar5++(ir0)
        rpts    *ar6--(ir1)
        rpts    *ar7++(ir0)%
        rpts    *ar0--(ir1)%
        rpts    *ar1
        rpts    *ar2++(ir0)b
        rpts    0
        rpts    ffffH
        swi
        trapu   0
        traplo  1
        trapls  2
        traphi  3
        traphs  4
        trapeq  5
        trapne  6
        traplt  7
        traple  8
        trapgt  9
        trapge  10
        trap    11
        trapc   13
        trapnc  14
        trapz   15
        trapnz  16
        trapnv  17
        trapv   18
        trapnuf 19
        trapuf  10
        trapnlv 20
        traplv  21
        trapnluf 22
        trapluf 23
        trapzuf 31
;;; Interlock
        sigi
        ldfi    @1234H, r2
        ldfi    *+ar1(2), r3
        ldfi    *-ar2(3), r4
        ldfi    *++ar3(4), r5
        ldfi    *--ar4(5), r6
        ldfi    *ar5++(6), r7
        ldfi    *ar6--(7), r0
        ldfi    *ar7++(8)%, r1
        ldfi    *ar0--(9)%, r2
        ldfi    *+ar1(ir0), r3
        ldfi    *-ar2(ir1), r4
        ldfi    *++ar3(ir0), r5
        ldfi    *--ar4(ir1), r6
        ldfi    *ar5++(ir0), r7
        ldfi    *ar6--(ir1), r0
        ldfi    *ar7++(ir0)%, r1
        ldfi    *ar0--(ir1)%, r2
        ldfi    *ar1, r3
        ldfi    *ar2++(ir0)b, r4
        ldii    @1234H, ar2
        ldii    *+ar1(2), r3
        ldii    *-ar2(3), r4
        ldii    *++ar3(4), r5
        ldii    *--ar4(5), r6
        ldii    *ar5++(6), r7
        ldii    *ar6--(7), r0
        ldii    *ar7++(8)%, r1
        ldii    *ar0--(9)%, r2
        ldii    *+ar1(ir0), r3
        ldii    *-ar2(ir1), r4
        ldii    *++ar3(ir0), r5
        ldii    *--ar4(ir1), r6
        ldii    *ar5++(ir0), r7
        ldii    *ar6--(ir1), r0
        ldii    *ar7++(ir0)%, r1
        ldii    *ar0--(ir1)%, r2
        ldii    *ar1, r3
        ldii    *ar2++(ir0)b, r4
        stfi    r2, @1234H
        stfi    r3, *+ar4(5)
        stfi    r4, *-ar5(6)
        stfi    r5, *++ar6(7)
        stfi    r6, *--ar7(8)
        stfi    r7, *ar0++(9)
        stfi    r0, *ar1--(10)
        stfi    r1, *ar2++(11)%
        stfi    r2, *ar3--(12)%
        stfi    r3, *+ar4(ir1)
        stfi    r4, *-ar5(ir0)
        stfi    r5, *++ar6(ir1)
        stfi    r6, *--ar7(ir0)
        stfi    r7, *ar0++(ir1)
        stfi    r0, *ar1--(ir0)
        stfi    r1, *ar2++(ir1)%
        stfi    r2, *ar3--(ir0)%
        stfi    r3, *ar4
        stfi    r4, *ar5++(ir0)b
        stii    r2, @1234H
        stii    ar3, *+ar4(5)
        stii    r4, *-ar5(6)
        stii    r5, *++ar6(7)
        stii    r6, *--ar7(8)
        stii    r7, *ar0++(9)
        stii    r0, *ar1--(10)
        stii    r1, *ar2++(11)%
        stii    r2, *ar3--(12)%
        stii    r3, *+ar4(ir1)
        stii    r4, *-ar5(ir0)
        stii    r5, *++ar6(ir1)
        stii    r6, *--ar7(ir0)
        stii    r7, *ar0++(ir1)
        stii    r0, *ar1--(ir0)
        stii    r1, *ar2++(ir1)%
        stii    r2, *ar3--(ir0)%
        stii    r3, *ar4
        stii    r4, *ar5++(ir0)b
;;; parallel
        absf  *+ar0, r1
||      stf   r2, *-ar3
        stf   r2, *-ar3
||      absf  *+ar0, r1
        absi  *++ar4, r5
||      sti   r6, *--ar7
        sti   r6, *--ar7
||      absi  *++ar4, r5
        addf3 *ar0++, r1, r2
||      stf   r3, *ar4--
        addf3 r1, *ar0++, r2
||      stf   r3, *ar4--
        stf   r3, *ar4--
||      addf3 *ar0++, r1, r2
        stf   r3, *ar4--
||      addf3 r1, *ar0++, r2
        addi3 *ar5++%, r6, r7
||      sti   r0, *ar1--%
        addi3 r6, *ar5++%, r7
||      sti   r0, *ar1--%
        sti   r0, *ar1--%
||      addi3 *ar5++%, r6, r7
        sti   r0, *ar1--%
||      addi3 r6, *ar5++%, r7
        and3  *+ar2(ir0), r3, r4
||      sti   r5, *-ar6(ir0)
        and3  r3, *+ar2(ir0), r4
||      sti   r5, *-ar6(ir0)
        sti   r5, *-ar6(ir0)
||      and3  *+ar2(ir0), r3, r4
        sti   r5, *-ar6(ir0)
||      and3  r3, *+ar2(ir0), r4
        ash3  r6, *++ar7(ir0), r0
||      sti   r1, *--ar2(ir0)
        sti   r1, *--ar2(ir0)
||      ash3  r6, *++ar7(ir0), r0
        fix   *ar3++(ir0), r4
||      sti   r5, *ar6--(ir0)
        sti   r5, *ar6--(ir0)
||      fix   *ar3++(ir0), r4
        float *ar7++(ir0)%, r0
||      stf   r1, *ar2--(ir0)%
        stf   r1, *ar2--(ir0)%
||      float *ar7++(ir0)%, r0
        ldf   *+ar3(ir1), r4
||      ldf   *-ar5(ir1), r6
        ldf   *-ar5(ir1), r6
||      ldf   *+ar3(ir1), r4
        ldf   *++ar7(ir1), r0
||      stf   r1, *--ar2(ir1)
        stf   r1, *--ar2(ir1)
||      ldf   *++ar7(ir1), r0
        ldi   *ar3++(ir1), r4
||      ldi   *ar5--(ir1), r6
        ldi   *ar5--(ir1), r6
||      ldi   *ar3++(ir1), r4
        ldi   *ar4++(ir1)%, r5
||      sti   r6, *ar7--(ir1)%
        sti   r6, *ar7--(ir1)%
||      ldi   *ar4++(ir1)%, r5
        lsh3  r0, *ar1++(ir0)b, r2
||      sti   r3, *ar4
        sti   r3, *ar4
||      lsh3  r0, *ar1++(ir0)b, r2
        mpyf3 *+ar5, r6, r7
||      stf   r0, *-ar1
        mpyf3 r6, *+ar5, r7
||      stf   r0, *-ar1
        stf   r0, *-ar1
||      mpyf3 *+ar5, r6, r7
        stf   r0, *-ar1
||      mpyf3 r6, *+ar5, r7
        mpyi3 *++ar2, r3, r4
||      sti   r5, *--ar6
        mpyi3 r3, *++ar2, r4
||      sti   r5, *--ar6
        sti   r5, *--ar6
||      mpyi3 *++ar2, r3, r4
        sti   r5, *--ar6
||      mpyi3 r3, *++ar2, r4
        negf  *ar7++, r0
||      stf   r1, *ar2--
        stf   r1, *ar2--
||      negf  *ar7++, r0
        negi  *ar3++%, r4
||      sti   r5, *ar6--%
        sti   r5, *ar6--%
||      negi  *ar3++%, r4
        not   *+ar7(ir0), r0
||      sti   r1, *-ar2(ir0)
        sti   r1, *-ar2(ir0)
||      not   *+ar7(ir0), r0
        or3   *++ar3(ir0), r4, r5
||      sti   r6, *--ar7(ir0)
        or3   r4, *++ar3(ir0), r5
||      sti   r6, *--ar7(ir0)
        sti   r6, *--ar7(ir0)
||      or3   *++ar3(ir0), r4, r5
        sti   r6, *--ar7(ir0)
||      or3   r4, *++ar3(ir0), r5
        stf   r0, *ar1++(ir0)
||      stf   r2, *ar3--(ir0)
        stf   r2, *ar3--(ir0)
||      stf   r0, *ar1++(ir0)
        sti   r4, *ar5++(ir0)%
||      sti   r6, *ar7--(ir0)%
        sti   r6, *ar7--(ir0)%
||      sti   r4, *ar5++(ir0)%
        subf3 r0, *+ar1(ir1), r2
||      stf   r3, *-ar4(ir1)
        stf   r3, *-ar4(ir1)
||      subf3 r0, *+ar1(ir1), r2
        subi3 r5, *++ar6(ir1), r7
||      sti   r0, *--ar1(ir1)
        sti   r0, *--ar1(ir1)
||      subi3 r5, *++ar6(ir1), r7
        xor3  *ar2++(ir1), r3, r4
||      sti   r5, *ar6--(ir1)
        xor3  r3, *ar2++(ir1), r4
||      sti   r5, *ar6--(ir1)
        sti   r5, *ar6--(ir1)
||      xor3  *ar2++(ir1), r3, r4
        sti   r5, *ar6--(ir1)
||      xor3  r3, *ar2++(ir1), r4
        mpyf3 *ar4, *ar3, r1
||      addf3 r5, r2, r2        ; p=0
        addf3 r5, r2, r2
||      mpyf3 *ar4, *ar3, r1
        mpyf3 *ar3, r1, r1
||      addf3 r2, *ar4, r2      ; p=1
        addf3 r2, *ar4, r2
||      mpyf3 *ar3, r1, r1
        mpyf3 r2, r1, r1
||      addf3 *ar3, *ar4, r2    ; p=2
        addf3 *ar3, *ar4, r2
||      mpyf3 r2, r1, r1
        mpyf3 *ar3, r1, r1
||      addf3 *ar4, r2, r2      ; p=3
        addf3 *ar4, r2, r2
||      mpyf3 *ar3, r1, r1

        mpyf3 *ar4, *ar3, r1
||      subf3 r5, r2, r2        ; p=0
        subf3 r5, r2, r2
||      mpyf3 *ar4, *ar3, r1
        mpyf3 *ar3, r1, r1
||      subf3 r2, *ar4, r2      ; p=1
        subf3 r2, *ar4, r2
||      mpyf3 *ar3, r1, r1
        mpyf3 r2, r1, r1
||      subf3 *ar3, *ar4, r2    ; p=2
        subf3 *ar3, *ar4, r2
||      mpyf3 r2, r1, r1
        mpyf3 *ar3, r1, r1
||      subf3 *ar4, r2, r2      ; p=3
        subf3 *ar4, r2, r2
||      mpyf3 *ar3, r1, r1

        mpyi3 *ar4, *ar3, r1
||      addi3 r5, r2, r2        ; p=0
        addi3 r5, r2, r2
||      mpyi3 *ar4, *ar3, r1
        mpyi3 *ar3, r1, r1
||      addi3 r2, *ar4, r2      ; p=1
        addi3 r2, *ar4, r2
||      mpyi3 *ar3, r1, r1
        mpyi3 r2, r1, r1
||      addi3 *ar3, *ar4, r2    ; p=2
        addi3 *ar3, *ar4, r2
||      mpyi3 r2, r1, r1
        mpyi3 *ar3, r1, r1
||      addi3 *ar4, r2, r2      ; p=3
        addi3 *ar4, r2, r2
||      mpyi3 *ar3, r1, r1

        mpyi3 *ar4, *ar3, r1
||      subi3 r5, r2, r2        ; p=0
        subi3 r5, r2, r2
||      mpyi3 *ar4, *ar3, r1
        mpyi3 *ar3, r1, r1
||      subi3 r2, *ar4, r2      ; p=1
        subi3 r2, *ar4, r2
||      mpyi3 *ar3, r1, r1
        mpyi3 r2, r1, r1
||      subi3 *ar3, *ar4, r2    ; p=2
        subi3 *ar3, *ar4, r2
||      mpyi3 r2, r1, r1
        mpyi3 *ar3, r1, r1
||      subi3 *ar4, r2, r2      ; p=3
        subi3 *ar4, r2, r2
||      mpyi3 *ar3, r1, r1
;;; parallel no3
        addf *ar0++, r1, r2
||      stf  r3, *ar4--
        stf  r3, *ar4--
||      addf *ar0++, r1, r2
        addf r1, *ar0++, r2
||      stf  r3, *ar4--
        stf  r3, *ar4--
||      addf r1, *ar0++, r2
        addi *ar5++%, r6, r7
||      sti  r0, *ar1--%
        sti  r0, *ar1--%
||      addi *ar5++%, r6, r7
        addi r6, *ar5++%, r7
||      sti  r0, *ar1--%
        sti  r0, *ar1--%
||      addi r6, *ar5++%, r7
        and  *+ar2(ir0), r3, r4
||      sti  r5, *-ar6(ir0)
        sti  r5, *-ar6(ir0)
||      and  *+ar2(ir0), r3, r4
        and  r3, *+ar2(ir0), r4
||      sti  r5, *-ar6(ir0)
        sti  r5, *-ar6(ir0)
||      and  r3, *+ar2(ir0), r4
        ash  r6, *++ar7(ir0), r0
||      sti  r1, *--ar2(ir0)
        sti  r1, *--ar2(ir0)
||      ash  r6, *++ar7(ir0), r0
        lsh  r0, *ar1++(ir0)b, r2
||      sti  r3, *ar4
        sti  r3, *ar4
||      lsh  r0, *ar1++(ir0)b, r2
        mpyf *+ar5, r6, r7
||      stf  r0, *-ar1
        stf  r0, *-ar1
||      mpyf *+ar5, r6, r7
        mpyf r6, *+ar5, r7
||      stf  r0, *-ar1
        stf  r0, *-ar1
||      mpyf r6, *+ar5, r7
        mpyi *++ar2, r3, r4
||      sti  r5, *--ar6
        sti  r5, *--ar6
||      mpyi *++ar2, r3, r4
        mpyi r3, *++ar2, r4
||      sti  r5, *--ar6
        sti  r5, *--ar6
||      mpyi r3, *++ar2, r4
        or   *++ar3(ir0), r4, r5
||      sti  r6, *--ar7(ir0)
        sti  r6, *--ar7(ir0)
||      or   *++ar3(ir0), r4, r5
        or   r4, *++ar3(ir0), r5
||      sti  r6, *--ar7(ir0)
        sti  r6, *--ar7(ir0)
||      or   r4, *++ar3(ir0), r5
        subf r0, *+ar1(ir1), r2
||      stf  r3, *-ar4(ir1)
        stf  r3, *-ar4(ir1)
||      subf r0, *+ar1(ir1), r2
        subi r5, *++ar6(ir1), r7
||      sti  r0, *--ar1(ir1)
        sti  r0, *--ar1(ir1)
||      subi r5, *++ar6(ir1), r7
        xor  *ar2++(ir1), r3, r4
||      sti  r5, *ar6--(ir1)
        sti  r5, *ar6--(ir1)
||      xor  *ar2++(ir1), r3, r4
        xor  r3, *ar2++(ir1), r4
||      sti  r5, *ar6--(ir1)
        sti  r5, *ar6--(ir1)
||      xor  r3, *ar2++(ir1), r4
        mpyf *ar4, *ar3, r1
||      addf r5, r2, r2         ; p=0
        addf r5, r2, r2
||      mpyf *ar4, *ar3, r1
        mpyf *ar3, r1, r1
||      addf r2, *ar4, r2       ; p=1
        addf r2, *ar4, r2
||      mpyf *ar3, r1, r1
        mpyf r2, r1, r1
||      addf *ar3, *ar4, r2     ; p=2
        addf *ar3, *ar4, r2
||      mpyf r2, r1, r1
        mpyf *ar3, r1, r1
||      addf *ar4, r2, r2       ; p=3
        addf *ar4, r2, r2
||      mpyf *ar3, r1, r1
        mpyf *ar4, *ar3, r1
||      subf r5, r2, r2         ; p=0
        subf r5, r2, r2
||      mpyf *ar4, *ar3, r1
        mpyf *ar3, r1, r1
||      subf r2, *ar4, r2       ; p=1
        subf r2, *ar4, r2
||      mpyf *ar3, r1, r1
        mpyf r2, r1, r1
||      subf *ar3, *ar4, r2     ; p=2
        subf *ar3, *ar4, r2
||      mpyf r2, r1, r1
        mpyf *ar3, r1, r1
||      subf *ar4, r2, r2       ; p=3
        subf *ar4, r2, r2
||      mpyf *ar3, r1, r1
        mpyi *ar4, *ar3, r1
||      addi r5, r2, r2         ; p=0
        addi r5, r2, r2
||      mpyi *ar4, *ar3, r1
        mpyi *ar3, r1, r1
||      addi r2, *ar4, r2       ; p=1
        addi r2, *ar4, r2
||      mpyi *ar3, r1, r1
        mpyi r2, r1, r1
||      addi *ar3, *ar4, r2     ; p=2
        addi *ar3, *ar4, r2
||      mpyi r2, r1, r1
        mpyi *ar3, r1, r1
||      addi *ar4, r2, r2       ; p=3
        addi *ar4, r2, r2
||      mpyi *ar3, r1, r1
        mpyi *ar4, *ar3, r1
||      subi r5, r2, r2         ; p=0
        subi r5, r2, r2
||      mpyi *ar4, *ar3, r1
        mpyi *ar3, r1, r1
||      subi r2, *ar4, r2       ; p=1
        subi r2, *ar4, r2
||      mpyi *ar3, r1, r1
        mpyi r2, r1, r1
||      subi *ar3, *ar4, r2     ; p=2
        subi *ar3, *ar4, r2
||      mpyi r2, r1, r1
        mpyi *ar3, r1, r1
||      subi *ar4, r2, r2       ; p=3
        subi *ar4, r2, r2
||      mpyi *ar3, r1, r1
;;; parallel unary
        addf *ar0++, r1, r1
||      stf  r3, *ar4--
        addf *ar0++, r1
||      stf  r3, *ar4--
        stf  r3, *ar4--
||      addf *ar0++, r1, r1
        stf  r3, *ar4--
||      addf *ar0++, r1
        addi *ar5++%, r6, r6
||      sti  r0, *ar1--%
        addi *ar5++%, r6
||      sti  r0, *ar1--%
        sti  r0, *ar1--%
||      addi *ar5++%, r6, r6
        sti  r0, *ar1--%
||      addi *ar5++%, r6
        and  *+ar2(ir0), r3, r3
||      sti  r5, *-ar6(ir0)
        and  *+ar2(ir0), r3
||      sti  r5, *-ar6(ir0)
        sti  r5, *-ar6(ir0)
||      and  *+ar2(ir0), r3, r3
        sti  r5, *-ar6(ir0)
||      and  *+ar2(ir0), r3
        mpyf *+ar5, r6, r6
||      stf  r0, *-ar1
        mpyf *+ar5, r6
||      stf  r0, *-ar1
        stf  r0, *-ar1
||      mpyf *+ar5, r6, r6
        stf  r0, *-ar1
||      mpyf *+ar5, r6
        mpyi *++ar2, r4, r4
||      sti  r5, *--ar6
        mpyi *++ar2, r4
||      sti  r5, *--ar6
        sti  r5, *--ar6
||      mpyi *++ar2, r3, r3
        sti  r5, *--ar6
||      mpyi *++ar2, r3
        or  *++ar3(ir0), r4, r4
||      sti r6, *--ar7(ir0)
        or  *++ar3(ir0), r4
||      sti r6, *--ar7(ir0)
        sti r6, *--ar7(ir0)
||      or  *++ar3(ir0), r4, r4
        sti r6, *--ar7(ir0)
||      or  *++ar3(ir0), r4
        xor *ar2++(ir1), r3, r3
||      sti r5, *ar6--(ir1)
        xor *ar2++(ir1), r3
||      sti r5, *ar6--(ir1)
        sti r5, *ar6--(ir1)
||      xor *ar2++(ir1), r3, r3
        sti r5, *ar6--(ir1)
||      xor *ar2++(ir1), r3
        mpyf *ar4, *ar3, r1
||      addf r5, r2             ; p=0
        addf r5, r2
||      mpyf *ar4, *ar3, r1
        mpyf *ar3, r1
||      addf r2, *ar4, r2       ; p=1
        addf r2, *ar4, r2
||      mpyf *ar3, r1
        mpyf r2, r1
||      addf *ar3, *ar4, r2     ; p=2
        addf *ar3, *ar4, r2
||      mpyf r2, r1
        mpyf *ar3, r1
||      addf *ar4, r2, r2       ; p=3
        addf *ar4, r2, r2
||      mpyf *ar3, r1
        mpyf *ar3, r1
||      addf *ar4, r2
        addf *ar4, r2
||      mpyf *ar3, r1
        mpyf *ar3, r1, r1
||      addf *ar4, r2
        addf *ar4, r2
||      mpyf *ar3, r1, r1
        mpyf *ar3, r1, r1
||      addf *ar4, r2
        addf *ar4, r2
||      mpyf *ar3, r1, r1
        mpyf *ar4, *ar3, r1
||      subf r5, r2             ; p=0
        subf r5, r2
||      mpyf *ar4, *ar3, r1
        mpyf *ar3, r1
||      subf r2, *ar4, r2       ; p=1
        subf r2, *ar4, r2
||      mpyf *ar3, r1
        mpyf r2, r1
||      subf *ar3, *ar4, r2     ; p=2
        subf *ar3, *ar4, r2
||      mpyf r2, r1
        mpyf *ar3, r1
||      subf *ar4, r2, r2       ; p=3
        subf *ar4, r2, r2
||      mpyf *ar3, r1
        mpyf *ar3, r1
||      subf *ar4, r2
        subf *ar4, r2
||      mpyf *ar3, r1
        mpyf *ar3, r1, r1
||      subf *ar4, r2
        subf *ar4, r2
||      mpyf *ar3, r1, r1
        mpyf *ar3, r1, r1
||      subf *ar4, r2
        subf *ar4, r2
||      mpyf *ar3, r1, r1
        mpyi *ar4, *ar3, r1
||      addi r5, r2             ; p=0
        addi r5, r2
||      mpyi *ar4, *ar3, r1
        mpyi *ar3, r1
||      addi r2, *ar4, r2       ; p=1
        addi r2, *ar4, r2
||      mpyi *ar3, r1
        mpyi r2, r1
||      addi *ar3, *ar4, r2     ; p=2
        addi *ar3, *ar4, r2
||      mpyi r2, r1
        mpyi *ar3, r1
||      addi *ar4, r2, r2       ; p=3
        addi *ar4, r2, r2
||      mpyi *ar3, r1
        mpyi *ar3, r1
||      addi *ar4, r2
        addi *ar4, r2
||      mpyi *ar3, r1
        mpyi *ar3, r1, r1
||      addi *ar4, r2
        addi *ar4, r2
||      mpyi *ar3, r1, r1
        mpyi *ar3, r1, r1
||      addi *ar4, r2
        addi *ar4, r2
||      mpyi *ar3, r1, r1
        mpyi *ar4, *ar3, r1
||      subi r5, r2             ; p=0
        subi r5, r2
||      mpyi *ar4, *ar3, r1
        mpyi *ar3, r1
||      subi r2, *ar4, r2       ; p=1
        subi r2, *ar4, r2
||      mpyi *ar3, r1
        mpyi r2, r1
||      subi *ar3, *ar4, r2     ; p=2
        subi *ar3, *ar4, r2
||      mpyi r2, r1
        mpyi *ar3, r1
||      subi *ar4, r2, r2       ; p=3
        subi *ar4, r2, r2
||      mpyi *ar3, r1
        mpyi *ar3, r1
||      subi *ar4, r2
        subi *ar4, r2
||      mpyi *ar3, r1
        mpyi *ar3, r1, r1
||      subi *ar4, r2
        subi *ar4, r2
||      mpyi *ar3, r1, r1
        mpyi *ar3, r1, r1
||      subi *ar4, r2
        subi *ar4, r2
||      mpyi *ar3, r1, r1

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
