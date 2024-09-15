/*
 * Copyright 2024 Tadashi G. Takaoka
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

#include "fixed64.h"
#include "test_unit_helper.h"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_ctor() {
    fixed64_t v;
    EQ("0", "0000-00000000-00000000-0000", v.str());
    TRUE("0", v.zero());
    EQ("0", 0, v.value());

    EQ("1", "0000-00000000-00000001-0000", v.set(1).str());
    FALSE("1", v.negative());
    EQ("1", 1, v.value());
    EQ("1", "0000-00000000-00000000-0001", v.set(0, 1).str());

    v.negate();
    EQ("-1", "FFFF-FFFFFFFF-FFFFFFFF-FFFF", v.str());
    TRUE("-1", v.negative());
    EQ("-1", UINT64_MAX, v.value());

    EQ("UINT64_MAX", "0000-FFFFFFFF-FFFFFFFF-0000", v.set(UINT64_MAX).str());
    FALSE("UINT64_MAX", v.negative());
    EQ("UINT64_MAX", UINT64_MAX, v.value());

    v.negate();
    EQ("-UINT64_MAX", "FFFF-00000000-00000001-0000", v.str());
    TRUE("-UINT64_MAX", v.negative());
    EQ("-UINT64_MAX", 1, v.value());
}

void test_shift() {
    fixed64_t v{UINT64_C(0x0003000400050007)};
    EQ("3457", "0000-00030004-00050007-0000", v.str());
    v.shift_right(1);
    EQ("3457", "0000-00018002-00028003-8000", v.str());
    v.shift_right(35);
    EQ("3457", "0000-00000000-00003000-4001", v.str());  // sticky bit
    v.shift_left(4);
    EQ("3457", "0000-00000000-00030004-0010", v.str());
    v.shift_left(48);
    EQ("3457", "0003-00040010-00000000-0000", v.str());
    v.shift_left(14);
    EQ("3457", "C001-00040000-00000000-0000", v.str());
    v.shift_right(16, true);
    EQ("3457", "FFFF-C0010004-00000000-0000", v.str());  // arithmetic

    v.set(UINT64_C(0x8000000000000000));
    EQ("8000", "0000-80000000-00000000-0000", v.str());
    v.shift_right(79);
    EQ("8000", "0000-00000000-00000000-0001", v.str()); // sticky

    v.set(UINT64_C(0x8000000000000000), UINT16_C(0x0001));
    EQ("8000", "0000-80000000-00000000-0001", v.str());
    v.shift_left(16);
    EQ("8000", "8000-00000000-00000001-0000", v.str());
    v.shift_right(17);
    EQ("8000", "0000-40000000-00000000-0001", v.str());  // logical, sticky
}

void test_normalize() {
    fixed64_t n{UINT64_C(0x0003000400050007)};
    FALSE("3457", n.normal());
    EQ("3457", "0000-00030004-00050007-0000", n.str());
    EQ("3457", -14, n.normalize());
    TRUE("3457", n.normal());
    EQ("3467", "0000-C0010001-4001C000-0000", n.str());

    n.shift_left(2);
    EQ("3457", "0003-00040005-00070000-0000", n.str());
    EQ("3457", 2, n.normalize());
    TRUE("3457", n.normal());
    EQ("3457", "0000-C0010001-4001C000-0000", n.str());

    fixed64_t zero{0};
    EQ("1", 0, zero.normalize());
    EQ("1", "0000-00000000-00000000-0000", zero.str());

    fixed64_t one{1};
    EQ("1", 0, 63 + one.normalize());
    EQ("1", "0000-80000000-00000000-0000", one.str());

    fixed64_t three{3};
    EQ("3", 1, 63 + three.normalize());
    EQ("3", "0000-C0000000-00000000-0000", three.str());

    fixed64_t seven{7};
    EQ("7", 2, 63 + seven.normalize());
    EQ("7", "0000-E0000000-00000000-0000", seven.str());

    fixed64_t nine{9};
    EQ("9", 3, 63 + nine.normalize());
    EQ("9", "0000-90000000-00000000-0000", nine.str());
}

void test_round53() {
    fixed64_t e3f{UINT64_C(0x3FF), UINT16_C(0xFFFF)};
    EQ("e3f", "0000-00000000-000003FF-FFFF", e3f.str());
    FALSE("e3f", e3f.round_off(53));
    EQ("e3f", "0000-00000000-00000000-0000", e3f.str());

    fixed64_t e40{UINT64_C(0x400), UINT16_C(0x0000)};
    EQ("e40", "0000-00000000-00000400-0000", e40.str());
    FALSE("e40", e40.round_off(53));
    EQ("e40", "0000-00000000-00000000-0000", e40.str());  // nearest even

    fixed64_t e41{UINT64_C(0x400), UINT16_C(0x0001)};
    EQ("e41", "0000-00000000-00000400-0001", e41.str());
    FALSE("e41", e41.round_off(53));
    EQ("e41", "0000-00000000-00000800-0000", e41.str());

    fixed64_t o3f{UINT64_C(0x7FF), UINT16_C(0xFFFF)};
    EQ("o3f", "0000-00000000-000007FF-FFFF", o3f.str());
    FALSE("o3f", o3f.round_off(53));
    EQ("o3f", "0000-00000000-00000800-0000", o3f.str());

    fixed64_t o40{UINT64_C(0xC00), UINT16_C(0x0000)};
    EQ("o40", "0000-00000000-00000C00-0000", o40.str());
    FALSE("o40", o40.round_off(53));
    EQ("o40", "0000-00000000-00001000-0000", o40.str());

    fixed64_t o41{UINT64_C(0xC00), UINT16_C(0x0001)};
    EQ("o41", "0000-00000000-00000C00-0001", o41.str());
    FALSE("o41", o41.round_off(53));
    EQ("o41", "0000-00000000-00001000-0000", o41.str());

    fixed64_t max3f{UINT64_C(0xFFFFFFFFFFFFF3FF), UINT16_C(0xFFFF)};
    EQ("max3f", "0000-FFFFFFFF-FFFFF3FF-FFFF", max3f.str());
    FALSE("max3f", max3f.round_off(53));
    EQ("max3f", "0000-FFFFFFFF-FFFFF000-0000", max3f.str());

    fixed64_t max40{UINT64_C(0xFFFFFFFFFFFFFC00), UINT16_C(0x0000)};
    EQ("max40", "0000-FFFFFFFF-FFFFFC00-0000", max40.str());
    TRUE("max40", max40.round_off(53));
    EQ("max40", "0001-00000000-00000000-0000", max40.str());

    fixed64_t sub3f{UINT64_C(0x7FFFFFFFFFFFF3FF), UINT16_C(0xFFFF)};
    EQ("sub3f", "0000-7FFFFFFF-FFFFF3FF-FFFF", sub3f.str());
    FALSE("sub3f", sub3f.normal());
    FALSE("sub3f", sub3f.round_off(53));
    EQ("sub3f", "0000-7FFFFFFF-FFFFF000-0000", sub3f.str());
    FALSE("sub3f", sub3f.normal());

    fixed64_t sub40{UINT64_C(0x7FFFFFFFFFFFFC00), UINT16_C(0x0000)};
    EQ("sub40", "0000-7FFFFFFF-FFFFFC00-0000", sub40.str());
    FALSE("sub40", sub40.normal());
    FALSE("sub40", sub40.round_off(53));
    EQ("sub40", "0000-80000000-00000000-0000", sub40.str());
    TRUE("sub40", sub40.normal());
}

void test_round64() {
    fixed64_t e7f{UINT64_C(0), UINT16_C(0x7FFF)};
    EQ("e7f", "0000-00000000-00000000-7FFF", e7f.str());
    FALSE("e7f", e7f.round_off(64));
    EQ("e7f", "0000-00000000-00000000-0000", e7f.str());

    fixed64_t e80{UINT64_C(0), UINT16_C(0x8000)};
    EQ("e80", "0000-00000000-00000000-8000", e80.str());
    FALSE("e80", e80.round_off(64));
    EQ("e80", "0000-00000000-00000000-0000", e80.str());  // nearest even

    fixed64_t e81{UINT64_C(0), UINT16_C(0x8001)};
    EQ("e81", "0000-00000000-00000000-8001", e81.str());
    FALSE("e81", e81.round_off(64));
    EQ("e81", "0000-00000000-00000001-0000", e81.str());

    fixed64_t o7f{UINT64_C(1), UINT16_C(0x7FFF)};
    EQ("o7f", "0000-00000000-00000001-7FFF", o7f.str());
    FALSE("o7f", o7f.round_off(64));
    EQ("o7f", "0000-00000000-00000001-0000", o7f.str());

    fixed64_t o80{UINT64_C(1), UINT16_C(0x8000)};
    EQ("o80", "0000-00000000-00000001-8000", o80.str());
    FALSE("o80", o80.round_off(64));
    EQ("o80", "0000-00000000-00000002-0000", o80.str());

    fixed64_t o81{UINT64_C(1), UINT16_C(0x8001)};
    EQ("o81", "0000-00000000-00000001-8001", o81.str());
    FALSE("o81", o81.round_off(64));
    EQ("o81", "0000-00000000-00000002-0000", o81.str());

    fixed64_t max7f{UINT64_C(0xFFFFFFFFFFFFFFFF), UINT16_C(0x7FFF)};
    EQ("max7f", "0000-FFFFFFFF-FFFFFFFF-7FFF", max7f.str());
    FALSE("max7f", max7f.round_off(64));
    EQ("max7f", "0000-FFFFFFFF-FFFFFFFF-0000", max7f.str());

    fixed64_t max80{UINT64_C(0xFFFFFFFFFFFFFFFF), UINT16_C(0x8000)};
    EQ("max80", "0000-FFFFFFFF-FFFFFFFF-8000", max80.str());
    TRUE("max80", max80.round_off(64));
    EQ("max80", "0001-00000000-00000000-0000", max80.str());

    fixed64_t sub7f{UINT64_C(0x7FFFFFFFFFFFFFFF), UINT16_C(0x7FFF)};
    EQ("sub7f", "0000-7FFFFFFF-FFFFFFFF-7FFF", sub7f.str());
    FALSE("sub7f", sub7f.normal());
    FALSE("sub7f", sub7f.round_off(64));
    EQ("sub7f", "0000-7FFFFFFF-FFFFFFFF-0000", sub7f.str());
    FALSE("sub7f", sub7f.normal());

    fixed64_t sub80{UINT64_C(0x7FFFFFFFFFFFFFFF), UINT16_C(0x8000)};
    EQ("sub80", "0000-7FFFFFFF-FFFFFFFF-8000", sub80.str());
    FALSE("sub80", sub80.normal());
    FALSE("sub80", sub80.round_off(64));
    EQ("sub80", "0000-80000000-00000000-0000", sub80.str());
    TRUE("sub80", sub80.normal());
}

void test_compare() {
    fixed64_t zero;
    fixed64_t one{UINT64_C(0x8000000000000000)};
    fixed64_t ten{UINT64_C(0xA000000000000000)};
    TRUE("0==0", zero == zero);
    TRUE("1==1", one == one);
    TRUE("10==10", ten == ten);
    TRUE("0<=0", zero <= zero);
    TRUE("1<=1", one <= one);
    TRUE("10<=10", ten <= ten);
    FALSE("0<0", zero < zero);
    FALSE("1<1", one < one);
    FALSE("10<10", ten < ten);

    fixed64_t one1{UINT64_C(0x8000000000000000), UINT16_C(0x0001)};
    fixed64_t ten1{UINT64_C(0xA000000000000000), UINT16_C(0x0001)};
    FALSE("1==1.1", one == one1);
    FALSE("10.1==10", ten1 == ten);
    TRUE("1<=1.1", one <= one1);
    FALSE("1.1<=1", one1 <= one);
    TRUE("10<=10.1", ten <= ten1);
    FALSE("10.1<=10", ten1 <= ten);
    TRUE("1<1.1", one < one1);
    TRUE("10<10.1", ten < ten1);
}

void test_add() {
    const fixed64_t one{UINT64_C(0x8000000000000000)};
    EQ("1", "0000-80000000-00000000-0000", one.str());
    auto v = one + one;
    EQ("1+1", "0001-00000000-00000000-0000", v.str());
    EQ("1+1", 1, v.normalize());
    EQ("1+1", "0000-80000000-00000000-0000", v.str());

    v = one - one;
    EQ("1-1", "0000-00000000-00000000-0000", v.str());

    const fixed64_t max{UINT64_MAX};
    EQ("max", "0000-FFFFFFFF-FFFFFFFF-0000", max.str());
    v = max + max;
    EQ("max+max", "0001-FFFFFFFF-FFFFFFFE-0000", v.str());
    EQ("max+max", 1, v.normalize());
    EQ("max+max", "0000-FFFFFFFF-FFFFFFFF-0000", v.str());

    EQ("-max", "FFFF-00000000-00000001-0000", (-max).str());
    v = -max - max;
    EQ("-max-max", "FFFE-00000000-00000002-0000", v.str());
    v.negate();
    EQ("-(-max-max)", "0001-FFFFFFFF-FFFFFFFE-0000", v.str());

    v = max - max;
    EQ("max-max", "0000-00000000-00000000-0000", v.str());
}

void test_mul() {
    const fixed64_t one{UINT64_C(0x8000000000000000)};
    auto v = one * one;
    EQ("1*1", "0000-80000000-00000000-0000", v.str());

    const fixed64_t three{UINT64_C(0xC000000000000000)};
    EQ("3", "0000-C0000000-00000000-0000", three.str());
    v = three * three;
    EQ("3*3", "0001-20000000-00000000-0000", v.str());
    EQ("3*3", 1, v.normalize());
    EQ("3*3", "0000-90000000-00000000-0000", v.str());  // 9.0

    const fixed64_t max{UINT64_MAX};
    EQ("max*max", "0001-FFFFFFFF-FFFFFFFC-0000", (max * max).str());

    const fixed64_t x{UINT64_C(0x89ABCDEF01234567)};  // 1.0755555550588501824
    const fixed64_t y{UINT64_C(0x876543210FEDCBA9)};  // 1.0577777778088218635
    EQ("x*y", "0000-91A01CF6-0F023087-B2CB", (x * y).str());
}

void test_div() {
    const fixed64_t one{UINT64_C(0x8000000000000000)};
    const fixed64_t three{UINT64_C(0xC000000000000000)};
    EQ("3", "0000-C0000000-00000000-0000", three.str());
    EQ("1/3", "0000-55555555-55555555-5555", (one / three).str());

    const fixed64_t ten{UINT64_C(0xA000000000000000)};
    EQ("ten", "0000-A0000000-00000000-0000", ten.str());
    auto one_10 = one / ten;
    EQ("1/10", "0000-66666666-66666666-6666", one_10.str());
    EQ("1/10", -1, one_10.normalize());
    EQ("1/10", "0000-CCCCCCCC-CCCCCCCC-CCCC", one_10.str());  // 0.1...
    FALSE("1/10", one_10.round_off(64));
    EQ("1/10", "0000-CCCCCCCC-CCCCCCCD-0000", one_10.str());  // 0.1...

    const fixed64_t hundred{UINT64_C(0xC800000000000000)};
    auto one_100 = one / hundred;
    EQ("hundred", "0000-C8000000-00000000-0000", hundred.str());
    EQ("1/100", "0000-51EB851E-B851EB85-1EB8", one_100.str());
    EQ("1/100", -1, one_100.normalize());
    EQ("1/100", "0000-A3D70A3D-70A3D70A-3D70", one_100.str());  // 0.01...
    FALSE("1/100", one_100.round_off(64));
    EQ("1/100", "0000-A3D70A3D-70A3D70A-0000", one_100.str());  // 0.01...
}

void test_power10() {
    const fixed64_t one{UINT64_C(0x8000000000000000)};
    auto e0 = one;
    EQ("e0", 0, e0.power10(0));
    EQ("e0", "0000-80000000-00000000-0000", e0.str());

    auto p4095 = one;
    EQ("e+4095", 13603, p4095.power10(4095));
    EQ("e+4095", "0000-9D19DB35-3B4D4615-6DD9", p4095.str());
    auto p4096 = one;
    EQ("e+4096", 13606, p4096.power10(4096));
    EQ("e+4096", "0000-C4605202-8A20979A-C94C", p4096.str());
    auto p4097 = one;
    EQ("e+4097", 13609, p4097.power10(4097));
    EQ("e+4097", "0000-F5786683-2CA8BD81-7B9F", p4097.str());

    auto m4095 = one;
    EQ("e-4095", -13604, m4095.power10(-4095));
    EQ("e-4095", "0000-D09445FB-078247D5-B95A", m4095.str());
    auto m4096 = one;
    EQ("e-4096", -13607, m4096.power10(-4096));
    EQ("e-4096", "0000-A6DD04C8-D2CE9FDE-2DE2", m4096.str());
    auto m4097 = one;
    EQ("e-4097", -13610, m4097.power10(-4097));
    EQ("e-4097", "0000-857D9D6D-7572197E-8B1B", m4097.str());
}

void run_tests() {
    RUN_TEST(test_ctor);
    RUN_TEST(test_shift);
    RUN_TEST(test_normalize);
    RUN_TEST(test_round53);
    RUN_TEST(test_round64);
    RUN_TEST(test_compare);
    RUN_TEST(test_add);
    RUN_TEST(test_mul);
    RUN_TEST(test_div);
    RUN_TEST(test_power10);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
