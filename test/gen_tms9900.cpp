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

#include "dis_tms9900.h"
#include "gen_driver.h"

using namespace libasm::tms9900;
using namespace libasm::test;

int main(int argc, const char **argv) {
    DisTms9900 dis9900;
    dis9900.setRelativeTarget(true);
    GenDriver<Config> driver(dis9900);
    if (driver.main(argc, argv))
        return 1;

    TestGenerator<Config> generator(dis9900, 0x0100);
    generator.generate(driver);
/*
    if (strcmp(dis9900.getCpu(), "99105") == 0) {
        generator
            .generate(driver, 0x001C)
            .generate(driver, 0x001D)
            .generate(driver, 0x0029)
            .generate(driver, 0x002A);
    }
*/

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
