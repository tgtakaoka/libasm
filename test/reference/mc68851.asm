        cpu     68020
        pmmu    on
        ;; Unexpected extra bytes
        pmove.l tc,(a2)         ; F012 4200<0000>
        pmove.w bac7,(a2)       ; F012 761C<0000>
        ;; Wrong opcode
        pbbs    *+$1234         ;<F0B0>1232
        pbcc    *+$1234         ;<F08F>1232
        pbbs.x  *+$12345678     ;<F0C0>1234 5676
        pbcc.x  *+$12345678     ;<F08F>1234 5676
        pdbbs   d2,*+$1234      ; F04A<0000>1230
        pdbcc   d2,*+$1234      ; F04A<000F>1230
        ptrapbs                 ; F07C<0000>
        ptrapcc                 ; F07C<000F>
        ptrapbs.w #$1234        ; F07A<0000>1234
        ptrapcc.w #$1234        ; F07A<000F>1234
        ptrapbs.l #$12345678    ; F07B<0000>1234 5678
        ptrapcc.l #$12345678    ; F07B<000F>1234 5678
        psbs    d2              ; F042<0000>
        pscc    d2              ; F042<000F>
        ;; Wrong offset
        prestore (*+$1234,pc)   ; F17A<1232>
        prestore (*+52,pc,d3.l) ; F17B<3832>
        ;; Wrong extension code
        pvalid  val,($1234,a2)    ; F02A<2800>1234
        pvalid  val,($34,a2,d3.w) ; F02A<2800 3034>
        pvalid  val,($1234).w     ; F02A<2800>1234
        pvalid  val,($12345678).l ; F02A<2800 1234>5678
        pvalid  a3,($1234,a2)     ; F02A<2C03>1234
        pvalid  a4,($34,a2,d3.w)  ; F02A<2C04 3034>
        pvalid  a5,($1234).w      ; F02A<2C05 1234:
        pvalid  a6,($12345678).l  ; F02A<2C06 1234>5678
        ;; Addressing mode allowed
        pmove.l tc,(a2)+        ;<F01A 4200>
        pmove.q drp,-(a2)       ;<F022 4600>
        pmove.q srp,($1234).w   ;<F038 4A00 1234>
        pmove.q crp,($123456).l ;<F039 4E00 0012 3456>
        prestore (a2)+          ;<F15A>
        ;; Unsized instruction
        pflushr (a2)            ;<F012 A000>
