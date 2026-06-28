;;; CP1600 branch-condition aliases (Data Manual Table: BC=BLGE, BNC=BLLT,
;;; BZE=BEQ, BNZE=BNEQ).  ASL's CP-1600 target rejects these mnemonics
;;; ("unknown instruction"); see ~/asl_bug_cp1600.md.  The .asl reference
;;; emits the identical bytes via WORD data directives instead.
L_ALIAS:
        BLGE    L_ALIAS
        BLLT    L_ALIAS
        BEQ     L_ALIAS
        BNEQ    L_ALIAS
