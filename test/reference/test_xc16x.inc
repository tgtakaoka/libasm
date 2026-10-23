;;; Copyright 2026 Tadashi G. Takaoka
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

;;; MAC unit
        CoABS
        CoABS     R4, R7
        CoABS     R7, [R12+QR0]
        CoABS     [IDX0-], [R1-QR0]
        CoADD     R13, R6
        CoADD     R0, [R11-QR1]
        CoADD     [IDX1+QX0], [R0]
        CoADD2    R6, R5
        CoADD2    R9, [R10-]
        CoADD2    [IDX0+QX1], [R15+QR0]
        CoASHR    #9
        CoASHR    #16, rnd
        CoASHR    R5
        CoASHR    R8, rnd
        CoASHR    [R8+]
        CoASHR    [R13-], rnd
        CoCMP     R1, R2
        CoCMP     R4, [R7-QR0]
        CoCMP     [IDX1-], [R12+QR1]
        CoLOAD    R10, R1
        CoLOAD    R13, [R6]
        CoLOAD    [IDX0-QX0], [R11+]
        CoLOAD-   R3, R0
        CoLOAD-   R6, [R5+QR0]
        CoLOAD-   [IDX1+QX1], [R10-QR0]
        CoLOAD2   R12, R15
        CoLOAD2   R15, [R4-QR1]
        CoLOAD2   [IDX0], [R9]
        CoLOAD2-  R5, R14
        CoLOAD2-  R8, [R3-]
        CoLOAD2-  [IDX1+], [R8+QR0]
        CoMAC     R14, R13
        CoMAC     R1, R2, rnd
        CoMAC     R4, [R7-QR1]
        CoMAC     R7, [R12], rnd
        CoMAC     [IDX0-QX0], [R1+]
        CoMAC     [IDX1-QX0], [R6-], rnd
        CoMAC-    R0, R11
        CoMAC-    R3, [R0-QR0]
        CoMAC-    [IDX0-QX1], [R5+QR1]
        CoMACM    [IDX1-QX1], [R10-QR1]
        CoMACM    [IDX0], [R15], rnd
        CoMACM-   [IDX1], [R4+]
        CoMACMR   [IDX0+], [R9-]
        CoMACMR   [IDX1+], [R14+QR0], rnd
        CoMACMRsu [IDX0-], [R3-QR0]
        CoMACMRsu [IDX1-], [R8+QR1], rnd
        CoMACMRu  [IDX0+QX0], [R13-QR1]
        CoMACMRu  [IDX1+QX0], [R2], rnd
        CoMACMRus [IDX0-QX0], [R7+]
        CoMACMRus [IDX1-QX0], [R12-], rnd
        CoMACMsu  [IDX0+QX1], [R1+QR0]
        CoMACMsu  [IDX1+QX1], [R6-QR0], rnd
        CoMACMsu- [IDX0-QX1], [R11+QR1]
        CoMACMu   [IDX1-QX1], [R0-QR1]
        CoMACMu   [IDX0], [R5], rnd
        CoMACMu-  [IDX1], [R10+]
        CoMACMus  [IDX0+], [R15-]
        CoMACMus  [IDX1+], [R4+QR0], rnd
        CoMACMus- [IDX0-], [R9-QR0]
        CoMACR    R5, R14
        CoMACR    R8, R3, rnd
        CoMACR    R11, [R8]
        CoMACR    R14, [R13+], rnd
        CoMACR    [IDX1-QX0], [R2-]
        CoMACR    [IDX0+QX1], [R7+QR0], rnd
        CoMACRsu  R7, R12
        CoMACRsu  R10, R1, rnd
        CoMACRsu  R13, [R6-QR1]
        CoMACRsu  R0, [R11], rnd
        CoMACRsu  [IDX1], [R0+]
        CoMACRsu  [IDX0+], [R5-], rnd
        CoMACRu   R9, R10
        CoMACRu   R12, R15, rnd
        CoMACRu   R15, [R4+QR1]
        CoMACRu   R2, [R9-QR1], rnd
        CoMACRu   [IDX1+QX0], [R14]
        CoMACRu   [IDX0-QX0], [R3+], rnd
        CoMACRus  R11, R8
        CoMACRus  R14, R13, rnd
        CoMACRus  R1, [R2-QR0]
        CoMACRus  R4, [R7+QR1], rnd
        CoMACRus  [IDX1-QX1], [R12-QR1]
        CoMACRus  [IDX0], [R1], rnd
        CoMACsu   R13, R6
        CoMACsu   R0, R11, rnd
        CoMACsu   R3, [R0+QR0]
        CoMACsu   R6, [R5-QR0], rnd
        CoMACsu   [IDX1-], [R10+QR1]
        CoMACsu   [IDX0+QX0], [R15-QR1], rnd
        CoMACsu-  R15, R4
        CoMACsu-  R2, [R9+]
        CoMACsu-  [IDX1-QX0], [R14-]
        CoMACu    R8, R3
        CoMACu    R11, R8, rnd
        CoMACu    R14, [R13+QR1]
        CoMACu    R1, [R2-QR1], rnd
        CoMACu    [IDX0], [R7]
        CoMACu    [IDX1], [R12+], rnd
        CoMACu-   R10, R1
        CoMACu-   R13, [R6+QR0]
        CoMACu-   [IDX0-], [R11-QR0]
        CoMACus   R3, R0
        CoMACus   R6, R5, rnd
        CoMACus   R9, [R10]
        CoMACus   R12, [R15+], rnd
        CoMACus   [IDX1-QX0], [R4-]
        CoMACus   [IDX0+QX1], [R9+QR0], rnd
        CoMACus-  R5, R14
        CoMACus-  R8, [R3+QR1]
        CoMACus-  [IDX1-QX1], [R8-QR1]
        CoMAX     R14, R13
        CoMAX     R1, [R2+]
        CoMAX     [IDX0+], [R7-]
        CoMIN     R7, R12
        CoMIN     R10, [R1-QR0]
        CoMIN     [IDX1-], [R6+QR1]
        CoMOV     [IDX0+QX0], [R11-QR1]
        CoMUL     R3, R0
        CoMUL     R6, R5, rnd
        CoMUL     R9, [R10-]
        CoMUL     R12, [R15+QR0], rnd
        CoMUL     [IDX1+QX1], [R4-QR0]
        CoMUL     [IDX0-QX1], [R9+QR1], rnd
        CoMUL-    R5, R14
        CoMUL-    R8, [R3]
        CoMUL-    [IDX1], [R8+]
        CoMULsu   R14, R13
        CoMULsu   R1, R2, rnd
        CoMULsu   R4, [R7-QR0]
        CoMULsu   R7, [R12+QR1], rnd
        CoMULsu   [IDX0+QX0], [R1-QR1]
        CoMULsu   [IDX1+QX0], [R6], rnd
        CoMULsu-  R0, R11
        CoMULsu-  R3, [R0-]
        CoMULsu-  [IDX0+QX1], [R5+QR0]
        CoMULu    R9, R10
        CoMULu    R12, R15, rnd
        CoMULu    R15, [R4-QR1]
        CoMULu    R2, [R9], rnd
        CoMULu    [IDX1], [R14+]
        CoMULu    [IDX0+], [R3-], rnd
        CoMULu-   R11, R8
        CoMULu-   R14, [R13-QR0]
        CoMULu-   [IDX1-], [R2+QR1]
        CoMULus   R4, R7
        CoMULus   R7, R12, rnd
        CoMULus   R10, [R1+]
        CoMULus   R13, [R6-], rnd
        CoMULus   [IDX0+QX1], [R11+QR0]
        CoMULus   [IDX1+QX1], [R0-QR0], rnd
        CoMULus-  R6, R5
        CoMULus-  R9, [R10-QR1]
        CoMULus-  [IDX0], [R15]
        CoNEG
        CoNEG     rnd
        CoNOP     [IDX1+]
        CoNOP     [IDX0-], [R3-QR0]
        CoNOP     [R8+QR1]
        CoRND
        CoSHL     #5
        CoSHL     R4
        CoSHL     [R12-]
        CoSHR     #9
        CoSHR     R13
        CoSHR     [R11+QR1]
        CoSTORE   R3, MRW
        CoSTORE   [R6], MSW
        CoSUB     R9, R10
        CoSUB     R12, [R15-]
        CoSUB     [IDX1+], [R4+QR0]
        CoSUB2    R2, R9
        CoSUB2    R5, [R14+QR1]
        CoSUB2    [IDX0+QX0], [R3-QR1]
        CoSUB2R   R11, R8
        CoSUB2R   R14, [R13+]
        CoSUB2R   [IDX1-QX0], [R2-]
        CoSUBR    R4, R7
        CoSUBR    R7, [R12-QR0]
        CoSUBR    [IDX0-QX1], [R1+QR1]

;;; MRW repeat control
- USR0   CoMACM    [IDX0+], [R0+]
- USR1   CoMAC     R12, [R2+]
- USR0   CoSTORE   [R3+], MAL

;;; JMPA/CALLA conditions and branch prediction
        JMPA      cc_UC, xjmp
        JMPA+     cc_NET, xjmp
        JMPA-     cc_Z, xjmp
        JMPA      cc_NZ, xjmp
        JMPA+     cc_V, xjmp
        JMPA-     cc_NV, xjmp
        JMPA      cc_N, xjmp
        JMPA+     cc_NN, xjmp
        JMPA-     cc_C, xjmp
        JMPA      cc_NC, xjmp
        JMPA+     cc_SGT, xjmp
        JMPA-     cc_SLE, xjmp
        JMPA      cc_SLT, xjmp
        JMPA+     cc_SGE, xjmp
        JMPA-     cc_UGT, xjmp
        JMPA      cc_ULE, xjmp
        JMPA+     cc_nusr0, xjmp
        JMPA-     cc_nusr1, xjmp
        JMPA      cc_usr0, xjmp
        JMPA+     cc_usr1, xjmp
        CALLA     cc_UC, xjmp
        CALLA+    cc_NET, xjmp
        CALLA-    cc_Z, xjmp
        CALLA     cc_NZ, xjmp
        CALLA+    cc_V, xjmp
        CALLA-    cc_NV, xjmp
        CALLA     cc_N, xjmp
        CALLA+    cc_NN, xjmp
        CALLA-    cc_C, xjmp
        CALLA     cc_NC, xjmp
        CALLA+    cc_SGT, xjmp
        CALLA-    cc_SLE, xjmp
        CALLA     cc_SLT, xjmp
        CALLA+    cc_SGE, xjmp
        CALLA-    cc_UGT, xjmp
        CALLA     cc_ULE, xjmp
        CALLA+    cc_nusr0, xjmp
        CALLA-    cc_nusr1, xjmp
        CALLA     cc_usr0, xjmp
        CALLA+    cc_usr1, xjmp
xloop:  CoMACM    [IDX1-], [R4-]
        JMPA      cc_nusr1, xloop
xjmp:   SBRK
        ENWDT
