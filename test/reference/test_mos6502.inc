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

;;; 0X
        brk
        ora     ($02,x)
        ora     $06
        asl     $07
        php
        ora     #$0a
        asl     a
        ora     $0f0e
        asl     $100f
;;; 1X
        bpl     *+$13
        ora     ($12),y
        ora     $16,x
        asl     $17,x
        clc
        ora     $1b1a,y
        ora     $1f1e,x
        asl     $201f,x
;;; 2X
        jsr     $2221
        and     ($22,x)
        bit     $25
        and     $26
        rol     $27
        plp
        and     #$2a
        rol     a
        bit     $2e3d
        and     $2f2e
        rol     $302f
;;; 3X
        bmi     *+$33
        and     ($32),y
        and     $36,x
        rol     $37,x
        sec
        and     $3b3a,y
        and     $3f3e,x
        rol     $403f,x
;;; 4X
        rti
        eor     ($42,x)
        eor     $46
        lsr     $47
        pha
        eor     #$4a
        lsr     a
        jmp     $4e4d
        eor     $4f4e
        lsr     $504f
;;; 5X
        bvc     *+$53
        eor     ($52),y
        eor     $56,x
        lsr     $57,x
        cli
        eor     $5b5a,y
        eor     $5f5e,x
        lsr     $605f,x
;;; 6X
        rts
        adc     ($62,x)
        adc     $66
        ror     $67
        pla
        adc     #$6a
        ror     a
        jmp     ($6e6d)
        adc     $6f6e
        ror     $706f
;;; 7X
        bvs     *+$73
        adc     ($72),y
        adc     $76,x
        ror     $77,x
        sei
        adc     $7b7a,y
        adc     $7f7e,x
        ror     $807f,x
;;; 8X
        sta     ($82,x)
        sty     $85
        sta     $86
        stx     $87
        dey
        txa
        sty     $8e8d
        sta     $8f8e
        stx     $908f
;;; 9X
        bcc     *-109
        sta     ($92),y
        sty     $95,x
        sta     $96,x
        stx     $97,y
        tya
        sta     $9b9a,y
        txs
        sta     $9f9e,x
;;; AX
        ldy     #$a1
        lda     ($a2,x)
        ldx     #$a3
        ldy     $a5
        lda     $a6
        ldx     $a7
        tay
        lda     #$aa
        tax
        ldy     $aead
        lda     $afae
        ldx     $b0af
;;; BX
        bcs     *-77
        lda     ($b2),y
        ldy     $b5,x
        lda     $b6,x
        ldx     $b7,y
        clv
        lda     $bbba,y
        tsx
        ldy     $bebd,x
        lda     $bfbe,x
        ldx     $c0bf,y
;;; CX
        cpy     #$c1
        cmp     ($c2,x)
        cpy     $c5
        cmp     $c6
        dec     $c7
        iny
        cmp     #$ca
        dex
        cpy     $cecd
        cmp     $cfce
        dec     $d0cf
;;; DX
        bne     *-45
        cmp     ($d2),y
        cmp     $d6,x
        dec     $d7,x
        cld
        cmp     $dbda,y
        cmp     $dfde,x
        dec     $e0df,x
;;; EX
        cpx     #$e1
        sbc     ($e2,x)
        cpx     $e5
        sbc     $e6
        inc     $e7
        inx
        sbc     #$ea
        nop
        cpx     $eeed
        sbc     $efee
        inc     $f0ef
;;; FX
        beq     *-13
        sbc     ($f2),y
        sbc     $f6,x
        inc     $f7,x
        sed
        sbc     $fbfa,y
        sbc     $fffe,x
        inc     >$00ff,x

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
