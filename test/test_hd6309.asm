	CPU     6309
        ORG     $1000

        NOP
        SYNC
        DAA
        SEX
        RTS
        ABX
        RTI
        MUL
        SWI
        SWI2
        SWI3

        NEGA
        COMA
        LSRA
        RORA
        ASRA
        ASLA
        ROLA
        DECA
        INCA
        TSTA
        CLRA

        NEGB
        COMB
        LSRB
        RORB
        ASRB
        ASLB
        ROLB
        DECB
        INCB
        TSTB
        CLRB

        SEXW
        PSHSW
        PULSW
        PSHUW
        PULUW

        NEGD
        COMD
        LSRD
        RORD
        ASRD
        ASLD
        ROLD
        DECD
        INCD
        TSTD
        CLRD

        COME
        DECE
        INCE
        TSTE
        CLRE

        COMF
        DECF
        INCF
        TSTF
        CLRF

        PSHS A
        PULS A
        PSHU A
        PULU A

        PSHS CC
        PSHS A
        PSHS B
        PSHS DP
        PSHS X
        PSHS Y
        PSHS U
        PSHS PC
        PSHS D

        PULU CC
        PULU A
        PULU B
        PULU DP
        PULU X
        PULU Y
        PULU S
        PULU PC
        PULU D

        PSHS CC,A,B,DP,X,Y,U,PC
        PULS PC,A,DP,B,X,U,Y,CC
        PSHU PC,S,Y,X,DP,B,A,CC
        PULU DP,CC,PC,A,B,S,Y,X
        PSHS PC,U,Y,X,DP,D,CC
        PULS CC,D,DP,X,U,Y,PC

        PSHS #0

        EXG A,B
        TFR X,Y
        ADDR A,B
        ADCR A,B
        SUBR A,B
        SBCR A,B
        ANDR A,B
        ORR A,B
        EORR A,B
        CMPR A,B

        TFR A,B
        TFR A,CC
        TFR A,DP
        TFR A,Z
        TFR A,E
        TFR A,F
        TFR B,A
        TFR CC,A
        TFR DP,A
        TFR Z,A
        TFR E,A
        TFR F,A

        TFR D,X
        TFR D,Y
        TFR D,U
        TFR D,S
        TFR D,PC
        TFR D,W
        TFR D,V
        TFR X,D
        TFR Y,D
        TFR U,D
        TFR S,D
        TFR PC,D
        TFR W,D
        TFR V,D

	org     $1000
        BRA $1002
        BRN $1000
        BHI $1004
        BLS $1002
        BHS $1002
        BLO $1002
        BNE $1002
        BEQ $1002
        BVC $1002
        BVS $1002
        BPL $1002
        BMI $1002
        BGE $1002
        BLT $1002
        BGT $1002
        BLE $1002

        LBRA $1003
        LBRN $1000
        LBHI $1008
        LBLS $1004
        LBHS $1004
        LBLO $1004
        LBNE $1004
        LBEQ $1004
        LBVC $1004
        LBVS $1004
        LBPL $1004
        LBMI $1004
        LBGE $1004
        LBLT $1004
        LBGT $1004
        LBLE $1004

        BSR  $1042
        LBSR $1043

sub0F82:        equ     $0F82
sub1081:        equ     $1081
sub9002:        equ     $9002
sub9003:        equ     $9003

	org     $1000
        BSR  sub1081
	org     $1000
        BSR  sub0F82
	org     $1000
        LBSR sub9002
	org     $1000
        LBSR sub9003

        ORCC  #$10
        ANDCC #$FE
        CWAI  #$EF

        LDMD  #$01
        BITMD #$80

        SUBA #$90
        CMPA #$90
        SBCA #$90
        ANDA #$90
        BITA #$90
        LDA  #$90
        EORA #$90
        ADCA #$90
        ORA  #$90
        ADDA #$90

        SUBB #$90
        CMPB #$90
        SBCB #$90
        ANDB #$90
        BITB #$90
        LDB  #$90
        EORB #$90
        ADCB #$90
        ORB  #$90
        ADDB #$90

        SUBD #$90A0
        ADDD #$90A0
        LDD  #$90A0
        CMPD #$90A0
        SBCD #$90A0
        ANDD #$90A0
        BITD #$90A0
        EORD #$90A0
        ADCD #$90A0
        ORD  #$90A0

        CMPX #$90A0
        LDX  #$90A0

        CMPY #$90A0
        LDY  #$90A0

        CMPU #$90A0
        LDU  #$90A0

        CMPS #$90A0
        LDS  #$90A0

        SUBE #$90
        ADDE #$90
        LDE  #$90
        CMPE #$90

        SUBF #$90
        ADDF #$90
        LDF  #$90
        CMPF #$90

        SUBW #$90A0
        ADDW #$90A0
        LDW  #$90A0
        CMPW #$90A0

        LDQ  #$12345678

        MULD #$90A0
        DIVD #$90A0
        DIVQ #$90A0

dir90:  equ     $90
sym90A0:        equ     $90A0

        LDA  #dir90
        CMPX #sym90A0
        LDY  #sym90A0
        LDS  #sym90A0

        SBCD #sym90A0
        LDE  #dir90

        LDQ  #sym90A0
        MULD #dir90

        NEG $10
        COM $10
        LSR $10
        ROR $10
        ASR $10
        ASL $10
        ROL $10
        DEC $10
        INC $10
        TST $10
        CLR $10

        SUBA $90
        CMPA $90
        SBCA $90
        ANDA $90
        BITA $90
        LDA  $90
        STA  $90
        EORA $90
        ADCA $90
        ORA  $90
        ADDA $90

	assume  dpr:$12
        SUBB <$1290
        CMPB <$1290
        SBCB <$1290
        ANDB <$1290
        BITB <$1290
        LDB  <$1290
        STB  <$1290
        EORB <$1290
        ADCB <$1290
        ORB  <$1290
        ADDB <$1290

        SUBD $90
        ADDD $90
        LDD  $90
        STD  <$1290
        CMPD <$1290
        SBCD $90
        ANDD $90
        BITD $90
        EORD <$1290
        ADCD <$1290
        ORD  <$1290

        CMPX $90
        LDX  $90
        STX  <$1290

        CMPY $90
        LDY  $90
        STY  <$1290

        CMPU $90
        LDU  $90
        STU  <$1290

        CMPS $90
        LDS  $90
        STS  <$1290

        JMP $10
        JSR <$1290

        SUBE $90
        ADDE $90
        LDE  $90
        STE  $90
        CMPE $90

        SUBF <$1290
        ADDF <$1290
        LDF  <$1290
        STF  <$1290
        CMPF <$1290

        SUBW $90
        ADDW $90
        LDW  <$1290
        STW  <$1290
        CMPW <$1290

        LDQ  $90
        STQ  <$1290

        MULD <$1290
        DIVD $90
        DIVQ <$1290

        OIM #$30,$10
        AIM #$30,$10
        EIM #$30,<$1290
        TIM #$30,<$1290

dir10:  equ     $10
sym1290:        equ     $1290

	assume  dpr:0
        NEG  dir10
        LDA  dir10
	assume  dpr:$12
        STB  <sym1290
        CMPX <sym1290
	assume  dpr:0
        STU  <dir10
        LDY  <dir10
	assume  dpr:$12
        LDS  <sym1290
        JMP  <sym1290
	assume  dpr:0
        JSR  dir90

	assume  dpr:$90
        SBCD <sym90A0
	assume  dpr:0
        LDE  dir90

	assume  dpr:$12
        LDQ  <sym1290
        assume  dpr:0
        MULD dir90

        OIM #$30,dir10
	assume  dpr:$12	
        OIM #$30,<sym1290

        NEG $1234
        COM $1234
        LSR $1234
        ROR $1234
        ASR $1234
        ASL $1234
        ROL $1234
        DEC $1234
        INC $1234
        TST $1234
        CLR $1234

        SUBA >$90
        CMPA >$90
        SBCA >$90
        ANDA >$90
        BITA >$90
        LDA  >$90
        STA  >$90
        EORA >$90
        ADCA >$90
        ORA  >$90
        ADDA >$90

        SUBB $9ABC
        CMPB $9ABC
        SBCB $9ABC
        ANDB $9ABC
        BITB $9ABC
        LDB  $9ABC
        STB  $9ABC
        EORB $9ABC
        ADCB $9ABC
        ORB  $9ABC
        ADDB $9ABC

        SUBD $9ABC
        ADDD $9ABC
        LDD  $9ABC
        STD  $9ABC
        CMPD >$90
        SBCD $9ABC
        ANDD $9ABC
        BITD $9ABC
        EORD $9ABC
        ORD  >$90
        ADCD $9ABC

        CMPX $9ABC
        LDX  >$90
        STX  $9ABC

        CMPY $9ABC
        LDY  >$90
        STY  $9ABC

        CMPU $9ABC
        LDU  >$90
        STU  $9ABC

        CMPS $9ABC
        LDS  >$90
        STS  $9ABC

        JMP $1234
        JSR >$90

        SUBE $9ABC
        ADDE $9ABC
        LDE  >$90
        STE  $9ABC
        CMPE $9ABC

        SUBF $9ABC
        ADDF $9ABC
        LDF  >$90
        STF  $9ABC
        CMPF $9ABC

        SUBW $9ABC
        ADDW $9ABC
        LDW  >$90
        STW  $9ABC
        CMPW $9ABC

        LDQ  >$90
        STQ  $1290

        MULD $1290
        DIVD >$90
        DIVQ $1290

        OIM #$30,>$10
        AIM #$30,>$10
        EIM #$30,$1290
        TIM #$30,$1290

        NEG >dir90
        LDA  sym1290
        STB >dir90
        CMPX sym1290
        STU >dir90
        LDY  sym1290
        LDS >dir90
        JMP  sym1290
        JSR >dir90

        SBCD sym1290
        LDE  >dir90

        LDQ  sym1290
        MULD >dir90

        OIM #$30,>dir90
        OIM #$30,sym1290

        LEAX ,Y
        LEAY ,U
        LEAU ,S
        LEAS ,X

        NEG  ,X
        COM  ,X
        LSR  ,X
        ROR  ,X
        ASR  ,X
        ASL  ,X
        ROL  ,X
        DEC  ,X
        INC  ,X
        TST  ,X
        CLR  ,X

        SUBA ,Y
        CMPA ,Y
        SBCA ,Y
        ANDA ,Y
        BITA ,Y
        LDA  ,Y
        STA  ,Y
        EORA ,Y
        ADCA ,Y
        ORA  ,Y
        ADDA ,Y

        SUBB ,U
        CMPB ,U
        SBCB ,U
        ANDB ,U
        BITB ,U
        LDB  ,U
        STB  ,U
        EORB ,U
        ADCB ,U
        ORB  ,U
        ADDB ,U

        SUBD ,S
        ADDD ,S
        LDD  ,S
        STD  ,S
        CMPD ,S
        SBCD ,W
        ANDD ,W++
        BITD ,--W
        EORD [,W]
        ADCD [,W++]
        ORD  [,--W]

        CMPX 2,Y
        LDX  2,Y
        STX  2,Y

        CMPY -2,X
        LDY  -2,X
        STY  -2,X

        CMPU ,X++
        LDU  ,X++
        STU  ,X++

        CMPS ,--Y
        LDS  ,--Y
        STS  ,--Y

        JMP  [,X]
        JSR  [,X++]

        SUBE ,X
        ADDE ,X
        LDE  ,X
        STE  ,X
        CMPE ,X

        SUBF ,X
        ADDF ,X
        LDF  ,X
        STF  ,X
        CMPF ,X

        SUBW ,X
        CMPW ,X
        LDW  ,X
        STW  ,X
        ADDW ,X

        LDQ  ,X
        STQ  ,X

        MULD ,X
        DIVD -1,X
        DIVQ 1,X

        OIM  #$30,,X
        AIM  #$30,,X+
        EIM  #$30,[,--X]
        TIM  #$30,-2,X

        LDA ,X
        LDA ,Y
        LDA ,U
        LDA ,S
        LDA [,X]
        LDA [,Y]
        LDA [,U]
        LDA [,S]

        LDA 0,X
        LDA 0,Y
        LDA 0,U
        LDA 0,S
        LDA -1,X
        LDA -1,Y
        LDA -1,U
        LDA -1,S
        LDA 15,X
        LDA 15,Y
        LDA 15,U
        LDA 15,S
        LDA -16,X
        LDA -16,Y
        LDA -16,U
        LDA -16,S

        LDA 0,X
        LDA 127,X
        LDA -128,X
        LDA [0,X]
        LDA [127,X]
        LDA [-128,X]
        LDA 0,Y
        LDA 127,Y
        LDA -128,Y
        LDA [0,Y]
        LDA [127,Y]
        LDA [-128,Y]
        LDA 0,U
        LDA 127,U
        LDA -128,U
        LDA [0,U]
        LDA [127,U]
        LDA [-128,U]
        LDA 0,S
        LDA 127,S
        LDA -128,S
        LDA [0,S]
        LDA [127,S]
        LDA [-128,S]

        LDA 0,X
        LDA 32767,X
        LDA -32768,X
        LDA [0,X]
        LDA [32767,X]
        LDA [-32768,X]
        LDA 0,Y
        LDA 32767,Y
        LDA -32768,Y
        LDA [0,Y]
        LDA [32767,Y]
        LDA [-32768,Y]
        LDA 0,U
        LDA 32767,U
        LDA -32768,U
        LDA [0,U]
        LDA [32767,U]
        LDA [-32768,U]
        LDA 0,S
        LDA 32767,S
        LDA -32768,S
        LDA [0,S]
        LDA [32767,S]
        LDA [-32768,S]

        LDA A,X
        LDA B,X
        LDA D,X
        LDA [A,X]
        LDA [B,X]
        LDA [D,X]

        LDA A,Y
        LDA B,Y
        LDA D,Y
        LDA [A,Y]
        LDA [B,Y]
        LDA [D,Y]

        LDA A,U
        LDA B,U
        LDA D,U
        LDA [A,U]
        LDA [B,U]
        LDA [D,U]

        LDA A,S
        LDA B,S
        LDA D,S
        LDA [A,S]
        LDA [B,S]
        LDA [D,S]

        LDA ,X+
        LDA ,X++
        LDA ,-X
        LDA ,--X
        LDA [,X++]
        LDA [,--X]

        LDA ,Y+
        LDA ,Y++
        LDA ,-Y
        LDA ,--Y
        LDA [,Y++]
        LDA [,--Y]

        LDA ,U+
        LDA ,U++
        LDA ,-U
        LDA ,--U
        LDA [,U++]
        LDA [,--U]

        LDA ,S+
        LDA ,S++
        LDA ,-S
        LDA ,--S
        LDA [,S++]
        LDA [,--S]

        LDA $1003,PC
        LDA $1082,PC
        LDA $0F83,PC
        LDA [$1003,PC]
        LDA [$1082,PC]
        LDA [$0F83,PC]

        LDA $1004,PC
        LDA $9003,PC
        LDA $9004,PC
        LDA [$1004,PC]
        LDA [$9003,PC]
        LDA [$9004,PC]

        LDA [$1234]

        LDA E,X
        LDA F,X
        LDA W,X
        LDA [E,X]
        LDA [F,X]
        LDA [W,X]

        LDA E,Y
        LDA F,Y
        LDA W,Y
        LDA [E,Y]
        LDA [F,Y]
        LDA [W,Y]

        LDA E,U
        LDA F,U
        LDA W,U
        LDA [E,U]
        LDA [F,U]
        LDA [W,U]

        LDA E,S
        LDA F,S
        LDA W,S
        LDA [E,S]
        LDA [F,S]
        LDA [W,S]

        LDA ,W
        LDA 0,W
        LDA 32767,W
        LDA -32768,W
        LDA ,W++
        LDA ,--W
        LDA [,W]
        LDA [0,W]
        LDA [32767,W]
        LDA [-32768,W]
        LDA [,W++]
        LDA [,--W]

label0F83:      equ     $0F83
label1003:      equ     $1003
label1004:      equ     $1004
label1082:      equ     $1082
label9003:      equ     $9003
label9004:      equ     $9004
label1234:      equ     $1234

	org     $1000
        LDA label1003,PC
	org     $1000
        LDA label1082,PC
	org     $1000
        LDA label0F83,PC
	org     $1000
        LDA [label1003,PC]
	org     $1000
        LDA [label1082,PC]
	org     $1000
        LDA [label0F83,PC]

	org     $1000
        LDA label1004,PC
	org     $1000
        LDA label9003,PC
	org     $1000
        LDA label9004,PC
	org     $1000
        LDA [label1004,PC]
	org     $1000
        LDA [label9003,PC]
	org     $1000
        LDA [label9004,PC]

        LDA [label1234]

        TFM X+,Y+
        TFM X-,Y-
        TFM X+,Y
        TFM X,Y+

        TFM D+,X+
        TFM Y+,U+
        TFM U+,S+
        TFM S+,D+

	assume  dpr:0
        BAND  A.1,$34.2
        BIAND A.1,$34.2
        BOR   A.1,$34.2
        BIOR  A.1,$34.2
        BEOR  A.1,$34.2
        BIEOR A.1,$34.2
        LDBT  A.1,$34.2
        STBT  A.1,$34.2

        LDBT CC.0,$34.7
        LDBT CC.1,$34.7
        LDBT CC.2,$34.7
        LDBT CC.3,$34.7
        LDBT CC.4,$34.7
        LDBT CC.5,$34.7
        LDBT CC.6,$34.7
        LDBT CC.7,$34.7
        LDBT A.7,$34.0
        LDBT B.2,$34.4

dir34:  equ     $34
bit0:   equ     0
bit2:   equ     2
bit4:   equ     4
bit7:   equ     7
sym9030:        equ     $9030

        LDBT CC.bit0,dir34.bit7
	assume  dpr:$90
        LDBT B.bit2,<sym9030.bit4
