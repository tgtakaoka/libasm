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
;        MFPI    R0              ; should be warning
;        MTPI    R1              ; should be warning
;        MFPD    R2              ; should be warning
;        MTPD    R3              ; should be warning
        MUL     @SP, R1
        DIV     (R2), R3
        ASH     (R0)+, R4
        ASHC    R1, R2
        MARK    023
        CSM     (R0)
        TSTSET  (R1)
        WRTLCK  (R2)
;        TSTSET  R1              ; must be error (trap 10)
;        WRTLCK  R2              ; error (trap 10)

        ABSD    AC0
        ABSF    AC1
        ADDD    #1.23, AC2
        ADDF    #1.23, AC1
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
        LDCLD   #1234567, AC1
        LDCID   (R2), AC3
        LDCID   #12345, AC2
        LDCLF   @(R3)+, AC0
        LDCLF   #1234567, AC1
        LDCIF   R4, AC1
        LDCIF   #12345, AC2
        LDEXP   R5, AC2
        LDEXP   #20, AC2
        LDD     @(SP)+, AC3
        LDF     6(SP), AC2
        LDCFD   #2.34, AC3
        LDCDF   #-3.45, AC0
        .radix  10
        LDCLD   #1234567, AC2
        LDCID   #12345, AC3
        LDCLF   #1234567, AC0
        LDCIF   #12345, AC1
        .radix  8
        LDEXP   #020, AC2
        LDD     #4.56, AC3
        LDF     #-4.56, AC2
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
        STCFD   AC1, (R2)
        STCDF   AC3, @#2000
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
