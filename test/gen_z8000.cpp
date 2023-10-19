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

#include "dis_z8000.h"
#include "gen_driver.h"

using namespace libasm::z8000;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisZ8000 dis8000;
    GenDriver driver(dis8000);
    if (driver.main(argc, argv))
        return 1;

    dis8000.setOption("relative", "enable");
    dis8000.setOption("intel-hex", "enable");
    if (driver.generateGas()) {
        dis8000.setOption("c-style", "enable");
        dis8000.setOption("ioaddr-prefix", "enable");
        dis8000.setOption("origin-char", ".");
        dis8000.setOption("short-direct", "disable");
    }

    const Config::uintptr_t org =
            dis8000.addressWidth() == libasm::ADDRESS_24BIT ? 0x10000 : 0x1000;
    TestGenerator generator(driver, dis8000, org);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
