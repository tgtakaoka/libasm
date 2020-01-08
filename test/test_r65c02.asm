	CPU     65C02
	ORG     $1000

zero10:         equ     $0010
zero90:         equ     $0090
zeroFF:         equ     $00FF
abs0010:        equ     $0010
abs1234:        equ     $1234
abs0100:        equ     $0100

        BRK
        RTI
        RTS
        NOP

        PHP
        PLP
        PHA
        PLA

        DEY
        INY
        DEX
        INX

        TYA
        TAY
        TXA
        TAX
        TXS
        TSX

        CLC
        SEC
        CLI
        SEI
        CLV
        CLD
        SED

        ;; R65C02
        PHY
        PLY
        PHX
        PLX

        ASL A
        ROL A
        LSR A
        ROR A

        ;; R65C02
        INC A
        DEC A

        LDY #0
        LDX #16
        CPY #255
        CPX #$FF

        ORA #$90
        AND #$90
        EOR #$90
        ADC #$90
        LDA #$90
        CMP #$90
        SBC #$90

        ;; R65C02
        BIT #$90

        LDX #zero10
        CPY #zeroFF
        SBC #zero90

        ;; R65C02
        BIT #zero90

        BIT $10
        ORA $10
        AND $10
        EOR $10
        ADC $10
        STA $10
        LDA $10
        CMP $10
        SBC $10

        STY $10
        LDY $10
        CPY $10
        STX $10
        LDX $10
        CPX $10

        DEC $10
        INC $10

        ASL $10
        ROL $10
        LSR $10
        ROR $10

        ;; R65C02
        TSB $10
        TRB $10
        STZ $10

        SBC zero10
        DEC zeroFF
        ROR zero90

        ;; R65C02
        STZ zero10

        ORA $10,X
        AND $10,X
        EOR $10,X
        ADC $10,X
        STA $10,X
        LDA $10,X
        CMP $10,X
        SBC $10,X

        STY $10,X
        LDY $10,X
        STX $10,Y
        LDX $10,Y

        DEC $10,X
        INC $10,X

        ASL $10,X
        ROL $10,X
        LSR $10,X
        ROR $10,X

        ;; R65C02
        BIT $10,X
        STZ $10,X

        SBC zero10,X
        STY zero90,X
        LDY zeroFF,X
        STX zero90,Y
        LDX zeroFF,Y
        ROR zero90,X

	;; R65C02
        BIT zero10,X

        BIT $1234
        ORA $1234
        AND $1234
        EOR $1234
        ADC $1234
        STA $1234
        LDA $1234
        CMP $1234
        SBC $1234

        STY $1234
        LDY $1234
        CPY $1234
        STX $1234
        LDX $1234
        CPX $1234

        DEC $1234
        INC $1234

        ASL $1234
        ROL $1234
        LSR $1234
        ROR $1234

        JMP $1234
        JSR $1234

        ;; R65C02
        TSB $1234
        TRB $1234
        STZ $1234

        SBC >abs0010
        LDX abs1234
        JSR abs0100

        ;; R65C02
        TSB abs1234

        ORA $1234,X
        AND $1234,X
        EOR $1234,X
        ADC $1234,X
        STA $1234,X
        LDA $1234,X
        CMP $1234,X
        SBC $1234,X

        ORA $1234,Y
        AND $1234,Y
        EOR $1234,Y
        ADC $1234,Y
        STA $1234,Y
        LDA $1234,Y
        CMP $1234,Y
        SBC $1234,Y

        LDY $1234,X
        LDX $1234,Y

        DEC $1234,X
        INC $1234,X

        ASL $1234,X
        ROL $1234,X
        LSR $1234,X
        ROR $1234,X

        ;; R65C02
        BIT $1234,X
        STZ $1234,X

        SBC >abs0010,X
        STA abs1234,Y
        LDY abs0100,X
        LDX abs1234,Y
        LSR abs0100,X

        ;; R65C02
        STZ abs1234,X

        JMP ($1234)

        JMP (>abs0010)
        JMP (abs1234)

        ;; R65C02
        JMP ($1234,X)

        ;; R65C02
        JMP (>abs0010,X)
        JMP (abs1234,X)

        ;; R65C02
        ORA ($10)
        AND ($10)
        EOR ($10)
        ADC ($10)
        STA ($10)
        LDA ($10)
        CMP ($10)
        SBC ($10)

        ORA (zero10)
        AND (zeroFF)

        ORA ($10,X)
        AND ($10,X)
        EOR ($10,X)
        ADC ($10,X)
        STA ($10,X)
        LDA ($10,X)
        CMP ($10,X)
        SBC ($10,X)

        LDA (zero10,X)

        ORA ($10),Y
        AND ($10),Y
        EOR ($10),Y
        ADC ($10),Y
        STA ($10),Y
        LDA ($10),Y
        CMP ($10),Y
        SBC ($10),Y

        LDA (zero10),Y

        ;; ;; R65C02
        ;; RMB0 $10
        ;; RMB1 $10
        ;; RMB2 $10
        ;; RMB3 $10
        ;; RMB4 $10
        ;; RMB5 $10
        ;; RMB6 $10
        ;; RMB7 $10
        ;; SMB0 $10
        ;; SMB1 $10
        ;; SMB2 $10
        ;; SMB3 $10
        ;; SMB4 $10
        ;; SMB5 $10
        ;; SMB6 $10
        ;; SMB7 $10

        ;; RMB7 zero10
        ;; SMB0 zero10

        ORG     $2000
        BPL     $2002

        ORG     $2100
        BMI     $2100
        BVC     $2104

        ORG     $2200
        BVS     $2281

        ORG     $2300
        BCC     $2282

        ORG     $2400
        BCS     $2402
        BNE     $2402
        BEQ     $2402
	;; R65C02
        BRA     $2402

label2F82:      equ     $2F82
label3100:      equ     $3100
label3281:      equ     $3281

        ORG     $3000
        BMI     label2F82

        ORG     $3100
        BVS     label3100
        ;; 65C02
        BRA     label3100

        ORG     $3200
        BCC     label3281

;; 	;; R65C02
;; 	ORG     $4000
;;         BBR0 $10,$4003
;;         BBR1 $10,$4000
;;         BBR2 $10,$4006
;;         ORG     $4100
;;         BBR3 $10,$4182
;; 	ORG     $4200
;;         BBR4 $10,$4183
;; 	ORG     $4300
;;         BBR5 $10,$4283
;;         BBR6 $10,$4303
;;         BBR7 $10,$4303
;;         BBS0 $10,$4303
;;         BBS1 $10,$4300
;;         BBS2 $10,$4306
;;         BBS3 $10,$4382
;;         BBS4 $10,$4392
;;         BBS5 $10,$4303
;;         BBS6 $10,$4303
;;         BBS7 $10,$4303

;; label4482:      equ     $4482
;; label4483:      equ     $4483
;; label4500:      equ     $4500
;;         ORG     $4400
;;         BBR3 zero10,label4482
;;         ORG     $4500
;;         BBR4 zero10,label4483
;;         BBS4 zero10,label4500
;;         BBS5 zero10,label4500



