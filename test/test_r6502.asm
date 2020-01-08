	CPU     6502
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

        ASL A
        ROL A
        LSR A
        ROR A

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

        LDX #zero10
        CPY #zeroFF
        SBC #zero90

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

        SBC zero10
        DEC zeroFF
        ROR zero90

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

        SBC zero10,X
        STY zero90,X
        LDY zeroFF,X
        STX zero90,Y
        LDX zeroFF,Y
        ROR zero90,X

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

        SBC >abs0010
        LDX abs1234
        JSR abs0100

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

        SBC >abs0010,X
        STA abs1234,Y
        LDY abs0100,X
        LDX abs1234,Y
        LSR abs0100,X

        JMP ($1234)

        JMP (>abs0010)
        JMP (abs1234)

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

label2F82:      equ     $2F82
label3100:      equ     $3100
label3281:      equ     $3281

        ORG     $3000
        BMI     label2F82

        ORG     $3100
        BVS     label3100

        ORG     $3200
        BCC     label3281
