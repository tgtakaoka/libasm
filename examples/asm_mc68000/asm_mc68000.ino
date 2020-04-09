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

#include <asm_mc68000.h>
#include <libcli.h>

using namespace libasm;
using namespace libasm::mc68000;

AsmMc68000 as68k;
Assembler &assembler(as68k);

void assemble(const char *line) {
  Insn insn;
  if (assembler.encode(line, insn, 0x10000, nullptr)) {
    Cli.print(F("Error "));
    Cli.print(assembler.getError());
    Cli.print(F(" at: "));
    Cli.println(assembler.errorAt());
  } else {
    Cli.printUint24(insn.address());
    Cli.print(':');
    for (int i = 0; i < insn.length(); i += 2) {
      Cli.print(' ');
      uint16_t val = static_cast<uint16_t>(insn.bytes()[i]) << 8;
      val |= insn.bytes()[i+1];
      Cli.printUint16(val);
    }
    Cli.println();
  }
}

bool handleLine(Cli::State state, char *line, uintptr_t extra) {
  assemble(line);
  return Cli.readLine(handleLine, 0);
}

void setup() {
  assembler.setCpu("68008");

  Serial.begin(9600);
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
