	include test_r6502.asm

        cpu     65c02

;;; R65C02
	org     $0150

;;; 0X
        tsb     $05
        tsb     $0e0d
;;; 1X
        ora     ($13)
        trb     $15
        inc     a
        trb     $1e1d
;;; 3X
        and     ($33)
        bit     $35,x
        dec     a
        bit     $3e3d,x
;;; 5X
        eor     ($53)
        phy
;;; 6X
        stz     $65
;;; 7X
        adc     ($73)
        stz     $75,x
        ply
        jmp     ($7e7d,x)
;;; 8X
        bra     *-125
        bit     #$8a
;;; 9X
        sta     ($93)
        stz     $9e9d
        stz     $a09f,x
;;; BX
        lda     ($b3)
;;; DX
        cmp     ($d3)
        phx
;;; FX
        sbc     ($f3)
        plx

        end
;;; comment
