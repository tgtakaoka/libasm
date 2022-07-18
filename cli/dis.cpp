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

#include "dis_scn2650.h"
#include "dis_cdp1802.h"
#include "dis_commander.h"
#include "dis_i8048.h"
#include "dis_i8051.h"
#include "dis_i8080.h"
#include "dis_i8086.h"
#include "dis_i8096.h"
#include "dis_ins8060.h"
#include "dis_ins8070.h"
#include "dis_mc6800.h"
#include "dis_mc68000.h"
#include "dis_mc6805.h"
#include "dis_mc6809.h"
#include "dis_mn1610.h"
#include "dis_mos6502.h"
#include "dis_ns32000.h"
#include "dis_tlcs90.h"
#include "dis_tms32010.h"
#include "dis_tms9900.h"
#include "dis_z8.h"
#include "dis_z80.h"
#include "dis_z8000.h"

using namespace libasm;
using namespace libasm::cli;
using namespace libasm::driver;

mos6502::DisMos6502 dis6502;
mc6800::DisMc6800 dis6800;
mc6805::DisMc6805 dis6805;
mc6809::DisMc6809 dis6809;
i8048::DisI8048 dis8048;
i8051::DisI8051 dis8051;
i8080::DisI8080 dis8080;
z80::DisZ80 disz80;
z8::DisZ8 disz8;
ins8060::DisIns8060 dis8060;
ins8070::DisIns8070 dis8070;
cdp1802::DisCdp1802 dis1802;
scn2650::DisScn2650 dis2650;
i8086::DisI8086 dis8086;
i8096::DisI8096 dis8096;
tlcs90::DisTlcs90 dis90;
tms32010::DisTms32010 dis32010;
tms9900::DisTms9900 dis9900;
mc68000::DisMc68000 dis68000;
z8000::DisZ8000 disz8000;
ns32000::DisNs32000 dis32000;
mn1610::DisMn1610 dis1610;

Disassembler *disassemblers[] = {
        &dis6800,
        &dis6805,
        &dis6809,
        &dis6502,
        &dis8048,
        &dis8051,
        &dis8080,
        &disz80,
        &disz8,
        &dis90,
        &dis8060,
        &dis8070,
        &dis1802,
        &dis2650,
        &dis32010,
        &dis8086,
        &dis8096,
        &dis68000,
        &dis9900,
        &disz8000,
        &dis32000,
        &dis1610,
};

int main(int argc, const char **argv) {
    DisCommander commander(disassemblers, std::end(disassemblers));
    if (commander.parseArgs(argc, argv))
        return commander.usage();
    return commander.disassemble();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
