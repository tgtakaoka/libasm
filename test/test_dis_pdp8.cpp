/*
 * Copyright 2023 Tadashi G. Takaoka
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

#include "dis_pdp8.h"
#include "test_dis_helper.h"

using namespace libasm;
using namespace libasm::pdp8;
using namespace libasm::test;

DisPdp8 dispdp8;
Disassembler &disassembler(dispdp8);

bool hd6120() {
    return strcmp_P("6120", disassembler.config().cpu_P()) == 0;
}

void set_up() {
    disassembler.reset();
}

void tear_down() {
    symtab.reset();
}

// clang-format off
void test_cpu() {
    EQUALS("cpu 6100", true,   disassembler.setCpu("6100"));
    EQUALS_P("cpu 6100", "6100", disassembler.config().cpu_P());

    EQUALS("cpu 6120", true,   disassembler.setCpu("6120"));
    EQUALS_P("cpu 6120", "6120", disassembler.config().cpu_P());

    EQUALS("cpu IM6100", true,   disassembler.setCpu("IM6100"));
    EQUALS_P("cpu IM6100", "6100", disassembler.config().cpu_P());

    EQUALS("cpu HD6120", true,   disassembler.setCpu("HD6120"));
    EQUALS_P("cpu HD6120", "6120", disassembler.config().cpu_P());
}

void test_memory_reference() {
    constexpr uint16_t IA = 0400;
    if (hd6120()) {
        ATEST(004000, "AND", "00022",   00000|0022);
        ATEST(004000, "AND", "I 00043", 00000|0043|IA);
        ATEST(004000, "TAD", "00064",   01000|0064);
        ATEST(004000, "TAD", "I 00105", 01000|0105|IA);
        ATEST(004000, "ISZ", "00170",   02000|0170);
        ATEST(004000, "ISZ", "I 00147", 02000|0147|IA);
        ATEST(004000, "DCA", "00170",   03000|0170);
        ATEST(004000, "DCA", "I 00011", 03000|0011|IA);
        ATEST(004000, "JMS", "00032",   04000|0032);
        ATEST(004000, "JMS", "I 00053", 04000|0053|IA);
        ATEST(004000, "JMP", "00074",   05000|0074);
        ATEST(004000, "JMP", "I 00115", 05000|0115|IA);
    } else {
        ATEST(04000, "AND", "0022",   00000|0022);
        ATEST(04000, "AND", "I 0043", 00000|0043|IA);
        ATEST(04000, "TAD", "0064",   01000|0064);
        ATEST(04000, "TAD", "I 0105", 01000|0105|IA);
        ATEST(04000, "ISZ", "0170",   02000|0170);
        ATEST(04000, "ISZ", "I 0147", 02000|0147|IA);
        ATEST(04000, "DCA", "0170",   03000|0170);
        ATEST(04000, "DCA", "I 0011", 03000|0011|IA);
        ATEST(04000, "JMS", "0032",   04000|0032);
        ATEST(04000, "JMS", "I 0053", 04000|0053|IA);
        ATEST(04000, "JMP", "0074",   05000|0074);
        ATEST(04000, "JMP", "I 0115", 05000|0115|IA);
    }

    constexpr uint16_t MP = 0200;
    if (hd6120()) {
        ATEST(074000, "AND", "74022",   00000|0022|MP);
        ATEST(074000, "AND", "I 74043", 00000|0043|MP|IA);
        ATEST(074000, "TAD", "74064",   01000|0064|MP);
        ATEST(074000, "TAD", "I 74105", 01000|0105|MP|IA);
        ATEST(074000, "ISZ", "74126",   02000|0126|MP);
        ATEST(074000, "ISZ", "I 74147", 02000|0147|MP|IA);
        ATEST(074000, "DCA", "74170",   03000|0170|MP);
        ATEST(074000, "DCA", "I 74011", 03000|0011|MP|IA);
        ATEST(074000, "JMS", "74032",   04000|0032|MP);
        ATEST(074000, "JMS", "I 74053", 04000|0053|MP|IA);
        ATEST(074000, "JMP", "74074",   05000|0074|MP);
        ATEST(074000, "JMP", "I 74115", 05000|0115|MP|IA);
    } else {
        ATEST(04000, "AND", "4022",   00000|0022|MP);
        ATEST(04000, "AND", "I 4043", 00000|0043|MP|IA);
        ATEST(04000, "TAD", "4064",   01000|0064|MP);
        ATEST(04000, "TAD", "I 4105", 01000|0105|MP|IA);
        ATEST(04000, "ISZ", "4126",   02000|0126|MP);
        ATEST(04000, "ISZ", "I 4147", 02000|0147|MP|IA);
        ATEST(04000, "DCA", "4170",   03000|0170|MP);
        ATEST(04000, "DCA", "I 4011", 03000|0011|MP|IA);
        ATEST(04000, "JMS", "4032",   04000|0032|MP);
        ATEST(04000, "JMS", "I 4053", 04000|0053|MP|IA);
        ATEST(04000, "JMP", "4074",   05000|0074|MP);
        ATEST(04000, "JMP", "I 4115", 05000|0115|MP|IA);
    }

    symtab.intern(00017, "auto017");
    symtab.intern(00020, "zero020");
    symtab.intern(04017, "label4017");
    symtab.intern(04020, "label4020");

    ATEST(04000, "AND", "zero020",     00000|0020);
    ATEST(04000, "AND", "I auto017",   00000|0017|IA);
    ATEST(04000, "AND", "I label4017", 00000|0017|MP|IA);
    ATEST(04000, "AND", "label4020",   00000|0020|MP);
}

void test_group1() {
    TEST("NOP", "",            07000);
    TEST("IAC", "",            07001);
    TEST("BSW", "",            07002);
    TEST("BSW", "IAC",         07003);
    TEST("RAL", "",            07004);
    TEST("RAL", "IAC",         07005);
    TEST("RTL", "",            07006);
    TEST("RTL", "IAC",         07007);
    TEST("RAR", "",            07010);
    TEST("RAR", "IAC",         07011);
    TEST("RTR", "",            07012);
    TEST("RTR", "IAC",         07013);
    if (hd6120()) {
        TEST("R3L", "",            07014);
        TEST("R3L", "IAC",         07015);
    } else {
        ERRT("",    "", UNKNOWN_INSTRUCTION, "", 07014);
        ERRT("IAC", "", INVALID_INSTRUCTION, "", 07015);
    }
    ERRT("",    "", UNKNOWN_INSTRUCTION, "", 07016);
    ERRT("IAC", "", INVALID_INSTRUCTION, "", 07017);
    TEST("CML", "",            07020);
    TEST("CML", "IAC",         07021);
    TEST("CML", "BSW",         07022);
    TEST("CML", "BSW IAC",     07023);
    TEST("CML", "RAL",         07024);
    TEST("CML", "RAL IAC",     07025);
    TEST("CML", "RTL",         07026);
    TEST("CML", "RTL IAC",     07027);
    TEST("CML", "RAR",         07030);
    TEST("CML", "RAR IAC",     07031);
    TEST("CML", "RTR",         07032);
    TEST("CML", "RTR IAC",     07033);
    if (hd6120()) {
        TEST("CML", "R3L",         07034);
        TEST("CML", "R3L IAC",     07035);
    } else {
        ERRT("CML", "",    INVALID_INSTRUCTION, "", 07034);
        ERRT("CML", "IAC", INVALID_INSTRUCTION, "", 07035);
    }
    ERRT("CML", "",    INVALID_INSTRUCTION, "", 07036);
    ERRT("CML", "IAC", INVALID_INSTRUCTION, "", 07037);
    TEST("CMA", "",            07040);
    TEST("CIA", "",            07041);
    TEST("CMA", "BSW",         07042);
    TEST("CIA", "BSW",         07043);
    TEST("CMA", "RAL",         07044);
    TEST("CIA", "RAL",         07045);
    TEST("CMA", "RTL",         07046);
    TEST("CIA", "RTL",         07047);
    TEST("CMA", "RAR",         07050);
    TEST("CIA", "RAR",         07051);
    TEST("CMA", "RTR",         07052);
    TEST("CIA", "RTR",         07053);
    if (hd6120()) {
        TEST("CMA", "R3L",         07054);
        TEST("CIA", "R3L",         07055);
    } else {
        ERRT("CMA", "", INVALID_INSTRUCTION, "", 07054);
        ERRT("CIA", "", INVALID_INSTRUCTION, "", 07055);
    }
    ERRT("CMA", "", INVALID_INSTRUCTION, "", 07056);
    ERRT("CIA", "", INVALID_INSTRUCTION, "", 07057);
    TEST("CMA", "CML",         07060);
    TEST("CIA", "CML",         07061);
    TEST("CMA", "CML BSW",     07062);
    TEST("CIA", "CML BSW",     07063);
    TEST("CMA", "CML RAL",     07064);
    TEST("CIA", "CML RAL",     07065);
    TEST("CMA", "CML RTL",     07066);
    TEST("CIA", "CML RTL",     07067);
    TEST("CMA", "CML RAR",     07070);
    TEST("CIA", "CML RAR",     07071);
    TEST("CMA", "CML RTR",     07072);
    TEST("CIA", "CML RTR",     07073);
    if (hd6120()) {
        TEST("CMA", "CML R3L",     07074);
        TEST("CIA", "CML R3L",     07075);
    } else {
        ERRT("CMA", "CML", INVALID_INSTRUCTION, "", 07074);
        ERRT("CIA", "CML", INVALID_INSTRUCTION, "", 07075);
    }
    ERRT("CMA", "CML", INVALID_INSTRUCTION, "", 07076);
    ERRT("CIA", "CML", INVALID_INSTRUCTION, "", 07077);
    TEST("CLL", "",            07100);
    TEST("CLL", "IAC",         07101);
    TEST("CLL", "BSW",         07102);
    TEST("CLL", "BSW IAC",     07103);
    TEST("CLL", "RAL",         07104);
    TEST("CLL", "RAL IAC",     07105);
    TEST("CLL", "RTL",         07106);
    TEST("CLL", "RTL IAC",     07107);
    TEST("CLL", "RAR",         07110);
    TEST("CLL", "RAR IAC",     07111);
    TEST("CLL", "RTR",         07112);
    TEST("CLL", "RTR IAC",     07113);
    if (hd6120()) {
        TEST("CLL", "R3L",         07114);
        TEST("CLL", "R3L IAC",     07115);
    } else {
        ERRT("CLL", "",    INVALID_INSTRUCTION, "", 07114);
        ERRT("CLL", "IAC", INVALID_INSTRUCTION, "", 07115);
    }
    ERRT("CLL", "",    INVALID_INSTRUCTION, "", 07116);
    ERRT("CLL", "IAC", INVALID_INSTRUCTION, "", 07117);
    TEST("STL", "",            07120);
    TEST("STL", "IAC",         07121);
    TEST("STL", "BSW",         07122);
    TEST("STL", "BSW IAC",     07123);
    TEST("STL", "RAL",         07124);
    TEST("STL", "RAL IAC",     07125);
    TEST("STL", "RTL",         07126);
    TEST("STL", "RTL IAC",     07127);
    TEST("STL", "RAR",         07130);
    TEST("STL", "RAR IAC",     07131);
    TEST("STL", "RTR",         07132);
    TEST("STL", "RTR IAC",     07133);
    if (hd6120()) {
        TEST("STL", "R3L",         07134);
        TEST("STL", "R3L IAC",     07135);
    } else {
        ERRT("STL", "",    INVALID_INSTRUCTION, "", 07134);
        ERRT("STL", "IAC", INVALID_INSTRUCTION, "", 07135);
    }
    ERRT("STL", "",    INVALID_INSTRUCTION, "", 07136);
    ERRT("STL", "IAC", INVALID_INSTRUCTION, "", 07137);
    TEST("CLL", "CMA",         07140);
    TEST("CLL", "CIA",         07141);
    TEST("CLL", "CMA BSW",     07142);
    TEST("CLL", "CIA BSW",     07143);
    TEST("CLL", "CMA RAL",     07144);
    TEST("CLL", "CIA RAL",     07145);
    TEST("CLL", "CMA RTL",     07146);
    TEST("CLL", "CIA RTL",     07147);
    TEST("CLL", "CMA RAR",     07150);
    TEST("CLL", "CIA RAR",     07151);
    TEST("CLL", "CMA RTR",     07152);
    TEST("CLL", "CIA RTR",     07153);
    if (hd6120()) {
        TEST("CLL", "CMA R3L",     07154);
        TEST("CLL", "CIA R3L",     07155);
    } else {
        ERRT("CLL", "CMA", INVALID_INSTRUCTION, "", 07154);
        ERRT("CLL", "CIA", INVALID_INSTRUCTION, "", 07155);
    }
    ERRT("CLL", "CMA", INVALID_INSTRUCTION, "", 07156);
    ERRT("CLL", "CIA", INVALID_INSTRUCTION, "", 07157);
    TEST("STL", "CMA",         07160);
    TEST("STL", "CIA",         07161);
    TEST("STL", "CMA BSW",     07162);
    TEST("STL", "CIA BSW",     07163);
    TEST("STL", "CMA RAL",     07164);
    TEST("STL", "CIA RAL",     07165);
    TEST("STL", "CMA RTL",     07166);
    TEST("STL", "CIA RTL",     07167);
    TEST("STL", "CMA RAR",     07170);
    TEST("STL", "CIA RAR",     07171);
    TEST("STL", "CMA RTR",     07172);
    TEST("STL", "CIA RTR",     07173);
    if (hd6120()) {
        TEST("STL", "CMA R3L",     07174);
        TEST("STL", "CIA R3L",     07175);
    } else {
        ERRT("STL", "CMA", INVALID_INSTRUCTION, "", 07174);
        ERRT("STL", "CIA", INVALID_INSTRUCTION, "", 07175);
    }
    ERRT("STL", "CMA", INVALID_INSTRUCTION, "", 07176);
    ERRT("STL", "CIA", INVALID_INSTRUCTION, "", 07177);
    TEST("CLA", "",            07200);
    TEST("CLA", "IAC",         07201);
    TEST("CLA", "BSW",         07202);
    TEST("CLA", "BSW IAC",     07203);
    TEST("GLK", "",            07204);
    TEST("GLK", "IAC",         07205);
    TEST("CLA", "RTL",         07206);
    TEST("CLA", "RTL IAC",     07207);
    TEST("CLA", "RAR",         07210);
    TEST("CLA", "RAR IAC",     07211);
    TEST("CLA", "RTR",         07212);
    TEST("CLA", "RTR IAC",     07213);
    if (hd6120()) {
        TEST("CLA", "R3L",         07214);
        TEST("CLA", "R3L IAC",     07215);
    } else {
        ERRT("CLA", "",    INVALID_INSTRUCTION, "", 07214);
        ERRT("CLA", "IAC", INVALID_INSTRUCTION, "", 07215);
    }
    ERRT("CLA", "",    INVALID_INSTRUCTION, "", 07216);
    ERRT("CLA", "IAC", INVALID_INSTRUCTION, "", 07217);
    TEST("CLA", "CML",         07220);
    TEST("CLA", "CML IAC",     07221);
    TEST("CLA", "CML BSW",     07222);
    TEST("CLA", "CML BSW IAC", 07223);
    TEST("GLK", "CML",         07224);
    TEST("GLK", "CML IAC",     07225);
    TEST("CLA", "CML RTL",     07226);
    TEST("CLA", "CML RTL IAC", 07227);
    TEST("CLA", "CML RAR",     07230);
    TEST("CLA", "CML RAR IAC", 07231);
    TEST("CLA", "CML RTR",     07232);
    TEST("CLA", "CML RTR IAC", 07233);
    if (hd6120()) {
        TEST("CLA", "CML R3L",     07234);
        TEST("CLA", "CML R3L IAC", 07235);
    } else {
        ERRT("CLA", "CML",     INVALID_INSTRUCTION, "", 07234);
        ERRT("CLA", "CML IAC", INVALID_INSTRUCTION, "", 07235);
    }
    ERRT("CLA", "CML",     INVALID_INSTRUCTION, "", 07236);
    ERRT("CLA", "CML IAC", INVALID_INSTRUCTION, "", 07237);
    TEST("STA", "",            07240);
    TEST("STA", "IAC",         07241);
    TEST("STA", "BSW",         07242);
    TEST("STA", "BSW IAC",     07243);
    TEST("GLK", "CMA",         07244);
    TEST("GLK", "CIA",         07245);
    TEST("STA", "RTL",         07246);
    TEST("STA", "RTL IAC",     07247);
    TEST("STA", "RAR",         07250);
    TEST("STA", "RAR IAC",     07251);
    TEST("STA", "RTR",         07252);
    TEST("STA", "RTR IAC",     07253);
    if (hd6120()) {
        TEST("STA", "R3L",         07254);
        TEST("STA", "R3L IAC",     07255);
    } else {
        ERRT("STA", "",    INVALID_INSTRUCTION, "", 07254);
        ERRT("STA", "IAC", INVALID_INSTRUCTION, "", 07255);
    }
    ERRT("STA", "",    INVALID_INSTRUCTION, "", 07256);
    ERRT("STA", "IAC", INVALID_INSTRUCTION, "", 07257);
    TEST("STA", "CML",         07260);
    TEST("STA", "CML IAC",     07261);
    TEST("STA", "CML BSW",     07262);
    TEST("STA", "CML BSW IAC", 07263);
    TEST("GLK", "CMA CML",     07264);
    TEST("GLK", "CIA CML",     07265);
    TEST("STA", "CML RTL",     07266);
    TEST("STA", "CML RTL IAC", 07267);
    TEST("STA", "CML RAR",     07270);
    TEST("STA", "CML RAR IAC", 07271);
    TEST("STA", "CML RTR",     07272);
    TEST("STA", "CML RTR IAC", 07273);
    if (hd6120()) {
        TEST("STA", "CML R3L",     07274);
        TEST("STA", "CML R3L IAC", 07275);
    } else {
        ERRT("STA", "CML",     INVALID_INSTRUCTION, "", 07274);
        ERRT("STA", "CML IAC", INVALID_INSTRUCTION, "", 07275);
    }
    ERRT("STA", "CML",     INVALID_INSTRUCTION, "", 07276);
    ERRT("STA", "CML IAC", INVALID_INSTRUCTION, "", 07277);
    TEST("CLA", "CLL",         07300);
    TEST("CLA", "CLL IAC",     07301);
    TEST("CLA", "CLL BSW",     07302);
    TEST("CLA", "CLL BSW IAC", 07303);
    TEST("GLK", "CLL",         07304);
    TEST("GLK", "CLL IAC",     07305);
    TEST("CLA", "CLL RTL",     07306);
    TEST("CLA", "CLL RTL IAC", 07307);
    TEST("CLA", "CLL RAR",     07310);
    TEST("CLA", "CLL RAR IAC", 07311);
    TEST("CLA", "CLL RTR",     07312);
    TEST("CLA", "CLL RTR IAC", 07313);
    if (hd6120()) {
        TEST("CLA", "CLL R3L",     07314);
        TEST("CLA", "CLL R3L IAC", 07315);
    } else {
        ERRT("CLA", "CLL",     INVALID_INSTRUCTION, "", 07314);
        ERRT("CLA", "CLL IAC", INVALID_INSTRUCTION, "", 07315);
    }
    ERRT("CLA", "CLL",     INVALID_INSTRUCTION, "", 07316);
    ERRT("CLA", "CLL IAC", INVALID_INSTRUCTION, "", 07317);
    TEST("STL", "CLA",         07320);
    TEST("STL", "CLA IAC",     07321);
    TEST("STL", "CLA BSW",     07322);
    TEST("STL", "CLA BSW IAC", 07323);
    TEST("GLK", "STL",         07324);
    TEST("GLK", "STL IAC",     07325);
    TEST("STL", "CLA RTL",     07326);
    TEST("STL", "CLA RTL IAC", 07327);
    TEST("STL", "CLA RAR",     07330);
    TEST("STL", "CLA RAR IAC", 07331);
    TEST("STL", "CLA RTR",     07332);
    TEST("STL", "CLA RTR IAC", 07333);
    if (hd6120()) {
        TEST("STL", "CLA R3L",     07334);
        TEST("STL", "CLA R3L IAC", 07335);
    } else {
        ERRT("STL", "CLA",     INVALID_INSTRUCTION, "", 07334);
        ERRT("STL", "CLA IAC", INVALID_INSTRUCTION, "", 07335);
    }
    ERRT("STL", "CLA",     INVALID_INSTRUCTION, "", 07336);
    ERRT("STL", "CLA IAC", INVALID_INSTRUCTION, "", 07337);
    TEST("STA", "CLL",         07340);
    TEST("STA", "CLL IAC",     07341);
    TEST("STA", "CLL BSW",     07342);
    TEST("STA", "CLL BSW IAC", 07343);
    TEST("GLK", "CLL CMA",     07344);
    TEST("GLK", "CLL CIA",     07345);
    TEST("STA", "CLL RTL",     07346);
    TEST("STA", "CLL RTL IAC", 07347);
    TEST("STA", "CLL RAR",     07350);
    TEST("STA", "CLL RAR IAC", 07351);
    TEST("STA", "CLL RTR",     07352);
    TEST("STA", "CLL RTR IAC", 07353);
    if (hd6120()) {
        TEST("STA", "CLL R3L",     07354);
        TEST("STA", "CLL R3L IAC", 07355);
    } else {
        ERRT("STA", "CLL",     INVALID_INSTRUCTION, "", 07354);
        ERRT("STA", "CLL IAC", INVALID_INSTRUCTION, "", 07355);
    }
    ERRT("STA", "CLL",     INVALID_INSTRUCTION, "", 07356);
    ERRT("STA", "CLL IAC", INVALID_INSTRUCTION, "", 07357);
    TEST("STL", "STA",         07360);
    TEST("STL", "STA IAC",     07361);
    TEST("STL", "STA BSW",     07362);
    TEST("STL", "STA BSW IAC", 07363);
    TEST("GLK", "STL CMA",     07364);
    TEST("GLK", "STL CIA",     07365);
    TEST("STL", "STA RTL",     07366);
    TEST("STL", "STA RTL IAC", 07367);
    TEST("STL", "STA RAR",     07370);
    TEST("STL", "STA RAR IAC", 07371);
    TEST("STL", "STA RTR",     07372);
    TEST("STL", "STA RTR IAC", 07373);
    if (hd6120()) {
        TEST("STL", "STA R3L",     07374);
        TEST("STL", "STA R3L IAC", 07375);
    } else {
        ERRT("STL", "STA",     INVALID_INSTRUCTION, "", 07374);
        ERRT("STL", "STA IAC", INVALID_INSTRUCTION, "", 07375);
    }
    ERRT("STL", "STA",     INVALID_INSTRUCTION, "", 07376);
    ERRT("STL", "STA IAC", INVALID_INSTRUCTION, "", 07377);
}

void test_group2() {
    TEST("NOP", "",            07400);
    TEST("SKP", "",            07410);
    TEST("SNL", "",            07420);
    TEST("SZL", "",            07430);
    TEST("SZA", "",            07440);
    TEST("SNA", "",            07450);
    TEST("SZA", "SNL",         07460);
    TEST("SNA", "SZL",         07470);
    TEST("SMA", "",            07500);
    TEST("SPA", "",            07510);
    TEST("SMA", "SNL",         07520);
    TEST("SPA", "SZL",         07530);
    TEST("SMA", "SZA",         07540);
    TEST("SPA", "SNA",         07550);
    TEST("SMA", "SZA SNL",     07560);
    TEST("SPA", "SNA SZL",     07570);
    TEST("CLA", "",            07600);
    TEST("SKP", "CLA",         07610);
    TEST("SNL", "CLA",         07620);
    TEST("SZL", "CLA",         07630);
    TEST("SZA", "CLA",         07640);
    TEST("SNA", "CLA",         07650);
    TEST("SZA", "SNL CLA",     07660);
    TEST("SNA", "SZL CLA",     07670);
    TEST("SMA", "CLA",         07700);
    TEST("SPA", "CLA",         07710);
    TEST("SMA", "SNL CLA",     07720);
    TEST("SPA", "SZL CLA",     07730);
    TEST("SMA", "SZA CLA",     07740);
    TEST("SPA", "SNA CLA",     07750);
    TEST("SMA", "SZA SNL CLA", 07760);
    TEST("SPA", "SNA SZL CLA", 07770);

    TEST("HLT", "",                07402);
    TEST("SKP", "HLT",             07412);
    TEST("SNL", "HLT",             07422);
    TEST("SZL", "HLT",             07432);
    TEST("SZA", "HLT",             07442);
    TEST("SNA", "HLT",             07452);
    TEST("SZA", "SNL HLT",         07462);
    TEST("SNA", "SZL HLT",         07472);
    TEST("SMA", "HLT",             07502);
    TEST("SPA", "HLT",             07512);
    TEST("SMA", "SNL HLT",         07522);
    TEST("SPA", "SZL HLT",         07532);
    TEST("SMA", "SZA HLT",         07542);
    TEST("SPA", "SNA HLT",         07552);
    TEST("SMA", "SZA SNL HLT",     07562);
    TEST("SPA", "SNA SZL HLT",     07572);
    TEST("CLA", "HLT",             07602);
    TEST("SKP", "CLA HLT",         07612);
    TEST("SNL", "CLA HLT",         07622);
    TEST("SZL", "CLA HLT",         07632);
    TEST("SZA", "CLA HLT",         07642);
    TEST("SNA", "CLA HLT",         07652);
    TEST("SZA", "SNL CLA HLT",     07662);
    TEST("SNA", "SZL CLA HLT",     07672);
    TEST("SMA", "CLA HLT",         07702);
    TEST("SPA", "CLA HLT",         07712);
    TEST("SMA", "SNL CLA HLT",     07722);
    TEST("SPA", "SZL CLA HLT",     07732);
    TEST("SMA", "SZA CLA HLT",     07742);
    TEST("SPA", "SNA CLA HLT",     07752);
    TEST("SMA", "SZA SNL CLA HLT", 07762);
    TEST("SPA", "SNA SZL CLA HLT", 07772);

    TEST("OSR", "",            07404);
    TEST("SKP", "OSR",         07414);
    TEST("SNL", "OSR",         07424);
    TEST("SZL", "OSR",         07434);
    TEST("SZA", "OSR",         07444);
    TEST("SNA", "OSR",         07454);
    TEST("SZA", "SNL OSR",     07464);
    TEST("SNA", "SZL OSR",     07474);
    TEST("SMA", "OSR",         07504);
    TEST("SPA", "OSR",         07514);
    TEST("SMA", "SNL OSR",     07524);
    TEST("SPA", "SZL OSR",     07534);
    TEST("SMA", "SZA OSR",     07544);
    TEST("SPA", "SNA OSR",     07554);
    TEST("SMA", "SZA SNL OSR", 07564);
    TEST("SPA", "SNA SZL OSR", 07574);
    TEST("LAS", "",            07604);
    TEST("SKP", "LAS",         07614);
    TEST("SNL", "LAS",         07624);
    TEST("SZL", "LAS",         07634);
    TEST("SZA", "LAS",         07644);
    TEST("SNA", "LAS",         07654);
    TEST("SZA", "SNL LAS",     07664);
    TEST("SNA", "SZL LAS",     07674);
    TEST("SMA", "LAS",         07704);
    TEST("SPA", "LAS",         07714);
    TEST("SMA", "SNL LAS",     07724);
    TEST("SPA", "SZL LAS",     07734);
    TEST("SMA", "SZA LAS",     07744);
    TEST("SPA", "SNA LAS",     07754);
    TEST("SMA", "SZA SNL LAS", 07764);
    TEST("SPA", "SNA SZL LAS", 07774);

    TEST("OSR", "HLT",             07406);
    TEST("SKP", "OSR HLT",         07416);
    TEST("SNL", "OSR HLT",         07426);
    TEST("SZL", "OSR HLT",         07436);
    TEST("SZA", "OSR HLT",         07446);
    TEST("SNA", "OSR HLT",         07456);
    TEST("SZA", "SNL OSR HLT",     07466);
    TEST("SNA", "SZL OSR HLT",     07476);
    TEST("SMA", "OSR HLT",         07506);
    TEST("SPA", "OSR HLT",         07516);
    TEST("SMA", "SNL OSR HLT",     07526);
    TEST("SPA", "SZL OSR HLT",     07536);
    TEST("SMA", "SZA OSR HLT",     07546);
    TEST("SPA", "SNA OSR HLT",     07556);
    TEST("SMA", "SZA SNL OSR HLT", 07566);
    TEST("SPA", "SNA SZL OSR HLT", 07576);
    TEST("LAS", "HLT",             07606);
    TEST("SKP", "LAS HLT",         07616);
    TEST("SNL", "LAS HLT",         07626);
    TEST("SZL", "LAS HLT",         07636);
    TEST("SZA", "LAS HLT",         07646);
    TEST("SNA", "LAS HLT",         07656);
    TEST("SZA", "SNL LAS HLT",     07666);
    TEST("SNA", "SZL LAS HLT",     07676);
    TEST("SMA", "LAS HLT",         07706);
    TEST("SPA", "LAS HLT",         07716);
    TEST("SMA", "SNL LAS HLT",     07726);
    TEST("SPA", "SZL LAS HLT",     07736);
    TEST("SMA", "SZA LAS HLT",     07746);
    TEST("SPA", "SNA LAS HLT",     07756);
    TEST("SMA", "SZA SNL LAS HLT", 07766);
    TEST("SPA", "SNA SZL LAS HLT", 07776);
}

void test_group3() {
    TEST("NOP", "",    07401);
    TEST("MQL", "",    07421);
    TEST("MQA", "",    07501);
    TEST("SWP", "",    07521);
    TEST("CLA", "",    07601);
    TEST("CAM", "",    07621);
    TEST("ACL", "",    07701);
    TEST("CLA", "SWP", 07721);
}

void test_stack_operation() {
    TEST("PPC1", "", 06205);
    TEST("PAC1", "", 06215);
    TEST("RTN1", "", 06225);
    TEST("POP1", "", 06235);
    TEST("RSP1", "", 06207);
    TEST("LSP1", "", 06217);

    TEST("PPC2", "", 06245);
    TEST("PAC2", "", 06255);
    TEST("RTN2", "", 06265);
    TEST("POP2", "", 06275);
    TEST("RSP2", "", 06227);
    TEST("LSP2", "", 06237);
}

void test_internal_control() {
    TEST("ION", "", 06001);
    TEST("IOF", "", 06002);
    TEST("RTF", "", 06005);
    TEST("SGT", "", 06006);
    TEST("CAF", "", 06007);
    if (hd6120()) {
        TEST("WSR", "", 06246);
        TEST("GCF", "", 06256);
    }
}

void test_main_memory() {
    TEST("SKON", "", 06000);
    TEST("SRQ",  "", 06003);
    TEST("GTF",  "", 06004);
    if (hd6120()) {
        TEST("PR0", "", 06206);
        TEST("PR1", "", 06216);
        TEST("PR2", "", 06226);
        TEST("PR3", "", 06236);
    }
}

void test_memory_extension() {
    TEST("CDF",     "00", 06201);
    TEST("CDF",     "70", 06271);
    TEST("CIF",     "00", 06202);
    TEST("CIF",     "70", 06272);
    TEST("CDF", "CIF 00", 06203);
    TEST("CDF", "CIF 70", 06273);

    TEST("RDF", "", 06214);
    TEST("RIF", "", 06224);
    TEST("RIB", "", 06234);
    TEST("RMF", "", 06244);
}

void test_io_transfer() {
    TEST("IOT", "01 0", 06010);
    TEST("IOT", "01 7", 06017);
    TEST("IOT", "77 0", 06770);
    TEST("IOT", "77 7", 06777);

    symtab.intern(077, "dev77");
    symtab.intern(1, "cmd1");
    symtab.intern(6, "cmd6");

    TEST("IOT", "dev77 cmd1", 06771);
    TEST("IOT", "dev77 cmd6", 06776);
}

void test_illegal() {
    // group 3
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07403);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07405);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07407);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07411);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07413);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07415);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07417);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07423);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07425);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07427);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07431);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07433);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07435);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07437);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07441);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07443);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07445);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07447);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07451);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07453);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07455);
    ERRT("NOP", "", INVALID_INSTRUCTION, "", 07457);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07461);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07463);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07465);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07467);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07471);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07473);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07475);
    ERRT("MQL", "", INVALID_INSTRUCTION, "", 07477);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07503);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07505);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07507);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07511);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07513);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07515);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07517);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07523);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07525);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07527);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07531);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07533);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07535);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07537);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07541);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07543);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07545);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07547);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07551);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07553);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07555);
    ERRT("MQA", "", INVALID_INSTRUCTION, "", 07557);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07561);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07563);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07565);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07567);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07571);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07573);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07575);
    ERRT("SWP", "", INVALID_INSTRUCTION, "", 07577);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07603);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07605);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07607);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07611);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07613);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07615);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07617);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07623);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07625);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07627);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07631);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07633);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07635);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07637);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07641);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07643);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07645);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07647);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07651);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07653);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07655);
    ERRT("CLA", "", INVALID_INSTRUCTION, "", 07657);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07661);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07663);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07665);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07667);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07671);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07673);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07675);
    ERRT("CAM", "", INVALID_INSTRUCTION, "", 07677);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07703);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07705);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07707);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07711);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07713);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07715);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07717);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07723);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07725);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07727);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07731);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07733);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07735);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07737);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07741);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07743);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07745);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07747);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07751);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07753);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07755);
    ERRT("ACL", "", INVALID_INSTRUCTION, "", 07757);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07761);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07763);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07765);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07767);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07771);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07773);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07775);
    ERRT("CLA", "SWP", INVALID_INSTRUCTION, "", 07777);
}

// clang-format on

void run_tests(const char *cpu) {
    disassembler.setCpu(cpu);
    RUN_TEST(test_memory_reference);
    RUN_TEST(test_group1);
    RUN_TEST(test_group2);
    RUN_TEST(test_group3);
    if (hd6120())
        RUN_TEST(test_stack_operation);
    RUN_TEST(test_internal_control);
    RUN_TEST(test_main_memory);
    if (hd6120())
        RUN_TEST(test_memory_extension);
    RUN_TEST(test_io_transfer);
    RUN_TEST(test_illegal);
}

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
