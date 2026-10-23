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

#include "dis_c166.h"
#include "gen_driver.h"
#include "reg_c166.h"
#include "tokenizer.h"

#include <cstdlib>
#include <string>
#include <unordered_set>

using namespace libasm::c166;
using namespace libasm::gen;

namespace {

const RegisterTokenizer REG_RL("RL", 7, "RLn");
const RegisterTokenizer REG_RH("RH", 7, "RHn");
const RegisterTokenizer REG_R("R", 15, "Rn");

// A reg operand is written as its SFR address, which a mem operand may be
// too.  Mark the first SFR address only, so that "mem, reg" is told from
// "reg, mem" and a "reg, mem" whose mem is an SFR address, which ASL reads
// as "mem, reg", is not written.
// |Hex| = IntelNumber (native ..H) or CstyleNumber (gnu-as 0x..).
template <class Hex>
struct SfrTokenizer final : Tokenizer {
    const char *tokenize(const char *p, std::string &out) const override {
        if (out.find("sfr") != std::string::npos)
            return nullptr;
        if (!out.empty() && (out.back() == '#' || out.back() == '+' ||
                                    isalnum(static_cast<unsigned char>(out.back()))))
            return nullptr;
        const char *end;
        if (!Hex::match(p, end) || !isHex(p, end))
            return nullptr;
        const auto addr = strtoul(p, nullptr, 16);
        if (addr >= 0xFE00 && addr < 0xFFE0 && (addr & 1) == 0) {
            out.append("sfr");
            return end;
        }
        return nullptr;
    }

private:
    static bool isHex(const char *p, const char *end);
};

template <>
bool SfrTokenizer<IntelNumber>::isHex(const char *, const char *end) {
    return toupper(end[-1]) == 'H';
}

template <>
bool SfrTokenizer<CstyleNumber>::isHex(const char *p, const char *) {
    return p[0] == '0' && tolower(p[1]) == 'x';
}

const SfrTokenizer<IntelNumber> SFR;
const SfrTokenizer<CstyleNumber> SFR_GAS;

// A core SFR name is kept whole, so that the number tokenizer does not fold
// DPP0-DPP3 or CPUCON1-CPUCON2 into one.
struct SfrNameTokenizer final : Tokenizer {
    SfrNameTokenizer(CpuType cpuType) {
        for (uint32_t addr = 0xFE00; addr < 0x10000; addr += 2) {
            const auto *name_P = libasm::c166::reg::sfrName_P(addr, cpuType);
            if (name_P) {
                std::string name;
                for (auto p = name_P; pgm_read_byte(p); p++)
                    name += tolower(pgm_read_byte(p));
                _names.insert(name);
            }
        }
    }

    const char *tokenize(const char *p, std::string &out) const override {
        if (!out.empty() && isalnum(static_cast<unsigned char>(out.back())))
            return nullptr;
        auto end = p;
        std::string name;
        while (isalnum(static_cast<unsigned char>(*end)))
            name += tolower(*end++);
        if (_names.count(name) == 0)
            return nullptr;
        out.append(p, end);
        return end;
    }

private:
    std::unordered_set<std::string> _names;
};

}  // namespace

int main(int argc, const char **argv) {
    DisC166 dis166;
    GenDriver driver(dis166);
    if (driver.main(argc, argv))
        return 1;

    dis166.setOption("relative", "enable");
    if (driver.generateGas()) {
        // Core SFRs by name, as GNU as knows them.
        dis166.setOption("gnu-as", "enable");
        dis166.setOption("origin-char", ".");
    } else {
        // Spell the operands so that ASL reads the same generated source.
        dis166.setOption("cc-prefix", "disable");
        dis166.setOption("index-sharp", "disable");
    }
    dis166.setOption("canonical-mem", "enable");

    const SfrNameTokenizer sfrName(dis166.cpuType());
    const auto sym = dis166.curSym();
    const auto toks = driver.generateGas() ? standardTokenizers<CstyleNumber>(sym,
                                                     {&sfrName, &REG_RL, &REG_RH, &REG_R, &SFR_GAS})
                                           : standardTokenizers<IntelNumber>(
                                                     sym, {&REG_RL, &REG_RH, &REG_R, &SFR});
    TestGenerator generator(driver, dis166, 0x10100, toks);
    // A register GNU as has no name for; counting it would cut the walk of
    // the reg byte short of the named ones.
    if (driver.generateGas())
        generator.ignoreError(libasm::UNKNOWN_REGISTER);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
