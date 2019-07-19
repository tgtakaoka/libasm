/* -*- mode: c++; -*- */
#ifndef __MEMORY_H__
#define __MEMORY_H__

class Memory {
public:
    virtual bool hasNext() const = 0;
    target::uintptr_t address() const { return _address; }
    target::byte_t readByte() {
        const target::byte_t val = nextByte();
        _address++;
        return val;
    }

protected:
    target::uintptr_t _address;

    Memory(target::uintptr_t address) : _address(address) {}
    virtual target::byte_t nextByte() = 0;
};

#endif
