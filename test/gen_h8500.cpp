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

#include "dis_h8500.h"
#include "gen_driver.h"

using namespace libasm::h8500;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisH8500 dish8500;
    GenDriver driver(dish8500);
    if (driver.main(argc, argv))
        return 1;

    // C-style numbers + relative branch targets so the test generator can
    // tokenize operand values and collapse address/branch variants (Hitachi
    // H'xxxx and absolute targets are not recognized by the tokenizer).
    dish8500.setOption("c-style", "enable");
    dish8500.setOption("relative", "enable");
    if (driver.generateGas()) {
        // h8500-hms-as rejects the FP alias inside a register list; emit R6.
        dish8500.setOption("fp-alias", "off");
        // h8500-hms-as uses '.' as the current-location symbol, not '$'.
        dish8500.setOption("origin-char", ".");
    } else {
        dish8500.setOption("origin-char", "*");
    }

    TestGenerator generator(driver, dish8500, 0x0100);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
