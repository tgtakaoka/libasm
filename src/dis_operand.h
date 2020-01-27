/* -*- mode: c++; -*- */
#ifndef _DIS_OPERAND_H__
#define _DIS_OPERAND_H__

#include <stdint.h>

class DisOperand {
public:
    virtual char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, uint8_t size) const = 0;
    void setUppercase(bool uppercase) { _hexBase = uppercase ? 'A' : 'a'; }

protected:
    char _hexBase = 'A';

    char *outputNumber(
        char *p, uint32_t val, int8_t radix, uint8_t size) const;
    char *outputRelaxed(
        char *p, uint32_t val, int8_t radix, uint8_t size) const;
};

class DisMotoOperand : public DisOperand {
public:
    char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, uint8_t size) const override;
};

class DisIntelOperand : public DisOperand {
public:
    char *output(
        char *p, uint32_t val, int8_t radix,
        bool relax, uint8_t size) const override;
};

#endif
