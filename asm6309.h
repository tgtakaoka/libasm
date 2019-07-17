/* -*- mode: c++; -*- */
#ifndef __ASM6309_H__
#define __ASM6309_H__

#include "config_6309.h"
typedef AddrMode6309 AddrMode;
typedef RegName6309 RegName;
#define TARGET_ENUMS_DEFINED

#include "asm6809.h"

class Asm6309 : public Asm6809 {
protected:
    Error getOperand32(const char *&line, target::dword_t &val);
    Error encodeBitOperation(const char *line);
    Error encodeImmediatePlus(const char *line);
    Error encodeTransferMemory(const char *line);
};

#endif // __ASM6309_H__
