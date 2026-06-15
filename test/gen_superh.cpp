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

#include "dis_superh.h"
#include "gen_driver.h"

using namespace libasm::superh;
using namespace libasm::gen;

int main(int argc, const char **argv) {
    DisSuperH dissuperh;
    libasm::Disassembler &dis = dissuperh;
    GenDriver driver(dissuperh);
    if (driver.main(argc, argv))
        return 1;

    if (driver.generateGas()) {
        dis.setOption("gnu-as", "enable");
    } else {
        dis.setOption("relative", "enable");
        dis.setOption("origin-char", "*");
        dis.setOption("motorola-style", "enable");
    }
    // SH-2A has optional FPU; enable it for SH-2A autogen so FCNVDS / FCNVSD
    // appear.  Leave it off for SH-1 / SH-2 / SH-DSP -- those CPUs don't have
    // an FPU and ASL rejects FADD / FLDI0 / ... when targeting them.  SH-2E's
    // FPU is mandatory and is enabled automatically by setCpu.
    if (strcasecmp_P("SH-2A", dis.config().cpu_P()) == 0)
        dis.setOption("fpu", "true");

    TestGenerator generator(driver, dis, 0x0000);
    generator.generate();

    return driver.close();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
