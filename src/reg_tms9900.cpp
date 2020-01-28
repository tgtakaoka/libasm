#include <ctype.h>

#include "config_tms9900.h"
#include "reg_tms9900.h"

static bool isidchar(const char c) {
    return isalnum(c) || c == '_';
}

host::uint_t RegTms9900::regNameLen(RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return 2;
    if (isalpha(r)) return 3;
    return 0;
}

RegName RegTms9900::parseRegName(const char *line) const {
    if (toupper(*line++) != 'R')
        return REG_UNDEF;
    if (isdigit(*line) && !isidchar(line[1]))
        return RegName(*line);
    if (*line++ == '1' && (*line >= '0' && *line < '6') && !isidchar(line[1]))
        return RegName('A' + (*line - '0'));
    return REG_UNDEF;
}

char *RegTms9900::outRegName(char *out, host::uint_t regno) const {
    regno &= 0x0f;
    *out++ = _uppercase ? 'R' : 'r';
    if (regno < 10) {
        *out++ = regno + '0';
    } else {
        *out++ = '1';
        *out++ = regno - 10 + '0';
    }
    *out = 0;
    return out;
}

uint16_t RegTms9900::encodeRegNumber(RegName regName) const {
    const char r = char(regName);
    if (isdigit(r)) return r - '0';
    return r - 'A' + 10;
}
