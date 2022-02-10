;;; Copyright 2022 Tadashi G. Takaoka
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

        cpu     8096
        org     2000h
;;; 0X
        skip    01h
        clr     02h
        not     04h
        neg     04h
        dec     06h
        ext     08h
        inc     08h
        shr     0ah, #9
        shr     12h, 11h
        shl     0ch, #11
        shl     12h, 11h
        shra    0ch, #11
        shra    12h, 11h
        shrl    10h, #15
        shrl    14h, 13h
        shll    10h, #15
        shll    14h, 13h
        shral   10h, #15
        shral   14h, 13h
        norml   14h, 13h
;;; 1X
        clrb    12h
        notb    13h
        negb    14h
        decb    16h
        extb    18h
        incb    18h
        shrb    1ah, #9
        shrb    1ah, 19h
        shlb    1bh, #10
        shlb    1bh, 1ah
        shrab   1ch, #11
        shrab   1ch, 1bh
;;; 2X
        sjmp    $+23h
        sjmp    $+124h
        sjmp    $+225h
        sjmp    $+326h
        sjmp    $-3d9h
        sjmp    $-2d8h
        sjmp    $-1d7h
        sjmp    $-0d6h
        scall   $+2bh
        scall   $+12ch
        scall   $+22dh
        scall   $+32eh
        scall   $-3d1h
        scall   $-2d0h
        scall   $-1cfh
        scall   $-0ceh
;;; 3X
        jbc     31h, 0, $+35h
        jbc     32h, 1, $+36h
        jbc     33h, 2, $+37h
        jbc     34h, 3, $+38h
        jbc     35h, 4, $+39h
        jbc     36h, 5, $+3ah
        jbc     37h, 6, $+3bh
        jbc     38h, 7, $+3ch
        jbs     39h, 0, $+3dh
        jbs     3ah, 1, $+3eh
        jbs     3bh, 2, $+3fh
        jbs     3ch, 3, $+40h
        jbs     3dh, 4, $+41h
        jbs     3eh, 5, $+42h
        jbs     3fh, 6, $+43h
        jbs     40h, 7, $+44h
;;; 4X
        and     46h, 44h, 42h
        and     46h, 44h, #4342h
        and     48h, 46h, [44h]
        and     48h, 46h, 45h[44h]
        and     4ah, 48h, 4746h[44h]
        add     4ah, 48h, 46h
        add     4ah, 48h, #4746h
        add     4ch, 4ah, [48h]
        add     4ch, 4ah, 49h[48h]
        add     4eh, 4ch, 4b4ah[48h]
        sub     4eh, 4ch, 4ah
        sub     4eh, 4ch, #4b4ah
        sub     50h, 4eh, [4ch]
        sub     50h, 4eh, 4dh[4ch]
        sub     52h, 50h, 4f4eh[4ch]
        mulu    54h, 50h, 4eh
        mulu    54h, 50h, #4f4eh
        mulu    54h, 52h, [50h]
        mulu    58h, 54h, 52h[50h]
        mulu    58h, 54h, 5352h[50h]
        mul     54h, 50h, 4eh
        mul     54h, 50h, #4f4eh
        mul     54h, 52h, [50h]
        mul     58h, 54h, 52h[50h]
        mul     58h, 54h, 5352h[50h]
;;; 5X
        andb    53h, 52h, 51h
        andb    54h, 53h, #52h
        andb    56h, 55h, [54h]
        andb    57h, 56h, 55h[54h]
        andb    59h, 58h, 5756h[54h]
        addb    57h, 56h, 55h
        addb    5ah, 59h, #58h
        addb    5ah, 59h, [58h]
        addb    5bh, 5ah, 59h[58h]
        addb    5dh, 5ch, 5b5ah[58h]
        subb    5bh, 5ah, 59h
        subb    5ch, 5bh, #5ah
        subb    5eh, 5dh, [5ch]
        subb    5fh, 5eh, 5dh[5ch]
        subb    61h, 60h, 5f5eh[5ch]
        mulub   60h, 5fh, 5eh
        mulub   60h, 5fh, #5eh
        mulub   62h, 61h, [60h]
        mulub   64h, 62h, 61h[60h]
        mulub   66h, 64h, 6362h[60h]
        mulb    60h, 5fh, 5eh
        mulb    60h, 5fh, #5eh
        mulb    62h, 61h, [60h]
        mulb    64h, 62h, 61h[60h]
        mulb    66h, 64h, 6362h[60h]
;;; 6X
        and     64h, 62h
        and     64h, #6362h
        and     66h, [64h]
        and     66h, 65h[64h]
        and     68h, 6766h[64h]
        add     68h, 66h
        add     68h, #6766h
        add     6ah, [68h]
        add     6ah, 69h[68h]
        add     6ch, 6b6ah[68h]
        sub     6ch, 6ah
        sub     6ch, #6b6ah
        sub     6eh, [6ch]
        sub     6eh, 6dh[6ch]
        sub     72h, 7170h[6eh]
        mulu    70h, 6eh
        mulu    70h, #6f6eh
        mulu    74h, [70h]
        mulu    74h, 72h[70h]
        mulu    74h, 7372h[70h]
        mul     70h, 6eh
        mul     70h, #6f6eh
        mul     74h, [70h]
        mul     74h, 72h[70h]
        mul     74h, 7372h[70h]
;;; 7X
        andb    72h, 71h
        andb    73h, #72h
        andb    75h, [74h]
        andb    76h, 75h[74h]
        andb    78h, 7776h[74h]
        addb    76h, 75h
        addb    77h, #76h
        addb    79h, [78h]
        addb    7ah, 79h[78h]
        addb    7ch, 7b7ah[78h]
        subb    7ah, 79h
        subb    7bh, #7ah
        subb    7eh, [7ch]
        subb    7eh, 7dh[7ch]
        subb    80h, 7f7eh[7ch]
        mulub   7eh, 7dh
        mulub   80h, #7fh
        mulub   82h, [80h]
        mulub   82h, -127[80h]
        mulub   84h, 8382h[80h]
        mulb    7eh, 7dh
        mulb    80h, #7fh
        mulb    82h, [80h]
        mulb    82h, -127[80h]
        mulb    84h, 8382h[80h]
;;; 8X
        or      84h, 82h
        or      84h, #8382h
        or      86h, [84h]
        or      86h, -123[84h]
        or      88h, 8786h[84h]
        xor     88h, 86h
        xor     88h, #8786h
        xor     8ah, [88h]
        xor     8ah, -119[88h]
        xor     8ch, 8b8ah[88h]
        cmp     8ch, 8ah
        cmp     8ch, #8b8ah
        cmp     8eh, [8ch]
        cmp     8eh, 0dh[8ch]
        cmp     90h, 8f8eh[8ch]
        divu    90h, 8eh
        divu    90h, #8f8eh
        divu    94h, [90h]
        divu    94h, -111[90h]
        divu    94h, 9392h[90h]
        div     90h, 8eh
        div     90h, #8f8eh
        div     94h, [90h]
        div     94h, -111[90h]
        div     94h, 9392h[90h]
;;; 9X
        orb     92h, 91h
        orb     93h, #92h
        orb     95h, [94h]
        orb     96h, -107[94h]
        orb     98h, 9796h[94h]
        xorb    96h, 95h
        xorb    97h, #96h
        xorb    99h, [98h]
        xorb    9ah, -103[98h]
        xorb    9ch, 9b9ah[98h]
        cmpb    9ah, 99h
        cmpb    9bh, #9ah
        cmpb    9dh, [9ch]
        cmpb    9eh, -99[9ch]
        cmpb    0a0h, 9f9eh[9ch]
        divub   9eh, 9dh
        divub   0a0h, #9eh
        divub   0a2h, [0a0h]
        divub   0a2h, -95[0a0h]
        divub   0a4h, 0a3a2h[0a0h]
        divb    9eh, 9dh
        divb    0a0h, #9eh
        divb    0a2h, [0a0h]
        divb    0a2h, -95[0a0h]
        divb    0a4h, 0a3a2h[0a0h]
;;; AX
        ld      0a4h, 0a2h
        ld      0a4h, #0a3a2h
        ld      0a6h, [0a4h]
        ld      0a6h, -91[0a4h]
        ld      0a8h, 0a7a6h[0a4h]
        addc    0a8h, 0a6h
        addc    0a8h, #0a7a6h
        addc    0aah, [0a8h]
        addc    0aah, -87[0a8h]
        addc    0ach, 0abaah[0a8h]
        subc    0ach, 0aah
        subc    0ach, #0abaah
        subc    0aeh, [0ach]
        subc    0aeh, -83[0ach]
        subc    0b0h, 0afaeh[0ach]
        ldbze   0aeh, 0adh
        ldbze   0b0h, #0afh
        ldbze   0b2h, [0b0h]
        ldbze   0b2h, -79[0b0h]
        ldbze   0b4h, 0b3b2h[0b0h]
;;; BX
        ldb     0b2h, 0b1h
        ldb     0b3h, #0b2h
        ldb     0b5h, [0b4h]
        ldb     0b6h, -75[0b4h]
        ldb     0b8h, 0b7b6h[0b4h]
        addcb   0b6h, 0b5h
        addcb   0b7h, #0b6h
        addcb   0b9h, [0b8h]
        addcb   0bah, -71[0b8h]
        addcb   0bch, 0bbbah[0b8h]
        subcb   0bah, 0b9h
        subcb   0bbh, #0bah
        subcb   0bdh, [0bch]
        subcb   0beh, -67[0bch]
        subcb   0c0h, 0bfbeh[0bch]
        ldbse   0beh, 0bdh
        ldbse   0c0h, #0bfh
        ldbse   0c2h, [0c0h]
        ldbse   0c2h, -63[0c0h]
        ldbse   0c4h, 0c3c2h[0c0h]
;;; CX
        st      0c4h, 0c2h
        st      0c6h, [0c4h]
        st      0c6h, -59[0c4h]
        st      0c8h, 0c7c6h[0c4h]
        stb     0c6h, 0c5h
        stb     0c7h, [0c6h]
        stb     0cah, -55[0c8h]
        stb     0cch, 0cbcah[0c8h]
        push    0cah
        push    #0cbcah
        push    [0cch]
        push    -51[0cch]
        push    0dfdeh[0cch]
        pop     0ceh
        pop     [0d0h]
        pop     -47[0d0h]
        pop     0d3d2h[0d0h]
;;; DX
        jnst    $-2dh
        jnh     $-2ch
        jgt     $-2bh
        jnc     $-2ah
        jnvt    $-29h
        jnv     $-28h
        jge     $-27h
        jne     $-26h
        jst     $-25h
        jh      $-24h
        jle     $-23h
        jc      $-22h
        jvt     $-21h
        jv      $-20h
        jlt     $-1fh
        je      $-1eh
;;; EX
        djnz    0e1h, $-27
        br      [0e4h]
        ljmp    $-1615h
        lcall   $-0e0dh
;;; FX
        ret
        pushf
        popf
        trap
        clrc
        setc
        di
        ei
        clrvt
        nop
        rst
