      NOP
      LXI  B,2
      STAX B
      INX  B
      INR  B
      DCR  B
      MVI  B,7
      RLC
      DAD  B
      LDAX B
      DCX  B
      INR  C
      DCR  C
      MVI  C,15
      RRC
      LXI  D,0012H
      STAX D
      INX  D
      INR  D
      DCR  D
      MVI  D,17H
      RAL
      DAD  D
      LDAX D
      DCX  D
      INR  E
      DCR  E
      MVI  E,1FH
      RAR
      LXI  H,0022H
      SHLD 0023H
      INX  H
      INR  H
      DCR  H
      MVI  H,27H
      DAA
      DAD  H
      LHLD 002BH
      DCX  H
      INR  L
      DCR  L
      MVI  L,2FH
      CMA
      LXI  SP,0032H
      STA  0033H
      INX  SP
      INR  M
      DCR  M
      MVI  M,37H
      STC
      DAD  SP
      LDA  003BH
      DCX  SP
      INR  A
      DCR  A
      MVI  A,3FH
      CMC
      MOV  B,B
      MOV  B,C
      MOV  B,D
      MOV  B,E
      MOV  B,H
      MOV  B,L
      MOV  B,M
      MOV  B,A
      MOV  C,B
      MOV  C,C
      MOV  C,D
      MOV  C,E
      MOV  C,H
      MOV  C,L
      MOV  C,M
      MOV  C,A
      MOV  D,B
      MOV  D,C
      MOV  D,D
      MOV  D,E
      MOV  D,H
      MOV  D,L
      MOV  D,M
      MOV  D,A
      MOV  E,B
      MOV  E,C
      MOV  E,D
      MOV  E,E
      MOV  E,H
      MOV  E,L
      MOV  E,M
      MOV  E,A
      MOV  H,B
      MOV  H,C
      MOV  H,D
      MOV  H,E
      MOV  H,H
      MOV  H,L
      MOV  H,M
      MOV  H,A
      MOV  L,B
      MOV  L,C
      MOV  L,D
      MOV  L,E
      MOV  L,H
      MOV  L,L
      MOV  L,M
      MOV  L,A
      MOV  M,B
      MOV  M,C
      MOV  M,D
      MOV  M,E
      MOV  M,H
      MOV  M,L
      HLT
      MOV  M,A
      MOV  A,B
      MOV  A,C
      MOV  A,D
      MOV  A,E
      MOV  A,H
      MOV  A,L
      MOV  A,M
      MOV  A,A
      ADD  B
      ADD  C
      ADD  D
      ADD  E
      ADD  H
      ADD  L
      ADD  M
      ADD  A
      ADC  B
      ADC  C
      ADC  D
      ADC  E
      ADC  H
      ADC  L
      ADC  M
      ADC  A
      SUB  B
      SUB  C
      SUB  D
      SUB  E
      SUB  H
      SUB  L
      SUB  M
      SUB  A
      SBB  B
      SBB  C
      SBB  D
      SBB  E
      SBB  H
      SBB  L
      SBB  M
      SBB  A
      ANA  B
      ANA  C
      ANA  D
      ANA  E
      ANA  H
      ANA  L
      ANA  M
      ANA  A
      XRA  B
      XRA  C
      XRA  D
      XRA  E
      XRA  H
      XRA  L
      XRA  M
      XRA  A
      ORA  B
      ORA  C
      ORA  D
      ORA  E
      ORA  H
      ORA  L
      ORA  M
      ORA  A
      CMP  B
      CMP  C
      CMP  D
      CMP  E
      CMP  H
      CMP  L
      CMP  M
      CMP  A
      RNZ
      POP  B
      JNZ  00C3H
      JMP  00C4H
      CNZ  00C5H
      PUSH B
      ADI  0C7H
      RST  0
      RZ
      RET
      JZ   00CBH
      CZ   00CDH
      CALL 00CEH
      ACI  0CFH
      RST  1
      RNC
      POP  D
      JNC  00D3H
      OUT  0D4H
      CNC  00D5H
      PUSH D
      SUI  0D7H
      RST  2
      RC
      JC   00DBH
      IN   0DCH
      CC   00DDH
      SBI  0DFH
      RST  3
      RPO
      POP  H
      JPO  00E3H
      XTHL
      CPO  00E5H
      PUSH H
      ANI  0E7H
      RST  4
      RPE
      PCHL
      JPE  00EBH
      XCHG
      CPE  00EDH
      XRI  0EFH
      RST  5
      RP
      POP  PSW
      JP   00F3H
      DI
      CP   00F5H
      PUSH PSW
      ORI  0F7H
      RST  6
      RM
      SPHL
      JM   00FBH
      EI
      CM   00FDH
      CPI  0FFH
      RST  7