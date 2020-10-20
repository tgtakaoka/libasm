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

#include "dis_z80.h"
#include "gen_driver.h"

using namespace libasm::z80;
using namespace libasm::test;

static bool filterZ80Prefix(uint8_t opc) {
    if (opc == 0xCB) return true;
    if (opc == 0xDD) return true;
    if (opc == 0xED) return true;
    if (opc == 0xFD) return true;
    return false;
}

static bool filterZ80IxBitPrefix(uint8_t opc) {
    if (opc == 0xCB) return true;
    return false;
}

int main(int argc, const char **argv) {
    DisZ80 disz80;
    disz80.setRelativeTarget(true);
    GenDriver<Config> driver(disz80);
    if (driver.main(argc, argv))
        return 1;
    if (strcmp(disz80.getCpu(), "Z80"))
        driver.pseudo("Z80SYNTAX EXCLUSIVE");

    TestGenerator<Config> generator(disz80, 0x0100);
    generator
        .generate(driver, filterZ80Prefix)
        .generate(driver, 0xED)
        .generate(driver, 0xDD, filterZ80IxBitPrefix)
        .generate(driver, 0xFD, filterZ80IxBitPrefix)
        .generate(driver, 0xCB)
        .generate(driver, 0xDD, 0xCB, 0x7F)
        .generate(driver, 0xFD, 0xCB, 0x80);

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
