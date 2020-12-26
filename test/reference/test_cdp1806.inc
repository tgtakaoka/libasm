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

;;; 680X
        stpc
        dtc
        spm2
        scm2
        spm1
        scm1
        ldc
        stm
        gec
        etq
        xie
        xid
        cie
        cid
;;; 682X
        dbnz    0,2122H
        dbnz    1,2223H
        dbnz    2,2324H
        dbnz    3,2425H
        dbnz    4,2526H
        dbnz    5,2627H
        dbnz    6,2728H
        dbnz    7,2829H
        dbnz    8,292AH
        dbnz    9,2A2BH
        dbnz    10,2B2CH
        dbnz    11,2C2DH
        dbnz    12,2D2EH
        dbnz    13,2E2FH
        dbnz    14,2F30H
        dbnz    15,3031H
;;; 683X
        bci     033FH
        bxi     0340H
;;; 686X
        rlxa    0
        rlxa    1
        rlxa    2
        rlxa    3
        rlxa    4
        rlxa    5
        rlxa    6
        rlxa    7
        rlxa    8
        rlxa    9
        rlxa    10
        rlxa    11
        rlxa    12
        rlxa    13
        rlxa    14
        rlxa    15
;;; 687X
        dadc
        dsav
        dsmb
        daci    7DH
        dsbi    80H
;;; 688X
        scal    0,8182H
        scal    1,8283H
        scal    2,8384H
        scal    3,8485H
        scal    4,8586H
        scal    5,8687H
        scal    6,8788H
        scal    7,8889H
        scal    8,898AH
        scal    9,8A8BH
        scal    10,8B9CH
        scal    11,8C8DH
        scal    12,8D8EH
        scal    13,8E8FH
        scal    14,8F90H
        scal    15,9091H
;;; 689X
        sret    0
        sret    1
        sret    2
        sret    3
        sret    4
        sret    5
        sret    6
        sret    7
        sret    8
        sret    9
        sret    10
        sret    11
        sret    12
        sret    13
        sret    14
        sret    15
;;; 68AX
        rsxd    0
        rsxd    1
        rsxd    2
        rsxd    3
        rsxd    4
        rsxd    5
        rsxd    6
        rsxd    7
        rsxd    8
        rsxd    9
        rsxd    10
        rsxd    11
        rsxd    12
        rsxd    13
        rsxd    14
        rsxd    15
;;; 68BX
        rnx     0
        rnx     1
        rnx     2
        rnx     3
        rnx     4
        rnx     5
        rnx     6
        rnx     7
        rnx     8
        rnx     9
        rnx     10
        rnx     11
        rnx     12
        rnx     13
        rnx     14
        rnx     15
;;; 68CX
        rldi    0,0C1C2H
        rldi    1,0C2C3H
        rldi    2,0C3C4H
        rldi    3,0C4C5H
        rldi    4,0C5C6H
        rldi    5,0C6C7H
        rldi    6,0C7C8H
        rldi    7,0C8C9H
        rldi    8,0C9CAH
        rldi    9,0CACBH
        rldi    10,0CBCCH
        rldi    11,0CCCDH
        rldi    12,0CDCEH
        rldi    13,0CECFH
        rldi    14,0CFD0H
        rldi    15,0D0D1H
;;; 68FX
        dadd
        dsm
        dadi    0FDH
        dsmi    00H
;;;
;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
