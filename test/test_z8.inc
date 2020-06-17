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

        align   32

;;; one operand
        dec     23H
        rlc     23H
        inc     23H
;       jp      @23H            ; should be error
        jp      @24H
        da      23H
        pop     23H
        com     23H
        push    23H
        decw    23H
        rl      23H
        incw    23H
        clr     23H
        rrc     23H
        sra     23H
        rr      23H
        swap    23H
        dec     @24H
        rlc     @24H
        inc     @24H
        srp     #20h
        da      @24H
        pop     @24H
        com     @24H
        push    @24H
        decw    @24H
        rl      @24H
        incw    @24H
        clr     @24H
        rrc     @24H
        sra     @24H
        rr      @24H
        swap    @24H
;;; two operands
        align   32
        add     r2,r3
        adc     r2,r3
        sub     r2,r3
        sbc     r2,r3
        or      r2,r3
        and     r2,r3
        tcm     r2,r3
        tm      r2,r3
        cp      r2,r3
        xor     r2,r3
        add     r2,@r3
        adc     r2,@r3
        sub     r2,@r3
        sbc     r2,@r3
        or      r2,@r3
        and     r2,@r3
        tcm     r2,@r3
        tm      r2,@r3
        cp      r2,@r3
        xor     r2,@r3
        add     23H,45H
        adc     23H,45H
        sub     23H,45H
        sbc     23H,45H
        or      23H,45H
        and     23H,45H
        tcm     23H,45H
        tm      23H,45H
        cp      23H,45H
        xor     23H,45H
        add     23H,@45H
        adc     23H,@45H
        sub     23H,@45H
        sbc     23H,@45H
        or      23H,@45H
        and     23H,@45H
        tcm     23H,@45H
        tm      23H,@45H
        cp      23H,@45H
        xor     23H,@45H
        add     23H,#45H
        adc     23H,#45H
        sub     23H,#45H
        sbc     23H,#45H
        or      23H,#45H
        and     23H,#45H
        tcm     23H,#45H
        tm      23H,#45H
        cp      23H,#45H
        xor     23H,#45H
;;; working register number in instruction code
        align   32
        ld      r0, 45H
        ld      r1, 45H
        ld      r2, 45H
        ld      r3, 45H
        ld      r4, 45H
        ld      r5, 45H
        ld      r6, 45H
        ld      r7, 45H
        ld      r8, 45H
        ld      r9, 45H
        ld      r10,45H
        ld      r11,45H
        ld      r12,45H
        ld      r13,45H
        ld      r14,45H
        ld      r15,45H
        ld      45H, r0
        ld      45H, r1
        ld      45H, r2
        ld      45H, r3
        ld      45H, r4
        ld      45H, r5
        ld      45H, r6
        ld      45H, r7
        ld      45H, r8
        ld      45H, r9
        ld      45H,r10
        ld      45H,r11
        ld      45H,r12
        ld      45H,r13
        ld      45H,r14
        ld      45H,r15
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
;;; condition code in instruction
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
;;; single byte instruction
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
        di
        ei
        ret
        iret
        rcf
        scf
        ccf
        nop
;;; load instructions
        align   32
        ld      23H,45H
        ld      23H,@45H
        ld      @23H,45H
        ld      23H,#45H
;;; one operand working register
        align   32
        dec     r2
        rlc     r2
        inc     r2
        jp      @rr2
        da      r2
        pop     r2
        com     r2
        push    r2
        decw    r2
        rl      r2
        incw    r2
        clr     r2
        rrc     r2
        sra     r2
        rr      r2
        swap    r2
;;; two operands working register
        align   32
        add     r3,45H
        adc     r3,45H
        sub     r3,45H
        sbc     r3,45H
        or      r3,45H
        and     r3,45H
        tcm     r3,45H
        tm      r3,45H
        cp      r3,45H
        xor     r3,45H
        add     23H,r5
        adc     23H,r5
        sub     23H,r5
        sbc     23H,r5
        or      23H,r5
        and     23H,r5
        tcm     23H,r5
        tm      23H,r5
        cp      23H,r5
        xor     23H,r5
        add     r3,@45H
        adc     r3,@45H
        sub     r3,@45H
        sbc     r3,@45H
        or      r3,@45H
        and     r3,@45H
        tcm     r3,@45H
        tm      r3,@45H
        cp      r3,@45H
        xor     r3,@45H
        add     23H,@r5
        adc     23H,@r5
        sub     23H,@r5
        sbc     23H,@r5
        or      23H,@r5
        and     23H,@r5
        tcm     23H,@r5
        tm      23H,@r5
        cp      23H,@r5
        xor     23H,@r5
        add     r3,#45H
        adc     r3,#45H
        sub     r3,#45H
        sbc     r3,#45H
        or      r3,#45H
        and     r3,#45H
        tcm     r3,#45H
        tm      r3,#45H
        cp      r3,#45H
        xor     r3,#45H
;;; load instructions working register
        align   32
        ld      r3,r5
        ld      r3,@45H
        ld      23H,@r5
        ld      @23H,r5
;;; different instruction code between Z8 and Z88
        align   32
        ld      r2,@r4
        ld      @r2,r4
        ld      @23H,#45H
        ld      @r3,#45H
        ld      r2,45H(r3)
        ld      45H(r3),r2

        ldc     r2,@rr4
        lde     r2,@rr4
        ldc     @rr4,r2
        lde     @rr4,r2

        call    1234H
;       call    @23H            ; should be warning
        call    @24H


;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
