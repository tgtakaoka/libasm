/*
 * Copyright 2020 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "asm_base.h"

#include <ctype.h>

#include "symbol_table.h"
#include "value_parser.h"

namespace libasm {

Error Assembler::encode(const char *line, Insn &insn, uint32_t addr, SymbolTable *symtab) {
    _symtab = symtab;
    resetError();
    const uint8_t addrUnit = static_cast<uint8_t>(addressUnit());
    insn.resetAddress(addr / addrUnit);
    const char *scan = skipSpaces(line);
    if (endOfLine(scan))
        return OK;
    _scan = scan;
    return encode(insn);
}

bool Assembler::hasSymbol(const char *symbol) const {
    return _symtab && _symtab->hasSymbol(symbol);
}

uint32_t Assembler::lookupSymbol(const char *symbol) const {
    return _symtab ? _symtab->lookupSymbol(symbol) : 0;
}

bool Assembler::endOfLine(const char *scan) const {
    return *scan == 0 || *scan == ';' || *scan == _commentChar;
}

uint16_t Assembler::parseExpr16(const char *expr, const char *end) {
    Value value;
    _scan = _parser.eval(expr, end, value, _symtab);
    setError(_parser.error());
    if (value.overflowUint16())
        setErrorIf(OVERFLOW_RANGE);
    if (value.isUndefined())
        setErrorIf(UNDEFINED_SYMBOL);
    return value.getUnsigned();
}

uint32_t Assembler::parseExpr32(const char *expr, const char *end) {
    Value value;
    _scan = _parser.eval(expr, end, value, _symtab);
    setError(_parser.error());
    if (value.isUndefined())
        setErrorIf(UNDEFINED_SYMBOL);
    return value.getUnsigned();
}

const char *Assembler::scanExpr(const char *expr, char delim, uint16_t nesting) const {
    nesting++;
    while (!endOfLine(expr)) {
        const char c = *expr;
        if (c == delim)
            return expr;
        expr++;
        if (c == '(') {
            expr = scanExpr(expr, ')', nesting);
        } else if (c == '[') {
            expr = scanExpr(expr, ']', nesting);
        } else if (c == '\'') {
            char val;
            expr = _parser.readChar(expr, val);
            if (*expr == '\'')
                expr++;
        }
    }
    return expr;
}

const char *Assembler::skipSpaces(const char *scan) {
    while (*scan == ' ' || *scan == '\t')
        scan++;
    return scan;
}

bool Assembler::overflowRel8(int16_t s16) {
    return s16 < -128 || s16 >= 128;
}

bool Assembler::overflowRel8(int32_t sval32) {
    return sval32 < -128 || sval32 >= 128;
}

bool Assembler::overflowUint8(uint16_t u16) {
    const int16_t s16 = static_cast<int16_t>(u16);
    return s16 < -128 || (s16 >= 0 && u16 >= 256);
}

bool Assembler::overflowUint8(uint32_t u32) {
    const int32_t s32 = static_cast<int32_t>(u32);
    return s32 < -128 || (s32 >= 0 && u32 >= 256);
}

bool Assembler::overflowRel16(int32_t s32) {
    return s32 < -32768L || s32 >= 32768L;
}

bool Assembler::overflowUint16(uint32_t u32) {
    const int32_t s32 = static_cast<int32_t>(u32);
    return s32 < -32768L || (s32 >= 0 && u32 >= 65536UL);
}

}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
