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

#include "dis_m6502.h"
#include "dis_mc6800.h"
#include "dis_mc6809.h"
#include "dis_i8080.h"
#include "dis_z80.h"
#include "dis_tms9900.h"
#include "dis_mc68000.h"
#include "dis_driver.h"

#include <vector>

using namespace libasm;
using namespace libasm::cli;

m6502::DisM6502     dis6502;
mc6800::DisMc6800   dis6800;
mc6809::DisMc6809   dis6809;
i8080::DisI8080     dis8080;
z80::DisZ80         disz80;
tms9900::DisTms9900 dis9900;
mc68000::DisMc68000 dis68000;

std::vector<Disassembler *> disassemblers = {
    &dis6809, &dis6800, &dis6502,
    &dis8080, &disz80,
    &dis9900, &dis68000,
};

int main(int argc, const char **argv) {
    DisDriver driver;
    if (driver.parseOption(argc, argv, disassemblers))
        return driver.usage();
    return driver.disassemble();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: