
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

#include "table_tms320.h"
#include "entry_table.h"
#include "entry_tms320.h"
#include "text_tms320.h"

using namespace libasm::text::tms320;

namespace libasm {
namespace tms320 {

#define E3(_opc, _cf, _name, _opr1, _opr2, _opr3) \
    {_opc, Entry::Flags::create(_cf, _opr1, _opr2, _opr3), _name}
#define E2(_opc, _cf, _name, _opr1, _opr2) E3(_opc, _cf, _name, _opr1, _opr2, M_NONE)
#define E1(_opc, _cf, _name, _opr1) E2(_opc, _cf, _name, _opr1, M_NONE)
#define E0(_opc, _cf, _name) E1(_opc, _cf, _name, M_NONE)

// clang-format off
constexpr Entry TABLE_TMS32010[] PROGMEM = {
    E3(0x0000, CF_0FFF, TEXT_ADD,  M_MAM,  M_LS4,  M_NARP),
    E3(0x1000, CF_0FFF, TEXT_SUB,  M_MAM,  M_LS4,  M_NARP),
    E3(0x2000, CF_0FFF, TEXT_LAC,  M_MAM,  M_LS4,  M_NARP),
    E3(0x3000, CF_01FF, TEXT_SAR,  M_AR,   M_MAM,  M_NARP),
    E3(0x3800, CF_01FF, TEXT_LAR,  M_AR,   M_MAM,  M_NARP),
    E3(0x4000, CF_07FF, TEXT_IN,   M_MAM,  M_PA,   M_NARP),
    E3(0x4800, CF_07FF, TEXT_OUT,  M_MAM,  M_PA,   M_NARP),
    E3(0x5000, CF_00FF, TEXT_SACL, M_MAM,  M_LS0,  M_NARP),
    E3(0x5800, CF_07FF, TEXT_SACH, M_MAM,  M_LS3,  M_NARP),
    E2(0x6000, CF_00FF, TEXT_ADDH, M_MAM,  M_NARP),
    E2(0x6100, CF_00FF, TEXT_ADDS, M_MAM,  M_NARP),
    E2(0x6200, CF_00FF, TEXT_SUBH, M_MAM,  M_NARP),
    E2(0x6300, CF_00FF, TEXT_SUBS, M_MAM,  M_NARP),
    E2(0x6400, CF_00FF, TEXT_SUBC, M_MAM,  M_NARP),
    E2(0x6500, CF_00FF, TEXT_ZALH, M_MAM,  M_NARP),
    E2(0x6600, CF_00FF, TEXT_ZALS, M_MAM,  M_NARP),
    E2(0x6700, CF_00FF, TEXT_TBLR, M_MAM,  M_NARP),
    E1(0x6880, CF_0001, TEXT_LARP, M_ARK),
    E2(0x6800, CF_00FF, TEXT_MAR,  M_MAM,  M_NARP),
    E2(0x6900, CF_00FF, TEXT_DMOV, M_MAM,  M_NARP),
    E2(0x6A00, CF_00FF, TEXT_LT,   M_MAM,  M_NARP),
    E2(0x6B00, CF_00FF, TEXT_LTD,  M_MAM,  M_NARP),
    E2(0x6C00, CF_00FF, TEXT_LTA,  M_MAM,  M_NARP),
    E2(0x6D00, CF_00FF, TEXT_MPY,  M_MAM,  M_NARP),
    E1(0x6E00, CF_0001, TEXT_LDPK, M_UI1),
    E2(0x6F00, CF_00FF, TEXT_LDP,  M_MAM,  M_NARP),
    E2(0x7000, CF_01FF, TEXT_LARK, M_AR,   M_UI8),
    E2(0x7800, CF_00FF, TEXT_XOR,  M_MAM,  M_NARP),
    E2(0x7900, CF_00FF, TEXT_AND,  M_MAM,  M_NARP),
    E2(0x7A00, CF_00FF, TEXT_OR,   M_MAM,  M_NARP),
    E2(0x7B00, CF_00FF, TEXT_LST,  M_MAM,  M_NARP),
    E2(0x7C00, CF_00FF, TEXT_SST,  M_MAM,  M_NARP),
    E2(0x7D00, CF_00FF, TEXT_TBLW, M_MAM,  M_NARP),
    E1(0x7E00, CF_00FF, TEXT_LACK, M_UI8),
    E0(0x7F80, CF_0000, TEXT_NOP),
    E0(0x7F81, CF_0000, TEXT_DINT),
    E0(0x7F82, CF_0000, TEXT_EINT),
    E0(0x7F88, CF_0000, TEXT_ABS),
    E0(0x7F89, CF_0000, TEXT_ZAC),
    E0(0x7F8A, CF_0000, TEXT_ROVM),
    E0(0x7F8B, CF_0000, TEXT_SOVM),
    E0(0x7F8C, CF_0000, TEXT_CALA),
    E0(0x7F8D, CF_0000, TEXT_RET),
    E0(0x7F8E, CF_0000, TEXT_PAC),
    E0(0x7F8F, CF_0000, TEXT_APAC),
    E0(0x7F90, CF_0000, TEXT_SPAC),
    E0(0x7F9C, CF_0000, TEXT_PUSH),
    E0(0x7F9D, CF_0000, TEXT_POP),
    E1(0x8000, CF_1FFF, TEXT_MPYK, M_SI13),
    E1(0xF400, CF_0000, TEXT_BANZ, M_PM12),
    E1(0xF500, CF_0000, TEXT_BV,   M_PM12),
    E1(0xF600, CF_0000, TEXT_BIOZ, M_PM12),
    E1(0xF800, CF_0000, TEXT_CALL, M_PM12),
    E1(0xF900, CF_0000, TEXT_B,    M_PM12),
    E1(0xFA00, CF_0000, TEXT_BLZ,  M_PM12),
    E1(0xFB00, CF_0000, TEXT_BLEZ, M_PM12),
    E1(0xFC00, CF_0000, TEXT_BGZ,  M_PM12),
    E1(0xFD00, CF_0000, TEXT_BGEZ, M_PM12),
    E1(0xFE00, CF_0000, TEXT_BNZ,  M_PM12),
    E1(0xFF00, CF_0000, TEXT_BZ,   M_PM12),
};

constexpr uint8_t INDEX_TMS32010[] PROGMEM = {
     37,  // TEXT_ABS
      0,  // TEXT_ADD
      9,  // TEXT_ADDH
     10,  // TEXT_ADDS
     28,  // TEXT_AND
     44,  // TEXT_APAC
     53,  // TEXT_B
     49,  // TEXT_BANZ
     57,  // TEXT_BGEZ
     56,  // TEXT_BGZ
     51,  // TEXT_BIOZ
     55,  // TEXT_BLEZ
     54,  // TEXT_BLZ
     58,  // TEXT_BNZ
     50,  // TEXT_BV
     59,  // TEXT_BZ
     41,  // TEXT_CALA
     52,  // TEXT_CALL
     35,  // TEXT_DINT
     19,  // TEXT_DMOV
     36,  // TEXT_EINT
      5,  // TEXT_IN
      2,  // TEXT_LAC
     33,  // TEXT_LACK
      4,  // TEXT_LAR
     26,  // TEXT_LARK
     17,  // TEXT_LARP
     25,  // TEXT_LDP
     24,  // TEXT_LDPK
     30,  // TEXT_LST
     20,  // TEXT_LT
     22,  // TEXT_LTA
     21,  // TEXT_LTD
     18,  // TEXT_MAR
     23,  // TEXT_MPY
     48,  // TEXT_MPYK
     34,  // TEXT_NOP
     29,  // TEXT_OR
      6,  // TEXT_OUT
     43,  // TEXT_PAC
     47,  // TEXT_POP
     46,  // TEXT_PUSH
     42,  // TEXT_RET
     39,  // TEXT_ROVM
      8,  // TEXT_SACH
      7,  // TEXT_SACL
      3,  // TEXT_SAR
     40,  // TEXT_SOVM
     45,  // TEXT_SPAC
     31,  // TEXT_SST
      1,  // TEXT_SUB
     13,  // TEXT_SUBC
     11,  // TEXT_SUBH
     12,  // TEXT_SUBS
     16,  // TEXT_TBLR
     32,  // TEXT_TBLW
     27,  // TEXT_XOR
     38,  // TEXT_ZAC
     14,  // TEXT_ZALH
     15,  // TEXT_ZALS
};

constexpr Entry TABLE_TMS3202X[] PROGMEM = {
    E3(0x0000, CF_0FFF, TEXT_ADD,  M_MAM,  M_LS4,  M_NARP),
    E3(0x1000, CF_0FFF, TEXT_SUB,  M_MAM,  M_LS4,  M_NARP),
    E3(0x2000, CF_0FFF, TEXT_LAC,  M_MAM,  M_LS4,  M_NARP),
    E3(0x3000, CF_07FF, TEXT_LAR,  M_AR,   M_MAM,  M_NARP),
    E2(0x3800, CF_00FF, TEXT_MPY,  M_MAM,  M_NARP),
    E2(0x3900, CF_00FF, TEXT_SQRA, M_MAM,  M_NARP),
    E2(0x3C00, CF_00FF, TEXT_LT,   M_MAM,  M_NARP),
    E2(0x3D00, CF_00FF, TEXT_LTA,  M_MAM,  M_NARP),
    E2(0x3E00, CF_00FF, TEXT_LTP,  M_MAM,  M_NARP),
    E2(0x3F00, CF_00FF, TEXT_LTD,  M_MAM,  M_NARP),
    E2(0x4000, CF_00FF, TEXT_ZALH, M_MAM,  M_NARP),
    E2(0x4100, CF_00FF, TEXT_ZALS, M_MAM,  M_NARP),
    E2(0x4200, CF_00FF, TEXT_LACT, M_MAM,  M_NARP),
    E2(0x4400, CF_00FF, TEXT_SUBH, M_MAM,  M_NARP),
    E2(0x4500, CF_00FF, TEXT_SUBS, M_MAM,  M_NARP),
    E2(0x4600, CF_00FF, TEXT_SUBT, M_MAM,  M_NARP),
    E2(0x4700, CF_00FF, TEXT_SUBC, M_MAM,  M_NARP),
    E2(0x4800, CF_00FF, TEXT_ADDH, M_MAM,  M_NARP),
    E2(0x4900, CF_00FF, TEXT_ADDS, M_MAM,  M_NARP),
    E2(0x4A00, CF_00FF, TEXT_ADDT, M_MAM,  M_NARP),
    E2(0x4B00, CF_00FF, TEXT_RPT,  M_MAM,  M_NARP),
    E2(0x4C00, CF_00FF, TEXT_XOR,  M_MAM,  M_NARP),
    E2(0x4D00, CF_00FF, TEXT_OR,   M_MAM,  M_NARP),
    E2(0x4E00, CF_00FF, TEXT_AND,  M_MAM,  M_NARP),
    E2(0x5000, CF_00FF, TEXT_LST,  M_MAM,  M_NARP),
    E2(0x5100, CF_00FF, TEXT_LST1, M_MAM,  M_NARP),
    E2(0x5200, CF_00FF, TEXT_LDP,  M_MAM,  M_NARP),
    E2(0x5300, CF_00FF, TEXT_LPH,  M_MAM,  M_NARP),
    E2(0x5400, CF_00FF, TEXT_PSHD, M_MAM,  M_NARP),
    E0(0x5500, CF_0000, TEXT_NOP),
    E1(0x5588, CF_0007, TEXT_LARP, M_ARK),
    E2(0x5500, CF_00FF, TEXT_MAR,  M_MAM,  M_NARP),
    E2(0x5600, CF_00FF, TEXT_DMOV, M_MAM,  M_NARP),
    E2(0x5700, CF_00FF, TEXT_BITT, M_MAM,  M_NARP),
    E2(0x5800, CF_00FF, TEXT_TBLR, M_MAM,  M_NARP),
    E2(0x5900, CF_00FF, TEXT_TBLW, M_MAM,  M_NARP),
    E2(0x5A00, CF_00FF, TEXT_SQRS, M_MAM,  M_NARP),
    E2(0x5B00, CF_00FF, TEXT_LTS,  M_MAM,  M_NARP),
    E3(0x5C00, CF_00FF, TEXT_MACD, M_PM16, M_MAM,  M_NARP),
    E3(0x5D00, CF_00FF, TEXT_MAC,  M_PM16, M_MAM,  M_NARP),
    E3(0x6000, CF_07FF, TEXT_SACL, M_MAM,  M_LS3,  M_NARP),
    E3(0x6800, CF_07FF, TEXT_SACH, M_MAM,  M_LS3,  M_NARP),
    E3(0x7000, CF_07FF, TEXT_SAR,  M_AR,   M_MAM,  M_NARP),
    E2(0x7800, CF_00FF, TEXT_SST,  M_MAM,  M_NARP),
    E2(0x7900, CF_00FF, TEXT_SST1, M_MAM,  M_NARP),
    E2(0x7A00, CF_00FF, TEXT_POPD, M_MAM,  M_NARP),
    E3(0x8000, CF_0FFF, TEXT_IN,   M_MAM,  M_PA,   M_NARP),
    E3(0x9000, CF_0FFF, TEXT_BIT,  M_MAM,  M_BIT,  M_NARP),
    E1(0xA000, CF_1FFF, TEXT_MPYK, M_SI13),
    E2(0xC000, CF_07FF, TEXT_LARK, M_AR,   M_UI8),
    E1(0xC800, CF_01FF, TEXT_LDPK, M_UI9),
    E0(0xCA00, CF_0000, TEXT_ZAC),
    E1(0xCA00, CF_00FF, TEXT_LACK, M_UI8),
    E1(0xCB00, CF_00FF, TEXT_RPTK, M_UI8),
    E0(0xCE00, CF_0000, TEXT_EINT),
    E0(0xCE01, CF_0000, TEXT_DINT),
    E0(0xCE02, CF_0000, TEXT_ROVM),
    E0(0xCE03, CF_0000, TEXT_SOVM),
    E0(0xCE06, CF_0000, TEXT_RSXM),
    E0(0xCE07, CF_0000, TEXT_SSXM),
    E1(0xCE08, CF_0003, TEXT_SPM,  M_UI2),
    E0(0xCE0C, CF_0000, TEXT_RXF),
    E0(0xCE0D, CF_0000, TEXT_SXF),
    E1(0xCE0E, CF_0001, TEXT_FORT, M_UI1),
    E0(0xCE14, CF_0000, TEXT_PAC),
    E0(0xCE15, CF_0000, TEXT_APAC),
    E0(0xCE16, CF_0000, TEXT_SPAC),
    E0(0xCE18, CF_0000, TEXT_SFL),
    E0(0xCE19, CF_0000, TEXT_SFR),
    E0(0xCE1B, CF_0000, TEXT_ABS),
    E0(0xCE1C, CF_0000, TEXT_PUSH),
    E0(0xCE1D, CF_0000, TEXT_POP),
    E0(0xCE1E, CF_0000, TEXT_TRAP),
    E0(0xCE1F, CF_0000, TEXT_IDLE),
    E0(0xCE20, CF_0000, TEXT_RTXM),
    E0(0xCE21, CF_0000, TEXT_STXM),
    E0(0xCE23, CF_0000, TEXT_NEG),
    E0(0xCE24, CF_0000, TEXT_CALA),
    E0(0xCE25, CF_0000, TEXT_BACC),
    E0(0xCE26, CF_0000, TEXT_RET),
    E0(0xCE27, CF_0000, TEXT_CMPL),
    E1(0xCE50, CF_0003, TEXT_CMPR, M_UI2),
    E2(0xD000, CF_0700, TEXT_LRLK, M_AR,   M_PM16),
    E2(0xD001, CF_0F00, TEXT_LALK, M_SI16, M_LS4),
    E2(0xD002, CF_0F00, TEXT_ADLK, M_SI16, M_LS4),
    E2(0xD003, CF_0F00, TEXT_SBLK, M_SI16, M_LS4),
    E2(0xD004, CF_0F00, TEXT_ANDK, M_UI16, M_LS4),
    E2(0xD005, CF_0F00, TEXT_ORK,  M_UI16, M_LS4),
    E2(0xD006, CF_0F00, TEXT_XORK, M_UI16, M_LS4),
    E3(0xE000, CF_0FFF, TEXT_OUT,  M_MAM,  M_PA,   M_NARP),
    E3(0xF080, CF_007F, TEXT_BV,   M_PM16, M_MAR,  M_NARP),
    E3(0xF180, CF_007F, TEXT_BGZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF280, CF_007F, TEXT_BLEZ, M_PM16, M_MAR,  M_NARP),
    E3(0xF380, CF_007F, TEXT_BLZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF480, CF_007F, TEXT_BGEZ, M_PM16, M_MAR,  M_NARP),
    E3(0xF580, CF_007F, TEXT_BNZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF680, CF_007F, TEXT_BZ,   M_PM16, M_MAR,  M_NARP),
    E3(0xF780, CF_007F, TEXT_BNV,  M_PM16, M_MAR,  M_NARP),
    E3(0xF880, CF_007F, TEXT_BBZ,  M_PM16, M_MAR,  M_NARP),
    E3(0xF980, CF_007F, TEXT_BBNZ, M_PM16, M_MAR,  M_NARP),
    E3(0xFA80, CF_007F, TEXT_BIOZ, M_PM16, M_MAR,  M_NARP),
    E3(0xFB80, CF_007F, TEXT_BANZ, M_PM16, M_MAR,  M_NARP),
    E3(0xFC00, CF_00FF, TEXT_BLKP, M_PM16, M_MAM,  M_NARP),
    E3(0xFD00, CF_00FF, TEXT_BLKD, M_PM16, M_MAM,  M_NARP),
    E3(0xFE80, CF_007F, TEXT_CALL, M_PM16, M_MAR,  M_NARP),
    E3(0xFF80, CF_007F, TEXT_B,    M_PM16, M_MAR,  M_NARP),
};

constexpr uint8_t INDEX_TMS3202X[] PROGMEM = {
     69,  // TEXT_ABS
      0,  // TEXT_ADD
     17,  // TEXT_ADDH
     18,  // TEXT_ADDS
     19,  // TEXT_ADDT
     84,  // TEXT_ADLK
     23,  // TEXT_AND
     86,  // TEXT_ANDK
     65,  // TEXT_APAC
    105,  // TEXT_B
     78,  // TEXT_BACC
    101,  // TEXT_BANZ
     99,  // TEXT_BBNZ
     98,  // TEXT_BBZ
     94,  // TEXT_BGEZ
     91,  // TEXT_BGZ
    100,  // TEXT_BIOZ
     47,  // TEXT_BIT
     33,  // TEXT_BITT
     92,  // TEXT_BLEZ
    103,  // TEXT_BLKD
    102,  // TEXT_BLKP
     93,  // TEXT_BLZ
     97,  // TEXT_BNV
     95,  // TEXT_BNZ
     90,  // TEXT_BV
     96,  // TEXT_BZ
     77,  // TEXT_CALA
    104,  // TEXT_CALL
     80,  // TEXT_CMPL
     81,  // TEXT_CMPR
     55,  // TEXT_DINT
     32,  // TEXT_DMOV
     54,  // TEXT_EINT
     63,  // TEXT_FORT
     73,  // TEXT_IDLE
     46,  // TEXT_IN
      2,  // TEXT_LAC
     52,  // TEXT_LACK
     12,  // TEXT_LACT
     83,  // TEXT_LALK
      3,  // TEXT_LAR
     49,  // TEXT_LARK
     30,  // TEXT_LARP
     26,  // TEXT_LDP
     50,  // TEXT_LDPK
     27,  // TEXT_LPH
     82,  // TEXT_LRLK
     24,  // TEXT_LST
     25,  // TEXT_LST1
      6,  // TEXT_LT
      7,  // TEXT_LTA
      9,  // TEXT_LTD
      8,  // TEXT_LTP
     37,  // TEXT_LTS
     39,  // TEXT_MAC
     38,  // TEXT_MACD
     31,  // TEXT_MAR
      4,  // TEXT_MPY
     48,  // TEXT_MPYK
     76,  // TEXT_NEG
     29,  // TEXT_NOP
     22,  // TEXT_OR
     87,  // TEXT_ORK
     89,  // TEXT_OUT
     64,  // TEXT_PAC
     71,  // TEXT_POP
     45,  // TEXT_POPD
     28,  // TEXT_PSHD
     70,  // TEXT_PUSH
     79,  // TEXT_RET
     56,  // TEXT_ROVM
     20,  // TEXT_RPT
     53,  // TEXT_RPTK
     58,  // TEXT_RSXM
     74,  // TEXT_RTXM
     61,  // TEXT_RXF
     41,  // TEXT_SACH
     40,  // TEXT_SACL
     42,  // TEXT_SAR
     85,  // TEXT_SBLK
     67,  // TEXT_SFL
     68,  // TEXT_SFR
     57,  // TEXT_SOVM
     66,  // TEXT_SPAC
     60,  // TEXT_SPM
      5,  // TEXT_SQRA
     36,  // TEXT_SQRS
     43,  // TEXT_SST
     44,  // TEXT_SST1
     59,  // TEXT_SSXM
     75,  // TEXT_STXM
      1,  // TEXT_SUB
     16,  // TEXT_SUBC
     13,  // TEXT_SUBH
     14,  // TEXT_SUBS
     15,  // TEXT_SUBT
     62,  // TEXT_SXF
     34,  // TEXT_TBLR
     35,  // TEXT_TBLW
     72,  // TEXT_TRAP
     21,  // TEXT_XOR
     88,  // TEXT_XORK
     51,  // TEXT_ZAC
     10,  // TEXT_ZALH
     11,  // TEXT_ZALS
};

constexpr Entry TABLE_TMS32020[] PROGMEM = {
    E0(0xCE82, CF_0000, TEXT_NORM),
};

constexpr uint8_t INDEX_TMS32020[] PROGMEM = {
      0,  // TEXT_NORM
};

constexpr Entry TABLE_TMS320C2X[] PROGMEM = {
    E2(0x3A00, CF_00FF, TEXT_MPYA, M_MAM,  M_NARP),
    E2(0x3B00, CF_00FF, TEXT_MPYS, M_MAM,  M_NARP),
    E2(0x4300, CF_00FF, TEXT_ADDC, M_MAM,  M_NARP),
    E2(0x4F00, CF_00FF, TEXT_SUBB, M_MAM,  M_NARP),
    E3(0x5E80, CF_0000, TEXT_BC,   M_PM16, M_MAR, M_NARP),
    E3(0x5F80, CF_0000, TEXT_BNC,  M_PM16, M_MAR, M_NARP),
    E2(0x7B00, CF_00FF, TEXT_ZALR, M_MAM,  M_NARP),
    E2(0x7C00, CF_00FF, TEXT_SPL,  M_MAM,  M_NARP),
    E2(0x7D00, CF_00FF, TEXT_SPH,  M_MAM,  M_NARP),
    E1(0x7E00, CF_00FF, TEXT_ADRK, M_UI8),
    E1(0x7F00, CF_00FF, TEXT_SBRK, M_UI8),
    E1(0xCC00, CF_00FF, TEXT_ADDK, M_UI8),
    E1(0xCD00, CF_00FF, TEXT_SUBK, M_UI8),
    E0(0xCE30, CF_0000, TEXT_RC),
    E0(0xCE31, CF_0000, TEXT_SC),
    E0(0xCE32, CF_0000, TEXT_RTC),
    E0(0xCE33, CF_0000, TEXT_STC),
    E0(0xCE34, CF_0000, TEXT_ROL),
    E0(0xCE35, CF_0000, TEXT_ROR),
    E0(0xCE36, CF_0000, TEXT_RFSM),
    E0(0xCE37, CF_0000, TEXT_SFSM),
    E0(0xCE38, CF_0000, TEXT_RHM),
    E0(0xCE39, CF_0000, TEXT_SHM),
    E1(0xCE82, CF_0000, TEXT_NORM, M_IND),
    E2(0xCF00, CF_00FF, TEXT_MPYU, M_MAM,  M_NARP),
};

constexpr uint8_t INDEX_TMS320C2X[] = {
      2,  // TEXT_ADDC
     11,  // TEXT_ADDK
      9,  // TEXT_ADRK
      4,  // TEXT_BC
      5,  // TEXT_BNC
      0,  // TEXT_MPYA
      1,  // TEXT_MPYS
     24,  // TEXT_MPYU
     23,  // TEXT_NORM
     13,  // TEXT_RC
     19,  // TEXT_RFSM
     21,  // TEXT_RHM
     17,  // TEXT_ROL
     18,  // TEXT_ROR
     15,  // TEXT_RTC
     10,  // TEXT_SBRK
     14,  // TEXT_SC
     20,  // TEXT_SFSM
     22,  // TEXT_SHM
      8,  // TEXT_SPH
      7,  // TEXT_SPL
     16,  // TEXT_STC
      3,  // TEXT_SUBB
     12,  // TEXT_SUBK
      6,  // TEXT_ZALR
};

// Assembler only
constexpr Entry TABLE_TMS320C2X_E[] PROGMEM = {
    E3(0x4800, CF_00FF, TEXT_ADD,  M_MAM,  M_LS16, M_NARP), // ADDH
    E3(0x4400, CF_00FF, TEXT_SUB,  M_MAM,  M_LS16, M_NARP), // SUBH
    E3(0x2000, CF_0FFF, TEXT_LACC, M_MAM,  M_LS4,  M_NARP), // LAC
    E2(0xD001, CF_0F00, TEXT_LACC, M_IM16, M_LS4),          // LALK
    E3(0x4000, CF_00FF, TEXT_LACC, M_MAM,  M_LS16, M_NARP), // ZALH
    E1(0xCA00, CF_00FF, TEXT_LACL, M_IMU8),                 // LACK/ZAC
    E2(0x4100, CF_00FF, TEXT_LACL, M_MAM,  M_NARP),         // ZALS
    E1(0xC800, CF_01FF, TEXT_LDP,  M_IMU9),                 // LDPK
    E2(0xC000, CF_07FF, TEXT_LAR,  M_AR,   M_IMU8),         // LARK
    E2(0xD000, CF_0700, TEXT_LAR,  M_AR,   M_IMU16),        // LRLK
    E1(0xCC00, CF_00FF, TEXT_ADD,  M_IMU8),                 // ADDK
    E1(0xCD00, CF_00FF, TEXT_SUB,  M_IMU8),                 // SUBK
    E2(0xD002, CF_0F00, TEXT_ADD,  M_IM16, M_LS4),          // ADLK
    E2(0xD003, CF_0F00, TEXT_SUB,  M_IM16, M_LS4),          // SBLK
    E1(0xA000, CF_1FFF, TEXT_MPY,  M_IM13),                 // MPYK
    E2(0xD004, CF_0F00, TEXT_AND,  M_IMU16, M_LS4),         // ANDK
    E2(0xD005, CF_0F00, TEXT_OR,   M_IMU16, M_LS4),         // ORK
    E2(0xD006, CF_0F00, TEXT_XOR,  M_IMU16, M_LS4),         // XORK
    E2(0x0080, CF_0000, TEXT_BCND, M_PM16,  M_CC),          // B/BZ/BNZ/BLZ/BLEZ/BGZ/BGEZ/BNV/BV
                                                            // BNC/BC/BIOZ/BBZ/BBNZ
    E1(0xCB00, CF_00FF, TEXT_RPT,  M_IMU8),                 // RPTK
    E1(0x5588, CF_0007, TEXT_MAR,  M_ARK),                  // LARP
    E1(0xCE00, CF_0000, TEXT_CLRC, M_CTL),                  // RC/CNFD/EINT/ROVM/RSXM/RTC/RXF
    E1(0xCE01, CF_0000, TEXT_SETC, M_CTL),                  // SC/CNFP/DINT/SOVM/SSXM/STC/SXF
    E3(0x5000, CF_01FF, TEXT_LST,  M_STN,  M_MAM,  M_NARP), // LST/LST1
    E3(0x7800, CF_01FF, TEXT_SST,  M_STN,  M_MAM,  M_NARP), // SST/SST1
    E3(0xFC00, CF_00FF, TEXT_BLPD, M_PM16, M_MAM,  M_NARP), // BLKP
    E3(0xFD00, CF_00FF, TEXT_BLDD, M_PM16, M_MAM,  M_NARP), // BLDP
};

constexpr uint8_t INDEX_TMS320C2X_E[] PROGMEM = {
      0,  // TEXT_ADD
     10,  // TEXT_ADD
     12,  // TEXT_ADD
     15,  // TEXT_AND
     18,  // TEXT_BCND
     26,  // TEXT_BLDD
     25,  // TEXT_BLPD
     21,  // TEXT_CLRC
      2,  // TEXT_LACC
      3,  // TEXT_LACC
      4,  // TEXT_LACC
      5,  // TEXT_LACL
      6,  // TEXT_LACL
      8,  // TEXT_LAR
      9,  // TEXT_LAR
      7,  // TEXT_LDP
     23,  // TEXT_LST
     20,  // TEXT_MAR
     14,  // TEXT_MPY
     16,  // TEXT_OR
     19,  // TEXT_RPT
     22,  // TEXT_SETC
     24,  // TEXT_SST
      1,  // TEXT_SUB
     11,  // TEXT_SUB
     13,  // TEXT_SUB
     17,  // TEXT_XOR
};

constexpr Entry TABLE_TMS320CNFx[] PROGMEM = {
    E0(0xCE04, CF_0000, TEXT_CNFD),
    E0(0xCE05, CF_0000, TEXT_CNFP),
};

constexpr uint8_t INDEX_TMS320CNFx[] = {
      0,  // TEXT_CNFD
      1,  // TEXT_CNFP
};

constexpr Entry TABLE_TMS320CONF[] PROGMEM = {
    E1(0xCE3C, CF_0003, TEXT_CONF, M_UI2),
};

constexpr uint8_t INDEX_TMS320CONF[] = {
      0,  // TEXT_CONF
};

constexpr Entry TABLE_TMS320C20X[] PROGMEM = {
    E3(0x0000, CF_07FF, TEXT_LAR,  M_AR,   M_MAM,  M_NARP),
    E2(0x0A00, CF_00FF, TEXT_SUBC, M_MAM,  M_NARP),
    E2(0x0B00, CF_00FF, TEXT_RPT,  M_MAM,  M_NARP),
    E3(0x0C00, CF_00FF, TEXT_OUT,  M_MAM,  M_PA16, M_NARP),
    E2(0x0D00, CF_00FF, TEXT_LDP,  M_MAM,  M_NARP),
    E3(0x0E00, CF_01FF, TEXT_LST,  M_STN,  M_MAM,  M_NARP),
    E3(0x6A00, CF_00FF, TEXT_LACC, M_MAM,  M_LS16, M_NARP),
    E3(0x6100, CF_00FF, TEXT_ADD,  M_MAM,  M_LS16, M_NARP),
    E3(0x6500, CF_00FF, TEXT_SUB,  M_MAM,  M_LS16, M_NARP),
    E3(0x1000, CF_0FFF, TEXT_LACC, M_MAM,  M_LS4,  M_NARP),
    E3(0x2000, CF_0FFF, TEXT_ADD,  M_MAM,  M_LS4,  M_NARP),
    E3(0x3000, CF_0FFF, TEXT_SUB,  M_MAM,  M_LS4,  M_NARP),
    E3(0x4000, CF_0FFF, TEXT_BIT,  M_MAM,  M_BIT,  M_NARP),
    E2(0x5000, CF_00FF, TEXT_MPYA, M_MAM,  M_NARP),
    E2(0x5100, CF_00FF, TEXT_MPYS, M_MAM,  M_NARP),
    E2(0x5200, CF_00FF, TEXT_SQRA, M_MAM,  M_NARP),
    E2(0x5300, CF_00FF, TEXT_SQRS, M_MAM,  M_NARP),
    E2(0x5400, CF_00FF, TEXT_MPY,  M_MAM,  M_NARP),
    E2(0x5500, CF_00FF, TEXT_MPYU, M_MAM,  M_NARP),
    E2(0x6000, CF_00FF, TEXT_ADDC, M_MAM,  M_NARP),
    E2(0x6200, CF_00FF, TEXT_ADDS, M_MAM,  M_NARP),
    E2(0x6300, CF_00FF, TEXT_ADDT, M_MAM,  M_NARP),
    E2(0x6400, CF_00FF, TEXT_SUBB, M_MAM,  M_NARP),
    E2(0x6600, CF_00FF, TEXT_SUBS, M_MAM,  M_NARP),
    E2(0x6700, CF_00FF, TEXT_SUBT, M_MAM,  M_NARP),
    E2(0x6800, CF_00FF, TEXT_ZALR, M_MAM,  M_NARP),
    E2(0x6900, CF_00FF, TEXT_LACL, M_MAM,  M_NARP),
    E2(0x6B00, CF_00FF, TEXT_LACT, M_MAM,  M_NARP),
    E2(0xBE81, CF_0000, TEXT_AND,  M_IMU16, M_LS16),
    E2(0xBE82, CF_0000, TEXT_OR,   M_IMU16, M_LS16),
    E2(0xBE83, CF_0000, TEXT_XOR,  M_IMU16, M_LS16),
    E2(0x6C00, CF_00FF, TEXT_XOR,  M_MAM,  M_NARP),
    E2(0x6D00, CF_00FF, TEXT_OR,   M_MAM,  M_NARP),
    E2(0x6E00, CF_00FF, TEXT_AND,  M_MAM,  M_NARP),
    E2(0x6F00, CF_00FF, TEXT_BITT, M_MAM,  M_NARP),
    E2(0x7000, CF_00FF, TEXT_LTA,  M_MAM,  M_NARP),
    E2(0x7100, CF_00FF, TEXT_LTP,  M_MAM,  M_NARP),
    E2(0x7200, CF_00FF, TEXT_LTD,  M_MAM,  M_NARP),
    E2(0x7300, CF_00FF, TEXT_LT,   M_MAM,  M_NARP),
    E2(0x7400, CF_00FF, TEXT_LTS,  M_MAM,  M_NARP),
    E2(0x7500, CF_00FF, TEXT_LPH,  M_MAM,  M_NARP),
    E2(0x7600, CF_00FF, TEXT_PSHD, M_MAM,  M_NARP),
    E2(0x7700, CF_00FF, TEXT_DMOV, M_MAM,  M_NARP),
    E1(0x7800, CF_00FF, TEXT_ADRK, M_IMU8),
    E3(0x7980, CF_007F, TEXT_B,    M_PM16, M_MAR,  M_NARP),
    E3(0x7A80, CF_007F, TEXT_CALL, M_PM16, M_MAR,  M_NARP),
    E3(0x7B80, CF_007F, TEXT_BANZ, M_PM16, M_MAR,  M_NARP),
    E1(0x7C00, CF_00FF, TEXT_SBRK, M_IMU8),
    E3(0x8000, CF_07FF, TEXT_SAR,  M_AR,   M_MAM,  M_NARP),
    E2(0x8A00, CF_00FF, TEXT_POPD, M_MAM,  M_NARP),
    E0(0x8B00, CF_0000, TEXT_NOP),
    E2(0x8B00, CF_00FF, TEXT_MAR,  M_MAM,  M_NARP),
    E2(0x8C00, CF_00FF, TEXT_SPL,  M_MAM,  M_NARP),
    E2(0x8D00, CF_00FF, TEXT_SPH,  M_MAM,  M_NARP),
    E3(0x8E00, CF_01FF, TEXT_SST,  M_STN,  M_MAM,  M_NARP),
    E3(0x9000, CF_07FF, TEXT_SACL, M_MAM,  M_LS3,  M_NARP),
    E3(0x9800, CF_07FF, TEXT_SACH, M_MAM,  M_LS3,  M_NARP),
    E1(0xA080, CF_0000, TEXT_NORM, M_IND),
    E3(0xA200, CF_00FF, TEXT_MAC,  M_PM16, M_MAM,  M_NARP),
    E3(0xA300, CF_00FF, TEXT_MACD, M_PM16, M_MAM,  M_NARP),
    E3(0xA500, CF_00FF, TEXT_BLPD, M_IMU16, M_MAM, M_NARP),
    E2(0xA600, CF_00FF, TEXT_TBLR, M_MAM,  M_NARP),
    E2(0xA700, CF_00FF, TEXT_TBLW, M_MAM,  M_NARP),
    E3(0xA800, CF_00FF, TEXT_BLDD, M_IMU16, M_MAM, M_NARP),
    E3(0xA900, CF_00FF, TEXT_BLDD, M_MAM, M_IMU16, M_NARP),
    E3(0xAE00, CF_00FF, TEXT_SPLK, M_IMU16, M_MAM, M_NARP),
    E3(0xAF00, CF_00FF, TEXT_IN,   M_MAM,  M_PA16, M_NARP),
    E2(0xB000, CF_07FF, TEXT_LAR,  M_AR,   M_IMU8),
    E1(0xB800, CF_00FF, TEXT_ADD,  M_IMU8),
    E1(0xB900, CF_00FF, TEXT_LACL, M_IMU8),
    E1(0xBA00, CF_00FF, TEXT_SUB,  M_IMU8),
    E1(0xBB00, CF_00FF, TEXT_RPT,  M_IMU8),
    E1(0xBC00, CF_01FF, TEXT_LDP,  M_IMU9),
    E0(0xBE00, CF_0000, TEXT_ABS),
    E0(0xBE01, CF_0000, TEXT_CMPL),
    E0(0xBE02, CF_0000, TEXT_NEG),
    E0(0xBE03, CF_0000, TEXT_PAC),
    E0(0xBE04, CF_0000, TEXT_APAC),
    E0(0xBE05, CF_0000, TEXT_SPAC),
    E0(0xBE09, CF_0000, TEXT_SFL),
    E0(0xBE0A, CF_0000, TEXT_SFR),
    E0(0xBE0C, CF_0000, TEXT_ROL),
    E0(0xBE0D, CF_0000, TEXT_ROR),
    E0(0xBE20, CF_0000, TEXT_BACC),
    E0(0xBE22, CF_0000, TEXT_IDLE),
    E0(0xBE30, CF_0000, TEXT_CALA),
    E0(0xBE32, CF_0000, TEXT_POP),
    E0(0xBE3C, CF_0000, TEXT_PUSH),
    E1(0xBE40, CF_000E, TEXT_CLRC, M_CTL),
    E1(0xBE41, CF_000E, TEXT_SETC, M_CTL),
    E0(0xBE51, CF_0000, TEXT_TRAP),
    E0(0xBE52, CF_0000, TEXT_NMI),
    E1(0xBE60, CF_001F, TEXT_INTR, M_UI5),
    E1(0xBF00, CF_0003, TEXT_SPM,  M_UI2),
    E2(0xBF08, CF_0007, TEXT_LAR,  M_ARK,  M_IMU16),
    E1(0xBF44, CF_0003, TEXT_CMPR, M_UI2),
    E2(0xBF80, CF_000F, TEXT_LACC, M_IM16, M_LS4L),
    E2(0xBF90, CF_000F, TEXT_ADD,  M_IM16, M_LS4L),
    E2(0xBFA0, CF_000F, TEXT_SUB,  M_IM16, M_LS4L),
    E2(0xBFB0, CF_000F, TEXT_AND,  M_IMU16, M_LS4L),
    E2(0xBFC0, CF_000F, TEXT_OR,   M_IMU16, M_LS4L),
    E2(0xBFD0, CF_000F, TEXT_XOR,  M_IMU16, M_LS4L),
    E1(0xC000, CF_1FFF, TEXT_MPY,  M_IM13),
    E0(0xEF00, CF_0000, TEXT_RET),
    E2(0xE000, CF_03FF, TEXT_BCND, M_PM16, M_CC),
    E2(0xE800, CF_03FF, TEXT_CC,   M_PM16, M_CC),
    E1(0xEC00, CF_03FF, TEXT_RETC, M_CC),
};

constexpr uint8_t INDEX_TMS320C20X[] PROGMEM = {
     73,  // TEXT_ABS
      7,  // TEXT_ADD
     10,  // TEXT_ADD
     68,  // TEXT_ADD
     97,  // TEXT_ADD
     19,  // TEXT_ADDC
     20,  // TEXT_ADDS
     21,  // TEXT_ADDT
     43,  // TEXT_ADRK
     28,  // TEXT_AND
     33,  // TEXT_AND
     99,  // TEXT_AND
     77,  // TEXT_APAC
     44,  // TEXT_B
     83,  // TEXT_BACC
     46,  // TEXT_BANZ
    104,  // TEXT_BCND
     12,  // TEXT_BIT
     34,  // TEXT_BITT
     63,  // TEXT_BLDD
     64,  // TEXT_BLDD
     60,  // TEXT_BLPD
     85,  // TEXT_CALA
     45,  // TEXT_CALL
    105,  // TEXT_CC
     88,  // TEXT_CLRC
     74,  // TEXT_CMPL
     95,  // TEXT_CMPR
     42,  // TEXT_DMOV
     84,  // TEXT_IDLE
     66,  // TEXT_IN
     92,  // TEXT_INTR
      6,  // TEXT_LACC
      9,  // TEXT_LACC
     96,  // TEXT_LACC
     26,  // TEXT_LACL
     69,  // TEXT_LACL
     27,  // TEXT_LACT
      0,  // TEXT_LAR
     67,  // TEXT_LAR
     94,  // TEXT_LAR
      4,  // TEXT_LDP
     72,  // TEXT_LDP
     40,  // TEXT_LPH
      5,  // TEXT_LST
     38,  // TEXT_LT
     35,  // TEXT_LTA
     37,  // TEXT_LTD
     36,  // TEXT_LTP
     39,  // TEXT_LTS
     58,  // TEXT_MAC
     59,  // TEXT_MACD
     51,  // TEXT_MAR
     17,  // TEXT_MPY
    102,  // TEXT_MPY
     13,  // TEXT_MPYA
     14,  // TEXT_MPYS
     18,  // TEXT_MPYU
     75,  // TEXT_NEG
     91,  // TEXT_NMI
     50,  // TEXT_NOP
     57,  // TEXT_NORM
     29,  // TEXT_OR
     32,  // TEXT_OR
    100,  // TEXT_OR
      3,  // TEXT_OUT
     76,  // TEXT_PAC
     86,  // TEXT_POP
     49,  // TEXT_POPD
     41,  // TEXT_PSHD
     87,  // TEXT_PUSH
    103,  // TEXT_RET
    106,  // TEXT_RETC
     81,  // TEXT_ROL
     82,  // TEXT_ROR
      2,  // TEXT_RPT
     71,  // TEXT_RPT
     56,  // TEXT_SACH
     55,  // TEXT_SACL
     48,  // TEXT_SAR
     47,  // TEXT_SBRK
     89,  // TEXT_SETC
     79,  // TEXT_SFL
     80,  // TEXT_SFR
     78,  // TEXT_SPAC
     53,  // TEXT_SPH
     52,  // TEXT_SPL
     65,  // TEXT_SPLK
     93,  // TEXT_SPM
     15,  // TEXT_SQRA
     16,  // TEXT_SQRS
     54,  // TEXT_SST
      8,  // TEXT_SUB
     11,  // TEXT_SUB
     70,  // TEXT_SUB
     98,  // TEXT_SUB
     22,  // TEXT_SUBB
      1,  // TEXT_SUBC
     23,  // TEXT_SUBS
     24,  // TEXT_SUBT
     61,  // TEXT_TBLR
     62,  // TEXT_TBLW
     90,  // TEXT_TRAP
     30,  // TEXT_XOR
     31,  // TEXT_XOR
    101,  // TEXT_XOR
     25,  // TEXT_ZALR
};

constexpr Entry TABLE_TMS320C5X[] PROGMEM = {
    E2(0x0800, CF_00FF, TEXT_LAMM,  M_MAM,  M_NARP),
    E3(0x0900, CF_00FF, TEXT_SMMR,  M_MAM,  M_IMU16, M_NARP),
    E2(0x5700, CF_00FF, TEXT_BLDP,  M_MAM,  M_NARP),
    E2(0x5800, CF_00FF, TEXT_XPL,   M_MAM,  M_NARP),
    E2(0x5900, CF_00FF, TEXT_OPL,   M_MAM,  M_NARP),
    E2(0x5A00, CF_00FF, TEXT_APL,   M_MAM,  M_NARP),
    E2(0x5B00, CF_00FF, TEXT_CPL,   M_MAM,  M_NARP),
    E3(0x5C00, CF_00FF, TEXT_XPL,   M_IMU16, M_MAM,  M_NARP),
    E3(0x5D00, CF_00FF, TEXT_OPL,   M_IMU16, M_MAM,  M_NARP),
    E3(0x5E00, CF_00FF, TEXT_APL,   M_IMU16, M_MAM,  M_NARP),
    E3(0x5F00, CF_00FF, TEXT_CPL,   M_IMU16, M_MAM,  M_NARP),
    E3(0x7D80, CF_007F, TEXT_BD,    M_PM16, M_MAR,   M_NARP),
    E3(0x7E80, CF_007F, TEXT_CALLD, M_PM16, M_MAR,   M_NARP),
    E3(0x7F80, CF_007F, TEXT_BANZD, M_PM16, M_MAR,   M_NARP),
    E2(0x8800, CF_00FF, TEXT_SAMM,  M_MAM,  M_NARP),
    E3(0x8900, CF_00FF, TEXT_LMMR,  M_MAM,  M_IMU16, M_NARP),
    E3(0xA400, CF_00FF, TEXT_BLPD,  M_BMAR, M_MAM,  M_NARP),
    E2(0xAA00, CF_00FF, TEXT_MADS,  M_MAM,  M_NARP),
    E2(0xAB00, CF_00FF, TEXT_MADD,  M_MAM,  M_NARP),
    E3(0xAC00, CF_00FF, TEXT_BLDD,  M_BMAR, M_MAM,  M_NARP),
    E3(0xAD00, CF_00FF, TEXT_BLDD,  M_MAM,  M_BMAR, M_NARP),
    E0(0xBE10, CF_0000, TEXT_ADDB),
    E0(0xBE11, CF_0000, TEXT_ADCB),
    E0(0xBE12, CF_0000, TEXT_ANDB),
    E0(0xBE13, CF_0000, TEXT_ORB),
    E0(0xBE14, CF_0000, TEXT_ROLB),
    E0(0xBE15, CF_0000, TEXT_RORB),
    E0(0xBE16, CF_0000, TEXT_SFLB),
    E0(0xBE17, CF_0000, TEXT_SFRB),
    E0(0xBE18, CF_0000, TEXT_SBB),
    E0(0xBE19, CF_0000, TEXT_SBBB),
    E0(0xBE1A, CF_0000, TEXT_XORB),
    E0(0xBE1B, CF_0000, TEXT_CRGT),
    E0(0xBE1C, CF_0000, TEXT_CRLT),
    E0(0xBE1D, CF_0000, TEXT_EXAR),
    E0(0xBE1E, CF_0000, TEXT_SACB),
    E0(0xBE1F, CF_0000, TEXT_LACB),
    E0(0xBE21, CF_0000, TEXT_BACCD),
    E0(0xBE23, CF_0000, TEXT_IDLE2),
    E0(0xBE38, CF_0000, TEXT_RETI),
    E0(0xBE3A, CF_0000, TEXT_RETE),
    E0(0xBE3D, CF_0000, TEXT_CALAD),
    E0(0xBE58, CF_0000, TEXT_ZPR),
    E0(0xBE59, CF_0000, TEXT_ZAP),
    E0(0xBE5A, CF_0000, TEXT_SATH),
    E0(0xBE5B, CF_0000, TEXT_SATL),
    E1(0xBEC5, CF_0000, TEXT_RPTZ,  M_IMU16),
    E1(0xBEC6, CF_0000, TEXT_RPTB,  M_PM16),
    E1(0xBFE0, CF_000F, TEXT_BSAR,  M_LS4Z),
    E2(0xE400, CF_13FF, TEXT_XC,    M_XCN,   M_CC),
    E2(0xF000, CF_03FF, TEXT_BCNDD, M_PM16,  M_CC),
    E2(0xF800, CF_03FF, TEXT_CCD,   M_PM16,  M_CC),
    E0(0xFF00, CF_0000, TEXT_RETD),
    E1(0xFC00, CF_03FF, TEXT_RETCD, M_CC),
};

constexpr uint8_t INDEX_TMS320C5X[] PROGMEM = {
     22,  // TEXT_ADCB
     21,  // TEXT_ADDB
     23,  // TEXT_ANDB
      5,  // TEXT_APL
      9,  // TEXT_APL
     37,  // TEXT_BACCD
     13,  // TEXT_BANZD
     50,  // TEXT_BCNDD
     11,  // TEXT_BD
     19,  // TEXT_BLDD
     20,  // TEXT_BLDD
      2,  // TEXT_BLDP
     16,  // TEXT_BLPD
     48,  // TEXT_BSAR
     41,  // TEXT_CALAD
     12,  // TEXT_CALLD
     51,  // TEXT_CCD
      6,  // TEXT_CPL
     10,  // TEXT_CPL
     32,  // TEXT_CRGT
     33,  // TEXT_CRLT
     34,  // TEXT_EXAR
     38,  // TEXT_IDLE2
     36,  // TEXT_LACB
      0,  // TEXT_LAMM
     15,  // TEXT_LMMR
     18,  // TEXT_MADD
     17,  // TEXT_MADS
      4,  // TEXT_OPL
      8,  // TEXT_OPL
     24,  // TEXT_ORB
     53,  // TEXT_RETCD
     52,  // TEXT_RETD
     40,  // TEXT_RETE
     39,  // TEXT_RETI
     25,  // TEXT_ROLB
     26,  // TEXT_RORB
     47,  // TEXT_RPTB
     46,  // TEXT_RPTZ
     35,  // TEXT_SACB
     14,  // TEXT_SAMM
     44,  // TEXT_SATH
     45,  // TEXT_SATL
     29,  // TEXT_SBB
     30,  // TEXT_SBBB
     27,  // TEXT_SFLB
     28,  // TEXT_SFRB
      1,  // TEXT_SMMR
     49,  // TEXT_XC
     31,  // TEXT_XORB
      3,  // TEXT_XPL
      7,  // TEXT_XPL
     43,  // TEXT_ZAP
     42,  // TEXT_ZPR
};

// clang-format on

using EntryPage = entry::TableBase<Entry>;

constexpr EntryPage TMS32010_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS32010), ARRAY_RANGE(INDEX_TMS32010)},
};

using Cpu = entry::CpuBase<CpuType, EntryPage>;

constexpr EntryPage TMS32020_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS3202X), ARRAY_RANGE(INDEX_TMS3202X)},
        {ARRAY_RANGE(TABLE_TMS32020), ARRAY_RANGE(INDEX_TMS32020)},
        {ARRAY_RANGE(TABLE_TMS320CNFx), ARRAY_RANGE(INDEX_TMS320CNFx)},
};

constexpr EntryPage TMS320C25_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C2X), ARRAY_RANGE(INDEX_TMS320C2X)},
        {ARRAY_RANGE(TABLE_TMS3202X), ARRAY_RANGE(INDEX_TMS3202X)},
        {ARRAY_RANGE(TABLE_TMS320C2X_E), ARRAY_RANGE(INDEX_TMS320C2X_E)},
        {ARRAY_RANGE(TABLE_TMS320CNFx), ARRAY_RANGE(INDEX_TMS320CNFx)},
};

constexpr EntryPage TMS320C26_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C2X), ARRAY_RANGE(INDEX_TMS320C2X)},
        {ARRAY_RANGE(TABLE_TMS3202X), ARRAY_RANGE(INDEX_TMS3202X)},
        {ARRAY_RANGE(TABLE_TMS320C2X_E), ARRAY_RANGE(INDEX_TMS320C2X_E)},
        {ARRAY_RANGE(TABLE_TMS320CONF), ARRAY_RANGE(INDEX_TMS320CONF)},
};

constexpr EntryPage TMS320C20X_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C20X), ARRAY_RANGE(INDEX_TMS320C20X)},
};

constexpr EntryPage TMS320C5X_PAGES[] PROGMEM = {
        {ARRAY_RANGE(TABLE_TMS320C20X), ARRAY_RANGE(INDEX_TMS320C20X)},
        {ARRAY_RANGE(TABLE_TMS320C5X), ARRAY_RANGE(INDEX_TMS320C5X)},
};

constexpr Cpu CPU_TABLE[] PROGMEM = {
        {TMS32010, TEXT_CPU_32010, ARRAY_RANGE(TMS32010_PAGES)},
        {TMS32015, TEXT_CPU_32015, ARRAY_RANGE(TMS32010_PAGES)},
        {TMS32020, TEXT_CPU_32020, ARRAY_RANGE(TMS32020_PAGES)},
        {TMS320C25, TEXT_CPU_320C25, ARRAY_RANGE(TMS320C25_PAGES)},
        {TMS320C26, TEXT_CPU_320C26, ARRAY_RANGE(TMS320C26_PAGES)},
        {TMS320C20X, TEXT_CPU_320C20X, ARRAY_RANGE(TMS320C20X_PAGES)},
        {TMS320C5X, TEXT_CPU_320C5X, ARRAY_RANGE(TMS320C5X_PAGES)},
};

const Cpu *cpu(CpuType cpuType) {
    return Cpu::search(cpuType, ARRAY_RANGE(CPU_TABLE));
}

bool acceptMode(AddrMode opr, AddrMode table, bool is320c) {
    if (opr == table)
        return true;
    if (opr == M_NONE)  // These can be ommitted.
        return table == M_LS0 || table == M_LS3 || table == M_LS4 || table == M_LS4L ||
               table == M_MAR || table == M_NARP;
    if (opr == M_AR)
        return table == M_ARK || table == M_NARP;
    if (opr == M_ARP || opr == M_INC || opr == M_DEC || opr == M_INC0 || opr == M_DEC0 ||
            opr == M_IBR0 || opr == M_DBR0)
        return table == M_MAM || table == M_IND || table == M_MAR;
    if (opr == M_UI16) {
        if (is320c && table == M_LS16)
            return false;
        return table == M_MAM || table == M_NARP || (table >= M_UI1 && table <= M_PM16);
    }
    if (opr == M_LS16) {
        if (is320c) {
            if (table == M_LS4)
                return false;
            return table == M_MAM || table == M_NARP || (table >= M_UI1 && table <= M_PM16);
        } else {
            return table == M_MAM || table == M_NARP || (table >= M_UI1 && table <= M_PM16);
        }
    }
    if (opr == M_SI16)
        return table >= M_UI1 && table <= M_BIT;
    if (opr == M_IMU8)
        return table >= M_STN && table <= M_IMU16;
    if (opr == M_IMU16)
        return table >= M_IMU9 && table <= M_IMU16;
    if (opr == M_IM16)
        return table == M_IM13;
    if (opr == M_CCCTL)
        return table == M_CC || table == M_CTL;
    return false;
}

bool acceptMode(AddrMode opr, AddrMode table) {
    return acceptMode(opr, table, false);
}

bool acceptModes(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode(insn.op1.mode, table.mode1()) && acceptMode(insn.op2.mode, table.mode2()) &&
           acceptMode(insn.op3.mode, table.mode3());
}

bool acceptMode320c(AddrMode opr, AddrMode table) {
    return acceptMode(opr, table, true);
}

bool acceptModes320c(AsmInsn &insn, const Entry *entry) {
    const auto table = entry->readFlags();
    return acceptMode320c(insn.op1.mode, table.mode1()) &&
           acceptMode320c(insn.op2.mode, table.mode2()) &&
           acceptMode320c(insn.op3.mode, table.mode3());
}

Error searchName(CpuType cpuType, AsmInsn &insn) {
    const auto c = cpu(cpuType);
    if (cpuType >= TMS320C25) {
        c->searchName(insn, acceptModes320c);
    } else {
        c->searchName(insn, acceptModes);
    }
    return insn.getError();
}

bool matchOpCode(DisInsn &insn, const Entry *entry, CpuType cpuType) {
    auto opc = insn.opCode();
    const auto flags = entry->readFlags();
    const auto mode1 = flags.mode1();
    const auto mode2 = flags.mode2();
    auto mask = flags.mask();
    if (opc & 0x80) {
        if (mode2 == M_NARP || flags.mode3() == M_NARP) {
            const auto narp = (opc & 8);
            const auto ar = (opc & 7);
            if (cpuType == TMS32010) {
                if (opc & 0x46)
                    return false;  // check reserved bits
                if (narp && ar)
                    return false;  // check no next ARP
            } else {
                if (narp == 0 && ar)
                    return false;  // check no next ARP
            }
            mask |= 0xF;
        }
        if (mode1 == M_MAM || mode2 == M_MAM || mode1 == M_MAR || mode2 == M_MAR ||
                mode1 == M_IND) {
            const auto modify = (opc >> 4) & 7;
            if (modify == 3)
                return false;  // Don't allow both increment and decrement.
            mask |= 0x70;
        }
    }
    if (mode2 == M_LS3 && cpuType < TMS320C25) {
        const auto shift = (opc >> 8) & 7;
        if (shift == 0 || shift == 1 || shift == 4) {
            ;  // OK
        } else {
            return false;
        }
    }

    opc &= ~mask;
    return opc == entry->readOpCode();
}

bool matchOpCode1x(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(insn, entry, TMS32010);
}

bool matchOpCode20(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(insn, entry, TMS32020);
}

bool matchOpCode2x(DisInsn &insn, const Entry *entry, const EntryPage *) {
    return matchOpCode(insn, entry, TMS320C25);
}

Error searchOpCode(CpuType cpuType, DisInsn &insn, StrBuffer &out) {
    const auto c = cpu(cpuType);
    if (cpuType == TMS32010 || cpuType == TMS32015) {
        c->searchOpCode(insn, out, matchOpCode1x);
    } else if (cpuType == TMS32020) {
        c->searchOpCode(insn, out, matchOpCode20);
    } else {
        c->searchOpCode(insn, out, matchOpCode2x);
    }
    return insn.getError();
}

const /*PROGMEM*/ char *TableTms320::listCpu_P() const {
    return TEXT_TMS320_LIST;
}

const /*PROGMEM*/ char *TableTms320::cpuName_P(CpuType cpuType) const {
    return cpu(cpuType)->name_P();
}

Error TableTms320::searchCpuName(StrScanner &name, CpuType &cpuType) const {
    name.iexpectText_P(TEXT_TMS320_LIST, 3);  // Ignore "TMS" if exist
    const auto t = Cpu::search(name, ARRAY_RANGE(CPU_TABLE));
    if (t) {
        cpuType = t->readCpuType();
    } else {
        return UNSUPPORTED_CPU;
    }
    return OK;
}

const TableTms320 TABLE;

}  // namespace tms320
}  // namespace libasm

// Local Variables:
// mode: c++
// c-basic-offset: 4
// tab-width: 4
// End:
// vim: set ft=cpp et ts=4 sw=4:
