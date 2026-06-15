/*
 * Copyright 2026 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "asm_superh.h"
#include "dis_superh.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_sh1() {
    PREP_ASM(superh::AsmSuperH, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("SH-1",
            R"(        cpu   SH-1
; comment line
        org   H'1000
label1: equ   H'1100
        nop
        mov   r0, r1
        bra   label1
label2: nop
)",
            R"(          0 :                            cpu   SH-1
          0 :                    ; comment line
       1000 :                            org   H'1000
       1000 : =1100              label1: equ   H'1100
       1000 : 0009                       nop
       1002 : 6103                       mov   r0, r1
       1004 : a07c                       bra   label1
       1006 : 0009               label2: nop
)");
}

void test_dis_sh1() {
    PREP_DIS(superh::DisSuperH);

    driver.setUppercase(true);

    DIS16("SH-1", 0x1000,
            R"(      CPU    SH-1
      ORG    H'00001000
      NOP
      MOV    R0, R1
)",
            R"(       0 :                            CPU    SH-1
    1000 :                            ORG    H'00001000
    1000 : 0009                       NOP
    1002 : 6103                       MOV    R0, R1
)",
            0x0009, 0x6103);
}

void test_asm_sh2() {
    PREP_ASM(superh::AsmSuperH, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("SH-2",
            R"(        cpu   SH-2
        org   H'2000
        dt    r0
        braf  r0
        dmuls.l r0, r1
)",
            R"(          0 :                            cpu   SH-2
       2000 :                            org   H'2000
       2000 : 4010                       dt    r0
       2002 : 0023                       braf  r0
       2004 : 310d                       dmuls.l r0, r1
)");
}

void test_dis_sh2() {
    PREP_DIS(superh::DisSuperH);

    driver.setUppercase(true);

    DIS16("SH-2", 0x2000,
            R"(      CPU    SH-2
      ORG    H'00002000
      DT     R0
      BRAF   R0
      DMULS.L    R0, R1
)",
            R"(       0 :                            CPU    SH-2
    2000 :                            ORG    H'00002000
    2000 : 4010                       DT     R0
    2002 : 0023                       BRAF   R0
    2004 : 310D                       DMULS.L    R0, R1
)",
            0x4010, 0x0023, 0x310D);
}

void test_asm_shdsp() {
    PREP_ASM(superh::AsmSuperH, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("SH-DSP",
            R"(        cpu   SH-DSP
        org   H'3000
        lds   r0, dsr
        lds   r0, a0
        setrc #0
)",
            R"(          0 :                            cpu   SH-DSP
       3000 :                            org   H'3000
       3000 : 406a                       lds   r0, dsr
       3002 : 407a                       lds   r0, a0
       3004 : 8200                       setrc #0
)");
}

void test_dis_shdsp() {
    PREP_DIS(superh::DisSuperH);

    driver.setUppercase(true);

    DIS16("SH-DSP", 0x3000,
            R"(      CPU    SH-DSP
      ORG    H'00003000
      LDS    R0, DSR
      LDS    R0, A0
      SETRC  #0
)",
            R"(       0 :                            CPU    SH-DSP
    3000 :                            ORG    H'00003000
    3000 : 406A                       LDS    R0, DSR
    3002 : 407A                       LDS    R0, A0
    3004 : 8200                       SETRC  #0
)",
            0x406A, 0x407A, 0x8200);
}

void test_asm_sh2e() {
    PREP_ASM(superh::AsmSuperH, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("SH-2E",
            R"(        cpu   SH-2E
        org   H'4000
        fadd  fr0, fr1
        flds  fr0, fpul
        lds   r0, fpscr
)",
            R"(          0 :                            cpu   SH-2E
       4000 :                            org   H'4000
       4000 : f100                       fadd  fr0, fr1
       4002 : f01d                       flds  fr0, fpul
       4004 : 406a                       lds   r0, fpscr
)");
}

void test_dis_sh2e() {
    PREP_DIS(superh::DisSuperH);

    driver.setUppercase(true);

    DIS16("SH-2E", 0x4000,
            R"(      CPU    SH-2E
      ORG    H'00004000
      FADD   FR0, FR1
      FLDS   FR0, FPUL
      LDS    R0, FPSCR
)",
            R"(       0 :                            CPU    SH-2E
    4000 :                            ORG    H'00004000
    4000 : F100                       FADD   FR0, FR1
    4002 : F01D                       FLDS   FR0, FPUL
    4004 : 406A                       LDS    R0, FPSCR
)",
            0xF100, 0xF01D, 0x406A);
}

void test_asm_sh2a() {
    PREP_ASM(superh::AsmSuperH, MotorolaDirective);

    driver.setUpperHex(false);

    ASM("SH-2A",
            R"(        cpu   SH-2A
        org   H'5000
        movrt r0
        nott
        movi20 #0, r0
)",
            R"(          0 :                            cpu   SH-2A
       5000 :                            org   H'5000
       5000 : 0039                       movrt r0
       5002 : 0068                       nott
       5004 : 0000 0000                  movi20 #0, r0
)");
}

void test_dis_sh2a() {
    PREP_DIS(superh::DisSuperH);

    driver.setUppercase(true);

    DIS16("SH-2A", 0x5000,
            R"(      CPU    SH-2A
      ORG    H'00005000
      MOVRT  R0
      NOTT
      MOVI20 #0, R0
)",
            R"(       0 :                            CPU    SH-2A
    5000 :                            ORG    H'00005000
    5000 : 0039                       MOVRT  R0
    5002 : 0068                       NOTT
    5004 : 0000 0000                  MOVI20 #0, R0
)",
            0x0039, 0x0068, 0x0000, 0x0000);
}

void run_tests() {
    RUN_TEST(test_asm_sh1);
    RUN_TEST(test_dis_sh1);
    RUN_TEST(test_asm_sh2);
    RUN_TEST(test_dis_sh2);
    RUN_TEST(test_asm_shdsp);
    RUN_TEST(test_dis_shdsp);
    RUN_TEST(test_asm_sh2e);
    RUN_TEST(test_dis_sh2e);
    RUN_TEST(test_asm_sh2a);
    RUN_TEST(test_dis_sh2a);
}

}  // namespace test
}  // namespace driver
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
