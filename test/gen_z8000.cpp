/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include <cctype>

#include "dis_z8000.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::z8000;
using namespace libasm::gen;

namespace {

// Match a Z8001 extern symbol "seg_SS_off_XXXX" (2 + 4 hex digits) emitted by
// DisZ8000 with the extern-symbol option (gas path).  Collapse to "s" so the
// generator dedups across addresses instead of one test per segmented address.
struct Z8001ExternSymTokenizer : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (p[0] != 's' || p[1] != 'e' || p[2] != 'g' || p[3] != '_')
            return nullptr;
        if (!isxdigit(p[4]) || !isxdigit(p[5]))
            return nullptr;
        if (p[6] != '_' || p[7] != 'o' || p[8] != 'f' || p[9] != 'f' || p[10] != '_')
            return nullptr;
        if (!isxdigit(p[11]) || !isxdigit(p[12]) || !isxdigit(p[13]) || !isxdigit(p[14]))
            return nullptr;
        out.push_back('s');
        return p + 15;
    }
};

const Z8001ExternSymTokenizer EXTERN_SYM;
// Register file: word R0-R15, byte RH0-RH7/RL0-RL7, long RR0-RR14, quad RQ0-RQ12.
// Order longer prefixes first so "RR4"/"RQ4"/"RH4"/"RL4" are not split by "R".
const RegisterTokenizer REG_RQ("RQ", 12, "RQn");
const RegisterTokenizer REG_RR("RR", 14, "RRn");
const RegisterTokenizer REG_RH("RH", 7, "RHn");
const RegisterTokenizer REG_RL("RL", 7, "RLn");
const RegisterTokenizer REG_R("R", 15, "Rn");

// |Hex| = IntelNumber (native ..H) or CstyleNumber (gnu-as 0x..).
template <class Hex>
TokenizerList tokenizers(char loc) {
    TokenizerList list =
            standardTokenizers<Hex>(loc, {&REG_RQ, &REG_RR, &REG_RH, &REG_RL, &REG_R});
    list.push_back(&EXTERN_SYM);
    return list;
}

}  // namespace

int main(int argc, const char **argv) {
    DisZ8000 dis8000;
    GenDriver driver(dis8000);
    if (driver.main(argc, argv))
        return 1;

    dis8000.setOption("relative", "enable");
    dis8000.setOption("intel-style", "enable");
    dis8000.setOption("segmented-addr", "enable");
    // Force long-direct encoding on both gas and Zilog paths so the gas
    // long-direct relocation output and the libasm asm output stay
    // byte-identical.  z8k-coff-ld only resolves Z8001 r_imm32 via the
    // long-direct form, so emitting consistent long-direct everywhere
    // keeps the generated .inc and .ginc producing the same bytes.
    dis8000.setOption("short-direct", "disable");
    if (driver.generateGas()) {
        dis8000.setOption("gnu-as", "enable");
        dis8000.setOption("extern-symbol", "enable");
    }

    // Z8001 text at seg 0x02 keeps it clear of segments referenced by the
    // generated direct-address operands (0x00, 0x01, 0x41-0x70, ...), so
    // the per-segment helper sections placed by the gas linker do not
    // collide with .text.  Z8002 (16-bit, non-segmented) keeps 0x1000.
    const Config::uintptr_t org =
            dis8000.addressWidth() == libasm::ADDRESS_16BIT ? 0x1000 : 0x20000;
    const auto sym = dis8000.curSym();
    const auto toks = driver.generateGas() ? tokenizers<CstyleNumber>(sym)
                                           : tokenizers<IntelNumber>(sym);
    TestGenerator generator(driver, dis8000, org, toks);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
