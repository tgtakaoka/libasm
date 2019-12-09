        org     $1000
        clra
        lda     #'a'
;;; line comment
label1:
label2: sta     $20             ; comment
        ldd     #label6-label5
        ldx     #label5
        bra     label1

label3: rmb     4
label4: fcb     'a',"bc\x64\145\"", '\'', '\\', $20, "'", '"', '\n',0
        fdb     $1234,$5678,$9abc ; comment
label5: fcc     "this is message",0
label6: equ     *
