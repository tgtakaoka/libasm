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

;;; 65C02
        rmb0    $08
        rmb1    $18
        rmb2    $28
        rmb3    $38
        rmb4    $48
        rmb5    $58
        rmb6    $68
        rmb7    $78
        smb0    $88
        smb1    $98
        smb2    $A8
        smb3    $B8
        smb4    $C8
        smb5    $D8
        smb6    $E8
        smb7    $F8
                 
        bbr0    $10,*+20
        bbr1    $20,*+36
        bbr2    $30,*+52
        bbr3    $40,*+68
        bbr4    $50,*+84
        bbr5    $60,*+100
        bbr6    $70,*+116
        bbr7    $80,*-124
        bbs0    $90,*-108
        bbs1    $A0,*-92
        bbs2    $B0,*-76
        bbs3    $C0,*-60
        bbs4    $D0,*-44
        bbs5    $E0,*-28
        bbs6    $F0,*-12
        bbs7    $00,*+4

;;; comment

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
