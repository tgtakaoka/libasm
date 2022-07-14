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

#include "dis_mos6502.h"
#include "gen_driver.h"

using namespace libasm::mos6502;
using namespace libasm::test;

int main(int argc, const char **argv) {
    DisMos6502 dis6502;
    dis6502.setOption("relative", "enable");
    dis6502.setOption("indirect-long", "disable");
    GenDriver<Config> driver(dis6502);
    if (driver.main(argc, argv))
        return 1;

    const Config::uintptr_t origin =
            dis6502.addressWidth() == libasm::ADDRESS_24BIT ? 0x100200 : 0x0200;
    TestGenerator<Config> generator(dis6502, origin);
    generator.generate(driver);

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
