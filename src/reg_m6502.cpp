#include <ctype.h>

#include "config_m6502.h"
#include "reg_m6502.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

host::uint_t RegM6502::regNameLen(RegName regName) const {
    return regName == REG_UNDEF ? 0 : 1;
}

bool RegM6502::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, char(regName)))return false;
    return !isidchar(*line);
}

RegName RegM6502::parseIndexReg(const char *line) const {
    if (compareRegName(line, REG_X)) return REG_X;
    if (compareRegName(line, REG_Y)) return REG_Y;
    return REG_UNDEF;
}

char *RegM6502::outRegName(char *out, const RegName regName) const {
    const char r = char(regName);
    *out++ = _uppercase ? r : tolower(r);
    *out = 0;
    return out;
}
