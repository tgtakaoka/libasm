;;; Copyright 2026 Tadashi G. Takaoka
;;;
;;; Minimum-mode instructions: the general register pairs are addressed with
;;; their 16-bit names (WA/BC/DE/HL); the index registers XIX/XIY/XIZ/XSP stay
;;; 32-bit. These forms are rejected in maximum mode, so this file is included
;;; only inside the MAXMODE OFF section (base 900 and 900/L).

;;; General-register memory addressing, 16-bit register names
        LD      A,(HL)
        LD      A,(BC)
        LD      A,(DE)
        LD      A,(-HL)
        LD      A,(HL+)
        LD      A,(HL+5)
        LD      (WA),A
        LDW     (HL),01234H

;;; Block transfer/compare with the 16-bit pointer pair (XDE/XHL -> DE/HL)
        LDI     (DE+),(HL+)
        LDIW    (DE+),(HL+)
        LDD     (DE-),(HL-)
        CPI     A,(HL+)
        CPIR    WA,(DE+)
        CPD     A,(HL-)

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
