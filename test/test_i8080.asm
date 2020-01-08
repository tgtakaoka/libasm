	CPU     8080
	ORG     1000H

        MOV B,B
        MOV B,C
        MOV B,D
        MOV B,E
        MOV B,H
        MOV B,L
        MOV B,M
        MOV B,A
        MOV C,B
        MOV C,C
        MOV C,D
        MOV C,E
        MOV C,H
        MOV C,L
        MOV C,M
        MOV C,A

        MOV D,B
        MOV D,C
        MOV D,D
        MOV D,E
        MOV D,H
        MOV D,L
        MOV D,M
        MOV D,A
        MOV E,B
        MOV E,C
        MOV E,D
        MOV E,E
        MOV E,H
        MOV E,L
        MOV E,M
        MOV E,A

        MOV H,B
        MOV H,C
        MOV H,D
        MOV H,E
        MOV H,H
        MOV H,L
        MOV H,M
        MOV H,A
        MOV L,B
        MOV L,C
        MOV L,D
        MOV L,E
        MOV L,H
        MOV L,L
        MOV L,M
        MOV L,A

        MOV M,B
        MOV M,C
        MOV M,D
        MOV M,E
        MOV M,H
        MOV M,L

        MOV M,A
        MOV A,B
        MOV A,C
        MOV A,D
        MOV A,E
        MOV A,H
        MOV A,L
        MOV A,M
        MOV A,A

        STAX B
        STAX D
        LDAX B
        LDAX D

        MVI B,0F6H
        MVI C,9FH
        MVI D,58
        MVI E,-128
        MVI H,127
        MVI L,-10
        MVI M,0F6H
        MVI A,0FEH

        LXI B,0BEEFH
        LXI D,1234H
        LXI H,-16657
        LXI SP,6789H

label1234:      equ     1234H
label5678:      equ     5678H
label6789:      equ     6789H
label9ABC:      equ     9ABCH
ABCD:           equ     0ABCDH
BEEF:           equ     0BEEFH

        LXI B,BEEF
        LXI D,label1234
        LXI H,BEEF
        LXI SP,label6789

        STA 9ABCH
        LDA 1234H

        SHLD 0ABCDH
        LHLD 5678H

        STA label9ABC
        LDA label1234

        SHLD ABCD
        LHLD label5678

        PUSH B
        PUSH D
        PUSH H
        PUSH PSW
        POP  B
        POP  D
        POP  H
        POP  PSW

        XTHL
        PCHL
        SPHL
        XCHG

        JMP 1234H
        JNZ 1234H
        JZ 1234H
        JNC 1234H
        JC 1234H
        JPO 1234H
        JPE 1234H
        JP 1234H
        JM 1234H

        CALL 1234H
        CNZ 1234H
        CZ 1234H
        CNC 1234H
        CC 1234H
        CPO 1234H
        CPE 1234H
        CP 1234H
        CM 1234H

        RET
        RNZ
        RZ
        RNC
        RC
        RPO
        RPE
        RP
        RM

        JMP label1234
        CC  label1234

        INR B
        INR C
        INR D
        INR E
        INR H
        INR L
        INR M
        INR A

        DCR B
        DCR C
        DCR D
        DCR E
        DCR H
        DCR L
        DCR M
        DCR A

        INX B
        INX D
        INX H
        INX SP
        DCX B
        DCX D
        DCX H
        DCX SP

        RST 0
        RST 1
        RST 2
        RST 3
        RST 4
        RST 5
        RST 6
        RST 7

        ADD B
        ADD C
        ADD D
        ADD E
        ADD H
        ADD L
        ADD M
        ADD A

        ADC B
        ADC C
        ADC D
        ADC E
        ADC H
        ADC L
        ADC M
        ADC A

        SUB B
        SUB C
        SUB D
        SUB E
        SUB H
        SUB L
        SUB M
        SUB A

        SBB B
        SBB C
        SBB D
        SBB E
        SBB H
        SBB L
        SBB M
        SBB A

        ANA B
        ANA C
        ANA D
        ANA E
        ANA H
        ANA L
        ANA M
        ANA A

        XRA B
        XRA C
        XRA D
        XRA E
        XRA H
        XRA L
        XRA M
        XRA A

        ORA B
        ORA C
        ORA D
        ORA E
        ORA H
        ORA L
        ORA M
        ORA A

        CMP B
        CMP C
        CMP D
        CMP E
        CMP H
        CMP L
        CMP M
        CMP A

        RLC
        RRC
        RAL
        RAR

        DAA
        CMA
        STC
        CMC

        DAD B
        DAD D
        DAD H
        DAD SP

        DI
        EI

        NOP
        HLT

        ADI 10B
        ACI 255

        SUI -2
        SBI 127

        ANI 377O
        XRI 0FFH
        ORI -1
        CPI -128

        OUT 0F1H
        IN  0F0H
