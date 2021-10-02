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
#include <dis_i8048.h>
#include <dis_i8051.h>
#include <dis_i8080.h>

libasm::i8080::DisI8080 dis8080;
libasm::i8048::DisI8048 dis8048;
libasm::i8051::DisI8051 dis8051;

libasm::Disassembler *disassemblers[] = {
        &dis8080,
        &dis8048,
        &dis8051,
};

libasm::arduino::DisExample example(ARRAY_RANGE(disassemblers));

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
