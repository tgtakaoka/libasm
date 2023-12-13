;### Copyright 2023 Tadashi G. Takaoka
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

;;; Data Transfer
        fld
        fild
        fbld
        fst
        fist
        fstp
        fistp
        fbstp
        fxch

;;; Comparison
        ficom
        fcom
        ficomp
        fcomp
        fcompp
        ftst
        fexam

;;; Constants
        fldz
        fld1
        fldpi
        fldl2t
        fldl2e
        fldlg2
        fldln2

;;; Arithmetic
        fiadd
        fadd
        faddp
        fisub
        fsub
        fsubp
        fisubr
        fsubr
        fsubrp
        fimul
        fmul
        fmulp
        fidiv
        fdiv
        fdivp
        fidivr
        fdivr
        fdivrp
        fsqrt
        fscale
        fprem
        frndint
        fxtracvt
        fabs
        fchs

;;; Transcendental
        fptan
        fpatan
        f2xmi
        fyl2x
        fyl2xp1

;;; Co-processor control
        finit
        fninit
        feni
        fneni
        fdisi
        fndisi
        fldcw
        fstcw
        fnstcw
        fstsw
        fnstsw
        fclex
        fnclex
        fstenv
        fnstenv
        fldenv
        fsave
        fnsave
        frstor
        fincsto
        fdecstp
        ffree
        fnop
        fwait

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
