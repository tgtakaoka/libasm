;;; Copyright 2021 Tadashi G. Takaoka
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

;;; 0x
        nop
        add     a,#4
        jmp     005h
        en      i
        dec     a
        in      a, p1
        in      a, p2
        movd    a, p4
        movd    a, p5
        movd    a, p6
        movd    a, p7
;;; 1x
        inc     @r0
        inc     @r1
        jb      0, 113h
        addc    a, #14h
        call    015h
        dis     i
        jtf     117h
        inc     a
        inc     r0
        inc     r1
        inc     r2
        inc     r3
        inc     r4
        inc     r5
        inc     r6
        inc     r7
;;; 2x
        xch     a, @r0
        xch     a, @r1
        mov     a, #24h
        jmp     125h
        en      tcnti
        jnt0    127h
        clr     a
        xch     a, r0
        xch     a, r1
        xch     a, r2
        xch     a, r3
        xch     a, r4
        xch     a, r5
        xch     a, r6
        xch     a, r7
;;; 3x
        xchd    a, @r0
        xchd    a, @r1
        jb      1, 133h
        call    135h
        dis     tcnti
        jt0     137h
        cpl     a
        outl    p1, a
        outl    p2, a
        movd    p4, a
        movd    p5, a
        movd    p6, a
        movd    p7, a
;;; 4x
        orl     a, @r0
        orl     a, @r1
        mov     a, t
        orl     a, #44h
        jmp     245h
        strt    cnt
        jnt1    147h
        swap    a
        orl     a, r0
        orl     a, r1
        orl     a, r2
        orl     a, r3
        orl     a, r4
        orl     a, r5
        orl     a, r6
        orl     a, r7
;;; 5x
        anl     a, @r0
        anl     a, @r1
        jb      2, 153h
        anl     a, #54h
        call    255h
        strt    t
        jt1     157h
        da      a
        anl     a, r0
        anl     a, r1
        anl     a, r2
        anl     a, r3
        anl     a, r4
        anl     a, r5
        anl     a, r6
        anl     a, r7
;;; 6x
        add     a, @r0
        add     a, @r1
        mov     t, a
        jmp     365h
        stop    tcnt
        rrc     a
        add     a, r0
        add     a, r1
        add     a, r2
        add     a, r3
        add     a, r4
        add     a, r5
        add     a, r6
        add     a, r7
;;; 7x
        addc    a, @r0
        addc    a, @r1
        jb      3, 173h
        call    375h
        ent0    clk
        jf1     177h
        rr      a
        addc    a, r0
        addc    a, r1
        addc    a, r2
        addc    a, r3
        addc    a, r4
        addc    a, r5
        addc    a, r6
        addc    a, r7
;;; 8x
        movx    a, @r0
        movx    a, @r1
        ret
        jmp     485h
        clr     f0
        jni     187h
        orl     p1, #8ah
        orl     p2, #8bh
        orld    p4, a
        orld    p5, a
        orld    p6, a
        orld    p7, a
;;; 9x
        movx    @r0, a
        movx    @r1, a
        jb      4, 193h
        retr
        call    495h
        cpl     f0
        jnz     197h
        clr     c
        anl     p1, #9ah
        anl     p2, #9bh
        anld    p4, a
        anld    p5, a
        anld    p6, a
        anld    p7, a
;;; Ax
        mov     @r0, a
        mov     @r1, a
        movp    a, @a
        jmp     5a5h
        clr     f1
        cpl     c
        mov     r0, a
        mov     r1, a
        mov     r2, a
        mov     r3, a
        mov     r4, a
        mov     r5, a
        mov     r6, a
        mov     r7, a
;;; Bx
	mov     @r0, #0b1h
        mov     @r1, #0b2h
        jb      5, 1b3h
        jmpp    @a
        call    5b5h
        cpl     f1
        jf0     1b7h
        mov     r0, #0b9h
        mov     r1, #0bah
        mov     r2, #0bbh
        mov     r3, #0bch
        mov     r4, #0bdh
        mov     r5, #0beh
        mov     r6, #0bfh
        mov     r7, #0c0h
;;; Cx
	jmp     6c5h
        sel     rb0
        jz      1c7h
        mov     a, psw
        dec     r0
        dec     r1
        dec     r2
        dec     r3
        dec     r4
        dec     r5
        dec     r6
        dec     r7
;;; Dx
        xrl     a, @r0
        xrl     a, @r1
        jb      6, 1d3h
        xrl     a, #0d4h
        call    6d5h
        sel     rb1
        mov     psw, a
        xrl     a, r0
        xrl     a, r1
        xrl     a, r2
        xrl     a, r3
        xrl     a, r4
        xrl     a, r5
        xrl     a, r6
        xrl     a, r7
;;; Ex
        movp3   a, @a
        jmp     7e5h
        sel     mb0
        jnc     1e7h
        rl      a
        djnz    r0, 2e9h
        djnz    r1, 2eah
        djnz    r2, 2ebh
        djnz    r3, 2ech
        djnz    r4, 2edh
        djnz    r5, 2eeh
        djnz    r6, 2efh
        djnz    r7, 2f0h
;;; Fx
        mov     a, @r0
        mov     a, @r1
        jb      7, 2f3h
        call    7f5h
        sel     mb1
        jc      2f7h
        rlc     a
        mov     a, r0
        mov     a, r1
        mov     a, r2
        mov     a, r3
        mov     a, r4
        mov     a, r5
        mov     a, r6
        mov     a, r7

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
