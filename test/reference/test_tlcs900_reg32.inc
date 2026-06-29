;;; Copyright 2026 Tadashi G. Takaoka
;;;
;;; General 32-bit register (XWA/XBC/XDE/XHL) instructions: maximum mode only.
;;; The CPU cannot use these registers in minimum mode (manual 3.2.1), so /L
;;; excludes this file.

        LD      XWA,XWA
        LD      XWA,XBC
        LD      XWA,XDE
        LD      XWA,XHL
        LD      XWA,XIX
        LD      XWA,XIY
        LD      XWA,XIZ
        LD      XWA,XSP
        LD      XIX,XWA
        LD      XIY,XWA
        LD      XIZ,XWA
        LD      XSP,XWA
        LD      XWA,012345678H
        LD      XBC,012345678H
        LD      XWA,(XIX)
        LD      XWA,(-XIX)
        LD      XWA,(XIX+)
        LD      XWA,(XIX+20H)
        LD      XWA,(XIX+1234H)
        LD      XWA,(034H)
        LD      XWA,(01234H)
        LD      XWA,(0123456H)
        LD      XWA,(XIX+A)
        LD      (XIX),XWA
        LD      (-XIX),XWA
        LD      (XIX+),XWA
        LD      (XIX+20H),XWA
        LD      (XIX+1234H),XWA
        LD      (034H),XWA
        LD      (01234H),XWA
        LD      (0123456H),XWA
        LD      (XIX+A),XWA
        PUSH    XWA
        PUSH    XBC
        POP     XWA
        ADD     XWA,XWA
        ADD     XWA,XBC
        ADD     XWA,012345678H
        ADD     XWA,(XIX)
        ADD     XWA,(XIX+20H)
        ADD     XWA,(034H)
        ADD     XWA,(01234H)
        ADD     XWA,(0123456H)
        ADD     XWA,(XIX+A)
        INC     1,XWA
        INC     2,XWA
        INC     4,XWA
        DEC     1,XWA
        MULA    XWA
        EXTZ    XWA
        EXTS    XWA
        PAA     XWA
