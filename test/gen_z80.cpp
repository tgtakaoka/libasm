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
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisZ80 disz80;
    GenDriver driver(disz80);
    if (driver.main(argc, argv))
        return 1;

    disz80.setOption("relative", "enable");
    disz80.setOption("extmode", "on");
    disz80.setOption("lwordmode", "on");
    const auto org = strcasecmp(driver.cpu(), "Z380") == 0 ? 0x10000 : 0x0100;

    TestGenerator generator(driver, disz80, org);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
