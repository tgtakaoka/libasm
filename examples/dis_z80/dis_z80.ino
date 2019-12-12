#include <dis_z80.h>
#include <str_memory.h>

DisZ80 disassembler;

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available()) {
    String line = Serial.readString();
    StrMemory memory(0x1000, line.c_str());
    char operands[20];
    Insn insn;
    if (disassembler.decode(memory, insn, operands, nullptr)) {
      Serial.print(F("Error "));
      Serial.println(disassembler.getError(), DEC);
    } else {
      Serial.print(insn.address(), HEX);
      Serial.print(':');
      for (int i = 0; i < insn.insnLen(); i++) {
        Serial.print(' ');
        uint8_t val = insn.bytes()[i];
        if (val < 0x10) Serial.print('0');
        Serial.print(val, HEX);
      }
      Serial.print(' ');
      Serial.print(insn.name());
      Serial.print(' ');
      Serial.println(operands);
    }
  }
}