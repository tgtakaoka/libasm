;;; AUTO GENERATED FILE
;;; generated by: gen_i8051 -u -C 8051 -o gen_i8051.asm -l gen_i8051.lst
      CPU   8051
      ORG   0100H
      NOP
      AJMP  0002H
      LJMP  0304H
      RR    A
      INC   A
      INC   06H
      INC   @R0
      INC   R0
      JBC   22H.1, $+21
      JBC   22H.1, $-125
      JBC   22H.1, $
      ACALL 0012H
      LCALL 1314H
      RRC   A
      DEC   A
      DEC   16H
      DEC   @R0
      DEC   R0
      JB    24H.1, $+37
      JB    24H.1, $-125
      JB    24H.1, $
      RET
      RL    A
      ADD   A, #25H
      ADD   A, 26H
      ADD   A, @R0
      ADD   A, R0
      JNB   26H.1, $+53
      JNB   26H.1, $-125
      JNB   26H.1, $
      RETI
      RLC   A
      ADDC  A, #35H
      ADDC  A, 36H
      ADDC  A, @R0
      ADDC  A, R0
      JC    $+67
      JC    $-126
      JC    $
      ORL   43H, A
      ORL   44H, #45H
      ORL   A, #45H
      ORL   A, 46H
      ORL   A, @R0
      ORL   A, R0
      JNC   $+83
      JNC   $-126
      JNC   $
      ANL   53H, A
      ANL   54H, #55H
      ANL   A, #55H
      ANL   A, 56H
      ANL   A, @R0
      ANL   A, R0
      JZ    $+99
      JZ    $-126
      JZ    $
      XRL   63H, A
      XRL   64H, #65H
      XRL   A, #65H
      XRL   A, 66H
      XRL   A, @R0
      XRL   A, R0
      JNZ   $+115
      JNZ   $-126
      JNZ   $
      ORL   C, 2EH.3
      JMP   @A+DPTR
      MOV   A, #75H
      MOV   76H, #77H
      MOV   @R0, #77H
      MOV   R0, #79H
      SJMP  $-125
      SJMP  $
      SJMP  $+1
      ANL   C, 80H.3
      MOVC  A, @A+PC
      DIV   AB
      MOV   87H, 86H
      MOV   87H, @R0
      MOV   89H, R0
      MOV   DPTR, #9192H
      MOV   90H.3, C
      MOVC  A, @A+DPTR
      SUBB  A, #95H
      SUBB  A, 96H
      SUBB  A, @R0
      SUBB  A, R0
      ORL   C, /0A0H.1
      MOV   C, 0A0H.3
      INC   DPTR
      MUL   AB
      MOV   @R0, 0A7H
      MOV   R0, 0A9H
      ANL   C, /0B0H.1
      CPL   0B0H.3
      CPL   C
      CJNE  A, #0B5H, $-71
      CJNE  A, #0B5H, $
      CJNE  A, #0B5H, $+1
      CJNE  A, 0B6H, $-70
      CJNE  A, 0B6H, $
      CJNE  A, 0B6H, $+1
      CJNE  @R0, #0B7H, $-69
      CJNE  @R0, #0B7H, $
      CJNE  @R0, #0B7H, $+1
      CJNE  R0, #0B9H, $-67
      CJNE  R0, #0B9H, $
      CJNE  R0, #0B9H, $+1
      PUSH  0C1H
      CLR   0C0H.3
      CLR   C
      SWAP  A
      XCH   A, 0C6H
      XCH   A, @R0
      XCH   A, R0
      POP   0D1H
      SETB  0D0H.3
      SETB  C
      DA    A
      DJNZ  0D6H, $-38
      DJNZ  0D6H, $
      DJNZ  0D6H, $+1
      XCHD  A, @R0
      DJNZ  R0, $-37
      DJNZ  R0, $
      DJNZ  R0, $+1
      MOVX  A, @DPTR
      MOVX  A, @R0
      CLR   A
      MOV   A, 0E6H
      MOV   A, @R0
      MOV   A, R0
      MOVX  @DPTR, A
      MOVX  @R0, A
      CPL   A
      MOV   0F6H, A
      MOV   @R0, A
      MOV   R0, A
