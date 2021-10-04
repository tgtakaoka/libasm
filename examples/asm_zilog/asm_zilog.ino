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
#include <asm_tlcs90.h>
#include <asm_z8.h>
#include <asm_z80.h>

libasm::tlcs90::AsmTlcs90 asm90;
libasm::z80::AsmZ80 asmz80;
libasm::z8::AsmZ8 asmz8;

libasm::Assembler *disassemblers[] = {
        &asmz80,
        &asm90,
        &asmz8,
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