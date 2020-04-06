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

#include "dis_mc6809.h"
#include "gen_driver.h"

using namespace libasm::mc6809;

static bool filterHd6309BitImmIndexed(uint8_t opc) {
    if (opc == 0x10) return true; // prefix
    if (opc == 0x11) return true; // prefix
    if (opc == 0x61) return true; // OIM #nn,<indexed>
    if (opc == 0x62) return true; // AIM #nn,<indexed>
    if (opc == 0x65) return true; // EIM #nn,<indexed>
    if (opc == 0x6B) return true; // TIM #nn,<indexed>
    return false;
}

int main(int argc, const char **argv) {
    DisMc6809 dis6809;
    GenDriver<target::uintptr_t> driver(dis6809);
    if (driver.main(argc, argv))
        return 1;

    TestGenerator<target::uintptr_t> generator(
        dis6809,
        sizeof(target::opcode_t),
        driver.uppercase());
    generator
        .generate(driver, filterHd6309BitImmIndexed)
        .generate(driver, 0x10)
        .generate(driver, 0x11)
        .generate(driver, 0x61, 0x62) // OIM
        .generate(driver, 0x62, 0x63) // AIM
        .generate(driver, 0x65, 0x66) // EIM
        .generate(driver, 0x6B, 0x6C); // TIM

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
