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

#include "dis_superh.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::superh;
using namespace libasm::gen;

namespace {
// SH prints lowercase registers: r0..r15 -> rn, fr0..fr15 -> frn, dr even -> drn.
const RegisterTokenizer REG_fr("fr", 15, "frn");
const RegisterTokenizer REG_dr("dr", 14, "drn");
const RegisterTokenizer REG_r("r", 15, "rn");
const RegisterTokenizer REG_a("a", 0, "an");
const RegisterTokenizer REG_x("x", 1, "xn");
const RegisterTokenizer REG_y("y", 1, "yn");
}  // namespace

int main(int argc, const char **argv) {
    DisSuperH dissuperh;
    libasm::Disassembler &dis = dissuperh;
    GenDriver driver(dissuperh);
    if (driver.main(argc, argv))
        return 1;

    // SH assembly is conventionally lowercase; emit lowercase mnemonics,
    // registers and hex regardless of the driver's -u flag.
    dis.setOption("upper-case", "false");
    dis.setOption("upper-hex", "false");

    if (driver.generateGas()) {
        dis.setOption("gnu-as", "enable");
    } else {
        dis.setOption("relative", "enable");
        dis.setOption("origin-char", "$");
        dis.setOption("motorola-style", "enable");
    }
    // SH displacements are unsigned (no sign to consolidate).  gnu-as prints
    // C-style (0x..) numbers, native Motorola ($..).
    const auto sym = dis.curSym();
    const auto toks = driver.generateGas()
            ? standardTokenizers<CstyleNumber>(sym, {&REG_fr, &REG_dr, &REG_r, &REG_a, &REG_x, &REG_y})
            : standardTokenizers<MotorolaNumber>(sym, {&REG_fr, &REG_dr, &REG_r, &REG_a, &REG_x, &REG_y});
    TestGenerator generator(driver, dis, 0x0000, toks);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
