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

#include "dis_h8300.h"
#include "gen_driver.h"

using namespace libasm::h8300;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisH8300 dish8300;
    GenDriver driver(dish8300);
    if (driver.main(argc, argv))
        return 1;

    dish8300.setOption("relative", "enable");
    dish8300.setOption("c-style", "enable");
    if (driver.generateGas()) {
        dish8300.setOption("gnu-as", "enable");
    } else {
        dish8300.setOption("origin-char", "*");
        // libasm and asl both prefer the canonical ER7/R7 form over the SP
        // alias; the gas path keeps the alias since gas accepts both.
        dish8300.setOption("sp-alias", "disable");
    }

    TestGenerator generator(driver, dish8300, 0x0100);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
