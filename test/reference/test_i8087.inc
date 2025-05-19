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

;## Data Transfer
        fnld    dword ptr [si]
        fnld    qword ptr [0x1234]
        fnld    tbyte ptr [di+52]
        fnld    st(0)
        fnild   word ptr [bp+0x1234]
        fnild   dword ptr [bx+si]
        fnild   qword ptr [bx+di-52]
        fnbld   tbyte ptr [bp+si+0x89AB]
        fnst    dword ptr [bp+0x1234]
        fnst    qword ptr [bx+si]
        fnst    st(1)
        fnist   word ptr [bx+di-52]
        fnist   dword ptr [bp+si+0x89AB]
        fnstp   dword ptr [bp+si+0x89AB]
        fnstp   qword ptr [si]
        fnstp   tbyte ptr [0x1234]
        fnstp   st(2)
        fnistp  word ptr [di+52]
        fnistp  dword ptr [bp+0x1234]
        fnistp  qword ptr [bx+si]
        fnbstp  tbyte ptr [bx+di-52]
        fnxch   st(3)
;## Comparison
        fnicom  word ptr [si]
        fnicom  dword ptr [0x1234]
        fncom   dword ptr [si]
        fncom   qword ptr [0x1234]
        fncom   st(1)
        fnicomp word ptr [di+52]
        fnicomp dword ptr [bp+0x1234]
        fncomp  dword ptr [di+52]
        fncomp  qword ptr [bp+0x1234]
        fncomp  st(3)
        fncompp
        fntst
        fnxam
;## Constants
        fnldz
        fnld1
        fnldpi
        fnldl2t
        fnldl2e
        fnldlg2
        fnldln2
;## Arithmetic
        fniadd  word ptr [si]
        fniadd  dword ptr [0x1234]
        fnadd   dword ptr [si]
        fnadd   qword ptr [0x1234]
        fnadd   st, st(1)
        fnadd   st(2), st
        fnaddp  st(3), st
        fnisub  word ptr [bx+si]
        fnisub  dword ptr [bx+di-52]
        fnsub   dword ptr [bx+si]
        fnsub   qword ptr [bx+di-52]
        fnsub   st, st(5)
        fnsub   st(6), st
        fnsubp  st(7), st
        fnisubr word ptr [bp+si+0x89AB]
        fnisubr dword ptr [si]
        fnsubr  dword ptr [bp+si+0x89AB]
        fnsubr  qword ptr [si]
        fnsubr  st, st(7)
        fnsubr  st(1), st
        fnsubrp st(2), st
        fnimul  word ptr [di+52]
        fnimul  dword ptr [bp+0x1234]
        fnmul   dword ptr [di+52]
        fnmul   qword ptr [bp+0x1234]
        fnmul   st, st(3)
        fnmul   st(4), st
        fnmulp  st(5), st
        fnidiv  word ptr [0x1234]
        fnidiv  dword ptr [di+52]
        fndiv   dword ptr [0x1234]
        fndiv   dword ptr [di+52]
        fndiv   st, st(2)
        fndiv   st(3), st
        fndivp  st(4), st
        fnidivr word ptr [bp+0x1234]
        fnidivr dword ptr [bx+si]
        fndivr  dword ptr [bp+0x1234]
        fndivr  qword ptr [bx+si]
        fndivr  st, st(4)
        fndivr  st(5), st
        fndivrp st(6), st
        fnsqrt
        fnscale
        fnprem
        fnrndint
        fnxtract
        fnabs
        fnchs
;## Transcendental
        fnptan
        fnpatan
        fn2xm1
        fnyl2x
        fnyl2xp1
;## Co-processor control
        finit
        fninit
        feni
        fneni
        fdisi
        fndisi
        fnldcw  [si]
        fstcw   [0x1234]
        fnstcw  [di+52]
        fstsw   [bp+0x1234]
        fnstsw  [bx+si]
        fclex
        fnclex
        fstenv  [bx+di-52]
        fnstenv [bp+si+0x89AB]
        fnldenv [di]
        fsave   [0x5678]
        fnsave  [si+52]
        fnrstor [bx+0x1234]
        fnincstp
        fndecstp
        fnfree  st(2)
        fnnop
        fwait
;## Segment override
        fnldcw  es:[si]
        fstcw   es:[si]

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
