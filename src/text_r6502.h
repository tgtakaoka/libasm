#ifndef __TEXT_R6502_H__
#define __TEXT_R6502_H__

#include "config_host.h"

// R6502 instructions

extern const char TEXT_ADC[] PROGMEM = "ADC";
extern const char TEXT_AND[] PROGMEM = "AND";
static const char TEXT_ASL[] PROGMEM = "ASL";
static const char TEXT_BCC[] PROGMEM = "BCC";
static const char TEXT_BCS[] PROGMEM = "BCS";
static const char TEXT_BEQ[] PROGMEM = "BEQ";
extern const char TEXT_BIT[] PROGMEM = "BIT";
static const char TEXT_BMI[] PROGMEM = "BMI";
static const char TEXT_BNE[] PROGMEM = "BNE";
static const char TEXT_BPL[] PROGMEM = "BPL";
static const char TEXT_BRK[] PROGMEM = "BRK";
static const char TEXT_BVC[] PROGMEM = "BVC";
static const char TEXT_BVS[] PROGMEM = "BVS";
static const char TEXT_CLC[] PROGMEM = "CLC";
static const char TEXT_CLD[] PROGMEM = "CLD";
static const char TEXT_CLI[] PROGMEM = "CLI";
static const char TEXT_CLV[] PROGMEM = "CLV";
extern const char TEXT_CMP[] PROGMEM = "CMP";
static const char TEXT_CPX[] PROGMEM = "CPX";
static const char TEXT_CPY[] PROGMEM = "CPY";
extern const char TEXT_DEC[] PROGMEM = "DEC";
static const char TEXT_DEX[] PROGMEM = "DEX";
static const char TEXT_DEY[] PROGMEM = "DEY";
extern const char TEXT_EOR[] PROGMEM = "EOR";
extern const char TEXT_INC[] PROGMEM = "INC";
static const char TEXT_INX[] PROGMEM = "INX";
static const char TEXT_INY[] PROGMEM = "INY";
extern const char TEXT_JMP[] PROGMEM = "JMP";
static const char TEXT_JSR[] PROGMEM = "JSR";
extern const char TEXT_LDA[] PROGMEM = "LDA";
static const char TEXT_LDX[] PROGMEM = "LDX";
static const char TEXT_LDY[] PROGMEM = "LDY";
static const char TEXT_LSR[] PROGMEM = "LSR";
static const char TEXT_NOP[] PROGMEM = "NOP";
extern const char TEXT_ORA[] PROGMEM = "ORA";
static const char TEXT_PHA[] PROGMEM = "PHA";
static const char TEXT_PHP[] PROGMEM = "PHP";
static const char TEXT_PLA[] PROGMEM = "PLA";
static const char TEXT_PLP[] PROGMEM = "PLP";
static const char TEXT_ROL[] PROGMEM = "ROL";
static const char TEXT_ROR[] PROGMEM = "ROR";
static const char TEXT_RTI[] PROGMEM = "RTI";
static const char TEXT_RTS[] PROGMEM = "RTS";
extern const char TEXT_SBC[] PROGMEM = "SBC";
extern const char TEXT_SEC[] PROGMEM = "SEC";
static const char TEXT_SED[] PROGMEM = "SED";
static const char TEXT_SEI[] PROGMEM = "SEI";
extern const char TEXT_STA[] PROGMEM = "STA";
static const char TEXT_STX[] PROGMEM = "STX";
static const char TEXT_STY[] PROGMEM = "STY";
static const char TEXT_TAX[] PROGMEM = "TAX";
static const char TEXT_TAY[] PROGMEM = "TAY";
static const char TEXT_TSX[] PROGMEM = "TSX";
static const char TEXT_TXA[] PROGMEM = "TXA";
static const char TEXT_TXS[] PROGMEM = "TXS";
static const char TEXT_TYA[] PROGMEM = "TYA";

#endif // __TEXT_R6502_H__
