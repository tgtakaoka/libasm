;;; Copyright 2023 Tadashi G. Takaoka
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

        cpu     f3850
        org     $100
;;; 0x
        lr      a,ku
        lr      a,kl
        lr      a,qu
        lr      a,ql
        lr      ku,a
        lr      kl,a
        lr      qu,a
        lr      ql,a
        lr      k,p
        lr      p,k
        lr      a,is
        lr      is,a
        pk
        lr      p0,q
        lr      q,dc
        lr      dc,q
;;; 1x
        lr      dc,h
        lr      h,dc
        sr      1
        sl      1
        sr      4
        sl      4
        lm
        st
        com
        lnk
        di
        ei
        pop
        lr      w,j
        lr      j,w
        inc
;;; 2x
        li      $21
        clr
        ni      $22
        oi      $23
        xi      $24
        ai      $25
        ci      $26
        in      $27
        out     $28
        pi      $292a
        jmp     $2a2b
        dci     $2b2c
        nop
        xdc
;;; 3x
        ds      0
        ds      1
        ds      2
        ds      3
        ds      4
        ds      5
        ds      6
        ds      7
        ds      8
        ds      9
        ds      10
        ds      11
        ds      12
        ds      13
        ds      14
        ds      15
        ds      j
        ds      hu
        ds      hl
        ds      s
        ds      i
        ds      d
;;; 4x
        lr      a,0
        lr      a,1
        lr      a,2
        lr      a,3
        lr      a,4
        lr      a,5
        lr      a,6
        lr      a,7
        lr      a,8
        lr      a,9
        lr      a,10
        lr      a,11
        lr      a,12
        lr      a,13
        lr      a,14
        lr      a,15
        lr      a,j
        lr      a,hu
        lr      a,hl
        lr      a,s
        lr      a,i
        lr      a,d
;;; 5x
        lr      0,a
        lr      1,a
        lr      2,a
        lr      3,a
        lr      4,a
        lr      5,a
        lr      6,a
        lr      7,a
        lr      8,a
        lr      9,a
        lr      10,a
        lr      11,a
        lr      12,a
        lr      13,a
        lr      14,a
        lr      15,a
        lr      j,a
        lr      hu,a
        lr      hl,a
        lr      s,a
        lr      i,a
        lr      d,a
;;; 6x
        lisu    0
        lisu    1
        lisu    2
        lisu    3
        lisu    4
        lisu    5
        lisu    6
        lisu    7
        lisl    0
        lisl    1
        lisl    2
        lisl    3
        lisl    4
        lisl    5
        lisl    6
        lisl    7
;;; 7x
        lis     0
        lis     1
        lis     2
        lis     3
        lis     4
        lis     5
        lis     6
        lis     7
        lis     8
        lis     9
        lis     10
        lis     11
        lis     12
        lis     13
        lis     14
        lis     15
;;; 8x
        bt      0,$-126
        bt      1,$-125
        bp      $-125
        bt      2,$-124
        bc      $-124
        bt      3,$-123
        bt      4,$-122
        bz      $-122
        bt      5,$-121
        bt      6,$-120
        bt      7,$-119
        am
        amd
        nm
        om
        xm
        cm
        adc
        br7     $-111
;;; 9x
        bf      0,$-110
        br      $-110
        bf      1,$-109
        bm      $-109
        bf      2,$-108
        bnc     $-108
        bf      3,$-107
        bf      4,$-106
        bnz     $-106
        bf      5,$-105
        bf      6,$-104
        bf      7,$-103
        bf      8,$-102
        bno     $-102
        bf      9,$-101
        bf      10,$-100
        bf      11,$-99
        bf      12,$-98
        bf      13,$-97
        bf      14,$-96
        bf      15,$-95
;;; Ax
        ins     0
        ins     1
        ins     2
        ins     3
        ins     4
        ins     5
        ins     6
        ins     7
        ins     8
        ins     9
        ins     10
        ins     11
        ins     12
        ins     13
        ins     14
        ins     15
;;; Bx
        outs    0
        outs    1
        outs    2
        outs    3
        outs    4
        outs    5
        outs    6
        outs    7
        outs    8
        outs    9
        outs    10
        outs    11
        outs    12
        outs    13
        outs    14
        outs    15
;;; Cx
        as      0
        as      1
        as      2
        as      3
        as      4
        as      5
        as      6
        as      7
        as      8
        as      9
        as      10
        as      11
        as      12
        as      13
        as      14
        as      15
        as      j
        as      hu
        as      hl
        as      s
        as      i
        as      d
;;; Dx
        asd     0
        asd     1
        asd     2
        asd     3
        asd     4
        asd     5
        asd     6
        asd     7
        asd     8
        asd     9
        asd     10
        asd     11
        asd     12
        asd     13
        asd     14
        asd     15
        asd     j
        asd     hu
        asd     hl
        asd     s
        asd     i
        asd     d
;;; Ex
        xs      0
        xs      1
        xs      2
        xs      3
        xs      4
        xs      5
        xs      6
        xs      7
        xs      8
        xs      9
        xs      10
        xs      11
        xs      12
        xs      13
        xs      14
        xs      15
        xs      j
        xs      hu
        xs      hl
        xs      s
        xs      i
        xs      d
;;; Fx
        ns      0
        ns      1
        ns      2
        ns      3
        ns      4
        ns      5
        ns      6
        ns      7
        ns      8
        ns      9
        ns      10
        ns      11
        ns      12
        ns      13
        ns      14
        ns      15
        ns      j
        ns      hu
        ns      hl
        ns      s
        ns      i
        ns      d

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
