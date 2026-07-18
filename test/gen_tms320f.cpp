/*
 * Copyright 2025 Tadashi G. Takaoka
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

#include "dis_tms320f.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::tms320f;
using namespace libasm::gen;

namespace {
const RegisterTokenizer REG_AR("AR", 7, "ARn");
const RegisterTokenizer REG_IR("IR", 1, "IRn");
const RegisterTokenizer REG_R("R", 7, "Rn");
}  // namespace

int main(int argc, const char **argv) {
    DisTms320f dis320c3x;
    GenDriver driver(dis320c3x);
    if (driver.main(argc, argv))
        return 1;

    dis320c3x.setOption("relative", "on");
    if (driver.generateGas())
        dis320c3x.setOption("gnu-as", "on");
    const auto sym = dis320c3x.curSym();
    const auto toks = driver.generateGas() ? standardTokenizers<CstyleNumber>(sym, {&REG_AR, &REG_IR, &REG_R})
                                           : standardTokenizers<IntelNumber>(sym, {&REG_AR, &REG_IR, &REG_R});
    TestGenerator generator(driver, dis320c3x, 0x01000, toks);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
