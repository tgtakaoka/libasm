;;; AUTO GENERATED FILE
;;; generated by: gen_tms32010 -u -C 32010 -o gen_tms32010.asm -l gen_tms32010.lst
      CPU  32010
      ORG  100H
      ADD  00H, 1
      ADD  *, 1, AR0
      ADD  *, 1
      ADD  *-, 1, AR0
      ADD  *-, 1
      ADD  *+, 1, AR0
      ADD  *+, 1
      SUB  00H
      SUB  *, 0, AR0
      SUB  *
      SUB  *-, 0, AR0
      SUB  *-
      SUB  *+, 0, AR0
      SUB  *+
      SUB  00H, 1
      SUB  *, 1
      SUB  *-, 1
      SUB  *+, 1
      LAC  00H
      LAC  *, 0, AR0
      LAC  *
      LAC  *-, 0, AR0
      LAC  *-
      LAC  *+, 0, AR0
      LAC  *+
      LAC  00H, 1
      LAC  *, 1
      LAC  *-, 1
      LAC  *+, 1
      SAR  AR0, 00H
      SAR  AR0, *, AR0
      SAR  AR0, *
      SAR  AR0, *-, AR0
      SAR  AR0, *-
      SAR  AR0, *+, AR0
      SAR  AR0, *+
      LAR  AR0, 00H
      LAR  AR0, *, AR0
      LAR  AR0, *
      LAR  AR0, *-, AR0
      LAR  AR0, *-
      LAR  AR0, *+, AR0
      LAR  AR0, *+
      IN   00H, PA0
      IN   *, PA0, AR0
      IN   *, PA0
      IN   *-, PA0, AR0
      IN   *-, PA0
      IN   *+, PA0, AR0
      IN   *+, PA0
      OUT  00H, PA0
      OUT  *, PA0, AR0
      OUT  *, PA0
      OUT  *-, PA0, AR0
      OUT  *-, PA0
      OUT  *+, PA0, AR0
      OUT  *+, PA0
      SACL 00H
      SACL *, 0, AR0
      SACL *
      SACL *-, 0, AR0
      SACL *-
      SACL *+, 0, AR0
      SACL *+
      SACH 00H
      SACH *, 0, AR0
      SACH *
      SACH *-, 0, AR0
      SACH *-
      SACH *+, 0, AR0
      SACH *+
      SACH 00H, 1
      SACH *, 1
      SACH *-, 1
      SACH *+, 1
      ADDH 00H
      ADDH *, AR0
      ADDH *
      ADDH *-, AR0
      ADDH *-
      ADDH *+, AR0
      ADDH *+
      ADDS 00H
      ADDS *, AR0
      ADDS *
      ADDS *-, AR0
      ADDS *-
      ADDS *+, AR0
      ADDS *+
      SUBH 00H
      SUBH *, AR0
      SUBH *
      SUBH *-, AR0
      SUBH *-
      SUBH *+, AR0
      SUBH *+
      SUBS 00H
      SUBS *, AR0
      SUBS *
      SUBS *-, AR0
      SUBS *-
      SUBS *+, AR0
      SUBS *+
      SUBC 00H
      SUBC *, AR0
      SUBC *
      SUBC *-, AR0
      SUBC *-
      SUBC *+, AR0
      SUBC *+
      ZALH 00H
      ZALH *, AR0
      ZALH *
      ZALH *-, AR0
      ZALH *-
      ZALH *+, AR0
      ZALH *+
      ZALS 00H
      ZALS *, AR0
      ZALS *
      ZALS *-, AR0
      ZALS *-
      ZALS *+, AR0
      ZALS *+
      TBLR 00H
      TBLR *, AR0
      TBLR *
      TBLR *-, AR0
      TBLR *-
      TBLR *+, AR0
      TBLR *+
      MAR  00H
      LARP AR0
      MAR  *
      MAR  *-, AR0
      MAR  *-
      MAR  *+, AR0
      MAR  *+
      DMOV 00H
      DMOV *, AR0
      DMOV *
      DMOV *-, AR0
      DMOV *-
      DMOV *+, AR0
      DMOV *+
      LT   00H
      LT   *, AR0
      LT   *
      LT   *-, AR0
      LT   *-
      LT   *+, AR0
      LT   *+
      LTD  00H
      LTD  *, AR0
      LTD  *
      LTD  *-, AR0
      LTD  *-
      LTD  *+, AR0
      LTD  *+
      LTA  00H
      LTA  *, AR0
      LTA  *
      LTA  *-, AR0
      LTA  *-
      LTA  *+, AR0
      LTA  *+
      MPY  00H
      MPY  *, AR0
      MPY  *
      MPY  *-, AR0
      MPY  *-
      MPY  *+, AR0
      MPY  *+
      LDPK 0
      LDP  00H
      LDP  *, AR0
      LDP  *
      LDP  *-, AR0
      LDP  *-
      LDP  *+, AR0
      LDP  *+
      LARK AR0, 0
      XOR  00H
      XOR  *, AR0
      XOR  *
      XOR  *-, AR0
      XOR  *-
      XOR  *+, AR0
      XOR  *+
      AND  00H
      AND  *, AR0
      AND  *
      AND  *-, AR0
      AND  *-
      AND  *+, AR0
      AND  *+
      OR   00H
      OR   *, AR0
      OR   *
      OR   *-, AR0
      OR   *-
      OR   *+, AR0
      OR   *+
      LST  00H
      LST  *, AR0
      LST  *
      LST  *-, AR0
      LST  *-
      LST  *+, AR0
      LST  *+
      SST  80H
      SST  *, AR0
      SST  *
      SST  *-, AR0
      SST  *-
      SST  *+, AR0
      SST  *+
      TBLW 00H
      TBLW *, AR0
      TBLW *
      TBLW *-, AR0
      TBLW *-
      TBLW *+, AR0
      TBLW *+
      LACK 0
      NOP
      DINT
      EINT
      ABS
      ZAC
      ROVM
      SOVM
      CALA
      RET
      PAC
      APAC
      SPAC
      PUSH
      POP
      MPYK 0
      MPYK -4096
      BANZ 0FFH
      BV   0FFH
      BIOZ 0FFH
      CALL 0FFH
      B    0FFH
      BLZ  0FFH
      BLEZ 0FFH
      BGZ  0FFH
      BGEZ 0FFH
      BNZ  0FFH
      BZ   100H
      ADD  00H
      ADD  *
      ADD  *-
      ADD  *+