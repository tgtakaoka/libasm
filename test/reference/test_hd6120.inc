;;; Copyright 2023 Tadashi G. Takaoka
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

;;; Page 8
        ORG     2000Q

;;; Operate instructions (Group 1)
        R3L
        R3L IAC

        CML R3L
        CML R3L IAC

        CMA R3L
        CMA R3L IAC
        CIA R3L

        CMA CML R3L
        CMA CML R3L IAC
        CIA CML R3L

        CLL R3L
        CLL R3L IAC

        CLL CML R3L
        STL R3L
        CLL CML R3L IAC
        STL R3L IAC

        CLL CMA R3L
        CLL CMA R3L IAC
        CLL CIA R3L

        CLL CMA CML R3L
        STL CMA R3L
        CLL CMA CML R3L IAC
        STL CMA R3L IAC
        STL CIA R3L

        CLA R3L
        CLA R3L IAC

        CLA CML R3L
        CLA CML R3L IAC

        CLA CMA R3L
        STA R3L
        CLA CMA R3L IAC
        STA R3L IAC

        CLA CMA CML R3L
        STA CML R3L
        CLA CMA CML R3L IAC
        STA CML R3L IAC

        CLA CLL R3L
        CLA CLL R3L IAC

        CLA CLL CML R3L
        STL CLA R3L
        CLA CLL CML R3L IAC
        STL CLA R3L IAC

        CLA CLL CMA R3L
        STA CLL R3L
        CLA CLL CMA R3L IAC
        STA CLL R3L IAC

        CLA CLL CMA CML R3L
        STA CLL CML R3L
        STL CLA CMA R3L
        STL STA R3L
        CLA CLL CMA CML R3L IAC
        STA CLL CML R3L IAC
        STL CLA CMA R3L IAC
        CIA CLA CLL CML R3L
        STL CIA CLA R3L
        STL STA R3L IAC

;;; Stack operation
        PPC1
        PAC1
        RTN1
        POP1
        RSP1
        LSP1
        PPC2
        PAC2
        RTN2
        POP2
        RSP2
        LSP2

;;; Internal control
        WSR
        GCF

;;; Main memory control
        PR0
        PR1
        PR2
        PR3

;;; Memory extension
        CDF     7
        CIF     1
        CDF CIF 6
        CIF CDF 5
        RDF
        RIF
        RIB
        RMF

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
