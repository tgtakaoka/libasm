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

        HALT
        WAIT
        RTI
        BPT
        IOT
        RESET
        RTT
        MFPT
        RTS     R0
        RTS     PC
        CLC
        CLV
        CLZ
        CLN
        CCC
        NOP
        SEC
        SEV
        SEZ
        SEN
        SCC
        MOV     R1, (R3)+
        MOV     -(R5), 0123456(R7)
        MOVB    @R2, @(R4)+
        MOVB    @-(SP), @0123456(R0)
        CMP     R1, #0176543
        CMPB    @#0177777, R2
        BIT     R3, *+10
        BITB    @*+20, R4
        BIC     R5, @SP
        BICB    @(SP)+, (R0)
        BIS     R1, @-(SP)
        BISB    033(SP), R3
        ADD     @077(SP), R4
        SUB     056(PC), R5
        XOR     R0, -2(SP)
        JMP     @#01234
        SWAB    R0
        CLR     (R1)
        CLRB    (R2)+
        COM     @(R3)+
        COMB    -(R4)
        INC     @-(R5)
        INCB    066(SP)
        DEC     @076(PC)
        DECB    @054(R0)
        NEG     (SP)
        NEGB    (R5)+
        ADC     @(R4)+
        ADCB    -(R3)
        SBC     @-(R2)
        TST     -022(R1)
        TSTB    @-011(R0)
        ROR     (PC)
        RORB    (SP)
        ROL     (SP)+
        ROLB    -(SP)
        ASR     @SP
        ASRB    @(SP)+
        ASL     @-(SP)
        ASLB    -0100(SP)
        MTPS    @077(SP)
        SXT     076(SP)
        MFPS    @-0100(SP)
        BR      *+2
        BNE     *+4
        BEQ     *+6
        BGE     *+10
        BLT     *+12
        BGT     *+14
        BLE     *+16
        SOB     R0, *-10
        SOB     R5, *-20
        JSR     PC, @#054322
        BPL     *-2
        BMI     *-4
        BHI     *-6
        BLOS    *-10
        BVC     *-12
        BVS     *-14
        BCC     *-16
        BHIS    *+20
        BCS     *-22
        BLO     *+22
        EMT     377
        TRAP    001

;# Local Variables:
;# mode: asm
;# End:
;# vim: set ft=asm:
