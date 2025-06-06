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

;### MC68040 CPU
        move16  (a1)+, (a2)+
        move16  (a1)+, ($12345678)
        move16  ($12345678), (a1)+
        move16  (a1), ($12345678)
        move16  ($12345678), (a1)
        cinvl   dc, (a2)
        cinvl   ic, (a2)
        cinvl   dc/ic, (a2)
        cinvl   nc, (a2)
        cinvp   dc, (a2)
        cinvp   ic, (a2)
        cinvp   dc/ic, (a2)
        cinvp   nc, (a2)
        cinva   dc
        cinva   ic
        cinva   dc/ic
        cinva   nc
        cpushl  dc, (a2)
        cpushl  ic, (a2)
        cpushl  dc/ic, (a2)
        cpushl  nc, (a2)
        cpushp  dc, (a2)
        cpushp  ic, (a2)
        cpushp  dc/ic, (a2)
        cpushp  nc, (a2)
        cpusha  dc
        cpusha  ic
        cpusha  dc/ic
        cpusha  nc
        movec   d1, tc
        movec   a2, itt0
        movec   d3, itt1
        movec   a4, dtt0
        movec   d5, dtt1
        movec   a6, mmusr
        movec   d7, urp
        movec   a0, srp
        movec   tc, d0
        movec   itt0, a1
        movec   itt1, d2
        movec   dtt0, a3
        movec   dtt1, d4
        movec   mmusr, a5
        movec   urp, d6
        movec   srp, a7

;### MC68040 FPU
        fmove.x fp0, fp1
        fmove.l d2, fp3
        fmove.x (a6), fp7
        fmove.p (a0)+, fp1
        fmove.w -(a2), fp3
        fmove.d ($1234,a4), fp5
        fmove.b ($23,a6,d7.w), fp0
        fmove.l ($004566).w, fp1
        fmove.s ($56789a).l, fp2
        fmove.x (*+$1234,pc), fp3
        fmove.p (*+90,pc,a4.l), fp5
        fmove.l #$6789abcd, fp6
        fmove.s #7.88999976e-10, fp7
        fmove.x #-89000000032, fp0
        fmove.p #9.12e20, fp1
        fmove.w #$1234, fp2
        fmove.d #-8.25, fp3
        fmove.b #$23, fp4
        fmove.l fp3, d2
        fmove.x fp7, (a6)
        fmove.p fp1, (a0)+
        fmove.p fp1, (a0)+{#16}
        fmove.p fp1, (a0)+{d2}
        fmove.w fp3, -(a2)
        fmove.d fp5, ($1234,a4)
        fmove.b fp0, ($23,a6,d7.w)
        fmove.l fp1, ($004566).w
        fmove.s fp2, ($56789a).l
        fmovem.x (a6), fp0
        fmovem.x (a6), fp0/fp4-fp7
        fmovem.x (a0)+, fp7
        fmovem.x (a0)+, fp0/fp4-fp7
        fmovem.x ($1234,a4), fp3
        fmovem.x ($1234,a4), fp3-fp5
        fmovem.x ($23,a6,d7.w), fp3
        fmovem.x ($004566).w, fp4
        fmovem.x ($56789a).l, fp5
        fmovem.x (*+$1234,pc), fp6
        fmovem.x (*+90,pc,a4.l), fp7
        fmovem.x (a6), d1
        fmovem.x (a0)+, d2
        fmovem.x ($1234,a4), d3
        fmovem.x ($23,a6,d7.w), d4
        fmovem.x ($004566).w, d5
        fmovem.x ($56789a).l, d6
        fmovem.x (*+$1234,pc), d7
        fmovem.x (*+90,pc,a4.l), d0
        fmovem.x fp0/fp6, (a6)
        fmovem.x fp0/fp4-fp7, (a6)
        fmovem.x fp0/fp4-fp7, -(a2)
        fmovem.x fp0, -(a2)
        fmovem.x fp3-fp5, ($1234,a4)
        fmovem.x fp2, ($23,a6,d7.w)
        fmovem.x fp1, ($004566).w
        fmovem.x fp0, ($56789a).l
        fmovem.x d1, (a6)
        fmovem.x d2, -(a2)
        fmovem.x d3, ($1234,a4)
        fmovem.x d4, ($23,a6,d7.w)
        fmovem.x d5, ($004566).w
        fmovem.x d6, ($56789a).l
        fmove.l d0, fpcr
        fmove.l d1, fpsr
        fmove.l d2, fpiar
        fmove.l a5, fpiar
        fmove.l (a6), fpcr
        fmove.l (a7), fpsr
        fmove.l (a0), fpiar
        fmove.l (a1)+, fpcr
        fmove.l (a2)+, fpsr
        fmove.l (a3)+, fpiar
        fmove.l -(a4), fpcr
        fmove.l -(a5), fpsr
        fmove.l -(a6), fpiar
        fmove.l ($1234,a7), fpcr
        fmove.l ($2345,a0), fpsr
        fmove.l ($3456,a1), fpiar
        fmove.l ($45,a2,d1.w), fpcr
        fmove.l ($56,a3,d2.l), fpsr
        fmove.l ($67,a4,a3.w), fpiar
        fmove.l ($00789a).w, fpcr
        fmove.l ($ffff89aa).w, fpsr
        fmove.l ($ffff9abc).w, fpiar
        fmove.l ($abcdee).l, fpcr
        fmove.l ($bcdef0).l, fpsr
        fmove.l ($cdef00).l, fpiar
        fmove.l (*+$1234,pc), fpcr
        fmove.l (*+$2344,pc), fpsr
        fmove.l (*+$3456,pc), fpiar
        fmove.l (*+73,pc,d1.w), fpcr
        fmove.l (*+90,pc,d2.l), fpsr
        fmove.l (*+107,pc,a3.w), fpiar
        fmove.l #$12345678, fpcr
        fmove.l #$23456789, fpsr
        fmove.l #$3456789a, fpiar
        fmove.l fpcr, d0
        fmove.l fpsr, d1
        fmove.l fpiar, d2
        fmove.l fpiar, a5
        fmove.l fpcr, (a6)
        fmove.l fpsr, (a7)
        fmove.l fpiar, (a0)
        fmove.l fpcr, (a1)+
        fmove.l fpsr, (a2)+
        fmove.l fpiar, (a3)+
        fmove.l fpcr, -(a4)
        fmove.l fpsr, -(a5)
        fmove.l fpiar, -(a6)
        fmove.l fpcr, ($1234,a7)
        fmove.l fpsr, ($2345,a0)
        fmove.l fpiar, ($3456,a1)
        fmove.l fpcr, ($45,a2,d1.w)
        fmove.l fpsr, ($56,a3,d2.l)
        fmove.l fpiar, ($67,a4,a3.w)
        fmove.l fpcr, ($00789a).w
        fmove.l fpsr, ($ffff89aa).w
        fmove.l fpiar, ($ffff9abc).w
        fmove.l fpcr, ($abcdee).l
        fmove.l fpsr, ($bcdef0).l
        fmove.l fpiar, ($cdef00).l
        fmovem.l d0, fpcr
        fmovem.l d1, fpsr
        fmovem.l d2, fpiar
        fmovem.l a5, fpiar
        fmovem.l (a3), fpcr/fpiar
        fmovem.l (a4)+, fpcr/fpsr/fpiar
        fmovem.l -(a5), fpcr/fpsr
        fmovem.l ($1234,a6), fpsr/fpiar
        fmovem.l ($34,a7,d2.w), fpcr/fpiar
        fmovem.l ($005678).w, fpcr/fpsr/fpiar
        fmovem.l ($6789aa).l, fpcr/fpsr
        fmovem.l (*+$1234,pc), fpsr/fpiar
        fmovem.l (*+73,pc,d1.w), fpcr/fpiar
        fmovem.l fpcr, d0
        fmovem.l fpsr, d1
        fmovem.l fpiar, d2
        fmovem.l fpiar, a5
        fmovem.l fpcr/fpiar, (a3)
        fmovem.l fpcr/fpsr/fpiar, (a4)+
        fmovem.l fpcr/fpsr, -(a5)
        fmovem.l fpsr/fpiar, ($1234,a6)
        fmovem.l fpcr/fpiar, ($34,a7,d2.w)
        fmovem.l fpcr/fpsr/fpiar, ($005678).w
        fmovem.l fpcr/fpsr, ($6789aa).l

;;; Dyadic Operations
        fdiv.x fp0, fp1
        fdiv.l d2, fp3
        fdiv.x (a6), fp7
        fdiv.p (a0)+, fp1
        fdiv.w -(a2), fp3
        fdiv.d ($1234,a4), fp5
        fdiv.b ($23,a6,d7.w), fp0
        fdiv.l ($004566).w, fp1
        fdiv.s ($56789a).l, fp2
        fdiv.x (*+$1234,pc), fp3
        fdiv.p (*+90,pc,a4.l), fp5
        fdiv.l #$6789abcd, fp6
        fdiv.s #7.88999976e-10, fp7
        fdiv.x #-89000000032, fp0
        fdiv.p #9.12e20, fp1
        fdiv.w #$1234, fp2
        fdiv.d #-8.25, fp3
        fdiv.b #$23, fp4
        fadd.x fp0, fp1
        fadd.l d2, fp3
        fadd.x (a6), fp7
        fadd.p (a0)+, fp1
        fadd.w -(a2), fp3
        fadd.d ($1234,a4), fp5
        fadd.b ($23,a6,d7.w), fp0
        fadd.l ($004566).w, fp1
        fadd.s ($56789a).l, fp2
        fadd.x (*+$1234,pc), fp3
        fadd.p (*+90,pc,a4.l), fp5
        fadd.l #$6789abcd, fp6
        fadd.s #7.88999976e-10, fp7
        fadd.x #-89000000032, fp0
        fadd.p #9.12e20, fp1
        fadd.w #$1234, fp2
        fadd.d #-8.25, fp3
        fadd.b #$23, fp4
        fmul.x fp0, fp1
        fmul.l d2, fp3
        fmul.x (a6), fp7
        fmul.p (a0)+, fp1
        fmul.w -(a2), fp3
        fmul.d ($1234,a4), fp5
        fmul.b ($23,a6,d7.w), fp0
        fmul.l ($004566).w, fp1
        fmul.s ($56789a).l, fp2
        fmul.x (*+$1234,pc), fp3
        fmul.p (*+90,pc,a4.l), fp5
        fmul.l #$6789abcd, fp6
        fmul.s #7.88999976e-10, fp7
        fmul.x #-89000000032, fp0
        fmul.p #9.12e20, fp1
        fmul.w #$1234, fp2
        fmul.d #-8.25, fp3
        fmul.b #$23, fp4
        fsgldiv.x fp0, fp1
        fsgldiv.l d2, fp3
        fsgldiv.x (a6), fp7
        fsgldiv.p (a0)+, fp1
        fsgldiv.w -(a2), fp3
        fsgldiv.d ($1234,a4), fp5
        fsgldiv.b ($23,a6,d7.w), fp0
        fsgldiv.l ($004566).w, fp1
        fsgldiv.s ($56789a).l, fp2
        fsgldiv.x (*+$1234,pc), fp3
        fsgldiv.p (*+90,pc,a4.l), fp5
        fsgldiv.l #$6789abcd, fp6
        fsgldiv.s #7.88999976e-10, fp7
        fsgldiv.x #-89000000032, fp0
        fsgldiv.p #9.12e20, fp1
        fsgldiv.w #$1234, fp2
        fsgldiv.d #-8.25, fp3
        fsgldiv.b #$23, fp4
        fsglmul.x fp0, fp1
        fsglmul.l d2, fp3
        fsglmul.x (a6), fp7
        fsglmul.p (a0)+, fp1
        fsglmul.w -(a2), fp3
        fsglmul.d ($1234,a4), fp5
        fsglmul.b ($23,a6,d7.w), fp0
        fsglmul.l ($004566).w, fp1
        fsglmul.s ($56789a).l, fp2
        fsglmul.x (*+$1234,pc), fp3
        fsglmul.p (*+90,pc,a4.l), fp5
        fsglmul.l #$6789abcd, fp6
        fsglmul.s #7.88999976e-10, fp7
        fsglmul.x #-89000000032, fp0
        fsglmul.p #9.12e20, fp1
        fsglmul.w #$1234, fp2
        fsglmul.d #-8.25, fp3
        fsglmul.b #$23, fp4
        fsub.x fp0, fp1
        fsub.l d2, fp3
        fsub.x (a6), fp7
        fsub.p (a0)+, fp1
        fsub.w -(a2), fp3
        fsub.d ($1234,a4), fp5
        fsub.b ($23,a6,d7.w), fp0
        fsub.l ($004566).w, fp1
        fsub.s ($56789a).l, fp2
        fsub.x (*+$1234,pc), fp3
        fsub.p (*+90,pc,a4.l), fp5
        fsub.l #$6789abcd, fp6
        fsub.s #7.88999976e-10, fp7
        fsub.x #-89000000032, fp0
        fsub.p #9.12e20, fp1
        fsub.w #$1234, fp2
        fsub.d #-8.25, fp3
        fsub.b #$23, fp4
        fcmp.x fp0, fp1
        fcmp.l d2, fp3
        fcmp.x (a6), fp7
        fcmp.p (a0)+, fp1
        fcmp.w -(a2), fp3
        fcmp.d ($1234,a4), fp5
        fcmp.b ($23,a6,d7.w), fp0
        fcmp.l ($004566).w, fp1
        fcmp.s ($56789a).l, fp2
        fcmp.x (*+$1234,pc), fp3
        fcmp.p (*+90,pc,a4.l), fp5
        fcmp.l #$6789abcd, fp6
        fcmp.s #7.88999976e-10, fp7
        fcmp.x #-89000000032, fp0
        fcmp.p #9.12e20, fp1
        fcmp.w #$1234, fp2
        fcmp.d #-8.25, fp3
        fcmp.b #$23, fp4
        ftst.x fp0
        ftst.l d2
        ftst.x (a6)
        ftst.p (a0)+
        ftst.w -(a2)
        ftst.d ($1234,a4)
        ftst.b ($23,a6,d7.w)
        ftst.l ($004566).w
        ftst.s ($56789a).l
        ftst.x (*+$1234,pc)
        ftst.p (*+90,pc,a4.l)
        ftst.l #$6789abcd
        ftst.s #7.88999976e-10
        ftst.x #-89000000032
        ftst.p #9.12e20
        ftst.w #$1234
        ftst.d #-8.25
        ftst.b #$23

;;; monadic operations
        fsqrt.x fp0, fp1
        fsqrt.x fp2
        fsqrt.l d2, fp3
        fsqrt.x (a6), fp7
        fsqrt.p (a0)+, fp1
        fsqrt.w -(a2), fp3
        fsqrt.d ($1234,a4), fp5
        fsqrt.b ($23,a6,d7.w), fp0
        fsqrt.l ($004566).w, fp1
        fsqrt.s ($56789a).l, fp2
        fsqrt.x (*+$1234,pc), fp3
        fsqrt.p (*+90,pc,a4.l), fp5
        fsqrt.l #$6789abcd, fp6
        fsqrt.s #7.88999976e-10, fp7
        fsqrt.x #-89000000032, fp0
        fsqrt.p #9.12e20, fp1
        fsqrt.w #$1234, fp2
        fsqrt.d #-8.25, fp3
        fsqrt.b #$23, fp4
        fabs.x fp0, fp1
        fabs.x fp2
        fabs.l d2, fp3
        fabs.x (a6), fp7
        fabs.p (a0)+, fp1
        fabs.w -(a2), fp3
        fabs.d ($1234,a4), fp5
        fabs.b ($23,a6,d7.w), fp0
        fabs.l ($004566).w, fp1
        fabs.s ($56789a).l, fp2
        fabs.x (*+$1234,pc), fp3
        fabs.p (*+90,pc,a4.l), fp5
        fabs.l #$6789abcd, fp6
        fabs.s #7.88999976e-10, fp7
        fabs.x #-89000000032, fp0
        fabs.p #9.12e20, fp1
        fabs.w #$1234, fp2
        fabs.d #-8.25, fp3
        fabs.b #$23, fp4
        fneg.x fp0, fp1
        fneg.x fp2
        fneg.l d2, fp3
        fneg.x (a6), fp7
        fneg.p (a0)+, fp1
        fneg.w -(a2), fp3
        fneg.d ($1234,a4), fp5
        fneg.b ($23,a6,d7.w), fp0
        fneg.l ($004566).w, fp1
        fneg.s ($56789a).l, fp2
        fneg.x (*+$1234,pc), fp3
        fneg.p (*+90,pc,a4.l), fp5
        fneg.l #$6789abcd, fp6
        fneg.s #7.88999976e-10, fp7
        fneg.x #-89000000032, fp0
        fneg.p #9.12e20, fp1
        fneg.w #$1234, fp2
        fneg.d #-8.25, fp3
        fneg.b #$23, fp4

;;; program control operations
        fnop
        fbf    *+$1234
        fbeq.w *+$1234
        fbeq.l *+$1234
        fbogt  *+$1234
        fboge  *+$1234
        fbolt  *+$1234
        fbole  *+$1234
        fbogl  *+$1234
        fbor   *+$1234
        fbun   *+$1234
        fbueq  *+$1234
        fbugt  *+$1234
        fbuge  *+$1234
        fbult  *+$1234
        fbule  *+$1234
        fbne   *+$1234
        fbt.x  *+$1234
        fbsf   *+$1234
        fbseq  *+$1234
        fbgt   *+$1234
        fbge   *+$1234
        fblt   *+$1234
        fble   *+$1234
        fbgl   *+$1234
        fbgle  *+$1234
        fbngle *+$1234
        fbngl  *+$1234
        fbnle  *+$1234
        fbnlt  *+$1234
        fbnge  *+$1234
        fbngt  *+$1234
        fbsne  *+$1234
        fbst   *+$1234
        fbst.w *+$70
        fbst.l *+$70
        fbst.x *+$70

        fbf    *+$123456
        fbeq.x *+$001234
        fbogt  *+$123456
        fboge  *+$123456
        fbolt  *+$123456
        fbole  *+$123456
        fbogl  *+$123456
        fbor   *+$123456
        fbun   *+$123456
        fbueq  *+$123456
        fbugt  *+$123456
        fbuge  *+$123456
        fbult  *+$123456
        fbule  *+$123456
        fbne   *+$123456
        fbt    *+$123456
        fbsf   *+$123456
        fbseq  *+$123456
        fbgt   *+$123456
        fbge   *+$123456
        fblt   *+$123456
        fble   *+$123456
        fbgl   *+$123456
        fbgle  *+$123456
        fbngle *+$123456
        fbngl  *+$123456
        fbnle  *+$123456
        fbnlt  *+$123456
        fbnge  *+$123456
        fbngt  *+$123456
        fbsne  *+$123456
        fbst   *+$123456
        fsf d2
        fdbf d2, *+$1234
        fsf (a2)
        fsf (a2)+
        fsf -(a2)
        fsf ($1234,a2)
        fsf (18,a2,d3.w)
        fsf ($001234).w
        fsf ($123456).l
        fseq d2
        fdbeq d2, *+$1234
        fseq (a2)
        fseq (a2)+
        fseq -(a2)
        fseq ($1234,a2)
        fseq (18,a2,d3.w)
        fseq ($001234).w
        fseq ($123456).l
        fsogt d2
        fdbogt d2, *+$1234
        fsogt (a2)
        fsogt (a2)+
        fsogt -(a2)
        fsogt ($1234,a2)
        fsogt (18,a2,d3.w)
        fsogt ($001234).w
        fsogt ($123456).l
        fsoge d2
        fdboge d2, *+$1234
        fsoge (a2)
        fsoge (a2)+
        fsoge -(a2)
        fsoge ($1234,a2)
        fsoge (18,a2,d3.w)
        fsoge ($001234).w
        fsoge ($123456).l
        fsolt d2
        fdbolt d2, *+$1234
        fsolt (a2)
        fsolt (a2)+
        fsolt -(a2)
        fsolt ($1234,a2)
        fsolt (18,a2,d3.w)
        fsolt ($001234).w
        fsolt ($123456).l
        fsole d2
        fdbole d2, *+$1234
        fsole (a2)
        fsole (a2)+
        fsole -(a2)
        fsole ($1234,a2)
        fsole (18,a2,d3.w)
        fsole ($001234).w
        fsole ($123456).l
        fsogl d2
        fdbogl d2, *+$1234
        fsogl (a2)
        fsogl (a2)+
        fsogl -(a2)
        fsogl ($1234,a2)
        fsogl (18,a2,d3.w)
        fsogl ($001234).w
        fsogl ($123456).l
        fsor d2
        fdbor d2, *+$1234
        fsor (a2)
        fsor (a2)+
        fsor -(a2)
        fsor ($1234,a2)
        fsor (18,a2,d3.w)
        fsor ($001234).w
        fsor ($123456).l
        fsun d2
        fdbun d2, *+$1234
        fsun (a2)
        fsun (a2)+
        fsun -(a2)
        fsun ($1234,a2)
        fsun (18,a2,d3.w)
        fsun ($001234).w
        fsun ($123456).l
        fsueq d2
        fdbueq d2, *+$1234
        fsueq (a2)
        fsueq (a2)+
        fsueq -(a2)
        fsueq ($1234,a2)
        fsueq (18,a2,d3.w)
        fsueq ($001234).w
        fsueq ($123456).l
        fsugt d2
        fdbugt d2, *+$1234
        fsugt (a2)
        fsugt (a2)+
        fsugt -(a2)
        fsugt ($1234,a2)
        fsugt (18,a2,d3.w)
        fsugt ($001234).w
        fsugt ($123456).l
        fsuge d2
        fdbuge d2, *+$1234
        fsuge (a2)
        fsuge (a2)+
        fsuge -(a2)
        fsuge ($1234,a2)
        fsuge (18,a2,d3.w)
        fsuge ($001234).w
        fsuge ($123456).l
        fsult d2
        fdbult d2, *+$1234
        fsult (a2)
        fsult (a2)+
        fsult -(a2)
        fsult ($1234,a2)
        fsult (18,a2,d3.w)
        fsult ($001234).w
        fsult ($123456).l
        fsule d2
        fdbule d2, *+$1234
        fsule (a2)
        fsule (a2)+
        fsule -(a2)
        fsule ($1234,a2)
        fsule (18,a2,d3.w)
        fsule ($001234).w
        fsule ($123456).l
        fsne d2
        fdbne d2, *+$1234
        fsne (a2)
        fsne (a2)+
        fsne -(a2)
        fsne ($1234,a2)
        fsne (18,a2,d3.w)
        fsne ($001234).w
        fsne ($123456).l
        fst d2
        fdbt d2, *+$1234
        fst (a2)
        fst (a2)+
        fst -(a2)
        fst ($1234,a2)
        fst (18,a2,d3.w)
        fst ($001234).w
        fst ($123456).l
        fssf d2
        fdbsf d2, *+$1234
        fssf (a2)
        fssf (a2)+
        fssf -(a2)
        fssf ($1234,a2)
        fssf (18,a2,d3.w)
        fssf ($001234).w
        fssf ($123456).l
        fsseq d2
        fdbseq d2, *+$1234
        fsseq (a2)
        fsseq (a2)+
        fsseq -(a2)
        fsseq ($1234,a2)
        fsseq (18,a2,d3.w)
        fsseq ($001234).w
        fsseq ($123456).l
        fsgt d2
        fdbgt d2, *+$1234
        fsgt (a2)
        fsgt (a2)+
        fsgt -(a2)
        fsgt ($1234,a2)
        fsgt (18,a2,d3.w)
        fsgt ($001234).w
        fsgt ($123456).l
        fsge d2
        fdbge d2, *+$1234
        fsge (a2)
        fsge (a2)+
        fsge -(a2)
        fsge ($1234,a2)
        fsge (18,a2,d3.w)
        fsge ($001234).w
        fsge ($123456).l
        fslt d2
        fdblt d2, *+$1234
        fslt (a2)
        fslt (a2)+
        fslt -(a2)
        fslt ($1234,a2)
        fslt (18,a2,d3.w)
        fslt ($001234).w
        fslt ($123456).l
        fsle d2
        fdble d2, *+$1234
        fsle (a2)
        fsle (a2)+
        fsle -(a2)
        fsle ($1234,a2)
        fsle (18,a2,d3.w)
        fsle ($001234).w
        fsle ($123456).l
        fsgl d2
        fdbgl d2, *+$1234
        fsgl (a2)
        fsgl (a2)+
        fsgl -(a2)
        fsgl ($1234,a2)
        fsgl (18,a2,d3.w)
        fsgl ($001234).w
        fsgl ($123456).l
        fsgle d2
        fdbgle d2, *+$1234
        fsgle (a2)
        fsgle (a2)+
        fsgle -(a2)
        fsgle ($1234,a2)
        fsgle (18,a2,d3.w)
        fsgle ($001234).w
        fsgle ($123456).l
        fsngle d2
        fdbngle d2, *+$1234
        fsngle (a2)
        fsngle (a2)+
        fsngle -(a2)
        fsngle ($1234,a2)
        fsngle (18,a2,d3.w)
        fsngle ($001234).w
        fsngle ($123456).l
        fsngl d2
        fdbngl d2, *+$1234
        fsngl (a2)
        fsngl (a2)+
        fsngl -(a2)
        fsngl ($1234,a2)
        fsngl (18,a2,d3.w)
        fsngl ($001234).w
        fsngl ($123456).l
        fsnle d2
        fdbnle d2, *+$1234
        fsnle (a2)
        fsnle (a2)+
        fsnle -(a2)
        fsnle ($1234,a2)
        fsnle (18,a2,d3.w)
        fsnle ($001234).w
        fsnle ($123456).l
        fsnlt d2
        fdbnlt d2, *+$1234
        fsnlt (a2)
        fsnlt (a2)+
        fsnlt -(a2)
        fsnlt ($1234,a2)
        fsnlt (18,a2,d3.w)
        fsnlt ($001234).w
        fsnlt ($123456).l
        fsnge d2
        fdbnge d2, *+$1234
        fsnge (a2)
        fsnge (a2)+
        fsnge -(a2)
        fsnge ($1234,a2)
        fsnge (18,a2,d3.w)
        fsnge ($001234).w
        fsnge ($123456).l
        fsngt d2
        fdbngt d2, *+$1234
        fsngt (a2)
        fsngt (a2)+
        fsngt -(a2)
        fsngt ($1234,a2)
        fsngt (18,a2,d3.w)
        fsngt ($001234).w
        fsngt ($123456).l
        fssne d2
        fdbsne d2, *+$1234
        fssne (a2)
        fssne (a2)+
        fssne -(a2)
        fssne ($1234,a2)
        fssne (18,a2,d3.w)
        fssne ($001234).w
        fssne ($123456).l
        fsst d2
        fdbst d2, *+$1234
        fsst (a2)
        fsst (a2)+
        fsst -(a2)
        fsst ($1234,a2)
        fsst (18,a2,d3.w)
        fsst ($001234).w
        fsst ($123456).l

;;; system control operations
        frestore (a6)
        frestore (a0)+
;        frestore -(a0)
        frestore ($1234,a4)
        frestore ($23,a6,d7.w)
        frestore ($004566).w
        frestore ($56789a).l
;        frestore (*+$1234,pc)
;        frestore (*+90,pc,a4.l)
        fsave (a6)
;        fsave (a0)+
        fsave -(a0)
        fsave ($1234,a4)
        fsave ($23,a6,d7.w)
        fsave ($004566).w
        fsave ($56789a).l
        ftrapf.w #$1234
        ftrapf.l #$12345678
        ftrapf
        ftrapeq.w #$1234
        ftrapeq.l #$12345678
        ftrapeq
        ftrapogt.w #$1234
        ftrapogt.l #$12345678
        ftrapogt
        ftrapoge.w #$1234
        ftrapoge.l #$12345678
        ftrapoge
        ftrapolt.w #$1234
        ftrapolt.l #$12345678
        ftrapolt
        ftrapole.w #$1234
        ftrapole.l #$12345678
        ftrapole
        ftrapogl.w #$1234
        ftrapogl.l #$12345678
        ftrapogl
        ftrapor.w #$1234
        ftrapor.l #$12345678
        ftrapor
        ftrapun.w #$1234
        ftrapun.l #$12345678
        ftrapun
        ftrapueq.w #$1234
        ftrapueq.l #$12345678
        ftrapueq
        ftrapugt.w #$1234
        ftrapugt.l #$12345678
        ftrapugt
        ftrapuge.w #$1234
        ftrapuge.l #$12345678
        ftrapuge
        ftrapult.w #$1234
        ftrapult.l #$12345678
        ftrapult
        ftrapule.w #$1234
        ftrapule.l #$12345678
        ftrapule
        ftrapne.w #$1234
        ftrapne.l #$12345678
        ftrapne
        ftrapt.w #$1234
        ftrapt.l #$12345678
        ftrapt
        ftrapsf.w #$1234
        ftrapsf.l #$12345678
        ftrapsf
        ftrapseq.w #$1234
        ftrapseq.l #$12345678
        ftrapseq
        ftrapgt.w #$1234
        ftrapgt.l #$12345678
        ftrapgt
        ftrapge.w #$1234
        ftrapge.l #$12345678
        ftrapge
        ftraplt.w #$1234
        ftraplt.l #$12345678
        ftraplt
        ftraple.w #$1234
        ftraple.l #$12345678
        ftraple
        ftrapgl.w #$1234
        ftrapgl.l #$12345678
        ftrapgl
        ftrapgle.w #$1234
        ftrapgle.l #$12345678
        ftrapgle
        ftrapngle.w #$1234
        ftrapngle.l #$12345678
        ftrapngle
        ftrapngl.w #$1234
        ftrapngl.l #$12345678
        ftrapngl
        ftrapnle.w #$1234
        ftrapnle.l #$12345678
        ftrapnle
        ftrapnlt.w #$1234
        ftrapnlt.l #$12345678
        ftrapnlt
        ftrapnge.w #$1234
        ftrapnge.l #$12345678
        ftrapnge
        ftrapngt.w #$1234
        ftrapngt.l #$12345678
        ftrapngt
        ftrapsne.w #$1234
        ftrapsne.l #$12345678
        ftrapsne
        ftrapst.w #$1234
        ftrapst.l #$12345678
        ftrapst

;### MC68040 FPU
        fsmove.x fp0, fp1
        fsmove.l d2, fp3
        fsmove.x (a6), fp7
        fsmove.p (a0)+, fp1
        fsmove.w -(a2), fp3
        fsmove.d ($1234,a4), fp5
        fsmove.b ($23,a6,d7.w), fp0
        fsmove.l ($004566).w, fp1
        fsmove.s ($56789a).l, fp2
        fsmove.x (*+$1234,pc), fp3
        fsmove.p (*+90,pc,a4.l), fp5
        fsmove.l #$6789abcd, fp6
        fsmove.s #7.88999976e-10, fp7
        fsmove.x #-89000000032, fp0
        fsmove.p #9.12e20, fp1
        fsmove.w #$1234, fp2
        fsmove.d #-8.25, fp3
        fsmove.b #$23, fp4
        fdmove.x fp0, fp1
        fdmove.l d2, fp3
        fdmove.x (a6), fp7
        fdmove.p (a0)+, fp1
        fdmove.w -(a2), fp3
        fdmove.d ($1234,a4), fp5
        fdmove.b ($23,a6,d7.w), fp0
        fdmove.l ($004566).w, fp1
        fdmove.s ($56789a).l, fp2
        fdmove.x (*+$1234,pc), fp3
        fdmove.p (*+90,pc,a4.l), fp5
        fdmove.l #$6789abcd, fp6
        fdmove.s #7.88999976e-10, fp7
        fdmove.x #-89000000032, fp0
        fdmove.p #9.12e20, fp1
        fdmove.w #$1234, fp2
        fdmove.d #-8.25, fp3
        fdmove.b #$23, fp4
        fsdiv.x fp0, fp1
        fsdiv.l d2, fp3
        fsdiv.x (a6), fp7
        fsdiv.p (a0)+, fp1
        fsdiv.w -(a2), fp3
        fsdiv.d ($1234,a4), fp5
        fsdiv.b ($23,a6,d7.w), fp0
        fsdiv.l ($004566).w, fp1
        fsdiv.s ($56789a).l, fp2
        fsdiv.x (*+$1234,pc), fp3
        fsdiv.p (*+90,pc,a4.l), fp5
        fsdiv.l #$6789abcd, fp6
        fsdiv.s #7.88999976e-10, fp7
        fsdiv.x #-89000000032, fp0
        fsdiv.p #9.12e20, fp1
        fsdiv.w #$1234, fp2
        fsdiv.d #-8.25, fp3
        fsdiv.b #$23, fp4
        fddiv.x fp0, fp1
        fddiv.l d2, fp3
        fddiv.x (a6), fp7
        fddiv.p (a0)+, fp1
        fddiv.w -(a2), fp3
        fddiv.d ($1234,a4), fp5
        fddiv.b ($23,a6,d7.w), fp0
        fddiv.l ($004566).w, fp1
        fddiv.s ($56789a).l, fp2
        fddiv.x (*+$1234,pc), fp3
        fddiv.p (*+90,pc,a4.l), fp5
        fddiv.l #$6789abcd, fp6
        fddiv.s #7.88999976e-10, fp7
        fddiv.x #-89000000032, fp0
        fddiv.p #9.12e20, fp1
        fddiv.w #$1234, fp2
        fddiv.d #-8.25, fp3
        fddiv.b #$23, fp4
        fsadd.x fp0, fp1
        fsadd.l d2, fp3
        fsadd.x (a6), fp7
        fsadd.p (a0)+, fp1
        fsadd.w -(a2), fp3
        fsadd.d ($1234,a4), fp5
        fsadd.b ($23,a6,d7.w), fp0
        fsadd.l ($004566).w, fp1
        fsadd.s ($56789a).l, fp2
        fsadd.x (*+$1234,pc), fp3
        fsadd.p (*+90,pc,a4.l), fp5
        fsadd.l #$6789abcd, fp6
        fsadd.s #7.88999976e-10, fp7
        fsadd.x #-89000000032, fp0
        fsadd.p #9.12e20, fp1
        fsadd.w #$1234, fp2
        fsadd.d #-8.25, fp3
        fsadd.b #$23, fp4
        fdadd.x fp0, fp1
        fdadd.l d2, fp3
        fdadd.x (a6), fp7
        fdadd.p (a0)+, fp1
        fdadd.w -(a2), fp3
        fdadd.d ($1234,a4), fp5
        fdadd.b ($23,a6,d7.w), fp0
        fdadd.l ($004566).w, fp1
        fdadd.s ($56789a).l, fp2
        fdadd.x (*+$1234,pc), fp3
        fdadd.p (*+90,pc,a4.l), fp5
        fdadd.l #$6789abcd, fp6
        fdadd.s #7.88999976e-10, fp7
        fdadd.x #-89000000032, fp0
        fdadd.p #9.12e20, fp1
        fdadd.w #$1234, fp2
        fdadd.d #-8.25, fp3
        fdadd.b #$23, fp4
        fsmul.x fp0, fp1
        fsmul.l d2, fp3
        fsmul.x (a6), fp7
        fsmul.p (a0)+, fp1
        fsmul.w -(a2), fp3
        fsmul.d ($1234,a4), fp5
        fsmul.b ($23,a6,d7.w), fp0
        fsmul.l ($004566).w, fp1
        fsmul.s ($56789a).l, fp2
        fsmul.x (*+$1234,pc), fp3
        fsmul.p (*+90,pc,a4.l), fp5
        fsmul.l #$6789abcd, fp6
        fsmul.s #7.88999976e-10, fp7
        fsmul.x #-89000000032, fp0
        fsmul.p #9.12e20, fp1
        fsmul.w #$1234, fp2
        fsmul.d #-8.25, fp3
        fsmul.b #$23, fp4
        fdmul.x fp0, fp1
        fdmul.l d2, fp3
        fdmul.x (a6), fp7
        fdmul.p (a0)+, fp1
        fdmul.w -(a2), fp3
        fdmul.d ($1234,a4), fp5
        fdmul.b ($23,a6,d7.w), fp0
        fdmul.l ($004566).w, fp1
        fdmul.s ($56789a).l, fp2
        fdmul.x (*+$1234,pc), fp3
        fdmul.p (*+90,pc,a4.l), fp5
        fdmul.l #$6789abcd, fp6
        fdmul.s #7.88999976e-10, fp7
        fdmul.x #-89000000032, fp0
        fdmul.p #9.12e20, fp1
        fdmul.w #$1234, fp2
        fdmul.d #-8.25, fp3
        fdmul.b #$23, fp4
        fssub.x fp0, fp1
        fssub.l d2, fp3
        fssub.x (a6), fp7
        fssub.p (a0)+, fp1
        fssub.w -(a2), fp3
        fssub.d ($1234,a4), fp5
        fssub.b ($23,a6,d7.w), fp0
        fssub.l ($004566).w, fp1
        fssub.s ($56789a).l, fp2
        fssub.x (*+$1234,pc), fp3
        fssub.p (*+90,pc,a4.l), fp5
        fssub.l #$6789abcd, fp6
        fssub.s #7.88999976e-10, fp7
        fssub.x #-89000000032, fp0
        fssub.p #9.12e20, fp1
        fssub.w #$1234, fp2
        fssub.d #-8.25, fp3
        fssub.b #$23, fp4
        fdsub.x fp0, fp1
        fdsub.l d2, fp3
        fdsub.x (a6), fp7
        fdsub.p (a0)+, fp1
        fdsub.w -(a2), fp3
        fdsub.d ($1234,a4), fp5
        fdsub.b ($23,a6,d7.w), fp0
        fdsub.l ($004566).w, fp1
        fdsub.s ($56789a).l, fp2
        fdsub.x (*+$1234,pc), fp3
        fdsub.p (*+90,pc,a4.l), fp5
        fdsub.l #$6789abcd, fp6
        fdsub.s #7.88999976e-10, fp7
        fdsub.x #-89000000032, fp0
        fdsub.p #9.12e20, fp1
        fdsub.w #$1234, fp2
        fdsub.d #-8.25, fp3
        fdsub.b #$23, fp4
        fssqrt.x fp0, fp1
        fssqrt.x fp2
        fssqrt.l d2, fp3
        fssqrt.x (a6), fp7
        fssqrt.p (a0)+, fp1
        fssqrt.w -(a2), fp3
        fssqrt.d ($1234,a4), fp5
        fssqrt.b ($23,a6,d7.w), fp0
        fssqrt.l ($004566).w, fp1
        fssqrt.s ($56789a).l, fp2
        fssqrt.x (*+$1234,pc), fp3
        fssqrt.p (*+90,pc,a4.l), fp5
        fssqrt.l #$6789abcd, fp6
        fssqrt.s #7.88999976e-10, fp7
        fssqrt.x #-89000000032, fp0
        fssqrt.p #9.12e20, fp1
        fssqrt.w #$1234, fp2
        fssqrt.d #-8.25, fp3
        fssqrt.b #$23, fp4
        fdsqrt.x fp0, fp1
        fdsqrt.x fp2
        fdsqrt.l d2, fp3
        fdsqrt.x (a6), fp7
        fdsqrt.p (a0)+, fp1
        fdsqrt.w -(a2), fp3
        fdsqrt.d ($1234,a4), fp5
        fdsqrt.b ($23,a6,d7.w), fp0
        fdsqrt.l ($004566).w, fp1
        fdsqrt.s ($56789a).l, fp2
        fdsqrt.x (*+$1234,pc), fp3
        fdsqrt.p (*+90,pc,a4.l), fp5
        fdsqrt.l #$6789abcd, fp6
        fdsqrt.s #7.88999976e-10, fp7
        fdsqrt.x #-89000000032, fp0
        fdsqrt.p #9.12e20, fp1
        fdsqrt.w #$1234, fp2
        fdsqrt.d #-8.25, fp3
        fdsqrt.b #$23, fp4
        fsabs.x fp0, fp1
        fsabs.x fp2
        fsabs.l d2, fp3
        fsabs.x (a6), fp7
        fsabs.p (a0)+, fp1
        fsabs.w -(a2), fp3
        fsabs.d ($1234,a4), fp5
        fsabs.b ($23,a6,d7.w), fp0
        fsabs.l ($004566).w, fp1
        fsabs.s ($56789a).l, fp2
        fsabs.x (*+$1234,pc), fp3
        fsabs.p (*+90,pc,a4.l), fp5
        fsabs.l #$6789abcd, fp6
        fsabs.s #7.88999976e-10, fp7
        fsabs.x #-89000000032, fp0
        fsabs.p #9.12e20, fp1
        fsabs.w #$1234, fp2
        fsabs.d #-8.25, fp3
        fsabs.b #$23, fp4
        fdabs.x fp0, fp1
        fdabs.x fp2
        fdabs.l d2, fp3
        fdabs.x (a6), fp7
        fdabs.p (a0)+, fp1
        fdabs.w -(a2), fp3
        fdabs.d ($1234,a4), fp5
        fdabs.b ($23,a6,d7.w), fp0
        fdabs.l ($004566).w, fp1
        fdabs.s ($56789a).l, fp2
        fdabs.x (*+$1234,pc), fp3
        fdabs.p (*+90,pc,a4.l), fp5
        fdabs.l #$6789abcd, fp6
        fdabs.s #7.88999976e-10, fp7
        fdabs.x #-89000000032, fp0
        fdabs.p #9.12e20, fp1
        fdabs.w #$1234, fp2
        fdabs.d #-8.25, fp3
        fdabs.b #$23, fp4
        fsneg.x fp0, fp1
        fsneg.x fp2
        fsneg.l d2, fp3
        fsneg.x (a6), fp7
        fsneg.p (a0)+, fp1
        fsneg.w -(a2), fp3
        fsneg.d ($1234,a4), fp5
        fsneg.b ($23,a6,d7.w), fp0
        fsneg.l ($004566).w, fp1
        fsneg.s ($56789a).l, fp2
        fsneg.x (*+$1234,pc), fp3
        fsneg.p (*+90,pc,a4.l), fp5
        fsneg.l #$6789abcd, fp6
        fsneg.s #7.88999976e-10, fp7
        fsneg.x #-89000000032, fp0
        fsneg.p #9.12e20, fp1
        fsneg.w #$1234, fp2
        fsneg.d #-8.25, fp3
        fsneg.b #$23, fp4
        fdneg.x fp0, fp1
        fdneg.x fp2
        fdneg.l d2, fp3
        fdneg.x (a6), fp7
        fdneg.p (a0)+, fp1
        fdneg.w -(a2), fp3
        fdneg.d ($1234,a4), fp5
        fdneg.b ($23,a6,d7.w), fp0
        fdneg.l ($004566).w, fp1
        fdneg.s ($56789a).l, fp2
        fdneg.x (*+$1234,pc), fp3
        fdneg.p (*+90,pc,a4.l), fp5
        fdneg.l #$6789abcd, fp6
        fdneg.s #7.88999976e-10, fp7
        fdneg.x #-89000000032, fp0
        fdneg.p #9.12e20, fp1
        fdneg.w #$1234, fp2
        fdneg.d #-8.25, fp3
        fdneg.b #$23, fp4

;# MC68040 PMMU
        pflushn (a2)
        pflush  (a2)
        pflushan
        pflusha
        ptestr  (a2)
        ptestw  (a2)

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
