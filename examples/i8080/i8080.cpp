/*
 * Copyright 2022 Tadashi G. Takaoka
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

#include <pio_example.h>
#include <asm_i8080.h>
#include <asm_z80.h>
#include <dis_i8080.h>
#include <dis_z80.h>

libasm::i8080::AsmI8080 asm8080;
libasm::i8080::DisI8080 dis8080;
libasm::z80::AsmZ80 asmz80;
libasm::z80::DisZ80 disz80;

struct Example : libasm::pio::Example {
    Example() : libasm::pio::Example(asm8080, dis8080) {}

protected:
    bool processPseudo(const char *line) override {
        if (strcasecmp(line, "INTEL") == 0) {
            switchSyntax(asm8080, dis8080);
            return true;
        }
        if (strcasecmp(line, "ZILOG") == 0) {
            switchSyntax(asmz80, disz80);
            return true;
        }
        return libasm::pio::Example::processPseudo(line);
    }

private:
    void switchSyntax(libasm::Assembler &assembler, libasm::Disassembler &disassembler) {
        char cpu[10];
        strcpy(cpu, cpu_P());
        _asm = &assembler;
        _dis = &disassembler;
        if (isAsm()) {
            _asm->setCpu(cpu);
        } else {
            _dis->setCpu(cpu);
        }
    }
};

Example example;

int main() {
    return example.run();
}

// Local Variables:
// mode: c++
// c-basic-offset: 2
// tab-width: 2
// End:
// vim: set ft=cpp et ts=2 sw=2:
