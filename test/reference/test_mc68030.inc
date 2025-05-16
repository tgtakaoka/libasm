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

        pmove.l (a2),tc
        pmove.q (a2),srp
        pmove.q (a2),crp
        pmove.w (a2),psr
        pmove.l (a2),tt0
        pmove.l (a2),tt1
        pmove.l ($1234,a2),tc
        pmove.q ($1234,a2),srp
        pmove.q ($1234,a2),crp
        pmove.w ($1234,a2),psr
        pmove.l ($1234,a2),tt0
        pmove.l ($1234,a2),tt1
        pmove.l ($34,a2,d3.w),tc
        pmove.q ($34,a2,d3.w),srp
        pmove.q ($34,a2,d3.w),crp
        pmove.w ($34,a2,d3.w),psr
        pmove.l ($34,a2,d3.w),tt0
        pmove.l ($34,a2,d3.w),tt1
        pmove.l ($1234).w,tc
        pmove.q ($1234).w,srp
        pmove.q ($1234).w,crp
        pmove.w ($1234).w,psr
        pmove.l ($1234).w,tt0
        pmove.l ($1234).w,tt1
        pmove.l ($12345678).l,tc
        pmove.q ($12345678).l,srp
        pmove.q ($12345678).l,crp
        pmove.w ($12345678).l,psr
        pmove.l ($12345678).l,tt0
        pmove.l ($12345678).l,tt1
        pmovefd.l (a2),tc
        pmovefd.q (a2),srp
        pmovefd.q (a2),crp
        pmovefd.l (a2),tt0
        pmovefd.l (a2),tt1
        pmovefd.l ($1234,a2),tc
        pmovefd.q ($1234,a2),srp
        pmovefd.q ($1234,a2),crp
        pmovefd.l ($1234,a2),tt0
        pmovefd.l ($1234,a2),tt1
        pmovefd.l ($34,a2,d3.w),tc
        pmovefd.q ($34,a2,d3.w),srp
        pmovefd.q ($34,a2,d3.w),crp
        pmovefd.l ($34,a2,d3.w),tt0
        pmovefd.l ($34,a2,d3.w),tt1
        pmovefd.l ($1234).w,tc
        pmovefd.q ($1234).w,srp
        pmovefd.q ($1234).w,crp
        pmovefd.l ($1234).w,tt0
        pmovefd.l ($1234).w,tt1
        pmovefd.l ($12345678).l,tc
        pmovefd.q ($12345678).l,srp
        pmovefd.q ($12345678).l,crp
        pmovefd.l ($12345678).l,tt0
        pmovefd.l ($12345678).l,tt1
        pmove.l tc,(a2)
        pmove.q srp,(a2)
        pmove.q crp,(a2)
        pmove.w psr,(a2)
        pmove.l tt0,(a2)
        pmove.l tt1,(a2)
        pmove.l tc,($1234,a2)
        pmove.q srp,($1234,a2)
        pmove.q crp,($1234,a2)
        pmove.w psr,($1234,a2)
        pmove.l tt0,($1234,a2)
        pmove.l tt1,($1234,a2)
        pmove.l tc,($34,a2,d3.w)
        pmove.q srp,($34,a2,d3.w)
        pmove.q crp,($34,a2,d3.w)
        pmove.w psr,($34,a2,d3.w)
        pmove.l tt0,($34,a2,d3.w)
        pmove.l tt1,($34,a2,d3.w)
        pmove.l tc,($1234).w
        pmove.q srp,($1234).w
        pmove.q crp,($1234).w
        pmove.w psr,($1234).w
        pmove.l tt0,($1234).w
        pmove.l tt1,($1234).w
        pmove.l tc,($12345678).l
        pmove.q srp,($12345678).l
        pmove.q crp,($12345678).l
        pmove.w psr,($12345678).l
        pmove.l tt0,($12345678).l
        pmove.l tt1,($12345678).l
        pflusha
        pflush  #7,#4
        pflush  d4,#7
        pflush  sfc,#1
        pflush  dfc,#7
        pflush  #7,#4,(a2)
        pflush  d4,#7,($1234,a2)
        pflush  sfc,#1,($34,a2,d3.w)
        pflush  dfc,#7,($1234).w
        pflush  #7,#4,($12345678).l
        ploadr  #7,(a2)
        ploadr  d3,($1234,a2)
        ploadr  sfc,($34,a2,d3.w)
        ploadr  dfc,($1234).w
        ploadr  #4,($12345678).l
        ploadw  #7,(a2)
        ploadw  d3,($1234,a2)
        ploadw  sfc,($34,a2,d3.w)
        ploadw  dfc,($1234).w
        ploadw  #4,($12345678).l
        ptestr  #7,(a2),#4
        ptestr  d3,($1234,a2),#7
        ptestr  sfc,($34,a2,d3.w),#1
        ptestr  dfc,($1234).w,#5
        ptestr  #4,($12345678).l,#2
        ptestw  #7,(a2),#4
        ptestw  d3,($1234,a2),#7
        ptestw  sfc,($34,a2,d3.w),#1
        ptestw  dfc,($1234).w,#5
        ptestw  #4,($12345678).l,#2
        ptestw  #7,(a2),#4,a3
        ptestw  d3,($1234,a2),#7,a3
        ptestw  sfc,($34,a2,d3.w),#1,a3
        ptestw  dfc,($1234).w,#5,a3
        ptestw  #4,($12345678).l,#2,a3

;### Local Variables:
;### mode: asm
;### End:
;### vim: set ft=asm:
