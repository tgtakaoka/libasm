/* -*- mode: c++; c-basic-offset: 2; tab-width: 2; -*- */
#include <asm_mc6809.h>

AsmMc6809 as6809;
Assembler<target::uintptr_t> &assembler(as6809);

String line;
bool line_ready = false;

void setup() {
  Serial.begin(9600);
  assembler.setCpu("6309");
}

void loop() {
  if (line_ready) {
    Serial.println(line);
    Insn insn;
    if (assembler.encode(line.c_str(), insn, 0x1000, nullptr)) {
      Serial.print(F("Error "));
      Serial.print(assembler.getError(), DEC);
      Serial.print(F(" at: "));
      Serial.println(assembler.errorAt());
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
    line = "";
    line_ready = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') {
      line_ready = true;
    } else {
      line += c;
    }
  }
}
