#include "config_mc6800.h"
#include "reg_mc6800.h"
#include "table_mc6800.h"

#include <ctype.h>

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

static bool regCharCaseEqual(char c, char regChar) {
    return toupper(c) == toupper(regChar);
}

char RegMc6800::regName1stChar(const RegName regName) const {
    const char r = char(regName);
    return _uppercase ? toupper(r) : tolower(r);
}

bool RegMc6800::compareRegName(const char *line, RegName regName) const {
    if (!regCharCaseEqual(*line++, regName1stChar(regName))) return false;
    return !isidchar(*line);
}

host::uint_t RegMc6800::regNameLen(RegName regName) const {
    return regName == REG_UNDEF ? 0 : 1;
}

char *RegMc6800::outRegName(char *out, const RegName regName) const {
    *out++ = regName1stChar(regName);
    *out = 0;
    return out;
}

RegName RegMc6800::parseRegName(const char *line) const {
    if (compareRegName(line, REG_A)) return REG_A;
    if (compareRegName(line, REG_B)) return REG_B;
    if (compareRegName(line, REG_X)) return REG_X;
    return REG_UNDEF;
}
