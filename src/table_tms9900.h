/* -*- mode: c++; -*- */
#ifndef __TABLE_TMS9900_H__
#define __TABLE_TMS9900_H__

#include "config_tms9900.h"
#include "insn_tms9900.h"

class TableTms9900 {
public:
    Error searchName(Insn &insn) const;
    Error searchInsnCode(Insn &insn) const;

    void setMcuType(McuType mcuType) { _mcuType = mcuType; }
    bool is9995() const { return _mcuType == TMS9995; }

private:
    McuType _mcuType;
};

extern TableTms9900 TableTms9900;
#endif // __TABLE_TMS9900_H__
