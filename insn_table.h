/* -*- mode: c++; -*- */
#ifndef __INSN_TABLE_H__
#define __INSN_TABLE_H__

#include "config_host.h"
#include "config_target.h"
#include "error_reporter.h"
#include "insn_entry.h"

class Insn;

class InsnTable {
public:
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

#endif // __INSN_TABLE_H__
