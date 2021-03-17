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

#include "dis_ns32000.h"
#include "gen_driver.h"

using namespace libasm::ns32000;
using namespace libasm::test;

int main(int argc, const char **argv) {
    DisNs32000 dis32000;
    GenDriver<Config> driver(dis32000);
    dis32000.setRelativeTarget(true);
    if (driver.main(argc, argv))
        return 1;

    if (driver.generateGas()) {
        dis32000.stringOptionBraket(true);
        dis32000.setCurrentOriginSymbol('.');
        dis32000.pcRelativeParen(true);
    }
    TestGenerator<Config> generator(dis32000, 0x10000);
    generator.generate(driver);

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
