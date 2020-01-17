#include <dis_mc6809.h>
#include <str_memory.h>

DisMc6809 dis6809;
Disassembler<uint16_t> &disassembler(dis6809);

String line;
bool line_ready = false;

void setup() {
  Serial.begin(9800);
  disassembler.acceptCpu("6309");
}

void loop() {
  if (line_ready) {
    Serial.println(line);
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