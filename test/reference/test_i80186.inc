;### Copyright 2022 Tadashi G. Takaoka
;###
;### Licensed under the Apache License, Version 2.0 (the "License");
;### you may not use this file except in compliance with the License.
;### You may obtain a copy of the License at
;###
;###     http://www.apache.org/licenses/LICENSE-2.0
;###
;### Unless required by applicable law or agreed to in writing, software
;### distributed under the License is distributed on an "AS IS" BASIS,
;### WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;### See the License for the specific language governing permissions and
;### limitations under the License.

;### Data Transfer
;### Push Immediate
        push    -1
        push    127
        push    -128
        push    128
        push    32767
        push    -32768
        push    32768
;### Push All
        pusha
;### PopAll
        popa

;### Arithmetic
;### Integer Immediate multiply
        imul    ax, -1
        imul    cx, 127
        imul    dx, -128
        imul    bx, 4
        imul    sp, 5
        imul    bp, 6
        imul    si, 7
        imul    di, 8
        imul    ax, 128
        imul    cx, 255
        imul    dx, 300H
        imul    bx, 400H
        imul    sp, 500H
        imul    bp, 600H
        imul    si, 700H
        imul    di, 800H
        imul    ax, bp, -1
        imul    cx, [si], 127
        imul    dx, [1234H], -128
        imul    bx, [di-52], 4
        imul    sp, [bp+1234H], 5
        imul    bp, [bx+si], 6
        imul    si, [bx+di+52], 7
        imul    di, [bp+si+1234H], 8
        imul    ax, bp, 128
        imul    cx, [si], 255
        imul    dx, [1234H], 300H
        imul    bx, [di-52], 400H
        imul    sp, [bp+1234H], 500H
        imul    bp, [bx+si], 600H
        imul    si, [bx+di+52], 700H
        imul    di, [bp+si+1234H], 800H

;### Logic
;### Shift left, unsigned shift left
        shl     ch,2
        shl     byte ptr [si],3
        shl     byte ptr [0x1234],4
        shl     byte ptr [di-0x34],5
        shl     byte ptr [bp+0x1234],6
        shl     byte ptr [bx+si],7
        shl     byte ptr [bx+di+0x34],0
        shl     byte ptr [bp+si+0x1234],2
        shl     bp,8
        shl     word ptr [si],9
        shl     word ptr [0x1234],10
        shl     word ptr [di-0x34],11
        shl     word ptr [bp+0x1234],12
        shl     word ptr [bx+si],13
        shl     word ptr [bx+di+0x34],14
        shl     word ptr [bp+si+0x1234],15
;### Shift arithmetically left, signed shift left
        sal     ch,2
        sal     byte ptr [si],3
        sal     byte ptr [0x1234],4
        sal     byte ptr [di-0x34],5
        sal     byte ptr [bp+0x1234],6
        sal     byte ptr [bx+si],7
        sal     byte ptr [bx+di+0x34],0
        sal     byte ptr [bp+si+0x1234],2
        sal     bp,8
        sal     word ptr [si],9
        sal     word ptr [0x1234],10
        sal     word ptr [di-0x34],11
        sal     word ptr [bp+0x1234],12
        sal     word ptr [bx+si],13
        sal     word ptr [bx+di+0x34],14
        sal     word ptr [bp+si+0x1234],15
;### Shift right, unsigned shift right
        shr     ch,2
        shr     byte ptr [si],3
        shr     byte ptr [0x1234],4
        shr     byte ptr [di-0x34],5
        shr     byte ptr [bp+0x1234],6
        shr     byte ptr [bx+si],7
        shr     byte ptr [bx+di+0x34],0
        shr     byte ptr [bp+si+0x1234],2
        shr     bp,8
        shr     word ptr [si],9
        shr     word ptr [0x1234],10
        shr     word ptr [di-0x34],11
        shr     word ptr [bp+0x1234],12
        shr     word ptr [bx+si],13
        shr     word ptr [bx+di+0x34],14
        shr     word ptr [bp+si+0x1234],15
;### Shift arithmetically right, signed shift right
        sar     ch,1
        sar     byte ptr [si],3
        sar     byte ptr [0x1234],4
        sar     byte ptr [di-0x34],5
        sar     byte ptr [bp+0x1234],6
        sar     byte ptr [bx+si],7
        sar     byte ptr [bx+di+0x34],0
        sar     byte ptr [bp+si+0x1234],2
        sar     bp,8
        sar     word ptr [si],9
        sar     word ptr [0x1234],10
        sar     word ptr [di-0x34],11
        sar     word ptr [bp+0x1234],12
        sar     word ptr [bx+si],13
        sar     word ptr [bx+di+0x34],14
        sar     word ptr [bp+si+0x1234],15
;### Rotate left
        rol     ch,2
        rol     byte ptr [si],3
        rol     byte ptr [0x1234],4
        rol     byte ptr [di-0x34],5
        rol     byte ptr [bp+0x1234],6
        rol     byte ptr [bx+si],7
        rol     byte ptr [bx+di+0x34],0
        rol     byte ptr [bp+si+0x1234],2
        rol     bp,8
        rol     word ptr [si],9
        rol     word ptr [0x1234],10
        rol     word ptr [di-0x34],11
        rol     word ptr [bp+0x1234],12
        rol     word ptr [bx+si],13
        rol     word ptr [bx+di+0x34],14
        rol     word ptr [bp+si+0x1234],15
;### Rotate right
        ror     ch,2
        ror     byte ptr [si],3
        ror     byte ptr [0x1234],4
        ror     byte ptr [di-0x34],5
        ror     byte ptr [bp+0x1234],6
        ror     byte ptr [bx+si],7
        ror     byte ptr [bx+di+0x34],0
        ror     byte ptr [bp+si+0x1234],2
        ror     bp,8
        ror     word ptr [si],9
        ror     word ptr [0x1234],10
        ror     word ptr [di-0x34],11
        ror     word ptr [bp+0x1234],12
        ror     word ptr [bx+si],13
        ror     word ptr [bx+di+0x34],14
        ror     word ptr [bp+si+0x1234],15
;### Rotate left with carry
        rcl     ch,2
        rcl     byte ptr [si],3
        rcl     byte ptr [0x1234],4
        rcl     byte ptr [di-0x34],5
        rcl     byte ptr [bp+0x1234],6
        rcl     byte ptr [bx+si],7
        rcl     byte ptr [bx+di+0x34],0
        rcl     byte ptr [bp+si+0x1234],2
        rcl     bp,8
        rcl     word ptr [si],9
        rcl     word ptr [0x1234],10
        rcl     word ptr [di-0x34],11
        rcl     word ptr [bp+0x1234],12
        rcl     word ptr [bx+si],13
        rcl     word ptr [bx+di+0x34],14
        rcl     word ptr [bp+si+0x1234],15
;### Rotate right with carry
        rcr     ch,2
        rcr     byte ptr [si],3
        rcr     byte ptr [0x1234],4
        rcr     byte ptr [di-0x34],5
        rcr     byte ptr [bp+0x1234],6
        rcr     byte ptr [bx+si],7
        rcr     byte ptr [bx+di+0x34],0
        rcr     byte ptr [bp+si+0x1234],2
        rcr     bp,8
        rcr     word ptr [si],9
        rcr     word ptr [0x1234],10
        rcr     word ptr [di-0x34],11
        rcr     word ptr [bp+0x1234],12
        rcr     word ptr [bx+si],13
        rcr     word ptr [bx+di+0x34],14
        rcr     word ptr [bp+si+0x1234],15

;### String manipulation
;### Repeat
        rep     insb
        rep     insw
        rep     outsb
        rep     outsw
;### Repeat While Equal/Zero
        repe    insb
        repe    insw
        repe    outsb
        repe    outsw
        repz    insb
        repz    insw
        repz    outsb
        repz    outsw
;### Repeat While Not Equal/Not Zero
        repne   insb
        repne   insw
        repne   outsb
        repne   outsw
        repnz   insb
        repnz   insw
        repnz   outsb
        repnz   outsw
;### Input byte string
        insb
;### Input word string
        insw
;### Output string byte
        outsb
;### Output string word
        outsw

;### Control Transfer
;### Enter Procedure
        enter   30, 1
        enter   65535, 0
        enter   0, 255
;### Leave Procedure
        leave
;### Detect value out of range
        bound   si, [si]
        bound   di, [1234h]
        bound   ax, [di+52]
        bound   cx, [bp+1234h]
        bound   dx, [bx+si]
        bound   bx, [bx+di+52]
        bound   sp, [bp+si+1234h]

;### local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
