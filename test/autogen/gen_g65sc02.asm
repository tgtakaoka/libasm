;;; AUTO GENERATED FILE
;;; generated by: gen_mos6502 -u -C 65SC02 -o gen_g65sc02.asm -l gen_g65sc02.lst
      CPU   65SC02
      ORG   $0200
      BRK
      ORA   ($02,X)
      TSB   $05
      ORA   $06
      ASL   $07
      PHP
      ORA   #10
      ASL   A
      TSB   $0E0D
      TSB   >$000D
      ORA   $0F0E
      ORA   >$000E
      ASL   $100F
      ASL   >$000F
      BPL   *+19
      ORA   ($12),Y
      ORA   ($13)
      TRB   $15
      ORA   $16,X
      ASL   $17,X
      CLC
      ORA   $1B1A,Y
      ORA   >$001A,Y
      INC   A
      TRB   $1E1D
      TRB   >$001D
      ORA   $1F1E,X
      ORA   >$001E,X
      ASL   $201F,X
      ASL   >$001F,X
      JSR   $2221
      JSR   >$0021
      AND   ($22,X)
      BIT   $25
      AND   $26
      ROL   $27
      PLP
      AND   #$2A
      ROL   A
      BIT   $2E2D
      BIT   >$002D
      AND   $2F2E
      AND   >$002E
      ROL   $302F
      ROL   >$002F
      BMI   *+51
      AND   ($32),Y
      AND   ($33)
      BIT   $35,X
      AND   $36,X
      ROL   $37,X
      SEC
      AND   $3B3A,Y
      AND   >$003A,Y
      DEC   A
      BIT   $3E3D,X
      BIT   >$003D,X
      AND   $3F3E,X
      AND   >$003E,X
      ROL   $403F,X
      ROL   >$003F,X
      RTI
      EOR   ($42,X)
      EOR   $46
      LSR   $47
      PHA
      EOR   #$4A
      LSR   A
      JMP   $4E4D
      JMP   >$004D
      EOR   $4F4E
      EOR   >$004E
      LSR   $504F
      LSR   >$004F
      BVC   *+83
      EOR   ($52),Y
      EOR   ($53)
      EOR   $56,X
      LSR   $57,X
      CLI
      EOR   $5B5A,Y
      EOR   >$005A,Y
      PHY
      EOR   $5F5E,X
      EOR   >$005E,X
      LSR   $605F,X
      LSR   >$005F,X
      RTS
      ADC   ($62,X)
      STZ   $65
      ADC   $66
      ROR   $67
      PLA
      ADC   #$6A
      ROR   A
      JMP   ($6E6D)
      JMP   (>$006D)
      ADC   $6F6E
      ADC   >$006E
      ROR   $706F
      ROR   >$006F
      BVS   *+115
      ADC   ($72),Y
      ADC   ($73)
      STZ   $75,X
      ADC   $76,X
      ROR   $77,X
      SEI
      ADC   $7B7A,Y
      ADC   >$007A,Y
      PLY
      JMP   ($7E7D,X)
      JMP   (>$007D,X)
      ADC   $7F7E,X
      ADC   >$007E,X
      ROR   $807F,X
      ROR   >$007F,X
      BRA   *-125
      STA   ($82,X)
      STY   $85
      STA   $86
      STX   $87
      DEY
      BIT   #$8A
      TXA
      STY   $8E8D
      STY   >$008D
      STA   $8F8E
      STA   >$008E
      STX   $908F
      STX   >$008F
      BCC   *-109
      STA   ($92),Y
      STA   ($93)
      STY   $95,X
      STA   $96,X
      STX   $97,Y
      TYA
      STA   $9B9A,Y
      STA   >$009A,Y
      TXS
      STZ   $9E9D
      STZ   >$009D
      STA   $9F9E,X
      STA   >$009E,X
      STZ   $A09F,X
      STZ   >$009F,X
      LDY   #$A1
      LDA   ($A2,X)
      LDX   #$A3
      LDY   $A5
      LDA   $A6
      LDX   $A7
      TAY
      LDA   #$AA
      TAX
      LDY   $AEAD
      LDY   >$00AD
      LDA   $AFAE
      LDA   >$00AE
      LDX   $B0AF
      LDX   >$00AF
      BCS   *-77
      LDA   ($B2),Y
      LDA   ($B3)
      LDY   $B5,X
      LDA   $B6,X
      LDX   $B7,Y
      CLV
      LDA   $BBBA,Y
      LDA   >$00BA,Y
      TSX
      LDY   $BEBD,X
      LDY   >$00BD,X
      LDA   $BFBE,X
      LDA   >$00BE,X
      LDX   $C0BF,Y
      LDX   >$00BF,Y
      CPY   #$C1
      CMP   ($C2,X)
      CPY   $C5
      CMP   $C6
      DEC   $C7
      INY
      CMP   #$CA
      DEX
      CPY   $CECD
      CPY   >$00CD
      CMP   $CFCE
      CMP   >$00CE
      DEC   $D0CF
      DEC   >$00CF
      BNE   *-45
      CMP   ($D2),Y
      CMP   ($D3)
      CMP   $D6,X
      DEC   $D7,X
      CLD
      CMP   $DBDA,Y
      CMP   >$00DA,Y
      PHX
      CMP   $DFDE,X
      CMP   >$00DE,X
      DEC   $E0DF,X
      DEC   >$00DF,X
      CPX   #$E1
      SBC   ($E2,X)
      CPX   $E5
      SBC   $E6
      INC   $E7
      INX
      SBC   #$EA
      NOP
      CPX   $EEED
      CPX   >$00ED
      SBC   $EFEE
      SBC   >$00EE
      INC   $F0EF
      INC   >$00EF
      BEQ   *-13
      SBC   ($F2),Y
      SBC   ($F3)
      SBC   $F6,X
      INC   $F7,X
      SED
      SBC   $FBFA,Y
      SBC   >$00FA,Y
      PLX
      SBC   $FFFE,X
      SBC   >$00FE,X
      INC   >$00FF,X
      INC   $01FF,X
