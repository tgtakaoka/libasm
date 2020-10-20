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

        cpu     68000
        org     $10000

        align   32
        ori.b   #5,ccr
        ori.b   #5,d1
;       ori.b   #5,a2
        ori.b   #5,(a3)
        ori.b   #5,(a4)+
        ori.b   #5,-(a5)
        ori.b   #5,(4,a6)
        ori.b   #5,(8,a7,d3)
        ori.b   #5,(-4).w
        ori.b   #5,(8).l
;       ori.b   #5,(*,pc)
;       ori.b   #5,(*,pc,d3)
        ori.w   #5,sr
        ori.w   #5,d1
;       ori.w   #5,a2
        ori.w   #5,(a3)
        ori.w   #5,(a4)+
        ori.w   #5,-(a5)
        ori.w   #5,(4,a6)
        ori.w   #5,(8,a7,d3)
        ori.w   #5,(-4).w
        ori.w   #5,(8).l
;       ori.w   #5,(*,pc)
;       ori.w   #5,(*,pc,d3)
        ori.l   #5,d1
;       ori.l   #5,a2
        ori.l   #5,(a3)
        ori.l   #5,(a4)+
        ori.l   #5,-(a5)
        ori.l   #5,(4,a6)
        ori.l   #5,(8,a7,d3)
        ori.l   #5,(-4).w
        ori.l   #5,(8).l
;       ori.l   #5,(*,pc)
;       ori.l   #5,(*,pc,d3)

        align   32
        andi.b  #5,ccr
        andi.b  #5,d1
;       andi.b  #5,a2
        andi.b  #5,(a3)
        andi.b  #5,(a4)+
        andi.b  #5,-(a5)
        andi.b  #5,(4,a6)
        andi.b  #5,(8,a7,d3)
        andi.b  #5,(-4).w
        andi.b  #5,(8).l
;       andi.b  #5,(*,pc)
;       andi.b  #5,(*,pc,d3)
        andi.w  #5,sr
        andi.w  #5,d1
;       andi.w  #5,a2
        andi.w  #5,(a3)
        andi.w  #5,(a4)+
        andi.w  #5,-(a5)
        andi.w  #5,(4,a6)
        andi.w  #5,(8,a7,d3)
        andi.w  #5,(-4).w
        andi.w  #5,(8).l
;       andi.w  #5,(*,pc)
;       andi.w  #5,(*,pc,d3)
        andi.l  #5,d1
;       andi.l  #5,a2
        andi.l  #5,(a3)
        andi.l  #5,(a4)+
        andi.l  #5,-(a5)
        andi.l  #5,(4,a6)
        andi.l  #5,(8,a7,d3)
        andi.l  #5,(-4).w
        andi.l  #5,(8).l
;       andi.l  #5,(*,pc)
;       andi.l  #5,(*,pc,d3)

        align   32
        subi.b  #5,d1
;       subi.b  #5,a2
        subi.b  #5,(a3)
        subi.b  #5,(a4)+
        subi.b  #5,-(a5)
        subi.b  #5,(4,a6)
        subi.b  #5,(8,a7,d3)
        subi.b  #5,(-4).w
        subi.b  #5,(8).l
;       subi.b  #5,(*,pc)
;       subi.b  #5,(*,pc,d3)
        subi.w  #5,d1
        subi.w  #5,a2           ; suba.w #5,a2
        subi.w  #5,(a3)
        subi.w  #5,(a4)+
        subi.w  #5,-(a5)
        subi.w  #5,(4,a6)
        subi.w  #5,(8,a7,d3)
        subi.w  #5,(-4).w
        subi.w  #5,(8).l
;       subi.w  #5,(*,pc)
;       subi.w  #5,(*,pc,d3)
        subi.l  #5,d1
        subi.l  #5,a2           ; suba.l #5,a2
        subi.l  #5,(a3)
        subi.l  #5,(a4)+
        subi.l  #5,-(a5)
        subi.l  #5,(4,a6)
        subi.l  #5,(8,a7,d3)
        subi.l  #5,(-4).w
        subi.l  #5,(8).l
;       subi.l  #5,(*,pc)
;       subi.l  #5,(*,pc,d3)

        align   32
        addi.b  #5,d1
;       addi.b  #5,a2
        addi.b  #5,(a3)
        addi.b  #5,(a4)+
        addi.b  #5,-(a5)
        addi.b  #5,(4,a6)
        addi.b  #5,(8,a7,d3)
        addi.b  #5,(-4).w
        addi.b  #5,(8).l
;       addi.b  #5,(*,pc)
;       addi.b  #5,(*,pc,d3)
        addi.w  #5,d1
        addi.w  #5,a2           ; adda.w #5,a2
        addi.w  #5,(a3)
        addi.w  #5,(a4)+
        addi.w  #5,-(a5)
        addi.w  #5,(4,a6)
        addi.w  #5,(8,a7,d3)
        addi.w  #5,(-4).w
        addi.w  #5,(8).l
;       addi.w  #5,(*,pc)
;       addi.w  #5,(*,pc,d3)
        addi.l  #5,d1
        addi.l  #5,a2           ; adda.l #5,a2
        addi.l  #5,(a3)
        addi.l  #5,(a4)+
        addi.l  #5,-(a5)
        addi.l  #5,(4,a6)
        addi.l  #5,(8,a7,d3)
        addi.l  #5,(-4).w
        addi.l  #5,(8).l
;       addi.l  #5,(*,pc)
;       addi.l  #5,(*,pc,d3)

        align   32
        eori.b  #5,ccr
        eori.b  #5,d1
;       eori.b  #5,a2
        eori.b  #5,(a3)
        eori.b  #5,(a4)+
        eori.b  #5,-(a5)
        eori.b  #5,(4,a6)
        eori.b  #5,(8,a7,d3)
        eori.b  #5,(-4).w
        eori.b  #5,(8).l
;       eori.b  #5,(*,pc)
;       eori.b  #5,(*,pc,d3)
        eori.w  #5,sr
        eori.w  #5,d1
;       eori.w  #5,a2
        eori.w  #5,(a3)
        eori.w  #5,(a4)+
        eori.w  #5,-(a5)
        eori.w  #5,(4,a6)
        eori.w  #5,(8,a7,d3)
        eori.w  #5,(-4).w
        eori.w  #5,(8).l
;       eori.w  #5,(*,pc)
;       eori.w  #5,(*,pc,d3)
        eori.l  #5,d1
;       eori.l  #5,a2
        eori.l  #5,(a3)
        eori.l  #5,(a4)+
        eori.l  #5,-(a5)
        eori.l  #5,(4,a6)
        eori.l  #5,(8,a7,d3)
        eori.l  #5,(-4).w
        eori.l  #5,(8).l
;       eori.l  #5,(*,pc)
;       eori.l  #5,(*,pc,d3)

        align   32
        cmpi.b  #5,d1
;       cmpi.b  #5,a2
        cmpi.b  #5,(a3)
        cmpi.b  #5,(a4)+
        cmpi.b  #5,-(a5)
        cmpi.b  #5,(4,a6)
        cmpi.b  #5,(8,a7,d3)
        cmpi.b  #5,(-4).w
        cmpi.b  #5,(8).l
;       cmpi.b  #5,(*,pc)
;       cmpi.b  #5,(*,pc,d3)
        cmpi.w  #5,d1
        cmpi.w  #5,a2           ; cmpa.w #5,a2
        cmpi.w  #5,(a3)
        cmpi.w  #5,(a4)+
        cmpi.w  #5,-(a5)
        cmpi.w  #5,(4,a6)
        cmpi.w  #5,(8,a7,d3)
        cmpi.w  #5,(-4).w
        cmpi.w  #5,(8).l
;       cmpi.w  #5,(*,pc)
;       cmpi.w  #5,(*,pc,d3)
        cmpi.l  #5,d1
        cmpi.l  #5,a2           ; cmpa.l #5,a2
        cmpi.l  #5,(a3)
        cmpi.l  #5,(a4)+
        cmpi.l  #5,-(a5)
        cmpi.l  #5,(4,a6)
        cmpi.l  #5,(8,a7,d3)
        cmpi.l  #5,(-4).w
        cmpi.l  #5,(8).l
;       cmpi.l  #5,(*,pc)
;       cmpi.l  #5,(*,pc,d3)

        align   32
        bchg.l  #5,d1
;       bchg.l  #5,a2
        bchg.b  #5,(a3)
        bchg.b  #5,(a4)+
        bchg.b  #5,-(a5)
        bchg.b  #5,(4,a6)
        bchg.b  #5,(8,a7,d3)
        bchg.b  #5,(-4).w
        bchg.b  #5,(8).l
;       bchg.b  #5,(*,pc)
;       bchg.b  #5,(*,pc,d4)
        bchg.l  d2,d1
;       bchg.l  d2,a2
        bchg.b  d2,(a3)
        bchg.b  d2,(a4)+
        bchg.b  d2,-(a5)
        bchg.b  d2,(4,a6)
        bchg.b  d2,(8,a7,d3)
        bchg.b  d2,(-4).w
        bchg.b  d2,(8).l
;       bchg.b  d2,(*,pc)
;       bchg.b  d2,(*,pc,d4)

        align   32
        bclr.l  #5,d1
;       bclr.b  #5,a2
        bclr.b  #5,(a3)
        bclr.b  #5,(a4)+
        bclr.b  #5,-(a5)
        bclr.b  #5,(4,a6)
        bclr.b  #5,(8,a7,d3)
        bclr.b  #5,(-4).w
        bclr.b  #5,(8).l
;       bclr.b  #5,(*,pc)
;       bclr.b  #5,(*,pc,d4)
        bclr.l  d2,d1
;       bclr.l  d2,a2
        bclr.b  d2,(a3)
        bclr.b  d2,(a4)+
        bclr.b  d2,-(a5)
        bclr.b  d2,(4,a6)
        bclr.b  d2,(8,a7,d3)
        bclr.b  d2,(-4).w
        bclr.b  d2,(8).l
;       bclr.b  d2,(*,pc)
;       bclr.b  d2,(*,pc,d4)

        align   32
        bset.l  #5,d1
;       bset.b  #5,a2
        bset.b  #5,(a3)
        bset.b  #5,(a4)+
        bset.b  #5,-(a5)
        bset.b  #5,(4,a6)
        bset.b  #5,(8,a7,d3)
        bset.b  #5,(-4).w
        bset.b  #5,(8).l
;       bset.b  #5,(*,pc)
;       bset.b  #5,(*,pc,d4)
        bset.l  d2,d1
;       bset.l  d2,a2
        bset.b  d2,(a3)
        bset.b  d2,(a4)+
        bset.b  d2,-(a5)
        bset.b  d2,(4,a6)
        bset.b  d2,(8,a7,d3)
        bset.b  d2,(-4).w
        bset.b  d2,(8).l
;       bset.b  d2,(*,pc)
;       bset.b  d2,(*,pc,d4)

        align   32
        btst.l  #5,d1
;       btst.b  #5,a2
        btst.b  #5,(a3)
        btst.b  #5,(a4)+
        btst.b  #5,-(a5)
        btst.b  #5,(4,a6)
        btst.b  #5,(8,a7,d3)
        btst.b  #5,(-4).w
        btst.b  #5,(8).l
        btst.b  #5,(*,pc)
        btst.b  #5,(*,pc,d4)
        btst.l  d2,d1
;       btst.l  d2,a2
        btst.b  d2,(a3)
        btst.b  d2,(a4)+
        btst.b  d2,-(a5)
        btst.b  d2,(4,a6)
        btst.b  d2,(8,a7,d3)
        btst.b  d2,(-4).w
        btst.b  d2,(8).l
        btst.b  d2,(*,pc)
        btst.b  d2,(*,pc,d4)

        align   32
        movep.w d2,(4,a6)
        movep.w (4,a6),d1
        movep.l d2,(4,a6)
        movep.l (4,a6),d1

        align   32
        movea.w d2,a2
        movea.w a4,a2
        movea.w (a3),a2
        movea.w (a4)+,a2
        movea.w -(a5),a2
        movea.w (4,a6),a2
        movea.w (8,a7,d3),a2
        movea.w (-4).w,a2
        movea.w (8).l,a2
        movea.w #5,a2
        movea.w (*,pc),a2
        movea.w (*,pc,d3),a2
        movea.l d2,a2
        movea.l a4,a2
        movea.l (a3),a2
        movea.l (a4)+,a2
        movea.l -(a5),a2
        movea.l (4,a6),a2
        movea.l (8,a7,d3),a2
        movea.l (-4).w,a2
        movea.l (8).l,a2
        movea.l #5,a2
        movea.l (*,pc),a2
        movea.l (*,pc,d3),a2

        align   32
        move.w  sr,d1
;       move.w  sr,a2
        move.w  sr,(a3)
        move.w  sr,(a4)+
        move.w  sr,-(a5)
        move.w  sr,(4,a6)
        move.w  sr,(8,a7,d3)
        move.w  sr,(-4).w
        move.w  sr,(8).l
;       move.w  sr,(*,pc)
;       move.w  sr,(*,pc,d3)

        align   32
        move.b  d1,ccr
;       move.b  a2,ccr
        move.b  (a3),ccr
        move.b  (a4)+,ccr
        move.b  -(a5),ccr
        move.b  (4,a6),ccr
        move.b  (8,a7,d3),ccr
        move.b  (-4).w,ccr
        move.b  (8).l,ccr
        move.b  #5,ccr
        move.b  (*,pc),ccr
        move.b  (*,pc,d3),ccr

        align   32
        move.w  d1,sr
;       move.w  a2,sr
        move.w  (a3),sr
        move.w  (a4)+,sr
        move.w  -(a5),sr
        move.w  (4,a6),sr
        move.w  (8,a7,d3),sr
        move.w  (-4).w,sr
        move.w  (8).l,sr
        move.w  #5,sr
        move.w  (*,pc),sr
        move.w  (*,pc,d3),sr

        align   32
        move.b  d2,d1
;       move.b  a4,d1
        move.b  (a3),d1
        move.b  (a4)+,d1
        move.b  -(a5),d1
        move.b  (4,a6),d1
        move.b  (8,a7,d3),d1
        move.b  (-4).w,d1
        move.b  (8).l,d1
        move.b  #5,d1
        move.b  (*,pc),d1
        move.b  (*,pc,d3),d1
;       move.b  d2,a2
;       move.b  a4,a2
;       move.b  (a3),a2
;       move.b  (a4)+,a2
;       move.b  -(a5),a2
;       move.b  (4,a6),a2
;       move.b  (8,a7,d3),a2
;       move.b  (-4).w,a2
;       move.b  (8).l,a2
;       move.b  #5,a2
;       move.b  (*,pc),a2
;       move.b  (*,pc,d3),a2
        move.b  d2,(a3)
;       move.b  a4,(a3)
        move.b  (a3),(a3)
        move.b  (a4)+,(a3)
        move.b  -(a5),(a3)
        move.b  (4,a6),(a3)
        move.b  (8,a7,d3),(a3)
        move.b  (-4).w,(a3)
        move.b  (8).l,(a3)
        move.b  #5,(a3)
        move.b  (*,pc),(a3)
        move.b  (*,pc,d3),(a3)
;       move.b  a4,(a4)+
        move.b  (a3),(a4)+
        move.b  (a4)+,(a4)+
        move.b  -(a5),(a4)+
        move.b  (4,a6),(a4)+
        move.b  (8,a7,d3),(a4)+
        move.b  (-4).w,(a4)+
        move.b  (8).l,(a4)+
        move.b  #5,(a4)+
        move.b  (*,pc),(a4)+
        move.b  (*,pc,d3),(a4)+
;       move.b  a4,-(a5)
        move.b  (a3),-(a5)
        move.b  (a4)+,-(a5)
        move.b  -(a5),-(a5)
        move.b  (4,a6),-(a5)
        move.b  (8,a7,d3),-(a5)
        move.b  (-4).w,-(a5)
        move.b  (8).l,-(a5)
        move.b  #5,-(a5)
        move.b  (*,pc),-(a5)
        move.b  (*,pc,d3),-(a5)
;       move.b  a4,(4,a6)
        move.b  (a3),(4,a6)
        move.b  (a4)+,(4,a6)
        move.b  -(a5),(4,a6)
        move.b  (4,a6),(4,a6)
        move.b  (8,a7,d3),(4,a6)
        move.b  (-4).w,(4,a6)
        move.b  (8).l,(4,a6)
        move.b  #5,(4,a6)
        move.b  (*,pc),(4,a6)
        move.b  (*,pc,d3),(4,a6)
;       move.b  a4,(0,a4,d2)
        move.b  (a3),(0,a4,d2)
        move.b  (a4)+,(0,a4,d2)
        move.b  -(a5),(0,a4,d2)
        move.b  (4,a6),(0,a4,d2)
        move.b  (8,a7,d3),(0,a4,d2)
        move.b  (-4).w,(0,a4,d2)
        move.b  (8).l,(0,a4,d2)
        move.b  #5,(0,a4,d2)
        move.b  (*,pc),(0,a4,d2)
        move.b  (*,pc,d3),(0,a4,d2)
;       move.b  a4,(-4).w
        move.b  (a3),(-4).w
        move.b  (a4)+,(-4).w
        move.b  -(a5),(-4).w
        move.b  (4,a6),(-4).w
        move.b  (8,a7,d3),(-4).w
        move.b  (-4).w,(-4).w
        move.b  (8).l,(-4).w
        move.b  #5,(-4).w
        move.b  (*,pc),(-4).w
        move.b  (*,pc,d3),(-4).w
;       move.b  a4,(8).l
        move.b  (a3),(8).l
        move.b  (a4)+,(8).l
        move.b  -(a5),(8).l
        move.b  (4,a6),(8).l
        move.b  (8,a7,d3),(8).l
        move.b  (-4).w,(8).l
        move.b  (8).l,(8).l
        move.b  #5,(8).l
        move.b  (*,pc),(8).l
        move.b  (*,pc,d3),(8).l

        align   32
        move.w  d2,d1
        move.w  a4,d1
        move.w  (a3),d1
        move.w  (a4)+,d1
        move.w  -(a5),d1
        move.w  (4,a6),d1
        move.w  (8,a7,d3),d1
        move.w  (-4).w,d1
        move.w  (8).l,d1
        move.w  #5,d1
        move.w  (*,pc),d1
        move.w  (*,pc,d3),d1
        move.w  d2,a2
        move.w  a4,a2
        move.w  (a3),a2
        move.w  (a4)+,a2
        move.w  -(a5),a2
        move.w  (4,a6),a2
        move.w  (8,a7,d3),a2
        move.w  (-4).w,a2
        move.w  (8).l,a2
        move.w  #5,a2
        move.w  (*,pc),a2
        move.w  (*,pc,d3),a2
        move.w  d2,(a3)
        move.w  a4,(a3)
        move.w  (a3),(a3)
        move.w  (a4)+,(a3)
        move.w  -(a5),(a3)
        move.w  (4,a6),(a3)
        move.w  (8,a7,d3),(a3)
        move.w  (-4).w,(a3)
        move.w  (8).l,(a3)
        move.w  #5,(a3)
        move.w  (*,pc),(a3)
        move.w  (*,pc,d3),(a3)
        move.w  a4,(a4)+
        move.w  (a3),(a4)+
        move.w  (a4)+,(a4)+
        move.w  -(a5),(a4)+
        move.w  (4,a6),(a4)+
        move.w  (8,a7,d3),(a4)+
        move.w  (-4).w,(a4)+
        move.w  (8).l,(a4)+
        move.w  #5,(a4)+
        move.w  (*,pc),(a4)+
        move.w  (*,pc,d3),(a4)+
        move.w  a4,-(a5)
        move.w  (a3),-(a5)
        move.w  (a4)+,-(a5)
        move.w  -(a5),-(a5)
        move.w  (4,a6),-(a5)
        move.w  (8,a7,d3),-(a5)
        move.w  (-4).w,-(a5)
        move.w  (8).l,-(a5)
        move.w  #5,-(a5)
        move.w  (*,pc),-(a5)
        move.w  (*,pc,d3),-(a5)
        move.w  a4,(4,a6)
        move.w  (a3),(4,a6)
        move.w  (a4)+,(4,a6)
        move.w  -(a5),(4,a6)
        move.w  (4,a6),(4,a6)
        move.w  (8,a7,d3),(4,a6)
        move.w  (-4).w,(4,a6)
        move.w  (8).l,(4,a6)
        move.w  #5,(4,a6)
        move.w  (*,pc),(4,a6)
        move.w  (*,pc,d3),(4,a6)
        move.w  a4,(0,a4,d2)
        move.w  (a3),(0,a4,d2)
        move.w  (a4)+,(0,a4,d2)
        move.w  -(a5),(0,a4,d2)
        move.w  (4,a6),(0,a4,d2)
        move.w  (8,a7,d3),(0,a4,d2)
        move.w  (-4).w,(0,a4,d2)
        move.w  (8).l,(0,a4,d2)
        move.w  #5,(0,a4,d2)
        move.w  (*,pc),(0,a4,d2)
        move.w  (*,pc,d3),(0,a4,d2)
        move.w  a4,(-4).w
        move.w  (a3),(-4).w
        move.w  (a4)+,(-4).w
        move.w  -(a5),(-4).w
        move.w  (4,a6),(-4).w
        move.w  (8,a7,d3),(-4).w
        move.w  (-4).w,(-4).w
        move.w  (8).l,(-4).w
        move.w  #5,(-4).w
        move.w  (*,pc),(-4).w
        move.w  (*,pc,d3),(-4).w
        move.w  a4,(8).l
        move.w  (a3),(8).l
        move.w  (a4)+,(8).l
        move.w  -(a5),(8).l
        move.w  (4,a6),(8).l
        move.w  (8,a7,d3),(8).l
        move.w  (-4).w,(8).l
        move.w  (8).l,(8).l
        move.w  #5,(8).l
        move.w  (*,pc),(8).l
        move.w  (*,pc,d3),(8).l

        align   32
        move.l  d2,d1
        move.l  a4,d1
        move.l  (a3),d1
        move.l  (a4)+,d1
        move.l  -(a5),d1
        move.l  (4,a6),d1
        move.l  (8,a7,d3),d1
        move.l  (-4).l,d1
        move.l  (8).l,d1
        move.l  #5,d1
        move.l  (*,pc),d1
        move.l  (*,pc,d3),d1
        move.l  d2,a2
        move.l  a4,a2
        move.l  (a3),a2
        move.l  (a4)+,a2
        move.l  -(a5),a2
        move.l  (4,a6),a2
        move.l  (8,a7,d3),a2
        move.l  (-4).l,a2
        move.l  (8).l,a2
        move.l  #5,a2
        move.l  (*,pc),a2
        move.l  (*,pc,d3),a2
        move.l  d2,(a3)
        move.l  a4,(a3)
        move.l  (a3),(a3)
        move.l  (a4)+,(a3)
        move.l  -(a5),(a3)
        move.l  (4,a6),(a3)
        move.l  (8,a7,d3),(a3)
        move.l  (-4).l,(a3)
        move.l  (8).l,(a3)
        move.l  #5,(a3)
        move.l  (*,pc),(a3)
        move.l  (*,pc,d3),(a3)
        move.l  a4,(a4)+
        move.l  (a3),(a4)+
        move.l  (a4)+,(a4)+
        move.l  -(a5),(a4)+
        move.l  (4,a6),(a4)+
        move.l  (8,a7,d3),(a4)+
        move.l  (-4).l,(a4)+
        move.l  (8).l,(a4)+
        move.l  #5,(a4)+
        move.l  (*,pc),(a4)+
        move.l  (*,pc,d3),(a4)+
        move.l  a4,-(a5)
        move.l  (a3),-(a5)
        move.l  (a4)+,-(a5)
        move.l  -(a5),-(a5)
        move.l  (4,a6),-(a5)
        move.l  (8,a7,d3),-(a5)
        move.l  (-4).l,-(a5)
        move.l  (8).l,-(a5)
        move.l  #5,-(a5)
        move.l  (*,pc),-(a5)
        move.l  (*,pc,d3),-(a5)
        move.l  a4,(4,a6)
        move.l  (a3),(4,a6)
        move.l  (a4)+,(4,a6)
        move.l  -(a5),(4,a6)
        move.l  (4,a6),(4,a6)
        move.l  (8,a7,d3),(4,a6)
        move.l  (-4).l,(4,a6)
        move.l  (8).l,(4,a6)
        move.l  #5,(4,a6)
        move.l  (*,pc),(4,a6)
        move.l  (*,pc,d3),(4,a6)
        move.l  a4,(0,a4,d2)
        move.l  (a3),(0,a4,d2)
        move.l  (a4)+,(0,a4,d2)
        move.l  -(a5),(0,a4,d2)
        move.l  (4,a6),(0,a4,d2)
        move.l  (8,a7,d3),(0,a4,d2)
        move.l  (-4).l,(0,a4,d2)
        move.l  (8).l,(0,a4,d2)
        move.l  #5,(0,a4,d2)
        move.l  (*,pc),(0,a4,d2)
        move.l  (*,pc,d3),(0,a4,d2)
        move.l  a4,(-4).l
        move.l  (a3),(-4).l
        move.l  (a4)+,(-4).l
        move.l  -(a5),(-4).l
        move.l  (4,a6),(-4).l
        move.l  (8,a7,d3),(-4).l
        move.l  (-4).l,(-4).l
        move.l  (8).l,(-4).l
        move.l  #5,(-4).l
        move.l  (*,pc),(-4).l
        move.l  (*,pc,d3),(-4).l
        move.l  a4,(8).l
        move.l  (a3),(8).l
        move.l  (a4)+,(8).l
        move.l  -(a5),(8).l
        move.l  (4,a6),(8).l
        move.l  (8,a7,d3),(8).l
        move.l  (-4).l,(8).l
        move.l  (8).l,(8).l
        move.l  #5,(8).l
        move.l  (*,pc),(8).l
        move.l  (*,pc,d3),(8).l

        align   32
        negx.b  d1
;       negx.b  a2
        negx.b  (a3)
        negx.b  (a4)+
        negx.b  -(a5)
        negx.b  (4,a6)
        negx.b  (8,a7,d3)
        negx.b  (-4).w
        negx.b  (8).l
;       negx.b  (*,pc)
;       negx.b  (*,pc,d3)
        negx.w  d1
;       negx.w  a2
        negx.w  (a3)
        negx.w  (a4)+
        negx.w  -(a5)
        negx.w  (4,a6)
        negx.w  (8,a7,d3)
        negx.w  (-4).w
        negx.w  (8).l
;       negx.w  (*,pc)
;       negx.w  (*,pc,d3)
        negx.l  d1
;       negx.l  a2
        negx.l  (a3)
        negx.l  (a4)+
        negx.l  -(a5)
        negx.l  (4,a6)
        negx.l  (8,a7,d3)
        negx.l  (-4).w
        negx.l  (8).l
;       negx.l  (*,pc)
;       negx.l  (*,pc,d3)

        align   32
        clr.b   d1
;       clr.b   a2
        clr.b   (a3)
        clr.b   (a4)+
        clr.b   -(a5)
        clr.b   (4,a6)
        clr.b   (8,a7,d3)
        clr.b   (-4).w
        clr.b   (8).l
;       clr.b   (*,pc)
;       clr.b   (*,pc,d3)
        clr.w   d1
;       clr.w   a2
        clr.w   (a3)
        clr.w   (a4)+
        clr.w   -(a5)
        clr.w   (4,a6)
        clr.w   (8,a7,d3)
        clr.w   (-4).w
        clr.w   (8).l
;       clr.w   (*,pc)
;       clr.w   (*,pc,d3)
        clr.l   d1
;       clr.l   a2
        clr.l   (a3)
        clr.l   (a4)+
        clr.l   -(a5)
        clr.l   (4,a6)
        clr.l   (8,a7,d3)
        clr.l   (-4).w
        clr.l   (8).l
;       clr.l   (*,pc)
;       clr.l   (*,pc,d3)

        align   32
        neg.b   d1
;       neg.b   a2
        neg.b   (a3)
        neg.b   (a4)+
        neg.b   -(a5)
        neg.b   (4,a6)
        neg.b   (8,a7,d3)
        neg.b   (-4).w
        neg.b   (8).l
;       neg.b   (*,pc)
;       neg.b   (*,pc,d3)
        neg.w   d1
;       neg.w   a2
        neg.w   (a3)
        neg.w   (a4)+
        neg.w   -(a5)
        neg.w   (4,a6)
        neg.w   (8,a7,d3)
        neg.w   (-4).w
        neg.w   (8).l
;       neg.w   (*,pc)
;       neg.w   (*,pc,d3)
        neg.l   d1
;       neg.l   a2
        neg.l   (a3)
        neg.l   (a4)+
        neg.l   -(a5)
        neg.l   (4,a6)
        neg.l   (8,a7,d3)
        neg.l   (-4).w
        neg.l   (8).l
;       neg.l   (*,pc)
;       neg.l   (*,pc,d3)

        align   32
        not.b   d1
;       not.b   a2
        not.b   (a3)
        not.b   (a4)+
        not.b   -(a5)
        not.b   (4,a6)
        not.b   (8,a7,d3)
        not.b   (-4).w
        not.b   (8).l
;       not.b   (*,pc)
;       not.b   (*,pc,d3)
        not.w   d1
;       not.w   a2
        not.w   (a3)
        not.w   (a4)+
        not.w   -(a5)
        not.w   (4,a6)
        not.w   (8,a7,d3)
        not.w   (-4).w
        not.w   (8).l
;       not.w   (*,pc)
;       not.w   (*,pc,d3)
        not.l   d1
;       not.l   a2
        not.l   (a3)
        not.l   (a4)+
        not.l   -(a5)
        not.l   (4,a6)
        not.l   (8,a7,d3)
        not.l   (-4).w
        not.l   (8).l
;       not.l   (*,pc)
;       not.l   (*,pc,d3)

        align   32
        ext.w   d1
        ext.l   d1

        align   32
        nbcd.b  d1
;       nbcd.b  a2
        nbcd.b  (a3)
        nbcd.b  (a4)+
        nbcd.b  -(a5)
        nbcd.b  (4,a6)
        nbcd.b  (8,a7,d3)
        nbcd.b  (-4).w
        nbcd.b  (8).l
;       nbcd.b  (*,pc)
;       nbcd.b  (*,pc,d3)

        align   32
        swap    d1

        align   32
;       pea.l   d1
;       pea.l   a2
        pea.l   (a3)
;       pea.l   (a4)+
;       pea.l   -(a5)
        pea.l   (4,a6)
        pea.l   (8,a7,d3)
        pea.l   (-4).w
        pea.l   (8).l
;       pea.l   #5
        pea.l   (*,pc)
        pea.l   (*,pc,d3)

        align   32
        tas.b   d1
;       tas.b   a2
        tas.b   (a3)
        tas.b   (a4)+
        tas.b   -(a5)
        tas.b   (4,a6)
        tas.b   (8,a7,d3)
        tas.b   (-4).w
        tas.b   (8).l
;       tas.b   (*,pc)
;       tas.b   (*,pc,d3)

        align   32
        tst.b   d1
;       tst.b   a2
        tst.b   (a3)
        tst.b   (a4)+
        tst.b   -(a5)
        tst.b   (4,a6)
        tst.b   (8,a7,d3)
        tst.b   (-4).w
        tst.b   (8).l
;       tst.b   #$45
;       tst.b   (*,pc)
;       tst.b   (*,pc,d3)
        tst.w   d1
;       tst.w   a2
        tst.w   (a3)
        tst.w   (a4)+
        tst.w   -(a5)
        tst.w   (4,a6)
        tst.w   (8,a7,d3)
        tst.w   (-4).w
        tst.w   (8).l
;       tst.w   #$1234
;       tst.w   (*,pc)
;       tst.w   (*,pc,d3)
        tst.l   d1
;       tst.l   a2
        tst.l   (a3)
        tst.l   (a4)+
        tst.l   -(a5)
        tst.l   (4,a6)
        tst.l   (8,a7,d3)
        tst.l   (-4).w
        tst.l   (8).l
;       tst.l   #$12345678
;       tst.l   (*,pc)
;       tst.l   (*,pc,d3)

        align   32
        illegal
        trap    #5
        link    a1,#5
        unlk    a1
        move.l  a1,usp
        move.l  usp,a1
        reset
        nop
        stop    #5
        rte
        rts
        trapv
        rtr

        align   32
;       jsr     d1
;       jsr     a2
        jsr     (a3)
;       jsr     (a4)+
;       jsr     -(a5)
        jsr     (4,a6)
        jsr     (8,a7,d3)
        jsr     (-4).w
        jsr     (8).l
;       jsr     #5
        jsr     (*,pc)
        jsr     (*,pc,d3)

        align   32
;       jmp     d1
;       jmp     a2
        jmp     (a3)
;       jmp     (a4)+
;       jmp     -(a5)
        jmp     (4,a6)
        jmp     (8,a7,d3)
        jmp     (-4).w
        jmp     (8).l
;       jmp     #5
        jmp     (*,pc)
        jmp     (*,pc,d3)

        align   32
        movem.w d0-d7,(a3)
;       movem.w d0-d7,(a4)+
        movem.w d0-d7,-(a5)
        movem.w a7/a5/a3/a1/d7/d5/d3/d1,(4,a6)
        movem.w a6/a4/a2/a0/d6/d4/d2/d0,(8,a7,d3)
        movem.w a0-a7/d0-d7,(-4).w
        movem.w a0-a7/d0-d7,(8).l
;       movem.w a0-a7/d0-d7,(*,pc)
;       movem.w a0-a7/d0-d7,(*,pc,d2)
        movem.l a0-a7/d0-d7,(a3)
;       movem.l a0-a7/d0-d7,(a4)+
        movem.l a0-a7/d0-d7,-(a5)
        movem.l a0-a7/d0-d7,(4,a6)
        movem.l a0-a7/d0-d7,(8,a7,d3)
        movem.l a0-a7/d0-d7,(-4).l
        movem.l a0-a7/d0-d7,(8).l
;       movem.l a0-a7/d0-d7,(*,pc)
;       movem.l a0-a7/d0-d7,(*,pc,d2)

        movem.w (a3),d0-d7
        movem.w (a4)+,a0-a7
;       movem.w -(a5),a0-a7
        movem.w (4,a6),d0/d2/d4/d6/a0/a2/a4/a6
        movem.w (8,a7,d3),d1/d3/d5/d7/a1/a3/a5/a7
        movem.w (-4).w,d0-d7/a0-a7
        movem.w (8).l,d0-d7/a0-a7
        movem.w (*,pc),d0-d7/a0-a7
        movem.w (*,pc,d2),d0-d7/a0-a7
        movem.l (a3),d0-d7/a0-a7
        movem.l (a4)+,d0-d7/a0-a7
;       movem.l -(a5),d0-d7/a0-a7
        movem.l (4,a6),d0-d7/a0-a7
        movem.l (8,a7,d3),d0-d7/a0-a7
        movem.l (-4).l,d0-d7/a0-a7
        movem.l (8).l,d0-d7/a0-a7
        movem.l (*,pc),d0-d7/a0-a7
        movem.l (*,pc,d2),d0-d7/a0-a7

        align   32
;       lea.l   d1,a2
;       lea.l   a2,a2
        lea.l   (a3),a2
;       lea.l   (a4)+,a2
;       lea.l   -(a5),a2
        lea.l   (4,a6),a2
        lea.l   (8,a7,d3),a2
        lea.l   (-4).w,a2
        lea.l   (8).l,a2
;       lea.l   #5,a2
        lea.l   (*,pc),a2
        lea.l   (*,pc,d3),a2

        align   32
        chk.w   d2,d1
;       chk.w   a4,d1
        chk.w   (a3),d1
        chk.w   (a4)+,d1
        chk.w   -(a5),d1
        chk.w   (4,a6),d1
        chk.w   (8,a7,d3),d1
        chk.w   (-4).w,d1
        chk.w   (8).l,d1
        chk.w   #5,d1
        chk.w   (*,pc),d1
        chk.w   (*,pc,d3),d1

        align   32
        addq.b  #5,d1
;       addq.b  #5,a2
        addq.b  #5,(a3)
        addq.b  #5,(a4)+
        addq.b  #5,-(a5)
        addq.b  #5,(4,a6)
        addq.b  #5,(8,a7,d3)
        addq.b  #5,(-4).w
        addq.b  #5,(8).l
;       addq.b  #5,(*,pc)
;       addq.b  #5,(*,pc,d3)
        addq.w  #5,d1
        addq.w  #5,a2
        addq.w  #5,(a3)
        addq.w  #5,(a4)+
        addq.w  #5,-(a5)
        addq.w  #5,(4,a6)
        addq.w  #5,(8,a7,d3)
        addq.w  #5,(-4).w
        addq.w  #5,(8).l
;       addq.w  #5,(*,pc)
;       addq.w  #5,(*,pc,d3)
        addq.l  #5,d1
        addq.l  #5,a2
        addq.l  #5,(a3)
        addq.l  #5,(a4)+
        addq.l  #5,-(a5)
        addq.l  #5,(4,a6)
        addq.l  #5,(8,a7,d3)
        addq.l  #5,(-4).w
        addq.l  #5,(8).l
;       addq.l  #5,(*,pc)
;       addq.l  #5,(*,pc,d3)

        align   32
        subq.b  #5,d1
;       subq.b  #5,a2
        subq.b  #5,(a3)
        subq.b  #5,(a4)+
        subq.b  #5,-(a5)
        subq.b  #5,(4,a6)
        subq.b  #5,(8,a7,d3)
        subq.b  #5,(-4).w
        subq.b  #5,(8).l
;       subq.b  #5,(*,pc)
;       subq.b  #5,(*,pc,d3)
        subq.w  #5,d1
        subq.w  #5,a2
        subq.w  #5,(a3)
        subq.w  #5,(a4)+
        subq.w  #5,-(a5)
        subq.w  #5,(4,a6)
        subq.w  #5,(8,a7,d3)
        subq.w  #5,(-4).w
        subq.w  #5,(8).l
;       subq.w  #5,(*,pc)
;       subq.w  #5,(*,pc,d3)
        subq.l  #5,d1
        subq.l  #5,a2
        subq.l  #5,(a3)
        subq.l  #5,(a4)+
        subq.l  #5,-(a5)
        subq.l  #5,(4,a6)
        subq.l  #5,(8,a7,d3)
        subq.l  #5,(-4).w
        subq.l  #5,(8).l
;       subq.l  #5,(*,pc)
;       subq.l  #5,(*,pc,d3)

        align   32
        st.b    d1
;       st.b    a2
        st.b    (a3)
        st.b    (a4)+
        st.b    -(a5)
        st.b    (4,a6)
        st.b    (8,a7,d3)
        st.b    (-4).w
        st.b    (8).l
;       st.b    (*,pc)
;       st.b    (*,pc,d3)

        align   32
        sf.b    d1
;       sf.b    a2
        sf.b    (a3)
        sf.b    (a4)+
        sf.b    -(a5)
        sf.b    (4,a6)
        sf.b    (8,a7,d3)
        sf.b    (-4).w
        sf.b    (8).l
;       sf.b    (*,pc)
;       sf.b    (*,pc,d3)

        align   32
        shi.b   d1
;       shi.b   a2
        shi.b   (a3)
        shi.b   (a4)+
        shi.b   -(a5)
        shi.b   (4,a6)
        shi.b   (8,a7,d3)
        shi.b   (-4).w
        shi.b   (8).l
;       shi.b   (*,pc)
;       shi.b   (*,pc,d3)

        align   32
        sls.b   d1
;       sls.b   a2
        sls.b   (a3)
        sls.b   (a4)+
        sls.b   -(a5)
        sls.b   (4,a6)
        sls.b   (8,a7,d3)
        sls.b   (-4).w
        sls.b   (8).l
;       sls.b   (*,pc)
;       sls.b   (*,pc,d3)

        align   32
        scc.b   d1
;       scc.b   a2
        scc.b   (a3)
        scc.b   (a4)+
        scc.b   -(a5)
        scc.b   (4,a6)
        scc.b   (8,a7,d3)
        scc.b   (-4).w
        scc.b   (8).l
;       scc.b   (*,pc)
;       scc.b   (*,pc,d3)

        align   32
        scs.b   d1
;       scs.b   a2
        scs.b   (a3)
        scs.b   (a4)+
        scs.b   -(a5)
        scs.b   (4,a6)
        scs.b   (8,a7,d3)
        scs.b   (-4).w
        scs.b   (8).l
;       scs.b   (*,pc)
;       scs.b   (*,pc,d3)

        align   32
        slo.b   d1
;       slo.b   a2
        slo.b   (a3)
        slo.b   (a4)+
        slo.b   -(a5)
        slo.b   (4,a6)
        slo.b   (8,a7,d3)
        slo.b   (-4).w
        slo.b   (8).l
;       slo.b   (*,pc)
;       slo.b   (*,pc,d3)

        align   32
        sls.b   d1
;       sls.b   a2
        sls.b   (a3)
        sls.b   (a4)+
        sls.b   -(a5)
        sls.b   (4,a6)
        sls.b   (8,a7,d3)
        sls.b   (-4).w
        sls.b   (8).l
;       sls.b   (*,pc)
;       sls.b   (*,pc,d3)

        align   32
        sne.b   d1
;       sne.b   a2
        sne.b   (a3)
        sne.b   (a4)+
        sne.b   -(a5)
        sne.b   (4,a6)
        sne.b   (8,a7,d3)
        sne.b   (-4).w
        sne.b   (8).l
;       sne.b   (*,pc)
;       sne.b   (*,pc,d3)

        align   32
        seq.b   d1
;       seq.b   a2
        seq.b   (a3)
        seq.b   (a4)+
        seq.b   -(a5)
        seq.b   (4,a6)
        seq.b   (8,a7,d3)
        seq.b   (-4).w
        seq.b   (8).l
;       seq.b   (*,pc)
;       seq.b   (*,pc,d3)

        align   32
        svc.b   d1
;       svc.b   a2
        svc.b   (a3)
        svc.b   (a4)+
        svc.b   -(a5)
        svc.b   (4,a6)
        svc.b   (8,a7,d3)
        svc.b   (-4).w
        svc.b   (8).l
;       svc.b   (*,pc)
;       svc.b   (*,pc,d3)

        align   32
        svs.b   d1
;       svs.b   a2
        svs.b   (a3)
        svs.b   (a4)+
        svs.b   -(a5)
        svs.b   (4,a6)
        svs.b   (8,a7,d3)
        svs.b   (-4).w
        svs.b   (8).l
;       svs.b   (*,pc)
;       svs.b   (*,pc,d3)

        align   32
        spl.b   d1
;       spl.b   a2
        spl.b   (a3)
        spl.b   (a4)+
        spl.b   -(a5)
        spl.b   (4,a6)
        spl.b   (8,a7,d3)
        spl.b   (-4).w
        spl.b   (8).l
;       spl.b   (*,pc)
;       spl.b   (*,pc,d3)

        align   32
        smi.b   d1
;       smi.b   a2
        smi.b   (a3)
        smi.b   (a4)+
        smi.b   -(a5)
        smi.b   (4,a6)
        smi.b   (8,a7,d3)
        smi.b   (-4).w
        smi.b   (8).l
;       smi.b   (*,pc)
;       smi.b   (*,pc,d3)

        align   32
        sge.b   d1
;       sge.b   a2
        sge.b   (a3)
        sge.b   (a4)+
        sge.b   -(a5)
        sge.b   (4,a6)
        sge.b   (8,a7,d3)
        sge.b   (-4).w
        sge.b   (8).l
;       sge.b   (*,pc)
;       sge.b   (*,pc,d3)

        align   32
        slt.b   d1
;       slt.b   a2
        slt.b   (a3)
        slt.b   (a4)+
        slt.b   -(a5)
        slt.b   (4,a6)
        slt.b   (8,a7,d3)
        slt.b   (-4).w
        slt.b   (8).l
;       slt.b   (*,pc)
;       slt.b   (*,pc,d3)

        align   32
        sgt.b   d1
;       sgt.b   a2
        sgt.b   (a3)
        sgt.b   (a4)+
        sgt.b   -(a5)
        sgt.b   (4,a6)
        sgt.b   (8,a7,d3)
        sgt.b   (-4).w
        sgt.b   (8).l
;       sgt.b   (*,pc)
;       sgt.b   (*,pc,d3)

        align   32
        sle.b   d1
;       sle.b   a2
        sle.b   (a3)
        sle.b   (a4)+
        sle.b   -(a5)
        sle.b   (4,a6)
        sle.b   (8,a7,d3)
        sle.b   (-4).w
        sle.b   (8).l
;       sle.b   (*,pc)
;       sle.b   (*,pc,d3)

        align   32
        dbt     d1,*
        dbra    d1,*
        dbf     d1,*
        dbhi    d1,*
        dbls    d1,*
        dbcc    d1,*
        dbhs    d1,*
        dbcs    d1,*
        dblo    d1,*
        dbne    d1,*
        dbeq    d1,*
        dbvc    d1,*
        dbvs    d1,*
        dbpl    d1,*
        dbmi    d1,*
        dbge    d1,*
        dblt    d1,*
        dbgt    d1,*
        dble    d1,*

        align   32
        bra.b   *
        bra.w   *
        bsr.b   *
        bsr.w   *
        bhi.b   *
        bhi.w   *
        bls.b   *
        bls.w   *
        bcc.b   *
        bcc.w   *
        bhs.b   *
        bhs.w   *
        bcs.b   *
        bcs.w   *
        blo.b   *
        blo.w   *
        bne.b   *
        bne.w   *
        beq.b   *
        beq.w   *
        bvc.b   *
        bvc.w   *
        bvs.b   *
        bvs.w   *
        bpl.b   *
        bpl.w   *
        bmi.b   *
        bmi.w   *
        bge.b   *
        bge.w   *
        blt.b   *
        blt.w   *
        bgt.b   *
        bgt.w   *
        ble.b   *
        ble.w   *

        align   32
        moveq.l #5,d1

        align   32
        divu.w  d2,d1
;       divu.w  a4,d1
        divu.w  (a3),d1
        divu.w  (a4)+,d1
        divu.w  -(a5),d1
        divu.w  (4,a6),d1
        divu.w  (8,a4,d1),d1
        divu.w  (-4).w,d1
        divu.w  (8).l,d1
        divu.w  #5,d1
        divu.w  (*,pc),d1
        divu.w  (*,pc,d3),d1

        align   32
        divs.w  d2,d1
;       divs.w  a4,d1
        divs.w  (a3),d1
        divs.w  (a4)+,d1
        divs.w  -(a5),d1
        divs.w  (4,a6),d1
        divs.w  (8,a4,d1),d1
        divs.w  (-4).w,d1
        divs.w  (8).l,d1
        divs.w  #5,d1
        divs.w  (*,pc),d1
        divs.w  (*,pc,d3),d1

        align   32
        sbcd.b  d2,d1
        sbcd.b  -(a2),-(a3)

        align   32
        or.b    d2,d1
;       or.b    a4,d1
        or.b    (a3),d1
        or.b    (a4)+,d1
        or.b    -(a5),d1
        or.b    (4,a6),d1
        or.b    (8,a7,d3),d1
        or.b    (-4).w,d1
        or.b    (8).l,d1
        or.b    #5,d1
        or.b    (*,pc),d1
        or.b    (*,pc,d3),d1
        or.b    d2,d1
;       or.b    d1,a4
        or.b    d1,(a3)
        or.b    d1,(a4)+
        or.b    d1,-(a5)
        or.b    d1,(4,a6)
        or.b    d1,(8,a7,d3)
        or.b    d1,(-4).w
        or.b    d1,(8).l
;       or.b    d1,#5
;       or.b    d1,(*,pc)
;       or.b    d1,(*,pc,d3)
        or.w    d2,d1
;       or.w    a4,d1
        or.w    (a3),d1
        or.w    (a4)+,d1
        or.w    -(a5),d1
        or.w    (4,a6),d1
        or.w    (8,a7,d3),d1
        or.w    (-4).w,d1
        or.w    (8).l,d1
        or.w    #5,d1
        or.w    (*,pc),d1
        or.w    (*,pc,d3),d1
        or.w    d2,d1
;       or.w    d1,a4
        or.w    d1,(a3)
        or.w    d1,(a4)+
        or.w    d1,-(a5)
        or.w    d1,(4,a6)
        or.w    d1,(8,a7,d3)
        or.w    d1,(-4).w
        or.w    d1,(8).l
;       or.w    d1,#5
;       or.w    d1,(*,pc)
;       or.w    d1,(*,pc,d3)
        or.l    d2,d1
;       or.l    a4,d1
        or.l    (a3),d1
        or.l    (a4)+,d1
        or.l    -(a5),d1
        or.l    (4,a6),d1
        or.l    (8,a7,d3),d1
        or.l    (-4).w,d1
        or.l    (8).l,d1
        or.l    #5,d1
        or.l    (*,pc),d1
        or.l    (*,pc,d3),d1
        or.l    d2,d1
;       or.l    d1,a4
        or.l    d1,(a3)
        or.l    d1,(a4)+
        or.l    d1,-(a5)
        or.l    d1,(4,a6)
        or.l    d1,(8,a7,d3)
        or.l    d1,(-4).w
        or.l    d1,(8).l
;       or.l    d1,#5
;       or.l    d1,(*,pc)
;       or.l    d1,(*,pc,d3)

        align   32
        sub.b   d2,d1
;       sub.b   a4,d1
        sub.b   (a3),d1
        sub.b   (a4)+,d1
        sub.b   -(a5),d1
        sub.b   (4,a6),d1
        sub.b   (8,a7,d3),d1
        sub.b   (-4).w,d1
        sub.b   (8).l,d1
        sub.b   #5,d1
        sub.b   (*,pc),d1
        sub.b   (*,pc,d3),d1
;       sub.b   d1,a4
        sub.b   d1,(a3)
        sub.b   d1,(a4)+
        sub.b   d1,-(a5)
        sub.b   d1,(4,a6)
        sub.b   d1,(8,a7,d3)
        sub.b   d1,(-4).w
        sub.b   d1,(8).l
;       sub.b   d1,#5
;       sub.b   d1,(*,pc)
;       sub.b   d1,(*,pc,d3)
        sub.w   d2,d1
        sub.w   a4,d1
        sub.w   (a3),d1
        sub.w   (a4)+,d1
        sub.w   -(a5),d1
        sub.w   (4,a6),d1
        sub.w   (8,a7,d3),d1
        sub.w   (-4).w,d1
        sub.w   (8).l,d1
        sub.w   #5,d1
        sub.w   (*,pc),d1
        sub.w   (*,pc,d3),d1
        sub.w   d1,a4           ; suba.w d1,a4
        sub.w   d1,(a3)
        sub.w   d1,(a4)+
        sub.w   d1,-(a5)
        sub.w   d1,(4,a6)
        sub.w   d1,(8,a7,d3)
        sub.w   d1,(-4).w
        sub.w   d1,(8).l
;       sub.w   d1,#5
;       sub.w   d1,(*,pc)
;       sub.w   d1,(*,pc,d3)
        sub.l   d2,d1
        sub.l   a4,d1
        sub.l   (a3),d1
        sub.l   (a4)+,d1
        sub.l   -(a5),d1
        sub.l   (4,a6),d1
        sub.l   (8,a7,d3),d1
        sub.l   (-4).w,d1
        sub.l   (8).l,d1
        sub.l   #5,d1
        sub.l   (*,pc),d1
        sub.l   (*,pc,d3),d1
        sub.l   d1,a4           ; suba.l d1,a4
        sub.l   d1,(a3)
        sub.l   d1,(a4)+
        sub.l   d1,-(a5)
        sub.l   d1,(4,a6)
        sub.l   d1,(8,a7,d3)
        sub.l   d1,(-4).w
        sub.l   d1,(8).l
;       sub.l   d1,#5
;       sub.l   d1,(*,pc)
;       sub.l   d1,(*,pc,d3)

        align   32
        subx.b  d2,d1
        subx.b  -(a2),-(a3)
        subx.w  d2,d1
        subx.w  -(a2),-(a3)

        align   32
        suba.w  d2,a2
        suba.w  a4,a2
        suba.w  (a3),a2
        suba.w  (a4)+,a2
        suba.w  -(a5),a2
        suba.w  (4,a6),a2
        suba.w  (8,a7,d3),a2
        suba.w  (-4).w,a2
        suba.w  (8).l,a2
        suba.w  #5,a2
        suba.w  (*,pc),a2
        suba.w  (*,pc,d3),a2
        suba.l  d2,a2
        suba.l  a4,a2
        suba.l  (a3),a2
        suba.l  (a4)+,a2
        suba.l  -(a5),a2
        suba.l  (4,a6),a2
        suba.l  (8,a7,d3),a2
        suba.l  (-4).w,a2
        suba.l  (8).l,a2
        suba.l  #5,a2
        suba.l  (*,pc),a2
        suba.l  (*,pc,d3),a2

        align   32
        eor.b   d1,d2
;       eor.b   d1,a4
        eor.b   d1,(a3)
        eor.b   d1,(a4)+
        eor.b   d1,-(a5)
        eor.b   d1,(4,a6)
        eor.b   d1,(8,a7,d3)
        eor.b   d1,(-4).w
        eor.b   d1,(8).l
;       eor.b   d1,#5
;       eor.b   d1,(*,pc)
;       eor.b   d1,(*,pc,d3)
        eor.w   d1,d2
;       eor.w   d1,a4
        eor.w   d1,(a3)
        eor.w   d1,(a4)+
        eor.w   d1,-(a5)
        eor.w   d1,(4,a6)
        eor.w   d1,(8,a7,d3)
        eor.w   d1,(-4).w
        eor.w   d1,(8).l
;       eor.w   d1,#5
;       eor.w   d1,(*,pc)
;       eor.w   d1,(*,pc,d3)
        eor.l   d1,d2
;       eor.l   d1,a4
        eor.l   d1,(a3)
        eor.l   d1,(a4)+
        eor.l   d1,-(a5)
        eor.l   d1,(4,a6)
        eor.l   d1,(8,a7,d3)
        eor.l   d1,(-4).w
        eor.l   d1,(8).l
;       eor.l   d1,#5
;       eor.l   d1,(*,pc)
;       eor.l   d1,(*,pc,d3)

        align   32
        cmpm.b  (a2)+,(a3)+
        cmpm.w  (a2)+,(a3)+
        cmpm.l  (a2)+,(a3)+

        align   32
        cmp.b   d2,d1
;       cmp.b   a4,d1
        cmp.b   (a3),d1
        cmp.b   (a4)+,d1
        cmp.b   -(a5),d1
        cmp.b   (4,a6),d1
        cmp.b   (8,a7,d3),d1
        cmp.b   (-4).w,d1
        cmp.b   (8).l,d1
        cmp.b   #5,d1
        cmp.b   (*,pc),d1
        cmp.b   (*,pc,d3),d1
        cmp.w   d2,d1
        cmp.w   a4,d1
        cmp.w   (a3),d1
        cmp.w   (a4)+,d1
        cmp.w   -(a5),d1
        cmp.w   (4,a6),d1
        cmp.w   (8,a7,d3),d1
        cmp.w   (-4).w,d1
        cmp.w   (8).l,d1
        cmp.w   #5,d1
        cmp.w   (*,pc),d1
        cmp.w   (*,pc,d3),d1
        cmp.l   d2,d1
        cmp.l   a4,d1
        cmp.l   (a3),d1
        cmp.l   (a4)+,d1
        cmp.l   -(a5),d1
        cmp.l   (4,a6),d1
        cmp.l   (8,a7,d3),d1
        cmp.l   (-4).w,d1
        cmp.l   (8).l,d1
        cmp.l   #5,d1
        cmp.l   (*,pc),d1
        cmp.l   (*,pc,d3),d1

        align   32
        cmpa.w  d2,a2
        cmpa.w  a4,a2
        cmpa.w  (a3),a2
        cmpa.w  (a4)+,a2
        cmpa.w  -(a5),a2
        cmpa.w  (4,a6),a2
        cmpa.w  (8,a7,d3),a2
        cmpa.w  (-4).w,a2
        cmpa.w  (8).l,a2
        cmpa.w  #5,a2
        cmpa.w  (*,pc),a2
        cmpa.w  (*,pc,d3),a2
        cmpa.l  d2,a2
        cmpa.l  a4,a2
        cmpa.l  (a3),a2
        cmpa.l  (a4)+,a2
        cmpa.l  -(a5),a2
        cmpa.l  (4,a6),a2
        cmpa.l  (8,a7,d3),a2
        cmpa.l  (-4).w,a2
        cmpa.l  (8).l,a2
        cmpa.l  #5,a2
        cmpa.l  (*,pc),a2
        cmpa.l  (*,pc,d3),a2

        align   32
        mulu.w  d2,d1
;       mulu.w  a4,d1
        mulu.w  (a3),d1
        mulu.w  (a4)+,d1
        mulu.w  -(a5),d1
        mulu.w  (4,a6),d1
        mulu.w  (8,a4,d1),d1
        mulu.w  (-4).w,d1
        mulu.w  (8).l,d1
        mulu.w  #5,d1
        mulu.w  (*,pc),d1
        mulu.w  (*,pc,d3),d1

        align   32
        muls.w  d2,d1
;       muls.w  a4,d1
        muls.w  (a3),d1
        muls.w  (a4)+,d1
        muls.w  -(a5),d1
        muls.w  (4,a6),d1
        muls.w  (8,a4,d1),d1
        muls.w  (-4).w,d1
        muls.w  (8).l,d1
        muls.w  #5,d1
        muls.w  (*,pc),d1
        muls.w  (*,pc,d3),d1

        align   32
        abcd.b  d2,d1
        abcd.b  -(a2),-(a3)

        align   32
        exg.l   d1,d2
        exg.l   d1,a2
        exg.l   a1,a2

        align   32
        and.b   d2,d1
;       and.b   a4,d1
        and.b   (a3),d1
        and.b   (a4)+,d1
        and.b   -(a5),d1
        and.b   (4,a6),d1
        and.b   (8,a7,d3),d1
        and.b   (-4).w,d1
        and.b   (8).l,d1
        and.b   #5,d1
        and.b   (*,pc),d1
        and.b   (*,pc,d3),d1
;       and.b   d1,a4
        and.b   d1,(a3)
        and.b   d1,(a4)+
        and.b   d1,-(a5)
        and.b   d1,(4,a6)
        and.b   d1,(8,a7,d3)
        and.b   d1,(-4).w
        and.b   d1,(8).l
;       and.b   d1,#5
;       and.b   d1,(*,pc)
;       and.b   d1,(*,pc,d3)
        and.w   d2,d1
;       and.w   a4,d1
        and.w   (a3),d1
        and.w   (a4)+,d1
        and.w   -(a5),d1
        and.w   (4,a6),d1
        and.w   (8,a7,d3),d1
        and.w   (-4).w,d1
        and.w   (8).l,d1
        and.w   #5,d1
        and.w   (*,pc),d1
        and.w   (*,pc,d3),d1
;       and.w   d1,a4
        and.w   d1,(a3)
        and.w   d1,(a4)+
        and.w   d1,-(a5)
        and.w   d1,(4,a6)
        and.w   d1,(8,a7,d3)
        and.w   d1,(-4).w
        and.w   d1,(8).l
;       and.w   d1,#5
;       and.w   d1,(*,pc)
;       and.w   d1,(*,pc,d3)
        and.l   d2,d1
;       and.l   a4,d1
        and.l   (a3),d1
        and.l   (a4)+,d1
        and.l   -(a5),d1
        and.l   (4,a6),d1
        and.l   (8,a7,d3),d1
        and.l   (-4).w,d1
        and.l   (8).l,d1
        and.l   #5,d1
        and.l   (*,pc),d1
        and.l   (*,pc,d3),d1
;       and.l   d1,a4
        and.l   d1,(a3)
        and.l   d1,(a4)+
        and.l   d1,-(a5)
        and.l   d1,(4,a6)
        and.l   d1,(8,a7,d3)
        and.l   d1,(-4).w
        and.l   d1,(8).l
;       and.l   d1,#5
;       and.l   d1,(*,pc)
;       and.l   d1,(*,pc,d3)

        align   32
        add.b   d2,d1
;       add.b   a4,d1
        add.b   (a3),d1
        add.b   (a4)+,d1
        add.b   -(a5),d1
        add.b   (4,a6),d1
        add.b   (8,a7,d3),d1
        add.b   (-4).w,d1
        add.b   (8).l,d1
        add.b   #5,d1
        add.b   (*,pc),d1
        add.b   (*,pc,d3),d1
;       add.b   d1,a4
        add.b   d1,(a3)
        add.b   d1,(a4)+
        add.b   d1,-(a5)
        add.b   d1,(4,a6)
        add.b   d1,(8,a7,d3)
        add.b   d1,(-4).w
        add.b   d1,(8).l
;       add.b   d1,#5
;       add.b   d1,(*,pc)
;       add.b   d1,(*,pc,d3)
        add.w   d2,d1
        add.w   a4,d1
        add.w   (a3),d1
        add.w   (a4)+,d1
        add.w   -(a5),d1
        add.w   (4,a6),d1
        add.w   (8,a7,d3),d1
        add.w   (-4).w,d1
        add.w   (8).l,d1
        add.w   #5,d1
        add.w   (*,pc),d1
        add.w   (*,pc,d3),d1
        add.w   d1,a4           ; adda.w d1,a4
        add.w   d1,(a3)
        add.w   d1,(a4)+
        add.w   d1,-(a5)
        add.w   d1,(4,a6)
        add.w   d1,(8,a7,d3)
        add.w   d1,(-4).w
        add.w   d1,(8).l
;       add.w   d1,#5
;       add.w   d1,(*,pc)
;       add.w   d1,(*,pc,d3)
        add.l   d2,d1
        add.l   a4,d1
        add.l   (a3),d1
        add.l   (a4)+,d1
        add.l   -(a5),d1
        add.l   (4,a6),d1
        add.l   (8,a7,d3),d1
        add.l   (-4).w,d1
        add.l   (8).l,d1
        add.l   #5,d1
        add.l   (*,pc),d1
        add.l   (*,pc,d3),d1
        add.l   d1,a4           ; adda.l d1,a4
        add.l   d1,(a3)
        add.l   d1,(a4)+
        add.l   d1,-(a5)
        add.l   d1,(4,a6)
        add.l   d1,(8,a7,d3)
        add.l   d1,(-4).w
        add.l   d1,(8).l
;       add.l   d1,#5
;       add.l   d1,(*,pc)
;       add.l   d1,(*,pc,d3)

        align   32
        addx.b  d2,d1
        addx.b  -(a2),-(a3)
        addx.w  d2,d1
        addx.w  -(a2),-(a3)

        align   32
        adda.w  d2,a2
        adda.w  a4,a2
        adda.w  (a3),a2
        adda.w  (a4)+,a2
        adda.w  -(a5),a2
        adda.w  (4,a6),a2
        adda.w  (8,a7,d3),a2
        adda.w  (-4).w,a2
        adda.w  (8).l,a2
        adda.w  #5,a2
        adda.w  (*,pc),a2
        adda.w  (*,pc,d3),a2
        adda.l  d2,a2
        adda.l  a4,a2
        adda.l  (a3),a2
        adda.l  (a4)+,a2
        adda.l  -(a5),a2
        adda.l  (4,a6),a2
        adda.l  (8,a7,d3),a2
        adda.l  (-4).w,a2
        adda.l  (8).l,a2
        adda.l  #5,a2
        adda.l  (*,pc),a2
        adda.l  (*,pc,d3),a2

        align   32
        asr.b   d2,d1
        asr.b   #5,d1
        asr.w   d2,d1
        asr.w   #5,d1
        asr.l   d2,d1
        asr.l   #5,d1
        asr.w   (a3)
        asr.w   (a4)+
        asr.w   -(a5)
        asr.w   (4,a6)
        asr.w   (8,a7,d3)
        asr.w   (-4).w
        asr.w   (8).l
;       asr.w   (*,pc)
;       asr.w   (*,pc,d3)

        align   32
        asl.b   d2,d1
        asl.b   #5,d1
        asl.w   d2,d1
        asl.w   #5,d1
        asl.l   d2,d1
        asl.l   #5,d1
        asl.w   (a3)
        asl.w   (a4)+
        asl.w   -(a5)
        asl.w   (4,a6)
        asl.w   (8,a7,d3)
        asl.w   (-4).w
        asl.w   (8).l
;       asl.w   (*,pc)
;       asl.w   (*,pc,d3)

        align   32
        lsr.b   d2,d1
        lsr.b   #5,d1
        lsr.w   d2,d1
        lsr.w   #5,d1
        lsr.l   d2,d1
        lsr.l   #5,d1
        lsr.w   (a3)
        lsr.w   (a4)+
        lsr.w   -(a5)
        lsr.w   (4,a6)
        lsr.w   (8,a7,d3)
        lsr.w   (-4).w
        lsr.w   (8).l
;       lsr.w   (*,pc)
;       lsr.w   (*,pc,d3)

        align   32
        lsl.b   d2,d1
        lsl.b   #5,d1
        lsl.w   d2,d1
        lsl.w   #5,d1
        lsl.l   d2,d1
        lsl.l   #5,d1
        lsl.w   (a3)
        lsl.w   (a4)+
        lsl.w   -(a5)
        lsl.w   (4,a6)
        lsl.w   (8,a7,d3)
        lsl.w   (-4).w
        lsl.w   (8).l
;       lsl.w   (*,pc)
;       lsl.w   (*,pc,d3)

        align   32
        roxr.b  d2,d1
        roxr.b  #5,d1
        roxr.w  d2,d1
        roxr.w  #5,d1
        roxr.l  d2,d1
        roxr.l  #5,d1
        roxr.w  (a3)
        roxr.w  (a4)+
        roxr.w  -(a5)
        roxr.w  (4,a6)
        roxr.w  (8,a7,d3)
        roxr.w  (-4).w
        roxr.w  (8).l
;       roxr.w  (*,pc)
;       roxr.w  (*,pc,d3)

        align   32
        roxl.b  d2,d1
        roxl.b  #5,d1
        roxl.w  d2,d1
        roxl.w  #5,d1
        roxl.l  d2,d1
        roxl.l  #5,d1
        roxl.w  (a3)
        roxl.w  (a4)+
        roxl.w  -(a5)
        roxl.w  (4,a6)
        roxl.w  (8,a7,d3)
        roxl.w  (-4).w
        roxl.w  (8).l
;       roxl.w  (*,pc)
;       roxl.w  (*,pc,d3)

        align   32
        ror.b   d2,d1
        ror.b   #5,d1
        ror.w   d2,d1
        ror.w   #5,d1
        ror.l   d2,d1
        ror.l   #5,d1
        ror.w   (a3)
        ror.w   (a4)+
        ror.w   -(a5)
        ror.w   (4,a6)
        ror.w   (8,a7,d3)
        ror.w   (-4).w
        ror.w   (8).l
;       ror.w   (*,pc)
;       ror.w   (*,pc,d3)

        align   32
        rol.b   d2,d1
        rol.b   #5,d1
        rol.w   d2,d1
        rol.w   #5,d1
        rol.l   d2,d1
        rol.l   #5,d1
        rol.w   (a3)
        rol.w   (a4)+
        rol.w   -(a5)
        rol.w   (4,a6)
        rol.w   (8,a7,d3)
        rol.w   (-4).w
        rol.w   (8).l
;       rol.w   (*,pc)
;       rol.w   (*,pc,d3)

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
