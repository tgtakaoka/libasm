/*
 * Copyright 2023 Tadashi G. Takaoka
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
#include <asm_im6100.h>
#include <dis_im6100.h>

libasm::im6100::AsmIm6100 asm6100;
libasm::im6100::DisIm6100 dis6100;

libasm::arduino::Example example(asm6100, dis6100);

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