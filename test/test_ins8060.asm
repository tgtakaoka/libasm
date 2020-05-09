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

        cpu     sc/mp
        org     0x0100

;;; 0X
        halt
        xae
        ccl
        scl
        dint
        ien
        csa
        cas
        nop
;;; 1X
        sio
        sr
        srl
        rr
        rrl
;;; 3X
        xpal    pc
        xpal    p0
        xpal    p1
        xpal    p2
        xpal    p3
        xpah    pc
        xpah    p0
        xpah    p1
        xpah    p2
        xpah    p3
        xppc    pc
        xppc    p0
        xppc    p1
        xppc    p2
        xppc    p3
;;; 4X
        lde
;;; 5X
        ane
        ore
;;; 6X
        xre
        dae
;;; 7X
        ade
        cae
;;; 8X
        dly     0x90
;;; 9X
        jmp     $               ; pc points the last byte, will be +1
        jmp     +127(p1)
        jmp     -127(p2)
        jmp     e(p3)
        jp      $-125
        jp      -127(P1)
        jp      e(p2)
        jp      +127(p3)
        jz      $+129
        jz      e(p1)
        jz      +127(p2)
        jz      -127(p3)
        jnz     e(pc)
        jnz     e(p0)
        jnz     +127(p1)
        jnz     -127(p2)
        jnz     e(p3)
;;; AX
        ild     $
        ild     +127(p1)
        ild     -127(p2)
        ild     e(p3)
;;; BX
        dld     $+128
        dld     +127(p1)
        dld     -127(p2)
        dld     e(p3)
;;; CX
        ld      $               ; pc points the last instruction byte
        ld      +127(p1)
        ld      -127(p2)
        ld      e(p3)
        ldi     0xC5
        ld      @e(p1)
        ld      @+127(p2)
        ld      @-127(p3)
        st      e(pc)
        st      +127(p1)
        st      -127(p2)
        st      e(p3)
        st      @e(p1)
        st      @+127(p2)
        st      @-127(p3)
;;; DX
        and     e(p0)
        and     +127(p1)
        and     -127(p2)
        and     e(p3)
        ani     0xD5
        and     @e(p1)
        and     @+127(p2)
        and     @-127(p3)
        or      $
        or      +127(p1)
        or      -127(p2)
        or      e(p3)
        ori     0xDB
        or      @e(p1)
        or      @+127(p2)
        or      @-127(p3)
;;; EX
        xor     $
        xor     +127(p1)
        xor     -127(p2)
        xor     e(p3)
        xri     0xE5
        xor     @e(p1)
        xor     @+127(p2)
        xor     @-127(p3)
        dad     $
        dad     +127(p1)
        dad     -127(p2)
        dad     e(p3)
        dai     0xEB
        dad     @e(p1)
        dad     @+127(p2)
        dad     @-127(p3)
;;; FX
        add     $
        add     +127(p1)
        add     -127(p2)
        add     e(p3)
        adi     0xF5
        add     @e(p1)
        add     @+127(p2)
        add     @-127(p3)
        cad     $
        cad     +127(p1)
        cad     -127(p2)
        cad     e(p3)
        cai     0xFB
        cad     @e(p1)
        cad     @+127(p2)
        cad     @-127(p3)

        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
