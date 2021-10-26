/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include <arduino_example.h>
#include <asm_mc6800.h>
#include <asm_mc6805.h>
#include <asm_mc6809.h>

libasm::mc6800::AsmMc6800 asm6800;
libasm::mc6805::AsmMc6805 asm6805;
libasm::mc6809::AsmMc6809 asm6809;

libasm::Assembler *disassemblers[] = {
        &asm6800,
        &asm6805,
        &asm6809,
};

libasm::arduino::AsmExample example(ARRAY_RANGE(disassemblers));

void setup() {
    Serial.begin(9600);
    example.begin(Serial);
}

void loop() {
    example.loop();
}

// Local Variables:
// mode: c++
// c-basic-offset: 2
// tab-width: 2
// End:
// vim: set ft=cpp et ts=2 sw=2:
