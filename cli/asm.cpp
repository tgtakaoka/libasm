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
#include "asm_commander.h"
#include "asm_directive.h"
#include "asm_f3850.h"
#include "asm_i8048.h"
#include "asm_i8051.h"
#include "asm_i8080.h"
#include "asm_i8086.h"
#include "asm_i8096.h"
#include "asm_ins8060.h"
#include "asm_ins8070.h"
#include "asm_mc6800.h"
#include "asm_mc68000.h"
#include "asm_mc6805.h"
#include "asm_mc6809.h"
#include "asm_mn1610.h"
#include "asm_mos6502.h"
#include "asm_ns32000.h"
#include "asm_scn2650.h"
#include "asm_tlcs90.h"
#include "asm_tms32010.h"
#include "asm_tms9900.h"
#include "asm_z8.h"
#include "asm_z80.h"
#include "asm_z8000.h"

using namespace libasm;
using namespace libasm::cli;
using namespace libasm::driver;

mos6502::AsmMos6502 asm6502;
mc6800::AsmMc6800 asm6800;
mc6805::AsmMc6805 asm6805;
mc6809::AsmMc6809 asm6809;
i8048::AsmI8048 asm8048;
i8051::AsmI8051 asm8051;
i8080::AsmI8080 asm8080;
z80::AsmZ80 asmz80;
z8::AsmZ8 asmz8;
ins8060::AsmIns8060 asm8060;
ins8070::AsmIns8070 asm8070;
cdp1802::AsmCdp1802 asm1802;
scn2650::AsmScn2650 asm2650;
f3850::AsmF3850 asm3850;
i8086::AsmI8086 asm8086;
i8096::AsmI8096 asm8096;
tlcs90::AsmTlcs90 asm90;
tms9900::AsmTms9900 asm9900;
tms32010::AsmTms32010 asm32010;
mc68000::AsmMc68000 asm68000;
z8000::AsmZ8000 asmz8000;
ns32000::AsmNs32000 asm32000;
mn1610::AsmMn1610 asm1610;

MotorolaDirective dir6502(asm6502);
MotorolaDirective dir6800(asm6800);
MotorolaDirective dir6805(asm6805);
MotorolaDirective dir6809(asm6809);
IntelDirective dir8048(asm8048);
IntelDirective dir8051(asm8051);
IntelDirective dir8080(asm8080);
IntelDirective dirz80(asmz80);
IntelDirective dirz8(asmz8);
NationalDirective dir8060(asm8060);
NationalDirective dir8070(asm8070);
IntelDirective dir1802(asm1802);
IntelDirective dir2650(asm2650);
FairchildDirective dir3850(asm3850);
IntelDirective dir8086(asm8086);
IntelDirective dir8096(asm8096);
IntelDirective dir90(asm90);
IntelDirective dir9900(asm9900);
IntelDirective dir32010(asm32010);
MotorolaDirective dir68000(asm68000);
IntelDirective dirz8000(asmz8000);
NationalDirective dir32000(asm32000);
NationalDirective dir1610(asm1610);

AsmDirective *directives[] = {
        &dir6800,
        &dir6805,
        &dir6809,
        &dir6502,
        &dir8048,
        &dir8051,
        &dir8080,
        &dirz80,
        &dirz8,
        &dir90,
        &dir8060,
        &dir8070,
        &dir1802,
        &dir2650,
        &dir3850,
        &dir32010,
        &dir8086,
        &dir8096,
        &dir68000,
        &dir9900,
        &dirz8000,
        &dir32000,
        &dir1610,
};

int main(int argc, const char **argv) {
    AsmCommander commander(directives, std::end(directives));
    if (commander.parseArgs(argc, argv))
        return commander.usage();
    return commander.assemble();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
