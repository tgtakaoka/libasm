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

#include <dis_mc68000.h>
#include <str_memory.h>
#include <libcli.h>

using namespace libasm;
using namespace libasm::mc68000;

DisMc68000 dis68k;
Disassembler<Config> &disassembler(dis68k);

void disassemble(DisMemory<Config> &memory) {
  char operands[20];
  Insn insn;
  while (memory.hasNext()) {
    if (disassembler.decode(memory, insn, operands, nullptr)) {
      Cli.print(F("Error "));
      Cli.println(disassembler.getError());
    } else {
      Cli.printUint24(insn.address());
      Cli.print(':');
      for (int i = 0; i < insn.length(); i += 2) {
        Cli.print(' ');
        uint16_t val = static_cast<uint16_t>(insn.bytes()[i]) << 8;
        val |= insn.bytes()[i+1];
        Cli.printUint16(val);
      }
      for (int i = insn.length(); i < Config::code_max; i += 2)
        Cli.print(F("     "));
      Cli.print(' ');
      for (size_t n = Cli.print(insn.name()); n <= Config::name_max; n++)
        Cli.print(' ');
      Cli.println(operands);
    }
  }
}

bool handleLine(Cli::State state, char *line, uintptr_t extra) {
  StrMemory<Config> memory(0x10000, line);
  disassemble(memory);
  return Cli.readLine(handleLine, 0);
}

void setup() {
  disassembler.setCpu("68008");

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
