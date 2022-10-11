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

#include "asm_mos6502.h"

namespace libasm {
namespace mos6502 {

const char AsmMos6502::OPT_BOOL_LONGA[] PROGMEM = "longa";
const char AsmMos6502::OPT_DESC_LONGA[] PROGMEM = "enable 16-bit accumulator";
const char AsmMos6502::OPT_BOOL_LONGI[] PROGMEM = "longi";
const char AsmMos6502::OPT_DESC_LONGI[] PROGMEM = "enable 16-bit index registers";

Error AsmMos6502::encodeRelative(InsnMos6502 &insn, AddrMode mode, const Operand &op) {
    const Config::uintptr_t bank = insn.address() & ~0xFFFF;
    const uint16_t base = insn.address() + insn.length() + (mode == REL ? 1 : 2);
    const uint16_t target = op.getError() ? base : op.val32;
    const int16_t delta = target - base;
    if (addressWidth() == ADDRESS_24BIT && op.isOK() && (op.val32 & ~0xFFFF) != bank) {
    too_far:
        insn.reset();
        return setError(op, OPERAND_TOO_FAR);
    }
    if (mode == REL) {
        if (overflowRel8(delta))
            goto too_far;
        insn.emitByte(delta);
    } else {
        insn.emitUint16(delta);
    }
    return OK;
}

Error AsmMos6502::selectMode(char size, Operand &op, AddrMode zp, AddrMode abs, AddrMode labs) const {
    if (size == '}') {
        if (labs == IMPL)
            return op.setError(UNKNOWN_OPERAND);
        op.mode = labs;
    } else if (size == '>') {
        op.mode = abs;
    } else if (size == '<') {
        op.mode = zp;
    } else if (op.val32 >= 0x10000L) {
        op.mode = labs;
    } else {
        op.mode = (op.val32 < 0x100) ? zp : abs;
    }
    return OK;
}

static char parseIndirect(StrScanner &p) {
    if (p.expect('('))
        return ')';
    if (p.expect('['))
        return ']';
    return 0;
}

static char parseSizeOverride(StrScanner &p) {
    if (p.expect('<'))
        return '<';
    if (p.expect('>')) {
        if (p.expect('>'))
            return '}';  // >>
        return '>';
    }
    return 0;
}

Error AsmMos6502::parseOperand(StrScanner &scan, Operand &op, Operand &extra) const {
    StrScanner p(scan.skipSpaces());
    op.setAt(p);
    if (endOfLine(*p))
        return OK;

    if (p.expect('#')) {  // #nn
        op.val32 = parseExpr32(p, op);
        if (parserError())
            return getError();
        op.mode = IMMA;
        scan = p;
        return OK;
    }
    StrScanner a(p);
    const RegName reg = RegMos6502::parseRegName(a);
    if (reg != REG_UNDEF) {
        if (reg == REG_A) {
            op.mode = ACCM;
            scan = a;
            return OK;
        }
        return op.setError(p, REGISTER_NOT_ALLOWED);
    }

    const char indir = parseIndirect(p);
    const char size = parseSizeOverride(p);
    op.val32 = parseExpr32(p, op);
    if (parserError())
        return op.getError();

    RegName base = REG_UNDEF;
    RegName index = REG_UNDEF;
    bool hasExtra = false;
    StrScanner basep;
    StrScanner indexp;
    if (p.skipSpaces().expect(',')) {  // val,extra val,base (val,base) (val,base),index
        basep = p.skipSpaces();
        base = RegMos6502::parseRegName(p);
        if (base == REG_UNDEF) {
            extra.val32 = parseExpr32(p, extra);
            if (parserError())
                return extra.getError();
            hasExtra = true;
        }
        if (indir && !p.skipSpaces().expect(indir))
            return op.setError(p, indir == ')' ? MISSING_CLOSING_PAREN : MISSING_CLOSING_BRACKET);
        if (p.skipSpaces().expect(',')) {
            indexp = p.skipSpaces();
            index = RegMos6502::parseRegName(p);
            if (index == REG_UNDEF)
                return op.setError(p, UNKNOWN_OPERAND);
        }
    } else if (indir) {  // (val) (val),index
        if (!p.expect(indir))
            return op.setError(p, indir == ')' ? MISSING_CLOSING_PAREN : MISSING_CLOSING_BRACKET);
        if (p.skipSpaces().expect(',')) {
            indexp = p.skipSpaces();
            index = RegMos6502::parseRegName(p);
            if (index == REG_UNDEF)
                return op.setError(p, UNKNOWN_OPERAND);
        }
    }

    scan = p.skipSpaces();
    if (base == REG_UNDEF) {
        if (index == REG_UNDEF) {
            if (hasExtra) {
                if (indir)
                    return op.setError(UNKNOWN_OPERAND);
                op.mode = ZPG_REL;  // zp,rel
                return OK;
            }
            switch (indir) {
            case ')':  // (abs) (zp)
                return selectMode(size, op, ZPG_IDIR, ABS_IDIR);
            case ']':  // [abs] [zp]
                return selectMode(size, op, ZPG_IDIR_LONG, ABS_IDIR_LONG);
            default:  // labs abs zp
                return selectMode(size, op, ZPG, ABS, ABS_LONG);
            }
        }
        if (indir == 0)
            return op.setError(UNKNOWN_OPERAND);
        if (index == REG_Y) {  // (zp),Y [zp],Y
            op.mode = (indir == ']') ? ZPG_IDIR_LONG_IDY : ZPG_IDIR_IDY;
            return OK;
        }
        return op.setError(indexp, REGISTER_NOT_ALLOWED);
    }
    switch (indir) {
    case 0:
        if (index != REG_UNDEF)
            return op.setError(REGISTER_NOT_ALLOWED);
        switch (base) {
        case REG_X:  // labs,X abs,X zp,X
            return selectMode(size, op, ZPG_IDX, ABS_IDX, ABS_LONG_IDX);
        case REG_Y:  // abs,Y zp,Y
            return selectMode(size, op, ZPG_IDY, ABS_IDY);
        case REG_S:  // off,S
            op.mode = SP_REL;
            return OK;
        default:
            return op.setError(basep, REGISTER_NOT_ALLOWED);
        }
    case ')':
        switch (base) {
        case REG_X:  // (abs,X) (zp,X)
            if (index == REG_UNDEF)
                return selectMode(size, op, ZPG_IDX_IDIR, ABS_IDX_IDIR);
            return op.setError(indexp, REGISTER_NOT_ALLOWED);
        case REG_S:  // (off,S),Y
            if (index == REG_UNDEF)
                return op.setError(UNKNOWN_OPERAND);
            if (index == REG_Y) {
                op.mode = SP_REL_IDIR_IDY;
                return OK;
            }
            return op.setError(indexp, REGISTER_NOT_ALLOWED);
        default:
            return op.setError(basep, REGISTER_NOT_ALLOWED);
        }
    default:
        return op.setError(UNKNOWN_OPERAND);
    }
}

Error AsmMos6502::parseTableOnOff(StrScanner &scan, bool (TableMos6502::*set)(bool val)) {
    StrScanner p(scan.skipSpaces());
    const StrScanner name = _parser.readSymbol(p);
    if (name.iequals_P(PSTR("on"))) {
        if (!(TableMos6502::TABLE.*set)(true))
            setError(scan, OPERAND_NOT_ALLOWED);
    } else if (name.iequals_P(PSTR("off"))) {
        if (!(TableMos6502::TABLE.*set)(false))
            setError(scan, OPERAND_NOT_ALLOWED);
    } else {
        setError(scan, UNKNOWN_OPERAND);
        return OK;
    }
    scan = p;
    return OK;
}

Error AsmMos6502::processPseudo(StrScanner &scan, const char *name) {
    if (strcasecmp_P(name, PSTR("longa")) == 0)
        return parseTableOnOff(scan, &TableMos6502::setLongAccumulator);
    if (strcasecmp_P(name, PSTR("longi")) == 0)
        return parseTableOnOff(scan, &TableMos6502::setLongIndex);
    return UNKNOWN_INSTRUCTION;
}

Error AsmMos6502::encodeImpl(StrScanner &scan, Insn &_insn) {
    InsnMos6502 insn(_insn);
    insn.nameBuffer().text(_parser.readSymbol(scan));

    if (processPseudo(scan, insn.name()) == OK)
        return getError();

    Operand op, extra;
    if (parseOperand(scan, op, extra) && (op.hasError() || extra.hasError()))
        return setError(op.hasError() ? op : extra);

    if (!endOfLine(*scan.skipSpaces()))
        return setError(scan, GARBAGE_AT_END);
    setErrorIf(op);
    setErrorIf(extra);

    insn.setAddrMode(op.mode);
    const auto error = TableMos6502::TABLE.searchName(insn);
    if (error)
        return setError(op, error);

    const AddrMode mode = insn.addrMode();
    switch (mode) {
    case IMPL:
    case ACCM:
        insn.emitInsn();
        break;
    case REL:
    case REL_LONG:
        insn.emitInsn();
        encodeRelative(insn, mode, op);
        break;
    case ZPG_REL:
        insn.emitInsn();
        insn.emitByte(op.val32);
        encodeRelative(insn, REL, extra);
        break;
    case BLOCK_MOVE:
        insn.emitInsn();
        insn.emitByte(extra.val32 >> 16);
        insn.emitByte(op.val32 >> 16);
        break;
    case IMMA:
        insn.emitInsn();
        if (TableMos6502::TABLE.longAccumulator()) {
            insn.emitUint16(op.val32);
        } else {
            insn.emitByte(op.val32);
        }
        break;
    case IMMX:
        insn.emitInsn();
        if (TableMos6502::TABLE.longIndex()) {
            insn.emitUint16(op.val32);
        } else {
            insn.emitByte(op.val32);
        }
        break;
    case IMM8:
    case ZPG:
    case ZPG_IDX:
    case ZPG_IDY:
    case ZPG_IDX_IDIR:
    case ZPG_IDIR_IDY:
    case ZPG_IDIR:
    case SP_REL:
    case SP_REL_IDIR_IDY:
    case ZPG_IDIR_LONG:
    case ZPG_IDIR_LONG_IDY:
        insn.emitInsn();
        insn.emitByte(op.val32);
        break;
    case ABS:
    case ABS_IDX:
    case ABS_IDY:
    case ABS_IDIR:
    case ABS_IDX_IDIR:
    case ABS_IDIR_LONG:
        insn.emitInsn();
        insn.emitUint16(op.val32);
        break;
    case ABS_LONG:
    case ABS_LONG_IDX:
        insn.emitInsn();
        insn.emitUint16(op.val32);
        insn.emitByte(op.val32 >> 16);
        break;
    }
    return getError();
}

}  // namespace mos6502
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
