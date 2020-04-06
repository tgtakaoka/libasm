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

#include "dis_i8080.h"
#include "gen_driver.h"

using namespace libasm::i8080;

int main(int argc, const char **argv) {
    DisI8080 dis8080;
    GenDriver<target::uintptr_t> driver(dis8080);
    if (driver.main(argc, argv))
        return 1;

    TestGenerator<target::uintptr_t> generator(
        dis8080,
        sizeof(target::opcode_t),
        driver.uppercase());
    generator.generate(driver);

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
