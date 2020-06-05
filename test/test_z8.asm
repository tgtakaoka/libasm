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

        cpu     z8601
        org     0

        dec     >01h
        dec     r1
        dec     @02h
        dec     @r2
        rlc     11h
        rlc     r1
        rlc     @12h
        rlc     @r2

        inc     21h
        inc     r1
        inc     @02h
        inc     @r2

;        jp      @031h           ; should be error
        jp      @032h
        jp      @rr2
        srp     #30h

        da      041h
        da      r1
        da      @042h
        da      @r2
        pop     051h
        pop     r1
        pop     @052h
        pop     @r2
        com     061h
        com     r1
        com     @062h
        com     @r2
        push    071h
        push    r1
        push    @072h
        push    @r2
        decw    081h
        decw    r1
        decw    @082h
        decw    @r2
        rl      091h
        rl      r1
        rl      @092h
        rl      @r2
        incw    0a1h
        incw    r1
        incw    @0a2h
        incw    @r2
        clr     0b1h
        clr     r1
        clr     @0b2h
        clr     @r2
        rrc     0c1h
        rrc     r1
        rrc     @0c2h
        rrc     @r2
        sra     0d1h
        sra     r1
        sra     @0d2h
        sra     @r2
        rr      0e1h
        rr      r2
        rr      @0e2h
        rr      @r0
        swap    0f1h
        swap    r1
        swap    @0f2h
        swap    @r2

        add     r0,r3
        add     r4,@r5
        add     78h,56h
        add     r8,56h
        add     78h,r6
        add     >7,@6
        add     >7,@r6
        add     r7,@6
        add     >7,#8
        add     r7,#8
        add     @8,#9
        add     @r8,#9

        adc     r3,r4
        adc     r1,@r4
        adc     16h,15h
        adc     r6,15h
        adc     16h,r6
        adc     17h,@16h
        adc     17h,@r6
        adc     r7,@16h
        adc     17h,#18h
        adc     r7,#18h
        adc     @18h,#19h
        adc     @r8,#19h

        sub     r2,r3
        sub     r4,@r5
        sub     26h,25h
        sub     r6,25h
        sub     26h,r5
        sub     27h,@26h
        sub     27h,@r6
        sub     r7,@26h
        sub     27h,#28h
        sub     r7,#28h
        sub     @28h,#29h
        sub     @r8,#29h

        sbc     r3,r3
        sbc     r3,@r4
        sbc     78h,56h
        sbc     r8,56h
        sbc     78h,r6
        sbc     37h,@36h
        sbc     27h,@r6
        sbc     r7,@36h
        sbc     37h,#38h
        sbc     r7,#38h
        sbc     @38h,#39h
        sbc     @r8,#39h

        or      r4,r3
        or      r4,@r4
        or      46h,45h
        or      r6,45h
        or      46h,r5
        or      89h,@67h
        or      89h,@r7
        or      r9,@67h
        or      47h,#48h
        or      r7,#48h
        or      @48h,#49h
        or      @r8,#49h

        and     r5,r3
        and     r5,@r4
        and     56h,55h
        and     r6,55h
        and     56h,r5
        and     57h,@56h
        and     57h,@r6
        and     r7,@56h
        and     78h,#9ah
        and     r8,#9ah
        and     @58h,#59h
        and     @r8,#59h

        tcm     r6,r3
        tcm     r6,@r4
        tcm     66h,65h
        tcm     r6,65h
        tcm     66h,r5
        tcm     67h,@66h
        tcm     67h,@r6
        tcm     r7,@66h
        tcm     67h,#68h
        tcm     r7,#68h
        tcm     @89h,#0abh
        tcm     @r9,#0abh

        tm      r7,r3
        tm      r7,@r4
        tm      76h,75h
        tm      r6,75h
        tm      76h,r5
        tm      77h,@76h
        tm      77h,@r6
        tm      r7,@76h
        tm      77h,#78h
        tm      r7,#78h
        tm      @78h,#79h
        tm      @r8,#79h

        cp      r10,r3
        cp      r10,@r4
        cp      0a6h,0a5h
        cp      r6,0a5h
        cp      0a6h,r5
        cp      0a7h,@0a6h
        cp      0a7h,@r6
        cp      r7,@0a6h
        cp      0a7h,#0a8h
        cp      r7,#0a8h
        cp      @0a8h,#0a9h
        cp      @r8,#0a9h

        xor     r11,r3
        xor     r11,@r4
        xor     0b6h,0b5h
        xor     r6,0b5h
        xor     0b6h,r5
        xor     0b7h,@0b6h
        xor     0b7h,@r6
        xor     r7,@0b6h
        xor     0a7h,#0a8h
        xor     r7,#0abh
        xor     @0a8h,#0a9h
        xor     @r8,#0a9h

;        call    @0d5h           ; should be warning
        call    @0d6h
        call    0d7d8h

        lde     r7,@rr4
        ldei    @r7,@rr4
        lde     @rr4,r9
        ldei    @rr4,@r9

        ldc     r11,@rr4
        ldci    @r11,@rr4
        ldc     @rr4,r13
        ldci    @rr4,@r13

        ld      r12,0c9h(r8)
        ld      0d9h(r8),r13

        ld      r14,@r4
        ld      0e6h,0e5h
        ld      0e7h,@0e6h
        ld      0e7h,#0e8h
        ld      @0e8h,#0e9h
        ld      @r15,r4
        ld      @0f7h,0f6h
;;; x8
        ld      r0, >9
        ld      r0, 9           ; LD @r0,@r9
        ld      r0, r9
        ld      r1, 19h
        ld      r2, 29h
        ld      r3, 39h
        ld      r4, 49h
        ld      r5, 59h
        ld      r6, 69h
        ld      r7, 79h
        ld      r8, 89h
        ld      r9, 99h
        ld      r10,0a9h
        ld      r11,0b9h
        ld      r12,0c9h
        ld      r13,0d9h
        ld      r14,0e9h
        ld      r14,r9
        ld      r15,0f9h
;;; x9
        ld      >10, r0
        ld      10,  r0         ; LD r10,r0
        ld      1ah, r1
        ld      2ah, r2
        ld      3ah, r3
        ld      4ah, r4
        ld      5ah, r5
        ld      6ah, r6
        ld      7ah, r7
        ld      8ah, r8
        ld      9ah, r9
        ld      0aah,r10
        ld      0bah,r11
        ld      0cah,r12
        ld      0dah,r13
        ld      0eah,r14
        ld      0fah,r15
;;; xA
        djnz    r0, $+13
        djnz    r1, $+29
        djnz    r2, $+45
        djnz    r3, $+61
        djnz    r4, $+77
        djnz    r5, $+93
        djnz    r6, $+109
        djnz    r7, $+125
        djnz    r8, $-115
        djnz    r9, $-99
        djnz    r10,$-83
        djnz    r11,$-67
        djnz    r12,$-51
        djnz    r13,$-35
        djnz    r14,$-19
        djnz    r15,$-3
;;; xB
        jr      f,   $+14
        jr      lt,  $+30
        jr      le,  $+46
        jr      ule, $+62
        jr      ov,  $+78
        jr      mi,  $+94
        jr      z,   $+110
        jr      eq,  $+110
        jr      c,   $+126
        jr      ult, $+126
        jr      $-114
        jr      ge,  $-98
        jr      gt,  $-82
        jr      ugt, $-66
        jr      nov, $-50
        jr      pl,  $-34
        jr      nz,  $-18
        jr      ne,  $-18
        jr      nc,  $-2
        jr      uge, $-2
;;; xC
        ld      r0, #0dh
        ld      r1, #1dh
        ld      r2, #2dh
        ld      r3, #3dh
        ld      r4, #4dh
        ld      r5, #5dh
        ld      r6, #6dh
        ld      r7, #7dh
        ld      r8, #8dh
        ld      r9, #9dh
        ld      r10,#0adh
        ld      r11,#0bdh
        ld      r12,#0cdh
        ld      r13,#0ddh
        ld      r14,#0edh
        ld      r15,#0fdh
;;; xD
        jp      f,   0e0fh
        jp      lt,  1e1fh
        jp      le,  2e2fh
        jp      ule, 3e3fh
        jp      ov,  4e4fh
        jp      mi,  5e5fh
        jp      z,   6e6fh
        jp      eq,  6e6fh
        jp      c,   7e7fh
        jp      ult, 7e7fh
        jp      8e8fh
        jp      ge,  9e9fh
        jp      gt,  0aeafh
        jp      ugt, 0be0fh
        jp      nov, 0cecfh
        jp      pl,  0dedfh
        jp      nz,  0eeefh
        jp      ne,  0eeefh
        jp      nc,  0feffh
        jp      uge, 0feffh
;;; xE
        inc     r0
        inc     r1
        inc     r2
        inc     r3
        inc     r4
        inc     r5
        inc     r6
        inc     r7
        inc     r8
        inc     r9
        inc     r10
        inc     r11
        inc     r12
        inc     r13
        inc     r14
        inc     r15
;;; xF
        stop
        halt
        di
        ei
        ret
        iret
        rcf
        scf
        ccf
        nop

        end
        ;; comment

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
