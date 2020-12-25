### Copyright 2020 Tadashi G. Takaoka
###
### Licensed under the Apache License, Version 2.0 (the "License")#
### you may not use this file except in compliance with the License.
### You may obtain a copy of the License at
###
###     http://www.apache.org/licenses/LICENSE-2.0
###
### Unless required by applicable law or agreed to in writing, software
### distributed under the License is distributed on an "AS IS" BASIS,
### WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
### See the License for the specific language governing permissions and
### limitations under the License.

        .code16

### Data Transfer
### Move
        mov     al,cl
        mov     dl,bl
        mov     ah,ch
        mov     [si],dh
        mov     [0x1234],bh
        mov     [di-0x34],al
        mov     [bp+0x1234],cl
        mov     [bx+si],dl
        mov     [bx+di+0x34],bl
        mov     [bp+si+0x1234],ah
        mov     dh,[si]
        mov     bh,[0x1234]
        mov     al,[di-0x34]
        mov     cl,[bp+0x1234]
        mov     dl,[bx+si]
        mov     bl,[bx+di+0x34]
        mov     ah,[bp+si+0x1234]
        mov     ax,cx
        mov     dx,bx
        mov     sp,bp
        mov     [si],si
        mov     [0x1234],di
        mov     [di-0x34],ax
        mov     [bp+0x1234],cx
        mov     [bx+si],dx
        mov     [bx+di+0x34],bx
        mov     [bp+si+0x1234],sp
        mov     si,[si]
        mov     di,[0x1234]
        mov     ax,[di-0x34]
        mov     cx,[bp+0x1234]
        mov     dx,[bx+si]
        mov     bx,[bx+di+0x34]
        mov     sp,[bp+si+0x1234]
        mov     al,0x56
        mov     byte ptr [si],0x56
        mov     byte ptr [0x89ab],0x56
        mov     byte ptr [di-0x34],0x56
        mov     byte ptr [bp+0x89ab],0x56
        mov     byte ptr [bx+si],0x56
        mov     byte ptr [bx+di+0x34],0x56
        mov     byte ptr [bp+si+0x1234],0x56
        mov     ax,0x5678
        mov     word ptr [si],0x5678
        mov     word ptr [0x1234],0x5678
        mov     word ptr [di+0x34],0x5678
        mov     word ptr [bp+0x1234],0x5678
        mov     word ptr [bx+si],0x5678
        mov     word ptr [bx+di-0x34],0x5678
        mov     word ptr [bp+si+0x89ab],0x5678
        mov     al,[0x1234]
        mov     ax,[0x1234]
        mov     [0x1234],al
        mov     [0x1234],ax
        mov     ax,es
        mov     [si],cs
        mov     [0x89ab],ss
        mov     [di-0x34],ds
        mov     [bp+0x1234],es
        mov     [bx+si],cs
        mov     [bx+di+0x34],ss
        mov     [bp+si+0x1234],ds
        mov     es,ax
        mov     cs,[si]
        mov     ss,[0x1234]
        mov     ds,[di-0x34]
        mov     es,[bp+0x1234]
        mov     cs,[bx+si]
        mov     ss,[bx+di+0x34]
        mov     ds,[bp+si+0x1234]
### Push data onto stack
        push    bp
        push    [si]
        push    [0x1234]
        push    [di-0x34]
        push    [bp+0x1234]
        push    [bx+si]
        push    [bx+di+0x34]
        push    [bp+si+0x1234]
        push    es
        push    cs
        push    ss
        push    ds
### Pop data from stack
        pop     bp
        pop     [si]
        pop     [0x1234]
        pop     [di-0x34]
        pop     [bp+0x1234]
        pop     [bx+si]
        pop     [bx+di+0x34]
        pop     [bp+si+0x1234]
        pop     es
#       pop     cs
        pop     ss
        pop     ds
### Exchange data
        xchg    al,ch
        xchg    [si],cl
        xchg    [0x1234],dl
        xchg    [di-0x34],bl
        xchg    [bp+0x1234],ah
        xchg    [bx+si],ch
        xchg    [bx+di+0x34],dh
        xchg    [bp+si+0x1234],bh
        xchg    ax,bp
        xchg    [si],cx
        xchg    [0x1234],dx
        xchg    [di-0x34],bx
        xchg    [bp+0x1234],sp
        xchg    [bx+si],bp
        xchg    [bx+di+0x34],si
        xchg    [bp+si+0x1234],di
### Input from port
        in      al,0x34
        in      al,dx
        in      ax,0x34
        in      ax,dx
### Output to port
        out     0x34,al
        out     dx,al
        out     0x34,ax
        out     dx,ax
### Table look-up translation
        xlat
### Load effective address
        lea     cx,[si]
        lea     dx,[0x1234]
        lea     bx,[di-0x34]
        lea     sp,[bp+0x1234]
        lea     bp,[bx+si]
        lea     si,[bx+di+0x34]
        lea     di,[bp+si+0x1234]
### Load pointer using DS
        lds     cx,[si]
        lds     dx,[0x1234]
        lds     bx,[di-0x34]
        lds     sp,[bp+0x1234]
        lds     bp,[bx+si]
        lds     si,[bx+di+0x34]
        lds     di,[bp+si+0x1234]
### Load ES with pointer
        les     cx,[si]
        les     dx,[0x1234]
        les     bx,[di-0x34]
        les     sp,[bp+0x1234]
        les     bp,[bx+si]
        les     si,[bx+di+0x34]
        les     di,[bp+si+0x1234]
### Load FLAGS into 0xA register
        lahf
### Store 0xA into FLAGS
        sahf
### Push FLAGS register onto stack
        pushf
### Pop FLAGS register from stack
        popf

### Arithmetic
### Add
        add     al,cl
        add     dl,bl
        add     ah,ch
        add     [si],dh
        add     [0x1234],bh
        add     [di-0x34],al
        add     [bp+0x1234],cl
        add     [bx+si],dl
        add     [bx+di+0x34],bl
        add     [bp+si+0x1234],ah
        add     dh,[si]
        add     bh,[0x1234]
        add     al,[di-0x34]
        add     cl,[bp+0x1234]
        add     dl,[bx+si]
        add     bl,[bx+di+0x34]
        add     ah,[bp+si+0x1234]
        add     ax,cx
        add     dx,bx
        add     sp,bp
        add     [si],si
        add     [0x1234],di
        add     [di-0x34],ax
        add     [bp+0x1234],cx
        add     [bx+si],dx
        add     [bx+di+0x34],bx
        add     [bp+si+0x1234],sp
        add     si,[si]
        add     di,[0x1234]
        add     ax,[di-0x34]
        add     cx,[bp+0x1234]
        add     dx,[bx+si]
        add     bx,[bx+di+0x34]
        add     sp,[bp+si+0x1234]
        add     al,0x56
        add     cl,0x56
        add     byte ptr [si],0x56
        add     byte ptr [0x1234],0x56
        add     byte ptr [di-0x34],0x56
        add     byte ptr [bp+0x1234],0x56
        add     byte ptr [bx+si],0x56
        add     byte ptr [bx+di+0x34],0x56
        add     byte ptr [bp+si+0x1234],0x56
        add     ax,0x5678
        add     cx,0x5678
        add     word ptr [si],0x5678
        add     word ptr [0x1234],0x5678
        add     word ptr [di-0x34],0x5678
        add     word ptr [bp+0x1234],0x5678
        add     word ptr [bx+si],0x5678
        add     word ptr [bx+di+0x34],0x5678
        add     word ptr [bp+si+0x1234],0x5678
        add     ax,0x56
        add     ax,-16
        add     cx,0x56
        add     cx,-16
        add     word ptr [si],0x56
        add     word ptr [0x1234],0x56
        add     word ptr [di-0x34],0x56
        add     word ptr [bp+0x1234],0x56
        add     word ptr [bx+si],0x56
        add     word ptr [bx+di+0x34],0x56
        add     word ptr [bp+si+0x1234],0x56
### Add with carry
        adc     al,cl
        adc     dl,bl
        adc     ah,ch
        adc     [si],dh
        adc     [0x1234],bh
        adc     [di-0x34],al
        adc     [bp+0x1234],cl
        adc     [bx+si],dl
        adc     [bx+di+0x34],bl
        adc     [bp+si+0x1234],ah
        adc     dh,[si]
        adc     bh,[0x1234]
        adc     al,[di-0x34]
        adc     cl,[bp+0x1234]
        adc     dl,[bx+si]
        adc     bl,[bx+di+0x34]
        adc     ah,[bp+si+0x1234]
        adc     ax,cx
        adc     dx,bx
        adc     sp,bp
        adc     [si],si
        adc     [0x1234],di
        adc     [di-0x34],ax
        adc     [bp+0x1234],cx
        adc     [bx+si],dx
        adc     [bx+di+0x34],bx
        adc     [bp+si+0x1234],sp
        adc     si,[si]
        adc     di,[0x1234]
        adc     ax,[di-0x34]
        adc     cx,[bp+0x1234]
        adc     dx,[bx+si]
        adc     bx,[bx+di+0x34]
        adc     sp,[bp+si+0x1234]
        adc     al,0x56
        adc     cl,0x56
        adc     byte ptr [si],0x56
        adc     byte ptr [0x1234],0x56
        adc     byte ptr [di-0x34],0x56
        adc     byte ptr [bp+0x1234],0x56
        adc     byte ptr [bx+si],0x56
        adc     byte ptr [bx+di+0x34],0x56
        adc     byte ptr [bp+si+0x1234],0x56
        adc     ax,0x5678
        adc     cx,0x5678
        adc     word ptr [si],0x5678
        adc     word ptr [0x1234],0x5678
        adc     word ptr [di-0x34],0x5678
        adc     word ptr [bp+0x1234],0x5678
        adc     word ptr [bx+si],0x5678
        adc     word ptr [bx+di+0x34],0x5678
        adc     word ptr [bp+si+0x1234],0x5678
        adc     ax,0x56
        adc     ax,-16
        adc     cx,0x56
        adc     cx,-16
        adc     word ptr [si],0x56
        adc     word ptr [0x1234],0x56
        adc     word ptr [di-0x34],0x56
        adc     word ptr [bp+0x1234],0x56
        adc     word ptr [bx+si],0x56
        adc     word ptr [bx+di+0x34],0x56
        adc     word ptr [bp+si+0x1234],0x56
### Increment by 1
        inc     ch
        inc     byte ptr [si]
        inc     byte ptr [0x1234]
        inc     byte ptr [di-0x34]
        inc     byte ptr [bp+0x1234]
        inc     byte ptr [bx+si]
        inc     byte ptr [bx+di+0x34]
        inc     byte ptr [bp+si+0x1234]
        inc     bp
        inc     word ptr [si]
        inc     word ptr [0x1234]
        inc     word ptr [di-0x34]
        inc     word ptr [bp+0x1234]
        inc     word ptr [bx+si]
        inc     word ptr [bx+di+0x34]
        inc     word ptr [bp+si+0x1234]
### ASCII adjust AL after addition
        aaa
### Decimal adjust AL after addition
        daa
### Subtraction
        sub     al,cl
        sub     dl,bl
        sub     ah,ch
        sub     [si],dh
        sub     [0x1234],bh
        sub     [di-0x34],al
        sub     [bp+0x1234],cl
        sub     [bx+si],dl
        sub     [bx+di+0x34],bl
        sub     [bp+si+0x1234],ah
        sub     dh,[si]
        sub     bh,[0x1234]
        sub     al,[di-0x34]
        sub     cl,[bp+0x1234]
        sub     dl,[bx+si]
        sub     bl,[bx+di+0x34]
        sub     ah,[bp+si+0x1234]
        sub     ax,cx
        sub     dx,bx
        sub     sp,bp
        sub     [si],si
        sub     [0x1234],di
        sub     [di-0x34],ax
        sub     [bp+0x1234],cx
        sub     [bx+si],dx
        sub     [bx+di+0x34],bx
        sub     [bp+si+0x1234],sp
        sub     si,[si]
        sub     di,[0x1234]
        sub     ax,[di-0x34]
        sub     cx,[bp+0x1234]
        sub     dx,[bx+si]
        sub     bx,[bx+di+0x34]
        sub     sp,[bp+si+0x1234]
        sub     al,0x56
        sub     cl,0x56
        sub     byte ptr [si],0x56
        sub     byte ptr [0x1234],0x56
        sub     byte ptr [di-0x34],0x56
        sub     byte ptr [bp+0x1234],0x56
        sub     byte ptr [bx+si],0x56
        sub     byte ptr [bx+di+0x34],0x56
        sub     byte ptr [bp+si+0x1234],0x56
        sub     ax,0x5678
        sub     cx,0x5678
        sub     word ptr [si],0x5678
        sub     word ptr [0x1234],0x5678
        sub     word ptr [di-0x34],0x5678
        sub     word ptr [bp+0x1234],0x5678
        sub     word ptr [bx+si],0x5678
        sub     word ptr [bx+di+0x34],0x5678
        sub     word ptr [bp+si+0x1234],0x5678
        sub     ax,0x56
        sub     ax,-16
        sub     cx,0x56
        sub     cx,-16
        sub     word ptr [si],0x56
        sub     word ptr [0x1234],0x56
        sub     word ptr [di-0x34],0x56
        sub     word ptr [bp+0x1234],0x56
        sub     word ptr [bx+si],0x56
        sub     word ptr [bx+di+0x34],0x56
        sub     word ptr [bp+si+0x1234],0x56
### Subtract with borrow
        sbb     al,cl
        sbb     dl,bl
        sbb     ah,ch
        sbb     [si],dh
        sbb     [0x1234],bh
        sbb     [di-0x34],al
        sbb     [bp+0x1234],cl
        sbb     [bx+si],dl
        sbb     [bx+di+0x34],bl
        sbb     [bp+si+0x1234],ah
        sbb     dh,[si]
        sbb     bh,[0x1234]
        sbb     al,[di-0x34]
        sbb     cl,[bp+0x1234]
        sbb     dl,[bx+si]
        sbb     bl,[bx+di+0x34]
        sbb     ah,[bp+si+0x1234]
        sbb     ax,cx
        sbb     dx,bx
        sbb     sp,bp
        sbb     [si],si
        sbb     [0x1234],di
        sbb     [di-0x34],ax
        sbb     [bp+0x1234],cx
        sbb     [bx+si],dx
        sbb     [bx+di+0x34],bx
        sbb     [bp+si+0x1234],sp
        sbb     si,[si]
        sbb     di,[0x1234]
        sbb     ax,[di-0x34]
        sbb     cx,[bp+0x1234]
        sbb     dx,[bx+si]
        sbb     bx,[bx+di+0x34]
        sbb     sp,[bp+si+0x1234]
        sbb     al,0x56
        sbb     cl,0x56
        sbb     byte ptr [si],0x56
        sbb     byte ptr [0x1234],0x56
        sbb     byte ptr [di-0x34],0x56
        sbb     byte ptr [bp+0x1234],0x56
        sbb     byte ptr [bx+si],0x56
        sbb     byte ptr [bx+di+0x34],0x56
        sbb     byte ptr [bp+si+0x1234],0x56
        sbb     ax,0x5678
        sbb     cx,0x5678
        sbb     word ptr [si],0x5678
        sbb     word ptr [0x1234],0x5678
        sbb     word ptr [di-0x34],0x5678
        sbb     word ptr [bp+0x1234],0x5678
        sbb     word ptr [bx+si],0x5678
        sbb     word ptr [bx+di+0x34],0x5678
        sbb     word ptr [bp+si+0x1234],0x5678
        sbb     ax,0x56
        sbb     ax,-16
        sbb     cx,0x56
        sbb     cx,-16
        sbb     word ptr [si],0x56
        sbb     word ptr [0x1234],0x56
        sbb     word ptr [di-0x34],0x56
        sbb     word ptr [bp+0x1234],0x56
        sbb     word ptr [bx+si],0x56
        sbb     word ptr [bx+di+0x34],0x56
        sbb     word ptr [bp+si+0x1234],0x56
### Decrement by 1
        dec     ch
        dec     byte ptr [si]
        dec     byte ptr [0x1234]
        dec     byte ptr [di-0x34]
        dec     byte ptr [bp+0x1234]
        dec     byte ptr [bx+si]
        dec     byte ptr [bx+di+0x34]
        dec     byte ptr [bp+si+0x1234]
        dec     bp
        dec     word ptr [si]
        dec     word ptr [0x1234]
        dec     word ptr [di-0x34]
        dec     word ptr [bp+0x1234]
        dec     word ptr [bx+si]
        dec     word ptr [bx+di+0x34]
        dec     word ptr [bp+si+0x1234]
### Negation
        neg     ch
        neg     byte ptr [si]
        neg     byte ptr [0x1234]
        neg     byte ptr [di-0x34]
        neg     byte ptr [bp+0x1234]
        neg     byte ptr [bx+si]
        neg     byte ptr [bx+di+0x34]
        neg     byte ptr [bp+si+0x1234]
        neg     bp
        neg     word ptr [si]
        neg     word ptr [0x1234]
        neg     word ptr [di-0x34]
        neg     word ptr [bp+0x1234]
        neg     word ptr [bx+si]
        neg     word ptr [bx+di+0x34]
        neg     word ptr [bp+si+0x1234]
### Compare operands
        cmp     al,cl
        cmp     dl,bl
        cmp     ah,ch
        cmp     [si],dh
        cmp     [0x1234],bh
        cmp     [di-0x34],al
        cmp     [bp+0x1234],cl
        cmp     [bx+si],dl
        cmp     [bx+di+0x34],bl
        cmp     [bp+si+0x1234],ah
        cmp     dh,[si]
        cmp     bh,[0x1234]
        cmp     al,[di-0x34]
        cmp     cl,[bp+0x1234]
        cmp     dl,[bx+si]
        cmp     bl,[bx+di+0x34]
        cmp     ah,[bp+si+0x1234]
        cmp     ax,cx
        cmp     dx,bx
        cmp     sp,bp
        cmp     [si],si
        cmp     [0x1234],di
        cmp     [di-0x34],ax
        cmp     [bp+0x1234],cx
        cmp     [bx+si],dx
        cmp     [bx+di+0x34],bx
        cmp     [bp+si+0x1234],sp
        cmp     si,[si]
        cmp     di,[0x1234]
        cmp     ax,[di-0x34]
        cmp     cx,[bp+0x1234]
        cmp     dx,[bx+si]
        cmp     bx,[bx+di+0x34]
        cmp     sp,[bp+si+0x1234]
        cmp     al,0x56
        cmp     cl,0x56
        cmp     byte ptr [si],0x56
        cmp     byte ptr [0x1234],0x56
        cmp     byte ptr [di-0x34],0x56
        cmp     byte ptr [bp+0x1234],0x56
        cmp     byte ptr [bx+si],0x56
        cmp     byte ptr [bx+di+0x34],0x56
        cmp     byte ptr [bp+si+0x1234],0x56
        cmp     ax,0x5678
        cmp     cx,0x5678
        cmp     word ptr [si],0x5678
        cmp     word ptr [0x1234],0x5678
        cmp     word ptr [di-0x34],0x5678
        cmp     word ptr [bp+0x1234],0x5678
        cmp     word ptr [bx+si],0x5678
        cmp     word ptr [bx+di+0x34],0x5678
        cmp     word ptr [bp+si+0x1234],0x5678
        cmp     ax,0x56
        cmp     ax,-16
        cmp     cx,0x56
        cmp     cx,-16
        cmp     word ptr [si],0x56
        cmp     word ptr [0x1234],0x56
        cmp     word ptr [di-0x34],0x56
        cmp     word ptr [bp+0x1234],0x56
        cmp     word ptr [bx+si],0x56
        cmp     word ptr [bx+di+0x34],0x56
        cmp     word ptr [bp+si+0x1234],0x56
### ASCII adjust AL after subtraction
        aas
### Decimal adjust AL after subtraction
        das
### Unsigned multiply
        mul     ch
        mul     byte ptr [si]
        mul     byte ptr [0x1234]
        mul     byte ptr [di-0x34]
        mul     byte ptr [bp+0x1234]
        mul     byte ptr [bx+si]
        mul     byte ptr [bx+di+0x34]
        mul     byte ptr [bp+si+0x1234]
        mul     bp
        mul     word ptr [si]
        mul     word ptr [0x1234]
        mul     word ptr [di-0x34]
        mul     word ptr [bp+0x1234]
        mul     word ptr [bx+si]
        mul     word ptr [bx+di+0x34]
        mul     word ptr [bp+si+0x1234]
### Signed multiply
        imul    ch
        imul    byte ptr [si]
        imul    byte ptr [0x1234]
        imul    byte ptr [di-0x34]
        imul    byte ptr [bp+0x1234]
        imul    byte ptr [bx+si]
        imul    byte ptr [bx+di+0x34]
        imul    byte ptr [bp+si+0x1234]
        imul    bp
        imul    word ptr [si]
        imul    word ptr [0x1234]
        imul    word ptr [di-0x34]
        imul    word ptr [bp+0x1234]
        imul    word ptr [bx+si]
        imul    word ptr [bx+di+0x34]
        imul    word ptr [bp+si+0x1234]
### ASCII adjust AX afrer multiplication
        aam
### Unsigned divide
        div     ch
        div     byte ptr [si]
        div     byte ptr [0x1234]
        div     byte ptr [di-0x34]
        div     byte ptr [bp+0x1234]
        div     byte ptr [bx+si]
        div     byte ptr [bx+di+0x34]
        div     byte ptr [bp+si+0x1234]
        div     bp
        div     word ptr [si]
        div     word ptr [0x1234]
        div     word ptr [di-0x34]
        div     word ptr [bp+0x1234]
        div     word ptr [bx+si]
        div     word ptr [bx+di+0x34]
        div     word ptr [bp+si+0x1234]
### Signed divide
        idiv    ch
        idiv    byte ptr [si]
        idiv    byte ptr [0x1234]
        idiv    byte ptr [di-0x34]
        idiv    byte ptr [bp+0x1234]
        idiv    byte ptr [bx+si]
        idiv    byte ptr [bx+di+0x34]
        idiv    byte ptr [bp+si+0x1234]
        idiv    bp
        idiv    word ptr [si]
        idiv    word ptr [0x1234]
        idiv    word ptr [di-0x34]
        idiv    word ptr [bp+0x1234]
        idiv    word ptr [bx+si]
        idiv    word ptr [bx+di+0x34]
        idiv    word ptr [bp+si+0x1234]
### ASCII adjust AX before division
        aad
### Convert byte to word
        cbw
### Convert word to double word
        cwd

### Logic
### Negate the operand, logical NOT
        not     ch
        not     byte ptr [si]
        not     byte ptr [0x1234]
        not     byte ptr [di-0x34]
        not     byte ptr [bp+0x1234]
        not     byte ptr [bx+si]
        not     byte ptr [bx+di+0x34]
        not     byte ptr [bp+si+0x1234]
        not     bp
        not     word ptr [si]
        not     word ptr [0x1234]
        not     word ptr [di-0x34]
        not     word ptr [bp+0x1234]
        not     word ptr [bx+si]
        not     word ptr [bx+di+0x34]
        not     word ptr [bp+si+0x1234]
### Shift left, unsigned shift left
        shl     ch,1
        shl     byte ptr [si],1
        shl     byte ptr [0x1234],1
        shl     byte ptr [di-0x34],1
        shl     byte ptr [bp+0x1234],1
        shl     byte ptr [bx+si],1
        shl     byte ptr [bx+di+0x34],1
        shl     byte ptr [bp+si+0x1234],1
        shl     bp,1
        shl     word ptr [si],1
        shl     word ptr [0x1234],1
        shl     word ptr [di-0x34],1
        shl     word ptr [bp+0x1234],1
        shl     word ptr [bx+si],1
        shl     word ptr [bx+di+0x34],1
        shl     word ptr [bp+si+0x1234],1
        shl     ch,cl
        shl     byte ptr [si],cl
        shl     byte ptr [0x1234],cl
        shl     byte ptr [di-0x34],cl
        shl     byte ptr [bp+0x1234],cl
        shl     byte ptr [bx+si],cl
        shl     byte ptr [bx+di+0x34],cl
        shl     byte ptr [bp+si+0x1234],cl
        shl     bp,cl
        shl     word ptr [si],cl
        shl     word ptr [0x1234],cl
        shl     word ptr [di-0x34],cl
        shl     word ptr [bp+0x1234],cl
        shl     word ptr [bx+si],cl
        shl     word ptr [bx+di+0x34],cl
        shl     word ptr [bp+si+0x1234],cl
### Shift arithmetically left, signed shift left
        sal     ch,1
        sal     byte ptr [si],1
        sal     byte ptr [0x1234],1
        sal     byte ptr [di-0x34],1
        sal     byte ptr [bp+0x1234],1
        sal     byte ptr [bx+si],1
        sal     byte ptr [bx+di+0x34],1
        sal     byte ptr [bp+si+0x1234],1
        sal     bp,1
        sal     word ptr [si],1
        sal     word ptr [0x1234],1
        sal     word ptr [di-0x34],1
        sal     word ptr [bp+0x1234],1
        sal     word ptr [bx+si],1
        sal     word ptr [bx+di+0x34],1
        sal     word ptr [bp+si+0x1234],1
        sal     ch,cl
        sal     byte ptr [si],cl
        sal     byte ptr [0x1234],cl
        sal     byte ptr [di-0x34],cl
        sal     byte ptr [bp+0x1234],cl
        sal     byte ptr [bx+si],cl
        sal     byte ptr [bx+di+0x34],cl
        sal     byte ptr [bp+si+0x1234],cl
        sal     bp,cl
        sal     word ptr [si],cl
        sal     word ptr [0x1234],cl
        sal     word ptr [di-0x34],cl
        sal     word ptr [bp+0x1234],cl
        sal     word ptr [bx+si],cl
        sal     word ptr [bx+di+0x34],cl
        sal     word ptr [bp+si+0x1234],cl
### Shift right, unsigned shift right
        shr     ch,1
        shr     byte ptr [si],1
        shr     byte ptr [0x1234],1
        shr     byte ptr [di-0x34],1
        shr     byte ptr [bp+0x1234],1
        shr     byte ptr [bx+si],1
        shr     byte ptr [bx+di+0x34],1
        shr     byte ptr [bp+si+0x1234],1
        shr     bp,1
        shr     word ptr [si],1
        shr     word ptr [0x1234],1
        shr     word ptr [di-0x34],1
        shr     word ptr [bp+0x1234],1
        shr     word ptr [bx+si],1
        shr     word ptr [bx+di+0x34],1
        shr     word ptr [bp+si+0x1234],1
        shr     ch,cl
        shr     byte ptr [si],cl
        shr     byte ptr [0x1234],cl
        shr     byte ptr [di-0x34],cl
        shr     byte ptr [bp+0x1234],cl
        shr     byte ptr [bx+si],cl
        shr     byte ptr [bx+di+0x34],cl
        shr     byte ptr [bp+si+0x1234],cl
        shr     bp,cl
        shr     word ptr [si],cl
        shr     word ptr [0x1234],cl
        shr     word ptr [di-0x34],cl
        shr     word ptr [bp+0x1234],cl
        shr     word ptr [bx+si],cl
        shr     word ptr [bx+di+0x34],cl
        shr     word ptr [bp+si+0x1234],cl
### Shift arithmetically right, signed shift right
        sar     ch,1
        sar     byte ptr [si],1
        sar     byte ptr [0x1234],1
        sar     byte ptr [di-0x34],1
        sar     byte ptr [bp+0x1234],1
        sar     byte ptr [bx+si],1
        sar     byte ptr [bx+di+0x34],1
        sar     byte ptr [bp+si+0x1234],1
        sar     bp,1
        sar     word ptr [si],1
        sar     word ptr [0x1234],1
        sar     word ptr [di-0x34],1
        sar     word ptr [bp+0x1234],1
        sar     word ptr [bx+si],1
        sar     word ptr [bx+di+0x34],1
        sar     word ptr [bp+si+0x1234],1
        sar     ch,cl
        sar     byte ptr [si],cl
        sar     byte ptr [0x1234],cl
        sar     byte ptr [di-0x34],cl
        sar     byte ptr [bp+0x1234],cl
        sar     byte ptr [bx+si],cl
        sar     byte ptr [bx+di+0x34],cl
        sar     byte ptr [bp+si+0x1234],cl
        sar     bp,cl
        sar     word ptr [si],cl
        sar     word ptr [0x1234],cl
        sar     word ptr [di-0x34],cl
        sar     word ptr [bp+0x1234],cl
        sar     word ptr [bx+si],cl
        sar     word ptr [bx+di+0x34],cl
        sar     word ptr [bp+si+0x1234],cl
### Rotate left
        rol     ch,1
        rol     byte ptr [si],1
        rol     byte ptr [0x1234],1
        rol     byte ptr [di-0x34],1
        rol     byte ptr [bp+0x1234],1
        rol     byte ptr [bx+si],1
        rol     byte ptr [bx+di+0x34],1
        rol     byte ptr [bp+si+0x1234],1
        rol     bp,1
        rol     word ptr [si],1
        rol     word ptr [0x1234],1
        rol     word ptr [di-0x34],1
        rol     word ptr [bp+0x1234],1
        rol     word ptr [bx+si],1
        rol     word ptr [bx+di+0x34],1
        rol     word ptr [bp+si+0x1234],1
        rol     ch,cl
        rol     byte ptr [si],cl
        rol     byte ptr [0x1234],cl
        rol     byte ptr [di-0x34],cl
        rol     byte ptr [bp+0x1234],cl
        rol     byte ptr [bx+si],cl
        rol     byte ptr [bx+di+0x34],cl
        rol     byte ptr [bp+si+0x1234],cl
        rol     bp,cl
        rol     word ptr [si],cl
        rol     word ptr [0x1234],cl
        rol     word ptr [di-0x34],cl
        rol     word ptr [bp+0x1234],cl
        rol     word ptr [bx+si],cl
        rol     word ptr [bx+di+0x34],cl
        rol     word ptr [bp+si+0x1234],cl
### Rotate right
        ror     ch,1
        ror     byte ptr [si],1
        ror     byte ptr [0x1234],1
        ror     byte ptr [di-0x34],1
        ror     byte ptr [bp+0x1234],1
        ror     byte ptr [bx+si],1
        ror     byte ptr [bx+di+0x34],1
        ror     byte ptr [bp+si+0x1234],1
        ror     bp,1
        ror     word ptr [si],1
        ror     word ptr [0x1234],1
        ror     word ptr [di-0x34],1
        ror     word ptr [bp+0x1234],1
        ror     word ptr [bx+si],1
        ror     word ptr [bx+di+0x34],1
        ror     word ptr [bp+si+0x1234],1
        ror     ch,cl
        ror     byte ptr [si],cl
        ror     byte ptr [0x1234],cl
        ror     byte ptr [di-0x34],cl
        ror     byte ptr [bp+0x1234],cl
        ror     byte ptr [bx+si],cl
        ror     byte ptr [bx+di+0x34],cl
        ror     byte ptr [bp+si+0x1234],cl
        ror     bp,cl
        ror     word ptr [si],cl
        ror     word ptr [0x1234],cl
        ror     word ptr [di-0x34],cl
        ror     word ptr [bp+0x1234],cl
        ror     word ptr [bx+si],cl
        ror     word ptr [bx+di+0x34],cl
        ror     word ptr [bp+si+0x1234],cl
### Rotate left with carry
        rcl     ch,1
        rcl     byte ptr [si],1
        rcl     byte ptr [0x1234],1
        rcl     byte ptr [di-0x34],1
        rcl     byte ptr [bp+0x1234],1
        rcl     byte ptr [bx+si],1
        rcl     byte ptr [bx+di+0x34],1
        rcl     byte ptr [bp+si+0x1234],1
        rcl     bp,1
        rcl     word ptr [si],1
        rcl     word ptr [0x1234],1
        rcl     word ptr [di-0x34],1
        rcl     word ptr [bp+0x1234],1
        rcl     word ptr [bx+si],1
        rcl     word ptr [bx+di+0x34],1
        rcl     word ptr [bp+si+0x1234],1
        rcl     ch,cl
        rcl     byte ptr [si],cl
        rcl     byte ptr [0x1234],cl
        rcl     byte ptr [di-0x34],cl
        rcl     byte ptr [bp+0x1234],cl
        rcl     byte ptr [bx+si],cl
        rcl     byte ptr [bx+di+0x34],cl
        rcl     byte ptr [bp+si+0x1234],cl
        rcl     bp,cl
        rcl     word ptr [si],cl
        rcl     word ptr [0x1234],cl
        rcl     word ptr [di-0x34],cl
        rcl     word ptr [bp+0x1234],cl
        rcl     word ptr [bx+si],cl
        rcl     word ptr [bx+di+0x34],cl
        rcl     word ptr [bp+si+0x1234],cl
### Rotate right with carry
        rcr     ch,1
        rcr     byte ptr [si],1
        rcr     byte ptr [0x1234],1
        rcr     byte ptr [di-0x34],1
        rcr     byte ptr [bp+0x1234],1
        rcr     byte ptr [bx+si],1
        rcr     byte ptr [bx+di+0x34],1
        rcr     byte ptr [bp+si+0x1234],1
        rcr     bp,1
        rcr     word ptr [si],1
        rcr     word ptr [0x1234],1
        rcr     word ptr [di-0x34],1
        rcr     word ptr [bp+0x1234],1
        rcr     word ptr [bx+si],1
        rcr     word ptr [bx+di+0x34],1
        rcr     word ptr [bp+si+0x1234],1
        rcr     ch,cl
        rcr     byte ptr [si],cl
        rcr     byte ptr [0x1234],cl
        rcr     byte ptr [di-0x34],cl
        rcr     byte ptr [bp+0x1234],cl
        rcr     byte ptr [bx+si],cl
        rcr     byte ptr [bx+di+0x34],cl
        rcr     byte ptr [bp+si+0x1234],cl
        rcr     bp,cl
        rcr     word ptr [si],cl
        rcr     word ptr [0x1234],cl
        rcr     word ptr [di-0x34],cl
        rcr     word ptr [bp+0x1234],cl
        rcr     word ptr [bx+si],cl
        rcr     word ptr [bx+di+0x34],cl
        rcr     word ptr [bp+si+0x1234],cl
### Logical AND
        and     al,cl
        and     dl,bl
        and     ah,ch
        and     [si],dh
        and     [0x1234],bh
        and     [di-0x34],al
        and     [bp+0x1234],cl
        and     [bx+si],dl
        and     [bx+di+0x34],bl
        and     [bp+si+0x1234],ah
        and     dh,[si]
        and     bh,[0x1234]
        and     al,[di-0x34]
        and     cl,[bp+0x1234]
        and     dl,[bx+si]
        and     bl,[bx+di+0x34]
        and     ah,[bp+si+0x1234]
        and     ax,cx
        and     dx,bx
        and     sp,bp
        and     [si],si
        and     [0x1234],di
        and     [di-0x34],ax
        and     [bp+0x1234],cx
        and     [bx+si],dx
        and     [bx+di+0x34],bx
        and     [bp+si+0x1234],sp
        and     si,[si]
        and     di,[0x1234]
        and     ax,[di-0x34]
        and     cx,[bp+0x1234]
        and     dx,[bx+si]
        and     bx,[bx+di+0x34]
        and     sp,[bp+si+0x1234]
        and     al,0x56
        and     cl,0x56
        and     byte ptr [si],0x56
        and     byte ptr [0x1234],0x56
        and     byte ptr [di-0x34],0x56
        and     byte ptr [bp+0x1234],0x56
        and     byte ptr [bx+si],0x56
        and     byte ptr [bx+di+0x34],0x56
        and     byte ptr [bp+si+0x1234],0x56
        and     ax,0x5678
        and     ax,-16
        and     cx,0x5678
        and     cx,-16
        and     word ptr [si],0x5678
        and     word ptr [0x1234],0x5678
        and     word ptr [di-0x34],0x5678
        and     word ptr [bp+0x1234],0x5678
        and     word ptr [bx+si],0x5678
        and     word ptr [bx+di+0x34],0x5678
        and     word ptr [bp+si+0x1234],0x5678
### Logical compare
        test    al,cl
        test    dl,bl
        test    ah,ch
        test    dh,[si]
        test    bh,[0x1234]
        test    al,[di-0x34]
        test    cl,[bp+0x1234]
        test    dl,[bx+si]
        test    bl,[bx+di+0x34]
        test    ah,[bp+si+0x1234]
        test    [si],dh
        test    [0x1234],bh
        test    [di-0x34],al
        test    [bp+0x1234],cl
        test    [bx+si],dl
        test    [bx+di+0x34],bl
        test    [bp+si+0x1234],ah
        test    ax,cx
        test    dx,bx
        test    sp,bp
        test    si,[si]
        test    di,[0x1234]
        test    ax,[di-0x34]
        test    cx,[bp+0x1234]
        test    dx,[bx+si]
        test    bx,[bx+di+0x34]
        test    sp,[bp+si+0x1234]
        test    [si],si
        test    [0x1234],di
        test    [di-0x34],ax
        test    [bp+0x1234],cx
        test    [bx+si],dx
        test    [bx+di+0x34],bx
        test    [bp+si+0x1234],sp
        test    al,0x56
        test    cl,0x56
        test    byte ptr [si],0x56
        test    byte ptr [0x1234],0x56
        test    byte ptr [di-0x34],0x56
        test    byte ptr [bp+0x1234],0x56
        test    byte ptr [bx+si],0x56
        test    byte ptr [bx+di+0x34],0x56
        test    byte ptr [bp+si+0x1234],0x56
        test    ax,0x5678
        test    ax,-16
        test    cx,0x5678
        test    cx,-16
        test    word ptr [si],0x5678
        test    word ptr [0x1234],0x5678
        test    word ptr [di-0x34],0x5678
        test    word ptr [bp+0x1234],0x5678
        test    word ptr [bx+si],0x5678
        test    word ptr [bx+di+0x34],0x5678
        test    word ptr [bp+si+0x1234],0x5678
### Logical OR
        or      al,cl
        or      dl,bl
        or      ah,ch
        or      [si],dh
        or      [0x1234],bh
        or      [di-0x34],al
        or      [bp+0x1234],cl
        or      [bx+si],dl
        or      [bx+di+0x34],bl
        or      [bp+si+0x1234],ah
        or      dh,[si]
        or      bh,[0x1234]
        or      al,[di-0x34]
        or      cl,[bp+0x1234]
        or      dl,[bx+si]
        or      bl,[bx+di+0x34]
        or      ah,[bp+si+0x1234]
        or      ax,cx
        or      dx,bx
        or      sp,bp
        or      [si],si
        or      [0x1234],di
        or      [di-0x34],ax
        or      [bp+0x1234],cx
        or      [bx+si],dx
        or      [bx+di+0x34],bx
        or      [bp+si+0x1234],sp
        or      si,[si]
        or      di,[0x1234]
        or      ax,[di-0x34]
        or      cx,[bp+0x1234]
        or      dx,[bx+si]
        or      bx,[bx+di+0x34]
        or      sp,[bp+si+0x1234]
        or      al,0x56
        or      cl,0x56
        or      byte ptr [si],0x56
        or      byte ptr [0x1234],0x56
        or      byte ptr [di-0x34],0x56
        or      byte ptr [bp+0x1234],0x56
        or      byte ptr [bx+si],0x56
        or      byte ptr [bx+di+0x34],0x56
        or      byte ptr [bp+si+0x1234],0x56
        or      ax,0x5678
        or      ax,-16
        or      cx,0x5678
        or      cx,-16
        or      word ptr [si],0x5678
        or      word ptr [0x1234],0x5678
        or      word ptr [di-0x34],0x5678
        or      word ptr [bp+0x1234],0x5678
        or      word ptr [bx+si],0x5678
        or      word ptr [bx+di+0x34],0x5678
        or      word ptr [bp+si+0x1234],0x5678
### Exclusive OR
        xor     al,cl
        xor     dl,bl
        xor     ah,ch
        xor     [si],dh
        xor     [0x1234],bh
        xor     [di-0x34],al
        xor     [bp+0x1234],cl
        xor     [bx+si],dl
        xor     [bx+di+0x34],bl
        xor     [bp+si+0x1234],ah
        xor     dh,[si]
        xor     bh,[0x1234]
        xor     al,[di-0x34]
        xor     cl,[bp+0x1234]
        xor     dl,[bx+si]
        xor     bl,[bx+di+0x34]
        xor     ah,[bp+si+0x1234]
        xor     ax,cx
        xor     dx,bx
        xor     sp,bp
        xor     [si],si
        xor     [0x1234],di
        xor     [di-0x34],ax
        xor     [bp+0x1234],cx
        xor     [bx+si],dx
        xor     [bx+di+0x34],bx
        xor     [bp+si+0x1234],sp
        xor     si,[si]
        xor     di,[0x1234]
        xor     ax,[di-0x34]
        xor     cx,[bp+0x1234]
        xor     dx,[bx+si]
        xor     bx,[bx+di+0x34]
        xor     sp,[bp+si+0x1234]
        xor     al,0x56
        xor     cl,0x56
        xor     byte ptr [si],0x56
        xor     byte ptr [0x1234],0x56
        xor     byte ptr [di-0x34],0x56
        xor     byte ptr [bp+0x1234],0x56
        xor     byte ptr [bx+si],0x56
        xor     byte ptr [bx+di+0x34],0x56
        xor     byte ptr [bp+si+0x1234],0x56
        xor     ax,0x5678
        xor     ax,-16
        xor     cx,0x5678
        xor     cx,-16
        xor     word ptr [si],0x5678
        xor     word ptr [0x1234],0x5678
        xor     word ptr [di-0x34],0x5678
        xor     word ptr [bp+0x1234],0x5678
        xor     word ptr [bx+si],0x5678
        xor     word ptr [bx+di+0x34],0x5678
        xor     word ptr [bp+si+0x1234],0x5678

### String manipulation
### Repeat
        rep     movsb
        rep     movsw
        rep     cmpsb
        rep     cmpsw
        rep     stosb
        rep     stosw
        rep     lodsb
        rep     lodsw
        rep     scasb
        rep     scasw
### Repeat While Equal/Zero
        repe    movsb
        repe    movsw
        repe    cmpsb
        repe    cmpsw
        repe    stosb
        repe    stosw
        repe    lodsb
        repe    lodsw
        repe    scasb
        repe    scasw
        repz    movsb
        repz    movsw
        repz    cmpsb
        repz    cmpsw
        repz    stosb
        repz    stosw
        repz    lodsb
        repz    lodsw
        repz    scasb
        repz    scasw
### Repeat While Not Equal/Not Zero
        repne   movsb
        repne   movsw
        repne   cmpsb
        repne   cmpsw
        repne   stosb
        repne   stosw
        repne   lodsb
        repne   lodsw
        repne   scasb
        repne   scasw
        repnz   movsb
        repnz   movsw
        repnz   cmpsb
        repnz   cmpsw
        repnz   stosb
        repnz   stosw
        repnz   lodsb
        repnz   lodsw
        repnz   scasb
        repnz   scasw
### Move byte from string to string
        movsb
### Move word from string to string
        movsw
### Compare bytes in memory
        cmpsb
### Compare words in memory
        cmpsw
### Compare byte string
        scasb
### Compare word string
        scasw
### Load string byte
        lodsb
### Load string word
        lodsw
### Store byte string
        stosb
### Store word string
        stosw

### Control transfer
### Call procedure
        call    $
        call    ax
        call    si
        call    [si]
        call    [0x1234]
        call    [di-0x34]
        call    [bp+0x1234]
        call    [bx+si]
        call    [bx+di+0x34]
        call    [bp+si+0x1234]
        lcall   0x1234,0x5678
        lcall   [si]
        lcall   [0x1234]
        lcall   [di-0x34]
        lcall   [bp+0x1234]
        lcall   [bx+si]
        lcall   [bx+di+0x34]
        lcall   [bp+si+0x1234]
### Jump
        jmp     $+0x1234
        jmp     $
        jmp     ax
        jmp     si
        jmp     [si]
        jmp     [0x1234]
        jmp     [di-0x34]
        jmp     [bp+0x1234]
        jmp     [bx+si]
        jmp     [bx+di+0x34]
        jmp     [bp+si+0x1234]
        ljmp    0x1234,0x5678
        ljmp    [si]
        ljmp    [0x1234]
        ljmp    [di-0x34]
        ljmp    [bp+0x1234]
        ljmp    [bx+si]
        ljmp    [bx+di+0x34]
        ljmp    [bp+si+0x1234]
### Return from procedure
        ret
        ret     16
        lret
        lret    16
### Jump on Equal/Zero
        je      $
        jz      $
### Jump on Less/Not Greater or Equal
        jl      $
        jnge    $
### Jump on Less or Equal/Not Greater
        jle     $
        jng     $
### Jump on Below/Not Above or Equal/Carry
        jb      $
        jnae    $
        jc      $
### Jump on Below or Equal/Not Above
        jbe     $
        jna     $
### Jump on Parity/Parity Even
        jpe     $
        jp      $
### Jump on Overflow
        jo      $
### Jump on Sign
        js      $
### Jump on Not Equal/Not Zero
        jne     $
        jnz     $
### Jump on Not Less/Greater or Equal
        jge     $
        jnl     $
### Jump on Not Less or Equal/Greater
        jg      $
        jnle    $
### Jump on Not Below/Above or Equal/Not Carry
        jae     $
        jnb     $
        jnc     $
### Jump on Not Below or Equal/Above
        ja      $
        jnbe    $
### Jump on Not Parity/Parity Odd
        jpo     $
        jnp     $
### Jump on Not Overflow
        jno     $
### Jump on Not Sign
        jns     $
### Loop CX Times
        loop    $
### Loop While Zero/Equal
        loope   $
        loopz   $
### Loop While Not Zero/Equal
        loopne  $
        loopnz  $
### Jump if CX is zero
        jcxz    $
### Call interrupt
        int     2
        int     3
### Call interrupt if overflow
        into
### Return from interrupt
        iret

### Processor control
### Clear carry flag
        clc
### Complement carry flag
        cmc
### Set carry flag
        stc
### Clear direction flag
        cld
### Set direction flag
        std
### Clear interrupt flag
        cli
### Set interrupt flag
        sti
### Enter halt state
        hlt
### Wait until not busy
        wait
### Assert BUS LOCK# signal
        lock
### No operation
        nop

### Segment override
#        mov     es:[bx],ah
#        mov     es:[bp],ah
#        mov     es:[si],ah
#        mov     es:[di],ah
#        mov     es:[0x1234],ah
#        mov     cs:[bx],ah
#        mov     cs:[bp],ah
#        mov     cs:[si],ah
#        mov     cs:[di],ah
#        mov     cs:[0x1234],ah
#        mov     ss:[bx],ah
#        mov     ss:[bp],ah
#        mov     ss:[si],ah
#        mov     ss:[di],ah
#        mov     ss:[0x1234],ah
#        mov     ds:[bx],ah
#        mov     ds:[bp],ah
#        mov     ds:[si],ah
#        mov     ds:[di],ah
#        mov     ds:[0x1234],ah
#
#        jmp     es:[si]
#        jmp     cs:[si]
#        jmp     ss:[si]
#        jmp     ds:[si]

### Local Variables:
### mode: asm
### End:
### vim: set ft=asm:
