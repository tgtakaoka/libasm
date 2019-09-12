#ifndef __TEXT_R65C02_H__
#define __TEXT_R65C02_H__

#include "config_host.h"

// R6502 instructions

extern const char TEXT_ADC[] PROGMEM;
extern const char TEXT_AND[] PROGMEM;
extern const char TEXT_BIT[] PROGMEM;
extern const char TEXT_CMP[] PROGMEM;
extern const char TEXT_DEC[] PROGMEM;
extern const char TEXT_EOR[] PROGMEM;
extern const char TEXT_INC[] PROGMEM;
extern const char TEXT_JMP[] PROGMEM;
extern const char TEXT_LDA[] PROGMEM;
extern const char TEXT_ORA[] PROGMEM;
extern const char TEXT_SBC[] PROGMEM;
extern const char TEXT_STA[] PROGMEM;

// R65C02 instructions

static const char TEXT_BBR0[] PROGMEM = "BBR0";
static const char TEXT_BBR1[] PROGMEM = "BBR1";
static const char TEXT_BBR2[] PROGMEM = "BBR2";
static const char TEXT_BBR3[] PROGMEM = "BBR3";
static const char TEXT_BBR4[] PROGMEM = "BBR4";
static const char TEXT_BBR5[] PROGMEM = "BBR5";
static const char TEXT_BBR6[] PROGMEM = "BBR6";
static const char TEXT_BBR7[] PROGMEM = "BBR7";
static const char TEXT_BBS0[] PROGMEM = "BBS0";
static const char TEXT_BBS1[] PROGMEM = "BBS1";
static const char TEXT_BBS2[] PROGMEM = "BBS2";
static const char TEXT_BBS3[] PROGMEM = "BBS3";
static const char TEXT_BBS4[] PROGMEM = "BBS4";
static const char TEXT_BBS5[] PROGMEM = "BBS5";
static const char TEXT_BBS6[] PROGMEM = "BBS6";
static const char TEXT_BBS7[] PROGMEM = "BBS7";
static const char TEXT_BRA[] PROGMEM = "BRA";
static const char TEXT_PHX[] PROGMEM = "PHX";
static const char TEXT_PHY[] PROGMEM = "PHY";
static const char TEXT_PLX[] PROGMEM = "PLX";
static const char TEXT_PLY[] PROGMEM = "PLY";
static const char TEXT_RMB0[] PROGMEM = "RMB0";
static const char TEXT_RMB1[] PROGMEM = "RMB1";
static const char TEXT_RMB2[] PROGMEM = "RMB2";
static const char TEXT_RMB3[] PROGMEM = "RMB3";
static const char TEXT_RMB4[] PROGMEM = "RMB4";
static const char TEXT_RMB5[] PROGMEM = "RMB5";
static const char TEXT_RMB6[] PROGMEM = "RMB6";
static const char TEXT_RMB7[] PROGMEM = "RMB7";
static const char TEXT_SMB0[] PROGMEM = "SMB0";
static const char TEXT_SMB1[] PROGMEM = "SMB1";
static const char TEXT_SMB2[] PROGMEM = "SMB2";
static const char TEXT_SMB3[] PROGMEM = "SMB3";
static const char TEXT_SMB4[] PROGMEM = "SMB4";
static const char TEXT_SMB5[] PROGMEM = "SMB5";
static const char TEXT_SMB6[] PROGMEM = "SMB6";
static const char TEXT_SMB7[] PROGMEM = "SMB7";
static const char TEXT_STP[] PROGMEM = "STP";
static const char TEXT_STZ[] PROGMEM = "STZ";
static const char TEXT_TRB[] PROGMEM = "TRB";
static const char TEXT_TSB[] PROGMEM = "TSB";
static const char TEXT_WAI[] PROGMEM = "WAI";

#endif // __TEXT_R65C02_H__
