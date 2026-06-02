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

#include "dis_tlcs900.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::tlcs900;
using namespace libasm::test;

DisTlcs900 dis900;
Disassembler &disassembler(dis900);

void set_up() {
    disassembler.reset();
    disassembler.setRelativeTarget(true);
}

void tear_down() {
    symtab.reset();
}

static bool is_tlcs900() {
    return strcmp_P("TLCS900", disassembler.config().cpu_P()) == 0;
}
static bool is_tlcs900l() {
    return strcmp_P("TLCS900L", disassembler.config().cpu_P()) == 0;
}
static bool is_tlcs900h() {
    return strcmp_P("TLCS900H", disassembler.config().cpu_P()) == 0;
}
static bool is_tlcs900l1() {
    return strcmp_P("TLCS900L1", disassembler.config().cpu_P()) == 0;
}
// clang-format off
void test_cpu() {
    EQUALS("cpu tlcs900",   true, disassembler.setCpu("tlcs900"));
    EQUALS_P("get cpu", "TLCS900",   disassembler.config().cpu_P());
    EQUALS("cpu tlcs900l",  true, disassembler.setCpu("tlcs900l"));
    EQUALS_P("get cpu", "TLCS900L",  disassembler.config().cpu_P());
    EQUALS("cpu tlcs900h",  true, disassembler.setCpu("tlcs900h"));
    EQUALS_P("get cpu", "TLCS900H",  disassembler.config().cpu_P());
    EQUALS("cpu tlcs900l1", true, disassembler.setCpu("tlcs900l1"));
    EQUALS_P("get cpu", "TLCS900L1", disassembler.config().cpu_P());
}

void test_single() {
    TEST("NOP",    "", 0x00);
    if (is_tlcs900()) {
        TEST("NORMAL", "", 0x01);
        TEST("MAX",    "", 0x04);
    } else if (is_tlcs900l()) {
        UNKN(0x01);
        TEST("MIN",    "", 0x04);
    } else {
        UNKN(0x01);
        UNKN(0x04);
    }
    TEST("PUSH",   "SR", 0x02);
    TEST("POP",    "SR", 0x03);
    TEST("HALT",   "", 0x05);
    TEST("RETI",   "", 0x07);
    TEST("LD",    "(34H), 56H",       0x08, 0x34, 0x56);
    TEST("PUSH",  "34H",             0x09, 0x34);
    TEST("LDW",   "(34H), 1234H",    0x0A, 0x34, 0x34, 0x12);
    TEST("PUSHW", "1234H",           0x0B, 0x34, 0x12);
    TEST("RCF",    "", 0x10);
    TEST("SCF",    "", 0x11);
    TEST("CCF",    "", 0x12);
    TEST("ZCF",    "", 0x13);
    TEST("PUSH",  "A",               0x14);
    TEST("POP",   "A",               0x15);
    TEST("EX",    "F, F'",           0x16);
    TEST("INCF",   "", 0x0C);
    TEST("DECF",   "", 0x0D);
    TEST("LDX",   "(0034H), 0056H",  0xF7, 0x00, 0x34, 0x00, 0x56, 0x00);
    // sub-byte 0x3C decodes as NSP/XNSP on base; as INTNEST on /L, /H, /L1 (16-bit only).
    if (is_tlcs900()) {
        TEST("LDC",   "XNSP, XSP",      0xEF, 0x2E, 0x3C);
        TEST("LDC",   "XSP, XNSP",      0xEF, 0x2F, 0x3C);
        TEST("LDC",   "NSP, SP",        0xDF, 0x2E, 0x3C);
        TEST("LDC",   "SP, NSP",        0xDF, 0x2F, 0x3C);
    } else {
        TEST("LDC",   "INTNEST, WA",    0xD8, 0x2E, 0x3C);
        TEST("LDC",   "WA, INTNEST",    0xD8, 0x2F, 0x3C);
    }
    TEST("LDC",   "DMAS0, XWA",    0xE8, 0x2E, 0x00);
    TEST("LDC",   "DMAS1, XBC",    0xE9, 0x2E, 0x04);
    TEST("LDC",   "DMAS3, XHL",    0xEB, 0x2E, 0x0C);
    TEST("LDC",   "DMAD0, XWA",    0xE8, 0x2E, 0x10);
    TEST("LDC",   "DMAD3, XHL",    0xEB, 0x2E, 0x1C);
    TEST("LDC",   "DMAC0, WA",     0xD8, 0x2E, 0x20);
    TEST("LDC",   "DMAC3, HL",     0xDB, 0x2E, 0x2C);
    TEST("LDC",   "DMAM0, A",      0xC9, 0x2E, 0x22);
    TEST("LDC",   "DMAM3, D",      0xCC, 0x2E, 0x2E);
    TEST("LDC",   "XWA, DMAS0",    0xE8, 0x2F, 0x00);
    TEST("LDC",   "XBC, DMAS1",    0xE9, 0x2F, 0x04);
    TEST("LDC",   "WA, DMAC0",     0xD8, 0x2F, 0x20);
    TEST("LDC",   "A, DMAM0",      0xC9, 0x2F, 0x22);
    TEST("LDC",   "XHL, DMAD3",    0xEB, 0x2F, 0x1C);
    TEST("EI",     "0", 0x06, 0x00);
    TEST("EI",     "7", 0x06, 0x07);
    TEST("LDF",    "0", 0x17, 0x00);
    TEST("LDF",    "3", 0x17, 0x03);
    // RFP width depends on reset register mode: base and /L start in MIN (3-bit),
    // /H and /L1 start in MAX (2-bit, top bit masked off).
    if (is_tlcs900() || is_tlcs900l()) {
        TEST("LDF", "7", 0x17, 0x07);
    } else {
        TEST("LDF", "3", 0x17, 0x07);
    }
    TEST("SWI",    "0", 0xF8);
    TEST("SWI",    "7", 0xFF);
}

void test_tlcs900l() {
    // MINC1 #buf_size, r16: prefix(D8+r) + 0x38 + uint16(buf_size-1)
    TEST("MINC1", "2, BC",  0xD9, 0x38, 0x01, 0x00); // BC=reg1, 2-1=1
    TEST("MINC1", "4, DE",  0xDA, 0x38, 0x03, 0x00); // DE=reg2, 4-1=3
    TEST("MINC1", "8, HL",  0xDB, 0x38, 0x07, 0x00); // HL=reg3, 8-1=7
    TEST("MINC2", "4, BC",  0xD9, 0x39, 0x02, 0x00); // 4-2=2
    TEST("MINC2", "8, WA",  0xD8, 0x39, 0x06, 0x00); // WA=reg0, 8-2=6
    TEST("MINC4", "8, BC",  0xD9, 0x3A, 0x04, 0x00); // 8-4=4
    TEST("MINC4", "16, DE", 0xDA, 0x3A, 0x0C, 0x00); // 16-4=12
    // MDEC1 #buf_size, r16: prefix(D8+r) + 0x3C + uint16(buf_size-1)
    TEST("MDEC1", "2, BC",  0xD9, 0x3C, 0x01, 0x00);
    TEST("MDEC2", "4, DE",  0xDA, 0x3D, 0x02, 0x00);
    TEST("MDEC4", "8, HL",  0xDB, 0x3E, 0x04, 0x00);
}

void test_ld8() {
    // LD r8, r8
    TEST("LD", "A, A", 0xC9, 0x89);
    TEST("LD", "A, W", 0xC8, 0x89);
    TEST("LD", "A, B", 0xCA, 0x89);
    TEST("LD", "A, C", 0xCB, 0x89);
    TEST("LD", "A, D", 0xCC, 0x89);
    TEST("LD", "A, E", 0xCD, 0x89);
    TEST("LD", "A, H", 0xCE, 0x89);
    TEST("LD", "A, L", 0xCF, 0x89);
    TEST("LD", "W, A", 0xC9, 0x88);
    TEST("LD", "B, A", 0xC9, 0x8A);
    TEST("LD", "C, A", 0xC9, 0x8B);
    TEST("LD", "D, A", 0xC9, 0x8C);
    TEST("LD", "E, A", 0xC9, 0x8D);
    TEST("LD", "H, A", 0xC9, 0x8E);
    TEST("LD", "L, A", 0xC9, 0x8F);
    // LD r8, #n8
    TEST("LD", "A, 34H", 0x21, 0x34);
    TEST("LD", "W, 34H", 0x20, 0x34);
    TEST("LD", "B, 34H", 0x22, 0x34);
    TEST("LD", "C, 34H", 0x23, 0x34);
    TEST("LD", "D, 34H", 0x24, 0x34);
    TEST("LD", "E, 34H", 0x25, 0x34);
    TEST("LD", "H, 34H", 0x26, 0x34);
    TEST("LD", "L, 34H", 0x27, 0x34);
    // LD r8, (mem)
    TEST("LD", "A, (XIX)",       0x84, 0x21);
    TEST("LD", "A, (XIY)",       0x85, 0x21);
    TEST("LD", "A, (XIZ)",       0x86, 0x21);
    TEST("LD", "A, (XSP)",       0x87, 0x21);
    TEST("LD", "A, (-XIX)",      0x80, 0x21);
    TEST("LD", "A, (-XIY)",      0x81, 0x21);
    TEST("LD", "A, (-XIZ)",      0x82, 0x21);
    TEST("LD", "A, (XIX+)",      0x88, 0x21);
    TEST("LD", "A, (XIY+)",      0x89, 0x21);
    TEST("LD", "A, (XIZ+)",      0x8A, 0x21);
    TEST("LD", "A, (XSP+)",      0x8B, 0x21);
    // complex encoding of pre-dec/post-inc (ASL compatibility)
    TEST("LD", "A, (-XIX)",      0xC4, 0xF0, 0x21);
    TEST("LD", "A, (-XIY)",      0xC4, 0xF4, 0x21);
    TEST("LD", "A, (XIX+)",      0xC5, 0xF0, 0x21);
    TEST("LD", "A, (XIY+)",      0xC5, 0xF4, 0x21);
    TEST("LD", "A, (XIX+32)",    0x8C, 0x20, 0x21);
    TEST("LD", "A, (XIX-32)",    0x8C, 0xE0, 0x21);
    TEST("LD", "A, (XIX+1234H)", 0xC3, 0xF1, 0x34, 0x12, 0x21);
    TEST("LD", "A, (34H)",       0xC0, 0x34, 0x21);
    TEST("LD", "A, (1234H)",     0xC1, 0x34, 0x12, 0x21);
    TEST("LD", "A, (123456H)",   0xC2, 0x56, 0x34, 0x12, 0x21);
    TEST("LD", "A, (XIX+A)",     0xC3, 0x03, 0xF0, 0xE0, 0x21);
    TEST("LD", "A, (XIX+B)",     0xC3, 0x03, 0xF0, 0xE5, 0x21);
    // LD (mem), r8
    TEST("LD", "(XIX), A",       0xB4, 0x41);
    TEST("LD", "(XIY), A",       0xB5, 0x41);
    TEST("LD", "(-XIX), A",      0xF4, 0xF0, 0x41);
    TEST("LD", "(XIX+), A",      0xB8, 0x41);
    TEST("LD", "(XIX+32), A",    0xBC, 0x20, 0x41);
    TEST("LD", "(XIX+1234H), A", 0xF3, 0xF1, 0x34, 0x12, 0x41);
    TEST("LD", "(34H), A",       0xF0, 0x34, 0x41);
    TEST("LD", "(1234H), A",     0xF1, 0x34, 0x12, 0x41);
    TEST("LD", "(123456H), A",   0xF2, 0x56, 0x34, 0x12, 0x41);
    TEST("LD", "(XIX+A), A",     0xF3, 0x03, 0xF0, 0xE0, 0x41);
    // LD (mem), #n8
    TEST("LD", "(XIX), 34H",       0xB4, 0x00, 0x34);
    TEST("LD", "(-XIX), 34H",      0xF4, 0xF0, 0x00, 0x34);
    TEST("LD", "(XIX+), 34H",      0xB8, 0x00, 0x34);
    TEST("LD", "(XIX+32), 34H",    0xBC, 0x20, 0x00, 0x34);
    TEST("LD", "(XIX+1234H), 34H", 0xF3, 0xF1, 0x34, 0x12, 0x00, 0x34);
    TEST("LD", "(34H), 34H",       0x08, 0x34, 0x34);
    TEST("LD", "(1234H), 34H",     0xF1, 0x34, 0x12, 0x00, 0x34);
    TEST("LD", "(123456H), 34H",   0xF2, 0x56, 0x34, 0x12, 0x00, 0x34);
    TEST("LD", "(XIX+A), 34H",     0xF3, 0x03, 0xF0, 0xE0, 0x00, 0x34);
    // LD (mem), (mem): two equivalent encodings — dst-prefixed (PM_MEMD)
    // and src-prefixed (PM_MEM8). Disassembler must accept both.
    TEST("LD", "(1234H), (0ABCDH)", 0xF1, 0x34, 0x12, 0x14, 0xCD, 0xAB);  // dst-prefixed (ASL form)
    TEST("LD", "(1234H), (0ABCDH)", 0xC1, 0xCD, 0xAB, 0x19, 0x34, 0x12);  // src-prefixed
    TEST("LD", "(1234H), (56H)",    0xC0, 0x56, 0x19, 0x34, 0x12);        // 8-bit src absolute
}

void test_ld16() {
    // LD r16, r16
    TEST("LD", "WA, WA", 0xD8, 0x88);
    TEST("LD", "WA, BC", 0xD9, 0x88);
    TEST("LD", "WA, DE", 0xDA, 0x88);
    TEST("LD", "WA, HL", 0xDB, 0x88);
    TEST("LD", "WA, IX", 0xDC, 0x88);
    TEST("LD", "WA, IY", 0xDD, 0x88);
    TEST("LD", "WA, IZ", 0xDE, 0x88);
    TEST("LD", "WA, SP", 0xDF, 0x88);
    TEST("LD", "BC, WA", 0xD8, 0x89);
    TEST("LD", "DE, WA", 0xD8, 0x8A);
    TEST("LD", "HL, WA", 0xD8, 0x8B);
    TEST("LD", "IX, WA", 0xD8, 0x8C);
    TEST("LD", "IY, WA", 0xD8, 0x8D);
    TEST("LD", "IZ, WA", 0xD8, 0x8E);
    TEST("LD", "SP, WA", 0xD8, 0x8F);
    // LD r16, #n16
    TEST("LD", "WA, 1234H", 0x30, 0x34, 0x12);
    TEST("LD", "BC, 1234H", 0x31, 0x34, 0x12);
    TEST("LD", "DE, 1234H", 0x32, 0x34, 0x12);
    TEST("LD", "HL, 1234H", 0x33, 0x34, 0x12);
    TEST("LD", "IX, 1234H", 0x34, 0x34, 0x12);
    TEST("LD", "IY, 1234H", 0x35, 0x34, 0x12);
    TEST("LD", "IZ, 1234H", 0x36, 0x34, 0x12);
    TEST("LD", "SP, 1234H", 0x37, 0x34, 0x12);
    // LD r16, (mem)
    TEST("LD", "WA, (XIX)",       0x94, 0x20);
    TEST("LD", "WA, (XIY)",       0x95, 0x20);
    TEST("LD", "WA, (-XIX)",      0x90, 0x20);
    TEST("LD", "WA, (XIX+)",      0x98, 0x20);
    TEST("LD", "WA, (XIX+32)",    0x9C, 0x20, 0x20);
    TEST("LD", "WA, (XIX+1234H)", 0xD3, 0xF1, 0x34, 0x12, 0x20);
    TEST("LD", "WA, (34H)",       0xD0, 0x34, 0x20);
    TEST("LD", "WA, (1234H)",     0xD1, 0x34, 0x12, 0x20);
    TEST("LD", "WA, (123456H)",   0xD2, 0x56, 0x34, 0x12, 0x20);
    TEST("LD", "WA, (XIX+A)",     0xD3, 0x03, 0xF0, 0xE0, 0x20);
    // LD (mem), r16
    TEST("LD", "(XIX), WA",       0xB4, 0x50);
    TEST("LD", "(XIY), WA",       0xB5, 0x50);
    TEST("LD", "(-XIX), WA",      0xF4, 0xF1, 0x50);
    TEST("LD", "(XIX+), WA",      0xB8, 0x50);
    TEST("LD", "(XIX+32), WA",    0xBC, 0x20, 0x50);
    TEST("LD", "(XIX+1234H), WA", 0xF3, 0xF1, 0x34, 0x12, 0x50);
    TEST("LD", "(34H), WA",       0xF0, 0x34, 0x50);
    TEST("LD", "(1234H), WA",     0xF1, 0x34, 0x12, 0x50);
    TEST("LD", "(123456H), WA",   0xF2, 0x56, 0x34, 0x12, 0x50);
    TEST("LD", "(XIX+A), WA",     0xF3, 0x03, 0xF0, 0xE0, 0x50);
}

void test_ld32() {
    // LD r32, r32
    TEST("LD", "XWA, XWA", 0xE8, 0x88);
    TEST("LD", "XWA, XBC", 0xE9, 0x88);
    TEST("LD", "XWA, XDE", 0xEA, 0x88);
    TEST("LD", "XWA, XHL", 0xEB, 0x88);
    TEST("LD", "XWA, XIX", 0xEC, 0x88);
    TEST("LD", "XWA, XIY", 0xED, 0x88);
    TEST("LD", "XWA, XIZ", 0xEE, 0x88);
    TEST("LD", "XWA, XSP", 0xEF, 0x88);
    TEST("LD", "XIX, XWA", 0xE8, 0x8C);
    TEST("LD", "XIY, XWA", 0xE8, 0x8D);
    TEST("LD", "XIZ, XWA", 0xE8, 0x8E);
    TEST("LD", "XSP, XWA", 0xE8, 0x8F);
    // LD r32, #n32
    TEST("LD", "XWA, 12345678H", 0x40, 0x78, 0x56, 0x34, 0x12);
    TEST("LD", "XBC, 12345678H", 0x41, 0x78, 0x56, 0x34, 0x12);
    TEST("LD", "XIX, 12345678H", 0x44, 0x78, 0x56, 0x34, 0x12);
    TEST("LD", "XIY, 12345678H", 0x45, 0x78, 0x56, 0x34, 0x12);
    // LD r32, (mem)
    TEST("LD", "XWA, (XIX)",       0xA4, 0x20);
    TEST("LD", "XWA, (-XIX)",      0xA0, 0x20);
    TEST("LD", "XWA, (XIX+)",      0xA8, 0x20);
    TEST("LD", "XWA, (XIX+32)",    0xAC, 0x20, 0x20);
    TEST("LD", "XWA, (XIX+1234H)", 0xE3, 0xF1, 0x34, 0x12, 0x20);
    TEST("LD", "XWA, (34H)",       0xE0, 0x34, 0x20);
    TEST("LD", "XWA, (1234H)",     0xE1, 0x34, 0x12, 0x20);
    TEST("LD", "XWA, (123456H)",   0xE2, 0x56, 0x34, 0x12, 0x20);
    TEST("LD", "XWA, (XIX+A)",     0xE3, 0x03, 0xF0, 0xE0, 0x20);
}

void test_push_pop() {
    TEST("PUSH", "34H", 0x09, 0x34);
    TEST("PUSH", "F",   0x18);
    TEST("POP",  "F",   0x19);
    TEST("PUSH", "WA",  0x28);
    TEST("PUSH", "BC",  0x29);
    TEST("PUSH", "HL",  0x2B);
    TEST("PUSH", "SP",  0x2F);
    TEST("POP",  "WA",  0x48);
    TEST("POP",  "BC",  0x49);
    TEST("POP",  "HL",  0x4B);
    TEST("POP",  "SP",  0x4F);
    TEST("PUSH", "XWA", 0x38);
    TEST("PUSH", "XBC", 0x39);
    TEST("PUSH", "XIX", 0x3C);
    TEST("POP",  "XWA", 0x58);
    TEST("POP",  "XBC", 0x59);
    TEST("POP",  "XIX", 0x5C);
    // PUSH/POP from/to memory
    TEST("PUSH", "(XIX)",       0x84, 0x04);
    TEST("PUSH", "(XIY)",       0x85, 0x04);
    TEST("PUSH", "(-XIX)",      0x80, 0x04);
    TEST("PUSH", "(XIX+)",      0x88, 0x04);
    TEST("PUSH", "(XIX+32)",    0x8C, 0x20, 0x04);
    TEST("PUSH", "(34H)",       0xC0, 0x34, 0x04);
    TEST("PUSH", "(1234H)",     0xC1, 0x34, 0x12, 0x04);
    TEST("POP",  "(XIX)",       0xB4, 0x04);
    TEST("POP",  "(XIY)",       0xB5, 0x04);
    TEST("POP",  "(-XIX)",      0xF4, 0xF2, 0x04);
    TEST("POP",  "(XIX+)",      0xB8, 0x04);
    TEST("POP",  "(XIX+32)",    0xBC, 0x20, 0x04);
    TEST("POP",  "(34H)",       0xF0, 0x34, 0x04);
    TEST("POP",  "(1234H)",     0xF1, 0x34, 0x12, 0x04);
}

void test_alu() {
    // ALU r8, r8
    TEST("ADD", "A, B",   0xCA, 0x81);
    TEST("ADD", "A, A",   0xC9, 0x81);
    TEST("ADC", "A, B",   0xCA, 0x91);
    TEST("SUB", "A, B",   0xCA, 0xA1);
    TEST("SBC", "A, B",   0xCA, 0xB1);
    TEST("EX",  "A, B",   0xCA, 0xB9);
    TEST("AND", "A, B",   0xCA, 0xC1);
    TEST("XOR", "A, B",   0xCA, 0xD1);
    TEST("OR",  "A, B",   0xCA, 0xE1);
    TEST("CP",  "A, B",   0xCA, 0xF1);
    // ALU r16, r16
    TEST("ADD", "WA, BC", 0xD9, 0x80);
    TEST("SUB", "BC, WA", 0xD8, 0xA1);
    TEST("EX",  "WA, BC", 0xD9, 0xB8);
    TEST("CP",  "BC, WA", 0xD8, 0xF1);
    // ALU r32, r32
    TEST("ADD", "XWA, XBC", 0xE9, 0x80);
    // ALU r8, #n8
    TEST("ADD", "A, 34H", 0xC9, 0xC8, 0x34);
    TEST("ADC", "A, 34H", 0xC9, 0xC9, 0x34);
    TEST("SUB", "A, 34H", 0xC9, 0xCA, 0x34);
    TEST("SBC", "A, 34H", 0xC9, 0xCB, 0x34);
    TEST("AND", "A, 34H", 0xC9, 0xCC, 0x34);
    TEST("XOR", "A, 34H", 0xC9, 0xCD, 0x34);
    TEST("OR",  "A, 34H", 0xC9, 0xCE, 0x34);
    TEST("CP",  "A, 34H", 0xC9, 0xCF, 0x34);
    // ALU r16, #n16
    TEST("ADD", "BC, 1234H", 0xD9, 0xC8, 0x34, 0x12);
    TEST("CP",  "HL, 1234H", 0xDB, 0xCF, 0x34, 0x12);
    // ALU r32, #n32
    TEST("ADD", "XWA, 12345678H", 0xE8, 0xC8, 0x78, 0x56, 0x34, 0x12);
    TEST("CP",  "XBC, 12345678H", 0xE9, 0xCF, 0x78, 0x56, 0x34, 0x12);
    // ALU r8, (mem)
    TEST("ADD", "A, (XIX)", 0x84, 0x81);
    TEST("ADC", "A, (XIX)", 0x84, 0x91);
    TEST("SUB", "A, (XIX)", 0x84, 0xA1);
    TEST("SBC", "A, (XIX)", 0x84, 0xB1);
    TEST("AND", "A, (XIX)", 0x84, 0xC1);
    TEST("XOR", "A, (XIX)", 0x84, 0xD1);
    TEST("OR",  "A, (XIX)", 0x84, 0xE1);
    TEST("CP",  "A, (XIX)", 0x84, 0xF1);
    // ALU r16, (mem)
    TEST("ADD", "WA, (XIX)", 0x94, 0x80);
    TEST("CP",  "HL, (XIX)", 0x94, 0xF3);
    // ALU r32, (mem)
    TEST("ADD", "XWA, (XIX)", 0xA4, 0x80);
    // CP (mem), #n8
    TEST("CP",  "(XIX), 34H",    0x84, 0x3F, 0x34);
    TEST("CP",  "(1234H), 34H",  0xC1, 0x34, 0x12, 0x3F, 0x34);
    // LD (abs16), (mem)
    TEST("LD",  "(1234H), (XIX)", 0x84, 0x19, 0x34, 0x12);
    // RLD/RRD
    TEST("RLD", "A, (XIX)", 0x84, 0x06);
    TEST("RRD", "A, (XIX)", 0x84, 0x07);
    // MUL/MULS/DIV/DIVS from memory
    TEST("MUL",  "WA, (XIX)", 0x84, 0x41);
    TEST("MULS", "WA, (XIX)", 0x84, 0x49);
    TEST("DIV",  "WA, (XIX)", 0x84, 0x51);
    TEST("DIVS", "WA, (XIX)", 0x84, 0x59);
    // LD (mem), #n16/#n32 (MEMD context)
    TEST("LD", "(XIX), 1234H",    0xB4, 0x01, 0x34, 0x12);
    TEST("LD", "(XIX), 12345678H",0xB4, 0x02, 0x78, 0x56, 0x34, 0x12);
    // LD (mem), (abs16)
    TEST("LD", "(XIX), (1234H)",  0xB4, 0x14, 0x34, 0x12);
    // LDA r16, (mem)
    TEST("LDA", "WA, (XIX)",       0xB4, 0x20);
    TEST("LDA", "BC, (XIX)",       0xB4, 0x21);
    TEST("LDA", "HL, (XIX)",       0xB4, 0x23);
    TEST("LDA", "WA, (XIX+32)",    0xBC, 0x20, 0x20);
    TEST("LDA", "WA, (1234H)",     0xF1, 0x34, 0x12, 0x20);
    // JP/CALL cc, (mem)
    TEST("JP",   "Z, (XIX)",       0xB4, 0xD6);
    TEST("JP",   "NZ, (XIX)",      0xB4, 0xDE);
    TEST("JP",   "F, (XIX)",       0xB4, 0xD0);
    TEST("CALL", "Z, (XIX)",       0xB4, 0xE6);
    TEST("CALL", "NC, (XIX+32)",   0xBC, 0x20, 0xEF);
}

void test_bit() {
    // Bit ops on registers (bit in following byte)
    TEST("ANDCF", "2, A",  0xC9, 0x20, 0x02);
    TEST("ORCF",  "5, A",  0xC9, 0x21, 0x05);
    TEST("XORCF", "6, A",  0xC9, 0x22, 0x06);
    TEST("LDCF",  "7, A",  0xC9, 0x23, 0x07);
    TEST("STCF",  "0, A",  0xC9, 0x24, 0x00);
    TEST("RES",   "3, A",  0xC9, 0x30, 0x03);
    TEST("SET",   "7, A",  0xC9, 0x31, 0x07);
    TEST("CHG",   "0, A",  0xC9, 0x32, 0x00);
    TEST("BIT",   "4, A",  0xC9, 0x33, 0x04);
    TEST("TSET",  "1, A",  0xC9, 0x34, 0x01);
    // 16-bit register
    TEST("RES",   "3, BC", 0xD9, 0x30, 0x03);
    TEST("SET",   "7, HL", 0xDB, 0x31, 0x07);
    TEST("BIT",   "4, WA", 0xD8, 0x33, 0x04);
    // 32-bit register
    TEST("RES",   "0, XWA", 0xE8, 0x30, 0x00);
    TEST("SET",   "7, XBC", 0xE9, 0x31, 0x07);
    // Bit ops on memory (bit in opcode low 3 bits)
    TEST("ANDCF", "0, (XIX)", 0xB4, 0x80);
    TEST("ANDCF", "7, (XIX)", 0xB4, 0x87);
    TEST("ORCF",  "0, (XIX)", 0xB4, 0x88);
    TEST("ORCF",  "7, (XIX)", 0xB4, 0x8F);
    TEST("XORCF", "3, (XIX)", 0xB4, 0x93);
    TEST("LDCF",  "4, (XIX)", 0xB4, 0x9C);
    TEST("STCF",  "2, (XIX)", 0xB4, 0xA2);
    TEST("TSET",  "1, (XIX)", 0xB4, 0xA9);
    TEST("RES",   "3, (XIX)", 0xB4, 0xB3);
    TEST("SET",   "7, (XIX)", 0xB4, 0xBF);
    TEST("CHG",   "0, (XIX)", 0xB4, 0xC0);
    TEST("BIT",   "5, (XIX)", 0xB4, 0xCD);
    // With different memory addressing modes
    TEST("RES",   "3, (34H)",   0xF0, 0x34, 0xB3);
    TEST("SET",   "7, (1234H)", 0xF1, 0x34, 0x12, 0xBF);
}

void test_shift() {
    // Rotate/shift on registers
    TEST("RLC", "1, A",  0xC9, 0xE8, 0x01);
    TEST("RRC", "2, A",  0xC9, 0xE9, 0x02);
    TEST("RL",  "3, A",  0xC9, 0xEA, 0x03);
    TEST("RR",  "4, A",  0xC9, 0xEB, 0x04);
    TEST("SLA", "1, A",  0xC9, 0xEC, 0x01);
    TEST("SRA", "2, A",  0xC9, 0xED, 0x02);
    TEST("SLL", "3, A",  0xC9, 0xEE, 0x03);
    TEST("SRL", "4, A",  0xC9, 0xEF, 0x04);
    // 16-bit register
    TEST("RLC", "1, BC", 0xD9, 0xE8, 0x01);
    TEST("SRL", "3, BC", 0xD9, 0xEF, 0x03);
    // On memory
    TEST("RLC", "1, (XIX)", 0x84, 0xE8, 0x01);
    TEST("SRL", "4, (XIX)", 0x84, 0xEF, 0x04);
}

void test_inc_dec() {
    // INC/DEC on registers
    TEST("INC", "1, A",  0xC9, 0x61);
    TEST("INC", "2, A",  0xC9, 0x62);
    TEST("INC", "4, A",  0xC9, 0x64);
    TEST("DEC", "1, A",  0xC9, 0x69);
    TEST("DEC", "2, A",  0xC9, 0x6A);
    TEST("DEC", "4, A",  0xC9, 0x6C);
    // 16-bit register
    TEST("INC", "1, BC", 0xD9, 0x61);
    TEST("INC", "2, HL", 0xDB, 0x62);
    TEST("DEC", "4, WA", 0xD8, 0x6C);
    // On memory
    TEST("INC", "1, (XIX)", 0x84, 0x61);
    TEST("INC", "4, (XIX)", 0x84, 0x64);
    TEST("DEC", "2, (XIX)", 0x84, 0x6A);
}

void test_unary() {
    // CPL/NEG
    TEST("CPL", "A",   0xC9, 0x06);
    TEST("NEG", "A",   0xC9, 0x07);
    TEST("CPL", "BC",  0xD9, 0x06);
    TEST("NEG", "BC",  0xD9, 0x07);
    TEST("CPL", "XWA", 0xE8, 0x06);
    TEST("NEG", "XWA", 0xE8, 0x07);
    // DAA/EXTZ/EXTS/PAA/MIRR/MULA
    TEST("DAA",  "A",   0xC9, 0x10);
    TEST("EXTZ", "BC",  0xD9, 0x12);
    TEST("EXTS", "BC",  0xD9, 0x13);
    TEST("PAA",  "XBC", 0xE9, 0x14);
    TEST("MIRR", "BC",  0xD9, 0x16);
    TEST("MULA", "BC",  0xD9, 0x19);
    // BS1F/BS1B
    TEST("BS1F", "A, WA", 0xD8, 0x0E);
    TEST("BS1F", "A, BC", 0xD9, 0x0E);
    TEST("BS1B", "A, HL", 0xDB, 0x0F);
    // LINK/UNLK
    TEST("LINK", "XBC, 1234H", 0xE9, 0x0C, 0x34, 0x12);
    TEST("UNLK", "XBC",        0xE9, 0x0D);
    // SCC
    TEST("SCC", "Z, A",   0xC9, 0x76);
    TEST("SCC", "NZ, A",  0xC9, 0x7E);
    TEST("SCC", "C, A",   0xC9, 0x77);
    TEST("SCC", "NC, BC", 0xD9, 0x7F);
    // MUL/MULS/DIV/DIVS on registers
    TEST("MUL",  "WA, A", 0xC9, 0x41);
    TEST("MULS", "WA, A", 0xC9, 0x49);
    TEST("DIV",  "WA, A", 0xC9, 0x51);
    TEST("DIVS", "WA, A", 0xC9, 0x59);
    TEST("MUL",  "WA, B", 0xCA, 0x41);
    TEST("DIV",  "WA, L", 0xCF, 0x51);
    // DJNZ (decrement r8 and branch if non-zero); at address 0, base=3, relative output
    TEST("DJNZ", "A, $+3", 0xC9, 0x1C, 0x00);
    TEST("DJNZ", "B, $+3", 0xCA, 0x1C, 0x00);
    TEST("DJNZ", "A, $",   0xC9, 0x1C, 0xFD);
}

void test_branch() {
    // Unconditional
    TEST("RET",  "", 0x0E);
    TEST("RETD", "1234H", 0x0F, 0x34, 0x12);
    // CALR rel16: at address 0, relative output
    TEST("CALR", "$+3", 0x1E, 0x00, 0x00);
    TEST("CALR", "$",   0x1E, 0xFD, 0xFF);
    // LDAR rd, target: F3 13 rel16 reg_byte. base = address + 4.
    TEST("LDAR", "WA, $+4",  0xF3, 0x13, 0x00, 0x00, 0x20);
    TEST("LDAR", "HL, $+4",  0xF3, 0x13, 0x00, 0x00, 0x23);
    TEST("LDAR", "XWA, $+4", 0xF3, 0x13, 0x00, 0x00, 0x30);
    TEST("LDAR", "XHL, $+4", 0xF3, 0x13, 0x00, 0x00, 0x33);
    // JP/CALL to absolute 16-bit/24-bit address
    TEST("JP",   "(1234H)",   0x1A, 0x34, 0x12);
    TEST("JP",   "(123456H)", 0x1B, 0x56, 0x34, 0x12);
    TEST("CALL", "(1234H)",   0x1C, 0x34, 0x12);
    TEST("CALL", "(123456H)", 0x1D, 0x56, 0x34, 0x12);
    // JR cc, rel8: at address 0, relative output ($=0, base=2)
    TEST("JR", "F, $+2",   0x60, 0x00);
    TEST("JR", "LT, $+2",  0x61, 0x00);
    TEST("JR", "Z, $+2",   0x66, 0x00);
    TEST("JR", "C, $+2",   0x67, 0x00);
    TEST("JR", "GE, $+2",  0x69, 0x00);
    TEST("JR", "NZ, $+2",  0x6E, 0x00);
    TEST("JR", "NC, $+2",  0x6F, 0x00);
    TEST("JR", "Z, $",     0x66, 0xFE);
    TEST("JR", "Z, $+129", 0x66, 0x7F);
    // JRL cc, rel16: at address 0, relative output ($=0, base=3)
    TEST("JRL", "Z, $+3",  0x76, 0x00, 0x00);
    TEST("JRL", "NZ, $+3", 0x7E, 0x00, 0x00);
    TEST("JRL", "Z, $",    0x76, 0xFD, 0xFF);
    // RET cc
    TEST("RET", "F",  0xB0, 0xF0);
    TEST("RET", "Z",  0xB0, 0xF6);
    TEST("RET", "NZ", 0xB0, 0xFE);
    TEST("RET", "NC", 0xB0, 0xFF);
    // Block instructions
    TEST("LDI",  "", 0x83, 0x10);
    TEST("LDIR", "", 0x83, 0x11);
    TEST("LDD",  "", 0x83, 0x12);
    TEST("LDDR", "", 0x83, 0x13);
    TEST("CPI",  "", 0x83, 0x14);
    TEST("CPIR", "", 0x83, 0x15);
    TEST("CPD",  "", 0x83, 0x16);
    TEST("CPDR", "", 0x83, 0x17);
}

void test_abs_reg() {
    // ABREG8: prefix 0xC7 + register code + opcode
    // LD dst_r8, abs_r8
    TEST("LD", "A, A0",  0xC7, 0x00, 0x89);  // bank 0, offset 0=A
    TEST("LD", "A, W0",  0xC7, 0x01, 0x89);  // bank 0, offset 1=W
    TEST("LD", "A, H0",  0xC7, 0x0D, 0x89);  // bank 0, offset D=H
    TEST("LD", "A, A'",  0xC7, 0xD0, 0x89);  // prev bank, offset 0=A
    TEST("LD", "A, H'",  0xC7, 0xDD, 0x89);  // prev bank, offset D=H
    TEST("LD", "A, A",   0xC7, 0xE0, 0x89);  // current bank, offset 0=A
    TEST("LD", "A, IXL", 0xC7, 0xF0, 0x89);  // fixed IXL
    TEST("LD", "A, QSPH",0xC7, 0xFF, 0x89);  // fixed QSPH
    TEST("LD", "W, A0",  0xC7, 0x00, 0x88);  // dst=W
    // ADD abs_r8, #n8
    TEST("ADD", "A0, 34H", 0xC7, 0x00, 0xC8, 0x34);
    TEST("CP",  "A', 34H", 0xC7, 0xD0, 0xCF, 0x34);
    // ABREG16: prefix 0xD7 + register code + opcode
    // LD dst_r16, abs_r16
    TEST("LD", "WA, WA0", 0xD7, 0x00, 0x88);  // bank 0, offset 0=WA
    TEST("LD", "BC, HL0", 0xD7, 0x0C, 0x89);  // bank 0, offset C=HL -> BC dst
    TEST("LD", "BC, WA'", 0xD7, 0xD0, 0x89);  // prev bank WA
    TEST("LD", "BC, WA",  0xD7, 0xE0, 0x89);  // current bank WA
    TEST("LD", "IX, IX",  0xD7, 0xF0, 0x8C);  // fixed IX
    TEST("LD", "SP, QSP", 0xD7, 0xFE, 0x8F);  // fixed QSP
    // ADD abs_r16, #n16
    TEST("ADD", "WA0, 1234H", 0xD7, 0x00, 0xC8, 0x34, 0x12);
    // ABREG32: prefix 0xE7 + register code + opcode
    // LD dst_r32, abs_r32
    TEST("LD", "XWA, XWA0", 0xE7, 0x00, 0x88);  // bank 0, offset 0=XWA
    TEST("LD", "XHL, XHL0", 0xE7, 0x0C, 0x8B);  // bank 0, offset C=XHL -> XHL dst
    TEST("LD", "XHL, XWA'", 0xE7, 0xD0, 0x8B);  // prev bank XWA
    TEST("LD", "XHL, XWA",  0xE7, 0xE0, 0x8B);  // current bank XWA
    TEST("LD", "XHL, XIX",  0xE7, 0xF0, 0x8B);  // fixed XIX
    TEST("LD", "XHL, XSP",  0xE7, 0xFC, 0x8B);  // fixed XSP
    // ADD abs_r32, #n32
    TEST("ADD", "XWA0, 12345678H", 0xE7, 0x00, 0xC8, 0x78, 0x56, 0x34, 0x12);
    // LD abs_r as destination, from standard register (opcode 0x98 | reg)
    TEST("LD", "A0, A",      0xC7, 0x00, 0x99);  // abs A0 <- A (encodeReg8(A)=1)
    TEST("LD", "A0, W",      0xC7, 0x00, 0x98);  // abs A0 <- W (encodeReg8(W)=0)
    TEST("LD", "WA0, BC",    0xD7, 0x00, 0x99);  // abs WA0 <- BC
    TEST("LD", "XWA0, XBC",  0xE7, 0x00, 0x99);  // abs XWA0 <- XBC
    // LD abs_r as destination, from immediate (opcode 0x03, imm follows PM)
    TEST("LD", "A0, 34H",         0xC7, 0x00, 0x03, 0x34);
    TEST("LD", "WA0, 1234H",      0xD7, 0x00, 0x03, 0x34, 0x12);
    TEST("LD", "XWA0, 12345678H", 0xE7, 0x00, 0x03, 0x78, 0x56, 0x34, 0x12);
}

void test_illegal() {
    // First-byte holes that don't decode as anything.
    UNKN(0x1F);
    UNKN(0xC6);
    UNKN(0xD6);
    UNKN(0xE6);
    UNKN(0xF6);
    // Gap between POP rr (0x48-0x4F) and POP xrr (0x58-0x5F).
    for (uint8_t opc = 0x50; opc <= 0x57; opc++)
        UNKN(opc);

    // NORMAL (0x01) is base only; /L has only MIN at 0x04. /H and /L1
    // expose neither, so both 0x01 and 0x04 are illegal.
    if (is_tlcs900l())
        UNKN(0x01);
    if (is_tlcs900h() || is_tlcs900l1()) {
        UNKN(0x01);
        UNKN(0x04);
    }

    // PM_BLOCK (0x83) valid sub-byte range is 0x10-0x17.
    UNKN(0x83, 0x00);
    UNKN(0x83, 0x0F);
    UNKN(0x83, 0x18);
    UNKN(0x83, 0xFF);

    // PM_RETCC (0xB0) valid sub-byte range is 0xF0-0xFF (RET cc).
    UNKN(0xB0, 0x00);
    UNKN(0xB0, 0xEF);

    // PM_LDAR (0xF3 0x13) valid trailing reg-byte range is 0x20-0x27 or
    // 0x30-0x37 (LDAR rr / xrr).
    UNKN(0xF3, 0x13, 0x00, 0x00, 0x00);
    UNKN(0xF3, 0x13, 0x00, 0x00, 0x1F);
    UNKN(0xF3, 0x13, 0x00, 0x00, 0x28);
    UNKN(0xF3, 0x13, 0x00, 0x00, 0x2F);
    UNKN(0xF3, 0x13, 0x00, 0x00, 0x38);
    UNKN(0xF3, 0x13, 0x00, 0x00, 0xFF);
}

// clang-format on
void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_single);
    if (is_tlcs900l())
        RUN_TEST(test_tlcs900l);
    RUN_TEST(test_ld8);
    RUN_TEST(test_ld16);
    RUN_TEST(test_ld32);
    RUN_TEST(test_push_pop);
    RUN_TEST(test_alu);
    RUN_TEST(test_bit);
    RUN_TEST(test_shift);
    RUN_TEST(test_inc_dec);
    RUN_TEST(test_unary);
    RUN_TEST(test_branch);
    RUN_TEST(test_abs_reg);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
