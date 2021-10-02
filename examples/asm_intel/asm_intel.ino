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
#include <asm_i8048.h>
#include <asm_i8051.h>
#include <asm_i8080.h>

libasm::i8080::AsmI8080 asm8080;
libasm::i8048::AsmI8048 asm8048;
libasm::i8051::AsmI8051 asm8051;

libasm::Assembler *disassemblers[] = {
        &asm8080,
        &asm8048,
        &asm8051,
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
