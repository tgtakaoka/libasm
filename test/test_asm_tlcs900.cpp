/*
 * Copyright 2026 Tadashi G. Takaoka
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "asm_tlcs900.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::tlcs900;
using namespace libasm::test;

AsmTlcs900 asm900;
Assembler &assembler(asm900);

static bool is_tlcs900() {
    return strcmp_P("TLCS900", assembler.config().cpu_P()) == 0;
}
static bool is_tlcs900l() {
    return strcmp_P("TLCS900L", assembler.config().cpu_P()) == 0;
}
static bool is_tlcs900h2() {
    return strcmp_P("TLCS900H2", assembler.config().cpu_P()) == 0;
}

void set_up() {
    assembler.reset();
    // Base TLCS900 resets to minimum mode; the operand suite exercises the
    // general 32-bit registers XWA-XHL, which exist only in maximum mode.
    // /L is minimum-mode only and keeps the reset (minimum) mode.
    if (!is_tlcs900l())
        assembler.setOption("maximum-mode", "on");
}

void tear_down() {
    symtab.reset();
}
// clang-format off
void test_cpu() {
    EQUALS("cpu tlcs900",   true, assembler.setCpu("tlcs900"));
    EQUALS_P("get cpu", "TLCS900",   assembler.config().cpu_P());
    EQUALS("cpu tlcs900l",  true, assembler.setCpu("tlcs900l"));
    EQUALS_P("get cpu", "TLCS900L",  assembler.config().cpu_P());
    EQUALS("cpu tlcs900h",  true, assembler.setCpu("tlcs900h"));
    EQUALS_P("get cpu", "TLCS900H",  assembler.config().cpu_P());
    EQUALS("cpu tlcs900l1", true, assembler.setCpu("tlcs900l1"));
    EQUALS_P("get cpu", "TLCS900L1", assembler.config().cpu_P());
    EQUALS("cpu tlcs900h2", true, assembler.setCpu("tlcs900h2"));
    EQUALS_P("get cpu", "TLCS900H2", assembler.config().cpu_P());
}

void test_single() {
    TEST("NOP",     0x00);
    if (is_tlcs900()) {
        TEST("NORMAL",  0x01);
    } else {
        ERRT("NORMAL", UNKNOWN_INSTRUCTION, "NORMAL");
    }
    TEST("PUSH SR", 0x02);
    TEST("POP SR",  0x03);
    if (is_tlcs900()) {
        TEST("MAX", 0x04);
        ERRT("MIN", UNKNOWN_INSTRUCTION, "MIN");
    } else if (is_tlcs900l()) {
        TEST("MIN", 0x04);
        ERRT("MAX", UNKNOWN_INSTRUCTION, "MAX");
    } else {
        ERRT("MAX", UNKNOWN_INSTRUCTION, "MAX");
        ERRT("MIN", UNKNOWN_INSTRUCTION, "MIN");
    }
    TEST("HALT",    0x05);
    TEST("RETI",    0x07);
    TEST("LD (034H),056H",    0x08, 0x34, 0x56);
    TEST("PUSH 034H",         0x09, 0x34);
    TEST("LDW (034H),01234H", 0x0A, 0x34, 0x34, 0x12);
    TEST("PUSHW 01234H",      0x0B, 0x34, 0x12);
    TEST("RCF",     0x10);
    TEST("SCF",     0x11);
    TEST("CCF",     0x12);
    TEST("ZCF",     0x13);
    TEST("PUSH A",  0x14);
    TEST("POP A",   0x15);
    TEST("EX F,F'", 0x16);
    TEST("INCF",    0x0C);
    TEST("DECF",    0x0D);
    if (!is_tlcs900h2()) {
        TEST("LDX (034H),056H",   0xF7, 0x00, 0x34, 0x00, 0x56, 0x00);
        TEST("LDX (01234H),056H", 0xF7, 0x00, 0x34, 0x12, 0x56, 0x00);
    } else {
        ERRT("LDX (034H),056H",   UNKNOWN_INSTRUCTION, "LDX (034H),056H");
        ERRT("LDX (01234H),056H", UNKNOWN_INSTRUCTION, "LDX (01234H),056H");
    }
    // Sub-byte 0x3C is NSP/XNSP on every variant except /L, where it is INTNEST.
    if (is_tlcs900l()) {
        TEST("LDC INTNEST,WA",  0xD8, 0x2E, 0x3C);
        TEST("LDC WA,INTNEST",  0xD8, 0x2F, 0x3C);
        ERRT("LDC NSP,SP",      REGISTER_NOT_ALLOWED, "NSP,SP");
        ERRT("LDC XNSP,XSP",    REGISTER_NOT_ALLOWED, "XNSP,XSP");
    } else {
        TEST("LDC XNSP,XSP",    0xEF, 0x2E, 0x3C);
        TEST("LDC XSP,XNSP",    0xEF, 0x2F, 0x3C);
        TEST("LDC NSP,SP",      0xDF, 0x2E, 0x3C);
        TEST("LDC SP,NSP",      0xDF, 0x2F, 0x3C);
        ERRT("LDC INTNEST,WA",  REGISTER_NOT_ALLOWED, "INTNEST,WA");
    }
    // LDC DMA control registers
    TEST("LDC DMAS0,XWA",   0xE8, 0x2E, 0x00);
    TEST("LDC DMAS1,XBC",   0xE9, 0x2E, 0x04);
    TEST("LDC DMAS3,XHL",   0xEB, 0x2E, 0x0C);
    TEST("LDC DMAD0,XWA",   0xE8, 0x2E, 0x10);
    TEST("LDC DMAD3,XHL",   0xEB, 0x2E, 0x1C);
    TEST("LDC DMAC0,WA",    0xD8, 0x2E, 0x20);
    TEST("LDC DMAC3,HL",    0xDB, 0x2E, 0x2C);
    TEST("LDC DMAM0,A",     0xC9, 0x2E, 0x22);
    TEST("LDC DMAM3,D",     0xCC, 0x2E, 0x2E);
    TEST("LDC XWA,DMAS0",   0xE8, 0x2F, 0x00);
    TEST("LDC XBC,DMAS1",   0xE9, 0x2F, 0x04);
    TEST("LDC WA,DMAC0",    0xD8, 0x2F, 0x20);
    TEST("LDC A,DMAM0",     0xC9, 0x2F, 0x22);
    TEST("LDC XHL,DMAD3",   0xEB, 0x2F, 0x1C);
    // Size mismatch errors
    ERRT("LDC DMAS0,WA",  ILLEGAL_SIZE, "WA");
    ERRT("LDC DMAC0,XWA", ILLEGAL_SIZE, "XWA");
    ERRT("LDC DMAM0,WA",  ILLEGAL_SIZE, "WA");
    ERRT("LDC WA,DMAS0",  ILLEGAL_SIZE, "WA,DMAS0");
    TEST("EI 0",    0x06, 0x00);
    TEST("EI 7",    0x06, 0x07);
    TEST("EI",      0x06, 0x00);  // bare EI defaults to level 0 (ASL convention)
    TEST("DI",      0x06, 0x07);
    TEST("LDF 0",   0x17, 0x00);
    TEST("LDF 3",   0x17, 0x03);
    // test_single runs in maximum mode (4 register banks): LDF takes 0-3.
    // The minimum-mode range (0-7) is covered by test_minmode.
    ERRT("LDF 4",   OVERFLOW_RANGE, "4", 0x17, 0x00);
    ERRT("LDF 7",   OVERFLOW_RANGE, "7", 0x17, 0x03);
    TEST("SWI 0",   0xF8);
    TEST("SWI 7",   0xFF);
    TEST("SWI",     0xFF);  // bare SWI defaults to vec 7 (ASL convention)
}

void test_tlcs900l() {
    // MAXMODE directive (ASL convention). /L is MIN-only: rejects MAXMODE ON,
    // accepts MAXMODE OFF as a no-op.
    ERRT("MAXMODE ON",  OPERAND_NOT_ALLOWED, "ON");
    TEST("MAXMODE OFF");
    // MINC1 buf_size, r16: prefix(D8+r) + 0x38 + uint16(buf_size-1)
    TEST("MINC1 2,BC",    0xD9, 0x38, 0x01, 0x00); // BC=reg1, 2-1=1
    TEST("MINC1 4,DE",    0xDA, 0x38, 0x03, 0x00); // DE=reg2, 4-1=3
    TEST("MINC1 8,HL",    0xDB, 0x38, 0x07, 0x00); // HL=reg3, 8-1=7
    TEST("MINC2 4,BC",    0xD9, 0x39, 0x02, 0x00); // 4-2=2
    TEST("MINC2 8,WA",    0xD8, 0x39, 0x06, 0x00); // WA=reg0, 8-2=6
    TEST("MINC4 8,BC",    0xD9, 0x3A, 0x04, 0x00); // 8-4=4
    TEST("MINC4 16,DE",   0xDA, 0x3A, 0x0C, 0x00); // 16-4=12
    // MDEC1 buf_size, r16: prefix(D8+r) + 0x3C + uint16(buf_size-1)
    TEST("MDEC1 2,BC",    0xD9, 0x3C, 0x01, 0x00);
    TEST("MDEC2 4,DE",    0xDA, 0x3D, 0x02, 0x00);
    TEST("MDEC4 8,HL",    0xDB, 0x3E, 0x04, 0x00);
    // Max valid buffer size is 2^15 (n=15).
    TEST("MINC1 8000H,BC", 0xD9, 0x38, 0xFF, 0x7F);
    // The buffer must be 2^n (2*step <= size <= 0x8000); out-of-range still
    // encodes (size-step) but flags OVERFLOW_RANGE.
    ERRT("MINC1 3,BC", OVERFLOW_RANGE, "3,BC", 0xD9, 0x38, 0x02, 0x00); // not 2^n
    ERRT("MINC1 1,BC", OVERFLOW_RANGE, "1,BC", 0xD9, 0x38, 0x00, 0x00); // n=0, too small
    ERRT("MINC4 4,BC", OVERFLOW_RANGE, "4,BC", 0xD9, 0x3A, 0x00, 0x00); // n=2 < min n=3
    // The index register is word-only (D8+r); 8-bit and 32-bit are rejected.
    ERRT("MINC1 8,A",   OPERAND_NOT_ALLOWED, "8,A");
    ERRT("MINC1 8,XIX", OPERAND_NOT_ALLOWED, "8,XIX");
    ERRT("MDEC2 8,XBC", OPERAND_NOT_ALLOWED, "8,XBC");
}

void test_ld_reg8_reg8() {
    TEST("LD A,A", 0xC9, 0x89);
    TEST("LD A,W", 0xC8, 0x89);
    TEST("LD A,B", 0xCA, 0x89);
    TEST("LD A,C", 0xCB, 0x89);
    TEST("LD A,D", 0xCC, 0x89);
    TEST("LD A,E", 0xCD, 0x89);
    TEST("LD A,H", 0xCE, 0x89);
    TEST("LD A,L", 0xCF, 0x89);
    TEST("LD W,A", 0xC9, 0x88);
    TEST("LD B,A", 0xC9, 0x8A);
    TEST("LD C,A", 0xC9, 0x8B);
    TEST("LD D,A", 0xC9, 0x8C);
    TEST("LD E,A", 0xC9, 0x8D);
    TEST("LD H,A", 0xC9, 0x8E);
    TEST("LD L,A", 0xC9, 0x8F);
}

void test_ld_reg8_imm() {
    TEST("LD A,034H", 0x21, 0x34);
    TEST("LD W,034H", 0x20, 0x34);
    TEST("LD B,034H", 0x22, 0x34);
    TEST("LD C,034H", 0x23, 0x34);
    TEST("LD D,034H", 0x24, 0x34);
    TEST("LD E,034H", 0x25, 0x34);
    TEST("LD H,034H", 0x26, 0x34);
    TEST("LD L,034H", 0x27, 0x34);
}

void test_ld_reg8_mem() {
    TEST("LD A,(XWA)",       0x80, 0x21);
    TEST("LD A,(XBC)",       0x81, 0x21);
    TEST("LD A,(XDE)",       0x82, 0x21);
    TEST("LD A,(XHL)",       0xC3, 0xEC, 0x21);  // 0x83 reserved (block) -> complex
    TEST("LD A,(XIX)",       0x84, 0x21);
    TEST("LD A,(XIY)",       0x85, 0x21);
    TEST("LD A,(XIZ)",       0x86, 0x21);
    TEST("LD A,(XSP)",       0x87, 0x21);
    TEST("LD A,(-XIX)",      0xC4, 0xF0, 0x21);
    TEST("LD A,(XIX+)",      0xC5, 0xF0, 0x21);
    TEST("LD A,(-XWA)",      0xC4, 0xE0, 0x21);
    TEST("LD A,(XWA+)",      0xC5, 0xE0, 0x21);
    TEST("LD A,(-XWA')",     0xC4, 0xD0, 0x21);  // previous-bank base
    TEST("LD A,(XBC0)",      0xC3, 0x04, 0x21);  // bank-0 base (complex)
    TEST("LD A,(XIX+20H)",   0x8C, 0x20, 0x21);
    TEST("LD A,(XIX-20H)",   0x8C, 0xE0, 0x21);
    TEST("LD A,(XIX-2)",     0x8C, 0xFE, 0x21);
    TEST("LD A,(XIX+127)",   0x8C, 0x7F, 0x21);
    TEST("LD A,(XIX-128)",   0x8C, 0x80, 0x21);
    TEST("LD A,(XIX+128)",   0xC3, 0xF1, 0x80, 0x00, 0x21);
    TEST("LD A,(XIX-129)",   0xC3, 0xF1, 0x7F, 0xFF, 0x21);
    TEST("LD A,(XIX+1234H)", 0xC3, 0xF1, 0x34, 0x12, 0x21);
    TEST("LD A,(034H)",      0xC0, 0x34, 0x21);
    TEST("LD A,(01234H)",    0xC1, 0x34, 0x12, 0x21);
    TEST("LD A,(0123456H)",  0xC2, 0x56, 0x34, 0x12, 0x21);
    TEST("LD A,(XIX+A)",     0xC3, 0x03, 0xF0, 0xE0, 0x21);
    TEST("LD A,(XIX+B)",     0xC3, 0x03, 0xF0, 0xE5, 0x21);
}

void test_ld_mem_reg8() {
    TEST("LD (XWA),A",       0xF3, 0xE0, 0x41);  // 0xB0 reserved (retcc) -> complex
    TEST("LD (XBC),A",       0xB1, 0x41);
    TEST("LD (XHL),A",       0xB3, 0x41);
    TEST("LD (XIX),A",       0xB4, 0x41);
    TEST("LD (XIY),A",       0xB5, 0x41);
    TEST("LD (-XIX),A",      0xF4, 0xF0, 0x41);
    TEST("LD (XIX+),A",      0xF5, 0xF0, 0x41);
    TEST("LD (XIX+20H),A",   0xBC, 0x20, 0x41);
    TEST("LD (XIX+1234H),A", 0xF3, 0xF1, 0x34, 0x12, 0x41);
    TEST("LD (034H),A",      0xF0, 0x34, 0x41);
    TEST("LD (01234H),A",    0xF1, 0x34, 0x12, 0x41);
    TEST("LD (0123456H),A",  0xF2, 0x56, 0x34, 0x12, 0x41);
    TEST("LD (XIX+A),A",     0xF3, 0x03, 0xF0, 0xE0, 0x41);
}

void test_ld_mem_imm8() {
    TEST("LD (XIX),034H",       0xB4, 0x00, 0x34);
    TEST("LD (-XIX),034H",      0xF4, 0xF0, 0x00, 0x34);
    TEST("LD (XIX+),034H",      0xF5, 0xF0, 0x00, 0x34);
    TEST("LD (XIX+20H),034H",   0xBC, 0x20, 0x00, 0x34);
    TEST("LD (XIX+1234H),034H", 0xF3, 0xF1, 0x34, 0x12, 0x00, 0x34);
    TEST("LD (034H),034H",      0x08, 0x34, 0x34);
    TEST("LD (01234H),034H",    0xF1, 0x34, 0x12, 0x00, 0x34);
    TEST("LD (0123456H),034H",  0xF2, 0x56, 0x34, 0x12, 0x00, 0x34);
    TEST("LD (XIX+A),034H",     0xF3, 0x03, 0xF0, 0xE0, 0x00, 0x34);
}

void test_ld_reg16_reg16() {
    TEST("LD WA,WA", 0xD8, 0x88);
    TEST("LD WA,BC", 0xD9, 0x88);
    TEST("LD WA,DE", 0xDA, 0x88);
    TEST("LD WA,HL", 0xDB, 0x88);
    TEST("LD WA,IX", 0xDC, 0x88);
    TEST("LD WA,IY", 0xDD, 0x88);
    TEST("LD WA,IZ", 0xDE, 0x88);
    TEST("LD WA,SP", 0xDF, 0x88);
    TEST("LD BC,WA", 0xD8, 0x89);
    TEST("LD DE,WA", 0xD8, 0x8A);
    TEST("LD HL,WA", 0xD8, 0x8B);
    TEST("LD IX,WA", 0xD8, 0x8C);
    TEST("LD IY,WA", 0xD8, 0x8D);
    TEST("LD IZ,WA", 0xD8, 0x8E);
    TEST("LD SP,WA", 0xD8, 0x8F);
}

void test_ld_reg16_imm() {
    TEST("LD WA,01234H", 0x30, 0x34, 0x12);
    TEST("LD BC,01234H", 0x31, 0x34, 0x12);
    TEST("LD DE,01234H", 0x32, 0x34, 0x12);
    TEST("LD HL,01234H", 0x33, 0x34, 0x12);
    TEST("LD IX,01234H", 0x34, 0x34, 0x12);
    TEST("LD IY,01234H", 0x35, 0x34, 0x12);
    TEST("LD IZ,01234H", 0x36, 0x34, 0x12);
    TEST("LD SP,01234H", 0x37, 0x34, 0x12);
}

void test_ld_reg16_mem() {
    TEST("LD WA,(XIX)",       0x94, 0x20);
    TEST("LD WA,(XIY)",       0x95, 0x20);
    TEST("LD WA,(-XIX)",      0xD4, 0xF1, 0x20);
    TEST("LD WA,(XIX+)",      0xD5, 0xF1, 0x20);
    TEST("LD WA,(XIX+20H)",   0x9C, 0x20, 0x20);
    TEST("LD WA,(XIX+1234H)", 0xD3, 0xF1, 0x34, 0x12, 0x20);
    TEST("LD WA,(034H)",      0xD0, 0x34, 0x20);
    TEST("LD WA,(01234H)",    0xD1, 0x34, 0x12, 0x20);
    TEST("LD WA,(0123456H)",  0xD2, 0x56, 0x34, 0x12, 0x20);
    TEST("LD WA,(XIX+A)",     0xD3, 0x03, 0xF0, 0xE0, 0x20);
}

void test_ld_mem_reg16() {
    TEST("LD (XIX),WA",       0xB4, 0x50);
    TEST("LD (XIY),WA",       0xB5, 0x50);
    TEST("LD (-XIX),WA",      0xF4, 0xF1, 0x50);
    TEST("LD (XIX+),WA",      0xF5, 0xF1, 0x50);
    TEST("LD (XIX+20H),WA",   0xBC, 0x20, 0x50);
    TEST("LD (XIX+1234H),WA", 0xF3, 0xF1, 0x34, 0x12, 0x50);
    TEST("LD (034H),WA",      0xF0, 0x34, 0x50);
    TEST("LD (01234H),WA",    0xF1, 0x34, 0x12, 0x50);
    TEST("LD (0123456H),WA",  0xF2, 0x56, 0x34, 0x12, 0x50);
    TEST("LD (XIX+A),WA",     0xF3, 0x03, 0xF0, 0xE0, 0x50);
}

void test_ld_reg32_reg32() {
    TEST("LD XWA,XWA", 0xE8, 0x88);
    TEST("LD XWA,XBC", 0xE9, 0x88);
    TEST("LD XWA,XDE", 0xEA, 0x88);
    TEST("LD XWA,XHL", 0xEB, 0x88);
    TEST("LD XWA,XIX", 0xEC, 0x88);
    TEST("LD XWA,XIY", 0xED, 0x88);
    TEST("LD XWA,XIZ", 0xEE, 0x88);
    TEST("LD XWA,XSP", 0xEF, 0x88);
    TEST("LD XIX,XWA", 0xE8, 0x8C);
    TEST("LD XIY,XWA", 0xE8, 0x8D);
    TEST("LD XIZ,XWA", 0xE8, 0x8E);
    TEST("LD XSP,XWA", 0xE8, 0x8F);
}

void test_ld_reg32_imm() {
    TEST("LD XWA,012345678H", 0x40, 0x78, 0x56, 0x34, 0x12);
    TEST("LD XBC,012345678H", 0x41, 0x78, 0x56, 0x34, 0x12);
    TEST("LD XIX,012345678H", 0x44, 0x78, 0x56, 0x34, 0x12);
    TEST("LD XIY,012345678H", 0x45, 0x78, 0x56, 0x34, 0x12);
}

void test_ld_reg32_mem() {
    TEST("LD XWA,(XIX)",       0xA4, 0x20);
    TEST("LD XWA,(-XIX)",      0xE4, 0xF2, 0x20);
    TEST("LD XWA,(XIX+)",      0xE5, 0xF2, 0x20);
    TEST("LD XWA,(XIX+20H)",   0xAC, 0x20, 0x20);
    TEST("LD XWA,(XIX+1234H)", 0xE3, 0xF1, 0x34, 0x12, 0x20);
    TEST("LD XWA,(034H)",      0xE0, 0x34, 0x20);
    TEST("LD XWA,(01234H)",    0xE1, 0x34, 0x12, 0x20);
    TEST("LD XWA,(0123456H)",  0xE2, 0x56, 0x34, 0x12, 0x20);
    TEST("LD XWA,(XIX+A)",     0xE3, 0x03, 0xF0, 0xE0, 0x20);
}

void test_push_pop() {
    TEST("PUSH 034H",  0x09, 0x34);
    TEST("PUSH F",     0x18);
    TEST("POP F",      0x19);
    TEST("PUSH WA",    0x28);
    TEST("PUSH BC",    0x29);
    TEST("PUSH HL",    0x2B);
    TEST("PUSH SP",    0x2F);
    TEST("POP WA",     0x48);
    TEST("POP BC",     0x49);
    TEST("POP HL",     0x4B);
    TEST("POP SP",     0x4F);
    TEST("PUSH XWA",   0x38);
    TEST("PUSH XBC",   0x39);
    TEST("PUSH XIX",   0x3C);
    TEST("POP XWA",    0x58);
    TEST("POP XBC",    0x59);
    TEST("POP XIX",    0x5C);
    // PUSH/POP from/to memory
    TEST("PUSH (XIX)",       0x84, 0x04);
    TEST("PUSH (XIY)",       0x85, 0x04);
    TEST("PUSH (-XIX)",      0xC4, 0xF0, 0x04);
    TEST("PUSH (XIX+)",      0xC5, 0xF0, 0x04);
    TEST("PUSH (XIX+20H)",   0x8C, 0x20, 0x04);
    TEST("PUSH (034H)",      0xC0, 0x34, 0x04);
    TEST("PUSH (01234H)",    0xC1, 0x34, 0x12, 0x04);
    TEST("POP (XIX)",        0xB4, 0x04);
    TEST("POP (XIY)",        0xB5, 0x04);
    TEST("POP (-XIX)",       0xF4, 0xF2, 0x04);
    TEST("POP (XIX+)",       0xF5, 0xF2, 0x04);
    TEST("POP (XIX+20H)",    0xBC, 0x20, 0x04);
    TEST("POP (034H)",       0xF0, 0x34, 0x04);
    TEST("POP (01234H)",     0xF1, 0x34, 0x12, 0x04);
}

void test_alu_reg_reg() {
    // ADD dst, src: src in prefix (PM_REG8), dst in opcode bits[2:0]
    TEST("ADD A,B",   0xCA, 0x81);
    TEST("ADD A,A",   0xC9, 0x81);
    TEST("ADD WA,BC", 0xD9, 0x80);
    TEST("ADD XWA,XBC", 0xE9, 0x80);
    TEST("ADC A,B",   0xCA, 0x91);
    TEST("SUB A,B",   0xCA, 0xA1);
    TEST("SBC A,B",   0xCA, 0xB1);
    TEST("EX  A,B",   0xCA, 0xB9);
    TEST("AND A,B",   0xCA, 0xC1);
    TEST("XOR A,B",   0xCA, 0xD1);
    TEST("OR  A,B",   0xCA, 0xE1);
    TEST("CP  A,B",   0xCA, 0xF1);
    // 16-bit
    TEST("ADD WA,HL", 0xDB, 0x80);
    TEST("SUB BC,WA", 0xD8, 0xA1);
    TEST("EX  WA,BC", 0xD9, 0xB8);
    TEST("CP  BC,WA", 0xD8, 0xF1);
}

void test_alu_reg_imm() {
    // ALU r8, #n8
    TEST("ADD A,034H",  0xC9, 0xC8, 0x34);
    TEST("ADC A,034H",  0xC9, 0xC9, 0x34);
    TEST("SUB A,034H",  0xC9, 0xCA, 0x34);
    TEST("SBC A,034H",  0xC9, 0xCB, 0x34);
    TEST("AND A,034H",  0xC9, 0xCC, 0x34);
    TEST("XOR A,034H",  0xC9, 0xCD, 0x34);
    TEST("OR  A,034H",  0xC9, 0xCE, 0x34);
    TEST("CP  A,034H",  0xC9, 0xCF, 0x34);
    // ALU r16, #n16
    TEST("ADD BC,01234H", 0xD9, 0xC8, 0x34, 0x12);
    TEST("CP  HL,01234H", 0xDB, 0xCF, 0x34, 0x12);
    // ALU r32, #n32
    TEST("ADD XWA,012345678H", 0xE8, 0xC8, 0x78, 0x56, 0x34, 0x12);
    TEST("CP  XBC,012345678H", 0xE9, 0xCF, 0x78, 0x56, 0x34, 0x12);
}

void test_alu_mem_src() {
    // ALU r8, (mem): PM_MEM8, dst in opcode bits[2:0]
    TEST("ADD A,(XIX)",     0x84, 0x81);
    TEST("ADC A,(XIX)",     0x84, 0x91);
    TEST("SUB A,(XIX)",     0x84, 0xA1);
    TEST("SBC A,(XIX)",     0x84, 0xB1);
    TEST("AND A,(XIX)",     0x84, 0xC1);
    TEST("XOR A,(XIX)",     0x84, 0xD1);
    TEST("OR  A,(XIX)",     0x84, 0xE1);
    TEST("CP  A,(XIX)",     0x84, 0xF1);
    // ALU r16, (mem): PM_MEM16
    TEST("ADD WA,(XIX)",    0x94, 0x80);
    TEST("CP  HL,(XIX)",    0x94, 0xF3);
    // ALU r32, (mem): PM_MEM32
    TEST("ADD XWA,(XIX)",   0xA4, 0x80);
    // CP (mem), #n8
    TEST("CP (XIX),034H",   0x84, 0x3F, 0x34);
    TEST("CP (01234H),034H",0xC1, 0x34, 0x12, 0x3F, 0x34);
    // LD (abs16), (mem_src)
    TEST("LD (01234H),(XIX)", 0x84, 0x19, 0x34, 0x12);
    // RLD/RRD
    TEST("RLD A,(XIX)",     0x84, 0x06);
    TEST("RRD A,(XIX)",     0x84, 0x07);
    // MUL/DIV from memory
    TEST("MUL  WA,(XIX)",   0x84, 0x41);
    TEST("MULS WA,(XIX)",   0x84, 0x49);
    TEST("DIV  WA,(XIX)",   0x84, 0x51);
    TEST("DIVS WA,(XIX)",   0x84, 0x59);
}

void test_ld_memd() {
    // LD (mem), imm16/imm32
    TEST("LD (XIX),01234H",      0xB4, 0x01, 0x34, 0x12);
    TEST("LD (XIX),012345678H",  0xB4, 0x02, 0x78, 0x56, 0x34, 0x12);
    // LD (mem), (abs16)
    TEST("LD (XIX),(01234H)",    0xB4, 0x14, 0x34, 0x12);
    // LDA r16, (mem)
    TEST("LDA WA,(XIX)",         0xB4, 0x20);
    TEST("LDA BC,(XIX)",         0xB4, 0x21);
    TEST("LDA HL,(XIX)",         0xB4, 0x23);
    TEST("LDA WA,(XIX+20H)",     0xBC, 0x20, 0x20);
    TEST("LDA WA,(01234H)",      0xF1, 0x34, 0x12, 0x20);
    // JP/CALL cc, (mem)
    TEST("JP  Z,(XIX)",          0xB4, 0xD6);
    TEST("JP  NZ,(XIX)",         0xB4, 0xDE);
    TEST("JP  F,(XIX)",          0xB4, 0xD0);
    TEST("CALL Z,(XIX)",         0xB4, 0xE6);
    TEST("CALL NC,(XIX+20H)",    0xBC, 0x20, 0xEF);
}

void test_bit_reg() {
    // Bit ops in register context (bit in following byte)
    TEST("ANDCF 2,A",  0xC9, 0x20, 0x02);
    TEST("ORCF  5,A",  0xC9, 0x21, 0x05);
    TEST("XORCF 6,A",  0xC9, 0x22, 0x06);
    TEST("LDCF  7,A",  0xC9, 0x23, 0x07);
    TEST("STCF  0,A",  0xC9, 0x24, 0x00);
    TEST("RES 3,A",    0xC9, 0x30, 0x03);
    TEST("SET 7,A",    0xC9, 0x31, 0x07);
    TEST("CHG 0,A",    0xC9, 0x32, 0x00);
    TEST("BIT 4,A",    0xC9, 0x33, 0x04);
    TEST("TSET 1,A",   0xC9, 0x34, 0x01);
    // 16-bit register context
    TEST("RES 3,BC",   0xD9, 0x30, 0x03);
    TEST("SET 7,HL",   0xDB, 0x31, 0x07);
    TEST("BIT 4,WA",   0xD8, 0x33, 0x04);
    // 32-bit register context
    TEST("RES 0,XWA",  0xE8, 0x30, 0x00);
    TEST("SET 7,XBC",  0xE9, 0x31, 0x07);
}

void test_bit_mem() {
    // Bit ops in MEMD context (bit in opcode low 3 bits)
    TEST("ANDCF 0,(XIX)", 0xB4, 0x80);
    TEST("ANDCF 7,(XIX)", 0xB4, 0x87);
    TEST("ORCF  0,(XIX)", 0xB4, 0x88);
    TEST("ORCF  7,(XIX)", 0xB4, 0x8F);
    TEST("XORCF 3,(XIX)", 0xB4, 0x93);
    TEST("LDCF  4,(XIX)", 0xB4, 0x9C);
    TEST("STCF  2,(XIX)", 0xB4, 0xA2);
    TEST("TSET  1,(XIX)", 0xB4, 0xA9);
    TEST("RES 3,(XIX)",   0xB4, 0xB3);
    TEST("SET 7,(XIX)",   0xB4, 0xBF);
    TEST("CHG 0,(XIX)",   0xB4, 0xC0);
    TEST("BIT 5,(XIX)",   0xB4, 0xCD);
    // With different addressing modes
    TEST("RES 3,(034H)",  0xF0, 0x34, 0xB3);
    TEST("SET 7,(01234H)",0xF1, 0x34, 0x12, 0xBF);
}

void test_shift() {
    // Rotate/shift: count in following byte, src in prefix
    TEST("RLC 1,A",    0xC9, 0xE8, 0x01);
    TEST("RRC 2,A",    0xC9, 0xE9, 0x02);
    TEST("RL  3,A",    0xC9, 0xEA, 0x03);
    TEST("RR  4,A",    0xC9, 0xEB, 0x04);
    TEST("SLA 1,A",    0xC9, 0xEC, 0x01);
    TEST("SRA 2,A",    0xC9, 0xED, 0x02);
    TEST("SLL 3,A",    0xC9, 0xEE, 0x03);
    TEST("SRL 4,A",    0xC9, 0xEF, 0x04);
    // 16-bit
    TEST("RLC 1,BC",   0xD9, 0xE8, 0x01);
    TEST("SRL 3,BC",   0xD9, 0xEF, 0x03);
    // Memory source
    TEST("RLC 1,(XIX)", 0x84, 0xE8, 0x01);
    TEST("SRL 4,(XIX)", 0x84, 0xEF, 0x04);
}

void test_inc_dec() {
    // INC/DEC step 1/2/4 in register context
    TEST("INC 1,A",   0xC9, 0x61);
    TEST("INC 2,A",   0xC9, 0x62);
    TEST("INC 4,A",   0xC9, 0x64);
    TEST("DEC 1,A",   0xC9, 0x69);
    TEST("DEC 2,A",   0xC9, 0x6A);
    TEST("DEC 4,A",   0xC9, 0x6C);
    // 16-bit register
    TEST("INC 1,BC",  0xD9, 0x61);
    TEST("INC 2,HL",  0xDB, 0x62);
    TEST("DEC 4,WA",  0xD8, 0x6C);
    // Memory source
    TEST("INC 1,(XIX)",  0x84, 0x61);
    TEST("INC 4,(XIX)",  0x84, 0x64);
    TEST("DEC 2,(XIX)",  0x84, 0x6A);
}

void test_unary() {
    // CPL/NEG: unary on register (M_SRC)
    TEST("CPL A",    0xC9, 0x06);
    TEST("NEG A",    0xC9, 0x07);
    TEST("CPL BC",   0xD9, 0x06);
    TEST("NEG BC",   0xD9, 0x07);
    // NEG/CPL are byte/word only (manual BW-); the long form is undefined.
    ERRT("CPL XWA",  OPERAND_NOT_ALLOWED, "XWA");
    ERRT("NEG XWA",  OPERAND_NOT_ALLOWED, "XWA");
    // DAA/EXTZ/EXTS/PAA/MIRR/MULA
    TEST("DAA  A",   0xC9, 0x10);
    TEST("EXTZ BC",  0xD9, 0x12);
    TEST("EXTS BC",  0xD9, 0x13);
    TEST("PAA  XBC", 0xE9, 0x14);
    TEST("MIRR BC",  0xD9, 0x16);
    TEST("MULA XBC", 0xD9, 0x19);  // 32-bit operand via D8+r (PM_REG16) alias
    // Operand size is fixed per instruction (manual B/W/L flags): DAA byte
    // only; MIRR word only; EXTZ/EXTS/PAA word or long (no byte).
    ERRT("DAA  WA",  OPERAND_NOT_ALLOWED, "WA");
    ERRT("DAA  XWA", OPERAND_NOT_ALLOWED, "XWA");
    ERRT("MIRR A",   OPERAND_NOT_ALLOWED, "A");
    ERRT("MIRR XBC", OPERAND_NOT_ALLOWED, "XBC");
    ERRT("EXTZ A",   OPERAND_NOT_ALLOWED, "A");
    ERRT("PAA  A",   OPERAND_NOT_ALLOWED, "A");
    // BS1F/BS1B: result in A, source is 16-bit register
    TEST("BS1F A,WA", 0xD8, 0x0E);
    TEST("BS1F A,BC", 0xD9, 0x0E);
    TEST("BS1B A,HL", 0xDB, 0x0F);
    // LINK/UNLK: 32-bit register
    TEST("LINK XBC,01234H", 0xE9, 0x0C, 0x34, 0x12);
    TEST("UNLK XBC",        0xE9, 0x0D);
    // SCC: set register on condition
    TEST("SCC Z,A",   0xC9, 0x76);
    TEST("SCC NZ,A",  0xC9, 0x7E);
    TEST("SCC C,A",   0xC9, 0x77);
    TEST("SCC NC,BC", 0xD9, 0x7F);
    TEST("SCC OV,A",  0xC9, 0x74);
    TEST("SCC T,A",   0xC9, 0x78);
    TEST("SCC PL,A",  0xC9, 0x7D);
    // DJNZ: decrement and jump
    TEST("DJNZ A,3",  0xC9, 0x1C, 0x00);
    TEST("DJNZ B,3",  0xCA, 0x1C, 0x00);
    TEST("DJNZ A,0",  0xC9, 0x1C, 0xFD);
}

void test_mul_div() {
    // MUL/MULS/DIV/DIVS with register source
    TEST("MUL  WA,A",  0xC9, 0x41);
    TEST("MULS WA,A",  0xC9, 0x49);
    TEST("DIV  WA,A",  0xC9, 0x51);
    TEST("DIVS WA,A",  0xC9, 0x59);
    TEST("MUL  WA,B",  0xCA, 0x41);
    TEST("DIV  WA,L",  0xCF, 0x51);
    // Non-WA destination (byte op: dest 16-bit, R=2*idx+1).
    TEST("MUL  BC,A",  0xC9, 0x43);
    TEST("MUL  DE,C",  0xCB, 0x45);
    TEST("MULS HL,B",  0xCA, 0x4F);
    TEST("DIV  BC,A",  0xC9, 0x53);
    // Word op: dest 32-bit (R=idx), source 16-bit.
    TEST("MUL  XWA,BC", 0xD9, 0x40);
    TEST("MUL  XBC,DE", 0xDA, 0x41);
    TEST("DIV  XWA,BC", 0xD9, 0x50);
    TEST("DIVS XHL,IX", 0xDC, 0x5B);
    TEST("MUL  XBC,(XIX)", 0x94, 0x41);
    // Immediate source: dest carried in the prefix; byte op -> imm8, word op -> imm16.
    TEST("MUL  WA,5",   0xC9, 0x08, 0x05);
    TEST("MUL  BC,5",   0xCB, 0x08, 0x05);
    TEST("MUL  XWA,5",  0xD8, 0x08, 0x05, 0x00);
    TEST("MULS WA,5",   0xC9, 0x09, 0x05);
    TEST("DIV  WA,5",   0xC9, 0x0A, 0x05);
    TEST("DIVS BC,5",   0xCB, 0x0B, 0x05);
}

void test_branch() {
    // Unconditional
    TEST("RET",            0x0E);
    TEST("RETD 01234H",    0x0F, 0x34, 0x12);
    // CALR rel16: at address 0, base=3 (1 opc + 2 disp bytes)
    TEST("CALR 3",         0x1E, 0x00, 0x00);
    TEST("CALR 0",         0x1E, 0xFD, 0xFF);
    // JP/CALL to absolute address (unconditional)
    TEST("JP   (01234H)",  0x1A, 0x34, 0x12);
    TEST("JP   (0123456H)",0x1B, 0x56, 0x34, 0x12);
    TEST("CALL (01234H)",  0x1C, 0x34, 0x12);
    TEST("CALL (0123456H)",0x1D, 0x56, 0x34, 0x12);
    // cc=T (always) folds to the compact unconditional form
    TEST("RET  T",         0x0E);
    TEST("JP   T,(01234H)",0x1A, 0x34, 0x12);
    TEST("CALL T,(0123456H)",0x1D, 0x56, 0x34, 0x12);
    // JR cc, rel8: at address 0, base=2
    TEST("JR F,2",         0x60, 0x00);
    TEST("JR LT,2",        0x61, 0x00);
    TEST("JR Z,2",         0x66, 0x00);
    TEST("JR C,2",         0x67, 0x00);
    TEST("JR GE,2",        0x69, 0x00);
    TEST("JR NZ,2",        0x6E, 0x00);
    TEST("JR NC,2",        0x6F, 0x00);
    TEST("JR Z,0",         0x66, 0xFE);
    TEST("JR Z,129",       0x66, 0x7F);
    // Aliases
    TEST("JR EQ,2",        0x66, 0x00);
    TEST("JR ULT,2",       0x67, 0x00);
    // LDAR rd, target: F3 13 rel16 reg_byte. base = address + 4.
    TEST("LDAR WA, 4",     0xF3, 0x13, 0x00, 0x00, 0x20);
    TEST("LDAR HL, 4",     0xF3, 0x13, 0x00, 0x00, 0x23);
    TEST("LDAR XWA, 4",    0xF3, 0x13, 0x00, 0x00, 0x30);
    TEST("LDAR XHL, 4",    0xF3, 0x13, 0x00, 0x00, 0x33);
    // JRL cc, rel16: at address 0, base=3
    TEST("JRL Z,3",        0x76, 0x00, 0x00);
    TEST("JRL NZ,3",       0x7E, 0x00, 0x00);
    TEST("JRL Z,0",        0x76, 0xFD, 0xFF);
    // RET cc (prefix 0xB0)
    TEST("RET F",          0xB0, 0xF0);
    TEST("RET Z",          0xB0, 0xF6);
    TEST("RET NZ",         0xB0, 0xFE);
    TEST("RET NC",         0xB0, 0xFF);
    // Undefined target: the displacement field is left zero.
    ERUS("JR Z,UNDEF",     "UNDEF", 0x66, 0x00);
    ERUS("JRL Z,UNDEF",    "UNDEF", 0x76, 0x00, 0x00);
    ERUS("CALR UNDEF",     "UNDEF", 0x1E, 0x00, 0x00);
    ERUS("LDAR WA, UNDEF", "UNDEF", 0xF3, 0x13, 0x00, 0x00, 0x20);
}

void test_block() {
    TEST("LDI",   0x83, 0x10);
    TEST("LDIR",  0x83, 0x11);
    TEST("LDD",   0x83, 0x12);
    TEST("LDDR",  0x83, 0x13);
    TEST("CPI",   0x83, 0x14);
    TEST("CPIR",  0x83, 0x15);
    TEST("CPD",   0x83, 0x16);
    TEST("CPDR",  0x83, 0x17);
}

void test_abs_reg() {
    // ABREG8: prefix 0xC7 + register code + opcode
    // LD dst_r8, bank-N abs register
    TEST("LD A,A0",   0xC7, 0x00, 0x89);  // A in bank 0
    TEST("LD A,W0",   0xC7, 0x01, 0x89);  // W in bank 0
    TEST("LD A,B0",   0xC7, 0x05, 0x89);  // B in bank 0
    TEST("LD A,H0",   0xC7, 0x0D, 0x89);  // H in bank 0
    TEST("LD W,A0",   0xC7, 0x00, 0x88);  // dst=W
    // LD dst_r8, prev-bank register
    TEST("LD A,A'",   0xC7, 0xD0, 0x89);
    TEST("LD A,H'",   0xC7, 0xDD, 0x89);
    // LD dst_r8, Q-variant abs register
    TEST("LD A,QA0",  0xC7, 0x02, 0x89);  // QA bank 0
    TEST("LD A,QB0",  0xC7, 0x07, 0x89);  // QB bank 0
    TEST("LD A,QH0",  0xC7, 0x0F, 0x89);  // QH bank 0
    TEST("LD A,QA'",  0xC7, 0xD2, 0x89);
    TEST("LD A,QA",   0xC7, 0xE2, 0x89);  // QA current bank
    // LD dst_r8, fixed abs register
    TEST("LD A,IXL",  0xC7, 0xF0, 0x89);
    TEST("LD A,IXH",  0xC7, 0xF1, 0x89);
    TEST("LD A,QIXL", 0xC7, 0xF2, 0x89);
    TEST("LD A,QSPH", 0xC7, 0xFF, 0x89);
    TEST("LD A,SPL",  0xC7, 0xFC, 0x89);
    // ADD/CP abs_r8, #n8
    TEST("ADD A0,034H",  0xC7, 0x00, 0xC8, 0x34);
    TEST("CP  A',034H",  0xC7, 0xD0, 0xCF, 0x34);
    TEST("ADD QA0,034H", 0xC7, 0x02, 0xC8, 0x34);
    // ABREG16: prefix 0xD7 + register code + opcode
    // LD dst_r16, bank-N abs register
    TEST("LD WA,WA0",  0xD7, 0x00, 0x88);
    TEST("LD BC,WA0",  0xD7, 0x00, 0x89);
    TEST("LD BC,HL0",  0xD7, 0x0C, 0x89);
    TEST("LD BC,WA'",  0xD7, 0xD0, 0x89);
    // LD dst_r16, Q-variant abs register
    TEST("LD BC,QWA0", 0xD7, 0x02, 0x89);  // QWA bank 0
    TEST("LD BC,QBC0", 0xD7, 0x06, 0x89);
    TEST("LD BC,QHL3", 0xD7, 0x3E, 0x89);  // QHL bank 3
    TEST("LD BC,QWA'", 0xD7, 0xD2, 0x89);
    TEST("LD BC,QWA",  0xD7, 0xE2, 0x89);  // QWA current bank
    // LD dst_r16, fixed Q-variant
    TEST("LD SP,QSP",  0xD7, 0xFE, 0x8F);
    TEST("LD IX,QIX",  0xD7, 0xF2, 0x8C);
    // ADD/CP abs_r16, #n16
    TEST("ADD WA0,01234H",  0xD7, 0x00, 0xC8, 0x34, 0x12);
    TEST("CP  QHL3,01234H", 0xD7, 0x3E, 0xCF, 0x34, 0x12);
    // ABREG32: prefix 0xE7 + register code + opcode
    // LD dst_r32, bank-N abs register
    TEST("LD XWA,XWA0",  0xE7, 0x00, 0x88);
    TEST("LD XHL,XWA0",  0xE7, 0x00, 0x8B);
    TEST("LD XHL,XHL0",  0xE7, 0x0C, 0x8B);
    TEST("LD XHL,XWA'",  0xE7, 0xD0, 0x8B);
    TEST("LD XHL,XHL3",  0xE7, 0x3C, 0x8B);
    // ADD/CP abs_r32, #n32
    TEST("ADD XWA0,012345678H",  0xE7, 0x00, 0xC8, 0x78, 0x56, 0x34, 0x12);
    TEST("CP  XHL3,012345678H",  0xE7, 0x3C, 0xCF, 0x78, 0x56, 0x34, 0x12);
    // LD abs_r8 as destination, from standard register (opcode 0x98|reg)
    TEST("LD A0,A",     0xC7, 0x00, 0x99);
    TEST("LD A0,W",     0xC7, 0x00, 0x98);
    TEST("LD A',B",     0xC7, 0xD0, 0x9A);
    // LD abs_r16 as destination, from standard register
    TEST("LD WA0,BC",   0xD7, 0x00, 0x99);
    TEST("LD QHL3,IX",  0xD7, 0x3E, 0x9C);
    // LD abs_r32 as destination, from standard register
    TEST("LD XWA0,XBC", 0xE7, 0x00, 0x99);
    TEST("LD XHL3,XIX", 0xE7, 0x3C, 0x9C);
    // LD abs_r as destination, from immediate (opcode 0x03; imm width by PM)
    TEST("LD A0,034H",          0xC7, 0x00, 0x03, 0x34);
    TEST("LD A',056H",          0xC7, 0xD0, 0x03, 0x56);
    TEST("LD WA0,01234H",       0xD7, 0x00, 0x03, 0x34, 0x12);
    TEST("LD XWA0,012345678H",  0xE7, 0x00, 0x03, 0x78, 0x56, 0x34, 0x12);
    // LD abs_r ↔ memory is structurally impossible (TLCS-900 has only one prefix byte).
    ERRT("LD A0,(XIX)",  OPERAND_NOT_ALLOWED, "A0,(XIX)");
    ERRT("LD (XIX),A0",  OPERAND_NOT_ALLOWED, "(XIX),A0");
}

// clang-format on
// Register-indirect single-byte forms whose prefix byte would be a reserved
// prefix (0x83 = block transfer, 0xB0 = conditional return) must fall back to
// the complex (r32) encoding, plus previous-bank / bank-N base registers that
// only have a complex form.
void test_reserved_and_bank() {
    // (XHL) byte-source single byte is 0x83 (block) -> complex C3 EC
    TEST("LD A,(XHL)",   0xC3, 0xEC, 0x21);
    // (XHL) word/long-source bytes 0x93/0xA3 are NOT reserved -> single byte
    TEST("LD WA,(XHL)",  0x93, 0x20);
    TEST("LD XWA,(XHL)", 0xA3, 0x20);
    // (XWA) destination single byte is 0xB0 (retcc) -> complex F3 E0
    TEST("LD (XWA),A",   0xF3, 0xE0, 0x41);
    // other destination bases keep the single byte
    TEST("LD (XHL),A",   0xB3, 0x41);
    // previous-bank and bank-N base registers (complex only)
    TEST("LD A,(XWA')",  0xC3, 0xD0, 0x21);
    TEST("LD A,(-XHL')", 0xC4, 0xDC, 0x21);
    TEST("LD A,(XDE3)",  0xC3, 0x38, 0x21);
    TEST("LD A,(XBC0+)", 0xC5, 0x04, 0x21);
}

// Minimum mode (TLCS900L): the general 32-bit registers XWA/XBC/XDE/XHL are
// unavailable as data operands and as memory base registers; only the index
// registers XIX/XIY/XIZ/XSP remain usable (manual 3.2.1). This covers the
// instruction classes that remain valid in minimum mode plus the rejections.
void test_minmode() {
    // LD: 8/16-bit registers, immediates, and XIX-based addressing.
    TEST("LD A,B",            0xCA, 0x89);
    TEST("LD A,34H",          0x21, 0x34);
    TEST("LD WA,BC",          0xD9, 0x88);
    TEST("LD WA,1234H",       0x30, 0x34, 0x12);
    TEST("LD A,(XIX)",        0x84, 0x21);
    TEST("LD A,(-XIX)",       0xC4, 0xF0, 0x21);
    TEST("LD A,(XIX+20H)",    0x8C, 0x20, 0x21);
    TEST("LD (XIX),A",        0xB4, 0x41);
    TEST("LD A,(1234H)",      0xC1, 0x34, 0x12, 0x21);
    TEST("LD XIX,12345678H",  0x44, 0x78, 0x56, 0x34, 0x12);
    TEST("LD XSP,1234H",      0x47, 0x34, 0x12, 0x00, 0x00);
    // PUSH/POP, ALU, INC/DEC, shift, bit, branch, MUL/DIV.
    TEST("PUSH BC",           0x29);
    TEST("PUSH XIX",          0x3C);
    TEST("POP WA",            0x48);
    TEST("ADD A,B",           0xCA, 0x81);
    TEST("ADD A,34H",         0xC9, 0xC8, 0x34);
    TEST("ADD WA,BC",         0xD9, 0x80);
    TEST("ADD XIX,XIY",       0xED, 0x84);
    TEST("SUB A,B",           0xCA, 0xA1);
    TEST("CP A,B",            0xCA, 0xF1);
    TEST("AND A,B",           0xCA, 0xC1);
    TEST("INC 1,A",           0xC9, 0x61);
    TEST("DEC 1,WA",          0xD8, 0x69);
    TEST("RLC 1,A",           0xC9, 0xE8, 0x01);
    TEST("SLA 1,WA",          0xD8, 0xEC, 0x01);
    TEST("BIT 3,A",           0xC9, 0x33, 0x03);
    TEST("SET 3,(XIX)",       0xB4, 0xBB);
    TEST("JR NZ,$",           0x6E, 0xFE);
    TEST("MUL WA,B",          0xCA, 0x41);
    TEST("DIV WA,B",          0xCA, 0x51);
    // 8 register banks in minimum mode: LDF takes 0-7.
    TEST("LDF 7",             0x17, 0x07);
    ERRT("LDF 8",             OVERFLOW_RANGE, "8", 0x17, 0x00);
    // The general 32-bit registers XWA-XHL are rejected as data operands...
    ERRT("LD XWA,XBC", REGISTER_NOT_ALLOWED, "XBC", 0xE9, 0x88);
    ERRT("MULA XWA",   REGISTER_NOT_ALLOWED, "XWA", 0xD8, 0x19);
    ERRT("EXTZ XWA",   REGISTER_NOT_ALLOWED, "XWA", 0xE8, 0x12);
    ERRT("PUSH XWA",   REGISTER_NOT_ALLOWED, "XWA", 0x38);
    // ...and as memory base registers.
    ERRT("LD A,(XHL)",  REGISTER_NOT_ALLOWED, "XHL)");
    ERRT("LD A,(XBC)",  REGISTER_NOT_ALLOWED, "XBC)");
    ERRT("LD A,(-XWA)", REGISTER_NOT_ALLOWED, "XWA)");
}

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    if (is_tlcs900l()) {
        // /L is minimum-mode only: the maximum-mode operand suite (general
        // 32-bit registers) does not apply.
        RUN_TEST(test_tlcs900l);
        RUN_TEST(test_minmode);
        return;
    }
    RUN_TEST(test_single);
    RUN_TEST(test_ld_reg8_reg8);
    RUN_TEST(test_ld_reg8_imm);
    RUN_TEST(test_ld_reg8_mem);
    RUN_TEST(test_ld_mem_reg8);
    RUN_TEST(test_ld_mem_imm8);
    RUN_TEST(test_ld_reg16_reg16);
    RUN_TEST(test_ld_reg16_imm);
    RUN_TEST(test_ld_reg16_mem);
    RUN_TEST(test_ld_mem_reg16);
    RUN_TEST(test_ld_reg32_reg32);
    RUN_TEST(test_ld_reg32_imm);
    RUN_TEST(test_ld_reg32_mem);
    RUN_TEST(test_push_pop);
    RUN_TEST(test_alu_reg_reg);
    RUN_TEST(test_alu_reg_imm);
    RUN_TEST(test_alu_mem_src);
    RUN_TEST(test_ld_memd);
    RUN_TEST(test_bit_reg);
    RUN_TEST(test_bit_mem);
    RUN_TEST(test_shift);
    RUN_TEST(test_inc_dec);
    RUN_TEST(test_unary);
    RUN_TEST(test_mul_div);
    RUN_TEST(test_branch);
    RUN_TEST(test_block);
    RUN_TEST(test_abs_reg);
    RUN_TEST(test_reserved_and_bank);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
