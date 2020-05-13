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

#include "asm_cdp1802.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "asm_i8080.h"
#include "asm_ins8060.h"
#include "asm_ins8070.h"
#include "asm_mc6800.h"
#include "asm_mc6809.h"
#include "asm_mc68000.h"
#include "asm_mos6502.h"
#include "asm_tms9900.h"
#include "asm_w65c816.h"
#include "asm_z80.h"

#include <vector>

using namespace libasm;
using namespace libasm::cli;

mos6502::AsmMos6502 asm6502;
w65c816::AsmW65C816 asm65816;
mc6800::AsmMc6800   asm6800;
mc6809::AsmMc6809   asm6809;
i8080::AsmI8080     asm8080;
z80::AsmZ80         asmz80;
ins8060::AsmIns8060 asm8060;
ins8070::AsmIns8070 asm8070;
cdp1802::AsmCdp1802 asm1802;
tms9900::AsmTms9900 asm9900;
mc68000::AsmMc68000 asm68000;

AsmMostekDirective dir6502(asm6502);
AsmMostekDirective dir65816(asm65816);
AsmMotoDirective   dir6800(asm6800);
AsmMotoDirective   dir6809(asm6809);
AsmIntelDirective  dir8080(asm8080);
AsmIntelDirective  dirz80(asmz80);
AsmIntelDirective  dir8060(asm8060);
AsmIntelDirective  dir8070(asm8070);
AsmIntelDirective  dir1802(asm1802);
AsmIntelDirective  dir9900(asm9900);
AsmMotoDirective   dir68000(asm68000);

std::vector<AsmDirective *> directives = {
    &dir6809, &dir6800, &dir6502, &dir65816,
    &dir8080, &dirz80,  &dir8060, &dir8070,
    &dir1802, &dir9900, &dir68000
};

int main(int argc, const char **argv) {
    AsmDriver driver(directives);
    if (driver.parseOption(argc, argv))
        return driver.usage();
    return driver.assemble();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
