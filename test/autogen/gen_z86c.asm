;;; AUTO GENERATED FILE
;;; generated by: gen_z8 -u -C Z86C40 -o gen_z86c.asm -l gen_z86c.lst
      CPU     Z86C40
      ORG     0100H
      DEC     >01H
      DEC     10H
      DEC     @02H
      ADD     R0,R3
      ADD     R0,@R4
      ADD     >06H,>05H
      ADD     10H,>05H
      ADD     >07H,@06H
      ADD     10H,@06H
      ADD     >07H,#8
      ADD     @08H,#9
      LD      R0,>09H
      LD      R0,10H
      LD      >0AH,R0
      LD      10H,R0
      DJNZ    R0,$+13
      DJNZ    R0,$-126
      DJNZ    R0,$
      JR      F,$+14
      JR      F,$-126
      JR      F,$
      LD      R0,#13
      JP      F,0E0FH
      INC     R0
      RLC     11H
      RLC     >00H
      RLC     @12H
      ADC     R1,R3
      ADC     R1,@R4
      ADC     16H,15H
      ADC     >00H,15H
      ADC     17H,@16H
      ADC     >00H,@16H
      ADC     17H,#24
      ADC     @18H,#25
      JR      LT,$+30
      JR      LT,$-126
      JR      LT,$
      JP      LT,1E1FH
      INC     21H
      INC     >00H
      INC     @22H
      SUB     R2,R3
      SUB     R2,@R4
      SUB     26H,25H
      SUB     >00H,25H
      SUB     27H,@26H
      SUB     >00H,@26H
      SUB     27H,#28H
      SUB     @28H,#29H
      JR      LE,$+46
      JR      LE,$-126
      JR      LE,$
      JP      LE,2E2FH
      JP      @32H
      SRP     #40H
      SBC     R3,R3
      SBC     R3,@R4
      SBC     36H,35H
      SBC     >00H,35H
      SBC     37H,@36H
      SBC     >00H,@36H
      SBC     37H,#38H
      SBC     @38H,#39H
      JR      ULE,$+62
      JR      ULE,$-126
      JR      ULE,$
      JP      ULE,3E3FH
      DA      41H
      DA      >00H
      DA      @42H
      OR      R4,R3
      OR      R4,@R4
      OR      46H,45H
      OR      >00H,45H
      OR      47H,@46H
      OR      >00H,@46H
      OR      47H,#48H
      OR      @48H,#49H
      JR      OV,$+78
      JR      OV,$-126
      JR      OV,$
      JP      OV,4E4FH
      POP     51H
      POP     >00H
      POP     @52H
      AND     R5,R3
      AND     R5,@R4
      AND     56H,55H
      AND     >00H,55H
      AND     57H,@56H
      AND     >00H,@56H
      AND     57H,#58H
      AND     @58H,#59H
      JR      MI,$+94
      JR      MI,$-126
      JR      MI,$
      JP      MI,5E5FH
      COM     61H
      COM     >00H
      COM     @62H
      TCM     R6,R3
      TCM     R6,@R4
      TCM     66H,65H
      TCM     >00H,65H
      TCM     67H,@66H
      TCM     >00H,@66H
      TCM     67H,#68H
      TCM     @68H,#69H
      JR      Z,$+110
      JR      Z,$-126
      JR      Z,$
      JP      Z,6E6FH
      STOP
      PUSH    71H
      PUSH    >00H
      PUSH    @72H
      TM      R7,R3
      TM      R7,@R4
      TM      76H,75H
      TM      >00H,75H
      TM      77H,@76H
      TM      >00H,@76H
      TM      77H,#78H
      TM      @78H,#79H
      JR      C,$+126
      JR      C,$-126
      JR      C,$
      JP      C,7E7FH
      HALT
      DECW    82H
      DECW    >00H
      DECW    @82H
      LDE     R8,@RR4
      LDEI    @R8,@RR4
      JR      $-114
      JR      $
      JR      $+1
      JP      8E8FH
      DI
      RL      91H
      RL      >00H
      RL      @92H
      LDE     @RR4,R9
      LDEI    @RR4,@R9
      JR      GE,$-98
      JR      GE,$
      JR      GE,$+1
      JP      GE,9E9FH
      EI
      INCW    0A2H
      INCW    >00H
      INCW    @0A2H
      CP      R10,R3
      CP      R10,@R4
      CP      0A6H,0A5H
      CP      >00H,0A5H
      CP      0A7H,@0A6H
      CP      >00H,@0A6H
      CP      0A7H,#0A8H
      CP      @0A8H,#0A9H
      JR      GT,$-82
      JR      GT,$
      JR      GT,$+1
      JP      GT,0AEAFH
      RET
      CLR     0B1H
      CLR     >00H
      CLR     @0B2H
      XOR     R11,R3
      XOR     R11,@R4
      XOR     0B6H,0B5H
      XOR     >00H,0B5H
      XOR     0B7H,@0B6H
      XOR     >00H,@0B6H
      XOR     0B7H,#0B8H
      XOR     @0B8H,#0B9H
      JR      UGT,$-66
      JR      UGT,$
      JR      UGT,$+1
      JP      UGT,0BEBFH
      IRET
      RRC     0C1H
      RRC     >00H
      RRC     @0C2H
      LDC     R12,@RR4
      LDCI    @R12,@RR4
      LD      R12,0C9H(R8)
      JR      NOV,$-50
      JR      NOV,$
      JR      NOV,$+1
      JP      NOV,0CECFH
      RCF
      SRA     0D1H
      SRA     >00H
      SRA     @0D2H
      LDC     @RR4,R13
      LDCI    @RR4,@R13
      CALL    @0D6H
      CALL    0D7D8H
      LD      0D9H(R8),R13
      JR      PL,$-34
      JR      PL,$
      JR      PL,$+1
      JP      PL,0DEDFH
      SCF
      RR      0E1H
      RR      >00H
      RR      @0E2H
      LD      R14,@R4
      LD      0E6H,0E5H
      LD      >00H,0E5H
      LD      0E7H,@0E6H
      LD      >00H,@0E6H
      LD      0E7H,#0E8H
      LD      @0E8H,#0E9H
      JR      NZ,$-18
      JR      NZ,$
      JR      NZ,$+1
      JP      NZ,0EEEFH
      CCF
      SWAP    0F1H
      SWAP    >00H
      SWAP    @0F2H
      LD      @R15,R4
      LD      @0F7H,0F6H
      JR      NC,$-2
      JR      NC,$
      JR      NC,$+1
      JP      NC,0FEFFH
      NOP