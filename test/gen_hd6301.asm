;;; AUTO GENERATED FILE
;;; generated by: gen_mc6800 -u -C 6301 -o gen_hd6301.asm

      NOP
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
      TAB
      TBA
      XGDX
      DAA
      SLP
      ABA
      BRA  $0038
      BRN  $003B
      BHI  $003E
      BLS  $0041
      BHS  $0044
      BLO  $0047
      BNE  $004A
      BEQ  $004D
      BVC  $0050
      BVS  $0053
      BPL  $0056
      BMI  $0059
      BGE  $005C
      BLT  $005F
      BGT  $0062
      BLE  $0065
      TSX
      INS
      PUL  A
      PUL  B
      DES
      TXS
      PSH  A
      PSH  B
      PULX
      RTS
      ABX
      RTI
      PSHX
      MUL
      WAI
      SWI
      NEG  A
      COM  A
      LSR  A
      ROR  A
      ASR  A
      ASL  A
      ROL  A
      DEC  A
      INC  A
      TST  A
      CLR  A
      NEG  B
      COM  B
      LSR  B
      ROR  B
      ASR  B
      ASL  B
      ROL  B
      DEC  B
      INC  B
      TST  B
      CLR  B
      NEG  97,X
      COM  100,X
      LSR  101,X
      ROR  103,X
      ASR  104,X
      ASL  105,X
      ROL  106,X
      DEC  107,X
      INC  109,X
      TST  110,X
      JMP  111,X
      CLR  112,X
      NEG  >$0071
      AIM  #0,$72
      OIM  #0,$73
      COM  >$0074
      LSR  >$0075
      EIM  #0,$76
      ROR  >$0077
      ASR  >$0078
      ASL  >$0079
      ROL  >$007A
      DEC  >$007B
      TIM  #0,$7C
      INC  >$007D
      TST  >$007E
      JMP  >$007F
      CLR  >$0080
      SUB  A #$81
      CMP  A #$82
      SBC  A #$83
      SUBD #$84
      AND  A #$85
      BIT  A #$86
      LDA  A #$87
      EOR  A #$89
      ADC  A #$8A
      ORA  A #$8B
      ADD  A #$8C
      CPX  #$8D
      BSR  $004D
      LDS  #$8F
      SUB  A $91
      CMP  A $92
      SBC  A $93
      SUBD $94
      AND  A $95
      BIT  A $96
      LDA  A $97
      STA  A $98
      EOR  A $99
      ADC  A $9A
      ORA  A $9B
      ADD  A $9C
      CPX  $9D
      JSR  $9E
      LDS  $9F
      STS  $A0
      SUB  A 161,X
      CMP  A 162,X
      SBC  A 163,X
      SUBD 164,X
      AND  A 165,X
      BIT  A 166,X
      LDA  A 167,X
      STA  A 168,X
      EOR  A 169,X
      ADC  A 170,X
      ORA  A 171,X
      ADD  A 172,X
      CPX  173,X
      JSR  174,X
      LDS  175,X
      STS  176,X
      SUB  A >$00B1
      CMP  A >$00B2
      SBC  A >$00B3
      SUBD >$00B4
      AND  A >$00B5
      BIT  A >$00B6
      LDA  A >$00B7
      STA  A >$00B8
      EOR  A >$00B9
      ADC  A >$00BA
      ORA  A >$00BB
      ADD  A >$00BC
      CPX  >$00BD
      JSR  >$00BE
      LDS  >$00BF
      STS  >$00C0
      SUB  B #$C1
      CMP  B #$C2
      SBC  B #$C3
      ADDD #$C4
      AND  B #$C5
      BIT  B #$C6
      LDA  B #$C7
      EOR  B #$C9
      ADC  B #$CA
      ORA  B #$CB
      ADD  B #$CC
      LDD  #$CD
      LDX  #$CF
      SUB  B $D1
      CMP  B $D2
      SBC  B $D3
      ADDD $D4
      AND  B $D5
      BIT  B $D6
      LDA  B $D7
      STA  B $D8
      EOR  B $D9
      ADC  B $DA
      ORA  B $DB
      ADD  B $DC
      LDD  $DD
      STD  $DE
      LDX  $DF
      STX  $E0
      SUB  B 225,X
      CMP  B 226,X
      SBC  B 227,X
      ADDD 228,X
      AND  B 229,X
      BIT  B 230,X
      LDA  B 231,X
      STA  B 232,X
      EOR  B 233,X
      ADC  B 234,X
      ORA  B 235,X
      ADD  B 236,X
      LDD  237,X
      STD  238,X
      LDX  239,X
      STX  240,X
      SUB  B >$00F1
      CMP  B >$00F2
      SBC  B >$00F3
      ADDD >$00F4
      AND  B >$00F5
      BIT  B >$00F6
      LDA  B >$00F7
      STA  B >$00F8
      EOR  B >$00F9
      ADC  B >$00FA
      ORA  B >$00FB
      ADD  B >$00FC
      LDD  >$00FD
      STD  >$00FE
      LDX  >$00FF
      STX  $0100
      AIM  #$62,0,X
      OIM  #$63,0,X
      EIM  #$66,0,X
      TIM  #$6C,0,X
      AIM  #$FB,$00