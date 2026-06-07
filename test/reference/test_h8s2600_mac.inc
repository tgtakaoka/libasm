;;; Copyright 2026 Tadashi G. Takaoka
;;;
;;; Licensed under the Apache License, Version 2.0 (the "License");
;;; you may not use this file except in compliance with the License.
;;; You may obtain a copy of the License at
;;;
;;;     http://www.apache.org/licenses/LICENSE-2.0
;;;
;;; Unless required by applicable law or agreed to in writing, software
;;; distributed under the License is distributed on an "AS IS" BASIS,
;;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;;; See the License for the specific language governing permissions and
;;; limitations under the License.

;;; H8S/2600 additions over H8S/2000: MAC unit (mode-shared)

;;; CLRMAC (01A0)
        clrmac                         ; 01A0

;;; MAC @ERn+,@ERm+ (0160 6D|ern*16|erm)
;;; Signed 16x16 multiply-accumulate; ERn is the multiplicand pointer,
;;; ERm is the multiplier pointer. Both registers post-increment by 2.
        mac     @er0+,@er0+            ; 0160 6D00
        mac     @er0+,@er7+            ; 0160 6D07
        mac     @er7+,@er0+            ; 0160 6D70
        mac     @er3+,@er4+            ; 0160 6D34

;;; LDMAC ERs,MACH (032|ers)
        ldmac   er0,mach               ; 0320
        ldmac   er1,mach               ; 0321
        ldmac   er2,mach               ; 0322
        ldmac   er3,mach               ; 0323
        ldmac   er4,mach               ; 0324
        ldmac   er5,mach               ; 0325
        ldmac   er6,mach               ; 0326
        ldmac   er7,mach               ; 0327

;;; LDMAC ERs,MACL (033|ers)
        ldmac   er0,macl               ; 0330
        ldmac   er1,macl               ; 0331
        ldmac   er2,macl               ; 0332
        ldmac   er3,macl               ; 0333
        ldmac   er4,macl               ; 0334
        ldmac   er5,macl               ; 0335
        ldmac   er6,macl               ; 0336
        ldmac   er7,macl               ; 0337

;;; STMAC MACH,ERd (022|erd)
        stmac   mach,er0               ; 0220
        stmac   mach,er1               ; 0221
        stmac   mach,er2               ; 0222
        stmac   mach,er3               ; 0223
        stmac   mach,er4               ; 0224
        stmac   mach,er5               ; 0225
        stmac   mach,er6               ; 0226
        stmac   mach,er7               ; 0227

;;; STMAC MACL,ERd (023|erd)
        stmac   macl,er0               ; 0230
        stmac   macl,er1               ; 0231
        stmac   macl,er2               ; 0232
        stmac   macl,er3               ; 0233
        stmac   macl,er4               ; 0234
        stmac   macl,er5               ; 0235
        stmac   macl,er6               ; 0236
        stmac   macl,er7               ; 0237

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
