# Copyright 2020 Tadashi G. Takaoka
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

        .z8002
        .include "test_z8000.dat"
        .text
# Clear
        clr     r2
        clr     @r2
        clr     data1234
        clr     data1234(r2)
        clrb    rh1
        clrb    @r2
        clrb    data1234
        clrb    data1234(r2)
# Exchange
        ex      r1,r2
        ex      r1,@r2
        ex      r1,data1234
        ex      r1,data1234(r2)
        exb     rl1,rh2
        exb     rl1,@r2
        exb     rl1,data1234
        exb     rl1,data1234(r2)
# Load
        ld      r1,r2
        ld      r1,@r2
        ld      r1,data1234
        ld      r1,data1234(r2)
        ld      r1,r2(#data1234)
        ld      r1,r2(r4)
        ld      @r2,r1
        ld      data1234,r1
        ld      data1234(r2),r1
        ld      r2(#data1234),r1
        ld      r2(r4),r1
        ld      r2,#0x5678
        ld      @r2,#0x5678
        ld      data1234,#0x5678
        ld      data1234(r2),#0x5678
        ldb     rh1,rh2
        ldb     rh1,@r2
        ldb     rh1,data1234
        ldb     rh1,data1234(r2)
        ldb     rh1,r2(#data1234)
        ldb     rh1,r2(r3)
        ldb     @r2,rh1
        ldb     data1234,rh1
        ldb     data1234(r2),rh1
        ldb     r2(#data1234),rh1
        ldb     r2(r3),rh1
        ldb     rh1,#0x56
        ldb     @r2,#0x56
        ldb     data1234,#0x56
        ldb     data1234(r2),#0x56
        ldl     rr2,rr4
        ldl     rr2,@r4
        ldl     rr2,data1234
        ldl     rr2,data1234(r4)
        ldl     rr2,r4(#data1234)
        ldl     rr2,r4(r6)
        ldl     @r4,rr2
        ldl     data1234,rr2
        ldl     data1234(r4),rr2
        ldl     r4(#data1234),rr2
        ldl     r4(r5),rr2
        ldl     rr2,#0x12345678
# Load Address
        lda     r2,data1234
        lda     r2,data1234(r4)
        lda     r2,r4(#data1234)
        lda     r2,r4(r6)
# Load Address Relative
        ldar    r2,.
# Load Constant
        ldk     r1,#15
# Load Multiple
        ldm     r0,@r2,#16
        ldm     r0,data1234,#15
        ldm     r0,data1234(r2),#14
        ldm     @r2,r1,#1
        ldm     data1234,r1,#2
        ldm     data1234(r2),r1,#3
# Load Relative
        ldr     r1,.
        ldr     .,r1
        ldrb    rh1,.
        ldrb    .,rh1
        ldrl    rr2,.
        ldrl    .,rr2
# Pop
        pop     r1,@r4
        pop     @r2,@r4
        pop     data1234,@r4
        pop     data1234(r2),@r4
        popl    rr2,@r4
        popl    @r2,@r4
        popl    data1234,@r4
        popl    data1234(r2),@r4
# Push
        push    @r4,r1
        push    @r4,#0x1234
        push    @r4,@r2
        push    @r4,data1234
        push    @r4,data1234(r2)
        pushl   @r4,rr2
        pushl   @r4,@r2
        pushl   @r4,data1234
        pushl   @r4,data1234(r2)
# Add with Carry
        adc     r1,r2
        adcb    rh1,rh2
# Add
        add     r1,r2
        add     r1,#0x1234
        add     r1,@r2
        add     r1,data1234
        add     r1,data1234(r2)
        addb    rh1,rh2
        addb    rh1,#0x23
        addb    rh1,@r2
        addb    rh1,data1234
        addb    rh1,data1234(r2)
        addl    rr2,rr4
        addl    rr2,#0x12345678
        addl    rr2,@r4
        addl    rr2,data1234
        addl    rr2,data1234(r4)
# Compare
        cp      r1,r2
        cp      r1,#0x1234
        cp      r1,@r2
        cp      r1,data1234
        cp      r1,data1234(r2)
        cp      @r2,#0x1234
        cp      data1234,#0x1234
        cp      data1234(r2),#0x1234
        cpb     rh1,rh2
        cpb     rh1,#0x12
        cpb     rh1,@r2
        cpb     rh1,data1234
        cpb     rh1,data1234(r2)
        cpb     @r2,#0x12
        cpb     data1234,#0x12
        cpb     data1234(r2),#0x12
        cpl     rr2,rr4
        cpl     rr2,#0x12345678
        cpl     rr2,@r4
        cpl     rr2,data1234
        cpl     rr2,data1234(r4)
# Decimal Adjust
        dab     rh1
# Decrement
        dec     r1,#16
        dec     @r2,#15
        dec     data1234,#14
        dec     data1234(r2),#13
        decb    rh1,#8
        decb    @r2,#7
        decb    data1234,#6
        decb    data1234(r2),#5
# Divide
        div     rr2,r4
        div     rr2,#0x1234
        div     rr2,@r4
        div     rr2,data1234
        div     rr2,data1234(r4)
        divl    rq4,rr6
        divl    rq4,#0x12345678
        divl    rq4,@r6
        divl    rq4,data1234
        divl    rq4,data1234(r6)
# Extend Sign
        exts    rr2
        extsb   r1
        extsl   rq4
# Increment
        inc     r1,#16
        inc     @r2,#15
        inc     data1234,#14
        inc     data1234(r2),#13
        incb    rh1,#8
        incb    @r2,#7
        incb    data1234,#6
        incb    data1234(r2),#5
# Multiply
        mult    rr2,r4
        mult    rr2,#0x1234
        mult    rr2,@r4
        mult    rr2,data1234
        mult    rr2,data1234(r4)
        multl   rq4,rr6
        multl   rq4,#0x12345678
        multl   rq4,@r6
        multl   rq4,data1234
        multl   rq4,data1234(r6)
# Negate
        neg     r1
        neg     @r2
        neg     data1234
        neg     data1234(r2)
        negb    rh1
        negb    @r2
        negb    data1234
        negb    data1234(r2)
# Subtract with Carry
        sbc     r1,r2
        sbcb    rh1,rh2
# Subtract
        sub     r1,r2
        sub     r1,#0x1234
        sub     r1,@r2
        sub     r1,data1234
        sub     r1,data1234(r2)
        subb    rh1,rh2
        subb    rh1,#0x12
        subb    rh1,@r2
        subb    rh1,data1234
        subb    rh1,data1234(r2)
        subl    rr2,rr4
        subl    rr2,#0x12345678
        subl    rr2,@r4
        subl    rr2,data1234
        subl    rr2,data1234(r4)
# And
        and     r1,r2
        and     r1,#0x1234
        and     r1,@r2
        and     r1,data1234
        and     r1,data1234(r2)
        andb    rh1,rh2
        andb    rh1,#0x12
        andb    rh1,@r2
        andb    rh1,data1234
        andb    rh1,data1234(r2)
# Complement
        com     r1
        com     @r2
        com     data1234
        com     data1234(r2)
        comb    rh1
        comb    @r2
        comb    data1234
        comb    data1234(r2)
# Or
        or      r1,r2
        or      r1,#0x1234
        or      r1,@r2
        or      r1,data1234
        or      r1,data1234(r2)
        orb     rh1,rh2
        orb     rh1,#0x12
        orb     rh1,@r2
        orb     rh1,data1234
        orb     rh1,data1234(r2)
# Test
        test    r1
        test    @r2
        test    data1234
        test    data1234(r2)
        testb   rh1
        testb   @r2
        testb   data1234
        testb   data1234(r2)
        testl   rr2
        testl   @r2
        testl   data1234
        testl   data1234(r2)
# Exclusive Or
        xor     r1,r2
        xor     r1,#0x1234
        xor     r1,@r2
        xor     r1,data1234
        xor     r1,data1234(r2)
        xorb    rh1,rh2
        xorb    rh1,#0x12
        xorb    rh1,@r2
        xorb    rh1,data1234
        xorb    rh1,data1234(r2)
# Call Procedure
        call    @r2
        call    data1234
        call    data1234(r2)
# Call Procedure Relative
        calr    .
# Decrement and Jump if Not Zero
        djnz    r1,.
        dbjnz   rh1,.
# Interrupt Return
        iret
# Jump
        jp      @r2
        jp      data1234
        jp      data1234(r2)
        jp      eq,@r2
        jp      ne,data1234
        jp      z,data1234(r2)
# Jump Relative
        jr      f,.
        jr      lt,.
        jr      le,.
        jr      ule,.
        jr      ov,.
        jr      mi,.
        jr      z,.
        jr      eq,.
        jr      c,.
        jr      ult,.
        jr      .
        jr      ge,.
        jr      gt,.
        jr      ugt,.
        jr      nov,.
        jr      pl,.
        jr      nz,.
        jr      ne,.
        jr      nc,.
        jr      uge,.
# Return from Procedure
        ret     f
        ret     lt
        ret     le
        ret     ule
        ret     ov
        ret     mi
        ret     z
        ret     eq
        ret     c
        ret     ult
        ret
        ret     ge
        ret     gt
        ret     ugt
        ret     nov
        ret     pl
        ret     nz
        ret     ne
        ret     nc
        ret     uge
# System call
        sc      #3
# Bit Test
        bit     r1,r2
        bit     r1,#15
        bit     @r2,#14
        bit     data1234,#13
        bit     data1234(r2),#12
        bitb    rh1,r2
        bitb    rh1,#7
        bitb    @r2,#6
        bitb    data1234,#5
        bitb    data1234(r2),#4
# Reset Bit
        res     r1,r2
        res     r1,#15
        res     @r2,#14
        res     data1234,#13
        res     data1234(r2),#12
        resb    rh1,r2
        resb    rh1,#7
        resb    @r2,#6
        resb    data1234,#5
        resb    data1234(r2),#4
# Set Bit
        set     r1,r2
        set     r1,#15
        set     @r2,#14
        set     data1234,#13
        set     data1234(r2),#12
        setb    rh1,r2
        setb    rh1,#7
        setb    @r2,#6
        setb    data1234,#5
        setb    data1234(r2),#4
# Test and Set
        tset    r1
        tset    @r2
        tset    data1234
        test    data1234(r2)
        tsetb   rh1
        tsetb   @r2
        tsetb   data1234
        tsetb   data1234(r2)
# Test Condition Code
        tcc     f,r1
        tcc     lt,r1
        tcc     le,r1
        tcc     ule,r1
        tcc     ov,r1
        tcc     mi,r1
        tcc     z,r1
        tcc     eq,r1
        tcc     c,r1
        tcc     ult,r1
        tcc     r1
        tcc     ge,r1
        tcc     gt,r1
        tcc     ugt,r1
        tcc     nov,r1
        tcc     pl,r1
        tcc     nz,r1
        tcc     ne,r1
        tcc     nc,r1
        tcc     uge,r1
        tccb    f,rh1
        tccb    lt,rh1
        tccb    le,rh1
        tccb    ule,rh1
        tccb    ov,rh1
        tccb    mi,rh1
        tccb    z,rh1
        tccb    eq,rh1
        tccb    c,rh1
        tccb    ult,rh1
        tccb    rh1
        tccb    ge,rh1
        tccb    gt,rh1
        tccb    ugt,rh1
        tccb    nov,rh1
        tccb    pl,rh1
        tccb    nz,rh1
        tccb    ne,rh1
        tccb    nc,rh1
        tccb    uge,rh1
# Rotate Left
        rl      r1,#1
        rlb     rh1,#2
# Rotate Left through Carry
        rlc     r1,#2
        rlcb    rh1,#1
# Rotate Left Digit
        rldb    rh1,rh2
# Rotate Right
        rr      r1,#2
        rrb     rh1,#1
# Rotate Right through Carry
        rrc     r1,#1
        rrcb    rh1,#2
# Rotate Rihgt Digit
        rrdb    rh1,rh2
# Shift Dynamic Arithmetic
        sda     r1,r2
        sdab    rh1,r2
        sdal    rr4,r2
# Shift Dynamic Logical
        sdl     r1,r2
        sdlb    rh1,r2
        sdll    rr4,r2
# Shift Left Arithmetic
        sla     r1,#16
        slab    rh1,#8
        slal    rr4,#32
# Shift Left Logical
        sll     r1,#1
        sllb    rh1,#1
        slll    rr4,#1
# Shift Right Arithmetic
        sra     r1,#2
        srab    rh1,#2
        sral    rr4,#2
# Shift Right Logical
        srl     r1,#3
        srlb    rh1,#3
        srll    rr4,#3
# Compare and Decrement
        cpd     r1,@r2,r4,eq
        cpdb    rh1,@r2,r4,ne
# Compare, Decrement, and Repeat
        cpdr    r1,@r2,r4,eq
        cpdrb   rh1,@r2,r4,ne
# Compare and Increment
        cpi     r1,@r2,r4,eq
        cpib    rh1,@r2,r4,ne
# Compare, Increment, and Repeat
        cpir    r1,@r2,r4,eq
        cpirb   rh1,@r2,r4,ne
# Compare String and Decrement
        cpsd    @r2,@r4,r6,eq
        cpsdb   @r2,@r4,r6,ne
# Compare String, Decrement, and Repeat
        cpsdr   @r2,@r4,r6,eq
        cpsdrb  @r2,@r4,r6,ne
# Compare Sring and Increment
        cpsi    @r2,@r4,r6,eq
        cpsib   @r2,@r4,r6,ne
# Compare String, Increment, and Repeat
        cpsir   @r2,@r4,r6,eq
        cpsirb  @r2,@r4,r6,ne
# Load and Decrement
        ldd     @r2,@r4,r6
        lddb    @r2,@r4,r6
# Load, Decrement, and Repeat
        lddr    @r2,@r4,r6
        lddrb   @r2,@r4,r6
# Load and Increment
        ldi     @r2,@r4,r6
        ldib    @r2,@r4,r6
# Load, Increment, and Repeat
        ldir    @r2,@r4,r6
        ldirb   @r2,@r4,r6
# Translate and Decrement
        trdb    @r2,@r4,r6
# Translate, Decrement, and Repeat
        trdrb   @r2,@r4,r6
# Translate and Increment
        trib    @r2,@r4,r6
# Translate, Increment, and Repeat
        trirb   @r2,@r4,r6
# Translate, Test, and Decrement
        trtdb   @r2,@r4,r6
# Translate, Test, Decrement, and Repeat
        trtdrb  @r2,@r4,r6
# Translate, Test, and Increment
        trtib   @r2,@r4,r6
# Translate, Test, Increment, and Repeat
        trtirb  @r2,@r4,r6
# Input
        in      r1,@r2
        inb     rh1,@r2
        in      r1,#0x1234
        inb     rh1,#0x1234
# Input and Decrement
        ind     @r2,@r4,r6
        indb    @r2,@r4,r6
# Input, Decrement, and Repeat
        indr    @r2,@r4,r6
        indrb   @r2,@r4,r6
# Input and Increment
        ini     @r2,@r4,r6
        inib    @r2,@r4,r6
# Input, Increment, and Repeat
        inir    @r2,@r4,r6
        inirb   @r2,@r4,r6
# Output, Decrement, and Repeat
        otdr    @r2,@r4,r6
        otdrb   @r2,@r4,r6
# Output, Increment, and Repeat
        otir    @r2,@r4,r6
        otirb   @r2,@r4,r6
# Output
        out     @r1,r2
        outb    @r1,rh2
        out     #0x1234,r2
        outb    #0x1234,rh2
# Output and Decrement
        outd    @r2,@r4,r6
        outdb   @r2,@r4,r6
# Output and Increment
        outi    @r2,@r4,r6
        outib   @r2,@r4,r6
# Special Input
        sin     r1,#0x1234
        sinb    rh1,#0x1234
# Special Input, Decrement
        sind    @r2,@r4,r6
        sindb   @r2,@r4,r6
# Special Input, Decrement, and Repeat
        sindr   @r2,@r4,r6
        sindrb  @r2,@r4,r6
# Special Input and Increment
        sini    @r2,@r4,r6
        sinib   @r2,@r4,r6
# Special Input, Increment, and Repeat
        sinir   @r2,@r4,r6
        sinirb  @r2,@r4,r6
# Special Output, Decrement, and Repeat
        sotdr   @r2,@r4,r6
        sotdrb  @r2,@r4,r6
# Special Output, Increment, and Repeat
        sotir   @r2,@r4,r6
        sotirb  @r2,@r4,r6
# Special Output
        sout    #0x1234,r2
        soutb   #0x1234,rl2
# Special Output and Decrement
        soutd   @r2,@r4,r6
        soutdb  @r2,@r4,r6
# Special Output and Increment
        souti   @r2,@r4,r6
        soutib  @r2,@r4,r6
# Complement Flag
        comflg  c,z,s,p,v
        comflg  v,p,s,z,c
        comflg  c
        comflg  v
        comflg  s
# Disable Interrupt
        di      vi,nvi
        di      nvi
        di      vi
# Enable Interrupt
        ei      nvi,vi
        ei      nvi
        ei      vi
# Halt
        halt
# Load Control Register
        ldctl   fcw,r1
        ldctl   refresh,r1
        ldctl   psapseg,r1
        ldctl   psapoff,r1
        ldctl   nspseg,r1
        ldctl   nspoff,r1
        ldctl   r1,fcw
        ldctl   r1,refresh
        ldctl   r1,psapseg
        ldctl   r1,psapoff
        ldctl   r1,nspseg
        ldctl   r1,nspoff
        ldctlb  rh1,flags
        ldctlb  flags,rh2
# Load Program Status
        ldps    @r2
        ldps    data1234
        ldps    data1234(r2)
# Multi-Micro Bit Test
        mbit
# Multi-Micro Request
        mreq    r1
# Multi-Micro Reset
        mres
# Multi-Micro Set
        mset
# No Operation
        nop
# Reset Flag
        resflg  c,z,s,p,v
        resflg  v,p,s,z,c
        resflg  c
        resflg  v
        resflg  s
# Set Flag
        setflg  c,z,s,p,v
        setflg  v,p,s,z,c
        setflg  c
        setflg  v
        setflg  s

        .end

# Local Variables:
# mode: asm
# End:
# vim: set ft=asm:
