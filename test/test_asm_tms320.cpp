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

#include "asm_tms320.h"
#include "test_asm_helper.h"

using namespace libasm;
using namespace libasm::tms320;
using namespace libasm::test;

AsmTms320 asm320;
Assembler &assembler(asm320);

bool is32010() {
    return strcasecmp_P("32010", assembler.config().cpu_P()) == 0;
}

bool is32015() {
    return strcasecmp_P("32015", assembler.config().cpu_P()) == 0;
}

bool is3201x() {
    return is32010() || is32015();
}

bool is32020() {
    return strcasecmp_P("32020", assembler.config().cpu_P()) == 0;
}

bool is320C25() {
    return strcasecmp_P("320C25", assembler.config().cpu_P()) == 0;
}

bool is320C26() {
    return strcasecmp_P("320C26", assembler.config().cpu_P()) == 0;
}

bool is3202x() {
    return is32020() || is320C25() || is320C26();
}

bool is320C2x() {
    return is320C25() || is320C26();
}

bool is320C20x() {
    return strcasecmp_P("320C20x", assembler.config().cpu_P()) == 0;
}

bool is320C5x() {
    return strcasecmp_P("320C5x", assembler.config().cpu_P()) == 0;
}

void set_up() {
    assembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 32010", true,   assembler.setCpu("32010"));
    EQUALS_P("cpu 32010", "32010", assembler.config().cpu_P());

    EQUALS("cpu 32015", true,   assembler.setCpu("32015"));
    EQUALS_P("cpu 32015", "32015", assembler.config().cpu_P());

    EQUALS("cpu 32020", true,   assembler.setCpu("32020"));
    EQUALS_P("cpu 32020", "32020", assembler.config().cpu_P());

    EQUALS("cpu 320C25", true,   assembler.setCpu("320C25"));
    EQUALS_P("cpu 320C25", "320C25", assembler.config().cpu_P());

    EQUALS("cpu 320C26", true,   assembler.setCpu("320C26"));
    EQUALS_P("cpu 320C26", "320C26", assembler.config().cpu_P());

    EQUALS("cpu 320C20X", true,   assembler.setCpu("320C20X"));
    EQUALS_P("cpu 320C20X", "320C20X", assembler.config().cpu_P());

    EQUALS("cpu 320C5X", true,   assembler.setCpu("320C5X"));
    EQUALS_P("cpu 320C5X", "320C5X", assembler.config().cpu_P());

    EQUALS("cpu TMS32010", true,   assembler.setCpu("TMS32010"));
    EQUALS_P("cpu TMS32010", "32010", assembler.config().cpu_P());

    EQUALS("cpu TMS32015", true,   assembler.setCpu("TMS32015"));
    EQUALS_P("cpu TMS32015", "32015", assembler.config().cpu_P());

    EQUALS("cpu TMS32020", true,   assembler.setCpu("TMS32020"));
    EQUALS_P("cpu TMS32020", "32020", assembler.config().cpu_P());

    EQUALS("cpu TMS320C25", true,   assembler.setCpu("TMS320C25"));
    EQUALS_P("cpu TMS320C25", "320C25", assembler.config().cpu_P());

    EQUALS("cpu TMS320C26", true,   assembler.setCpu("TMS320C26"));
    EQUALS_P("cpu TMS320C26", "320C26", assembler.config().cpu_P());

    EQUALS("cpu TMS320C20X", true,   assembler.setCpu("TMS320C20X"));
    EQUALS_P("cpu TMS320C20X", "320C20X", assembler.config().cpu_P());

    EQUALS("cpu TMS320C5X", true,   assembler.setCpu("TMS320C5X"));
    EQUALS_P("cpu TMS320C5X", "320C5X", assembler.config().cpu_P());
}

void test_accumrator_1x() {
    TEST("ABS", 0x7F88);

    TEST("ADD 70H",         0x0070);
    TEST("ADD 8FH",         0x000F);
    if (is32010()) {
        ERRT("ADD 090H", OVERFLOW_RANGE, "090H", 0x0010);
    }
    if (is32015()) {
        TEST("ADD 090H",                         0x0010);
    }
    ERRT("ADD 0100H",       OVERFLOW_RANGE, "0100H", 0x0000);
    ERUS("ADD UNDEF",       "UNDEF",                 0x0000);
    TEST("ADD 70H, 15",                           0x0F70);
    ERRT("ADD 70H, 16",     OVERFLOW_RANGE, "16", 0x0070);
    ERUS("ADD 70H, UNDEF",  "UNDEF",              0x0070);
    ERUS("ADD UNDEF, 15",   "UNDEF, 15", 0x0F00);
    TEST("add *, 0, ar0",   0x0080);
    TEST("ADD *, 0, AR1",   0x0081);
    TEST("ADD *, 0, 0",     0x0080);
    ERRT("ADD *, 0, 2",     UNKNOWN_REGISTER, "2", 0x0080);
    TEST("ADD *",           0x0088);
    TEST("ADD *-",          0x0098);
    TEST("ADD *+",          0x00A8);
    TEST("ADD *, 1",        0x0188);
    TEST("ADD *-, 2",       0x0298);
    TEST("ADD *+, 3",       0x03A8);
    TEST("ADD *, 0, AR0",   0x0080);
    ERRT("ADD *, -1, AR0",  OVERFLOW_RANGE, "-1, AR0", 0x0F80);
    ERUS("ADD *, UNDEF, AR1", "UNDEF, AR1", 0x0081);
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

    symtab.intern(0x00, "$zero_data");
    symtab.intern(0xFF, "_max$data");

    TEST("LACK $zero_data", 0x7E00);
    TEST("LACK _max$data",  0x7EFF);

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
    ERRT("SACH 70H, 2",     ILLEGAL_CONSTANT, "2", 0x5A70);
    ERRT("SACH 70H, 3",     ILLEGAL_CONSTANT, "3", 0x5B70);
    TEST("SACH 70H, 4",     0x5C70);
    ERRT("SACH 70H, 5",     ILLEGAL_CONSTANT, "5", 0x5D70);
    ERRT("SACH 70H, 6",     ILLEGAL_CONSTANT, "6", 0x5E70);
    ERRT("SACH 70H, 7",     ILLEGAL_CONSTANT, "7", 0x5F70);
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
    ERRT("SACL 70H, 1",     ILLEGAL_CONSTANT, "1", 0x5070);
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

void test_accumrator_2x() {
    TEST("ABS", 0xCE1B);

    TEST("ADD 70H",         0x0070);
    TEST("ADD 0FFFFH",      0x007F);
    TEST("ADD 70H, 15",     0x0F70);
    if (is32020()) {
        ERRT("ADD 70H, 16", OVERFLOW_RANGE, "16", 0x0070);
    } else {
        TEST("ADD 70H, 16", 0x4870); // ADDH
        ERRT("ADD 70H, 17", OVERFLOW_RANGE, "17", 0x0170);
    }
    TEST("ADD *",           0x0080);
    TEST("ADD *, 0, AR0",   0x0088);
    TEST("ADD *, 0, AR7",   0x008F);
    TEST("ADD *-",          0x0090);
    TEST("ADD *+",          0x00A0);
    TEST("ADD *BR0-",       0x00C0);
    TEST("ADD *0-",         0x00D0);
    TEST("ADD *0+",         0x00E0);
    TEST("ADD *BR0+",       0x00F0);
    TEST("ADD *, 1",        0x0180);
    TEST("ADD *-, 2",       0x0290);
    TEST("ADD *+, 3",       0x03A0);
    TEST("ADD *, 0, AR0",   0x0088);
    TEST("ADD *, 4, AR1",   0x0489);
    TEST("ADD *-, 4, AR0",  0x0498);
    TEST("ADD *-, 0, AR1",  0x0099);
    TEST("ADD *+, 15, AR0", 0x0FA8);
    TEST("ADD *BR0+, 15, AR7", 0x0FFF);

    TEST("ADDH 70H",     0x4870);
    TEST("ADDH *",       0x4880);
    TEST("ADDH *-",      0x4890);
    TEST("ADDH *+",      0x48A0);
    TEST("ADDH *, AR0",  0x4888);
    TEST("ADDH *-, AR1", 0x4899);
    TEST("ADDH *+, AR0", 0x48A8);
    if (is320C2x()) {
        TEST("ADD 70H,16",      0x4870);
        TEST("ADD *,  16",      0x4880);
        TEST("ADD *-, 16",      0x4890);
        TEST("ADD *+, 16",      0x48A0);
        TEST("ADD *,  16, AR0", 0x4888);
        TEST("ADD *-, 16, AR1", 0x4899);
        TEST("ADD *+, 16, AR0", 0x48A8);
    }

    TEST("ADDS 70H",     0x4970);
    TEST("ADDS *",       0x4980);
    TEST("ADDS *-",      0x4990);
    TEST("ADDS *+",      0x49A0);
    TEST("ADDS *, AR0",  0x4988);
    TEST("ADDS *-, AR1", 0x4999);
    TEST("ADDS *+, AR0", 0x49A8);

    TEST("ADDT 70H",     0x4A70);
    TEST("ADDT *",       0x4A80);
    TEST("ADDT *-",      0x4A90);
    TEST("ADDT *+",      0x4AA0);
    TEST("ADDT *, AR0",  0x4A88);
    TEST("ADDT *-, AR1", 0x4A99);
    TEST("ADDT *+, AR0", 0x4AA8);

    TEST("ADLK 0",          0xD002, 0x0000);
    TEST("ADLK 0FFFFH",     0xD002, 0xFFFF);
    TEST("ADLK -1",         0xD002, 0xFFFF);
    TEST("ADLK 18, 4",      0xD402, 0x0012);
    TEST("ADLK 00FFH, 10",  0xDA02, 0x00FF);
    TEST("ADLK 0FFFFH, 15", 0xDF02, 0xFFFF);
    if (is320C2x()) {
        TEST("ADD #0",          0xCC00);
        TEST("ADD #0FFFFH",     0xD002, 0xFFFF);
        TEST("ADD #18, 4",      0xD402, 0x0012);
        TEST("ADD #00FFH, 10",  0xDA02, 0x00FF);
        TEST("ADD #0FFFFH, 15", 0xDF02, 0xFFFF);
    }

    if (is320C2x()) {
        TEST("ADDC 70H",     0x4370);
        TEST("ADDC *",       0x4380);
        TEST("ADDC *-",      0x4390);
        TEST("ADDC *+",      0x43A0);
        TEST("ADDC *, AR0",  0x4388);
        TEST("ADDC *-, AR1", 0x4399);
        TEST("ADDC *+, AR0", 0x43A8);

        TEST("ADDK 0",   0xCC00);
        TEST("ADDK 255", 0xCCFF);
        TEST("ADD #0",   0xCC00);
        TEST("ADD #255", 0xCCFF);
    }

    TEST("AND 70H",     0x4E70);
    TEST("AND *",       0x4E80);
    TEST("AND *-",      0x4E90);
    TEST("AND *+",      0x4EA0);
    TEST("AND *, AR0",  0x4E88);
    TEST("AND *-, AR1", 0x4E99);
    TEST("AND *+, AR0", 0x4EA8);

    TEST("ANDK 0",          0xD004, 0x0000);
    TEST("ANDK 0FFFFH",     0xD004, 0xFFFF);
    TEST("ANDK 18, 4",      0xD404, 0x0012);
    TEST("ANDK 00FFH, 10",  0xDA04, 0x00FF);
    TEST("ANDK 0FFFFH, 15", 0xDF04, 0xFFFF);
    if (is320C2x()) {
        TEST("AND #0",          0xD004, 0x0000);
        TEST("AND #0FFFFH",     0xD004, 0xFFFF);
        TEST("AND #18, 4",      0xD404, 0x0012);
        TEST("AND #00FFH, 10",  0xDA04, 0x00FF);
        TEST("AND #0FFFFH, 15", 0xDF04, 0xFFFF);
    }

    TEST("CMPL", 0xCE27);

    TEST("LAC 70H",         0x2070);
    TEST("LAC 70H, 15",     0x2F70);
    TEST("LAC *",           0x2080);
    TEST("LAC *, 0, AR0",   0x2088);
    TEST("LAC *, 0, AR7",   0x208F);
    TEST("LAC *-",          0x2090);
    TEST("LAC *+",          0x20A0);
    TEST("LAC *BR0-",       0x20C0);
    TEST("LAC *0-",         0x20D0);
    TEST("LAC *0+",         0x20E0);
    TEST("LAC *BR0+",       0x20F0);
    TEST("LAC *, 1",        0x2180);
    TEST("LAC *-, 2",       0x2290);
    TEST("LAC *+, 3",       0x23A0);
    TEST("LAC *, 0, AR0",   0x2088);
    TEST("LAC *, 4, AR1",   0x2489);
    TEST("LAC *-, 4, AR0",  0x2498);
    TEST("LAC *-, 0, AR1",  0x2099);
    TEST("LAC *+, 15, AR0", 0x2FA8);
    TEST("LAC *BR0+, 15, AR7", 0x2FFF);
    if (is320C2x()) {
        TEST("LACC 70H",         0x2070);
        TEST("LACC 70H, 15",     0x2F70);
        TEST("LACC *",           0x2080);
        TEST("LACC *, 0, AR0",   0x2088);
        TEST("LACC *, 0, AR7",   0x208F);
        TEST("LACC *-",          0x2090);
        TEST("LACC *+",          0x20A0);
        TEST("LACC *BR0-",       0x20C0);
        TEST("LACC *0-",         0x20D0);
        TEST("LACC *0+",         0x20E0);
        TEST("LACC *BR0+",       0x20F0);
        TEST("LACC *, 1",        0x2180);
        TEST("LACC *-, 2",       0x2290);
        TEST("LACC *+, 3",       0x23A0);
        TEST("LACC *, 0, AR0",   0x2088);
        TEST("LACC *, 4, AR1",   0x2489);
        TEST("LACC *-, 4, AR0",  0x2498);
        TEST("LACC *-, 0, AR1",  0x2099);
        TEST("LACC *+, 15, AR0", 0x2FA8);
        TEST("LACC *BR0+, 15, AR7", 0x2FFF);
    }

    TEST("ZAC",      0xCA00);
    TEST("LACK 1",   0xCA01);
    TEST("LACK 255", 0xCAFF);
    if (is320C2x()) {
        TEST("LACL #0",   0xCA00);
        TEST("LACL #1",   0xCA01);
        TEST("LACL #255", 0xCAFF);
    }

    TEST("LACT 70H",     0x4270);
    TEST("LACT *",       0x4280);
    TEST("LACT *-",      0x4290);
    TEST("LACT *+",      0x42A0);
    TEST("LACT *, AR0",  0x4288);
    TEST("LACT *-, AR1", 0x4299);
    TEST("LACT *+, AR0", 0x42A8);

    TEST("LALK 0",          0xD001, 0x0000);
    TEST("LALK 0FFFFH",     0xD001, 0xFFFF);
    TEST("LALK 18, 4",      0xD401, 0x0012);
    TEST("LALK 00FFH, 10",  0xDA01, 0x00FF);
    TEST("LALK 0FFFFH, 15", 0xDF01, 0xFFFF);
    if (is320C2x()) {
        TEST("LACC #0",          0xD001, 0x0000);
        TEST("LACC #0FFFFH",     0xD001, 0xFFFF);
        TEST("LACC #18, 4",      0xD401, 0x0012);
        TEST("LACC #00FFH, 10",  0xDA01, 0x00FF);
        TEST("LACC #0FFFFH, 15", 0xDF01, 0xFFFF);
    }

    TEST("NEG", 0xCE23);

    if (is32020()) {
        TEST("NORM", 0xCE82);
    }
    if (is320C2x()) {
        TEST("NORM *",     0xCE82);
        TEST("NORM *-",    0xCE92);
        TEST("NORM *+",    0xCEA2);
        TEST("NORM *BR0-", 0xCEC2);
        TEST("NORM *0-",   0xCED2);
        TEST("NORM *0+",   0xCEE2);
        TEST("NORM *BR0+", 0xCEF2);
    }

    TEST("OR 70H",     0x4D70);
    TEST("OR *",       0x4D80);
    TEST("OR *-",      0x4D90);
    TEST("OR *+",      0x4DA0);
    TEST("OR *, AR0",  0x4D88);
    TEST("OR *-, AR1", 0x4D99);
    TEST("OR *+, AR0", 0x4DA8);

    TEST("ORK 0",          0xD005, 0x0000);
    TEST("ORK 0FFFFH",     0xD005, 0xFFFF);
    TEST("ORK 18, 4",      0xD405, 0x0012);
    TEST("ORK 00FFH, 10",  0xDA05, 0x00FF);
    TEST("ORK 0FFFFH, 15", 0xDF05, 0xFFFF);
    if (is320C2x()) {
        TEST("OR #0",          0xD005, 0x0000);
        TEST("OR #0FFFFH",     0xD005, 0xFFFF);
        TEST("OR #18, 4",      0xD405, 0x0012);
        TEST("OR #00FFH, 10",  0xDA05, 0x00FF);
        TEST("OR #0FFFFH, 15", 0xDF05, 0xFFFF);
    }

    TEST("SACH 70H",         0x6870);
    if (is32020()) {
        TEST("SACH 70H, 1",  0x6970);
        TEST("SACH 70H, 4",  0x6C70);
    } else {
        TEST("SACH 70H, 1",  0x6970);
        TEST("SACH 70H, 2",  0x6A70);
        TEST("SACH 70H, 3",  0x6B70);
        TEST("SACH 70H, 4",  0x6C70);
        TEST("SACH 70H, 5",  0x6D70);
        TEST("SACH 70H, 6",  0x6E70);
        TEST("SACH 70H, 7",  0x6F70);
    }
    TEST("SACH *",           0x6880);

    TEST("SACH *, 0, AR0",   0x6888);
    TEST("SACH *, 0, AR7",   0x688F);
    TEST("SACH *-",          0x6890);
    TEST("SACH *+",          0x68A0);
    TEST("SACH *BR0-",       0x68C0);
    TEST("SACH *0-",         0x68D0);
    TEST("SACH *0+",         0x68E0);
    TEST("SACH *BR0+",       0x68F0);
    TEST("SACH *, 1",        0x6980);
    TEST("SACH *-, 1",       0x6990);
    TEST("SACH *+, 4",       0x6CA0);
    TEST("SACH *, 0, AR0",   0x6888);
    TEST("SACH *, 4, AR1",   0x6C89);
    TEST("SACH *-, 4, AR0",  0x6C98);
    TEST("SACH *-, 0, AR1",  0x6899);
    TEST("SACH *+, 4, AR0",  0x6CA8);
    if (is32020()) {
        TEST("SACH *BR0+, 4, AR7", 0x6CFF);
    } else {
        TEST("SACH *BR0+, 7, AR7", 0x6FFF);
    }

    TEST("SACL 70H",         0x6070);
    if (is32020()) {
        TEST("SACL 70H, 1",  0x6170);
        TEST("SACL 70H, 4",  0x6470);
    } else {
        TEST("SACL 70H, 1",  0x6170);
        TEST("SACL 70H, 2",  0x6270);
        TEST("SACL 70H, 3",  0x6370);
        TEST("SACL 70H, 4",  0x6470);
        TEST("SACL 70H, 5",  0x6570);
        TEST("SACL 70H, 6",  0x6670);
        TEST("SACL 70H, 7",  0x6770);
    }
    TEST("SACL *",           0x6080);
    TEST("SACL *, 0, AR0",   0x6088);
    TEST("SACL *, 0, AR7",   0x608F);
    TEST("SACL *-",          0x6090);
    TEST("SACL *+",          0x60A0);
    TEST("SACL *BR0-",       0x60C0);
    TEST("SACL *0-",         0x60D0);
    TEST("SACL *0+",         0x60E0);
    TEST("SACL *BR0+",       0x60F0);
    TEST("SACL *, 1",        0x6180);
    TEST("SACL *-, 1",       0x6190);
    TEST("SACL *+, 4",       0x64A0);
    TEST("SACL *, 0, AR0",   0x6088);
    TEST("SACL *, 4, AR1",   0x6489);
    TEST("SACL *-, 4, AR0",  0x6498);
    TEST("SACL *-, 0, AR1",  0x6099);
    TEST("SACL *+, 4, AR0",  0x64A8);
    if (is32020()) {
        TEST("SACL *BR0+, 4, AR7", 0x64FF);
    } else {
        TEST("SACL *BR0+, 7, AR7", 0x67FF);
    }

    TEST("SUB 70H",         0x1070);
    TEST("SUB 70H, 15",     0x1F70);
    if (is32020()) {
        ERRT("SUB 70H, 16", OVERFLOW_RANGE, "16", 0x1070);
    } else {
        TEST("SUB 70H, 16",     0x4470); // SUBH
        ERRT("SUB 70H, 17", OVERFLOW_RANGE, "17", 0x1170);
    }
    TEST("SUB *",           0x1080);
    TEST("SUB *, 0, AR0",   0x1088);
    TEST("SUB *, 0, AR7",   0x108F);
    TEST("SUB *-",          0x1090);
    TEST("SUB *+",          0x10A0);
    TEST("SUB *BR0-",       0x10C0);
    TEST("SUB *0-",         0x10D0);
    TEST("SUB *0+",         0x10E0);
    TEST("SUB *BR0+",       0x10F0);
    TEST("SUB *, 1",        0x1180);
    TEST("SUB *-, 2",       0x1290);
    TEST("SUB *+, 3",       0x13A0);
    TEST("SUB *, 0, AR0",   0x1088);
    TEST("SUB *, 4, AR1",   0x1489);
    TEST("SUB *-, 4, AR0",  0x1498);
    TEST("SUB *-, 0, AR1",  0x1099);
    TEST("SUB *+, 15, AR0", 0x1FA8);
    TEST("SUB *BR0+, 15, AR7", 0x1FFF);

    TEST("SUBC 70H",     0x4770);
    TEST("SUBC *",       0x4780);
    TEST("SUBC *-",      0x4790);
    TEST("SUBC *+",      0x47A0);
    TEST("SUBC *, AR0",  0x4788);
    TEST("SUBC *-, AR1", 0x4799);
    TEST("SUBC *+, AR0", 0x47A8);

    TEST("SUBH 70H",     0x4470);
    TEST("SUBH *",       0x4480);
    TEST("SUBH *-",      0x4490);
    TEST("SUBH *+",      0x44A0);
    TEST("SUBH *, AR0",  0x4488);
    TEST("SUBH *-, AR1", 0x4499);
    TEST("SUBH *+, AR0", 0x44A8);
    if (is320C2x()) {
        TEST("SUB 70H,16",      0x4470);
        TEST("SUB *,  16",      0x4480);
        TEST("SUB *-, 16",      0x4490);
        TEST("SUB *+, 16",      0x44A0);
        TEST("SUB *,  16, AR0", 0x4488);
        TEST("SUB *-, 16, AR1", 0x4499);
        TEST("SUB *+, 16, AR0", 0x44A8);
    }

    TEST("SUBS 70H",     0x4570);
    TEST("SUBS *",       0x4580);
    TEST("SUBS *-",      0x4590);
    TEST("SUBS *+",      0x45A0);
    TEST("SUBS *, AR0",  0x4588);
    TEST("SUBS *-, AR1", 0x4599);
    TEST("SUBS *+, AR0", 0x45A8);

    TEST("SUBT 70H",     0x4670);
    TEST("SUBT *",       0x4680);
    TEST("SUBT *-",      0x4690);
    TEST("SUBT *+",      0x46A0);
    TEST("SUBT *, AR0",  0x4688);
    TEST("SUBT *-, AR1", 0x4699);
    TEST("SUBT *+, AR0", 0x46A8);

    TEST("SBLK 0",          0xD003, 0x0000);
    TEST("SBLK 0FFFFH",     0xD003, 0xFFFF);
    TEST("SBLK 18, 4",      0xD403, 0x0012);
    TEST("SBLK 00FFH, 10",  0xDA03, 0x00FF);
    TEST("SBLK 0FFFFH, 15", 0xDF03, 0xFFFF);
    if (is320C2x()) {
        TEST("SUB #0",          0xCD00);
        TEST("SUB #0FFFFH",     0xD003, 0xFFFF);
        TEST("SUB #18, 4",      0xD403, 0x0012);
        TEST("SUB #00FFH, 10",  0xDA03, 0x00FF);
        TEST("SUB #0FFFFH, 15", 0xDF03, 0xFFFF);
    }

    if (is320C2x()) {
        TEST("SUBB 70H",     0x4F70);
        TEST("SUBB *",       0x4F80);
        TEST("SUBB *-",      0x4F90);
        TEST("SUBB *+",      0x4FA0);
        TEST("SUBB *, AR0",  0x4F88);
        TEST("SUBB *-, AR1", 0x4F99);
        TEST("SUBB *+, AR0", 0x4FA8);

        TEST("SUBK 0",   0xCD00);
        TEST("SUBK 255", 0xCDFF);
        TEST("SUB #0",   0xCD00);
        TEST("SUB #255", 0xCDFF);
    }

    TEST("XOR 70H",     0x4C70);
    TEST("XOR *",       0x4C80);
    TEST("XOR *-",      0x4C90);
    TEST("XOR *+",      0x4CA0);
    TEST("XOR *, AR0",  0x4C88);
    TEST("XOR *-, AR1", 0x4C99);
    TEST("XOR *+, AR0", 0x4CA8);

    TEST("XORK 0",          0xD006, 0x0000);
    TEST("XORK 0FFFFH",     0xD006, 0xFFFF);
    TEST("XORK 18, 4",      0xD406, 0x0012);
    TEST("XORK 00FFH, 10",  0xDA06, 0x00FF);
    TEST("XORK 0FFFFH, 15", 0xDF06, 0xFFFF);
    if (is320C2x()) {
        TEST("XOR #0",          0xD006, 0x0000);
        TEST("XOR #0FFFFH",     0xD006, 0xFFFF);
        TEST("XOR #18, 4",      0xD406, 0x0012);
        TEST("XOR #00FFH, 10",  0xDA06, 0x00FF);
        TEST("XOR #0FFFFH, 15", 0xDF06, 0xFFFF);
    }

    TEST("ZALH 70H",     0x4070);
    TEST("ZALH *",       0x4080);
    TEST("ZALH *-",      0x4090);
    TEST("ZALH *+",      0x40A0);
    TEST("ZALH *, AR0",  0x4088);
    TEST("ZALH *-, AR1", 0x4099);
    TEST("ZALH *+, AR0", 0x40A8);
    if (is320C2x()) {
        TEST("LACC 70H,16",      0x4070);
        TEST("LACC *,  16",      0x4080);
        TEST("LACC *-, 16",      0x4090);
        TEST("LACC *+, 16",      0x40A0);
        TEST("LACC *,  16, AR0", 0x4088);
        TEST("LACC *-, 16, AR1", 0x4099);
        TEST("LACC *+, 16, AR0", 0x40A8);
    }

    TEST("ZALS 70H",     0x4170);
    TEST("ZALS *",       0x4180);
    TEST("ZALS *-",      0x4190);
    TEST("ZALS *+",      0x41A0);
    TEST("ZALS *, AR0",  0x4188);
    TEST("ZALS *-, AR1", 0x4199);
    TEST("ZALS *+, AR0", 0x41A8);
    if (is320C2x()) {
        TEST("LACL 70H",     0x4170);
        TEST("LACL *",       0x4180);
        TEST("LACL *-",      0x4190);
        TEST("LACL *+",      0x41A0);
        TEST("LACL *, AR0",  0x4188);
        TEST("LACL *-, AR1", 0x4199);
        TEST("LACL *+, AR0", 0x41A8);
    }

    if (is320C2x()) {
        TEST("ZALR 70H",     0x7B70);
        TEST("ZALR *",       0x7B80);
        TEST("ZALR *-",      0x7B90);
        TEST("ZALR *+",      0x7BA0);
        TEST("ZALR *, AR0",  0x7B88);
        TEST("ZALR *-, AR1", 0x7B99);
        TEST("ZALR *+, AR0", 0x7BA8);
    }

    TEST("SFL", 0xCE18);
    TEST("SFR", 0xCE19);

    if (is320C2x()) {
        TEST("ROL", 0xCE34);
        TEST("ROR", 0xCE35);
    }
}

void test_accumrator_c5x() {
    TEST("ABS ", 0xBE00);

    TEST("ADD 70H",         0x2070);
    TEST("ADD 70H, 15",     0x2F70);
    TEST("ADD *",           0x2080);
    TEST("ADD *, 0, AR0",   0x2088);
    TEST("ADD *, 0, AR7",   0x208F);
    TEST("ADD *-",          0x2090);
    TEST("ADD *+",          0x20A0);
    TEST("ADD *BR0-",       0x20C0);
    TEST("ADD *0-",         0x20D0);
    TEST("ADD *0+",         0x20E0);
    TEST("ADD *BR0+",       0x20F0);
    TEST("ADD *, 1",        0x2180);
    TEST("ADD *-, 2",       0x2290);
    TEST("ADD *+, 3",       0x23A0);
    TEST("ADD *, 0, AR0",   0x2088);
    TEST("ADD *, 4, AR1",   0x2489);
    TEST("ADD *-, 4, AR0",  0x2498);
    TEST("ADD *-, 0, AR1",  0x2099);
    TEST("ADD *+, 15, AR0", 0x2FA8);
    TEST("ADD *BR0+, 15, AR7", 0x2FFF);

    TEST("ADD 70H, 16",     0x6170);
    TEST("ADD *, 16",       0x6180);
    TEST("ADD *-, 16",      0x6190);
    TEST("ADD *+, 16",      0x61A0);
    TEST("ADD *, 16, AR0",  0x6188);
    TEST("ADD *-, 16, AR1", 0x6199);
    TEST("ADD *+, 16, AR0", 0x61A8);

    TEST("ADDS 70H",     0x6270);
    TEST("ADDS *",       0x6280);
    TEST("ADDS *-",      0x6290);
    TEST("ADDS *+",      0x62A0);
    TEST("ADDS *, AR0",  0x6288);
    TEST("ADDS *-, AR1", 0x6299);
    TEST("ADDS *+, AR0", 0x62A8);

    TEST("ADDT 70H",     0x6370);
    TEST("ADDT *",       0x6380);
    TEST("ADDT *-",      0x6390);
    TEST("ADDT *+",      0x63A0);
    TEST("ADDT *, AR0",  0x6388);
    TEST("ADDT *-, AR1", 0x6399);
    TEST("ADDT *+, AR0", 0x63A8);

    TEST("ADD #0",   0xB800);
    TEST("ADD #255", 0xB8FF);
    TEST("ADD #-1",         0xBF90, 0xFFFF);
    TEST("ADD #0FFFFH",     0xBF90, 0xFFFF);
    TEST("ADD #18, 4",      0xBF94, 0x0012);
    TEST("ADD #00FFH, 10",  0xBF9A, 0x00FF);
    TEST("ADD #0FFFFH, 15", 0xBF9F, 0xFFFF);

    if (is320C5x()) {
        TEST("ADDB", 0xBE10);
    }

    TEST("ADDC 70H",     0x6070);
    TEST("ADDC *",       0x6080);
    TEST("ADDC *-",      0x6090);
    TEST("ADDC *+",      0x60A0);
    TEST("ADDC *, AR0",  0x6088);
    TEST("ADDC *-, AR1", 0x6099);
    TEST("ADDC *+, AR0", 0x60A8);

    if (is320C5x()) {
        TEST("ADCB", 0xBE11);
    }

    TEST("AND 70H",     0x6E70);
    TEST("AND *",       0x6E80);
    TEST("AND *-",      0x6E90);
    TEST("AND *+",      0x6EA0);
    TEST("AND *, AR0",  0x6E88);
    TEST("AND *-, AR1", 0x6E99);
    TEST("AND *+, AR0", 0x6EA8);

    TEST("AND #0",          0xBFB0, 0x0000);
    TEST("AND #0FFFFH",     0xBFB0, 0xFFFF);
    TEST("AND #18, 4",      0xBFB4, 0x0012);
    TEST("AND #00FFH, 10",  0xBFBA, 0x00FF);
    TEST("AND #0FFFFH, 15", 0xBFBF, 0xFFFF);
    TEST("AND #0FFFFH, 16", 0xBE81, 0xFFFF);

    if (is320C5x()) {
        TEST("ANDB", 0xBE12);
    }

    TEST("CMPL ", 0xBE01);

    TEST("LACC 70H",         0x1070);
    TEST("LACC 70H, 15",     0x1F70);
    TEST("LACC *",           0x1080);
    TEST("LACC *, 0, AR0",   0x1088);
    TEST("LACC *, 0, AR7",   0x108F);
    TEST("LACC *-",          0x1090);
    TEST("LACC *+",          0x10A0);
    TEST("LACC *BR0-",       0x10C0);
    TEST("LACC *0-",         0x10D0);
    TEST("LACC *0+",         0x10E0);
    TEST("LACC *BR0+",       0x10F0);
    TEST("LACC *, 1",        0x1180);
    TEST("LACC *-, 2",       0x1290);
    TEST("LACC *+, 3",       0x13A0);
    TEST("LACC *, 0, AR0",   0x1088);
    TEST("LACC *, 4, AR1",   0x1489);
    TEST("LACC *-, 4, AR0",  0x1498);
    TEST("LACC *-, 0, AR1",  0x1099);
    TEST("LACC *+, 15, AR0", 0x1FA8);
    TEST("LACC *BR0+, 15, AR7", 0x1FFF);

    TEST("LACL #0",   0xB900);
    TEST("LACL #1",   0xB901);
    TEST("LACL #255", 0xB9FF);

    TEST("LACT 70H",     0x6B70);
    TEST("LACT *",       0x6B80);
    TEST("LACT *-",      0x6B90);
    TEST("LACT *+",      0x6BA0);
    TEST("LACT *, AR0",  0x6B88);
    TEST("LACT *-, AR1", 0x6B99);
    TEST("LACT *+, AR0", 0x6BA8);

    TEST("LACC #0",          0xBF80, 0x0000);
    TEST("LACC #0FFFFH",     0xBF80, 0xFFFF);
    TEST("LACC #18, 4",      0xBF84, 0x0012);
    TEST("LACC #00FFH, 10",  0xBF8A, 0x00FF);
    TEST("LACC #0FFFFH, 15", 0xBF8F, 0xFFFF);

    TEST("NEG ", 0xBE02);

    TEST("NORM *",     0xA080);
    TEST("NORM *-",    0xA090);
    TEST("NORM *+",    0xA0A0);
    TEST("NORM *BR0-", 0xA0C0);
    TEST("NORM *0-",   0xA0D0);
    TEST("NORM *0+",   0xA0E0);
    TEST("NORM *BR0+", 0xA0F0);

    TEST("OR 70H",     0x6D70);
    TEST("OR *",       0x6D80);
    TEST("OR *-",      0x6D90);
    TEST("OR *+",      0x6DA0);
    TEST("OR *, AR0",  0x6D88);
    TEST("OR *-, AR1", 0x6D99);
    TEST("OR *+, AR0", 0x6DA8);

    TEST("OR #0",          0xBFC0, 0x0000);
    TEST("OR #0FFFFH",     0xBFC0, 0xFFFF);
    TEST("OR #18, 4",      0xBFC4, 0x0012);
    TEST("OR #00FFH, 10",  0xBFCA, 0x00FF);
    TEST("OR #0FFFFH, 15", 0xBFCF, 0xFFFF);
    TEST("OR #0FFFFH, 16", 0xBE82, 0xFFFF);

    if (is320C5x()) {
        TEST("ORB", 0xBE13);
    }

    TEST("SACH 70H",    0x9870);
    TEST("SACH 70H, 1", 0x9970);
    TEST("SACH 70H, 2", 0x9A70);
    TEST("SACH 70H, 3", 0x9B70);
    TEST("SACH 70H, 4", 0x9C70);
    TEST("SACH 70H, 5", 0x9D70);
    TEST("SACH 70H, 6", 0x9E70);
    TEST("SACH 70H, 7", 0x9F70);
    TEST("SACH *",      0x9880);
    TEST("SACH *, 0, AR0",  0x9888);
    TEST("SACH *, 0, AR7",  0x988F);
    TEST("SACH *-",         0x9890);
    TEST("SACH *+",         0x98A0);
    TEST("SACH *BR0-",      0x98C0);
    TEST("SACH *0-",        0x98D0);
    TEST("SACH *0+",        0x98E0);
    TEST("SACH *BR0+",      0x98F0);
    TEST("SACH *, 1",       0x9980);
    TEST("SACH *-, 1",      0x9990);
    TEST("SACH *+, 4",      0x9CA0);
    TEST("SACH *, 0, AR0",  0x9888);
    TEST("SACH *, 4, AR1",  0x9C89);
    TEST("SACH *-, 4, AR0", 0x9C98);
    TEST("SACH *-, 0, AR1", 0x9899);
    TEST("SACH *+, 4, AR0", 0x9CA8);
    TEST("SACH *BR0+, 7, AR7", 0x9FFF);

    TEST("SACL 70H",    0x9070);
    TEST("SACL 70H, 1", 0x9170);
    TEST("SACL 70H, 2", 0x9270);
    TEST("SACL 70H, 3", 0x9370);
    TEST("SACL 70H, 4", 0x9470);
    TEST("SACL 70H, 5", 0x9570);
    TEST("SACL 70H, 6", 0x9670);
    TEST("SACL 70H, 7", 0x9770);
    TEST("SACL *",      0x9080);
    TEST("SACL *, 0, AR0",  0x9088);
    TEST("SACL *, 0, AR7",  0x908F);
    TEST("SACL *-",         0x9090);
    TEST("SACL *+",         0x90A0);
    TEST("SACL *BR0-",      0x90C0);
    TEST("SACL *0-",        0x90D0);
    TEST("SACL *0+",        0x90E0);
    TEST("SACL *BR0+",      0x90F0);
    TEST("SACL *, 1",       0x9180);
    TEST("SACL *-, 1",      0x9190);
    TEST("SACL *+, 4",      0x94A0);
    TEST("SACL *, 0, AR0",  0x9088);
    TEST("SACL *, 4, AR1",  0x9489);
    TEST("SACL *-, 4, AR0", 0x9498);
    TEST("SACL *-, 0, AR1", 0x9099);
    TEST("SACL *+, 4, AR0", 0x94A8);
    TEST("SACL *BR0+, 7, AR7", 0x97FF);

    if (is320C5x()) {
        TEST("SAMM 70H",        0x8870);
        TEST("SAMM *, AR7",     0x888F);
        TEST("SAMM *-",         0x8890);
        TEST("SAMM *+",         0x88A0);
        TEST("SAMM *BR0-",      0x88C0);
        TEST("SAMM *0-",        0x88D0);
        TEST("SAMM *0+",        0x88E0);
        TEST("SAMM *BR0+",      0x88F0);
        TEST("SAMM *",          0x8880);
        TEST("SAMM *-",         0x8890);
        TEST("SAMM *+",         0x88A0);
        TEST("SAMM *, AR0",     0x8888);
        TEST("SAMM *-, AR1",    0x8899);
        TEST("SAMM *BR0+, AR7", 0x88FF);
    }

    TEST("SUB 70H",         0x3070);
    TEST("SUB 70H, 15",     0x3F70);
    TEST("SUB *",           0x3080);
    TEST("SUB *, 0, AR0",   0x3088);
    TEST("SUB *, 0, AR7",   0x308F);
    TEST("SUB *-",          0x3090);
    TEST("SUB *+",          0x30A0);
    TEST("SUB *BR0-",       0x30C0);
    TEST("SUB *0-",         0x30D0);
    TEST("SUB *0+",         0x30E0);
    TEST("SUB *BR0+",       0x30F0);
    TEST("SUB *, 1",        0x3180);
    TEST("SUB *-, 2",       0x3290);
    TEST("SUB *+, 3",       0x33A0);
    TEST("SUB *, 0, AR0",   0x3088);
    TEST("SUB *, 4, AR1",   0x3489);
    TEST("SUB *-, 4, AR0",  0x3498);
    TEST("SUB *-, 0, AR1",  0x3099);
    TEST("SUB *+, 15, AR0", 0x3FA8);
    TEST("SUB *BR0+, 15, AR7", 0x3FFF);

    TEST("SUB #0",   0xBA00);
    TEST("SUB #255", 0xBAFF);
    TEST("SUB #-1",         0xBFA0, 0xFFFF);
    TEST("SUB #0FFFFH",     0xBFA0, 0xFFFF);
    TEST("SUB #18, 4",      0xBFA4, 0x0012);
    TEST("SUB #00FFH, 10",  0xBFAA, 0x00FF);
    TEST("SUB #0FFFFH, 15", 0xBFAF, 0xFFFF);

    TEST("SUB 70H, 16",     0x6570);
    TEST("SUB *, 16",       0x6580);
    TEST("SUB *-, 16",      0x6590);
    TEST("SUB *+, 16",      0x65A0);
    TEST("SUB *, 16, AR0",  0x6588);
    TEST("SUB *-, 16, AR1", 0x6599);
    TEST("SUB *+, 16, AR0", 0x65A8);

    if (is320C5x()) {
        TEST("SBB", 0xBE18);
    }

    TEST("SUBC 70H",     0x0A70);
    TEST("SUBC *",       0x0A80);
    TEST("SUBC *-",      0x0A90);
    TEST("SUBC *+",      0x0AA0);
    TEST("SUBC *, AR0",  0x0A88);
    TEST("SUBC *-, AR1", 0x0A99);
    TEST("SUBC *+, AR0", 0x0AA8);

    TEST("SUBS 70H",     0x6670);
    TEST("SUBS *",       0x6680);
    TEST("SUBS *-",      0x6690);
    TEST("SUBS *+",      0x66A0);
    TEST("SUBS *, AR0",  0x6688);
    TEST("SUBS *-, AR1", 0x6699);
    TEST("SUBS *+, AR0", 0x66A8);

    TEST("SUBT 70H",     0x6770);
    TEST("SUBT *",       0x6780);
    TEST("SUBT *-",      0x6790);
    TEST("SUBT *+",      0x67A0);
    TEST("SUBT *, AR0",  0x6788);
    TEST("SUBT *-, AR1", 0x6799);
    TEST("SUBT *+, AR0", 0x67A8);

    TEST("SUBB 70H",     0x6470);
    TEST("SUBB *",       0x6480);
    TEST("SUBB *-",      0x6490);
    TEST("SUBB *+",      0x64A0);
    TEST("SUBB *, AR0",  0x6488);
    TEST("SUBB *-, AR1", 0x6499);
    TEST("SUBB *+, AR0", 0x64A8);

    if (is320C5x()) {
        TEST("SBBB", 0xBE19);
    }

    TEST("XOR 70H",     0x6C70);
    TEST("XOR *",       0x6C80);
    TEST("XOR *-",      0x6C90);
    TEST("XOR *+",      0x6CA0);
    TEST("XOR *, AR0",  0x6C88);
    TEST("XOR *-, AR1", 0x6C99);
    TEST("XOR *+, AR0", 0x6CA8);

    TEST("XOR #0",          0xBFD0, 0x0000);
    TEST("XOR #0FFFFH",     0xBFD0, 0xFFFF);
    TEST("XOR #18, 4",      0xBFD4, 0x0012);
    TEST("XOR #00FFH, 10",  0xBFDA, 0x00FF);
    TEST("XOR #0FFFFH, 15", 0xBFDF, 0xFFFF);
    TEST("XOR #0FFFFH, 16", 0xBE83, 0xFFFF);

    if (is320C5x()) {
        TEST("XORB", 0xBE1A);
    }

    TEST("LACC 70H, 16",     0x6A70);
    TEST("LACC *, 16",       0x6A80);
    TEST("LACC *-, 16",      0x6A90);
    TEST("LACC *+, 16",      0x6AA0);
    TEST("LACC *, 16, AR0",  0x6A88);
    TEST("LACC *-, 16, AR1", 0x6A99);
    TEST("LACC *+, 16, AR0", 0x6AA8);

    TEST("LACL 70H",     0x6970);
    TEST("LACL *",       0x6980);
    TEST("LACL *-",      0x6990);
    TEST("LACL *+",      0x69A0);
    TEST("LACL *, AR0",  0x6988);
    TEST("LACL *-, AR1", 0x6999);
    TEST("LACL *+, AR0", 0x69A8);

    if (is320C5x()) {
        TEST("LAMM 70H",     0x0870);
        TEST("LAMM *",       0x0880);
        TEST("LAMM *-",      0x0890);
        TEST("LAMM *+",      0x08A0);
        TEST("LAMM *, AR0",  0x0888);
        TEST("LAMM *-, AR1", 0x0899);
        TEST("LAMM *+, AR0", 0x08A8);
    }

    TEST("ZALR 70H",     0x6870);
    TEST("ZALR *",       0x6880);
    TEST("ZALR *-",      0x6890);
    TEST("ZALR *+",      0x68A0);
    TEST("ZALR *, AR0",  0x6888);
    TEST("ZALR *-, AR1", 0x6899);
    TEST("ZALR *+, AR0", 0x68A8);

    if (is320C5x()) {
        TEST("ZAP", 0xBE59);
    }

    TEST("SFL", 0xBE09);
    TEST("SFR", 0xBE0A);

    if (is320C5x()) {
        TEST("SFLB", 0xBE16);
        TEST("SFRB", 0xBE17);
    }

    TEST("ROL", 0xBE0C);
    TEST("ROR", 0xBE0D);

    if (is320C5x()) {
        TEST("ROLB", 0xBE14);
        TEST("RORB", 0xBE15);

        TEST("SATH", 0xBE5A);
        TEST("SATL", 0xBE5B);

        TEST("BSAR 1",  0xBFE0);
        TEST("BSAR 16", 0xBFEF);

        TEST("CRGT", 0xBE1B);
        TEST("CRLT", 0xBE1C);
        TEST("EXAR", 0xBE1D);
        TEST("SACB", 0xBE1E);
        TEST("LACB", 0xBE1F);
    }
}

void test_auxiliary_1x() {
    TEST("LAR AR0, 70H",     0x3870);
    TEST("LAR 0,   70H",     0x3870);
    ERRT("LAR 2,   70H",     UNKNOWN_REGISTER, "2,   70H", 0x3870);
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
    ERRT("LARK 1,   256", OVERFLOW_RANGE, "256", 0x7100);
    ERRT("LARK 2,   128", UNKNOWN_REGISTER, "2,   128", 0x7080);

    TEST("LARP AR0", 0x6880);
    TEST("LARP AR1", 0x6881);
    TEST("LARP 0",   0x6880);
    TEST("LARP 1",   0x6881);
    ERRT("LARP 2",   UNKNOWN_REGISTER, "2", 0x6880);

    TEST("LDP 70H",     0x6F70);
    TEST("LDP *",       0x6F88);
    TEST("LDP *-",      0x6F98);
    TEST("LDP *+",      0x6FA8);
    TEST("LDP *, AR0",  0x6F80);
    TEST("LDP *-, AR1", 0x6F91);
    TEST("LDP *+, AR0", 0x6FA0);

    TEST("LDPK 0", 0x6E00);
    TEST("LDPK 1", 0x6E01);
    ERRT("LDPK 2", OVERFLOW_RANGE, "2", 0x6E00);

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

void test_auxiliary_2x() {
    TEST("LAR AR0, 70H",     0x3070);
    TEST("LAR AR1, *",       0x3180);
    TEST("LAR AR2, *-",      0x3290);
    TEST("LAR AR3, *+",      0x33A0);
    TEST("LAR AR4, *, AR0",  0x3488);
    TEST("LAR AR5, *-, AR1", 0x3599);
    TEST("LAR AR6, *+, AR0", 0x36A8);
    TEST("LAR AR7, 70H",     0x3770);
    TEST("LAR AR0, *",       0x3080);
    TEST("LAR AR1, *-",      0x3190);
    TEST("LAR AR2, *+",      0x32A0);
    TEST("LAR AR3, *, AR0",  0x3388);
    TEST("LAR AR4, *-, AR1", 0x3499);
    TEST("LAR AR5, *+, AR0", 0x35A8);

    TEST("LARK AR0, 255", 0xC0FF);
    TEST("LARK AR7, 128", 0xC780);
    if (is320C2x()) {
        TEST("LAR AR0, #255", 0xC0FF);
        TEST("LAR AR7, #128", 0xC780);
    }

    TEST("LARP AR0", 0x5588);
    TEST("LARP AR7", 0x558F);
    if (is320C2x()) {
        TEST("MAR AR0", 0x5588);
        TEST("MAR AR7", 0x558F);
    }

    TEST("LDP 70H",     0x5270);
    TEST("LDP *",       0x5280);
    TEST("LDP *-",      0x5290);
    TEST("LDP *+",      0x52A0);
    TEST("LDP *, AR0",  0x5288);
    TEST("LDP *-, AR1", 0x5299);
    TEST("LDP *+, AR0", 0x52A8);

    TEST("LDPK 000H", 0xC800);
    TEST("LDPK 1FFH", 0xC9FF);
    if (is320C2x()) {
        TEST("LDP #000H", 0xC800);
        TEST("LDP #1FFH", 0xC9FF);
    }

    TEST("LRLK AR0, 0000H",  0xD000, 0x0000);
    TEST("LRLK AR7, 0FFFFH", 0xD700, 0xFFFF);
    if (is320C2x()) {
        TEST("LAR AR0, #0000H",  0xC000);
        TEST("LAR AR7, #0FFFFH", 0xD700, 0xFFFF);
    }

    if (is320C2x()) {
        TEST("ADRK 0",   0x7E00);
        TEST("ADRK 255", 0x7EFF);

        TEST("SBRK 0",   0x7F00);
        TEST("SBRK 255", 0x7FFF);
    }

    TEST("MAR 70H",     0x5570);
    TEST("MAR *",       0x5580);
    TEST("MAR *-",      0x5590);
    TEST("MAR *+",      0x55A0);
    TEST("MAR *-, AR1", 0x5599);
    TEST("MAR *+, AR0", 0x55A8);

    TEST("SAR AR0, 70H",     0x7070);
    TEST("SAR AR0, *",       0x7080);
    TEST("SAR AR0, *-",      0x7090);
    TEST("SAR AR0, *+",      0x70A0);
    TEST("SAR AR0, *, AR0",  0x7088);
    TEST("SAR AR0, *-, AR1", 0x7099);
    TEST("SAR AR0, *+, AR0", 0x70A8);
    TEST("SAR AR1, 70H",     0x7170);
    TEST("SAR AR1, *",       0x7180);
    TEST("SAR AR1, *-",      0x7190);
    TEST("SAR AR1, *+",      0x71A0);
    TEST("SAR AR1, *, AR0",  0x7188);
    TEST("SAR AR1, *-, AR1", 0x7199);
    TEST("SAR AR1, *+, AR0", 0x71A8);
}

void test_auxiliary_c5x() {
    TEST("LAR AR0, 70H",     0x0070);
    TEST("LAR AR1, *",       0x0180);
    TEST("LAR AR2, *-",      0x0290);
    TEST("LAR AR3, *+",      0x03A0);
    TEST("LAR AR4, *, AR0",  0x0488);
    TEST("LAR AR5, *-, AR1", 0x0599);
    TEST("LAR AR6, *+, AR0", 0x06A8);
    TEST("LAR AR7, 70H",     0x0770);
    TEST("LAR AR0, *",       0x0080);
    TEST("LAR AR1, *-",      0x0190);
    TEST("LAR AR2, *+",      0x02A0);
    TEST("LAR AR3, *, AR0",  0x0388);
    TEST("LAR AR4, *-, AR1", 0x0499);
    TEST("LAR AR5, *+, AR0", 0x05A8);

    TEST("LAR AR0, #255", 0xB0FF);
    TEST("LAR AR7, #128", 0xB780);

    TEST("MAR *, AR0", 0x8B88); // LARP AR0
    TEST("MAR *, AR7", 0x8B8F); // LARP AR7

    TEST("LDP 70H",     0x0D70);
    TEST("LDP *",       0x0D80);
    TEST("LDP *-",      0x0D90);
    TEST("LDP *+",      0x0DA0);
    TEST("LDP *, AR0",  0x0D88);
    TEST("LDP *-, AR1", 0x0D99);
    TEST("LDP *+, AR0", 0x0DA8);

    TEST("LDP #000H", 0xBC00);
    TEST("LDP #1FFH", 0xBDFF);

    TEST("LAR AR0, #0",      0xB000);
    TEST("LAR AR3, #0FFH",   0xB3FF);
    TEST("LAR AR4, #100H",   0xBF0C, 0x0100);
    TEST("LAR AR7, #0FFFFH", 0xBF0F, 0xFFFF);

    TEST("ADRK #0",   0x7800);
    TEST("ADRK #255", 0x78FF);

    TEST("SBRK #0",   0x7C00);
    TEST("SBRK #255", 0x7CFF);

    TEST("MAR 70H",     0x8B70);
    TEST("MAR *",       0x8B80);
    TEST("MAR *-",      0x8B90);
    TEST("MAR *+",      0x8BA0);
    TEST("MAR *, AR0",  0x8B88);
    TEST("MAR *-, AR1", 0x8B99);
    TEST("MAR *+, AR0", 0x8BA8);

    TEST("SAR AR0, 70H",     0x8070);
    TEST("SAR AR0, *",       0x8080);
    TEST("SAR AR0, *-",      0x8090);
    TEST("SAR AR0, *+",      0x80A0);
    TEST("SAR AR0, *, AR0",  0x8088);
    TEST("SAR AR0, *-, AR1", 0x8099);
    TEST("SAR AR0, *+, AR0", 0x80A8);
    TEST("SAR AR1, 70H",     0x8170);
    TEST("SAR AR1, *",       0x8180);
    TEST("SAR AR1, *-",      0x8190);
    TEST("SAR AR1, *+",      0x81A0);
    TEST("SAR AR1, *, AR0",  0x8188);
    TEST("SAR AR1, *-, AR1", 0x8199);
    TEST("SAR AR1, *+, AR0", 0x81A8);
}

void test_multiply_1x() {
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
    ERRT("MPYK 4096",  OVERFLOW_RANGE, "4096",  0x9000);
    TEST("MPYK -1",    0x9FFF);
    TEST("MPYK -2",    0x9FFE);
    TEST("MPYK -4096", 0x9000);
    ERRT("MPYK -4097", OVERFLOW_RANGE, "-4097", 0x8FFF);

    TEST("PAC",  0x7F8E);
    TEST("SPAC", 0x7F90);
}

void test_multiply_2x() {
    TEST("APAC", 0xCE15);

    TEST("LPH 70H",     0x5370);
    TEST("LPH *",       0x5380);
    TEST("LPH *-",      0x5390);
    TEST("LPH *+",      0x53A0);
    TEST("LPH *, AR0",  0x5388);
    TEST("LPH *-, AR1", 0x5399);
    TEST("LPH *+, AR0", 0x53A8);

    TEST("LT 70H",     0x3C70);
    TEST("LT *",       0x3C80);
    TEST("LT *-",      0x3C90);
    TEST("LT *+",      0x3CA0);
    TEST("LT *, AR0",  0x3C88);
    TEST("LT *-, AR1", 0x3C99);
    TEST("LT *+, AR0", 0x3CA8);

    TEST("LTA 70H",     0x3D70);
    TEST("LTA *",       0x3D80);
    TEST("LTA *-",      0x3D90);
    TEST("LTA *+",      0x3DA0);
    TEST("LTA *, AR0",  0x3D88);
    TEST("LTA *-, AR1", 0x3D99);
    TEST("LTA *+, AR0", 0x3DA8);

    TEST("LTD 70H",     0x3F70);
    TEST("LTD *",       0x3F80);
    TEST("LTD *-",      0x3F90);
    TEST("LTD *+",      0x3FA0);
    TEST("LTD *, AR0",  0x3F88);
    TEST("LTD *-, AR1", 0x3F99);
    TEST("LTD *+, AR0", 0x3FA8);

    TEST("LTP 70H",     0x3E70);
    TEST("LTP *",       0x3E80);
    TEST("LTP *-",      0x3E90);
    TEST("LTP *+",      0x3EA0);
    TEST("LTP *, AR0",  0x3E88);
    TEST("LTP *-, AR1", 0x3E99);
    TEST("LTP *+, AR0", 0x3EA8);

    TEST("LTS 70H",     0x5B70);
    TEST("LTS *",       0x5B80);
    TEST("LTS *-",      0x5B90);
    TEST("LTS *+",      0x5BA0);
    TEST("LTS *, AR0",  0x5B88);
    TEST("LTS *-, AR1", 0x5B99);
    TEST("LTS *+, AR0", 0x5BA8);

    TEST("MAC 1000H, 70H",     0x5D70, 0x1000);
    TEST("MAC 2000H, *",       0x5D80, 0x2000);
    TEST("MAC 4000H, *-",      0x5D90, 0x4000);
    TEST("MAC 8000H, *+",      0x5DA0, 0x8000);
    TEST("MAC 1234H, *, AR0",  0x5D88, 0x1234);
    TEST("MAC 5678H, *-, AR1", 0x5D99, 0x5678);
    TEST("MAC 9ABCH, *+, AR0", 0x5DA8, 0x9ABC);

    TEST("MACD 1000H, 70H",     0x5C70, 0x1000);
    TEST("MACD 2000H, *",       0x5C80, 0x2000);
    TEST("MACD 4000H, *-",      0x5C90, 0x4000);
    TEST("MACD 8000H, *+",      0x5CA0, 0x8000);
    TEST("MACD 1234H, *, AR0",  0x5C88, 0x1234);
    TEST("MACD 5678H, *-, AR1", 0x5C99, 0x5678);
    TEST("MACD 9ABCH, *+, AR0", 0x5CA8, 0x9ABC);

    TEST("MPY 70H",     0x3870);
    TEST("MPY *",       0x3880);
    TEST("MPY *-",      0x3890);
    TEST("MPY *+",      0x38A0);
    TEST("MPY *, AR0",  0x3888);
    TEST("MPY *-, AR1", 0x3899);
    TEST("MPY *+, AR0", 0x38A8);

    TEST("MPYK 0",     0xA000);
    TEST("MPYK 2",     0xA002);
    TEST("MPYK 4095",  0xAFFF);
    TEST("MPYK -1",    0xBFFF);
    TEST("MPYK -2",    0xBFFE);
    TEST("MPYK -4096", 0xB000);
    if (is320C2x()) {
        TEST("MPY #0",     0xA000);
        TEST("MPY #2",     0xA002);
        TEST("MPY #4095",  0xAFFF);
        TEST("MPY #-1",    0xBFFF);
        TEST("MPY #-2",    0xBFFE);
        TEST("MPY #-4096", 0xB000);
    }

    if (is320C2x()) {
        TEST("MPYA 70H",     0x3A70);
        TEST("MPYA *",       0x3A80);
        TEST("MPYA *-",      0x3A90);
        TEST("MPYA *+",      0x3AA0);
        TEST("MPYA *, AR0",  0x3A88);
        TEST("MPYA *-, AR1", 0x3A99);
        TEST("MPYA *+, AR0", 0x3AA8);

        TEST("MPYS 70H",     0x3B70);
        TEST("MPYS *",       0x3B80);
        TEST("MPYS *-",      0x3B90);
        TEST("MPYS *+",      0x3BA0);
        TEST("MPYS *, AR0",  0x3B88);
        TEST("MPYS *-, AR1", 0x3B99);
        TEST("MPYS *+, AR0", 0x3BA8);

        TEST("MPYU 70H",     0xCF70);
        TEST("MPYU *",       0xCF80);
        TEST("MPYU *-",      0xCF90);
        TEST("MPYU *+",      0xCFA0);
        TEST("MPYU *, AR0",  0xCF88);
        TEST("MPYU *-, AR1", 0xCF99);
        TEST("MPYU *+, AR0", 0xCFA8);
    }

    TEST("PAC ", 0xCE14);
    TEST("SPAC", 0xCE16);

    if (is320C2x()) {
        TEST("SPH 70H",     0x7D70);
        TEST("SPH *",       0x7D80);
        TEST("SPH *-",      0x7D90);
        TEST("SPH *+",      0x7DA0);
        TEST("SPH *, AR0",  0x7D88);
        TEST("SPH *-, AR1", 0x7D99);
        TEST("SPH *+, AR0", 0x7DA8);

        TEST("SPL 70H",     0x7C70);
        TEST("SPL *",       0x7C80);
        TEST("SPL *-",      0x7C90);
        TEST("SPL *+",      0x7CA0);
        TEST("SPL *, AR0",  0x7C88);
        TEST("SPL *-, AR1", 0x7C99);
        TEST("SPL *+, AR0", 0x7CA8);
    }

    TEST("SPM 0", 0xCE08);
    TEST("SPM 1", 0xCE09);
    TEST("SPM 2", 0xCE0A);
    TEST("SPM 3", 0xCE0B);

    TEST("SQRA 70H",     0x3970);
    TEST("SQRA *",       0x3980);
    TEST("SQRA *-",      0x3990);
    TEST("SQRA *+",      0x39A0);
    TEST("SQRA *, AR0",  0x3988);
    TEST("SQRA *-, AR1", 0x3999);
    TEST("SQRA *+, AR0", 0x39A8);

    TEST("SQRS 70H",     0x5A70);
    TEST("SQRS *",       0x5A80);
    TEST("SQRS *-",      0x5A90);
    TEST("SQRS *+",      0x5AA0);
    TEST("SQRS *, AR0",  0x5A88);
    TEST("SQRS *-, AR1", 0x5A99);
    TEST("SQRS *+, AR0", 0x5AA8);
}

void test_multiply_c5x() {
    TEST("APAC ", 0xBE04);

    TEST("LTA 70H",     0x7070);
    TEST("LTA *",       0x7080);
    TEST("LTA *-",      0x7090);
    TEST("LTA *+",      0x70A0);
    TEST("LTA *, AR0",  0x7088);
    TEST("LTA *-, AR1", 0x7099);
    TEST("LTA *+, AR0", 0x70A8);

    TEST("LTD 70H",     0x7270);
    TEST("LTD *",       0x7280);
    TEST("LTD *-",      0x7290);
    TEST("LTD *+",      0x72A0);
    TEST("LTD *, AR0",  0x7288);
    TEST("LTD *-, AR1", 0x7299);
    TEST("LTD *+, AR0", 0x72A8);

    TEST("LTP 70H",     0x7170);
    TEST("LTP *",       0x7180);
    TEST("LTP *-",      0x7190);
    TEST("LTP *+",      0x71A0);
    TEST("LTP *, AR0",  0x7188);
    TEST("LTP *-, AR1", 0x7199);
    TEST("LTP *+, AR0", 0x71A8);

    TEST("LTS 70H",     0x7470);
    TEST("LTS *",       0x7480);
    TEST("LTS *-",      0x7490);
    TEST("LTS *+",      0x74A0);
    TEST("LTS *, AR0",  0x7488);
    TEST("LTS *-, AR1", 0x7499);
    TEST("LTS *+, AR0", 0x74A8);

    TEST("MAC 1000H, 70H",     0xA270, 0x1000);
    TEST("MAC 2000H, *",       0xA280, 0x2000);
    TEST("MAC 4000H, *-",      0xA290, 0x4000);
    TEST("MAC 8000H, *+",      0xA2A0, 0x8000);
    TEST("MAC 1234H, *, AR0",  0xA288, 0x1234);
    TEST("MAC 5678H, *-, AR1", 0xA299, 0x5678);
    TEST("MAC 9ABCH, *+, AR0", 0xA2A8, 0x9ABC);

    TEST("MACD 1000H, 70H",     0xA370, 0x1000);
    TEST("MACD 2000H, *",       0xA380, 0x2000);
    TEST("MACD 4000H, *-",      0xA390, 0x4000);
    TEST("MACD 8000H, *+",      0xA3A0, 0x8000);
    TEST("MACD 1234H, *, AR0",  0xA388, 0x1234);
    TEST("MACD 5678H, *-, AR1", 0xA399, 0x5678);
    TEST("MACD 9ABCH, *+, AR0", 0xA3A8, 0x9ABC);

    if (is320C5x()) {
        TEST("MADD 70H",     0xAB70);
        TEST("MADD *",       0xAB80);
        TEST("MADD *-",      0xAB90);
        TEST("MADD *+",      0xABA0);
        TEST("MADD *, AR0",  0xAB88);
        TEST("MADD *-, AR1", 0xAB99);
        TEST("MADD *+, AR0", 0xABA8);

        TEST("MADS 70H",     0xAA70);
        TEST("MADS *",       0xAA80);
        TEST("MADS *-",      0xAA90);
        TEST("MADS *+",      0xAAA0);
        TEST("MADS *, AR0",  0xAA88);
        TEST("MADS *-, AR1", 0xAA99);
        TEST("MADS *+, AR0", 0xAAA8);
    }

    TEST("MPY 70H",     0x5470);
    TEST("MPY *",       0x5480);
    TEST("MPY *-",      0x5490);
    TEST("MPY *+",      0x54A0);
    TEST("MPY *, AR0",  0x5488);
    TEST("MPY *-, AR1", 0x5499);
    TEST("MPY *+, AR0", 0x54A8);

    TEST("MPY #0",     0xC000);
    TEST("MPY #2",     0xC002);
    TEST("MPY #4095",  0xCFFF);
    TEST("MPY #-1",    0xDFFF);
    TEST("MPY #-2",    0xDFFE);
    TEST("MPY #-4096", 0xD000);

    TEST("MPYA 70H",     0x5070);
    TEST("MPYA *",       0x5080);
    TEST("MPYA *-",      0x5090);
    TEST("MPYA *+",      0x50A0);
    TEST("MPYA *, AR0",  0x5088);
    TEST("MPYA *-, AR1", 0x5099);
    TEST("MPYA *+, AR0", 0x50A8);

    TEST("MPYS 70H",     0x5170);
    TEST("MPYS *",       0x5180);
    TEST("MPYS *-",      0x5190);
    TEST("MPYS *+",      0x51A0);
    TEST("MPYS *, AR0",  0x5188);
    TEST("MPYS *-, AR1", 0x5199);
    TEST("MPYS *+, AR0", 0x51A8);

    TEST("MPYU 70H",     0x5570);
    TEST("MPYU *",       0x5580);
    TEST("MPYU *-",      0x5590);
    TEST("MPYU *+",      0x55A0);
    TEST("MPYU *, AR0",  0x5588);
    TEST("MPYU *-, AR1", 0x5599);
    TEST("MPYU *+, AR0", 0x55A8);

    TEST("PAC",  0xBE03);
    TEST("SPAC", 0xBE05);
    if (is320C5x()) {
        TEST("ZPR", 0xBE58);
    }

    TEST("SPH 70H",     0x8D70);
    TEST("SPH *",       0x8D80);
    TEST("SPH *-",      0x8D90);
    TEST("SPH *+",      0x8DA0);
    TEST("SPH *, AR0",  0x8D88);
    TEST("SPH *-, AR1", 0x8D99);
    TEST("SPH *+, AR0", 0x8DA8);

    TEST("SPL 70H",     0x8C70);
    TEST("SPL *",       0x8C80);
    TEST("SPL *-",      0x8C90);
    TEST("SPL *+",      0x8CA0);
    TEST("SPL *, AR0",  0x8C88);
    TEST("SPL *-, AR1", 0x8C99);
    TEST("SPL *+, AR0", 0x8CA8);

    TEST("SPM 0", 0xBF00);
    TEST("SPM 1", 0xBF01);
    TEST("SPM 2", 0xBF02);
    TEST("SPM 3", 0xBF03);

    TEST("SQRA 70H",     0x5270);
    TEST("SQRA *",       0x5280);
    TEST("SQRA *-",      0x5290);
    TEST("SQRA *+",      0x52A0);
    TEST("SQRA *, AR0",  0x5288);
    TEST("SQRA *-, AR1", 0x5299);
    TEST("SQRA *+, AR0", 0x52A8);

    TEST("SQRS 70H",     0x5370);
    TEST("SQRS *",       0x5380);
    TEST("SQRS *-",      0x5390);
    TEST("SQRS *+",      0x53A0);
    TEST("SQRS *, AR0",  0x5388);
    TEST("SQRS *-, AR1", 0x5399);
    TEST("SQRS *+, AR0", 0x53A8);
}

void test_branch_1x() {
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
    TEST("B $+80H", 0xF900, 0x0080);
    ERRT("B UNDEF", UNDEFINED_SYMBOL, "UNDEF", 0xF900, 0x0000);
    ERRT("B 1000H", OVERFLOW_RANGE, "1000H", 0xF900, 0x0000);
    ERRT("B 2000H", OVERFLOW_RANGE, "2000H", 0xF900, 0x0000);
    ERRT("B 4000H", OVERFLOW_RANGE, "4000H", 0xF900, 0x0000);
    ERRT("B 8000H", OVERFLOW_RANGE, "8000H", 0xF900, 0x0000);

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
    ERRT("CALL UNDEF", UNDEFINED_SYMBOL, "UNDEF", 0xF800, 0x0000);

    TEST("CALA", 0x7F8C);
    TEST("RET",  0x7F8D);
}

void test_branch_2x() {
    TEST("B 1000H",  0xFF80, 0x1000);
    TEST("B 2000H",  0xFF80, 0x2000);
    TEST("B 4000H",  0xFF80, 0x4000);
    TEST("B 8000H",  0xFF80, 0x8000);
    TEST("B 0FFFFH", 0xFF80, 0xFFFF);
    TEST("B 8000H, *-",    0xFF90, 0x8000);
    TEST("B 8000H, *+",    0xFFA0, 0x8000);
    TEST("B 8000H, *BR0-", 0xFFC0, 0x8000);
    TEST("B 8000H, *0-",   0xFFD0, 0x8000);
    TEST("B 8000H, *0+",   0xFFE0, 0x8000);
    TEST("B 8000H, *BR0+", 0xFFF0, 0x8000);
    if (is320C2x()) {
        TEST("BCND 1000H, UNC",  0xFF80, 0x1000);
        TEST("BCND 2000H, UNC",  0xFF80, 0x2000);
        TEST("BCND 4000H, UNC",  0xFF80, 0x4000);
        TEST("BCND 8000H, UNC",  0xFF80, 0x8000);
        TEST("BCND 0FFFFH, UNC", 0xFF80, 0xFFFF);
    }

    TEST("BBNZ 9000H",     0xF980, 0x9000);
    TEST("BBNZ 9000H, *+", 0xF9A0, 0x9000);
    TEST("BBZ 9000H",      0xF880, 0x9000);
    TEST("BBZ 9000H, *+",  0xF8A0, 0x9000);
    TEST("BGEZ 9000H",     0xF480, 0x9000);
    TEST("BGEZ 9000H, *+", 0xF4A0, 0x9000);
    TEST("BGZ 9000H",      0xF180, 0x9000);
    TEST("BGZ 9000H, *+",  0xF1A0, 0x9000);
    TEST("BIOZ 9000H",     0xFA80, 0x9000);
    TEST("BIOZ 9000H, *+", 0xFAA0, 0x9000);
    TEST("BLEZ 9000H",     0xF280, 0x9000);
    TEST("BLEZ 9000H, *+", 0xF2A0, 0x9000);
    TEST("BLZ 9000H",      0xF380, 0x9000);
    TEST("BLZ 9000H, *+",  0xF3A0, 0x9000);
    TEST("BNV 9000H",      0xF780, 0x9000);
    TEST("BNV 9000H, *+",  0xF7A0, 0x9000);
    TEST("BNZ 9000H",      0xF580, 0x9000);
    TEST("BNZ 9000H, *+",  0xF5A0, 0x9000);
    TEST("BV 9000H",       0xF080, 0x9000);
    TEST("BV 9000H, *+",   0xF0A0, 0x9000);
    TEST("BZ 9000H",       0xF680, 0x9000);
    TEST("BZ 9000H, *+",   0xF6A0, 0x9000);
    if (is320C2x()) {
        TEST("BCND 9000H, TC",  0xF980, 0x9000);
        TEST("BCND 9000H, NTC", 0xF880, 0x9000);
        TEST("BCND 9000H, GEQ", 0xF480, 0x9000);
        TEST("BCND 9000H, GT",  0xF180, 0x9000);
        TEST("BCND 9000H, BIO", 0xFA80, 0x9000);
        TEST("BCND 9000H, LEQ", 0xF280, 0x9000);
        TEST("BCND 9000H, LT",  0xF380, 0x9000);
        TEST("BCND 9000H, NOV", 0xF780, 0x9000);
        TEST("BCND 9000H, NEQ", 0xF580, 0x9000);
        TEST("BCND 9000H, OV",  0xF080, 0x9000);
        TEST("BCND 9000H, EQ" , 0xF680, 0x9000);
        TEST("BCND 9000H, C",   0x5E80, 0x9000);
        TEST("BCND 9000H, NC",  0x5F80, 0x9000);
    }
    if (is320C2x()) {
        TEST("BC 9000H",       0x5E80, 0x9000);
        TEST("BC 9000H, *+",   0x5EA0, 0x9000);
        TEST("BNC 9000H",      0x5F80, 0x9000);
        TEST("BNC 9000H, *+",  0x5fA0, 0x9000);

        TEST("BCND 9000H, C",  0x5E80, 0x9000);
        TEST("BCND 9000H, NC", 0x5F80, 0x9000);
    }

    TEST("BANZ 9000H",     0xFB80, 0x9000);
    TEST("BANZ 9000H, *+", 0xFBA0, 0x9000);
    TEST("CALL 9000H",     0xFE80, 0x9000);
    TEST("CALL 9000H, *+", 0xFEA0, 0x9000);

    TEST("BACC", 0xCE25);
    TEST("CALA", 0xCE24);
    TEST("RET ", 0xCE26);
}

void test_branch_c5x() {
    TEST("BACC ", 0xBE20);

    if (is320C5x()) {
        TEST("BACCD", 0xBE21);
    }

    TEST("B 1000H",  0x7980, 0x1000);
    TEST("B 2000H",  0x7980, 0x2000);
    TEST("B 4000H",  0x7980, 0x4000);
    TEST("B 8000H",  0x7980, 0x8000);
    TEST("B 0FFFFH", 0x7980, 0xFFFF);
    TEST("B 8000H, *-",    0x7990, 0x8000);
    TEST("B 8000H, *+",    0x79A0, 0x8000);
    TEST("B 8000H, *BR0-", 0x79C0, 0x8000);
    TEST("B 8000H, *0-",   0x79D0, 0x8000);
    TEST("B 8000H, *0+",   0x79E0, 0x8000);
    TEST("B 8000H, *BR0+", 0x79F0, 0x8000);

    if (is320C5x()) {
        TEST("BD 1000H",  0x7D80, 0x1000);
        TEST("BD 2000H",  0x7D80, 0x2000);
        TEST("BD 4000H",  0x7D80, 0x4000);
        TEST("BD 8000H",  0x7D80, 0x8000);
        TEST("BD 0FFFFH", 0x7D80, 0xFFFF);
        TEST("BD 8000H, *-",    0x7D90, 0x8000);
        TEST("BD 8000H, *+",    0x7DA0, 0x8000);
        TEST("BD 8000H, *BR0-", 0x7DC0, 0x8000);
        TEST("BD 8000H, *0-",   0x7DD0, 0x8000);
        TEST("BD 8000H, *0+",   0x7DE0, 0x8000);
        TEST("BD 8000H, *BR0+", 0x7DF0, 0x8000);
    }

    TEST("BANZ 9000H",     0x7B80, 0x9000);
    TEST("BANZ 9000H, *+", 0x7BA0, 0x9000);

    if (is320C5x()) {
        TEST("BANZD 9000H",     0x7F80, 0x9000);
        TEST("BANZD 9000H, *+", 0x7FA0, 0x9000);
    }

    TEST("BCND 9000H, NTC", 0xE200, 0x9000);
    TEST("BCND 9000H, TC",  0xE100, 0x9000);
    TEST("BCND 9000H, BIO", 0xE000, 0x9000);
    TEST("BCND 9000H, GEQ", 0xE38C, 0x9000);
    TEST("BCND 9000H, GT",  0xE304, 0x9000);
    TEST("BCND 9000H, LEQ", 0xE3CC, 0x9000);
    TEST("BCND 9000H, LT",  0xE344, 0x9000);
    TEST("BCND 9000H, NOV", 0xE302, 0x9000);
    TEST("BCND 9000H, NEQ", 0xE308, 0x9000);
    TEST("BCND 9000H, OV",  0xE322, 0x9000);
    TEST("BCND 9000H, EQ",  0xE388, 0x9000);
    TEST("BCND 9000H, C",   0xE311, 0x9000);
    TEST("BCND 9000H, NC",  0xE301, 0x9000);
    TEST("BCND 9000H, LEQ,OV,C",    0xE3FF, 0x9000);
    TEST("BCND 9000H, TC,LEQ,OV,C", 0xE1FF, 0x9000);

    TEST("CALA ", 0xBE30);

    if (is320C5x()) {
        TEST("CALAD", 0xBE3D);
    }

    TEST("CALL 9000H",     0x7A80, 0x9000);
    TEST("CALL 9000H, *+", 0x7AA0, 0x9000);

    if (is320C5x()) {
        TEST("CALLD 9000H",     0x7E80, 0x9000);
        TEST("CALLD 9000H, *+", 0x7EA0, 0x9000);
    }

    if (is320C5x()) {
        TEST("BCNDD 9000H, NTC", 0xF200, 0x9000);
        TEST("BCNDD 9000H, TC",  0xF100, 0x9000);
        TEST("BCNDD 9000H, BIO", 0xF000, 0x9000);
        TEST("BCNDD 9000H, GEQ", 0xF38C, 0x9000);
        TEST("BCNDD 9000H, GT",  0xF304, 0x9000);
        TEST("BCNDD 9000H, LEQ", 0xF3CC, 0x9000);
        TEST("BCNDD 9000H, LT",  0xF344, 0x9000);
        TEST("BCNDD 9000H, NOV", 0xF302, 0x9000);
        TEST("BCNDD 9000H, NEQ", 0xF308, 0x9000);
        TEST("BCNDD 9000H, OV",  0xF322, 0x9000);
        TEST("BCNDD 9000H, EQ",  0xF388, 0x9000);
        TEST("BCNDD 9000H, C",   0xF311, 0x9000);
        TEST("BCNDD 9000H, NC",  0xF301, 0x9000);
        TEST("BCNDD 9000H, LEQ,OV,C",    0xF3FF, 0x9000);
        TEST("BCNDD 9000H, TC,LEQ,OV,C", 0xF1FF, 0x9000);
    }

    TEST("CC 9000H, NTC", 0xEA00, 0x9000);
    TEST("CC 9000H, TC",  0xE900, 0x9000);
    TEST("CC 9000H, BIO", 0xE800, 0x9000);
    TEST("CC 9000H, GEQ", 0xEB8C, 0x9000);
    TEST("CC 9000H, GT",  0xEB04, 0x9000);
    TEST("CC 9000H, LEQ", 0xEBCC, 0x9000);
    TEST("CC 9000H, LT",  0xEB44, 0x9000);
    TEST("CC 9000H, NOV", 0xEB02, 0x9000);
    TEST("CC 9000H, NEQ", 0xEB08, 0x9000);
    TEST("CC 9000H, OV",  0xEB22, 0x9000);
    TEST("CC 9000H, EQ",  0xEB88, 0x9000);
    TEST("CC 9000H, C",   0xEB11, 0x9000);
    TEST("CC 9000H, NC",  0xEB01, 0x9000);
    TEST("CC 9000H, LEQ,OV,C",     0xEBFF, 0x9000);
    TEST("CC 9000H, NTC,LEQ,OV,C", 0xEAFF, 0x9000);


    if (is320C5x()) {
        TEST("CCD 9000H, NTC", 0xFA00, 0x9000);
        TEST("CCD 9000H, TC",  0xF900, 0x9000);
        TEST("CCD 9000H, BIO", 0xF800, 0x9000);
        TEST("CCD 9000H, GEQ", 0xFB8C, 0x9000);
        TEST("CCD 9000H, GT",  0xFB04, 0x9000);
        TEST("CCD 9000H, LEQ", 0xFBCC, 0x9000);
        TEST("CCD 9000H, LT",  0xFB44, 0x9000);
        TEST("CCD 9000H, NOV", 0xFB02, 0x9000);
        TEST("CCD 9000H, NEQ", 0xFB08, 0x9000);
        TEST("CCD 9000H, OV",  0xFB22, 0x9000);
        TEST("CCD 9000H, EQ",  0xFB88, 0x9000);
        TEST("CCD 9000H, C",   0xFB11, 0x9000);
        TEST("CCD 9000H, NC",  0xFB01, 0x9000);
        TEST("CCD 9000H, LEQ,OV,C",     0xFBFF, 0x9000);
        TEST("CCD 9000H, NTC,LEQ,OV,C", 0xFAFF, 0x9000);
    }

    TEST("RET",      0xEF00);
    TEST("RETC BIO", 0xEC00);
    TEST("RETC TC",  0xED00);
    TEST("RETC NTC", 0xEE00);
    TEST("RETC NC",  0xEF01);
    TEST("RETC C",   0xEF11);
    TEST("RETC NOV", 0xEF02);
    TEST("RETC OV",  0xEF22);
    TEST("RETC NEQ", 0xEF08);
    TEST("RETC EQ",  0xEF88);
    TEST("RETC LT",  0xEF44);
    TEST("RETC GEQ", 0xEF8C);
    TEST("RETC GT",  0xEF04);
    TEST("RETC LEQ", 0xEFCC);
    TEST("RETC LEQ,OV,C",     0xEFFF);
    TEST("RETC BIO,LEQ,OV,C", 0xECFF);

    if (is320C5x()) {
        TEST("RETD",      0xFF00);
        TEST("RETCD BIO", 0xFC00);
        TEST("RETCD TC",  0xFD00);
        TEST("RETCD NTC", 0xFE00);
        TEST("RETCD NC",  0xFF01);
        TEST("RETCD C",   0xFF11);
        TEST("RETCD NOV", 0xFF02);
        TEST("RETCD OV",  0xFF22);
        TEST("RETCD NEQ", 0xFF08);
        TEST("RETCD EQ",  0xFF88);
        TEST("RETCD GEQ", 0xFF8C);
        TEST("RETCD LT",  0xFF44);
        TEST("RETCD GT",  0xFF04);
        TEST("RETCD GEQ", 0xFF8C);
        TEST("RETCD LEQ", 0xFFCC);
        TEST("RETCD LEQ,OV,C",     0xFFFF);
        TEST("RETCD BIO,LEQ,OV,C", 0xFCFF);

        TEST("RETI", 0xBE38);
        TEST("RETE", 0xBE3A);

        TEST("XC 1, BIO", 0xE400);
        TEST("XC 1, TC",  0xE500);
        TEST("XC 1, NTC", 0xE600);
        TEST("XC 1, NC",  0xE701);
        TEST("XC 1, C",   0xE711);
        TEST("XC 1, NOV", 0xE702);
        TEST("XC 1, OV",  0xE722);
        TEST("XC 1, NEQ", 0xE708);
        TEST("XC 1, EQ",  0xE788);
        TEST("XC 1, GEQ", 0xE78C);
        TEST("XC 1, LT",  0xE744);
        TEST("XC 1, GT",  0xE704);
        TEST("XC 1, GEQ", 0xE78C);
        TEST("XC 1, LT",  0xE744);
        TEST("XC 1, LEQ", 0xE7CC);
        TEST("XC 1, LEQ,OV,C",     0xE7FF);
        TEST("XC 1, BIO,LEQ,OV,C", 0xE4FF);
        TEST("XC 2, BIO", 0xF400);
        TEST("XC 2, TC",  0xF500);
        TEST("XC 2, NTC", 0xF600);
        TEST("XC 2, NC",  0xF701);
        TEST("XC 2, C",   0xF711);
        TEST("XC 2, NOV", 0xF702);
        TEST("XC 2, OV",  0xF722);
        TEST("XC 2, NEQ", 0xF708);
        TEST("XC 2, EQ",  0xF788);
        TEST("XC 2, GEQ", 0xF78C);
        TEST("XC 2, LT",  0xF744);
        TEST("XC 2, GT",  0xF704);
        TEST("XC 2, GEQ", 0xF78C);
        TEST("XC 2, LT",  0xF744);
        TEST("XC 2, LEQ", 0xF7CC);
        TEST("XC 2, LEQ,OV,C",     0xF7FF);
        TEST("XC 2, BIO,LEQ,OV,C", 0xF4FF);
    }
}

void test_control_1x() {
    TEST("DINT", 0x7F81);
    TEST("EINT", 0x7F82);
    TEST("NOP",  0x7F80);
    TEST("POP",  0x7F9D);
    TEST("PUSH", 0x7F9C);
    TEST("ROVM", 0x7F8A);
    TEST("SOVM", 0x7F8B);

    TEST("LST 00H",     0x7B00);
    TEST("LST 70H",     0x7B70);
    if (is32010()) {
        ERRT("LST 90H", OVERFLOW_RANGE, "90H", 0x7B10);
    }
    if (is32015()) {
        TEST("LST 90H", 0x7B10);
    }
    TEST("LST *",       0x7B88);
    TEST("LST *-",      0x7B98);
    TEST("LST *+",      0x7BA8);
    TEST("LST *, AR0",  0x7B80);
    TEST("LST *-, AR1", 0x7B91);
    TEST("LST *+, AR0", 0x7BA0);

    ERRT("SST 00H", OVERFLOW_RANGE, "00H", 0x7C00);
    ERRT("SST 10H", OVERFLOW_RANGE, "10H", 0x7C10);
    TEST("SST 80H", 0x7C00);
    TEST("SST 8FH", 0x7C0F);
    if (is32010()) {
        ERRT("SST 90H", OVERFLOW_RANGE, "90H", 0x7C10);
    }
    if (is32015()) {
        TEST("SST 90H", 0x7C10);
    }
    TEST("SST *",       0x7C88);
    TEST("SST *-",      0x7C98);
    TEST("SST *+",      0x7CA8);
    TEST("SST *, AR0",  0x7C80);
    TEST("SST *-, AR1", 0x7C91);
    TEST("SST *+, AR0", 0x7CA0);
}

void test_control_2x() {
    TEST("BIT 70H, 0", 0x9070);
    if (is320C26()) {
        TEST("CONF 0", 0xCE3C);
        TEST("CONF 1", 0xCE3D);
        TEST("CONF 2", 0xCE3E);
        TEST("CONF 3", 0xCE3F);
    } else {
        TEST("CNFD", 0xCE04);
        TEST("CNFP", 0xCE05);
        if (is320C2x()) {
            TEST("CLRC CNF", 0xCE04);
            TEST("SETC CNF", 0xCE05);
        }
    }

    TEST("DINT", 0xCE01);
    TEST("EINT", 0xCE00);
    TEST("NOP ", 0x5500);
    TEST("POP ", 0xCE1D);
    TEST("PUSH", 0xCE1C);
    TEST("POPD *",  0x7A80);
    TEST("PSHD *+", 0x54A0);
    TEST("ROVM", 0xCE02);
    TEST("SOVM", 0xCE03);
    TEST("RSXM", 0xCE06);
    TEST("SSXM", 0xCE07);
    if (is320C2x()) {
        TEST("RC", 0xCE30);
        TEST("SC", 0xCE31);
        TEST("RHM", 0xCE38);
        TEST("SHM", 0xCE39);
        TEST("RTC", 0xCE32);
        TEST("STC", 0xCE33);

        TEST("CLRC INTM", 0xCE00);
        TEST("SETC INTM", 0xCE01);
        TEST("CLRC OVM",  0xCE02);
        TEST("SETC OVM",  0xCE03);
        TEST("CLRC SXM",  0xCE06);
        TEST("SETC SXM",  0xCE07);
        TEST("CLRC C",    0xCE30);
        TEST("SETC C",    0xCE31);
        TEST("CLRC HM",   0xCE38);
        TEST("SETC HM",   0xCE39);
        TEST("CLRC TC",   0xCE32);
        TEST("SETC TC",   0xCE33);
    }

    TEST("TRAP", 0xCE1E);

    TEST("RPT 00H",     0x4B00);
    TEST("RPT 70H",     0x4B70);
    TEST("RPT *",       0x4B80);
    TEST("RPT *-",      0x4B90);
    TEST("RPT *+",      0x4BA0);
    TEST("RPT *, AR0",  0x4B88);
    TEST("RPT *-, AR7", 0x4B9F);
    TEST("RPT *+, AR4", 0x4BAC);
    TEST("RPTK 100",    0xCB64);
    if (is320C2x()) {
        TEST("RPT #100",    0xCB64);
    }

    TEST("LST 00H",     0x5000);
    TEST("LST 70H",     0x5070);
    TEST("LST *",       0x5080);
    TEST("LST *-",      0x5090);
    TEST("LST *+",      0x50A0);
    TEST("LST *, AR0",  0x5088);
    TEST("LST *-, AR7", 0x509F);
    TEST("LST *+, AR4", 0x50AC);
    TEST("LST1 00H",     0x5100);
    TEST("LST1 70H",     0x5170);
    TEST("LST1 *",       0x5180);
    TEST("LST1 *-",      0x5190);
    TEST("LST1 *+",      0x51A0);
    TEST("LST1 *, AR0",  0x5188);
    TEST("LST1 *-, AR7", 0x519F);
    TEST("LST1 *+, AR4", 0x51AC);
    if (is320C2x()) {
        TEST("LST #0, 00H",     0x5000);
        TEST("LST #0, 70H",     0x5070);
        TEST("LST #0, *",       0x5080);
        TEST("LST #0, *-",      0x5090);
        TEST("LST #0, *+",      0x50A0);
        TEST("LST #0, *, AR0",  0x5088);
        TEST("LST #0, *-, AR7", 0x509F);
        TEST("LST #0, *+, AR4", 0x50AC);
        TEST("LST #1, 00H",     0x5100);
        TEST("LST #1, 70H",     0x5170);
        TEST("LST #1, *",       0x5180);
        TEST("LST #1, *-",      0x5190);
        TEST("LST #1, *+",      0x51A0);
        TEST("LST #1, *, AR0",  0x5188);
        TEST("LST #1, *-, AR7", 0x519F);
        TEST("LST #1, *+, AR4", 0x51AC);
        ERRT("LST #2, 00H", OVERFLOW_RANGE, "#2, 00H", 0x5000);
    }

    TEST("SST 10H",     0x7810);
    TEST("SST 1FH",     0x781F);
    TEST("SST 7FH",     0x787F);
    ERRT("SST 80H", OVERFLOW_RANGE, "80H", 0x7800);
    TEST("SST *",       0x7880);
    TEST("SST *-",      0x7890);
    TEST("SST *+",      0x78A0);
    TEST("SST *, AR0",  0x7888);
    TEST("SST *-, AR7", 0x789F);
    TEST("SST *+, AR0", 0x78A8);
    TEST("SST1 10H",     0x7910);
    TEST("SST1 1FH",     0x791F);
    TEST("SST1 7FH",     0x797F);
    ERRT("SST1 80H", OVERFLOW_RANGE, "80H", 0x7900);
    TEST("SST1 *",       0x7980);
    TEST("SST1 *-",      0x7990);
    TEST("SST1 *+",      0x79A0);
    TEST("SST1 *, AR0",  0x7988);
    TEST("SST1 *-, AR7", 0x799F);
    TEST("SST1 *+, AR0", 0x79A8);
    if (is320C2x()) {
        TEST("SST #0, 10H",     0x7810);
        TEST("SST #0, 1FH",     0x781F);
        TEST("SST #0, 7FH",     0x787F);
        TEST("SST #0, *",       0x7880);
        TEST("SST #0, *-",      0x7890);
        TEST("SST #0, *+",      0x78A0);
        TEST("SST #0, *, AR0",  0x7888);
        TEST("SST #0, *-, AR7", 0x789F);
        TEST("SST #0, *+, AR0", 0x78A8);
        TEST("SST #1, 10H",     0x7910);
        TEST("SST #1, 1FH",     0x791F);
        TEST("SST #1, 7FH",     0x797F);
        TEST("SST #1, *",       0x7980);
        TEST("SST #1, *-",      0x7990);
        TEST("SST #1, *+",      0x79A0);
        TEST("SST #1, *, AR0",  0x7988);
        TEST("SST #1, *-, AR7", 0x799F);
        TEST("SST #1, *+, AR0", 0x79A8);
    }
}

void test_control_c5x() {
    TEST("BIT 70H, 2", 0x4270);

    TEST("CLRC INTM", 0xBE40);
    TEST("SETC INTM", 0xBE41);
    TEST("CLRC OVM",  0xBE42);
    TEST("SETC OVM",  0xBE43);
    TEST("CLRC CNF",  0xBE44);
    TEST("SETC CNF",  0xBE45);
    TEST("CLRC SXM",  0xBE46);
    TEST("SETC SXM",  0xBE47);
    if (is320C5x()) {
        TEST("CLRC HM", 0xBE48);
        TEST("SETC HM", 0xBE49);
    } else {
        ERRT("CLRC HM",  ILLEGAL_REGISTER, "HM", 0xBE48);
        ERRT("SETC HM",  ILLEGAL_REGISTER, "HM", 0xBE49);
    }
    TEST("CLRC TC",   0xBE4A);
    TEST("SETC TC",   0xBE4B);
    TEST("CLRC C",    0xBE4E);
    TEST("SETC C",    0xBE4F);

    TEST("NOP",     0x8B00);
    TEST("POP",     0xBE32);
    TEST("PUSH ",   0xBE3C);
    TEST("POPD *",  0x8A80);
    TEST("PSHD *+", 0x76A0);

    TEST("IDLE",    0xBE22);
    if (is320C5x()) {
        TEST("IDLE2", 0xBE23);
    }

    TEST("TRAP ",   0xBE51);
    TEST("NMI",     0xBE52);
    TEST("INTR 31", 0xBE7F);

    TEST("RPT 00H",     0x0B00);
    TEST("RPT 70H",     0x0B70);
    TEST("RPT *",       0x0B80);
    TEST("RPT *-",      0x0B90);
    TEST("RPT *+",      0x0BA0);
    TEST("RPT *, AR0",  0x0B88);
    TEST("RPT *-, AR7", 0x0B9F);
    TEST("RPT *+, AR4", 0x0BAC);
    TEST("RPT #100",    0xBB64);

    if (is320C5x()) {
        TEST("RPTZ #1234H", 0xBEC5, 0x1234);
        TEST("RPTB 1234H",  0xBEC6, 0x1234);
    }

    TEST("LST #0, 00H",     0x0E00);
    TEST("LST #0, 70H",     0x0E70);
    TEST("LST #0, *",       0x0E80);
    TEST("LST #0, *-",      0x0E90);
    TEST("LST #0, *+",      0x0EA0);
    TEST("LST #0, *, AR0",  0x0E88);
    TEST("LST #0, *-, AR7", 0x0E9F);
    TEST("LST #0, *+, AR4", 0x0EAC);
    TEST("LST #1, 00H",     0x0F00);
    TEST("LST #1, 70H",     0x0F70);
    TEST("LST #1, *",       0x0F80);
    TEST("LST #1, *-",      0x0F90);
    TEST("LST #1, *+",      0x0FA0);
    TEST("LST #1, *, AR0",  0x0F88);
    TEST("LST #1, *-, AR7", 0x0F9F);
    TEST("LST #1, *+, AR4", 0x0FAC);

    TEST("SST #0, 10H",     0x8E10);
    TEST("SST #0, 1FH",     0x8E1F);
    TEST("SST #0, 7FH",     0x8E7F);
    TEST("SST #0, *",       0x8E80);
    TEST("SST #0, *-",      0x8E90);
    TEST("SST #0, *+",      0x8EA0);
    TEST("SST #0, *, AR0",  0x8E88);
    TEST("SST #0, *-, AR7", 0x8E9F);
    TEST("SST #0, *+, AR0", 0x8EA8);
    TEST("SST #1, 10H",     0x8F10);
    TEST("SST #1, 1FH",     0x8F1F);
    TEST("SST #1, 7FH",     0x8F7F);
    TEST("SST #1, *",       0x8F80);
    TEST("SST #1, *-",      0x8F90);
    TEST("SST #1, *+",      0x8FA0);
    TEST("SST #1, *, AR0",  0x8F88);
    TEST("SST #1, *-, AR7", 0x8F9F);
    TEST("SST #1, *+, AR0", 0x8FA8);
}

void test_dataio_1x() {
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

    TEST("in 70h, pa0",     0x4070);
    TEST("IN 70H, PA7",     0x4770);
    TEST("IN 70H, 0",       0x4070);
    TEST("IN 70H, 7",       0x4770);
    ERRT("IN 70H, 8",       OVERFLOW_RANGE, "8", 0x4070);
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

    ERRT("OUT *+, UNDEF, AR0", UNDEFINED_SYMBOL, "UNDEF, AR0", 0x48A0);
    ERRT("OUT *+, PA7, UNDEF", UNDEFINED_SYMBOL, "UNDEF",      0x4FA0);
    ERRT("OUT *+, UNDEF, AR1", UNDEFINED_SYMBOL, "UNDEF, AR1", 0x48A1);
    ERRT("OUT *+, PA7, UNDEF", UNDEFINED_SYMBOL, "UNDEF",      0x4FA0);
}

void test_dataio_2x() {
    TEST("BLKD 1234H, 00H",     0xFD00, 0x1234);
    TEST("BLKD 1234H, 70H",     0xFD70, 0x1234);
    TEST("BLKD 1234H, *",       0xFD80, 0x1234);
    TEST("BLKD 1234H, *-",      0xFD90, 0x1234);
    TEST("BLKD 1234H, *+",      0xFDA0, 0x1234);
    TEST("BLKD 1234H, *, AR0",  0xFD88, 0x1234);
    TEST("BLKD 1234H, *-, AR1", 0xFD99, 0x1234);
    TEST("BLKD 1234H, *+, AR0", 0xFDA8, 0x1234);
    if (is320C2x()) {
        TEST("BLDD 1234H, 00H",     0xFD00, 0x1234);
        TEST("BLDD 1234H, 70H",     0xFD70, 0x1234);
        TEST("BLDD 1234H, *",       0xFD80, 0x1234);
        TEST("BLDD 1234H, *-",      0xFD90, 0x1234);
        TEST("BLDD 1234H, *+",      0xFDA0, 0x1234);
        TEST("BLDD 1234H, *, AR0",  0xFD88, 0x1234);
        TEST("BLDD 1234H, *-, AR1", 0xFD99, 0x1234);
        TEST("BLDD 1234H, *+, AR0", 0xFDA8, 0x1234);
    }

    TEST("BLKP 1234H, 00H",     0xFC00, 0x1234);
    TEST("BLKP 1234H, 70H",     0xFC70, 0x1234);
    TEST("BLKP 1234H, *",       0xFC80, 0x1234);
    TEST("BLKP 1234H, *-",      0xFC90, 0x1234);
    TEST("BLKP 1234H, *+",      0xFCA0, 0x1234);
    TEST("BLKP 1234H, *, AR0",  0xFC88, 0x1234);
    TEST("BLKP 1234H, *-, AR1", 0xFC99, 0x1234);
    TEST("BLKP 1234H, *+, AR0", 0xFCA8, 0x1234);
    if (is320C2x()) {
        TEST("BLPD 1234H, 00H",     0xFC00, 0x1234);
        TEST("BLPD 1234H, 70H",     0xFC70, 0x1234);
        TEST("BLPD 1234H, *",       0xFC80, 0x1234);
        TEST("BLPD 1234H, *-",      0xFC90, 0x1234);
        TEST("BLPD 1234H, *+",      0xFCA0, 0x1234);
        TEST("BLPD 1234H, *, AR0",  0xFC88, 0x1234);
        TEST("BLPD 1234H, *-, AR1", 0xFC99, 0x1234);
        TEST("BLPD 1234H, *+, AR0", 0xFCA8, 0x1234);
    }

    TEST("DMOV 00H",     0x5600);
    TEST("DMOV 70H",     0x5670);
    TEST("DMOV *",       0x5680);
    TEST("DMOV *-",      0x5690);
    TEST("DMOV *+",      0x56A0);
    TEST("DMOV *, AR0",  0x5688);
    TEST("DMOV *-, AR1", 0x5699);
    TEST("DMOV *+, AR0", 0x56A8);

    TEST("FORT 0", 0xCE0E);
    TEST("FORT 1", 0xCE0F);

    if (is320C2x()) {
        TEST("RFSM", 0xCE36);
        TEST("SFSM", 0xCE37);
    }
    TEST("RTXM", 0xCE20);
    TEST("STXM", 0xCE21);
    TEST("RXF",  0xCE0C);
    TEST("SXF",  0xCE0D);
    if (is320C2x()) {
        TEST("CLRC XF",  0xCE0C);
        TEST("SETC XF",  0xCE0D);
    }

    TEST("TBLR 00H",     0x5800);
    TEST("TBLR 70H",     0x5870);
    TEST("TBLR *",       0x5880);
    TEST("TBLR *-",      0x5890);
    TEST("TBLR *+",      0x58A0);
    TEST("TBLR *, AR0",  0x5888);
    TEST("TBLR *-, AR1", 0x5899);
    TEST("TBLR *+, AR0", 0x58A8);

    TEST("TBLW 00H",     0x5900);
    TEST("TBLW 70H",     0x5970);
    TEST("TBLW *",       0x5980);
    TEST("TBLW *-",      0x5990);
    TEST("TBLW *+",      0x59A0);
    TEST("TBLW *, AR0",  0x5988);
    TEST("TBLW *-, AR1", 0x5999);
    TEST("TBLW *+, AR0", 0x59A8);

    TEST("IN 70H, PA0",     0x8070);
    TEST("IN 70H, PA15",    0x8F70);
    TEST("IN *, PA0, AR0",  0x8088);
    TEST("IN *, PA15, AR7", 0x8F8F);
    TEST("IN *, PA0",       0x8080);
    TEST("IN *-, PA1",      0x8190);
    TEST("IN *+, PA2",      0x82A0);
    TEST("IN *BR0-, PA0, AR7", 0x80CF);
    TEST("IN *, PA1",       0x8180);
    TEST("IN *-, PA2",      0x8290);
    TEST("IN *+, PA3",      0x83A0);
    TEST("IN *, PA0, AR0",  0x8088);
    TEST("IN *, PA4, AR1",  0x8489);
    TEST("IN *-, PA4, AR0", 0x8498);
    TEST("IN *-, PA0, AR1", 0x8099);
    TEST("IN *+, PA7, AR0", 0x87A8);
    TEST("IN *+, PA7, AR1", 0x87A9);

    TEST("OUT 70H, PA8",     0xE870);
    TEST("OUT 70H, PA15",    0xEF70);
    TEST("OUT *, PA8, AR0",  0xE888);
    TEST("OUT *, PA15, AR1", 0xEF89);
    TEST("OUT *, PA8",       0xE880);
    TEST("OUT *-, PA9",      0xE990);
    TEST("OUT *+, PA10",     0xEAA0);
    TEST("OUT *, PA9",       0xE980);
    TEST("OUT *-, PA10",      0xEA90);
    TEST("OUT *+, PA11",      0xEBA0);
    TEST("OUT *, PA8, AR0",   0xE888);
    TEST("OUT *, PA12, AR1",  0xEC89);
    TEST("OUT *-, PA12, AR0", 0xEC98);
    TEST("OUT *-, PA8, AR1",  0xE899);
    TEST("OUT *+, PA15, AR0", 0xEFA8);
    TEST("OUT *+, PA15, AR1", 0xEFA9);
}

void test_dataio_c5x() {
    TEST("BLDD #1234H, 00H",     0xA800, 0x1234);
    TEST("BLDD #1234H, 70H",     0xA870, 0x1234);
    TEST("BLDD #1234H, *",       0xA880, 0x1234);
    TEST("BLDD #1234H, *-",      0xA890, 0x1234);
    TEST("BLDD #1234H, *+",      0xA8A0, 0x1234);
    TEST("BLDD #1234H, *, AR0",  0xA888, 0x1234);
    TEST("BLDD #1234H, *-, AR1", 0xA899, 0x1234);
    TEST("BLDD #1234H, *+, AR0", 0xA8A8, 0x1234);

    TEST("BLDD 00H, #1234H",     0xA900, 0x1234);
    TEST("BLDD 70H, #1234H",     0xA970, 0x1234);
    TEST("BLDD *, #1234H",       0xA980, 0x1234);
    TEST("BLDD *-, #1234H",      0xA990, 0x1234);
    TEST("BLDD *+, #1234H",      0xA9A0, 0x1234);
    TEST("BLDD *, #1234H, AR0",  0xA988, 0x1234);
    TEST("BLDD *-, #1234H, AR1", 0xA999, 0x1234);
    TEST("BLDD *+, #1234H, AR0", 0xA9A8, 0x1234);

    if (is320C5x()) {
        TEST("BLDD BMAR, 00H",     0xAC00);
        TEST("BLDD BMAR, 70H",     0xAC70);
        TEST("BLDD BMAR, *",       0xAC80);
        TEST("BLDD BMAR, *-",      0xAC90);
        TEST("BLDD BMAR, *+",      0xACA0);
        TEST("BLDD BMAR, *, AR0",  0xAC88);
        TEST("BLDD BMAR, *-, AR1", 0xAC99);
        TEST("BLDD BMAR, *+, AR0", 0xACA8);

        TEST("BLDD 00H, BMAR",     0xAD00);
        TEST("BLDD 70H, BMAR",     0xAD70);
        TEST("BLDD *, BMAR",       0xAD80);
        TEST("BLDD *-, BMAR",      0xAD90);
        TEST("BLDD *+, BMAR",      0xADA0);
        TEST("BLDD *, BMAR, AR0",  0xAD88);
        TEST("BLDD *-, BMAR, AR1", 0xAD99);
        TEST("BLDD *+, BMAR, AR0", 0xADA8);

        TEST("BLDP 00H",     0x5700);
        TEST("BLDP 70H",     0x5770);
        TEST("BLDP *",       0x5780);
        TEST("BLDP *-",      0x5790);
        TEST("BLDP *+",      0x57A0);
        TEST("BLDP *, AR0",  0x5788);
        TEST("BLDP *-, AR1", 0x5799);
        TEST("BLDP *+, AR0", 0x57A8);
    }

    TEST("BLPD #1234H, 00H",     0xA500, 0x1234);
    TEST("BLPD #1234H, 70H",     0xA570, 0x1234);
    TEST("BLPD #1234H, *",       0xA580, 0x1234);
    TEST("BLPD #1234H, *-",      0xA590, 0x1234);
    TEST("BLPD #1234H, *+",      0xA5A0, 0x1234);
    TEST("BLPD #1234H, *, AR0",  0xA588, 0x1234);
    TEST("BLPD #1234H, *-, AR1", 0xA599, 0x1234);
    TEST("BLPD #1234H, *+, AR0", 0xA5A8, 0x1234);

    TEST("DMOV 00H",     0x7700);
    TEST("DMOV 70H",     0x7770);
    TEST("DMOV *",       0x7780);
    TEST("DMOV *-",      0x7790);
    TEST("DMOV *+",      0x77A0);
    TEST("DMOV *, AR0",  0x7788);
    TEST("DMOV *-, AR1", 0x7799);
    TEST("DMOV *+, AR0", 0x77A8);

    TEST("CLRC XF", 0xBE4C);
    TEST("SETC XF", 0xBE4D);

    TEST("TBLR 00H",     0xA600);
    TEST("TBLR 70H",     0xA670);
    TEST("TBLR *",       0xA680);
    TEST("TBLR *-",      0xA690);
    TEST("TBLR *+",      0xA6A0);
    TEST("TBLR *, AR0",  0xA688);
    TEST("TBLR *-, AR1", 0xA699);
    TEST("TBLR *+, AR0", 0xA6A8);

    TEST("TBLW 00H",     0xA700);
    TEST("TBLW 70H",     0xA770);
    TEST("TBLW *",       0xA780);
    TEST("TBLW *-",      0xA790);
    TEST("TBLW *+",      0xA7A0);
    TEST("TBLW *, AR0",  0xA788);
    TEST("TBLW *-, AR1", 0xA799);
    TEST("TBLW *+, AR0", 0xA7A8);

    TEST("IN 70H, 1234H",        0xAF70, 0x1234);
    TEST("IN *, 1234H",          0xAF80, 0x1234);
    TEST("IN *, 1234H, AR0",     0xAF88, 0x1234);
    TEST("IN *-, 1234H",         0xAF90, 0x1234);
    TEST("IN *-, 1234H, AR1",    0xAF99, 0x1234);
    TEST("IN *+, 1234H",         0xAFA0, 0x1234);
    TEST("IN *+, 1234H, AR2",    0xAFAA, 0x1234);
    TEST("IN *BR0-, 1234H",      0xAFC0, 0x1234);
    TEST("IN *BR0-, 1234H, AR3", 0xAFCB, 0x1234);
    TEST("IN *0-, 1234H",        0xAFD0, 0x1234);
    TEST("IN *0-, 1234H, AR4",   0xAFDC, 0x1234);
    TEST("IN *0+, 1234H",        0xAFE0, 0x1234);
    TEST("IN *0+, 1234H, AR5",   0xAFED, 0x1234);
    TEST("IN *BR0+, 1234H",      0xAFF0, 0x1234);
    TEST("IN *BR0+, 1234H, AR6", 0xAFFE, 0x1234);

    TEST("OUT 70H, 1234H",        0x0C70, 0x1234);
    TEST("OUT *, 1234H",          0x0C80, 0x1234);
    TEST("OUT *, 1234H, AR0",     0x0C88, 0x1234);
    TEST("OUT *-, 1234H",         0x0C90, 0x1234);
    TEST("OUT *-, 1234H, AR1",    0x0C99, 0x1234);
    TEST("OUT *+, 1234H",         0x0CA0, 0x1234);
    TEST("OUT *+, 1234H, AR2",    0x0CAA, 0x1234);
    TEST("OUT *BR0-, 1234H",      0x0CC0, 0x1234);
    TEST("OUT *BR0-, 1234H, AR3", 0x0CCB, 0x1234);
    TEST("OUT *0-, 1234H",        0x0CD0, 0x1234);
    TEST("OUT *0-, 1234H, AR4",   0x0CDC, 0x1234);
    TEST("OUT *0+, 1234H",        0x0CE0, 0x1234);
    TEST("OUT *0+, 1234H, AR5",   0x0CED, 0x1234);
    TEST("OUT *BR0+, 1234H",      0x0CF0, 0x1234);
    TEST("OUT *BR0+, 1234H, AR6", 0x0CFE, 0x1234);

    if (is320C5x()) {
        TEST("LMMR 00H, #1234H",     0x8900, 0x1234);
        TEST("LMMR 70H, #1234H",     0x8970, 0x1234);
        TEST("LMMR *, #1234H",       0x8980, 0x1234);
        TEST("LMMR *-, #1234H",      0x8990, 0x1234);
        TEST("LMMR *+, #1234H",      0x89A0, 0x1234);
        TEST("LMMR *, #1234H, AR0",  0x8988, 0x1234);
        TEST("LMMR *-, #1234H, AR1", 0x8999, 0x1234);
        TEST("LMMR *+, #1234H, AR0", 0x89A8, 0x1234);

        TEST("SMMR 00H, #1234H",     0x0900, 0x1234);
        TEST("SMMR 70H, #1234H",     0x0970, 0x1234);
        TEST("SMMR *, #1234H",       0x0980, 0x1234);
        TEST("SMMR *-, #1234H",      0x0990, 0x1234);
        TEST("SMMR *+, #1234H",      0x09A0, 0x1234);
        TEST("SMMR *, #1234H, AR0",  0x0988, 0x1234);
        TEST("SMMR *-, #1234H, AR1", 0x0999, 0x1234);
        TEST("SMMR *+, #1234H, AR0", 0x09A8, 0x1234);
    }
}

void test_parallel_c5x() {
    TEST("LPH 70H",     0x7570);
    TEST("LPH *",       0x7580);
    TEST("LPH *-",      0x7590);
    TEST("LPH *+",      0x75A0);
    TEST("LPH *, AR0",  0x7588);
    TEST("LPH *-, AR1", 0x7599);
    TEST("LPH *+, AR0", 0x75A8);

    TEST("LT 70H",     0x7370);
    TEST("LT *",       0x7380);
    TEST("LT *-",      0x7390);
    TEST("LT *+",      0x73A0);
    TEST("LT *, AR0",  0x7388);
    TEST("LT *-, AR1", 0x7399);
    TEST("LT *+, AR0", 0x73A8);

    TEST("SPLK #1234H, 00H",     0xAE00, 0x1234);
    TEST("SPLK #1234H, 70H",     0xAE70, 0x1234);
    TEST("SPLK #1234H, *",       0xAE80, 0x1234);
    TEST("SPLK #1234H, *-",      0xAE90, 0x1234);
    TEST("SPLK #1234H, *+",      0xAEA0, 0x1234);
    TEST("SPLK #1234H, *, AR0",  0xAE88, 0x1234);
    TEST("SPLK #1234H, *-, AR1", 0xAE99, 0x1234);
    TEST("SPLK #1234H, *+, AR0", 0xAEA8, 0x1234);

    if (is320C5x()) {
        TEST("APL 70H",        0x5A70);
        TEST("APL *",          0x5A80);
        TEST("APL *, AR7",     0x5A8F);
        TEST("APL *-",         0x5A90);
        TEST("APL *-, AR2",    0x5A9A);
        TEST("APL *+",         0x5AA0);
        TEST("APL *+, AR1",    0x5AA9);
        TEST("APL *BR0-",      0x5AC0);
        TEST("APL *BR0-, AR0", 0x5AC8);
        TEST("APL *0-",        0x5AD0);
        TEST("APL *0-, AR3",   0x5ADB);
        TEST("APL *0+",        0x5AE0);
        TEST("APL *0+, AR5",   0x5AED);
        TEST("APL *BR0+",      0x5AF0);
        TEST("APL *BR0+, AR7", 0x5AFF);

        TEST("APL #1234H, 70H",        0x5E70, 0x1234);
        TEST("APL #1234H, *",          0x5E80, 0x1234);
        TEST("APL #1234H, *, AR7",     0x5E8F, 0x1234);
        TEST("APL #1234H, *-",         0x5E90, 0x1234);
        TEST("APL #1234H, *-, AR2",    0x5E9A, 0x1234);
        TEST("APL #1234H, *+",         0x5EA0, 0x1234);
        TEST("APL #1234H, *+, AR1",    0x5EA9, 0x1234);
        TEST("APL #1234H, *BR0-",      0x5EC0, 0x1234);
        TEST("APL #1234H, *BR0-, AR0", 0x5EC8, 0x1234);
        TEST("APL #1234H, *0-",        0x5ED0, 0x1234);
        TEST("APL #1234H, *0-, AR3",   0x5EDB, 0x1234);
        TEST("APL #1234H, *0+",        0x5EE0, 0x1234);
        TEST("APL #1234H, *0+, AR5",   0x5EED, 0x1234);
        TEST("APL #1234H, *BR0+",      0x5EF0, 0x1234);
        TEST("APL #1234H, *BR0+, AR7", 0x5EFF, 0x1234);

        TEST("CPL 70H",        0x5B70);
        TEST("CPL *",          0x5B80);
        TEST("CPL *, AR7",     0x5B8F);
        TEST("CPL *-",         0x5B90);
        TEST("CPL *-, AR2",    0x5B9A);
        TEST("CPL *+",         0x5BA0);
        TEST("CPL *+, AR1",    0x5BA9);
        TEST("CPL *BR0-",      0x5BC0);
        TEST("CPL *BR0-, AR0", 0x5BC8);
        TEST("CPL *0-",        0x5BD0);
        TEST("CPL *0-, AR3",   0x5BDB);
        TEST("CPL *0+",        0x5BE0);
        TEST("CPL *0+, AR5",   0x5BED);
        TEST("CPL *BR0+",      0x5BF0);
        TEST("CPL *BR0+, AR7", 0x5BFF);

        TEST("CPL #1234H, 70H",        0x5F70, 0x1234);
        TEST("CPL #1234H, *",          0x5F80, 0x1234);
        TEST("CPL #1234H, *, AR7",     0x5F8F, 0x1234);
        TEST("CPL #1234H, *-",         0x5F90, 0x1234);
        TEST("CPL #1234H, *-, AR2",    0x5F9A, 0x1234);
        TEST("CPL #1234H, *+",         0x5FA0, 0x1234);
        TEST("CPL #1234H, *+, AR1",    0x5FA9, 0x1234);
        TEST("CPL #1234H, *BR0-",      0x5FC0, 0x1234);
        TEST("CPL #1234H, *BR0-, AR0", 0x5FC8, 0x1234);
        TEST("CPL #1234H, *0-",        0x5FD0, 0x1234);
        TEST("CPL #1234H, *0-, AR3",   0x5FDB, 0x1234);
        TEST("CPL #1234H, *0+",        0x5FE0, 0x1234);
        TEST("CPL #1234H, *0+, AR5",   0x5FED, 0x1234);
        TEST("CPL #1234H, *BR0+",      0x5FF0, 0x1234);
        TEST("CPL #1234H, *BR0+, AR7", 0x5FFF, 0x1234);

        TEST("OPL 70H",        0x5970);
        TEST("OPL *",          0x5980);
        TEST("OPL *, AR7",     0x598F);
        TEST("OPL *-",         0x5990);
        TEST("OPL *-, AR2",    0x599A);
        TEST("OPL *+",         0x59A0);
        TEST("OPL *+, AR1",    0x59A9);
        TEST("OPL *BR0-",      0x59C0);
        TEST("OPL *BR0-, AR0", 0x59C8);
        TEST("OPL *0-",        0x59D0);
        TEST("OPL *0-, AR3",   0x59DB);
        TEST("OPL *0+",        0x59E0);
        TEST("OPL *0+, AR5",   0x59ED);
        TEST("OPL *BR0+",      0x59F0);
        TEST("OPL *BR0+, AR7", 0x59FF);

        TEST("OPL #1234H, 70H",        0x5D70, 0x1234);
        TEST("OPL #1234H, *",          0x5D80, 0x1234);
        TEST("OPL #1234H, *, AR7",     0x5D8F, 0x1234);
        TEST("OPL #1234H, *-",         0x5D90, 0x1234);
        TEST("OPL #1234H, *-, AR2",    0x5D9A, 0x1234);
        TEST("OPL #1234H, *+",         0x5DA0, 0x1234);
        TEST("OPL #1234H, *+, AR1",    0x5DA9, 0x1234);
        TEST("OPL #1234H, *BR0-",      0x5DC0, 0x1234);
        TEST("OPL #1234H, *BR0-, AR0", 0x5DC8, 0x1234);
        TEST("OPL #1234H, *0-",        0x5DD0, 0x1234);
        TEST("OPL #1234H, *0-, AR3",   0x5DDB, 0x1234);
        TEST("OPL #1234H, *0+",        0x5DE0, 0x1234);
        TEST("OPL #1234H, *0+, AR5",   0x5DED, 0x1234);
        TEST("OPL #1234H, *BR0+",      0x5DF0, 0x1234);
        TEST("OPL #1234H, *BR0+, AR7", 0x5DFF, 0x1234);

        TEST("XPL 70H",        0x5870);
        TEST("XPL *",          0x5880);
        TEST("XPL *, AR7",     0x588F);
        TEST("XPL *-",         0x5890);
        TEST("XPL *-, AR2",    0x589A);
        TEST("XPL *+",         0x58A0);
        TEST("XPL *+, AR1",    0x58A9);
        TEST("XPL *BR0-",      0x58C0);
        TEST("XPL *BR0-, AR0", 0x58C8);
        TEST("XPL *0-",        0x58D0);
        TEST("XPL *0-, AR3",   0x58DB);
        TEST("XPL *0+",        0x58E0);
        TEST("XPL *0+, AR5",   0x58ED);
        TEST("XPL *BR0+",      0x58F0);
        TEST("XPL *BR0+, AR7", 0x58FF);

        TEST("XPL #1234H, 70H",        0x5C70, 0x1234);
        TEST("XPL #1234H, *",          0x5C80, 0x1234);
        TEST("XPL #1234H, *, AR7",     0x5C8F, 0x1234);
        TEST("XPL #1234H, *-",         0x5C90, 0x1234);
        TEST("XPL #1234H, *-, AR2",    0x5C9A, 0x1234);
        TEST("XPL #1234H, *+",         0x5CA0, 0x1234);
        TEST("XPL #1234H, *+, AR1",    0x5CA9, 0x1234);
        TEST("XPL #1234H, *BR0-",      0x5CC0, 0x1234);
        TEST("XPL #1234H, *BR0-, AR0", 0x5CC8, 0x1234);
        TEST("XPL #1234H, *0-",        0x5CD0, 0x1234);
        TEST("XPL #1234H, *0-, AR3",   0x5CDB, 0x1234);
        TEST("XPL #1234H, *0+",        0x5CE0, 0x1234);
        TEST("XPL #1234H, *0+, AR5",   0x5CED, 0x1234);
        TEST("XPL #1234H, *BR0+",      0x5CF0, 0x1234);
        TEST("XPL #1234H, *BR0+, AR7", 0x5CFF, 0x1234);
    }
}

void test_comment_1x() {
    COMM("ADDH 70H     ; comment", "; comment", 0x6070);
    COMM("ADDH *       ; comment", "; comment", 0x6088);
    COMM("ADDH *-      ; comment", "; comment", 0x6098);
    COMM("ADDH *+      ; comment", "; comment", 0x60A8);
    COMM("ADDH *, AR0  ; comment", "; comment", 0x6080);
    COMM("ADDH *-, AR1 ; comment", "; comment", 0x6091);
    COMM("ADDH *+, AR0 ; comment", "; comment", 0x60A0);
    COMM("ADDH *-, 1   ; comment", "; comment", 0x6091);
    COMM("ADDH *+, 0   ; comment", "; comment", 0x60A0);
    COMM(".byte -128, 255  ; comment", "; comment", 0x0080, 0x00FF);
    COMM(".word -128, 256  ; comment", "; comment", 0xFF80, 0x0100);
    COMM(".long 12345678H  ; comment", "; comment", 0x5678, 0x1234);
    COMM(R"(.string "TEXT" ; comment)", "; comment", 0x4554, 0x5458);
}

void test_undef_1x() {
    ERRT("ADDH *+, UNDEF",    UNDEFINED_SYMBOL, "UNDEF",        0x60A0);
    ERRT("LAC UNDEF",         UNDEFINED_SYMBOL, "UNDEF",        0x2000);
    ERRT("LAC UNDEF, 15",     UNDEFINED_SYMBOL, "UNDEF, 15",    0x2F00);
    ERRT("LAC 70H, UNDEF",    UNDEFINED_SYMBOL, "UNDEF",        0x2070);
    ERRT("LAC UNDEF, UNDEF",  UNDEFINED_SYMBOL, "UNDEF, UNDEF", 0x2000);
    ERRT("LAC *, UNDEF, AR1", UNDEFINED_SYMBOL, "UNDEF, AR1",   0x2081);
}

void test_undef_2x() {
    ERRT("ADDH *+, UNDEF",    UNDEFINED_SYMBOL, "UNDEF",        0x48A8);
    ERRT("LAC UNDEF",         UNDEFINED_SYMBOL, "UNDEF",        0x2000);
    ERRT("LAC UNDEF, 15",     UNDEFINED_SYMBOL, "UNDEF, 15",    0x2F00);
    ERRT("LAC 70H, UNDEF",    UNDEFINED_SYMBOL, "UNDEF",        0x2070);
    ERRT("LAC UNDEF, UNDEF",  UNDEFINED_SYMBOL, "UNDEF, UNDEF", 0x2000);
    ERRT("LAC *, UNDEF, AR7", UNDEFINED_SYMBOL, "UNDEF, AR7",   0x208F);
}

void test_undef_c5x() {
    ERRT("ADD  *+, UNDEF",     UNDEFINED_SYMBOL, "UNDEF",        0x20A0);
    ERRT("LACC UNDEF",         UNDEFINED_SYMBOL, "UNDEF",        0x1000);
    ERRT("LACC UNDEF, 15",     UNDEFINED_SYMBOL, "UNDEF, 15",    0x1F00);
    ERRT("LACC 70H, UNDEF",    UNDEFINED_SYMBOL, "UNDEF",        0x1070);
    ERRT("LACC UNDEF, UNDEF",  UNDEFINED_SYMBOL, "UNDEF, UNDEF", 0x1000);
    ERRT("LACC *, UNDEF, AR7", UNDEFINED_SYMBOL, "UNDEF, AR7",   0x108F);
}

void test_data_constant() {
    TEST(".byte -128, 255",   0x0080, 0x00FF);
    TEST(".byte 1234H",       0x0034);
    TEST(".word -128, 256",   0xFF80, 0x0100);
    TEST(".long 12345678H",   0x5678, 0x1234);
    TEST(R"(.byte '''','"')", 0x0027, 0x0022);
    TEST(R"(.byte "ABC")",    0x4241, 0x0043);
    TEST(R"(.word "ABC")",    0x4241, 0x0043);
    TEST(R"(.long "ABC")",    0x4241, 0x0043);
    TEST(R"(.string "A""B")", 0x2241, 0x0042);
    ERRT(R"(.string "A""B)",  MISSING_CLOSING_DQUOTE, R"("A""B)");
    ERRT(".byte 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2",
         0x0001, 0x0000, 0x0002);
    ERRT(".word 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2",
         0x0001, 0x0000, 0x0002);
    ERRT(".long 1, UNDEF, 2", UNDEFINED_SYMBOL, "UNDEF, 2",
          0x0001, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000);

    ERRT(R"(.string ")"
          "1234567890" "1234567890" "1234567890" "1234567890" "1234567890" "1234567890"
          R"(1234567890")",
          NO_MEMORY,
          R"(567890")",
          0x3231, 0x3433, 0x3635, 0x3837, 0x3039, 0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
          0x3231, 0x3433, 0x3635, 0x3837, 0x3039, 0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
          0x3231, 0x3433, 0x3635, 0x3837, 0x3039, 0x3231, 0x3433, 0x3635, 0x3837, 0x3039,
          0x3231, 0x3433);

    ERRT(".long "
         "12345678H, 9ABCDEF0H, 12345678H, 9ABCDEF0H, "
         "12345678H, 9ABCDEF0H, 12345678H, 9ABCDEF0H, "
         "12345678H, 9ABCDEF0H, 12345678H, 9ABCDEF0H, "
         "12345678H, 9ABCDEF0H, 12345678H,  0BCDEF0H, "
         "0AAAABBBBH",
         NO_MEMORY,
         "0AAAABBBBH",
         0x5678, 0x1234, 0xDEF0, 0x9ABC, 0x5678, 0x1234, 0xDEF0, 0x9ABC,
         0x5678, 0x1234, 0xDEF0, 0x9ABC, 0x5678, 0x1234, 0xDEF0, 0x9ABC,
         0x5678, 0x1234, 0xDEF0, 0x9ABC, 0x5678, 0x1234, 0xDEF0, 0x9ABC,
         0x5678, 0x1234, 0xDEF0, 0x9ABC, 0x5678, 0x1234, 0xDEF0, 0x00BC);
}
// clang-format on

void run_tests(const char *cpu) {
    assembler.setCpu(cpu);
    if (is3201x()) {
        RUN_TEST(test_accumrator_1x);
        RUN_TEST(test_auxiliary_1x);
        RUN_TEST(test_multiply_1x);
        RUN_TEST(test_branch_1x);
        RUN_TEST(test_control_1x);
        RUN_TEST(test_dataio_1x);
        RUN_TEST(test_comment_1x);
        RUN_TEST(test_undef_1x);
    }
    if (is3202x()) {
        RUN_TEST(test_accumrator_2x);
        RUN_TEST(test_auxiliary_2x);
        RUN_TEST(test_multiply_2x);
        RUN_TEST(test_branch_2x);
        RUN_TEST(test_control_2x);
        RUN_TEST(test_dataio_2x);
        RUN_TEST(test_undef_2x);
    }
    if (is320C20x() || is320C5x()) {
        RUN_TEST(test_accumrator_c5x);
        RUN_TEST(test_auxiliary_c5x);
        RUN_TEST(test_multiply_c5x);
        RUN_TEST(test_branch_c5x);
        RUN_TEST(test_control_c5x);
        RUN_TEST(test_dataio_c5x);
        RUN_TEST(test_undef_c5x);
        RUN_TEST(test_parallel_c5x);
    }
    RUN_TEST(test_data_constant);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
