;;; Copyright 2025 Tadashi G. Takaoka
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


;;; 0X
        mvac    24, 32
        mvac    24, #17
        msac    24, 32
        msac    24, #17
;;; 4X
        rpt     32
        rptnst  32
        rptnh   32
        rptgt   32
        rptnc   32
        rptnvt  32
        rptnv   32
        rptge   32
        rptne   32
        rptst   32
        rpth    32
        rptle   32
        rptc    32
        rptvt   32
        rptv    32
        rptlt   32
        rpte    32
        rpti    32
        rptinst 32
        rptinh  32
        rptigt  32
        rptinc  32
        rptinvt 32
        rptinv  32
        rptige  32
        rptine  32
        rptist  32
        rptih   32
        rptile  32
        rptic   32
        rptivt  32
        rptiv   32
        rptilt  32
        rptie   32
        rpt     #20
        rpt     [32]
        rpt     [32]+
        rptie   [32]+
        mac     26, 32
        smac    26, 32
        macr    26, 32
        smacr   26, 32
        macz    26, 32
        smacz   26, 32
        macrz   26, 32
        smacrz  26, 32
        mac     26, [32]
        mac     26, [32]+
        mac     26, 52[32]
        mac     26, 5634h[32]
        mac     26, #3412h
;;; 6X
        mac     32
        smac    32
        macr    32
        smacr   32
        macz    32
        smacz   32
        macrz   32
        smacrz  32
        mac     [32]
        mac     [32]+
        mac     52[32]
        mac     5634h[32]
        mac     #3412h
;;; EX
        reti

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
