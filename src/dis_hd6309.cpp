#include "dis_hd6309.h"

#include "string_utils.h"
#include "table_hd6309.h"

static const RegName TABLE_INDEX_REGS[] = {
    NONE, NONE, NONE, NONE, NONE, B, A, E, NONE, NONE, F, D, NONE, NONE, W
};
constexpr host::uindex_t LENGTH_INDEXES =
    sizeof(TABLE_INDEX_REGS) / sizeof(TABLE_INDEX_REGS[0]);
static const RegName TABLE_REGISTERS[] = {
    D, X, Y, U, S, PC, W, V, A, B, CC, DP, ZERO, ZERO, E, F
};
constexpr host::uindex_t LENGTH_REGISTERS =
    sizeof(TABLE_REGISTERS) / sizeof(TABLE_REGISTERS[0]);

static const RegName TABLE_BIT_OP_REGS[] = { CC, A, B, NONE };

static const RegName TABLE_TFM_REGS[] = { D, X, Y, U, S };
constexpr host::uindex_t LENGTH_TFM_REGS =
    sizeof(TABLE_TFM_REGS) / sizeof(TABLE_TFM_REGS[0]);
static const char TABLE_TFM_SRC_MODES[4] = { '+', '-', '+',   0 };
static const char TABLE_TFM_DST_MODES[4] = { '+', '-',   0, '+' };

static char *outOpr32Hex(char *out, uint32_t val) {
    *out++ = '$';
    return outHex32(out, val);
}

Error DisHd6309::readUint32(Memory &memory, Insn &insn, target::uint32_t &val) {
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val = (target::uint32_t)memory.readByte() << 24;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint32_t)memory.readByte() << 16;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= (target::uint16_t)memory.readByte() << 8;
    if (!memory.hasNext()) return setError(NO_MEMORY);
    val |= memory.readByte();
    emitUint32(insn, val);
    return OK;
}

RegName DisHd6309::decodeIndexReg(target::byte_t regNum) const {
    if (regNum >= LENGTH_INDEXES) return NONE;
    return TABLE_INDEX_REGS[regNum];
}

RegName DisHd6309::decodeRegName(target::byte_t regNum) const {
    if (regNum >= LENGTH_REGISTERS) return NONE;
    return TABLE_REGISTERS[regNum];
}

Error DisHd6309::decodeIndexedExtra(
    Memory &memory, Insn &insn, target::byte_t post,
    RegName &index, target::uintptr_t &addr, host::int_t &offSize,
    RegName &base, host::int_t &incr) {
    if (post == 0x8F || post == 0x90) {
        // ,W [,W]
        base = W;
        return setError(OK);
    }
    if (post == 0xAF || post == 0xB0) {
        // n16,W [n16,W]
        offSize = 16;
        base = W;
        readUint16(memory, insn, addr);
        return setError(OK);
    }
    if (post == 0xCF || post == 0xD0 || post == 0xEF || post == 0xF0) {
        // ,W++ ,--W [,W++] [,--W]
        base = W;
        incr = (post < 0xE0) ? 2 : -2;
        return setError(OK);
    }
    return UNKNOWN_INSTRUCTION;     // bypass
}

Error DisHd6309::decodeImmediateExtra(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    if (insn.oprLen() == 4) {
        target::uint32_t val;
        if (readUint32(memory, insn, val)) return OK; // processed
        *operands++ = '#';
        outOpr32Hex(operands, val);
        outInt32(comments, val);
        return setError(OK);            // processed
    }
    return UNKNOWN_INSTRUCTION;         // bypass
}

Error DisHd6309::decodeImmediatePlus(
    Memory& memory, Insn &insn, char *operands, char *comments) {
    *operands++ = '#';
    target::byte_t val;
    if (readByte(memory, insn, val)) return getError();
    operands = outOpr8Hex(operands, val);
    *operands++ = ',';
    switch (insn.addrMode()) {
    case IMM_DIRECT:
        return decodeDirectPage(memory, insn, operands, comments);
    case IMM_EXTENDED:
        return decodeExtended(memory, insn, operands, comments);
    case IMM_INDEXED:
        return decodeIndexed(memory, insn, operands, comments);
    default:
        return setError(INTERNAL_ERROR);
    }
}

Error DisHd6309::decodeBitPosition(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const RegName reg = TABLE_BIT_OP_REGS[post >> 6];
    if (reg == NONE) return setError(ILLEGAL_REGISTER);
    operands = outRegName(operands, reg);
    *operands++ = ',';
    const host::uint_t dstBit = (post >> 3) & 0x7;
    operands = outInt16(operands, dstBit);
    *operands++ = ',';
    operands = outInt16(operands, post & 0x7);
    if (reg == 'C') {
        comments = outRegName(comments, reg);
        *comments++ = '.';
        comments = outCCRBits(comments, 1 << dstBit);
        *comments++ = ' ';
        *comments = 0;
    }
    *operands++ = ',';
    return decodeDirectPage(memory, insn, operands, comments);
}

static RegName decodeTfmReg(host::uint_t num) {
    return num < LENGTH_TFM_REGS ? TABLE_TFM_REGS[num] : NONE;
}

Error DisHd6309::decodeTransferMemory(
    Memory &memory, Insn &insn, char *operands, char *comments) {
    target::byte_t post;
    if (readByte(memory, insn, post)) return getError();
    const RegName src = decodeTfmReg(post >> 4);
    const RegName dst = decodeTfmReg(post & 0xf);
    const uint8_t mode = insn.insnCode() & 0x3;
    if (src == NONE || dst == NONE) return setError(ILLEGAL_REGISTER);
    operands = outRegName(operands, src);
    if (TABLE_TFM_SRC_MODES[mode]) *operands++ = TABLE_TFM_SRC_MODES[mode];
    *operands++ = ',';
    operands = outRegName(operands, dst);
    if (TABLE_TFM_DST_MODES[mode]) *operands++ = TABLE_TFM_DST_MODES[mode];
    *operands = 0;
    return setError(OK);
}

Error DisHd6309::decode(
    Memory &memory, Insn &insn, char *operands, char *comments,
    SymbolTable *symtab) {

    if (DisMc6809::decode(memory, insn, operands, comments, symtab) == OK)
        return OK;
    *operands = *comments = 0;

    if (TableHd6309.search(insn, insn.insnCode()))
        return setError(UNKNOWN_INSTRUCTION);

    switch (insn.addrMode()) {
    case INHERENT:
        return setError(OK);
    case DIRECT_PG:
        return decodeDirectPage(memory, insn, operands, comments);
    case EXTENDED:
        return decodeExtended(memory, insn, operands, comments);
    case INDEXED:
        return decodeIndexed(memory, insn, operands, comments);
    case IMMEDIATE:
        return decodeImmediate(memory, insn, operands, comments);
    case REGISTERS:
        return decodeRegisters(memory, insn, operands, comments);
    case IMM_DIRECT:
    case IMM_EXTENDED:
    case IMM_INDEXED:
        return decodeImmediatePlus(memory, insn, operands, comments);
    case BIT_OPERATION:
        return decodeBitPosition(memory, insn, operands, comments);
    case TRANSFER_MEM:
        return decodeTransferMemory(memory, insn, operands, comments);
    default:
        return setError(INTERNAL_ERROR);
    }
}
