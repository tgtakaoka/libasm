;### Copyright 2026 Tadashi G. Takaoka
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

;### Page jumps to a 24-bit (>1MB) address. Valid only on H8/500, whose
;### maximum-mode physical space is 16MB (24-bit); the H8/520 and H8/530 are
;### limited to 1MB (20-bit), so these targets overflow there and this file is
;### included only by the H8/500 wrappers.

;### PJMP - Page Jump to a 24-bit address
        pjmp    @0x123456               ;### 13 12 34 56
        pjmp    @0xFFFFFF               ;### 13 FF FF FF

;### PJSR - Page Jump to Subroutine at a 24-bit address
        pjsr    @0x654321               ;### 03 65 43 21
        pjsr    @0xFFFFFF               ;### 03 FF FF FF
