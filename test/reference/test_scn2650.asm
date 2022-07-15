;;; Copyright 2022 Tadashi G. Takaoka
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

        cpu     2650
        org     100h
;;; 0X
        lodz,r1
        lodz,r2
        lodz,r3
        lodi,r0 56H
        lodi,r1 67H
        lodi,r2 78H
        lodi,r3 89H
        lodr,r0 *$+0BH
        lodr,r1 $+0CH
        lodr,r2 $+0DH
        lodr,r3 $+0EH
        loda,r0 1123H
        loda,r0 0345H,r0,+
        loda,r0 0567H,r0,-
        loda,r0 0789H,r0
        loda,r0 *19ABH
        loda,r0 *0BCDH,r0,+
        loda,r0 *0DEFH,r0,-
        loda,r0 *0F01H,r0
        loda,r1 0123H
        loda,r0 1345H,r1,+
        loda,r0 0567H,r1,-
        loda,r0 0789H,r1
        loda,r1 *09ABH
        loda,r0 *1BCDH,r1,+
        loda,r0 *0DEFH,r1,-
        loda,r0 *0F01H,r1
        loda,r2 0123H
        loda,r0 0345H,r2,+
        loda,r0 1567H,r2,-
        loda,r0 0789H,r2
        loda,r2 *09ABH
        loda,r0 *0BCDH,r2,+
        loda,r0 *1DEFH,r2,-
        loda,r0 *0F01H,r2
        loda,r3 0123H
        loda,r0 0345H,r3,+
        loda,r0 0567H,r3,-
        loda,r0 1789H,r3
        loda,r3 *09ABH
        loda,r0 *0BCDH,r3,+
        loda,r0 *0DEFH,r3,-
        loda,r0 *1F01H,r3
;;; 1X
        spsu
        spsl
        retc,eq
        retc,gt
        retc,lt
        retc,un
        bctr,eq $+1BH
        bctr,eq *$+1CH
        bctr,gt $+1CH
        bctr,gt *$+2DH
        bctr,lt $+1DH
        bctr,lt *$+3EH
        bctr,un $+1EH
        bctr,un *$-31H
        bcta,eq 7D1EH
        bcta,eq *5EF0H
        bcta,gt 1E1FH
        bcta,gt *6F01H
        bcta,lt 1F20H
        bcta,lt *7012H
        bcta,un 2021H
        bcta,un *49ABH
;;; 2X
        eorz,r0
        eorz,r1
        eorz,r2
        eorz,r3
        eori,r0 25H
        eori,r1 26H
        eori,r2 27H
        eori,r3 28H
        eorr,r0 $+2BH
        eorr,r1 $+2CH
        eorr,r2 *$+2DH
        eorr,r3 $+2EH
        eora,r0 0123H
        eora,r0 0345H,r0,+
        eora,r0 1567H,r0,-
        eora,r0 0789H,r0
        eora,r0 *09ABH
        eora,r0 *0BCDH,r0,+
        eora,r0 *1DEFH,r0,-
        eora,r0 *0F01H,r0
        eora,r1 0123H
        eora,r0 0345H,r1,+
        eora,r0 0567H,r1,-
        eora,r0 1789H,r1
        eora,r1 *09ABH
        eora,r0 *0BCDH,r1,+
        eora,r0 *0DEFH,r1,-
        eora,r0 *1F01H,r1
        eora,r2 1123H
        eora,r0 0345H,r2,+
        eora,r0 0567H,r2,-
        eora,r0 0789H,r2
        eora,r2 *19ABH
        eora,r0 *0BCDH,r2,+
        eora,r0 *0DEFH,r2,-
        eora,r0 *0F01H,r2
        eora,r3 0123H
        eora,r0 1345H,r3,+
        eora,r0 0567H,r3,-
        eora,r0 0789H,r3
        eora,r3 *09ABH
        eora,r0 *1BCDH,r3,+
        eora,r0 *0DEFH,r3,-
        eora,r0 *0F01H,r3
;;; 3X
        redc,r0
        redc,r1
        redc,r2
        redc,r3
        rete,eq
        rete,gt
        rete,lt
        rete,un
        bstr,eq $+3BH
        bstr,eq *$+1CH
        bstr,gt $+3CH
        bstr,gt *$+2DH
        bstr,lt $+3DH
        bstr,lt *$+3EH
        bstr,un $+3EH
        bstr,un *$-31H
        bsta,eq 3D3EH
        bsta,eq *5EF0H
        bsta,gt 3E3FH
        bsta,gt *6F01H
        bsta,lt 3F40H
        bsta,lt *7012H
        bsta,un 4041H
        bsta,un 1041H
        bsta,un *09ABH
;;; 4X
        halt
        andz,r1
        andz,r2
        andz,r3
        andi,r0 56H
        andi,r1 67H
        andi,r2 78H
        andi,r3 89H
        andr,r0 $+1CH
        andr,r1 *$+2DH
        andr,r2 *$+3EH
        andr,r3 *$-31H
        anda,r0 1123H
        anda,r0 0345H,r0,+
        anda,r0 0567H,r0,-
        anda,r0 0789H,r0
        anda,r0 *19ABH
        anda,r0 *0BCDH,r0,+
        anda,r0 *0DEFH,r0,-
        anda,r0 *0F01H,r0
        anda,r1 0123H
        anda,r0 1345H,r1,+
        anda,r0 0567H,r1,-
        anda,r0 0789H,r1
        anda,r1 *09ABH
        anda,r0 *1BCDH,r1,+
        anda,r0 *0DEFH,r1,-
        anda,r0 *0F01H,r1
        anda,r2 0123H
        anda,r0 0345H,r2,+
        anda,r0 1567H,r2,-
        anda,r0 0789H,r2
        anda,r2 *09ABH
        anda,r0 *0BCDH,r2,+
        anda,r0 *1DEFH,r2,-
        anda,r0 *0F01H,r2
        anda,r3 0123H
        anda,r0 0345H,r3,+
        anda,r0 0567H,r3,-
        anda,r0 1789H,r3
        anda,r3 *09ABH
        anda,r0 *0BCDH,r3,+
        anda,r0 *0DEFH,r3,-
        anda,r0 *1F01H,r3
;;; 5X
        rrr,r0
        rrr,r1
        rrr,r2
        rrr,r3
        rede,r0 55H
        rede,r1 56H
        rede,r2 57H
        rede,r3 58H
        brnr,r0 $+1BH
        brnr,r0 *$+1CH
        brnr,r1 $+1CH
        brnr,r1 *$+2DH
        brnr,r2 $+1DH
        brnr,r2 *$+3EH
        brnr,r3 $+1EH
        brnr,r3 *$-31H
        brna,r0 5D5EH
        brna,r0 *5EF0H
        brna,r1 5E5FH
        brna,r1 *6F01H
        brna,r2 5F60H
        brna,r2 *7012H
        brna,r3 6061H
        brna,r3 *09ABH
;;; 6X
        lodz,r0                 ; interpreted as iorz,r0
        iorz,r0
        iorz,r1
        iorz,r2
        iorz,r3
        iori,r0 25H
        iori,r1 26H
        iori,r2 27H
        iori,r3 28H
        iorr,r0 $+2BH
        iorr,r1 *$+2CH
        iorr,r2 $+2DH
        iorr,r3 $+2EH
        iora,r0 0123H
        iora,r0 1345H,r0,+
        iora,r0 0567H,r0,-
        iora,r0 0789H,r0
        iora,r0 *09ABH
        iora,r0 *1BCDH,r0,+
        iora,r0 *0DEFH,r0,-
        iora,r0 *0F01H,r0
        iora,r1 0123H
        iora,r0 0345H,r1,+
        iora,r0 1567H,r1,-
        iora,r0 0789H,r1
        iora,r1 *09ABH
        iora,r0 *0BCDH,r1,+
        iora,r0 *1DEFH,r1,-
        iora,r0 *0F01H,r1
        iora,r2 0123H
        iora,r0 0345H,r2,+
        iora,r0 0567H,r2,-
        iora,r0 1789H,r2
        iora,r2 *09ABH
        iora,r0 *0BCDH,r2,+
        iora,r0 *0DEFH,r2,-
        iora,r0 *1F01H,r2
        iora,r3 1123H
        iora,r0 0345H,r3,+
        iora,r0 0567H,r3,-
        iora,r0 0789H,r3
        iora,r3 *19ABH
        iora,r0 *0BCDH,r3,+
        iora,r0 *0DEFH,r3,-
        iora,r0 *0F01H,r3
;;; 7X
        redd,r0
        redd,r1
        redd,r2
        redd,r3
        cpsu    75H
        cpsl    76H
        ppsu    77H
        ppsl    78H
        bsnr,r0 $-05H
        bsnr,r0 *$+1CH
        bsnr,r1 $-04H
        bsnr,r1 *$+2DH
        bsnr,r2 $-03H
        bsnr,r2 *$+3EH
        bsnr,r3 $-02H
        bsnr,r3 *$-31H
        bsna,r0 7D7EH
        bsna,r0 *5EF0H
        bsna,r1 7E7FH
        bsna,r1 *6F01H
        bsna,r2 7F80H
        bsna,r3 *09ABH
;;; 8X
        addz,r0
        addz,r1
        addz,r2
        addz,r3
        addi,r0 25H
        addi,r1 26H
        addi,r2 27H
        addi,r3 28H
        addr,r0 $+2BH
        addr,r1 $+2CH
        addr,r2 *$+2DH
        addr,r3 $+2EH
        adda,r0 1123H
        adda,r0 0345H,r0,+
        adda,r0 0567H,r0,-
        adda,r0 0789H,r0
        adda,r0 *19ABH
        adda,r0 *0BCDH,r0,+
        adda,r0 *0DEFH,r0,-
        adda,r0 *0F01H,r0
        adda,r1 0123H
        adda,r0 1345H,r1,+
        adda,r0 0567H,r1,-
        adda,r0 0789H,r1
        adda,r1 *09ABH
        adda,r0 *1BCDH,r1,+
        adda,r0 *0DEFH,r1,-
        adda,r0 *0F01H,r1
        adda,r2 0123H
        adda,r0 0345H,r2,+
        adda,r0 1567H,r2,-
        adda,r0 0789H,r2
        adda,r2 *09ABH
        adda,r0 *0BCDH,r2,+
        adda,r0 *1DEFH,r2,-
        adda,r0 *0F01H,r2
        adda,r3 0123H
        adda,r0 0345H,r3,+
        adda,r0 0567H,r3,-
        adda,r0 1789H,r3
        adda,r3 *09ABH
        adda,r0 *0BCDH,r3,+
        adda,r0 *0DEFH,r3,-
        adda,r0 *1F01H,r3
;;; 9X
        lpsu
        lpsl
        dar,r0
        dar,r1
        dar,r2
        dar,r3
        bcfr,eq $+1BH
        bcfr,eq *$+1CH
        bcfr,gt $+1CH
        bcfr,gt *$+2DH
        bcfr,lt $+1DH
        bcfr,lt *$+3EH
        zbrr    3FH
        zbrr    1FC0H
        zbrr    *3FH
        zbrr    *1FC0H
        bcfa,eq 7D1EH
        bcfa,eq *1EF0H
        bcfa,gt 1E1FH
        bcfa,gt *6F01H
        bcfa,lt 1F20H
        bcfa,lt *7012H
        bxa     0123H
        bxa     7FFFH
        bxa     *7FFFH
        bxa     7FFFH,r3
        bxa     *7FFFH,r3
;;; AX
        subz,r0
        subz,r1
        subz,r2
        subz,r3
        subi,r0 0A5H
        subi,r1 0A6H
        subi,r2 0A7H
        subi,r3 0A8H
        subr,r0 $+2BH
        subr,r1 $+2CH
        subr,r2 $+2DH
        subr,r3 *$+2EH
        suba,r0 0123H
        suba,r0 1345H,r0,+
        suba,r0 0567H,r0,-
        suba,r0 0789H,r0
        suba,r0 *09ABH
        suba,r0 *1BCDH,r0,+
        suba,r0 *0DEFH,r0,-
        suba,r0 *0F01H,r0
        suba,r1 0123H
        suba,r0 0345H,r1,+
        suba,r0 1567H,r1,-
        suba,r0 0789H,r1
        suba,r1 *09ABH
        suba,r0 *0BCDH,r1,+
        suba,r0 *1DEFH,r1,-
        suba,r0 *0F01H,r1
        suba,r2 0123H
        suba,r0 0345H,r2,+
        suba,r0 0567H,r2,-
        suba,r0 1789H,r2
        suba,r2 *09ABH
        suba,r0 *0BCDH,r2,+
        suba,r0 *0DEFH,r2,-
        suba,r0 *1F01H,r2
        suba,r3 1123H
        suba,r0 0345H,r3,+
        suba,r0 0567H,r3,-
        suba,r0 0789H,r3
        suba,r3 *19ABH
        suba,r0 *0BCDH,r3,+
        suba,r0 *0DEFH,r3,-
        suba,r0 *0F01H,r3
;;; BX
        wrtc,r0
        wrtc,r1
        wrtc,r2
        wrtc,r3
        tpsu    0B5H
        tpsl    0B6H
        bsfr,eq $+1BH
        bsfr,eq *$+1CH
        bsfr,gt $+1CH
        bsfr,gt *$+2DH
        bsfr,lt $+1DH
        bsfr,lt *$+3EH
        zbsr    3FH
        zbsr    1FC0H
        zbsr    *3FH
        zbsr    *1FC0H
        bsfa,eq 1D1EH
        bsfa,eq *1EF0H
        bsfa,gt 1E1FH
        bsfa,gt *6F01H
        bsfa,lt 1F20H
        bsfa,lt *7012H
        bsxa     0123H
        bsxa     7FFFH
        bsxa    *7FFFH
        bsxa     7FFFH,r3
        bsxa     *7FFFH,r3
;;; CX
        nop
        strz,r1
        strz,r2
        strz,r3
        strr,r0 $+2BH
        strr,r1 *$+2CH
        strr,r2 $+2DH
        strr,r3 $+2EH
        stra,r0 0123H
        stra,r0 1345H,r0,+
        stra,r0 0567H,r0,-
        stra,r0 0789H,r0
        stra,r0 *09ABH
        stra,r0 *1BCDH,r0,+
        stra,r0 *0DEFH,r0,-
        stra,r0 *0F01H,r0
        stra,r1 0123H
        stra,r0 0345H,r1,+
        stra,r0 1567H,r1,-
        stra,r0 0789H,r1
        stra,r1 *09ABH
        stra,r0 *0BCDH,r1,+
        stra,r0 *1DEFH,r1,-
        stra,r0 *0F01H,r1
        stra,r2 0123H
        stra,r0 0345H,r2,+
        stra,r0 0567H,r2,-
        stra,r0 1789H,r2
        stra,r2 *09ABH
        stra,r0 *0BCDH,r2,+
        stra,r0 *0DEFH,r2,-
        stra,r0 *1F01H,r2
        stra,r3 1123H
        stra,r0 0345H,r3,+
        stra,r0 0567H,r3,-
        stra,r0 0789H,r3
        stra,r3 *19ABH
        stra,r0 *0BCDH,r3,+
        stra,r0 *0DEFH,r3,-
        stra,r0 *0F01H,r3
;;; DX
        rrl,r0
        rrl,r1
        rrl,r2
        rrl,r3
        wrte,r0 0D5H
        wrte,r1 0D6H
        wrte,r2 0D7H
        wrte,r3 0D8H
        birr,r0 $+1BH
        birr,r0 *$+1CH
        birr,r1 $+1CH
        birr,r1 *$+2DH
        birr,r2 $+1DH
        birr,r2 *$+3EH
        birr,r3 $+1EH
        birr,r3 *$-31H
        bira,r0 5D5EH
        bira,r0 *6F01H
        bira,r1 5E5FH
        bira,r1 *5EDFH
        bira,r2 5F60H
        bira,r2 *7012H
        bira,r3 0123H
        bira,r3 *5FE0H
;;; EX
        comz,r0
        comz,r1
        comz,r2
        comz,r3
        comi,r0 0E5H
        comi,r1 0E6H
        comi,r2 0E7H
        comi,r3 0E8H
        comr,r0 *$-15H
        comr,r1 *$-14H
        comr,r2 *$-13H
        comr,r3 $-12H
        coma,r0 0123H
        coma,r0 0345H,r0,+
        coma,r0 0567H,r0,-
        coma,r0 1789H,r0
        coma,r0 *09ABH
        coma,r0 *0BCDH,r0,+
        coma,r0 *0DEFH,r0,-
        coma,r0 *1F01H,r0
        coma,r1 1123H
        coma,r0 0345H,r1,+
        coma,r0 0567H,r1,-
        coma,r0 0789H,r1
        coma,r1 *19ABH
        coma,r0 *0BCDH,r1,+
        coma,r0 *0DEFH,r1,-
        coma,r0 *0F01H,r1
        coma,r2 0123H
        coma,r0 1345H,r2,+
        coma,r0 0567H,r2,-
        coma,r0 0789H,r2
        coma,r2 *09ABH
        coma,r0 *1BCDH,r2,+
        coma,r0 *0DEFH,r2,-
        coma,r0 *0F01H,r2
        coma,r3 0123H
        coma,r0 0345H,r3,+
        coma,r0 1567H,r3,-
        coma,r0 0789H,r3
        coma,r3 *09ABH
        coma,r0 *0BCDH,r3,+
        coma,r0 *1DEFH,r3,-
        coma,r0 *0F01H,r3
;;; FX
        wrtd,r0
        wrtd,r1
        wrtd,r2
        wrtd,r3
        tmi,r0  0F5H
        tmi,r1  0F6H
        tmi,r2  0F7H
        tmi,r3  0F8H
        bdrr,r0 $+1BH
        bdrr,r0 *$+1CH
        bdrr,r1 $+1CH
        bdrr,r1 *$+2DH
        bdrr,r2 $+1DH
        bdrr,r2 *$+3EH
        bdrr,r3 $+1EH
        bdrr,r3 *$-31H
        bdra,r0 5D5EH
        bdra,r0 *7DFEH
        bdra,r1 5E5FH
        bdra,r1 *7EFFH
        bdra,r2 5F60H
        bdra,r2 *5CBAH
        bdra,r3 0011H
        bdra,r3 *7EFDH

;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
