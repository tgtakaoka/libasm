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

        CPU     6100
;;; Page 0
        ORG     00H
ZPAGE = .

;;; Page 1
        ORG     80H

;;; Memory reference instructions
CPAGE = .
        AND     ZPAGE+12h
        AND     I ZPAGE+23h
        TAD     ZPAGE+34h
        TAD     I ZPAGE+45h
        ISZ     ZPAGE+56h
        ISZ     I ZPAGE+67h
        DCA     ZPAGE+78h
        DCA     I ZPAGE+09h
        JMS     ZPAGE+1ah
        JMS     I ZPAGE+2bh
        JMP     ZPAGE+3ch
        JMP     I ZPAGE+4dh

        AND     CPAGE+09h
        AND     I CPAGE+1ah
        TAD     CPAGE+2bh
        TAD     I CPAGE+3ch
        ISZ     CPAGE+4dh
        ISZ     I CPAGE+5eh
        DCA     CPAGE+6fh
        DCA     I CPAGE+70h
        JMS     CPAGE+01h
        JMS     I CPAGE+12h
        JMP     CPAGE+23h
        JMP     I CPAGE+34h

;;; Operate instructions (Group 1)
        NOP
        IAC
        BSW
        BSW IAC
        RAL
        RAL IAC
        RTL
        RTL IAC
        RAR
        RAR IAC
        RTR
        RTR IAC

        CML
        CML IAC
        CML BSW
        CML BSW IAC
        CML RAL
        CML RAL IAC
        CML RTL
        CML RTL IAC
        CML RAR
        CML RAR IAC
        CML RTR
        CML RTR IAC

        CMA
        CMA IAC
        CIA
        CMA BSW
        CML BSW IAC
        CIA BSW
        CMA RAL
        CMA RAL IAC
        CIA RAL
        CMA RTL
        CMA RTL IAC
        CIA RTL
        CMA RAR
        CMA RAR IAC
        CIA RAR
        CMA RTR
        CMA RTR IAC
        CIA RTR

        CMA CML
        CMA CML IAC
        CIA CML
        CMA CML BSW
        CMA CML BSW IAC
        CIA CML BSW
        CMA CML RAL
        CMA CML RAL IAC
        CIA CML RAL
        CMA CML RTL
        CMA CML RTL IAC
        CIA CML RTL
        CMA CML RAR
        CMA CML RAR IAC
        CIA CML RAR
        CMA CML RTR
        CMA CML RTR IAC
        CIA CML RTR

        CLL
        CLL IAC
        CLL BSW
        CLL BSW IAC
        CLL RAL
        CLL RAL IAC
        CLL RTL
        CLL RTL IAC
        CLL RAR
        CLL RAR IAC
        CLL RTR
        CLL RTR IAC

        CLL CML
        STL
        CLL CML IAC
        STL IAC
        CLL CML BSW
        STL BSW
        CLL CML BSW IAC
        STL BSW IAC
        CLL CML RAL
        STL RAL
        CLL CML RAL IAC
        STL RAL IAC
        CLL CML RTL
        STL RTL
        CLL CML RTL IAC
        STL RTL IAC
        CLL CML RAR
        STL RAR
        CLL CML RAR IAC
        STL RAR IAC
        CLL CML RTR
        STL RTR
        CLL CML RTR IAC
        STL RTR IAC

        CLL CMA
        CLL CMA IAC
        CLL CIA
        CLL CMA BSW
        CLL CMA BSW IAC
        CLL CIA BSW
        CLL CMA RAL
        CLL CMA RAL IAC
        CLL CIA RAL
        CLL CMA RTL
        CLL CMA RTL IAC
        CLL CIA RTL
        CLL CMA RAR
        CLL CMA RAR IAC
        CLL CIA RAR
        CLL CMA RTR
        CLL CMA RTR IAC
        CLL CIA RTR

        CLL CMA CML
        STL CMA
        CLL CMA CML IAC
        STL CMA IAC
        STL CIA
        CLL CMA CML BSW
        STL CMA BSW
        CLL CMA CML BSW IAC
        STL CMA BSW IAC
        STL CIA BSW
        CLL CMA CML RAL
        STL CMA RAL
        CLL CMA CML RAL IAC
        STL CMA RAL IAC
        STL CIA RAL
        CLL CMA CML RTL
        STL CMA RTL
        CLL CMA CML RTL IAC
        STL CMA RTL IAC
        STL CIA RTL
        CLL CMA CML RAR
        STL CMA RAR
        CLL CMA CML RAR IAC
        STL CMA RAR IAC
        STL CIA RAR
        CLL CMA CML RTR
        STL CMA RTR
        CLL CMA CML RTR IAC
        STL CMA RTR IAC
        STL CIA RTR

        CLA
        CLA IAC
        CLA BSW
        CLA BSW IAC
        CLA RAL
        GLK
        CLA RAL IAC
        GLK IAC
        CLA RTL
        CLA RTL IAC
        CLA RAR
        CLA RAR IAC
        CLA RTR
        CLA RTR IAC

        CLA CML
        CLA CML IAC
        CLA CML BSW
        CLA CML BSW IAC
        CLA CML RAL
        GLK CML
        CLA CML RAL IAC
        GLK CML IAC
        CLA CML RTL
        CLA CML RTL IAC
        CLA CML RAR
        CLA CML RAR IAC
        CLA CML RTR
        CLA CML RTR IAC

        CLA CMA
        STA
        CLA CMA IAC
        STA IAC
        CLA CMA BSW
        STA BSW
        CLA CMA BSW IAC
        STA BSW IAC
        CLA CMA RAL
        GLK CMA
        CLA CMA RAL IAC
        GLK CMA IAC
        GLK CIA
        CLA CMA RTL
        STA RTL
        CLA CMA RTL IAC
        STA RTL IAC
        CLA CMA RAR
        STA RAR
        CLA CMA RAR IAC
        STA RAR IAC
        CLA CMA RTR
        STA RTR
        CLA CMA RTR IAC
        STA RTR IAC

        CLA CMA CML
        STA CML
        CLA CMA CML IAC
        STA CML IAC
        CLA CMA CML BSW
        STA CML BSW
        CLA CMA CML BSW IAC
        STA CML BSW IAC
        CLA CMA CML RAL
        GLK CMA CML
        CLA CMA CML RAL IAC
        CLA CIA CML RAL
        GLK CMA CML IAC
        GLK CIA CML
        CLA CMA CML RTL
        STA CML RTL
        CLA CMA CML RTL IAC
        STA CML RTL IAC
        CLA CMA CML RAR
        STA CML RAR
        CLA CMA CML RAR IAC
        STA CML RAR IAC
        CLA CMA CML RTR
        STA CML RTR
        CLA CMA CML RTR IAC
        STA CML RTR IAC

        CLA CLL
        CLA CLL IAC
        CLA CLL BSW
        CLA CLL BSW IAC
        CLA CLL RAL
        GLK CLL
        CLA CLL RAL IAC
        GLK CLL IAC
        CLA CLL RTL
        CLA CLL RTL IAC
        CLA CLL RAR
        CLA CLL RAR IAC
        CLA CLL RTR
        CLA CLL RTR IAC

        CLA CLL CML
        STL CLA
        CLA CLL CML IAC
        STL CLA IAC
        CLA CLL CML BSW
        STL CLA BSW
        CLA CLL CML BSW IAC
        STL CLA BSW IAC
        CLA CLL CML RAL
        STL CLA RAL
        GLK CLL CML
        GLK STL
        CLA CLL CML RAL IAC
        STL CLA RAL IAC
        GLK CLL CML IAC
        GLK STL IAC
        CLA CLL CML RTL
        STL CLA RTL
        CLA CLL CML RTL IAC
        STL CLA RTL IAC
        CLA CLL CML RAR
        STL CLA RAR
        CLA CLL CML RAR IAC
        STL CLA RAR IAC
        CLA CLL CML RTR
        STL CLA RTR
        CLA CLL CML RTR IAC
        STL CLA RTR IAC

        CLA CLL CMA
        STA CLL
        CLA CLL CMA IAC
        STA CLL IAC
        CLA CLL CMA BSW
        STA CLL BSW
        CLA CLL CMA BSW IAC
        STA CLL BSW IAC
        CLA CLL CMA RAL
        STA CLL RAL
        GLK CLL CMA
        CLA CLL CMA RAL IAC
        CLA CIA CLL RAL
        GLK CLL CMA IAC
        GLK CLL CIA
        CLA CLL CMA RTL
        STA CLL RTL
        CLA CLL CMA RTL IAC
        STA CLL RTL IAC
        CLA CLL CMA RAR
        STA CLL RAR
        CLA CLL CMA RAR IAC
        STA CLL RAR IAC
        CLA CLL CMA RTR
        STA CLL RTR
        CLA CLL CMA RTR IAC
        STA CLL RTR IAC

        CLA CLL CMA CML
        STA CLL CML
        STL CLA CMA
        STL STA
        CLA CLL CMA CML IAC
        STA CLL CML IAC
        STL CLA CMA IAC
        CIA CLA CLL CML
        STL STA IAC
        CLA CLL CMA CML BSW
        STA CLL CML BSW
        STL CLA CMA BSW
        STL STA BSW
        CLA CLL CMA CML BSW IAC
        STA CLL CML BSW IAC
        STL CLA CMA BSW IAC
        CIA CLA CLL CML BSW
        STL STA BSW IAC
        CLA CLL CMA CML RAL
        STA CLL CML RAL
        STL CLA CMA RAL
        GLK STL CMA
        CLA CLL CMA CML RAL IAC
        STA CLL CML RAL IAC
        STL CLA CMA RAL IAC
        GLK STL CIA
        CLA CLL CMA CML RTL
        STA CLL CML RTL
        STL CLA CMA RTL
        STL STA RTL
        CLA CLL CMA CML RTL IAC
        STA CLL CML RTL IAC
        STL CLA CMA RTL IAC
        CIA CLA CLL CML RTL
        STL CIA CLA RTL
        STL STA RTL IAC
        CLA CLL CMA CML RAR
        STA CLL CML RAR
        STL CLA CMA RAR
        STL STA RAR
        CLA CLL CMA CML RAR IAC
        STA CLL CML RAR IAC
        STL CLA CMA RAR IAC
        CIA CLA CLL CML RAR
        STL CIA CLA RAR
        STL STA RAR IAC
        CLA CLL CMA CML RTR
        STA CLL CML RTR
        STL CLA CMA RTR
        STL STA RTR
        CLA CLL CMA CML RTR IAC
        STA CLL CML RTR IAC
        STL CLA CMA RTR IAC
        CIA CLA CLL CML RTR
        STL CIA CLA RTR
        STL STA RTR IAC

;;; Operate instructions (Group 2)
        NOP
        SKP
        SNL
        SZL
        SZA
        SNA
        SZA SNL
        SNA SZL
        SMA
        SPA
        SMA SNL
        SPA SZL
        SMA SZA
        SPA SNA
        SMA SZA SNL
        SPA SNA SZL

        CLA
        SKP CLA
        SNL CLA
        SZL CLA
        SZA CLA
        SNA CLA
        SZA SNL CLA
        SNA SZL CLA
        SMA CLA
        SPA CLA
        SMA SNL CLA
        SPA SZL CLA
        SMA SZA CLA
        SPA SNA CLA
        SMA SZA SNL CLA
        SPA SNA SZL CLA

        HLT
        SKP HLT
        SNL HLT
        SZL HLT
        SZA HLT
        SNA HLT
        SZA SNL HLT
        SNA SZL HLT
        SMA HLT
        SPA HLT
        SMA SNL HLT
        SPA SZL HLT
        SMA SZA HLT
        SPA SNA HLT
        SMA SZA SNL HLT
        SPA SNA SZL HLT

        CLA HLT
        SKP CLA HLT
        SNL CLA HLT
        SZL CLA HLT
        SZA CLA HLT
        SNA CLA HLT
        SZA SNL CLA HLT
        SNA SZL CLA HLT
        SMA CLA HLT
        SPA CLA HLT
        SMA SNL CLA HLT
        SPA SZL CLA HLT
        SMA SZA CLA HLT
        SPA SNA CLA HLT
        SMA SZA SNL CLA HLT
        SPA SNA SZL CLA HLT

        OSR
        SKP OSR
        SNL OSR
        SZL OSR
        SZA OSR
        SNA OSR
        SZA SNL OSR
        SNA SZL OSR
        SMA OSR
        SPA OSR
        SMA SNL OSR
        SPA SZL OSR
        SMA SZA OSR
        SPA SNA OSR
        SMA SZA SNL OSR
        SPA SNA SZL OSR

        CLA OSR
        LAS
        SKP CLA OSR
        SKP LAS
        SNL CLA OSR
        SNL LAS
        SZL CLA OSR
        SZL LAS
        SZA CLA OSR
        SZA LAS
        SNA CLA OSR
        SNA LAS
        SZA SNL CLA OSR
        SZA SNL LAS
        SNA SZL CLA OSR
        SNA SZL LAS
        SMA CLA OSR
        SMA LAS
        SPA CLA OSR
        SPA LAS
        SMA SNL CLA OSR
        SMA SNL LAS
        SPA SZL CLA OSR
        SPA SZL LAS
        SMA SZA CLA OSR
        SMA SZA LAS
        SPA SNA CLA OSR
        SPA SNA LAS
        SMA SZA SNL CLA OSR
        SMA SZA SNL LAS
        SPA SNA SZL CLA OSR
        SPA SNA SZL LAS

        OSR HLT
        SKP OSR HLT
        SNL OSR HLT
        SZL OSR HLT
        SZA OSR HLT
        SNA OSR HLT
        SZA SNL OSR HLT
        SNA SZL OSR HLT
        SMA OSR HLT
        SPA OSR HLT
        SMA SNL OSR HLT
        SPA SZL OSR HLT
        SMA SZA OSR HLT
        SPA SNA OSR HLT
        SMA SZA SNL OSR HLT
        SPA SNA SZL OSR HLT

        CLA OSR HLT
        LAS HLT
        SKP CLA OSR HLT
        SKP LAS HLT
        SNL CLA OSR HLT
        SNL LAS HLT
        SZL CLA OSR HLT
        SZL LAS HLT
        SZA CLA OSR HLT
        SZA LAS HLT
        SNA CLA OSR HLT
        SNA LAS HLT
        SZA SNL CLA OSR HLT
        SZA SNL LAS HLT
        SNA SZL CLA OSR HLT
        SNA SZL LAS HLT
        SMA CLA OSR HLT
        SMA LAS HLT
        SPA CLA OSR HLT
        SPA LAS HLT
        SMA SNL CLA OSR HLT
        SMA SNL LAS HLT
        SPA SZL CLA OSR HLT
        SPA SZL LAS HLT
        SMA SZA CLA OSR HLT
        SMA SZA LAS HLT
        SPA SNA CLA OSR HLT
        SPA SNA LAS HLT
        SMA SZA SNL CLA OSR HLT
        SMA SZA SNL LAS HLT
        SPA SNA SZL CLA OSR HLT
        SPA SNA SZL LAS HLT

;;; Operate instructions (Group 3)
        MQL
        MQA
        MQA MQL
        CLA MQL
        CLA MQA
        CLA MQA MQL

        SWP
        MQA MQL
        CAM
        CLA MQL
        ACL
        CLA MQA
        CLA SWP
        CLA MQA MQL

;;; Input/Output Transfer instructions
        SKON
        ION
        IOF
        SRQ
        GTF
        RTF
        SGT
        CAF
        IOT     1 0
        IOT     1 7
        IOT     77Q 0
        IOT     77Q 7

        END

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
