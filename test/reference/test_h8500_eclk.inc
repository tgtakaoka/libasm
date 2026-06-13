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

;;; E-clock peripheral access instructions.
;;; Valid on H8/500 and H8/532 (not H8/520, which has no E-clock pin).
;;; Format C (EXT): [EA byte][EA ext][0x00][OP byte].

;;; MOVFPE @EAs,Rd (move byte from E-clock-synchronized memory to register)
        movfpe  @r0,r1                  ; D0 00 81
        movfpe  @0xa0:8,r2              ; 05 A0 00 82
        movfpe  @0x1234:16,r3           ; 15 12 34 00 83
        movfpe  @(0x10,r4),r5           ; E4 10 00 85
        movfpe  @(0x1234,r4),r5         ; F4 12 34 00 85

;;; MOVTPE Rs,@EAd (move byte from register to E-clock-synchronized memory)
        movtpe  r1,@r0                  ; D0 00 91
        movtpe  r2,@0xa0:8              ; 05 A0 00 92
        movtpe  r3,@0x1234:16           ; 15 12 34 00 93
        movtpe  r5,@(0x10,r4)           ; E4 10 00 95
        movtpe  r5,@(0x1234,r4)         ; F4 12 34 00 95

;;; Local Variables:
;;; mode: asm
;;; End:
