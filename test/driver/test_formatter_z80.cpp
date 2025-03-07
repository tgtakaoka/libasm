/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "asm_z80.h"
#include "dis_z80.h"
#include "test_formatter_helper.h"

namespace libasm {
namespace driver {
namespace test {

void set_up() {}

void tear_down() {}

void test_asm_z80() {
    PREP_ASM(z80::AsmZ80, ZilogDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add(R"(        defw  1234H, 5678H, 9ABCH
        defb  'a,', 'bc''de', 0   ; DB requires surrounding quotes
        defs  2                   ; DS allocate spaces
data1:  equ   8AH
data2:  defm  'A', '''', 'C'+80H, 'a''c'
)");

    driver.setUpperHex(false);

    ASM("z80",
            R"(        cpu   z80
; comment line
        org   0abcdh
        include "data/db.inc"
        res   0, (iy-128)
        set   7, (ix+127)
        ld    A, low data2
)",
            R"(          0 :                            cpu   z80
          0 :                    ; comment line
       abcd :                            org   0abcdh
       abcd :                            include "data/db.inc"
(1)    abcd : 34 12 78 56 bc 9a          defw  1234H, 5678H, 9ABCH
(1)    abd3 : 61 2c 62 63 27 64          defb  'a,', 'bc''de', 0   ; DB requires surrounding quotes
       abd9 : 65 00
(1)    abdb :                            defs  2                   ; DS allocate spaces
(1)    abdd : =8a                data1:  equ   8AH
(1)    abdd : 41 27 c3 61 27 63  data2:  defm  'A', '''', 'C'+80H, 'a''c'
       abe3 : fd cb 80 86                res   0, (iy-128)
       abe7 : dd cb 7f fe                set   7, (ix+127)
       abeb : 3e dd                      ld    A, low data2
)");
}

void test_dis_z80() {
    PREP_DIS(z80::DisZ80);

    DIS8("z80", 0xabcd,
            R"(      cpu   z80
      org   0ABCDH
      res   0, (iy-128)
; test.bin: error: Unknown instruction
;     ABD1 : DD CB 00 EF
)",
            R"(       0 :                            cpu   z80
    ABCD :                            org   0ABCDH
    ABCD : FD CB 80 86                res   0, (iy-128)
test.bin: error: Unknown instruction
    ABD1 : DD CB 00 EF
)",
            0xfd, 0xcb, 0x80, 0x86, 0xdd, 0xcb, 0x00, 0xef);
}

void test_asm_z180() {
    PREP_ASM(z80::AsmZ80, ZilogDirective);

    TestReader inc("data/db.inc");
    sources.add(inc);
    inc.add(R"(        defw  1234H, 5678H, 9ABCH
        defb  'a,', 'bc''de', 0   ; DB requires surrounding quotes
        defs  2                   ; DS allocate spaces
data1:  equ   8AH
data2:  defm  'A', '''', 'C'+80H, 'a''c'
)");

    driver.setUpperHex(false);

    ASM("z180",
            R"(        cpu   z180
; comment line
        org   0abcdeh
        include "data/db.inc"
        res   0, (iy-128)
        set   7, (ix+127)
        ld    A, low data2
)",
            R"(          0 :                            cpu   z180
          0 :                    ; comment line
      abcde :                            org   0abcdeh
      abcde :                            include "data/db.inc"
(1)   abcde : 34 12 78 56 bc 9a          defw  1234H, 5678H, 9ABCH
(1)   abce4 : 61 2c 62 63 27 64          defb  'a,', 'bc''de', 0   ; DB requires surrounding quotes
      abcea : 65 00
(1)   abcec :                            defs  2                   ; DS allocate spaces
(1)   abcee : =8a                data1:  equ   8AH
(1)   abcee : 41 27 c3 61 27 63  data2:  defm  'A', '''', 'C'+80H, 'a''c'
      abcf4 : fd cb 80 86                res   0, (iy-128)
      abcf8 : dd cb 7f fe                set   7, (ix+127)
      abcfc : 3e ee                      ld    A, low data2
)");
}

void test_dis_z180() {
    PREP_DIS(z80::DisZ80);

    DIS8("z180", 0xabcde,
            R"(      cpu   z180
      org   0ABCDEH
      res   0, (iy-128)
; test.bin: error: Unknown instruction
;    ABCE2 : DD CB 00 EF
)",
            R"(       0 :                            cpu   z180
   ABCDE :                            org   0ABCDEH
   ABCDE : FD CB 80 86                res   0, (iy-128)
test.bin: error: Unknown instruction
   ABCE2 : DD CB 00 EF
)",
            0xfd, 0xcb, 0x80, 0x86, 0xdd, 0xcb, 0x00, 0xef);
}

void run_tests() {
    RUN_TEST(test_asm_z80);
    RUN_TEST(test_dis_z80);
    RUN_TEST(test_asm_z180);
    RUN_TEST(test_dis_z180);
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
