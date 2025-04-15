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
        fld     dword ptr [si]
        fld     qword ptr [1234H]
        fld     tbyte ptr [di+52]
        fld     st(0)
        fild    word ptr [bp+1234H]
        fild    dword ptr [bx+si]
        fild    qword ptr [bx+di-52]
        fbld    tbyte ptr [bp+si+89ABH]
        fst     dword ptr [bp+1234H]
        fst     qword ptr [bx+si]
        fst     st(1)
        fist    word ptr [bx+di-52]
        fist    dword ptr [bp+si+89ABH]
        fstp    dword ptr [bp+si+89ABH]
        fstp    qword ptr [si]
        fstp    tbyte ptr [1234H]
        fstp    st(2)
        fistp   word ptr [di+52]
        fistp   dword ptr [bp+1234H]
        fistp   qword ptr [bx+si]
        fbstp   tbyte ptr [bx+di-52]
        fxch    st(3)

;;; Comparison
        ficom   word ptr [si]
        ficom   dword ptr [1234H]
        fcom    dword ptr [si]
        fcom    qword ptr [1234H]
        fcom    st(1)
        ficomp  word ptr [di+52]
        ficomp  dword ptr [bp+1234H]
        fcomp   dword ptr [di+52]
        fcomp   qword ptr [bp+1234H]
        fcomp   st(3)
        fcompp
        ftst
        fxam

;;; Constants
        fldz
        fld1
        fldpi
        fldl2t
        fldl2e
        fldlg2
        fldln2

;;; Arithmetic
        fiadd   word ptr [si]
        fiadd   dword ptr [1234H]
        fadd    dword ptr [si]
        fadd    qword ptr [1234H]
        fadd    st, st(1)
        fadd    st(2), st
        faddp   st(3), st
        fisub   word ptr [bx+si]
        fisub   dword ptr [bx+di-52]
        fsub    dword ptr [bx+si]
        fsub    qword ptr [bx+di-52]
        fsub    st, st(5)
        fsub    st(6), st
        fsubp   st(7), st
        fisubr  word ptr [bp+si+89ABH]
        fisubr  dword ptr [si]
        fsubr   dword ptr [bp+si+89ABH]
        fsubr   qword ptr [si]
        fsubr   st, st(7)
        fsubr   st(1), st
        fsubrp  st(2), st
        fimul   word ptr [di+52]
        fimul   dword ptr [bp+1234H]
        fmul    dword ptr [di+52]
        fmul    qword ptr [bp+1234H]
        fmul    st, st(3)
        fmul    st(4), st
        fmulp   st(5), st
        fidiv   word ptr [1234H]
        fidiv   dword ptr [di+52]
        fdiv    dword ptr [1234H]
        fdiv    dword ptr [di+52]
        fdiv    st, st(2)
        fdiv    st(3), st
        fdivp   st(4), st
        fidivr  word ptr [bp+1234H]
        fidivr  dword ptr [bx+si]
        fdivr   dword ptr [bp+1234H]
        fdivr   qword ptr [bx+si]
        fdivr   st, st(4)
        fdivr   st(5), st
        fdivrp  st(6), st
        fsqrt
        fscale
        fprem
        frndint
        fxtract
        fabs
        fchs

;;; Transcendental
        fptan
        fpatan
        f2xm1
        fyl2x
        fyl2xp1

;;; Co-processor control
        finit
        fninit
        feni
        fneni
        fdisi
        fndisi
        fldcw   [si]
        fstcw   [1234H]
        fnstcw  [di+52]
        fstsw   [bp+1234H]
        fnstsw  [bx+si]
        fclex
        fnclex
        fstenv  [bx+di-52]
        fnstenv [bp+si+89ABH]
        fldenv  [di]
        fsave   [5678H]
        fnsave  [si+52]
        frstor  [bx+1234H]
        fincstp
        fdecstp
        ffree   st(2)
        fnop
        fwait

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
