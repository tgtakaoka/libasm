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

# Add: ADDi src/gen, dest/gen
        addb    r0, r1
        addw    r2, r3
        addd    r4, r5
# Add Quick: ADDQi src/quick, dest/gen
        addqb   -8, r0
        addqw   7, r1
        addqd   -8, r2
# Add with Carry: ADDCi src/gen, dest/gen
        addcb   32, r0
        addcw   r2, r3
        addcd   8(sb), r0
# Subtract: SUBi src/gen, dest/gen
        subb    r0, r1
        subw    r2, r3
        subd    4(sb), 20(sb)
# Subtract with Carry: SUBCi src/gen, dest/gen
        subcb   32, r1
        subcw   tos, -8(fp)
        subcd   r4, r5
# Negate: NEGi src/gen, dest/gen
        negb    r5, r6
        negw    4(sb), 6(sb)
        negd    r4, r5
# Absolute Value: ABSi src/gen, dest/gen
        absb    r5, r6
        absw    r2, r3
        absd    8(sp), r7
# Multiply: MULi src/gen, dest/gen
        mulb    r0, r1
        mulw    5, r0
        muld    4(-4(fp)), 3(sb)
# Multiply Extended Integer: MEIi src/gen, dest/gen
        meib    r0, r1
        meiw    r2, 10(sb)
        meid    r2, r0
# Divide: DIVi src/gen, dest/gen
        divb    r0, r1
        divw    10(sp), 4(sp)
        divd    -6(fp), 12(sb)
# Modulus: MODi src/gen, dest/gen
        modb    4(sb), 8(sb)
        modw    r2, r3
        modd    r4, r5
# Quotient: QUOi src/gen, dest/gen
        quob    r0, r7
        quow    4(sb), 8(sb)
        quod    r4, r5
# Reminder: REMi src/gen, dest/gen
        remb    4(sb), 8(sb)
        remw    r2, r3
        remd    r4, r5
# Divide Extended Integer: DEIi src/gen, dest/gen
        deib    r0, r1
        deiw    r2, r0
        deid    r4, r5
# Move: MOVi src/gen, dest/gen
        movb    r0, r1
        movw    r2, r3
        movd    r0, 8(sb)
# Move Quick: MOVQi src/quick, dest/gen
        movqb   0, r0
        movqw   7, tos
        movqd   -8, r2
# Move with Sign-Extension: MOVXii src/gen, dest/gen
        movxbw  2(sb), r0
        movxbd  r2, r3
        movxwd  r4, r5
# Move with Zero-Extension: MOVZii src/gen, dest/gen
        movzbw  -4(fp), r0
        movzbd  r2, r3
        movzwd  r4, r5
# Compare: CMPi src1/gen, src2/gen
        cmpb    7(sb), 4(r0)
        cmpw    r2, r3
        cmpd    r4, r5
# Compare Quick: CMPQi src1/quick, src2/gen
        cmpqb   -8, r0
        cmpqw   7, r1
        cmpqd   -8, r2
# Add Packed Decimal: ADDPi src/gen, dest/gen
        addpb   5(sb), tos
        addpw   r2, r3
        addpd   r0, r1
# Subtract Packed Decimal: SUBPi src/gen, dest/gen
        subpb   -8(fp), 16(fp)
        subpw   r2, r3
        subpd   0x00000099, r1
# Logical AND: ANDi src/gen, dest/gen
        andb    r0, r1
        andw    r2, r3
        andd    r4, r5
# Logical OR: ORi src/gen, dest/gen
        orb     -6(fp), 11(sb)
        orw     r2, r3
        ord     r4, r5
# Bit Clear: BICi src/gen, dest/gen
        bicb    r0, 3(sb)
        bicw    r2, r3
        bicd    r4, r5
# Exclusive OR: XORi src/gen, dest/gen
        xorb    -8(fp), -4(fp)
        xorw    r2, r3
        xord    r4, r5
# Complement: COMi src/gen, dest/gen
        comb    r0, -4(fp)
        comw    r2, r3
        comd    r4, r5
# Arithmetic Shift: ASHi count/gen, dest/gen
        ashb    2, 16(sb)
        ashw    tos, 16(sb)
        ashd    r4, r5
# Logical Shift: LSHi count/gen, dest/gen
        lshb    4, 8(sb)
        lshw    -4(fp), 8(sb)
        lshd    r4, r5
# Rotate: ROTi count/gen, dest/gen
        rotb    4, r5
        rotw    -3, 16(sp)
        rotd    r4, r5
# Complement Boolean: NOTi src/gen, dest/gen
        notb    r0, r0
        notw    10(r1), tos
        notd    r4, r5
# Save Condition as Boolean: Scci dest/gen
        seqb    r0
        sneb    r1
        scsb    r2
        sccb    r3
        shib    r4
        slsb    r5
        sgtb    r6
        sleb    r7
        sfsb    0(r0)
        sfcb    1(r1)
        slob    2(r2)
        shsb    3(r3)
        sltb    4(r4)
        sgeb    5(r5)
        seqw    6(r6)
        snew    7(r7)
        scsw    8(9(fp))
        sccw    10(11(sp))
        shiw    12(13(sb))
        slsw    @14
        sgtw    ext(15)+16
        slew    tos
        sfsw    18(fp)
        sfcw    19(sp)
        slow    10(sb)
        shsw    label0(pc)
        sltw    22(r0)[r7:b]
        sgew    23(r1)[r6:w]
        seqd    24(r2)[r5:d]
        sned    25(r3)[r4:q]
        scsd    -1(r5)
        sccd    -2(r6)
label0:
        shid    tos
        slsd    -4(-5(fp))
        sgtd    -6(-7(sp))
        sled    -8(-9(sb))
        sfsd    @0x1234
        sfcd    ext(-10)+(-11)
        slod    -12(fp)
        shsd    -13(sp)
        sltd    -14(sb)
        sged    label0(pc)
# Test Bit: TBITi offset/gen, base/gen
        tbitb   r0, r1
        tbitw   r0, 0(r1)
        tbitd   r4, r5
# Set Bit: SBITi offset/gen, base/gen
        sbitb   r0, r1
        sbitw   r0, 1(r1)
        sbitd   r4, r5
# Set Bit Interlocked offset/gen, base/gen
        sbitib  r0, r1
        sbitiw  r2, r3
        sbitid  r4, r5
# Clear Bit: CBITi offset/gen, base/gen
        cbitb   r0, r1
        cbitw   r0, 0(r1)
        cbitd   r4, r5
# Clear Bit Interlocked: CBITIi
        cbitib  r0, r1
        cbitiw  r2, r3
        cbitid  r4, r5
# Invert Bit: IBITi offset/gen, base/gen
        ibitb   r0, 1(r1)
        ibitw   r2, r3
        ibitd   r4, r5
# Find First Set Bit: FFSi base/gen, offset/gen
        ffsb    -4(fp), tos
        ffsw    8(sb), r0
        ffsd    r4, r5
# Convert to Bit Pointer: CVP offset/reg, base/gen, dest/gen
        cvtp    r0, 32(sb), r2
# Extract Field: EXTi offset/reg, base/gen, dest/gen, length/disp
        extb    r0, r1, r2, 20
        extw    r0, 0(r1), r2, 7
        extd    r6, r7, tos, 4
# Extract Field Short: EXTSi base/gen, dest/gen, offset/imm, length/imm
        extsb   r0, r1, 3, 4
        extsw   16(sb), r2, 4, 7
        extsd   r4, r5, 7, 8
# Insert Field: INSi offset/reg, src/gen, base/gen, length/disp
        insb    r0, r1, r2, 10
        insw    r0, r2, 0(r1), 7
        insd    r5, r6, tos, 16
# Insert Field Short: INSSi src/gen, base/gen, offset/imm, length/imm
        inssb   r0, r1, 0, 7
        inssw   r2, 16(sb), 4, 7
        inssd   r4, r5, 6, 8
# Move String: MOVSi options
        movsb   [w]
        movsw   [b,w]
        movsd   []
# Move String,  Translating: MOVST options
        movst   []
# Compare String: CMPSi options
        cmpsb   []
        cmpsw   [b,u]
        cmpsd   [u]
# Compare String,  Translating: CMPST options
        cmpst   []
# Skip String: SKPSi options
        skpsb   []
        skpsw   [b]
        skpsd   [u]
# Skip String,  Translating: SKPST options
        skpst   [w]
# Move Multiple: MOVMi block1/gen, block2/gen, length/disp
        movmb   r0, r1, 10
        movmw   10(r0), 16(r1), 4
        movmd   r4, r5, 4
# Compare Multiple: CMPMi block1/gen, block2/gen, length/disp
        cmpmb   r0, r1, 10
        cmpmw   10(r0), 16(r1), 4
        cmpmd   r4, r5, 4
# Bounds Check: CHECKi dest/reg, bounds/gen, src/gen
        checkb  r0, 4(sb), r2
        checkw  r3, r4, r5
        checkd  r6, r7, tos
# Calculate Index: INDEXi accum/reg, length/gen, index/gen
        indexb  r0, 20(sb), -4(fp)
        indexw  r2, r3, r4
        indexd  r5, r6, tos
# Jump: JUMP dest/gen
        jump     0(-8(fp))
# Conditional Branch: Bcc dest/disp
        beq     label1
        bne     label1
        bcs     label1
        bcc     label1
        bhi     label1
        bls     label1
        bgt     label1
label1:
        ble     label1
        bfs     label1
        bfc     label1
        blo     label0
        bhs     label1
        blt     label1
        bge     label1
# Unconditional Branch: BR dest/disp
        br      label2
        br      label0
# Case Branch: CASEi src/gen
        caseb   label2(pc)[r7:b]
        casew   r1
        cased   r2
label2:
# Add,  Compare and Branch: ACBi inc/quick, index/gen, dest/disp
        acbb    -1, r0, label2
        acbw    -2, r1, label2
        acbd    4, r2, label3
# Jump to Subroutine: JSR dest/gen
        jsr     0(4(sb))
# Branch to Subroutine: BSR dest/disp
        bsr     label3
# Return from Subroutine: RET constant/disp
        ret     16
# Call External Procedure: CXP index/disp
        cxp     1
label3:
# Call External Procedure with Descriptor: CXPD desc/gen
        cxpd    8(sb)
# Return from External Procedure: RXP constant/disp
        rxp     16
# Breakpoint Trap: BPT
        bpt
# Flag Trap: FLAG
        flag
# Supervisor Call Trap: SVC
        svc
# Return from Trap: RETT constant/disp
        rett    16
# Return from Interrupt: RETI
        reti
# Compute Effective Address: ADDR src/gen, dest/gen
        addr    4(fp), r0
# Save General Purpose RegistersL SAVE reglist/imm
        save    [r0,r2,r7]
# Restore General Purpose Registers: RESTORE reglist/imm
        restore [r0,r2,r7]
# Enter New Procedure Context: ENTER reglist/imm, constant/disp
        enter   [r0,r2,r7], 16
# Exit Procedure Context: EXIT reglist/imm
        exit    [r0,r2,r7]
# Adjust Stack Pointer: ADJSPi src/gen
        adjspb  r0
        adjspw  r1
        adjspd  -4(fp)
# Bit Clear in PSR: BICPSR[BW] src/gen
        bicpsrb 0xa2
        bicpsrw r1
# Bit Set in PSR: BISPSR[BW] src/gen
        bispsrb 0xa2
        bispsrw r1
# Load Processor Register: LPRi procreg/short, src/gen
        lprb    upsr, r0
        lprd    fp, r0
        lprd    sp, r6
        lprd    sb, r7
        lprw    psr, r2
        lprd    intbase, r3
        lprw    mod, 4(sb)
# Store Processor Register: SPRi procreg/short, dest/gen
        sprb    upsr, r5
        sprd    fp, r0
        sprd    sp, r6
        sprd    sb, r7
        sprw    psr, r2
        sprd    intbase, r3
        sprw    mod, 4(sb)
# Set Configuration Register: SETCFG cfglist/short
        setcfg  [i,f,m]
        setcfg  []
# No Operation: NOP
        nop
# Wait for Interrupt: WAIT
        wait
# Diagnose: DIA
        dia
# Move Value from Supervisor to User Space: MOVSUi src/gen, dest/gen
        movsub  5(sp), 9(sb)
        movsuw  r2, r3
        movsud  r4, r5
# Move Value from User to Supervisor Space: MOVUSi src/gen, dest/gen
        movusb  9(sb), 5(sp)
        movusw  r2, r3
        movusd  r4, r5
# Longest instruction
        extd    r1, 0x112233(0x334455(fp))[r3:w], 0x556677(0x778899(sb))[r4:w], 32
# Register
        addw    r1, r2
# Register Relative
        addw    4(r1), r2
        addw    4(r1), 32(r2)
        addw    r1, 32(r2)
# Memory Relative
        addw    6(4(fp)), r2
        addw    6(4(fp)), 16(32(sb))
        addw    r2, 16(32(sb))
# Immediate
        addb    0x56, r1
        addw    0x1234, r1
        addd    0x12345678, r1
# Absolute
        addw    @0x1234, 4(r2)
        addw    @0x1234, @0x5678
        addw    4(r2), @0x5678
# External
        addw    ext(2)+4, 10(r2)
        addw    ext(-2)+(-4), 10(r2)
        addw    ext(-2)+4, ext(-6)+8
        addw    ext(2)+(-4), ext(6)+(-8)
        addw    10(r2), ext(6)+8
        addw    10(r2), ext(-6)+(-8)
# Top of Stack
        addw    tos, 4(r2)
        addw    tos, tos
        addw    4(r2), tos
# Memory Space
        addw    4(fp), 6(r2)
        addw    4(sp), 6(r2)
        addw    4(sb), 6(r2)
        addw    label4(pc), 6(r2)
        addw    6(r2), 4(fp)
        addw    6(r2), 4(sp)
        addw    6(r2), 4(sb)
        addw    6(r2), label4(pc)
        addw    4(fp), 6(sb)
        addw    6(sb), 8(sp)
        addw    8(sp), label4(pc)
        addw    label4(pc), label3(pc)
# Scaled Indexing
        addw    r1[r3:b], 8(r2)
        addw    4(r1)[r3:b], 8(r2)
        addw    6(4(fp))[r3:b], 8(r2)
        addw    @0x1234[r3:b], 8(r2)
        addw    ext(4)+6[r3:b], 8(r2)
        addw    tos[r3:b], 8(r2)
        addw    4(sp)[r3:b], 8(r2)
        addw    label4(pc)[r3:b], 8(r2)
        addw    8(r2), r1[r3:w]
label4:
        addw    8(r2), 4(r1)[r3:w]
        addw    8(r2), 6(4(fp))[r3:w]
        addw    8(r2), @0x1234[r3:w]
        addw    8(r2), ext(4)+6[r3:w]
        addw    8(r2), tos[r3:w]
        addw    8(r2), 4(sp)[r3:w]
        addw    8(r2), label4(pc)[r3:w]
        addw    8(r2)[r3:d], r1[r5:w]
        addw    8(r2)[r3:d], 4(r1)[r5:w]
        addw    8(r2)[r3:d], 6(4(fp))[r5:w]
        addw    8(r2)[r3:d], @0x1234[r5:w]
        addw    8(r2)[r3:q], ext(4)+6[r5:w]
        addw    8(r2)[r3:q], tos[r5:w]
        addw    8(r2)[r3:q], 4(sp)[r5:w]
        addw    8(r2)[r3:q], label4(pc)[r5:w]

# Local Variables:
# mode: asm
# End:
# vim: set ft=asm:
