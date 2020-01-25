        include "test_tms9900.asm"

        cpu     TMS9995

;;; TMS9995
        org     0100H

;;; 00X
        lst     r8
        lwp     r9
;;; 01X
        divs    r8
        mpys    r12

        end                     ; comment
