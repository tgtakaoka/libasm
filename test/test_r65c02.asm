	include test_r6502.asm

        cpu     65c02

;;; R65C02
	org     $0150

;;; 0X
        tsb     $04
        tsb     $0c0c
;;; 1X
        ora     ($12)
        trb     $14
        inc     a
        trb     $1c1c
;;; 3X
        and     ($32)
        bit     $34,x
        dec     a
        bit     $3c3c,x
;;; 5X
        eor     ($52)
        phy
;;; 6X
        stz     $64
;;; 7X
        adc     ($72)
        stz     $74,x
        ply
        jmp     ($7c7c,x)
;;; 8X
        bra     *+$80
        bit     #$89
;;; 9X
        sta     ($92)
        stz     $9c9c
        stz     $9e9e,x
;;; BX
        lda     ($b2)
;;; DX
        cmp     ($d2)
        phx
;;; FX
        sbc     ($f2)
        plx

        end
;;; comment
