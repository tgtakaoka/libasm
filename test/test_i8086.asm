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

        cpu     8086
        org     1000H

;;; Data Transfer
;;; Move
        mov     al,cl
        mov     dl,bl
        mov     ah,ch
        mov     [si],dh
        mov     [1234H],bh
        mov     [di-34H],al
        mov     [bp+1234H],cl
        mov     [bx+si],dl
        mov     [bx+di+34H],bl
        mov     [bp+si+1234H],ah
        mov     dh,[si]
        mov     bh,[1234H]
        mov     al,[di-34H]
        mov     cl,[bp+1234H]
        mov     dl,[bx+si]
        mov     bl,[bx+di+34H]
        mov     ah,[bp+si+1234H]
        mov     ax,cx
        mov     dx,bx
        mov     sp,bp
        mov     [si],si
        mov     [1234H],di
        mov     [di-34H],ax
        mov     [bp+1234H],cx
        mov     [bx+si],dx
        mov     [bx+di+34H],bx
        mov     [bp+si+1234H],sp
        mov     si,[si]
        mov     di,[1234H]
        mov     ax,[di-34H]
        mov     cx,[bp+1234H]
        mov     dx,[bx+si]
        mov     bx,[bx+di+34H]
        mov     sp,[bp+si+1234H]
        mov     al,56h
        mov     byte ptr [si],56h
        mov     byte ptr [89abH],56h
        mov     byte ptr [di-34H],56h
        mov     byte ptr [bp+89abH],56h
        mov     byte ptr [bx+si],56h
        mov     byte ptr [bx+di+34H],56h
        mov     byte ptr [bp+si+1234H],56h
        mov     ax,5678h
        mov     word ptr [si],5678h
        mov     word ptr [1234H],5678h
        mov     word ptr [di+34H],5678h
        mov     word ptr [bp+1234H],5678h
        mov     word ptr [bx+si],5678h
        mov     word ptr [bx+di-34H],5678h
        mov     word ptr [bp+si+89abH],5678h
        mov     al,[1234h]
        mov     ax,[1234h]
        mov     [1234h],al
        mov     [1234h],ax
        mov     ax,es
        mov     [si],cs
        mov     [89abH],ss
        mov     [di-34H],ds
        mov     [bp+1234H],es
        mov     [bx+si],cs
        mov     [bx+di+34H],ss
        mov     [bp+si+1234H],ds
        mov     es,ax
        mov     cs,[si]
        mov     ss,[1234H]
        mov     ds,[di-34H]
        mov     es,[bp+1234H]
        mov     cs,[bx+si]
        mov     ss,[bx+di+34H]
        mov     ds,[bp+si+1234H]
;;; Push data onto stack
        push    bp
        push    [si]
        push    [1234H]
        push    [di-34H]
        push    [bp+1234H]
        push    [bx+si]
        push    [bx+di+34H]
        push    [bp+si+1234H]
        push    cs
;;; Pop data from stack
        pop     bp
        pop     [si]
        pop     [1234H]
        pop     [di-34H]
        pop     [bp+1234H]
        pop     [bx+si]
        pop     [bx+di+34H]
        pop     [bp+si+1234H]
        pop     cs
;;; Exchange data
        xchg    al,ch
        xchg    [si],cl
        xchg    [1234H],dl
        xchg    [di-34H],bl
        xchg    [bp+1234H],ah
        xchg    [bx+si],ch
        xchg    [bx+di+34H],dh
        xchg    [bp+si+1234H],bh
        xchg    ax,bp
        xchg    [si],cx
        xchg    [1234H],dx
        xchg    [di-34H],bx
        xchg    [bp+1234H],sp
        xchg    [bx+si],bp
        xchg    [bx+di+34H],si
        xchg    [bp+si+1234H],di
;;; Input from port
        in      al,34H
        in      al,dx
        in      ax,34H
        in      ax,dx
;;; Output to port
        out     34H,al
        out     dx,al
        out     34H,ax
        out     dx,ax
;;; Table look-up translation
        xlat
;;; Load effective address
        lea     cx,[si]
        lea     dx,[1234H]
        lea     bx,[di-34H]
        lea     sp,[bp+1234H]
        lea     bp,[bx+si]
        lea     si,[bx+di+34H]
        lea     di,[bp+si+1234H]
;;; Load pointer using DS
        lds     cx,[si]
        lds     dx,[1234H]
        lds     bx,[di-34H]
        lds     sp,[bp+1234H]
        lds     bp,[bx+si]
        lds     si,[bx+di+34H]
        lds     di,[bp+si+1234H]
;;; Load ES with pointer
        les     cx,[si]
        les     dx,[1234H]
        les     bx,[di-34H]
        les     sp,[bp+1234H]
        les     bp,[bx+si]
        les     si,[bx+di+34H]
        les     di,[bp+si+1234H]
;;; Load FLAGS into AH register
        lahf
;;; Store AH into FLAGS
        sahf
;;; Push FLAGS register onto stack
        pushf
;;; Pop FLAGS register from stack
        popf

;;; Arithmetic
;;; Add
        add     al,cl
        add     dl,bl
        add     ah,ch
        add     [si],dh
        add     [1234H],bh
        add     [di-34H],al
        add     [bp+1234H],cl
        add     [bx+si],dl
        add     [bx+di+34H],bl
        add     [bp+si+1234H],ah
        add     dh,[si]
        add     bh,[1234H]
        add     al,[di-34H]
        add     cl,[bp+1234H]
        add     dl,[bx+si]
        add     bl,[bx+di+34H]
        add     ah,[bp+si+1234H]
        add     ax,cx
        add     dx,bx
        add     sp,bp
        add     [si],si
        add     [1234H],di
        add     [di-34H],ax
        add     [bp+1234H],cx
        add     [bx+si],dx
        add     [bx+di+34H],bx
        add     [bp+si+1234H],sp
        add     si,[si]
        add     di,[1234H]
        add     ax,[di-34H]
        add     cx,[bp+1234H]
        add     dx,[bx+si]
        add     bx,[bx+di+34H]
        add     sp,[bp+si+1234H]
        add     al,56h
        add     cl,56h
        add     byte ptr [si],56h
        add     byte ptr [1234H],56h
        add     byte ptr [di-34H],56h
        add     byte ptr [bp+1234H],56h
        add     byte ptr [bx+si],56h
        add     byte ptr [bx+di+34H],56h
        add     byte ptr [bp+si+1234H],56h
        add     ax,5678h
        add     cx,5678h
        add     word ptr [si],5678h
        add     word ptr [1234H],5678h
        add     word ptr [di-34H],5678h
        add     word ptr [bp+1234H],5678h
        add     word ptr [bx+si],5678h
        add     word ptr [bx+di+34H],5678h
        add     word ptr [bp+si+1234H],5678h
        add     ax,56h
        add     ax,-16
        add     cx,56h
        add     cx,-16
        add     word ptr [si],56h
        add     word ptr [1234H],56h
        add     word ptr [di-34H],56h
        add     word ptr [bp+1234H],56h
        add     word ptr [bx+si],56h
        add     word ptr [bx+di+34H],56h
        add     word ptr [bp+si+1234H],56h
;;; Add with carry
        adc     al,cl
        adc     dl,bl
        adc     ah,ch
        adc     [si],dh
        adc     [1234H],bh
        adc     [di-34H],al
        adc     [bp+1234H],cl
        adc     [bx+si],dl
        adc     [bx+di+34H],bl
        adc     [bp+si+1234H],ah
        adc     dh,[si]
        adc     bh,[1234H]
        adc     al,[di-34H]
        adc     cl,[bp+1234H]
        adc     dl,[bx+si]
        adc     bl,[bx+di+34H]
        adc     ah,[bp+si+1234H]
        adc     ax,cx
        adc     dx,bx
        adc     sp,bp
        adc     [si],si
        adc     [1234H],di
        adc     [di-34H],ax
        adc     [bp+1234H],cx
        adc     [bx+si],dx
        adc     [bx+di+34H],bx
        adc     [bp+si+1234H],sp
        adc     si,[si]
        adc     di,[1234H]
        adc     ax,[di-34H]
        adc     cx,[bp+1234H]
        adc     dx,[bx+si]
        adc     bx,[bx+di+34H]
        adc     sp,[bp+si+1234H]
        adc     al,56h
        adc     cl,56h
        adc     byte ptr [si],56h
        adc     byte ptr [1234H],56h
        adc     byte ptr [di-34H],56h
        adc     byte ptr [bp+1234H],56h
        adc     byte ptr [bx+si],56h
        adc     byte ptr [bx+di+34H],56h
        adc     byte ptr [bp+si+1234H],56h
        adc     ax,5678h
        adc     cx,5678h
        adc     word ptr [si],5678h
        adc     word ptr [1234H],5678h
        adc     word ptr [di-34H],5678h
        adc     word ptr [bp+1234H],5678h
        adc     word ptr [bx+si],5678h
        adc     word ptr [bx+di+34H],5678h
        adc     word ptr [bp+si+1234H],5678h
        adc     ax,56h
        adc     ax,-16
        adc     cx,56h
        adc     cx,-16
        adc     word ptr [si],56h
        adc     word ptr [1234H],56h
        adc     word ptr [di-34H],56h
        adc     word ptr [bp+1234H],56h
        adc     word ptr [bx+si],56h
        adc     word ptr [bx+di+34H],56h
        adc     word ptr [bp+si+1234H],56h
;;; Increment by 1
        inc     ch
        inc     byte ptr [si]
        inc     byte ptr [1234H]
        inc     byte ptr [di-34H]
        inc     byte ptr [bp+1234H]
        inc     byte ptr [bx+si]
        inc     byte ptr [bx+di+34H]
        inc     byte ptr [bp+si+1234H]
        inc     bp
        inc     word ptr [si]
        inc     word ptr [1234H]
        inc     word ptr [di-34H]
        inc     word ptr [bp+1234H]
        inc     word ptr [bx+si]
        inc     word ptr [bx+di+34H]
        inc     word ptr [bp+si+1234H]
;;; ASCII adjust AL after addition
        aaa
;;; Decimal adjust AL after addition
        daa
;;; Subtraction
        sub     al,cl
        sub     dl,bl
        sub     ah,ch
        sub     [si],dh
        sub     [1234H],bh
        sub     [di-34H],al
        sub     [bp+1234H],cl
        sub     [bx+si],dl
        sub     [bx+di+34H],bl
        sub     [bp+si+1234H],ah
        sub     dh,[si]
        sub     bh,[1234H]
        sub     al,[di-34H]
        sub     cl,[bp+1234H]
        sub     dl,[bx+si]
        sub     bl,[bx+di+34H]
        sub     ah,[bp+si+1234H]
        sub     ax,cx
        sub     dx,bx
        sub     sp,bp
        sub     [si],si
        sub     [1234H],di
        sub     [di-34H],ax
        sub     [bp+1234H],cx
        sub     [bx+si],dx
        sub     [bx+di+34H],bx
        sub     [bp+si+1234H],sp
        sub     si,[si]
        sub     di,[1234H]
        sub     ax,[di-34H]
        sub     cx,[bp+1234H]
        sub     dx,[bx+si]
        sub     bx,[bx+di+34H]
        sub     sp,[bp+si+1234H]
        sub     al,56h
        sub     cl,56h
        sub     byte ptr [si],56h
        sub     byte ptr [1234H],56h
        sub     byte ptr [di-34H],56h
        sub     byte ptr [bp+1234H],56h
        sub     byte ptr [bx+si],56h
        sub     byte ptr [bx+di+34H],56h
        sub     byte ptr [bp+si+1234H],56h
        sub     ax,5678h
        sub     cx,5678h
        sub     word ptr [si],5678h
        sub     word ptr [1234H],5678h
        sub     word ptr [di-34H],5678h
        sub     word ptr [bp+1234H],5678h
        sub     word ptr [bx+si],5678h
        sub     word ptr [bx+di+34H],5678h
        sub     word ptr [bp+si+1234H],5678h
        sub     ax,56h
        sub     ax,-16
        sub     cx,56h
        sub     cx,-16
        sub     word ptr [si],56h
        sub     word ptr [1234H],56h
        sub     word ptr [di-34H],56h
        sub     word ptr [bp+1234H],56h
        sub     word ptr [bx+si],56h
        sub     word ptr [bx+di+34H],56h
        sub     word ptr [bp+si+1234H],56h
;;; Subtract with borrow
        sbb     al,cl
        sbb     dl,bl
        sbb     ah,ch
        sbb     [si],dh
        sbb     [1234H],bh
        sbb     [di-34H],al
        sbb     [bp+1234H],cl
        sbb     [bx+si],dl
        sbb     [bx+di+34H],bl
        sbb     [bp+si+1234H],ah
        sbb     dh,[si]
        sbb     bh,[1234H]
        sbb     al,[di-34H]
        sbb     cl,[bp+1234H]
        sbb     dl,[bx+si]
        sbb     bl,[bx+di+34H]
        sbb     ah,[bp+si+1234H]
        sbb     ax,cx
        sbb     dx,bx
        sbb     sp,bp
        sbb     [si],si
        sbb     [1234H],di
        sbb     [di-34H],ax
        sbb     [bp+1234H],cx
        sbb     [bx+si],dx
        sbb     [bx+di+34H],bx
        sbb     [bp+si+1234H],sp
        sbb     si,[si]
        sbb     di,[1234H]
        sbb     ax,[di-34H]
        sbb     cx,[bp+1234H]
        sbb     dx,[bx+si]
        sbb     bx,[bx+di+34H]
        sbb     sp,[bp+si+1234H]
        sbb     al,56h
        sbb     cl,56h
        sbb     byte ptr [si],56h
        sbb     byte ptr [1234H],56h
        sbb     byte ptr [di-34H],56h
        sbb     byte ptr [bp+1234H],56h
        sbb     byte ptr [bx+si],56h
        sbb     byte ptr [bx+di+34H],56h
        sbb     byte ptr [bp+si+1234H],56h
        sbb     ax,5678h
        sbb     cx,5678h
        sbb     word ptr [si],5678h
        sbb     word ptr [1234H],5678h
        sbb     word ptr [di-34H],5678h
        sbb     word ptr [bp+1234H],5678h
        sbb     word ptr [bx+si],5678h
        sbb     word ptr [bx+di+34H],5678h
        sbb     word ptr [bp+si+1234H],5678h
        sbb     ax,56h
        sbb     ax,-16
        sbb     cx,56h
        sbb     cx,-16
        sbb     word ptr [si],56h
        sbb     word ptr [1234H],56h
        sbb     word ptr [di-34H],56h
        sbb     word ptr [bp+1234H],56h
        sbb     word ptr [bx+si],56h
        sbb     word ptr [bx+di+34H],56h
        sbb     word ptr [bp+si+1234H],56h
;;; Decrement by 1
        dec     ch
        dec     byte ptr [si]
        dec     byte ptr [1234H]
        dec     byte ptr [di-34H]
        dec     byte ptr [bp+1234H]
        dec     byte ptr [bx+si]
        dec     byte ptr [bx+di+34H]
        dec     byte ptr [bp+si+1234H]
        dec     bp
        dec     word ptr [si]
        dec     word ptr [1234H]
        dec     word ptr [di-34H]
        dec     word ptr [bp+1234H]
        dec     word ptr [bx+si]
        dec     word ptr [bx+di+34H]
        dec     word ptr [bp+si+1234H]
;;; Negation
        neg     ch
        neg     byte ptr [si]
        neg     byte ptr [1234H]
        neg     byte ptr [di-34H]
        neg     byte ptr [bp+1234H]
        neg     byte ptr [bx+si]
        neg     byte ptr [bx+di+34H]
        neg     byte ptr [bp+si+1234H]
        neg     bp
        neg     word ptr [si]
        neg     word ptr [1234H]
        neg     word ptr [di-34H]
        neg     word ptr [bp+1234H]
        neg     word ptr [bx+si]
        neg     word ptr [bx+di+34H]
        neg     word ptr [bp+si+1234H]
;;; Compare operands
        cmp     al,cl
        cmp     dl,bl
        cmp     ah,ch
        cmp     [si],dh
        cmp     [1234H],bh
        cmp     [di-34H],al
        cmp     [bp+1234H],cl
        cmp     [bx+si],dl
        cmp     [bx+di+34H],bl
        cmp     [bp+si+1234H],ah
        cmp     dh,[si]
        cmp     bh,[1234H]
        cmp     al,[di-34H]
        cmp     cl,[bp+1234H]
        cmp     dl,[bx+si]
        cmp     bl,[bx+di+34H]
        cmp     ah,[bp+si+1234H]
        cmp     ax,cx
        cmp     dx,bx
        cmp     sp,bp
        cmp     [si],si
        cmp     [1234H],di
        cmp     [di-34H],ax
        cmp     [bp+1234H],cx
        cmp     [bx+si],dx
        cmp     [bx+di+34H],bx
        cmp     [bp+si+1234H],sp
        cmp     si,[si]
        cmp     di,[1234H]
        cmp     ax,[di-34H]
        cmp     cx,[bp+1234H]
        cmp     dx,[bx+si]
        cmp     bx,[bx+di+34H]
        cmp     sp,[bp+si+1234H]
        cmp     al,56h
        cmp     cl,56h
        cmp     byte ptr [si],56h
        cmp     byte ptr [1234H],56h
        cmp     byte ptr [di-34H],56h
        cmp     byte ptr [bp+1234H],56h
        cmp     byte ptr [bx+si],56h
        cmp     byte ptr [bx+di+34H],56h
        cmp     byte ptr [bp+si+1234H],56h
        cmp     ax,5678h
        cmp     cx,5678h
        cmp     word ptr [si],5678h
        cmp     word ptr [1234H],5678h
        cmp     word ptr [di-34H],5678h
        cmp     word ptr [bp+1234H],5678h
        cmp     word ptr [bx+si],5678h
        cmp     word ptr [bx+di+34H],5678h
        cmp     word ptr [bp+si+1234H],5678h
        cmp     ax,56h
        cmp     ax,-16
        cmp     cx,56h
        cmp     cx,-16
        cmp     word ptr [si],56h
        cmp     word ptr [1234H],56h
        cmp     word ptr [di-34H],56h
        cmp     word ptr [bp+1234H],56h
        cmp     word ptr [bx+si],56h
        cmp     word ptr [bx+di+34H],56h
        cmp     word ptr [bp+si+1234H],56h
;;; ASCII adjust AL after subtraction
        aas
;;; Decimal adjust AL after subtraction
        das
;;; Unsigned multiply
        mul     ch
        mul     byte ptr [si]
        mul     byte ptr [1234H]
        mul     byte ptr [di-34H]
        mul     byte ptr [bp+1234H]
        mul     byte ptr [bx+si]
        mul     byte ptr [bx+di+34H]
        mul     byte ptr [bp+si+1234H]
        mul     bp
        mul     word ptr [si]
        mul     word ptr [1234H]
        mul     word ptr [di-34H]
        mul     word ptr [bp+1234H]
        mul     word ptr [bx+si]
        mul     word ptr [bx+di+34H]
        mul     word ptr [bp+si+1234H]
;;; Signed multiply
        imul    ch
        imul    byte ptr [si]
        imul    byte ptr [1234H]
        imul    byte ptr [di-34H]
        imul    byte ptr [bp+1234H]
        imul    byte ptr [bx+si]
        imul    byte ptr [bx+di+34H]
        imul    byte ptr [bp+si+1234H]
        imul    bp
        imul    word ptr [si]
        imul    word ptr [1234H]
        imul    word ptr [di-34H]
        imul    word ptr [bp+1234H]
        imul    word ptr [bx+si]
        imul    word ptr [bx+di+34H]
        imul    word ptr [bp+si+1234H]
;;; ASCII adjust AX afrer multiplication
        aam
;;; Unsigned divide
        div     ch
        div     byte ptr [si]
        div     byte ptr [1234H]
        div     byte ptr [di-34H]
        div     byte ptr [bp+1234H]
        div     byte ptr [bx+si]
        div     byte ptr [bx+di+34H]
        div     byte ptr [bp+si+1234H]
        div     bp
        div     word ptr [si]
        div     word ptr [1234H]
        div     word ptr [di-34H]
        div     word ptr [bp+1234H]
        div     word ptr [bx+si]
        div     word ptr [bx+di+34H]
        div     word ptr [bp+si+1234H]
;;; Signed divide
        idiv    ch
        idiv    byte ptr [si]
        idiv    byte ptr [1234H]
        idiv    byte ptr [di-34H]
        idiv    byte ptr [bp+1234H]
        idiv    byte ptr [bx+si]
        idiv    byte ptr [bx+di+34H]
        idiv    byte ptr [bp+si+1234H]
        idiv    bp
        idiv    word ptr [si]
        idiv    word ptr [1234H]
        idiv    word ptr [di-34H]
        idiv    word ptr [bp+1234H]
        idiv    word ptr [bx+si]
        idiv    word ptr [bx+di+34H]
        idiv    word ptr [bp+si+1234H]
;;; ASCII adjust AX before division
        aad
;;; Convert byte to word
        cbw
;;; Convert word to double word
        cwd

;;; Logic
;;; Negate the operand, logical NOT
        not     ch
        not     byte ptr [si]
        not     byte ptr [1234H]
        not     byte ptr [di-34H]
        not     byte ptr [bp+1234H]
        not     byte ptr [bx+si]
        not     byte ptr [bx+di+34H]
        not     byte ptr [bp+si+1234H]
        not     bp
        not     word ptr [si]
        not     word ptr [1234H]
        not     word ptr [di-34H]
        not     word ptr [bp+1234H]
        not     word ptr [bx+si]
        not     word ptr [bx+di+34H]
        not     word ptr [bp+si+1234H]
;;; Shift left, unsigned shift left
        shl     ch,1
        shl     byte ptr [si],1
        shl     byte ptr [1234H],1
        shl     byte ptr [di-34H],1
        shl     byte ptr [bp+1234H],1
        shl     byte ptr [bx+si],1
        shl     byte ptr [bx+di+34H],1
        shl     byte ptr [bp+si+1234H],1
        shl     bp,1
        shl     word ptr [si],1
        shl     word ptr [1234H],1
        shl     word ptr [di-34H],1
        shl     word ptr [bp+1234H],1
        shl     word ptr [bx+si],1
        shl     word ptr [bx+di+34H],1
        shl     word ptr [bp+si+1234H],1
        shl     ch,cl
        shl     byte ptr [si],cl
        shl     byte ptr [1234H],cl
        shl     byte ptr [di-34H],cl
        shl     byte ptr [bp+1234H],cl
        shl     byte ptr [bx+si],cl
        shl     byte ptr [bx+di+34H],cl
        shl     byte ptr [bp+si+1234H],cl
        shl     bp,cl
        shl     word ptr [si],cl
        shl     word ptr [1234H],cl
        shl     word ptr [di-34H],cl
        shl     word ptr [bp+1234H],cl
        shl     word ptr [bx+si],cl
        shl     word ptr [bx+di+34H],cl
        shl     word ptr [bp+si+1234H],cl
;;; Shift arithmetically left, signed shift left
        sal     ch,1
        sal     byte ptr [si],1
        sal     byte ptr [1234H],1
        sal     byte ptr [di-34H],1
        sal     byte ptr [bp+1234H],1
        sal     byte ptr [bx+si],1
        sal     byte ptr [bx+di+34H],1
        sal     byte ptr [bp+si+1234H],1
        sal     bp,1
        sal     word ptr [si],1
        sal     word ptr [1234H],1
        sal     word ptr [di-34H],1
        sal     word ptr [bp+1234H],1
        sal     word ptr [bx+si],1
        sal     word ptr [bx+di+34H],1
        sal     word ptr [bp+si+1234H],1
        sal     ch,cl
        sal     byte ptr [si],cl
        sal     byte ptr [1234H],cl
        sal     byte ptr [di-34H],cl
        sal     byte ptr [bp+1234H],cl
        sal     byte ptr [bx+si],cl
        sal     byte ptr [bx+di+34H],cl
        sal     byte ptr [bp+si+1234H],cl
        sal     bp,cl
        sal     word ptr [si],cl
        sal     word ptr [1234H],cl
        sal     word ptr [di-34H],cl
        sal     word ptr [bp+1234H],cl
        sal     word ptr [bx+si],cl
        sal     word ptr [bx+di+34H],cl
        sal     word ptr [bp+si+1234H],cl
;;; Shift right, unsigned shift right
        shr     ch,1
        shr     byte ptr [si],1
        shr     byte ptr [1234H],1
        shr     byte ptr [di-34H],1
        shr     byte ptr [bp+1234H],1
        shr     byte ptr [bx+si],1
        shr     byte ptr [bx+di+34H],1
        shr     byte ptr [bp+si+1234H],1
        shr     bp,1
        shr     word ptr [si],1
        shr     word ptr [1234H],1
        shr     word ptr [di-34H],1
        shr     word ptr [bp+1234H],1
        shr     word ptr [bx+si],1
        shr     word ptr [bx+di+34H],1
        shr     word ptr [bp+si+1234H],1
        shr     ch,cl
        shr     byte ptr [si],cl
        shr     byte ptr [1234H],cl
        shr     byte ptr [di-34H],cl
        shr     byte ptr [bp+1234H],cl
        shr     byte ptr [bx+si],cl
        shr     byte ptr [bx+di+34H],cl
        shr     byte ptr [bp+si+1234H],cl
        shr     bp,cl
        shr     word ptr [si],cl
        shr     word ptr [1234H],cl
        shr     word ptr [di-34H],cl
        shr     word ptr [bp+1234H],cl
        shr     word ptr [bx+si],cl
        shr     word ptr [bx+di+34H],cl
        shr     word ptr [bp+si+1234H],cl
;;; Shift arithmetically right, signed shift right
        sar     ch,1
        sar     byte ptr [si],1
        sar     byte ptr [1234H],1
        sar     byte ptr [di-34H],1
        sar     byte ptr [bp+1234H],1
        sar     byte ptr [bx+si],1
        sar     byte ptr [bx+di+34H],1
        sar     byte ptr [bp+si+1234H],1
        sar     bp,1
        sar     word ptr [si],1
        sar     word ptr [1234H],1
        sar     word ptr [di-34H],1
        sar     word ptr [bp+1234H],1
        sar     word ptr [bx+si],1
        sar     word ptr [bx+di+34H],1
        sar     word ptr [bp+si+1234H],1
        sar     ch,cl
        sar     byte ptr [si],cl
        sar     byte ptr [1234H],cl
        sar     byte ptr [di-34H],cl
        sar     byte ptr [bp+1234H],cl
        sar     byte ptr [bx+si],cl
        sar     byte ptr [bx+di+34H],cl
        sar     byte ptr [bp+si+1234H],cl
        sar     bp,cl
        sar     word ptr [si],cl
        sar     word ptr [1234H],cl
        sar     word ptr [di-34H],cl
        sar     word ptr [bp+1234H],cl
        sar     word ptr [bx+si],cl
        sar     word ptr [bx+di+34H],cl
        sar     word ptr [bp+si+1234H],cl
;;; Rotate left
        rol     ch,1
        rol     byte ptr [si],1
        rol     byte ptr [1234H],1
        rol     byte ptr [di-34H],1
        rol     byte ptr [bp+1234H],1
        rol     byte ptr [bx+si],1
        rol     byte ptr [bx+di+34H],1
        rol     byte ptr [bp+si+1234H],1
        rol     bp,1
        rol     word ptr [si],1
        rol     word ptr [1234H],1
        rol     word ptr [di-34H],1
        rol     word ptr [bp+1234H],1
        rol     word ptr [bx+si],1
        rol     word ptr [bx+di+34H],1
        rol     word ptr [bp+si+1234H],1
        rol     ch,cl
        rol     byte ptr [si],cl
        rol     byte ptr [1234H],cl
        rol     byte ptr [di-34H],cl
        rol     byte ptr [bp+1234H],cl
        rol     byte ptr [bx+si],cl
        rol     byte ptr [bx+di+34H],cl
        rol     byte ptr [bp+si+1234H],cl
        rol     bp,cl
        rol     word ptr [si],cl
        rol     word ptr [1234H],cl
        rol     word ptr [di-34H],cl
        rol     word ptr [bp+1234H],cl
        rol     word ptr [bx+si],cl
        rol     word ptr [bx+di+34H],cl
        rol     word ptr [bp+si+1234H],cl
;;; Rotate right
        ror     ch,1
        ror     byte ptr [si],1
        ror     byte ptr [1234H],1
        ror     byte ptr [di-34H],1
        ror     byte ptr [bp+1234H],1
        ror     byte ptr [bx+si],1
        ror     byte ptr [bx+di+34H],1
        ror     byte ptr [bp+si+1234H],1
        ror     bp,1
        ror     word ptr [si],1
        ror     word ptr [1234H],1
        ror     word ptr [di-34H],1
        ror     word ptr [bp+1234H],1
        ror     word ptr [bx+si],1
        ror     word ptr [bx+di+34H],1
        ror     word ptr [bp+si+1234H],1
        ror     ch,cl
        ror     byte ptr [si],cl
        ror     byte ptr [1234H],cl
        ror     byte ptr [di-34H],cl
        ror     byte ptr [bp+1234H],cl
        ror     byte ptr [bx+si],cl
        ror     byte ptr [bx+di+34H],cl
        ror     byte ptr [bp+si+1234H],cl
        ror     bp,cl
        ror     word ptr [si],cl
        ror     word ptr [1234H],cl
        ror     word ptr [di-34H],cl
        ror     word ptr [bp+1234H],cl
        ror     word ptr [bx+si],cl
        ror     word ptr [bx+di+34H],cl
        ror     word ptr [bp+si+1234H],cl
;;; Rotate left with carry
        rcl     ch,1
        rcl     byte ptr [si],1
        rcl     byte ptr [1234H],1
        rcl     byte ptr [di-34H],1
        rcl     byte ptr [bp+1234H],1
        rcl     byte ptr [bx+si],1
        rcl     byte ptr [bx+di+34H],1
        rcl     byte ptr [bp+si+1234H],1
        rcl     bp,1
        rcl     word ptr [si],1
        rcl     word ptr [1234H],1
        rcl     word ptr [di-34H],1
        rcl     word ptr [bp+1234H],1
        rcl     word ptr [bx+si],1
        rcl     word ptr [bx+di+34H],1
        rcl     word ptr [bp+si+1234H],1
        rcl     ch,cl
        rcl     byte ptr [si],cl
        rcl     byte ptr [1234H],cl
        rcl     byte ptr [di-34H],cl
        rcl     byte ptr [bp+1234H],cl
        rcl     byte ptr [bx+si],cl
        rcl     byte ptr [bx+di+34H],cl
        rcl     byte ptr [bp+si+1234H],cl
        rcl     bp,cl
        rcl     word ptr [si],cl
        rcl     word ptr [1234H],cl
        rcl     word ptr [di-34H],cl
        rcl     word ptr [bp+1234H],cl
        rcl     word ptr [bx+si],cl
        rcl     word ptr [bx+di+34H],cl
        rcl     word ptr [bp+si+1234H],cl
;;; Rotate right with carry
        rcr     ch,1
        rcr     byte ptr [si],1
        rcr     byte ptr [1234H],1
        rcr     byte ptr [di-34H],1
        rcr     byte ptr [bp+1234H],1
        rcr     byte ptr [bx+si],1
        rcr     byte ptr [bx+di+34H],1
        rcr     byte ptr [bp+si+1234H],1
        rcr     bp,1
        rcr     word ptr [si],1
        rcr     word ptr [1234H],1
        rcr     word ptr [di-34H],1
        rcr     word ptr [bp+1234H],1
        rcr     word ptr [bx+si],1
        rcr     word ptr [bx+di+34H],1
        rcr     word ptr [bp+si+1234H],1
        rcr     ch,cl
        rcr     byte ptr [si],cl
        rcr     byte ptr [1234H],cl
        rcr     byte ptr [di-34H],cl
        rcr     byte ptr [bp+1234H],cl
        rcr     byte ptr [bx+si],cl
        rcr     byte ptr [bx+di+34H],cl
        rcr     byte ptr [bp+si+1234H],cl
        rcr     bp,cl
        rcr     word ptr [si],cl
        rcr     word ptr [1234H],cl
        rcr     word ptr [di-34H],cl
        rcr     word ptr [bp+1234H],cl
        rcr     word ptr [bx+si],cl
        rcr     word ptr [bx+di+34H],cl
        rcr     word ptr [bp+si+1234H],cl
;;; Logical AND
        and     al,cl
        and     dl,bl
        and     ah,ch
        and     [si],dh
        and     [1234H],bh
        and     [di-34H],al
        and     [bp+1234H],cl
        and     [bx+si],dl
        and     [bx+di+34H],bl
        and     [bp+si+1234H],ah
        and     dh,[si]
        and     bh,[1234H]
        and     al,[di-34H]
        and     cl,[bp+1234H]
        and     dl,[bx+si]
        and     bl,[bx+di+34H]
        and     ah,[bp+si+1234H]
        and     ax,cx
        and     dx,bx
        and     sp,bp
        and     [si],si
        and     [1234H],di
        and     [di-34H],ax
        and     [bp+1234H],cx
        and     [bx+si],dx
        and     [bx+di+34H],bx
        and     [bp+si+1234H],sp
        and     si,[si]
        and     di,[1234H]
        and     ax,[di-34H]
        and     cx,[bp+1234H]
        and     dx,[bx+si]
        and     bx,[bx+di+34H]
        and     sp,[bp+si+1234H]
        and     al,56h
        and     cl,56h
        and     byte ptr [si],56h
        and     byte ptr [1234H],56h
        and     byte ptr [di-34H],56h
        and     byte ptr [bp+1234H],56h
        and     byte ptr [bx+si],56h
        and     byte ptr [bx+di+34H],56h
        and     byte ptr [bp+si+1234H],56h
        and     ax,5678h
        and     ax,-16
        and     cx,5678h
        and     cx,-16
        and     word ptr [si],5678h
        and     word ptr [1234H],5678h
        and     word ptr [di-34H],5678h
        and     word ptr [bp+1234H],5678h
        and     word ptr [bx+si],5678h
        and     word ptr [bx+di+34H],5678h
        and     word ptr [bp+si+1234H],5678h
;;; Logical compare
        test    al,cl
        test    dl,bl
        test    ah,ch
        test    dh,[si]
        test    bh,[1234H]
        test    al,[di-34H]
        test    cl,[bp+1234H]
        test    dl,[bx+si]
        test    bl,[bx+di+34H]
        test    ah,[bp+si+1234H]
        test    [si],dh
        test    [1234H],bh
        test    [di-34H],al
        test    [bp+1234H],cl
        test    [bx+si],dl
        test    [bx+di+34H],bl
        test    [bp+si+1234H],ah
        test    ax,cx
        test    dx,bx
        test    sp,bp
        test    si,[si]
        test    di,[1234H]
        test    ax,[di-34H]
        test    cx,[bp+1234H]
        test    dx,[bx+si]
        test    bx,[bx+di+34H]
        test    sp,[bp+si+1234H]
        test    [si],si
        test    [1234H],di
        test    [di-34H],ax
        test    [bp+1234H],cx
        test    [bx+si],dx
        test    [bx+di+34H],bx
        test    [bp+si+1234H],sp
        test    al,56h
        test    cl,56h
        test    byte ptr [si],56h
        test    byte ptr [1234H],56h
        test    byte ptr [di-34H],56h
        test    byte ptr [bp+1234H],56h
        test    byte ptr [bx+si],56h
        test    byte ptr [bx+di+34H],56h
        test    byte ptr [bp+si+1234H],56h
        test    ax,5678h
        test    ax,-16
        test    cx,5678h
        test    cx,-16
        test    word ptr [si],5678h
        test    word ptr [1234H],5678h
        test    word ptr [di-34H],5678h
        test    word ptr [bp+1234H],5678h
        test    word ptr [bx+si],5678h
        test    word ptr [bx+di+34H],5678h
        test    word ptr [bp+si+1234H],5678h
;;; Logical OR
        or      al,cl
        or      dl,bl
        or      ah,ch
        or      [si],dh
        or      [1234H],bh
        or      [di-34H],al
        or      [bp+1234H],cl
        or      [bx+si],dl
        or      [bx+di+34H],bl
        or      [bp+si+1234H],ah
        or      dh,[si]
        or      bh,[1234H]
        or      al,[di-34H]
        or      cl,[bp+1234H]
        or      dl,[bx+si]
        or      bl,[bx+di+34H]
        or      ah,[bp+si+1234H]
        or      ax,cx
        or      dx,bx
        or      sp,bp
        or      [si],si
        or      [1234H],di
        or      [di-34H],ax
        or      [bp+1234H],cx
        or      [bx+si],dx
        or      [bx+di+34H],bx
        or      [bp+si+1234H],sp
        or      si,[si]
        or      di,[1234H]
        or      ax,[di-34H]
        or      cx,[bp+1234H]
        or      dx,[bx+si]
        or      bx,[bx+di+34H]
        or      sp,[bp+si+1234H]
        or      al,56h
        or      cl,56h
        or      byte ptr [si],56h
        or      byte ptr [1234H],56h
        or      byte ptr [di-34H],56h
        or      byte ptr [bp+1234H],56h
        or      byte ptr [bx+si],56h
        or      byte ptr [bx+di+34H],56h
        or      byte ptr [bp+si+1234H],56h
        or      ax,5678h
        or      ax,-16
        or      cx,5678h
        or      cx,-16
        or      word ptr [si],5678h
        or      word ptr [1234H],5678h
        or      word ptr [di-34H],5678h
        or      word ptr [bp+1234H],5678h
        or      word ptr [bx+si],5678h
        or      word ptr [bx+di+34H],5678h
        or      word ptr [bp+si+1234H],5678h
;;; Exclusive OR
        xor     al,cl
        xor     dl,bl
        xor     ah,ch
        xor     [si],dh
        xor     [1234H],bh
        xor     [di-34H],al
        xor     [bp+1234H],cl
        xor     [bx+si],dl
        xor     [bx+di+34H],bl
        xor     [bp+si+1234H],ah
        xor     dh,[si]
        xor     bh,[1234H]
        xor     al,[di-34H]
        xor     cl,[bp+1234H]
        xor     dl,[bx+si]
        xor     bl,[bx+di+34H]
        xor     ah,[bp+si+1234H]
        xor     ax,cx
        xor     dx,bx
        xor     sp,bp
        xor     [si],si
        xor     [1234H],di
        xor     [di-34H],ax
        xor     [bp+1234H],cx
        xor     [bx+si],dx
        xor     [bx+di+34H],bx
        xor     [bp+si+1234H],sp
        xor     si,[si]
        xor     di,[1234H]
        xor     ax,[di-34H]
        xor     cx,[bp+1234H]
        xor     dx,[bx+si]
        xor     bx,[bx+di+34H]
        xor     sp,[bp+si+1234H]
        xor     al,56h
        xor     cl,56h
        xor     byte ptr [si],56h
        xor     byte ptr [1234H],56h
        xor     byte ptr [di-34H],56h
        xor     byte ptr [bp+1234H],56h
        xor     byte ptr [bx+si],56h
        xor     byte ptr [bx+di+34H],56h
        xor     byte ptr [bp+si+1234H],56h
        xor     ax,5678h
        xor     ax,-16
        xor     cx,5678h
        xor     cx,-16
        xor     word ptr [si],5678h
        xor     word ptr [1234H],5678h
        xor     word ptr [di-34H],5678h
        xor     word ptr [bp+1234H],5678h
        xor     word ptr [bx+si],5678h
        xor     word ptr [bx+di+34H],5678h
        xor     word ptr [bp+si+1234H],5678h

;;; String manipulation
;;; Repeat
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
;;; Repeat While Equal/Zero
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
;;; Repeat While Not Equal/Not Zero
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
;;; Move byte from string to string
        movsb
;;; Move word from string to string
        movsw
;;; Compare bytes in memory
        cmpsb
;;; Compare words in memory
        cmpsw
;;; Compare byte string
        scasb
;;; Compare word string
        scasw
;;; Load string byte
        lodsb
;;; Load string word
        lodsw
;;; Store byte string
        stosb
;;; Store word string
        stosw

;;; Control transfer
;;; Call procedure
        call    $
        call    ax
        call    si
        call    [si]
        call    [1234H]
        call    [di-34H]
        call    [bp+1234H]
        call    [bx+si]
        call    [bx+di+34H]
        call    [bp+si+1234H]
        callf   1234H:5678H
        callf   [si]
        callf   [1234H]
        callf   [di-34H]
        callf   [bp+1234H]
        callf   [bx+si]
        callf   [bx+di+34H]
        callf   [bp+si+1234H]
;;; Jump
        jmp     $+1234H
        jmp     $
        jmp     ax
        jmp     si
        jmp     [si]
        jmp     [1234H]
        jmp     [di-34H]
        jmp     [bp+1234H]
        jmp     [bx+si]
        jmp     [bx+di+34H]
        jmp     [bp+si+1234H]
        jmpf    1234H:5678H
        jmpf    [si]
        jmpf    [1234H]
        jmpf    [di-34H]
        jmpf    [bp+1234H]
        jmpf    [bx+si]
        jmpf    [bx+di+34H]
        jmpf    [bp+si+1234H]
;;; Return from procedure
        ret
        ret     16
        retf
        retf    16
;;; Jump on Equal/Zero
        je      $
        jz      $
;;; Jump on Less/Not Greater or Equal
        jl      $
        jnge    $
;;; Jump on Less or Equal/Not Greater
        jle     $
        jng     $
;;; Jump on Below/Not Above or Equal/Carry
        jb      $
        jnae    $
        jc      $
;;; Jump on Below or Equal/Not Above
        jbe     $
        jna     $
;;; Jump on Parity/Parity Even
        jpe     $
        jp      $
;;; Jump on Overflow
        jo      $
;;; Jump on Sign
        js      $
;;; Jump on Not Equal/Not Zero
        jne     $
        jnz     $
;;; Jump on Not Less/Greater or Equal
        jge     $
        jnl     $
;;; Jump on Not Less or Equal/Greater
        jg      $
        jnle    $
;;; Jump on Not Below/Above or Equal/Not Carry
        jae     $
        jnb     $
        jnc     $
;;; Jump on Not Below or Equal/Above
        ja      $
        jnbe    $
;;; Jump on Not Parity/Parity Odd
        jpo     $
        jnp     $
;;; Jump on Not Overflow
        jno     $
;;; Jump on Not Sign
        jns     $
;;; Loop CX Times
        loop    $
;;; Loop While Zero/Equal
        loope   $
        loopz   $
;;; Loop While Not Zero/Equal
        loopne  $
        loopnz  $
;;; Jump if CX is zero
        jcxz    $
;;; Call interrupt
        int     2
        int     3
;;; Call interrupt if overflow
        into
;;; Return from interrupt
        iret

;;; Processor control
;;; Clear carry flag
        clc
;;; Complement carry flag
        cmc
;;; Set carry flag
        stc
;;; Clear direction flag
        cld
;;; Set direction flag
        std
;;; Clear interrupt flag
        cli
;;; Set interrupt flag
        sti
;;; Enter halt state
        hlt
;;; Wait until not busy
        wait
;;; Assert BUS LOCK# signal
        lock
;;; No operation
        nop

;;; Segment override
        mov     es:[bx],ah
        mov     es:[bp],ah
        mov     es:[si],ah
        mov     es:[di],ah
        mov     es:[1234H],ah
        mov     cs:[bx],ah
        mov     cs:[bp],ah
        mov     cs:[si],ah
        mov     cs:[di],ah
        mov     cs:[1234H],ah
        mov     ss:[bx],ah
        mov     ss:[bp],ah
        mov     ss:[si],ah
        mov     ss:[di],ah
        mov     ss:[1234H],ah
        mov     ds:[bx],ah
        mov     ds:[bp],ah
        mov     ds:[si],ah
        mov     ds:[di],ah
        mov     ds:[1234H],ah

	jmp     es:[si]
        jmp     cs:[si]
        jmp     ss:[si]
        jmp     ds:[si]

        end

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
        
