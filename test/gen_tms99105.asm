;;; AUTO GENERATED FILE
;;; generated by: gen_tms9900 -u -C TMS99105 -o gen_tms99105.asm
      CPU TMS99105
      LST  R0
      LWP  R0
      BLSK R0,00B1H
      EVAD R0
      EVAD *R0
      EVAD @0121H
      EVAD @0122H(R1)
      EVAD *R0+
      BIND R0
      BIND *R0
      BIND @0161H
      BIND @0162H(R1)
      BIND *R0+
      DIVS R0
      DIVS *R0
      DIVS @01A1H
      DIVS @01A2H(R1)
      DIVS *R0+
      MPYS R0
      MPYS *R0
      MPYS @01E1H
      MPYS @01E2H(R1)
      MPYS *R0+
      LI   R0,0201H
      AI   R0,0221H
      ANDI R0,0241H
      ORI  R0,0261H
      CI   R0,0281H
      STWP R0
      STST R0
      LWPI 02E1H
      LIMI 0301H
      IDLE
      RSET
      RTWP
      RTWP 1
      CKON
      CKOF
      LREX
      BLWP R0
      BLWP *R0
      BLWP @0421H
      BLWP @0422H(R1)
      BLWP *R0+
      B    R0
      B    *R0
      B    @0461H
      B    @0462H(R1)
      B    *R0+
      X    R0
      X    *R0
      X    @04A1H
      X    @04A2H(R1)
      X    *R0+
      CLR  R0
      CLR  *R0
      CLR  @04E1H
      CLR  @04E2H(R1)
      CLR  *R0+
      NEG  R0
      NEG  *R0
      NEG  @0521H
      NEG  @0522H(R1)
      NEG  *R0+
      INV  R0
      INV  *R0
      INV  @0561H
      INV  @0562H(R1)
      INV  *R0+
      INC  R0
      INC  *R0
      INC  @05A1H
      INC  @05A2H(R1)
      INC  *R0+
      INCT R0
      INCT *R0
      INCT @05E1H
      INCT @05E2H(R1)
      INCT *R0+
      DEC  R0
      DEC  *R0
      DEC  @0621H
      DEC  @0622H(R1)
      DEC  *R0+
      DECT R0
      DECT *R0
      DECT @0661H
      DECT @0662H(R1)
      DECT *R0+
      BL   R0
      BL   *R0
      BL   @06A1H
      BL   @06A2H(R1)
      BL   *R0+
      SWPB R0
      SWPB *R0
      SWPB @06E1H
      SWPB @06E2H(R1)
      SWPB *R0+
      SETO R0
      SETO *R0
      SETO @0721H
      SETO @0722H(R1)
      SETO *R0+
      ABS  R0
      ABS  *R0
      ABS  @0761H
      ABS  @0762H(R1)
      ABS  *R0+
      SRA  R0,R0
      SRA  R0,1
      SRL  R0,R0
      SRL  R0,1
      SLA  R0,R0
      SLA  R0,1
      SRC  R0,R0
      SRC  R0,1
      JMP  $+2
      JMP  $-0FEH
      JMP  $
      JLT  $+2
      JLT  $-0FEH
      JLT  $
      JLE  $+2
      JLE  $-0FEH
      JLE  $
      JEQ  $+2
      JEQ  $-0FEH
      JEQ  $
      JHE  $+2
      JHE  $-0FEH
      JHE  $
      JGT  $+2
      JGT  $-0FEH
      JGT  $
      JNE  $+2
      JNE  $-0FEH
      JNE  $
      JNC  $+2
      JNC  $-0FEH
      JNC  $
      JOC  $+2
      JOC  $-0FEH
      JOC  $
      JNO  $+2
      JNO  $-0FEH
      JNO  $
      JL   $+2
      JL   $-0FEH
      JL   $
      JH   $+2
      JH   $-0FEH
      JH   $
      JOP  $+2
      JOP  $-0FEH
      JOP  $
      SBO  0
      SBO  -128
      SBZ  0
      SBZ  -128
      TB   0
      TB   -128
      COC  R0,R0
      COC  *R0,R0
      COC  @2021H,R0
      COC  @2022H(R1),R0
      COC  *R0+,R0
      CZC  R0,R0
      CZC  *R0,R0
      CZC  @2421H,R0
      CZC  @2422H(R1),R0
      CZC  *R0+,R0
      XOR  R0,R0
      XOR  *R0,R0
      XOR  @2821H,R0
      XOR  @2822H(R1),R0
      XOR  *R0+,R0
      XOP  R0,0
      XOP  *R0,0
      XOP  @2C21H,0
      XOP  @2C22H(R1),0
      XOP  *R0+,0
      LDCR R0,16
      LDCR *R0,16
      LDCR @3021H,16
      LDCR @3022H(R1),16
      LDCR *R0+,16
      STCR R0,16
      STCR *R0,16
      STCR @3421H,16
      STCR @3422H(R1),16
      STCR *R0+,16
      MPY  R0,R0
      MPY  *R0,R0
      MPY  @3821H,R0
      MPY  @3822H(R1),R0
      MPY  *R0+,R0
      DIV  R0,R0
      DIV  *R0,R0
      DIV  @3C21H,R0
      DIV  @3C22H(R1),R0
      DIV  *R0+,R0
      SZC  R0,R0
      SZC  *R0,R0
      SZC  @4021H,R0
      SZC  @4022H(R1),R0
      SZC  *R0+,R0
      SZC  R0,*R0
      SZC  *R0,*R0
      SZC  @4421H,*R0
      SZC  @4422H(R1),*R0
      SZC  *R0+,*R0
      SZC  R0,@4801H
      SZC  *R0,@4811H
      SZC  @0,@4821H
      SZC  @0(R1),@4822H
      SZC  *R0+,@4831H
      SZC  R0,@4841H(R1)
      SZC  *R0,@4851H(R1)
      SZC  @0,@4861H(R1)
      SZC  @0(R1),@4862H(R1)
      SZC  *R0+,@4871H(R1)
      SZC  R0,*R0+
      SZC  *R0,*R0+
      SZC  @4C21H,*R0+
      SZC  @4C22H(R1),*R0+
      SZC  *R0+,*R0+
      SZCB R0,R0
      SZCB *R0,R0
      SZCB @5021H,R0
      SZCB @5022H(R1),R0
      SZCB *R0+,R0
      SZCB R0,*R0
      SZCB *R0,*R0
      SZCB @5421H,*R0
      SZCB @5422H(R1),*R0
      SZCB *R0+,*R0
      SZCB R0,@5801H
      SZCB *R0,@5811H
      SZCB @0,@5821H
      SZCB @0(R1),@5822H
      SZCB *R0+,@5831H
      SZCB R0,@5841H(R1)
      SZCB *R0,@5851H(R1)
      SZCB @0,@5861H(R1)
      SZCB @0(R1),@5862H(R1)
      SZCB *R0+,@5871H(R1)
      SZCB R0,*R0+
      SZCB *R0,*R0+
      SZCB @5C21H,*R0+
      SZCB @5C22H(R1),*R0+
      SZCB *R0+,*R0+
      S    R0,R0
      S    *R0,R0
      S    @6021H,R0
      S    @6022H(R1),R0
      S    *R0+,R0
      S    R0,*R0
      S    *R0,*R0
      S    @6421H,*R0
      S    @6422H(R1),*R0
      S    *R0+,*R0
      S    R0,@6801H
      S    *R0,@6811H
      S    @0,@6821H
      S    @0(R1),@6822H
      S    *R0+,@6831H
      S    R0,@6841H(R1)
      S    *R0,@6851H(R1)
      S    @0,@6861H(R1)
      S    @0(R1),@6862H(R1)
      S    *R0+,@6871H(R1)
      S    R0,*R0+
      S    *R0,*R0+
      S    @6C21H,*R0+
      S    @6C22H(R1),*R0+
      S    *R0+,*R0+
      SB   R0,R0
      SB   *R0,R0
      SB   @7021H,R0
      SB   @7022H(R1),R0
      SB   *R0+,R0
      SB   R0,*R0
      SB   *R0,*R0
      SB   @7421H,*R0
      SB   @7422H(R1),*R0
      SB   *R0+,*R0
      SB   R0,@7801H
      SB   *R0,@7811H
      SB   @0,@7821H
      SB   @0(R1),@7822H
      SB   *R0+,@7831H
      SB   R0,@7841H(R1)
      SB   *R0,@7851H(R1)
      SB   @0,@7861H(R1)
      SB   @0(R1),@7862H(R1)
      SB   *R0+,@7871H(R1)
      SB   R0,*R0+
      SB   *R0,*R0+
      SB   @7C21H,*R0+
      SB   @7C22H(R1),*R0+
      SB   *R0+,*R0+
      C    R0,R0
      C    *R0,R0
      C    @8021H,R0
      C    @8022H(R1),R0
      C    *R0+,R0
      C    R0,*R0
      C    *R0,*R0
      C    @8421H,*R0
      C    @8422H(R1),*R0
      C    *R0+,*R0
      C    R0,@8801H
      C    *R0,@8811H
      C    @0,@8821H
      C    @0(R1),@8822H
      C    *R0+,@8831H
      C    R0,@8841H(R1)
      C    *R0,@8851H(R1)
      C    @0,@8861H(R1)
      C    @0(R1),@8862H(R1)
      C    *R0+,@8871H(R1)
      C    R0,*R0+
      C    *R0,*R0+
      C    @8C21H,*R0+
      C    @8C22H(R1),*R0+
      C    *R0+,*R0+
      CB   R0,R0
      CB   *R0,R0
      CB   @9021H,R0
      CB   @9022H(R1),R0
      CB   *R0+,R0
      CB   R0,*R0
      CB   *R0,*R0
      CB   @9421H,*R0
      CB   @9422H(R1),*R0
      CB   *R0+,*R0
      CB   R0,@9801H
      CB   *R0,@9811H
      CB   @0,@9821H
      CB   @0(R1),@9822H
      CB   *R0+,@9831H
      CB   R0,@9841H(R1)
      CB   *R0,@9851H(R1)
      CB   @0,@9861H(R1)
      CB   @0(R1),@9862H(R1)
      CB   *R0+,@9871H(R1)
      CB   R0,*R0+
      CB   *R0,*R0+
      CB   @9C21H,*R0+
      CB   @9C22H(R1),*R0+
      CB   *R0+,*R0+
      A    R0,R0
      A    *R0,R0
      A    @0A021H,R0
      A    @0A022H(R1),R0
      A    *R0+,R0
      A    R0,*R0
      A    *R0,*R0
      A    @0A421H,*R0
      A    @0A422H(R1),*R0
      A    *R0+,*R0
      A    R0,@0A801H
      A    *R0,@0A811H
      A    @0,@0A821H
      A    @0(R1),@0A822H
      A    *R0+,@0A831H
      A    R0,@0A841H(R1)
      A    *R0,@0A851H(R1)
      A    @0,@0A861H(R1)
      A    @0(R1),@0A862H(R1)
      A    *R0+,@0A871H(R1)
      A    R0,*R0+
      A    *R0,*R0+
      A    @0AC21H,*R0+
      A    @0AC22H(R1),*R0+
      A    *R0+,*R0+
      AB   R0,R0
      AB   *R0,R0
      AB   @0B021H,R0
      AB   @0B022H(R1),R0
      AB   *R0+,R0
      AB   R0,*R0
      AB   *R0,*R0
      AB   @0B421H,*R0
      AB   @0B422H(R1),*R0
      AB   *R0+,*R0
      AB   R0,@0B801H
      AB   *R0,@0B811H
      AB   @0,@0B821H
      AB   @0(R1),@0B822H
      AB   *R0+,@0B831H
      AB   R0,@0B841H(R1)
      AB   *R0,@0B851H(R1)
      AB   @0,@0B861H(R1)
      AB   @0(R1),@0B862H(R1)
      AB   *R0+,@0B871H(R1)
      AB   R0,*R0+
      AB   *R0,*R0+
      AB   @0BC21H,*R0+
      AB   @0BC22H(R1),*R0+
      AB   *R0+,*R0+
      MOV  R0,R0
      MOV  *R0,R0
      MOV  @0C021H,R0
      MOV  @0C022H(R1),R0
      MOV  *R0+,R0
      MOV  R0,*R0
      MOV  *R0,*R0
      MOV  @0C421H,*R0
      MOV  @0C422H(R1),*R0
      MOV  *R0+,*R0
      MOV  R0,@0C801H
      MOV  *R0,@0C811H
      MOV  @0,@0C821H
      MOV  @0(R1),@0C822H
      MOV  *R0+,@0C831H
      MOV  R0,@0C841H(R1)
      MOV  *R0,@0C851H(R1)
      MOV  @0,@0C861H(R1)
      MOV  @0(R1),@0C862H(R1)
      MOV  *R0+,@0C871H(R1)
      MOV  R0,*R0+
      MOV  *R0,*R0+
      MOV  @0CC21H,*R0+
      MOV  @0CC22H(R1),*R0+
      MOV  *R0+,*R0+
      MOVB R0,R0
      MOVB *R0,R0
      MOVB @0D021H,R0
      MOVB @0D022H(R1),R0
      MOVB *R0+,R0
      MOVB R0,*R0
      MOVB *R0,*R0
      MOVB @0D421H,*R0
      MOVB @0D422H(R1),*R0
      MOVB *R0+,*R0
      MOVB R0,@0D801H
      MOVB *R0,@0D811H
      MOVB @0,@0D821H
      MOVB @0(R1),@0D822H
      MOVB *R0+,@0D831H
      MOVB R0,@0D841H(R1)
      MOVB *R0,@0D851H(R1)
      MOVB @0,@0D861H(R1)
      MOVB @0(R1),@0D862H(R1)
      MOVB *R0+,@0D871H(R1)
      MOVB R0,*R0+
      MOVB *R0,*R0+
      MOVB @0DC21H,*R0+
      MOVB @0DC22H(R1),*R0+
      MOVB *R0+,*R0+
      SOC  R0,R0
      SOC  *R0,R0
      SOC  @0E021H,R0
      SOC  @0E022H(R1),R0
      SOC  *R0+,R0
      SOC  R0,*R0
      SOC  *R0,*R0
      SOC  @0E421H,*R0
      SOC  @0E422H(R1),*R0
      SOC  *R0+,*R0
      SOC  R0,@0E801H
      SOC  *R0,@0E811H
      SOC  @0,@0E821H
      SOC  @0(R1),@0E822H
      SOC  *R0+,@0E831H
      SOC  R0,@0E841H(R1)
      SOC  *R0,@0E851H(R1)
      SOC  @0,@0E861H(R1)
      SOC  @0(R1),@0E862H(R1)
      SOC  *R0+,@0E871H(R1)
      SOC  R0,*R0+
      SOC  *R0,*R0+
      SOC  @0EC21H,*R0+
      SOC  @0EC22H(R1),*R0+
      SOC  *R0+,*R0+
      SOCB R0,R0
      SOCB *R0,R0
      SOCB @0F021H,R0
      SOCB @0F022H(R1),R0
      SOCB *R0+,R0
      SOCB R0,*R0
      SOCB *R0,*R0
      SOCB @0F421H,*R0
      SOCB @0F422H(R1),*R0
      SOCB *R0+,*R0
      SOCB R0,@0F801H
      SOCB *R0,@0F811H
      SOCB @0,@0F821H
      SOCB @0(R1),@0F822H
      SOCB *R0+,@0F831H
      SOCB R0,@0F841H(R1)
      SOCB *R0,@0F851H(R1)
      SOCB @0,@0F861H(R1)
      SOCB @0(R1),@0F862H(R1)
      SOCB *R0+,@0F871H(R1)
      SOCB R0,*R0+
      SOCB *R0,*R0+
      SOCB @0FC21H,*R0+
      SOCB @0FC22H(R1),*R0+
      SOCB *R0+,*R0+