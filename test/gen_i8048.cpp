/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "dis_i8048.h"
#include "gen_driver.h"
#include "tokenizer.h"

using namespace libasm::i8048;
using namespace libasm::gen;

namespace {
const RegisterTokenizer REG_Rn("R", 7, "Rn");
const RegisterTokenizer REG_Pn("P", 7, "Pn");
const RegisterTokenizer REG_RBn("RB", 1, "RBn");
const RegisterTokenizer REG_MBn("MB", 1, "MBn");
const RegisterTokenizer REG_Fn("F", 1, "Fn");
}  // namespace

int main(int argc, const char **argv) {
    DisI8048 dis8048;
    GenDriver driver(dis8048);
    if (driver.main(argc, argv))
        return 1;

    TestGenerator generator(driver, dis8048, 0x0100, standardTokenizers<IntelNumber>(dis8048.curSym(), {&REG_RBn, &REG_MBn, &REG_Fn, &REG_Rn, &REG_Pn}));
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
