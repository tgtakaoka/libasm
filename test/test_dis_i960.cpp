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

#include "dis_i960.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::i960;
using namespace libasm::test;

DisI960 disi960;
Disassembler &disassembler(disi960);

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

void test_cpu() {
    EQUALS("cpu 80960", true, disassembler.setCpu("80960"));
    EQUALS_P("cpu 80960", "80960", disassembler.config().cpu_P());
}

// Integer and ordinal arithmetic, and the extended forms.  A set mode bit
// makes a source field a five-bit literal instead of a register.
void test_arithmetic() {
    TEST("ADDI", "G0, G1, G2", 0x59944090);
    TEST("ADDI", "31, G1, G2", 0x5994489F);
    TEST("ADDI", "G0, 0, G2", 0x59901090);
    TEST("ADDI", "1, 2, G3", 0x59989881);
    TEST("ADDO", "G0, G1, G2", 0x59944010);
    TEST("ADDO", "16, G1, G2", 0x59944810);
    TEST("ADDC", "G0, G1, G2", 0x5B944010);
    TEST("ADDC", "G0, 7, G2", 0x5B91D010);
    TEST("SUBI", "G0, G1, G2", 0x59944190);
    TEST("SUBI", "31, G1, G2", 0x5994499F);
    TEST("SUBO", "G0, G1, G2", 0x59944110);
    TEST("SUBO", "G0, 31, G2", 0x5997D110);
    TEST("SUBC", "G0, G1, G2", 0x5B944110);
    TEST("MULI", "G0, G1, G2", 0x74944090);
    TEST("MULI", "3, G1, G2", 0x74944883);
    TEST("MULO", "G0, G1, G2", 0x70944090);
    TEST("DIVI", "G0, G1, G2", 0x74944590);
    TEST("DIVO", "G0, G1, G2", 0x70944590);
    TEST("DIVO", "G0, 4, G2", 0x70911590);
    TEST("REMI", "G0, G1, G2", 0x74944410);
    TEST("REMO", "G0, G1, G2", 0x70944410);
    TEST("MODI", "G0, G1, G2", 0x74944490);
    TEST("EMUL", "G0, G1, G4", 0x67A44010);
    TEST("EMUL", "5, G1, G4", 0x67A44805);
    TEST("EDIV", "G0, G4, G2", 0x67950090);
    TEST("AND", "G0, G1, G2", 0x58944090);
}

// Logical
void test_logical() {
    TEST("ANDNOT", "G0, G1, G2", 0x58944110);
    TEST("ANDNOT", "31, G1, G2", 0x5894491F);
    TEST("NAND", "G0, G1, G2", 0x58944710);
    TEST("NOR", "G0, G1, G2", 0x58944410);
    TEST("NOT", "G0, G2", 0x58901510);
    TEST("NOT", "31, G2", 0x58901D1F);
    TEST("NOTAND", "G0, G1, G2", 0x58944210);
    TEST("NOTOR", "G0, G1, G2", 0x58944690);
    TEST("OR", "G0, G1, G2", 0x58944390);
    TEST("ORNOT", "G0, G1, G2", 0x58944590);
    TEST("XNOR", "G0, G1, G2", 0x58944490);
    TEST("XOR", "G0, G1, G2", 0x58944310);
    TEST("XOR", "8, 9, G2", 0x58925B08);
    TEST("ALTERBIT", "0, G1, G2", 0x58944F80);
}

// Bit, bit-field and shift
void test_bit_field() {
    TEST("ALTERBIT", "31, G1, G2", 0x58944F9F);
    TEST("CHKBIT", "G0, G1", 0x5A046710);
    TEST("CHKBIT", "5, G1", 0x5A046F05);
    TEST("CLRBIT", "G0, G1, G2", 0x58944610);
    TEST("NOTBIT", "G0, G1, G2", 0x58944010);
    TEST("SETBIT", "G0, G1, G2", 0x58944190);
    TEST("SCANBIT", "G0, G2", 0x64901090);
    TEST("SPANBIT", "G0, G2", 0x64901010);
    TEST("SCANBYTE", "G0, G1", 0x5A046610);
    TEST("SCANBYTE", "0, G1", 0x5A046E00);
    TEST("EXTRACT", "G0, G1, G2", 0x65944090);
    TEST("EXTRACT", "4, 8, G2", 0x65921884);
    TEST("MODIFY", "G0, G1, G2", 0x65944010);
    TEST("MODIFY", "31, G1, G2", 0x6594481F);
    TEST("ROTATE", "G0, G1, G2", 0x59944690);
}

// Compare
void test_compare() {
    TEST("ROTATE", "1, G1, G2", 0x59944E81);
    TEST("SHLI", "G0, G1, G2", 0x59944710);
    TEST("SHLO", "G0, G1, G2", 0x59944610);
    TEST("SHLO", "31, G1, G2", 0x59944E1F);
    TEST("SHRDI", "G0, G1, G2", 0x59944510);
    TEST("SHRI", "G0, G1, G2", 0x59944590);
    TEST("SHRO", "G0, G1, G2", 0x59944410);
    TEST("CMPI", "G0, G1", 0x5A046090);
    TEST("CMPI", "31, G1", 0x5A04689F);
    TEST("CMPI", "G0, 0", 0x5A003090);
}

// Move.  movl names an even register and movt and movq a multiple of four.
void test_move() {
    TEST("CMPO", "G0, G1", 0x5A046010);
    TEST("CONCMPI", "G0, G1", 0x5A046190);
    TEST("CONCMPO", "G0, G1", 0x5A046110);
    TEST("CMPDECI", "G0, G1, G2", 0x5A944390);
    TEST("CMPDECO", "G0, G1, G2", 0x5A944310);
    TEST("CMPINCI", "G0, G1, G2", 0x5A944290);
    TEST("CMPINCO", "G0, G1, G2", 0x5A944210);
    TEST("MOV", "G0, G2", 0x5C901610);
    TEST("MOV", "31, G2", 0x5C901E1F);
    TEST("MOV", "G0, SP", 0x5C081610);
    TEST("MOV", "PFP, RIP", 0x5C101600);
    TEST("MOV", "FP, G2", 0x5C90161F);
}

// Processor management
void test_processor() {
    TEST("MOVL", "G0, G4", 0x5DA01610);
    TEST("MOVL", "R4, R8", 0x5D401604);
    TEST("MOVT", "G0, G4", 0x5EA01610);
    TEST("MOVT", "R4, R8", 0x5E401604);
    TEST("MOVQ", "G0, G4", 0x5FA01610);
    TEST("MOVQ", "R8, R12", 0x5F601608);
    TEST("CALLS", "G0", 0x66003010);
    TEST("CALLS", "7", 0x66003807);
    TEST("FLUSHREG", "", 0x66003E80);
    TEST("FMARK", "", 0x66003E00);
    TEST("MARK", "", 0x66003D80);
}

// Atomic and synchronous
void test_synchronous() {
    TEST("SYNCF", "", 0x66003F80);
    TEST("MODAC", "G0, G1, G2", 0x64944290);
    TEST("MODAC", "0, G1, G2", 0x64944A80);
    TEST("MODPC", "G0, G1, G2", 0x65944290);
    TEST("MODTC", "G0, G1, G2", 0x65944210);
    TEST("ATADD", "G0, G1, G2", 0x61944110);
}

// CTRL format: the operand is the target address
void test_branch() {
    TEST("ATMOD", "G0, G1, G2", 0x61944010);
    TEST("SYNLD", "G0, G2", 0x61901290);
    TEST("SYNMOV", "G0, G1", 0x60046010);
    TEST("SYNMOVL", "G0, G1", 0x60046090);
    TEST("SYNMOVQ", "G0, G1", 0x60046110);
    ATEST(0x0010018C, "B", "0x0010018C", 0x08000000);
    ATEST(0x00100190, "BAL", "0x0010018C", 0x0BFFFFFC);
    ATEST(0x00100194, "CALL", "0x0010018C", 0x09FFFFF8);
    TEST("RET", "", 0x0A000000);
    ATEST(0x0010019C, "BNO", "0x0010018C", 0x10FFFFF0);
    ATEST(0x001001A0, "BG", "0x0010018C", 0x11FFFFEC);
    ATEST(0x001001A4, "BE", "0x0010018C", 0x12FFFFE8);
    ATEST(0x001001A8, "BGE", "0x0010018C", 0x13FFFFE4);
    ATEST(0x001001AC, "BL", "0x0010018C", 0x14FFFFE0);
    ATEST(0x001001B0, "BNE", "0x0010018C", 0x15FFFFDC);
    ATEST(0x001001B4, "BLE", "0x0010018C", 0x16FFFFD8);
    ATEST(0x001001B8, "BO", "0x0010018C", 0x17FFFFD4);
    TEST("FAULTNO", "", 0x18000000);
    TEST("FAULTG", "", 0x19000000);
    TEST("FAULTE", "", 0x1A000000);
}

// COBR format: compare and branch
void test_compare_branch() {
    TEST("FAULTGE", "", 0x1B000000);
    TEST("FAULTL", "", 0x1C000000);
    TEST("FAULTNE", "", 0x1D000000);
    TEST("FAULTLE", "", 0x1E000000);
    TEST("FAULTO", "", 0x1F000000);
    ATEST(0x001001DC, "CMPIBNO", "G0, G1, 0x001001DC", 0x38844000);
    ATEST(0x001001E0, "CMPIBG", "G0, G1, 0x001001DC", 0x39845FFC);
    ATEST(0x001001E4, "CMPIBE", "G0, G1, 0x001001DC", 0x3A845FF8);
    ATEST(0x001001E8, "CMPIBGE", "G0, G1, 0x001001DC", 0x3B845FF4);
    ATEST(0x001001EC, "CMPIBL", "G0, G1, 0x001001DC", 0x3C845FF0);
    ATEST(0x001001F0, "CMPIBNE", "G0, G1, 0x001001DC", 0x3D845FEC);
    ATEST(0x001001F4, "CMPIBLE", "G0, G1, 0x001001DC", 0x3E845FE8);
    ATEST(0x001001F8, "CMPIBO", "G0, G1, 0x001001DC", 0x3F845FE4);
    ATEST(0x001001FC, "CMPIBE", "31, G1, 0x001001DC", 0x3AFC7FE0);
    ATEST(0x00100200, "CMPOBG", "G0, G1, 0x001001DC", 0x31845FDC);
    ATEST(0x00100204, "CMPOBE", "G0, G1, 0x001001DC", 0x32845FD8);
    ATEST(0x00100208, "CMPOBGE", "G0, G1, 0x001001DC", 0x33845FD4);
    ATEST(0x0010020C, "CMPOBL", "G0, G1, 0x001001DC", 0x34845FD0);
    ATEST(0x00100210, "CMPOBNE", "G0, G1, 0x001001DC", 0x35845FCC);
}

// COBR format: test
void test_test() {
    ATEST(0x00100214, "CMPOBLE", "G0, G1, 0x001001DC", 0x36845FC8);
    ATEST(0x00100218, "CMPOBLE", "0, G1, 0x001001DC", 0x36047FC4);
    ATEST(0x0010021C, "BBC", "G0, G1, 0x001001DC", 0x30845FC0);
    ATEST(0x00100220, "BBC", "31, G1, 0x001001DC", 0x30FC7FBC);
    ATEST(0x00100224, "BBS", "G0, G1, 0x001001DC", 0x37845FB8);
    TEST("TESTNO", "G0", 0x20800000);
    TEST("TESTG", "G1", 0x21880000);
    TEST("TESTE", "G2", 0x22900000);
}

// MEM format: every addressing mode but IP-relative
void test_memory() {
    TEST("TESTGE", "G3", 0x23980000);
    TEST("TESTL", "G4", 0x24A00000);
    TEST("TESTNE", "G5", 0x25A80000);
    TEST("TESTLE", "G6", 0x26B00000);
    TEST("TESTO", "FP", 0x27F80000);
    TEST("LD", "0x040, G6", 0x90B00040);
    TEST("LD", "0xFFF, G6", 0x90B00FFF);
    TEST("LD", "0x040(G5), G6", 0x90B56040);
    TEST("LD", "(G5), G6", 0x90B55000);
    TEST("LD", "0x00001234(G5), G6", 0x90B57400, 0x00001234);
    TEST("LD", "0x12345678, G6", 0x90B03000, 0x12345678);
    TEST("LD", "(G5)[G4*1], G6", 0x90B55C14);
    TEST("LD", "(G5)[G4*2], G6", 0x90B55C94);
    TEST("LD", "(G5)[G4*4], G6", 0x90B55D14);
}

// The load and store family
void test_load_store() {
    TEST("LD", "(G5)[G4*8], G6", 0x90B55D94);
    TEST("LD", "(G5)[G4*16], G6", 0x90B55E14);
    TEST("LD", "0x00001234[G4*4], G6", 0x90B03914, 0x00001234);
    TEST("LD", "0x00001234(G5)[G4*4], G6", 0x90B57D14, 0x00001234);
    TEST("LD", "0xFFFFFFFF(G5)[G4*16], G6", 0x90B57E14, 0xFFFFFFFF);
    TEST("LDOB", "0x040(G5), G6", 0x80B56040);
    TEST("LDOS", "0x040(G5), G6", 0x88B56040);
    TEST("LDIB", "0x040(G5), G6", 0xC0B56040);
    TEST("LDIS", "0x040(G5), G6", 0xC8B56040);
    TEST("LD", "0x040(G5), G6", 0x90B56040);
    TEST("LDL", "0x040(G5), G4", 0x98A56040);
    TEST("LDT", "0x040(G5), G4", 0xA0A56040);
    TEST("LDQ", "0x040(G5), G4", 0xB0A56040);
    TEST("LDOB", "(G5)[G4*2], G6", 0x80B55C94);
    TEST("LDL", "0x00001234(G5), G8", 0x98C57400, 0x00001234);
    TEST("LDT", "0x00001234(G5), G8", 0xA0C57400, 0x00001234);
    TEST("LDQ", "0x00001234(G5), G8", 0xB0C57400, 0x00001234);
    TEST("STOB", "G6, 0x040(G5)", 0x82B56040);
    TEST("STOS", "G6, 0x040(G5)", 0x8AB56040);
    TEST("STIB", "G6, 0x040(G5)", 0xC2B56040);
    TEST("STIS", "G6, 0x040(G5)", 0xCAB56040);
    TEST("ST", "G6, 0x040(G5)", 0x92B56040);
    TEST("STL", "G4, 0x040(G5)", 0x9AA56040);
    TEST("STT", "G4, 0x040(G5)", 0xA2A56040);
    TEST("STQ", "G4, 0x040(G5)", 0xB2A56040);
    TEST("STOB", "G6, (G5)[G4*2]", 0x82B55C94);
    TEST("STL", "G8, 0x00001234(G5)", 0x9AC57400, 0x00001234);
    TEST("STT", "G8, 0x00001234(G5)", 0xA2C57400, 0x00001234);
    TEST("STQ", "G8, 0x00001234(G5)", 0xB2C57400, 0x00001234);
    TEST("LDA", "0x040, G6", 0x8CB00040);
    TEST("LDA", "0x040(G5), G6", 0x8CB56040);
    TEST("LDA", "0x12345678, G6", 0x8CB03000, 0x12345678);
}

// lda, and branch or call through a memory operand
void test_effective_address() {
    TEST("LDA", "0x00001234(G5)[G4*4], G6", 0x8CB57D14, 0x00001234);
    TEST("BX", "(G5)", 0x84055000);
    TEST("BX", "0x040(G5)", 0x84056040);
    TEST("BX", "0x12345678", 0x84003000, 0x12345678);
    TEST("BALX", "(G5), G6", 0x85B55000);
    TEST("BALX", "0x00001234(G5)[G4*4], G6", 0x85B57D14, 0x00001234);
    TEST("CALLX", "(G5)", 0x86055000);
    TEST("CALLX", "0x040", 0x86000040);
    TEST("CALLX", "0x00001234(G5)[G4*8]", 0x86057D94, 0x00001234);
}

// MEMB mode 5: the operand is the displacement, to which the processor adds
// the IP and a constant of 8 (Manual Table 5-1 and 5-8).
void test_ip_relative() {
    ATEST(0x100350, "LDOB", "-8(IP), G8", 0x80C01400, 0xFFFFFFF8);
    ATEST(0x100358, "LDOS", "0(IP), G8", 0x88C01400, 0x00000000);
    ATEST(0x100360, "LDIB", "4(IP), G8", 0xC0C01400, 0x00000004);
    ATEST(0x100368, "LDIS", "0x00000040(IP), G8", 0xC8C01400, 0x00000040);
    ATEST(0x100370, "LD", "-0x00000040(IP), G8", 0x90C01400, 0xFFFFFFC0);
    ATEST(0x100378, "LDL", "0x00001234(IP), G8", 0x98C01400, 0x00001234);
    ATEST(0x100380, "LDT", "-0x00001234(IP), G8", 0xA0C01400, 0xFFFFEDCC);
    ATEST(0x100388, "LDQ", "0x12345678(IP), G8", 0xB0C01400, 0x12345678);
    ATEST(0x100390, "STOB", "G8, -8(IP)", 0x82C01400, 0xFFFFFFF8);
    ATEST(0x100398, "ST", "G8, 0x00000040(IP)", 0x92C01400, 0x00000040);
    ATEST(0x1003A0, "LDA", "0(IP), G8", 0x8CC01400, 0x00000000);
    ATEST(0x1003A8, "BX", "-16(IP)", 0x84001400, 0xFFFFFFF0);
    ATEST(0x1003B0, "BALX", "32(IP), G8", 0x85C01400, 0x00000020);
    ATEST(0x1003B8, "CALLX", "-0x7FFFFFF0(IP)", 0x86001400, 0x80000010);
}
// A MEMB displacement which would also fit a MEMA operand assembles back to
// the shorter encoding.  GNU as and the Intel assembler reserve the 32-bit
// displacement for any operand naming a symbol, however low it turns out to
// be, so the longer form is ordinary code and is decoded by default; the
// canonical-mem option refuses it for a caller which needs the encoding to
// survive reassembly.
void test_canonical_mem() {
    TEST("LD", "0x00000040, G6", 0x90B03000, 0x00000040);
    TEST("LD", "0x00000040(G5), G6", 0x90B57400, 0x00000040);
    TEST("LD", "0x00000FFF, G6", 0x90B03000, 0x00000FFF);

    disassembler.setOption("canonical-mem", "on");
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B03000, 0x00000040);
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B57400, 0x00000040);
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B03000, 0x00000FFF);
    // One past the MEMA offset has no shorter encoding, so it is kept.
    TEST("LD", "0x00001000, G6", 0x90B03000, 0x00001000);
    // Nor has an indexed mode, MEMA having no index.
    TEST("LD", "0x00000040(G5)[G4*4], G6", 0x90B57D14, 0x00000040);
}

// The operand of the IP-relative mode is its offset from IP + 8;
// ip-operand=target writes it as the address to be reached instead, which is
// how ASL reads it.
void test_ip_operand() {
    ATEST(0x100350, "LDOB", "-8(IP), G8", 0x80C01400, 0xFFFFFFF8);

    disassembler.setOption("ip-operand", "target");
    ATEST(0x100350, "LDOB", "0x00100350(IP), G8", 0x80C01400, 0xFFFFFFF8);
    ATEST(0x100350, "LDOB", "0x00100398(IP), G8", 0x80C01400, 0x00000040);
}

// A field an instruction does not use carries a set mode bit, which is what
// GNU as and the Intel assembler emit.  Turning the option off expects the
// clear bit ASL writes instead; each form is unknown to the other.
void test_mode_bits() {
    // cmpi uses src1 and src2, so m3 belongs to the unused src/dst field.
    TEST("CMPI", "G0, G1", 0x5A046090);
    UNKN(0x5A044090);
    // not uses src1 and src/dst, so the unused field is src2.
    TEST("NOT", "G0, G2", 0x58901510);
    UNKN(0x58900510);
    // ret uses none of the three.
    TEST("RET", "", 0x0A000000);

    disassembler.setOption("mode-bits", "0");
    TEST("CMPI", "G0, G1", 0x5A044090);
    UNKN(0x5A046090);
    TEST("NOT", "G0, G2", 0x58900510);
    UNKN(0x58901510);
    TEST("RET", "", 0x0A000000);
}

// The opcode map in Appendix A of the 80960KB Programmer's Reference Manual
// leaves these undefined.  CTRL runs 08-0B and 10-1F, so 0C-0F is a hole;
// COBR runs 20-27 and 30-3F, so 28-2F is one; and the REG and MEM ranges are
// sparse.  Everything below 08 and above CF is undefined outright.
void test_illegal() {
    UNKN(0x00000000);
    UNKN(0x07000000);
    // CTRL hole between bal and bno.
    UNKN(0x0C000000);
    UNKN(0x0F000000);
    // Between the fault instructions and the test instructions.
    UNKN(0x1F800000);
    // COBR hole between testo and bbc.
    UNKN(0x28000000);
    UNKN(0x2F000000);
    // Between COBR and REG.
    UNKN(0x40000000);
    UNKN(0x57000000);
    // 585 is the one hole in the 580-58F row, which is otherwise full.
    UNKN(0x58002A80);
    // 594 through 597 and 599 are absent from the 590 row.
    UNKN(0x59002A00);
    UNKN(0x59002C80);
    // Above the MEM range.
    UNKN(0xD0000000);
    UNKN(0xFF000000);
}

// A reserved field, an undefined scale and a misaligned register pair or quad
// all leave the instruction undefined.  Rejecting them keeps a disassembled
// instruction one which assembles back to the same word.
void test_reserved_field() {
    // Bits 6 and 5 of a MEMB operand are reserved.
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B05020);
    // The scale is a power of two up to sixteen; 5 through 7 are undefined.
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B7B914, 0x00001234);
    // MEMB mode 6 is not defined.
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B59800);
    // A MEMA operand without an address base leaves that field zero.
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B04040);
    // A mode which names no index register leaves the index and scale zero.
    ERRT("LD", "", ILLEGAL_OPERAND_MODE, "", 0x90B05001);
    // One past the MEMA offset needs the MEMB form.
    TEST("LD", "0x00001000, G6", 0x90B03000, 0x00001000);
    TEST("LD", "0x00001000(G5), G6", 0x90B57400, 0x00001000);
    // An indexed mode may carry a small displacement, MEMA having no index.
    TEST("LD", "0x00000040(G5)[G4*4], G6", 0x90B57D14, 0x00000040);
    // ldl names an even register and ldq a multiple of four; these are the
    // encodings tested above with the src/dst field moved off the boundary.
    UNKN(0x98A86040);
    UNKN(0xB0A86040);
    // So does movl, in the source as well as the destination, and movq.
    UNKN(0x5DA81610);
    UNKN(0x5DA01611);
    UNKN(0x5FA81610);
    UNKN(0x5FA01611);
}

// A MEMB displacement word which the memory does not hold is a truncated
// instruction rather than an illegal one.
void test_no_memory() {
    NMEM("LD", "", "", 0x90B57400);
    NMEM("LDA", "", "", 0x8CB03000);
    NMEM("CALLX", "", "", 0x86003000);
}

// The 80960KB floating-point and decimal extensions, which the fpu option and
// the .fpu directive enable.  A source field takes a global or local register
// when its mode bit is clear and a floating-point register or one of the two
// real literals when it is set; a destination takes no literal (Table B-1,
// B-2 and the text below them).
void test_fpu() {

    TEST("ADDR", "FP0, FP1, FP2", 0x78107F80);
    TEST("ADDRL", "G0, G2, FP3", 0x791CA790);
    TEST("MULR", "1.0, G1, G2", 0x78944E16);
    TEST("SUBR", "0.0, FP0, FP1", 0x78083E90);
    TEST("DIVRL", "G0, G2, G4", 0x79A48590);
    TEST("CVTIR", "G0, FP0", 0x67003210);
    TEST("CVTILR", "G0, G2", 0x67901290);
    TEST("CVTRI", "FP0, G1", 0x6C881800);
    TEST("CVTRIL", "FP0, G2", 0x6C901880);
    TEST("MOVR", "FP0, FP1", 0x6C083C80);
    TEST("MOVRL", "G0, FP2", 0x6D103490);
    TEST("MOVRE", "G0, G4", 0x6EA01090);
    TEST("SCALER", "G0, FP1, FP2", 0x67107390);
    TEST("LOGBNR", "FP0, FP1", 0x68083D00);
    TEST("CLASSR", "FP0", 0x68003F80);
    TEST("CMPR", "FP0, FP1", 0x68007A80);
    TEST("CMPORL", "G0, G2", 0x6904A210);
    TEST("ATANR", "FP0, FP1, FP2", 0x68107800);
    TEST("DADDC", "G0, G1, G2", 0x64944110);
    TEST("DMOVT", "G0, G1", 0x64881210);
}

// Turning the option off leaves the base instruction set alone.
void test_no_fpu() {
    disassembler.setOption("fpu", "off");
    UNKN(0x78107F80);
    UNKN(0x791CA790);
    UNKN(0x64944090);
}

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_arithmetic);
    RUN_TEST(test_logical);
    RUN_TEST(test_bit_field);
    RUN_TEST(test_compare);
    RUN_TEST(test_move);
    RUN_TEST(test_processor);
    RUN_TEST(test_synchronous);
    RUN_TEST(test_branch);
    RUN_TEST(test_compare_branch);
    RUN_TEST(test_test);
    RUN_TEST(test_memory);
    RUN_TEST(test_load_store);
    RUN_TEST(test_effective_address);
    RUN_TEST(test_ip_relative);
    RUN_TEST(test_canonical_mem);
    RUN_TEST(test_ip_operand);
    RUN_TEST(test_mode_bits);
    RUN_TEST(test_fpu);
    RUN_TEST(test_no_fpu);
    RUN_TEST(test_illegal);
    RUN_TEST(test_reserved_field);
    RUN_TEST(test_no_memory);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
