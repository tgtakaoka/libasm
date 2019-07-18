/* -*- mode: c++; -*- */
#ifndef __TABLE_MC6809_H__
#define __TABLE_MC6809_H__

class TableMc6809 {
public:
    Error search(Insn &insn, const char *name) const;
    Error search(Insn &insn, AddrMode mode) const;

    static target::insn_t insnCode(
        target::opcode_t prefixCode, target::opcode_t opCode) {
        return (target::insn_t(prefixCode) << 8 | opCode);
    }
    static target::opcode_t prefixCode(target::insn_t insnCode) {
        return target::opcode_t(insnCode >> 8);
    }
    static target::opcode_t opCode(target::insn_t insnCode) {
        return target::opcode_t(insnCode & 0xff);
    }
    static bool isPrefixCode(target::opcode_t opCode);
};

#endif // __TABLE_MC6809_H__
