#include "asm_mc6809.h"

AsmMc6809 assembler;

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available()) {
   String line = Serial.readString();
   Insn insn;
   if (assembler.encode(line.c_str(), insn, 0x1000, nullptr)) {
      Serial.print(F("Error "));
      Serial.println(assembler.getError(), DEC);
   } else {
      Serial.print(insn.address(), HEX);
      Serial.print(':');
      for (int i = 0; i < insn.insnLen(); i++) {
        Serial.print(' ');
        const uint8_t val = insn.bytes()[i];
        if (val < 0x10) Serial.print('0');
        Serial.print(val, HEX);
      }
      Serial.println();
    }
  }
}
