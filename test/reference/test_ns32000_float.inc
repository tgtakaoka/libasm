;# Copyright 2020 Tadashi G. Takaoka
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

;# Add Floating: ADDf src/gen, dest/gen
        addf    f0, f7
        addl    f2, 16(sb)
;# Subtract Floating: SUBf src/gen, dest/gen
        subf    f0, f7
        subl    f2, 16(sb)
;# Multiply Floating: MULf src/gen, dest/gen
        mulf    f0, f7
        mull    -8(fp), 8(sb)
;# Divide Floating: DIVf src/gen, dest/gen
        divf    f0, f7
        divl    -8(fp), 16(sb)
;# Negate Floating: NEGf src/gen, dest/gen
        negf    f0, f2
        negl    f2, f3
;# Absolute Value Floating: ABSf src/gen, dest/gen
        absf    f0, f2
        absl    f2, f3
;# Compare Floating: CMPf src1/gen, src2/gen
        cmpf    f0, f2
        cmpf    f2, f3
;# Move Floating: MOVf src/gen, dest/gen
        movf    f0, 8(sb)
        movl    f2, f3
;# Move Long Floating to Floating: MOVLF src/gen, dest/gen
        movlf   f0, 12(sb)
;# Move Floating to Long Floating: MOVFL src/gen, dest/gen
        movfl   8(sb), f0
;# Move Integer to Floating: MOVif src/gen, dest/gen
        movbf   2, f0
        movwf   r1, f2
        movdf   r2, f3
        movbl   r3, f4
        movwl   r4, f5
        movdl   16(sb), f2
;# Round Floating to Integer: ROUNDfi src/gen, dest/gen
        roundfb f0, r0
        roundfw f1, r2
        roundfd f2, r3
        roundlb f3, r4
        roundlw f4, r5
        roundld f2, 12(sb)
;# Truncate Floating to Integer: TRUNCfi src/gen, dest/gen
        truncfb f0, r0
        truncfw f1, r2
        truncfd f2, r3
        trunclb f3, r4
        trunclw f4, r5
        truncld f2, 8(sb)
;# Floor Floating to Integer: FLOORfi src/gen, dest/gen
        floorfb f0, r0
        floorfw f1, r2
        floorfd f2, r3
        floorlb f3, r4
        floorlw f4, r5
        floorld f2, 16(sb)
;# Load FSR: LFSR src/gen
        lfsr    r0
;# Store FSR: SFSR dest/gen
        sfsr    tos

;# Local Variables:
;# mode: asm
;# End:
;# vim: set ft=asm:
