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

;;; 00X
        sram    @0123H(r15),15
        slam    r11,r0
        sm      *r4+,*r5+
        am      @0123H(r15),@0cdefH(r11)
        blsk    r3,4567H
;;; 01X
        bind    @2223H(r1)
        evad    r5
        rtwp    1
        rtwp    2
        rtwp    4
;;; 0CX
        tmb     @0123H(r15),7
        tcmb    r0,0
        tsmb    *r2,15

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
