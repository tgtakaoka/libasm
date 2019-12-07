/* -*- mode: c++; -*- */
#ifndef __DIS_MEMORY_H__
#define __DIS_MEMORY_H__

class DisMemory {
public:
    virtual bool hasNext() const = 0;
    target::uintptr_t address() const { return _address; }
    uint8_t readByte() {
        const uint8_t val = nextByte();
        _address++;
        return val;
    }

protected:
    target::uintptr_t _address;

    DisMemory(target::uintptr_t address) : _address(address) {}
    virtual uint8_t nextByte() = 0;
};

#endif
