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

;;; The 80960 base instruction set: everything GNU as accepts under -AKA,
;;; which is I_BASE plus the I_KX synchronous instructions.  The KB floating
;;; point and decimal instructions are covered separately.
;;;
;;; Register spelling is constrained by GNU as, which rejects r0, r1, r2 and
;;; g15 outright and accepts only the architectural names pfp, sp, rip and fp
;;; for them.  ASL takes either spelling and encodes both identically, so the
;;; alias names are the only ones usable in a source shared by both tools.
;;; Everything here is lowercase for the same reason.
;;;
;;; IP-relative operands are NOT here; see test_i960_iprel.inc for why they
;;; cannot be spelled the same way in both assemblers.

;;; REG format, three operands.  The src1 and src2 fields each carry a mode
;;; bit selecting between a register and a five-bit literal, so every
;;; three-operand instruction is exercised with a register pair, with a
;;; literal in src1, with a literal in src2, and with both.

;;; Integer and ordinal arithmetic
        addi    g0, g1, g2
        addi    31, g1, g2
        addi    g0, 0, g2
        addi    1, 2, g3
        addo    g0, g1, g2
        addo    16, g1, g2
        addc    g0, g1, g2
        addc    g0, 7, g2
        subi    g0, g1, g2
        subi    31, g1, g2
        subo    g0, g1, g2
        subo    g0, 31, g2
        subc    g0, g1, g2
        muli    g0, g1, g2
        muli    3, g1, g2
        mulo    g0, g1, g2
        divi    g0, g1, g2
        divo    g0, g1, g2
        divo    g0, 4, g2
        remi    g0, g1, g2
        remo    g0, g1, g2
        modi    g0, g1, g2

;;; Extended arithmetic.  emul writes a register pair, ediv reads one, so
;;; both name an even-numbered register.  No literal is used where the
;;; operand is a register PAIR: ASL evaluates such a literal as a floating
;;; point value, so it encodes 0 as 16 and 1 as 22 rather than as the plain
;;; five-bit integer literal GNU as and the manual call for.  See
;;; ~/asl_bugs/asl_bug_i960.md.
        emul    g0, g1, g4
        emul    5, g1, g4
        ediv    g0, g4, g2

;;; Logical
        and     g0, g1, g2
        andnot  g0, g1, g2
        andnot  31, g1, g2
        nand    g0, g1, g2
        nor     g0, g1, g2
        not     g0, g2
        not     31, g2
        notand  g0, g1, g2
        notor   g0, g1, g2
        or      g0, g1, g2
        ornot   g0, g1, g2
        xnor    g0, g1, g2
        xor     g0, g1, g2
        xor     8, 9, g2

;;; Bit and bit-field
        alterbit 0, g1, g2
        alterbit 31, g1, g2
        chkbit  g0, g1
        chkbit  5, g1
        clrbit  g0, g1, g2
        notbit  g0, g1, g2
        setbit  g0, g1, g2
        scanbit g0, g2
        spanbit g0, g2
        scanbyte g0, g1
        scanbyte 0, g1
        extract g0, g1, g2
        extract 4, 8, g2
        modify  g0, g1, g2
        modify  31, g1, g2

;;; Shift and rotate
        rotate  g0, g1, g2
        rotate  1, g1, g2
        shli    g0, g1, g2
        shlo    g0, g1, g2
        shlo    31, g1, g2
        shrdi   g0, g1, g2
        shri    g0, g1, g2
        shro    g0, g1, g2

;;; Compare
        cmpi    g0, g1
        cmpi    31, g1
        cmpi    g0, 0
        cmpo    g0, g1
        concmpi g0, g1
        concmpo g0, g1
        cmpdeci g0, g1, g2
        cmpdeco g0, g1, g2
        cmpinci g0, g1, g2
        cmpinco g0, g1, g2

;;; Move.  movl needs an even register, movt and movq a multiple of four.
        mov     g0, g2
        mov     31, g2
        mov     g0, sp
        mov     pfp, rip
        mov     fp, g2
        movl    g0, g4
        movl    r4, r8
        movt    g0, g4
        movt    r4, r8
        movq    g0, g4
        movq    r8, r12

;;; Processor management
        calls   g0
        calls   7
        flushreg
        fmark
        mark
        syncf
        modac   g0, g1, g2
        modac   0, g1, g2
        modpc   g0, g1, g2
        modtc   g0, g1, g2

;;; Atomic and synchronous.  These are the I_KX instructions; the operands
;;; are plain registers, no literals.
        atadd   g0, g1, g2
        atmod   g0, g1, g2
        synld   g0, g2
        synmov  g0, g1
        synmovl g0, g1
        synmovq g0, g1

;;; CTRL format.  A 24-bit word displacement reaches +-8M; the operand is the
;;; target address in both assemblers.
ctrl:   b       ctrl
        bal     ctrl
        call    ctrl
        ret
        bno     ctrl
        bg      ctrl
        be      ctrl
        bge     ctrl
        bl      ctrl
        bne     ctrl
        ble     ctrl
        bo      ctrl

;;; Fault if.  No operand; the condition is part of the opcode.
        faultno
        faultg
        faulte
        faultge
        faultl
        faultne
        faultle
        faulto

;;; COBR format.  A 13-bit word displacement reaches +-4K.  The src1 field
;;; has a mode bit for a five-bit literal, src2 is always a register.
cobr:   cmpibno g0, g1, cobr
        cmpibg  g0, g1, cobr
        cmpibe  g0, g1, cobr
        cmpibge g0, g1, cobr
        cmpibl  g0, g1, cobr
        cmpibne g0, g1, cobr
        cmpible g0, g1, cobr
        cmpibo  g0, g1, cobr
        cmpibe  31, g1, cobr
        cmpobg  g0, g1, cobr
        cmpobe  g0, g1, cobr
        cmpobge g0, g1, cobr
        cmpobl  g0, g1, cobr
        cmpobne g0, g1, cobr
        cmpoble g0, g1, cobr
        cmpoble 0, g1, cobr
        bbc     g0, g1, cobr
        bbc     31, g1, cobr
        bbs     g0, g1, cobr

;;; Test.  COBR format with only the src1 register field used.
        testno  g0
        testg   g1
        teste   g2
        testge  g3
        testl   g4
        testne  g5
        testle  g6
        testo   fp

;;; MEM format.  Every addressing mode except IP-relative, exercised on ld
;;; so the mode encoding is the only thing that varies.
;;;
;;; MEMA reaches a 12-bit unsigned offset, optionally added to a base
;;; register.  MEMB adds a second word for a 32-bit displacement and can
;;; scale an index register by 1, 2, 4, 8 or 16.
        ld      0x40, g6                ; MEMA, offset only
        ld      0xfff, g6               ; MEMA, largest offset
        ld      0x40(g5), g6            ; MEMA, offset + base
        ld      (g5), g6                ; MEMB, base only
        ld      0x1234(g5), g6          ; MEMB, displacement + base
        ld      0x12345678, g6          ; MEMB, displacement only
        ld      (g5)[g4*1], g6          ; MEMB, base + index
        ld      (g5)[g4*2], g6
        ld      (g5)[g4*4], g6
        ld      (g5)[g4*8], g6
        ld      (g5)[g4*16], g6
        ld      0x1234[g4*4], g6        ; MEMB, displacement + index
        ld      0x1234(g5)[g4*4], g6    ; MEMB, displacement + base + index
        ld      -1(g5)[g4*16], g6

;;; The load and store family over the same operand shapes.  ldl and stl name
;;; an even register, ldt, stt, ldq and stq a multiple of four.
        ldob    0x40(g5), g6
        ldos    0x40(g5), g6
        ldib    0x40(g5), g6
        ldis    0x40(g5), g6
        ld      0x40(g5), g6
        ldl     0x40(g5), g4
        ldt     0x40(g5), g4
        ldq     0x40(g5), g4
        ldob    (g5)[g4*2], g6
        ldl     0x1234(g5), g8
        ldt     0x1234(g5), g8
        ldq     0x1234(g5), g8

        stob    g6, 0x40(g5)
        stos    g6, 0x40(g5)
        stib    g6, 0x40(g5)
        stis    g6, 0x40(g5)
        st      g6, 0x40(g5)
        stl     g4, 0x40(g5)
        stt     g4, 0x40(g5)
        stq     g4, 0x40(g5)
        stob    g6, (g5)[g4*2]
        stl     g8, 0x1234(g5)
        stt     g8, 0x1234(g5)
        stq     g8, 0x1234(g5)

;;; lda computes an effective address rather than loading from it.
        lda     0x40, g6
        lda     0x40(g5), g6
        lda     0x12345678, g6
        lda     0x1234(g5)[g4*4], g6

;;; Branch and call through a memory operand.
        bx      (g5)
        bx      0x40(g5)
        bx      0x12345678
        balx    (g5), g6
        balx    0x1234(g5)[g4*4], g6
        callx   (g5)
        callx   0x40
        callx   0x1234(g5)[g4*8]
