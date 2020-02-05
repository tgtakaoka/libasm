/* -*- mode: c++; -*- */
#ifndef __DIS_LISTING_H__
#define __DIS_LISTING_H__

#include "asm_listing.h"

#include <string>

template<typename Addr, bool wordBase>
class DisListing : public AsmLine<Addr> {
public:
    DisListing(
        Disassembler<Addr>&disassembler,
        CliMemory<Addr> &memory,
        bool uppercase = false)
        : _disassembler(disassembler),
          _memory(memory),
          _listing(),
          _uppercase(uppercase),
          _labelWidth(8),
          _operandWidth(8)
    {}

    Error disassemble(Addr addr, Insn &insn) {
        _memory.setAddress(addr);
        const Error error = _disassembler.decode(
            _memory, insn, _operands, nullptr, _uppercase);
        _listing.reset(*this,  wordBase, _uppercase);
        _address = addr;
        _generated_size = insn.insnLen();
        _instruction = insn.name();
        return error;
    }

    const char *origin(Addr origin, bool withBytes = false) {
        _disassembler.getFormatter().output(
            _operands, origin, 16, false, sizeof(Addr));
        _listing.reset(*this, wordBase, _uppercase);
        _address = origin;
        _generated_size = 0;
        _instruction = _uppercase ? "ORG" : "org";
        return withBytes ? _listing.getLine() : _listing.getContent();
    }

    bool hasNext() const { return  _listing.hasNext(); }
    const char *getLine() { return _listing.getLine(); }
    const char *getContent() { return _listing.getContent(); }

private:
    Disassembler<Addr> &_disassembler;
    CliMemory<Addr> &_memory;
    AsmListing<Addr> _listing;
    bool _uppercase;
    int _labelWidth;
    int _operandWidth;
    Addr _address;
    int _generated_size;
    const char *_instruction;
    char _operands[40];

    // AsmLine<Addr>
    Addr startAddress() const override { return _address; }
    int generatedSize() const override { return _generated_size; }
    uint8_t getByte(int offset) const override {
        uint8_t val = 0;
        _memory.readByte(_address + offset, val);
        return val;
    }
    bool hasLabel() const override { return false; }
    bool hasInstruction() const override { return _instruction; }
    bool hasOperand() const override { return *_operands; }
    bool hasComment() const override { return false; }
    std::string getLabel() const override { return ""; }
    std::string getInstruction() const override {
        return std::move(std::string(_instruction));
    }
    std::string getOperand() const override {
        return std::move(std::string(_operands));
    }
    std::string getComment() const override { return ""; }
    int maxBytes() const override { return 6; }
    int labelWidth() const override { return _labelWidth; }
    int instructionWidth() const override { return Insn::getMaxName() + 1; }
    int operandWidth() const override { return _operandWidth; }
};

#endif
