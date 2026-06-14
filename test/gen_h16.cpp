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

#include "dis_h16.h"
#include "gen_driver.h"

using namespace libasm::h16;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisH16 dish16;
    GenDriver driver(dish16);
    if (driver.main(argc, argv))
        return 1;

    // The H16 exception vector table occupies 0x0000-0x03FC at reset (Manual
    // Table 4-13).  Place test code at 0x1000 so PC-relative deltas produce
    // targets in a sensible code-area range rather than the wrap-around
    // negative range that ORG=0 would yield.
    TestGenerator generator(driver, dish16, 0x1000);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
