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

        rtd     #$1234

        moves.b (a2), d1
        moves.b (a3)+, a2
        moves.b -(a4), d3
        moves.b ($1234,a5), a4
        moves.b (18,a6,d3.w), d5
        moves.b ($001234).w, a6
        moves.b ($123456).l, d7
        moves.w (a2), d1
        moves.w (a3)+, a2
        moves.w -(a4), d3
        moves.w ($1234,a5), a4
        moves.w (18,a6,d3.w), d5
        moves.w ($001234).w, a6
        moves.w ($123456).l, d7
        moves.l (a2), d1
        moves.l (a3)+, a2
        moves.l -(a4), d3
        moves.l ($1234,a5), a4
        moves.l (18,a6,d3.w), d5
        moves.l ($001234).w, a6
        moves.l ($123456).l, d7

        moves.b d1, (a2)
        moves.b a2, (a3)+
        moves.b d3, -(a4)
        moves.b a4, ($1234,a5)
        moves.b d5, (18,a6,d3.w)
        moves.b a6, ($001234).w
        moves.b d7, ($123456).l
        moves.w d1, (a2)
        moves.w a2, (a3)+
        moves.w d3, -(a4)
        moves.w a4, ($1234,a5)
        moves.w d5, (18,a6,d3.w)
        moves.w a6, ($001234).w
        moves.w d7, ($123456).l
        moves.l d1, (a2)
        moves.l a2, (a3)+
        moves.l d3, -(a4)
        moves.l a4, ($1234,a5)
        moves.l d5, (18,a6,d3.w)
        moves.l a6, ($001234).w
        moves.l d7, ($123456).l

        movec sfc, d1
        movec dfc, a2
        movec usp, d3
        movec vbr, a4
        movec d5, sfc
        movec a6, dfc
        movec d7, usp
        movec a0, vbr

;# Local Variables:
;# mode: asm
;# End:
;# vim: set ft=asm:
