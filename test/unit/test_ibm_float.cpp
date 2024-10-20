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

#include <float.h>
#include "ibm_float.h"
#include "test_unit_helper.h"

#define __STR__(v) #v
#define STR(v) __STR__(v)
#define STR_DBL_MAX "1.79769313486231570814527423731704357e+308"
#define STR_DBL_MIN "2.22507385850720138309023271733240406e-308"

namespace libasm {
namespace test {

void set_up() {}

void tear_down() {}

float80_t f80;

const float80_t &read_f80(const char *str) {
    char *end;
    f80.read(str, &end);
    return f80;
}

const char *gcvt(const ibm_float32_t &f32) {
    static char buf[80];
    f80 = static_cast<float80_t>(f32);
    f80.gcvt(buf, sizeof(buf), 7);
    return buf;
}

void test_ibm_float32() {
    constexpr auto SGN_MASK = ibm_float32_t::SGN_MASK;
    constexpr auto EXP_MASK = ibm_float32_t::EXP_MASK;
    constexpr auto EXP_BASE = ibm_float32_t::EXP_BASE;
    constexpr auto SIG_MASK = ibm_float32_t::SIG_MASK;
    constexpr auto MSD_POS = ibm_float32_t::MSD_POS;
    constexpr auto MSD2_POS = MSD_POS - 4;
    constexpr auto MSD3_POS = MSD2_POS - 4;
#define EXP16(exp16) (exp16 + EXP_BASE)
#define SIG32(sig, pos) (UINT32_C(sig) << pos)
    ibm_float32_t f32;
    EQ("0.0", "00.000000", f32.set(0, 0).str());
    EQ("0.0", "0", gcvt(f32));
    EQ("0.0", OK, f32.set(f80));
    EQ("0.0", "00.000000", f32.str());

    EQ("1.0", "41.100000", f32.set(EXP16(1), SIG32(1, MSD_POS)).str());
    EQ("1.0", "1", gcvt(f32));
    EQ("1.0", OK, f32.set(f80));
    EQ("1.0", "41.100000", f32.str());

    EQ("-1.0", "C1.100000", f32.set(EXP16(1) + SGN_MASK, SIG32(1, MSD_POS)).str());
    EQ("-1.0", "-1", gcvt(f32));
    EQ("-1.0", OK, f32.set(f80));
    EQ("-1.0", "C1.100000", f32.str());

    EQ("IBMF32_MAX", "7F.FFFFFF", f32.set(EXP_MASK, SIG_MASK).str());
    EQ("IBMF32_MAX", STR(IBMF32_MAX), gcvt(f32));
    EQ("IBMF32_MAX", OK, f32.set(f80));
    EQ("IBMF32_MAX", "7F.FFFFFF", f32.str());

    EQ("IBMF32_MIN", "00.100000", f32.set(0, SIG32(1, MSD_POS)).str());
    EQ("IBMF32_MIN", STR(IBMF32_MIN), gcvt(f32));
    EQ("IBMF32_MIN", OK, f32.set(f80));
    EQ("IBMF32_MIN", "00.100000", f32.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f32.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MIN", OK, f32.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f32));

    EQ("1.0", "1", gcvt(f32.set(EXP16(1), SIG32(1, MSD_POS))));                 // 1/16 * 16^1
    EQ("2.0", "2", gcvt(f32.set(EXP16(1), SIG32(2, MSD_POS))));                 // 2/16 * 16^1
    EQ("4.0", "4", gcvt(f32.set(EXP16(1), SIG32(4, MSD_POS))));                 // 4/16 * 16^1
    EQ("8.0", "8", gcvt(f32.set(EXP16(1), SIG32(8, MSD_POS))));                 // 8/16 * 16^1
    EQ("16.0", "16", gcvt(f32.set(EXP16(2), SIG32(1, MSD_POS))));               // 1/16 * 16^2
    EQ("24.0", "24", gcvt(f32.set(EXP16(2), SIG32(24, MSD2_POS))));             // 24/16^2 * 16^2
    EQ("42.0", "42", gcvt(f32.set(EXP16(2), SIG32(42, MSD2_POS))));             // 42/16^2 * 16^2
    EQ("0.5", "0.5", gcvt(f32.set(EXP16(0), SIG32(8, MSD_POS))));               // 8/16 * 16^0
    EQ("0.125", "0.125", gcvt(f32.set(EXP16(0), SIG32(2, MSD_POS))));           // 2/16 * 16^0
    EQ("0.0078125", "0.0078125", gcvt(f32.set(EXP16(-1), SIG32(2, MSD_POS))));  // 2/16^2 * 16^-1
    EQ("1000.0", "1000", gcvt(f32.set(EXP16(3), SIG32(1000, MSD3_POS))));       // 1000/16^3 * 16^3
    EQ("118.625", "118.625", gcvt(f32.set(EXP16(2), SIG32(1898, MSD3_POS))));   // 1898/16 * 16^2
    EQ("101.0", "101", gcvt(f32.set(EXP16(2), SIG32(101, MSD2_POS))));          // 101/16^2 * 16^2
    EQ("0.03125", "0.03125", gcvt(f32.set(EXP16(-1), SIG32(8, MSD_POS))));      // 8/16 * 16^-1
}

const char *gcvt(const ibm_float64_t &f64) {
    static char buf[80];
    f80 = static_cast<float80_t>(f64);
    f80.gcvt(buf, sizeof(buf), 19);
    return buf;
}

void test_ibm_float64() {
    constexpr auto SGN_MASK = ibm_float64_t::SGN_MASK;
    constexpr auto EXP_MASK = ibm_float64_t::EXP_MASK;
    constexpr auto EXP_BASE = ibm_float64_t::EXP_BASE;
    constexpr auto SIG_MASK = ibm_float64_t::SIG_MASK;
    constexpr auto MSD_POS = ibm_float64_t::MSD_POS;
#define SIG64(sig, pos) (UINT64_C(sig) << pos)
    ibm_float64_t f64;
    EQ("0.0", "00.000000-00000000", f64.set(0, 0).str());
    EQ("0.0", "0", gcvt(f64));
    EQ("0.0", OK, f64.set(f80));
    EQ("0.0", "00.000000-00000000", f64.str());

    EQ("1.0", "41.100000-00000000", f64.set(EXP16(1), SIG64(1, MSD_POS)).str());
    EQ("1.0", "1", gcvt(f64));
    EQ("1.0", OK, f64.set(f80));
    EQ("1.0", "41.100000-00000000", f64.str());

    EQ("-1.0", "C1.100000-00000000", f64.set(EXP16(1) + SGN_MASK, SIG64(1, MSD_POS)).str());
    EQ("-1.0", "-1", gcvt(f64));
    EQ("-1.0", OK, f64.set(f80));
    EQ("-1.0", "C1.100000-00000000", f64.str());

    EQ("IBMF64_MAX", "7F.FFFFFF-FFFFFFFF", f64.set(EXP_MASK, SIG_MASK).str());
    EQ("IBMF64_MAX", STR(IBMF64_MAX), gcvt(f64));
    EQ("IBMF64_MAX", OK, f64.set(f80));
    EQ("IBMF64_MAX", "7F.FFFFFF-FFFFFFFF", f64.str());

    EQ("IBMF64_MIN", "00.100000-00000000", f64.set(0, SIG64(1, MSD_POS)).str());
    EQ("IBMF64_MIN", STR(IBMF64_MIN), gcvt(f64));
    EQ("IBMF64_MIN", OK, f64.set(f80));
    EQ("IBMF64_MIN", "00.100000-00000000", f64.str());

    EQ("DBL_MAX", OVERFLOW_RANGE, f64.set(read_f80(STR_DBL_MAX)));
    EQ("DBL_MIN", OK, f64.set(read_f80(STR_DBL_MIN)));
    EQ("DBL_MIN", "0", gcvt(f64));

    constexpr auto MSD2_POS = MSD_POS - 4;
    constexpr auto MSD3_POS = MSD2_POS - 4;
    EQ("1.0", "1", gcvt(f64.set(EXP16(1), SIG64(1, MSD_POS))));                 // 1/16 * 16^1
    EQ("2.0", "2", gcvt(f64.set(EXP16(1), SIG64(2, MSD_POS))));                 // 2/16 * 16^1
    EQ("4.0", "4", gcvt(f64.set(EXP16(1), SIG64(4, MSD_POS))));                 // 4/16 * 16^1
    EQ("8.0", "8", gcvt(f64.set(EXP16(1), SIG64(8, MSD_POS))));                 // 8/16 * 16^1
    EQ("16.0", "16", gcvt(f64.set(EXP16(2), SIG64(1, MSD_POS))));               // 1/16 * 16^2
    EQ("24.0", "24", gcvt(f64.set(EXP16(2), SIG64(24, MSD2_POS))));             // 24/16^2 * 16^2
    EQ("42.0", "42", gcvt(f64.set(EXP16(2), SIG64(42, MSD2_POS))));             // 42/16^2 * 16^2
    EQ("0.5", "0.5", gcvt(f64.set(EXP16(0), SIG64(8, MSD_POS))));               // 8/16 * 16^0
    EQ("0.125", "0.125", gcvt(f64.set(EXP16(0), SIG64(2, MSD_POS))));           // 2/16 * 16^0
    EQ("0.0078125", "0.0078125", gcvt(f64.set(EXP16(-1), SIG64(2, MSD_POS))));  // 2/16^2 * 16^-1
    EQ("1000.0", "1000", gcvt(f64.set(EXP16(3), SIG64(1000, MSD3_POS))));       // 1000/16^3 * 16^3
    EQ("118.625", "118.625", gcvt(f64.set(EXP16(2), SIG64(1898, MSD3_POS))));   // 1898/16 * 16^2
    EQ("101.0", "101", gcvt(f64.set(EXP16(2), SIG64(101, MSD2_POS))));          // 101/16^2 * 16^2
    EQ("0.03125", "0.03125", gcvt(f64.set(EXP16(-1), SIG64(8, MSD_POS))));      // 8/16 * 16^-1
}

void run_tests() {
    RUN_TEST(test_ibm_float32);
    RUN_TEST(test_ibm_float64);
}

}  // namespace test
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
