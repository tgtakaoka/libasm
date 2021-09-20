/*
 * Copyright 2021 Tadashi G. Takaoka
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

#include "asm_tms32010.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::tms32010;
using namespace libasm::test;

AsmTms32010 asm32010;
Assembler &assembler(asm32010);

static bool is32010() {
    return strcmp(assembler.getCpu(), "32010") == 0;
}

static void set_up() {
    assembler.reset();
}

static void tear_down() {
    symtab.reset();
}

// clang-format off
static void test_cpu() {
    EQUALS("cpu 32010", true,   assembler.setCpu("32010"));
    EQUALS("cpu 32010", "32010", assembler.getCpu());

    EQUALS("cpu TMS32010", true,   assembler.setCpu("TMS32010"));
    EQUALS("cpu TMS32010", "32010", assembler.getCpu());
}

static void test_accumrator() {
    TEST("ABS", 0x7F88);

    TEST("ADD 70H",         0x0070);
    TEST("ADD 8FH",         0x000F);
    if (is32010()) {
        ERRT("ADD 090H",    OVERFLOW_RANGE);
    } else {
        TEST("ADD 090H",    0x0010);
    }
    ERRT("ADD 0100H",       OPERAND_NOT_ALLOWED);
    ERUS("ADD UNDEF",       0x0000);
    TEST("ADD 70H, 15",     0x0F70);
    ERRT("ADD 70H, 16",     OPERAND_NOT_ALLOWED);
    ERUS("ADD 70H, UNDEF",  0x0070);
    ERUS("ADD UNDEF, 15",   0x0F00);
    TEST("ADD *, 0, AR0",   0x0080);
    TEST("ADD *, 0, AR1",   0x0081);
    TEST("ADD *, 0, 0",     0x0080);
    ERRT("ADD *, 0, 2",     OPERAND_NOT_ALLOWED);
    TEST("ADD *",           0x0088);
    TEST("ADD *-",          0x0098);
    TEST("ADD *+",          0x00A8);
    TEST("ADD *, 1",        0x0188);
    TEST("ADD *-, 2",       0x0298);
    TEST("ADD *+, 3",       0x03A8);
    TEST("ADD *, 0, AR0",   0x0080);
    ERRT("ADD *, -1, AR0",  OPERAND_NOT_ALLOWED);
    ERUS("ADD *, UNDEF, AR1", 0x0081);
    TEST("ADD *, 4, AR1",   0x0481);
    TEST("ADD *-, 4, AR0",  0x0490);
    TEST("ADD *-, 0, AR1",  0x0091);
    TEST("ADD *+, 15, AR0", 0x0FA0);
    TEST("ADD *+, 15, AR1", 0x0FA1);

    TEST("ADDH 70H",     0x6070);
    TEST("ADDH *",       0x6088);
    TEST("ADDH *-",      0x6098);
    TEST("ADDH *+",      0x60A8);
    TEST("ADDH *, AR0",  0x6080);
    TEST("ADDH *-, AR1", 0x6091);
    TEST("ADDH *+, AR0", 0x60A0);
    TEST("ADDH *-, 1",   0x6091);
    TEST("ADDH *+, 0",   0x60A0);

    TEST("ADDS 70H",     0x6170);
    TEST("ADDS *",       0x6188);
    TEST("ADDS *-",      0x6198);
    TEST("ADDS *+",      0x61A8);
    TEST("ADDS *, AR0",  0x6180);
    TEST("ADDS *-, AR1", 0x6191);
    TEST("ADDS *+, AR0", 0x61A0);

    TEST("AND 70H",     0x7970);
    TEST("AND *",       0x7988);
    TEST("AND *-",      0x7998);
    TEST("AND *+",      0x79A8);
    TEST("AND *, AR0",  0x7980);
    TEST("AND *-, AR1", 0x7991);
    TEST("AND *+, AR0", 0x79A0);

    TEST("LAC 70H",         0x2070);
    TEST("LAC 70H, 15",     0x2F70);
    TEST("LAC *, 0, AR0",   0x2080);
    TEST("LAC *, 0, AR1",   0x2081);
    TEST("LAC *",           0x2088);
    TEST("LAC *-",          0x2098);
    TEST("LAC *+",          0x20A8);
    TEST("LAC *, 1",        0x2188);
    TEST("LAC *-, 2",       0x2298);
    TEST("LAC *+, 3",       0x23A8);
    TEST("LAC *, 0, AR0",   0x2080);
    TEST("LAC *, 4, AR1",   0x2481);
    TEST("LAC *-, 4, AR0",  0x2490);
    TEST("LAC *-, 0, AR1",  0x2091);
    TEST("LAC *+, 15, AR0", 0x2FA0);
    TEST("LAC *+, 15, AR1", 0x2FA1);

    TEST("LACK 0",   0x7E00);
    TEST("LACK 255", 0x7EFF);

    TEST("OR 70H",     0x7A70);
    TEST("OR *",       0x7A88);
    TEST("OR *-",      0x7A98);
    TEST("OR *+",      0x7AA8);
    TEST("OR *, AR0",  0x7A80);
    TEST("OR *-, AR1", 0x7A91);
    TEST("OR *+, AR0", 0x7AA0);

    TEST("SACH 70H",        0x5870);
    TEST("SACH 70H, 0",     0x5870);
    TEST("SACH 70H, 1",     0x5970);
    ERRT("SACH 70H, 2",     OPERAND_NOT_ALLOWED);
    ERRT("SACH 70H, 3",     OPERAND_NOT_ALLOWED);
    TEST("SACH 70H, 4",     0x5C70);
    ERRT("SACH 70H, 5",     OPERAND_NOT_ALLOWED);
    ERRT("SACH 70H, 6",     OPERAND_NOT_ALLOWED);
    ERRT("SACH 70H, 7",     OPERAND_NOT_ALLOWED);
    TEST("SACH *, 0, AR0",  0x5880);
    TEST("SACH *, 0, AR1",  0x5881);
    TEST("SACH *",          0x5888);
    TEST("SACH *-",         0x5898);
    TEST("SACH *+",         0x58A8);
    TEST("SACH *, 1",       0x5988);
    TEST("SACH *-, 4",      0x5C98);
    TEST("SACH *+, 1",      0x59A8);
    TEST("SACH *, 0, AR0",  0x5880);
    TEST("SACH *, 4, AR1",  0x5C81);
    TEST("SACH *-, 4, AR0", 0x5C90);
    TEST("SACH *-, 0, AR1", 0x5891);
    TEST("SACH *+, 0, AR0", 0x58A0);
    TEST("SACH *+, 4, AR1", 0x5CA1);

    TEST("SACL 70H",        0x5070);
    TEST("SACL 70H, 0",     0x5070);
    ERRT("SACL 70H, 1",     OPERAND_NOT_ALLOWED);
    TEST("SACL *",          0x5088);
    TEST("SACL *-",         0x5098);
    TEST("SACL *+",         0x50A8);
    TEST("SACL *, 0, AR0",  0x5080);
    TEST("SACL *-, 0, AR1", 0x5091);
    TEST("SACL *+, 0, AR0", 0x50A0);

    TEST("SUB 70H",         0x1070);
    TEST("SUB 70H, 15",     0x1F70);
    TEST("SUB *, 0, AR0",   0x1080);
    TEST("SUB *, 0, AR1",   0x1081);
    TEST("SUB *",           0x1088);
    TEST("SUB *-",          0x1098);
    TEST("SUB *+",          0x10A8);
    TEST("SUB *, 1",        0x1188);
    TEST("SUB *-, 2",       0x1298);
    TEST("SUB *+, 3",       0x13A8);
    TEST("SUB *, 0, AR0",   0x1080);
    TEST("SUB *, 4, AR1",   0x1481);
    TEST("SUB *-, 4, AR0",  0x1490);
    TEST("SUB *-, 0, AR1",  0x1091);
    TEST("SUB *+, 15, AR0", 0x1FA0);
    TEST("SUB *+, 15, AR1", 0x1FA1);

    TEST("SUBC 70H",     0x6470);
    TEST("SUBC *",       0x6488);
    TEST("SUBC *-",      0x6498);
    TEST("SUBC *+",      0x64A8);
    TEST("SUBC *, AR0",  0x6480);
    TEST("SUBC *-, AR1", 0x6491);
    TEST("SUBC *+, AR0", 0x64A0);

    TEST("SUBH 70H",     0x6270);
    TEST("SUBH *",       0x6288);
    TEST("SUBH *-",      0x6298);
    TEST("SUBH *+",      0x62A8);
    TEST("SUBH *, AR0",  0x6280);
    TEST("SUBH *-, AR1", 0x6291);
    TEST("SUBH *+, AR0", 0x62A0);

    TEST("SUBS 70H",     0x6370);
    TEST("SUBS *",       0x6388);
    TEST("SUBS *-",      0x6398);
    TEST("SUBS *+",      0x63A8);
    TEST("SUBS *, AR0",  0x6380);
    TEST("SUBS *-, AR1", 0x6391);
    TEST("SUBS *+, AR0", 0x63A0);

    TEST("XOR 70H",     0x7870);
    TEST("XOR *",       0x7888);
    TEST("XOR *-",      0x7898);
    TEST("XOR *+",      0x78A8);
    TEST("XOR *, AR0",  0x7880);
    TEST("XOR *-, AR1", 0x7891);
    TEST("XOR *+, AR0", 0x78A0);

    TEST("ZAC", 0x7F89);

    TEST("ZALH 70H",     0x6570);
    TEST("ZALH *",       0x6588);
    TEST("ZALH *-",      0x6598);
    TEST("ZALH *+",      0x65A8);
    TEST("ZALH *, AR0",  0x6580);
    TEST("ZALH *-, AR1", 0x6591);
    TEST("ZALH *+, AR0", 0x65A0);

    TEST("ZALS 70H",     0x6670);
    TEST("ZALS *",       0x6688);
    TEST("ZALS *-",      0x6698);
    TEST("ZALS *+",      0x66A8);
    TEST("ZALS *, AR0",  0x6680);
    TEST("ZALS *-, AR1", 0x6691);
    TEST("ZALS *+, AR0", 0x66A0);
}

static void test_auxiliary() {
    TEST("LAR AR0, 70H",     0x3870);
    TEST("LAR 0,   70H",     0x3870);
    ERRT("LAR 2,   70H",     OPERAND_NOT_ALLOWED);
    TEST("LAR AR0, *",       0x3888);
    TEST("LAR AR0, *-",      0x3898);
    TEST("LAR AR0, *+",      0x38A8);
    TEST("LAR AR0, *, AR0",  0x3880);
    TEST("LAR AR0, *-, AR1", 0x3891);
    TEST("LAR AR0, *+, AR0", 0x38A0);
    TEST("LAR AR1, 70H",     0x3970);
    TEST("LAR AR1, *",       0x3988);
    TEST("LAR AR1, *-",      0x3998);
    TEST("LAR AR1, *+",      0x39A8);
    TEST("LAR AR1, *, AR0",  0x3980);
    TEST("LAR 1,   *, 0",    0x3980);
    TEST("LAR AR1, *-, AR1", 0x3991);
    TEST("LAR AR1, *+, AR0", 0x39A0);

    TEST("LARK AR0, 255", 0x70FF);
    TEST("LARK AR1, 128", 0x7180);
    TEST("LARK 0,   255", 0x70FF);
    ERRT("LARK 1,   256", OPERAND_NOT_ALLOWED);
    ERRT("LARK 2,   128", OPERAND_NOT_ALLOWED);

    TEST("LARP AR0", 0x6880);
    TEST("LARP AR1", 0x6881);
    TEST("LARP 0",   0x6880);
    TEST("LARP 1",   0x6881);
    ERRT("LARP 2",   OPERAND_NOT_ALLOWED);

    TEST("LDP 70H",     0x6F70);
    TEST("LDP *",       0x6F88);
    TEST("LDP *-",      0x6F98);
    TEST("LDP *+",      0x6FA8);
    TEST("LDP *, AR0",  0x6F80);
    TEST("LDP *-, AR1", 0x6F91);
    TEST("LDP *+, AR0", 0x6FA0);

    TEST("LDPK 0", 0x6E00);
    TEST("LDPK 1", 0x6E01);
    ERRT("LDPK 2", OPERAND_NOT_ALLOWED);

    TEST("MAR  70H",     0x6870);
    TEST("MAR  *",       0x6888);
    TEST("MAR  *-",      0x6898);
    TEST("MAR  *+",      0x68A8);
    TEST("LARP AR0",     0x6880);
    TEST("MAR  *-, AR1", 0x6891);
    TEST("MAR  *+, AR0", 0x68A0);

    TEST("SAR AR0, 70H",     0x3070);
    TEST("SAR AR0, *",       0x3088);
    TEST("SAR AR0, *-",      0x3098);
    TEST("SAR AR0, *+",      0x30A8);
    TEST("SAR AR0, *, AR0",  0x3080);
    TEST("SAR AR0, *-, AR1", 0x3091);
    TEST("SAR AR0, *+, AR0", 0x30A0);
    TEST("SAR AR1, 70H",     0x3170);
    TEST("SAR AR1, *",       0x3188);
    TEST("SAR AR1, *-",      0x3198);
    TEST("SAR AR1, *+",      0x31A8);
    TEST("SAR AR1, *, AR0",  0x3180);
    TEST("SAR AR1, *-, AR1", 0x3191);
    TEST("SAR AR1, *+, AR0", 0x31A0);
}

static void test_multiply() {
    TEST("APAC", 0x7F8F);

    TEST("LT 70H",     0x6A70);
    TEST("LT *",       0x6A88);
    TEST("LT *-",      0x6A98);
    TEST("LT *+",      0x6AA8);
    TEST("LT *, AR0",  0x6A80);
    TEST("LT *-, AR1", 0x6A91);
    TEST("LT *+, AR0", 0x6AA0);

    TEST("LTA 70H",     0x6C70);
    TEST("LTA *",       0x6C88);
    TEST("LTA *-",      0x6C98);
    TEST("LTA *+",      0x6CA8);
    TEST("LTA *, AR0",  0x6C80);
    TEST("LTA *-, AR1", 0x6C91);
    TEST("LTA *+, AR0", 0x6CA0);

    TEST("LTD 70H",     0x6B70);
    TEST("LTD *",       0x6B88);
    TEST("LTD *-",      0x6B98);
    TEST("LTD *+",      0x6BA8);
    TEST("LTD *, AR0",  0x6B80);
    TEST("LTD *-, AR1", 0x6B91);
    TEST("LTD *+, AR0", 0x6BA0);

    TEST("MPY 70H",     0x6D70);
    TEST("MPY *",       0x6D88);
    TEST("MPY *-",      0x6D98);
    TEST("MPY *+",      0x6DA8);
    TEST("MPY *, AR0",  0x6D80);
    TEST("MPY *-, AR1", 0x6D91);
    TEST("MPY *+, AR0", 0x6DA0);

    TEST("MPYK 0",     0x8000);
    TEST("MPYK 2",     0x8002);
    TEST("MPYK 4095",  0x8FFF);
    ERRT("MPYK 4096",  OVERFLOW_RANGE);
    TEST("MPYK -1",    0x9FFF);
    TEST("MPYK -2",    0x9FFE);
    TEST("MPYK -4096", 0x9000);
    ERRT("MPYK -4097", OVERFLOW_RANGE);

    TEST("PAC",  0x7F8E);
    TEST("SPAC", 0x7F90);
}

static void test_branch() {
    TEST("B 000H",  0xF900, 0x0000);
    TEST("B 001H",  0xF900, 0x0001);
    TEST("B 002H",  0xF900, 0x0002);
    TEST("B 004H",  0xF900, 0x0004);
    TEST("B 008H",  0xF900, 0x0008);
    TEST("B 010H",  0xF900, 0x0010);
    TEST("B 020H",  0xF900, 0x0020);
    TEST("B 040H",  0xF900, 0x0040);
    TEST("B 080H",  0xF900, 0x0080);
    TEST("B 100H",  0xF900, 0x0100);
    TEST("B 200H",  0xF900, 0x0200);
    TEST("B 400H",  0xF900, 0x0400);
    TEST("B 800H",  0xF900, 0x0800);
    TEST("B 0FFFH", 0xF900, 0x0FFF);
    ERUS("B UNDEF", 0xF900, 0x0000);
    ERRT("B 1000H", OVERFLOW_RANGE);
    ERRT("B 2000H", OVERFLOW_RANGE);
    ERRT("B 4000H", OVERFLOW_RANGE);
    ERRT("B 8000H", OVERFLOW_RANGE);

    TEST("BANZ 900H",  0xF400, 0x0900);
    TEST("BGEZ 900H",  0xFD00, 0x0900);
    TEST("BGZ  900H",  0xFC00, 0x0900);
    TEST("BIOZ 900H",  0xF600, 0x0900);
    TEST("BLEZ 900H",  0xFB00, 0x0900);
    TEST("BLZ  900H",  0xFA00, 0x0900);
    TEST("BNZ  900H",  0xFE00, 0x0900);
    TEST("BV   900H",  0xF500, 0x0900);
    TEST("BZ   900H",  0xFF00, 0x0900);
    TEST("CALL 900H",  0xF800, 0x0900);
    ERUS("CALL UNDEF", 0xF800, 0x0000);

    TEST("CALA", 0x7F8C);
    TEST("RET",  0x7F8D);
}

static void test_control() {
    TEST("DINT", 0x7F81);
    TEST("EINT", 0x7F82);
    TEST("NOP",  0x7F80);
    TEST("POP",  0x7F9D);
    TEST("PUSH", 0x7F9C);
    TEST("ROVM", 0x7F8A);
    TEST("SOVM", 0x7F8B);

    TEST("LST 00H",     0x7B00);
    TEST("LST 70H",     0x7B70);
    TEST("LST *",       0x7B88);
    TEST("LST *-",      0x7B98);
    TEST("LST *+",      0x7BA8);
    TEST("LST *, AR0",  0x7B80);
    TEST("LST *-, AR1", 0x7B91);
    TEST("LST *+, AR0", 0x7BA0);

    ERRT("SST 00H",     OVERFLOW_RANGE);
    ERRT("SST 7FH",     OVERFLOW_RANGE);
    TEST("SST 80H",     0x7C00);
    TEST("SST 8FH",     0x7C0F);
    if (is32010()) {
        ERRT("SST 090H", OVERFLOW_RANGE);
    } else {
        TEST("SST 090H", 0x7C10);
    }
    TEST("SST *",       0x7C88);
    TEST("SST *-",      0x7C98);
    TEST("SST *+",      0x7CA8);
    TEST("SST *, AR0",  0x7C80);
    TEST("SST *-, AR1", 0x7C91);
    TEST("SST *+, AR0", 0x7CA0);
}

static void test_dataio() {
    TEST("DMOV 00H",     0x6900);
    TEST("DMOV 70H",     0x6970);
    TEST("DMOV *",       0x6988);
    TEST("DMOV *-",      0x6998);
    TEST("DMOV *+",      0x69A8);
    TEST("DMOV *, AR0",  0x6980);
    TEST("DMOV *-, AR1", 0x6991);
    TEST("DMOV *+, AR0", 0x69A0);

    TEST("TBLR 00H",     0x6700);
    TEST("TBLR 70H",     0x6770);
    TEST("TBLR *",       0x6788);
    TEST("TBLR *-",      0x6798);
    TEST("TBLR *+",      0x67A8);
    TEST("TBLR *, AR0",  0x6780);
    TEST("TBLR *-, AR1", 0x6791);
    TEST("TBLR *+, AR0", 0x67A0);

    TEST("TBLW 00H",     0x7D00);
    TEST("TBLW 70H",     0x7D70);
    TEST("TBLW *",       0x7D88);
    TEST("TBLW *-",      0x7D98);
    TEST("TBLW *+",      0x7DA8);
    TEST("TBLW *, AR0",  0x7D80);
    TEST("TBLW *-, AR1", 0x7D91);
    TEST("TBLW *+, AR0", 0x7DA0);

    TEST("IN 70H, PA0",     0x4070);
    TEST("IN 70H, PA7",     0x4770);
    TEST("IN 70H, 0",       0x4070);
    TEST("IN 70H, 7",       0x4770);
    ERRT("IN 70H, 8",       OPERAND_NOT_ALLOWED);
    TEST("IN *, PA0, AR0",  0x4080);
    TEST("IN *, PA7, AR1",  0x4781);
    TEST("IN *, 0,   0",    0x4080);
    TEST("IN *, 7,   1",    0x4781);
    TEST("IN *, PA0",       0x4088);
    TEST("IN *-, PA1",      0x4198);
    TEST("IN *+, PA2",      0x42A8);
    TEST("IN *, PA1",       0x4188);
    TEST("IN *-, PA2",      0x4298);
    TEST("IN *+, PA3",      0x43A8);
    TEST("IN *, PA0, AR0",  0x4080);
    TEST("IN *, PA4, AR1",  0x4481);
    TEST("IN *-, PA4, AR0", 0x4490);
    TEST("IN *-, PA0, AR1", 0x4091);
    TEST("IN *+, PA7, AR0", 0x47A0);
    TEST("IN *+, PA7, AR1", 0x47A1);

    TEST("OUT 70H, PA0",     0x4870);
    TEST("OUT 70H, PA7",     0x4F70);
    TEST("OUT *, PA0, AR0",  0x4880);
    TEST("OUT *, PA7, AR1",  0x4F81);
    TEST("OUT *, PA0",       0x4888);
    TEST("OUT *-, PA1",      0x4998);
    TEST("OUT *+, PA2",      0x4AA8);
    TEST("OUT *, PA1",       0x4988);
    TEST("OUT *-, PA2",      0x4A98);
    TEST("OUT *+, PA3",      0x4BA8);
    TEST("OUT *, PA0, AR0",  0x4880);
    TEST("OUT *, PA4, AR1",  0x4C81);
    TEST("OUT *-, PA4, AR0", 0x4C90);
    TEST("OUT *-, PA0, AR1", 0x4891);
    TEST("OUT *+, PA7, AR0", 0x4FA0);
    TEST("OUT *+, PA7, AR1", 0x4FA1);

    ERUS("OUT *+, UNDEF, AR0", 0x48A0);
    ERUS("OUT *+, PA7, UNDEF", 0x4FA0);
    ERUS("OUT *+, UNDEF, AR1", 0x48A1);
    ERUS("OUT *+, PA7, UNDEF", 0x4FA0);
}
// clang-format on

const char *run_cpu_test() {
    RUN_TEST(test_cpu);
    return assembler.listCpu();
}

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    RUN_TEST(test_accumrator);
    RUN_TEST(test_auxiliary);
    RUN_TEST(test_multiply);
    RUN_TEST(test_branch);
    RUN_TEST(test_control);
    RUN_TEST(test_dataio);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
