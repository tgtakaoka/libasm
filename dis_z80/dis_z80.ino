#include <string.h>

#include "dis_z80.h"

DisZ80 disassembler;

class LineMemory : public Memory {
  public:
    LineMemory(uint16_t addr, String line) : Memory(addr) {
      line.trim();
      reset(addr);
      int i = 0, val = 0;
      while (i < line.length()) {
        const char c = line.charAt(i++);
        if (isWhitespace(c)) {
          addByte(val);
          val = 0;
        } else if (isHexadecimalDigit(c)) {
          val <<= 4;
          if (isUpperCase(c)) val += c - 'A' + 10;
          else if (isLowerCase(c)) val += c - 'a' + 10;
          else val += c - '0';
        }
      }
      addByte(val);
    }
    bool hasNext() const override {
      return _rp < _end;
    }
    void reset(uint16_t addr) {
      _address = addr;
      _wp = _rp = 0;
    }
    void addByte(uint8_t val) {
      if (_wp < _end) _bytes[_wp++] = val;
    }
  protected:
    uint8_t nextByte() {
      return _rp < _end ? _bytes[_rp++] : 0;
    }
  private:
    uint8_t _bytes[8];
    static constexpr uint8_t _end = sizeof(_bytes);
    uint8_t _wp;
    uint8_t _rp;
};

void printHex8(uint8_t val) {
  if (val < 0x10) Serial.print('0');
  Serial.print(val, HEX);
}

void printHex16(uint16_t val) {
  printHex8(val >> 8);
  printHex8(val);
}

void print(const char *text, int width) {
  Serial.print(text);
  for (int i = strlen(text); i < width; i++)
    Serial.print(' ');
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  while (Serial.available()) {
    String line = Serial.readString();
    LineMemory memory(0x1000, line);
    char operands[20];
    Insn insn;
    if (disassembler.decode(memory, insn, operands, nullptr)) {
      Serial.print(F("Error "));
      Serial.println(disassembler.getError(), DEC);
    } else {
      printHex16(insn.address());;
      Serial.print(':');
      for (int i = 0; i < insn.insnLen(); i++) {
        Serial.print(' ');
        printHex8(insn.bytes()[i]);
      }
      for (int i = insn.insnLen(); i < 5; i++)
        Serial.print(F("   "));
      print(insn.name(), 6);
      print(operands, 12);
      Serial.println();
    }
  }
}
