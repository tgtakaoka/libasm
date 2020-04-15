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

#include "asm_mc68000.h"
#include "asm_directive.h"
#include "asm_driver.h"
#include "bin_formatter.h"

using namespace libasm::mc68000;
using namespace libasm::cli;

int main(int argc, const char **argv) {
    AsmMc68000 assembler;
    AsmMotoDirective directive(assembler);
    AsmDriver driver;
    if (driver.parseOption(argc, argv, directive))
        return driver.usage();
    return driver.assemble();
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4: