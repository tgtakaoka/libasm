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

        db 0DDH,0E2H            ; fucom   st(2)
        db 0DDH,0EAH            ; fucomp  st(2)
        db 0DAH,0E9H            ; fucompp
        db 0D9H,0F5H            ; fprem1
        db 0D9H,0FBH            ; fsincos
        db 0D9H,0FEH            ; fsin
        db 0D9H,0FFH            ; fcos

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
