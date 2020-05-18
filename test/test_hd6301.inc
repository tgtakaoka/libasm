;;; Copyright 2020 Tadashi G. Takaoka
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

;;; 1X
        xgdx
        slp
;;; 6X
        aim     #$62,$63,x
        oim     #$63,$64,x
        eim     #$66,$67,x
        tim     #$6c,$6d,x
        bclr    0,$63,x
        bset    1,$64,x
        btgl    2,$67,x
        btst    3,$6d,x
;;; 7X
        aim     #$72,$73
        oim     #$73,$74
        eim     #$76,$77
        tim     #$7c,$7d
        bclr    4,$73
        bset    5,$74
        btgl    6,$77
        btst    7,$7d

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
