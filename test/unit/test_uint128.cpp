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

#include "test_unit_helper.h"
#include "uint128.h"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

void test_constructor() {
    uint128 zero{0};
    EQ("0", "00000000-00000000-00000000-00000000", zero.str());
    TRUE("0", zero.zero());
    EQ("0", 0, static_cast<uint64_t>(zero));

    EQ("-0", "00000000-00000000-00000000-00000000", (-zero).str());
    TRUE("-0", (-zero).zero());
    EQ("-0", 0, static_cast<uint64_t>(-zero));

    uint128 one{1};
    EQ("1", "00000000-00000000-00000001-00000000", one.str());
    FALSE("-1", one.negative());
    EQ("1", 1, static_cast<uint64_t>(one));

    EQ("-1", "FFFFFFFF-FFFFFFFF-FFFFFFFF-00000000", (-one).str());
    TRUE("-1", (-one).negative());
    EQ("-1", 1, static_cast<uint64_t>(-one));

    uint128 imax{UINT64_C(0x8000'0000'0000'0000)};
    EQ("imax", "00000000-80000000-00000000-00000000", imax.str());
    FALSE("-imax", imax.negative());
    EQ("imax", 0x8000'0000'0000'0000, static_cast<uint64_t>(imax));

    EQ("-imax", "FFFFFFFF-80000000-00000000-00000000", (-imax).str());
    TRUE("-imax", (-imax).negative());
    EQ("-imax", 0x8000'0000'0000'0000, static_cast<uint64_t>(-imax));

    uint128 umax{UINT64_MAX};
    EQ("umax", "00000000-FFFFFFFF-FFFFFFFF-00000000", umax.str());
    FALSE("umax", umax.negative());
    EQ("umax", UINT64_MAX, static_cast<uint64_t>(umax));

    EQ("-umax", "FFFFFFFF-00000000-00000001-00000000", (-umax).str());
    TRUE("-umax", (-umax).negative());
    EQ("-umax", UINT64_MAX, static_cast<uint64_t>(-umax));

    const auto m1 = -one;
    EQ("-1", "FFFFFFFF-FFFFFFFF-FFFFFFFF-00000000", m1.str());
    TRUE("-1", m1.negative());
    EQ("-1", 1, static_cast<uint64_t>(m1));

    EQ("1", "00000000-00000000-00000001-00000000", (-m1).str());
    FALSE("", (-m1).negative());
    EQ("1", 1, static_cast<uint64_t>(-m1));
}

void test_shift() {
    uint128 v{UINT64_C(0x0003'0004'0005'0007)};
    EQ("3457", "00000000-00030004-00050007-00000000", v.str());
    v.shr(1);
    EQ("3457", "00000000-00018002-00028003-80000000", v.str());
    v.shr(35);
    EQ("sticky", "00000000-00000000-00003000-40005001", v.str());
    v.shl(4);
    EQ("3457", "00000000-00000000-00030004-00050010", v.str());
    v.shl(64);
    EQ("3457", "00030004-00050010-00000000-00000000", v.str());
    v.shl(14);
    EQ("3457", "C0010001-40040000-00000000-00000000", v.str());
    v.shr(32);
    EQ("3457", "FFFFFFFF-C0010001-40040000-00000000", v.str());

    uint128 n{UINT64_C(0x0003'0004'0005'0007)};
    EQ("3457", "00000000-00030004-00050007-00000000", n.str());
    FALSE("3457", n.overflow());
    EQ("normalize", 14, n.normalize());
    EQ("normalize", "00000000-C0010001-4001C000-00000000", n.str());
    FALSE("normalize", n.overflow());

    n.shl(2);
    EQ("overflow", "00000003-00040005-00070000-00000000", n.str());
    TRUE("overlfow", n.overflow());
    EQ("overflow", -2, n.normalize());
    EQ("overflow", "00000000-C0010001-4001C000-00000000", n.str());
    FALSE("overflow", n.overflow());

    uint128 x{UINT64_C(0x0000'0000'1234'5678)};
    EQ("x", "00000000-00000000-12345678-00000000", x.str());
    x.shr(36);
    EQ("x", "00000000-00000000-00000000-01234567", x.str());
    FALSE("x", x.zero());
    EQ("x", 71, x.normalize());
    EQ("x", "00000000-91A2B380-00000000-00000000", x.str());
}

void test_add() {
    const uint128 one{UINT64_C(0x8000'0000'0000'0000)};
    const uint128 max{UINT64_MAX};

    EQ("1", "00000000-80000000-00000000-00000000", one.str());
    auto v = one + one;
    EQ("1+1", "00000001-00000000-00000000-00000000", v.str());
    EQ("1+1", -1, v.normalize());
    EQ("1+1", "00000000-80000000-00000000-00000000", v.str());

    v = one - one;
    EQ("1-1", "00000000-00000000-00000000-00000000", v.str());

    EQ("max", "00000000-FFFFFFFF-FFFFFFFF-00000000", max.str());
    v = max + max;
    EQ("max+max", "00000001-FFFFFFFF-FFFFFFFE-00000000", v.str());
    EQ("max+max", -1, v.normalize());
    EQ("max+max", "00000000-FFFFFFFF-FFFFFFFF-00000000", v.str());

    EQ("-max", "FFFFFFFF-00000000-00000001-00000000", (-max).str());
    v = -max - max;
    EQ("-max-max", "FFFFFFFE-00000000-00000002-00000000", v.str());

    v = max - max;
    EQ("max-max", "00000000-00000000-00000000-00000000", v.str());
}

void test_mul() {
    const uint128 one{UINT64_C(0x8000'0000'0000'0000)};  // 1.0
    auto v = one * one;
    EQ("1*1", "00000000-80000000-00000000-00000000", v.str());

    const uint128 three{UINT64_C(0xC000'0000'0000'0000)};  // 3.0
    v = three * three;
    EQ("3*3", "00000001-20000000-00000000-00000000", v.str());
    EQ("3*3", -1, v.normalize());
    EQ("3*3", "00000000-90000000-00000000-00000000", v.str()); // 9.0

    const uint128 max{UINT64_MAX};
    EQ("max*max", "00000001-FFFFFFFF-FFFFFFFC-00000001", (max * max).str());

    const uint128 x{UINT64_C(0x89AB'CDEF'0123'4567)};  // 1.0755555550588501824
    const uint128 y{UINT64_C(0x8765'4321'0FED'CBA9)};  // 1.0577777778088218635
    EQ("x*y", "00000000-91A01CF6-0F023087-B2CAFD5D", (x * y).str());
}

void test_div() {
    const uint128 one{UINT64_C(0x8000'0000'0000'0000)};    // 1.0
    const uint128 three{UINT64_C(0xC000'0000'0000'0000)};  // 3.0
    EQ("1/3", "00000000-55555555-55555555-55555555", (one / three).str());

    const uint128 ten = {UINT64_C(0xA000'0000'0000'0000)};
    auto one_10 = one / ten;
    EQ("1/10", "00000000-66666666-66666666-66666666", one_10.str());
    EQ("1/10", 1, one_10.normalize());
    EQ("1/10", "00000000-CCCCCCCC-CCCCCCCC-CCCCCCCC", one_10.str()); // 0.1...

    const uint128 hundred{UINT64_C(0xC800'0000'0000'0000)};
    auto one_100 = one / hundred;
    EQ("1/100", "00000000-51EB851E-B851EB85-1EB851EB", one_100.str());
    EQ("1/100", 1, one_100.normalize());
    EQ("1/100", "00000000-A3D70A3D-70A3D70A-3D70A3D6", one_100.str()); // 0.01...
}

void test_power10() {
    const uint128 one{UINT64_C(0x8000'0000'0000'0000)};
    auto e0 = one;
    EQ("e0", 0, e0.power10(0));
    EQ("e0", "00000000-80000000-00000000-00000000", e0.str());
    auto e4096 = one;
    EQ("e4096", 13606, e4096.power10(4096));
    EQ("e4096", "00000000-C4605202-8A20979A-C94C1540", e4096.str());
    auto e4097 = one;
    EQ("e4097", 13609, e4097.power10(4097));
    EQ("e4097", "00000000-F5786683-2CA8BD81-7B9F1A90", e4097.str());
}

void run_tests() {
    RUN_TEST(test_constructor);
    RUN_TEST(test_shift);
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
