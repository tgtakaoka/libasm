;;; AUTO GENERATED FILE
;;; generated by: gen_mc6800 -u -C 6811 -o gen_mc68hc11.asm -l gen_mc68hc11.lst
      CPU   6811
      ORG   $0100
      NOP
      IDIV
      FDIV
      LSRD
      ASLD
      TAP
      TPA
      INX
      DEX
      CLV
      SEV
      CLC
      SEC
      CLI
      SEI
      SBA
      CBA
      BRSET $13,#20,*+25
      BRSET $13,#19,*-124
      BRSET $13,#19,*
      BRCLR $14,#21,*+26
      BRCLR $14,#20,*-124
      BRCLR $14,#20,*
      BSET  $15,#22
      BCLR  $16,#23
      TAB
      TBA
      BSET  29,Y,#30
      BCLR  30,Y,#31
      BRSET 31,Y,#32,*+38
      BRCLR 32,Y,#$21,*+39
      TSY
      TYS
      PULY
      ABY
      PSHY
      NEG   97,Y
      COM   100,Y
      LSR   101,Y
      ROR   103,Y
      ASR   104,Y
      ASL   105,Y
      ROL   106,Y
      DEC   107,Y
      INC   109,Y
      TST   110,Y
      JMP   111,Y
      CLR   112,Y
      CPY   #$8D8E
      XGDY
      CPY   $9D
      SUBA  161,Y
      CMPA  162,Y
      SBCA  163,Y
      SUBD  164,Y
      ANDA  165,Y
      BITA  166,Y
      LDAA  167,Y
      STAA  168,Y
      EORA  169,Y
      ADCA  170,Y
      ORAA  171,Y
      ADDA  172,Y
      CPY   173,Y
      JSR   174,Y
      LDS   175,Y
      STS   176,Y
      CPY   $BDBE
      CPY   >$00BE
      LDY   #$CFD0
      LDY   $DF
      STY   $E0
      SUBB  225,Y
      CMPB  226,Y
      SBCB  227,Y
      ADDD  228,Y
      ANDB  229,Y
      BITB  230,Y
      LDAB  231,Y
      STAB  232,Y
      EORB  233,Y
      ADCB  234,Y
      ORAB  235,Y
      ADDB  236,Y
      LDD   237,Y
      STD   238,Y
      LDY   239,Y
      STY   240,Y
      LDY   $FF00
      LDY   >$0000
      STY   >$0001
      STY   $0101
      INY
      DEY
      DAA
      CPD   #$8485
      CPD   $94
      CPD   164,X
      CPY   173,X
      CPD   $B4B5
      CPD   >$00B5
      LDY   239,X
      STY   240,X
      ABA
      BSET  29,X,#30
      BCLR  30,X,#31
      BRSET 31,X,#32,*+37
      BRSET 31,X,#31,*-124
      BRSET 31,X,#31,*
      BRCLR 32,X,#$21,*+38
      BRCLR 32,X,#32,*-124
      BRCLR 32,X,#32,*
      BRA   *+35
      BRA   *-126
      BRA   *
      BRN   *+36
      BRN   *-126
      BRN   *
      BHI   *+37
      BHI   *-126
      BHI   *
      BLS   *+38
      BLS   *-126
      BLS   *
      BHS   *+39
      BHS   *-126
      BHS   *
      BLO   *+40
      BLO   *-126
      BLO   *
      BNE   *+41
      BNE   *-126
      BNE   *
      BEQ   *+42
      BEQ   *-126
      BEQ   *
      BVC   *+43
      BVC   *-126
      BVC   *
      BVS   *+44
      BVS   *-126
      BVS   *
      BPL   *+45
      BPL   *-126
      BPL   *
      BMI   *+46
      BMI   *-126
      BMI   *
      BGE   *+47
      BGE   *-126
      BGE   *
      BLT   *+48
      BLT   *-126
      BLT   *
      BGT   *+49
      BGT   *-126
      BGT   *
      BLE   *+50
      BLE   *-126
      BLE   *
      TSX
      INS
      PULA
      PULB
      DES
      TXS
      PSHA
      PSHB
      PULX
      RTS
      ABX
      RTI
      PSHX
      MUL
      WAI
      SWI
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
      NEG   97,X
      COM   100,X
      LSR   101,X
      ROR   103,X
      ASR   104,X
      ASL   105,X
      ROL   106,X
      DEC   107,X
      INC   109,X
      TST   110,X
      JMP   111,X
      CLR   112,X
      NEG   $7172
      COM   $7475
      LSR   $7576
      ROR   $7778
      ASR   $7879
      ASL   $797A
      ROL   $7A7B
      DEC   $7B7C
      INC   $7D7E
      TST   $7E7F
      JMP   $7F80
      CLR   $8081
      SUBA  #$81
      CMPA  #$82
      SBCA  #$83
      SUBD  #$8485
      ANDA  #$85
      BITA  #$86
      LDAA  #$87
      EORA  #$89
      ADCA  #$8A
      ORAA  #$8B
      ADDA  #$8C
      CPX   #$8D8E
      BSR   *-112
      BSR   *
      BSR   *+1
      LDS   #$8F90
      XGDX
      SUBA  $91
      CMPA  $92
      SBCA  $93
      SUBD  $94
      ANDA  $95
      BITA  $96
      LDAA  $97
      STAA  $98
      EORA  $99
      ADCA  $9A
      ORAA  $9B
      ADDA  $9C
      CPX   $9D
      JSR   $9E
      LDS   $9F
      STS   $A0
      SUBA  161,X
      CMPA  162,X
      SBCA  163,X
      SUBD  164,X
      ANDA  165,X
      BITA  166,X
      LDAA  167,X
      STAA  168,X
      EORA  169,X
      ADCA  170,X
      ORAA  171,X
      ADDA  172,X
      CPX   173,X
      JSR   174,X
      LDS   175,X
      STS   176,X
      SUBA  $B1B2
      CMPA  $B2B3
      SBCA  $B3B4
      SUBD  $B4B5
      ANDA  $B5B6
      BITA  $B6B7
      LDAA  $B7B8
      STAA  $B8B9
      EORA  $B9BA
      ADCA  $BABB
      ORAA  $BBBC
      ADDA  $BCBD
      CPX   $BDBE
      JSR   $BEBF
      LDS   $BFC0
      STS   $C0C1
      SUBB  #$C1
      CMPB  #$C2
      SBCB  #$C3
      ADDD  #$C4C5
      ANDB  #$C5
      BITB  #$C6
      LDAB  #$C7
      EORB  #$C9
      ADCB  #$CA
      ORAB  #$CB
      ADDB  #$CC
      LDD   #$CDCE
      LDX   239,Y
      STX   240,Y
      CPD   164,Y
      CPX   173,Y
      LDX   #$CFD0
      STOP
      SUBB  $D1
      CMPB  $D2
      SBCB  $D3
      ADDD  $D4
      ANDB  $D5
      BITB  $D6
      LDAB  $D7
      STAB  $D8
      EORB  $D9
      ADCB  $DA
      ORAB  $DB
      ADDB  $DC
      LDD   $DD
      STD   $DE
      LDX   $DF
      STX   $E0
      SUBB  225,X
      CMPB  226,X
      SBCB  227,X
      ADDD  228,X
      ANDB  229,X
      BITB  230,X
      LDAB  231,X
      STAB  232,X
      EORB  233,X
      ADCB  234,X
      ORAB  235,X
      ADDB  236,X
      LDD   237,X
      STD   238,X
      LDX   239,X
      STX   240,X
      SUBB  $F1F2
      CMPB  $F2F3
      SBCB  $F3F4
      ADDD  $F4F5
      ANDB  $F5F6
      BITB  $F6F7
      LDAB  $F7F8
      STAB  $F8F9
      EORB  $F9FA
      ADCB  $FAFB
      ORAB  $FBFC
      ADDB  $FCFD
      LDD   $FDFE
      STD   $FEFF
      STD   >$0001
      LDX   $FF00
      LDX   >$0001
      STX   >$0001
      STX   $0102