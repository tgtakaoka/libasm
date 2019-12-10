/* -*- mode: c++; -*- */
#ifndef __DIS_MEMORY_H__
#define __DIS_MEMORY_H__

template<typename Addr>
class DisMemory {
public:
    virtual bool hasNext() const = 0;
    Addr address() const { return _address; }
    uint8_t readByte() {
        const uint8_t val = nextByte();
        _address++;
        return val;
    }

protected:
    Addr _address;

    DisMemory(Addr address) : _address(address) {}
    virtual uint8_t nextByte() = 0;
};

#endif
