;### Copyright 2025 Tadashi G. Takaoka
;###
;### Licensed under the Apache License, Version 2.0 (the "License");
;### you may not use this file except in compliance with the License.
;### You may obtain a copy of the License at
;###
;###     http://www.apache.org/licenses/LICENSE-2.0
;###
;### Unless required by applicable law or agreed to in writing, software
;### distributed under the License is distributed on an "AS IS" BASIS,
;### WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;### See the License for the specific language governing permissions and
;### limitations under the License.

;### Control transfer
;### Call procedure
        callf   0x1234:0x5678
        callf   [si]
        callf   [0x1234]
        callf   [di-0x34]
        callf   [bp+0x1234]
        callf   [bx+si]
        callf   [bx+di+0x34]
        callf   [bp+si+0x1234]
;### Jump
        jmpf    0x1234:0x5678
        jmpf    [si]
        jmpf    [0x1234]
        jmpf    [di-0x34]
        jmpf    [bp+0x1234]
        jmpf    [bx+si]
        jmpf    [bx+di+0x34]
        jmpf    [bp+si+0x1234]
;### Return from procedure
        retf
        retf    16

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
