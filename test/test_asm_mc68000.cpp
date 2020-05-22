/*
 * Copyright 2020 Tadashi G. Takaoka
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

#include "asm_mc68000.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::mc68000;
using namespace libasm::test;

AsmMc68000 as68000;
Assembler &assembler(as68000);

static void set_up() {
}

static void tear_down() {
    symtab.reset();
}

static void test_cpu() {
    asserter.equals(
        "cpu 68000", true, assembler.setCpu("68000"));
    asserter.equals(
        "get cpu", "68000", assembler.getCpu());

    asserter.equals(
        "cpu 68k", true, assembler.setCpu("68k"));
    asserter.equals(
        "get cpu", "68000", assembler.getCpu());

    asserter.equals(
        "cpu mc68000", true, assembler.setCpu("mc68000"));
    asserter.equals(
        "get cpu", "68000", assembler.getCpu());
}

static void test_inherent() {
    TEST("RESET", 047160);
    TEST("NOP",   047161);
    TEST("STOP #$1234", 047162, 0x1234);
    TEST("RTE",   047163);
    TEST("RTS",   047165);
    TEST("TRAPV", 047166);
    TEST("RTR",   047167);
}

static void test_move_mlt() {
    TEST("MOVEM.W D0-D7/A1-A7,-(A0)", 044240, 0xFF7F);
    TEST("MOVEM.W D0-D7/A1-A7,(A0)",  044220, 0xFEFF);
    TEST("MOVEM   D0,-(A0)",          044240, 0x8000);
    TEST("MOVEM.L (A1)+,D7-A0/A6-A7", 046331, 0xC180);
    TEST("MOVEM.L (A1)+,D0",          046331, 0x0001);
    TEST("MOVEM.L (A1)+,D4-A3/D0",    046331, 0x0FF1);
    ETEST(DUPLICATE_REGISTER, "MOVEM.L (A1)+,D4-A3/D7");
}

static void test_comment() {
    TEST("NOP             ; comment", 047161);
    TEST("ORI  # 0 , CCR  ; comment", 000074, 0x0000);
    TEST("ANDI # 0 , SR   ; comment", 001174, 0x0000);
    TEST("MOVE ( 0 ) , D1 ; comment", 031070, 0x0000);
    TEST("MOVE # 0 , D1   ; comment", 031074, 0x0000);
    TEST("MOVE D1 , ( 0 ) ; comment", 030701, 0x0000);
    TEST("MOVE D2 , ( 0 , A3 )      ; comment", 033502, 0x0000);
    TEST("MOVE ( 0 , A1 , D2 ) , D3 ; comment", 033061, 0x2000);
    TEST("MOVEP D1 , ( 0 , A1 ); comment", 001611, 0x0000);
    TEST("MOVEP ( 0 , A1 ) , D1; comment", 001411, 0x0000);
    TEST("MOVEQ # 0 , D1       ; comment", 071000);
    TEST("ADDQ  # 8 , D1       ; comment", 050101);
    TEST("ADDQ  # 1 , ( 0 , A1 )   ; comment", 051151, 0x0000);
    TEST("LSR   # 8 , D1       ; comment", 0160111);
    TEST("ROR   ( 0 , A1 )     ; comment", 0163351, 0x0000);

    ATEST(0x1000, "BRA   *      ; comment", 060000|0xFE);
    ATEST(0x1000, "DBRA  D0 , * ; comment", 050710, 0xFFFE);
    ATEST(0x1000, "MOVEA ( * , PC ) , A1        ; comment", 021172, 0xFFFE);
    ATEST(0x1000, "MOVEA ( * , PC , D1.L ) , A1 ; comment", 021173, 0x18FE);
}

static void test_undefined_symbol() {
    ETEST(UNDEFINED_SYMBOL, "ORI  #UNDEF,CCR", 000074, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "ANDI #UNDEF,SR",  001174, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE (UNDEF),D1",       031070, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE #UNDEF,D1",        031074, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE D1,(UNDEF)",       030701, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE D2,(UNDEF,A3)",    033502, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVE (UNDEF,A1,D2),D3", 033061, 0x2000);
    ETEST(UNDEFINED_SYMBOL, "MOVEP D1,(UNDEF,A1)",   001611, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVEP (UNDEF,A1),D1",   001411, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "MOVEQ #UNDEF,D1",       071000);
    ETEST(UNDEFINED_SYMBOL, "ADDQ  #UNDEF,D1",       050101);
    ETEST(UNDEFINED_SYMBOL, "ADDQ  #1,(UNDEF,A1)",   051151, 0x0000);
    ETEST(UNDEFINED_SYMBOL, "LSR   #UNDEF,D1",       0160111);
    ETEST(UNDEFINED_SYMBOL, "ROR   (UNDEF,A1)",      0163351, 0x0000);

    EATEST(UNDEFINED_SYMBOL, 0x1000, "BRA UNDEF",      060000|0xFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "DBRA  D0,UNDEF", 050710, 0xFFFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "MOVEA (UNDEF,PC),A1",      021172, 0xFFFE);
    EATEST(UNDEFINED_SYMBOL, 0x1000, "MOVEA (UNDEF,PC,D1.L),A1", 021173, 0x18FE);
}

static void run_test(void (*test)(), const char *test_name) {
    asserter.clear(test_name);
    set_up();
    test();
    tear_down();
    asserter.check();
}

int main(int argc, char **argv) {
    RUN_TEST(test_cpu);
    RUN_TEST(test_inherent);
    RUN_TEST(test_move_mlt);
    RUN_TEST(test_comment);
    RUN_TEST(test_undefined_symbol);
    return 0;
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
