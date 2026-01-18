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


        absf  r0, r1
||      stf   r2, *-ar3
        absi  ar4, r5
||      sti   r6, *--ar7
        addf3 r5, r1, r2
||      stf   r3, *ar4--
        addi3 ar5, r6, r7
||      sti   r0, *ar1--%
        and3  ar2, r3, r4
||      sti   r5, *-ar6(ir0)
        ash3  r6, ar7, r0
||      sti   r1, *--ar2(ir0)
        fix   r3, r4
||      sti   r5, *ar6--(ir0)
        float r7, r0
||      stf   r1, *ar2--(ir0)%
        ldf   r3, r4
||      ldf   *-ar5(ir1), r6
        ldf   r7, r0
||      stf   r1, *--ar2(ir1)
        ldi   ar3, r4
||      ldi   *ar5--(ir1), r6
        ldi   ar4, r5
||      sti   r6, *ar7--(ir1)%
        lsh3  r0, ar1, r2
||      sti   r3, *ar4
        mpyf3 r5, r6, r7
||      stf   r0, *-ar1
        mpyi3 ar2, r3, r4
||      sti   r5, *--ar6
        negf  r7, r0
||      stf   r1, *ar2--
        negi  ar3, r4
||      sti   r5, *ar6--%
        not   ar7, r0
||      sti   r1, *-ar2(ir0)
        or3   ar3, r4, r5
||      sti   r6, *--ar7(ir0)

;;         stf   r2, *ar3--(ir0)
;; ||      stf   r0, r1
;;         sti   r6, *ar7--(ir0)%
;; ||      sti   r4, ar5
        subf3 r0, r1, r2
||      stf   r3, *-ar4(ir1)
        subi3 r5, ar6, r7
||      sti   r0, *--ar1(ir1)
        xor3  ar2, r3, r4
||      sti   r5, *ar6--(ir1)

        mpyf3 r1, *ar2, r0
||      addf3 r4, r5, r2
        mpyf3 *ar1, r2, r0
||      addf3 r4, r5, r2
        mpyf3 r1, r2, r0
||      addf3 r4, r5, r2
        mpyf3 *ar1, r2, r0
||      addf3 r4, r5, r3
        mpyf3 r1, r2, r0
||      addf3 r4, *ar5, r3
        mpyf3 r1, r2, r0
||      addf3 r4, r5, r3
        mpyf3 r1, r2, r1
||      addf3 r4, *ar5, r2
        mpyf3 r1, r2, r1
||      addf3 *ar4, r5, r2
        mpyf3 r1, r2, r1
||      addf3 r4, r5, r2
        mpyf3 *ar1, r2, r1
||      addf3 r4, r5, r3
        mpyf3 r1, r2, r1
||      addf3 *ar4, r5, r3
        mpyf3 r1, r2, r1
||      addf3 r4, r5, r3

        mpyf3 r1, *ar2, r0
||      subf3 r4, r5, r2
        mpyf3 *ar1, r2, r0
||      subf3 r4, r5, r2
        mpyf3 r1, r2, r0
||      subf3 r4, r5, r2
        mpyf3 *ar1, r2, r0
||      subf3 r4, r5, r3
        mpyf3 r1, r2, r0
||      subf3 r4, *ar5, r3
        mpyf3 r1, r2, r0
||      subf3 r4, r5, r3
        mpyf3 r1, r2, r1
||      subf3 r4, *ar5, r2
        mpyf3 r1, r2, r1
||      subf3 *ar4, r5, r2
        mpyf3 r1, r2, r1
||      subf3 r4, r5, r2
        mpyf3 *ar1, r2, r1
||      subf3 r4, r5, r3
        mpyf3 r1, r2, r1
||      subf3 *ar4, r5, r3
        mpyf3 r1, r2, r1
||      subf3 r4, r5, r3

        mpyi3 ar1, *ar2, r0
||      addi3 r4, r5, r2
        mpyi3 *ar1, ar2, r0
||      addi3 r4, r5, r2
        mpyi3 r1, r2, r0
||      addi3 r4, r5, r2
        mpyi3 *ar1, r2, r0
||      addi3 r4, ar5, r3
        mpyi3 ar1, r2, r0
||      addi3 r4, *ar5, r3
        mpyi3 r1, r2, r0
||      addi3 r4, r5, r3
        mpyi3 r1, r2, r1
||      addi3 ar4, *ar5, r2
        mpyi3 r1, r2, r1
||      addi3 *ar4, ar5, r2
        mpyi3 r1, r2, r1
||      addi3 r4, r5, r2
        mpyi3 *ar1, r2, r1
||      addi3 ar4, r5, r3
        mpyi3 ar1, r2, r1
||      addi3 *ar4, r5, r3
        mpyi3 r1, r2, r1
||      addi3 r4, r5, r3

        mpyi3 ar1, *ar2, r0
||      subi3 r4, r5, r2
        mpyi3 *ar1, ar2, r0
||      subi3 r4, r5, r2
        mpyi3 r1, r2, r0
||      subi3 r4, r5, r2
        mpyi3 *ar1, r2, r0
||      subi3 r4, ar5, r3
        mpyi3 ar1, r2, r0
||      subi3 r4, *ar5, r3
        mpyi3 r1, r2, r0
||      subi3 r4, r5, r3
        mpyi3 r1, r2, r1
||      subi3 ar4, *ar5, r2
        mpyi3 r1, r2, r1
||      subi3 *ar4, ar5, r2
        mpyi3 r1, r2, r1
||      subi3 r4, r5, r2
        mpyi3 *ar1, r2, r1
||      subi3 ar4, r5, r3
        mpyi3 ar1, r2, r1
||      subi3 *ar4, r5, r3
        mpyi3 r1, r2, r1
||      subi3 r4, r5, r3

        addf r5, r1, r2
||      stf   r3, *ar4--
        addi ar5, r6, r7
||      sti   r0, *ar1--%
        and  ar2, r3, r4
||      sti   r5, *-ar6(ir0)
        ash  r6, ar7, r0
||      sti   r1, *--ar2(ir0)
        lsh  r0, ar1, r2
||      sti   r3, *ar4
        mpyf r5, r6, r7
||      stf   r0, *-ar1
        mpyi ar2, r3, r4
||      sti   r5, *--ar6
        or   ar3, r4, r5
||      sti   r6, *--ar7(ir0)
        subf r0, r1, r2
||      stf   r3, *-ar4(ir1)
        subi r5, ar6, r7
||      sti   r0, *--ar1(ir1)
        xor  ar2, r3, r4
||      sti   r5, *ar6--(ir1)

        mpyf r1, *ar2, r0
||      addf r4, r5, r2
        mpyf *ar1, r2, r0
||      addf r4, r5, r2
        mpyf r1, r2, r0
||      addf r4, r5, r2
        mpyf *ar1, r2, r0
||      addf r4, r5, r3
        mpyf r1, r2, r0
||      addf r4, *ar5, r3
        mpyf r1, r2, r0
||      addf r4, r5, r3
        mpyf r1, r2, r1
||      addf r4, *ar5, r2
        mpyf r1, r2, r1
||      addf *ar4, r5, r2
        mpyf r1, r2, r1
||      addf r4, r5, r2
        mpyf *ar1, r2, r1
||      addf r4, r5, r3
        mpyf r1, r2, r1
||      addf *ar4, r5, r3
        mpyf r1, r2, r1
||      addf r4, r5, r3

        mpyf r1, *ar2, r0
||      subf r4, r5, r2
        mpyf *ar1, r2, r0
||      subf r4, r5, r2
        mpyf r1, r2, r0
||      subf r4, r5, r2
        mpyf *ar1, r2, r0
||      subf r4, r5, r3
        mpyf r1, r2, r0
||      subf r4, *ar5, r3
        mpyf r1, r2, r0
||      subf r4, r5, r3
        mpyf r1, r2, r1
||      subf r4, *ar5, r2
        mpyf r1, r2, r1
||      subf *ar4, r5, r2
        mpyf r1, r2, r1
||      subf r4, r5, r2
        mpyf *ar1, r2, r1
||      subf r4, r5, r3
        mpyf r1, r2, r1
||      subf *ar4, r5, r3
        mpyf r1, r2, r1
||      subf r4, r5, r3

        mpyi ar1, *ar2, r0
||      addi r4, r5, r2
        mpyi *ar1, ar2, r0
||      addi r4, r5, r2
        mpyi r1, r2, r0
||      addi r4, r5, r2
        mpyi *ar1, r2, r0
||      addi r4, ar5, r3
        mpyi ar1, r2, r0
||      addi r4, *ar5, r3
        mpyi r1, r2, r0
||      addi r4, r5, r3
        mpyi r1, r2, r1
||      addi ar4, *ar5, r2
        mpyi r1, r2, r1
||      addi *ar4, ar5, r2
        mpyi r1, r2, r1
||      addi r4, r5, r2
        mpyi *ar1, r2, r1
||      addi ar4, r5, r3
        mpyi ar1, r2, r1
||      addi *ar4, r5, r3
        mpyi r1, r2, r1
||      addi r4, r5, r3

        mpyi ar1, *ar2, r0
||      subi r4, r5, r2
        mpyi *ar1, ar2, r0
||      subi r4, r5, r2
        mpyi r1, r2, r0
||      subi r4, r5, r2
        mpyi *ar1, r2, r0
||      subi r4, ar5, r3
        mpyi ar1, r2, r0
||      subi r4, *ar5, r3
        mpyi r1, r2, r0
||      subi r4, r5, r3
        mpyi r1, r2, r1
||      subi ar4, *ar5, r2
        mpyi r1, r2, r1
||      subi *ar4, ar5, r2
        mpyi r1, r2, r1
||      subi r4, r5, r2
        mpyi *ar1, r2, r1
||      subi ar4, r5, r3
        mpyi ar1, r2, r1
||      subi *ar4, r5, r3
        mpyi r1, r2, r1
||      subi r4, r5, r3

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
