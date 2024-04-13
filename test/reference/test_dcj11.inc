;# Copyright 2024 Tadashi G. Takaoka
;#
;# Licensed under the Apache License, Version 2.0 (the "License");
;# you may not use this file except in compliance with the License.
;# You may obtain a copy of the License at
;#
;#     http://www.apache.org/licenses/LICENSE-2.0
;#
;# Unless required by applicable law or agreed to in writing, software
;# distributed under the License is distributed on an "AS IS" BASIS,
;# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;# See the License for the specific language governing permissions and
;# limitations under the License.

        MFPI    (R0)
        MTPI    (R1)
        MFPD    (R2)
        MTPD    (R3)
        MFPI    R0              ; must be error
        MTPI    R1              ; must be error
        MFPD    R2              ; must be error
        MTPD    R3              ; must be error
        MUL     @SP, R1
        DIV     (R2), R3
        ASH     (R0)+, R4
        ASHC    R1, R2
        MARK    023
        CSM     (R0)
        TSTSET  (R1)
        WRTLCK  (R2)
        TSTSET  R1              ; must be error

        ABSD    AC0
        ABSF    AC1
        ADDD    #0.0, AC2
        ADDF    #0.0, AC1
        CFCC
        CLRD    -(R2)
        CLRF    @(R1)+
        CMPD    AC5, AC0
        CMPF    -2(SP), AC1
        DIVD    (R1)+, AC2
        DIVF    -(R2), AC3
        LDCFD   @(R3)+, AC3
        LDCDF   AC0, AC0
        LDCLD   R1, AC2
        LDCID   (R2), AC3
        LDCLF   @(R3)+, AC0
        LDCIF   R4, AC1
        LDEXP   R5, AC2
        LDD     @(SP)+, AC3
        LDF     6(SP), AC2
        LDFPS   R5
        MODD    AC0, AC1
        MODF    (R2)+, AC3
        MODD    #2, AC1
        MODF    #3, AC3
        MULD    -(R3), AC2
        MULF    6(R4), AC1
        NEGD    AC2
        NEGF    -4(SP)
        SETF
        SETD
        SETI
        SETL
;        STCFD   AC1, AC1
;        STCDF   AC3, AC3
        STD     AC1, -(SP)
        STF     AC2, @R1
        STCDL   AC0, R1
        STCDI   AC1, -(SP)
        STCFL   AC2, -4(SP)
        STCFI   AC3, (R2)
        STEXP   AC0, R1
        STFPS   R2
        STST    R4
        STST    2(R4)
        SUBD    (R0), AC2
        SUBF    (R1)+, AC1
        TSTD    AC2
        TSTF    (R1)+

;# Local Variables:
;# mode: asm
;# End:
;# vim: set ft=asm:
