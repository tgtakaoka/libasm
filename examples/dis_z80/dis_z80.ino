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

#include <dis_z80.h>
#include <str_memory.h>
#include <libcli.h>

using namespace libasm;
using namespace libasm::z80;

DisZ80 disz80;
Disassembler<Config> &disassembler(disz80);

void disassemble(DisMemory<Config> &memory) {
  char operands[20];
  Insn<Config> insn;
  while (memory.hasNext()) {
    if (disassembler.decode(memory, insn, operands, nullptr)) {
      Cli.print(F("Error "));
      Cli.println(disassembler.getError());
    } else {
      Cli.printUint16(insn.address());
      Cli.print(':');
      for (int i = 0; i < insn.length(); i++) {
        Cli.print(' ');
        uint8_t val = insn.bytes()[i];
        Cli.printUint8(val);
      }
      for (int i = insn.length(); i < Config::code_max; i++)
        Cli.print(F("   "));
      Cli.print(' ');
      for (size_t n = Cli.print(insn.name()); n <= Config::name_max; n++)
        Cli.print(' ');
      Cli.println(operands);
    }
  }
}

bool handleLine(Cli::State state, char *line, uintptr_t extra) {
  StrMemory<Config> memory(0x1000, line);
  disassemble(memory);
  return Cli.readLine(handleLine, 0);
}

void setup() {
  disassembler.setCpu("z80");

  Serial.begin(9800);
  Cli.begin(Serial);
  Cli.readLine(handleLine, 0);
}

void loop() {
  Cli.loop();
}

// Local Variables:
// mode: c++
// c-basic-offset: 2
// tab-width: 2
// End:
// vim: set ft=cpp et ts=2 sw=2:
