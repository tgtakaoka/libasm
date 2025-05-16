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

;;; Data Movement Operations
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
        fmovecr.x #$00, fp0
        fmovecr.x #$10, fp2
        fmovecr.x #$20, fp4
        fmovecr.x #$3f, fp7

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
        fmod.x fp0, fp1
        fmod.l d2, fp3
        fmod.x (a6), fp7
        fmod.p (a0)+, fp1
        fmod.w -(a2), fp3
        fmod.d ($1234,a4), fp5
        fmod.b ($23,a6,d7.w), fp0
        fmod.l ($004566).w, fp1
        fmod.s ($56789a).l, fp2
        fmod.x (*+$1234,pc), fp3
        fmod.p (*+90,pc,a4.l), fp5
        fmod.l #$6789abcd, fp6
        fmod.s #7.88999976e-10, fp7
        fmod.x #-89000000032, fp0
        fmod.p #9.12e20, fp1
        fmod.w #$1234, fp2
        fmod.d #-8.25, fp3
        fmod.b #$23, fp4
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
        frem.x fp0, fp1
        frem.l d2, fp3
        frem.x (a6), fp7
        frem.p (a0)+, fp1
        frem.w -(a2), fp3
        frem.d ($1234,a4), fp5
        frem.b ($23,a6,d7.w), fp0
        frem.l ($004566).w, fp1
        frem.s ($56789a).l, fp2
        frem.x (*+$1234,pc), fp3
        frem.p (*+90,pc,a4.l), fp5
        frem.l #$6789abcd, fp6
        frem.s #7.88999976e-10, fp7
        frem.x #-89000000032, fp0
        frem.p #9.12e20, fp1
        frem.w #$1234, fp2
        frem.d #-8.25, fp3
        frem.b #$23, fp4
        fscale.x fp0, fp1
        fscale.l d2, fp3
        fscale.x (a6), fp7
        fscale.p (a0)+, fp1
        fscale.w -(a2), fp3
        fscale.d ($1234,a4), fp5
        fscale.b ($23,a6,d7.w), fp0
        fscale.l ($004566).w, fp1
        fscale.s ($56789a).l, fp2
        fscale.x (*+$1234,pc), fp3
        fscale.p (*+90,pc,a4.l), fp5
        fscale.l #$6789abcd, fp6
        fscale.s #7.88999976e-10, fp7
        fscale.x #-89000000032, fp0
        fscale.p #9.12e20, fp1
        fscale.w #$1234, fp2
        fscale.d #-8.25, fp3
        fscale.b #$23, fp4
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
        fsincos.x fp0, fp2:fp1
        fsincos.l d2, fp1:fp3
        fsincos.x (a6), fp0:fp7
        fsincos.p (a0)+, fp7:fp1
        fsincos.w -(a2), fp6:fp3
        fsincos.d ($1234,a4), fp5:fp5
        fsincos.b ($23,a6,d7.w), fp4:fp0
        fsincos.l ($004566).w, fp3:fp1
        fsincos.s ($56789a).l, fp1:fp2
        fsincos.x (*+$1234,pc), fp0:fp3
        fsincos.p (*+90,pc,a4.l), fp7:fp5
        fsincos.l #$6789abcd, fp5:fp6
        fsincos.s #7.88999976e-10, fp6:fp7
        fsincos.x #-89000000032, fp4:fp0
        fsincos.p #9.12e20, fp3:fp1
        fsincos.w #$1234, fp1:fp2
        fsincos.d #-8.25, fp0:fp3
        fsincos.b #$23, fp7:fp4
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
        fint.x fp0, fp1
        fint.x fp2
        fint.l d2, fp3
        fint.x (a6), fp7
        fint.p (a0)+, fp1
        fint.w -(a2), fp3
        fint.d ($1234,a4), fp5
        fint.b ($23,a6,d7.w), fp0
        fint.l ($004566).w, fp1
        fint.s ($56789a).l, fp2
        fint.x (*+$1234,pc), fp3
        fint.p (*+90,pc,a4.l), fp5
        fint.l #$6789abcd, fp6
        fint.s #7.88999976e-10, fp7
        fint.x #-89000000032, fp0
        fint.p #9.12e20, fp1
        fint.w #$1234, fp2
        fint.d #-8.25, fp3
        fint.b #$23, fp4
        fsinh.x fp0, fp1
        fsinh.x fp2
        fsinh.l d2, fp3
        fsinh.x (a6), fp7
        fsinh.p (a0)+, fp1
        fsinh.w -(a2), fp3
        fsinh.d ($1234,a4), fp5
        fsinh.b ($23,a6,d7.w), fp0
        fsinh.l ($004566).w, fp1
        fsinh.s ($56789a).l, fp2
        fsinh.x (*+$1234,pc), fp3
        fsinh.p (*+90,pc,a4.l), fp5
        fsinh.l #$6789abcd, fp6
        fsinh.s #7.88999976e-10, fp7
        fsinh.x #-89000000032, fp0
        fsinh.p #9.12e20, fp1
        fsinh.w #$1234, fp2
        fsinh.d #-8.25, fp3
        fsinh.b #$23, fp4
        fintrz.x fp0, fp1
        fintrz.x fp2
        fintrz.l d2, fp3
        fintrz.x (a6), fp7
        fintrz.p (a0)+, fp1
        fintrz.w -(a2), fp3
        fintrz.d ($1234,a4), fp5
        fintrz.b ($23,a6,d7.w), fp0
        fintrz.l ($004566).w, fp1
        fintrz.s ($56789a).l, fp2
        fintrz.x (*+$1234,pc), fp3
        fintrz.p (*+90,pc,a4.l), fp5
        fintrz.l #$6789abcd, fp6
        fintrz.s #7.88999976e-10, fp7
        fintrz.x #-89000000032, fp0
        fintrz.p #9.12e20, fp1
        fintrz.w #$1234, fp2
        fintrz.d #-8.25, fp3
        fintrz.b #$23, fp4
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
        flognp1.x fp0, fp1
        flognp1.x fp2
        flognp1.l d2, fp3
        flognp1.x (a6), fp7
        flognp1.p (a0)+, fp1
        flognp1.w -(a2), fp3
        flognp1.d ($1234,a4), fp5
        flognp1.b ($23,a6,d7.w), fp0
        flognp1.l ($004566).w, fp1
        flognp1.s ($56789a).l, fp2
        flognp1.x (*+$1234,pc), fp3
        flognp1.p (*+90,pc,a4.l), fp5
        flognp1.l #$6789abcd, fp6
        flognp1.s #7.88999976e-10, fp7
        flognp1.x #-89000000032, fp0
        flognp1.p #9.12e20, fp1
        flognp1.w #$1234, fp2
        flognp1.d #-8.25, fp3
        flognp1.b #$23, fp4
        fetoxm1.x fp0, fp1
        fetoxm1.x fp2
        fetoxm1.l d2, fp3
        fetoxm1.x (a6), fp7
        fetoxm1.p (a0)+, fp1
        fetoxm1.w -(a2), fp3
        fetoxm1.d ($1234,a4), fp5
        fetoxm1.b ($23,a6,d7.w), fp0
        fetoxm1.l ($004566).w, fp1
        fetoxm1.s ($56789a).l, fp2
        fetoxm1.x (*+$1234,pc), fp3
        fetoxm1.p (*+90,pc,a4.l), fp5
        fetoxm1.l #$6789abcd, fp6
        fetoxm1.s #7.88999976e-10, fp7
        fetoxm1.x #-89000000032, fp0
        fetoxm1.p #9.12e20, fp1
        fetoxm1.w #$1234, fp2
        fetoxm1.d #-8.25, fp3
        fetoxm1.b #$23, fp4
        ftanh.x fp0, fp1
        ftanh.x fp2
        ftanh.l d2, fp3
        ftanh.x (a6), fp7
        ftanh.p (a0)+, fp1
        ftanh.w -(a2), fp3
        ftanh.d ($1234,a4), fp5
        ftanh.b ($23,a6,d7.w), fp0
        ftanh.l ($004566).w, fp1
        ftanh.s ($56789a).l, fp2
        ftanh.x (*+$1234,pc), fp3
        ftanh.p (*+90,pc,a4.l), fp5
        ftanh.l #$6789abcd, fp6
        ftanh.s #7.88999976e-10, fp7
        ftanh.x #-89000000032, fp0
        ftanh.p #9.12e20, fp1
        ftanh.w #$1234, fp2
        ftanh.d #-8.25, fp3
        ftanh.b #$23, fp4
        fatan.x fp0, fp1
        fatan.x fp2
        fatan.l d2, fp3
        fatan.x (a6), fp7
        fatan.p (a0)+, fp1
        fatan.w -(a2), fp3
        fatan.d ($1234,a4), fp5
        fatan.b ($23,a6,d7.w), fp0
        fatan.l ($004566).w, fp1
        fatan.s ($56789a).l, fp2
        fatan.x (*+$1234,pc), fp3
        fatan.p (*+90,pc,a4.l), fp5
        fatan.l #$6789abcd, fp6
        fatan.s #7.88999976e-10, fp7
        fatan.x #-89000000032, fp0
        fatan.p #9.12e20, fp1
        fatan.w #$1234, fp2
        fatan.d #-8.25, fp3
        fatan.b #$23, fp4
        fasin.x fp0, fp1
        fasin.x fp2
        fasin.l d2, fp3
        fasin.x (a6), fp7
        fasin.p (a0)+, fp1
        fasin.w -(a2), fp3
        fasin.d ($1234,a4), fp5
        fasin.b ($23,a6,d7.w), fp0
        fasin.l ($004566).w, fp1
        fasin.s ($56789a).l, fp2
        fasin.x (*+$1234,pc), fp3
        fasin.p (*+90,pc,a4.l), fp5
        fasin.l #$6789abcd, fp6
        fasin.s #7.88999976e-10, fp7
        fasin.x #-89000000032, fp0
        fasin.p #9.12e20, fp1
        fasin.w #$1234, fp2
        fasin.d #-8.25, fp3
        fasin.b #$23, fp4
        fatanh.x fp0, fp1
        fatanh.x fp2
        fatanh.l d2, fp3
        fatanh.x (a6), fp7
        fatanh.p (a0)+, fp1
        fatanh.w -(a2), fp3
        fatanh.d ($1234,a4), fp5
        fatanh.b ($23,a6,d7.w), fp0
        fatanh.l ($004566).w, fp1
        fatanh.s ($56789a).l, fp2
        fatanh.x (*+$1234,pc), fp3
        fatanh.p (*+90,pc,a4.l), fp5
        fatanh.l #$6789abcd, fp6
        fatanh.s #7.88999976e-10, fp7
        fatanh.x #-89000000032, fp0
        fatanh.p #9.12e20, fp1
        fatanh.w #$1234, fp2
        fatanh.d #-8.25, fp3
        fatanh.b #$23, fp4
        fsin.x fp0, fp1
        fsin.x fp2
        fsin.l d2, fp3
        fsin.x (a6), fp7
        fsin.p (a0)+, fp1
        fsin.w -(a2), fp3
        fsin.d ($1234,a4), fp5
        fsin.b ($23,a6,d7.w), fp0
        fsin.l ($004566).w, fp1
        fsin.s ($56789a).l, fp2
        fsin.x (*+$1234,pc), fp3
        fsin.p (*+90,pc,a4.l), fp5
        fsin.l #$6789abcd, fp6
        fsin.s #7.88999976e-10, fp7
        fsin.x #-89000000032, fp0
        fsin.p #9.12e20, fp1
        fsin.w #$1234, fp2
        fsin.d #-8.25, fp3
        fsin.b #$23, fp4
        ftan.x fp0, fp1
        ftan.x fp2
        ftan.l d2, fp3
        ftan.x (a6), fp7
        ftan.p (a0)+, fp1
        ftan.w -(a2), fp3
        ftan.d ($1234,a4), fp5
        ftan.b ($23,a6,d7.w), fp0
        ftan.l ($004566).w, fp1
        ftan.s ($56789a).l, fp2
        ftan.x (*+$1234,pc), fp3
        ftan.p (*+90,pc,a4.l), fp5
        ftan.l #$6789abcd, fp6
        ftan.s #7.88999976e-10, fp7
        ftan.x #-89000000032, fp0
        ftan.p #9.12e20, fp1
        ftan.w #$1234, fp2
        ftan.d #-8.25, fp3
        ftan.b #$23, fp4
        fetox.x fp0, fp1
        fetox.x fp2
        fetox.l d2, fp3
        fetox.x (a6), fp7
        fetox.p (a0)+, fp1
        fetox.w -(a2), fp3
        fetox.d ($1234,a4), fp5
        fetox.b ($23,a6,d7.w), fp0
        fetox.l ($004566).w, fp1
        fetox.s ($56789a).l, fp2
        fetox.x (*+$1234,pc), fp3
        fetox.p (*+90,pc,a4.l), fp5
        fetox.l #$6789abcd, fp6
        fetox.s #7.88999976e-10, fp7
        fetox.x #-89000000032, fp0
        fetox.p #9.12e20, fp1
        fetox.w #$1234, fp2
        fetox.d #-8.25, fp3
        fetox.b #$23, fp4
        ftwotox.x fp0, fp1
        ftwotox.x fp2
        ftwotox.l d2, fp3
        ftwotox.x (a6), fp7
        ftwotox.p (a0)+, fp1
        ftwotox.w -(a2), fp3
        ftwotox.d ($1234,a4), fp5
        ftwotox.b ($23,a6,d7.w), fp0
        ftwotox.l ($004566).w, fp1
        ftwotox.s ($56789a).l, fp2
        ftwotox.x (*+$1234,pc), fp3
        ftwotox.p (*+90,pc,a4.l), fp5
        ftwotox.l #$6789abcd, fp6
        ftwotox.s #7.88999976e-10, fp7
        ftwotox.x #-89000000032, fp0
        ftwotox.p #9.12e20, fp1
        ftwotox.w #$1234, fp2
        ftwotox.d #-8.25, fp3
        ftwotox.b #$23, fp4
        ftentox.x fp0, fp1
        ftentox.x fp2
        ftentox.l d2, fp3
        ftentox.x (a6), fp7
        ftentox.p (a0)+, fp1
        ftentox.w -(a2), fp3
        ftentox.d ($1234,a4), fp5
        ftentox.b ($23,a6,d7.w), fp0
        ftentox.l ($004566).w, fp1
        ftentox.s ($56789a).l, fp2
        ftentox.x (*+$1234,pc), fp3
        ftentox.p (*+90,pc,a4.l), fp5
        ftentox.l #$6789abcd, fp6
        ftentox.s #7.88999976e-10, fp7
        ftentox.x #-89000000032, fp0
        ftentox.p #9.12e20, fp1
        ftentox.w #$1234, fp2
        ftentox.d #-8.25, fp3
        ftentox.b #$23, fp4
        flogn.x fp0, fp1
        flogn.x fp2
        flogn.l d2, fp3
        flogn.x (a6), fp7
        flogn.p (a0)+, fp1
        flogn.w -(a2), fp3
        flogn.d ($1234,a4), fp5
        flogn.b ($23,a6,d7.w), fp0
        flogn.l ($004566).w, fp1
        flogn.s ($56789a).l, fp2
        flogn.x (*+$1234,pc), fp3
        flogn.p (*+90,pc,a4.l), fp5
        flogn.l #$6789abcd, fp6
        flogn.s #7.88999976e-10, fp7
        flogn.x #-89000000032, fp0
        flogn.p #9.12e20, fp1
        flogn.w #$1234, fp2
        flogn.d #-8.25, fp3
        flogn.b #$23, fp4
        flog10.x fp0, fp1
        flog10.x fp2
        flog10.l d2, fp3
        flog10.x (a6), fp7
        flog10.p (a0)+, fp1
        flog10.w -(a2), fp3
        flog10.d ($1234,a4), fp5
        flog10.b ($23,a6,d7.w), fp0
        flog10.l ($004566).w, fp1
        flog10.s ($56789a).l, fp2
        flog10.x (*+$1234,pc), fp3
        flog10.p (*+90,pc,a4.l), fp5
        flog10.l #$6789abcd, fp6
        flog10.s #7.88999976e-10, fp7
        flog10.x #-89000000032, fp0
        flog10.p #9.12e20, fp1
        flog10.w #$1234, fp2
        flog10.d #-8.25, fp3
        flog10.b #$23, fp4
        flog2.x fp0, fp1
        flog2.x fp2
        flog2.l d2, fp3
        flog2.x (a6), fp7
        flog2.p (a0)+, fp1
        flog2.w -(a2), fp3
        flog2.d ($1234,a4), fp5
        flog2.b ($23,a6,d7.w), fp0
        flog2.l ($004566).w, fp1
        flog2.s ($56789a).l, fp2
        flog2.x (*+$1234,pc), fp3
        flog2.p (*+90,pc,a4.l), fp5
        flog2.l #$6789abcd, fp6
        flog2.s #7.88999976e-10, fp7
        flog2.x #-89000000032, fp0
        flog2.p #9.12e20, fp1
        flog2.w #$1234, fp2
        flog2.d #-8.25, fp3
        flog2.b #$23, fp4
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
        fcosh.x fp0, fp1
        fcosh.x fp2
        fcosh.l d2, fp3
        fcosh.x (a6), fp7
        fcosh.p (a0)+, fp1
        fcosh.w -(a2), fp3
        fcosh.d ($1234,a4), fp5
        fcosh.b ($23,a6,d7.w), fp0
        fcosh.l ($004566).w, fp1
        fcosh.s ($56789a).l, fp2
        fcosh.x (*+$1234,pc), fp3
        fcosh.p (*+90,pc,a4.l), fp5
        fcosh.l #$6789abcd, fp6
        fcosh.s #7.88999976e-10, fp7
        fcosh.x #-89000000032, fp0
        fcosh.p #9.12e20, fp1
        fcosh.w #$1234, fp2
        fcosh.d #-8.25, fp3
        fcosh.b #$23, fp4
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
        facos.x fp0, fp1
        facos.x fp2
        facos.l d2, fp3
        facos.x (a6), fp7
        facos.p (a0)+, fp1
        facos.w -(a2), fp3
        facos.d ($1234,a4), fp5
        facos.b ($23,a6,d7.w), fp0
        facos.l ($004566).w, fp1
        facos.s ($56789a).l, fp2
        facos.x (*+$1234,pc), fp3
        facos.p (*+90,pc,a4.l), fp5
        facos.l #$6789abcd, fp6
        facos.s #7.88999976e-10, fp7
        facos.x #-89000000032, fp0
        facos.p #9.12e20, fp1
        facos.w #$1234, fp2
        facos.d #-8.25, fp3
        facos.b #$23, fp4
        fcos.x fp0, fp1
        fcos.x fp2
        fcos.l d2, fp3
        fcos.x (a6), fp7
        fcos.p (a0)+, fp1
        fcos.w -(a2), fp3
        fcos.d ($1234,a4), fp5
        fcos.b ($23,a6,d7.w), fp0
        fcos.l ($004566).w, fp1
        fcos.s ($56789a).l, fp2
        fcos.x (*+$1234,pc), fp3
        fcos.p (*+90,pc,a4.l), fp5
        fcos.l #$6789abcd, fp6
        fcos.s #7.88999976e-10, fp7
        fcos.x #-89000000032, fp0
        fcos.p #9.12e20, fp1
        fcos.w #$1234, fp2
        fcos.d #-8.25, fp3
        fcos.b #$23, fp4
        fgetexp.x fp0, fp1
        fgetexp.x fp2
        fgetexp.l d2, fp3
        fgetexp.x (a6), fp7
        fgetexp.p (a0)+, fp1
        fgetexp.w -(a2), fp3
        fgetexp.d ($1234,a4), fp5
        fgetexp.b ($23,a6,d7.w), fp0
        fgetexp.l ($004566).w, fp1
        fgetexp.s ($56789a).l, fp2
        fgetexp.x (*+$1234,pc), fp3
        fgetexp.p (*+90,pc,a4.l), fp5
        fgetexp.l #$6789abcd, fp6
        fgetexp.s #7.88999976e-10, fp7
        fgetexp.x #-89000000032, fp0
        fgetexp.p #9.12e20, fp1
        fgetexp.w #$1234, fp2
        fgetexp.d #-8.25, fp3
        fgetexp.b #$23, fp4
        fgetman.x fp0, fp1
        fgetman.x fp2
        fgetman.l d2, fp3
        fgetman.x (a6), fp7
        fgetman.p (a0)+, fp1
        fgetman.w -(a2), fp3
        fgetman.d ($1234,a4), fp5
        fgetman.b ($23,a6,d7.w), fp0
        fgetman.l ($004566).w, fp1
        fgetman.s ($56789a).l, fp2
        fgetman.x (*+$1234,pc), fp3
        fgetman.p (*+90,pc,a4.l), fp5
        fgetman.l #$6789abcd, fp6
        fgetman.s #7.88999976e-10, fp7
        fgetman.x #-89000000032, fp0
        fgetman.p #9.12e20, fp1
        fgetman.w #$1234, fp2
        fgetman.d #-8.25, fp3
        fgetman.b #$23, fp4

;;; program control operations
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

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
