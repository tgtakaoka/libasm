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

;;; 02X
        li      r3,0405H
        ai      r2,0333H
        andi    r4,0255H
        ori     r6,0277H
        ci      r8,0299H
        lwpi    02f0H
        stwp    r10
        stst    r12
;;; 03X
        limi    0320H
        idle
        rset
        rtwp
        ckon
        ckof
        lrex
;;; 04X
        blwp    r5
        b       r4
        x       r8
        clr     r12
;;; 05X
        neg     r6
        inv     r4
        inc     r8
        inct    r12
;;; 06X
        dec     r7
        dect    r4
        bl      r8
        swpb    r12
;;; 07X
        seto    r8
        abs     r4
;;; 08X
        sra     r9,r0
        srl     r11,10
        sla     r12,11
        src     r13,12
;;; 1X
        jmp     $+24H
        jlt     $+26H
        jle     $+28H
        jeq     $+2aH
        jhe     $+2cH
        jgt     $+2eH
        jne     $+30H
        jnc     $+32H
        joc     $+34H
        jno     $+36H
        jl      $+38H
        jh      $+3aH
        jop     $+3cH
        sbo     1eH
        sbz     1fH
        tb      20H
;;; 2X
        coc     @2223H(r1),r0
        czc     @2627H(r5),r0
        xor     @2a2bH(r9),r0
        xop     @2e2fH(r13),0
;;; 3X
        ldcr    *r1+,16
        stcr    *r5+,16
        mpy     *r9+,r0
        div     *r13+,r0
;;; 4X
        szc     @89abH(r7),*r5
;;; 5X
        szcb    *r8+,*r9
;;; 6X
        s       r9,*r14
;;; 7X
        sb      *r10,@0bcdeH(r2)
;;; 8X
        c       @0cdefH(r11),@0123H(r6)
;;; 9X
        cb      *r12+,@0def0H(r10)
;;; AX
        a       r13,@0ef01H(r15)
;;; BX
        ab      *r14,*r3+
;;; CX
        mov     @0123H(r15),*r7+
;;; DX
        movb    *r0+,*r11+
;;; EX
        soc     r1,*r12+
;;; FX
        socb    *r2,r0

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
