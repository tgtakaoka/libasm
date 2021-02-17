;;; Copyright 2021 Tadashi G. Takaoka
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
;;; 687X
        dadc
        dsav
        dsmb
        daci    7DH
        dsbi    80H
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
